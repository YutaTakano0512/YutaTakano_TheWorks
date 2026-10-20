// ===================================================
// GOE_Character_Enemy_Miresu.cpp キャラクターオブジェクト
// 
// hara sougo  2024/12/10
// ===================================================
#define NO_CONTROLLER

#include "../../../../Game/Configuration/Param.h"
#include "../../../../game_main.h"
#include "../../../Sprite/sprite.h"
#include "../../../Polygon/modelData.h"
#include "../../../Texture/texture.h"
#include "../../../../Game/Entities/Block/block.h"
#include "../../../Systems/collision.h"
#include "../../../Systems/easing.h"

#include "../../../Managers/gameManager.h"
#include "../../../Managers/scene.h"

#include "../../gameObject.h"
#include "../../gameObjectPrefab.h"

#include "../../../Systems/camera.h"
#include "../../../Systems/ui.h"
#include "../../../Systems/ui_Game.h"

#include "../../../Entities/character.h"
#include "../../../Entities/player.h"

#include "../../../Managers/soundManager.h"
#include "../../../Game Events/GE_Gaya.h"

#include "../GOE_Effect.h"
#include "../GOE_Character.h"


#include "GOE_Character_EnemyProjectile.h"
#include "GOE_Character_Enemy_Miresu.h"

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
void GOE_Character_Enemy_Miresu::Init()
{
	SuperGOE::Init();

    m_Tex_Main_Idle = LoadTexData("TEX/enemy/miresu_sprite");

    GraphicSet(m_Tex_Main_Idle, 3, 3, 8);
	//Character::PlayMotionCharacterGameObject(
	//	goDat,
	//	Character::GetCharacterMotionFindByName(Character::GetCharacterMainData(goDat->dataIndex), "IDLE"),
	//	true,
	//	nullptr,
	//	60
	//);
    
    //m_TestModel = GetModelDataIndex("MODEL/tree");

    m_Tex_BeamCharge = LoadTexData("TEX/effect/beam/tex_atlas_01").textureId;
    m_Tex_Beam = LoadTexData("TEX/effect/beam/sozai_02").textureId;

    m_Model_Beam = GetModelDataIndex("MODEL/beam_01");


    m_Tex_Defeat = LoadTexData("TEX/enemy/mires_dis_01");

    {
        GAME_OBJECT_COLLISION_RECIEVE_CHILD* col = m_Obj->collision.rec.collision + 0;
        col->size = MkF3(ONE_BLOCK * 2.0f, ONE_BLOCK * 2.0f, ONE_BLOCK * 2.0f);
    }

    m_DefeatScore = 10;

    { //初期の向きをランダムにする
        m_BeforeVecBuf = NormalizeVector(MkF3(
            ((float)(rand() % 1000) / 1000),
            ((float)(rand() % 1000) / 1000),
            ((float)(rand() % 1000) / 1000)
        ));
        m_BeforeVec = m_BeforeVecBuf;
    }
}

// ===================================================
// FirstFrame
// 最初のフレームのみ実行
// ===================================================
void GOE_Character_Enemy_Miresu::FirstFrame()
{
	SuperGOE::FirstFrame();


	//if (HasTagGameObject(obj, "ENEMY_MIRESU_ATTACKER")
	//	|| HasTagGameObject(obj, "ENEMY_MIRESU_SHOOTER"))
	//{
	//	//Character::PlayMotionCharacterGameObject(
	//	//	goDat,
	//	//	Character::GetCharacterMotionFindByName(Character::GetCharacterMainData(goDat->dataIndex), "IDLE"),
	//	//	true,
	//	//	nullptr,
	//	//	60
	//	//);
	//}

	//アニメーションを再生する
	Character::PlayMotionCharacterGameObject(
		m_CharaData,
		Character::GetCharacterMotionFindByName(Character::GetCharacterMainData(m_CharaData->dataIndex), "IDLE"),
		true,
		(int*)(&m_CurrentAxisPrivate) //スプライトの番号を向きに依存させる
    ); 

    m_DefaultSpritePos = m_Obj->graphic.graph.spriteAnim[0].pos;
}

// ===================================================
// Uninit
// オブジェクトが削除されるとき
// ===================================================
void GOE_Character_Enemy_Miresu::Uninit()
{
    UnloadTexData(m_Tex_BeamCharge);
    UnloadTexData(m_Tex_Beam);

    UnloadTexData(m_Tex_Main_Idle);

	SuperGOE::Uninit();
}

// ===================================================
// Update
// 毎フレーム実行
// ===================================================
void GOE_Character_Enemy_Miresu::Update()
{
	SuperGOE::Update();

}

