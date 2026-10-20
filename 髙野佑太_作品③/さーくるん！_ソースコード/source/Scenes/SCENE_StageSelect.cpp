#pragma once
// ===================================================
// SCENE_StageSelect.cpp タイトルシーン制御
// 
// Takano Yuta  2025/01/17
// ===================================================
#include "../Game/Systems/sound.h"
#include "../game_main.h"

#include "SCENE_StageSelect.h"
#include "SCENE_Title.h"

#include "../Game/Texture/texData.h"
#include "../Game/Managers/bg.h"
#include "../Core/Input/controller_main.h"

#include "../Game/Systems/fade.h"
#include "../Game/Managers/scene.h"

#include "../Game/Sprite/sprite.h"
#include "../Game/Sprite/spriteGraph.h"
#include "../Game/Game Object/gameObject.h"
#include "../Game/Game Object/gameObjectGraph.h"
#include "../Game/Game Object/gameObjectPrefab.h"
#include "../Game/Managers/gameManager.h"
#include "../Game/Managers/soundManager.h"
#include "../Game/Systems/camera.h"

#include "../Game/Systems/ui.h"

#include "../Game/Systems/easing.h"



//このシーンのステート
static STAGESELECT_SCENE_STATE g_StageSelectState = STAGESELECT_SCENE_STATE_NONE;
//選んでいるステージのステート
static STAGESELECTTYPE g_StageSelect = STAGESELECT_TYPE_NONE;

//エフェクト関連
static TEX_DAT g_Effect = {};
static int g_EffectFrame = 0;
static TEX_DAT g_Pin = {};

//スティック判定(今いいのが出ないからとりあえずここにいっぱい増やす)
static bool stickTrigger_RX = false;
static bool stickTrigger_LX = false;
static bool stickTrigger_UY = false;
static bool stickTrigger_DY = false;

static bool g_animIn = false;
static bool g_animOut = false;
static float g_In = 0.0f;
static float g_Out = 0.0f;

static bool g_Stamp = false;
static float g_StampCount = 0.0f;

// ===================================================
//アニメーション用フレームカウンター
// ===================================================
struct FrameController
{
    float current;//現在のフレーム進行度
    bool isComplete;//アニメーション完了フラグ

    // 順再生処理
    void PlayForward(float deltaTime)
    {
        if (isComplete) return; // 完了状態ならスキップ

        current += deltaTime;

        // 完了判定
        if (current >= 1.0f)
        {
            current = 1.0f;
            isComplete = true;
        }
    }

    // 逆再生処理
    void PlayBackward(float deltaTime)
    {
        if (isComplete) return; // 完了状態ならスキップ

        current -= deltaTime;

        // 完了判定
        if (current <= 0.0f)
        {
            current = 0.0f;
            isComplete = true;
        }
    }

    //いつか点滅用の関数も作る

    //リセット
    void Reset() { current = 0.0f; isComplete = false; }
};

// ===================================================
//　UIのパラメータ
// ===================================================
typedef enum
{
    UI_TYPE_NONE,
    UI_TYPE_BG_ALL,
    UI_TYPE_MAP,
    UI_TYPE_LIGHT,
    UI_TYPE_CURSOR,
    UI_TYPE_DETAIL1,
    UI_TYPE_DETAIL2,
    UI_TYPE_NOSTAGE,
    UI_TYPE_BUTTON_A_ON,
    UI_TYPE_BUTTON_A_OFF,
    UI_TYPE_BUTTON_B_ON,
    UI_TYPE_BUTTON_B_OFF,
    UI_TYPE_STUMP,
    UI_TYPE_STUMP_2,
    UI_TYPE_MAX
}UI_TYPE;

typedef struct
{
    //UIの情報を入れる
    TEX_DAT        TextureData;//テクスチャIDをここに格納する

    Float3              position;//UIの位置を格納する
    float                  rotation;//UIの角度(ラジアン角)を格納する
    Float3              scale;//UIのサイズを格納する

    Float4              color;//UIの色を格納

    // 目標値(補間先)
    Float3              targetPosition;
    float                  targetRotation;
    Float3              targetScale;
    Float4              targetColor;

    //補間前の値
    Float3              oldPosition;
    float                  oldRotation;
    Float3              oldScale;
    Float4              oldColor;

    FrameController animation;
}UI_PARAMETAR;

UI_PARAMETAR g_UI[UI_TYPE_MAX] = {};

typedef enum
{
    STAGESELECT_STATE_DEFAULT,         // 通常選択状態
    STAGESELECT_STATE_DETAIL_ON,       // 詳細表示状態
}STAGESELECT_STATE;

STAGESELECT_STATE g_SelectState = STAGESELECT_STATE_DEFAULT;

//内部関数
//アニメーション関数
void UpdateUI(UI_PARAMETAR& ui, EASE_TYPE type, float deltaTime);
void UpdateUIPositionOnly(UI_PARAMETAR& ui, EASE_TYPE type, float deltaTime);
void UpdateUIScaleOnly(UI_PARAMETAR& ui, EASE_TYPE type, float deltaTime);
void UpdateUIRotationOnly(UI_PARAMETAR& ui, EASE_TYPE type, float deltaTime);
void UpdateUIColorOnly(UI_PARAMETAR& ui, EASE_TYPE type, float deltaTime);
void StartUIAnimation(UI_PARAMETAR& ui, Float3 newPosition, Float3 newScale, float newRotation, Float4 newColor);
void StartUIAnimationPositionOnly(UI_PARAMETAR& ui, Float3 newPosition);
void StartUIAnimationScaleOnly(UI_PARAMETAR& ui, Float3 newScale);
void StartUIAnimationRotationOnly(UI_PARAMETAR& ui, float newRotation);
void StartUIAnimationColorOnly(UI_PARAMETAR& ui, Float4 newColor);

float Lerp(float a, float b, float t);
Float3 Lerp(Float3 a, Float3 b, float t);
Float4 Lerp(Float4 a, Float4 b, float t);


//メモ：GetSceneBefore() 一個前のシーンを取得できる紙機能



// ===================================================
// ステージセレクトシーンのステージステート取得
// ===================================================;
STAGESELECTTYPE GetStageSelectState()
{
    return g_StageSelect;
}

