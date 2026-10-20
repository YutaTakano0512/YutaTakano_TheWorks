// ===================================================
// GOE_Character_PlayerProjectile.cpp オブジェクトイベントエフェクト
// 
// hara sougo  2025/01/12
// ===================================================
#include "../../../../game_main.h"
#include "../../../Sprite/sprite.h"
#include "../../../Polygon/modelData.h"
#include "../../../Texture/texture.h"
#include "../../../../Game/Entities/Block/block.h"
#include "../../../Systems/collision.h"
#include "../../../Systems/easing.h"

#include "../../gameObject.h"
#include "../../gameObjectGraph.h"
#include "../../gameObjectPrefab.h"

#include "../../../Systems/ui.h"
#include "../../../Managers/gameManager.h"
#include "../../../Managers/soundManager.h"

#include "../../../../Scenes/SCENE_GAME.h"
#include "../../../Game Stage/Stage_Base.h"

#include "../GOE_Effect.h"
#include "../GOE_Character_Enemy.h"
#include "GOE_Character_PlayerProjectile.h"

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
// オブジェクトの初期化
// ===================================================
void GOE_Character_PlayerProjectile::Init()
{
    m_Tex_EfBuff_Flow = LoadTexData("TEX/effect/buff_flow").textureId;
    m_Tex_EfBuff_Flare = LoadTexData("TEX/effect/buff_flare").textureId;

    m_Model_Beam_Misile = GetModelDataIndex("MODEL/beam_misile02");

    m_Tex_ShockWaveSprite = LoadTexData("TEX/effect/Ps_tex_sprite").textureId;
    m_Tex_ShockWaveFlow = LoadTexData("TEX/effect/Ps_tex_mesh").textureId;
    m_Model_ShockWaveFlow = GetModelDataIndex("MODEL/beam_01");
}

void GOE_Character_PlayerProjectile::FirstFrame()
{
    if (m_type == Type::ContriusBomb)
    {
    }
}

// ===================================================
// オブジェクトの終了
// ===================================================
void GOE_Character_PlayerProjectile::Uninit()
{
    UnloadTexData(m_Tex_EfBuff_Flow);
    UnloadTexData(m_Tex_EfBuff_Flare);

    UnloadTexData(m_Tex_ShockWaveSprite);
    UnloadTexData(m_Tex_ShockWaveFlow);
}

// ===================================================
// オブジェクトの更新
// ===================================================
void GOE_Character_PlayerProjectile::Update()
{
    if (m_type == Type::ContriusBomb)
    {
        m_Obj->hitStop = {};
    }
}

// ===================================================
// オブジェクトの更新(アクティブ)
// ===================================================
void GOE_Character_PlayerProjectile::UpdateActive()
{
    if (GameManagerIsMovieFlag())
    { //ムービー中は止める
        return;
    }

    if (m_type == Type::ContriusBomb)
    {
    }

    if (m_type == Type::ContriusShockWave)
    {
        float sca = (m_Obj->scale.x + m_Obj->scale.y) / 2.0f;

        if (m_Obj->frameActive % 5 == 0)
        {
            {
                Float3 posTmp = m_Obj->pos;

                GAME_OBJECT* go = GO_CreateEffect(GO_EFFECT_TYPE_EXPLOSION_1_GROUND, posTmp);
                if (go != nullptr)
                {
                    go->rot = 55.0f * m_Obj->frame;
                    go->scale = MkF3(
                        2.4f * sca,
                        2.4f * sca,
                        2.4f * sca
                    );
                    GameObjectSetLifeTime(go, 40);
                }
            }
        }
        {
            Stage_Base* stg = GetCurrentStageSceneGame();
            if (stg != nullptr)
            {
                Int2 stgsize = stg->GetStageSize();

                Float2 size = {};
                size.x = BLOCK_WIDTH * stgsize.x;
                size.y = BLOCK_HIGHT * stgsize.y;

                if (abs(m_Obj->pos.x) > size.x / 2
                    || abs(m_Obj->pos.y) > size.y / 2)
                {
                    DeleteGameObject(m_Obj);
                }
            }
        }
    }
}


