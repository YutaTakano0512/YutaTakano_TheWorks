#pragma once
#include <xaudio2.h>
#include <string>

//=============================================================================
// Audio クラス (XAudio2ベースの単一音源)
//=============================================================================
class Audio
{
public:
    static void InitMaster();
    static void UninitMaster();

    Audio();
    ~Audio();

    bool Load(const std::string& filePath);
    void Unload();

    void Play(bool loop = false);
    void Stop();
    void SetVolume(float volume);

    bool IsPlaying() const;

private:
    static IXAudio2*               m_XAudio2;
    static IXAudio2MasteringVoice* m_MasteringVoice;

    IXAudio2SourceVoice* m_SourceVoice = nullptr;
    BYTE*                m_SoundData = nullptr;
    UINT32               m_Length = 0;
};
