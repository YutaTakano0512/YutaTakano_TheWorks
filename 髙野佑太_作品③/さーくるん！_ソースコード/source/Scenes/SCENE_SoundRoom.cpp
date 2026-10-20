#pragma once
// ===================================================
// SCENE_SoundRoom.cpp タイトルシーン制御
// 
// Takano Yuta  2025/01/17
// ===================================================
#include "../Game/Systems/sound.h"
#include "../Core/DirectX/Audio.h"
#include "../game_main.h"

#include "SCENE_SoundRoom.h"
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


struct BGMSSET {
    std::wstring name;

    std::string intro;
    int introLen;
    std::string loop;
    float volume;
};
struct PLAYINGBGMSSET {
    bool isPlaying;
    const BGMSSET* bgmSet;
    SOUND_ID introID;
    SOUND_ID loopID;
    const std::vector<float>* introFFTData;
    const std::vector<float>* loopFFTData;
    int introFrame;
    int loopFrame;
};

const BGMSSET g_BGMSet[] = {
    { L"侵略と過去の栄光", "",                             0,                       "SOUND/BGM/ISEKI_LONG",          2.0f,  },
    { L"ENTITAS-IGNOTA",   "SOUND/BGM/ISEKI_BOSS_OLD_IN",  FRAME_ONE_SECOND * 5.95, "SOUND/BGM/ISEKI_BOSS_OLD_LOOP", 2.0f,  },
    { L"TRINITAS-MACHINA", "SOUND/BGM/ISEKI_BOSS_IN",      FRAME_ONE_SECOND * 5.55, "SOUND/BGM/ISEKI_BOSS_LOOP",     2.0f,  },
    { L"任務完了！",       "SOUND/BGM/Result_BGM_Fanfare", FRAME_ONE_SECOND * 6.5,  "SOUND/BGM/Result_BGM_Loop",     1.5f,  },
};

static PLAYINGBGMSSET g_PlayingBgmSet = {};
static bool g_InitLoad = false;

static int g_nowselect = 0;
static float g_nowselectlerp = 0.0f;

static float g_fadeTime = 0.0f;
static bool g_exitflag = false;

void _SoundRoom_PlayBGM(int index)
{
    g_PlayingBgmSet.isPlaying = true;
    g_PlayingBgmSet.bgmSet = g_BGMSet + index;
    g_PlayingBgmSet.introID = GetSoundData(g_PlayingBgmSet.bgmSet->intro);
    g_PlayingBgmSet.loopID = GetSoundData(g_PlayingBgmSet.bgmSet->loop);
    g_PlayingBgmSet.introFFTData = GetAudioFFTData(g_PlayingBgmSet.introID);
    g_PlayingBgmSet.loopFFTData = GetAudioFFTData(g_PlayingBgmSet.loopID);
    g_PlayingBgmSet.introFrame = 0;
    g_PlayingBgmSet.loopFrame = 0;

    SoundBGMStopRequest();
    SoundBGMRequestStopAllRequest();
    SoundEffectStopAllRequest();
    //INTROを再生
    SoundBGMIntroPlayRequest(g_PlayingBgmSet.introID,
        g_PlayingBgmSet.bgmSet->volume, 0);
    //BGMを再生する
    SoundBGMPlayRequest(g_PlayingBgmSet.loopID,
        true,
        g_PlayingBgmSet.bgmSet->volume,
        g_PlayingBgmSet.bgmSet->introLen,
        0.0f
    );
}

void _SoundRoom_StopBGM()
{
    SoundBGMStopRequest();
    SoundBGMRequestStopAllRequest();
    SoundEffectStopAllRequest();

    g_PlayingBgmSet.isPlaying = false;
}   


