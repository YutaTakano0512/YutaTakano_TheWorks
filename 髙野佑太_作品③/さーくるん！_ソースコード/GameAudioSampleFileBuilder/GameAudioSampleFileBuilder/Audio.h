#pragma once

#include <xaudio2.h>
#include <assert.h>

#include <vector>
#include <complex>

void InitAudio();
void UninitAudio();


int LoadAudio(const char* FileName, bool isBGM);
void UnloadAudio(int Index);

std::vector<float> GetAudioDat(int index);