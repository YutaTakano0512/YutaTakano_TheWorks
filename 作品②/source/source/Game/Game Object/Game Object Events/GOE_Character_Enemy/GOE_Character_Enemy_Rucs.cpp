// ===================================================
// GOE_Character_Enemy_Rucs.cpp キャラクターオブジェクト
// 
// hara sougo  2025/04/06
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


//#include "GOE_Character_Enemy_Miresu.h"
//#include "GOE_Character_Enemy_Regatosu.h"
#include "GOE_Character_EnemyProjectile.h"
#include "GOE_Character_Enemy_Rucs.h"

// ===================================================
// マクロ定義
// ===================================================

// ===================================================
// プロトタイプ宣言
// ===================================================

// ===================================================
// グローバル変数
// ===================================================



// ===================================================
// Init
// オブジェクトが生成されたとき
// ===================================================
void GOE_Character_Enemy_Rucs::Init()
{
	SuperGOE::Init();

    m_Tex_Model = LoadTexData("TEX/enemy/rucs1");
    m_shaderType = DPD_SHADER_TYPE_METAL_ENEMY_DARK;
    //m_shaderType = DPD_SHADER_TYPE_METAL_ENEMY;
    //m_shaderType = DPD_SHADER_TYPE_METAL_ENEMY_2;

    m_ModelIndex[0] = GetModelDataIndex("MODEL/enemy_rucs/body");
    m_ModelIndex[1] = GetModelDataIndex("MODEL/enemy_rucs/zenmai");
    m_ModelIndex[2] = GetModelDataIndex("MODEL/enemy_rucs/gear");

    m_Tex_Model_Blade = LoadTexData("TEX/effect/tex_blade");

    m_Tex_Shadow = LoadTexData("TEX/effect/shadow");

    m_Tex_Dis = LoadTexData("TEX/effect/NoiseTile");
    m_Tex_Warp = LoadTexData("TEX/effect/NoiseTile2");


    //エフェクトとか

    //スプライト非表示
    m_Obj->graphic.hidden = true;


    //m_Obj->graphic.shaderOpt.outline = DPD_SHADER_OPT_OUTLINE_NONE;

    //m_Obj->collision.atk.enable = false;
    //m_Obj->collision.rec.enable = false;
    //m_Obj->status.statusType = GAME_OBJECT_STATUS_TYPE_VOID;

    //判定有効化
    m_Obj->collision.atk.enable = true;
    m_Obj->collision.rec.enable = true;
    m_Obj->status.statusType = GAME_OBJECT_STATUS_TYPE_ENEMY_MIDIUM;

    m_Obj->graphic.shaderOpt.outline = DPD_SHADER_OPT_OUTLINE_NORMAL;
    m_Obj->graphic.shaderOpt.outlineLayer = 0;


    m_Obj->size = MkF2(ONE_BLOCK * 4, ONE_BLOCK * 4);

    for (int i = 0; i < MODEL_NUM; i++)
    {
        m_ModelTransSca[i].x = 1.0f;
        m_ModelTransSca[i].y = 1.0f;
        m_ModelTransSca[i].z = 1.0f;
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

    Character::SetStatusData(m_Obj, Character::GetStatusDataFindByName(HARD_STATUS_NAME));

    m_BeforeVecBuf = { 0.0f, 1.0f, 0.0f };
    m_BeforeVec = m_BeforeVecBuf;

    //StartBattle();
}

// ===================================================
// FirstFrame
// 最初のフレームのみ実行
// ===================================================
void GOE_Character_Enemy_Rucs::FirstFrame()
{
	SuperGOE::FirstFrame();


    //if (m_StartMotionBeforeShader != -1)
    //{
    //    SetPostEffectShader(m_StartMotionBeforeShader);
    //}

    //m_Obj->pos.z = 0.0f;


    m_Obj->pos.z = -10000.0f;
    StartWarp(Float2ToFloat3(Float3ToFloat2(m_Obj->pos), 0.0f), 0.0f, 30, true);
}

// ===================================================
// Uninit
// オブジェクトが削除されるとき
// ===================================================
void GOE_Character_Enemy_Rucs::Uninit()
{
    UnloadTexData(m_Tex_Model);
    UnloadTexData(m_Tex_Model_Blade);

	SuperGOE::Uninit();
}

// ===================================================
// Update
// 毎フレーム実行
// ===================================================
void GOE_Character_Enemy_Rucs::Update()
{
	SuperGOE::Update();

    //if (GameManagerIsMovieFlag())
    //{ //ムービー中は止める
    //    return;
    //}
    if (IsApp())
    { //登場演出中は止める
        return;
    }


    if (!m_StartedFlag)
    {
        ////開始フラグがたったら初回イベントを動かす
        //if (m_StartFlag)
        //{
        //    update_start();
        //}
        update_start();
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
void GOE_Character_Enemy_Rucs::UpdateActive()
{
    if (!IsApp() && m_WarpFrame >= 0)
    {
        return;
    }
    if (m_ExitFlag)
    {
        DeleteGameObject(m_Obj);
        return;
    }

	SuperGOE::UpdateActive();

    if (GameManagerIsMovieFlag())
    { //ムービー中は止める
        return;
    }
    if (IsApp())
    { //登場演出中は止める
        return;
    }


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
            }
            if (m_Dead_Frame == m_Dead_FrameMax)
            {
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
                                    + (m_ModelTransPos[0].y - 200.0f) * m_Obj->scale.y)

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

                if (m_AttackAct_Frame >= 0)
                {
                    update_attack();
                }
                //行動
                //switch (m_CurrentState)
                //{
                //case ActionState::Blade:
                //    update_blade();
                //    break;
                //case ActionState::Beam:
                //    update_beam();
                //    break;
                //case ActionState::Misile:
                //    update_misile();
                //    break;
                //case ActionState::MachineGun:
                //    update_machinegun();
                //    break;
                //case ActionState::EnergyBallBit:
                //    update_energyballbit();
                //    break;
                //case ActionState::Granade:
                //    update_granade();
                //    break;
                //case ActionState::EnemyCall:
                //    update_enemyCall();
                //    break;

                //case ActionState::SuperBeam:
                //    update_super_beam();
                //    break;
                //default:break;
                //}
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
            if ((headDeg >= 0 && headDeg < (90 - sikai))
                || (headDeg > (270 + sikai) && headDeg <= 360))
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

    { //あにむ

        if (m_StartedFlag)
        {
            {
                {
                    //for (int i = 0; i < MODEL_NUM; i++)
                    //{
                    //    m_ModelTransPos[i] = {};
                    //    m_ModelTransRot[i] = {};
                    //    m_ModelTransPos[i].y = sinf((float)m_Obj->frameActive / 30) * 10.0f;
                    //}

                    m_ModelTransRot[1].y += -2.0f;
                    m_ModelTransRot[2].y += 2.0f;
                }

                if (m_SimpleMoveFrame >= 0)
                {
                }
            }
        }

        if (m_Anim_Frame >= 0)
        {
            if (m_Anim_State == AnimState::Shoot)
            { //びいむ
            }
            else if (m_Anim_State == AnimState::Dead)
            { //
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
                }

            }
            m_Anim_Mod = 0.0f;
            m_Anim_Frame = -1;
        }
    }
}


void GOE_Character_Enemy_Rucs::drawBody(int i, int flashnum, Float3 shakePos,
    DrawType type
)
{
    DRAW_POLYGON_DAT drawDatTmp = {};
    drawDatTmp.pos = MkF3(
        0.0f + m_ModelTransPos[i].x * MODEL_SCA,
        0.0f + m_ModelTransPos[i].y * MODEL_SCA,
        0.0f + m_ModelTransPos[i].z * MODEL_SCA
    );
    drawDatTmp.size = MkF3(
        m_ModelTransSca[i].x * 100.0f * MODEL_SCA,
        m_ModelTransSca[i].y * -100.0f * MODEL_SCA,
        m_ModelTransSca[i].z * 100.0f * MODEL_SCA
    );

    drawDatTmp.rot = 0.0f;
    //drawDatTmp.rot3D = MkF3(0.0f, 0.0f + (float)GetMainGameFrame() * 2.2f, 0.0f);
    drawDatTmp.rot3D = MkF3(
        m_ModelTransRot[i].x + 0.0f,
        m_ModelTransRot[i].y + 0.0f,
        m_ModelTransRot[i].z + 0.0f
    );


    drawDatTmp.texUV = { 0.0f, 0.0f };
    drawDatTmp.texWH = { 1.0f, 1.0f };

    drawDatTmp.startPos3D = m_Obj->pos;
    //drawDatTmp.startPos3D.z += -800.0f;
    drawDatTmp.startPos3D.z += -300.0f;

    drawDatTmp.startRot3D = MkF3(
        90.0f,
        0.0f,
        0.0f + m_CurrentAxisPrivate
    );
    drawDatTmp.startScale = MkF2(1.0f, 1.0f);
    drawDatTmp.startScale3D = MkF3(1.0f, 1.0f, 1.0f);

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
    else if (type == DrawType::WarpIn || type == DrawType::WarpOut)
    {
        float mod = ((float)(m_WarpFrame) / m_WarpFrameMax);

        if (type == DrawType::WarpOut)
        {
            //mod = EasingInBack(EasingOutSine(1.0f - mod));
            mod = (EasingInQuad(1.0f - mod));

            drawDatTmp.startPos3D = m_WarpPosBefore;
            drawDatTmp.startPos3D.z += -300.0f;

            drawDatTmp.startRot3D = MkF3(
                90.0f,
                0.0f,
                0.0f + m_WarpAxisBefore
            );
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

        drawDatTmp.pos.x += 2800.0f;
        drawDatTmp.pos.y += 800.0f;
        drawDatTmp.pos.z += 4000.0f;
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
            drawDatTmp.shaderOpt.outline = DPD_SHADER_OPT_OUTLINE_NONE;

            drawDatTmp.texNo = 0;
            drawDatTmp.shaderOpt.blendState = DPD_SHADER_OPT_BLEND_STATE_DEFAULT;
            drawDatTmp.color = MkF4(
                0.2f,
                0.0f,
                0.1f,
                1.0f);
            opt.priority = (DRAW_POLYGON_PRIORITY)(opt.priority - 1);
            opt.depthBuffIgnore = true;

            DrawPolygonInCameraEx(drawDatTmp, opt);


            drawDatTmp.texUV.x = (float)m_Obj->frameActive * 0.012f;
            drawDatTmp.texUV.y = (float)m_Obj->frameActive * 0.02f;
            drawDatTmp.texWH.x = 0.7f;
            drawDatTmp.texWH.y = 0.7f;
            drawDatTmp.texNo = m_Tex_Model_Blade.textureId;
            drawDatTmp.shaderOpt.blendState = DPD_SHADER_OPT_BLEND_STATE_ADDCOLOR;
            drawDatTmp.color = MkF4(
                1.00f,
                0.30f,
                1.30f,
                1.0f);
            opt.priority = (DRAW_POLYGON_PRIORITY)(opt.priority + 1);
            opt.depthBuffIgnore = false;
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
    }


    DrawPolygonInCameraEx(drawDatTmp, opt);
}

void GOE_Character_Enemy_Rucs::shootBeam(
    Float3 pos, Float3 vec, float scale, int type
)
{
    //SEを再生
    SoundEffectPlayRequest(
        GetSoundData("SOUND/SE/enemy/attack/Laser"),
        1.5f, 0);
    //SEを再生
    SoundEffectPlayRequest(
        GetSoundData("SOUND/SE/enemy/attack/Small_Explosion"),
        2.5f, 0);

    CameraShakeRequestAll(CAMERA_SHAKE_MIDIUM_FRAME, CAMERA_SHAKE_MIDIUM_POWER);

    Float3 nvec = NormalizeVector(vec);

    GOE_Character_EnemyProjectile* goepro =
        GOE_Character_EnemyProjectile::CreateProjectile(
            type == 1 ? GOE_Character_EnemyProjectile::Type::EnergyBallBitKakusan:
            GOE_Character_EnemyProjectile::Type::Beam,
            MkF3(
                pos.x + m_Obj->pos.x + nvec.x * ONE_BLOCK * 3.0f,
                pos.y + m_Obj->pos.y + nvec.y * ONE_BLOCK * 3.0f,
                pos.z + m_Obj->pos.z + nvec.z * ONE_BLOCK * 3.0f - ONE_BLOCK * 4.0f
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
            {
                Character::MOTION_COL_DAT motionColDetails =
                    Character::GetMotionColDataFindByName(
                        Character::MOTION_COL_TYPE_ATK,
                        BEAM_ATK_COL_NAME
                    );

                GAME_OBJECT_COLLISION_ATTACK_CHILD attackColTmp = motionColDetails.atkCol;
                attackColTmp.subjectFamily[GAME_OBJECT_FAMILY_PLAYER] = true;

                attackColTmp.size = MkF3(
                    ONE_BLOCK * 3,
                    ONE_BLOCK * 3,
                    ONE_BLOCK * 3
                );
                attackColTmp.pos.z = -attackColTmp.size.z / 2;
                attackColTmp.pos.y += attackColTmp.size.y / 2 * 1.0f;

                beamObj->collision.atk.enable = true;
                beamObj->collision.atk.collision[0] = attackColTmp;
                beamObj->collision.atk.collision[0].enable = true;
            }
            beamObj->blockCollisionIgnore = false;

            GameObjectSetLifeTime(beamObj, FRAME_BEAM_LIFETIME);
            GameObjectMoveRequest(beamObj, FRAME_BEAM_LIFETIME,
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
void GOE_Character_Enemy_Rucs::Draw()
{
	SuperGOE::Draw();

    if (m_Dead_Frame > 0)
    {
        //if(m_Dead_Frame < 8)
        //{
        //    DRAW_POLYGON_DAT dsdat = {};
        //    dsdat.pos.x = 0.0f;
        //    dsdat.pos.y = 0.0f;
        //    dsdat.pos.z = 0.0f;

        //    dsdat.size.x = SCREEN_WIDTH;
        //    dsdat.size.y = SCREEN_HEIGHT;
        //    dsdat.size.z = 1.0f;

        //    dsdat.rot = 0.0f;

        //    dsdat.color = {
        //        0.15f,
        //        0.15f,
        //        0.15f,
        //        1.0 };
        //    dsdat.texNo = 0;
        //    //dsdat.color = { 1.0f, 1.0f, 1.0f, 1.0 };
        //    //dsdat.texNo = m_Tex_PowerBg;

        //    dsdat.loadTexType = LOADTEXTURETYPE_MAIN;

        //    dsdat.texUV.x += 0.02 * (float)m_Obj->frame;
        //    //dsdat.texWH = { 1.0f, 1.0f };
        //    dsdat.texWH = { 1.0f * ((float)SCREEN_HEIGHT / SCREEN_WIDTH), 1.0f };

        //    dsdat.startRot = 0.0f;
        //    dsdat.startScale = MkF2(1.0f, 1.0f);
        //    dsdat.startScale3D = MkF3(1.0f, 1.0f, 1.0f);

        //    dsdat.modelNo = 0;

        //    //dsdat.shaderOpt.bloom = true;
        //    //dsdat.shaderOpt.blendState = DPD_SHADER_OPT_BLEND_STATE_ADDCOLOR;

        //    DRAW_POLYGON_IN_CAMERA_EX_OPTION opt = {};
        //    opt.reverse = false;
        //    opt.fixed = true;
        //    opt.enableShade = false;
        //    opt.billboard = false;
        //    opt.priority = (DRAW_POLYGON_PRIORITY)(DRAW_POLYGON_PRIORITY_NML_ALWAYS - 1);

        //    DrawPolygonInCameraEx(dsdat, opt);
        //}
        for (int i = 0; i < MODEL_NUM; i++)
        {
            drawBody(i, 0, {}, DrawType::Dead);
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


    if (!m_StartedFlag)
    {
        draw_start();
        return;
    }
    draw_attack();

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
void GOE_Character_Enemy_Rucs::Dead()
{
	SuperGOE::Dead();

    m_Obj->hitReaction = {};
    m_Obj->hitStop = {};
    m_Obj->moveRequ.enable = false;

    GameManagerResultAddScore(
        1000,
        RESULT_SCORE_EX_TYPE_ENEMY_DEFEAT
    );

    SoundEffectPlayRequest(
        GetSoundData("SOUND/SE/player/attack/activation"),
        2.0f, 0.0f, 0.75f
    );
    SoundEffectPlayRequest(SOUND_TYPE_SE_9, 5.0f, 0, 0.9f);
    //SoundEffectPlayRequest(SOUND_TYPE_SE_9, 3.0f, 60, 0.9f);
    //SoundEffectPlayRequest(SOUND_TYPE_SE_9, 1.0f, 120, 0.9f);

    m_BeforeVecBuf = NormalizeVector({ 0.0f, 1.0f, 0.0f });
    m_BeforeVec = m_BeforeVecBuf;

    m_CurrentState = ActionState::Idle;

    StartAnim(AnimState::Idle, m_Dead_FrameMax, true);

    m_DeadFlag = true;
}

void GOE_Character_Enemy_Rucs::CalcDamage(
    GAME_OBJECT_DAMAGE_RESULT* output,

    const GAME_OBJECT* obj, const GAME_OBJECT* objAttacker,
    int damage,
    GAME_OBJECT_COLLISION_ATTACK_CHILD colAtk,
    GAME_OBJECT_COLLISION_RECIEVE_CHILD colRec
)
{
    output->damage = CalcRecvDamageMultiPlay(output->damage);
}


// ===================================================
// DamageAttacker
// オブジェクトの攻撃判定がヒットしたとき
// ===================================================
void GOE_Character_Enemy_Rucs::DamageAttacker(GAME_OBJECT_DAMAGE_RESULT dr)
{
	SuperGOE::DamageAttacker(dr);

}

// ===================================================
// DamageReciever
// オブジェクトが
// 別のオブジェクトの攻撃判定にヒットしたとき
// ===================================================
void GOE_Character_Enemy_Rucs::DamageReciever(GAME_OBJECT_DAMAGE_RESULT dr)
{
	SuperGOE::DamageReciever(dr);


}

// ===================================================
// BlockHit
// オブジェクトがブロックに触れたとき
// ===================================================
void GOE_Character_Enemy_Rucs::BlockHit(GAME_OBJECT_POSMOD_RESULT pmr)
{
	SuperGOE::BlockHit(pmr);
}



//void GOE_Character_Enemy_Rucs::StartBattle()
//{
//    m_StartFlag = true;
//}

void GOE_Character_Enemy_Rucs::update_action_pattern()
{
    update_action_pattern_hard();
}

void GOE_Character_Enemy_Rucs::update_action_pattern_normal()
{
}

void GOE_Character_Enemy_Rucs::update_action_pattern_hard()
{
    int actF = m_ActCycle % 4;

    if (m_StartedFrameCount < FRAME_ONE_SECOND * 1)
    {
        return;
    }

    m_CurrentState = ActionState::Attack;
    m_AttackActNum = actF;
    m_AttackAct_Frame = 0;
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

void GOE_Character_Enemy_Rucs::update_attack()
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
    Float3 vec3d = NormalizeVector(MkF3(
        m_Obj->pos.x - targetObject->pos.x,
        m_Obj->pos.y - targetObject->pos.y,
        m_Obj->pos.z + ONE_BLOCK * 4.0f - targetObject->pos.z
    ));

    bool isEnd = true;

    if (m_AttackActNum == 0)
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
                50
            );
        }
        if (m_AttackAct_Frame == 1)
        {
            //StartWarp
            StartMove(
                MkF3(
                    targetObject->pos.x + vec2d.x * ONE_BLOCK * -20.0f,
                    targetObject->pos.y + vec2d.y * ONE_BLOCK * -20.0f,
                    targetObject->pos.z
                ),
                CalculateRadToDeg(atan2(-vec2d.y, -vec2d.x) - 90.0f),
                //25);
                80);
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

            StartAnim(AnimState::Shoot, 140);
        }
        if (m_AttackAct_Frame >= 2
            && m_AttackAct_Frame < 30
        )
        {
            if (m_AttackAct_Frame % 10 == 0)
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
        if (m_AttackAct_Frame == 30)
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

        if (m_AttackAct_Frame >= 70
            && m_AttackAct_Frame < 130
            && m_AttackAct_Frame % 12 == 0
        )
        {
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
                postmp.z += ONE_BLOCK * -4.0f;

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

                float angle = 90.0f;
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
                            (float)cos(radtmp) * 3.0f,
                            (float)sin(radtmp) * 3.0f,
                            vec3d.z
                        ),
                        1.6f
                    );
                }
            }
        }

        if (m_AttackAct_Frame > 150)
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
                    targetObject->pos.x + vec2d.x * ONE_BLOCK * 9.0f,
                    targetObject->pos.y + vec2d.y * ONE_BLOCK * 9.0f,
                    targetObject->pos.z
                ),
                CalculateRadToDeg(atan2(-vec2d.y, -vec2d.x)) - 90.0f,
                60
            );
        }

        if (m_AttackAct_Frame == 1)
        {
            //SEを再生
            SoundEffectPlayRequest(GetSoundData("SOUND/SE/inperiusu/Small_Explosion"),
                4.0f, 0, 1.1);

            StartAnim(AnimState::Shoot, 40);

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
                postmp.z += ONE_BLOCK * -4.0f;

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

                float angle = 80.0f;
                int bulletnum = 4;
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
                    //shootBeam(
                    //    {},
                    //    MkF3(
                    //        (float)cos(radtmp) * 0.4f,
                    //        (float)sin(radtmp) * 0.4f,
                    //        -0.01f
                    //    ),
                    //    1.4f,
                    //    1
                    //);
                    shootBeam(
                        {},
                        MkF3(
                            (float)cos(radtmp) * 2.0f,
                            (float)sin(radtmp) * 2.0f,
                            vec3d.z
                        ),
                        1.6f
                    );
                }
            }
        }

        if (m_AttackAct_Frame > 90)
        {
            isEnd = true;
        }
    }
    if (m_AttackActNum == 2)
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
                -ONE_BLOCK * 8.0f * vec.x + targetObject->pos.x,
                -ONE_BLOCK * 8.0f * vec.y + targetObject->pos.y
            );

            //StartWarp
            StartMove(Float2ToFloat3(afterPos),
                CalculateRadToDeg(atan2(
                    targetObject->pos.y - afterPos.y,
                    targetObject->pos.x - afterPos.x
                ) - 90.0f),
                //25);
                50);
        }

        if (m_AttackAct_Frame > 3)
        {
            isEnd = true;
        }
    }

    if (m_AttackActNum == 3)
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
                60
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
                    -ONE_BLOCK * 24.0f * vec.x + targetObject->pos.x,
                    -ONE_BLOCK * 24.0f * vec.y + targetObject->pos.y
                );

                //StartWarp
                StartMove(Float2ToFloat3(afterPos),
                    CalculateRadToDeg(atan2(
                        targetObject->pos.y - afterPos.y,
                        targetObject->pos.x - afterPos.x
                    ) - 90.0f),
                    //25);
                    70);
            }

            //SEを再生
            SoundEffectPlayRequest(GetSoundData("SOUND/SE/inperiusu/Small_Explosion"),
                4.0f, 0, 1.1);

            StartAnim(AnimState::Shoot, 40);

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
                postmp.z += ONE_BLOCK * -4.0f;

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

                float angle = 60.0f;
                int bulletnum = 3;
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
                    //shootBeam(
                    //    {},
                    //    MkF3(
                    //        (float)cos(radtmp) * 0.3f,
                    //        (float)sin(radtmp) * 0.3f,
                    //        -0.01f
                    //    ),
                    //    1.6f,
                    //    1
                    //);
                    shootBeam(
                        {},
                        MkF3(
                            (float)cos(radtmp) * 1.3f,
                            (float)sin(radtmp) * 1.3f,
                            vec3d.z
                        ),
                        1.6f
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
        m_ActionCooldown = 40;
        m_AttackAct_Frame = -1;
        m_CurrentState = ActionState::Idle;
    }
}
void GOE_Character_Enemy_Rucs::draw_attack()
{
}


