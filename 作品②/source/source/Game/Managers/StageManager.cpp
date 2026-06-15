// ===================================================
// StageManager.h ステージ処理
// 
// hara sougo  2025/02/26
// ===================================================
#include "../../game_main.h"
#include "../Sprite/sprite.h"
#include "../Polygon/modelData.h"
#include "../Texture/texture.h"
#include "../Entities/Block/block.h"
#include "../Entities/Block/blockPrefab.h"
#include "../Systems/collision.h"
#include "../Systems/easing.h"

#include "../Managers/scene.h"
#include "../../scenes/SCENE_GAME.h"

#include "../Managers/soundManager.h"
#include "../Managers/gameManager.h"
#include "../Game Map/gameMap.h"

#include "../Game Object/gameObject.h"
#include "../Game Object/gameObjectPrefab.h"

#include "../Managers/bg.h"

#include "../Systems/ui.h"
#include "../Systems/ui_Game.h"

#include "../Entities/player.h"
#include "../Systems/camera.h"

#include "../Entities/character.h"

#include "../Game Events/GE_Gaya.h"
#include "../Game Object/Game Object Events/GOE_Character_Player.h"

#include "StageManager.h"
#include "../Game Stage/Stage_Tutorial.h"
#include "../Game Stage/Stage_Ruins.h"
#include "../Game Stage/Stage_Simulation.h"

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



void StageManager::Init()
{
 /*   m_Tex_danger_middle = LoadTexData("TEX/ui/in_game_master/stagebefore/normalmode");
    m_Tex_danger_high = LoadTexData("TEX/ui/in_game_master/stagebefore/hardmode");
    m_Tex_stageName = LoadTexData("TEX/ui/in_game_master/stagebefore/ko_dai");
    m_Tex_menuback = LoadTexData("TEX/ui/in_game_master/stagebefore/back");
    m_Tex_ready = LoadTexData("TEX/ui/in_game_master/stagebefore/ready");
    m_Tex_Operation_description =
        LoadTexData("TEX/ui/in_game_master/stagebefore/ctrl_info");*/
    m_Tex_Btn_ZR[0] = LoadTexData("TEX/ui/in_game_master/stagebefore/_Button_ZR_off");
    m_Tex_Btn_ZR[1] = LoadTexData("TEX/ui/in_game_master/stagebefore/_Button_ZR_on");
    m_Tex_Bar_ZR[0] = LoadTexData("TEX/ui/in_game_master/stagebefore/Bar_ZR_off");
    m_Tex_Bar_ZR[1] = LoadTexData("TEX/ui/in_game_master/stagebefore/Bar_ZR_on");
    m_Tex_Bar_ZR[2] = LoadTexData("TEX/ui/in_game_master/stagebefore/ZR_line");
    //m_Tex_Line_ZR = LoadTexData("TEX/ui/in_game_master/stagebefore/ZR_line");
    m_Tex_Text_ZR = LoadTexData("TEX/ui/in_game_master/stagebefore/Text_ZR");

    m_Tex_Btn_ZL = LoadTexData("TEX/ui/in_game_master/stagebefore/ZL_Return");
    m_Tex_Mission_Z = LoadTexData("TEX/ui/in_game_master/stagebefore/Mission_Z");
    m_Tex_Text_stageName = LoadTexData("TEX/ui/in_game_master/stagebefore/text_codai");

    m_Tex_Check = LoadTexData("TEX/ui/in_game_master/stagebefore/check");
    m_Tex_Ttrl_Btn_B = LoadTexData("TEX/ui/in_game_master/stagebefore/Dodge");
    m_Tex_Ttrl_Btn_A = LoadTexData("TEX/ui/in_game_master/stagebefore/kako");
    m_Tex_Ttrl_Btn_LS = LoadTexData("TEX/ui/in_game_master/stagebefore/Move");
    m_Tex_Ttrl_Btn_X = LoadTexData("TEX/ui/in_game_master/stagebefore/skill");

    m_Tex_Ttrl_Confirm_Text = LoadTexData("TEX/ui/in_game_master/stagebefore/text_check_cho");
    m_Ttrl_Confirm.SetTextTex(m_Tex_Ttrl_Confirm_Text);

   //まず待機ステージへ移動{
    m_Main_Wait_Flag = true;
    m_Main_Wait_EndFlag = false;
    m_Main_Wait_EndFadeFlag = -1;


#if defined(_DEBUG) || defined(DEBUG)
    if (Keyboard_IsKeyDown(KK_H)) //仮
    {
        m_Diff = Difficulty::Hard;
    }
#endif

    Stage_Base* stgbuff = new Stage_Tutorial(m_Diff);
    m_Stage = stgbuff;
    m_Stage->Init();

    m_Main_Wait_Flag = m_Main_Wait_Flag;

    //参加
    SetPlayerLogInEnable(true);
}