// ===================================================
// UpdateActive
// 毎フレーム実行
// ただし、ヒットストップ中などの
// 非アクティブ時は実行されない
// ===================================================
void GOE_Character_Enemy_Miresu::UpdateActive()
{
	SuperGOE::UpdateActive();

    m_CurrentAxisPrivate = (m_CurrentAxisEx - 1 + 8) % 8;


    //フラグがたって居たら削除する
    if (m_DeadFlag)
    {
        if (m_IsRare)
        {
            GE_GayaStartRequest("game_rare_enemy_defeat_mayu", 80);
        }

        Float3 addPos = {};
        addPos.z = -320.0f;

        {
            GAME_OBJECT* go = GO_CreateAfterEffect(m_Obj, 90, MkF4(1.0f, 1.0f, 1.0f, 1.0f));
            if (go != nullptr)
            {
                go->graphic.stop = false;
                go->graphic.graph.spriteAnim[0].texDat = m_Tex_Defeat;
                go->graphic.graph.spriteAnim[0].anim = true;
                go->graphic.graph.spriteAnim[0].no = 0;
                go->graphic.graph.spriteAnim[0].frame = 0;
                go->graphic.graph.spriteAnim[0].xcnt = 6;
                go->graphic.graph.spriteAnim[0].hcnt = 6;
                go->graphic.graph.spriteAnim[0].delay = go->lifetimeFrameMax / 30;
                go->graphic.graph.spriteAnim[0].cntMax = 6 * 6;
                go->graphic.graph.spriteAnim[0].size.x = 720.0f * 0.7f;
                go->graphic.graph.spriteAnim[0].size.y = 720.0f * 0.7f;
            }
        }

        EnemyVanish(addPos);
        return;
    }
    if (GameManagerIsMovieFlag())
    { //ムービー中は止める
        return;
    }
    if (IsApp())
    { //登場演出中は止める
        return;
    }

    if (m_IsRare
        && m_Obj->frame > 0 
        && !m_TypeRareStartEventFlag
    )
    {
        m_TypeRareStartEventFlag = true;

        //GameStopRequest(180);

        //CameraZoomRequestAll(
        //    0.04f,
        //    //Float3ToFloat2(m_Obj->pos),
        //    m_Obj->pos,
        //    EasingInOutSine,
        //    0.4f
        //);
        //CameraZoomResetRequestAll(
        //    0.03f,
        //    EasingInOutSine,
        //    130
        //);

        std::string gayan = "";
        switch (m_Type)
        {
        case GOE_Character_Enemy_Miresu::Type::Hard:
            gayan = ("iseki_rand_mirehard_mayu");
            break;
        case GOE_Character_Enemy_Miresu::Type::Speed:
            gayan = ("iseki_rand_mirespeed_mayu");
            break;
        case GOE_Character_Enemy_Miresu::Type::Strong:
            gayan = ("iseki_rand_mirestrong_mayu");
            break;
        case GOE_Character_Enemy_Miresu::Type::Rapid:
            gayan = ("iseki_rand_mirerapid_mayu");
            break;
        default:
            break;
        }
        if (gayan.size() > 0)
        {
            GAYA_REQUEST_OPT gayaopt = {};
            gayaopt.graph = m_Obj->graphic.graph;
            m_RareGayaId = GE_GayaStartRequest(gayan, 10, gayaopt);
        }
    }

	//行動パターン
	
    do {
        m_Obj->collision.atk.collision[0].power = m_CollisionDefault.atk.collision[0].power;
        m_Obj->collision.rec.collision[0].guardMod = m_CollisionDefault.rec.collision[0].guardMod;

        if (GetIsBuffBreak())
        {
            m_BeamChargeFrame = 0;

            break;
        }

        float speedMod = 1.0f;

        if (GetIsBuffSuper())
        {
            speedMod *= BUFF_SUPER_MOD_SPEED;

            //バフ
            m_Obj->collision.atk.collision[0].power = m_CollisionDefault.atk.collision[0].power * BUFF_SUPER_MOD_ATTACK;
            m_Obj->collision.rec.collision[0].guardMod = m_CollisionDefault.rec.collision[0].guardMod * BUFF_SUPER_MOD_GUARD;
        }
        else if (GetIsBuff())
        {
            if (!IsActiveGameObject(m_BuffOwner))
            { //指令元がいなくなったら崩壊
                SetBuffBreak();
            }

            speedMod *= BUFF_MOD_SPEED;

            //バフ
            m_Obj->collision.atk.collision[0].power = m_CollisionDefault.atk.collision[0].power * BUFF_MOD_ATTACK;
            m_Obj->collision.rec.collision[0].guardMod = m_CollisionDefault.rec.collision[0].guardMod * BUFF_MOD_GUARD;
        }

        if (m_Type == Type::Speed)
        {
            speedMod *= 3.0f;
        }

        m_SpeedMod = speedMod;

        if (m_Type == Type::Strong)
        {
            update_action_hybrid();
        }
        else
        {
            //
            // 狙撃型
            //
            if (HasTagGameObject(m_Obj, CHARACTER_NAME_SHOOTER))
            {
                update_action_shooter();
            }

            //
            // 戦闘型
            //
            if (HasTagGameObject(m_Obj, CHARACTER_NAME_ATTACKER))
            {
                update_action_attacker();
            }
        }

    } while (false);

    if (m_BuffFrame > 0)
    {
        m_BuffFrame--;
    }

    {
        float mod = std::max(std::min((float)m_TargetFrame / 20, 1.0f), 0.0f);
        //m_Obj->graphic.pos3d.z = (sin(2.0f * SYS_GENERAL_PI
        //    * std::min((float)m_TargetFrame / 30, 1.0f)) / 2.0f + 0.5f) * -20.0f;
        m_Obj->graphic.graph.spriteAnim[0].pos.y = 
            m_DefaultSpritePos.y 
            + (1.0f - (cos(2.0f * SYS_GENERAL_PI * EasingInOutSine(mod) ) / 2.0f + 0.5f))
            * -300.0f;

        m_Obj->graphic.graph.spriteAnim[0].pos.y +=
            //sin((float)m_Obj->moveRequ.frame * 1.2f) * 20.0f;
            sin((float)m_Obj->frameActive * 1.2f) * 8.0f;
    }

    //バフエフェクト
    if (GetIsBuffBreak())
    //if (GetIsBuffBreak() || GetIsBuffSuper() || GetIsBuff())
    {
        int indextmp = GetGameObjectIndex(m_Obj);
        if((m_Obj->frameActive + indextmp * 13) % 8 == 0)
        {
            Float3 postmp = m_Obj->pos;
            postmp.x += sin((float)m_Obj->frame * 13.0f + (0.7f * indextmp)) * ONE_BLOCK * 1.2f * m_Obj->scale.x;
            postmp.y += cos((float)m_Obj->frame *  3.0f + (1.3f * indextmp)) * ONE_BLOCK * 1.2f * m_Obj->scale.y + 0.0f;
            postmp.z += cos((float)m_Obj->frame * 13.0f + (1.3f * indextmp)) * ONE_BLOCK * 1.2f * m_Obj->scale.y + -250.0f;
            //postmp.z += -150.0f * m_Obj->scale.y;
            GAME_OBJECT* go = GO_CreateEffect(GO_EFFECT_TYPE_SPARK, postmp);
            if (go != nullptr)
            {
                go->graphic.color = MkF4(
                    2.0f,
                    2.0f,
                    0.1f,
                    1.0f
                );
                go->rot = -50.0f * m_Obj->frame * indextmp;
                go->scale = MkF3(
                    2.4f,
                    2.4f,
                    2.4f
                );
                GameObjectSetLifeTime(go, 5);
            }
        }
    }

    //{ //ビーム
    //    if (m_Beam_Objects.size() > 0)
    //    {
    //        for (const auto& beamObjIde : m_Beam_Objects)
    //        {
    //            GAME_OBJECT* beamObj = GetGameObjectFindByIdentifier(beamObjIde.second);
    //            if (beamObj != nullptr)
    //            {
    //                if (beamObj->frameActive % 4 == 0)
    //                {
    //                    for (int i = 0; i < 2; i++)
    //                    {
    //                        Float3 posTmp = beamObj->pos;
    //                        posTmp.x += sin((float)m_Obj->frame * 13.0f + (0.7f * i)) * ONE_BLOCK * 0.6f * m_Obj->scale.x;
    //                        posTmp.y += cos((float)m_Obj->frame * +3.0f + (1.3f * i)) * ONE_BLOCK * 0.6f * m_Obj->scale.y;
    //                        posTmp.z += cos((float)m_Obj->frame * 13.0f + (1.3f * i)) * ONE_BLOCK * 0.6f * m_Obj->scale.y;

    //                        posTmp.x += m_BeamShootVec.x * 100.0f;
    //                        posTmp.y += m_BeamShootVec.y * 100.0f;
    //                        posTmp.z += -80.0f;

    //                        GAME_OBJECT* go = GO_CreateEffect(GO_EFFECT_TYPE_SPARK, posTmp);
    //                        if (go != nullptr)
    //                        {
    //                            go->graphic.color = MkF4(
    //                                1.0f,
    //                                1.0f,
    //                                2.0f,
    //                                1.0f
    //                            );
    //                            go->rot = CalculateRadToDeg(atan2(m_BeamShootVec.y, m_BeamShootVec.x)) - 45.0f
    //                                + 55.0f * beamObj->frame * (i + 1);
    //                            go->scale = MkF3(
    //                                2.2f,
    //                                2.2f,
    //                                2.2f
    //                            );
    //                            GameObjectSetLifeTime(go, 12);
    //                        }
    //                    }
    //                }
    //            }
    //            else
    //            {
    //                //m_Beam_Objects.erase(beamObjIde.first);
    //            }
    //        }
    //    }
    //}

}

