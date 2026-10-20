
//#include "main.h"
#include "../../Game/Systems/sys_general.h"

#include <iostream>
#include <fstream>

#include "audio.h"
#include <future>

#define _USE_MATH_DEFINES
#include <math.h>

#include <cmath>

constexpr static int downsampleFactor = 735; // 1フレームあたりのサンプル数を設定
using Complex = std::complex<double>;


static IXAudio2* g_Xaudio{};
static IXAudio2MasteringVoice* g_MasteringVoice{};


void InitAudio()
{
	// XAudio生成
	XAudio2Create(&g_Xaudio, 0);

	// マスタリングボイス生成
	g_Xaudio->CreateMasteringVoice(&g_MasteringVoice);
}


void UninitAudio()
{
	g_MasteringVoice->DestroyVoice();
	g_Xaudio->Release();
}







struct AUDIO
{
    bool use;
    std::string FileName;

    IXAudio2SourceVoice* SourceVoice{};
    BYTE* SoundData{};

    UINT32						Length{};
    UINT32						PlayLength{};

    DWORD sampleRate;
    WORD           bitPerSample;
    WORD           channels;


    std::future<void> loadtask;

    // バッファ保存
    //XAUDIO2_BUFFER bufinfo;
    std::vector<float> spectrum; // FFT結果を保存するためのベクトル
};

#define AUDIO_MAX 100
static AUDIO g_Audio[AUDIO_MAX]{};

// IXAudio2SourceVoice から現在の再生時間を取得
float getCurrentPlaybackTime(AUDIO* audiop)
{
    XAUDIO2_VOICE_STATE state;
    audiop->SourceVoice->GetState(&state);

    // 再生カーソルの位置をサンプルレートで割ることで秒単位の時間を取得
    return static_cast<float>(state.SamplesPlayed) / audiop->sampleRate;
}

// 再生時間に基づくFFTデータ取得
const std::vector<float>* GetAudioFFTData(int index)
{
    if (index < 0 || index >= AUDIO_MAX)
    {
        return 0;
    }

    AUDIO* audiop = g_Audio + index;

    return &audiop->spectrum;
}
// 再生時間に基づくFFTデータ取得
int GetAudioFFTIndex(int index)
{
    if (index < 0 || index >= AUDIO_MAX)
    {
        return -1;
    }

    AUDIO* audiop = g_Audio + index;
    float playbackTime = getCurrentPlaybackTime(audiop);

    // ダウンサンプリングを考慮したフレームインデックス計算
    int frameIndex = static_cast<int>(playbackTime * audiop->sampleRate / downsampleFactor);

    //if (frameIndex >= 0 && frameIndex < audiop->spectrum.size())
    //{
    //    return frameIndex;
    //}

    //return -1; // データなしの場合の処理
    return frameIndex % audiop->spectrum.size(); // データなしの場合の処理
}

bool IsAudioPlaying(int index)
{
    if (index < 0 || index >= AUDIO_MAX)
    {
        return false;
    }

    AUDIO* audiop = g_Audio + index;
    XAUDIO2_VOICE_STATE state;
    audiop->SourceVoice->GetState(&state);

    return state.BuffersQueued > 0; // バッファがキューに残っていれば再生中
}

void LoadAudioFFTData(int index)
{
    if (index < 0 || index >= AUDIO_MAX)
    {
        return;
    }

    AUDIO* audiop = g_Audio + index;

    if(audiop)
    {
        // ファイル名の指定
        std::string filename = GetRemoveExtension(audiop->FileName) + ".sp";

        // テキストファイルを開く
        std::ifstream file(filename);
        if (!file.is_open()) {
            return;
        }

        std::string value;
        while (file >> value)
        {
            audiop->spectrum.push_back(std::stof(value));
        }

        // ファイルを閉じる
        file.close();
    }
}