// ===================================================
// ステージセレクトシーンの初期化
// ===================================================
void InitializeSceneSoundRoom(void)
{
    g_PlayingBgmSet = {};
    g_fadeTime = 0.0f;
    g_exitflag = false;

    if (!g_InitLoad)
    {
        g_InitLoad = true;

        //for (int i = 0; i < ARRAYSIZE(g_BGMSet); i++)
        //{
        //    SOUND_ID soundid_intro = GetSoundData(g_BGMSet[i].intro);
        //    SOUND_ID soundid_loop = GetSoundData(g_BGMSet[i].loop);
        //    LoadAudioFFTData(soundid_intro);
        //    LoadAudioFFTData(soundid_loop);
        //}
    }

    //SetMainGameDrawFPS(2);

    UIReset();
    //
    GameManagerSetSceneRequestCancel();

    //{
    //    SOUND_ID bgm = GetSoundData("SOUND/BGM/Test_BGM_Title2");
    //    if (bgm != GetSoundBGMId())
    //    {
    //        //異なるBGMが流れている、
    //        //またはBGMが流れていない場合再生する。
    //        SoundBGMPlayRequest(bgm,
    //            (FRAME_ONE_MINUTE * 1)
    //            + (FRAME_ONE_SECOND * 30),
    //            1.0f);
    //    }
    //}

    SoundBGMStopRequest();

    ResetCamera();

    //SetBg(BG_TYPE_NONE);
    SetBg(BG_TYPE_TUTORIAL_1);

    //CameraMoveRequestAll(FRAME_ONE_SECOND * 1, MkF2(0.0f, 0.0f), 0.0f, MkF2(1.0f, 1.0f), 1.75f, EasingInOutSine, CAMERA_MOVE_TYPE_SMOOTH);

    //デバッグログ
    //AllocConsole(); // コンソールを作成
    //FILE* a =  freopen("CONOUT$", "w", stdout); // stdoutをコンソールにリダイレクト
    //printf("Hello, World!\n"); // コンソールに出力
}