// ===================================================
// ステージセレクトシーンのUIの初期化
// ===================================================
void InitializeUIParameter(void)
{
    g_UI[UI_TYPE_BG_ALL].TextureData = GetTexData(TEX_TYPE_UI_BACK);
    g_UI[UI_TYPE_CURSOR].TextureData = GetTexData(TEX_TYPE_UI_CURSOR);
    if (GetDifficultySceneTitle() == Difficulty::Normal)
    {
        g_UI[UI_TYPE_DETAIL1].TextureData = GetTexData(TEX_TYPE_UI_DETAIL1);
        g_UI[UI_TYPE_DETAIL2].TextureData = GetTexData(TEX_TYPE_UI_DETAIL2);
    }
    else if (GetDifficultySceneTitle() == Difficulty::Hard)
    {
        g_UI[UI_TYPE_DETAIL1].TextureData = GetTexData(TEX_TYPE_UI_DETAIL3);
        g_UI[UI_TYPE_DETAIL2].TextureData = GetTexData(TEX_TYPE_UI_DETAIL4);
    }

    g_UI[UI_TYPE_LIGHT].TextureData = GetTexData(TEX_TYPE_CHARACTER_LIGHT);
    g_UI[UI_TYPE_MAP].TextureData = GetTexData(TEX_TYPE_UI_MAP);

    g_UI[UI_TYPE_NOSTAGE].TextureData = GetTexData(TEX_TYPE_UI_NOSTAGE);
    g_UI[UI_TYPE_BUTTON_A_ON].TextureData = GetTexData(TEX_TYPE_UI_BUTTON_A_ON);
    g_UI[UI_TYPE_BUTTON_A_OFF].TextureData = GetTexData(TEX_TYPE_UI_BUTTON_A_OFF);
    g_UI[UI_TYPE_BUTTON_B_ON].TextureData = GetTexData(TEX_TYPE_UI_BUTTON_B_ON);
    g_UI[UI_TYPE_BUTTON_B_OFF].TextureData = GetTexData(TEX_TYPE_UI_BUTTON_B_OFF);
    g_UI[UI_TYPE_STUMP].TextureData = GetTexData(TEX_TYPE_UI_STUMP);
    g_UI[UI_TYPE_STUMP_2].TextureData = GetTexData(TEX_TYPE_UI_STUMP);

    for (int i = 1; i < UI_TYPE_MAX; i++)
    {
        g_UI[i].animation.current = 1.0f;
        g_UI[i].animation.isComplete = true;
        g_UI[i].oldPosition = { 0.0f,0.0f,0.0f };
        g_UI[i].oldRotation = 0.0f;
        g_UI[i].oldColor = { 0.0f,0.0f,0.0f,0.0f };
        g_UI[i].oldScale = { 0.0f,0.0f,0.0f };
        g_UI[i].targetPosition = { 0.0f,0.0f,0.0f };
        g_UI[i].targetRotation = 0.0f;
        g_UI[i].targetColor = { 0.0f,0.0f,0.0f,0.0f };
        g_UI[i].targetScale = { 0.0f,0.0f,0.0f };
    }
    //ここに最初のUIの位置やサイズを入れる
    //全部
    {
        g_UI[UI_TYPE_BG_ALL].position = MkF3(0.0f, 0.0f, 0.0f);
        g_UI[UI_TYPE_BG_ALL].rotation = 0.0f;
        g_UI[UI_TYPE_BG_ALL].scale = MkF3(3840.0f * 0.6f, 2160.0f * 0.6f, 1.0f);
        g_UI[UI_TYPE_BG_ALL].color = MkF4(1.0f, 1.0f, 1.0f, 1.0f);
    }
    //マップ
    {
        g_UI[UI_TYPE_MAP].position = MkF3(-235.0f, 116.0f, 0.0f);
        g_UI[UI_TYPE_MAP].rotation = 0.0f;
        g_UI[UI_TYPE_MAP].scale = MkF3(2590.0f * 0.52f, 1634.0f * 0.52f, 1.0f);
        g_UI[UI_TYPE_MAP].color = MkF4(1.0f, 1.0f, 1.0f, 1.0f);
    }
    //LIGHT
    {
        g_UI[UI_TYPE_LIGHT].position = MkF3(0.0f, 0.0f, 0.0f);
        g_UI[UI_TYPE_LIGHT].rotation = 0.0f;
        g_UI[UI_TYPE_LIGHT].scale = MkF3(3840.0f * 0.6f, 2160.0f * 0.6f, 1.0f);
        g_UI[UI_TYPE_LIGHT].color = MkF4(1.0f, 1.0f, 1.0f, 1.0f);
    }
    //カーソル
    {
        g_UI[UI_TYPE_CURSOR].position = MkF3(-277.5f, 5.0f, 0.0f);
        g_UI[UI_TYPE_CURSOR].rotation = 0.0f;
        g_UI[UI_TYPE_CURSOR].scale = MkF3(697.0f * 0.59f, 641.0f * 0.59f, 1.0f);
        g_UI[UI_TYPE_CURSOR].color = MkF4(1.0f, 1.0f, 1.0f, 1.0f);
    }
    //詳細画面の表示
    {
        g_UI[UI_TYPE_DETAIL1].position = MkF3(0.0f, 600.0f, 600.0f);
        g_UI[UI_TYPE_DETAIL1].rotation = 0.0f;
        g_UI[UI_TYPE_DETAIL1].scale = MkF3(1859.0f * 0.35f, 1856.0f * 0.35f, 1.0f);
        g_UI[UI_TYPE_DETAIL1].color = MkF4(1.0f, 1.0f, 1.0f, 0.0f);
    }
    {
        g_UI[UI_TYPE_DETAIL2].position = MkF3(0.0f, 600.0f, 600.0f);
        g_UI[UI_TYPE_DETAIL2].rotation = 0.0f;
        g_UI[UI_TYPE_DETAIL2].scale = MkF3(1325.0f * 0.35f, 1856.0f * 0.35f, 1.0f);
        g_UI[UI_TYPE_DETAIL2].color = MkF4(1.0f, 1.0f, 1.0f, 0.0f);
    }
    //調査中ですよの表示
    {
        g_UI[UI_TYPE_NOSTAGE].position = MkF3(0.0f, 0.0f, 0.0f);
        g_UI[UI_TYPE_NOSTAGE].rotation = 0.0f;
        g_UI[UI_TYPE_NOSTAGE].scale = MkF3(2044.0f * 0.5f, 271.0f * 0.5f, 1.0f);
        g_UI[UI_TYPE_NOSTAGE].color = MkF4(1.0f, 1.0f, 1.0f, 0.0f);
    }
    // ボタンA
    {
        g_UI[UI_TYPE_BUTTON_A_ON].position = MkF3(380.0f, 265.0f, 0.0f);
        g_UI[UI_TYPE_BUTTON_A_ON].rotation = 0.0f;
        g_UI[UI_TYPE_BUTTON_A_ON].scale = MkF3(455.0f * 0.5f, 253.0f * 0.5f, 1.0f);
        g_UI[UI_TYPE_BUTTON_A_ON].color = MkF4(1.0f, 1.0f, 1.0f, 0.0f);
    }
    {
        g_UI[UI_TYPE_BUTTON_A_OFF].position = MkF3(380.0f, 265.0f, 0.0f);
        g_UI[UI_TYPE_BUTTON_A_OFF].rotation = 0.0f;
        g_UI[UI_TYPE_BUTTON_A_OFF].scale = MkF3(455.0f * 0.5f, 253.0f * 0.5f, 1.0f);
        g_UI[UI_TYPE_BUTTON_A_OFF].color = MkF4(1.0f, 1.0f, 1.0f, 0.0f);
    }
    // ボタンB
    {
        g_UI[UI_TYPE_BUTTON_B_ON].position = MkF3(-385.0f, 270.0f, 0.0f);
        g_UI[UI_TYPE_BUTTON_B_ON].rotation = 0.0f;
        g_UI[UI_TYPE_BUTTON_B_ON].scale = MkF3(399.0f * 0.5f, 246.0f * 0.5f, 1.0f);
        g_UI[UI_TYPE_BUTTON_B_ON].color = MkF4(1.0f, 1.0f, 1.0f, 0.0f);
    }
    {
        g_UI[UI_TYPE_BUTTON_B_OFF].position = MkF3(-385.0f, 270.0f, 0.0f);
        g_UI[UI_TYPE_BUTTON_B_OFF].rotation = 0.0f;
        g_UI[UI_TYPE_BUTTON_B_OFF].scale = MkF3(399.0f * 0.5f, 246.0f * 0.5f, 1.0f);
        g_UI[UI_TYPE_BUTTON_B_OFF].color = MkF4(1.0f, 1.0f, 1.0f, 0.0f);
    }
    // スタンプ
    {
        g_UI[UI_TYPE_STUMP].position = MkF3(300.0f, 160.0f, 0.0f);
        g_UI[UI_TYPE_STUMP].rotation = 0.0f;
        g_UI[UI_TYPE_STUMP].scale = MkF3(388.0f, 213.0f, 1.0f);
        g_UI[UI_TYPE_STUMP].color = MkF4(1.0f, 1.0f, 1.0f, 0.0f);
    }
    {
        g_UI[UI_TYPE_STUMP_2].position = MkF3(280.0f, 180.0f, 0.0f);
        g_UI[UI_TYPE_STUMP_2].rotation = 0.0f;
        g_UI[UI_TYPE_STUMP_2].scale = MkF3(388.0f * 0.5f, 213.0f * 0.5f, 1.0f);
        g_UI[UI_TYPE_STUMP_2].color = MkF4(1.0f, 1.0f, 1.0f, 0.0f);
    }
}

