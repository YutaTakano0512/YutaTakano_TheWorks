// ===================================================
// sound.h サウンド
// 
// hara sougo  2024/08/21
// ===================================================
#ifndef _SOUND_H_
#define _SOUND_H_


#include "../../Core/_systemModeSwitch.h"

#ifdef SWITCH_MODE
#else
#include "../../Core/DirectX/dx_sound.h"
typedef int SOUND_ID;
#endif

#ifdef IS_MAIN //メイン以外からは呼び出せないようにしておく
void InitSound();
void UninitSound();
void UpdateSound();
#endif


#ifdef IS_SOUND_MANAGER //サウンドマネージャー以外からは呼び出せないようにしておく
void PlayBGM(SOUND_ID soundId, int loopf, float playBeginSecond,
    float playEndSecond);
void StopBGM(int fadef);
SOUND_ID GetBGM();
void SetVolumeBGM(float volume, int delayFrame = 0);

void PlaySE(SOUND_ID soundId, float volume, int delayFrame, float pitch);
void StopSE(SOUND_ID soundId, int fadef);
#endif

#endif