void GOE_Character_PlayerProjectile::Draw()
{
    if (m_type == Type::ContriusBomb)
    {
    }
    if (m_type == Type::ContriusShockWave)
    {
        //DRAW_POLYGON_DAT drawDatBaseTmp = {};

        //drawDatBaseTmp.rot = 0.0f;

        //drawDatBaseTmp.rot3D = MkF3(0.0f, 0.0f, 0.0f);

        //drawDatBaseTmp.color = MkF4(1.0f * 2, 0.9f * 2, 1.0f * 2, 0.9f);

        //drawDatBaseTmp.texNo = 0;
        //drawDatBaseTmp.loadTexType = LOADTEXTURETYPE_MAIN;

        //drawDatBaseTmp.texUV = { 0.0f, 0.0f };
        //drawDatBaseTmp.texWH = { 1.0f, 1.0f };

        //drawDatBaseTmp.startScale = MkF2(1.0f, 1.0f);
        //drawDatBaseTmp.startScale3D = MkF3(1.0f, 1.0f, 1.0f);

        //drawDatBaseTmp.modelNo = m_Model_Beam_Misile;
        ////drawDatBaseTmp.modelNo = m_Model_Beam;

        //drawDatBaseTmp.shaderOpt.bloom = DPD_SHADER_OPT_BLOOM_NORMAL;
        //drawDatBaseTmp.shaderOpt.blendState = DPD_SHADER_OPT_BLEND_STATE_ADDCOLOR;

        //DRAW_POLYGON_IN_CAMERA_EX_OPTION optBaseTmp = {};
        //optBaseTmp.reverse = false;
        //optBaseTmp.fixed = false;
        //optBaseTmp.enableShade = true;
        ////optBaseTmp.priority = DRAW_POLYGON_PRIORITY_NML_u1;
        //optBaseTmp.priority = DRAW_POLYGON_PRIORITY_NML_d1;
        ////optBaseTmp.depthBuffIgnore = true;

        //{
        //    GAME_OBJECT* beamObj = m_Obj;
        //    if (beamObj != nullptr
        //        && (beamObj->vel.x != 0.0f || beamObj->vel.y != 0.0f))
        //    {

        //        {

        //            DRAW_POLYGON_DAT drawDatTmp = drawDatBaseTmp;
        //            //drawDatTmp.color = MkF4(1.0f, 0.4f, 0.0f, 1.0f);
        //            drawDatTmp.color = MkF4(2.0f, 0.8f, 0.0f, 1.0f);
        //            drawDatTmp.texUV.x = -0.008f * beamObj->frameActive;
        //            drawDatTmp.texNo = m_Tex_EfBuff_Flare;
        //            drawDatTmp.size = MkF3(
        //                100.0f * m_Obj->scale.x * 1.0f,
        //                100.0f * m_Obj->scale.y * 1.0f,
        //                130.0f * m_Obj->scale.z * 1.0f
        //            );

        //            drawDatTmp.pos = {};
        //            ///drawDatTmp.pos.z += -drawDatTmp.size.z;

        //            drawDatTmp.startPos3D = beamObj->pos;
        //            drawDatTmp.startPos3D.y += ONE_BLOCK * 0.5f * m_Obj->scale.y;
        //            drawDatTmp.startPos3D.z += ONE_BLOCK * -5.0f * m_Obj->scale.z;

        //            {
        //                {
        //                    float addSizeTmp = sin((float)(beamObj->frameActive) * 0.8f) * 2.0f;
        //                    drawDatTmp.size.x += addSizeTmp;
        //                    drawDatTmp.size.y += addSizeTmp;
        //                }

        //                {
        //                    //int endf = 120;
        //                    int endf = 8;
        //                    if (beamObj->frameActive < endf)
        //                    {
        //                        float startMod = EasingOutElastic((float)beamObj->frameActive / endf);
        //                        drawDatTmp.size.x *= startMod;
        //                        drawDatTmp.size.y *= startMod;
        //                        //drawDatTmp.size.z *= startMod;
        //                    }
        //                }
        //                {
        //                    //int endf = 40;
        //                    int endf = 30;
        //                    if (beamObj->frameActive >= beamObj->lifetimeFrameMax - endf)
        //                    {
        //                        int f = beamObj->frameActive - (beamObj->lifetimeFrameMax - endf);
        //                        float endMod = 1.0f - (float)f / endf;
        //                        drawDatTmp.size.x *= endMod;
        //                        drawDatTmp.size.y *= endMod;
        //                        //drawDatTmp.size.z *= endMod;
        //                    }
        //                }

        //                DRAW_POLYGON_DAT drawDatTmp2 = drawDatTmp;
        //                drawDatTmp2.size.z *= -1.5f;
        //                drawDatTmp2.texUV.x = 0.005f * beamObj->frameActive;
        //                drawDatTmp2.texNo = m_Tex_EfBuff_Flow;
        //                drawDatTmp2.size.x *= 1.5f;
        //                drawDatTmp2.size.y *= 1.5f;
        //                //drawDatTmp2.color = MkF4(1.0f, 0.8f, 0.1f, 0.9f);
        //                drawDatTmp2.color = MkF4(2.0f, 1.6f, 0.2f, 0.9f);
        //                drawDatTmp2.startPos3D.z += ONE_BLOCK * 8.0f * m_Obj->scale.z;

        //                DRAW_POLYGON_IN_CAMERA_EX_OPTION opt2 = optBaseTmp;
        //                //opt2.priority = DRAW_POLYGON_PRIORITY_NML_u2;
        //                //opt2.priority = DRAW_POLYGON_PRIORITY_NML_d1;
        //                opt2.priority = DRAW_POLYGON_PRIORITY_NORMAL;


        //                DrawPolygonInCameraEx(drawDatTmp2, opt2);
        //                DrawPolygonInCameraEx(drawDatTmp, optBaseTmp);
        //            }
        //        }

        //    }
        //}

        DRAW_POLYGON_DAT drawDatBaseTmp = {};

        drawDatBaseTmp.rot = 0.0f;

        drawDatBaseTmp.rot3D = MkF3(0.0f, 0.0f, 0.0f);

        drawDatBaseTmp.color = m_ColorGenral;

        drawDatBaseTmp.texNo = 0;
        drawDatBaseTmp.loadTexType = LOADTEXTURETYPE_MAIN;

        drawDatBaseTmp.texUV = { 0.0f, 0.0f };
        drawDatBaseTmp.texWH = { 1.0f, 1.0f };

        drawDatBaseTmp.startScale = MkF2(1.0f, 1.0f);
        drawDatBaseTmp.startScale3D = MkF3(1.0f, 1.0f, 1.0f);

        drawDatBaseTmp.modelNo = 0;

        drawDatBaseTmp.shaderOpt.bloom = DPD_SHADER_OPT_BLOOM_NORMAL;
        drawDatBaseTmp.shaderOpt.blendState = DPD_SHADER_OPT_BLEND_STATE_ADDCOLOR;

        DRAW_POLYGON_IN_CAMERA_EX_OPTION optBaseTmp = {};
        optBaseTmp.reverse = false;
        optBaseTmp.fixed = false;
        optBaseTmp.enableShade = true;
        //optBaseTmp.priority = DRAW_POLYGON_PRIORITY_NML_u1;
        //optBaseTmp.priority = DRAW_POLYGON_PRIORITY_NML_d1;
        //optBaseTmp.depthBuffIgnore = true;

        {
            GAME_OBJECT* beamObj = m_Obj;
            if (beamObj != nullptr
                && (beamObj->vel.x != 0.0f || beamObj->vel.y != 0.0f))
            {
                Float2 sizesca = {1.0f, 1.0f};
                //{
                //    float addSizeTmp = sin((float)(beamObj->frameActive) * 0.8f) * 2.0f;
                //    sizesca.x += addSizeTmp;
                //    sizesca.y += addSizeTmp;
                //}

                {
                    //int endf = 120;
                    int endf = 8;
                    if (beamObj->frameActive < endf)
                    {
                        float startMod = EasingOutElastic((float)beamObj->frameActive / endf);
                        sizesca.x *= startMod;
                        sizesca.y *= startMod;
                        //sizesca.z *= startMod;
                    }
                }
                {
                    //int endf = 40;
                    int endf = 30;
                    if (beamObj->frameActive >= beamObj->lifetimeFrameMax - endf)
                    {
                        int f = beamObj->frameActive - (beamObj->lifetimeFrameMax - endf);
                        float endMod = 1.0f - (float)f / endf;
                        sizesca.x *= endMod;
                        sizesca.y *= endMod;
                        //drawDatTmp.size.z *= endMod;
                    }
                }

                {

                    DRAW_POLYGON_DAT drawDatTmp = drawDatBaseTmp;
                    drawDatTmp.color = m_ColorGenral;
                    //drawDatTmp.texUV.x = -0.008f * beamObj->frameActive;
                    drawDatTmp.texNo = m_Tex_ShockWaveFlow;
                    drawDatTmp.modelNo = m_Model_ShockWaveFlow;

                    drawDatTmp.size = MkF3(
                        45.0f * m_Obj->scale.x * 1.0f * sizesca.x,
                        45.0f * m_Obj->scale.y * 1.0f * sizesca.y,
                        150.0f * m_Obj->scale.z * 1.0f
                    );

                    drawDatTmp.pos = {};
                    drawDatTmp.pos.z = drawDatTmp.size.z * 0.5f;
                    drawDatTmp.startPos3D = beamObj->pos;

                    DRAW_POLYGON_IN_CAMERA_EX_OPTION opt = optBaseTmp;
                    //opt.priority = DRAW_POLYGON_PRIORITY_NORMAL;
                    opt.priority = DRAW_POLYGON_PRIORITY_NML_d1;
                    opt.depthBuffIgnore = true;



                    DRAW_POLYGON_DAT drawDatTmp2 = drawDatTmp;
                    drawDatTmp2.texNo = m_Tex_ShockWaveSprite;
                    drawDatTmp2.modelNo = 0;
                    drawDatTmp2.color = m_ColorGenral;
                    drawDatTmp2.size = MkF3(
                        900.0f * m_Obj->scale.x * 1.0f * sizesca.x,
                        900.0f * m_Obj->scale.z * 1.0f,
                        900.0f * m_Obj->scale.y * 1.0f * sizesca.y
                    );

                    drawDatTmp2.rot3D.x = 90.0f;
                    drawDatTmp2.pos = {};
                    //drawDatTmp2.pos.y = drawDatTmp2.size.y * -0.05f;
                    drawDatTmp2.pos.z = drawDatTmp2.size.y * -0.48f;
                    drawDatTmp2.startPos3D = beamObj->pos;

                    drawDatTmp2.shaderOpt.blendState = DPD_SHADER_OPT_BLEND_STATE_DEFAULT;

                    DRAW_POLYGON_IN_CAMERA_EX_OPTION opt2 = optBaseTmp;
                    opt2.priority = DRAW_POLYGON_PRIORITY_NORMAL;
                    //opt2.priority = DRAW_POLYGON_PRIORITY_NML_u1;
                    //opt2.depthBuffIgnore = false;
                    opt2.depthBuffIgnore = true;

                    for (int i = 0; i < 2; i++)
                    {
                        drawDatTmp2.rot3D.z += 90.0f;
                        DrawPolygonInCameraEx(drawDatTmp2, opt2);
                    }

                    DrawPolygonInCameraEx(drawDatTmp, opt);
                }

            }
        }
    }
}