void StageManager::Uninit()
{
    if (m_Stage != nullptr)
    {
        delete m_Stage;
    }
    //UnloadTexData(m_Tex_danger_middle);
    //UnloadTexData(m_Tex_danger_high);
    //UnloadTexData(m_Tex_stageName);
    //UnloadTexData(m_Tex_menuback);
    //UnloadTexData(m_Tex_ready);
    //UnloadTexData(m_Tex_Operation_description);
    UnloadTexData(m_Tex_Btn_ZR[0]);
    UnloadTexData(m_Tex_Btn_ZR[1]);
    UnloadTexData(m_Tex_Bar_ZR[0]);
    UnloadTexData(m_Tex_Bar_ZR[1]);
    UnloadTexData(m_Tex_Bar_ZR[2]);
    //UnloadTexData(m_Tex_Line_ZR);
    UnloadTexData(m_Tex_Text_ZR);

    UnloadTexData(m_Tex_Btn_ZL);
    UnloadTexData(m_Tex_Mission_Z);
    UnloadTexData(m_Tex_Text_stageName);

    UnloadTexData(m_Tex_Check);
    UnloadTexData(m_Tex_Ttrl_Btn_B);
    UnloadTexData(m_Tex_Ttrl_Btn_A);
    UnloadTexData(m_Tex_Ttrl_Btn_LS);
    UnloadTexData(m_Tex_Ttrl_Btn_X);

    UnloadTexData(m_Tex_Ttrl_Confirm_Text);
    
}


