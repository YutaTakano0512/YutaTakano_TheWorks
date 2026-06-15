// ===================================================
// ui_PauseMenu.cpp ui処理
// 
// hara sougo  2025/03/09
// ===================================================
#include "../../game_main.h"
#include "../Polygon/polygon.h"
#include "../Sprite/sprite.h"
#include "../Texture/texture.h"
#include "../Systems/fade.h"
#include "../Systems/camera.h"
#include "../Entities/character.h"
#include "../Entities/player.h"
#include "../Managers/scene.h"
#include "../Managers/soundManager.h"
#include "../Managers/gameManager.h"
#include "ui_Confirm.h"
#include "ui_PauseMenu.h"

// ===================================================
// マクロ定義
// ===================================================

// ===================================================
// 構造体宣言
// ===================================================

// ===================================================
// プロトタイプ宣言
// ===================================================

// ===================================================
// グローバル変数
// ===================================================


void UI_PauseMenu::init()
{
    { //読み込みとか変数の初期化

        //g_testModel = DX_ModelLoad("rom/model/hashira1.fbx", false);
        //g_testModel = GetModelDataIndex("MODEL/cube");

        m_tex_pause_text = GetTexData(TEX_TYPE_UI_PAUSE_TEXT);

        m_tex_text_check_cho
            = LoadTexData("TEX/ui/in_game_master/stagebefore/text_check_cho");

        m_tex_menubtn_items[0][0]
            = LoadTexData("TEX/ui/in_game_master/pause_menu/continuation_off");
        m_tex_menubtn_items[0][1]
            = LoadTexData("TEX/ui/in_game_master/pause_menu/continuation_on");

        m_tex_menubtn_items[1][0]
            = LoadTexData("TEX/ui/in_game_master/pause_menu/exit_off");
        m_tex_menubtn_items[1][1]
            = LoadTexData("TEX/ui/in_game_master/pause_menu/exit_on");

        conf_withdrawal.SetTextTex(m_tex_text_check_cho);
    }

}

void UI_PauseMenu::uninit()
{
    //UnloadTexData(m_tex_text_check_cho);
    //UnloadTexData(m_tex_btn_cont_on);
    //UnloadTexData(m_tex_btn_cont_off);
    //UnloadTexData(m_tex_btn_exit_on);
    //UnloadTexData(m_tex_btn_exit_off);
    for (int i = 0; i < 2; i++)
    {
        for (int j = 0; j < 2; j++)
        {
            UnloadTexData(m_tex_menubtn_items[i][j]);
        }
    }
}

