// ===================================================
// GOE_Character_Enemy_Boss_Kontoriusu.cpp キャラクターオブジェクト
// 
// hara sougo  2025/01/25
// ===================================================

//#define NO_CONTROLLER

#include "../../../../Game/Configuration/Param.h"
#include "../../../../game_main.h"
#include "../../../Sprite/sprite.h"
#include "../../../Texture/texture.h"
#include "../../../Polygon/modelData.h"
#include "../../../../Game/Entities/Block/block.h"
#include "../../../Systems/collision.h"
#include "../../../Systems/easing.h"

#include "../../../Managers/gameManager.h"
#include "../../../Managers/scene.h"
#include "../../../../Scenes/SCENE_GAME.h"

#include "../../gameObject.h"
#include "../../gameObjectPrefab.h"

#include "../../../Systems/camera.h"
#include "../../../Systems/font.h"
#include "../../../Systems/ui.h"
#include "../../../Systems/ui_Game.h"
#include "../../../Game Stage/Stage_Base.h"

#include "../../../Entities/character.h"
#include "../../../Entities/player.h"

#include "../../../Managers/soundManager.h"
#include "../../../Game Events/GE_Gaya.h"

#include "../GOE_Effect.h"
#include "../GOE_Character.h"

#include "../GOE_Character_Player.h"
#include "../GOE_ObjectDynamic.h"

//#include "GOE_Character_Enemy_Miresu.h"
//#include "GOE_Character_Enemy_Regatosu.h"
#include "GOE_Character_Enemy_Rucs.h"

#include "GOE_Character_EnemyProjectile.h"
#include "GOE_Character_Enemy_Boss_Kontoriusu.h"

// ===================================================
// マクロ定義
// ===================================================

// ===================================================
// プロトタイプ宣言
// ===================================================

// ===================================================
// グローバル変数
// ===================================================



void GOE_Character_Enemy_Boss_Kontoriusu::BecomeHard()
{
    m_IsHard = true;

    for (int i = 0; i < (int)HeadState::Max; i++)
    {
        if (m_Tex_Model_Head[i].enable)
        {
            UnloadTexData(m_Tex_Model_Head[i]);
        }
    }
    m_Tex_Model_Head[(int)HeadState::None] = LoadTexData("TEX/enemy/boss_kontoriusu/head/2_none");
    m_Tex_Model_Head[(int)HeadState::Idle] = LoadTexData("TEX/enemy/boss_kontoriusu/head/2_idle");
    m_Tex_Model_Head[(int)HeadState::W] = LoadTexData("TEX/enemy/boss_kontoriusu/head/2_w");
    m_Tex_Model_Head[(int)HeadState::Eye] = LoadTexData("TEX/enemy/boss_kontoriusu/head/2_eye");
    for (int i = 0; i < ((int)HeadState::Num_9 - (int)HeadState::Num_0) + 1; i++)
    {
        m_Tex_Model_Head[(int)HeadState::Num_0 + i] = LoadTexData(
            "TEX/enemy/boss_kontoriusu/head/2_num_" + std::to_string(i)
        );
    }

    if (m_Tex_Model.enable)
    {
        UnloadTexData(m_Tex_Model);
    }
    m_Tex_Model = LoadTexData("TEX/enemy/boss_kontoriusu/main_2");
    m_shaderType = DPD_SHADER_TYPE_METAL_ENEMY_2;

    if (m_Tex_StartingEff_Name.enable)
    {
        UnloadTexData(m_Tex_StartingEff_Name);
    }
    m_Tex_StartingEff_Name = LoadTexData("TEX/effect/boss/tex_inp_name_04");

    //Character::SetStatusData(m_Obj, Character::GetStatusDataFindByName(HARD_STATUS_NAME));

    Character::STATUS_DAT sttdat = Character::GetStatusDataFindByName(HARD_STATUS_NAME);

    m_Obj->status.hpMax = sttdat.status.hpMax;
    m_Obj->status.hp = m_Obj->status.hpMax;
    CalcHpMod(m_Obj, m_RetryCount);

    m_Obj->status.fullName = sttdat.status.fullName;
    m_Obj->status.nickName = sttdat.status.nickName;
}
// ===================================================
// Init
// オブジェクトが生成されたとき
// ===================================================
void GOE_Character_Enemy_Boss_Kontoriusu::Init()
{
	SuperGOE::Init();

    m_Tex_Model_Head[(int)HeadState::None] = LoadTexData("TEX/enemy/boss_kontoriusu/head/1_none");
    m_Tex_Model_Head[(int)HeadState::Idle] = LoadTexData("TEX/enemy/boss_kontoriusu/head/1_idle");
    m_Tex_Model_Head[(int)HeadState::W] = LoadTexData("TEX/enemy/boss_kontoriusu/head/1_w");
    m_Tex_Model_Head[(int)HeadState::Eye] = LoadTexData("TEX/enemy/boss_kontoriusu/head/1_w");
    for (int i = 0; i < ((int)HeadState::Num_9 - (int)HeadState::Num_0) + 1; i++)
    {
        m_Tex_Model_Head[(int)HeadState::Num_0 + i] = LoadTexData(
            //"TEX/enemy/boss_kontoriusu/head/1_num_" + std::to_string(i)
            "TEX/enemy/boss_kontoriusu/head/1_w"
        );
    }

    m_Tex_Model = LoadTexData("TEX/enemy/boss_kontoriusu/main_1");
    //m_shaderType = DPD_SHADER_TYPE_METAL_ENEMY;
    m_shaderType = DPD_SHADER_TYPE_METAL_ENEMY_2;

    m_ModelIndex[0] = GetModelDataIndex("MODEL/boss_kontoriusu/core");
    m_ModelIndex[1] = GetModelDataIndex("MODEL/boss_kontoriusu/body_r");
    m_ModelIndex[2] = GetModelDataIndex("MODEL/boss_kontoriusu/body_l");
    m_ModelIndex[3] = GetModelDataIndex("MODEL/boss_kontoriusu/arm_r");
    m_ModelIndex[4] = GetModelDataIndex("MODEL/boss_kontoriusu/arm_l");
    m_ModelIndex[5] = GetModelDataIndex("MODEL/boss_kontoriusu/head");
    m_ModelIndex[6] = GetModelDataIndex("MODEL/boss_kontoriusu/helmet");
    m_ModelIndex[7] = GetModelDataIndex("MODEL/boss_kontoriusu/hammer");

    m_Tex_Model_Blade = LoadTexData("TEX/effect/tex_blade");

    m_Tex_Shadow = LoadTexData("TEX/effect/shadow");

    m_Tex_Dis = LoadTexData("TEX/effect/NoiseTile");
    m_Tex_Warp = LoadTexData("TEX/effect/NoiseTile2");

    m_Tex_cursor_000 = LoadTexData("TEX/ui/in_game_master/cursor/cursor_000");
    m_Tex_cursor_010 = LoadTexData("TEX/ui/in_game_master/cursor/cursor_010");
    m_Tex_cursor_020 = LoadTexData("TEX/ui/in_game_master/cursor/cursor_020");


    m_Tex_AttackArea = LoadTexData("TEX/effect/tex_ex_attack").textureId;
    m_Model_Wave = GetModelDataIndex("MODEL/effect/legatos_wave01");

    m_ShaderIndex_CutInName_Vertex = DX_GetShaderIndex("SHADER/vs_general");
    m_ShaderIndex_CutInName_Pixel = DX_GetShaderIndex("SHADER/general/ps_cutin_name");

    m_Tex_StartingEff_Bk1 = LoadTexData("TEX/effect/boss/tex_inp_bk_01");
    m_Tex_StartingEff_Bk2 = LoadTexData("TEX/effect/boss/tex_inp_bk_02");
    m_Tex_StartingEff_Name = LoadTexData("TEX/effect/boss/tex_inp_name_03");
    //エフェクトとか

    //スプライト非表示
    m_Obj->graphic.hidden = true;


    m_Obj->graphic.shaderOpt.outline = DPD_SHADER_OPT_OUTLINE_NONE;

    m_Obj->collision.atk.enable = false;
    m_Obj->collision.rec.enable = false;
    m_Obj->status.statusType = GAME_OBJECT_STATUS_TYPE_VOID;

    m_Obj->size = MkF2(ONE_BLOCK * 2, ONE_BLOCK * 2);

    for (int i = 0; i < MODEL_NUM; i++)
    {
        m_ModelTransSca[i].x = 1.0f;
        m_ModelTransSca[i].y = 1.0f;
        m_ModelTransSca[i].z = 1.0f;
        m_ModelTransScaStart[i].x = 1.0f;
        m_ModelTransScaStart[i].y = 1.0f;
        m_ModelTransScaStart[i].z = 1.0f;
    }


    //起動前ポーズ
    //{
    //    m_ModelTransPos[0].y = 350.0f;
    //    m_ModelTransPos[0].z = 75.0f;

    //    m_ModelTransPos[1].y = 100.0f;

    //    m_ModelTransPos[3].x = 60.0f;
    //    m_ModelTransPos[3].y = 550.0f;
    //    m_ModelTransRot[3].x = 360.0f * 1;
    //    m_ModelTransRot[3].z = -35.0f;

    //    m_ModelTransPos[5].x = -80.0f;
    //    m_ModelTransPos[5].y = 550.0f;
    //    m_ModelTransRot[5].x = 360.0f * 1;
    //    m_ModelTransRot[5].z = 45.0f;
    //}
    {
        GAME_OBJECT_COLLISION_RECIEVE_CHILD* col = m_Obj->collision.rec.collision + 0;
        col->size = MkF3(ONE_BLOCK * 6.0f, ONE_BLOCK * 6.0f, ONE_BLOCK * 10.0f);
    }

    Character::SetStatusData(m_Obj, Character::GetStatusDataFindByName(NORMAL_STATUS_NAME));

    m_BeforeVecBuf = { 0.0f, 1.0f, 0.0f };
    m_BeforeVec = m_BeforeVecBuf;


    {
        Stage_Base* stg = GetCurrentStageSceneGame();
        if (stg != nullptr)
        {
            m_RetryCount = stg->GetRetryCountCurrentWave();
        }
    }

    CalcHpMod(m_Obj, m_RetryCount);

    StartBattle();
}

// ===================================================
// FirstFrame
// 最初のフレームのみ実行
// ===================================================
void GOE_Character_Enemy_Boss_Kontoriusu::FirstFrame()
{
	SuperGOE::FirstFrame();


    //if (m_StartMotionBeforeShader != -1)
    //{
    //    SetPostEffectShader(m_StartMotionBeforeShader);
    //}
    m_StartMotionShaderFlag = false;

    //判定有効化
    m_Obj->collision.atk.enable = true;
    m_Obj->collision.rec.enable = true;
    m_Obj->status.statusType = GAME_OBJECT_STATUS_TYPE_ENEMY_BOSS;
    m_Obj->graphic.shaderOpt.outline = DPD_SHADER_OPT_OUTLINE_NORMAL;

    //m_Obj->pos.z = 0.0f;

}

// ===================================================
// Uninit
// オブジェクトが削除されるとき
// ===================================================
void GOE_Character_Enemy_Boss_Kontoriusu::Uninit()
{
    UnloadTexData(m_Tex_StartingEff_Bk1);
    UnloadTexData(m_Tex_StartingEff_Bk2);
    UnloadTexData(m_Tex_StartingEff_Name);


    UnloadTexData(m_Tex_cursor_000);
    UnloadTexData(m_Tex_cursor_010);
    UnloadTexData(m_Tex_cursor_020);

    for (int i = 0; i < (int)HeadState::Max; i++)
    {
        UnloadTexData(m_Tex_Model_Head[i]);
    }
    UnloadTexData(m_Tex_Model);
    UnloadTexData(m_Tex_Model_Blade);

	SuperGOE::Uninit();
}

// ===================================================
// Update
// 毎フレーム実行
// ===================================================
void GOE_Character_Enemy_Boss_Kontoriusu::Update()
{
	SuperGOE::Update();

    if (!m_StartedFlag)
    {
        //開始フラグがたったら初回イベントを動かす
        if (m_StartFlag)
        {
            update_start();
        }
    }

    

    if (m_FastMoveFrame >= 0)
    {
        if (m_FastMoveFrame == 0)
        {
            //GameObjectMoveRequest(m_Obj,
            //    m_FastMoveFrameMax / 2,
            //    m_FastMovePosAfter,
            //    EasingInOutSine
            //);
            //m_Obj->pos = m_FastMovePosAfter;
            m_CurrentAxisPrivate = m_FastMoveAxisAfter;

            {
                float spd = (abs(m_BeforeVecBuf.x) + abs(m_BeforeVecBuf.y)) / 2;
                float rad = CalculateDegToRad(m_CurrentAxisPrivate);
                m_BeforeVecBuf.x = -sin(rad) * spd;
                m_BeforeVecBuf.y = cos(rad) * spd;

                m_BeforeVec = m_BeforeVecBuf;
                m_HeadVecBuf = m_BeforeVec;
                m_HeadVec = m_HeadVecBuf;
            }
        }

        if (m_FastMoveFrame <= m_FastMoveFrameMax / 2)
        {
            float mod = (float)m_FastMoveFrame / (m_FastMoveFrameMax / 2);
            m_Obj->pos = m_FastMovePosBefore
                + (m_FastMovePosAfter - m_FastMovePosBefore) * mod;
        }

        if (m_FastMoveFrame == static_cast<int>(m_FastMoveFrameMax / 2))
        {
            if (m_FastMoveMiddleFunc != nullptr)
            {
                m_FastMoveMiddleFunc();
            }
        }

        m_FastMoveFrame++;
        if (m_FastMoveFrame > m_FastMoveFrameMax)
        {
            m_FastMoveFrame = -1;
        }
    }
    if (m_WarpFrame >= 0)
    {
        if (m_WarpFrame == 0)
        {
            m_Obj->pos = m_WarpPosAfter;
            m_CurrentAxisPrivate = m_WarpAxisAfter;

            {
                float spd = (abs(m_BeforeVecBuf.x) + abs(m_BeforeVecBuf.y)) / 2;
                float rad = CalculateDegToRad(m_CurrentAxisPrivate);
                m_BeforeVecBuf.x = -sin(rad) * spd;
                m_BeforeVecBuf.y = cos(rad) * spd;

                m_BeforeVec = m_BeforeVecBuf;
                m_HeadVecBuf = m_BeforeVec;
                m_HeadVec = m_HeadVecBuf;
            }
        }

        if (m_WarpFrame == static_cast<int>(m_WarpFrameMax / 2))
        {
            if (m_WarpMiddleFunc != nullptr)
            {
                m_WarpMiddleFunc();
            }
        }

        m_WarpFrame++;
        if (m_WarpFrame > m_WarpFrameMax)
        {
            m_WarpFrame = -1;
        }
    }
}

