// ===================================================
// SCENE_MultiClient.h マルチ クライアント制御
// 
// hara sougo  2024/12/15
// ===================================================
#ifndef _SCENE_MULTICLIENT_H_
#define _SCENE_MULTICLIENT_H_

#include "../Game/Systems/sys_general.h"

// ===================================================
// プロトタイプ宣言
// ===================================================
void InitializeSceneMultiClient(void);
void UpdateSceneMultiClient(void);
void DrawSceneMultiClient(void);
void FinalizeSceneMultiClient(void);

#endif