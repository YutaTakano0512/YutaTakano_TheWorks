// ===================================================
// SCENE_Editor.h エディタシーン制御
// 
// hara sougo  2024/08/11
// ===================================================
#ifndef _SCENE_EDITOR_H_
#define _SCENE_EDITOR_H_

#include "../Game/Systems/sys_general.h"

// ===================================================
// プロトタイプ宣言
// ===================================================
void InitializeSceneEditor(void);
void UpdateSceneEditor(void);
void DrawSceneEditor(void);
void FinalizeSceneEditor(void);


#ifdef SWITCH_MODE
#else
void OnWindowEventEditor(unsigned short cas, unsigned short cod);
#endif


#endif