// ===================================================
// UpdateActive
// 毎フレーム実行
// ただし、ヒットストップ中などの
// 非アクティブ時は実行されない
// ===================================================
void GOE_Character_Enemy_Boss_Kontoriusu::UpdateActive()
{
    if (m_FastMoveFrame >= 0)
    {
        return;
    }

    if (m_WarpFrame >= 0)
    {
        return;
    }

	SuperGOE::UpdateActive();


    float turnspeed = 0.03f;

    //開始前はいろいろ実行しない
    if (m_StartedFlag)
    {

        { //向きの設定

            //向きを計算
            constexpr float rotp = 360.0f / 64;
            float rot =
                DegNormalize(
                    CalculateRadToDeg(
                        atan2(m_BeforeVec.y, m_BeforeVec.x))
                    + 180.0f + 90.0f + rotp / 2
                );

            //動いていたら向きに合わせて方向をセット
            if (abs(m_BeforeVec.x) > 0.01f || abs(m_BeforeVec.y) > 0.01f)
            {
                m_CurrentAxisPrivate = (int)(rot / rotp) * rotp;
                //m_CurrentAxisHead = (int)(rot / rotp) * rotp;
            }

            //DebugPrintf("%f  %f\n", rot, (rot / rotp));
        }
        { //向きの設定

            //向きを計算
            constexpr float rotp = 360.0f / 64;
            float rot =
                DegNormalize(
                    CalculateRadToDeg(
                        atan2(m_HeadVec.y, m_HeadVec.x))
                    + 180.0f + 90.0f + rotp / 2
                );

            //動いていたら向きに合わせて方向をセット
            if (abs(m_HeadVec.x) > 0.01f || abs(m_HeadVec.y) > 0.01f)
            {
                m_CurrentAxisHead = (int)(rot / rotp) * rotp;
            }

            //DebugPrintf("%f  %f\n", rot, (rot / rotp));
        }

        if (m_Obj->status.deadFlag)
        { //たひ
            if (m_Dead_Frame == 0)
            {
                //プレイヤー以外を削除する
                for (int i = 0; i < GAME_OBJECT_MAX; i++)
                {
                    GAME_OBJECT* ob = GetGameObject(i);
                    if (IsActiveGameObject(ob)
                        && ob != m_Obj
                        && (ob->tags.size() <= 0
                            || (
                                !ob->tags[0].find("PLAYER") == 0
                                && !ob->tags[0].find("ENEMY_BOSS_INPERIUSU") == 0
                                )
                            )
                        && ob->status.statusType
                            != GAME_OBJECT_STATUS_TYPE_ENEMY_BOSS
                    )
                    {
                        DeleteGameObject(ob);
                    }
                }
                m_Obj->hitReaction = {};
                m_Obj->hitStop = {};
                m_Obj->moveRequ.enable = false;

                //ボスのHPを削り切ったらリザルトに送る
                //if (HasTagGameObject(m_Obj, "ENEMY_BOSS_INPERIUSU"))
                //{
             //       ShowBossbar_Game(GetGameObjectIndex(m_Obj), false, "");

                //	SoundEffectPlayRequest(SOUND_TYPE_SE_9, 15.0f, 0.0f);

                //	GameStopRequest(60);
                //	GameManagerSetSceneRequest(SCENE_RESULT, 60, true);
                //	//SetSceneFade(SCENE_RESULT);

                    //CameraZoomRequestAll(
                    //    0.04f,
                    //    MkF3(
                    //        m_Obj->pos.x,
                    //        m_Obj->pos.y,
                    //        1000.0f
                    //    ),
                    //    //m_Obj->pos,
                    //    EasingInOutSine,
                    //    0.65f
                    //);
                    //CameraZoomResetRequestAll(
                    //    0.02f,
                    //    EasingInOutSine,
                    //    60
                    //);
             //       SetCameraModeAll(CAMERA_MODE_NORMAL);
                //}

                ShowBossbar_Game(GetGameObjectIndex(m_Obj), false, "");

                GameManagerResultAddScore(
                    5000,
                    RESULT_SCORE_EX_TYPE_ENEMY_DEFEAT
                );

                SoundBGMStopRequest();
                SoundEffectStopAllRequest();

                SoundEffectPlayRequest(
                    GetSoundData("SOUND/SE/player/attack/activation"),
                    4.0f, 0.0f, 0.75f
                );
                SoundEffectPlayRequest(SOUND_TYPE_SE_9, 5.0f, 0, 0.9f);
                SoundEffectPlayRequest(SOUND_TYPE_SE_9, 3.0f, 60, 0.9f);
                SoundEffectPlayRequest(SOUND_TYPE_SE_9, 1.0f, 120, 0.9f);

                m_BeforeVecBuf = NormalizeVector({ 0.0f, 1.0f, 0.0f });
                m_BeforeVec = m_BeforeVecBuf;

                m_CurrentState = ActionState::Idle;

                //StartAnim(AnimState::Idle, m_Dead_FrameMax, true);

                StartAnim(AnimState::End,
                    32768,
                    true);

                SetCameraModeAll(CAMERA_MODE_NORMAL);

                GE_GayaCancelRequestAll();

                //for (int i = 0; i < PLAYER_MAX; i++)
                //{
                //    PLAYER* pl = GetPlayer(i);
                //    if (pl != nullptr)
                //    {
                //        GAME_OBJECT* plObj = pl->obj;
                //        if (plObj != nullptr)
                //        {
                //            plObj->pos.z = 32767.0f;
                //        }
                //    }
                //}

                Float3 zoomPos = m_Obj->pos;
                //zoomPos.z += 32000.0f;
                zoomPos.z += 30000.0f;

                Float3 zoomPosTa = m_Obj->pos;
                //zoomPosTa.z += -14000.0f;
                zoomPosTa.z += -9000.0f;

                CameraTargetMoveRequestAll(8,
                    zoomPosTa,
                    EasingInOutSine,
                    CAMERA_MOVE_TYPE_SMOOTH
                );
                CameraMoveRequestAll(8,
                    zoomPos,
                    0.0f,
                    MkF2(1.0f, 1.0f),
                    //0.35f,
                    0.5f,
                    EasingInOutSine,
                    CAMERA_MOVE_TYPE_SMOOTH
                );
            }
            //if (m_Dead_Frame == 0)
            //{
            //    for (int i = 0; i < PLAYER_MAX; i++)
            //    {
            //        PLAYER* pl = GetPlayer(i);
            //        if (pl != nullptr)
            //        {
            //            GAME_OBJECT* plObj = pl->obj;
            //            if (plObj != nullptr)
            //            {
            //                plObj->pos.z = 32767.0f;
            //            }
            //        }
            //    }

            //    Float3 zoomPos = m_Obj->pos;
            //    zoomPos.z += 32000.0f;

            //    Float3 zoomPosTa = m_Obj->pos;
            //    zoomPosTa.z += -14000.0f;

            //    CameraTargetMoveRequestAll(8,
            //        zoomPosTa,
            //        EasingInOutSine,
            //        CAMERA_MOVE_TYPE_SMOOTH
            //    );
            //    CameraMoveRequestAll(8,
            //        zoomPos,
            //        0.0f,
            //        MkF2(1.0f, 1.0f),
            //        //0.35f,
            //        0.5f,
            //        EasingInOutSine,
            //        CAMERA_MOVE_TYPE_SMOOTH
            //    );
            //}
            //if (m_Dead_Frame == 8)
            //{
            //    Float3 zoomPos = m_Obj->pos;
            //    zoomPos.x += 320.0f;
            //    zoomPos.z += 32000.0f;

            //    Float3 zoomPosTa = m_Obj->pos;
            //    zoomPosTa.x += -7500.0f;
            //    zoomPosTa.z += -17000.0f;

            //    CameraTargetMoveRequestAll(m_Dead_FrameMax,
            //        zoomPosTa,
            //        nullptr,
            //        CAMERA_MOVE_TYPE_SMOOTH
            //    );
            //    CameraMoveRequestAll(m_Dead_FrameMax,
            //        zoomPos,
            //        0.0f,
            //        MkF2(1.0f, 1.0f),
            //        //0.35f,
            //        0.55f,
            //        nullptr,
            //        CAMERA_MOVE_TYPE_SMOOTH
            //    );
            //}
            if (m_Dead_Frame == m_Dead_FrameMax)
            {
                GameManagerSetMoviePlayerHiddenFlag(false);
                SetCameraModeAll(CAMERA_MODE_PLAYER_CHASE);
                DeleteGameObject(m_Obj);
                return;
            }

            m_Dead_Frame++;
        }
        else if (m_SimpleMoveFrame >= 0)
        {
            if (m_SimpleMoveFrame == 0)
            { //初回

                GameObjectMoveRequest(m_Obj, m_SimpleMoveFrameMax,
                    m_SimpleMovePosAfter,
                    EasingInOutSine
                );
            }
            {
                m_BeforeVecBuf = NormalizeVector(m_Obj->vel);
            }

            m_SimpleMoveFrame++;
            if (m_SimpleMoveFrame >= m_SimpleMoveFrameMax)
            {
                if (abs(m_SimpleMoveAxisAfter - m_CurrentAxisPrivate) > 1.0f)
                {
                    StartTurn(m_SimpleMoveAxisAfter, 0.05f);
                }
                m_SimpleMoveFrame = -1;
            }

            //return;
        }
        else if (m_TurnSpeed > 0)
        {
            turnspeed = m_TurnSpeed;
            m_BeforeVecBuf = m_TurnVecAfter;

            if (
                abs(m_BeforeVecBuf.x - m_BeforeVec.x) < 0.1f
                && abs(m_BeforeVecBuf.y - m_BeforeVec.y) < 0.1f
                && abs(m_BeforeVecBuf.z - m_BeforeVec.z) < 0.1f
                )
            {
                m_BeforeVec = m_BeforeVecBuf;
                m_TurnSpeed = 0;
            }
        }
        else
        {
            //行動パターン
            {
                if (m_TargetObjIdentifier.objCreateFrame > 0)
                { //対象がいるとき

                    GAME_OBJECT* targetObject = GetGameObjectFindByIdentifier(m_TargetObjIdentifier);
                    if (targetObject != nullptr)
                    {
                        m_TargetDistance = CalculateDistance(targetObject->pos, m_Obj->pos);
                        m_TargetVec = NormalizeVector(MkF3(
                            targetObject->pos.x - m_Obj->pos.x,
                            targetObject->pos.y - m_Obj->pos.y,
                            0.0f
                        ));

                        {
                            //対象への向きを入れる
                            m_HeadVecBuf = NormalizeVector(MkF3(
                                targetObject->pos.x - m_Obj->pos.x,
                                targetObject->pos.y - m_Obj->pos.y,
                                //targetObject->pos.z - (m_Obj->pos.z - 120.0f * m_Obj->scale.y)
                                targetObject->pos.z - (m_Obj->pos.z
                                    + (m_ModelTransPos[5].y - 200.0f) * m_Obj->scale.y)

                            ));
                        }

                        if (m_ActionCooldown > 0)
                        {

                            //if (distance > ONE_BLOCK * 5.0f)
                            //{ //接近
                            //    m_Obj->vel.x += vec.x * ONE_BLOCK / 12;
                            //    m_Obj->vel.y += vec.y * ONE_BLOCK / 12;
                            //    m_Obj->vel.z += vec.z * ONE_BLOCK / 12;
                            //    //if (privateDat->targetFrame % 40 == 0)
                            //    //{
                            //    //	GameObjectMoveRequest(obj, 60,
                            //    //		MkF3(
                            //    //			obj->pos.x + vec.x * OneBlock * 2.0f,
                            //    //			obj->pos.y + vec.y * OneBlock * 2.0f,
                            //    //			obj->pos.z + vec.z * OneBlock * 2.0f
                            //    //		),
                            //    //		EasingInOutSine
                            //    //	);
                            //    //}
                            //}

                            m_ActionCooldown--;

                            //対象への向きを入れる

                            //m_BeforeVecBuf = vec;
                        }
                        else if (
                            m_CurrentState == ActionState::Idle
                            )
                        {
                            update_action_pattern();
                        }


                        //タゲ外し
                        if (IsDefeatGameObject(targetObject))
                        {
                            m_TargetObjIdentifier = {};
                            m_TargetFrame = -1;
                        }
                    }

                    //タゲはいってからのカウント
                    m_TargetFrame++;
                }
                else //対象がいないとき
                {
                    //索敵
                    {
                        //マス内のプレイヤーを検索
                        std::vector<PLAYER> players
                            = GetPlayesrFindByDistance(m_Obj->pos, ONE_BLOCK * 1000);

                        auto it = std::find_if(
                            players.begin(),
                            players.end(),
                            [](PLAYER buf) {
                                return IsDefeatGameObject(buf.obj);
                            }
                        );
                        if (it != players.end())
                        {
                            players.erase(it);
                        }


                        //マス以内にプレイヤーがいる
                        if (players.size() > 0)
                        {
                            //ターゲッティング
                            m_TargetObjIdentifier = GetIdentifierGameObject(players[0].obj);
                        }
                    }

                    m_BeforeVec = m_Obj->vel;
                }

                //if (m_AttackAct_Frame >= 0)
                //{
                //    update_attack();
                //}
                //行動
                switch (m_CurrentState)
                {
                case ActionState::Hammer:
                    update_hammer();
                    break;
                case ActionState::Fireworks:
                    update_fireworks();
                    break;
                case ActionState::Granade:
                    update_granade();
                    break;
                case ActionState::Laser:
                    //turnspeed = 0.1f;
                    update_laser();
                    break;
                case ActionState::SuperLaser:
                    update_superlaser();
                    break;

                case ActionState::Attack:
                    update_attack();
                    break;
                case ActionState::Test:
                    update_test();
                    break;
                default:break;
                }
            }

            m_StartedFrameCount++;
        }

        SetBeforeVec(
            &m_BeforeVec,
            &m_BeforeVecBuf,
            turnspeed
        );

        {
            Float3* bufp = &m_BeforeVecBuf;

            float headDeg = DegNormalize(
                DegNormalize(
                    CalculateRadToDeg(
                        atan2(m_HeadVecBuf.y, m_HeadVecBuf.x))
                    + 180.0f + 90.0f
                ) - m_CurrentAxisPrivate);

            float sikai = 30.0f;
            if (((headDeg >= 0 && headDeg < (90 - sikai))
                || (headDeg > (270 + sikai) && headDeg <= 360))
                && m_TargetDistance <= ONE_BLOCK * 256.0f
            )
            {
                bufp = &m_HeadVecBuf;
            }

            SetBeforeVec(
                &m_HeadVec,
                bufp,
                0.03f
            );
        }


        if (m_DeadFlag)
        {
            //{
            //    GAME_OBJECT* go = GO_CreateAfterEffect(m_Obj, 30, MkF4(1.0f, 1.0f, 1.0f, 1.0f));
            //    if (go != nullptr)
            //    {
            //        go->graphic.stop = false;
            //        go->graphic.graph.spriteAnim[0].texDat = m_Tex_Defeat;
            //        go->graphic.graph.spriteAnim[0].anim = true;
            //        go->graphic.graph.spriteAnim[0].xcnt = 6;
            //        go->graphic.graph.spriteAnim[0].hcnt = 6;
            //        go->graphic.graph.spriteAnim[0].cntMax = 6 * 6;
            //        go->graphic.graph.spriteAnim[0].size.x = 3240 / 6;
            //        go->graphic.graph.spriteAnim[0].size.y = 3240 / 6;
            //        go->graphic.graph.spriteAnim[0].pos3D = {};
            //        //go->graphic.graph.spriteAnim[0].pos3D.z += -720.0f;
            //    }
            //}
            //DeleteGameObject(m_Obj);
        }

    }

    {
        if (m_HammerVisibleFlag)
        {
            if (m_HammerVisibleFrame < m_HammerVisibleFrameMax)
            {
                m_HammerVisibleFrame++;
            }
        }
        else
        {
            if (m_HammerVisibleFrame > 0)
            {
                m_HammerVisibleFrame--;
            }
        }
    }

    { //あにむ


        //m_ModelTransSca[7].x = 0.0f;
        //m_ModelTransSca[7].y = 0.0f;
        //m_ModelTransSca[7].z = 0.0f;

        if (m_StartedFlag)
        {
            {
                {
                    for (int i = 0; i < MODEL_NUM; i++)
                    {
                        m_ModelTransPos[i] = {};
                        m_ModelTransRot[i] = {};
                        m_ModelTransPos[i].y = sinf((float)m_Obj->frameActive / 30) * 10.0f;
                    }

                    //ぼでー
                    {
                        float m = EasingInOutBounce(sinf((float)m_Obj->frameActive / 30)) * 12.0f;
                        m_ModelTransRot[1].z = -m;
                        m_ModelTransRot[2].z = m;
                    }

                    //腕
                    {
                        float m = sinf((float)m_Obj->frameActive / 30) * 5.0f;
                        m_ModelTransRot[3].z = -m;
                        m_ModelTransRot[4].z = m;
                    }

                    //頭の向き
                    m_ModelTransPos[5].x = m_HeadVec.x * 50.0f;
                    //m_ModelTransPos[0].z = (m_HeadVec.y - 1.0f) * -80.0f;
                    m_ModelTransRot[5].y = m_CurrentAxisHead - m_CurrentAxisPrivate;
                    m_ModelTransRot[5].x = m_HeadVec.z * 50.0f;
                    //m_ModelTransRot[5].z = 0.0f;
                    m_ModelTransPos[6] = m_ModelTransPos[5];
                    m_ModelTransRot[6] = m_ModelTransRot[5];
                }

                if (
                    m_SimpleMoveFrame >= 0
                    || m_FastMoveFrame >= 0
                    )
                {
                 //歩き
                    for (int i = 0; i < MODEL_NUM; i++)
                    {
                        m_ModelTransPos[i] = {};
                        m_ModelTransRot[i] = {};
                        m_ModelTransPos[i].y = sinf((float)m_Obj->frameActive / 12) * 6.0f;
                        m_ModelTransRot[i].x = 30.0f + sinf((float)m_Obj->frameActive / 12) * 2.0f;
                    }

                    //腕
                    {
                        float m = sinf((float)m_Obj->frameActive / 6) * 6.0f;
                        m_ModelTransRot[3].x += -m;
                        m_ModelTransRot[4].x += m;
                    }
                    //頭
                    {
                        float pm = -24.0f;
                        float rm = -12.0f;
                        m_ModelTransPos[5].z += pm;
                        m_ModelTransPos[6].z += pm;
                        m_ModelTransRot[5].x += rm;
                        m_ModelTransRot[6].x += rm;
                    }
                
                }
            }
        }

        if (m_Anim_Frame >= 0)
        {
            if (m_Anim_State == AnimState::Shoot
                || m_Anim_State == AnimState::ShootBefore
                || m_Anim_State == AnimState::ShootAfter
                )
            { //びいむ
                //Float2 vecH = NormalizeVector(MkF2(m_BeforeVec.x, m_BeforeVec.y));

                //{
                //    float vecHRad = atan2(vecH.x, vecH.y);

                //    m_Obj->graphic.rot3d = {};
                //    m_Obj->graphic.rot3d.y = CalculateRadToDeg(vecHRad) + 0.0f;
                //    m_Obj->graphic.rot3d.x = -90.0f * m_BeforeVec.z
                //        ;// -40.0f * ((float)m_AttackBeamCooldown / ATTACK_BEAM_COOLDOWN_FRAME);
                //}

                float kamaeF = ATTACK_BEAM_BEFORE_FIXED_FRAME;
                if (ATTACK_BEAM_BEFORE_FIXED_FRAME 
                    * (m_Anim_State == AnimState::Shoot ? 2:1)
                    > m_Anim_FrameMax
                    )
                {
                    kamaeF = m_Anim_FrameMax * 0.3f;
                }

                float mod1 = std::max(std::min((float)(m_Anim_Frame) / (kamaeF), 1.0f), 0.0f);
                float mod2 = 1.0f - std::max(std::min((float)(m_Anim_Frame - (m_Anim_FrameMax - kamaeF)) / (kamaeF), 1.0f), 0.0f);

                float mod = EasingInOutBack(mod1) * EasingInOutBack(mod2);
                if (m_Anim_State == AnimState::ShootBefore)
                {
                    mod = EasingInOutBack(mod1);
                }
                if (m_Anim_State == AnimState::ShootAfter)
                {
                    mod = EasingInOutBack(mod2);
                }

                for (int i = 0; i < MODEL_NUM; i++)
                {
                    m_ModelTransPosStart[i] = {};
                    m_ModelTransRotStart[i] = {};
                    m_ModelTransScaStart[i].x = 1.0f;
                    m_ModelTransScaStart[i].y = 1.0f;
                    m_ModelTransScaStart[i].z = 1.0f;

                    m_ModelTransPos[i] = {};
                    m_ModelTransRot[i] = {};
                    m_ModelTransRot[i].x = 16.0f * mod;

                    m_ModelTransPos[i].y = sinf((float)m_Obj->frameActive / 15) * 4.0f * mod;
                }

                //ぼでー
                {
                    float m = EasingInOutBounce(sinf((float)m_Obj->frameActive / 30)) * 12.0f * mod;
                    m_ModelTransRot[1].z = -m;
                    m_ModelTransRot[2].z = m;
                }

                //頭
                {
                    float pxm = -30.0f * mod;
                    float pym = -30.0f * mod;
                    float pzm = 50.0f * mod;
                    float rxm = 8.0f * mod;
                    float rym = -32.0f * mod;
                    for (int i = 0; i < 2; i++)
                    {
                        m_ModelTransPos[5 + i].x += pxm;
                        m_ModelTransPos[5 + i].y += pym;
                        m_ModelTransPos[5 + i].z += pzm;
                        m_ModelTransRot[5 + i].x += rxm;
                        m_ModelTransRot[5 + i].y += rym;
                    }
                }

                //腕
                {
                    m_ModelTransPos[3].x = 110.0f * mod;
                    m_ModelTransPos[3].y = -30.0f * mod;
                    m_ModelTransPos[3].z = -120.0f * mod;
                    m_ModelTransRot[3].x = -90.0f * mod;
                    m_ModelTransRot[3].y = -20.0f * mod;

                    m_ModelTransPos[4].x = 10.0f * mod;
                    m_ModelTransPos[4].z = 100.0f * mod;
                    m_ModelTransRot[4].x = -90.0f * mod;
                    m_ModelTransRot[4].y = 65.0f * mod;
                }
            }
            else if (
                m_Anim_State == AnimState::Hammer
                || m_Anim_State == AnimState::HammerBefore
            )
            { //攻撃
                float mod = m_Anim_Mod;
                float attackFrame = (float)m_Anim_Frame;
                float attackFrameMax = (float)m_Anim_FrameMax;
                float beforeMod = 1.0f;
                if (m_Anim_State == AnimState::HammerBefore)
                {
                    float kamaeF = ATTACK_HAMMER_BEFORE_FIXED_FRAME;
                    if (ATTACK_HAMMER_BEFORE_FIXED_FRAME > m_Anim_FrameMax)
                    {
                        kamaeF = m_Anim_FrameMax * 0.5f;
                    }

                    mod = m_Anim_Mod;
                    beforeMod = std::min((float)attackFrame / kamaeF, 1.0f);
                    attackFrame = 3.0f * (sinf(attackFrame * 2.0f) + 1.0f) / 2.0f;
                    attackFrameMax = 60;
                }
                else
                {
                    float waitf = 0.01f;
                    mod = EasingSideRange(
                        mod,
                        (float)ATTACK_HAMMER_ATTACKING_PRESS_MOD,
                        (float)ATTACK_HAMMER_ATTACKING_PRESS_MOD + waitf,
                        (float)ATTACK_HAMMER_ATTACKING_PRESSIN_MOD,
                        (float)ATTACK_HAMMER_ATTACKING_PRESSOUT_MOD
                    );
                    attackFrame = attackFrameMax * mod;
                    mod = attackFrame / attackFrameMax;
                }

                float modsi = (sinf(2.0f * SYS_GENERAL_PI * mod) + 1.0f) / 2.0f;
                float mod2 = std::min((float)attackFrame / (attackFrameMax / 3), 1.0f);
                mod2 = EasingInSine(mod2);
                float mod3 = std::max(std::min((float)(attackFrame - (attackFrameMax / 3)) / ((attackFrameMax / 3 * 2) - (attackFrameMax / 3)), 1.0f), 0.0f);
                float mod3si = (sinf(2.0f * SYS_GENERAL_PI * mod3) + 1.0f) / 2.0f;

                float mod4 = std::max(std::min((float)(attackFrame) / ((attackFrameMax / 3 * 2)), 1.0f), 0.0f);
                float mod5 = std::max(std::min((float)(attackFrame - (attackFrameMax / 3 * 2)) / (attackFrameMax - (attackFrameMax / 3 * 2)), 1.0f), 0.0f);


                for (int i = 0; i < MODEL_NUM; i++)
                {
                    m_ModelTransPosStart[i] = {};
                    m_ModelTransRotStart[i] = {};
                    m_ModelTransScaStart[i].x = 1.0f;
                    m_ModelTransScaStart[i].y = 1.0f;
                    m_ModelTransScaStart[i].z = 1.0f;

                    m_ModelTransPos[i] = {};
                    m_ModelTransRot[i] = {};

                    m_ModelTransRot[i].x = (-50.0f * beforeMod + 170.0f * mod2 + 10.0f * mod3si) * (1.0f - EasingOutCubic(mod5));
                    m_ModelTransRot[i].y = (80.0f * beforeMod + -150.0f * mod2 + -20.0f * mod3si) * (1.0f - EasingOutSine(mod5));
                    m_ModelTransRot[i].z = (20.0f * beforeMod + -40.0f * mod2 + 10.0f * mod3si) * (1.0f - EasingOutCubic(mod5));
                    ///m_Obj->graphic.graph.polygonModel[i].pos.y = -350.0f * (mod2 - EasingOutCubic(mod5));
                    m_ModelTransPos[i].y = -360.0f * (EasingInExpo(mod2) - EasingInCubic(mod5));
                }

                //身体
                {
                    float pym = -50.0f * (mod2 - EasingOutExpo(mod5));
                    float pzm = 40.0f * (mod2 - EasingOutExpo(mod5));
                    float rxm = 30.0f * (mod2 - EasingOutExpo(mod5)) + 50.0f * (1.0f - mod2);
                    float rym = 40.0f * (mod2 - EasingOutExpo(mod5)) + -5.0f * (1.0f - mod2);
                    for (int i = 0; i < 3; i++)
                    {
                        m_ModelTransPos[i].y += pym;
                        m_ModelTransPos[i].z += pzm;
                        m_ModelTransRot[i].x += rxm;
                        m_ModelTransRot[i].y += rym;
                    }
                }
                //頭
                {
                    float pxm = + -30.0f * (1.0f - mod2);
                    float pym = 130.0f * (mod2 - EasingOutExpo(mod5));
                    float pzm = 30.0f * (mod2 - EasingOutExpo(mod5));
                    float rxm = -50.0f * (mod2 - EasingOutExpo(mod5)) + 30.0f * (1.0f - mod2) * beforeMod;
                    float rym = 60.0f * (mod2 - EasingOutExpo(mod5)) + -30.0f * (1.0f - mod2) * beforeMod;
                    for (int i = 0; i < 2; i++)
                    {
                        m_ModelTransPos[5 + i].x += pxm;
                        m_ModelTransPos[5 + i].y += pym;
                        m_ModelTransPos[5 + i].z += pzm;
                        m_ModelTransRot[5 + i].x += rxm;
                        m_ModelTransRot[5 + i].y += rym;
                    }
                }

                //腕
                {
                    float rx = -260.0f * beforeMod + 170.0f * EasingInCubic(mod2) + mod3si * 8.0f;
                    float ry = 20.0f * beforeMod + -50.0f * EasingInCubic(mod2);
                    float rz = 0.0f + 0.0f * EasingInCubic(mod2);
                    float px = 30.0f * beforeMod + 40.0f * EasingInCubic(mod2);
                    float py = (sinf(1.0f * SYS_GENERAL_PI * EasingInCubic(mod2)) + 1.0f) / 2.0f * -1160.0f
                        * beforeMod
                        + (200.0f) * mod2 + 300.0f * beforeMod
                        + sinf((float)(attackFrameMax / 3)
                            * 1.8f * (mod3 > 0 ? 1 : 0)) * 10.0f 
                        + -30.0f * mod3si
                        ;
                    float pz = 240.0f * beforeMod + -500.0f * mod2;

                    float outmod = (1.0f - mod5);
                    for (int i = 0; i < 2; i++)
                    {
                        Float3& pmdPos = m_ModelTransPos[i == 1 ? 7 : 3];
                        Float3& pmdRot = m_ModelTransRot[i == 1 ? 7 : 3];
                        pmdPos.x = px * outmod;
                        pmdPos.y = py * outmod;
                        pmdPos.z = pz * outmod;
                        pmdRot.x = rx * outmod;
                        pmdRot.y = ry * outmod;
                        pmdRot.z = rz * outmod;
                    }
                    m_ModelTransPos[7].y += mod3si * 35.0f;

                    m_ModelTransSca[7] = { 1.0f, 1.0f, 1.0f };
                    //m_Obj->graphic.graph.polygonModel[7].size.x = 100.0f * (1.0f - mod5);
                    m_ModelTransSca[7].z = (1.0f + mod3si * -0.7f);
                }
            }

            else if (m_Anim_State == AnimState::GranadeBefore)
            { //
                m_HeadVecBuf = m_BeforeVec;
                m_HeadVec = m_HeadVecBuf;

                float mod = 1.0f - m_Anim_Mod;

                float kamaeF = m_Anim_FrameMax;

                float mod1 = EasingInOutBack(std::min((float)(m_Anim_Frame) / (kamaeF), 1.0f));
                if (mod1 >= 0.95f)
                {
                    mod1 += sin((float)m_Obj->frameActive * 1.0f) * 0.0025f;
                }

                for (int i = 0; i < MODEL_NUM; i++)
                {
                    m_ModelTransPosStart[i] = {};
                    m_ModelTransRotStart[i] = {};
                    m_ModelTransScaStart[i].x = 1.0f;
                    m_ModelTransScaStart[i].y = 1.0f;
                    m_ModelTransScaStart[i].z = 1.0f;

                    m_ModelTransPos[i] = {};
                    m_ModelTransRot[i] = {};
                }

                //ぼでー
                {
                    for (int i = 0; i < MODEL_NUM; i++)
                    {
                        m_ModelTransRot[i].y = -45.0f * mod1;
                        m_ModelTransRot[i].z = 5.0f * mod1;
                    }
                    m_ModelTransRot[5].x = -8.0f * mod1;
                    m_ModelTransRot[5].y = 25.0f * mod1;
                    m_ModelTransRot[6] = m_ModelTransRot[5];
                }

                //腕
                {
                    {
                        m_ModelTransPos[3].x = -80.0f * mod1;
                        m_ModelTransPos[3].y = -160.0f * mod1;
                        m_ModelTransPos[3].z = 10.0f * mod1;

                        m_ModelTransRot[3].x = -540.0f * mod1;
                        m_ModelTransRot[3].y = -90.0f * mod1;
                        m_ModelTransRot[3].z = 0.0f * mod1;
                    }
                    {
                        m_ModelTransPos[4].x = -40.0f * mod1;
                        m_ModelTransPos[4].y = 40.0f * mod1;
                        m_ModelTransPos[4].z = 80.0f * mod1;

                        m_ModelTransRot[4].x = -100.0f * mod1;
                        m_ModelTransRot[4].y = 50.0f * mod1;
                        m_ModelTransRot[4].z = 0.0f * mod1;
                    }
                }
            }
            else if (m_Anim_State == AnimState::Granade)
            { //
                m_HeadVecBuf = m_BeforeVec;
                m_HeadVec = m_HeadVecBuf;

                float mod = 1.0f - m_Anim_Mod;

                float kamaeF = ATTACK_BEAM_BEFORE_FIXED_FRAME;
                if (ATTACK_BEAM_BEFORE_FIXED_FRAME > m_Anim_FrameMax)
                {
                    kamaeF = m_Anim_FrameMax * 0.6f;
                }

                float mod1 = EasingInOutBack(
                    1.0f - std::max(
                        std::min(
                            (float)(m_Anim_Frame - (m_Anim_FrameMax - kamaeF)) / (kamaeF),
                            1.0f),
                        0.0f)
                );
                if (mod1 >= 0.95f)
                {
                    mod1 += sin((float)m_Obj->frameActive * 1.0f) * 0.0025f;
                }

                for (int i = 0; i < MODEL_NUM; i++)
                {
                    m_ModelTransPosStart[i] = {};
                    m_ModelTransRotStart[i] = {};
                    m_ModelTransScaStart[i].x = 1.0f;
                    m_ModelTransScaStart[i].y = 1.0f;
                    m_ModelTransScaStart[i].z = 1.0f;

                    m_ModelTransPos[i] = {};
                    m_ModelTransRot[i] = {};
                }

                //ぼでー
                {
                    for (int i = 0; i < MODEL_NUM; i++)
                    {
                        m_ModelTransRot[i].y = -45.0f * mod1;
                        m_ModelTransRot[i].z = 5.0f * mod1;
                    }
                    m_ModelTransRot[5].x = -8.0f * mod1;
                    m_ModelTransRot[5].y = 25.0f * mod1;
                    m_ModelTransRot[6] = m_ModelTransRot[5];
                }

                //腕
                {
                    {
                        m_ModelTransPos[3].x = -80.0f * mod1;
                        m_ModelTransPos[3].y = -160.0f * mod1;
                        m_ModelTransPos[3].z = 10.0f * mod1;

                        m_ModelTransRot[3].x = -540.0f * mod1;
                        m_ModelTransRot[3].y = -90.0f * mod1;
                        m_ModelTransRot[3].z = 0.0f * mod1;
                    }
                    {
                        m_ModelTransPos[4].x = -40.0f * mod1;
                        m_ModelTransPos[4].y = 40.0f * mod1;
                        m_ModelTransPos[4].z = 80.0f * mod1;

                        m_ModelTransRot[4].x = -100.0f * mod1;
                        m_ModelTransRot[4].y = 50.0f * mod1;
                        m_ModelTransRot[4].z = 0.0f * mod1;
                    }
                }
            }


            else if (
                m_Anim_State == AnimState::Start
                || m_Anim_State == AnimState::StartBefore
            )
            {
                float kamaeF = ATTACK_BEAM_BEFORE_FIXED_FRAME;
                if (ATTACK_BEAM_BEFORE_FIXED_FRAME * 2 > m_Anim_FrameMax)
                {
                    kamaeF = m_Anim_FrameMax * 0.3f;
                }

                float mod1 = std::max(std::min((float)(m_Anim_Frame) / (kamaeF), 1.0f), 0.0f);
                float mod2 = 1.0f - std::max(std::min((float)(m_Anim_Frame - (m_Anim_FrameMax - kamaeF)) / (kamaeF), 1.0f), 0.0f);

                float mod = EasingInOutBack(mod2);
                if (m_Anim_State == AnimState::StartBefore)
                {
                    mod = EasingInOutBack(mod1);
                    m_CurrentAxisPrivate = 
                        EasingInOutBack(
                            std::max(std::min((float)(m_Anim_Frame - (kamaeF)) / (m_Anim_FrameMax - kamaeF), 1.0f), 0.0f)
                        )
                            * (360.0f - 45.0f);
                }

                for (int i = 0; i < MODEL_NUM; i++)
                {
                    m_ModelTransPosStart[i] = {};
                    m_ModelTransRotStart[i] = {};
                    m_ModelTransScaStart[i].x = 1.0f;
                    m_ModelTransScaStart[i].y = 1.0f;
                    m_ModelTransScaStart[i].z = 1.0f;

                    m_ModelTransPos[i] = {};
                    m_ModelTransRot[i] = {};
                    m_ModelTransRot[i].x = 16.0f * mod;

                    m_ModelTransPos[i].y = sinf((float)m_Obj->frameActive / 15) * 4.0f * mod;
                }
                if (m_Anim_State == AnimState::Start)
                {
                    for (int i = 0; i < MODEL_NUM; i++)
                    {
                        m_ModelTransRot[i].z = -16.0f * mod;
                    }
                }


                //ぼでー
                {
                    float m = EasingInOutBounce(sinf((float)m_Obj->frameActive / 30)) * 12.0f * mod;
                    m_ModelTransRot[1].z = -m;
                    m_ModelTransRot[2].z = m;
                }

                //頭
                {
                    float pxm = -2.0f * mod;
                    float pym = 10.0f * mod;
                    float pzm = 50.0f * mod;
                    float rxm = 6.0f * mod;
                    float rym = -16.0f * mod;
                    for (int i = 0; i < 2; i++)
                    {
                        m_ModelTransPos[5 + i].x += pxm;
                        m_ModelTransPos[5 + i].y += pym;
                        m_ModelTransPos[5 + i].z += pzm;
                        m_ModelTransRot[5 + i].x += rxm;
                        m_ModelTransRot[5 + i].y += rym;
                    }
                }

                //腕
                {
                    m_ModelTransPos[3].x = 110.0f * mod;
                    m_ModelTransPos[3].y = -30.0f * mod;
                    m_ModelTransPos[3].z = -120.0f * mod;
                    m_ModelTransRot[3].x = -90.0f * mod;
                    m_ModelTransRot[3].y = -30.0f * mod;

                    m_ModelTransPos[4].x = 10.0f * mod;
                    m_ModelTransPos[4].y = 16.0f * mod;
                    m_ModelTransPos[4].z = 100.0f * mod;
                    m_ModelTransRot[4].x = -90.0f * mod;
                    m_ModelTransRot[4].y = 55.0f * mod;
                }
            }
            else if (
                m_Anim_State == AnimState::End
            )
            {
                float mod = std::max(std::min((float)(m_Anim_Frame) / (60), 1.0f), 0.0f);

                for (int i = 0; i < MODEL_NUM; i++)
                {
                    m_ModelTransPosStart[i] = {};
                    m_ModelTransRotStart[i] = {};
                    m_ModelTransScaStart[i].x = 1.0f;
                    m_ModelTransScaStart[i].y = 1.0f;
                    m_ModelTransScaStart[i].z = 1.0f;

                    m_ModelTransPos[i] = {};
                    m_ModelTransRot[i] = {};

                    //m_ModelTransRot[i].x = -8.0f + -14.0f * mod;
                    m_ModelTransRot[i].x = -8.0f + 90.0f * EasingOutBounce(mod);
                    m_ModelTransPos[i].y = 200.0f * EasingOutBounce(mod);
                    //m_ModelTransPos[i].y = 10.0f * mod;
                    //m_ModelTransRot[i].z = -3.0f * mod;
                }

                //頭
                {
                    for (int i = 0; i < 2; i++)
                    {
                        m_ModelTransPos[5 + i].y += -8.0f;
                        m_ModelTransRot[5 + i].x += 3.0f + 2.0f * mod;
                        m_ModelTransRot[5 + i].z += -2.0f + -3.0f * mod;
                    }
                }

                //腕
                {
                    m_ModelTransPos[3].x += -50.0f;
                    m_ModelTransPos[3].y += -190.0f;
                    m_ModelTransRot[3].x += -180.0f;
                    m_ModelTransRot[3].z += -40.0f +  0.0f * sinf((float)m_Obj->frameActive / 2);

                    m_ModelTransPos[4].x += 50.0f;
                    m_ModelTransPos[4].y += -190.0f;
                    m_ModelTransRot[4].x += -180.0f;
                    m_ModelTransRot[4].z += 40.0f + 0.0f * -sinf((float)m_Obj->frameActive / 2);


                    m_ModelTransPos[3].y += 70.0f * EasingInOutBack(mod);
                    m_ModelTransPos[4].y += 70.0f * EasingInOutBack(mod);
                }
            }
            else if (
                m_Anim_State == AnimState::Appeal_0
                || m_Anim_State == AnimState::Appeal_1
                || m_Anim_State == AnimState::Appeal_2
            )
            {
                float mod = std::max(std::min((float)(m_Anim_Frame) / (60), 1.0f), 0.0f);

                for (int i = 0; i < MODEL_NUM; i++)
                {
                    m_ModelTransPos[i] = {};
                    m_ModelTransRot[i] = {};

                    m_ModelTransPosStart[i] = {};
                    m_ModelTransRotStart[i] = {};
                    m_ModelTransScaStart[i].x = 1.0f;
                    m_ModelTransScaStart[i].y = 1.0f;
                    m_ModelTransScaStart[i].z = 1.0f;
                    //m_ModelTransRot[i].x = -8.0f + 90.0f * EasingOutBounce(mod);
                    m_ModelTransPos[i].y = -10.0f * m_Anim_Mod;

                    m_ModelTransRotStart[i].x = -36.0f;
                    if (m_Anim_State == AnimState::Appeal_2)
                    {
                        m_ModelTransRotStart[i].y = -45.0f;
                    }
                    else if (m_Anim_State == AnimState::Appeal_1)
                    {
                        m_ModelTransRotStart[i].y = 45.0f;
                    }
                    else
                    {
                        m_ModelTransRotStart[i].y = 0.0f;
                    }
                    m_ModelTransRotStart[i].z = 0.0f;
                }

                //腕
                {
                    float f = (float)m_Obj->frameActive * 0.8f;

                    m_ModelTransRot[3].x += -170.0f;
                    m_ModelTransRot[3].z += -40.0f + 32.0f * sinf(f);
                    m_ModelTransPos[3].x += -80.0f  + +(50.0f + 0.0f) * sinf(f);
                    m_ModelTransPos[3].y += -210.0f + +(10.0f + 0.0f) * sinf(f);

                    m_ModelTransRot[4].x += -170.0f;
                    m_ModelTransRot[4].z += 40.0f + 32.0f * -sinf(f);
                    m_ModelTransPos[4].x +=  +80.0f + -(50.0f + 0.0f) * sinf(f);
                    m_ModelTransPos[4].y += -210.0f + +(10.0f + 0.0f) * sinf(f);


                    //m_ModelTransPos[3].y += 70.0f * EasingInOutBack(mod);
                    //m_ModelTransPos[4].y += 70.0f * EasingInOutBack(mod);
                }
            }
        }
    }

    if (m_Anim_Frame >= 0)
    {
        m_Anim_Frame++;
        m_Anim_Mod = (float)m_Anim_Frame / m_Anim_FrameMax;
        if (m_Anim_Frame >= m_Anim_FrameMax)
        {
            if (!m_Anim_NoEndReset)
            {
                for (int i = 0; i < MODEL_NUM; i++)
                {
                    m_ModelTransPos[i] = {};
                    m_ModelTransRot[i] = {};
                    m_ModelTransSca[i].x = 1.0f;
                    m_ModelTransSca[i].y = 1.0f;
                    m_ModelTransSca[i].z = 1.0f;
                    m_ModelTransPosStart[i] = {};
                    m_ModelTransRotStart[i] = {};
                    m_ModelTransScaStart[i].x = 1.0f;
                    m_ModelTransScaStart[i].y = 1.0f;
                    m_ModelTransScaStart[i].z = 1.0f;
                }

                //m_ModelTransSca[7].x = 0.0f;
                //m_ModelTransSca[7].y = 0.0f;
                //m_ModelTransSca[7].z = 0.0f;

            }
            m_Anim_Mod = 0.0f;
            m_Anim_Frame = -1;
        }
    }
}

void GOE_Character_Enemy_Boss_Kontoriusu::set_hammer_visible(bool b)
{
    m_HammerVisibleFlag = b;

    float modtmp = (float)m_HammerVisibleFrame / m_HammerVisibleFrameMax;
    if (b)
    {
        m_HammerVisibleFrameMax = 30;
        m_HammerVisibleFrame = m_HammerVisibleFrameMax * modtmp;
    }
    else
    {
        m_HammerVisibleFrameMax = 60;
        m_HammerVisibleFrame = m_HammerVisibleFrameMax * modtmp;
    }
}


