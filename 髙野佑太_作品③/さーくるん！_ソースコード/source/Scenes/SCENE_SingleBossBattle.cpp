#pragma once
// ===================================================
// SingleBossBattle.cpp タイトルシーン制御
// 
// Takano Yuta  2025/01/17
// ===================================================
#include "../Game/Systems/sound.h"
#include "../Core/DirectX/Audio.h"
#include "../game_main.h"

#include "SCENE_SingleBossBattle.h"
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
#include "../Game/Systems/font.h"

#include "../Game/Systems/easing.h"


struct BOSSSET {
    std::wstring stagename;
    std::wstring name;
    int bossNum;
    bool isHard;
};

const BOSSSET g_BossSet[] = {
    { L"コ・ダイ遺跡 ノーマル",  L"統制型インペリウス", 0, },
    { L"コ・ダイ遺跡 ハード",    L"強襲型コントリウス", 1, true,},
    { L"コ・ダイ遺跡 ハード",    L"強襲型インペリウス", 0, true,},
    { L"EX",                     L"破壊型コントリウス", 1, },
};

static bool g_InitLoad = false;

static int g_nowselect = 0;
static float g_nowselectlerp = 0.0f;

static float g_fadeTime = 0.0f;
static SCENE g_exitflag = SCENE_SINGLEBOSSBATTLE;

// ===================================================
// ステージセレクトシーンの初期化
// ===================================================
void InitializeSceneSingleBossBattle(void)
{
    g_fadeTime = 0.0f;
    g_exitflag = SCENE_SINGLEBOSSBATTLE;

    if (!g_InitLoad)
    {
        g_InitLoad = true;
    }

    UIReset();
    //
    GameManagerSetSceneRequestCancel();

    //SoundBGMStopRequest();

    ResetCamera();

    //SetBg(BG_TYPE_NONE);
    SetBg(BG_TYPE_TUTORIAL_1);
}

// ===================================================
// ステージセレクトシーンの更新
// ===================================================
void UpdateSceneSingleBossBattle(void)
{
    if (g_nowselectlerp > (float)g_nowselect)
    {
        g_nowselectlerp += std::max(((float)g_nowselect - g_nowselectlerp) / 5.0f, -1.0f / 3.0f);
        if (g_nowselectlerp <= (float)g_nowselect)
        {
            g_nowselectlerp = (float)g_nowselect;
        }
    }
    else if (g_nowselectlerp < (float)g_nowselect)
    {
        g_nowselectlerp += std::min(((float)g_nowselect - g_nowselectlerp) / 5.0f, 1.0f / 3.0f);
        if (g_nowselectlerp >= (float)g_nowselect)
        {
            g_nowselectlerp = (float)g_nowselect;
        }
    }

    if (g_exitflag != SCENE_SINGLEBOSSBATTLE)
    {
        if (g_fadeTime > 0.0f)
        {
            g_fadeTime += -1.0f / 20;
            if (g_fadeTime <= 0.0f)
            {
                SetSceneFade(g_exitflag, FADE_TYPE_STAR_WIPE);
            }
        }

        return;
    }
    else
    {
        if (!IsFading() && g_fadeTime < 1.0f)
        {
            g_fadeTime += 1.0f / 20;
            if (g_fadeTime >= 1.0f)
            {
                g_fadeTime = 1.0f;
            }

            return;
        }
    }

    if (IsFading())
    {
        return;
    }

    // シーン遷移(デバッグテスト用)
    if (Keyboard_IsKeyTrigger(KK_DELETE))
    {
        SetSceneFade(SCENE_TITLE);
    }


    {
        CONTROLLER_STICK_DAT stickTrigger_Dat = GetControllerMainLeftStickTrigger();

        bool stickTrigger_positive = false;
        bool stickTrigger_negative = false;

        //Lスティックの入力を受け取る
        if (stickTrigger_Dat.enable)
        {
            if (stickTrigger_Dat.stick.y > 0.4f)
            {
                stickTrigger_positive = true;
            }
            if (stickTrigger_Dat.stick.y < -0.4f)
            {
                stickTrigger_negative = true;
            }
        }

        if ((GetControllerMainTrigger(CONTROLLER_BUTTON_TYPE_UP) || stickTrigger_positive) && !IsFading())
        {
            SoundEffectPlayRequest(SOUND_TYPE_SE_UI_SELECT, 4.0f, 0.0f);

            g_nowselect--;
            g_nowselect = g_nowselect % ARRAYSIZE(g_BossSet); // 範囲外に出ないようにする
        }

        if ((GetControllerMainTrigger(CONTROLLER_BUTTON_TYPE_DOWN) || stickTrigger_negative) && !IsFading())
        {
            SoundEffectPlayRequest(SOUND_TYPE_SE_UI_SELECT, 4.0f, 0.0f);

            g_nowselect++;
            g_nowselect = g_nowselect % ARRAYSIZE(g_BossSet); // 範囲外に出ないようにする
        }
    }

    if (GetControllerMainTrigger(CONTROLLER_BUTTON_TYPE_B))
    {
        SoundEffectPlayRequest(SOUND_TYPE_SE_UI_CANCEL, 4.0f, 0.0f);

        g_exitflag = SCENE_TITLE;
    }
    else if (GetControllerMainTrigger(CONTROLLER_BUTTON_TYPE_A))
    {
        SoundEffectPlayRequest(SOUND_TYPE_SE_UI_OK, 4.0f, 0.0f);

        const BOSSSET& bossSet = g_BossSet[g_nowselect];
        SetDifficultySceneTitle(
            bossSet.isHard ?
            Difficulty::Hard 
            : Difficulty::Normal
        );

        //ボス戦開始処理
        g_exitflag = SCENE_CHARACTERSELECT;
    }

}

