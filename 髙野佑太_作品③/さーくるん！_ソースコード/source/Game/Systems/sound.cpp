// ===================================================
// sound.cpp サウンド
// 
// hara sougo  2024/08/21
// ===================================================
#define IS_SOUND_CPP
#include "../../Core/_systemModeSwitch.h"

#ifdef SWITCH_MODE
#include "../sound_switch.h"
#endif

#include "../../game_main.h"
#include "sound.h"


#ifdef SWITCH_MODE
static SOUND_ID bgmId = {};
static float bgmVolume = 0;
static int bgmFrameCount = 0;
static int bgmFrameCountMax = 0;
#else
static SOUND_ID bgmId = -1;
#endif


void InitSound()
{

#ifdef SWITCH_MODE
	 bgmId = {};
	 bgmVolume = 0;
	 bgmFrameCount = 0;
	 bgmFrameCountMax = 0;

	InitSoundSwitch();
#endif
}



void UninitSound()
{
#ifdef SWITCH_MODE
	UninitSoundSwitch();
#endif
}


void UpdateSound()
{
#ifdef SWITCH_MODE
	UpdateSoundSwitch();

	if (bgmFrameCountMax > 0)
	{
		if (bgmFrameCount <= 0)
		{
			PlayBGMSwitch(bgmId);
			SetVolumeBGMSwitch(bgmVolume, 0);
			bgmFrameCount = bgmFrameCountMax;
		}
		else
		{
			bgmFrameCount--;
		}
	}
#endif
}




void PlayBGM(SOUND_ID soundId, int loopf,
    float playBeginSecond,
    float playEndSecond
)
{
#ifdef SWITCH_MODE
	bgmId = soundId;
	bgmFrameCountMax = loopf;
	bgmFrameCount = bgmFrameCountMax;
	PlayBGMSwitch(bgmId);
#else
    if (bgmId >= 0)
    {
        DX_SoundStop(bgmId);
    }

	bgmId = soundId;
	DX_SoundPlay(bgmId, 1.0f, loopf, 1.0f, playBeginSecond, playEndSecond);
#endif
}

void StopBGM(int fadef)
{
#ifdef SWITCH_MODE
	bgmFrameCount = 0;
	bgmFrameCountMax = 0;

	StopBGMSwitch(fadef);
#else
    if (bgmId < 0)
    {
        return;
    }
	DX_SoundStop(bgmId);
    bgmId = -1;
#endif
}

SOUND_ID GetBGM()
{
	return bgmId;
}

void SetVolumeBGM(float volume, int delayFrame)
{
#ifdef SWITCH_MODE
	bgmVolume = volume;
	SetVolumeBGMSwitch(bgmVolume, delayFrame);
#else
	DX_SoundSetVolume(bgmId, volume);
#endif
}

void PlaySE(SOUND_ID soundId, float volume, int delayFrame, float pitch)
{
#ifdef SWITCH_MODE
	PlaySESwitch(soundId, volume, delayFrame);
	//PlaySESwitch(soundList[soundId]);
#else
	DX_SoundPlay(soundId, volume, false, pitch);
#endif
}
void StopSE(SOUND_ID soundId, int fadef)
{
#ifdef SWITCH_MODE
#else
    DX_SoundStop(soundId);
#endif
}