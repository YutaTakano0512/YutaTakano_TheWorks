// ===================================================
// soundManager.h サウンドの管理
// 
// hara sougo  2024/08/21
// ===================================================
#ifndef _SOUND_MANAGER_H_
#define _SOUND_MANAGER_H_


#include "../Systems/sys_general.h"

#include "../Systems/sound.h"
#include "../Sound/soundData.h"

// ===================================================
// マクロ定義
// ===================================================
constexpr float SM_BGM_VOLUME_MOD = 0.6f;

// ===================================================
// 構造体宣言
// ===================================================

// ===================================================
// プロトタイプ宣言
// ===================================================
#ifdef IS_GAME_MANAGER //ゲームマネージャー以外からは呼び出せないようにしておく
void InitSoundManager(void);

void UpdateSoundManager(void);

void UninitSoundManager(void);
#endif

bool SoundEffectPlayRequest(SOUND_ID soundId, float volume, int delayFrame = 0, float pitch = 1.0f);
bool SoundEffectStopRequest(SOUND_ID soundId, int fadef = 0.0f);
bool SoundBGMPlayRequest(
    SOUND_ID soundId,
    int loopf,
    float volume = 1.0f,
    int delayFrame = 0,
    float playBeginSecond = 0.0f,
    float playEndSecond = -1.0f
);
bool SoundBGMIntroPlayRequest(SOUND_ID soundId, float volume, int delayFrame = 0, float pitch = 1.0f);
bool SoundBGMIntroPlayRequest(SOUND_TYPE soundId, float volume, int delayFrame = 0, float pitch = 1.0f);

bool SoundEffectPlayRequest(SOUND_TYPE soundId, float volume, int delayFrame=0, float pitch=1.0f);
bool SoundEffectStopRequest(SOUND_TYPE soundId, int fadef = 0.0f);
bool SoundBGMPlayRequest(
    SOUND_TYPE soundId,
    int loopf, 
    float volume=1.0f,
    int delayFrame = 0,
    float playBeginSecond = 0.0f,
    float playEndSecond = -1.0f
);
bool SoundBGMSetVolumeRequest(float volume, int delayFrame =0.0f);
bool SoundBGMStopRequest(int fadef = 0.0f, int delayFrame = 0.0f);
bool SoundBGMRequestStopAllRequest();


bool SoundEffectStopAllRequest();

float GetSoundBGMVolume();
SOUND_ID GetSoundBGMId();
bool IsSoundBGMRequested(SOUND_ID soundId);
#endif