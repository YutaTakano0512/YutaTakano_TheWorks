#include "audio.h"
#include <fstream>
#include <iostream>

#pragma comment(lib, "xaudio2.lib")

//=============================================================================
// RIFFチャンク構造体
//=============================================================================
struct ChunkHeader
{
    char id[4];
    uint32_t size;
};

struct RiffHeader
{
    ChunkHeader chunk;
    char format[4];
};

struct FormatChunk
{
    ChunkHeader chunk;
    uint16_t audioFormat;
    uint16_t numChannels;
    uint32_t sampleRate;
    uint32_t byteRate;
    uint16_t blockAlign;
    uint16_t bitsPerSample;
};

//=============================================================================
// 静的メンバ定義
//=============================================================================
IXAudio2*               Audio::m_XAudio2 = nullptr;
IXAudio2MasteringVoice* Audio::m_MasteringVoice = nullptr;

void Audio::InitMaster()
{
    CoInitializeEx(nullptr, COINIT_MULTITHREADED);
    if (FAILED(XAudio2Create(&m_XAudio2, 0, XAUDIO2_DEFAULT_PROCESSOR))) return;
    if (FAILED(m_XAudio2->CreateMasteringVoice(&m_MasteringVoice))) return;
    
    // 全体的にSEの音量を下げる
    if (m_MasteringVoice) m_MasteringVoice->SetVolume(0.3f);
}

void Audio::UninitMaster()
{
    if (m_MasteringVoice)
    {
        m_MasteringVoice->DestroyVoice();
        m_MasteringVoice = nullptr;
    }
    if (m_XAudio2)
    {
        m_XAudio2->Release();
        m_XAudio2 = nullptr;
    }
    CoUninitialize();
}

//=============================================================================
// Audio メンバ関数
//=============================================================================
Audio::Audio() {}
Audio::~Audio() { Unload(); }

bool Audio::Load(const std::string& filePath)
{
    Unload();

    std::ifstream file(filePath, std::ios::binary);
    if (!file)
    {
        OutputDebugStringA(("Audio Error: Cannot open file " + filePath + "\n").c_str());
        return false;
    }

    RiffHeader riffHeader;
    file.read(reinterpret_cast<char*>(&riffHeader), sizeof(RiffHeader));
    if (strncmp(riffHeader.chunk.id, "RIFF", 4) != 0 || strncmp(riffHeader.format, "WAVE", 4) != 0)
    {
        OutputDebugStringA(("Audio Error: Not a valid RIFF/WAVE file " + filePath + "\n").c_str());
        return false;
    }

    WAVEFORMATEX wfx = {};
    ChunkHeader chunkHeader;
    while (file.read(reinterpret_cast<char*>(&chunkHeader), sizeof(ChunkHeader)))
    {
        if (strncmp(chunkHeader.id, "fmt ", 4) == 0)
        {
            FormatChunk fmtChunk;
            file.seekg(static_cast<std::streampos>(file.tellg()) - static_cast<std::streampos>(sizeof(ChunkHeader)));
            file.read(reinterpret_cast<char*>(&fmtChunk), sizeof(FormatChunk));

            wfx.wFormatTag      = fmtChunk.audioFormat;
            wfx.nChannels       = fmtChunk.numChannels;
            wfx.nSamplesPerSec  = fmtChunk.sampleRate;
            wfx.nAvgBytesPerSec = fmtChunk.byteRate;
            wfx.nBlockAlign     = fmtChunk.blockAlign;
            wfx.wBitsPerSample  = fmtChunk.bitsPerSample;
            wfx.cbSize          = 0;

            if (chunkHeader.size > sizeof(FormatChunk) - sizeof(ChunkHeader))
            {
                file.seekg(chunkHeader.size - (sizeof(FormatChunk) - sizeof(ChunkHeader)), std::ios::cur);
            }
        }
        else if (strncmp(chunkHeader.id, "data", 4) == 0)
        {
            m_Length = chunkHeader.size;
            m_SoundData = new BYTE[m_Length];
            file.read(reinterpret_cast<char*>(m_SoundData), m_Length);
            break;
        }
        else
        {
            file.seekg(chunkHeader.size, std::ios::cur);
        }
    }

    if (!m_SoundData || !m_XAudio2)
    {
        OutputDebugStringA(("Audio Error: Failed to parse data chunk " + filePath + "\n").c_str());
        return false;
    }

    if (FAILED(m_XAudio2->CreateSourceVoice(&m_SourceVoice, &wfx)))
    {
        OutputDebugStringA(("Audio Error: XAudio2 CreateSourceVoice failed " + filePath + "\n").c_str());
        delete[] m_SoundData;
        m_SoundData = nullptr;
        return false;
    }

    return true;
}

void Audio::Unload()
{
    if (m_SourceVoice)
    {
        m_SourceVoice->Stop(0);
        m_SourceVoice->FlushSourceBuffers();
        m_SourceVoice->DestroyVoice();
        m_SourceVoice = nullptr;
    }
    if (m_SoundData)
    {
        delete[] m_SoundData;
        m_SoundData = nullptr;
    }
}

void Audio::Play(bool loop)
{
    if (!m_SourceVoice || !m_SoundData) return;

    Stop();

    XAUDIO2_BUFFER buffer = {};
    buffer.pAudioData = m_SoundData;
    buffer.AudioBytes = m_Length;
    buffer.Flags      = XAUDIO2_END_OF_STREAM;
    buffer.LoopCount  = loop ? XAUDIO2_LOOP_INFINITE : 0;

    m_SourceVoice->SubmitSourceBuffer(&buffer);
    m_SourceVoice->Start(0);
}

void Audio::Stop()
{
    if (!m_SourceVoice) return;
    m_SourceVoice->Stop(0);
    m_SourceVoice->FlushSourceBuffers();
}

void Audio::SetVolume(float volume)
{
    if (!m_SourceVoice) return;
    m_SourceVoice->SetVolume(volume);
}

bool Audio::IsPlaying() const
{
    if (!m_SourceVoice) return false;
    XAUDIO2_VOICE_STATE state;
    m_SourceVoice->GetState(&state);
    return state.BuffersQueued > 0;
}