void GOE_Character_Enemy_Boss_Kontoriusu::drawBody(int i, int flashnum, Float3 shakePos,
    DrawType type,
    int childIndex
)
{
    if (i == 7 && m_HammerVisibleFrame <= 0)
    {
        return;
    }

    Float3 AnimPosStart = m_ModelTransPosStart[i];
    Float3 AnimRotStart = m_ModelTransRotStart[i];
    Float3 AnimScaStart = m_ModelTransScaStart[i];
    Float3 AnimPos = m_ModelTransPos[i];
    Float3 AnimRot = m_ModelTransRot[i];
    Float3 AnimSca = m_ModelTransSca[i];

    if (
        type != DrawType::WarpOut && !m_IsPosCfx
        && (
            m_Anim_State == AnimState::Appeal_0
            || m_Anim_State == AnimState::Appeal_1
            || m_Anim_State == AnimState::Appeal_2
        )
    )
    {
        AnimPosStart = {};
        AnimRotStart = {};
        AnimScaStart = {1.0f, 1.0f, 1.0f};
        AnimPos = {};
        AnimRot = {};
        AnimSca = { 1.0f, 1.0f, 1.0f };
    }


    DRAW_POLYGON_DAT drawDatTmp = {};
    drawDatTmp.pos = MkF3(
        0.0f + AnimPos.x * MODEL_SCA,
        0.0f + AnimPos.y * MODEL_SCA,
        0.0f + AnimPos.z * MODEL_SCA
    );
    drawDatTmp.size = MkF3(
        AnimSca.x * 100.0f * MODEL_SCA,
        AnimSca.y * -100.0f * MODEL_SCA,
        AnimSca.z * 100.0f * MODEL_SCA
    );

    drawDatTmp.rot = 0.0f;
    //drawDatTmp.rot3D = MkF3(0.0f, 0.0f + (float)GetMainGameFrame() * 2.2f, 0.0f);
    drawDatTmp.rot3D = MkF3(
        AnimRot.x + 0.0f,
        AnimRot.y + 0.0f,
        AnimRot.z + 0.0f
    );


    drawDatTmp.texUV = { 0.0f, 0.0f };
    drawDatTmp.texWH = { 1.0f, 1.0f };

    drawDatTmp.startPos3D = m_Obj->pos + AnimPosStart;
    //drawDatTmp.startPos3D.z += -800.0f;
    
    drawDatTmp.startPos3D.z += -300.0f;

    drawDatTmp.startRot3D = MkF3(
        90.0f,
        0.0f,
        0.0f + m_CurrentAxisPrivate
    ) + AnimRotStart;
    drawDatTmp.startScale = MkF2(1.0f, 1.0f);
    drawDatTmp.startScale3D = MkF3(1.0f, 1.0f, 1.0f);
    drawDatTmp.startScale3D.x *= AnimScaStart.x;
    drawDatTmp.startScale3D.y *= AnimScaStart.y;
    drawDatTmp.startScale3D.z *= AnimScaStart.z;

    drawDatTmp.modelNo = m_ModelIndex[i];

    drawDatTmp.shaderOpt = m_Obj->graphic.shaderOpt;

    drawDatTmp.startPos3D.x += shakePos.x;
    drawDatTmp.startPos3D.y += shakePos.y;
    drawDatTmp.startPos3D.z += shakePos.z;

    DRAW_POLYGON_IN_CAMERA_EX_OPTION opt = {};
    opt.reverse = false;
    opt.fixed = false;
    opt.enableShade = true;
    opt.shaderType = m_shaderType;

    //opt.priority = DRAW_POLYGON_PRIORITY_FORCE_ALWAYS;
    opt.priority = DRAW_POLYGON_PRIORITY_NORMAL;
    //opt.priority = DRAW_POLYGON_PRIORITY_FORCE_NORMAL;

    if (
        (type == DrawType::WarpOut && m_WarpPosCfxBefore)
        ||
        (type != DrawType::WarpOut && m_IsPosCfx)
    )
    {

        //drawDatTmp.startPos3D = {};
        //drawDatTmp.startPos3D.y += -100.0f;
        //drawDatTmp.startPos3D.x = camera.trans.pos.x;
        //drawDatTmp.startPos3D.y = camera.trans.pos.y;
        //drawDatTmp.startPos3D.z = camera.trans.pos.z / 10000.0f;

        drawDatTmp.startPos3D = m_PosCfx;
        if (type == DrawType::WarpOut)
        {
            drawDatTmp.startPos3D = m_WarpPosBefore;
        } 
        drawDatTmp.startPos3D =
            drawDatTmp.startPos3D
            + AnimPosStart;
        drawDatTmp.size = drawDatTmp.size * 0.1f * 0.8f;
        drawDatTmp.pos = drawDatTmp.pos * 0.1f * 0.8f;

        drawDatTmp.startRot3D = MkF3(
            90.0f,
            0.0f,
            0.0f
        ) + AnimRotStart;

        //opt.billboard = true;
        opt.cameraFix = true;
    }

    if (type == DrawType::Dead)
    {
        float mod = ((float)(m_Dead_Frame) / m_Dead_FrameMax);

        //mod = (EasingOutQuad(mod));
        mod = (EasingInQuad(1.0f - mod));

        drawDatTmp.shaderOpt.outline = DPD_SHADER_OPT_OUTLINE_NONE;

        drawDatTmp.loadTexType = LOADTEXTURETYPE_MAIN;

        opt.priority = DRAW_POLYGON_PRIORITY_ALWAYS;

        constexpr float coloutmod = 0.25f;
        float colmod = 0.45f + (0.55f + coloutmod) * mod;
        if (colmod > 1.0f)
        {
            float colmod2 = 1.0f - ((colmod - 1.0f) / coloutmod);
            float addcol = 100.0f * colmod2;


            if (i == 4)
            {
                drawDatTmp.color = MkF4(
                    2.0f + addcol,
                    2.0f + addcol,
                    2.0f + addcol,
                    0.5f);
                drawDatTmp.texNo = 0;
                drawDatTmp.shaderOpt.blendState = DPD_SHADER_OPT_BLEND_STATE_ADDCOLOR;
            }
            else
            {
                drawDatTmp.color = MkF4(
                    1.0f + addcol,
                    1.0f + addcol,
                    1.0f + addcol,
                    1.0f);
                drawDatTmp.texNo = m_Tex_Model.textureId;
            }
        }
        else
        {
            drawDatTmp.color = MkF4(
                colmod,
                colmod,
                colmod,
                2.0f);
            //drawDatTmp.color = MkF4(
            //    1.0f * mod,
            //    1.0f * mod,
            //    1.0f * mod,
            //    2.0f);
            drawDatTmp.texNo = m_Tex_Dis.textureId;
            drawDatTmp.shaderOpt.blendState = DPD_SHADER_OPT_BLEND_STATE_ADDCOLOR;
            drawDatTmp.texWH = {
                1.0f * 1.5f,
                1.0f * 1.5f
            };

            //opt.priority = DRAW_POLYGON_PRIORITY_ALWAYS;

            opt.shaderType = DPD_SHADER_TYPE_EFFECT_DIS;
        }
    }
    else if (type == DrawType::FastMoveAfterEffect)
    {
        int addnummax = CalculateDistance(
            m_FastMovePosBefore,
            m_FastMovePosAfter
        ) / (ONE_BLOCK * 8);

        Float3 addvec = NormalizeVector(m_FastMovePosAfter - m_FastMovePosBefore);

        int ind = childIndex - 1;
        float mod = ((float)(m_FastMoveFrame) / m_FastMoveFrameMax);

        float visibleSubMod = 0.3f / addnummax * ind;
        float visibleMod = 1.0f
            - std::max(mod - (0.5f + visibleSubMod), 0.0f)
            / (1.0f - 0.5 + visibleSubMod);

        drawDatTmp.startPos3D = m_FastMovePosBefore
            + addvec * ONE_BLOCK * 8.0f * ind;

        drawDatTmp.startPos3D.z += -300.0f;

        if (CalculateDistance({}, drawDatTmp.startPos3D) > ONE_BLOCK * 100.0f)
        {
            return;
        }

        //drawDatTmp.startRot3D = MkF3(
        //    90.0f,
        //    0.0f,
        //    0.0f + m_FastMoveAxisBefore 
        //    + (m_FastMoveAxisAfter - m_FastMoveAxisBefore) * modind
        //);

        drawDatTmp.shaderOpt.outline = DPD_SHADER_OPT_OUTLINE_NONE;

        drawDatTmp.loadTexType = LOADTEXTURETYPE_MAIN;


        drawDatTmp.color = MkF4(
            0.0f *  visibleMod,
            0.1f *  visibleMod,
            0.0f *  visibleMod,
            1.0f *  visibleMod
        );
        drawDatTmp.texNo = 0;
        drawDatTmp.shaderOpt.blendState = DPD_SHADER_OPT_BLEND_STATE_ADDCOLOR;

        opt.depthBuffIgnore = true;
        opt.priority = DRAW_POLYGON_PRIORITY_NML_d1;
        opt.shaderType = DPD_SHADER_TYPE_NONE;
    }
    else if (type == DrawType::WarpIn || type == DrawType::WarpOut)
    {
        float mod = ((float)(m_WarpFrame) / m_WarpFrameMax);

        if (type == DrawType::WarpOut)
        {
            //mod = EasingInBack(EasingOutSine(1.0f - mod));
            mod = (EasingInQuad(1.0f - mod));

            if (!m_WarpPosCfxBefore)
            {
                drawDatTmp.startPos3D = m_WarpPosBefore;
                drawDatTmp.startPos3D.z += -300.0f;

                drawDatTmp.startRot3D = MkF3(
                    90.0f,
                    0.0f,
                    0.0f + m_WarpAxisBefore
                );
            }
        }
        else
        {
            //mod = EasingInBack(EasingOutSine(mod));
            mod = (EasingOutQuad(mod));
        }

        {
            drawDatTmp.startScale3D.x = 0.2f + 0.8f * EasingInOutBack(mod);
            drawDatTmp.startScale3D.y = 2.5f - 1.5f * EasingInOutBack(mod);
            drawDatTmp.startScale3D.z = 0.2f + 0.8f * EasingInOutBack(mod);
        }

        drawDatTmp.shaderOpt.outline = DPD_SHADER_OPT_OUTLINE_NONE;

        drawDatTmp.loadTexType = LOADTEXTURETYPE_MAIN;

        constexpr float coloutmod = 0.1f;
        float colmod = 0.45f + (0.55f + coloutmod) * mod;
        if (colmod > 1.0f)
        {
            float colmod2 = 1.0f - ((colmod - 1.0f) / coloutmod);
            float addcol = 10.0f * colmod2;


            if (i == 7)
            {
                //drawDatTmp.color = MkF4(
                //    1.75f + addcol,
                //    1.10f + addcol,
                //    2.00f + addcol,
                //    1.0f);
                //drawDatTmp.texUV.x = (float)m_Obj->frameActive * 0.03f;
                //drawDatTmp.texUV.y = (float)m_Obj->frameActive * 0.1f;
                //drawDatTmp.shaderOpt.blendState = DPD_SHADER_OPT_BLEND_STATE_ADDCOLOR;

                //drawDatTmp.texNo = m_Tex_Model_Blade.textureId;
                drawDatTmp.color = MkF4(
                    2.0f + addcol,
                    2.0f + addcol,
                    2.0f + addcol,
                    0.5f);
                drawDatTmp.texNo = 0;
                drawDatTmp.shaderOpt.blendState = DPD_SHADER_OPT_BLEND_STATE_ADDCOLOR;
            }
            else if (i == 5)
            {
                drawDatTmp.color = MkF4(
                    1.0f + addcol,
                    1.0f + addcol,
                    1.0f + addcol,
                    1.0f);
                drawDatTmp.texNo = m_Tex_Model_Head[(int)m_HeadState].textureId;
            }
            else
            {
                drawDatTmp.color = MkF4(
                    1.0f + addcol,
                    1.0f + addcol,
                    1.0f + addcol,
                    1.0f);
                drawDatTmp.texNo = m_Tex_Model.textureId;
            }
        }
        else
        {
            drawDatTmp.color = MkF4(
                colmod,
                colmod,
                colmod,
                2.0f);
            //drawDatTmp.color = MkF4(
            //    1.0f * mod,
            //    1.0f * mod,
            //    1.0f * mod,
            //    2.0f);
            drawDatTmp.texNo = m_Tex_Warp.textureId;
            drawDatTmp.shaderOpt.blendState = DPD_SHADER_OPT_BLEND_STATE_ADDCOLOR;
            drawDatTmp.texWH = {
                1.0f * 2.0f,
                1.0f * 2.0f
            };

            //opt.priority = DRAW_POLYGON_PRIORITY_ALWAYS;

            opt.shaderType = DPD_SHADER_TYPE_EFFECT_WARP;
        }
    }
    else if (type == DrawType::CutInShadow)
    {
        drawDatTmp.loadTexType = LOADTEXTURETYPE_GRAY;

        drawDatTmp.pos.x += 800.0f;
        drawDatTmp.pos.y += 800.0f;
        drawDatTmp.pos.z += 2000.0f;
        drawDatTmp.size.x *= 3.6f;
        drawDatTmp.size.y *= 3.6f;
        drawDatTmp.size.z *= 3.6f;
        //drawDatTmp.startPos3D.y = -1000.0f;
        //drawDatTmp.startPos3D.z = -200.0f;

        drawDatTmp.color = MkF4(0.75f, 0.0f, 1.0f, 2.0f);

        drawDatTmp.texNo = 0;

        drawDatTmp.shaderOpt.outline = DPD_SHADER_OPT_OUTLINE_NONE;
        drawDatTmp.shaderOpt.bloom = DPD_SHADER_OPT_BLOOM_NONE;

        opt.depthBuffIgnore = true;
        opt.priority = DRAW_POLYGON_PRIORITY_NML_ALWAYS;

        //opt.shaderType = DPD_SHADER_TYPE_CUTIN;
        opt.shaderType = DPD_SHADER_TYPE_NONE;
    }
    else if (type == DrawType::Shadow)
    { //影
        drawDatTmp.loadTexType = LOADTEXTURETYPE_GRAY;

        drawDatTmp.startPos3D.z = -15.0f;
        drawDatTmp.startRot3D.x = 90.0f;
        drawDatTmp.startScale3D.y *= 0.001f;

        drawDatTmp.color = MkF4(-1.0f, -1.0f, -1.0f, 1.0f);

        drawDatTmp.texNo = 0;

        drawDatTmp.shaderOpt.outline = DPD_SHADER_OPT_OUTLINE_NONE;
        drawDatTmp.shaderOpt.bloom = DPD_SHADER_OPT_BLOOM_NONE;
    }
    else
    { //デフォ
        drawDatTmp.loadTexType = LOADTEXTURETYPE_MAIN;

        if (i == 7)
        {
            float hammerVisibleMod = (float)m_HammerVisibleFrame / m_HammerVisibleFrameMax;
            if (hammerVisibleMod <= 0)
            {
                return;
            }

            drawDatTmp.shaderOpt.outline = DPD_SHADER_OPT_OUTLINE_NONE;

            constexpr float coloutmod = 0.1f;
            //float colmod = 0.45f + (0.55f + coloutmod) * bladeVisibleMod;
            float colmod = 0.1f + (0.9f + coloutmod) * hammerVisibleMod;
            if (colmod > 1.0f)
            {
                float colmod2 = 1.0f - ((colmod - 1.0f) / coloutmod);
                float addcol = 10.0f * colmod2;


                drawDatTmp.texNo = 0;
                drawDatTmp.shaderOpt.blendState = DPD_SHADER_OPT_BLEND_STATE_DEFAULT;
                drawDatTmp.color = MkF4(
                    0.2f,
                    0.0f,
                    0.1f,
                    0.7f);
                opt.priority = (DRAW_POLYGON_PRIORITY)(opt.priority - 1);
                opt.depthBuffIgnore = true;
                opt.shaderType = DPD_SHADER_TYPE_EFFECT;

                DrawPolygonInCameraEx(drawDatTmp, opt);


                drawDatTmp.texUV.x = (float)m_Obj->frameActive * 0.012f;
                drawDatTmp.texUV.y = (float)m_Obj->frameActive * 0.02f;
                drawDatTmp.texWH.x = 0.7f;
                drawDatTmp.texWH.y = 0.7f;
                drawDatTmp.texNo = m_Tex_Model_Blade.textureId;
                drawDatTmp.shaderOpt.blendState = DPD_SHADER_OPT_BLEND_STATE_ADDCOLOR;

                drawDatTmp.color = MkF4(
                    1.00f + addcol,
                    0.30f + addcol,
                    1.30f + addcol,
                    1.0f);
                opt.priority = (DRAW_POLYGON_PRIORITY)(opt.priority + 1);
                opt.depthBuffIgnore = false;
                opt.shaderType = DPD_SHADER_TYPE_EFFECT;
            }
            else
            {
                drawDatTmp.color = MkF4(
                    colmod,
                    colmod,
                    colmod,
                    2.0f);
                drawDatTmp.texNo = m_Tex_Warp.textureId;
                drawDatTmp.shaderOpt.blendState = DPD_SHADER_OPT_BLEND_STATE_ADDCOLOR;
                drawDatTmp.texWH = {
                    1.0f * 1.2f,
                    1.0f * 1.2f
                };

                opt.shaderType = DPD_SHADER_TYPE_EFFECT_WARP;
            }

            //drawDatTmp.shaderOpt.outline = DPD_SHADER_OPT_OUTLINE_NONE;

            //drawDatTmp.texNo = 0;
            //drawDatTmp.shaderOpt.blendState = DPD_SHADER_OPT_BLEND_STATE_DEFAULT;
            //drawDatTmp.color = MkF4(
            //    0.2f,
            //    0.0f,
            //    0.1f,
            //    1.0f);
            //opt.priority = (DRAW_POLYGON_PRIORITY)(opt.priority - 1);
            //opt.depthBuffIgnore = true;

            //DrawPolygonInCameraEx(drawDatTmp, opt);


            //drawDatTmp.texUV.x = (float)m_Obj->frameActive * 0.012f;
            //drawDatTmp.texUV.y = (float)m_Obj->frameActive * 0.02f;
            //drawDatTmp.texWH.x = 0.7f;
            //drawDatTmp.texWH.y = 0.7f;
            //drawDatTmp.texNo = m_Tex_Model_Blade.textureId;
            //drawDatTmp.shaderOpt.blendState = DPD_SHADER_OPT_BLEND_STATE_ADDCOLOR;
            //drawDatTmp.color = MkF4(
            //    1.00f,
            //    0.30f,
            //    1.30f,
            //    1.0f);
            //opt.priority = (DRAW_POLYGON_PRIORITY)(opt.priority + 1);
            //opt.depthBuffIgnore = false;
        }
        else if(i == 5)
        {
            drawDatTmp.color = MkF4(1.0f, 1.0f, 1.0f, 1.0f);
            drawDatTmp.texNo = m_Tex_Model_Head[(int)m_HeadState].textureId;
            opt.shaderType = DPD_SHADER_TYPE_NONE;
        }
        else
        {
            switch (flashnum)
            {
            case 1:
                drawDatTmp.loadTexType = LOADTEXTURETYPE_GRAY;
                drawDatTmp.color = MkF4(1.6f, 1.6f, 1.6f, 2.0f);
                break;
            case 2:
                drawDatTmp.loadTexType = LOADTEXTURETYPE_GRAY;
                drawDatTmp.color = MkF4(0.5f, 0.5f, 0.5f, 2.0f);
                break;
            default:
                drawDatTmp.color = MkF4(1.0f, 1.0f, 1.0f, 1.0f);
                break;
            }
            drawDatTmp.texNo = m_Tex_Model.textureId;
        }
        if (m_StartMotionShaderFlag)
        {
            opt.priority = DRAW_POLYGON_PRIORITY_ALWAYS;

            opt.shaderType = DPD_SHADER_TYPE_CUTIN;
        }
    }


    DrawPolygonInCameraEx(drawDatTmp, opt);
}

void GOE_Character_Enemy_Boss_Kontoriusu::shootBeam(
    Float3 pos, Float3 vec, float scale,
    int lifetime,
    int type
)
{
    //SEを再生
    SoundEffectPlayRequest(
        GetSoundData("SOUND/SE/enemy/attack/Laser"),
        1.8f, 0);
    //SEを再生
    SoundEffectPlayRequest(
        GetSoundData("SOUND/SE/enemy/attack/Small_Explosion"),
        2.5f, 0);

    CameraShakeRequestAll(CAMERA_SHAKE_MIDIUM_FRAME, CAMERA_SHAKE_MIDIUM_POWER);


    Float3 nvec = NormalizeVector(vec);

    GOE_Character_EnemyProjectile::Type typetmp =
        GOE_Character_EnemyProjectile::Type::Beam;
    switch (type)
    {
    case 1:
        typetmp = GOE_Character_EnemyProjectile::Type::EnergyBallBitKakusan;
        break;
    case 2:
        typetmp = GOE_Character_EnemyProjectile::Type::EnergyBallBitLaser;
        break;
    default:
        break;
    }

    GOE_Character_EnemyProjectile* goepro =
        GOE_Character_EnemyProjectile::CreateProjectile(
            typetmp,
            MkF3(
                pos.x + m_Obj->pos.x + nvec.x * ONE_BLOCK * 3.0f,
                pos.y + m_Obj->pos.y + nvec.y * ONE_BLOCK * 3.0f,
                pos.z + m_Obj->pos.z + nvec.z * ONE_BLOCK * 3.0f - 180.0f
            ),
            vec
        );
    if (goepro != nullptr)
    {
        GAME_OBJECT* beamObj = goepro->GetGameObj();
        if (beamObj != nullptr)
        {
            beamObj->scale.x = scale;
            beamObj->scale.y = scale;
            beamObj->scale.z = scale;

            if (typetmp == GOE_Character_EnemyProjectile::Type::EnergyBallBitLaser)
            {
                goepro->SetNumFlag(m_FireworksLaserDelay);
            }
            else
            {
                Character::MOTION_COL_DAT motionColDetails =
                    Character::GetMotionColDataFindByName(
                        Character::MOTION_COL_TYPE_ATK,
                        BEAM_ATK_COL_NAME
                    );

                GAME_OBJECT_COLLISION_ATTACK_CHILD attackColTmp = motionColDetails.atkCol;
                attackColTmp.subjectFamily[GAME_OBJECT_FAMILY_PLAYER] = true;
                attackColTmp.subjectFamily[GAME_OBJECT_FAMILY_GIMIK] = true;

                attackColTmp.size = MkF3(
                    ONE_BLOCK * 3,
                    ONE_BLOCK * 3,
                    ONE_BLOCK * 3
                );
                //attackColTmp.pos.z = -attackColTmp.size.z / 2;
                attackColTmp.pos.y += attackColTmp.size.y / 2 * 1.0f;

                beamObj->collision.atk.enable = true;
                beamObj->collision.atk.collision[0] = attackColTmp;
                beamObj->collision.atk.collision[0].enable = true;
            }
            beamObj->blockCollisionIgnore = false;

            GameObjectSetLifeTime(beamObj, lifetime);
            GameObjectMoveRequest(beamObj, lifetime,
                MkF3(
                    beamObj->pos.x + vec.x * ONE_BLOCK * 90.0f,
                    beamObj->pos.y + vec.y * ONE_BLOCK * 90.0f,
                    beamObj->pos.z + vec.z * ONE_BLOCK * 90.0f
                ),
                nullptr
            );
        }
    }
}

// ===================================================
// Draw
// オブジェクトの描画イベント
// ===================================================
void GOE_Character_Enemy_Boss_Kontoriusu::Draw()
{
	SuperGOE::Draw();

    if (m_StartMotionShaderFlag)
    {
        {
            DRAW_POLYGON_DAT dsdat = {};
            dsdat.pos.x = 0.0f;
            dsdat.pos.y = 0.0f;
            dsdat.pos.z = 0.0f;

            dsdat.size.x = SCREEN_WIDTH;
            dsdat.size.y = SCREEN_HEIGHT;
            dsdat.size.z = 1.0f;

            dsdat.rot = 0.0f;

            //dsdat.color = { 0.0f, 0.0f, 1.0f, 1.0 };
            //dsdat.texNo = 0;
            dsdat.color = { 1.0f, 1.0f, 1.0f, 1.0 };
            dsdat.texNo = LoadTexData("TEX/effect/tex_anim_02").textureId;

            dsdat.loadTexType = LOADTEXTURETYPE_MAIN;

            dsdat.texUV.x += 0.02 * (float)m_Obj->frame;
            //dsdat.texWH = { 1.0f, 1.0f };
            dsdat.texWH = { 1.0f * ((float)SCREEN_HEIGHT / SCREEN_WIDTH), 1.0f };

            dsdat.startRot = 0.0f;
            dsdat.startScale = MkF2(1.0f, 1.0f);
            dsdat.startScale3D = MkF3(1.0f, 1.0f, 1.0f);

            dsdat.modelNo = 0;

            //dsdat.shaderOpt.bloom = true;
            //dsdat.shaderOpt.blendState = DPD_SHADER_OPT_BLEND_STATE_ADDCOLOR;

            DRAW_POLYGON_IN_CAMERA_EX_OPTION opt = {};
            opt.reverse = false;
            opt.fixed = true;
            opt.enableShade = false;
            opt.billboard = false;
            opt.priority = (DRAW_POLYGON_PRIORITY)(DRAW_POLYGON_PRIORITY_NML_ALWAYS - 1);

            DrawPolygonInCameraEx(dsdat, opt);
        }

        for (int i = 0; i < MODEL_NUM; i++)
        {
            drawBody(i, 0, {}, DrawType::CutInShadow);
        }
    }

    if (m_Dead_Frame > 0)
    {
        if(m_Dead_Frame < 8)
        {
            DRAW_POLYGON_DAT dsdat = {};
            dsdat.pos.x = 0.0f;
            dsdat.pos.y = 0.0f;
            dsdat.pos.z = 0.0f;

            dsdat.size.x = SCREEN_WIDTH;
            dsdat.size.y = SCREEN_HEIGHT;
            dsdat.size.z = 1.0f;

            dsdat.rot = 0.0f;

            dsdat.color = {
                0.15f,
                0.15f,
                0.15f,
                1.0 };
            dsdat.texNo = 0;
            //dsdat.color = { 1.0f, 1.0f, 1.0f, 1.0 };
            //dsdat.texNo = m_Tex_PowerBg;

            dsdat.loadTexType = LOADTEXTURETYPE_MAIN;

            dsdat.texUV.x += 0.02 * (float)m_Obj->frame;
            //dsdat.texWH = { 1.0f, 1.0f };
            dsdat.texWH = { 1.0f * ((float)SCREEN_HEIGHT / SCREEN_WIDTH), 1.0f };

            dsdat.startRot = 0.0f;
            dsdat.startScale = MkF2(1.0f, 1.0f);
            dsdat.startScale3D = MkF3(1.0f, 1.0f, 1.0f);

            dsdat.modelNo = 0;

            //dsdat.shaderOpt.bloom = true;
            //dsdat.shaderOpt.blendState = DPD_SHADER_OPT_BLEND_STATE_ADDCOLOR;

            DRAW_POLYGON_IN_CAMERA_EX_OPTION opt = {};
            opt.reverse = false;
            opt.fixed = true;
            opt.enableShade = false;
            opt.billboard = false;
            opt.priority = (DRAW_POLYGON_PRIORITY)(DRAW_POLYGON_PRIORITY_NML_ALWAYS - 1);

            DrawPolygonInCameraEx(dsdat, opt);
        }
        for (int i = 0; i < MODEL_NUM; i++)
        {
            drawBody(i, 0, {}, DrawType::Dead);
        }
    }
    else if (m_FastMoveFrame >= 0)
    {
        int addnum = CalculateDistance(
            m_FastMovePosBefore,
            m_Obj->pos
        ) / (ONE_BLOCK * 8);
        for (int j = 0; j < 1 + std::min(addnum, 1024); j++)
        {
            for (int i = 0; i < MODEL_NUM; i++)
            {
                drawBody(i, 0, {}, 
                    j == 0 ? DrawType::Default
                    : DrawType::FastMoveAfterEffect,
                    j
                );
            }
        }
    }
    else if (m_WarpFrame >= 0)
    {
        for (int j = 0; j < 2; j++)
        {
            for (int i = 0; i < MODEL_NUM; i++)
            {
                drawBody(i, 0, {}, j == 0 ? DrawType::WarpOut : DrawType::WarpIn);
            }
        }
    }
    else
    {
        Float3 shakePos = {};
        int flashnum = 0;
        if (m_Obj->hitReaction.frame > 0 && m_Obj->hitReaction.frame < m_Obj->hitReaction.frameMax - 1)
        {
            float hrModTmp = (float)m_Obj->hitReaction.frame / m_Obj->hitReaction.frameMax;

            {
                int ftmp = m_Obj->hitReaction.frame % 6;
                if (ftmp < 2)
                {
                    flashnum = 1;
                }
                if (ftmp >= 4)
                {
                    flashnum = 2;
                }
            }
            float shakemod = 0.2f + 0.8f * std::min(std::max((float)m_Obj->status.beforeDamage / 300.0f, 0.0f), 1.0f);
            shakePos.x += sin(1.2f * m_Obj->hitReaction.frame) * hrModTmp * 200.0f * shakemod;
        }

        //for (int j = 0; j < 2; j++)
        //{
        //    for (int i = 0; i < MODEL_NUM; i++)
        //    {
        //        drawBody(i, flashnum, shakePos, j == 0 ? 1: 0);
        //    }
        //}
        if(IsInStage())
        {
            DRAW_SPRITE_DAT dsdat = {};
            dsdat.x = m_Obj->pos.x;
            dsdat.y = m_Obj->pos.y;
            dsdat.z = -8.0f;
            //dsdat.z = -0.005f;
            dsdat.w = m_Obj->size.x * m_Obj->scale.x * 1.85f;
            dsdat.h = m_Obj->size.y * m_Obj->scale.y * 1.85f;
            {
                float mo = 1.0f - std::max(std::min(abs(m_Obj->pos.z) / (ONE_BLOCK * 80.0f), 1.0f), 0.0f);
                dsdat.w *= mo;
                dsdat.h *= mo;
            }

            dsdat.rot = 0.0f;
            dsdat.color = { 1.0f, 1.0f, 1.0f, 0.4f };

            dsdat.texNo = m_Tex_Shadow.textureId;
            dsdat.loadTexType = LOADTEXTURETYPE_MAIN;

            dsdat.texUV = { 0.0f, 0.0f };
            dsdat.texWH = { 1.0f, 1.0f };
            dsdat.startPos = { 0.0f, 0.0f };
            dsdat.startRot = 0.0f;
            dsdat.startScale = { 1.0f, 1.0f };

            DrawSpriteInCamera(dsdat, false, false,
                true
            );
        }
        //for (int j = 0; j < 2; j++)
        //{
        //    for (int i = 0; i < MODEL_NUM; i++)
        //    {
        //        drawBody(i, flashnum, shakePos, j == 0 ? DrawType::Shadow: DrawType::Default);
        //    }
        //}
        {
            for (int i = 0; i < MODEL_NUM; i++)
            {
                drawBody(i, flashnum, shakePos, DrawType::Default);
            }
        }
    }

    if (m_StartMotionShaderFlag)
    {

        //{
        //    DRAW_POLYGON_DAT dsdat = {};
        //    dsdat.pos.x = 0.0f;
        //    dsdat.pos.y = 0.0f;
        //    dsdat.pos.z = 0.0f;

        //    dsdat.size.x = SCREEN_WIDTH * 2.0f;
        //    dsdat.size.y = SCREEN_HEIGHT * 1.5f;
        //    dsdat.size.z = 1.0f;

        //    dsdat.rot = -10.0f;

        //    //dsdat.color = { 1.0f, 1.0f, 1.0f, 1.0 };
        //    dsdat.color = { 1.0f, 1.0f, 1.0f, 0.5f };
        //    //dsdat.texNo = m_Tex_StartingEff_Bk2.textureId;
        //    dsdat.texNo = m_Tex_StartingEff_Bk1.textureId;

        //    dsdat.loadTexType = LOADTEXTURETYPE_MAIN;

        //    dsdat.texUV.x = -0.08f * m_Obj->frame;

        //    dsdat.texWH = { 1.0f, 1.0f };

        //    dsdat.startRot = 0.0f;
        //    dsdat.startScale = MkF2(1.0f, 1.0f);
        //    dsdat.startScale3D = MkF3(1.0f, 1.0f, 1.0f);

        //    dsdat.modelNo = 0;

        //    dsdat.shaderOpt.blendState = DPD_SHADER_OPT_BLEND_STATE_ADDCOLOR;

        //    DRAW_POLYGON_IN_CAMERA_EX_OPTION opt = {};
        //    opt.reverse = false;
        //    opt.fixed = true;
        //    opt.billboard = false;
        //    opt.priority = DRAW_POLYGON_PRIORITY_NML_ALWAYS;
        //    opt.depthBuffIgnore = true;
        //    //opt.shaderType = DPD_SHADER_TYPE_EFFECT;

        //    DrawPolygonInCameraEx(dsdat, opt);
        //}
        //{
        //    DRAW_POLYGON_DAT dsdat = {};
        //    dsdat.pos.x = 0.0f;
        //    //dsdat.pos.y = 0.0f;
        //    dsdat.pos.y = 160.0f;
        //    dsdat.pos.z = 0.0f;

        //    dsdat.size.x = SCREEN_WIDTH * 2.0f;
        //    //dsdat.size.y = SCREEN_HEIGHT * 1.2f;
        //    dsdat.size.y = SCREEN_HEIGHT * 0.65f;
        //    dsdat.size.z = 1.0f;

        //    dsdat.rot = -10.0f;

        //    //dsdat.color = { 1.0f, 1.0f, 1.0f, 0.8 };
        //    //dsdat.color = { 1.0f, 1.0f, 1.0f, 1.0f };
        //    dsdat.color = { 0.7f, 0.0f, 1.0f, 1.0f };
        //    //dsdat.texNo = m_Tex_StartingEff_Bk1.textureId;
        //    dsdat.texNo = m_Tex_StartingEff_Bk2.textureId;

        //    dsdat.loadTexType = LOADTEXTURETYPE_MAIN;

        //    dsdat.texUV.x = -0.05f * m_Obj->frame;

        //    dsdat.texWH = { 1.0f, 1.0f };
        //    //dsdat.texWH = { 1.0f, 0.3f };

        //    dsdat.startRot = 0.0f;
        //    dsdat.startScale = MkF2(1.0f, 1.0f);
        //    dsdat.startScale3D = MkF3(1.0f, 1.0f, 1.0f);

        //    dsdat.modelNo = 0;

        //    dsdat.shaderOpt.blendState = DPD_SHADER_OPT_BLEND_STATE_ADDCOLOR;

        //    DRAW_POLYGON_IN_CAMERA_EX_OPTION opt = {};
        //    opt.reverse = false;
        //    opt.fixed = true;
        //    opt.billboard = false;
        //    opt.priority = DRAW_POLYGON_PRIORITY_FORCE_DEEPEST;
        //    opt.depthBuffIgnore = true;
        //    opt.shaderType = DPD_SHADER_TYPE_EFFECT;

        //    DrawPolygonInCameraEx(dsdat, opt);
        //}
        {
            DRAW_POLYGON_DAT dsdat = {};
            //dsdat.pos.x = 280.0f;
            //dsdat.pos.y = 60.0f;
            dsdat.pos.x = 260.0f;
            dsdat.pos.y = 180.0f;
            dsdat.pos.z = 0.0f;
            dsdat.pos.x += cos(-1.2f * m_Obj->frame) * 5.0f;
            dsdat.pos.y += sin(-0.7f * m_Obj->frame) * 5.0f;

            dsdat.size.x = 1300.0f;
            dsdat.size.y = 1300.0f;
            dsdat.size.z = 1.0f;

            dsdat.rot = -12.0f;

            dsdat.color = { 1.0f, 1.0f, 1.0f, 1.0 };
            dsdat.texNo = 

                m_Tex_StartingEff_Name.textureId
                ;

            dsdat.loadTexType = LOADTEXTURETYPE_MAIN;

            dsdat.texWH = { 1.0f, 1.0f };

            dsdat.startRot = 0.0f;
            dsdat.startScale = MkF2(1.0f, 1.0f);
            dsdat.startScale3D = MkF3(1.0f, 1.0f, 1.0f);

            dsdat.modelNo = 0;

            DRAW_POLYGON_IN_CAMERA_EX_OPTION opt = {};
            opt.reverse = false;
            opt.fixed = true;
            opt.billboard = false;
            //opt.priority = DRAW_POLYGON_PRIORITY_NML_ALWAYS;
            opt.priority = DRAW_POLYGON_PRIORITY_FORCE_DEEPEST;
            opt.depthBuffIgnore = true;
            //opt.shaderType = DPD_SHADER_TYPE_EFFECT;

            {
                DRAW_POLYGON_DAT dsdat2 = dsdat;
                dsdat2.pos.x += sin(1.2f * m_Obj->frame) * 80.0f;
                dsdat2.pos.y += cos(0.7f * m_Obj->frame) * 80.0f;
                //dsdat2.color = {
                //    1.0f,
                //    0.0f,
                //    0.6f,
                //    0.4f + tan(0.4f * m_Obj->frame) * 0.3f
                //};
                dsdat2.color = {
                    0.7f,
                    0.5f,
                    1.0f,
                    0.4f + tan(0.4f * m_Obj->frame) * 0.3f
                };
                dsdat2.shaderOpt.blendState = DPD_SHADER_OPT_BLEND_STATE_ADDCOLOR;
                DrawPolygonInCameraEx(dsdat2, opt);
            }

            opt.shaderType = DPD_SHADER_TYPE_EX;
            opt.shaderTypeEx.vs = m_ShaderIndex_CutInName_Vertex;
            opt.shaderTypeEx.ps = m_ShaderIndex_CutInName_Pixel;

            DrawPolygonInCameraEx(dsdat, opt);
        }

    }


    if (!m_StartedFlag)
    {
        draw_start();
        return;
    }
    draw_test();
    draw_attack();
    draw_hammer();
    draw_fireworks();
    draw_granade();
    draw_laser();
    draw_superlaser();

    //行動
    //switch (m_CurrentState)
    //{
    //case ActionState::Blade:
    //    draw_blade();
    //    break;
    //case ActionState::Beam:
    //    draw_beam();
    //    break;
    //case ActionState::Misile:
    //    draw_misile();
    //    break;
    //default:break;
    //}
    //draw_blade();
    //draw_beam();
    //draw_misile();
}

