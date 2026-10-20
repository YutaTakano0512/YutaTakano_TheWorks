// ===================================================
// GOE_ItemTransporter.cpp オブジェクトイベントエフェクト
// 
// hara sougo  2025/01/12
// ===================================================
#include "../../../game_main.h"
#include "../../Sprite/sprite.h"
#include "../../Polygon/modelData.h"
#include "../../Texture/texture.h"
#include "../../../Game/Entities/Block/block.h"
#include "../../Systems/collision.h"
#include "../../Systems/easing.h"

#include "../gameObject.h"
#include "../gameObjectGraph.h"
#include "../gameObjectPrefab.h"

#include "../../Systems/ui.h"
#include "../../Managers/gameManager.h"
#include "../../Managers/soundManager.h"

#include "../../../Scenes/SCENE_GAME.h"
#include "../../Game Stage/Stage_Base.h"

#include "GOE_Effect.h"
#include "GOE_Character_Player.h"
#include "GOE_Character_Enemy.h"
#include "GOE_Item.h"
#include "GOE_ItemTransporter.h"

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
void GOE_ItemTransporter::Init()
{
    m_Tex_Shadow = LoadTexData("TEX/effect/shadow").textureId;
    m_Tex_Propeller = LoadTexData("TEX/object/transporter/propeller").textureId;
    m_Tex_Main = LoadTexData("TEX/object/transporter/main").textureId;
    m_Model_Main = GetModelDataIndex("MODEL/object/transporter");
}

void GOE_ItemTransporter::FirstFrame()
{
    //if (m_type == Type::)
    //{
    //}
}

// ===================================================
// オブジェクトの終了
// ===================================================
void GOE_ItemTransporter::Uninit()
{
    UnloadTexData(m_Tex_Propeller);
    UnloadTexData(m_Tex_Main);
    UnloadTexData(m_Tex_Shadow);
}

// ===================================================
// オブジェクトの更新
// ===================================================
void GOE_ItemTransporter::Update()
{
}

// ===================================================
// オブジェクトの更新(アクティブ)
// ===================================================
void GOE_ItemTransporter::UpdateActive()
{
    if (GameManagerIsMovieFlag())
    { //ムービー中は止める
        return;
    }

    if (m_ItemTransportFlag)
    {
        if (m_ItemPurgeFlag)
        {
            if (m_Obj->moveRequ.frame <= 0)
            {
                DeleteGameObject(m_Obj);
                return;
            }
        }
        else
        {
            GAME_OBJECT* item = GetGameObjectFindByIdentifier(m_ObjItem);
            GAME_OBJECT* target = GetGameObjectFindByIdentifier(m_ObjTarget);
            if (item == nullptr || target == nullptr)
            {
                m_ItemPurgeFlag = true;
                GameObjectMoveRequest(m_Obj,
                    180,
                    MkF3(
                        0.0f,
                        ONE_BLOCK * 200.0f,
                        m_Obj->pos.z
                    ),
                    EasingInExpo
                );
            }
            else
            {
                if (m_TransportStartDelay > 0)
                {
                    m_TransportStartDelay--;
                }
                else
                {
                    Float3 targetPos = target->pos;
                    {
                        Stage_Base* stg = GetCurrentStageSceneGame();
                        if (stg != nullptr)
                        {
                            Int2 stgsize = stg->GetStageSize();

                            Float2 size = {};
                            size.x = BLOCK_WIDTH * stgsize.x * 0.75f - ITEM_PURGE_RANGE * 2;
                            size.y = BLOCK_HIGHT * stgsize.y * 0.75f - ITEM_PURGE_RANGE * 2;

                            if (targetPos.x < size.x / -2)
                            {
                                targetPos.x = size.x / -2;
                            }
                            if (targetPos.x > size.x / 2)
                            {
                                targetPos.x = size.x / 2;
                            }

                            if (targetPos.y < size.y / -2)
                            {
                                targetPos.y = size.y / -2;
                            }
                            if (targetPos.y > size.y / 2)
                            {
                                targetPos.y = size.y / 2;
                            }
                        }
                    }

                    float targetDistance = CalculateDistance(
                        Float3ToFloat2(m_Obj->pos),
                        Float3ToFloat2(targetPos)
                    );

                    Float2 addpos = {
                        (targetPos.x - m_Obj->pos.x) / std::max(targetDistance, 1.0f),
                        (targetPos.y - m_Obj->pos.y) / std::max(targetDistance, 1.0f),
                    };

                    float speed = 0.000001f + 0.999999f * (std::min(targetDistance / (ONE_BLOCK * 50.0f), 1.0f));
                    m_Obj->pos.x += addpos.x * 250.0f * speed;
                    m_Obj->pos.y += addpos.y * 250.0f * speed;

                    item->pos = m_Obj->pos;
                    item->pos.z += ONE_BLOCK * 3.0f;

                    targetDistance = CalculateDistance(
                        Float3ToFloat2(m_Obj->pos),
                        Float3ToFloat2(targetPos)
                    );

                    if (targetDistance <= ITEM_PURGE_RANGE)
                    {
                        m_ItemPurgeFlag = true;
                        GameObjectMoveRequest(item,
                            60,
                            MkF3(
                                item->pos.x,
                                item->pos.y,
                                0.0f
                            ),
                            EasingInSine
                        );
                        GameObjectSetLifeTime(item, 
                            FRAME_ONE_SECOND * 20
                        );

                        GameObjectMoveRequest(m_Obj,
                            300,
                            MkF3(
                                ONE_BLOCK * 200.0f,
                                m_Obj->pos.y,
                                m_Obj->pos.z
                            ),
                            EasingInSine
                        );
                    }
                }
            }
        }
    }
}


