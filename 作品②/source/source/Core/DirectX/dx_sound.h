//---------------------------------------------------------------------------
// dx_sound.h
// 
// sougo hara  2024/07/17
//---------------------------------------------------------------------------
#ifndef _DX_SOUND_H_
#define _DX_SOUND_H_


void DX_SoundInitialize(void);
void DX_SoundFinalize(void);

void DX_SoundWaitLoad();

int DX_SoundLoad(const char* path, bool isBGM);
void DX_SoundSetVolume(int id, float volume);
void DX_SoundStop(int id);
void DX_SoundPlay(
    int id, float volume, bool loop = false, float pitch= 1.0f,
    float playBeginSecond = 0.0f,
    float playEndSecond = -1.0f
);
#endif // _SOUND_H_