// ===================================================
// ステージセレクトシーンの初期化
// ===================================================
void InitializeSceneStageSelect(void)
{
    InitializeUIParameter();

    UIReset();
    //
    GameManagerSetSceneRequestCancel();

    {
        SOUND_ID bgm = GetSoundData("SOUND/BGM/Test_BGM_Title2");
        if (bgm != GetSoundBGMId())
        {
            //異なるBGMが流れている、
            //またはBGMが流れていない場合再生する。
            SoundBGMPlayRequest(bgm,
                (FRAME_ONE_MINUTE * 1)
                + (FRAME_ONE_SECOND * 30),
                1.0f);
        }
    }

    //キャラセレクトから来たときは初期化しない
    if (GetSceneBefore() == SCENE_TITLE)
    {
        ResetCamera();
    }
    else
    {
        g_UI[UI_TYPE_BUTTON_B_OFF].color = { 1.0f,1.0f,1.0f,1.0f };
            g_UI[UI_TYPE_BUTTON_A_OFF].color = { 1.0f,1.0f,1.0f,1.0f };
    }


    //各ステートの初期化
    //現在このシーンがどの状態か
    g_StageSelectState = STAGESELECT_SCENE_STATE_ANIMATION_IN;
    //ステージの状態
    g_StageSelect = STAGESELECT_TYPE_STAGE1;
    //詳細表示いているかの状態
    g_SelectState = STAGESELECT_STATE_DEFAULT;

    g_Effect = GetTexData(TEX_TYPE_EFFECT_MAP);
    g_Pin = GetTexData(TEX_TYPE_UI_PIN);
    g_EffectFrame = 0;

    //スティックの変数初期化
    stickTrigger_RX = false;
    stickTrigger_LX = false;
    stickTrigger_UY = false;
    stickTrigger_DY = false;

    g_animIn = false;
    g_animOut = false;
    g_In = 0.0f;
    g_Out = 0.0f;


    g_Stamp = false;
    g_StampCount = 0.0f;

    CameraMoveRequestAll(FRAME_ONE_SECOND * 1, MkF2(0.0f, 0.0f), 0.0f, MkF2(1.0f, 1.0f), 1.75f, EasingInOutSine, CAMERA_MOVE_TYPE_SMOOTH);

    //デバッグログ
    //AllocConsole(); // コンソールを作成
    //FILE* a =  freopen("CONOUT$", "w", stdout); // stdoutをコンソールにリダイレクト
    //printf("Hello, World!\n"); // コンソールに出力
}

// ===================================================
// ステージセレクトシーンの更新
// ===================================================
void UpdateSceneStageSelect(void)
{
    switch (g_StageSelectState)
    {
    case STAGESELECT_SCENE_STATE_ANIMATION_IN://シーン切り替わるとアニメーションが始まる

        if (g_In >= 1)
        {
            g_animIn = true;
            g_StageSelectState = STAGESELECT_SCENE_STATE_SELECT;
        }
        if (g_In < 1)
        {
            g_In += 1.0f / 60.0f;
        }
        break;

    case STAGESELECT_SCENE_STATE_SELECT://ステージ選択中
        if (g_animIn)
        {
            StartUIAnimationColorOnly(g_UI[UI_TYPE_BUTTON_B_OFF], { 1.0f,1.0f,1.0f,1.0f });
            StartUIAnimationColorOnly(g_UI[UI_TYPE_BUTTON_A_OFF], { 1.0f,1.0f,1.0f,1.0f });
            g_animIn = false;
        }

        Update_StageSelectType();
        if (g_Stamp)
        {
            if (g_StampCount <= 1.0f) { g_StampCount += 1.0f / 20.0f; }
            else if (g_StampCount > 1.0f)
            {
                CameraShakeRequestAll(CAMERA_SHAKE_SMALL_FRAME, CAMERA_SHAKE_SMALL_POWER);
                g_UI[UI_TYPE_STUMP_2].color = MkF4(1.0f, 1.0f, 1.0f, 1.0f);
                g_UI[UI_TYPE_STUMP].color = MkF4(1.5f, 1.5f, 1.5f, 1.0f);
                StartUIAnimation(g_UI[UI_TYPE_STUMP], { 280.0f, 180.0f, 0.0f }, { 388.0f * 0.5f, 213.0f * 0.5f, 1.0f }, 0.0f, { 1.0f,1.0f,1.0f,1.0f });
                StartUIAnimation(g_UI[UI_TYPE_STUMP_2], { 280.0f, 180.0f, 0.0f }, { 388.0f * 1.5f, 213.0f * 1.5f, 1.0f }, 0.0f, { 1.0f,1.0f,1.0f,0.0f });
                g_StageSelectState = STAGESELECT_SCENE_STATE_ANIMATION_STAMP;
                g_StampCount = 0.0f;
            }
        }
        break;

    case STAGESELECT_SCENE_STATE_ANIMATION_STAMP:
        if (g_StampCount <= 1.0f) { g_StampCount += 1.0f / 30.0f; }
        else if (g_StampCount > 1.0f)
        {
            // 詳細アイコンとボタンと机の色を戻す
            StartUIAnimationColorOnly(g_UI[UI_TYPE_BG_ALL], { 1.0f,1.0f,1.0f,1.0f });
            StartUIAnimationColorOnly(g_UI[UI_TYPE_MAP], { 1.0f,1.0f,1.0f,1.0f });
            StartUIAnimationColorOnly(g_UI[UI_TYPE_LIGHT], { 1.0f,1.0f,1.0f,1.0f });
            StartUIAnimationPositionOnly(g_UI[UI_TYPE_DETAIL1], { 0.0f,600.0f,600.0f });
            StartUIAnimationPositionOnly(g_UI[UI_TYPE_DETAIL2], { 0.0f,600.0f,600.0f });
            StartUIAnimationPositionOnly(g_UI[UI_TYPE_STUMP], {200.0f,800.0f,800.0f});
            StartUIAnimationColorOnly(g_UI[UI_TYPE_BG_ALL], { 1.0f,1.0f,1.0f,1.0f });
            g_StageSelectState = STAGESELECT_SCENE_STATE_ANIMATION_OUT_TO_NEXT;
        }

        break;

    case STAGESELECT_SCENE_STATE_ANIMATION_OUT_TO_NEXT://切り替わるタイミングでアニメーションを入れる
        if (g_Out <= 1.0f) 
        {
            g_Out += 1.0f / 35.0f;
        }
        else if (g_Out > 1.0f)
        {

            SoundEffectPlayRequest(SOUND_TYPE_SE_UI_SELECT_SCENE_EXIT, 4.0f, 10);//効果音を入れる
            SetScene(SCENE_CHARACTERSELECT);
        }
        break;

    case STAGESELECT_SCENE_STATE_ANIMATION_OUT_TO_TITLE:
        
        if (g_UI[UI_TYPE_BUTTON_B_ON].animation.isComplete)
        {
            SetSceneFade(SCENE_TITLE, FADE_TYPE_STAR_WIPE);
        }
        break;

    default:
        break;
    }

    //UIのアップデート
    UpdateUIColorOnly(g_UI[UI_TYPE_NOSTAGE],EASE_IN_QUINT, 1.0f / 60.0f);
    UpdateUIColorOnly(g_UI[UI_TYPE_BG_ALL],EASE_OUT_QUINT, 1.0f / 15.0f);
    UpdateUIColorOnly(g_UI[UI_TYPE_MAP],EASE_OUT_QUINT, 1.0f / 15.0f);
    UpdateUIColorOnly(g_UI[UI_TYPE_LIGHT],EASE_OUT_QUINT, 1.0f / 15.0f);
    UpdateUI(g_UI[UI_TYPE_BUTTON_A_ON], EASE_OUT_QUINT, 1.0f / 60.0f);
    UpdateUI(g_UI[UI_TYPE_BUTTON_A_OFF], EASE_OUT_QUINT, 1.0f / 60.0f);
    UpdateUI(g_UI[UI_TYPE_BUTTON_B_ON], EASE_OUT_QUINT, 1.0f / 60.0f);
    UpdateUI(g_UI[UI_TYPE_BUTTON_B_OFF], EASE_OUT_QUINT, 1.0f / 60.0f);
    UpdateUIPositionOnly(g_UI[UI_TYPE_DETAIL1], EASE_IN_OUT_QUAD, 1.0f / 15.0f);
    UpdateUIPositionOnly(g_UI[UI_TYPE_DETAIL2], EASE_IN_OUT_QUAD, 1.0f / 15.0f);
    UpdateUI(g_UI[UI_TYPE_CURSOR], EASE_OUT_ELASTIC, 1.0f / 60.0f);

    UpdateUI(g_UI[UI_TYPE_STUMP], EASE_IN_OUT_QUAD, 1.0f / 20.0f);
    
    UpdateUI(g_UI[UI_TYPE_STUMP_2], EASE_OUT_QUINT, 1.0f / 20.0f);

    {
        static float frameTime = 0.0f;
        const float frameDuration = 0.05f;

        frameTime += 1.0f / 60.0f;
        if (frameTime >= frameDuration)
        {
            frameTime = 0.0f;
            g_EffectFrame = (g_EffectFrame + 1) % 49;
        }
    }

}