void StageManager::Update()
{
    //DebugPrintf("%d\n", g_Stage_Main_Wait ? 1: 0);

    if (m_Stage != nullptr)
    {
        m_Stage->Update();
    }

    //待機所の処理
    if (m_Main_Wait_Flag)
    {
        //プレイヤーのHPが減らないように
        for (int i = 0; i < PLAYER_MAX; i++)
        {
            PLAYER* player = GetPlayer(i);
            GAME_OBJECT* playerObj = player->obj;
            if (playerObj != nullptr)
            {
                playerObj->status.hp = playerObj->status.hpMax;
            }
        }

        if (m_Main_Wait_EndFadeFlag >= 0)
        {
            m_Main_Wait_EndFadeFlag++;

            if (m_Main_Wait_EndFadeFlag
                == STAGE_MAIN_WAIT_END_FADE / 2)
            {
                //プレイヤー以外を削除する
                for (int i = 0; i < GAME_OBJECT_MAX; i++)
                {
                    GAME_OBJECT* ob = GetGameObject(i);
                    if (IsActiveGameObject(ob)
                        && (ob->tags.size() <= 0
                            || !ob->tags[0].find("PLAYER") == 0)
                        )
                    {
                        DeleteGameObject(ob);
                    }
                }

                //参加締め切り
                SetPlayerLogInEnable(false);

                //元々あったステージを削除
                if (m_Stage != nullptr)
                { //�X�e�[�W�̏I��
                    m_Stage->Uninit();
                    delete m_Stage;
                    m_Stage = nullptr;
                }

                Stage_Base* stgbuff = nullptr;

                //ステージを決定
                //if (Keyboard_IsKeyDown(KK_SPACE))
                if (m_SingleBossBattleNum >= 0)
                {
                    stgbuff = new Stage_Simulation(
                        m_Diff,
                        m_SingleBossBattleNum
                    );
                }
                else
                {
                    stgbuff = new Stage_Ruins(m_Diff);
                }

                m_Stage = stgbuff;
                m_Stage->Init();

                GameManagerResetResult();
                GameManagerResetResultAddScoreBuf();
                GameManagerResetResultAddScoreEvent();

                //プレイヤーの参加情報を格納する
                int playerActiveNum = 0;
                for (int i = 0; i < PLAYER_MAX; i++)
                {
                    PLAYER* player = GetPlayer(i);
                    GAME_OBJECT* playerObj = player->obj;
                    if (playerObj != nullptr)
                    {
                        GameManagerSetPlayer(
                            i,
                            (PLAYER_CHARACTER_TYPE)player->playerNo
                        );
                        playerActiveNum++;
                    }
                    else
                    {
                        GameManagerSetPlayer(i, PLAYER_CHARACTER_TYPE_None);
                    }
                }
                GameManagerSetPlayerNum(playerActiveNum);

                GameManagerResetSetDiff(m_Diff);

            }

            //フラグを折る
            if (m_Main_Wait_EndFadeFlag > STAGE_MAIN_WAIT_END_FADE)
            {
                m_Main_Wait_Flag = FALSE;

                m_Main_Wait_EndFadeFlag = -2;
            }

            //DebugPrintf("%d\n", m_Main_Wait_EndFadeFlag);

            //return;
        }

        else
        {
            ////戻る
            //if (
            //    GetControllerMainTrigger(CONTROLLER_BUTTON_TYPE_L)
            //)
            //{
            //    m_Main_Wait_EndFlag = true;

            //    SetSceneFade(SCENE_CHARACTERSELECT);
            //}
            //else if (
            //    GetControllerMainTrigger(CONTROLLER_BUTTON_TYPE_R)
            //)
            //{
            //    //進
            //    m_Main_Wait_EndFlag = true;

            //    m_Main_Wait_EndFadeFlag = 0;
            //}
            
            if (!m_Main_Wait_EndFlag)
            {
                if (m_TutorialOut_ConFlag)
                {
                    update_ConfirmMenu();
                }
                else
                {
                    bool susumu = GetControllerMainPress(CONTROLLER_BUTTON_TYPE_ZR)
                        ;
                    //bool modoru = GetControllerMainPress(CONTROLLER_BUTTON_TYPE_ZL)
                    //    ;
                    bool modoru = GetControllerMainTrigger(CONTROLLER_BUTTON_TYPE_ZL)
                        ;

                    //戻る
                    if (modoru)
                    {
                        GameManagerSetMovieFlag(true);

                        SoundEffectPlayRequest(SOUND_TYPE_SE_UI_OK, 4.0f, 0.0f);

                        m_TutorialOut_ConFlag = true;
                    }
                    ////戻る
                    //if (modoru)
                    //{
                    //    m_TutorialOutPushBtn++;
                    //    if (m_TutorialOutPushBtn > TUTORIAL_OUT_PUSH_BTN_FRAME)
                    //    {
                    //        m_Main_Wait_EndFlag = true;

                    //        SetSceneFade(SCENE_CHARACTERSELECT, FADE_TYPE_STAR_WIPE);
                    //    }
                    //}
                    //else
                    //{
                    //    m_TutorialOutPushBtn = 0;
                    //}

                    //進むー！〇すぞー！
                    if (!modoru && susumu)
                    {
                        m_BecomeMainPushBtn++;
                        if (m_BecomeMainPushBtn > TUTORIAL_OUT_PUSH_BTN_FRAME)
                        {
                            //進
                            m_Main_Wait_EndFlag = true;

                            m_Main_Wait_EndFadeFlag = 0;
                        }
                    }
                    else
                    {
                        m_BecomeMainPushBtn = 0;
                    }

                    //プレイヤーのチュートリアル
                    for (int i = 0; i < PLAYER_MAX; i++)
                    {
                        PLAYER* player = GetPlayer(i);
                        GAME_OBJECT* playerObj = player->obj;
                        if (playerObj != nullptr)
                        {
                            GOE_Character_Player* goe =
                                dynamic_cast<GOE_Character_Player*>(playerObj->events.source);  // ダウンキャスト
                            if (goe != nullptr)
                            {
                                if (goe->GetFlagTtrlAttack())
                                {
                                    m_Flag_Ttrl_Btn_B[i]++;
                                }
                                if (goe->GetFlagTtrlKako())
                                {
                                    m_Flag_Ttrl_Btn_A[i]++;
                                }
                                if (goe->GetFlagTtrlMove())
                                {
                                    m_Flag_Ttrl_Btn_LS[i]++;
                                }
                                if (goe->GetFlagTtrlSkill())
                                {
                                    m_Flag_Ttrl_Btn_X[i]++;
                                }
                            }
                        }
                    }
                }
            }

            
        }
    }

}