// ===================================================
// Draw
// オブジェクトの描画イベント
// ===================================================
void GOE_Character_Enemy_Miresu::Draw()
{
	SuperGOE::Draw();

    //if (GetIsBuff() || GetIsBuffSuper())
    //{
    //    DrawCharaEff(CharaEffType::Buff, MkF2(ONE_BLOCK * 8.0f, ONE_BLOCK * 8.0f));
    //}


    if (GetIsBuffSuper())
    {
        float gmod = (sin((float)m_Obj->frameActive / 4) + 1.0f) / 2 * -0.95f;
        float bmod = (cos((float)m_Obj->frameActive / 4) + 1.0f) / 2 * -0.95f;
        m_Obj->graphic.color = MkF4(
            1.0f,
            1.0f + gmod,
            1.0f + bmod,
            1.0f
        );
    }
    else if (GetIsBuffBreak())
    {
        //DrawCharaEff(CharaEffType::DeBuff, MkF2(ONE_BLOCK * 5.0f, ONE_BLOCK * 5.0f));

        float rgbmod = (cos((float)m_Obj->frameActive / 4) + 1.0f) / 2 * -0.5f - 0.4f;
        m_Obj->graphic.color = MkF4(
            1.0f + rgbmod,
            1.0f + rgbmod,
            1.0f + rgbmod,
            1.0f
        );
    }
    else if (GetIsBuff())
    {
        float gbmod = (sin((float)m_Obj->frameActive / 6) + 1.0f) / 2 * -0.95f;
        m_Obj->graphic.color = MkF4(
            1.0f - gbmod,
            1.0f - gbmod,
            1.0f + gbmod,
            1.0f
        );
    }
    else
    {
        m_Obj->graphic.color = MkF4(
            1.0f,
            1.0f,
            1.0f,
            1.0f
        );
    }

    //
    // ビーム
    //
    {
        DRAW_POLYGON_DAT drawDatBaseTmp = {};
        drawDatBaseTmp.size = MkF3(12.0f, 12.0f, 1800.0f);

        //drawDatBaseTmp.pos.y += -120.0f - drawDatBaseTmp.size.y / 2;
        //drawDatBaseTmp.pos.z += 30.0f + drawDatBaseTmp.size.z / 2 - (m_Obj->size.x * m_Obj->scale.x + m_Obj->size.y * m_Obj->scale.y) / 4;
        //drawDatBaseTmp.pos.z += 30.0f + drawDatBaseTmp.size.z / 2 - (m_Obj->size.x * m_Obj->scale.x + m_Obj->size.y * m_Obj->scale.y) / 4;

        drawDatBaseTmp.pos.z = drawDatBaseTmp.size.z / 2 * 0.75f;

        drawDatBaseTmp.rot = 0.0f;

        drawDatBaseTmp.rot3D = MkF3(0.0f, 0.0f, 0.0f);

        //drawDatTmp.color = MkF4(1.0f, 0.8f, 1.0f, 0.9f);
        //drawDatBaseTmp.color = MkF4(1.0f * 2, 0.9f * 2, 1.0f * 2, 0.9f);
        //drawDatTmp.color = MkF4(3.0f, 1.0f, 7.0f, 0.9f);
        drawDatBaseTmp.color = MkF4(0.6f, 0.0f, 0.0f, 0.9f);

        //drawDatTmp.texNo = LoadTexture("rom/texture/ui/in_game/charactor_icon.png");
        drawDatBaseTmp.texNo = 0;
        drawDatBaseTmp.loadTexType = LOADTEXTURETYPE_MAIN;

        drawDatBaseTmp.texUV = { 0.0f, 0.0f };
        drawDatBaseTmp.texWH = { 1.0f, 1.0f };

        drawDatBaseTmp.startPos3D = m_Obj->pos;
        drawDatBaseTmp.startPos3D.z += -180.0f;

        drawDatBaseTmp.startRot = CalculateRadToDeg(atan2(m_BeamShootVec.y, m_BeamShootVec.x)) - 90.0f;
        drawDatBaseTmp.startRot3D = MkF3(90.0f + 45.0f * m_BeamShootVec.z, 0.0f, 0.0f);

        drawDatBaseTmp.startScale = MkF2(1.0f, 1.0f);
        drawDatBaseTmp.startScale3D = MkF3(1.0f, 1.0f, 1.0f);

        drawDatBaseTmp.modelNo = m_Model_Beam;

        drawDatBaseTmp.shaderOpt.bloom = DPD_SHADER_OPT_BLOOM_NORMAL;
        drawDatBaseTmp.shaderOpt.blendState = DPD_SHADER_OPT_BLEND_STATE_ADDCOLOR;

        DRAW_POLYGON_IN_CAMERA_EX_OPTION optBaseTmp = {};
        optBaseTmp.reverse = false;
        optBaseTmp.fixed = false;
        optBaseTmp.enableShade = true;
        optBaseTmp.priority = DRAW_POLYGON_PRIORITY_NML_u3;
        //opt.priority = DRAW_POLYGON_PRIORITY_NORMAL;
        optBaseTmp.depthBuffIgnore = true;
        //opt.priority = DRAW_POLYGON_PRIORITY_FORCE_NORMAL;


        int beamFrameTmp = m_BeamChargeFrame - FRAME_BEAM_SHOOT_START;

        if(m_BeamChargeFrame > 0 && m_BeamChargeFrame < FRAME_BEAM_SHOOT_END)
        {
            DRAW_POLYGON_DAT drawDatTmp = {};
            drawDatTmp.size = MkF3(1800.0f, 1800.0f, 1.0f);

            {
                //float rotTmp = CalculateDegToRad(m_CurrentAxisEx * 45.0f + 90.0f);
                float rotTmp = CalculateDegToRad(m_CurrentAxisPrivate * 45.0f + 90.0f);
                drawDatTmp.pos.x += cos(rotTmp) * 150.0f;
                drawDatTmp.pos.y += sin(rotTmp) * 150.0f;
            }
            //drawDatTmp.pos.y += 120.0f;
            drawDatTmp.pos.z += -150.0f;

            drawDatTmp.rot = 0.0f;

            drawDatTmp.rot3D = MkF3(0.0f, 0.0f, 0.0f);

            //if (m_BeamChargeFrame < FRAME_BEAM_SHOOT_START)
            //{
            //    if (m_BeamChargeFrame % 2 == 0)
            //    {
            //        drawDatTmp.color = MkF4(2.0f, 2.0f, 2.0f, 0.9f);
            //    }
            //    else
            //    {
            //        drawDatTmp.color = MkF4(-1.0f, -1.0f, -1.0f, 0.9f);
            //    }
            //}
            //else
            //{
            //    drawDatTmp.color = MkF4(2.0f, 2.0f, 2.0f, 0.9f);

            //}
            drawDatTmp.color = MkF4(2.0f, 2.0f, 2.0f, 0.9f);

            drawDatTmp.texNo = m_Tex_BeamCharge;
            drawDatTmp.loadTexType = LOADTEXTURETYPE_MAIN;

            drawDatTmp.texUV = AnimationUV(
                m_BeamChargeFrame,
                ANIMCOUNT_BEAM_CHARGE_X,
                ANIMCOUNT_BEAM_CHARGE_Y
            );
            drawDatTmp.texWH = {
                1.0f / ANIMCOUNT_BEAM_CHARGE_X,
                1.0f / ANIMCOUNT_BEAM_CHARGE_Y
            };

            drawDatTmp.startPos3D = m_Obj->pos;

            //drawDatTmp.startRot = m_CurrentAxisEx * 45.0f;
            drawDatTmp.startRot3D = MkF3(0.0f, 0.0f, 0.0f);

            drawDatTmp.startScale = MkF2(1.0f, 1.0f);
            drawDatTmp.startScale3D = MkF3(1.0f, 1.0f, 1.0f);

            drawDatTmp.modelNo = 0;
            drawDatTmp.vertex.type = DRAW_POLYGON_TYPE_QUAD;

            drawDatTmp.shaderOpt.bloom = DPD_SHADER_OPT_BLOOM_NORMAL;
            drawDatTmp.shaderOpt.blendState = DPD_SHADER_OPT_BLEND_STATE_ADDCOLOR;

            DRAW_POLYGON_IN_CAMERA_EX_OPTION opt = {};
            opt.reverse = false;
            opt.fixed = false;
            opt.enableShade = false;
            opt.billboard = true;
            //opt.priority = DRAW_POLYGON_PRIORITY_NML_u2;
            opt.priority = DRAW_POLYGON_PRIORITY_NML_u3;
            //opt.depthBuffIgnore = true;

            DrawPolygonInCameraEx(drawDatTmp, opt);
        }

        //予測エフェクト
        {
            //int startbeforef = 30;
            int startbeforef = FRAME_BEAM_SHOOT_START - 30;
            if (m_BeamChargeFrame >= FRAME_BEAM_SHOOT_START - startbeforef
                && m_BeamChargeFrame <= FRAME_BEAM_SHOOT_START
            )
            {
                float startMod = EasingInOutElastic((float)(beamFrameTmp + startbeforef) / startbeforef);

                DRAW_POLYGON_DAT drawDatTmp2 = drawDatBaseTmp;
                drawDatTmp2.texNo = m_Tex_Beam;
                drawDatTmp2.size.x = 10.0f * startMod;
                drawDatTmp2.size.y = 10.0f * startMod;

                drawDatTmp2.color = MkF4(2.0f, 0.2f, 0.2f, 0.75f);
                drawDatTmp2.shaderOpt.blendState = DPD_SHADER_OPT_BLEND_STATE_ADDCOLOR;
                //drawDatTmp2.color = MkF4(2.0f, 0.5f, 0.5f, 0.75f);
                DRAW_POLYGON_IN_CAMERA_EX_OPTION opt2 = optBaseTmp;
                opt2.priority = DRAW_POLYGON_PRIORITY_NORMAL;
                DrawPolygonInCameraEx(drawDatTmp2, opt2);
            }
        }

    }


    //{
    //    DRAW_POLYGON_DAT drawDatTmp = {};
    //    drawDatTmp.pos.y += -140.0f;
    //    drawDatTmp.pos.z += -240.0f;

    //    drawDatTmp.size = MkF3(240.0f, 240.0f, 240.0f);

    //    drawDatTmp.rot = 0.0f;

    //    drawDatTmp.rot3D = MkF3(0.0f, 0.0f, 0.0f);

    //    drawDatTmp.color = MkF4(1.0f, 1.0f, 1.0f, 1.0f);

    //    //drawDatTmp.texNo = LoadTexture("rom/texture/ui/in_game/charactor_icon.png");
    //    drawDatTmp.texNo = 0;
    //    drawDatTmp.loadTexType = LOADTEXTURETYPE_MAIN;

    //    drawDatTmp.texUV = { 0.0f, 0.0f };
    //    drawDatTmp.texWH = { 1.0f, 1.0f };

    //    drawDatTmp.startPos3D = m_Obj->pos;

    //    drawDatTmp.startRot = m_CurrentAxisEx * 45.0f;
    //    drawDatTmp.startRot3D = MkF3(90.0f, 0.0f, 0.0f);

    //    drawDatTmp.startScale = MkF2(1.0f, 1.0f);
    //    drawDatTmp.startScale3D = MkF3(1.0f, 1.0f, 1.0f);

    //    drawDatTmp.modelNo = m_TestModel;

    //    DRAW_POLYGON_IN_CAMERA_EX_OPTION opt = {};
    //    opt.reverse = false;
    //    opt.fixed = false;
    //    opt.enableShade = true;
    //    opt.priority = DRAW_POLYGON_PRIORITY_NORMAL;
    //    //opt.priority = DRAW_POLYGON_PRIORITY_FORCE_NORMAL;
    //    DrawPolygonInCameraEx(drawDatTmp, opt);
    //}
}