// ===================================================
// ステージセレクトシーンの描画
// ===================================================
void DrawSceneStageSelect(void)
{
    //全部描画
    {
        DRAW_POLYGON_DAT drawDatTmp = {};
        drawDatTmp.pos = g_UI[UI_TYPE_BG_ALL].position;
        drawDatTmp.size = g_UI[UI_TYPE_BG_ALL].scale;
        drawDatTmp.rot = g_UI[UI_TYPE_BG_ALL].rotation;
        drawDatTmp.color = g_UI[UI_TYPE_BG_ALL].color;
        drawDatTmp.texNo = g_UI[UI_TYPE_BG_ALL].TextureData.textureId;
        drawDatTmp.loadTexType = LOADTEXTURETYPE_MAIN;
        drawDatTmp.texUV = MkF2(0.0f, 0.0f);
        drawDatTmp.texWH = MkF2(1.0f, 1.0f);
        drawDatTmp.startScale = MkF2(1.0f, 1.0f);
        drawDatTmp.startScale3D = MkF3(1.0f, 1.0f, 1.0f);
        drawDatTmp.shaderOpt.outline = DPD_SHADER_OPT_OUTLINE_NONE;
        DRAW_POLYGON_IN_CAMERA_EX_OPTION ropt = {};
        ropt.reverse = false;//反転
        ropt.fixed = false;//カメラの移動を無視する
        ropt.billboard = true;//ビルボード
        ropt.priority = DRAW_POLYGON_PRIORITY_FORCE_NORMAL;//レイヤ
        ropt.depthBuffIgnore = true;//深度バッファ
        //描画
        DrawPolygonInCameraEx(drawDatTmp, ropt);
    }
    // マップ
    {
        DRAW_POLYGON_DAT drawDatTmp = {};
        drawDatTmp.pos = g_UI[UI_TYPE_MAP].position;
        drawDatTmp.size = g_UI[UI_TYPE_MAP].scale;
        drawDatTmp.rot = g_UI[UI_TYPE_MAP].rotation;
        drawDatTmp.color = g_UI[UI_TYPE_MAP].color;
        drawDatTmp.texNo = g_UI[UI_TYPE_MAP].TextureData.textureId;
        drawDatTmp.loadTexType = LOADTEXTURETYPE_MAIN;
        drawDatTmp.texUV = MkF2(0.0f, 0.0f);
        drawDatTmp.texWH = MkF2(1.0f, 1.0f);
        drawDatTmp.startScale = MkF2(1.0f, 1.0f);
        drawDatTmp.startScale3D = MkF3(1.0f, 1.0f, 1.0f);
        drawDatTmp.shaderOpt.outline = DPD_SHADER_OPT_OUTLINE_NONE;
        DRAW_POLYGON_IN_CAMERA_EX_OPTION ropt = {};
        ropt.reverse = false;//反転
        ropt.fixed = false;//カメラの移動を無視する
        ropt.billboard = true;//ビルボード
        ropt.priority = DRAW_POLYGON_PRIORITY_FORCE_NORMAL;//レイヤ
        ropt.depthBuffIgnore = true;//深度バッファ
        //描画
        DrawPolygonInCameraEx(drawDatTmp, ropt);
    }
    // LIGHT
    {
        DRAW_POLYGON_DAT drawDatTmp = {};
        drawDatTmp.pos = g_UI[UI_TYPE_LIGHT].position;
        drawDatTmp.size = g_UI[UI_TYPE_LIGHT].scale;
        drawDatTmp.rot = g_UI[UI_TYPE_LIGHT].rotation;
        drawDatTmp.color = g_UI[UI_TYPE_LIGHT].color;
        drawDatTmp.texNo = g_UI[UI_TYPE_LIGHT].TextureData.textureId;
        drawDatTmp.loadTexType = LOADTEXTURETYPE_MAIN;
        drawDatTmp.texUV = MkF2(0.0f, 0.0f);
        drawDatTmp.texWH = MkF2(1.0f, 1.0f);
        drawDatTmp.startScale = MkF2(1.0f, 1.0f);
        drawDatTmp.startScale3D = MkF3(1.0f, 1.0f, 1.0f);
        drawDatTmp.shaderOpt.outline = DPD_SHADER_OPT_OUTLINE_NONE;
        DRAW_POLYGON_IN_CAMERA_EX_OPTION ropt = {};
        ropt.reverse = false;//反転
        ropt.fixed = false;//カメラの移動を無視する
        ropt.billboard = true;//ビルボード
        ropt.priority = DRAW_POLYGON_PRIORITY_FORCE_NORMAL;//レイヤ
        ropt.depthBuffIgnore = true;//深度バッファ
        //描画
        DrawPolygonInCameraEx(drawDatTmp, ropt);
    }
    //選択カーソル
    {
        DRAW_POLYGON_DAT drawDatTmp = {};
        drawDatTmp.pos = g_UI[UI_TYPE_CURSOR].position;
        drawDatTmp.size = g_UI[UI_TYPE_CURSOR].scale;
        drawDatTmp.rot = g_UI[UI_TYPE_CURSOR].rotation;
        drawDatTmp.color = g_UI[UI_TYPE_CURSOR].color;
        drawDatTmp.texNo = g_UI[UI_TYPE_CURSOR].TextureData.textureId;
        drawDatTmp.loadTexType = LOADTEXTURETYPE_MAIN;
        drawDatTmp.texUV = MkF2(0.0f, 0.0f);
        drawDatTmp.texWH = MkF2(1.0f, 1.0f);
        drawDatTmp.startScale = MkF2(1.0f, 1.0f);
        drawDatTmp.startScale3D = MkF3(1.0f, 1.0f, 1.0f);
        drawDatTmp.shaderOpt.outline = DPD_SHADER_OPT_OUTLINE_NONE;
        DRAW_POLYGON_IN_CAMERA_EX_OPTION ropt = {};
        ropt.reverse = false;//反転
        ropt.fixed = false;//カメラの移動を無視する
        ropt.billboard = true;//ビルボード
        ropt.priority = DRAW_POLYGON_PRIORITY_FORCE_NORMAL;//レイヤ
        ropt.depthBuffIgnore = true;//深度バッファ
        //描画
        DrawPolygonInCameraEx(drawDatTmp, ropt);
    }
    //エフェクト
    {
        int frameX = g_EffectFrame % 7;
        int frameY = g_EffectFrame / 7;
        float uvX = frameX * 1.0f / 7.0f;
        float uvY = frameY * 1.0f / 7.0f;

        DRAW_POLYGON_DAT drawDatTmp = {};
        drawDatTmp.pos = MkF3(-210.0f, 210.0f, 1.0f);
        drawDatTmp.size = MkF3(125.0f, 125.0f, 1.0f);
        drawDatTmp.rot = 0.0f;
        drawDatTmp.color = MkF4(1.0f, 1.0f, 1.0f, 1.0f);
        drawDatTmp.texNo = g_Effect.textureId;
        drawDatTmp.loadTexType = LOADTEXTURETYPE_MAIN;
        drawDatTmp.texUV = MkF2(uvX, uvY);
        drawDatTmp.texWH = MkF2(1.0f / 7.0f, 1.0f / 7.0f);
        drawDatTmp.startScale = MkF2(1.0, 1.0f);
        drawDatTmp.startScale3D = MkF3(1.0f, 1.0f, 1.0f);
        drawDatTmp.shaderOpt.outline = DPD_SHADER_OPT_OUTLINE_NONE;
        DRAW_POLYGON_IN_CAMERA_EX_OPTION ropt = {};
        ropt.reverse = false;//反転
        ropt.fixed = false;//カメラの移動を無視する
        ropt.billboard = true;//ビルボード
        ropt.priority = DRAW_POLYGON_PRIORITY_FORCE_NORMAL;//レイヤ
        ropt.depthBuffIgnore = true;//深度バッファ
        //描画
        DrawPolygonInCameraEx(drawDatTmp, ropt);
    }
    //ピン
    {
        DRAW_POLYGON_DAT drawDatTmp = {};
        drawDatTmp.pos = MkF3(-205.0f, 225.0f, 1.0f);
        drawDatTmp.size = MkF3(25.0f, 25.0f, 1.0f);
        drawDatTmp.rot = 0.0f;
        drawDatTmp.color = MkF4(1.0f, 1.0f, 1.0f, 1.0f);
        drawDatTmp.texNo = g_Pin.textureId;
        drawDatTmp.loadTexType = LOADTEXTURETYPE_MAIN;
        drawDatTmp.texUV = MkF2(0.0f, 0.0f);
        drawDatTmp.texWH = MkF2(1.0f, 1.0f);
        drawDatTmp.startScale = MkF2(1.0, 1.0f);
        drawDatTmp.startScale3D = MkF3(1.0f, 1.0f, 1.0f);
        drawDatTmp.shaderOpt.outline = DPD_SHADER_OPT_OUTLINE_NONE;
        DRAW_POLYGON_IN_CAMERA_EX_OPTION ropt = {};
        ropt.reverse = false;//反転
        ropt.fixed = false;//カメラの移動を無視する
        ropt.billboard = true;//ビルボード
        ropt.priority = DRAW_POLYGON_PRIORITY_FORCE_NORMAL;//レイヤ
        ropt.depthBuffIgnore = true;//深度バッファ
        //描画
        DrawPolygonInCameraEx(drawDatTmp, ropt);
    }
    //ステージないよ描画
    {
        DRAW_POLYGON_DAT drawDatTmp = {};
        drawDatTmp.pos = g_UI[UI_TYPE_NOSTAGE].position;
        drawDatTmp.size = g_UI[UI_TYPE_NOSTAGE].scale;
        drawDatTmp.rot = g_UI[UI_TYPE_NOSTAGE].rotation;
        drawDatTmp.color = g_UI[UI_TYPE_NOSTAGE].color;
        drawDatTmp.texNo = g_UI[UI_TYPE_NOSTAGE].TextureData.textureId;
        drawDatTmp.loadTexType = LOADTEXTURETYPE_MAIN;
        drawDatTmp.texUV = MkF2(0.0f, 0.0f);
        drawDatTmp.texWH = MkF2(1.0f, 1.0f);
        drawDatTmp.startScale = MkF2(1.0f, 1.0f);
        drawDatTmp.startScale3D = MkF3(1.0f, 1.0f, 1.0f);
        drawDatTmp.shaderOpt.outline = DPD_SHADER_OPT_OUTLINE_NONE;
        DRAW_POLYGON_IN_CAMERA_EX_OPTION ropt = {};
        ropt.reverse = false;//反転
        ropt.fixed = false;//カメラの移動を無視する
        ropt.billboard = true;//ビルボード
        ropt.priority = DRAW_POLYGON_PRIORITY_FORCE_ALWAYS;//レイヤ
        ropt.depthBuffIgnore = false;//深度バッファ
        //描画
        DrawPolygonInCameraEx(drawDatTmp, ropt);
    }
    //詳細説明の描画
    InDetailUI(g_StageSelect);

    //ボタンの表示
    {
        DRAW_POLYGON_DAT drawDatTmp = {};
        drawDatTmp.pos = g_UI[UI_TYPE_BUTTON_A_OFF].position;
        drawDatTmp.size = g_UI[UI_TYPE_BUTTON_A_OFF].scale;
        drawDatTmp.rot = g_UI[UI_TYPE_BUTTON_A_OFF].rotation;
        drawDatTmp.color = g_UI[UI_TYPE_BUTTON_A_OFF].color;
        drawDatTmp.texNo = g_UI[UI_TYPE_BUTTON_A_OFF].TextureData.textureId;
        drawDatTmp.loadTexType = LOADTEXTURETYPE_MAIN;
        drawDatTmp.texUV = MkF2(0.0f, 0.0f);
        drawDatTmp.texWH = MkF2(1.0f, 1.0f);
        drawDatTmp.startScale = MkF2(1.0f, 1.0f);
        drawDatTmp.startScale3D = MkF3(1.0f, 1.0f, 1.0f);
        drawDatTmp.shaderOpt.outline = DPD_SHADER_OPT_OUTLINE_NONE;
        DRAW_POLYGON_IN_CAMERA_EX_OPTION ropt = {};
        ropt.reverse = false;//反転
        ropt.fixed = false;//カメラの移動を無視する
        ropt.billboard = true;//ビルボード
        ropt.priority = DRAW_POLYGON_PRIORITY_FORCE_NORMAL;//レイヤ
        ropt.depthBuffIgnore = true;//深度バッファ
        //描画
        DrawPolygonInCameraEx(drawDatTmp, ropt);
    }
    {
        DRAW_POLYGON_DAT drawDatTmp = {};
        drawDatTmp.pos = g_UI[UI_TYPE_BUTTON_B_OFF].position;
        drawDatTmp.size = g_UI[UI_TYPE_BUTTON_B_OFF].scale;
        drawDatTmp.rot = g_UI[UI_TYPE_BUTTON_B_OFF].rotation;
        drawDatTmp.color = g_UI[UI_TYPE_BUTTON_B_OFF].color;
        drawDatTmp.texNo = g_UI[UI_TYPE_BUTTON_B_OFF].TextureData.textureId;
        drawDatTmp.loadTexType = LOADTEXTURETYPE_MAIN;
        drawDatTmp.texUV = MkF2(0.0f, 0.0f);
        drawDatTmp.texWH = MkF2(1.0f, 1.0f);
        drawDatTmp.startScale = MkF2(1.0f, 1.0f);
        drawDatTmp.startScale3D = MkF3(1.0f, 1.0f, 1.0f);
        drawDatTmp.shaderOpt.outline = DPD_SHADER_OPT_OUTLINE_NONE;
        DRAW_POLYGON_IN_CAMERA_EX_OPTION ropt = {};
        ropt.reverse = false;//反転
        ropt.fixed = false;//カメラの移動を無視する
        ropt.billboard = true;//ビルボード
        ropt.priority = DRAW_POLYGON_PRIORITY_FORCE_NORMAL;//レイヤ
        ropt.depthBuffIgnore = true;//深度バッファ
        DrawPolygonInCameraEx(drawDatTmp, ropt);
    }

    //ボタンの表示
    {
        DRAW_POLYGON_DAT drawDatTmp = {};
        drawDatTmp.pos = g_UI[UI_TYPE_BUTTON_A_ON].position;
        drawDatTmp.size = g_UI[UI_TYPE_BUTTON_A_ON].scale;
        drawDatTmp.rot = g_UI[UI_TYPE_BUTTON_A_ON].rotation;
        drawDatTmp.color = g_UI[UI_TYPE_BUTTON_A_ON].color;
        drawDatTmp.texNo = g_UI[UI_TYPE_BUTTON_A_ON].TextureData.textureId;
        drawDatTmp.loadTexType = LOADTEXTURETYPE_MAIN;
        drawDatTmp.texUV = MkF2(0.0f, 0.0f);
        drawDatTmp.texWH = MkF2(1.0f, 1.0f);
        drawDatTmp.startScale = MkF2(1.0f, 1.0f);
        drawDatTmp.startScale3D = MkF3(1.0f, 1.0f, 1.0f);
        drawDatTmp.shaderOpt.outline = DPD_SHADER_OPT_OUTLINE_NONE;
        DRAW_POLYGON_IN_CAMERA_EX_OPTION ropt = {};
        ropt.reverse = false;//反転
        ropt.fixed = false;//カメラの移動を無視する
        ropt.billboard = true;//ビルボード
        ropt.priority = DRAW_POLYGON_PRIORITY_FORCE_NORMAL;//レイヤ
        ropt.depthBuffIgnore = true;//深度バッファ
        //描画
        DrawPolygonInCameraEx(drawDatTmp, ropt);
    }
    {
        DRAW_POLYGON_DAT drawDatTmp = {};
        drawDatTmp.pos = g_UI[UI_TYPE_BUTTON_B_ON].position;
        drawDatTmp.size = g_UI[UI_TYPE_BUTTON_B_ON].scale;
        drawDatTmp.rot = g_UI[UI_TYPE_BUTTON_B_ON].rotation;
        drawDatTmp.color = g_UI[UI_TYPE_BUTTON_B_ON].color;
        drawDatTmp.texNo = g_UI[UI_TYPE_BUTTON_B_ON].TextureData.textureId;
        drawDatTmp.loadTexType = LOADTEXTURETYPE_MAIN;
        drawDatTmp.texUV = MkF2(0.0f, 0.0f);
        drawDatTmp.texWH = MkF2(1.0f, 1.0f);
        drawDatTmp.startScale = MkF2(1.0f, 1.0f);
        drawDatTmp.startScale3D = MkF3(1.0f, 1.0f, 1.0f);
        drawDatTmp.shaderOpt.outline = DPD_SHADER_OPT_OUTLINE_NONE;
        DRAW_POLYGON_IN_CAMERA_EX_OPTION ropt = {};
        ropt.reverse = false;//反転
        ropt.fixed = false;//カメラの移動を無視する
        ropt.billboard = true;//ビルボード
        ropt.priority = DRAW_POLYGON_PRIORITY_FORCE_NORMAL;//レイヤ
        ropt.depthBuffIgnore = true;//深度バッファ
        //描画
        DrawPolygonInCameraEx(drawDatTmp, ropt);
    }
    //スタンプ
    {
        DRAW_POLYGON_DAT drawDatTmp = {};
        drawDatTmp.pos = g_UI[UI_TYPE_STUMP].position;
        drawDatTmp.size = g_UI[UI_TYPE_STUMP].scale;
        drawDatTmp.rot = g_UI[UI_TYPE_STUMP].rotation;
        drawDatTmp.color = g_UI[UI_TYPE_STUMP].color;
        drawDatTmp.texNo = g_UI[UI_TYPE_STUMP].TextureData.textureId;
        drawDatTmp.loadTexType = LOADTEXTURETYPE_MAIN;
        drawDatTmp.texUV = MkF2(0.0f, 0.0f);
        drawDatTmp.texWH = MkF2(1.0f, 1.0f);
        drawDatTmp.startScale = MkF2(1.0f, 1.0f);
        drawDatTmp.startScale3D = MkF3(1.0f, 1.0f, 1.0f);
        drawDatTmp.shaderOpt.outline = DPD_SHADER_OPT_OUTLINE_NONE;
        DRAW_POLYGON_IN_CAMERA_EX_OPTION ropt = {};
        ropt.reverse = false;//反転
        ropt.fixed = false;//カメラの移動を無視する
        ropt.billboard = true;//ビルボード
        ropt.priority = DRAW_POLYGON_PRIORITY_FORCE_NORMAL;//レイヤ
        ropt.depthBuffIgnore = true;//深度バッファ
        //描画
        DrawPolygonInCameraEx(drawDatTmp, ropt);
    }
    //スタンプブラー
    {
        DRAW_POLYGON_DAT drawDatTmp = {};
        drawDatTmp.pos = g_UI[UI_TYPE_STUMP_2].position;
        drawDatTmp.size = g_UI[UI_TYPE_STUMP_2].scale;
        drawDatTmp.rot = g_UI[UI_TYPE_STUMP_2].rotation;
        drawDatTmp.color = g_UI[UI_TYPE_STUMP_2].color;
        drawDatTmp.texNo = g_UI[UI_TYPE_STUMP_2].TextureData.textureId;
        drawDatTmp.loadTexType = LOADTEXTURETYPE_MAIN;
        drawDatTmp.texUV = MkF2(0.0f, 0.0f);
        drawDatTmp.texWH = MkF2(1.0f, 1.0f);
        drawDatTmp.startScale = MkF2(1.0f, 1.0f);
        drawDatTmp.startScale3D = MkF3(1.0f, 1.0f, 1.0f);
        drawDatTmp.shaderOpt.outline = DPD_SHADER_OPT_OUTLINE_NONE;
        DRAW_POLYGON_IN_CAMERA_EX_OPTION ropt = {};
        ropt.reverse = false;//反転
        ropt.fixed = false;//カメラの移動を無視する
        ropt.billboard = true;//ビルボード
        ropt.priority = DRAW_POLYGON_PRIORITY_FORCE_NORMAL;//レイヤ
        ropt.depthBuffIgnore = true;//深度バッファ
        //描画
        DrawPolygonInCameraEx(drawDatTmp, ropt);
    }
}