// ===================================================
// ステージセレクトシーンの更新
// ===================================================
void UpdateSceneSoundRoom(void)
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

    if (g_exitflag)
    {
        if (g_fadeTime > 0.0f)
        {
            g_fadeTime += -1.0f / 20;
            if (g_fadeTime <= 0.0f)
            {
                SetSceneFade(SCENE_TITLE, FADE_TYPE_STAR_WIPE);
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

    //デバッグテスト用
    if (Keyboard_IsKeyTrigger(KK_ENTER))
    {
        int index = 0;
        if (Keyboard_IsKeyDown(KK_D2)) index = 1;
        if (Keyboard_IsKeyDown(KK_D3)) index = 2;
        if (Keyboard_IsKeyDown(KK_D4)) index = 3;

        _SoundRoom_PlayBGM(index);
    }
    //デバッグテスト用
    if (Keyboard_IsKeyTrigger(KK_BACK))
    {
        _SoundRoom_StopBGM();
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
            g_nowselect = g_nowselect % ARRAYSIZE(g_BGMSet); // 範囲外に出ないようにする
        }

        if ((GetControllerMainTrigger(CONTROLLER_BUTTON_TYPE_DOWN) || stickTrigger_negative) && !IsFading())
        {
            SoundEffectPlayRequest(SOUND_TYPE_SE_UI_SELECT, 4.0f, 0.0f);

            g_nowselect++;
            g_nowselect = g_nowselect % ARRAYSIZE(g_BGMSet); // 範囲外に出ないようにする
        }
    }

    if (GetControllerMainTrigger(CONTROLLER_BUTTON_TYPE_A))
    {
        if (g_PlayingBgmSet.isPlaying)
        {
        }
        else
        {
        }

        _SoundRoom_PlayBGM(g_nowselect);
    }

    if (GetControllerMainTrigger(CONTROLLER_BUTTON_TYPE_B))
    {
        SoundEffectPlayRequest(SOUND_TYPE_SE_UI_CANCEL, 4.0f, 0.0f);

        if (g_PlayingBgmSet.isPlaying)
        {
            _SoundRoom_StopBGM();
            g_PlayingBgmSet.bgmSet = nullptr;
        }
        else
        {
            g_exitflag = true;
        }
    }

    if(g_PlayingBgmSet.isPlaying)
    {
        if (g_PlayingBgmSet.bgmSet != nullptr 
            && g_PlayingBgmSet.introFrame < g_PlayingBgmSet.bgmSet->introLen)
        {
            g_PlayingBgmSet.introFrame++;
        }
        else
        {
            g_PlayingBgmSet.loopFrame++;
        }

        //std::vector<float> prs = ProcessAudio(g_PlayingBgmSet.loopID);

        //// 結果を出力
        //for (const auto& val : prs) {
        //    DebugPrintf("%f\n", abs(val));
        //}
    }
}

// ===================================================
// ステージセレクトシーンの描画
// ===================================================
void DrawSceneSoundRoom(void)
{
    float fadeMod = g_fadeTime / 1.0f;
    float fadeAddPosX = EasingInCirc(1.0f - fadeMod) * SCREEN_WIDTH;
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
            400.0f + fadeAddPosX,
            -50.0f,
            GetTexData(TEX_TYPE_TITLE_UI_PANEL_Z).width ,
            GetTexData(TEX_TYPE_TITLE_UI_PANEL_Z).height,
            GetTexData(TEX_TYPE_TITLE_UI_PANEL_Z).textureId,
            &colorBase,
            DRAW_POLYGON_PRIORITY_FORCE_DEEPEST);
        
    }
    {
        DrawSpriteColorQuad(
            410.0f + fadeAddPosX,
            -270.0f + 140.0f * g_nowselectlerp,
            GetTexData(TEX_TYPE_TITLE_BUTTON_Z2).width  * (1.0f + cursorAddScale),
            GetTexData(TEX_TYPE_TITLE_BUTTON_Z2).height * (1.0f + cursorAddScale),
            GetTexData(TEX_TYPE_TITLE_BUTTON_Z2).textureId,
            &colorBase,
            DRAW_POLYGON_PRIORITY_FORCE_DEEPEST);

    }

    
    {
        DRAW_FONT_DAT dat = {};
        dat.txt = L"";
        for (int i = 0; i < ARRAYSIZE(g_BGMSet); i++)
        {
            dat.txt += g_BGMSet[i].name + L"\n\n";
        }

        dat.fonttype = FONT_TYPE_STICK;
        dat.pos = MkF2(
            110.0f + fadeAddPosX,
            -320.0f
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
    }

    {
        DrawSpriteColorQuad(
            -350.0f - fadeAddPosX,
            550.0f,
            GetTexData(TEX_TYPE_TITLE_CONFIRM_CHECK_Z).width ,
            GetTexData(TEX_TYPE_TITLE_CONFIRM_CHECK_Z).height,
            GetTexData(TEX_TYPE_TITLE_CONFIRM_CHECK_Z).textureId,
            &colorBase,
            DRAW_POLYGON_PRIORITY_FORCE_DEEPEST);

    }
    {
        DRAW_FONT_DAT dat = {};
        if (g_PlayingBgmSet.bgmSet != nullptr)
        {
            dat.txt = L"♪  " + g_PlayingBgmSet.bgmSet->name;
        }
        else
        {
            dat.txt = L"♪  ---";
        }

        dat.fonttype = FONT_TYPE_STICK;
        dat.pos = MkF2(
            -820.0f - fadeAddPosX,
            410.0f
        );
        dat.fontSize = 70;
        dat.posShadow = MkF2(
            (float)dat.fontSize / 13.0f,
            (float)dat.fontSize / 13.0f
        );
        dat.color = FONT_TEXT_COLOR_BLACK;
        dat.colorShadow = FONT_TEXT_COLOR_WHITE;
        dat.textAlign = FONT_TEXT_ALIGNMENT_LEFT;
        dat.LineSpacingWidth = dat.fontSize;
        dat.LineSpacingHight = dat.fontSize;

        DrawFont(dat);
    }

    if (g_PlayingBgmSet.isPlaying)
    {
        //std::vector<float> prs = ProcessAudio(g_PlayingBgmSet.loopID);

        //// 結果を出力
        //for (const auto& val : prs) {
        //    DebugPrintf("%f\n", abs(val));
        //}
    }
    if (g_PlayingBgmSet.isPlaying && g_PlayingBgmSet.bgmSet != nullptr)
    {
        //SOUND_ID bgmID;
        //const std::vector<float>* bgmFFT;
        //if (IsAudioPlaying(g_PlayingBgmSet.introID))
        //{
        //    bgmID = g_PlayingBgmSet.introID;
        //    bgmFFT = g_PlayingBgmSet.introFFTData;
        //}
        //else
        //{
        //    bgmID = g_PlayingBgmSet.loopID;
        //    bgmFFT = g_PlayingBgmSet.loopFFTData;
        //}
        //int currentIndex = GetAudioFFTIndex(bgmID);

        //{
        //    DRAW_FONT_DAT dat = {};
        //    dat.txt = L": " + std::to_wstring(currentIndex);

        //    dat.fonttype = FONT_TYPE_STICK;
        //    dat.pos = MkF2(
        //        -820.0f,
        //        0.0f
        //    );
        //    dat.fontSize = 70;
        //    dat.posShadow = MkF2(
        //        (float)dat.fontSize / 13.0f,
        //        (float)dat.fontSize / 13.0f
        //    );
        //    dat.color = FONT_TEXT_COLOR_BLACK;
        //    dat.colorShadow = FONT_TEXT_COLOR_WHITE;
        //    dat.textAlign = FONT_TEXT_ALIGNMENT_LEFT;
        //    dat.LineSpacingWidth = dat.fontSize;
        //    dat.LineSpacingHight = dat.fontSize;

        //    DrawFont(dat);
        //}

        //for (int i = 0; i < 64; i++)
        //{
        //    int indextmp = (currentIndex + i) % bgmFFT->size();
        //    //float value = bgmFFT->at(indextmp);
        //    float value = (*bgmFFT)[indextmp];
        //    {
        //        Float4 colortmp = colorBase;
        //        colortmp.x *= (i % 2 == 0) ? 1.5f: 1.0f;
        //        colortmp.y = colortmp.x;
        //        colortmp.z = colortmp.x;
        //        DrawSpriteColorQuad(
        //            -900.0f + 10.0f * i,
        //            0.0f,
        //            10.0f,
        //            value * 1.0f,
        //            0,
        //            &colortmp,
        //            DRAW_POLYGON_PRIORITY_FORCE_DEEPEST);

        //    }
        //}



        for (int i = 0; i < 75; i++)
        {
            SOUND_ID bgmID;
            const std::vector<float>* bgmFFT;
            int indextmp;
            if (g_PlayingBgmSet.bgmSet != nullptr
                && g_PlayingBgmSet.introFrame < g_PlayingBgmSet.bgmSet->introLen)
            {
                bgmID = g_PlayingBgmSet.introID;
                bgmFFT = g_PlayingBgmSet.introFFTData;
                indextmp = (g_PlayingBgmSet.introFrame + i);
                if (indextmp >= bgmFFT->size())
                {
                    int indextmp2 = indextmp - bgmFFT->size();
                    bgmID = g_PlayingBgmSet.loopID;
                    bgmFFT = g_PlayingBgmSet.loopFFTData;
                    indextmp = indextmp2 % bgmFFT->size();
                }
            }
            else
            {
                bgmID = g_PlayingBgmSet.loopID;
                bgmFFT = g_PlayingBgmSet.loopFFTData;
                indextmp = (g_PlayingBgmSet.loopFrame + i) % bgmFFT->size();
            }

            float value = (*bgmFFT)[indextmp];
            {
                Float4 colortmp = colorBase;
                colortmp.x *= (i % 2 == 0) ? 1.5f: 1.0f;
                colortmp.y = colortmp.x;
                colortmp.z = colortmp.x;

                Float2 sizetmp = {
                    10.0f,
                    value * 1.0f,
                };
                DrawSpriteColorQuad(
                    -900.0f + sizetmp.x * i,
                    300.0f + sizetmp.y * -0.5f,
                    sizetmp.x,
                    sizetmp.y,
                    0,
                    &colortmp,
                    DRAW_POLYGON_PRIORITY_FORCE_DEEPEST);

            }
        }
    }
}

// ===================================================
// ステージセレクトシーンの終了処理
// ===================================================
void FinalizeSceneSoundRoom(void)
{
    SoundBGMStopRequest();
    SoundBGMRequestStopAllRequest();
    SoundEffectStopAllRequest();

    SetMainGameDrawFPS(1);

}