// ===================================================
// Dead
// オブジェクトのHPが0になったとき
// ===================================================
void GOE_Character_Enemy_Boss_Kontoriusu::Dead()
{
	SuperGOE::Dead();

    set_hammer_visible(false);

    //ムービーフラグ
    GameManagerSetMovieFlag(true);
    GameManagerSetMoviePlayerHiddenFlag(true);

    CameraFlashRequestAll(
        10,
        EasingInOutSine,
        MkF4(1.0f, 10.0f, 10.0f, 1.0f),
        DPD_SHADER_OPT_BLEND_STATE_ADDCOLOR
    );

    GameStopRequest(20);

    int waitf = 25;
    GameObjectHitStop(m_Obj, waitf);

    {
        SetCameraModeAll(CAMERA_MODE_NORMAL);

        Float3 zoomPos = m_Obj->pos;
        zoomPos.z += 42000.0f;

        Float3 zoomPosTa = m_Obj->pos;
        zoomPosTa.z += -16000.0f;

        CameraTargetMoveRequestAll(20,
            zoomPosTa,
            EasingInOutSine,
            CAMERA_MOVE_TYPE_SMOOTH
        );
        CameraMoveRequestAll(20,
            zoomPos,
            0.0f,
            MkF2(1.0f, 1.0f),
            0.32f,
            EasingInOutSine,
            CAMERA_MOVE_TYPE_SMOOTH
        );
    }

    m_DeadFlag = true;
}

void GOE_Character_Enemy_Boss_Kontoriusu::CalcDamage(
    GAME_OBJECT_DAMAGE_RESULT* output,

    const GAME_OBJECT* obj, const GAME_OBJECT* objAttacker,
    int damage,
    GAME_OBJECT_COLLISION_ATTACK_CHILD colAtk,
    GAME_OBJECT_COLLISION_RECIEVE_CHILD colRec
)
{
    output->damage = CalcRecvDamageMultiPlay(output->damage);

    if (m_CurrentState == ActionState::Hammer)
    {
        if (m_HammerSuperFlag)
        {
            CalcRecvDamageGuardMod(0.3f, colAtk, colRec, output);
        }
        if (m_HammerQuakeFlag)
        {
            CalcRecvDamageGuardMod(0.5f, colAtk, colRec, output);
        }
        if (m_HammerRumblingFlag)
        {
            CalcRecvDamageGuardMod(0.7f, colAtk, colRec, output);
        }
    }

    if (m_CurrentState == ActionState::Granade)
    {
        CalcRecvDamageGuardMod(0.5f, colAtk, colRec, output);
    }
    if (m_CurrentState == ActionState::Fireworks)
    {
        CalcRecvDamageGuardMod(0.7f, colAtk, colRec, output);
    }
    if (m_CurrentState == ActionState::Laser)
    {
        CalcRecvDamageGuardMod(0.7f, colAtk, colRec, output);
    }

    if (m_Dead_Frame > 0)
    {
        output->damage = 0;
        output->mod = 0.0f;
    }
}


// ===================================================
// DamageAttacker
// オブジェクトの攻撃判定がヒットしたとき
// ===================================================
void GOE_Character_Enemy_Boss_Kontoriusu::DamageAttacker(GAME_OBJECT_DAMAGE_RESULT dr)
{
	SuperGOE::DamageAttacker(dr);

    if (std::string("BLADE") == dr.colAtk.tag)
    {
        //SEを再生
        SoundEffectPlayRequest(GetSoundData("SOUND/SE/inperiusu/b_attackHit"), 1.5f, 0);

        GAME_OBJECT* recvObj = GetGameObject(dr.recObjIndex);

        if (recvObj != nullptr)
        {
            Float2 vec = MkF2(
                recvObj->pos.x - m_Obj->pos.x, 
                recvObj->pos.y - m_Obj->pos.y
            );
            vec = NormalizeVector(vec);

            GameObjectMoveRequest(recvObj,
                36,
                MkF3(
                    recvObj->pos.x + vec.x * ONE_BLOCK * 18.0f,
                    recvObj->pos.y + vec.y * ONE_BLOCK * 18.0f,
                    recvObj->pos.z
                ),
                EasingOutQuad
            );
        }
    }
}

// ===================================================
// DamageReciever
// オブジェクトが
// 別のオブジェクトの攻撃判定にヒットしたとき
// ===================================================
void GOE_Character_Enemy_Boss_Kontoriusu::DamageReciever(GAME_OBJECT_DAMAGE_RESULT dr)
{
	SuperGOE::DamageReciever(dr);


}

// ===================================================
// BlockHit
// オブジェクトがブロックに触れたとき
// ===================================================
void GOE_Character_Enemy_Boss_Kontoriusu::BlockHit(GAME_OBJECT_POSMOD_RESULT pmr)
{
	SuperGOE::BlockHit(pmr);
}



void GOE_Character_Enemy_Boss_Kontoriusu::StartBattle()
{
    m_StartFlag = true;
}

void GOE_Character_Enemy_Boss_Kontoriusu::update_action_pattern()
{
    if (m_IsHard)
    {
        update_action_pattern_hard();
    }
    else
    {
        update_action_pattern_normal();
    }
}

void GOE_Character_Enemy_Boss_Kontoriusu::update_action_pattern_normal_base1()
{
    auto hammer_general = [this](
        int warpnum = 0,
        float chargemod = 1.0f,
        int wavenum = 3,
        float waveangle = 40.0f
        )
        { //通常
            m_CurrentState = ActionState::Hammer;
            m_HammerFrame = 0;

            m_HammerWarpNum = warpnum;
            m_HammerWarpHastenFrame = 0;
            m_HammerBeforeSpeedMod = chargemod * 0.6f;
            m_HammerWaveNum = wavenum;
            m_HammerWaveAngle = waveangle;
            m_HammerWaveLifetime = 80;
            m_HammerWaveScale = 1.0f;
            m_HammerWaveSpeed = 70.0f;
            m_HammerWaveGapAngleStart = 0.0f;
            m_HammerWaveGapAngleEnd = 0.0f;
            m_HammerNoTurnFlag = false;
            m_HammerAfterWarpFlag = false;
            m_HammerSuperFlag = false;
            m_HammerQuakeFlag = false;
            m_HammerRumblingFlag = false;
        };

    auto hammer_storm = [this](int posindex)
        { //アースストーム
            m_CurrentState = ActionState::Hammer;
            m_HammerFrame = 0;

            Float3 actionStdPos[3] = {
                { ONE_BLOCK * 50.0f, ONE_BLOCK * -28.0f, 0.0f },
                { ONE_BLOCK * -50.0f, ONE_BLOCK * -28.0f, 0.0f },
                { 0.0f, ONE_BLOCK * -30.0f, 0.0f },
            };
            Float3 afpos = actionStdPos[posindex % 3];
            float rot = CalculateRadToDeg(atan2(-afpos.y, -afpos.x)) - 90.0f;
            StartWarp(afpos, rot, 25, true);

            m_HammerWarpNum = 0;
            m_HammerWarpHastenFrame = 0;
            //m_HammerBeforeSpeedMod = 2.4f;
            m_HammerBeforeSpeedMod = 1.3f;
            m_HammerWaveNum = 24;
            m_HammerWaveAngle = 360.0f;
            m_HammerWaveLifetime = 400;
            m_HammerWaveScale = 1.0f;
            m_HammerWaveSpeed = 350.0f;

            m_HammerNoTurnFlag = true;
            m_HammerAfterWarpFlag = true;
            m_HammerSuperFlag = true;
            m_HammerQuakeFlag = false;
            m_HammerRumblingFlag = false;

        };

    auto hammer_quake = [this](
        float chargemod = 1.0f
        )
        { //クエイク
            m_CurrentState = ActionState::Hammer;
            m_HammerFrame = 0;

            m_HammerWarpNum = 0;
            m_HammerWarpHastenFrame = 0;
            m_HammerBeforeSpeedMod = chargemod * 1.2f;
            m_HammerWaveNum = 0;
            m_HammerWaveAngle = 0.0f;
            m_HammerWaveGapAngleStart = 0.0f;
            m_HammerWaveGapAngleEnd = 0.0f;
            m_HammerWaveLifetime = 1;
            m_HammerWaveScale = 1.0f;
            m_HammerWaveSpeed = 0.0f;
            m_HammerNoTurnFlag = false;
            m_HammerAfterWarpFlag = false;
            m_HammerSuperFlag = false;

            m_HammerQuakeFlag = true;
            m_HammerQuakeArea = ONE_BLOCK * 32.0f;

            m_HammerRumblingFlag = false;
        };

    auto hammer_great = [this](
        float chargemod = 1.0f
        )
        { //グレート
            m_CurrentState = ActionState::Hammer;
            m_HammerFrame = 0;

            m_HammerWarpNum = 0;
            m_HammerWarpHastenFrame = 0;
            m_HammerBeforeSpeedMod = chargemod;
            m_HammerWaveNum = 8;
            m_HammerWaveAngle = 360.0f;
            m_HammerWaveLifetime = 100;
            m_HammerWaveScale = 0.6f;
            m_HammerWaveSpeed = 60.0f;
            m_HammerWaveGapAngleStart = 0.0f;
            m_HammerWaveGapAngleEnd = 0.0f;
            m_HammerNoTurnFlag = false;
            m_HammerAfterWarpFlag = false;
            m_HammerSuperFlag = false;

            m_HammerQuakeFlag = false;

            m_HammerRumblingFlag = true;
            m_HammerRumblingNum = 3;
            m_HammerRumblingArea = ONE_BLOCK * 8.0f;
            m_HammerRumblingDistance = ONE_BLOCK * 20.0f;
            m_HammerRumblingDelay = 30;
        };

    auto fireworks_general = [this]()
        { //はなび
            m_CurrentState = ActionState::Fireworks;
            m_FireworksFrame = 0;

            m_FireworksWarpNum = 0;
            m_FireworksWarpHastenFrame = 0;
            m_FireworksBeforeFrame = 60;
            m_FireworksBulletNum = 1;
            m_FireworksBulletAngle = 0.0f;
            m_FireworksBulletLifetime = FRAME_BEAM_LIFETIME;
            m_FireworksBulletScale = 1.0f;
            m_FireworksBulletSpeed = 0.1f;
            m_FireworksNoTurnFlag = false;
            m_FireworksLaserFlag = false;
        };

    auto fireworks_general2 = [this]()
        { //はなび2
            m_CurrentState = ActionState::Fireworks;
            m_FireworksFrame = 0;

            m_FireworksWarpNum = 3;
            m_FireworksWarpHastenFrame = 0;
            m_FireworksBeforeFrame = 80;
            m_FireworksBulletNum = 1;
            m_FireworksBulletAngle = 0.0f;
            m_FireworksBulletLifetime = FRAME_BEAM_LIFETIME * 0.6f;
            m_FireworksBulletScale = 1.0f;
            m_FireworksBulletSpeed = 0.1f;
            m_FireworksNoTurnFlag = false;
            m_FireworksLaserFlag = false;
        };

    auto granade_general = [this]()
        { //グレネード
            m_CurrentState = ActionState::Granade;
            m_GranadeFrame = 0;
        };

    auto laser_general = [this]()
        { //laser
            m_CurrentState = ActionState::Laser;
            m_LaserFrame = 0;
            m_LaserWarpNum = 0;
            m_LaserWarpHastenFrame = 0;
            m_LaserBeforeFrame = 90;
            m_LaserBulletNum = 8;
            m_LaserBulletAngle = 90.0f;
            m_LaserBulletLifetime = FRAME_BEAM_LIFETIME;
            m_LaserBulletScale = 1.0f;
            m_LaserBulletSpeed = 2.0f;
            m_LaserBulletRapidNum = 1;
            m_LaserBulletRapidDelay = 10;
            m_LaserNoTurnFlag = false;
        };

    auto laser_general2 = [this](
        int warpnum = 3
        )
        { //laser2
            m_CurrentState = ActionState::Laser;
            m_LaserFrame = 0;
            m_LaserWarpNum = warpnum;
            m_LaserWarpHastenFrame = 0;
            m_LaserBeforeFrame = 60;
            m_LaserBulletNum = 3;
            m_LaserBulletAngle = 50.0f;
            m_LaserBulletLifetime = FRAME_BEAM_LIFETIME;
            m_LaserBulletScale = 1.0f;
            m_LaserBulletSpeed = 3.0f;
            m_LaserBulletRapidNum = 4;
            m_LaserBulletRapidDelay = 10;
            m_LaserNoTurnFlag = false;
        };

    auto fireworks_laser_general = [this]()
        { //しせん
            m_CurrentState = ActionState::Fireworks;
            m_FireworksFrame = 0;

            m_FireworksWarpNum = 3;
            m_FireworksWarpHastenFrame = 0;
            m_FireworksBeforeFrame = 60;
            m_FireworksBulletNum = 1;
            m_FireworksBulletAngle = 0.0f;
            m_FireworksBulletLifetime = FRAME_ONE_SECOND * 12;
            m_FireworksBulletScale = 1.0f;
            m_FireworksBulletSpeed = 0.0f;
            m_FireworksNoTurnFlag = false;
            m_FireworksLaserFlag = true;
            m_FireworksLaserDelay = FRAME_ONE_SECOND * 2;


            Float3 afpos = { ONE_BLOCK * 60.0f, ONE_BLOCK * -10.0f, 0.0f };
            float rot = -135.0f;
            StartWarp(afpos, rot, 25, true);

            m_FireworksWarpPos = {
                {
                    { ONE_BLOCK * -60.0f, ONE_BLOCK * -10.0f, 0.0f },
                    135.0f
                },
                {
                    { 0.0f, ONE_BLOCK * -26.0f, 0.0f },
                    0.0f
                },
            };
        };

    //{
    //    int actF = m_ActCycle % 2;

    //    if (m_WazaCooldown > 0)
    //    {
    //        m_WazaCooldown--;
    //        return;
    //    }

    //    if (actF == 0)
    //    {
    //        SetCameraModeAll(CAMERA_MODE_BOSS_CHASE_WIDEMIDDLE);
    //        StartWarpCfx(
    //            true,
    //            { 1.0f, 1.0f, 0.0f },
    //            0, 25,
    //            [this]()
    //            {
    //                //StartAnim(AnimState::Appeal_0, FRAME_ONE_SECOND);
    //            }
    //        );
    //        m_WazaCooldown = 90;
    //        StartAnim(AnimState::Appeal_0, m_WazaCooldown);
    //    }
    //    if (actF == 1)
    //    {
    //        SetCameraModeAll(CAMERA_MODE_BOSS_CHASE_WIDEMIDDLE);
    //        StartWarpCfx(
    //            false,
    //            { 0.0f, -300.0f, 0.0f },
    //            0, 25,
    //            [this]()
    //            {
    //            }
    //        );
    //        m_WazaCooldown = 90;
    //    }

    //    return;
    //}

    if (!m_phase2Flag)
    {
        if (m_Obj->status.hp <= m_Obj->status.hpMax * PHASE2_HP_BORDER_N)
        {
            m_phase2Flag = true;

            SetPostEffectShaderVariable((float)GetMainGameFrame(), 0);
            SetPostEffectShaderVariable(1.0f, 1);

            m_ActCycle = 0;
            m_ActCycleNum = 0;
            m_WazaCooldown = 50;
        }
    }

    if (m_phase2Flag)
    {
        int actF = m_ActCycle % 10;

        if (actF == 0)
        {
            fireworks_laser_general();
        }
        if (actF == 1)
        {
            Float3 afpos = { 0.0f, ONE_BLOCK * -20.0f, 0.0f };
            float rot = 0.0f;
            StartWarp(afpos, rot, 25, true);

            m_CurrentState = ActionState::SuperLaser;
            m_SuperLaserFrame = 0;
            m_SuperLaserVariant = 0;
        }
        if (actF == 2)
        {
            hammer_storm(m_ActCycleNum);
        }
        if (actF == 3)
        {
            fireworks_general2();
        }
        if (actF == 4)
        {
            hammer_quake(0.7f);
        }
        if (actF == 5)
        {
            granade_general();
        }
        if (actF == 6)
        {
            hammer_great(0.4f);
        }
        if (actF == 7)
        {
            laser_general2(((m_ActCycleNum + 2) * 7) % 3);
        }

        if (actF == 8)
        {
            fireworks_general2();
        }
        if (actF == 9)
        {
            hammer_general(
                m_ActCycleNum % 2 == 0 ? 2 : 1,
                0.7f,
                12,
                360.0f
            );
        }
        if (actF == 10)
        {
            hammer_general(
                m_ActCycleNum % 2 == 0 ? 2 : 1,
                0.3f,
                5,
                60.0f
            );
        }
    }
    else
    {
        int actF = m_ActCycle % 10;

        if (actF == 0)
        {
            Float3 afpos = { 0.0f, ONE_BLOCK * -20.0f, 0.0f };
            float rot = 0.0f;
            StartWarp(afpos, rot, 25, true);

            m_CurrentState = ActionState::SuperLaser;
            m_SuperLaserFrame = 0;
            m_SuperLaserVariant = 0;
        }
        if (actF == 1)
        {
            hammer_storm(m_ActCycleNum);
        }
        if (actF == 2)
        {
            fireworks_laser_general();
        }
        if (actF == 3)
        {
            granade_general();
        }
        if (actF == 4)
        {
            hammer_general(
                m_ActCycleNum % 2 == 0 ? 2 : 1,
                0.8f,
                7,
                200.0f
            );
        }

        if (actF == 5)
        {
            fireworks_general2();
        }
        if (actF == 6)
        {
            hammer_great(0.6f);
        }
        if (actF == 7)
        {
            hammer_quake(0.7f);
        }
        if (actF == 8)
        {
            laser_general2(((m_ActCycleNum + 2) * 7) % 3);
        }
        if (actF == 9)
        {
            hammer_great(0.6f);
        }
    }
}

void GOE_Character_Enemy_Boss_Kontoriusu::update_action_pattern_normal()
{
    if (m_StartedFrameCount < FRAME_ONE_SECOND * 2)
    {
        return;
    }

    m_ActCycleNum++;
    update_action_pattern_normal_base1();

    m_ActCycle++;
}

void GOE_Character_Enemy_Boss_Kontoriusu::update_action_pattern_hard()
{
    if (m_StartedFrameCount < FRAME_ONE_SECOND * 2)
    {
        return;
    }

    m_ActCycleNum++;
    update_action_pattern_hard_base1();

    //m_CurrentState = ActionState::Attack;
    //m_AttackActNum = actF;
    //m_AttackAct_Frame = 0;

    //m_CurrentState = ActionState::Test;
    //m_TestActNum = actF;
    //m_TestAct_Frame = 0;

    //if (actF == 0)
    //{ //通常
    //    m_CurrentState = ActionState::Hammer;
    //    m_HammerFrame = 0;

    //    m_HammerWarpNum = 0;
    //    m_HammerWarpHastenFrame = 0;
    //    m_HammerBeforeSpeedMod = 1.0f;
    //    m_HammerWaveNum = 3;
    //    m_HammerWaveAngle = 40.0f;
    //    m_HammerWaveLifetime = 80;
    //    m_HammerWaveScale = 1.0f;
    //    m_HammerWaveSpeed = 70.0f;
    //    m_HammerNoTurnFlag = false;
    //    m_HammerAfterWarpFlag = false;
    //    m_HammerSuperFlag = false;
    //    m_HammerQuakeFlag = false;
    //    m_HammerRumblingFlag = false;
    //}

    //if (actF == 1)
    //{ //アースストーム
    //    m_CurrentState = ActionState::Hammer;
    //    m_HammerFrame = 0;

    //    Float3 actionStdPos[3] = {
    //        { ONE_BLOCK * 50.0f, ONE_BLOCK * -28.0f, 0.0f },
    //        { ONE_BLOCK * -50.0f, ONE_BLOCK * -28.0f, 0.0f },
    //        { 0.0f, ONE_BLOCK * -30.0f, 0.0f },
    //    };
    //    Float3 afpos = actionStdPos[(m_ActCycleNum - 1) % 3];
    //    float rot = CalculateRadToDeg(atan2(-afpos.y, -afpos.x)) - 90.0f;
    //    StartWarp(afpos, rot, 25, true);

    //    m_HammerWarpNum = 0;
    //    m_HammerWarpHastenFrame = 0;
    //    m_HammerBeforeSpeedMod = 2.4f;
    //    m_HammerWaveNum = 16;
    //    m_HammerWaveAngle = 270.0f;
    //    m_HammerWaveLifetime = 360;
    //    m_HammerWaveScale = 1.2f;
    //    m_HammerWaveSpeed = 360.0f;
    //    m_HammerNoTurnFlag = true;
    //    m_HammerAfterWarpFlag = true;
    //    m_HammerSuperFlag = true;
    //    m_HammerQuakeFlag = false;
    //    m_HammerRumblingFlag = false;

    //}
    //if (actF == 2)
    //{ //クエイク
    //    m_CurrentState = ActionState::Hammer;
    //    m_HammerFrame = 0;

    //    m_HammerWarpNum = 0;
    //    m_HammerWarpHastenFrame = 0;
    //    m_HammerBeforeSpeedMod = 1.0f;
    //    m_HammerWaveNum = 0;
    //    m_HammerWaveAngle = 0.0f;
    //    m_HammerWaveLifetime = 1;
    //    m_HammerWaveScale = 1.0f;
    //    m_HammerWaveSpeed = 0.0f;
    //    m_HammerNoTurnFlag = false;
    //    m_HammerAfterWarpFlag = false;
    //    m_HammerSuperFlag = false;

    //    m_HammerQuakeFlag = true;
    //    m_HammerQuakeArea = ONE_BLOCK * 24.0f;

    //    m_HammerRumblingFlag = false;
    //}

    //if (actF == 3)
    //{ //グレート
    //    m_CurrentState = ActionState::Hammer;
    //    m_HammerFrame = 0;

    //    m_HammerWarpNum = 0;
    //    m_HammerWarpHastenFrame = 0;
    //    m_HammerBeforeSpeedMod = 1.0f;
    //    m_HammerWaveNum = 8;
    //    m_HammerWaveAngle = 360.0f;
    //    m_HammerWaveLifetime = 80;
    //    m_HammerWaveScale = 0.5f;
    //    m_HammerWaveSpeed = 50.0f;
    //    m_HammerNoTurnFlag = false;
    //    m_HammerAfterWarpFlag = false;
    //    m_HammerSuperFlag = false;

    //    m_HammerQuakeFlag = false;

    //    m_HammerRumblingFlag = true;
    //    m_HammerRumblingNum = 3;
    //    m_HammerRumblingArea = ONE_BLOCK * 8.0f;
    //    m_HammerRumblingDistance = ONE_BLOCK * 20.0f;
    //    m_HammerRumblingDelay = 30;
    //}

    //if (actF == 4)
    //{ //はなび
    //    m_CurrentState = ActionState::Fireworks;
    //    m_FireworksFrame = 0;

    //    m_FireworksWarpNum = 0;
    //    m_FireworksWarpHastenFrame = 0;
    //    m_FireworksBeforeFrame = 50;
    //    m_FireworksBulletNum = 1;
    //    m_FireworksBulletAngle = 0.0f;
    //    m_FireworksBulletLifetime = FRAME_BEAM_LIFETIME;
    //    m_FireworksBulletScale = 1.0f;
    //    m_FireworksBulletSpeed = 0.1f;
    //    m_FireworksNoTurnFlag = false;
    //    m_FireworksLaserFlag = false;
    //}
    //if (actF == 5)
    //{ //はなび2
    //    m_CurrentState = ActionState::Fireworks;
    //    m_FireworksFrame = 0;

    //    m_FireworksWarpNum = 3;
    //    m_FireworksWarpHastenFrame = 0;
    //    m_FireworksBeforeFrame = 90;
    //    m_FireworksBulletNum = 1;
    //    m_FireworksBulletAngle = 0.0f;
    //    m_FireworksBulletLifetime = FRAME_BEAM_LIFETIME;
    //    m_FireworksBulletScale = 1.0f;
    //    m_FireworksBulletSpeed = 0.1f;
    //    m_FireworksNoTurnFlag = false;
    //    m_FireworksLaserFlag = false;
    //}
    
    //if (actF == 6)
    //{ //グレネード
    //    m_CurrentState = ActionState::Granade;
    //    m_GranadeFrame = 0;
    //}
    //if (actF == 7)
    //{ //laser
    //    m_CurrentState = ActionState::Laser;
    //    m_LaserFrame = 0;
    //    m_LaserWarpNum = 0;
    //    m_LaserWarpHastenFrame = 0;
    //    m_LaserBeforeFrame = 90;
    //    m_LaserBulletNum = 8;
    //    m_LaserBulletAngle = 90.0f;
    //    m_LaserBulletLifetime = FRAME_BEAM_LIFETIME;
    //    m_LaserBulletScale = 1.0f;
    //    m_LaserBulletSpeed = 2.0f;
    //    m_LaserBulletRapidNum = 1;
    //    m_LaserBulletRapidDelay = 10;
    //    m_LaserNoTurnFlag = false;
    //}
    //if (actF == 8)
    //{ //laser
    //    m_CurrentState = ActionState::Laser;
    //    m_LaserFrame = 0;
    //    m_LaserWarpNum = 3;
    //    m_LaserWarpHastenFrame = 0;
    //    m_LaserBeforeFrame = 60;
    //    m_LaserBulletNum = 3;
    //    m_LaserBulletAngle = 60.0f;
    //    m_LaserBulletLifetime = FRAME_BEAM_LIFETIME;
    //    m_LaserBulletScale = 1.0f;
    //    m_LaserBulletSpeed = 3.0f;
    //    m_LaserBulletRapidNum = 4;
    //    m_LaserBulletRapidDelay = 10;
    //    m_LaserNoTurnFlag = false;
    //}

    //if (actF == 9)
    //{ //しせん
    //    m_CurrentState = ActionState::Fireworks;
    //    m_FireworksFrame = 0;

    //    m_FireworksWarpNum = 3;
    //    m_FireworksWarpHastenFrame = 0;
    //    m_FireworksBeforeFrame = 90;
    //    m_FireworksBulletNum = 1;
    //    m_FireworksBulletAngle = 0.0f;
    //    m_FireworksBulletLifetime = FRAME_ONE_SECOND * 10;
    //    m_FireworksBulletScale = 1.0f;
    //    m_FireworksBulletSpeed = 0.0f;
    //    m_FireworksNoTurnFlag = false;
    //    m_FireworksLaserFlag = true;
    //    m_FireworksLaserDelay = FRAME_ONE_SECOND * 2;


    //    Float3 afpos = { ONE_BLOCK * 60.0f, ONE_BLOCK * -10.0f, 0.0f };
    //    float rot = -135.0f;
    //    StartWarp(afpos, rot, 25, true);

    //    m_FireworksWarpPos = {
    //        {
    //            { ONE_BLOCK * -60.0f, ONE_BLOCK * -10.0f, 0.0f },
    //            135.0f
    //        },
    //        {
    //            { 0.0f, ONE_BLOCK * -26.0f, 0.0f },
    //            0.0f
    //        },
    //    };
    //}
    
    //if (actF == 0)
    //{ //
    //    m_ActCycleNum++;
    //    m_AttackActNum = actF;
    //}
    //else if (actF == 1)
    //{ //
    //    m_ActionCooldown = 50;

    //    StartTurn(MkF2(0.0f, 1.0f * (m_ActCycleNum % 2 == 0 ? -1 : 1)), 0.03f);
    //}
    //else if (actF == 2)
    //{ //
    //    m_ActionCooldown = 80;
    //    StartAnim(AnimState::Hammer, 24);
    //}
    //else if (actF == 3)
    //{ //
    //    m_ActionCooldown = 80;
    //    StartAnim(AnimState::Shoot, 60);
    //}
    //else if (actF == 4)
    //{ //
    //    m_ActionCooldown = 200;
    //}

    //if (actF == 0)
    //{ //
    //    m_ActCycleNum++;
    //    m_ActionCooldown = 180;
    //    StartMove(
    //        {0.0f, 100.0f * (m_ActCycleNum % 2 == 0 ? -1 : 1) },
    //        m_CurrentAxisPrivate, 120);
    //}
    //else if (actF == 1)
    //{ //
    //    m_ActionCooldown = 50;

    //    StartTurn(MkF2(0.0f, 1.0f * (m_ActCycleNum % 2 == 0 ? -1:1)), 0.03f);
    //}
    //else if (actF == 2)
    //{ //
    //    m_ActionCooldown = 80;
    //    StartAnim(AnimState::Hammer, 24);
    //}
    //else if (actF == 3)
    //{ //
    //    m_ActionCooldown = 80;
    //    StartAnim(AnimState::Shoot, 60);
    //}
    //else if (actF == 4)
    //{ //
    //    m_ActionCooldown = 200;
    //}

    m_ActCycle++;
}