// ===================================================
// ステージセレクトシーンの終了処理
// ===================================================
void FinalizeSceneStageSelect(void)
{
}

// ===================================================
// ステージのステートによって詳細情報を表示する処理
// ===================================================
void InDetailUI(STAGESELECTTYPE stageselect)
{
    if (g_StageSelectState == STAGESELECT_SCENE_STATE_ANIMATION_IN)
    {
        return;
    }
    switch (stageselect)
    {
    case STAGESELECT_TYPE_STAGE1:
    {
        DRAW_POLYGON_DAT drawDatTmp = {};
        drawDatTmp.pos = g_UI[UI_TYPE_DETAIL1].position;
        drawDatTmp.size = g_UI[UI_TYPE_DETAIL1].scale;
        drawDatTmp.rot = g_UI[UI_TYPE_DETAIL1].rotation;
        drawDatTmp.color = g_UI[UI_TYPE_DETAIL1].color;
        drawDatTmp.texNo = g_UI[UI_TYPE_DETAIL1].TextureData.textureId;
        drawDatTmp.loadTexType = LOADTEXTURETYPE_MAIN;
        drawDatTmp.texUV = MkF2(0.0f, 0.0f);
        drawDatTmp.texWH = MkF2(1.0f, 1.0f);
        drawDatTmp.startScale = MkF2(1.0f, 1.0f);
        drawDatTmp.startScale3D = MkF3(1.0f, 1.0f, 1.0f);
        drawDatTmp.shaderOpt.outline = DPD_SHADER_OPT_OUTLINE_NONE;
        DRAW_POLYGON_IN_CAMERA_EX_OPTION ropt = {};
        ropt.reverse = false;//反転
        ropt.fixed = false;//カメラの移動を無視する
        ropt.billboard = true;//ビルボード
        ropt.priority = DRAW_POLYGON_PRIORITY_FORCE_NORMAL;//レイヤ
        ropt.depthBuffIgnore = true;//深度バッファ
        //描画
        DrawPolygonInCameraEx(drawDatTmp, ropt);
    }
    {
        DRAW_POLYGON_DAT drawDatTmp = {};
        drawDatTmp.pos = g_UI[UI_TYPE_DETAIL2].position;
        drawDatTmp.size = g_UI[UI_TYPE_DETAIL2].scale;
        drawDatTmp.rot = g_UI[UI_TYPE_DETAIL2].rotation;
        drawDatTmp.color = g_UI[UI_TYPE_DETAIL2].color;
        drawDatTmp.texNo = g_UI[UI_TYPE_DETAIL2].TextureData.textureId;
        drawDatTmp.loadTexType = LOADTEXTURETYPE_MAIN;
        drawDatTmp.texUV = MkF2(0.0f, 0.0f);
        drawDatTmp.texWH = MkF2(1.0f, 1.0f);
        drawDatTmp.startScale = MkF2(1.0f, 1.0f);
        drawDatTmp.startScale3D = MkF3(1.0f, 1.0f, 1.0f);
        drawDatTmp.shaderOpt.outline = DPD_SHADER_OPT_OUTLINE_NONE;
        DRAW_POLYGON_IN_CAMERA_EX_OPTION ropt = {};
        ropt.reverse = false;//反転
        ropt.fixed = false;//カメラの移動を無視する
        ropt.billboard = true;//ビルボード
        ropt.priority = DRAW_POLYGON_PRIORITY_FORCE_NORMAL;//レイヤ
        ropt.depthBuffIgnore = true;//深度バッファ
        //描画
        DrawPolygonInCameraEx(drawDatTmp, ropt);
    }
    break;
    case STAGESELECT_TYPE_STAGE2:
    {

    }
    break;
    default:
        break;
    }

}