// ===================================================
// Dead
// オブジェクトのHPが0になったとき
// ===================================================
void GOE_Character_Enemy_Miresu::Dead()
{
	SuperGOE::Dead();

    GE_GayaCancelRequest(m_RareGayaId);

    m_Obj->hitReaction = {};

	m_Obj->collision.atk.enable = false;
	//m_Obj->collision.rec.enable = false;

	//まだ消さない
	//DeleteGameObject(m_Obj);
}

// ===================================================
// DamageAttacker
// オブジェクトの攻撃判定がヒットしたとき
// ===================================================
void GOE_Character_Enemy_Miresu::DamageAttacker(GAME_OBJECT_DAMAGE_RESULT dr)
{
	SuperGOE::DamageAttacker(dr);
}

// ===================================================
// DamageReciever
// オブジェクトが
// 別のオブジェクトの攻撃判定にヒットしたとき
// ===================================================
void GOE_Character_Enemy_Miresu::DamageReciever(GAME_OBJECT_DAMAGE_RESULT dr)
{
	SuperGOE::DamageReciever(dr);


}

// ===================================================
// BlockHit
// オブジェクトがブロックに触れたとき
// ===================================================
void GOE_Character_Enemy_Miresu::BlockHit(GAME_OBJECT_POSMOD_RESULT pmr)
{
	SuperGOE::BlockHit(pmr);
}

