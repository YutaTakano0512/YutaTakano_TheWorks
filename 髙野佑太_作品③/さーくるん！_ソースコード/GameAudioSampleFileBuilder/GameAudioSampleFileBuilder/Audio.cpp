
//#include "main.h"
#include "audio.h"
#include <future>

#define _USE_MATH_DEFINES
#include <math.h>

#include <cmath>

#include <windows.h>
#include <mmiscapi.h>

#pragma comment(lib, "winmm.lib")
#pragma comment(lib, "xaudio2.lib")

using Complex = std::complex<double>;


static IXAudio2* g_Xaudio{};
static IXAudio2MasteringVoice* g_MasteringVoice{};


void InitAudio()
{
	// XAudio生成
	//XAudio2Create(&g_Xaudio, 0);
    HRESULT hr = XAudio2Create(&g_Xaudio, 0, XAUDIO2_DEFAULT_PROCESSOR);
    if (FAILED(hr)) {
        printf("XAudio2Create failed: 0x%X\n", hr);
    }
	// マスタリングボイス生成
    hr = g_Xaudio->CreateMasteringVoice(&g_MasteringVoice);
    if (FAILED(hr)) {
        printf("CreateMasteringVoice failed: 0x%X\n", hr);
    }


}


void UninitAudio()
{
	g_MasteringVoice->DestroyVoice();
	g_Xaudio->Release();
}







struct AUDIO
{
    bool use;
    IXAudio2SourceVoice* SourceVoice{};
    BYTE* SoundData{};

    int						Length{};
    int						PlayLength{};

    DWORD sampleRate;

    std::future<void> loadtask;

    // バッファ保存
    //XAUDIO2_BUFFER bufinfo;
    std::vector<float> spectrum; // FFT結果を保存するためのベクトル
};

#define AUDIO_MAX 100
static AUDIO g_Audio[AUDIO_MAX]{};


void FFT_Audio(std::vector<Complex>& a);

void loadAudioTask(AUDIO* audiop, const char* FileName, bool isBGM)
{
    {



        // サウンドデータ読込
        WAVEFORMATEX wfx = { 0 };

        {
            HMMIO hmmio = NULL;
            MMIOINFO mmioinfo = { 0 };
            MMCKINFO riffchunkinfo = { 0 };
            MMCKINFO datachunkinfo = { 0 };
            MMCKINFO mmckinfo = { 0 };
            UINT32 buflen;
            LONG readlen;


            hmmio = mmioOpen((LPSTR)FileName, &mmioinfo, MMIO_READ);
            assert(hmmio);

            riffchunkinfo.fccType = mmioFOURCC('W', 'A', 'V', 'E');
            mmioDescend(hmmio, &riffchunkinfo, NULL, MMIO_FINDRIFF);

            mmckinfo.ckid = mmioFOURCC('f', 'm', 't', ' ');
            mmioDescend(hmmio, &mmckinfo, &riffchunkinfo, MMIO_FINDCHUNK);

            if (mmckinfo.cksize >= sizeof(WAVEFORMATEX))
            {
                mmioRead(hmmio, (HPSTR)&wfx, sizeof(wfx));
            }
            else
            {
                PCMWAVEFORMAT pcmwf = { 0 };
                mmioRead(hmmio, (HPSTR)&pcmwf, sizeof(pcmwf));
                memset(&wfx, 0x00, sizeof(wfx));
                memcpy(&wfx, &pcmwf, sizeof(pcmwf));
                wfx.cbSize = 0;
            }
            mmioAscend(hmmio, &mmckinfo, 0);

            datachunkinfo.ckid = mmioFOURCC('d', 'a', 't', 'a');
            mmioDescend(hmmio, &datachunkinfo, &riffchunkinfo, MMIO_FINDCHUNK);



            buflen = datachunkinfo.cksize;
            audiop->SoundData = new unsigned char[buflen];
            readlen = mmioRead(hmmio, (HPSTR)audiop->SoundData, buflen);


            audiop->Length = readlen;
            audiop->PlayLength = readlen / wfx.nBlockAlign;
            audiop->sampleRate = wfx.nSamplesPerSec;


            mmioClose(hmmio, 0);
        }


        // サウンドソース生成
        HRESULT hr = g_Xaudio->CreateSourceVoice(&audiop->SourceVoice, &wfx); 
        if (FAILED(hr)) {
            printf("CreateSourceVoice failed: 0x%X\n", hr);
        }
        //AudioCallback myCallback;
        //g_Xaudio->CreateSourceVoice(&audiop->SourceVoice, &wfx, 0, XAUDIO2_DEFAULT_FREQ_RATIO, &myCallback);
        assert(audiop->SourceVoice);

        //重すぎ
        int16_t* audioSamples = reinterpret_cast<int16_t*>(audiop->SoundData);
        size_t numSamples = (audiop->Length / sizeof(int16_t)) * sizeof(int16_t) == audiop->Length ?
            audiop->Length / sizeof(int16_t) : audiop->Length / sizeof(int16_t) - 1;
        if (audioSamples != nullptr)
        {
            //std::vector<Complex> fftInput(numSamples);
            //for (size_t i = 0; i < numSamples && i < audiop->Length / sizeof(int16_t); i++) {
            //    fftInput[i] = Complex(static_cast<double>(audioSamples[i]) / 32768.0, 0.0); // 正規化
            //}

            //std::vector<Complex> fftInput(numSamples);

            int downsampleFactor = 735; // 1フレームあたりのサンプル数を設定
            size_t reducedSamples = numSamples / downsampleFactor;

            std::vector<Complex> fftInput(reducedSamples);
            for (size_t i = 0; i < reducedSamples && i * downsampleFactor < audiop->Length / sizeof(int16_t); i++) {
                fftInput[i] = Complex(static_cast<double>(audioSamples[i * downsampleFactor]) / 32768.0, 0.0); // 正規化
            }

            FFT_Audio(fftInput);

            // FFT結果の振幅データを格納
            std::vector<float> spectrum(reducedSamples / 2); // サイズ修正
            for (size_t i = 0; i < reducedSamples / 2; i++) { // ループ範囲を修正
                spectrum[i] = std::abs(fftInput[i]); // 振幅データを取得
            }

            audiop->spectrum = spectrum; // FFT結果を保存
        }
    }

}