// ===================================================
// ステージセレクトシーンの描画
// ===================================================
void DrawSceneSingleBossBattle(void)
{
    float fadeMod = g_fadeTime / 1.0f;
    float fadeAddPosX = EasingInCirc(1.0f - fadeMod) * SCREEN_WIDTH;
    float fadeAddPosY = EasingInCirc(1.0f - fadeMod) * -SCREEN_HEIGHT;
    float cursorAddScale = +0.05f * sin((float)GetMainGameFrame() / 20.0f);
    Float4 colorBase = { 1.0f, 1.0f, 1.0f, 1.0f };

    {
        DrawSpriteColorQuad(
            800.0f + fadeAddPosX,
            480.0f,
            GetTexData(TEX_TYPE_TITLE_BUTTON_A).width,
            GetTexData(TEX_TYPE_TITLE_BUTTON_A).height,
            GetTexData(TEX_TYPE_TITLE_BUTTON_A).textureId,
            &colorBase,
            DRAW_POLYGON_PRIORITY_FORCE_DEEPEST);

    }
    {
        DrawSpriteColorQuad(
            600.0f + fadeAddPosX,
            480.0f,
            GetTexData(TEX_TYPE_TITLE_BUTTON_B).width,
            GetTexData(TEX_TYPE_TITLE_BUTTON_B).height,
            GetTexData(TEX_TYPE_TITLE_BUTTON_B).textureId,
            &colorBase,
            DRAW_POLYGON_PRIORITY_FORCE_DEEPEST);

    }

    {
        DrawSpriteColorQuad(
            0.0f,
            -50.0f + fadeAddPosY,
            GetTexData(TEX_TYPE_TITLE_UI_PANEL_Z).width ,
            GetTexData(TEX_TYPE_TITLE_UI_PANEL_Z).height,
            GetTexData(TEX_TYPE_TITLE_UI_PANEL_Z).textureId,
            &colorBase,
            DRAW_POLYGON_PRIORITY_FORCE_DEEPEST);
        
    }
    {
        DrawSpriteColorQuad(
            0.0f,
            -270.0f + 140.0f * g_nowselectlerp + fadeAddPosY,
            GetTexData(TEX_TYPE_TITLE_BUTTON_Z2).width  * (1.0f + cursorAddScale),
            GetTexData(TEX_TYPE_TITLE_BUTTON_Z2).height * (1.0f + cursorAddScale),
            GetTexData(TEX_TYPE_TITLE_BUTTON_Z2).textureId,
            &colorBase,
            DRAW_POLYGON_PRIORITY_FORCE_DEEPEST);

    }

    
    {
        
        for (int i = 0; i < ARRAYSIZE(g_BossSet); i++)
        {
            DRAW_FONT_DAT dat = {};
            dat.txt = g_BossSet[i].name;

            dat.fonttype = FONT_TYPE_STICK;
            dat.pos = MkF2(
                -300.0f,
                -290.0f + 140.0f * i + fadeAddPosY
            );
            dat.fontSize = 60;
            dat.posShadow = MkF2(
                (float)dat.fontSize / 13.0f,
                (float)dat.fontSize / 13.0f
            );
            //dat.color = FONT_TEXT_COLOR_WHITE;
            //dat.colorShadow = FONT_TEXT_COLOR_BLACK;
            dat.color = FONT_TEXT_COLOR_BLACK;
            dat.colorShadow = FONT_TEXT_COLOR_WHITE;
            dat.textAlign = FONT_TEXT_ALIGNMENT_LEFT;
            dat.LineSpacingWidth = dat.fontSize;
            dat.LineSpacingHight = dat.fontSize;

            DrawFont(dat);

            dat.txt = g_BossSet[i].stagename;

            //if (g_BossSet[i].isHard)
            //{
            //    dat.color = FONT_TEXT_COLOR_RED;
            //}
            //else
            //{
            //    dat.color = FONT_TEXT_COLOR_BLUE;
            //}
            dat.color = FONT_TEXT_COLOR_BLACK;
            dat.colorShadow = FONT_TEXT_COLOR_ORANGE;
            dat.fonttype = FONT_TYPE_STICK;
            dat.pos.x += -10.0f;
            dat.pos.y += -60.0f;
            dat.fontSize = 40;
            dat.posShadow = MkF2(
                (float)dat.fontSize / 13.0f,
                (float)dat.fontSize / 13.0f
            );
            DrawFont(dat);
        }

    }

}

// ===================================================
// ステージセレクトシーンの終了処理
// ===================================================
void FinalizeSceneSingleBossBattle(void)
{
    //SoundBGMStopRequest();
    //SoundBGMRequestStopAllRequest();
    //SoundEffectStopAllRequest();

    SetMainGameDrawFPS(1);

}

int GetSelectNumberSingleBossBattle(void)
{
    const BOSSSET& bossSet = g_BossSet[g_nowselect];
    return bossSet.bossNum;
}