bool UI_PauseMenu::update()
{
    bool menuExit = false;

    if (state == State::Exit)
    {
        return false;
    }

    if (state == State::Withdrawal)
    {
        conf_withdrawal.Update();
        switch (conf_withdrawal.GetStateTrigger())
        {
        case UI_Confirm::StateTrigger::Yes:
            state = State::Exit;
            SoundEffectPlayRequest(SOUND_TYPE_SE_UI_OK, 4.0f, 0.0f);

            GamePause(false);
            GameStopRequest(60);
            SetSceneFade(SCENE_CHARACTERSELECT, FADE_TYPE_STAR_WIPE);
            break;

        case UI_Confirm::StateTrigger::No:
            state = State::Default;
            SoundEffectPlayRequest(SOUND_TYPE_SE_UI_CANCEL, 4.0f, 0.0f);

            break;

        default:break;
        }
    }
    else
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

        if (GetControllerMainTrigger(CONTROLLER_BUTTON_TYPE_PLUS)
            || GetControllerMainTrigger(CONTROLLER_BUTTON_TYPE_MINUS)
            || GetControllerMainTrigger(CONTROLLER_BUTTON_TYPE_B)
            )
        {
            menuExit = true;
        }

        if (GetControllerMainTrigger(CONTROLLER_BUTTON_TYPE_UP)
            || stickTrigger_positive)
        {
            SoundEffectPlayRequest(SOUND_TYPE_SE_UI_SELECT, 4.0f, 0.0f);

            m_cursorConfirmMenu = (StateMenu)((
                (int)m_cursorConfirmMenu - 1 + (int)StateMenu::Max)
                % (int)StateMenu::Max
                );

            m_old_button = m_MenuItems_ON_pos[
                static_cast<int>(m_cursorConfirmMenu)
            ].y;//現在の位置を保存する
            m_Interpolation = 0.0f;
            m_is_move = true;
        }

        if (GetControllerMainTrigger(CONTROLLER_BUTTON_TYPE_DOWN)
            || stickTrigger_negative)
        {
            SoundEffectPlayRequest(SOUND_TYPE_SE_UI_SELECT, 4.0f, 0.0f);

            m_cursorConfirmMenu =
                (StateMenu)(((int)m_cursorConfirmMenu + 1) % (int)StateMenu::Max);

            m_old_button = m_buttonZPositionY;
            m_Interpolation = 0.0f;
            m_is_move = true;
        }

        if (!m_is_move)
        {//ボタン動かす
            m_scaleTime += 0.05f;
            m_buttonZScale = 1.0f + 0.05f * sin(m_scaleTime);
        }

        //イージング
        float t = EasingInOutSine(m_Interpolation);

        //線形補間
        m_buttonZPositionY = m_old_button * (1.0f - t)
            + m_MenuItems_ON_pos[
                static_cast<int>(m_cursorConfirmMenu)
            ].y * t;

        if (m_is_move == true)
        {//trueになったら数秒語にfalse
            m_move++;

            if (m_move > 20.0f)
            {
                m_is_move = false;
                m_move = 0.0f;
            }
        }

        m_Interpolation += 0.3f;//速度の設定

        if (m_Interpolation > 1.0f)
        {
            m_Interpolation = 1.0f;
        }

        if (GetControllerMainTrigger(CONTROLLER_BUTTON_TYPE_A))
        {
            SoundEffectPlayRequest(SOUND_TYPE_SE_UI_OK, 4.0f, 0.0f);

            switch (m_cursorConfirmMenu)
            {
            case StateMenu::Default:
                menuExit = true;
                break;
            case StateMenu::Withdrawal:
                state = State::Withdrawal;
                break;
            default:
                break;
            }
        }

        //if (menuExit)
        //{
        //    g_sceneGame.gamePause = false;
        //    GamePause(g_sceneGame.gamePause);

        //    //ポーズ解除時にBGMの音量を戻す
        //    SoundBGMSetVolumeRequest(g_sceneGame.g_pauseBeforeBgmVolume, 0);

        //    //SoundEffectPlayRequest(SOUND_TYPE_PAUSE_CANCEL, 0.4f, 0);
        //}

        //if (GetControllerMainTrigger(CONTROLLER_BUTTON_TYPE_A))
        //{
        //    state = State::Withdrawal;
        //}
    }

    return menuExit;
}