int LoadAudio(const char *FileName, bool isBGM)
{
	int index = -1;

    {
        for (int i = 0; i < AUDIO_MAX; i++)
        {
            if (!g_Audio[i].use)
            {
                g_Audio[i].use = true;

                loadAudioTask(g_Audio + i, FileName, isBGM);

                index = i;
                break;
            }
        }
    }

	if (index == -1)
		return -1;

	return index;
}

void UnloadAudio(int Index)
{
    g_Audio[Index].SourceVoice->Stop();
    g_Audio[Index].SourceVoice->DestroyVoice();

    //if (g_Audio[Index].bufinfo.pAudioData)
    //{
    //    delete[] g_Audio[Index].bufinfo.pAudioData;
    //    g_Audio[Index].bufinfo.pAudioData = nullptr;
    //}

    delete[] g_Audio[Index].SoundData;
    g_Audio[Index].SoundData = nullptr;
}

std::vector<float> GetAudioDat(int index)
{
    return g_Audio[index].spectrum;
}

void FFT_Audio(std::vector<Complex>& a)
{
    int N = a.size();
    if (N <= 1) return;

    std::vector<Complex> even(N / 2), odd(N / 2);
    for (int i = 0; i < N / 2; i++) {
        even[i] = a[i * 2];
        odd[i] = a[i * 2 + 1];
    }

    // 再帰的に FFT を適用
    FFT_Audio(even);
    FFT_Audio(odd);

    // 計算結果を a に書き戻す
    for (int k = 0; k < N / 2; k++) {
        Complex twiddle = std::polar(1.0, -2.0 * M_PI * k / N) * odd[k];
        a[k] = even[k] + twiddle;
        a[k + N / 2] = even[k] - twiddle;
    }
}
//void FFT_Audio(std::vector<Complex>& a) {
//    int N = a.size();
//    int logN = 0;
//    for (int temp = N; temp > 1; temp >>= 1) logN++;
//
//    std::vector<int> bitRev(N);
//    for (int i = 0; i < N; i++) {
//        int rev = 0;
//        for (int j = 0; j < logN; j++) {
//            rev |= ((i >> j) & 1) << (logN - 1 - j);
//        }
//        bitRev[i] = rev;
//    }
//
//    // バタフライ演算
//    for (int len = 2; len <= N; len *= 2) {
//        Complex wn = std::polar(1.0, -2.0 * M_PI / len);
//        for (int i = 0; i < N; i += len) {
//            Complex w = 1;
//            for (int j = 0; j < len / 2; j++) {
//                Complex u = a[i + j];
//                Complex v = a[i + j + len / 2] * w;
//                a[i + j] = u + v;
//                a[i + j + len / 2] = u - v;
//                w *= wn;
//            }
//        }
//    }
//}