// ===================================================
// コントローラ入力処理
// ===================================================
void Update_StageSelectType(void)
{
    switch (g_SelectState)
    {
    case STAGESELECT_STATE_DEFAULT:
        Update_Default();
        break;
    case STAGESELECT_STATE_DETAIL_ON:
        Update_Detail();
        break;
    default:
        break;
    }
}
// ===================================================
// ステージを選んでいるシーン
// ===================================================
void Update_Default(void)
{
    CONTROLLER_STICK_DAT stickTrigger_Dat = GetControllerMainLeftStickTrigger();

    //Lスティックの入力を受け取る
    stickTrigger_RX = false;
    stickTrigger_LX = false;
    stickTrigger_UY = false;
    stickTrigger_DY = false;

    if (stickTrigger_Dat.enable)
    {
        if (stickTrigger_Dat.stick.x > 0.4f)
        {
            stickTrigger_RX = true;
        }
        if (stickTrigger_Dat.stick.y > 0.4f)
        {
            stickTrigger_UY = true;
        }
        if (stickTrigger_Dat.stick.x < -0.4f)
        {
            stickTrigger_LX = true;
        }
        if (stickTrigger_Dat.stick.y < -0.4f)
        {
            stickTrigger_DY = true;
        }
    }


    //右入力処理
    if (GetControllerMainTrigger(CONTROLLER_BUTTON_TYPE_RIGHT) || stickTrigger_RX)
    {
        g_StageSelect = (g_StageSelect == STAGESELECT_TYPE_STAGE1) ? STAGESELECT_TYPE_STAGE2 : STAGESELECT_TYPE_STAGE1;
        if (g_StageSelect == STAGESELECT_TYPE_STAGE2)
        {
            g_UI[UI_TYPE_NOSTAGE].color = MkF4(1.0f, 1.0f, 1.0f, 1.0f);
            StartUIAnimationColorOnly(g_UI[UI_TYPE_NOSTAGE], { 1.0f,1.0f,1.0f,0.0f });
            SoundEffectPlayRequest(SOUND_TYPE_SE_UI_SELECT_SCENE_ENTER2, 4.0f, 0);//ブブッみたいな効果音を入れる
            CameraShakeRequestAll(CAMERA_SHAKE_MIDIUM_FRAME, CAMERA_SHAKE_SMALL_POWER);
            g_StageSelect = STAGESELECT_TYPE_STAGE1;
            printf("現在のステート g_StageSelect = %d\n", g_StageSelect);

            return;
        }
    }
    //左入力処理
    else if (GetControllerMainTrigger(CONTROLLER_BUTTON_TYPE_LEFT) || stickTrigger_LX)
    {
        g_StageSelect = (g_StageSelect == STAGESELECT_TYPE_STAGE1) ? STAGESELECT_TYPE_STAGE2 : STAGESELECT_TYPE_STAGE1;
        if (g_StageSelect == STAGESELECT_TYPE_STAGE2)
        {
            g_UI[UI_TYPE_NOSTAGE].color = MkF4(1.0f, 1.0f, 1.0f, 1.0f);
            StartUIAnimationColorOnly(g_UI[UI_TYPE_NOSTAGE], { 1.0f,1.0f,1.0f,0.0f });
            SoundEffectPlayRequest(SOUND_TYPE_SE_UI_SELECT_SCENE_ENTER2, 4.0f, 0);//ブブッみたいな効果音を入れる
            CameraShakeRequestAll(CAMERA_SHAKE_MIDIUM_FRAME, CAMERA_SHAKE_SMALL_POWER);

            g_StageSelect = STAGESELECT_TYPE_STAGE1;
            printf("現在のステート g_StageSelect = %d\n", g_StageSelect);

            return;
        }
    }

    if (g_Stamp)
    {
        return;
    }
    //ステージを選択
    if (GetControllerMainTrigger(CONTROLLER_BUTTON_TYPE_A)
        && g_SelectState != STAGESELECT_STATE_DETAIL_ON) {
        SoundEffectPlayRequest(SOUND_TYPE_SE_UI_SELECT_SCENE_ENTER2, 4.0f, 0);
        if (g_StageSelect == STAGESELECT_TYPE_STAGE2)
        {
            //準備中なのでリターン
            return;
        }
        g_UI[UI_TYPE_BUTTON_A_ON].color = MkF4(1.0f, 1.0f, 1.0f, 1.0f);
        g_UI[UI_TYPE_CURSOR].scale = MkF3(735.0f * 0.35f, 726.0f * 0.35f, 1.0f);
        g_UI[UI_TYPE_CURSOR].color = MkF4(2.0f, 2.0f, 2.0f, 1.0f);
        g_UI[UI_TYPE_DETAIL1].color.w = 1.0f;
        g_UI[UI_TYPE_DETAIL2].color.w = 1.0f;
        StartUIAnimationPositionOnly(g_UI[UI_TYPE_DETAIL1], { -300.0f,0.0f,0.0f });
        StartUIAnimationPositionOnly(g_UI[UI_TYPE_DETAIL2], { 300.0f,0.0f,0.0f });
        StartUIAnimationColorOnly(g_UI[UI_TYPE_BG_ALL], { 0.5f,0.5f,0.5f,1.0f });
        StartUIAnimationColorOnly(g_UI[UI_TYPE_MAP], { 0.5f,0.5f,0.5f,1.0f });
        StartUIAnimationColorOnly(g_UI[UI_TYPE_LIGHT], { 0.5f,0.5f,0.5f,1.0f });
        StartUIAnimationColorOnly(g_UI[UI_TYPE_BUTTON_A_ON], { 1.0f,1.0f,1.0f, 0.0f });
        StartUIAnimation(g_UI[UI_TYPE_CURSOR], { -277.5f, 5.0f, 0.0 }, { 697.0f * 0.59f, 641.0f * 0.59f, 1.0f }, 0.0f, { 1.0f,1.0f,1.0f,1.0f });
        g_SelectState = STAGESELECT_STATE_DETAIL_ON;
    }
    if (GetControllerMainTrigger(CONTROLLER_BUTTON_TYPE_B)
        && g_StageSelectState != STAGESELECT_SCENE_STATE_ANIMATION_OUT_TO_TITLE) {
        SoundEffectPlayRequest(SOUND_TYPE_SE_UI_SELECT_SCENE_EXIT, 4.0f, 0);
        g_UI[UI_TYPE_BUTTON_B_ON].color = MkF4(1.0f, 1.0f, 1.0f, 1.0f);
        StartUIAnimationColorOnly(g_UI[UI_TYPE_BUTTON_B_ON], { 1.0f,1.0f,1.0f,0.0f });
        g_StageSelectState = STAGESELECT_SCENE_STATE_ANIMATION_OUT_TO_TITLE;
    }
}
// ===================================================
// ステージ詳細を表示するシーン
// ===================================================
void Update_Detail(void)
{
    g_UI[UI_TYPE_NOSTAGE].color = MkF4(1.0f, 1.0f, 1.0f, 0.0f);
    //次のシーンへ
    if (GetControllerMainTrigger(CONTROLLER_BUTTON_TYPE_A)
        && !g_Stamp)
    {
        g_UI[UI_TYPE_BUTTON_A_ON].color = MkF4(1.0f, 1.0f, 1.0f, 1.0f);
        StartUIAnimationColorOnly(g_UI[UI_TYPE_BUTTON_A_ON], { 1.0f,1.0f,1.0f, 0.0f });
        StartUIAnimation(g_UI[UI_TYPE_STUMP], { 280.0f, 180.0f, 0.0f }, { 388.0f * 0.5f, 213.0f * 0.5f, 1.0f }, 0.0f, { 1.0f,1.0f,1.0f,1.0f });
        SoundEffectPlayRequest(SOUND_TYPE_SE_UI_SELECT_SCENE_STAMP, 4.0f, 15);
        SoundEffectPlayRequest(SOUND_TYPE_SE_UI_SELECT_SCENE_ENTER2, 4.0f, 0);
        printf("選択されたステージ番号は%dです\n", g_StageSelect);
        g_Stamp = true;
    }
    //詳細表示を閉じる
    if (GetControllerMainTrigger(CONTROLLER_BUTTON_TYPE_B)
        && g_SelectState != STAGESELECT_STATE_DEFAULT
        && !g_Stamp)
    {
        g_UI[UI_TYPE_BUTTON_B_ON].color = MkF4(1.0f, 1.0f, 1.0f, 1.0f);
        StartUIAnimationColorOnly(g_UI[UI_TYPE_BUTTON_B_ON], { 1.0f,1.0f,1.0f,0.0f });
        StartUIAnimationPositionOnly(g_UI[UI_TYPE_DETAIL1], { 0.0f,600.0f,600.0f });
        StartUIAnimationPositionOnly(g_UI[UI_TYPE_DETAIL2], { 0.0f,600.0f,600.0f });
        StartUIAnimationColorOnly(g_UI[UI_TYPE_BG_ALL], { 1.0f,1.0f,1.0f,1.0f });
        StartUIAnimationColorOnly(g_UI[UI_TYPE_MAP], { 1.0f,1.0f,1.0f,1.0f });
        StartUIAnimationColorOnly(g_UI[UI_TYPE_LIGHT], { 1.0f,1.0f,1.0f,1.0f });
        SoundEffectPlayRequest(SOUND_TYPE_SE_UI_SELECT_SCENE_EXIT, 4.0f, 0);
        g_SelectState = STAGESELECT_STATE_DEFAULT;

    }
}



