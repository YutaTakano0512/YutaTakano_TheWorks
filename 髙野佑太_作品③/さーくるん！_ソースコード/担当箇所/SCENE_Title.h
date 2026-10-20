// ===================================================
// SCENE_Title.h タイトルシーン制御
// 
// hara sougo  2024/07/22
// ===================================================
#ifndef _SCENE_TITLE_H_
#define _SCENE_TITLE_H_

#include "../Game/Systems/sys_general.h"

// ==================================================
// 列挙体
// ==================================================
// マルチで参加しているかどうか
typedef enum
{
    LOG_OUT = 0,
    LOG_IN
}MULTI_FLAG;

// ===================================================
// プロトタイプ宣言
// ===================================================
void InitializeSceneTitle(void);
void UpdateSceneTitle(void);
void DrawSceneTitle(void);
void FinalizeSceneTitle(void);

MULTI_FLAG GetMultiFlag();
bool IsSingleBossBattleFlag();

enum class Difficulty;
Difficulty GetDifficultySceneTitle();
void SetDifficultySceneTitle(Difficulty diff);

#endif