void GOE_Character_Enemy_Boss_Kontoriusu::update_action_pattern_hard_base1()
{
    auto hammer_general = [this](
        int warpnum=0,
        float chargemod=1.0f,
        int wavenum = 3,
        float waveangle = 40.0f
        )
        { //通常
            m_CurrentState = ActionState::Hammer;
            m_HammerFrame = 0;

            m_HammerWarpNum = warpnum;
            m_HammerWarpHastenFrame = 0;
            m_HammerBeforeSpeedMod = chargemod * 0.6f;
            m_HammerWaveNum = wavenum;
            m_HammerWaveAngle = waveangle;
            m_HammerWaveLifetime = 80;
            m_HammerWaveScale = 1.0f;
            m_HammerWaveSpeed = 70.0f;
            m_HammerWaveGapAngleStart = 0.0f;
            m_HammerWaveGapAngleEnd = 0.0f;
            m_HammerNoTurnFlag = false;
            m_HammerAfterWarpFlag = false;
            m_HammerSuperFlag = false;
            m_HammerQuakeFlag = false;
            m_HammerRumblingFlag = false;
        };

    auto hammer_storm = [this](int posindex)
        { //アースストーム
            m_CurrentState = ActionState::Hammer;
            m_HammerFrame = 0;

            Float3 actionStdPos[3] = {
                { ONE_BLOCK * 50.0f, ONE_BLOCK * -28.0f, 0.0f },
                { ONE_BLOCK * -50.0f, ONE_BLOCK * -28.0f, 0.0f },
                { 0.0f, ONE_BLOCK * -30.0f, 0.0f },
            };
            Float3 afpos = actionStdPos[posindex % 3];
            float rot = CalculateRadToDeg(atan2(-afpos.y, -afpos.x)) - 90.0f;
            StartWarp(afpos, rot, 25, true);

            m_HammerWarpNum = 0;
            m_HammerWarpHastenFrame = 0;
            //m_HammerBeforeSpeedMod = 2.4f;
            m_HammerBeforeSpeedMod = 1.4f;
            m_HammerWaveNum = 48;
            m_HammerWaveAngle = 270.0f;
            m_HammerWaveLifetime = 600;
            m_HammerWaveScale = 1.0f;
            m_HammerWaveSpeed = 350.0f;

            {
                float gap = 0.4f * ((float)((
                    (m_ActCycleNum + 1) * 40)
                    % 100))
                    / 100.0f;
                m_HammerWaveGapAngleStart = 0.2f + gap;
                m_HammerWaveGapAngleEnd = 0.3f + gap;
            }

            m_HammerNoTurnFlag = true;
            m_HammerAfterWarpFlag = true;
            m_HammerSuperFlag = true;
            m_HammerQuakeFlag = false;
            m_HammerRumblingFlag = false;

        };

    auto hammer_quake = [this](
        float chargemod = 1.0f
        )
        { //クエイク
            m_CurrentState = ActionState::Hammer;
            m_HammerFrame = 0;

            m_HammerWarpNum = 0;
            m_HammerWarpHastenFrame = 0;
            m_HammerBeforeSpeedMod = chargemod * 1.2f;
            m_HammerWaveNum = 0;
            m_HammerWaveAngle = 0.0f;
            m_HammerWaveGapAngleStart = 0.0f;
            m_HammerWaveGapAngleEnd = 0.0f;
            m_HammerWaveLifetime = 1;
            m_HammerWaveScale = 1.0f;
            m_HammerWaveSpeed = 0.0f;
            m_HammerNoTurnFlag = false;
            m_HammerAfterWarpFlag = false;
            m_HammerSuperFlag = false;

            m_HammerQuakeFlag = true;
            m_HammerQuakeArea = ONE_BLOCK * 32.0f;

            m_HammerRumblingFlag = false;
        };

    auto hammer_great = [this](
        float chargemod = 1.0f
        )
        { //グレート
            m_CurrentState = ActionState::Hammer;
            m_HammerFrame = 0;

            m_HammerWarpNum = 0;
            m_HammerWarpHastenFrame = 0;
            m_HammerBeforeSpeedMod = chargemod;
            m_HammerWaveNum = 8;
            m_HammerWaveAngle = 360.0f;
            m_HammerWaveLifetime = 100;
            m_HammerWaveScale = 0.6f;
            m_HammerWaveSpeed = 60.0f;
            m_HammerWaveGapAngleStart = 0.0f;
            m_HammerWaveGapAngleEnd = 0.0f;
            m_HammerNoTurnFlag = false;
            m_HammerAfterWarpFlag = false;
            m_HammerSuperFlag = false;

            m_HammerQuakeFlag = false;

            m_HammerRumblingFlag = true;
            m_HammerRumblingNum = 3;
            m_HammerRumblingArea = ONE_BLOCK * 8.0f;
            m_HammerRumblingDistance = ONE_BLOCK * 20.0f;
            m_HammerRumblingDelay = 30;
        };

    auto fireworks_general = [this]()
    { //はなび
        m_CurrentState = ActionState::Fireworks;
        m_FireworksFrame = 0;

        m_FireworksWarpNum = 0;
        m_FireworksWarpHastenFrame = 0;
        m_FireworksBeforeFrame = 60;
        m_FireworksBulletNum = 1;
        m_FireworksBulletAngle = 0.0f;
        m_FireworksBulletLifetime = FRAME_BEAM_LIFETIME;
        m_FireworksBulletScale = 1.0f;
        m_FireworksBulletSpeed = 0.1f;
        m_FireworksNoTurnFlag = false;
        m_FireworksLaserFlag = false;
    };

    auto fireworks_general2 = [this]()
    { //はなび2
        m_CurrentState = ActionState::Fireworks;
        m_FireworksFrame = 0;

        m_FireworksWarpNum = 3;
        m_FireworksWarpHastenFrame = 0;
        m_FireworksBeforeFrame = 80;
        m_FireworksBulletNum = 1;
        m_FireworksBulletAngle = 0.0f;
        m_FireworksBulletLifetime = FRAME_BEAM_LIFETIME * 0.6f;
        m_FireworksBulletScale = 1.0f;
        m_FireworksBulletSpeed = 0.1f;
        m_FireworksNoTurnFlag = false;
        m_FireworksLaserFlag = false;
    };

    auto granade_general = [this]()
    { //グレネード
        m_CurrentState = ActionState::Granade;
        m_GranadeFrame = 0;
    };

    auto laser_general = [this]()
    { //laser
        m_CurrentState = ActionState::Laser;
        m_LaserFrame = 0;
        m_LaserWarpNum = 0;
        m_LaserWarpHastenFrame = 0;
        m_LaserBeforeFrame = 90;
        m_LaserBulletNum = 8;
        m_LaserBulletAngle = 90.0f;
        m_LaserBulletLifetime = FRAME_BEAM_LIFETIME;
        m_LaserBulletScale = 1.0f;
        m_LaserBulletSpeed = 2.0f;
        m_LaserBulletRapidNum = 1;
        m_LaserBulletRapidDelay = 10;
        m_LaserNoTurnFlag = false;
    };

    auto laser_general2 = [this](
        int warpnum = 3
        )
    { //laser2
        m_CurrentState = ActionState::Laser;
        m_LaserFrame = 0;
        m_LaserWarpNum = warpnum;
        m_LaserWarpHastenFrame = 0;
        m_LaserBeforeFrame = 60;
        m_LaserBulletNum = 3;
        m_LaserBulletAngle = 60.0f;
        m_LaserBulletLifetime = FRAME_BEAM_LIFETIME;
        m_LaserBulletScale = 1.0f;
        m_LaserBulletSpeed = 3.0f;
        m_LaserBulletRapidNum = 4;
        m_LaserBulletRapidDelay = 10;
        m_LaserNoTurnFlag = false;
    };

    auto fireworks_laser_general = [this]()
    { //しせん
        m_CurrentState = ActionState::Fireworks;
        m_FireworksFrame = 0;

        m_FireworksWarpNum = 3;
        m_FireworksWarpHastenFrame = 0;
        m_FireworksBeforeFrame = 60;
        m_FireworksBulletNum = 1;
        m_FireworksBulletAngle = 0.0f;
        m_FireworksBulletLifetime = FRAME_ONE_SECOND * 12;
        m_FireworksBulletScale = 1.0f;
        m_FireworksBulletSpeed = 0.0f;
        m_FireworksNoTurnFlag = false;
        m_FireworksLaserFlag = true;
        m_FireworksLaserDelay = FRAME_ONE_SECOND * 2;


        Float3 afpos = { ONE_BLOCK * 60.0f, ONE_BLOCK * -10.0f, 0.0f };
        float rot = -135.0f;
        StartWarp(afpos, rot, 25, true);

        m_FireworksWarpPos = {
            {
                { ONE_BLOCK * -60.0f, ONE_BLOCK * -10.0f, 0.0f },
                135.0f
            },
            {
                { 0.0f, ONE_BLOCK * -26.0f, 0.0f },
                0.0f
            },
        };
    };

    //{
    //    int actF = m_ActCycle % 2;

    //    if (m_WazaCooldown > 0)
    //    {
    //        m_WazaCooldown--;
    //        return;
    //    }

    //    if (actF == 0)
    //    {
    //        SetCameraModeAll(CAMERA_MODE_BOSS_CHASE_WIDEMIDDLE);
    //        StartWarpCfx(
    //            true,
    //            { 1.0f, 1.0f, 0.0f },
    //            0, 25,
    //            [this]()
    //            {
    //                //StartAnim(AnimState::Appeal_0, FRAME_ONE_SECOND);
    //            }
    //        );
    //        m_WazaCooldown = 90;
    //        StartAnim(AnimState::Appeal_0, m_WazaCooldown);
    //    }
    //    if (actF == 1)
    //    {
    //        SetCameraModeAll(CAMERA_MODE_BOSS_CHASE_WIDEMIDDLE);
    //        StartWarpCfx(
    //            false,
    //            { 0.0f, -300.0f, 0.0f },
    //            0, 25,
    //            [this]()
    //            {
    //            }
    //        );
    //        m_WazaCooldown = 90;
    //    }

    //    return;
    //}

    if (!m_phase2Flag)
    {
        if (m_Obj->status.hp <= m_Obj->status.hpMax * PHASE2_HP_BORDER_H)
        {
            m_phase2Flag = true;

            SetPostEffectShaderVariable((float)GetMainGameFrame(), 0);
            SetPostEffectShaderVariable(1.0f, 1);

            m_ActCycle = 0;
            m_ActCycleNum = 0;
            m_WazaCooldown = 60;
        }
    }

    if (m_phase2Flag)
    {
        int actF = m_ActCycle % 10;

        if (actF == 0)
        {
            Float3 afpos = { 0.0f, ONE_BLOCK * -10.0f, 0.0f };
            float rot = 0.0f;
            StartWarp(afpos, rot, 25, true);

            m_CurrentState = ActionState::SuperLaser;
            m_SuperLaserFrame = 0;
            m_SuperLaserVariant = m_ActCycleNum > 0 ? 1 : 0;
        }
        if (actF == 1)
        {
            hammer_storm(m_ActCycleNum);
        }
        if (actF == 2)
        {
            granade_general();
        }

        if (actF == 3)
        {
            fireworks_laser_general();
        }
        if (actF == 4)
        {
            hammer_general(
                m_ActCycleNum % 2 == 0 ? 2 : 1,
                0.8f,
                7,
                200.0f
            );
        }
        if (actF == 5)
        {
            laser_general2(((m_ActCycleNum + 2) * 7) % 3);
        }
        if (actF == 6)
        {
            hammer_great(0.6f);
        }
        if (actF == 7)
        {
            fireworks_general2();
        }
        if (actF == 8)
        {
            hammer_great(0.6f);
        }
        if (actF == 9)
        {
            hammer_quake(0.9f);
        }
    }
    else
    {
        int actF = m_ActCycle % 10;

        if (actF == 0)
        {
            laser_general();
        }
        if (actF == 1)
        {
            laser_general2(((m_ActCycleNum + 2) * 7) % 3);
        }
        if (actF == 2)
        {
            hammer_general(m_ActCycleNum % 2 == 0 ? 0 : 1);
        }
        if (actF == 3)
        {
            fireworks_general();
        }
        if (actF == 4)
        {
            hammer_general(
                0,
                0.75f,
                5,
                50.0f
            );
        }
        if (actF == 5)
        {
            fireworks_general2();
        }
        if (actF == 6)
        {
            hammer_quake();
        }
        if (actF == 7)
        {
            hammer_general(
                0,
                0.5f,
                5,
                50.0f
            );
        }
        if (actF == 8)
        {
            laser_general();
        }
        if (actF == 9)
        {
            hammer_great();
        }
    }
}

void GOE_Character_Enemy_Boss_Kontoriusu::safe_area_move()
{
    bool isOut = false;
    Float3 afterPos = {};
    {
        Stage_Base* stg = GetCurrentStageSceneGame();
        if (stg != nullptr)
        {
            Int2 stgsize = stg->GetStageSize();

            Float2 size = {};
            size.x = BLOCK_WIDTH * stgsize.x;
            size.y = BLOCK_HIGHT * stgsize.y;


            if (m_Obj->pos.y < size.y * -0.5f / 2.0f)
            {
                isOut = true;
                afterPos.x = m_Obj->pos.x;
                afterPos.y = ONE_BLOCK * -12.0f;
            }
            if (m_Obj->pos.y > size.y * 0.35f / 2.0f)
            {
                isOut = true;
                afterPos.x = m_Obj->pos.x;
                afterPos.y = ONE_BLOCK * -12.0f;
            }

            if (m_Obj->pos.x < size.x * -0.5f / 2.0f)
            {
                isOut = true;
                afterPos.x = size.x * -0.4f / 2.0f;
                afterPos.y = ONE_BLOCK * -12.0f;
            }
            if (m_Obj->pos.x > size.x * 0.5f / 2.0f)
            {
                isOut = true;
                afterPos.x = size.x * 0.4f / 2.0f;
                afterPos.y = ONE_BLOCK * -12.0f;
            }
        }
    }
    if (isOut)
    {
        StartWarp(afterPos, 0.0f, 25);
    }
}

void GOE_Character_Enemy_Boss_Kontoriusu::update_test()
{
    GAME_OBJECT* targetObject = GetGameObjectFindByIdentifier(m_TargetObjIdentifier);
    if (targetObject == nullptr)
    {
        return;
    }
    Float2 vec2d = NormalizeVector(MkF2(
        m_Obj->pos.x - targetObject->pos.x,
        m_Obj->pos.y - targetObject->pos.y
    ));

    bool isEnd = true;
    //if (m_TestActNum == 0)
    {
        isEnd = false;

        if (m_TestAct_Frame == 0)
        {
            set_hammer_visible(true);
            StartAnim(AnimState::HammerBefore, 60, true);
        }

        if (m_TestAct_Frame == 60)
        {
            StartAnim(AnimState::Hammer, 40);
        }

        if (m_TestAct_Frame == 75)
        {
            CameraShakeRequestAll(
                CAMERA_SHAKE_LARGE_FRAME, 
                CAMERA_SHAKE_LARGE_POWER
            );

            set_hammer_visible(false);
        }

        if (m_TestAct_Frame > 100)
        {
            isEnd = true;
        }
    }

    m_TestAct_Frame++;

    if (isEnd)
    {
        safe_area_move();

        m_ActionCooldown = m_WazaCooldown;
        m_TestAct_Frame = -1;
        m_CurrentState = ActionState::Idle;
    }
}

void GOE_Character_Enemy_Boss_Kontoriusu::draw_test()
{
}

void GOE_Character_Enemy_Boss_Kontoriusu::update_attack()
{

    GAME_OBJECT* targetObject = GetGameObjectFindByIdentifier(m_TargetObjIdentifier);
    if (targetObject == nullptr)
    {
        return;
    }
    Float2 vec2d = NormalizeVector(MkF2(
        m_Obj->pos.x - targetObject->pos.x,
        m_Obj->pos.y - targetObject->pos.y
    ));

    bool isEnd = true;
    if (m_AttackActNum == 0)
    {
        isEnd = false;

        if (m_AttackAct_Frame == 0)
        {
            StartWarp({0.0f, ONE_BLOCK * -20.0f, 0.0f}, 0.0f, 25);
        }
        if (m_AttackAct_Frame == 1)
        {
            //SEを再生
            SoundEffectPlayRequest(GetSoundData("SOUND/SE/inperiusu/b_kamae"),
                1.0f, 0, 1.6);

            StartAnim(AnimState::ShootBefore, 130);
        }
        if (m_AttackAct_Frame == 110)
        {
            { //うつよ
                CameraShakeRequestAll(CAMERA_SHAKE_LARGE_FRAME, CAMERA_SHAKE_LARGE_POWER);

                float angle = 180.0f;
                int bulletnum = 12;
                for (int i = 0; i < bulletnum; i++)
                {
                    //float radtmp = CalculateDegToRad(
                    //    m_CurrentAxisPrivate
                    //    + 180.0f + (angle - 180.0f) / 4 - (angle / bulletnum * i)
                    //);
                    float radtmp = CalculateDegToRad(
                        m_CurrentAxisPrivate + 90.0f
                        - (angle / 2) + (angle / bulletnum) * (0.5f + (float)i)
                    );
                    shootBeam(
                        {},
                        MkF3(
                            (float)cos(radtmp) * 1.0f,
                            (float)sin(radtmp) * 1.0f,
                            0.0f
                        ),
                        1.8f
                    );
                }
            }
        }

        if (m_AttackAct_Frame > 140)
        {
            isEnd = true;
        }
    }

    if (m_AttackActNum == 1)
    {
        isEnd = false;

        if (m_AttackAct_Frame == 0)
        {
            StartMove(
                MkF3(
                    targetObject->pos.x + vec2d.x * ONE_BLOCK * 6.0f,
                    targetObject->pos.y + vec2d.y * ONE_BLOCK * 6.0f,
                    targetObject->pos.z
                ),
                CalculateRadToDeg(atan2(-vec2d.y, -vec2d.x)) - 90.0f,
                3, true
            );
        }
        if (m_AttackAct_Frame == 1)
        {
            StartWarp(
                MkF3(
                    targetObject->pos.x + vec2d.x * ONE_BLOCK * -20.0f,
                    targetObject->pos.y + vec2d.y * ONE_BLOCK * -20.0f,
                    targetObject->pos.z
                ),
                CalculateRadToDeg(atan2(-vec2d.y, -vec2d.x) - 90.0f),
                25, true);
        }
        if (m_AttackAct_Frame == 2)
        {
            m_BeforeVecBuf.x = -vec2d.x;
            m_BeforeVecBuf.y = -vec2d.y;
            m_BeforeVecBuf.z = 0.0f;
            m_BeforeVec = m_BeforeVecBuf;

            //SEを再生
            SoundEffectPlayRequest(GetSoundData("SOUND/SE/inperiusu/b_kamae"),
                1.0f, 0, 1.6);

            StartAnim(AnimState::ShootBefore, 140);
        }

        if (m_AttackAct_Frame >= 70
            && m_AttackAct_Frame < 130
            && m_AttackAct_Frame % 10 == 0
        )
        {
            { //うつよ
                CameraShakeRequestAll(CAMERA_SHAKE_LARGE_FRAME, CAMERA_SHAKE_LARGE_POWER);

                float angle = 90.0f;
                int bulletnum = 5;
                for (int i = 0; i < bulletnum; i++)
                {
                    //float radtmp = CalculateDegToRad(
                    //    m_CurrentAxisPrivate
                    //    + 180.0f + (angle - 180.0f) / 4 - (angle / bulletnum * i)
                    //);
                    float radtmp = CalculateDegToRad(
                        m_CurrentAxisPrivate + 90.0f
                        - (angle / 2) + (angle / bulletnum) * (0.5f + (float)i)
                    );
                    shootBeam(
                        {},
                        MkF3(
                            (float)cos(radtmp) * 3.0f,
                            (float)sin(radtmp) * 3.0f,
                            0.0f
                        ),
                        1.2f
                    );
                }
            }
        }

        if (m_AttackAct_Frame > 150)
        {
            isEnd = true;
        }
    }

    if (m_AttackActNum == 2)
    {
        isEnd = false;

        if (m_AttackAct_Frame == 0)
        {
            StartMove(
                MkF3(
                    targetObject->pos.x + vec2d.x * ONE_BLOCK * 6.0f,
                    targetObject->pos.y + vec2d.y * ONE_BLOCK * 6.0f,
                    targetObject->pos.z
                ),
                CalculateRadToDeg(atan2(-vec2d.y, -vec2d.x)) - 90.0f,
                3, true
            );
        }

        if (m_AttackAct_Frame == 1)
        {
            //SEを再生
            SoundEffectPlayRequest(GetSoundData("SOUND/SE/inperiusu/Small_Explosion"),
                4.0f, 0, 1.1);

            StartAnim(AnimState::ShootBefore, 40);

            { //うつよ
                CameraShakeRequestAll(CAMERA_SHAKE_LARGE_FRAME, CAMERA_SHAKE_LARGE_POWER);

                float angle = 1.0f;
                int bulletnum = 1;
                for (int i = 0; i < bulletnum; i++)
                {
                    //float radtmp = CalculateDegToRad(
                    //    m_CurrentAxisPrivate
                    //    + 180.0f + (angle - 180.0f) / 4 - (angle / bulletnum * i)
                    //);
                    float radtmp = CalculateDegToRad(
                        m_CurrentAxisPrivate + 90.0f
                        - (angle / 2) + (angle / bulletnum) * (0.5f + (float)i)
                    );
                    shootBeam(
                        {},
                        MkF3(
                            (float)cos(radtmp) * 0.4f,
                            (float)sin(radtmp) * 0.4f,
                            -0.01f
                        ),
                        1.4f,
                        1
                    );
                }
            }
        }

        if (m_AttackAct_Frame > 90)
        {
            isEnd = true;
        }
    }
    if (m_AttackActNum == 3)
    {
        isEnd = false;

        {
            float deg = m_CurrentAxisPrivate - 90.0f;
            float rad = CalculateDegToRad(deg);
            Float2 vec = NormalizeVector(MkF2(
                -sin(rad),
                cos(rad)
            ));
            Float2 afterPos = MkF2(
                -ONE_BLOCK * 16.0f * vec.x + targetObject->pos.x,
                -ONE_BLOCK * 16.0f * vec.y + targetObject->pos.y
            );

            StartWarp(Float2ToFloat3(afterPos),
                CalculateRadToDeg(atan2(
                    targetObject->pos.y - afterPos.y,
                    targetObject->pos.x - afterPos.x
                ) - 90.0f),
                25, true);
        }

        if (m_AttackAct_Frame > 4)
        {
            isEnd = true;
        }
    }


    if (m_AttackActNum == 4)
    {
        isEnd = false;

        if (m_AttackAct_Frame == 0)
        {
            StartMove(
                MkF3(
                    targetObject->pos.x + vec2d.x * ONE_BLOCK * 6.0f,
                    targetObject->pos.y + vec2d.y * ONE_BLOCK * 6.0f,
                    targetObject->pos.z
                ),
                CalculateRadToDeg(atan2(-vec2d.y, -vec2d.x)) - 90.0f,
                3, true
            );
        }
        if (m_AttackAct_Frame == 1)
        {
            StartWarp(
                MkF3(
                    targetObject->pos.x + vec2d.x * ONE_BLOCK * -20.0f,
                    targetObject->pos.y + vec2d.y * ONE_BLOCK * -20.0f,
                    targetObject->pos.z
                ),
                CalculateRadToDeg(atan2(-vec2d.y, -vec2d.x) - 0.0f),
                25, true);
        }
        if (m_AttackAct_Frame == 2)
        {
            StartMove(
                MkF3(
                    targetObject->pos.x + vec2d.x * ONE_BLOCK * 6.0f,
                    targetObject->pos.y + vec2d.y * ONE_BLOCK * 6.0f,
                    targetObject->pos.z
                ),
                CalculateRadToDeg(atan2(-vec2d.y, -vec2d.x)) - 90.0f,
                3, true
            );
        }
        if (m_AttackAct_Frame == 3)
        {
            StartWarp(
                MkF3(
                    targetObject->pos.x + vec2d.x * ONE_BLOCK * -20.0f,
                    targetObject->pos.y + vec2d.y * ONE_BLOCK * -20.0f,
                    targetObject->pos.z
                ),
                CalculateRadToDeg(atan2(-vec2d.y, -vec2d.x) - 0.0f),
                25, true);
        }
        if (m_AttackAct_Frame == 4)
        {
            StartTurn(
                MkF2(
                    -vec2d.x,
                    -vec2d.y
                ),
                0.05f
            );
        }

        if (m_AttackAct_Frame > 5)
        {
            isEnd = true;
        }
    }

    if (m_AttackActNum == 5)
    {
        isEnd = false;

        if (m_AttackAct_Frame == 0)
        {
            StartMove(
                MkF3(
                    targetObject->pos.x + vec2d.x * ONE_BLOCK * -16.0f,
                    targetObject->pos.y + vec2d.y * ONE_BLOCK * -16.0f,
                    targetObject->pos.z
                ),
                CalculateRadToDeg(atan2(-vec2d.y, -vec2d.x)) - 90.0f,
                1.5, true
            );
        }

        if (m_AttackAct_Frame >= 1
            && m_AttackAct_Frame <= 3
        )
        {
            {
                float deg = m_CurrentAxisPrivate - 90.0f;
                float rad = CalculateDegToRad(deg);
                Float2 vec = NormalizeVector(MkF2(
                    -sin(rad),
                    cos(rad)
                ));
                Float2 afterPos = MkF2(
                    -ONE_BLOCK * 15.0f * vec.x + targetObject->pos.x,
                    -ONE_BLOCK * 15.0f * vec.y + targetObject->pos.y
                );

                StartWarp(Float2ToFloat3(afterPos),
                    CalculateRadToDeg(atan2(
                        targetObject->pos.y - afterPos.y,
                        targetObject->pos.x - afterPos.x
                    ) - 90.0f),
                    25, true);
            }

            //SEを再生
            SoundEffectPlayRequest(GetSoundData("SOUND/SE/inperiusu/Small_Explosion"),
                4.0f, 0, 1.1);

            StartAnim(AnimState::ShootBefore, 10);

            { //うつよ
                CameraShakeRequestAll(CAMERA_SHAKE_LARGE_FRAME, CAMERA_SHAKE_LARGE_POWER);

                float angle = 1.0f;
                int bulletnum = 1;
                for (int i = 0; i < bulletnum; i++)
                {
                    //float radtmp = CalculateDegToRad(
                    //    m_CurrentAxisPrivate
                    //    + 180.0f + (angle - 180.0f) / 4 - (angle / bulletnum * i)
                    //);
                    float radtmp = CalculateDegToRad(
                        m_CurrentAxisPrivate + 90.0f
                        - (angle / 2) + (angle / bulletnum) * (0.5f + (float)i)
                    );
                    shootBeam(
                        {},
                        MkF3(
                            (float)cos(radtmp) * 0.3f,
                            (float)sin(radtmp) * 0.3f,
                            -0.01f
                        ),
                        1.6f,
                        1
                    );
                }
            }
        }

        if (m_AttackAct_Frame > 90)
        {
            isEnd = true;
        }
    }

    m_AttackAct_Frame++;

    if (isEnd)
    {
        safe_area_move();

        m_ActionCooldown = m_WazaCooldown;
        m_AttackAct_Frame = -1;
        m_CurrentState = ActionState::Idle;
    }
}
void GOE_Character_Enemy_Boss_Kontoriusu::draw_attack()
{
}


