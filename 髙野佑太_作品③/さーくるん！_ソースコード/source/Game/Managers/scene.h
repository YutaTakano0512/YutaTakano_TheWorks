// ===================================================
// scene.h シーン制御
// 
// hara sougo  2024/07/22
// ===================================================
#ifndef _SCENE_H_
#define _SCENE_H_

#include "../Systems/sys_general.h"

#include "../Systems/fade.h"

// ===================================================
// マクロ定義
// ===================================================

// ===================================================
// 列挙体宣言
// ===================================================

//シーンを追加する際に書き加える必要があるところへ
// ↓を書いておきます
//addpoint
enum SCENE { // シーン管理用
	SCENE_INIT = -1,
	SCENE_TITLE = 0,
	SCENE_STAGESELECT,
	SCENE_CHARACTERSELECT,
	SCENE_GAME,
	SCENE_RESULT,
    SCENE_SOUNDROOM,
    SCENE_SINGLEBOSSBATTLE,
    SCENE_OP,

    SCENE_MULTICLIENT,
    SCENE_EDITOR,

	SCENE_MAX

};

// ===================================================
// 構造体宣言
// ===================================================
// 
// ===================================================
// プロトタイプ宣言
// ===================================================

void InitScene(void);
void UpdateScene(void);
void DrawScene(void);
void UninitScene(void);

SCENE GetSceneBefore(void);
SCENE GetScene(void);
void SetScene(SCENE s);

bool SetSceneFade(SCENE setScene,
    FADE_TYPE fadetype = FADE_TYPE_COLOR_BLACK,
    std::function<float(float)> easing= EasingInOutSine,
    int speed=0
);

#endif