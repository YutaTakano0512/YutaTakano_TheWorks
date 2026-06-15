// ===================================================
// SCENE_Game.h ゲームシーン制御
// 
// hara sougo  2024/07/22
// ===================================================
#ifndef _SCENE_GAME_H_
#define _SCENE_GAME_H_

#include "../Game/Systems/sys_general.h"

// ===================================================
// プロトタイプ宣言
// ===================================================
void InitializeSceneGame(void);
void UpdateSceneGame(void);
void DrawSceneGame(void);
void FinalizeSceneGame(void);

int GetStageCountFrameSceneGame(void);

bool IsStageCountStopSceneGame(void);
void SetStageCountStopSceneGame(bool b);


class Stage_Base;
Stage_Base* GetCurrentStageSceneGame();
#endif