void GOE_Character_Enemy_Boss_Kontoriusu::hammer_attack(int attackPointIndex)
{
    if (attackPointIndex < 0 || attackPointIndex >= m_HammerAttackPoint.size())
    {
        return;
    }

    float axisTmp = m_CurrentAxisPrivate;
    Float3 posBaseTmp = m_Obj->pos;

    float angle = m_HammerWaveAngle;
    int bulletnum = m_HammerWaveNum;

    for (int i = 0; i < bulletnum; i++)
    {
        {
            float angleModTmp = (1.0f / bulletnum) * i;

            if (m_HammerWaveGapAngleStart < angleModTmp
                && m_HammerWaveGapAngleEnd > angleModTmp)
            {
                continue;
            }
        }

        float radtmp = CalculateDegToRad(
            axisTmp + 90.0f
            - (angle / 2) + (angle / bulletnum) * (0.5f + (float)i)
        );

        Float3 vecWav = MkF3(
            cos(radtmp),
            sin(radtmp),
            0.0f
        );
        GOE_Character_EnemyProjectile* goepro =
            GOE_Character_EnemyProjectile::CreateProjectile(
                GOE_Character_EnemyProjectile::Type::ExplosionShockWave,
                m_HammerAttackPoint[attackPointIndex],
                vecWav
            );
        if (goepro != nullptr)
        {
            if (m_IsHard)
            {
                goepro->SetColor(MkF4(1.7f, 1.0f, 2.0f, 1.0f));
            }

            GAME_OBJECT* beamObj = goepro->GetGameObj();
            if (beamObj != nullptr)
            {
                beamObj->scale.x = 3.5f * m_HammerWaveScale;
                beamObj->scale.y = 3.5f * m_HammerWaveScale;
                beamObj->scale.z = 3.5f * m_HammerWaveScale;
                {
                    Character::MOTION_COL_DAT motionColDetails2 =
                        Character::GetMotionColDataFindByName(
                            Character::MOTION_COL_TYPE_ATK,
                            BEAM_ATK_COL_NAME
                        );

                    GAME_OBJECT_COLLISION_ATTACK_CHILD attackColTmp2 = motionColDetails2.atkCol;
                    attackColTmp2.subjectFamily[GAME_OBJECT_FAMILY_PLAYER] = true;
                    attackColTmp2.subjectFamily[GAME_OBJECT_FAMILY_GIMIK] = true;

                    attackColTmp2.size = MkF3(
                        ONE_BLOCK * 5,
                        ONE_BLOCK * 5,
                        ONE_BLOCK * 5
                    );
                    attackColTmp2.pos.z = -attackColTmp2.size.z / 2;
                    attackColTmp2.pos.y += attackColTmp2.size.y / 2 * 1.0f;

                    if (m_HammerSuperFlag)
                    {
                        attackColTmp2.invIgnore = true;

                        if (m_IsHard)
                        {

                            goepro->SetColor(MkF4(2.5f, 0.9f, 1.8f, 1.0f));
                        }
                        else
                        {

                            goepro->SetColor(MkF4(2.5f, 1.5f, 1.8f, 1.0f));
                        }

                    }

                    attackColTmp2.hitStopAttacker = 0;

                    beamObj->collision.atk.enable = true;
                    beamObj->collision.atk.ownerId = GetIdentifierGameObject(m_Obj);
                    beamObj->collision.atk.collision[0] = attackColTmp2;
                    beamObj->collision.atk.collision[0].enable = true;

                    //beamObj->collision.atk.collision[1] = beamObj->collision.atk.collision[0];
                    //beamObj->collision.atk.collision[1].subjectFamily[GAME_OBJECT_FAMILY_PLAYER] = false;
                    //beamObj->collision.atk.collision[1].subjectFamily[GAME_OBJECT_FAMILY_GIMIK] = true;
                    //beamObj->collision.atk.collision[1].hitCycle = 5;
                    //beamObj->collision.atk.collision[1].enable = true;

                }
                beamObj->blockCollisionIgnore = false;

                GameObjectSetLifeTime(beamObj, m_HammerWaveLifetime);
                GameObjectMoveRequest(beamObj, m_HammerWaveLifetime,
                    MkF3(
                        beamObj->pos.x + vecWav.x * ONE_BLOCK * m_HammerWaveSpeed,
                        beamObj->pos.y + vecWav.y * ONE_BLOCK * m_HammerWaveSpeed,
                        beamObj->pos.z + vecWav.z * ONE_BLOCK * m_HammerWaveSpeed
                    ),
                    nullptr
                );
            }
        }
    }

    if (m_HammerQuakeFlag)
    {
        for (int i = 0; i < PLAYER_MAX; i++)
        {
            PLAYER* player = GetPlayer(i);
            GAME_OBJECT* playerObj = player->obj;
            if (playerObj != nullptr)
            {
                if (CalculateDistance(
                    m_HammerAttackPoint[attackPointIndex],
                    playerObj->pos
                    ) < m_HammerQuakeArea
                )
                {
                    auto playerGOE =
                        dynamic_cast<GOE_Character_Player*>(playerObj->events.source);  // ダウンキャスト
                    if (playerGOE != nullptr)
                    {
                        playerGOE->SetStern(
                            m_HammerQuakeSternFrame,
                            true
                        );
                    }
                }

            }
        }


        //int summonEnemyNum = 5;
        //for (int i = 0; i < summonEnemyNum; i++)
        //{
        //    float rad = 2.0f * SYS_GENERAL_PI / summonEnemyNum * i;
        //    float dis = (BLOCK_WIDTH * 32);
        //    if (i % 2 != 0)
        //    {
        //        dis *= 0.75f;
        //    }


        //    GAME_OBJECT_COLLIDER collider = {};
        //    collider.type = GAME_OBJECT_COLLIDER_TYPE_CAPSULE;

        //    collider.capsule.radius = BLOCK_WIDTH
        //        * 4.0f
        //        ;

        //    //Float3 capVec = NormalizeVector(
        //    //    MkF3(
        //    //        ((float)(rand() % 1000) / 1000.0f) * 2.0f - 1.0f,
        //    //        ((float)(rand() % 1000) / 1000.0f) * 2.0f - 1.0f,
        //    //        0.0f
        //    //    )
        //    //);

        //    Float3 capVec = {
        //        sin(rad),
        //        cos(rad),
        //        0.0f
        //    };
        //    collider.capsule.pointA = capVec * BLOCK_WIDTH * +4.0f;
        //    collider.capsule.pointB = capVec * BLOCK_WIDTH * -4.0f;
        //    collider.capsule.pointA.z = collider.capsule.radius * 0.75f;
        //    collider.capsule.pointB.z = collider.capsule.radius * 0.75f;

        //    GOE_ObjectDynamic* goe = GOE_ObjectDynamic::CreateObjectDynamic(
        //        GOE_ObjectDynamic::Type::General,
        //        MkF3(
        //            m_Obj->pos.x + sin(rad) * dis,
        //            m_Obj->pos.y + cos(rad) * dis,
        //            BLOCK_WIDTH * -100.0f
        //        ),
        //        collider
        //    );
        //    goe->StartFall(2 * FRAME_ONE_SECOND);

        //    GameObjectHitStop(goe->GetGameObj(), 16 * i);

        //}


        int summonEnemyNum = 2;
        for (int i = 0; i < summonEnemyNum; i++)
        {

            int summonEnemyNum2 = 3 + 3 * i;
            for (int j = 0; j < summonEnemyNum2; j++)
            {
                float rad = 2.0f * SYS_GENERAL_PI / summonEnemyNum2 * j;

                Float2 dis = {
                    (BLOCK_WIDTH * 24) + (BLOCK_WIDTH * 48) * i,
                    (BLOCK_WIDTH * 16) + (BLOCK_WIDTH * 36) * i,
                };

                if (j % 2 != 0)
                {
                    dis.x *= 0.75f;
                    dis.y *= 0.75f;
                }


                GAME_OBJECT_COLLIDER collider = {};
                collider.type = GAME_OBJECT_COLLIDER_TYPE_CAPSULE;

                collider.capsule.radius = BLOCK_WIDTH
                    * 4.0f
                    ;

                Float3 capVec = {
                    sin(rad),
                    cos(rad),
                    0.0f
                };
                collider.capsule.pointA = capVec * BLOCK_WIDTH * +4.0f;
                collider.capsule.pointB = capVec * BLOCK_WIDTH * -4.0f;
                collider.capsule.pointA.z = collider.capsule.radius * 0.75f;
                collider.capsule.pointB.z = collider.capsule.radius * 0.75f;

                GOE_ObjectDynamic* goe = GOE_ObjectDynamic::CreateObjectDynamic(
                    GOE_ObjectDynamic::Type::General,
                    MkF3(
                        sin(rad) * dis.x,
                        cos(rad) * dis.y,
                        BLOCK_WIDTH * -100.0f
                    ),
                    collider
                );
                goe->SetOwner(GetIdentifierGameObject(m_Obj));
                goe->StartFall(2 * FRAME_ONE_SECOND);

                GameObjectHitStop(goe->GetGameObj(), 16 * i + 8 * j);

            }
        }
    }

    {
        Float3 postmp = {};
        postmp.x += m_HammerAttackPoint[attackPointIndex].x;
        postmp.y += m_HammerAttackPoint[attackPointIndex].y + ONE_BLOCK * 2.5f;
        //postmp.z += ;

        GAME_OBJECT* go = GO_CreateEffect(
            GO_EFFECT_TYPE_HAMMER_SHOCKWAVE,
            postmp);
        if (go != nullptr)
        {
            go->scale = MkF3(
                24.0f,
                24.0f,
                24.0f
            );
            GameObjectSetLifeTime(go, 32);
        }
    }
    {
        Float3 postmp = {};
        postmp.x += m_HammerAttackPoint[attackPointIndex].x;
        postmp.y += m_HammerAttackPoint[attackPointIndex].y;

        GAME_OBJECT* go = GO_CreateEffect(
            GO_EFFECT_TYPE_HAMMER_SHOCKDECAL,
            postmp);
        if (go != nullptr)
        {
            go->scale = MkF3(
                20.0f,
                20.0f,
                20.0f
            );
            GameObjectSetLifeTime(go, 30);
        }
    }

    if (m_HammerQuakeFlag)
    {
        CameraShakeRequestAll(
            CAMERA_SHAKE_EXLARGE_FRAME * 2.0f,
            CAMERA_SHAKE_EXLARGE_POWER * 3.0f
        );
    }
    else if (m_HammerRumblingFlag)
    {
        CameraShakeRequestAll(
            CAMERA_SHAKE_EXLARGE_FRAME,
            CAMERA_SHAKE_EXLARGE_POWER
        );
    }
    else
    {
        CameraShakeRequestAll(
            CAMERA_SHAKE_EXLARGE_FRAME,
            CAMERA_SHAKE_EXLARGE_POWER * 2.0f
        );
    }

    if (m_HammerSuperFlag)
    {
        SoundEffectPlayRequest(GetSoundData("SOUND/SE/enemy/attack/Small_Explosion"),
            3.2f, 0, 0.8f);
        SoundEffectPlayRequest(GetSoundData("SOUND/SE/enemy/attack/Explosion"),
            0.7f, 0, 0.6f);

        //SEを再生
        //SoundEffectPlayRequest(GetSoundData("SOUND/SE/inperiusu/b_attack"),
        //    2.4f, 0);
        SoundEffectPlayRequest(SOUND_TYPE_SE_9, 2.2f, 0, 0.7f);

        SoundEffectPlayRequest(
            GetSoundData("SOUND/SE/enemy/attack/super_beam_miminari"),
            0.65f, 3, 0.75f);
    }
    else
    {
        //SEを再生
        SoundEffectPlayRequest(GetSoundData("SOUND/SE/enemy/attack/Big_Explosion"),
            3.0f, 0, 0.9f);
        SoundEffectPlayRequest(SOUND_TYPE_SE_9, 2.2f, 0, 0.8f);

    }
}
void GOE_Character_Enemy_Boss_Kontoriusu::hammer_attack_all()
{
    for (int attackPointIndex = 0;
        attackPointIndex < m_HammerAttackPoint.size();
        attackPointIndex++)
    { //ウェーブ
        hammer_attack(attackPointIndex);
    }
}

void GOE_Character_Enemy_Boss_Kontoriusu::update_hammer()
{
    int ATTACK_HAMMER_ATTACKING_FRAME_STARTPOSE = m_Anim_FrameMax 
        * ATTACK_HAMMER_ATTACKING_PRESS_MOD
        * ATTACK_HAMMER_ATTACKING_PRESSIN_MOD
        ;
    int ATTACK_HAMMER_EFFECT_START_FRAME = ATTACK_HAMMER_ATTACKING_FRAME_STARTPOSE;
    int ATTACK_HAMMER_VANISH_FRAME = m_Anim_FrameMax * 0.7f;

    float ATTACK_CHARGE_MOD = (100.0f / (float)ATTACK_HAMMER_BEFORE_FRAME);

    if (m_HammerFrame == 0)
    {
        m_HammerAttackPoint.clear();
        m_HammerAttackStartFrame = -1;

    }

    Float3 vec = {};
    GAME_OBJECT* targetObject = GetGameObjectFindByIdentifier(m_TargetObjIdentifier);
    if (targetObject != nullptr)
    {
        float distance = CalculateDistance(targetObject->pos, m_Obj->pos);
        vec = NormalizeVector(MkF3(
            targetObject->pos.x - m_Obj->pos.x,
            targetObject->pos.y - m_Obj->pos.y,
            0.0f
        ));

        int dashF = m_HammerFrame;

        if (dashF == 0)
        {
        }

        {
            if (dashF == 1)
            {
                if (!m_HammerNoTurnFlag)
                {
                    StartTurn(Float3ToFloat2(m_TargetVec), 0.05f);
                }
            }
            if (dashF == 2)
            { //アニメーション

                {
                    Float3 attackVec = {
                        -sin(CalculateDegToRad(m_CurrentAxisPrivate)),
                        cos(CalculateDegToRad(m_CurrentAxisPrivate)),
                        0.0f,
                    };

                    Float3 attackPosBase = m_Obj->pos;
                    attackPosBase.x += ONE_BLOCK * 8.5 * attackVec.x;
                    attackPosBase.y += ONE_BLOCK * 8.5 * attackVec.y;
                    attackPosBase.z += 0.0f;
                    m_HammerAttackPoint.push_back(attackPosBase);

                    if (m_HammerRumblingFlag)
                    {
                        for (int i = 1; i < m_HammerRumblingNum; i++)
                        {
                            Float3 attackPos = attackPosBase;
                            attackPos.x += attackVec.x * m_HammerRumblingDistance * i;
                            attackPos.y += attackVec.y * m_HammerRumblingDistance * i;
                            attackPos.z += attackVec.z * m_HammerRumblingDistance * i;
                            m_HammerAttackPoint.push_back(attackPos);
                        }
                    }
                }

                //SEを再生
                //SoundEffectPlayRequest(GetSoundData("SOUND/SE/inperiusu/b_kamae"), 1.3f, 0);

                SoundEffectPlayRequest(GetSoundData("SOUND/SE/enemy/move/warpLow"), 1.5f, 0);

                if (m_HammerBeforeSpeedMod > 0.4f)
                {
                    //SEを再生
                    SoundEffectPlayRequest(
                        GetSoundData("SOUND/SE/enemy/attack/Beam_Charge"),
                        0.06f, 0, 4.0f
                    );
                    //SEを再生
                    //SoundEffectPlayRequest(GetSoundData("SOUND/SE/inperiusu/b_charge"), 1.3f, 5);
                }

                if (m_HammerSuperFlag)
                {
                    m_HeadState = HeadState::Eye;

                    //SEを再生
                    SoundEffectPlayRequest(
                        GetSoundData("SOUND/SE/enemy/attack/Beam_Charge"),
                        0.1f, 0, 1.2f * (1.1f / m_HammerBeforeSpeedMod)
                    );
                    //SEを再生
                    SoundEffectPlayRequest(
                        GetSoundData("SOUND/SE/enemy/attack/Annihilator_Charge"),
                        0.45f, ATTACK_HAMMER_BEFORE_FIXED_FRAME - 30,
                        1.25f * (2.2f / m_HammerBeforeSpeedMod)
                    );

                    //SetCameraModeAll(CAMERA_MODE_BOSS_CHASE_WIDEMIDDLE);
                    //SetCameraModeAll(CAMERA_MODE_BOSS_CHASE_WIDE);
                    SetCameraModeAll(CAMERA_MODE_BOSS_CHASE_EXWIDE);
                }

                if (m_HammerRumblingFlag)
                {
                    SetCameraModeAll(CAMERA_MODE_BOSS_CHASE_WIDEMIDDLE);
                }

                if (m_HammerWarpNum > 0)
                {
                    m_HeadState = (HeadState)((int)HeadState::Num_0 + (m_HammerWarpNum));
                }

                set_hammer_visible(true);
                StartAnim(AnimState::HammerBefore,
                    ATTACK_HAMMER_BEFORE_FRAME * m_HammerBeforeSpeedMod * ATTACK_CHARGE_MOD + 1
                );
                //StartBodyTexMask(
                //    ATTACK_BLADE_BEFORE_FRAME * m_HammerBeforeSpeedMod
                //    + ATTACK_BLADE_ATTACK_FRAME);
            }
        }

    }

    if (m_Anim_State == AnimState::HammerBefore)
    {
        if (ATTACK_HAMMER_BEFORE_FIXED_FRAME < m_Anim_FrameMax)
        {
            if (m_Anim_Frame >= ATTACK_HAMMER_BEFORE_FIXED_FRAME
                && m_Anim_Frame < m_Anim_FrameMax - 40)
            {
                if (m_HammerSuperFlag)
                {
                    if (m_Anim_Frame % 10 == 0)
                    {
                        CameraShakeRequestAll(
                            CAMERA_SHAKE_MIDIUM_FRAME,
                            CAMERA_SHAKE_LARGE_POWER * (0.1f + 0.9f * m_Anim_Mod)
                        );
                    }
                }

                if (m_Anim_Frame % 10 == 0)
                {
                    Float3 postmp = m_Obj->pos;
                    postmp.z += ONE_BLOCK * -4.0f;

                    GAME_OBJECT* go = GO_CreateEffect(
                        GO_EFFECT_TYPE_HAMMER_CHARGE_LOOP,
                        postmp);

                    if (go != nullptr)
                    {
                        go->graphic.color = MkF4(
                            1.0f,
                            1.0f,
                            1.0f,
                            0.8f
                        );
                        go->scale = MkF3(
                            16.0f * (1.0f + (1.4f * m_Anim_Mod)),
                            16.0f * (1.0f + (1.4f * m_Anim_Mod)),
                            16.0f * (1.0f + (1.4f * m_Anim_Mod))
                        );
                        GameObjectSetLifeTime(go, 30);
                    }
                }
            }

            if (m_Anim_Frame < m_Anim_FrameMax - 40)
            {
                //if (m_HammerWarpNum > 0)
                //{
                //    if (m_Anim_Frame % 3 == 0)
                //    {
                //        m_HeadState = (HeadState)(
                //            (int)HeadState::Num_0
                //            + (rand() % ((int)HeadState::Num_9 - (int)HeadState::Num_0 + 1))
                //            );
                //    }
                //}
                //else
                //{
                //}
            }
            if (m_Anim_Frame == m_Anim_FrameMax - 40)
            {
                //if (m_HammerWarpNum > 0)
                //{
                //    m_HeadState = (HeadState)((int)HeadState::Num_0 + (m_HammerWarpNum) + 1);
                //}
                //else
                //{
                //}

                {
                    Float3 postmp = m_Obj->pos;
                    postmp.z += ONE_BLOCK * -4.0f;

                    GAME_OBJECT* go = GO_CreateEffect(
                        GO_EFFECT_TYPE_HAMMER_CHARGE_END,
                        postmp);

                    if (go != nullptr)
                    {
                        go->graphic.color = MkF4(
                            1.0f,
                            1.0f,
                            1.0f,
                            0.8f
                        );
                        go->scale = MkF3(
                            24.0f * (1.0f + (1.4f * m_Anim_Mod)),
                            24.0f * (1.0f + (1.4f * m_Anim_Mod)),
                            24.0f * (1.0f + (1.4f * m_Anim_Mod))
                        );
                        GameObjectSetLifeTime(go, 30);
                    }
                }
            }
        }

        int startWarpF = std::max((int)(ATTACK_HAMMER_BEFORE_FRAME_STARTPOSE * 1.2
            * m_HammerBeforeSpeedMod - m_HammerWarpNum) - m_HammerWarpHastenFrame, 0);
        int endWarpF = std::max((int)(ATTACK_HAMMER_BEFORE_FRAME_STARTPOSE * 1.2
            * m_HammerBeforeSpeedMod) - m_HammerWarpHastenFrame, m_HammerWarpNum);

        if (m_Anim_Frame >= m_Anim_FrameMax - 1)
        {
            SoundEffectPlayRequest(
                GetSoundData("SOUND/SE/player/attack/Normal_Attack"),
                1.8f, 0.0f, 0.4f);

            StartAnim(AnimState::Hammer, ATTACK_HAMMER_ATTACK_FRAME, true);

            if (m_HammerWarpNum > 0)
            {
                m_HeadState = HeadState::W;
            }

        }
        else if (
            //m_IsHard &&
            m_Anim_Frame < endWarpF
            && m_Anim_Frame >= startWarpF)
        {
            GAME_OBJECT* targetObject = GetGameObjectFindByIdentifier(m_TargetObjIdentifier);
            if (targetObject != nullptr)
            {
                //SEを再生
                //SoundEffectPlayRequest(GetSoundData("SOUND/SE/inperiusu/b_attack"), 1.0f, 0);
                //SEを再生
                //SoundEffectPlayRequest(GetSoundData("SOUND/SE/Test_SE_Attack"), 0.8f, 0);

                float degMainTmp = m_CurrentAxisPrivate - 90.0f;
                {
                    float deg = degMainTmp;
                    float rad = CalculateDegToRad(deg);
                    Float2 vec = NormalizeVector(MkF2(
                        -sin(rad),
                        cos(rad)
                    ));
                    Float2 afterPos = {};
                    {
                        afterPos = MkF2(
                            -ONE_BLOCK * 16.0f * vec.x + targetObject->pos.x,
                            -ONE_BLOCK * 16.0f * vec.y + targetObject->pos.y
                        );
                    }

                    StartWarp(Float2ToFloat3(afterPos),
                        CalculateRadToDeg(atan2(
                            targetObject->pos.y - afterPos.y,
                            targetObject->pos.x - afterPos.x
                        ) - 90.0f),
                        25, true);
                }

            }
        }
    }

    //攻撃エフェクト、判定
    if (m_Anim_State == AnimState::Hammer
        && m_Anim_Frame >= ATTACK_HAMMER_EFFECT_START_FRAME
        )
    {
        if (m_Anim_Frame == ATTACK_HAMMER_EFFECT_START_FRAME)
        {
            if (m_HammerAttackPoint.size() > 0)
            {
                Float3 attackVec = {
                    -sin(CalculateDegToRad(m_CurrentAxisPrivate)),
                    cos(CalculateDegToRad(m_CurrentAxisPrivate)),
                    0.0f,
                };

                Float3 attackPosBase = m_Obj->pos;
                attackPosBase.x += ONE_BLOCK * 8.5 * attackVec.x;
                attackPosBase.y += ONE_BLOCK * 8.5 * attackVec.y;
                attackPosBase.z += 0.0f;
                m_HammerAttackPoint[0] = attackPosBase;
            }

            Character::MOTION_COL_DAT motionColDetails =
                Character::GetMotionColDataFindByName(
                    Character::MOTION_COL_TYPE_ATK,
                    BEAM_ATK_COL_NAME
                );
            GAME_OBJECT_COLLISION_ATTACK_CHILD attackColTmp = motionColDetails.atkCol;
            attackColTmp.subjectFamily[GAME_OBJECT_FAMILY_PLAYER] = true;
            attackColTmp.subjectFamily[GAME_OBJECT_FAMILY_GIMIK] = true;

            strcpy_s(attackColTmp.tag, ARRAYSIZE(attackColTmp.tag), "BLADE");

            //判定サイズ
            attackColTmp.size = MkF3(
                ONE_BLOCK * 10,
                ONE_BLOCK * 10,
                ONE_BLOCK * 10
            );
            attackColTmp.pos = m_HammerAttackPoint[0];
            attackColTmp.pos.x += -m_Obj->pos.x;
            attackColTmp.pos.y += -m_Obj->pos.y;
            attackColTmp.pos.z += -m_Obj->pos.z;
            attackColTmp.pos.z = -attackColTmp.size.z / 2;
            
            //GameObjectMoveRequest(m_Obj, 5,
            //    MkF3(
            //        m_Obj->pos.x + m_BeforeVec.x * ONE_BLOCK * 8.0f,
            //        m_Obj->pos.y + m_BeforeVec.y * ONE_BLOCK * 8.0f,
            //        m_Obj->pos.z
            //    ),
            //    nullptr //EasingInSine
            //);


            //攻撃判定を生成
            GameObjectAttackCollisionAdd(m_Obj, attackColTmp,
                0,
                //ATTACK_BLADE_ATTACK_START_FRAME,
                1
            );
            
            hammer_attack(0);

            m_HammerAttackStartFrame = 0;
        }
    }

    if (m_Anim_State == AnimState::Hammer
        && m_Anim_Frame == ATTACK_HAMMER_VANISH_FRAME
        )
    {
        set_hammer_visible(false);

        m_HeadState = HeadState::Idle;
    }

    if (m_HammerAttackStartFrame >= 0)
    {

        if (m_HammerRumblingFlag)
        {
            int f = m_HammerAttackStartFrame;
            if (m_HammerAttackStartFrame <= m_HammerRumblingDelay * (m_HammerAttackPoint.size() + 1)
                && (f % m_HammerRumblingDelay == 0)
            )
            {
                int index = f / m_HammerRumblingDelay;
                hammer_attack(index);
            }
        }

        m_HammerAttackStartFrame++;
    }

    m_HammerFrame++;

    if (m_HammerFrame > ATTACK_HAMMER_BEFORE_FRAME * m_HammerBeforeSpeedMod * ATTACK_CHARGE_MOD
        + ATTACK_HAMMER_ATTACK_FRAME + FRAME_ONE_SECOND * 0.25f
        )
    {
        m_ActionCooldown = m_WazaCooldown;
        m_HammerFrame = -1;
        m_CurrentState = ActionState::Idle;

        m_HeadState = HeadState::Idle;

        m_HammerAttackStartFrame = -1;

        SetCameraModeAll(CAMERA_MODE_BOSS_CHASE);
        {
            if (m_HammerAfterWarpFlag)
            {
                StartWarp({ 0.0f, ONE_BLOCK * -10.0f, 0.0f }, 0.0f, 25, true);
            }
            else
            {
                safe_area_move();
            }
        }
    }
}
void GOE_Character_Enemy_Boss_Kontoriusu::hammer_attack_area_draw()
{
    //float enterf = std::min((float)m_Init2Frame / GRANADE_AREA_ENTER_FRAME, 1.0f);
    float enterf = (float)m_HammerVisibleFrame / m_HammerVisibleFrameMax;

    DRAW_POLYGON_DAT drawDatTmp = {};

    drawDatTmp.rot = 0.0f;

    drawDatTmp.rot3D = MkF3(0.0f, 0.0f, 0.0f);

    drawDatTmp.texWH = { 1.0f , 1.0f };

    drawDatTmp.texUV = {
        0.0f,
        0.03f * m_Obj->frame
    };

    drawDatTmp.texNo = m_Tex_AttackArea;

    drawDatTmp.loadTexType = LOADTEXTURETYPE_MAIN;

    drawDatTmp.startPos3D.z = -5.0f;

    drawDatTmp.startRot3D = MkF3(90.0f, 0.0f, 0.0f);

    drawDatTmp.startScale = MkF2(1.0f, 1.0f);
    drawDatTmp.startScale3D = MkF3(1.0f, 1.0f, 1.0f);

    drawDatTmp.modelNo = m_Model_Wave;

    drawDatTmp.shaderOpt.bloom = DPD_SHADER_OPT_BLOOM_NONE;
    drawDatTmp.shaderOpt.blendState = DPD_SHADER_OPT_BLEND_STATE_ADDCOLOR;

    DRAW_POLYGON_IN_CAMERA_EX_OPTION opt = {};
    opt.reverse = false;
    opt.fixed = false;
    opt.enableShade = true;
    opt.billboard = false;
    //opt.priority = DRAW_POLYGON_PRIORITY_NORMAL;
    opt.priority = DRAW_POLYGON_PRIORITY_NML_d1;
    opt.depthBuffIgnore = true;


    Float3 postmp = drawDatTmp.startPos3D;

    if (m_HammerQuakeFlag)
    {
        drawDatTmp.color = MkF4(0.9f, 0.9f, 0.2f, 0.9f);
        drawDatTmp.size = MkF3(
            m_HammerQuakeArea * 2.0f / 100 * enterf,
            m_HammerQuakeArea * 2.0f / 100 * enterf,
            m_HammerQuakeArea * 2.0f / 100 * enterf
        );

        for (int i = 0; i < m_HammerAttackPoint.size(); i++)
        {
            drawDatTmp.startPos3D = postmp;
            drawDatTmp.startPos3D.x += m_HammerAttackPoint[i].x;
            drawDatTmp.startPos3D.y += m_HammerAttackPoint[i].y;
            drawDatTmp.startPos3D.z += m_HammerAttackPoint[i].z;
            DrawPolygonInCameraEx(drawDatTmp, opt);
        }
    }
    if (m_HammerRumblingFlag)
    {
        drawDatTmp.color = MkF4(0.9f, 0.0f, 0.2f, 0.9f);
        drawDatTmp.size = MkF3(
            m_HammerRumblingArea * 2.0f / 100 * enterf,
            m_HammerRumblingArea * 2.0f / 100 * enterf,
            m_HammerRumblingArea * 2.0f / 100 * enterf
        );

        for (int i = 0; i < m_HammerAttackPoint.size(); i++)
        {
            drawDatTmp.startPos3D = postmp;
            drawDatTmp.startPos3D.x += m_HammerAttackPoint[i].x;
            drawDatTmp.startPos3D.y += m_HammerAttackPoint[i].y;
            drawDatTmp.startPos3D.z += m_HammerAttackPoint[i].z;
            DrawPolygonInCameraEx(drawDatTmp, opt);
        }
    }
}
void GOE_Character_Enemy_Boss_Kontoriusu::draw_hammer()
{
    //bool  m_HammerQuakeFlag = false;
    //int   m_HammerQuakeSternFrame = FRAME_ONE_SECOND * 4;
    //float m_HammerQuakeArea = ONE_BLOCK * 10.0f;

    //bool  m_HammerRumblingFlag = false;
    //int   m_HammerRumblingNum = 3;
    //float m_HammerRumblingArea = ONE_BLOCK * 10.0f;
    //float m_HammerRumblingDistance = ONE_BLOCK * 16.0f;
    //int   m_HammerRumblingDelay = 30;

    hammer_attack_area_draw();
}

