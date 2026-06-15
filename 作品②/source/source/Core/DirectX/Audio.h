#pragma once

#include <xaudio2.h>
#include <assert.h>

#include <vector>
#include <complex>

void InitAudio();
void UninitAudio();


int LoadAudio(const char* FileName, bool isBGM);
void UnloadAudio(int Index);

void WaitLoadAudio();

void SetVolumeAudio(int Index, float volume);
void StopAudio(int Index);
void PlayAudio(
    int Index, float volume, bool Loop = true, float Pitch=1.0f,
    float playBeginSecond=0.0f,
    float playEndSecond = -1.0f
);


bool IsAudioPlaying(int index);
const std::vector<float>* GetAudioFFTData(int index);
int GetAudioFFTIndex(int index);
void LoadAudioFFTData(int index);
std::vector<float> ProcessAudio(int Index);