void StageManager::Draw()
{
    if (m_Stage != nullptr)
    {
        m_Stage->Draw();
    }

    if (m_Main_Wait_EndFadeFlag >= 0)
    {
        {
            DRAW_POLYGON_DAT dsdat = {};
            dsdat.pos.x = -1.0f;
            dsdat.pos.y = -1.0f;
            dsdat.pos.z = 0.0f;

            dsdat.size.x = SCREEN_WIDTH + 2.0f;
            dsdat.size.y = SCREEN_HEIGHT + 2.0f;
            dsdat.size.z = 1.0f;

            dsdat.rot = 0.0f;

            dsdat.color = MkF4(-10.0f, -10.0f, -10.0f, 2.0f);
            dsdat.color.w *= 1.0f - (cos(2.0f * SYS_GENERAL_PI
                * ((float)m_Main_Wait_EndFadeFlag / STAGE_MAIN_WAIT_END_FADE))
                / 2.0f + 0.5f);

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
            opt.priority = (DRAW_POLYGON_PRIORITY)(DRAW_POLYGON_PRIORITY_FORCE_ALWAYS - 1);

            DrawPolygonInCameraEx(dsdat, opt);
        }
    }

    if (!IsVisibleUI_Game())
    {
        return;
    }

    float movieFlagAlpha = GetUIMovieFlagAlpha_Game();

    if (m_Main_Wait_Flag && m_Main_Wait_EndFadeFlag <= 0)
    {
        for (int i = 0; i < PLAYER_MAX; i++)
        {
            if (i == 0)
            {
                if (m_SingleBossBattleNum < 0)
                {
                    DrawSpriteInCameraSingle(
                        i,
                        //表示座標 (四角形の真ん中)
                        -SCREEN_WIDTH / 2 + 240.0f,
                        -SCREEN_HEIGHT / 2 + 150.0f,

                        //サイズ width hight
                        500.0f,
                        500.0f,

                        //角度
                        0.0f,

                        //色
                        MkF4(1.0f, 1.0f, 1.0f, movieFlagAlpha),

                        m_Tex_Mission_Z.textureId,  //画像(テクスチャ)の番号
                        MkF2(0.0f, 0.0f),
                        MkF2(1.0f, 1.0f),
                        DRAW_POLYGON_PRIORITY_FORCE_NORMAL,
                        DPD_SHADER_TYPE_UI_BG_GENERAL
                    );
                    DrawSpriteInCameraSingle(
                        i,
                        //表示座標 (四角形の真ん中)
                        -SCREEN_WIDTH / 2 + 240.0f,
                        -SCREEN_HEIGHT / 2 + 180.0f,

                        //サイズ width hight
                        300.0f,
                        300.0f,

                        //角度
                        0.0f,

                        //色
                        MkF4(1.0f, 1.0f, 1.0f, movieFlagAlpha),

                        m_Tex_Text_stageName.textureId,  //画像(テクスチャ)の番号
                        MkF2(0.0f, 0.0f),
                        MkF2(1.0f, 1.0f)
                    );
                }

                //{
                //    TEX_DAT tex_diff =
                //        m_Diff == Difficulty::Hard
                //        ? m_Tex_danger_high : m_Tex_danger_middle;
                //    DrawSpriteInCameraSingle(
                //        i,
                //        //表示座標 (四角形の真ん中)
                //        -SCREEN_WIDTH / 2 + 150.0f,
                //        -SCREEN_HEIGHT / 2 + 140.0f,

                //        //サイズ width hight
                //        260.0f,
                //        260.0f * ((float)tex_diff.height / tex_diff.width),

                //        //角度
                //        0.0f,

                //        //色
                //        MkF4(1.0f, 1.0f, 1.0f, 1.0f),

                //        tex_diff.textureId,  //画像(テクスチャ)の番号
                //        MkF2(0.0f, 0.0f),
                //        MkF2(1.0f, 1.0f)
                //    );
                //}

                float TtrlModB = pow(EasingOutBack(std::min((
                    (float)m_Flag_Ttrl_Btn_B[i] / TUTORIAL_CHECK_FRAME),
                    1.0f)), 8.0f);
                float TtrlModA = pow(EasingOutBack(std::min((
                    (float)m_Flag_Ttrl_Btn_A[i] / TUTORIAL_CHECK_FRAME),
                    1.0f)), 8.0f);
                float TtrlModLS = pow(EasingOutBack(std::min((
                    (float)m_Flag_Ttrl_Btn_LS[i] / TUTORIAL_CHECK_FRAME),
                    1.0f)), 8.0f);
                float TtrlModX = pow(EasingOutBack(std::min((
                    (float)m_Flag_Ttrl_Btn_X[i] / TUTORIAL_CHECK_FRAME),
                    1.0f)), 8.0f);

                DrawSpriteInCameraSingle(
                    i,
                    //表示座標 (四角形の真ん中)
                    SCREEN_WIDTH / 2 - 260.0f,
                    -SCREEN_HEIGHT / 2 + 80.0f,

                    //サイズ width hight
                    400.0f,
                    400.0f,

                    //角度
                    0.0f,

                    //色
                    MkF4(1.0f, 1.0f, 1.0f, movieFlagAlpha),

                    m_Tex_Ttrl_Btn_A.textureId,  //画像(テクスチャ)の番号
                    MkF2(0.0f, 0.0f),
                    MkF2(1.0f, 1.0f),
                    DRAW_POLYGON_PRIORITY_FORCE_NORMAL,
                    DPD_SHADER_TYPE_UI_BG_GENERAL
                );
                DrawSpriteInCameraSingle(
                    i,
                    //表示座標 (四角形の真ん中)
                    SCREEN_WIDTH / 2 - 450.0f,
                    -SCREEN_HEIGHT / 2 + 80.0f,

                    //サイズ width hight
                    50.0f * (TtrlModA),
                    50.0f * (TtrlModA),

                    //角度
                    0.0f,

                    //色
                    MkF4(1.0f, 1.0f, 1.0f, movieFlagAlpha),

                    m_Tex_Check.textureId,  //画像(テクスチャ)の番号
                    MkF2(0.0f, 0.0f),
                    MkF2(1.0f, 1.0f)
                );

                DrawSpriteInCameraSingle(
                    i,
                    //表示座標 (四角形の真ん中)
                    SCREEN_WIDTH / 2 - 260.0f,
                    -SCREEN_HEIGHT / 2 + 160.0f,

                    //サイズ width hight
                    400.0f,
                    400.0f,

                    //角度
                    0.0f,

                    //色
                    MkF4(1.0f, 1.0f, 1.0f, movieFlagAlpha),

                    m_Tex_Ttrl_Btn_X.textureId,  //画像(テクスチャ)の番号
                    MkF2(0.0f, 0.0f),
                    MkF2(1.0f, 1.0f),
                    DRAW_POLYGON_PRIORITY_FORCE_NORMAL,
                    DPD_SHADER_TYPE_UI_BG_GENERAL
                );
                DrawSpriteInCameraSingle(
                    i,
                    //表示座標 (四角形の真ん中)
                    SCREEN_WIDTH / 2 - 450.0f,
                    -SCREEN_HEIGHT / 2 + 160.0f,

                    //サイズ width hight
                    50.0f * 1.3 * (TtrlModX),
                    50.0f * 1.3 * (TtrlModX),

                    //角度
                    0.0f,

                    //色
                    MkF4(1.0f, 1.0f, 1.0f, movieFlagAlpha),

                    m_Tex_Check.textureId,  //画像(テクスチャ)の番号
                    MkF2(0.0f, 0.0f),
                    MkF2(1.0f, 1.0f),
                    DRAW_POLYGON_PRIORITY_FORCE_NORMAL,
                    DPD_SHADER_TYPE_NO_UV_LOOP
                );

                DrawSpriteInCameraSingle(
                    i,
                    //表示座標 (四角形の真ん中)
                    SCREEN_WIDTH / 2 - 410.0f,
                    -SCREEN_HEIGHT / 2 + 230.0f,

                    //サイズ width hight
                    240.0f,
                    240.0f,

                    //角度
                    0.0f,

                    //色
                    MkF4(1.0f, 1.0f, 1.0f, movieFlagAlpha),

                    m_Tex_Ttrl_Btn_LS.textureId,  //画像(テクスチャ)の番号
                    MkF2(0.0f, 0.0f),
                    MkF2(1.0f, 1.0f),
                    DRAW_POLYGON_PRIORITY_FORCE_NORMAL,
                    DPD_SHADER_TYPE_UI_BG_GENERAL
                );
                DrawSpriteInCameraSingle(
                    i,
                    //表示座標 (四角形の真ん中)
                    SCREEN_WIDTH / 2 - 520.0f,
                    -SCREEN_HEIGHT / 2 + 235.0f,

                    //サイズ width hight
                    50.0f * (TtrlModLS),
                    50.0f * (TtrlModLS),

                    //角度
                    0.0f,

                    //色
                    MkF4(1.0f, 1.0f, 1.0f, movieFlagAlpha),

                    m_Tex_Check.textureId,  //画像(テクスチャ)の番号
                    MkF2(0.0f, 0.0f),
                    MkF2(1.0f, 1.0f)
                );

                DrawSpriteInCameraSingle(
                    i,
                    //表示座標 (四角形の真ん中)
                    SCREEN_WIDTH / 2 - 170.0f,
                    -SCREEN_HEIGHT / 2 + 235.0f,

                    //サイズ width hight
                    220.0f,
                    220.0f,

                    //角度
                    0.0f,

                    //色
                    MkF4(1.0f, 1.0f, 1.0f, movieFlagAlpha),

                    m_Tex_Ttrl_Btn_B.textureId,  //画像(テクスチャ)の番号
                    MkF2(0.0f, 0.0f),
                    MkF2(1.0f, 1.0f),
                    DRAW_POLYGON_PRIORITY_FORCE_NORMAL,
                    DPD_SHADER_TYPE_UI_BG_GENERAL
                );
                DrawSpriteInCameraSingle(
                    i,
                    //表示座標 (四角形の真ん中)
                    SCREEN_WIDTH / 2 - 280.0f,
                    -SCREEN_HEIGHT / 2 + 235.0f,

                    //サイズ width hight
                    50.0f * (TtrlModB),
                    50.0f * (TtrlModB),

                    //角度
                    0.0f,

                    //色
                    MkF4(1.0f, 1.0f, 1.0f, movieFlagAlpha),

                    m_Tex_Check.textureId,  //画像(テクスチャ)の番号
                    MkF2(0.0f, 0.0f),
                    MkF2(1.0f, 1.0f)
                );

                DrawSpriteInCameraSingle(
                    i,
                    //表示座標 (四角形の真ん中)
                    -SCREEN_WIDTH / 2 + 140.0f,
                    -SCREEN_HEIGHT / 2 + 320.0f,

                    //サイズ width hight
                    240.0f,
                    240.0f,

                    //角度
                    0.0f,

                    //色
                    MkF4(1.0f, 1.0f, 1.0f, movieFlagAlpha),

                    m_Tex_Btn_ZL.textureId,  //画像(テクスチャ)の番号
                    MkF2(0.0f, 0.0f),
                    MkF2(1.0f, 1.0f),
                    DRAW_POLYGON_PRIORITY_FORCE_NORMAL,
                    DPD_SHADER_TYPE_UI_BG_GENERAL
                );
                //DrawSpriteInCameraSingle(
                //    i,
                //    //表示座標 (四角形の真ん中)
                //    -SCREEN_WIDTH / 2 + 150.0f,
                //    -SCREEN_HEIGHT / 2 + 360.0f,

                //    //サイズ width hight
                //    1.5f * 260.0f * std::min(((float)m_TutorialOutPushBtn / STAGE_MAIN_WAIT_END_FADE), 1.0f),
                //    10.0f,

                //    //角度
                //    0.0f,

                //    //色
                //    MkF4(2.0f, 2.0f, 2.0f, 0.75f),

                //    0,  //画像(テクスチャ)の番号
                //    MkF2(0.0f, 0.0f),
                //    MkF2(1.0f, 1.0f)
                //);

                {
                    float m = std::min((
                        (float)m_BecomeMainPushBtn / TUTORIAL_OUT_PUSH_BTN_FRAME),
                        1.0f);

                    Float2 siz = { 450.0f, 450.0f };

                    for (int j = 0; j < 3; j++)
                    {
                        //if (j == 2 && m <= 0)
                        //{
                        //    continue;
                        //}

                        float m2 = 1.0f;
                        if (j == 1)
                        {
                            m2 = m;
                        }

                        DrawSpriteInCameraSingle(
                            i,
                            //表示座標 (四角形の真ん中)
                            //SCREEN_WIDTH / 2 - 220.0f,
                            10.0f + (-siz.x / 2 * 0.85f * (1.0f - m2)),
                            -SCREEN_HEIGHT / 2 + 80.0f,

                            //サイズ width hight
                            siz.x * m2,
                            siz.y * ((float)m_Tex_Bar_ZR[j].height / m_Tex_Bar_ZR[j].width),

                            //角度
                            0.0f,

                            //色
                            MkF4(1.0f, 1.0f, 1.0f, movieFlagAlpha),

                            m_Tex_Bar_ZR[j].textureId,  //画像(テクスチャ)の番号
                            MkF2(0.0f, 0.0f),
                            /*j == 1 ? MkF2(1.0f * m2, 1.0f) : */MkF2(1.0f, 1.0f),
                            DRAW_POLYGON_PRIORITY_FORCE_NORMAL,
                            j != 0 ? DPD_SHADER_TYPE_NONE : DPD_SHADER_TYPE_UI_BG_GENERAL
                        );
                    }
                    DrawSpriteInCameraSingle(
                        i,
                        //表示座標 (四角形の真ん中)
                        //SCREEN_WIDTH / 2 - 220.0f,
                        60.0f,
                        -SCREEN_HEIGHT / 2 + 85.0f,

                        //サイズ width hight
                        160.0f,
                        160.0f,

                        //角度
                        0.0f,

                        //色
                        MkF4(1.0f, 1.0f, 1.0f, movieFlagAlpha),

                        m_Tex_Text_ZR.textureId,  //画像(テクスチャ)の番号
                        MkF2(0.0f, 0.0f),
                        MkF2(1.0f, 1.0f),
                        DRAW_POLYGON_PRIORITY_FORCE_NORMAL,
                        DPD_SHADER_TYPE_UI_BG_GENERAL
                    );
                    DrawSpriteInCameraSingle(
                        i,
                        //表示座標 (四角形の真ん中)
                        //SCREEN_WIDTH / 2 - 220.0f,
                        -100.0f,
                        -SCREEN_HEIGHT / 2 + 85.0f,

                        //サイズ width hight
                        100.0f,
                        100.0f,

                        //角度
                        0.0f,

                        //色
                        MkF4(1.0f, 1.0f, 1.0f, movieFlagAlpha),

                        m_Tex_Btn_ZR[((m > 0) ? 1 : 0)].textureId,  //画像(テクスチャ)の番号
                        MkF2(0.0f, 0.0f),
                        MkF2(1.0f, 1.0f)
                    );
                }
            }
        }
    }

    draw_ConfirmMenu();

    //if (m_Main_Wait_Flag && m_Main_Wait_EndFadeFlag <= 0)
    //{
    //    DrawSpriteInCamera(
    //        //表示座標 (四角形の真ん中)
    //        -SCREEN_WIDTH / 2 + 190.0f,
    //        -SCREEN_HEIGHT / 2 + 60.0f,

    //        //サイズ width hight
    //        350.0f,
    //        350.0f * ((float)m_Tex_stageName.height / m_Tex_stageName.width),

    //        //角度
    //        0.0f,

    //        //色
    //        MkF4(1.0f, 1.0f, 1.0f, 1.0f),

    //        m_Tex_stageName.textureId,  //画像(テクスチャ)の番号
    //        MkF2(0.0f, 0.0f),
    //        MkF2(1.0f, 1.0f)
    //    );
    //    {
    //        TEX_DAT tex_diff =
    //            m_Diff == Difficulty::Hard
    //            ? m_Tex_danger_high : m_Tex_danger_middle;
    //        DrawSpriteInCamera(
    //            //表示座標 (四角形の真ん中)
    //            -SCREEN_WIDTH / 2 + 150.0f,
    //            -SCREEN_HEIGHT / 2 + 140.0f,

    //            //サイズ width hight
    //            260.0f,
    //            260.0f * ((float)tex_diff.height / tex_diff.width),

    //            //角度
    //            0.0f,

    //            //色
    //            MkF4(1.0f, 1.0f, 1.0f, 1.0f),

    //            tex_diff.textureId,  //画像(テクスチャ)の番号
    //            MkF2(0.0f, 0.0f),
    //            MkF2(1.0f, 1.0f)
    //        );
    //    }

    //    DrawSpriteInCamera(
    //        //表示座標 (四角形の真ん中)
    //        SCREEN_WIDTH / 2 - 280.0f,
    //        -SCREEN_HEIGHT / 2 + 160.0f,

    //        //サイズ width hight
    //        525.0f,
    //        525.0f * ((float)m_Tex_Operation_description.height / m_Tex_Operation_description.width),

    //        //角度
    //        0.0f,

    //        //色
    //        MkF4(1.0f, 1.0f, 1.0f, 1.0f),

    //        m_Tex_Operation_description.textureId,  //画像(テクスチャ)の番号
    //        MkF2(0.0f, 0.0f),
    //        MkF2(1.0f, 1.0f)
    //    );

    //    for (int i = 0; i < PLAYER_MAX; i++)
    //    {
    //        if (i == 0)
    //        {
    //            DrawSpriteInCameraSingle(
    //                i,
    //                //表示座標 (四角形の真ん中)
    //                -SCREEN_WIDTH / 2 + 150.0f,
    //                -SCREEN_HEIGHT / 2 + 320.0f,

    //                //サイズ width hight
    //                260.0f,
    //                260.0f * ((float)m_Tex_menuback.height / m_Tex_menuback.width),

    //                //角度
    //                0.0f,

    //                //色
    //                MkF4(1.0f, 1.0f, 1.0f, 1.0f),

    //                m_Tex_menuback.textureId,  //画像(テクスチャ)の番号
    //                MkF2(0.0f, 0.0f),
    //                MkF2(1.0f, 1.0f)
    //            );
    //            DrawSpriteInCameraSingle(
    //                i,
    //                //表示座標 (四角形の真ん中)
    //                -SCREEN_WIDTH / 2 + 150.0f,
    //                -SCREEN_HEIGHT / 2 + 360.0f,

    //                //サイズ width hight
    //                1.5f * 260.0f * std::min(((float)m_TutorialOutPushBtn / STAGE_MAIN_WAIT_END_FADE),1.0f),
    //                10.0f,

    //                //角度
    //                0.0f,

    //                //色
    //                MkF4(2.0f, 2.0f, 2.0f, 0.75f),

    //                0,  //画像(テクスチャ)の番号
    //                MkF2(0.0f, 0.0f),
    //                MkF2(1.0f, 1.0f)
    //            );

    //            DrawSpriteInCameraSingle(
    //                i,
    //                //表示座標 (四角形の真ん中)
    //                //SCREEN_WIDTH / 2 - 220.0f,
    //                10.0f,
    //                SCREEN_HEIGHT / 2 - 80.0f,

    //                //サイズ width hight
    //                500.0f,
    //                500.0f * ((float)m_Tex_ready.height / m_Tex_ready.width),

    //                //角度
    //                0.0f,

    //                //色
    //                MkF4(1.0f, 1.0f, 1.0f, 1.0f),

    //                m_Tex_ready.textureId,  //画像(テクスチャ)の番号
    //                MkF2(0.0f, 0.0f),
    //                MkF2(1.0f, 1.0f)
    //            );
    //            DrawSpriteInCameraSingle(
    //                i,
    //                //表示座標 (四角形の真ん中)
    //                10.0f,
    //                SCREEN_HEIGHT / 2 - 150.0f,

    //                //サイズ width hight
    //                1.5f * 500.0f * std::min(((float)m_BecomeMainPushBtn / STAGE_MAIN_WAIT_END_FADE), 1.0f),
    //                10.0f,

    //                //角度
    //                0.0f,

    //                //色
    //                MkF4(2.0f, 2.0f, 2.0f, 0.75f),

    //                0,  //画像(テクスチャ)の番号
    //                MkF2(0.0f, 0.0f),
    //                MkF2(1.0f, 1.0f)
    //            );
    //        }
    //    }
    //}
}

void StageManager::update_ConfirmMenu(void)
{
    m_Ttrl_Confirm.Update();
    switch (m_Ttrl_Confirm.GetStateTrigger())
    {
    case UI_Confirm::StateTrigger::Yes:
        SoundEffectPlayRequest(SOUND_TYPE_SE_UI_OK, 4.0f, 0.0f);

        m_Main_Wait_EndFlag = true;
        SetSceneFade(SCENE_CHARACTERSELECT, FADE_TYPE_STAR_WIPE);
        break;
    case UI_Confirm::StateTrigger::No:
        SoundEffectPlayRequest(SOUND_TYPE_SE_UI_CANCEL, 4.0f, 0.0f);

        GameManagerSetMovieFlag(false);
        m_TutorialOut_ConFlag = false;
        break;
    default:
        break;
    }
}
void StageManager::draw_ConfirmMenu(void)
{
    if (!m_TutorialOut_ConFlag)
    {
        return;
    }
    m_Ttrl_Confirm.Draw();
}