// ===================================================
// SCENE_StageSelect.h タイトルシーン制御
// 
// Takano Yuta  2024/11/29
// ===================================================
#ifndef _SCENE_STAGE_SELECT_H_
#define _SCENE_STAGE_SELECT_H_

#include "../Game/Systems/sys_general.h"
#include "../Game/Systems/easing.h"

// ===================================================
// 列挙体宣言
// ===================================================
//ステージセレクトシーンの状態遷移
typedef enum
{
    STAGESELECT_SCENE_STATE_NONE,
    STAGESELECT_SCENE_STATE_ANIMATION_IN,
    STAGESELECT_SCENE_STATE_SELECT,
    STAGESELECT_SCENE_STATE_ANIMATION_STAMP,
    STAGESELECT_SCENE_STATE_ANIMATION_OUT_TO_NEXT,
    STAGESELECT_SCENE_STATE_ANIMATION_OUT_TO_TITLE,
    STAGESELECT_SCENE_STATE_MAX,
}STAGESELECT_SCENE_STATE;

//選んでいるステージのモード状態
typedef enum
{
    STAGESELECT_TYPE_NONE,
    STAGESELECT_TYPE_STAGE1,//1
    STAGESELECT_TYPE_STAGE2,//2
    STAGESELECT_TYPE_MAX,
}STAGESELECTTYPE;

typedef enum
{
    EASE_IN_OUT_QUAD,
    EASE_OUT_QUINT,
    EASE_IN_QUINT,
    EASE_OUT_ELASTIC,
    EASE_COUNT //総数
}EASE_TYPE;

// イージング関数の型
typedef float (*EaseFunction)(float);

static EaseFunction easeFunctions[EASE_COUNT] =
{
    EasingInOutQuad,
    EasingOutQuint,
    EasingInQuint,
    EasingOutElastic,
};

// ===================================================
// 構造体
// ===================================================

// ===================================================
// プロトタイプ宣言
// ===================================================
static float SetEasing(EASE_TYPE type, float t)
{
    if (type < 0 || type >= EASE_COUNT)
    {
        printf("Error");
        return 0.0f;
    }
    return easeFunctions[type](t);
}

void InitializeUIParameter(void);
void InitializeSceneStageSelect(void);
void UpdateSceneStageSelect(void);
void DrawSceneStageSelect(void);
void FinalizeSceneStageSelect(void);


void InDetailUI(STAGESELECTTYPE stageselect);
void Update_StageSelectType(void);
void Update_Default(void);
void Update_Detail(void);




STAGESELECTTYPE GetStageSelectState();

#endif