void GOE_Character_PlayerProjectile::Dead()
{
}


void GOE_Character_PlayerProjectile::DamageAttacker(GAME_OBJECT_DAMAGE_RESULT dr)
{
}

void GOE_Character_PlayerProjectile::DamageReciever(GAME_OBJECT_DAMAGE_RESULT dr)
{
    if (m_type == Type::ContriusShockWave)
    {
        if (dr.damage > 0)
        {
        }
    }
    if (m_type == Type::ContriusBomb)
    {
        if (dr.damage > 0)
        {
            float drdamknock = std::min((float)dr.damage / 10, 30.0f);

            GAME_OBJECT* atkrObj = GetGameObject(dr.atkObjIndex);

            if (atkrObj != nullptr)
            {
                Float2 vec = MkF2(
                    atkrObj->pos.x - m_Obj->pos.x,
                    atkrObj->pos.y - m_Obj->pos.y
                );
                vec = NormalizeVector(vec);

                GameObjectMoveRequest(m_Obj,
                    36,
                    MkF3(
                        m_Obj->pos.x + -vec.x * ONE_BLOCK * drdamknock,
                        m_Obj->pos.y + -vec.y * ONE_BLOCK * drdamknock,
                        m_Obj->pos.z
                    ),
                    EasingOutQuad
                );
            }
        }
    }
}

void GOE_Character_PlayerProjectile::BlockHit(GAME_OBJECT_POSMOD_RESULT pmr)
{
    if (m_type == Type::ContriusShockWave)
    {
        DeleteGameObject(m_Obj);
    }
}

GOE_Character_PlayerProjectile* GOE_Character_PlayerProjectile::CreateProjectile(
    Type type,
    Float3 startPos,
    Float3 startVel
)
{
    GOE_Character_PlayerProjectile* goe_pp = nullptr;
    {
        GAME_OBJECT co = GetGameObjectPrefab(GAME_OBJECT_TYPE_DUMMY);
        co.pos = startPos;
        co.vel = startVel;

        GAME_OBJECT* coCrd = CreateGameObject(co);
        if (coCrd != nullptr)
        {
            goe_pp = new GOE_Character_PlayerProjectile(coCrd);

            if (goe_pp != nullptr)
            { //初期化イベント
                goe_pp->SetType(type);

                coCrd->events.source = goe_pp;
                coCrd->events.source->Init();
            }
        }
    }
    return goe_pp;
}