void GOE_Character_Enemy_Boss_Kontoriusu::fireworks_shoot()
{
    StartAnim(AnimState::ShootAfter, 20);

    {
        float radtmp = CalculateDegToRad(
            m_CurrentAxisPrivate + 90.0f
        );
        Float2 nvec = {
            (float)cos(radtmp),
            (float)sin(radtmp),
        };
        Float3 postmp = m_Obj->pos;
        postmp.x += nvec.x * ONE_BLOCK * 3.0f;
        postmp.y += nvec.y * ONE_BLOCK * 3.0f;
        postmp.z += ONE_BLOCK * -3.0f;

        GAME_OBJECT* go = GO_CreateEffect(
            GO_EFFECT_TYPE_HAMMER_CHARGE_END_NON_BILLBOARD,
            postmp);

        if (go != nullptr)
        {
            go->graphic.rot3d.x = 90.0f;
            go->graphic.rot3d.y = m_CurrentAxisPrivate;

            go->graphic.color = MkF4(
                1.0f,
                1.0f,
                1.0f,
                0.8f
            );
            go->scale = MkF3(
                6.0f,
                6.0f,
                6.0f
            );
            GameObjectSetLifeTime(go, 40);
        }
    }

    { //うつよ
        CameraShakeRequestAll(CAMERA_SHAKE_LARGE_FRAME, CAMERA_SHAKE_LARGE_POWER);

        float angle = m_FireworksBulletAngle;
        int bulletnum = m_FireworksBulletNum;
        for (int i = 0; i < bulletnum; i++)
        {
            float radtmp = CalculateDegToRad(
                m_CurrentAxisPrivate + 90.0f
                - (angle / 2) + (angle / bulletnum) * (0.5f + (float)i)
            );

            shootBeam(
                {
                    0.0f,
                    0.0f,
                    m_FireworksLaserFlag ? ONE_BLOCK * 2.0f: 0.0f
                },
                MkF3(
                    (float)cos(radtmp) * m_FireworksBulletSpeed,
                    (float)sin(radtmp) * m_FireworksBulletSpeed,
                    -0.01f
                ),
                1.6f * m_FireworksBulletScale,
                m_FireworksBulletLifetime,
                m_FireworksLaserFlag ? 2: 1
            );
        }
    }

    //SEを再生
    SoundEffectPlayRequest(
        GetSoundData("SOUND/SE/inperiusu/Small_Explosion"),
        4.0f, 0, 1.1);

}

void GOE_Character_Enemy_Boss_Kontoriusu::update_fireworks()
{
    bool isEnd = false;

    Float3 vec2d = {};
    GAME_OBJECT* targetObject = GetGameObjectFindByIdentifier(m_TargetObjIdentifier);
    if (targetObject != nullptr)
    {
        float distance = CalculateDistance(targetObject->pos, m_Obj->pos);
        vec2d = NormalizeVector(MkF3(
            targetObject->pos.x - m_Obj->pos.x,
            targetObject->pos.y - m_Obj->pos.y,
            0.0f
        ));



        if (m_FireworksFrame == 0)
        {
            if (!m_FireworksNoTurnFlag)
            {
                StartTurn(Float3ToFloat2(m_TargetVec), 0.05f);
            }
            //StartMove(
            //    MkF3(
            //        targetObject->pos.x + vec2d.x * ONE_BLOCK * -16.0f,
            //        targetObject->pos.y + vec2d.y * ONE_BLOCK * -16.0f,
            //        targetObject->pos.z
            //    ),
            //    CalculateRadToDeg(atan2(-vec2d.y, -vec2d.x)) - 90.0f,
            //    1.5, true
            //);
        }
        if (m_FireworksFrame == 1)
        {
            StartAnim(
                AnimState::ShootBefore,
                m_FireworksBeforeFrame + 1,
                true
            );
        }

        if (m_FireworksFrame >= 10
            && m_FireworksFrame < m_FireworksBeforeFrame - 40)
        {
            if (m_Anim_Frame % 10 == 0)
            {
                Float3 postmp = m_Obj->pos;
                postmp.z += ONE_BLOCK * -4.0f;

                GAME_OBJECT* go = GO_CreateEffect(
                    GO_EFFECT_TYPE_HAMMER_CHARGE_LOOP,
                    postmp);

                if (go != nullptr)
                {
                    go->graphic.color = MkF4(
                        1.0f,
                        1.0f,
                        1.0f,
                        0.8f
                    );
                    go->scale = MkF3(
                        12.0f * (1.0f + (1.4f * m_Anim_Mod)),
                        12.0f * (1.0f + (1.4f * m_Anim_Mod)),
                        12.0f * (1.0f + (1.4f * m_Anim_Mod))
                    );
                    GameObjectSetLifeTime(go, 30);
                }
            }
        }
        if (m_FireworksFrame == m_FireworksBeforeFrame - 40)
        {
            {
                Float3 postmp = m_Obj->pos;
                postmp.z += ONE_BLOCK * -4.0f;

                GAME_OBJECT* go = GO_CreateEffect(
                    GO_EFFECT_TYPE_HAMMER_CHARGE_END,
                    postmp);

                if (go != nullptr)
                {
                    go->graphic.color = MkF4(
                        1.0f,
                        1.0f,
                        1.0f,
                        0.8f
                    );
                    go->scale = MkF3(
                        20.0f * (1.0f + (1.4f * m_Anim_Mod)),
                        20.0f * (1.0f + (1.4f * m_Anim_Mod)),
                        20.0f * (1.0f + (1.4f * m_Anim_Mod))
                    );
                    GameObjectSetLifeTime(go, 30);
                }
            }
        }

        //ワープうち
        if (m_FireworksWarpNum > 0)
        {
            if (
                m_FireworksFrame >= m_FireworksBeforeFrame
                && m_FireworksFrame < m_FireworksWarpNum + m_FireworksBeforeFrame
                )
            {
                int posIndex = (m_FireworksFrame - m_FireworksBeforeFrame);
                if (posIndex >= 0 && posIndex < m_FireworksWarpPos.size())
                {
                    StartWarp(
                        m_FireworksWarpPos[posIndex].first,
                        m_FireworksWarpPos[posIndex].second,
                        25, true);
                }
                else
                {
                    float deg = m_CurrentAxisPrivate - 90.0f;
                    float rad = CalculateDegToRad(deg);
                    Float2 vec = NormalizeVector(MkF2(
                        -sin(rad),
                        cos(rad)
                    ));
                    Float2 afterPos = MkF2(
                        -ONE_BLOCK * 15.0f * vec.x + targetObject->pos.x,
                        -ONE_BLOCK * 15.0f * vec.y + targetObject->pos.y
                    );

                    StartWarp(Float2ToFloat3(afterPos),
                        CalculateRadToDeg(atan2(
                            targetObject->pos.y - afterPos.y,
                            targetObject->pos.x - afterPos.x
                        ) - 90.0f),
                        25, true);
                }

                fireworks_shoot();
            }
            if (m_FireworksFrame == m_FireworksWarpNum + m_FireworksBeforeFrame + 1)
            {
                m_FireworksWarpPosBuf = m_Obj->pos;

                if (m_IsHard)
                {
                    SetCameraModeAll(CAMERA_MODE_BOSS_CHASE_WIDEMIDDLE);
                    m_HeadState = HeadState::W;

                    int num = m_ActCycle % 3;
                    if (num == 0)
                    {
                        StartAnim(AnimState::Appeal_1, 28);
                        StartWarpCfx(
                            true,
                            { 55.0f, 20.0f, 0.0f },
                            0, 25,
                            [this]()
                            {
                                //StartAnim(AnimState::Appeal_0, FRAME_ONE_SECOND);
                            }
                        );
                    }
                    else if (num == 1)
                    {
                        StartAnim(AnimState::Appeal_2, 28);
                        StartWarpCfx(
                            true,
                            { -55.0f, 10.0f, 0.0f },
                            0, 25,
                            [this]()
                            {
                                //StartAnim(AnimState::Appeal_0, FRAME_ONE_SECOND);
                            }
                        );
                    }
                    else
                    {
                        StartAnim(AnimState::Appeal_0, 28);
                        StartWarpCfx(
                            true,
                            { 0.0f, 25.0f, 0.0f },
                            0, 25,
                            [this]()
                            {
                                //StartAnim(AnimState::Appeal_0, FRAME_ONE_SECOND);
                            }
                        );
                    }
                }
            }
        }
        else //その場うち
        {
            if (m_FireworksFrame == m_FireworksBeforeFrame)
            {
                fireworks_shoot();
            }
        }

    }

    if (m_FireworksFrame > m_FireworksBeforeFrame + 29)
    {
        if (m_IsPosCfx)
        {
            m_HeadState = HeadState::Idle;
            StartWarpCfx(
                false,
                m_FireworksWarpPosBuf,
                0, 25,
                [this]()
                {
                    StartAnim(AnimState::Idle, 1);
                    for (int i = 0; i < MODEL_NUM; i++)
                    {
                        m_ModelTransPos[i] = {};
                        m_ModelTransRot[i] = {};
                        m_ModelTransSca[i].x = 1.0f;
                        m_ModelTransSca[i].y = 1.0f;
                        m_ModelTransSca[i].z = 1.0f;
                        m_ModelTransPosStart[i] = {};
                        m_ModelTransRotStart[i] = {};
                        m_ModelTransScaStart[i].x = 1.0f;
                        m_ModelTransScaStart[i].y = 1.0f;
                        m_ModelTransScaStart[i].z = 1.0f;
                    }
                }
            );
        }
    }

    if (m_FireworksFrame > m_FireworksBeforeFrame + 30)
    {
        isEnd = true;
    }

    m_FireworksFrame++;

    if (isEnd)
    {
        safe_area_move();

        m_FireworksWarpPos.clear();

        m_ActionCooldown = m_WazaCooldown;
        m_FireworksFrame = -1;
        m_CurrentState = ActionState::Idle;
    }
}

void GOE_Character_Enemy_Boss_Kontoriusu::draw_fireworks()
{
}


void GOE_Character_Enemy_Boss_Kontoriusu::laser_shoot()
{
    StartAnim(AnimState::ShootAfter, 32767);

    {
        float radtmp = CalculateDegToRad(
            m_CurrentAxisPrivate + 90.0f
        );
        Float2 nvec = {
            (float)cos(radtmp),
            (float)sin(radtmp),
        };
        Float3 postmp = m_Obj->pos;
        postmp.x += nvec.x * ONE_BLOCK * 3.0f;
        postmp.y += nvec.y * ONE_BLOCK * 3.0f;
        postmp.z += ONE_BLOCK * -3.0f;

        GAME_OBJECT* go = GO_CreateEffect(
            GO_EFFECT_TYPE_HAMMER_CHARGE_END_NON_BILLBOARD,
            postmp);

        if (go != nullptr)
        {
            go->graphic.rot3d.x = 90.0f;
            go->graphic.rot3d.y = m_CurrentAxisPrivate;

            go->graphic.color = MkF4(
                1.0f,
                1.0f,
                1.0f,
                0.8f
            );
            go->scale = MkF3(
                6.0f,
                6.0f,
                6.0f
            );
            GameObjectSetLifeTime(go, 40);
        }
    }

    { //うつよ
        CameraShakeRequestAll(CAMERA_SHAKE_LARGE_FRAME, CAMERA_SHAKE_LARGE_POWER);

        float angle = m_LaserBulletAngle;
        int bulletnum = m_LaserBulletNum;
        for (int i = 0; i < bulletnum; i++)
        {
            float radtmp = CalculateDegToRad(
                m_CurrentAxisPrivate + 90.0f
                - (angle / 2) + (angle / bulletnum) * (0.5f + (float)i)
            );
            shootBeam(
                {},
                MkF3(
                    (float)cos(radtmp) * m_LaserBulletSpeed,
                    (float)sin(radtmp) * m_LaserBulletSpeed,
                    0.0f
                ),
                1.5f * m_LaserBulletScale,
                m_LaserBulletLifetime,
                0
            );
        }
    }
}
void GOE_Character_Enemy_Boss_Kontoriusu::update_laser()
{
    bool isEnd = false;

    Float3 vec2d = {};
    GAME_OBJECT* targetObject = GetGameObjectFindByIdentifier(m_TargetObjIdentifier);
    if (targetObject != nullptr)
    {
        float distance = CalculateDistance(targetObject->pos, m_Obj->pos);
        vec2d = NormalizeVector(MkF3(
            targetObject->pos.x - m_Obj->pos.x,
            targetObject->pos.y - m_Obj->pos.y,
            0.0f
        ));



        if (m_LaserFrame == 0)
        {
            if (!m_LaserNoTurnFlag)
            {
                StartTurn(Float3ToFloat2(m_TargetVec), 0.05f);
            }
            //StartMove(
            //    MkF3(
            //        targetObject->pos.x + vec2d.x * ONE_BLOCK * -16.0f,
            //        targetObject->pos.y + vec2d.y * ONE_BLOCK * -16.0f,
            //        targetObject->pos.z
            //    ),
            //    CalculateRadToDeg(atan2(-vec2d.y, -vec2d.x)) - 90.0f,
            //    1.5, true
            //);
        }
        if (m_LaserFrame == 1)
        {
            if (m_LaserWarpNum > 0)
            {
                m_HeadState = (HeadState)((int)HeadState::Num_0 + (m_LaserWarpNum));
            }

            StartAnim(
                AnimState::ShootBefore,
                m_LaserBeforeFrame + 1,
                true
            );
        }

        if (m_LaserFrame == ATTACK_LASER_TARGET_START_FRAME)
        {
            //SEを再生
            SoundEffectPlayRequest(
                GetSoundData("SOUND/SE/inperiusu/lockon"),
                0.28f, 0, 0.9f);
        }

        m_BeforeVecBuf = m_TargetVec;

        if (m_LaserFrame >= ATTACK_LASER_TARGET_START_FRAME
            && m_LaserFrame < m_LaserBeforeFrame - ATTACK_LASER_TARGET_ENDBEFORE_FRAME)
        {
            if (m_Anim_Frame % 10 == 0)
            {
                Float3 postmp = m_Obj->pos;
                postmp.z += ONE_BLOCK * -4.0f;

                GAME_OBJECT* go = GO_CreateEffect(
                    GO_EFFECT_TYPE_HAMMER_CHARGE_LOOP,
                    postmp);

                if (go != nullptr)
                {
                    go->graphic.color = MkF4(
                        1.0f,
                        1.0f,
                        1.0f,
                        0.8f
                    );
                    go->scale = MkF3(
                        12.0f * (1.0f + (1.4f * m_Anim_Mod)),
                        12.0f * (1.0f + (1.4f * m_Anim_Mod)),
                        12.0f * (1.0f + (1.4f * m_Anim_Mod))
                    );
                    GameObjectSetLifeTime(go, 30);
                }
            }
        }
        if (m_LaserFrame == m_LaserBeforeFrame - ATTACK_LASER_TARGET_ENDBEFORE_FRAME)
        {
            {
                Float3 postmp = m_Obj->pos;
                postmp.z += ONE_BLOCK * -4.0f;

                GAME_OBJECT* go = GO_CreateEffect(
                    GO_EFFECT_TYPE_HAMMER_CHARGE_END,
                    postmp);

                if (go != nullptr)
                {
                    go->graphic.color = MkF4(
                        1.0f,
                        1.0f,
                        1.0f,
                        0.8f
                    );
                    go->scale = MkF3(
                        20.0f * (1.0f + (1.4f * m_Anim_Mod)),
                        20.0f * (1.0f + (1.4f * m_Anim_Mod)),
                        20.0f * (1.0f + (1.4f * m_Anim_Mod))
                    );
                    GameObjectSetLifeTime(go, 30);
                }
            }
        }

        if (
            m_LaserFrame >= m_LaserBeforeFrame - m_LaserWarpNum
            && m_LaserFrame < m_LaserBeforeFrame
            )
        {
            {
                float deg = m_CurrentAxisPrivate - 90.0f;
                float rad = CalculateDegToRad(deg);
                Float2 vec = NormalizeVector(MkF2(
                    -sin(rad),
                    cos(rad)
                ));
                Float2 afterPos = MkF2(
                    -ONE_BLOCK * 32.0f * vec.x + targetObject->pos.x,
                    -ONE_BLOCK * 32.0f * vec.y + targetObject->pos.y
                );

                StartWarp(Float2ToFloat3(afterPos),
                    CalculateRadToDeg(atan2(
                        targetObject->pos.y - afterPos.y,
                        targetObject->pos.x - afterPos.x
                    ) - 90.0f),
                    25, true);
            }
        }

        if (m_LaserFrame == m_LaserBeforeFrame)
        {
            //laser_shoot();
            m_HeadState = HeadState::W;
        }

        if (
            m_LaserFrame >= m_LaserBeforeFrame
            && m_LaserFrame < m_LaserBeforeFrame + m_LaserBulletRapidNum * m_LaserBulletRapidDelay
            && (m_LaserFrame - m_LaserBeforeFrame) % m_LaserBulletRapidDelay == 0
        )
        {
            laser_shoot();
        }
        if (m_LaserFrame == m_LaserBeforeFrame + m_LaserBulletRapidNum * m_LaserBulletRapidDelay)
        {
            StartAnim(AnimState::ShootAfter, 30);
        }
        if (m_LaserFrame == m_LaserBeforeFrame + m_LaserBulletRapidNum * m_LaserBulletRapidDelay
            + 10)
        {
            m_HeadState = HeadState::Idle;
        }
    }

    if (m_LaserFrame > m_LaserBeforeFrame
        + m_LaserBulletRapidNum * m_LaserBulletRapidDelay + 30
    )
    {
        isEnd = true;
    }

    m_LaserFrame++;

    if (isEnd)
    {
        safe_area_move();

        m_ActionCooldown = m_WazaCooldown;
        m_LaserFrame = -1;
        m_CurrentState = ActionState::Idle;
    }
}

void GOE_Character_Enemy_Boss_Kontoriusu::draw_laser()
{
    GAME_OBJECT* targetObject = GetGameObjectFindByIdentifier(
        m_TargetObjIdentifier);
    if (targetObject != nullptr)
    {
        if (m_LaserFrame >= ATTACK_LASER_TARGET_START_FRAME
            && m_LaserFrame < m_LaserBeforeFrame
                + m_LaserBulletRapidNum * m_LaserBulletRapidDelay
        )
        {
            float enterf = std::min((float)(m_LaserFrame - ATTACK_LASER_TARGET_START_FRAME) / 12, 1.0f);
            for (int i = 0; i < 3; i++)
            {
                DRAW_POLYGON_DAT drawDatTmp = {};
                drawDatTmp.size = MkF3(
                    400.0f * 2.0f * enterf,
                    400.0f * 2.0f * enterf,
                    400.0f * 2.0f * enterf
                );

                if (i == 2)
                {
                    drawDatTmp.rot = 3.0f * m_Obj->frame;
                    drawDatTmp.color = MkF4(1.7f, 1.7f, 0.5f, 0.7f);
                    drawDatTmp.shaderOpt.blendState = DPD_SHADER_OPT_BLEND_STATE_ADDCOLOR;
                }
                else if (i == 1)
                {
                    drawDatTmp.rot = -5.0f * m_Obj->frame;
                    drawDatTmp.color = MkF4(1.7f, 1.7f, 0.5f, 0.7f);
                    drawDatTmp.shaderOpt.blendState = DPD_SHADER_OPT_BLEND_STATE_ADDCOLOR;
                }
                else
                {
                    drawDatTmp.color = MkF4(1.8f, 1.9f, 2.0f, 0.8f);
                    drawDatTmp.shaderOpt.blendState = DPD_SHADER_OPT_BLEND_STATE_ADDCOLOR;
                }

                drawDatTmp.rot3D = MkF3(0.0f, 0.0f, 0.0f);

                drawDatTmp.texWH = { 1.0f , 1.0f };

                drawDatTmp.texUV = {
                    0.0f,
                    0.0f
                };

                TEX_DAT texno[3] = {
                    m_Tex_cursor_000 ,
                    m_Tex_cursor_010 ,
                    m_Tex_cursor_020 ,
                };
                drawDatTmp.texNo = texno[i].textureId;

                drawDatTmp.loadTexType = LOADTEXTURETYPE_MAIN;

                drawDatTmp.startPos3D = targetObject->pos;
                drawDatTmp.startPos3D.z = ONE_BLOCK * -3.0f;

                drawDatTmp.startRot3D = MkF3(0.0f, 0.0f, 0.0f);

                drawDatTmp.startScale = MkF2(1.0f, 1.0f);
                drawDatTmp.startScale3D = MkF3(1.0f, 1.0f, 1.0f);

                drawDatTmp.modelNo = 0;

                drawDatTmp.shaderOpt.bloom = DPD_SHADER_OPT_BLOOM_ALWAYS;
                //drawDatTmp.shaderOpt.outline = DPD_SHADER_OPT_OUTLINE_RED;

                DRAW_POLYGON_IN_CAMERA_EX_OPTION opt = {};
                opt.reverse = false;
                opt.fixed = false;
                opt.enableShade = true;
                opt.billboard = true;
                opt.priority = DRAW_POLYGON_PRIORITY_NML_u1;
                opt.depthBuffIgnore = true;

                DrawPolygonInCameraEx(drawDatTmp, opt);
            }
        }
    }
}



void GOE_Character_Enemy_Boss_Kontoriusu::update_superlaser()
{
    bool isBeamCombo = 0;

    if (m_SuperLaserFrame == 0)
    {
        m_SuperLaserAfterFrame = -1;
        m_SuperLaser_Objects.clear();
    }

    if (m_SuperLaserFrame == 1)
    {
        //StartTurn(Float3ToFloat2(m_TargetVec), 0.05f);
    }

    if (m_SuperLaserFrame == 2)
    {
        //m_BeforeVecBuf = m_TargetVec;

                //SEを再生
        //SoundEffectPlayRequest(GetSoundData("SOUND/SE/inperiusu/b_kamae"), 1.3f, 0);

        SoundEffectPlayRequest(GetSoundData("SOUND/SE/enemy/move/warpLow"), 1.5f, 0);

        StartAnim(AnimState::ShootBefore,
            FRAME_GRANADE_START,
            true);

        SetCameraModeAll(CAMERA_MODE_BOSS_CHASE_WIDEMIDDLE);
    }
    if (m_SuperLaserFrame == FRAME_GRANADE_START + 2)
    {
        StartAnim(AnimState::ShootAfter,
            FRAME_GRANADE_AFTER,
            true);
    }

    if (m_SuperLaserFrame == FRAME_GRANADE_START + 10)
    {
        Float3 startposvec = NormalizeVector(m_BeforeVec);
        Float3 startpos = MkF3(
            m_Obj->pos.x + startposvec.x * 640.0f,
            m_Obj->pos.y + startposvec.y * 640.0f,
            m_Obj->pos.z - 300.0f
        );

        SoundEffectPlayRequest(GetSoundData("SOUND/SE/enemy/attack/Small_Explosion"), 3.0f, 0);
        //SEを再生
        //SoundEffectPlayRequest(GetSoundData("SOUND/SE/inperiusu/b_charge"), 0.5f, 5);
        //SEを再生
        //SoundEffectPlayRequest(GetSoundData("SOUND/SE/enemy/move/Inperius_Jet_End"), 1.2f, 35);

        CameraShakeRequestAll(
            CAMERA_SHAKE_LARGE_FRAME,
            CAMERA_SHAKE_EXLARGE_POWER * 0.8f
        );

        {
            Float3 efPos1 = startpos;
            //efPos1.z += ONE_BLOCK * -5.0f;
            efPos1.z += ONE_BLOCK * 3.0f;

            GAME_OBJECT* go = GO_CreateEffect(GO_EFFECT_TYPE_EXPLOSION_1, efPos1);
            if (go != nullptr)
            {
                go->scale = MkF3(
                    10.0f,
                    10.0f,
                    10.0f
                );
                GameObjectSetLifeTime(go, 45);
            }
        }

        m_Obj->vel.x = m_BeforeVec.x * -120.0f;
        m_Obj->vel.y = m_BeforeVec.y * -120.0f;
        {
            int granadenum = 1;

            for (int i = 0; i < granadenum; i++)
            {
                //(void)rand();

                GOE_Character_EnemyProjectile* goepro =
                    GOE_Character_EnemyProjectile::CreateProjectile(
                        GOE_Character_EnemyProjectile::Type::EnergyBallBomb,
                        startpos,
                        MkF3(
                            m_BeforeVec.x * 70.0f,
                            m_BeforeVec.y * 70.0f,
                            m_BeforeVec.z * 70.0f
                        )
                    );
                if (goepro != nullptr)
                {
                    GAME_OBJECT* beamObj = goepro->GetGameObj();
                    if (beamObj != nullptr)
                    {
                        beamObj->friction = 0.06f;

                        int BOMB_EXPLOSION_START_FRAME = FRAME_ONE_SECOND * 4;
                        int BOMB_EXPLOSION_START_BEFORE_FRAME = BOMB_EXPLOSION_START_FRAME - 80;
                        int BOMB_EXPLOSION_STARTING_FRAME = 40;
                        int BOMB_EXPLOSION_ATTACK_FRAME = 180;
                        int BOMB_EXPLOSION_AFTER_FRAME = 90;
                        int BOMB_AREA = ONE_BLOCK * 16.0f;

                        goepro->SetNumFlag(BOMB_EXPLOSION_START_FRAME, 0);
                        goepro->SetNumFlag(BOMB_AREA, 1);
                        goepro->SetNumFlag(BOMB_EXPLOSION_START_BEFORE_FRAME, 2);
                        goepro->SetNumFlag(BOMB_EXPLOSION_ATTACK_FRAME, 3);
                        goepro->SetNumFlag(BOMB_EXPLOSION_STARTING_FRAME, 4);
                        goepro->SetNumFlag(BOMB_EXPLOSION_AFTER_FRAME, 5);
                        goepro->SetNumFlag(m_SuperLaserVariant, 6);

                        
                        m_SuperLaser_Objects.push_back(GetIdentifierGameObject(beamObj));
                    }
                }
            }
        }
    }


    if (m_SuperLaserFrame == FRAME_GRANADE_START + FRAME_GRANADE_AFTER + 10)
    {
        if (!m_IsHard)
        {
            SetCameraModeAll(CAMERA_MODE_BOSS_CHASE);

            //StartWarp({ 0.0f, ONE_BLOCK * -15.0f, 0.0f }, 0.0f, 25, true);

            //m_ActionCooldown = m_WazaCooldown;
            m_ActionCooldown = 20;
            m_SuperLaserFrame = -1;
            m_CurrentState = ActionState::Idle;

            return;
        }

        if (isBeamCombo)
        {
            StartTurn(0.0f, 0.05f);
            //{ //多方向レーザー
            //    m_BeamFrame = 0;
            //    m_BeamNum = 3;
            //    m_BeamIsOutLine = false;
            //    m_BeamNoInitMove = true;
            //    m_BeamAfterWarpFlag = false;
            //    m_BeamRotatePattarn = 1;
            //    m_CurrentState = ActionState::Beam;
            //}
        }
        else
        {
            StartWarp({ 0.0f, ONE_BLOCK * -100.0f, ONE_BLOCK * -10000.0f },
                0.0f, 25, true);
        }
    }

    m_SuperLaserFrame++;

    {
        bool superGranadeEnd = false;
        if (m_SuperLaserFrame >= FRAME_ONE_SECOND * 12)
        {
            superGranadeEnd = true;
        }
        else
        {
            if (m_SuperLaserAfterFrame >= 0)
            {
                //if (m_SuperLaserAfterFrame >= FRAME_ONE_SECOND * 2)
                if (m_SuperLaserAfterFrame >= 1)
                {
                    superGranadeEnd = true;
                }

                m_SuperLaserAfterFrame++;
            }
            else if (
                m_SuperLaserFrame >=
                FRAME_GRANADE_START
                + FRAME_GRANADE_AFTER + FRAME_ONE_SECOND
                )
            {
                int count = 0;
                for (const auto& obid : m_SuperLaser_Objects)
                {
                    GAME_OBJECT* ob = GetGameObjectFindByIdentifier(obid);
                    if (ob != nullptr)
                    {
                        count++;
                    }
                }
                if (count <= 0)
                {
                    m_SuperLaserAfterFrame = 0;
                }
            }
        }

        if (superGranadeEnd)
        {
            SetCameraModeAll(CAMERA_MODE_BOSS_CHASE);

            StartWarp({ 0.0f, ONE_BLOCK * -15.0f, 0.0f }, 0.0f, 25, true);

            m_ActionCooldown = m_WazaCooldown;
            m_SuperLaserFrame = -1;
            m_CurrentState = ActionState::Idle;
        }
    }
}

void GOE_Character_Enemy_Boss_Kontoriusu::draw_superlaser()
{
}