void GOE_Character_Enemy_Miresu::SetType(Type type, bool isGeneral)
{
    m_Type = type;

    switch (m_Type)
    {
    case Type::Hard:
    {
        m_Obj->status.hpMax *= 5;
        m_Obj->status.hp = m_Obj->status.hpMax;
        break;
    }
    case Type::Strong:
    {
        m_Obj->collision.atk.collision[0].power *= 3.0f;
        break;
    }
    default:break;
    }

    if (m_Type != Type::Default)
    {
        if (m_Tex_Main_Idle.enable)
        {
            UnloadTexData(m_Tex_Main_Idle);
        }

        switch (m_Type)
        {
        case Type::Hard:
            m_Tex_Main_Idle = LoadTexData("TEX/enemy/miresu/hard");
            break;
        case Type::Speed:
            m_Tex_Main_Idle = LoadTexData("TEX/enemy/miresu/speed");
            break;
        case Type::Strong:
            m_Tex_Main_Idle = LoadTexData("TEX/enemy/miresu/strong");
            break;
        case Type::Rapid:
            m_Tex_Main_Idle = LoadTexData("TEX/enemy/miresu/rapid");
            break;
        default:break;
        }

        GraphicSet(m_Tex_Main_Idle, 3, 3, 8);

        if (!isGeneral)
        {
            m_IsRare = true;

            m_Obj->scale.x *= 1.75f;
            m_Obj->scale.y *= 1.75f;
            m_Obj->scale.z *= 1.75f;

            m_DefeatScore = 1000;
            m_DefeatScoreType = RESULT_SCORE_EX_TYPE_RARE_ENEMY_DEFEAT;
        }
    }
    else
    {
        //m_DefeatScore = 10;
        //m_DefeatScoreType = RESULT_SCORE_EX_TYPE_ENEMY_DEFEAT;
    }
}