// ===================================================
// UIのアニメーション関数
// ===================================================
void UpdateUI(UI_PARAMETAR& ui, EASE_TYPE type, float deltaTime)
{
    if (ui.animation.isComplete) return;

    //アニメーションを進める
    ui.animation.PlayForward(deltaTime);
    float t = ui.animation.current;

    t = SetEasing(type, ui.animation.current);

    //線形補間で状態の更新
    ui.position = Lerp(ui.oldPosition, ui.targetPosition, t);
    ui.rotation = Lerp(ui.oldRotation, ui.targetRotation, t);
    ui.scale = Lerp(ui.oldScale, ui.targetScale, t);
    ui.color = Lerp(ui.oldColor, ui.targetColor, t);
}

void UpdateUIPositionOnly(UI_PARAMETAR& ui, EASE_TYPE type, float deltaTime)
{
    if (ui.animation.isComplete) return;

    //アニメーションを進める
    ui.animation.PlayForward(deltaTime);
    float t = ui.animation.current;

    t = SetEasing(type, ui.animation.current);

    //線形補間で状態の更新
    ui.position = Lerp(ui.oldPosition, ui.targetPosition, t);
    //ui.rotation = Lerp(ui.oldRotation, ui.targetRotation, t);
    //ui.scale = Lerp(ui.oldScale, ui.targetScale, t);
    //ui.color = Lerp(ui.oldColor, ui.targetColor, t);

    printf("t = %f, position = (%f, %f, %f)\n", t, ui.position.x, ui.position.y, ui.position.z);
}

