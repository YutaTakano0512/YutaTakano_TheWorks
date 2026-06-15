// ===================================================
// SCENE_SingleBossBattle.h タイトルシーン制御
// 
// Takano Yuta  2024/11/29
// ===================================================
#ifndef _SCENE_SINGLEBOSSBATTLE_H_
#define _SCENE_SINGLEBOSSBATTLE_H_

#include "../Game/Systems/sys_general.h"
#include "../Game/Systems/easing.h"

// ===================================================
// 列挙体宣言
// ===================================================

// ===================================================
// 構造体
// ===================================================

// ===================================================
// プロトタイプ宣言
// ===================================================

void InitializeSceneSingleBossBattle(void);
void UpdateSceneSingleBossBattle(void);
void DrawSceneSingleBossBattle(void);
void FinalizeSceneSingleBossBattle(void);

int GetSelectNumberSingleBossBattle(void);

#endif