void UI_PauseMenu::draw()
{
    if (IsGamePause())
    { //ポーズの表示

        {
            DRAW_POLYGON_DAT dsdat = {};
            dsdat.pos.x = -1.0f;
            dsdat.pos.y = -1.0f;
            dsdat.pos.z = 0.0f;

            dsdat.size.x = SCREEN_WIDTH + 2.0f;
            dsdat.size.y = SCREEN_HEIGHT + 2.0f;
            dsdat.size.z = 1.0f;

            dsdat.rot = 0.0f;

            //dsdat.color = MkF4(-10.0f, -10.0f, -10.0f, 0.75f);
            dsdat.color = MkF4(0.0f, 0.0f, 0.0f, 0.75f);

            dsdat.texNo = 0;
            dsdat.loadTexType = LOADTEXTURETYPE_MAIN;

            dsdat.texUV = {
                0.0f,
                0.0f
            };
            dsdat.texWH = { 1.0f, 1.0f };

            dsdat.startPos = {
                0.0f,
                0.0f
            };
            dsdat.startRot = 0;
            dsdat.startScale = { 1.0f, 1.0f };
            dsdat.startScale3D = { 1.0f, 1.0f, 1.0f };

            dsdat.shaderOpt.bloom = DPD_SHADER_OPT_BLOOM_NONE;

            DRAW_POLYGON_IN_CAMERA_EX_OPTION opt = {};
            opt.reverse = false;
            opt.fixed = true;
            opt.depthBuffIgnore = true;
            //opt.priority = DRAW_POLYGON_PRIORITY_FORCE_ALWAYS;
            //opt.priority = DRAW_POLYGON_PRIORITY_FORCE_NML_u1;
            //opt.priority = DRAW_POLYGON_PRIORITY_FORCE_ALWAYS;
            opt.priority = DRAW_POLYGON_PRIORITY_FORCE_NORMAL;
            //opt.priority = DRAW_POLYGON_PRIORITY_FORCE_DEEPEST;
            //opt.shaderType = DPD_SHADER_TYPE_UI_BG_GENERAL;

            DrawPolygonInCameraEx(dsdat, opt);
        }

        //{
        //    DRAW_POLYGON_DAT dsdat = {};
        //    dsdat.pos.x = -1.0f;
        //    dsdat.pos.y = -1.0f;
        //    dsdat.pos.z = 0.0f;

        //    dsdat.size.x = SCREEN_WIDTH + 2.0f;
        //    dsdat.size.y = SCREEN_HEIGHT + 2.0f;
        //    dsdat.size.z = 1.0f;

        //    dsdat.rot = 0.0f;

        //    dsdat.color = MkF4(-10.0f, -10.0f, -10.0f, 0.3f);

        //    dsdat.texNo = 0;
        //    dsdat.loadTexType = LOADTEXTURETYPE_MAIN;

        //    dsdat.texUV = {
        //        0.0f,
        //        0.0f
        //    };
        //    dsdat.texWH = { 1.0f, 1.0f };

        //    dsdat.startPos = {
        //        0.0f,
        //        0.0f
        //    };
        //    dsdat.startRot = 0;
        //    dsdat.startScale = { 1.0f, 1.0f };
        //    dsdat.startScale3D = { 1.0f, 1.0f, 1.0f };

        //    dsdat.shaderOpt.bloom = DPD_SHADER_OPT_BLOOM_NONE;

        //    DRAW_POLYGON_IN_CAMERA_EX_OPTION opt = {};
        //    opt.reverse = false;
        //    opt.fixed = true;
        //    opt.depthBuffIgnore = true;
        //    //opt.priority = DRAW_POLYGON_PRIORITY_FORCE_ALWAYS;
        //    opt.priority = DRAW_POLYGON_PRIORITY_FORCE_DEEPEST;

        //    DrawPolygonInCameraEx(dsdat, opt);

        //    dsdat.pos = MkF3(
        //        (float)(SCREEN_WIDTH * 0.4f),
        //        (float)(SCREEN_HEIGHT * 0.4f + sin((float)GetMainGameFrame() / 60) * 20.0f),
        //        0.0f
        //    );
        //    dsdat.size = MkF3(
        //        400.0f,
        //        400.0f,
        //        1.0f
        //    );
        //    dsdat.texNo = m_tex_pause_text.textureId;
        //    dsdat.color = MkF4(1.0f, 1.0f, 1.0f, 1.0f);
        //    DrawPolygonInCameraEx(dsdat, opt);
        //}

        {
            Float4 color = { 1.0f, 1.0f, 1.0f, 1.0f };
            DrawSpriteColorQuad(0, 0,
                GetTexData(TEX_TYPE_TITLE_UI_PANEL_Z).width,
                GetTexData(TEX_TYPE_TITLE_UI_PANEL_Z).height,
                GetTexData(TEX_TYPE_TITLE_UI_PANEL_Z).textureId,
                &color,
                DRAW_POLYGON_PRIORITY_FORCE_ALWAYS);
            DrawSpriteColorQuad(390, 330,
                GetTexData(TEX_TYPE_TITLE_BUTTON_A).width,
                GetTexData(TEX_TYPE_TITLE_BUTTON_A).height,
                GetTexData(TEX_TYPE_TITLE_BUTTON_A).textureId,
                &color,
                DRAW_POLYGON_PRIORITY_FORCE_ALWAYS);
            DrawSpriteColorQuad(230, 330,
                GetTexData(TEX_TYPE_TITLE_BUTTON_B).width,
                GetTexData(TEX_TYPE_TITLE_BUTTON_B).height,
                GetTexData(TEX_TYPE_TITLE_BUTTON_B).textureId,
                &color,
                DRAW_POLYGON_PRIORITY_FORCE_ALWAYS);


            //ボタン
            DrawSpriteColorQuad(
                m_MenuItems_ON_pos[
                    static_cast<int>(m_cursorConfirmMenu)].x,
                m_buttonZPositionY,
                        GetTexData(TEX_TYPE_TITLE_BUTTON_Z2).width * m_buttonZScale,
                        GetTexData(TEX_TYPE_TITLE_BUTTON_Z2).height * m_buttonZScale,
                        GetTexData(TEX_TYPE_TITLE_BUTTON_Z2).textureId,
                        &color,
                        DRAW_POLYGON_PRIORITY_FORCE_ALWAYS);

            for (int i = 0; i < (int)StateMenu::Max; ++i)
            {//タイトルテキストの設定

                float textScale = 0.8f;

                if (i == static_cast<int>(m_cursorConfirmMenu))
                {
                    textScale = m_buttonZScale;
                }

                if (i == static_cast<int>(m_cursorConfirmMenu))
                {
                    DrawSpriteColorQuad(
                        m_MenuItems_ON_pos[i].x,
                        m_MenuItems_ON_pos[i].y,
                        (m_tex_menubtn_items[i][1]).width,
                        (m_tex_menubtn_items[i][1]).height,
                        (m_tex_menubtn_items[i][1]).textureId,
                        &color,
                        DRAW_POLYGON_PRIORITY_FORCE_ALWAYS);
                }
                else
                {
                    DrawSpriteColorQuad(
                        m_MenuItems_OFF_pos[i].x,
                        m_MenuItems_OFF_pos[i].y,
                        (m_tex_menubtn_items[i][0]).width * 0.8f,
                        (m_tex_menubtn_items[i][0]).height * 0.8f,
                        (m_tex_menubtn_items[i][0]).textureId,
                        &color,
                        DRAW_POLYGON_PRIORITY_FORCE_ALWAYS
                        );
                }
            }
        }

        //{
        //	Float4 color = MkF4(-10.0f, -10.0f, -10.0f, 0.3f);
        //	DRAW_SPRITE_BUFFER dsb = {};
        //	SpriteSetUp(&dsb);
        //	SpriteShape(&dsb,
        //		MkF2(-1.0f, -1.0f),
        //		0.0f,
        //		MkF2((float)SCREEN_WIDTH + 2.0f, (float)SCREEN_HEIGHT + 2.0f)
        //	);
        //	SpriteRelease(&dsb,
        //		color,
        //		0,
        //		MkF2(0.0f, 0.0f),
        //		MkF2(1.0f, 1.0f),
        //		LOADTEXTURETYPE_MAIN,
        //		true,
        //		DRAW_POLYGON_PRIORITY_FORCE_ALWAYS
        //	);
        //}
        //{
        //	Float4 color = MkF4(1.0f, 1.0f, 1.0f, 1.0f);
        //	DRAW_SPRITE_BUFFER dsb = {};
        //	SpriteSetUp(&dsb);
        //	SpriteShape(&dsb,
        //		MkF2((float)(SCREEN_WIDTH * 0.4f), (float)(SCREEN_HEIGHT * 0.4f + sin((float)GetMainGameFrame() / 60) * 20.0f)),
        //		0.0f,
        //		MkF2(400.0f, 400.0f)
        //	);
        //	SpriteRelease(&dsb,
        //		color,
        //		g_tex_pause_text.textureId,
        //		MkF2(0.0f, 0.0f),
        //		MkF2(1.0f, 1.0f),
        //		LOADTEXTURETYPE_MAIN,
        //		true,
        //		DRAW_POLYGON_PRIORITY_FORCE_ALWAYS
        //	);
        //}
        //{
        //    Float4 color = MkF4(1.0f, 1.0f, 1.0f, 1.0f);
        //    DRAW_SPRITE_BUFFER dsb = {};
        //    SpriteSetUp(&dsb);
        //    SpriteShape(&dsb,
        //        MkF2(0.0f, 0.0f),
        //        0.0f,
        //        MkF2(500.0f, 200.0f)
        //    );
        //    SpriteRelease(&dsb,
        //        color,
        //        g_tex_pause_retire_button.textureId,
        //        MkF2(0.0f, 0.0f),
        //        MkF2(1.0f, 1.0f),
        //        LOADTEXTURETYPE_MAIN,
        //        true,
        //        DRAW_POLYGON_PRIORITY_FORCE_ALWAYS
        //    );
        //}
    }
    else
    {
        if (GetScene() == SCENE_GAME)
        {
            //Float4 color = MkF4(1.0f, 1.0f, 1.0f, 0.75f);
            //DRAW_SPRITE_BUFFER dsb = {};
            //SpriteSetUp(&dsb);
            //SpriteShape(&dsb,
            //	MkF2((float)(SCREEN_WIDTH * 0.37f), (float)(SCREEN_HEIGHT * 0.39f)),
            //	0.0f,
            //	MkF2(600.0f, 600.0f)
            //);
            //SpriteRelease(&dsb,
            //	color,
            //	g_tex_pause_gui.textureId,
            //	MkF2(0.0f, 0.0f),
            //	MkF2(1.0f, 1.0f),
            //	LOADTEXTURETYPE_MAIN
            //);
        }
    }

    switch (state)
    {
    case State::Withdrawal:
        conf_withdrawal.Draw();
        break;
    default:
        break;
    }
}