void GOE_Character_Enemy_Miresu::update_action_attacker()
{
    float speedMod = m_SpeedMod;
    if (m_TargetObjIdentifier.objCreateFrame > 0)
    { //対象がいるとき

        GAME_OBJECT* targetObject = GetGameObjectFindByIdentifier(m_TargetObjIdentifier);
        if (targetObject != nullptr)
        {
            float distance = CalculateDistance(targetObject->pos, m_Obj->pos);
            Float3 vec = NormalizeVector(MkF3(
                targetObject->pos.x - m_Obj->pos.x,
                targetObject->pos.y - m_Obj->pos.y,
                0.0f
            ));

            {
                unsigned int dashOneCycle = (60 * 3) + (m_Type == Type::Rapid ? 60: 0);

                int dashChargeFrame = 60 * (1.0f / speedMod);
                unsigned int dashF = m_TargetFrame % dashOneCycle;

                //突進
                if (m_TargetFrame > dashOneCycle)
                {
                    if (dashF == 1 && distance < ONE_BLOCK * 16.0f)
                    { //ため
                        GameObjectMoveRequest(m_Obj, dashChargeFrame,
                            MkF3(
                                m_Obj->pos.x + vec.x * -ONE_BLOCK * 1.2f,
                                m_Obj->pos.y + vec.y * -ONE_BLOCK * 1.2f,
                                m_Obj->pos.z + vec.z * -ONE_BLOCK * 1.2f
                            ),
                            EasingInOutSine
                        );
                    }

                    if (m_Obj->moveRequ.enable)
                    {
                        if (dashF > 0 && dashF < dashChargeFrame)
                        { //ためエフェクト
                            if (dashF % 8 == 0)
                            {
                                //{
                                //    Float3 postmp = m_Obj->pos;
                                //    postmp.z += -350.0f * m_Obj->scale.y;
                                //    GAME_OBJECT* go = GO_CreateEffect(GO_EFFECT_TYPE_FLASH_1_ENEM, postmp);
                                //    if (go != nullptr)
                                //    {
                                //        go->scale = MkF3(
                                //            3.0f,
                                //            3.0f,
                                //            3.0f
                                //        );
                                //        GameObjectSetLifeTime(go, 32);
                                //    }
                                //}
                                {
                                    Float3 postmp = m_Obj->pos;
                                    postmp.z += -150.0f * m_Obj->scale.y;
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
                                            6.0,
                                            6.0,
                                            6.0
                                        );
                                        GameObjectSetLifeTime(go, 30);
                                    }
                                }
                                //CameraShakeRequest(CAMERA_SHAKE_SMALL_FRAME, CAMERA_SHAKE_SMALL_POWER);
                            }
                        }
                        else
                        {
                            //突進の残像
                            //if (dashF >= dashChargeFrame && dashF < dashChargeFrame + 9)
                            {
                                if (dashF % 3 == 0)
                                {
                                    GAME_OBJECT* go = GO_CreateAfterEffect(m_Obj, 30, MkF4(0.8f, 0.75f, 1.0f, 0.75f));
                                }
                            }
                        }

                        if ((dashF == dashChargeFrame)
                            || (
                                m_Type == Type::Rapid
                                && dashF >= dashChargeFrame
                                && dashF % 30 == 0
                                && dashF <= dashChargeFrame + 30 * 6
                            )
                        )
                        { //突進
                            GAME_OBJECT_COLLISION_ATTACK_CHILD attackColTmp = m_Obj->collision.atk.collision[0];
                            strcpy_s(attackColTmp.tag, GAME_OBJECT_TXT_MAX, "");

                            //attackColTmp.subjectFamily[GAME_OBJECT_FAMILY_ALL] = true;
                            attackColTmp.subjectFamily[GAME_OBJECT_FAMILY_PLAYER] = true;

                            attackColTmp.size = Float2ToFloat3(m_Obj->size);
                            attackColTmp.size.x = ONE_BLOCK * 1.2f * m_Obj->scale.x;
                            attackColTmp.size.y = ONE_BLOCK * 1.2f * m_Obj->scale.y;
                            attackColTmp.size.z = ONE_BLOCK * 3.5f * m_Obj->scale.z;
                            attackColTmp.pos.z = -attackColTmp.size.z / 2;

                            //攻撃判定を生成
                            GameObjectAttackCollisionAdd(m_Obj, attackColTmp,
                                1,
                                10
                            );

                            GameObjectMoveRequest(m_Obj, 30,
                                MkF3(
                                    m_Obj->pos.x + vec.x * ONE_BLOCK * 8.0f * speedMod,
                                    m_Obj->pos.y + vec.y * ONE_BLOCK * 8.0f * speedMod,
                                    m_Obj->pos.z + vec.z * ONE_BLOCK * 8.0f * speedMod
                                ),
                                EasingOutBack
                            );
                        }


                    }
                }

                if (!m_Obj->moveRequ.enable && distance > ONE_BLOCK * 5.0f)
                //if (dashF > 100 && distance > ONE_BLOCK * 5.0f)
                { //接近
                    //m_Obj->vel.x += vec.x * ONE_BLOCK / 30 * speedMod;
                    //m_Obj->vel.y += vec.y * ONE_BLOCK / 30 * speedMod;
                    //m_Obj->vel.z += vec.z * ONE_BLOCK / 30 * speedMod;
                    m_Obj->vel.x += vec.x * ONE_BLOCK / 25 * speedMod;
                    m_Obj->vel.y += vec.y * ONE_BLOCK / 25 * speedMod;
                    m_Obj->vel.z += vec.z * ONE_BLOCK / 25 * speedMod;
                    //if (privateDat->targetFrame % 40 == 0)
                    //{
                    //	GameObjectMoveRequest(obj, 60,
                    //		MkF3(
                    //			obj->pos.x + vec.x * OneBlock * 2.0f,
                    //			obj->pos.y + vec.y * OneBlock * 2.0f,
                    //			obj->pos.z + vec.z * OneBlock * 2.0f
                    //		),
                    //		EasingInOutSine
                    //	);
                    //}
                }
            }

            //タゲ外し
            if (distance >= ONE_BLOCK * 50
                || IsDefeatGameObject(targetObject))
            {
                m_TargetObjIdentifier = {};
                m_TargetFrame = -1;
                m_BeamChargeFrame = 0;
            }


            //対象への向きを入れる
            m_BeforeVec = vec;
        }

        //タゲはいってからのカウント
        m_TargetFrame++;
    }
    else //対象がいないとき
    {
        //非戦闘時
        {
            //マス内のプレイヤーを検索
            std::vector<PLAYER> players = GetPlayesrFindByDistance(m_Obj->pos, ONE_BLOCK * 20);
            players.reserve(PLAYER_MAX);

            //マス以内にプレイヤーがいない
            if (players.size() <= 0)
            {
                //巡回
                if ((m_Obj->frameActive + GetGameObjectIndex(m_Obj) * 37) % 240 == 0)
                {
                    Float3 movepos = {};
                    movepos.x = sin((float)GetMainGameFrame()) * ONE_BLOCK * 4.0f * speedMod;
                    movepos.y = cos((float)GetMainGameFrame()) * ONE_BLOCK * 4.0f * speedMod;

                    GameObjectMoveRequest(m_Obj, 180,
                        MkF3(
                            m_SpawnPoint.x + movepos.x,
                            m_SpawnPoint.y + movepos.y,
                            m_SpawnPoint.z + movepos.z
                        ),
                        EasingInOutSine
                    );
                }
            }
        }

        //索敵
        {

            float searchaArea = ONE_BLOCK * 10;
            if (m_Type == Type::Speed)
            {
                speedMod *= 1.5f;
            }

            //マス内のプレイヤーを検索
            std::vector<PLAYER> players = GetPlayesrFindByDistance(m_Obj->pos,
                searchaArea);
            players.reserve(PLAYER_MAX);

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
                m_TargetFrame = 0;

                { //えふぇくと
                    Float3 postmp = m_Obj->pos;
                    postmp.y += -200.0f * m_Obj->scale.y;
                    postmp.z += -550.0f * m_Obj->scale.y;
                    GAME_OBJECT* go = GO_CreateEffect(GO_EFFECT_TYPE_EXCLAMATION, postmp);
                    if (go != nullptr)
                    {
                        go->scale = MkF3(
                            1.0f,
                            1.0f,
                            1.0f
                        );
                        GameObjectSetLifeTime(go, 50);
                    }
                }
            }
        }

        m_BeforeVec = m_Obj->vel;
    }
}
void GOE_Character_Enemy_Miresu::update_action_shooter()
{
    float speedMod = m_SpeedMod;
    if (m_TargetObjIdentifier.objCreateFrame > 0)
    { //対象がいるとき
        GAME_OBJECT* targetObject = GetGameObjectFindByIdentifier(m_TargetObjIdentifier);
        if (targetObject != nullptr)
        {
            m_BeamTargetPos = GetGameObjectColRecAbsolutePos(targetObject);

            float distance = CalculateDistance(m_BeamTargetPos, m_Obj->pos);
            Float3 vec = NormalizeVector(MkF3(
                m_BeamTargetPos.x - m_Obj->pos.x,
                m_BeamTargetPos.y - m_Obj->pos.y,
                m_BeamTargetPos.z - m_Obj->pos.z
            ));

            {
                m_BeamShootVec = vec;
                m_BeamChargeFrame = m_TargetFrame % (ANIMCOUNTMAX_BEAM_CHARGE);

                if ((m_BeamChargeFrame == FRAME_BEAM_SHOOT_START)
                    || (
                        m_Type == Type::Rapid
                        && m_BeamChargeFrame >= FRAME_BEAM_SHOOT_START
                        && m_BeamChargeFrame % 8 == 0
                        && m_BeamChargeFrame <= FRAME_BEAM_SHOOT_START + 8 * 6
                        )
                    )
                {
                    //SEを再生
                    SoundEffectPlayRequest(
                        GetSoundData("SOUND/SE/enemy/attack/Laser"),
                        0.9f, 0);

                    CameraShakeRequestAll(CAMERA_SHAKE_MIDIUM_FRAME, CAMERA_SHAKE_MIDIUM_POWER);

                    //GAME_OBJECT beamObj = GetGameObjectPrefab(GAME_OBJECT_TYPE_DUMMY);
                    //beamObj.pos = m_Obj->pos;
                    //beamObj.pos.x += vec.x * ONE_BLOCK * 3.0f;
                    //beamObj.pos.y += vec.y * ONE_BLOCK * 3.0f;
                    //beamObj.pos.z += vec.z * ONE_BLOCK * 3.0f;

                    //{
                    //    Character::MOTION_COL_DAT motionColDetails =
                    //        Character::GetMotionColDataFindByName(
                    //            Character::MOTION_COL_TYPE_ATK,
                    //            BEAM_ATK_COL_NAME
                    //        );

                    //    GAME_OBJECT_COLLISION_ATTACK_CHILD attackColTmp = motionColDetails.atkCol;
                    //    attackColTmp.subjectFamily[GAME_OBJECT_FAMILY_PLAYER] = true;

                    //    attackColTmp.size = MkF3(
                    //        ONE_BLOCK * 3,
                    //        ONE_BLOCK * 3,
                    //        ONE_BLOCK * 3
                    //    );
                    //    attackColTmp.pos.z = -attackColTmp.size.z / 2;
                    //    attackColTmp.pos.y += attackColTmp.size.y / 2 * 1.0f;

                    //    beamObj.collision.atk.enable = true;
                    //    beamObj.collision.atk.collision[0] = attackColTmp;
                    //    beamObj.collision.atk.collision[0].enable = true;
                    //}

                    //GAME_OBJECT* beamObjCreated = CreateGameObject(beamObj);
                    //GameObjectSetLifeTime(beamObjCreated, FRAME_BEAM_LIFETIME);
                    //GameObjectMoveRequest(beamObjCreated, FRAME_BEAM_LIFETIME,
                    //    MkF3(
                    //        beamObjCreated->pos.x + vec.x * ONE_BLOCK * 60.0f,
                    //        beamObjCreated->pos.y + vec.y * ONE_BLOCK * 60.0f,
                    //        beamObjCreated->pos.z + vec.z * ONE_BLOCK * 60.0f
                    //    ),
                    //    EasingInSine
                    //);

                    ////m_Beam_Objects.push_back(GetIdentifierGameObject(beamObjCreated));
                    //m_Beam_Objects[m_Beam_Object_Count] = GetIdentifierGameObject(beamObjCreated);
                    //m_Beam_Object_Count++;

                    GOE_Character_EnemyProjectile* goepro = 
                        GOE_Character_EnemyProjectile::CreateProjectile(
                            GOE_Character_EnemyProjectile::Type::Beam, 
                            MkF3(
                                m_Obj->pos.x + vec.x * ONE_BLOCK * 3.0f,
                                m_Obj->pos.y + vec.y * ONE_BLOCK * 3.0f,
                                m_Obj->pos.z + vec.z * ONE_BLOCK * 3.0f - 80.0f
                            ),
                            vec
                        );
                    if (goepro != nullptr)
                    {
                        GAME_OBJECT* beamObj = goepro->GetGameObj();
                        if (beamObj != nullptr)
                        {
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
                                    beamObj->pos.x + vec.x * ONE_BLOCK * 60.0f,
                                    beamObj->pos.y + vec.y * ONE_BLOCK * 60.0f,
                                    beamObj->pos.z + vec.z * ONE_BLOCK * 60.0f
                                ),
                                EasingInSine
                            );
                        }
                    }
                }
            }

            ////タゲ外し
            //if(distance >= ONE_BLOCK * 50)
            //{
            //    m_TargetObjIdentifier = {};
            //    m_TargetFrame = -1;
            //    m_BeamChargeFrame = 0;
            //}

            //タゲ外し
            if (distance >= ONE_BLOCK * 50
                || IsDefeatGameObject(targetObject)
            )
            {
                m_TargetObjIdentifier = {};
                m_TargetFrame = -1;
                m_BeamChargeFrame = 0;
            }

            //対象への向きを入れる
            m_BeforeVec = vec;
        }

        //タゲはいってからのカウント
        m_TargetFrame++;
    }
    else //対象がいないとき
    {
        //非戦闘時

        //索敵
        {
            //マス内のプレイヤーを検索
            std::vector<PLAYER> players = GetPlayesrFindByDistance(m_Obj->pos, ONE_BLOCK * 30);
            players.reserve(PLAYER_MAX);

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
                m_TargetFrame = 0;
                m_BeamChargeFrame = 0;

                { //えふぇくと
                    Float3 postmp = m_Obj->pos;
                    postmp.y += -200.0f * m_Obj->scale.y;
                    postmp.z += -550.0f * m_Obj->scale.y;
                    GAME_OBJECT* go = GO_CreateEffect(GO_EFFECT_TYPE_EXCLAMATION, postmp);
                    if (go != nullptr)
                    {
                        go->scale = MkF3(
                            1.0f,
                            1.0f,
                            1.0f
                        );
                        GameObjectSetLifeTime(go, 50);
                    }
                }
            }
            else
            {
                {
                    //巡回
                    if ((m_Obj->frameActive + GetGameObjectIndex(m_Obj) * 37) % 240 == 0)
                    {
                        Float3 movepos = {};
                        movepos.x = sin((float)GetMainGameFrame()) * ONE_BLOCK * 4.0f * speedMod;
                        movepos.y = cos((float)GetMainGameFrame()) * ONE_BLOCK * 4.0f * speedMod;

                        GameObjectMoveRequest(m_Obj, 180,
                            MkF3(
                                m_SpawnPoint.x + movepos.x,
                                m_SpawnPoint.y + movepos.y,
                                m_SpawnPoint.z + movepos.z
                            ),
                            EasingInOutSine
                        );
                    }
                }
            }
        }

        m_BeforeVec = m_Obj->vel;
    }
}

