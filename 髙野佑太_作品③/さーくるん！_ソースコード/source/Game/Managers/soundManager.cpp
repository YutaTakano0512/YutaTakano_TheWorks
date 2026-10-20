// ===================================================
// soundManager.cpp サウンドの管理
// 
// hara sougo  2024/08/21
// ===================================================
#define IS_SOUND_MANAGER
#include "../../game_main.h"

#include "soundManager.h"

#define REQUEST_SOUND_MAX (64)

struct REQUEST_SOUND {
	bool isUse;
	SOUND_ID id;
    float volume;
    float pitch;
    int delayFrame;
};
struct REQUEST_BGM {
    bool isUse;
    SOUND_ID id;
    float volume;
    bool loop;
    int delayFrame;

    float playBeginSecond;
    float playEndSecond;
};

static REQUEST_SOUND requestSounds[REQUEST_SOUND_MAX] = {};
static REQUEST_BGM requestBgms[REQUEST_SOUND_MAX] = {};

static float g_BgmVolumeBuf = 0;

static int g_BgmStopDelayF = 0;

#if defined(_DEBUG) || defined(DEBUG)
#include "../../Core/Input/keyboard.h"
static bool g_isNoBGM = false;
#endif

void InitSoundManager(void)
{
    for (int i = 0; i < REQUEST_SOUND_MAX; i++)
    {
        requestSounds[i] = {};
        requestSounds[i].isUse = false;
    }
    for (int i = 0; i < REQUEST_SOUND_MAX; i++)
    {
        requestBgms[i] = {};
        requestBgms[i].isUse = false;
    }
}

void UpdateSoundManager(void)
{
#if defined(_DEBUG) || defined(DEBUG)
    //デバッグ BGM無効化
    if (GetAsyncKeyState(VK_SHIFT))
    {
        if (Keyboard_IsKeyTrigger(KK_F1))
        {
            g_isNoBGM = !g_isNoBGM;
            if (g_isNoBGM)
            {
                SoundBGMStopRequest();
            }
        }
    }
#endif

	//一気に再生
	for (int i = 0; i < REQUEST_SOUND_MAX; i++)
	{
		if (requestSounds[i].isUse)
		{
            if (requestSounds[i].delayFrame > 0)
            {
                requestSounds[i].delayFrame--;
            }
            else
            {
                PlaySE(requestSounds[i].id, requestSounds[i].volume, 0, requestSounds[i].pitch);
                //PlaySE(requestSounds[i].id, requestSounds[i].volume, requestSounds[i].delayFrame);
                requestSounds[i].isUse = false;
            }
		}
	}
    //一気に再生
    for (int i = 0; i < REQUEST_SOUND_MAX; i++)
    {
        if (requestBgms[i].isUse)
        {
            if (requestBgms[i].delayFrame > 0)
            {
                requestBgms[i].delayFrame--;
            }
            else
            {
#if defined(_DEBUG) || defined(DEBUG)
                //デバッグ BGM無効化
                if (g_isNoBGM)
                {
                }
                else
#endif
                {
                    PlayBGM(
                        requestBgms[i].id,
                        requestBgms[i].loop,
                        requestBgms[i].playBeginSecond,
                        requestBgms[i].playEndSecond
                    );
                }
                SetVolumeBGM(requestBgms[i].volume * SM_BGM_VOLUME_MOD);

                g_BgmVolumeBuf = requestBgms[i].volume;

                requestBgms[i].isUse = false;
            }
        }
    }

    if (g_BgmStopDelayF > 0)
    {
        g_BgmStopDelayF--;
        if (g_BgmStopDelayF <= 0)
        {
            StopBGM(0);
        }
    }
}

void UninitSoundManager(void)
{
}

bool SoundEffectPlayRequest(SOUND_TYPE soundId, float volume, int delayFrame, float pitch)
{
    return SoundEffectPlayRequest(
        GetSoundData(soundId), 
        volume * GetSoundDataPreset_Volume(soundId),
        delayFrame,
        pitch * GetSoundDataPreset_Pitch(soundId)
    );
}
bool SoundEffectPlayRequest(SOUND_ID soundId, float volume, int delayFrame, float pitch)
{
    if (soundId == -1)
    {
        return false;
    }

	for (int i = 0; i < REQUEST_SOUND_MAX; i++)
	{
		if (!requestSounds[i].isUse)
		{
			requestSounds[i].id = soundId;
            requestSounds[i].volume = volume;
            requestSounds[i].pitch = pitch;
			requestSounds[i].delayFrame = delayFrame;
			requestSounds[i].isUse = true;
			return true;
		}
	}
	return false;
}
bool SoundEffectStopRequest(SOUND_TYPE soundId, int fadef)
{
    return SoundEffectStopRequest(GetSoundData(soundId), fadef);
}
bool SoundEffectStopRequest(SOUND_ID soundId, int fadef)
{
    StopSE(soundId, fadef);

    return true;
}