void GOE_ItemTransporter::Draw()
{
    float sizeMain = 150.0f * (m_Obj->scale.x + m_Obj->scale.y + m_Obj->scale.z) / 3.0f;
    if (IsInStage())
    {
        DRAW_SPRITE_DAT dsdat = {};
        dsdat.x = m_Obj->pos.x;
        dsdat.y = m_Obj->pos.y;
        dsdat.z = -8.0f;
        //dsdat.z = -0.005f;
        dsdat.w = sizeMain * 3.0f;
        dsdat.h = sizeMain * 3.0f;

        {
            float mo = 1.0f - std::max(std::min(abs(m_Obj->pos.z) / (ONE_BLOCK * 80.0f), 1.0f), 0.0f);
            dsdat.w *= mo;
            dsdat.h *= mo;
        }

        dsdat.rot = 0.0f;
        dsdat.color = { 1.0f, 1.0f, 1.0f, 0.5f };

        dsdat.texNo = m_Tex_Shadow;
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

    {
        DRAW_POLYGON_DAT drawDatTmp = {};
        drawDatTmp.size = MkF3(
            sizeMain,
            -sizeMain,
            sizeMain
        );

        drawDatTmp.rot = 0.0f;

        drawDatTmp.rot3D = MkF3(
            90.0f,
            0.0f,
            0.0f
        );

        drawDatTmp.color = MkF4(
            1.0f,
            1.0f,
            1.0f,
            1.0f
        );

        drawDatTmp.texNo = m_Tex_Main;
        drawDatTmp.loadTexType = LOADTEXTURETYPE_MAIN;

        drawDatTmp.texUV = {
            0.0f,
            0.0f
        };
        drawDatTmp.texWH = {
            1.0f,
            1.0f
        };

        drawDatTmp.startPos3D = m_Obj->pos;

        drawDatTmp.startRot3D = MkF3(
            0.0f,
            0.0f,
            0.0f
        );

        drawDatTmp.startScale = MkF2(1.0f, 1.0f);
        drawDatTmp.startScale3D = MkF3(1.0f, 1.0f, 1.0f);

        drawDatTmp.modelNo = m_Model_Main;

        drawDatTmp.shaderOpt.outline = DPD_SHADER_OPT_OUTLINE_NORMAL;
        drawDatTmp.shaderOpt.outlineLayer = 1;

        DRAW_POLYGON_IN_CAMERA_EX_OPTION opt = {};
        opt.shaderType = DPD_SHADER_TYPE_METAL_ENEMY_2;
        opt.reverse = false;
        opt.fixed = false;
        opt.enableShade = true;
        opt.billboard = false;

        opt.priority = DRAW_POLYGON_PRIORITY_NORMAL;


        DrawPolygonInCameraEx(drawDatTmp, opt);
    }
    for(int i = 0; i < 4; i++)
    {
        DRAW_POLYGON_DAT drawDatTmp = {};
        drawDatTmp.size = MkF3(
            sizeMain * 1.625f,
            sizeMain * 1.625f,
            1.0f
        );

        drawDatTmp.rot = +5.0f * m_Obj->frameActive;

        drawDatTmp.color = MkF4(
            1.0f,
            1.0f,
            1.0f,
            1.0f
        );

        drawDatTmp.texNo = m_Tex_Propeller;
        drawDatTmp.loadTexType = LOADTEXTURETYPE_MAIN;

        drawDatTmp.texUV = {
            0.0f,
            0.0f
        };
        drawDatTmp.texWH = {
            1.0f,
            1.0f
        };

        drawDatTmp.startPos3D = m_Obj->pos;
        drawDatTmp.startPos3D.x += +1.65f * sizeMain * sin(2.0f * SYS_GENERAL_PI / 4 * i + SYS_GENERAL_PI / 4);
        drawDatTmp.startPos3D.y += +1.65f * sizeMain * cos(2.0f * SYS_GENERAL_PI / 4 * i + SYS_GENERAL_PI / 4);
        drawDatTmp.startPos3D.z += -0.6f  * sizeMain;


        drawDatTmp.startRot3D = MkF3(
            0.0f,
            0.0f,
            0.0f
        );

        drawDatTmp.startScale = MkF2(1.0f, 1.0f);
        drawDatTmp.startScale3D = MkF3(1.0f, 1.0f, 1.0f);

        drawDatTmp.modelNo = 0;

        DRAW_POLYGON_IN_CAMERA_EX_OPTION opt = {};
        opt.reverse = false;
        opt.fixed = false;
        opt.enableShade = true;
        opt.billboard = false;

        opt.priority = DRAW_POLYGON_PRIORITY_NORMAL;


        DrawPolygonInCameraEx(drawDatTmp, opt);
    }
}


void GOE_ItemTransporter::Dead()
{
}


void GOE_ItemTransporter::DamageAttacker(GAME_OBJECT_DAMAGE_RESULT dr)
{
}

void GOE_ItemTransporter::DamageReciever(GAME_OBJECT_DAMAGE_RESULT dr)
{
}

void GOE_ItemTransporter::BlockHit(GAME_OBJECT_POSMOD_RESULT pmr)
{
}

void GOE_ItemTransporter::StartTransport(int delay)
{
    m_ItemTransportFlag = true;
    m_TransportStartDelay = delay;
}

GOE_ItemTransporter* GOE_ItemTransporter::Create(
    Float3 startPos,
    GAME_OBJECT* item,
    GAME_OBJECT* target
)
{
    GOE_ItemTransporter* goe_pp = nullptr;
    {
        GAME_OBJECT co = GetGameObjectPrefab(GAME_OBJECT_TYPE_DUMMY);
        co.pos = startPos;

        GAME_OBJECT* coCrd = CreateGameObject(co);
        if (coCrd != nullptr)
        {
            goe_pp = new GOE_ItemTransporter(coCrd);

            if (goe_pp != nullptr)
            { //初期化イベント
                goe_pp->m_ObjItem = GetIdentifierGameObject(item);
                goe_pp->m_ObjTarget = GetIdentifierGameObject(target);

                coCrd->events.source = goe_pp;
                coCrd->events.source->Init();
            }
        }
    }
    return goe_pp;
}