//class AudioCallback : public IXAudio2VoiceCallback
//{
//public:
//    STDMETHOD_(void, OnBufferEnd) (void* pBufferContext)
//    {
//        if (pBufferContext)
//        {
//            AUDIO* audio = static_cast<AUDIO*>(pBufferContext);
//            if (audio->bufinfo.pAudioData)
//            {
//                delete[] audio->bufinfo.pAudioData; // 解放（再生完了後）
//                audio->bufinfo.pAudioData = nullptr;
//            }
//        }
//    }
//
//    STDMETHOD_(void, OnVoiceProcessingPassStart)(UINT32) {}
//    STDMETHOD_(void, OnVoiceProcessingPassEnd)() {}
//    STDMETHOD_(void, OnStreamEnd)() {}
//    STDMETHOD_(void, OnBufferStart)(void*) {}
//    STDMETHOD_(void, OnLoopEnd)(void*) {}
//    STDMETHOD_(void, OnVoiceError)(void*, HRESULT) {}
//};

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
        g_Xaudio->CreateSourceVoice(&audiop->SourceVoice, &wfx);
        //AudioCallback myCallback;
        //g_Xaudio->CreateSourceVoice(&audiop->SourceVoice, &wfx, 0, XAUDIO2_DEFAULT_FREQ_RATIO, &myCallback);
        assert(audiop->SourceVoice);

        audiop->FileName = FileName;

        audiop->bitPerSample = wfx.wBitsPerSample;  // 8 or 16 or 24 etc.
        audiop->channels = wfx.nChannels;      // モノラルなら1, ステレオなら2

        if(isBGM)
        {
            int16_t* audioSamples = reinterpret_cast<int16_t*>(audiop->SoundData);
            size_t numSamples = (audiop->Length / sizeof(int16_t)) * sizeof(int16_t) == audiop->Length ?
                audiop->Length / sizeof(int16_t) : audiop->Length / sizeof(int16_t) - 1;
            

            //重すぎ
            //if (audioSamples != nullptr)
            //{
            //    std::vector<Complex> fftInput(numSamples);
            //    for (size_t i = 0; i < numSamples && i < audiop->Length / sizeof(int16_t); i++) {
            //        fftInput[i] = Complex(static_cast<double>(audioSamples[i]) / 32768.0, 0.0); // 正規化
            //    }

            //    FFT_Audio(fftInput);

            //    std::vector<float> spectrum(numSamples / 2);
            //    for (size_t i = 0; i < numSamples / 2; i++) {
            //        spectrum[i] = std::abs(fftInput[i]); // 振幅データを取得
            //    }
            //    audiop->spectrum = spectrum; // FFT結果を保存
            //}

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

                //g_Audio[i].loadtask = std::async(
                //    std::launch::async,
                //    loadAudioTask,
                //    g_Audio + i,
                //    FileName
                //);
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

void WaitLoadAudio()
{
    // タスクの完了を待つ
    //bool isFinish = true;
    //do {
    //    for (int i = 0; i < AUDIO_MAX; i++)
    //    {
    //        if (g_Audio[i].loadtask.valid())
    //        {
    //            g_Audio[i].loadtask.get();
    //            isFinish = false;
    //            break;
    //        }
    //    }
    //} while (isFinish);
    for (int i = 0; i < AUDIO_MAX; i++)
    {
        if (g_Audio[i].loadtask.valid())
        {
            g_Audio[i].loadtask.get();
        }
    }
}





void PlayAudio(
    int   Index,
    float volume,
    bool  Loop,
    float Pitch,
    float playBeginSecond,
    float playEndSecond  // ここが <0 のとき、初回は playBegin から、あとはフルループ
)
{
    auto& A = g_Audio[Index];
    const auto sr = A.sampleRate;
    const auto bps = (A.bitPerSample / 8) * A.channels;  // バイト／フレーム
    const auto totalFrames = A.PlayLength;               // 総フレーム数

    // ---- １）共通して求めておく値 ----
    UINT32 startFrame = UINT32(playBeginSecond * sr);
    startFrame = (std::min)(startFrame, totalFrames);
    UINT32 framesToEnd = totalFrames - startFrame;

    // ---- 再生とループを分けるパス ----
    A.SourceVoice->Stop();
    A.SourceVoice->FlushSourceBuffers();

    if (Loop && playEndSecond < 0.0f)
    {
        // ■■ A) 初回：「startFrame→末尾まで」を一度だけ ■■
        XAUDIO2_BUFFER first = {};
        first.pAudioData = A.SoundData + size_t(startFrame) * bps;
        first.AudioBytes = framesToEnd * bps;
        first.Flags = XAUDIO2_END_OF_STREAM;  // １回だけなので終端はここで
        A.SourceVoice->SubmitSourceBuffer(&first);

        // ■■ B) その後：「全体」を無限ループ再生 ■■
        XAUDIO2_BUFFER loop = {};
        loop.pAudioData = A.SoundData;
        loop.AudioBytes = totalFrames * bps;
        loop.LoopBegin = 0;
        loop.LoopLength = totalFrames;
        loop.LoopCount = XAUDIO2_LOOP_INFINITE;
        // ※ループバッファは最後に END_OF_STREAM を立てずとも、その場で回り続けます
        A.SourceVoice->SubmitSourceBuffer(&loop);
    }
    else if (Loop && playBeginSecond > 0.0f && playEndSecond > 0.0f)
    {
        auto& A = g_Audio[Index];
        const auto sr = A.sampleRate;
        const auto bps = (A.bitPerSample / 8) * A.channels;  // バイト／フレーム
        const auto totalFrames = A.PlayLength;              // 総フレーム数

        // 1) フレームに変換
        UINT32 startF = static_cast<UINT32>(playBeginSecond * sr);
        UINT32 endF = static_cast<UINT32>(playEndSecond * sr);
        // 範囲チェック
        startF = (std::min)(startF, totalFrames);
        endF = (std::min)(endF, totalFrames);
        UINT32 lenF = (endF > startF) ? (endF - startF) : 0;

        // 2) バッファ構築
        XAUDIO2_BUFFER buf = {};
        buf.pAudioData = A.SoundData + size_t(startF) * bps;
        buf.AudioBytes = lenF * bps;
        buf.LoopBegin = 0;        // 部分バッファ先頭をループ開始
        buf.LoopLength = lenF;     // 部分バッファ全域をループ範囲に
        buf.LoopCount = XAUDIO2_LOOP_INFINITE;
        buf.Flags = XAUDIO2_END_OF_STREAM;

        // 3) 再生
        A.SourceVoice->Stop();
        A.SourceVoice->FlushSourceBuffers();
        A.SourceVoice->SubmitSourceBuffer(&buf);
        A.SourceVoice->SetVolume(volume);
        A.SourceVoice->SetFrequencyRatio(Pitch);
        A.SourceVoice->Start();


    }
    else
    {
        // それ以外の通常パターン（部分再生 or 単純ループ）
        // まず再生したい範囲をフレーム単位で決める
        UINT32 lengthFrames = (playEndSecond > 0.0f)
            ? UINT32((playEndSecond - playBeginSecond) * sr)
            : framesToEnd;
        lengthFrames = (std::min)(lengthFrames, framesToEnd);

        XAUDIO2_BUFFER buf = {};
        buf.pAudioData = A.SoundData + size_t(startFrame) * bps;
        buf.AudioBytes = lengthFrames * bps;
        buf.Flags = XAUDIO2_END_OF_STREAM;

        if (Loop)
        {
            // ループするならバッファ全体ではないですが、Submit 1 回で
            // PlayBegin/PlayLength を使う方法もありますし、上と同じく
            // 「先頭から lengthFrames だけ無限ループ」というパターンでも OK
            buf.PlayBegin = 0;
            buf.PlayLength = 0;
            buf.LoopBegin = 0;
            buf.LoopLength = lengthFrames;
            buf.LoopCount = XAUDIO2_LOOP_INFINITE;
        }
        A.SourceVoice->SubmitSourceBuffer(&buf);
    }

    // 共通：ボリューム・ピッチ設定＆再生スタート
    A.SourceVoice->SetVolume(volume);
    A.SourceVoice->SetFrequencyRatio(Pitch);
    A.SourceVoice->Start();
}

void SetVolumeAudio(int Index, float volume)
{
	g_Audio[Index].SourceVoice->SetVolume(volume);
}
void StopAudio(int Index)
{
	g_Audio[Index].SourceVoice->Stop();
}

//終端に達しているかを調べる
bool IsExitAudio(int Index)
{
	return false; //つくりかけ
	//g_Audio[Index].SourceVoice->ExitLoop();
}


//std::vector<float> GetAudioSamples(int Index)
//{
//    XAUDIO2_VOICE_STATE state;
//    g_Audio[Index].SourceVoice->GetState(&state);
//    if (state.BuffersQueued <= 0)
//    {
//        return {}; // 再生中ではない
//    }
//
//
//    // 現在の再生位置を取得（サンプル単位）
//    size_t samplesPlayed = state.SamplesPlayed;
//    size_t bufferSize = g_Audio[Index].bufinfo.AudioBytes / sizeof(int16_t); // 16bit PCM の場合
//
//    // 再生位置がバッファの範囲内かチェック
//    if (samplesPlayed >= bufferSize)
//    {
//        return {}; // 再生完了
//    }
//
//    if (g_Audio[Index].bufinfo.pAudioData == nullptr)
//    {
//        return {};
//    }
//
//    // bufinfo.pAudioData の型を適切にキャスト
//    const int16_t* audioData = reinterpret_cast<const int16_t*>(g_Audio[Index].bufinfo.pAudioData);
//    if (!audioData)
//    {
//        return {};
//    }
//
//    // 波形データを取得
//    std::vector<float> samples(bufferSize - samplesPlayed);
//
//    for (size_t i = 0; i < samples.size(); i++)
//    {
//        size_t index = samplesPlayed + i;
//        if (index < bufferSize)
//        {
//            samples[i] = static_cast<float>(audioData[index]) / 32768.0f; // 16bit PCM → float (-1.0 ～ 1.0)
//        }
//        else
//        {
//            samples[i] = 0.0f; // バッファを超えた場合は無音
//        }
//    }
//
//    return samples;
//}


void FFT_Audio(std::vector<Complex>& a)
{
    int N = (int)a.size();
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

std::vector<float> ProcessAudio(int Index)
{
    //auto audioSamples = GetAudioSamples(Index);

    //std::vector<Complex> fftInput(audioSamples.size());
    //for (size_t i = 0; i < audioSamples.size(); i++) {
    //    fftInput[i] = Complex(audioSamples[i], 0.0);
    //}

    //FFT_Audio(fftInput);

    ////結果を出力
    //std::vector<double> r;
    //for (const auto& val : fftInput) 
    //{
    //    r.emplace_back(abs(val));
    //}

    //return r;

    return g_Audio[Index].spectrum; // FFT結果を返す
}