void GOE_Character_Enemy_Boss_Kontoriusu::update_granade()
{
    bool isBeamCombo = 0;

    if (m_GranadeFrame == 0)
    {
        m_GranadeAfterFrame = -1;
        m_Granade_Objects.clear();
    }

    if (m_GranadeFrame == 1)
    {
        StartTurn(Float3ToFloat2(m_TargetVec), 0.05f);
    }

    if (m_GranadeFrame == 2)
    {
        //m_BeforeVecBuf = m_TargetVec;

                //SEを再生
        //SoundEffectPlayRequest(GetSoundData("SOUND/SE/inperiusu/b_kamae"), 1.3f, 0);

        SoundEffectPlayRequest(GetSoundData("SOUND/SE/enemy/move/warpLow"), 1.5f, 0);

        StartAnim(AnimState::GranadeBefore,
            FRAME_GRANADE_START,
            true);

        SetCameraModeAll(CAMERA_MODE_BOSS_CHASE_WIDE);
    }
    if (m_GranadeFrame == FRAME_GRANADE_START + 2)
    {
        StartAnim(AnimState::Granade,
            FRAME_GRANADE_AFTER,
            true);
    }

    if (m_GranadeFrame == FRAME_GRANADE_START + 10)
    {
        Float3 startpos = MkF3(
            m_Obj->pos.x + m_BeforeVec.x * 300.0f,
            m_Obj->pos.y + m_BeforeVec.y * 300.0f,
            m_Obj->pos.z + ONE_BLOCK * -9.0f
        );

        SoundEffectPlayRequest(GetSoundData("SOUND/SE/enemy/attack/Small_Explosion"), 3.0f, 0);
        //SEを再生
        //SoundEffectPlayRequest(GetSoundData("SOUND/SE/inperiusu/b_charge"), 0.5f, 5);
        //SEを再生
        //SoundEffectPlayRequest(GetSoundData("SOUND/SE/enemy/move/Inperius_Jet_End"), 1.2f, 35);

        CameraShakeRequestAll(
            CAMERA_SHAKE_LARGE_FRAME,
            CAMERA_SHAKE_EXLARGE_POWER * 0.8f
        );

        {
            Float3 efPos1 = startpos;
            //efPos1.z += ONE_BLOCK * -5.0f;
            efPos1.z += ONE_BLOCK * 3.0f;

            GAME_OBJECT* go = GO_CreateEffect(GO_EFFECT_TYPE_EXPLOSION_1, efPos1);
            if (go != nullptr)
            {
                go->scale = MkF3(
                    10.0f,
                    10.0f,
                    10.0f
                );
                GameObjectSetLifeTime(go, 45);
            }
        }

        {
            int granadenum = 5;

            for (int i = 0; i < granadenum; i++)
            {
                //(void)rand();

                GOE_Character_EnemyProjectile* goepro =
                    GOE_Character_EnemyProjectile::CreateProjectile(
                        GOE_Character_EnemyProjectile::Type::Granade,
                        startpos,
                        {}
                    );
                if (goepro != nullptr)
                {
                    GAME_OBJECT* beamObj = goepro->GetGameObj();
                    if (beamObj != nullptr)
                    {
                        bool isFinal = i == (granadenum - 1);

                        beamObj->scale = MkF3(
                            2.0f,
                            2.0f,
                            2.0f
                        );

                        Float3 hitpos = {};


                        beamObj->freeInt = 30 * i;

                        if (isFinal)
                        {
                            goepro->SetTarget(m_TargetObjIdentifier);

                            beamObj->scale = MkF3(
                                2.2f,
                                2.2f,
                                2.2f
                            );
                            if (isBeamCombo)
                            {
                            }
                            beamObj->freeInt += 60;
                        }
                        else
                        {
                            goepro->SetTarget(m_TargetObjIdentifier);
                        }

                        if (isBeamCombo)
                        {
                        }
                        else
                        {
                        }

                        goepro->SetNumFlag(isFinal ? 2 : 1, 0);
                        goepro->SetNumFlag(1, 1);
                        goepro->SetNumFlag(isFinal ? 10 : 5, 2);

                        m_Granade_Objects.push_back(GetIdentifierGameObject(beamObj));
                    }
                }
            }
        }
    }


    if (m_GranadeFrame == FRAME_GRANADE_START + FRAME_GRANADE_AFTER + 10)
    {
        if (!m_IsHard)
        {
            //SetCameraModeAll(CAMERA_MODE_BOSS_CHASE);

            //StartWarp({ 0.0f, ONE_BLOCK * -15.0f, 0.0f }, 0.0f, 25, true);

            //m_ActionCooldown = m_WazaCooldown;
            m_ActionCooldown = 20;
            m_GranadeFrame = -1;
            m_CurrentState = ActionState::Idle;
            return;
        }

        if (isBeamCombo)
        {
            StartTurn(0.0f, 0.05f);
            //{ //多方向レーザー
            //    m_BeamFrame = 0;
            //    m_BeamNum = 3;
            //    m_BeamIsOutLine = false;
            //    m_BeamNoInitMove = true;
            //    m_BeamAfterWarpFlag = false;
            //    m_BeamRotatePattarn = 1;
            //    m_CurrentState = ActionState::Beam;
            //}
        }
        else
        {
            if (m_IsHard)
            {
                StartWarp({ 0.0f, ONE_BLOCK * -100.0f, ONE_BLOCK * -10000.0f },
                    0.0f, 25, true);
            }
            else
            {
                StartWarp({ 0.0f, ONE_BLOCK * -80.0f, ONE_BLOCK * -20.0f },
                    0.0f, 25, true);
            }
        }
    }

    m_GranadeFrame++;

    {
        bool superGranadeEnd = false;
        if (m_GranadeFrame >= FRAME_ONE_SECOND * 12)
        {
            superGranadeEnd = true;
        }
        else
        {
            if (m_GranadeAfterFrame >= 0)
            {
                if (m_GranadeAfterFrame >= FRAME_ONE_SECOND * 2)
                {
                    superGranadeEnd = true;
                }

                m_GranadeAfterFrame++;
            }
            else if (
                m_GranadeFrame >=
                FRAME_GRANADE_START
                + FRAME_GRANADE_AFTER + FRAME_ONE_SECOND
                )
            {
                int count = 0;
                for (const auto& obid : m_Granade_Objects)
                {
                    GAME_OBJECT* ob = GetGameObjectFindByIdentifier(obid);
                    if (ob != nullptr)
                    {
                        count++;
                    }
                }
                if (count <= 0)
                {
                    m_GranadeAfterFrame = 0;
                }
            }
        }

        if (superGranadeEnd)
        {
            SetCameraModeAll(CAMERA_MODE_BOSS_CHASE);

            StartWarp({ 0.0f, ONE_BLOCK * -15.0f, 0.0f }, 0.0f, 25, true);

            m_ActionCooldown = m_WazaCooldown;
            m_GranadeFrame = -1;
            m_CurrentState = ActionState::Idle;
        }
    }
}

void GOE_Character_Enemy_Boss_Kontoriusu::draw_granade()
{
}



void GOE_Character_Enemy_Boss_Kontoriusu::event_playerPosSet()
{
    int apnum = GetActivePlayerNum();
    for (int i = 0; i < PLAYER_MAX; i++)
    {
        PLAYER* pl = GetPlayer(i);
        if (pl != nullptr)
        {
            GAME_OBJECT* plObj = pl->obj;
            if (plObj != nullptr)
            {
                plObj->pos.x = (1.0f * i - 0.5f * (apnum - 1)) * ONE_BLOCK * 3.0f;
                plObj->pos.y = ONE_BLOCK * 8.0f;
                plObj->pos.z = 0;

                GameObjectMoveRequest(plObj, 4,
                    MkF3(
                        plObj->pos.x,
                        plObj->pos.y - 1.0f,
                        plObj->pos.z
                    )
                );

                //Float3 postmp = {};
                //postmp.x = (1.0f * i - 0.5f * (apnum - 1)) * ONE_BLOCK * 3.0f;
                //postmp.y = ONE_BLOCK * 8.0f;
                //postmp.z = 0;
                //GameObjectMoveRequest(plObj, FRAME_ONE_SECOND * 2.0f,
                //    MkF3(
                //        postmp.x,
                //        postmp.y - ONE_BLOCK,
                //        postmp.z
                //    ),
                //    EasingInOutSine,
                //    EasingInOutSine,
                //    EasingInOutSine,
                //    true
                //);
            }
        }
    }
}

void GOE_Character_Enemy_Boss_Kontoriusu::become_start()
{
    m_HeadState = HeadState::Idle;

    if (m_StartMotionBeforeShaderVS != -1)
    {
        SetPostEffectShaderVS(m_StartMotionBeforeShaderVS);
    }
    if (m_StartMotionBeforeShaderPS != -1)
    {
        SetPostEffectShaderPS(m_StartMotionBeforeShaderPS);
    }

    ResetPostEffectGlobalOutlineWidth();

    m_StartMotionShaderFlag = false;

    //判定有効化
    m_Obj->collision.atk.enable = true;
    m_Obj->collision.rec.enable = true;
    m_Obj->status.statusType = GAME_OBJECT_STATUS_TYPE_ENEMY_BOSS;
    m_Obj->graphic.shaderOpt.outline = DPD_SHADER_OPT_OUTLINE_NORMAL;
    m_Obj->graphic.shaderOpt.outlineLayer = 1;

    //m_Obj->status.guardType = GAME_OBJECT_GUARD_TYPE_SHORT;

    m_Obj->pos.z = 0.0f;


    //ボスのHPを表示する
    ShowBossbar_Game(
        GetGameObjectIndex(m_Obj),
        true,
        m_Obj->status.fullName,
        m_IsHard ? BOSSBAR_TYPE_GAME::Contrius_H
        : BOSSBAR_TYPE_GAME::Contrius
    );

    //SetCameraTargetPosAll(MkF3(0, 0, 0));
}

void GOE_Character_Enemy_Boss_Kontoriusu::update_start()
{
    SOUND_ID bgmidtmp = GetSoundData("SOUND/BGM/ISEKI_BOSS_LOOP");
    
    //二回目以降
    if (m_RetryCount > 0)
    {
        if (m_StartFrame == 0)
        {
            GE_GayaCancelRequestAll();
            SoundBGMStopRequest();
        }
        if (m_StartFrame == 3)
        {
            SetCameraModeAll(CAMERA_MODE_BOSS_CHASE);
        }
        if (m_StartFrame == FRAME_ONE_SECOND * 1.0)
        {
            StartWarp(
                MkF3(
                    m_Obj->pos.x,
                    m_Obj->pos.y,
                    0.0f
                ),
                0.0f,
                30
            );
        }

        if (m_StartFrame == FRAME_ONE_SECOND * 1.1)
        {
            //BGMを再生する
            SoundBGMPlayRequest(bgmidtmp,
                true,
                2.0f,
                0.0
            );

            become_start();
        }
        if (m_StartFrame == FRAME_ONE_SECOND * 1.5)
        {
            m_HeadState = HeadState::Idle;

            m_StartedFlag = true;
        }
    }
    else
    { //初回
        if (m_StartFrame == 0)
        {
            m_HeadState = HeadState::None;

            GE_GayaCancelRequestAll();

            //カウントをストップ
            //SetStageCountStopSceneGame(true);

            //プレイヤーの硬直
            //GameManagerSetPlayerCtrlStop(true);

            //ムービーフラグ
            GameManagerSetMovieFlag(true, true);
            GameManagerSetMoviePlayerHiddenFlag(true);

            SetCameraModeAll(CAMERA_MODE_NORMAL);

            SoundBGMStopRequest();


            for (int i = 0; i < GAME_OBJECT_MAX; i++)
            {
                GAME_OBJECT* gObj = GetGameObjects() + i;
                if (IsActiveGameObject(gObj))
                {
                    if (HasTagGameObject(gObj,
                        GOE_Character_Enemy_Rucs::CHARACTER_NAME))
                    {
                        GOE_Character_Enemy_Rucs* goeTmp
                            = (GOE_Character_Enemy_Rucs*)(gObj->events.source);
                        if (goeTmp != nullptr)
                        {
                            goeTmp->StartExit();
                        }
                    }
                }
            }
        }

        if (m_StartFrame == FRAME_ONE_SECOND * 0.5f)
        {
            CameraMoveRequestAll(FRAME_ONE_SECOND * 1,
                Float2ToFloat3(Float3ToFloat2(m_Obj->pos), 0.0f),
                0.0f,
                MkF2(1.0f, 1.0f),
                0.8f,
                EasingInOutSine,
                CAMERA_MOVE_TYPE_SMOOTH
            );
            CameraTargetMoveRequestAll(FRAME_ONE_SECOND * 1,
                Float2ToFloat3(Float3ToFloat2(m_Obj->pos), -5000.0f),
                EasingInOutSine,
                CAMERA_MOVE_TYPE_SMOOTH
            );
            //CameraZoomRequestAll(
            //    0.04f,
            //    Float3ToFloat2(m_Obj->pos),
            //    //m_Obj->pos,
            //    EasingInOutSine,
            //    0.55f
            //);
        }

        if (m_StartFrame == FRAME_ONE_SECOND * 1.0)
        {
            //event_playerPosSet();

            StartWarp(
                MkF3(
                    m_Obj->pos.x,
                    m_Obj->pos.y,
                    0.0f
                ),
                0.0f,
                30
            );
        }

        static constexpr int ATTACK_BLADE_BEFORE_FRAME_STARTPOSE = 60;
        static constexpr int ATTACK_BLADE_ATTACKING_FRAME_STARTPOSE = 7;

        constexpr int startPosef = FRAME_ONE_SECOND * 2.5;
        if (m_StartFrame == startPosef - ATTACK_BLADE_BEFORE_FRAME_STARTPOSE)
        {
            //SEを再生
            //SoundEffectPlayRequest(GetSoundData("SOUND/SE/inperiusu/b_kamae"), 1.4f, 0);

            SoundEffectPlayRequest(GetSoundData("SOUND/SE/enemy/move/warpLow"), 1.5f, 0);

            StartAnim(AnimState::StartBefore, 
                ATTACK_BLADE_BEFORE_FRAME_STARTPOSE
                + ATTACK_BLADE_ATTACKING_FRAME_STARTPOSE,
                true);


            Float3 zoomPos = m_Obj->pos;
            zoomPos.x += 0.0f;
            zoomPos.y += -1200.0f;
            zoomPos.z += 5000.0f;
            Float3 zoomPosTa = m_Obj->pos;
            zoomPosTa.y += 10.0f;
            zoomPosTa.z += 18000.0f;
            CameraMoveRequestAll(60,
                zoomPos,
                0.0f,
                MkF2(1.0f, 1.0f),
                0.25f,
                EasingInOutSine,
                CAMERA_MOVE_TYPE_SMOOTH
            );
            CameraTargetMoveRequestAll(60,
                zoomPosTa,
                EasingInOutSine,
                CAMERA_MOVE_TYPE_SMOOTH
            );
        }

        if (m_StartFrame == startPosef)
        {
            //アウトライン有効化

            //BGMを再生する
            SoundBGMPlayRequest(bgmidtmp,
                true,
                2.0f,
                0.0
            );

            SoundEffectPlayRequest(GetSoundData("SOUND/SE/enemy/attack/Small_Explosion"),
                2.0f, 0, 0.8f);
            SoundEffectPlayRequest(
                GetSoundData("SOUND/SE/enemy/attack/super_beam_miminari"),
                0.45f, 3, 0.75f);

            
            CameraShakeRequestAll(CAMERA_SHAKE_SMALL_FRAME,
                CAMERA_SHAKE_EXLARGE_POWER);

            StartAnim(AnimState::Start, 
                ATTACK_HAMMER_ATTACK_FRAME,
                true);

            Float3 zoomPos = m_Obj->pos;
            //zoomPos.x += -550.0f;
            zoomPos.x += 230.0f;
            //zoomPos.x += 0.0f;
            zoomPos.y += 300.0f;
            zoomPos.z += -2500.0f;

            Float3 zoomPosTa = m_Obj->pos;
            zoomPosTa.x += 230.0f;
            //zoomPosTa.x += 230.0f * 2;
            zoomPosTa.y += -500.0f;
            zoomPosTa.z += -9500.0f;

            {
                CameraMoveRequestAll(ATTACK_BLADE_ATTACKING_FRAME_STARTPOSE - 1,
                    zoomPos,
                    -0.1f,
                    MkF2(1.0f, 1.0f),
                    //0.45f,
                    2.0f,
                    EasingInOutSine,
                    CAMERA_MOVE_TYPE_SMOOTH
                );
                CameraTargetMoveRequestAll(ATTACK_BLADE_ATTACKING_FRAME_STARTPOSE - 1,
                    zoomPosTa,
                    EasingInOutSine,
                    CAMERA_MOVE_TYPE_SMOOTH
                );
            }
        }

        constexpr int stopstartf = startPosef + ATTACK_BLADE_ATTACKING_FRAME_STARTPOSE;
        constexpr int stopframe = FRAME_ONE_SECOND * 2.25;
        if (m_StartFrame == stopstartf)
        {
            m_HeadState = HeadState::W;

            m_StartMotionBeforeShaderPS = GetPostEffectShaderPS();
            SetPostEffectShaderPS("SHADER/ps_world_iseki_boss");
            m_StartMotionBeforeShaderVS = GetPostEffectShaderVS();
            SetPostEffectShaderVS("SHADER/vs_general2");

            SetPostEffectGlobalOutlineWidth(20.0f);

            m_StartMotionShaderFlag = true;

            SetControllerMainLeftVibration(30, 0);
            SetControllerMainRightVibration(30, 0);

            GameObjectHitStop(m_Obj, stopframe + FRAME_ONE_SECOND * 0.5f);

            {
                CAMERA cInfo = GetCameraInfo(0);
                Float3 pos = cInfo.trans.pos;
                pos.x += -10.0f;

                CameraMoveRequestAll(stopframe,
                    pos,
                    cInfo.trans.rot + -0.01f,
                    MkF2(1.0f, 1.0f),
                    cInfo.trans.zoom + 0.15f,
                    nullptr, //EasingInOutSine,
                    CAMERA_MOVE_TYPE_SMOOTH
                );
            }
        }

        if (m_StartFrame == stopstartf + stopframe)
        {
            GameManagerSetMoviePlayerHiddenFlag(false);

            if (m_StartMotionBeforeShaderVS != -1)
            {
                SetPostEffectShaderVS(m_StartMotionBeforeShaderVS);
            }
            if (m_StartMotionBeforeShaderPS != -1)
            {
                SetPostEffectShaderPS(m_StartMotionBeforeShaderPS);
            }

            ResetPostEffectGlobalOutlineWidth();

            m_StartMotionShaderFlag = false;

            //SetCameraModeAll(CAMERA_MODE_PLAYER_CHASE);
            {
                Float3 zoomPos = m_Obj->pos;
                zoomPos.x += -450.0f;
                zoomPos.y += 300.0f + 4000.0f;
                zoomPos.z += 1000.0 + 28000.0f - 15000.0f;

                Float3 zoomPosTa = m_Obj->pos;
                zoomPosTa.z += -7500.0f;
                //zoomPosTa.x += -600.0f;
                //zoomPosTa.y += -500.0f;
                //zoomPosTa.z += -19000.0f + 12000.0f - 15000.0f;

                {
                    //CameraZoomResetRequestAll(
                    //    1.0f / 2 / 40,
                    //    nullptr,
                    //    0
                    //);
                    CameraMoveRequestAll(60,
                        zoomPos,
                        0.0f,
                        MkF2(1.0f, 1.0f),
                        0.3f,
                        EasingOutSine,
                        CAMERA_MOVE_TYPE_SMOOTH
                    );
                    CameraTargetMoveRequestAll(40,
                        zoomPosTa,
                        EasingOutSine,
                        CAMERA_MOVE_TYPE_SMOOTH
                    );
                    //{
                    //    CameraMoveRequestAll(40,
                    //        GetCameraPCPPos(0),
                    //        0.0f,
                    //        MkF2(1.0f, 1.0f),
                    //        GetCameraPCPZoom(0),
                    //        EasingOutSine,
                    //        CAMERA_MOVE_TYPE_SMOOTH
                    //    );
                    //    CameraTargetMoveRequestAll(40,
                    //        GetCameraPCPTarget(0),
                    //        EasingOutSine,
                    //        CAMERA_MOVE_TYPE_SMOOTH
                    //    );
                    //}
                }
            }
            //{

            //    CameraMoveRequestAll(40,
            //        Float2ToFloat3(Float3ToFloat2(m_Obj->pos), 0.0f),
            //        0.0f,
            //        MkF2(1.0f, 1.0f),
            //        0.4f,
            //        EasingInOutSine,
            //        CAMERA_MOVE_TYPE_SMOOTH
            //    );
            //    CameraTargetMoveRequestAll(40,
            //        Float2ToFloat3(Float3ToFloat2(m_Obj->pos), 0.0f),
            //        EasingInOutSine,
            //        CAMERA_MOVE_TYPE_SMOOTH
            //    );
            //}
        }

        if (m_StartFrame == stopstartf + stopframe + FRAME_ONE_SECOND * 0.5f)
        {
            set_hammer_visible(false);
        }

        //開始フレーム
        constexpr int startingF = stopstartf + stopframe + FRAME_ONE_SECOND / 2;

        if (m_StartFrame == startingF + FRAME_ONE_SECOND / 2/* + stopframe */)
        {

            m_HeadState = HeadState::Idle;

            //カウントストップを解除
            //SetStageCountStopSceneGame(false);

            //プレイヤーの硬直を解除
            //GameManagerSetPlayerCtrlStop(false);

            //ムービーフラグ
            GameManagerSetMovieFlag(false);

            SetCameraModeAll(CAMERA_MODE_BOSS_CHASE);
            //SetCameraModeAll(CAMERA_MODE_PLAYER_CHASE);
            m_StartedFlag = true;
        }

        //ムービースキップ処理 (多分後で変える)
        if (m_StartFrame < startingF
            && GameManagerIsMovieSkipTrigger()
            )
        {
            CameraFlashRequestAll(20, EasingOutSine, MkF4(-1.0f, -1.0f, -1.0f, 1.0f));

            if (!IsSoundBGMRequested(bgmidtmp))
            {
                SoundBGMStopRequest();


                //BGMを再生する
                SoundBGMPlayRequest(bgmidtmp,
                    true,
                    2.0f,
                    0.0
                );
            }

            CameraMoveCancelRequestAll();
            CameraTargetMoveCancelRequestAll();
            {
                Float3 zoomPos = m_Obj->pos;
                zoomPos.z += -33000.0f;

                CameraZoomResetRequestAll(
                    1.0f,
                    nullptr,
                    0
                );
                SetCameraTargetPosAll(MkF3(zoomPos.x, zoomPos.y - 10.0f, zoomPos.z + 10.0f));
                SetCameraPosAll(zoomPos);
                SetCameraZoomAll(0.6f);
            }
            SetCameraModeAll(CAMERA_MODE_PLAYER_CHASE_INSTANT);

            StopAnim();
            for (int i = 0; i < MODEL_NUM; i++)
            {
                m_ModelTransPos[i].x = 0.0f;
                m_ModelTransPos[i].y = 0.0f;
                m_ModelTransPos[i].z = 0.0f;

                m_ModelTransRot[i].x = 0.0f;
                m_ModelTransRot[i].y = 0.0f;
                m_ModelTransRot[i].z = 0.0f;

                m_ModelTransSca[i].x = 1.0f;
                m_ModelTransSca[i].y = 1.0f;
                m_ModelTransSca[i].z = 1.0f;

                m_ModelTransPosStart[i].x = 0.0f;
                m_ModelTransPosStart[i].y = 0.0f;
                m_ModelTransPosStart[i].z = 0.0f;
                m_ModelTransRotStart[i].x = 0.0f;
                m_ModelTransRotStart[i].y = 0.0f;
                m_ModelTransRotStart[i].z = 0.0f;
                m_ModelTransScaStart[i].x = 1.0f;
                m_ModelTransScaStart[i].y = 1.0f;
                m_ModelTransScaStart[i].z = 1.0f;
            }
            set_hammer_visible(false);
            m_Obj->hitStop = {};

            m_StartFrame = startingF;
        }

        //開始
        if (m_StartFrame == startingF)
        {
            become_start();


            SetCameraTargetPosAll(MkF3(0, 0, 0));
            //event_playerPosSet();

            if (m_IsHard)
            {
                GE_GayaStartRequest("iseki_kontoriusuhard_start_mayu");
            }
        }

        //if (m_StartFrame == FRAME_ONE_SECOND * 2.0)
        //{
        //    GameManagerSetMoviePlayerHiddenFlag(false);
        //    CameraZoomResetRequestAll(
        //        0.02f,
        //        EasingInOutSine,
        //        0
        //    );
        //}

        //if (m_StartFrame == FRAME_ONE_SECOND * 2.5)
        //{
        //    become_start();

        //    GE_GayaStartRequest("iseki_kontoriusuhard_start_mayu");
        //}
        //if (m_StartFrame == FRAME_ONE_SECOND * 2.5 + FRAME_ONE_SECOND / 2)
        //{
        //    m_HeadState = HeadState::Idle;

        //    //カウントストップを解除
        //    //SetStageCountStopSceneGame(false);

        //    //プレイヤーの硬直を解除
        //    //GameManagerSetPlayerCtrlStop(false);

        //    //ムービーフラグ
        //    GameManagerSetMovieFlag(false);

        //    SetCameraModeAll(CAMERA_MODE_BOSS_CHASE);
        //    //SetCameraModeAll(CAMERA_MODE_PLAYER_CHASE);
        //    m_StartedFlag = true;
        //}

    }

    m_StartFrame++;
}

void GOE_Character_Enemy_Boss_Kontoriusu::draw_start()
{
}

void GOE_Character_Enemy_Boss_Kontoriusu::StartTurn(float afterAxis, float speed)
{
    Float2 turnVecAfter = {};
    float rad = CalculateDegToRad(afterAxis);
    turnVecAfter.x = -sin(rad);
    turnVecAfter.y = cos(rad);

    StartTurn(turnVecAfter, speed);
}
void GOE_Character_Enemy_Boss_Kontoriusu::StartTurn(Float2 afterVec, float speed)
{
    m_TurnVecAfter = Float2ToFloat3(afterVec);
    m_TurnSpeed = speed;
}

void GOE_Character_Enemy_Boss_Kontoriusu::StartMove(Float3 afterPos, float afterAxis, double maxF,
    bool distanceOneF)
{

    {
        m_SimpleMoveFrame = 0;

        if (distanceOneF)
        {
            float dis = CalculateDistance(afterPos, m_Obj->pos);

            m_SimpleMoveFrameMax = (int)(maxF * (dis / ONE_BLOCK));
        }
        else
        {
            m_SimpleMoveFrameMax = maxF;
        }

        m_SimpleMovePosBefore = m_Obj->pos;
        m_SimpleMoveAxisBefore = m_CurrentAxisPrivate;

        m_SimpleMovePosAfter = afterPos;
        m_SimpleMoveAxisAfter = afterAxis;
    }
}

void GOE_Character_Enemy_Boss_Kontoriusu::startFastMove(
    Float3 afterPos, float afterAxis, int maxF,
    bool safeAreaIgnore,
    std::function<void()> middlefunc
)
{
    SoundEffectPlayRequest(
        GetSoundData("SOUND/SE/player/attack/Normal_Attack"),
        0.7f, 0.0f, 0.2f);

    SoundEffectPlayRequest(
        GetSoundData("SOUND/SE/enemy/move/warpLow"), 
        1.0f, 0, 1.5f);

    //SoundEffectPlayRequest(
    //    GetSoundData("SOUND/SE/enemy/move/warpHigh"),
    //    1.5f, 0);

    {
        m_FastMoveFrame = 0;
        m_FastMoveFrameMax = maxF;

        m_FastMoveMiddleFunc = middlefunc;

        m_FastMovePosBefore = m_Obj->pos;
        m_FastMoveAxisBefore = m_CurrentAxisPrivate;

        m_FastMovePosAfter = afterPos;
        m_FastMoveAxisAfter = afterAxis;

        if (!safeAreaIgnore)
        {
            Stage_Base* stg = GetCurrentStageSceneGame();
            if (stg != nullptr)
            {
                Int2 stgsize = stg->GetStageSize();

                Float2 size = {};
                size.x = BLOCK_WIDTH * stgsize.x * 0.5f;
                size.y = BLOCK_HIGHT * stgsize.y * 0.5f;

                if (m_FastMovePosAfter.x < size.x / -2)
                {
                    m_FastMovePosAfter.x = size.x / -2;
                }
                if (m_FastMovePosAfter.x > size.x / 2)
                {
                    m_FastMovePosAfter.x = size.x / 2;
                }

                if (m_FastMovePosAfter.y < size.y / -2)
                {
                    m_FastMovePosAfter.y = size.y / -2;
                }
                if (m_FastMovePosAfter.y > size.y / 2)
                {
                    m_FastMovePosAfter.y = size.y / 2;
                }
            }
        }

    }
}

void GOE_Character_Enemy_Boss_Kontoriusu::startWarp(
    Float3 afterPos, float afterAxis, int maxF,
    bool safeAreaIgnore,
    std::function<void()> middlefunc
)
{
    SoundEffectPlayRequest(GetSoundData("SOUND/SE/enemy/move/warpHigh"), 1.5f, 0);

    {
        m_WarpFrame = 0;
        m_WarpFrameMax = maxF;

        m_WarpMiddleFunc = middlefunc;

        m_WarpPosBefore = m_Obj->pos;
        m_WarpAxisBefore = m_CurrentAxisPrivate;

        m_WarpPosAfter = afterPos;
        m_WarpAxisAfter = afterAxis;

        if (!safeAreaIgnore)
        {
            Stage_Base* stg = GetCurrentStageSceneGame();
            if (stg != nullptr)
            {
                Int2 stgsize = stg->GetStageSize();

                Float2 size = {};
                size.x = BLOCK_WIDTH * stgsize.x * 0.5f;
                size.y = BLOCK_HIGHT * stgsize.y * 0.5f;

                if (m_WarpPosAfter.x < size.x / -2)
                {
                    m_WarpPosAfter.x = size.x / -2;
                }
                if (m_WarpPosAfter.x > size.x / 2)
                {
                    m_WarpPosAfter.x = size.x / 2;
                }

                if (m_WarpPosAfter.y < size.y / -2)
                {
                    m_WarpPosAfter.y = size.y / -2;
                }
                if (m_WarpPosAfter.y > size.y / 2)
                {
                    m_WarpPosAfter.y = size.y / 2;
                }
            }
        }

        if (!m_WarpPosCfxBefore)
        {
            Float3 postmp = m_WarpPosBefore;
            postmp.z += ONE_BLOCK * -4.0f * m_Obj->scale.z;

            GAME_OBJECT* go = GO_CreateEffect(
                GO_EFFECT_TYPE_WARP_IN,
                postmp);

            if (go != nullptr)
            {
                go->graphic.color = MkF4(
                    1.0f,
                    1.0f,
                    1.0f,
                    1.0f
                );
                go->scale = MkF3(
                    16.0f * m_Obj->scale.x,
                    16.0f * m_Obj->scale.y,
                    16.0f * m_Obj->scale.z
                );
                GameObjectSetLifeTime(go, 26);
            }
        }

        if (!m_IsPosCfx)
        {
            Float3 postmp = m_WarpPosAfter;
            postmp.z += ONE_BLOCK * -4.0f * m_Obj->scale.z;

            GAME_OBJECT* go = GO_CreateEffect(
                GO_EFFECT_TYPE_WARP_OUT,
                postmp);

            if (go != nullptr)
            {
                go->graphic.color = MkF4(
                    1.0f,
                    1.0f,
                    1.0f,
                    1.0f
                );
                go->scale = MkF3(
                    16.0f * m_Obj->scale.x * 1.25f,
                    16.0f * m_Obj->scale.y * 1.25f,
                    16.0f * m_Obj->scale.z * 1.25f
                );
                GameObjectSetLifeTime(go, 30);
            }
        }
    }
}
void GOE_Character_Enemy_Boss_Kontoriusu::StartWarp(Float3 afterPos, float afterAxis, int maxF,
    bool safeAreaIgnore,
    std::function<void()> middlefunc
)
{
    m_WarpPosCfxBefore = false;
    m_IsPosCfx = false;

    if (m_IsHard)
    {
        startWarp(afterPos, afterAxis, maxF,
            safeAreaIgnore,
            middlefunc
        );
    }
    else
    {
        startFastMove(afterPos, afterAxis, maxF,
            safeAreaIgnore,
            middlefunc
        );
    }
}

void GOE_Character_Enemy_Boss_Kontoriusu::StartWarpCfx(
    bool posCfxEnable,
    Float3 afterPos, float afterAxis, int maxF,
    std::function<void()> middlefunc
)
{
    if (!m_IsHard)
    {
        return;
    }

    m_WarpPosCfxBefore = m_IsPosCfx;
    m_IsPosCfx = posCfxEnable;

    Float3 enabepos = { 
        0.0f,
        ONE_BLOCK * -100.0f,
        ONE_BLOCK * -10000.0f
    };
    startWarp(
        posCfxEnable ? enabepos: afterPos,
        afterAxis, 
        maxF,
        true,
        middlefunc
    );
    m_WarpPosBefore = m_PosCfx;
    //m_WarpPosAfter = enabepos;
    m_PosCfx = afterPos;
}