void GOE_Character_Enemy_Rucs::update_start()
{
    if (m_StartFrame == 0)
    {
        ////判定有効化
        //m_Obj->collision.atk.enable = true;
        //m_Obj->collision.rec.enable = true;
        //m_Obj->status.statusType = GAME_OBJECT_STATUS_TYPE_ENEMY_BOSS;
        //m_Obj->graphic.shaderOpt.outline = DPD_SHADER_OPT_OUTLINE_PURPLE;

        //m_Obj->pos.z = 0.0f;

        m_StartedFlag = true;
    }

    m_StartFrame++;
}

void GOE_Character_Enemy_Rucs::draw_start()
{
}

void GOE_Character_Enemy_Rucs::StartExit()
{
    m_ExitFlag = true;
    StartWarp({0.0f, -100.0f * ONE_BLOCK, -10000.0f * ONE_BLOCK }, 0.0f, 30, true);
}

void GOE_Character_Enemy_Rucs::StartTurn(float afterAxis, float speed)
{
    Float2 turnVecAfter = {};
    float rad = CalculateDegToRad(afterAxis);
    turnVecAfter.x = -sin(rad);
    turnVecAfter.y = cos(rad);

    StartTurn(turnVecAfter, speed);
}
void GOE_Character_Enemy_Rucs::StartTurn(Float2 afterVec, float speed)
{
    m_TurnVecAfter = Float2ToFloat3(afterVec);
    m_TurnSpeed = speed;
}

void GOE_Character_Enemy_Rucs::StartMove(Float3 afterPos, float afterAxis, int maxF)
{
    {
        m_SimpleMoveFrame = 0;
        m_SimpleMoveFrameMax = maxF;

        m_SimpleMovePosBefore = m_Obj->pos;
        m_SimpleMoveAxisBefore = m_CurrentAxisPrivate;

        m_SimpleMovePosAfter = afterPos;
        m_SimpleMoveAxisAfter = afterAxis;
    }
}
void GOE_Character_Enemy_Rucs::StartWarp(Float3 afterPos, float afterAxis, int maxF,
    bool safeAreaIgnore)
{
    //SoundEffectPlayRequest(GetSoundData("SOUND/SE/enemy/move/warpHigh"), 2.0f, 0);

    {
        m_WarpFrame = 0;
        m_WarpFrameMax = maxF;

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
    }
}