void GOE_Character_Enemy_Miresu::update_action_hybrid()
{
    float speedMod = m_SpeedMod;
    if (m_TargetObjIdentifier.objCreateFrame > 0)
    { //対象がいるとき
        GAME_OBJECT* targetObject = GetGameObjectFindByIdentifier(m_TargetObjIdentifier);
        if (targetObject != nullptr)
        {
            m_BeamTargetPos = GetGameObjectColRecAbsolutePos(targetObject);

            float distance = CalculateDistance(m_BeamTargetPos, m_Obj->pos);
            Float3 vec = NormalizeVector(MkF3(
                m_BeamTargetPos.x - m_Obj->pos.x,
                m_BeamTargetPos.y - m_Obj->pos.y,
                m_BeamTargetPos.z - m_Obj->pos.z
            ));

            if (distance < ONE_BLOCK * 25.0f)
            {
                m_BeamChargeFrame = 0;

                unsigned int dashOneCycle = (60 * 3);

                int dashChargeFrame = 60 * (1.0f / speedMod);
                unsigned int dashF = m_TargetFrame % dashOneCycle;

                //突進
                if (m_TargetFrame > dashOneCycle)
                {
                    if (dashF == 1 && distance < ONE_BLOCK * 16.0f)
                    { //ため
                        GameObjectMoveRequest(m_Obj, dashChargeFrame,
                            MkF3(
                                m_Obj->pos.x + vec.x * -ONE_BLOCK * 1.2f,
                                m_Obj->pos.y + vec.y * -ONE_BLOCK * 1.2f,
                                m_Obj->pos.z
                            ),
                            EasingInOutSine
                        );
                    }

                    if (m_Obj->moveRequ.enable)
                    {
                        if (dashF > 0 && dashF < dashChargeFrame)
                        { //ためエフェクト
                            if (dashF % 8 == 0)
                            {
                                //{
                                //    Float3 postmp = m_Obj->pos;
                                //    postmp.z += -350.0f * m_Obj->scale.y;
                                //    GAME_OBJECT* go = GO_CreateEffect(GO_EFFECT_TYPE_FLASH_1_ENEM, postmp);
                                //    if (go != nullptr)
                                //    {
                                //        go->scale = MkF3(
                                //            3.0f,
                                //            3.0f,
                                //            3.0f
                                //        );
                                //        GameObjectSetLifeTime(go, 32);
                                //    }
                                //}
                                {
                                    Float3 postmp = m_Obj->pos;
                                    postmp.z += -150.0f * m_Obj->scale.y;
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
                                            6.0,
                                            6.0,
                                            6.0
                                        );
                                        GameObjectSetLifeTime(go, 30);
                                    }
                                }
                                //CameraShakeRequest(CAMERA_SHAKE_SMALL_FRAME, CAMERA_SHAKE_SMALL_POWER);
                            }
                        }
                        else
                        {
                            //突進の残像
                            //if (dashF >= dashChargeFrame && dashF < dashChargeFrame + 9)
                            {
                                if (dashF % 3 == 0)
                                {
                                    GAME_OBJECT* go = GO_CreateAfterEffect(m_Obj, 30, MkF4(0.8f, 0.75f, 1.0f, 0.75f));
                                }
                            }
                        }

                        if ((dashF == dashChargeFrame)
                            || (
                                m_Type == Type::Rapid
                                && dashF >= dashChargeFrame
                                && dashF % 30 == 0
                                && dashF <= dashChargeFrame + 30 * 6
                                )
                            )
                        { //突進
                            GAME_OBJECT_COLLISION_ATTACK_CHILD attackColTmp = m_Obj->collision.atk.collision[0];
                            strcpy_s(attackColTmp.tag, GAME_OBJECT_TXT_MAX, "");

                            //attackColTmp.subjectFamily[GAME_OBJECT_FAMILY_ALL] = true;
                            attackColTmp.subjectFamily[GAME_OBJECT_FAMILY_PLAYER] = true;

                            attackColTmp.size = Float2ToFloat3(m_Obj->size);
                            attackColTmp.size.x = ONE_BLOCK * 1.2f * m_Obj->scale.x;
                            attackColTmp.size.y = ONE_BLOCK * 1.2f * m_Obj->scale.y;
                            attackColTmp.size.z = ONE_BLOCK * 3.5f * m_Obj->scale.z;
                            attackColTmp.pos.z = -attackColTmp.size.z / 2;

                            //攻撃判定を生成
                            GameObjectAttackCollisionAdd(m_Obj, attackColTmp,
                                1,
                                10
                            );

                            GameObjectMoveRequest(m_Obj, 30,
                                MkF3(
                                    m_Obj->pos.x + vec.x * ONE_BLOCK * 8.0f * speedMod,
                                    m_Obj->pos.y + vec.y * ONE_BLOCK * 8.0f * speedMod,
                                    m_Obj->pos.z
                                ),
                                EasingOutBack
                            );
                        }


                    }
                }

                if (!m_Obj->moveRequ.enable && distance > ONE_BLOCK * 5.0f)
                    //if (dashF > 100 && distance > ONE_BLOCK * 5.0f)
                { //接近
                    //m_Obj->vel.x += vec.x * ONE_BLOCK / 30 * speedMod;
                    //m_Obj->vel.y += vec.y * ONE_BLOCK / 30 * speedMod;
                    //m_Obj->vel.z += vec.z * ONE_BLOCK / 30 * speedMod;
                    m_Obj->vel.x += vec.x * ONE_BLOCK / 25 * speedMod;
                    m_Obj->vel.y += vec.y * ONE_BLOCK / 25 * speedMod;
                    m_Obj->vel.z += vec.z * ONE_BLOCK / 25 * speedMod;
                    //if (privateDat->targetFrame % 40 == 0)
                    //{
                    //	GameObjectMoveRequest(obj, 60,
                    //		MkF3(
                    //			obj->pos.x + vec.x * OneBlock * 2.0f,
                    //			obj->pos.y + vec.y * OneBlock * 2.0f,
                    //			obj->pos.z + vec.z * OneBlock * 2.0f
                    //		),
                    //		EasingInOutSine
                    //	);
                    //}
                }
            }
            else
            {
                m_BeamShootVec = vec;
                m_BeamChargeFrame = m_TargetFrame % (ANIMCOUNTMAX_BEAM_CHARGE);

                if (m_BeamChargeFrame == FRAME_BEAM_SHOOT_START)
                {
                    //SEを再生
                    SoundEffectPlayRequest(
                        GetSoundData("SOUND/SE/enemy/attack/Laser"), 
                        0.9f, 0);

                    CameraShakeRequestAll(CAMERA_SHAKE_MIDIUM_FRAME, CAMERA_SHAKE_MIDIUM_POWER);

                    GOE_Character_EnemyProjectile* goepro =
                        GOE_Character_EnemyProjectile::CreateProjectile(
                            GOE_Character_EnemyProjectile::Type::Beam,
                            MkF3(
                                m_Obj->pos.x + vec.x * ONE_BLOCK * 3.0f,
                                m_Obj->pos.y + vec.y * ONE_BLOCK * 3.0f,
                                m_Obj->pos.z + vec.z * ONE_BLOCK * 3.0f - 80.0f
                            ),
                            vec
                        );
                    if (goepro != nullptr)
                    {
                        GAME_OBJECT* beamObj = goepro->GetGameObj();
                        if (beamObj != nullptr)
                        {
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
                                    beamObj->pos.x + vec.x * ONE_BLOCK * 60.0f,
                                    beamObj->pos.y + vec.y * ONE_BLOCK * 60.0f,
                                    beamObj->pos.z + vec.z * ONE_BLOCK * 60.0f
                                ),
                                EasingInSine
                            );
                        }
                    }
                }
            }

            //タゲ外し
            if (distance >= ONE_BLOCK * 50
                || IsDefeatGameObject(targetObject))
            {
                m_TargetObjIdentifier = {};
                m_TargetFrame = -1;
                m_BeamChargeFrame = 0;
            }

            //対象への向きを入れる
            m_BeforeVec = vec;
        }

        //タゲはいってからのカウント
        m_TargetFrame++;
    }
    else //対象がいないとき
    {
        //非戦闘時

        //索敵
        {

            float searchaArea = ONE_BLOCK * 30;
            if (m_Type == Type::Speed)
            {
                speedMod *= 2.0f;
            }

            //マス内のプレイヤーを検索
            std::vector<PLAYER> players = GetPlayesrFindByDistance(m_Obj->pos
                , searchaArea);
            players.reserve(PLAYER_MAX);

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
                m_TargetFrame = 0;
                m_BeamChargeFrame = 0;

                { //えふぇくと
                    Float3 postmp = m_Obj->pos;
                    postmp.y += -200.0f * m_Obj->scale.y;
                    postmp.z += -550.0f * m_Obj->scale.y;
                    GAME_OBJECT* go = GO_CreateEffect(GO_EFFECT_TYPE_EXCLAMATION, postmp);
                    if (go != nullptr)
                    {
                        go->scale = MkF3(
                            1.0f,
                            1.0f,
                            1.0f
                        );
                        GameObjectSetLifeTime(go, 50);
                    }
                }
            }
            else
            {
                {
                    //巡回
                    if ((m_Obj->frameActive + GetGameObjectIndex(m_Obj) * 37) % 240 == 0)
                    {
                        Float3 movepos = {};
                        movepos.x = sin((float)GetMainGameFrame()) * ONE_BLOCK * 4.0f * speedMod;
                        movepos.y = cos((float)GetMainGameFrame()) * ONE_BLOCK * 4.0f * speedMod;

                        GameObjectMoveRequest(m_Obj, 180,
                            MkF3(
                                m_SpawnPoint.x + movepos.x,
                                m_SpawnPoint.y + movepos.y,
                                m_SpawnPoint.z + movepos.z
                            ),
                            EasingInOutSine
                        );
                    }
                }
            }
        }

        m_BeforeVec = m_Obj->vel;
    }
}