bool SoundBGMIntroPlayRequest(SOUND_TYPE soundId, float volume, int delayFrame, float pitch)
{
#if defined(_DEBUG) || defined(DEBUG)
    //デバッグ BGM無効化
    if (g_isNoBGM)
    {
        return true;
    }
#endif
    return SoundEffectPlayRequest(
        soundId,
        volume * SM_BGM_VOLUME_MOD,
        delayFrame, pitch);
}
bool SoundBGMIntroPlayRequest(SOUND_ID soundId, float volume, int delayFrame, float pitch)
{
#if defined(_DEBUG) || defined(DEBUG)
    //デバッグ BGM無効化
    if (g_isNoBGM)
    {
        return true;
    }
#endif
    return SoundEffectPlayRequest(
        soundId,
        volume * SM_BGM_VOLUME_MOD,
        delayFrame, pitch);
}


bool SoundBGMPlayRequest(
    SOUND_TYPE soundId, 
    int loopf,
    float volume,
    int delayFrame,
    float playBeginSecond,
    float playEndSecond
)
{
    return SoundBGMPlayRequest(
        GetSoundData(soundId),
        loopf,
        volume * GetSoundDataPreset_Volume(soundId),
        delayFrame,
        playBeginSecond,
        playEndSecond
    );
}


bool SoundBGMPlayRequest(
    SOUND_ID soundId,
    int loopf, 
    float volume,
    int delayFrame,
    float playBeginSecond,
    float playEndSecond
)
{
    if (soundId == -1)
    {
        return false;
    }

	//PlayBGM(soundId, loopf);
	//SetVolumeBGM(volume);

    //g_BgmVolumeBuf = volume;
    //return true;

    g_BgmStopDelayF = 0;

    for (int i = 0; i < REQUEST_SOUND_MAX; i++)
    {
        if (!requestBgms[i].isUse)
        {
            requestBgms[i].id = soundId;
            requestBgms[i].volume = volume;
            requestBgms[i].loop = loopf;
            requestBgms[i].delayFrame = delayFrame;
            requestBgms[i].playBeginSecond = playBeginSecond;
            requestBgms[i].playEndSecond = playEndSecond;
            requestBgms[i].isUse = true;
            return true;
        }
    }

    return false;
}
bool SoundBGMSetVolumeRequest(float volume, int delayFrame)
{
	//SetVolumeBGM(volume, delayFrame);
    SetVolumeBGM(volume * SM_BGM_VOLUME_MOD);

    g_BgmVolumeBuf = volume;

	return true;
}
bool SoundBGMStopRequest(int fadef, int delayFrame)
{
    if (delayFrame > 0)
    {
        g_BgmStopDelayF = delayFrame;
    }
    else
    {
        SoundBGMRequestStopAllRequest();
        StopBGM(fadef);
    }

    return true;
}

bool SoundBGMRequestStopAllRequest()
{
    for (int i = 0; i < REQUEST_SOUND_MAX; i++)
    {
        requestBgms[i].isUse = false;
    }

    return true;
}

bool SoundEffectStopAllRequest()
{
    for (int i = 0; i < REQUEST_SOUND_MAX; i++)
    {
        requestSounds[i].isUse = false;
    }

    std::vector<SOUND_ID> dat = GetSoundIdAll();
    for (int i = 0; i < dat.size(); i++)
    {
        StopSE(dat[i], 0);
    }

    return true;
}

float GetSoundBGMVolume()
{
    return g_BgmVolumeBuf;
}

SOUND_ID GetSoundBGMId()
{
    return GetBGM();
}

bool IsSoundBGMRequested(SOUND_ID soundId)
{
    for (int i = 0; i < REQUEST_SOUND_MAX; i++)
    {
        if (requestBgms[i].isUse
            && requestBgms[i].id == soundId)
        {
            return true;
        }
    }
    return false;
}