void UpdateUIScaleOnly(UI_PARAMETAR& ui, EASE_TYPE type, float deltaTime)
{
    if (ui.animation.isComplete) return;

    //アニメーションを進める
    ui.animation.PlayForward(deltaTime);
    float t = ui.animation.current;

    t = SetEasing(type, ui.animation.current);

    //線形補間で状態の更新
    //ui.position = Lerp(ui.oldPosition, ui.targetPosition, t);
    //ui.rotation = Lerp(ui.oldRotation, ui.targetRotation, t);
    ui.scale = Lerp(ui.oldScale, ui.targetScale, t);
    //ui.color = Lerp(ui.oldColor, ui.targetColor, t);

    printf("t = %f, color = (%f, %f, %f, %f)\n", t, ui.color.x, ui.color.y, ui.color.z, ui.color.w);
}

void UpdateUIRotationOnly(UI_PARAMETAR& ui, EASE_TYPE type, float deltaTime)
{
    if (ui.animation.isComplete) return;

    //アニメーションを進める
    ui.animation.PlayForward(deltaTime);
    float t = ui.animation.current;

    t = SetEasing(type, ui.animation.current);

    //線形補間で状態の更新
    //ui.position = Lerp(ui.oldPosition, ui.targetPosition, t);
    ui.rotation = Lerp(ui.oldRotation, ui.targetRotation, t);
    //ui.scale = Lerp(ui.oldScale, ui.targetScale, t);
    //ui.color = Lerp(ui.oldColor, ui.targetColor, t);
}

void UpdateUIColorOnly(UI_PARAMETAR& ui, EASE_TYPE type, float deltaTime)
{
    if (ui.animation.isComplete) return;

    //アニメーションを進める
    ui.animation.PlayForward(deltaTime);
    float t = ui.animation.current;

    t = SetEasing(type, ui.animation.current);

    //線形補間で状態の更新
    //ui.position = Lerp(ui.oldPosition, ui.targetPosition, t);
    //ui.rotation = Lerp(ui.oldRotation, ui.targetRotation, t);
    //ui.scale = Lerp(ui.oldScale, ui.targetScale, t);
    ui.color = Lerp(ui.oldColor, ui.targetColor, t);

    printf("t = %f, color = (%f, %f, %f, %f)\n", t, ui.color.x, ui.color.y, ui.color.z, ui.color.w);
}

void StartUIAnimation(UI_PARAMETAR& ui, Float3 newPosition, Float3 newScale, float newRotation, Float4 newColor)
{

    // 補間の開始点を現在の値に設定
    ui.oldPosition = ui.position;
    ui.oldRotation = ui.rotation;
    ui.oldScale = ui.scale;
    ui.oldColor = ui.color;

    // 目標値を設定
    ui.targetPosition = newPosition;
    ui.targetRotation = newRotation;
    ui.targetScale = newScale;
    ui.targetColor = newColor;

    // アニメーション開始
    ui.animation.Reset();
}

void StartUIAnimationPositionOnly(UI_PARAMETAR& ui, Float3 newPosition)
{
    // 位置だけ新しい値を設定し、それ以外は維持
    ui.oldPosition = ui.position;
    ui.targetPosition = newPosition;

    ui.oldScale = ui.scale;
    ui.targetScale = ui.scale; // スケールは変えない

    ui.oldRotation = ui.rotation;
    ui.targetRotation = ui.rotation; // 回転は変えない

    ui.oldColor = ui.color;
    ui.targetColor = ui.color; // 色は変えない

    // アニメーション開始
    ui.animation.Reset();
}

void StartUIAnimationScaleOnly(UI_PARAMETAR& ui, Float3 newScale)
{
    ui.oldPosition = ui.position;
    ui.targetPosition = ui.position;//位置は変えない

    ui.oldScale = ui.scale;
    ui.targetScale = newScale;

    ui.oldRotation = ui.rotation;
    ui.targetRotation = ui.rotation; // 回転は変えない

    ui.oldColor = ui.color;
    ui.targetColor = ui.color; // 色は変えない

    // アニメーション開始
    ui.animation.Reset();
}

void StartUIAnimationRotationOnly(UI_PARAMETAR& ui, float newRotation)
{
    ui.oldPosition = ui.position;
    ui.targetPosition = ui.position;//位置は変えない

    ui.oldScale = ui.scale;
    ui.targetScale = ui.scale; // スケールは変えない

    ui.oldRotation = ui.rotation;
    ui.targetRotation = newRotation;

    ui.oldColor = ui.color;
    ui.targetColor = ui.color; // 色は変えない

    // アニメーション開始
    ui.animation.Reset();
}

void StartUIAnimationColorOnly(UI_PARAMETAR& ui, Float4 newColor)
{
    ui.oldPosition = ui.position;
    ui.targetPosition = ui.position;//位置は変えない

    ui.oldScale = ui.scale;
    ui.targetScale = ui.scale; // スケールは変えない

    ui.oldRotation = ui.rotation;
    ui.targetRotation = ui.rotation; // 回転は変えない

    ui.oldColor = ui.color;
    ui.targetColor = newColor;

    // アニメーション開始
    ui.animation.Reset();
}

float Lerp(float a, float b, float t)
{
    return a + (b - a) * t;
}

Float3 Lerp(Float3 a, Float3 b, float t)
{
    return { Lerp(a.x,b.x,t),Lerp(a.y,b.y,t),Lerp(a.z,b.z,t) };
}

Float4 Lerp(Float4 a, Float4 b, float t)
{
    return { Lerp(a.x, b.x, t), Lerp(a.y, b.y, t), Lerp(a.z, b.z, t), Lerp(a.w, b.w, t) };
}
