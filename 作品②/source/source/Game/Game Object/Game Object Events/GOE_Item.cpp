// ===================================================
// GOE_Item.cpp オブジェクトイベントエフェクト
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

#include "../../Game Events/GE_Gaya.h"

#include "GOE_Effect.h"
#include "GOE_Character_Player.h"
#include "GOE_Character_Enemy.h"
#include "GOE_Item.h"

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
void GOE_Item::Init()
{
    m_Tex_Shadow = LoadTexData("TEX/effect/shadow").textureId;
    m_Tex_Eff_loop = LoadTexData("TEX/effect/Hit_star_01").textureId;
    m_Tex_Heal = LoadTexData("TEX/item/heal").textureId;
    m_Model_Heal = GetModelDataIndex("MODEL/item/heal");
}

void GOE_Item::FirstFrame()
{
    //if (m_type == Type::)
    //{
    //}
}

// ===================================================
// オブジェクトの終了
// ===================================================
void GOE_Item::Uninit()
{
    UnloadTexData(m_Tex_Heal);

    UnloadTexData(m_Tex_Eff_loop);
    UnloadTexData(m_Tex_Shadow);
}

// ===================================================
// オブジェクトの更新
// ===================================================
void GOE_Item::Update()
{
    if (m_type == Type::Heal)
    {
    }
}

// ===================================================
// オブジェクトの更新(アクティブ)
// ===================================================
void GOE_Item::UpdateActive()
{
    if (GameManagerIsMovieFlag())
    { //ムービー中は止める
        return;
    }
    if (m_type == Type::Heal)
    {
        if (m_Obj->status.deadFlag)
        {
            if (m_gayaid.size() > 0)
            {
                GE_GayaCancelRequest(m_gayaid);
            }
            DeleteGameObject(m_Obj);
        }
        else
        {
            float indis = ONE_BLOCK * 4.0f
                * ((m_Obj->scale.x + m_Obj->scale.y + m_Obj->scale.z) / 3);
            std::map<int, GAME_OBJECT*> objs 
                = GetGameObjectFindByDistance(
                    m_Obj->pos,
                    indis
            );
            for (int i = 0; i < objs.size(); i++)
            {
                if (objs[i] != nullptr
                    && objs[i]->tags.size() > 0
                    && objs[i]->tags[0].find("PLAYER_") == 0)
                {
                    GOE_Character_Player* goe =
                        dynamic_cast<GOE_Character_Player*>(objs[i]->events.source);  // ダウンキャスト

                    if (goe != nullptr)
                    { //
                        GameObjectHitStop(objs[i], 10);
                        GameObjectHitStop(m_Obj,   10);
                        m_Obj->status.deadFlag = true;

                        objs[i]->status.hp += (int)(objs[i]->status.hpMax * 1.0f);
                        if (objs[i]->status.hp > objs[i]->status.hpMax)
                        {
                            objs[i]->status.hp = objs[i]->status.hpMax;
                        }
                        goe->HealingFlag();

                        SoundEffectPlayRequest(
                            GetSoundData("SOUND/SE/player/heal_mid"),
                            0.8f, 0.0f, 1.0f);

                        CameraFlashRequestAll(
                            30,
                            EasingInOutSine,
                            MkF4(0.1f, 8.0f, 0.1f, 0.3f),
                            DPD_SHADER_OPT_BLEND_STATE_ADDCOLOR
                        );

                        CameraShakeRequestAll(
                            CAMERA_SHAKE_LARGE_FRAME,
                            CAMERA_SHAKE_MIDIUM_POWER * 0.2f,
                            CAMERA_SHAKE_MIDIUM_POWER * 1.2f
                        );

                        {
                            GAME_OBJECT* go = GO_CreateEffect(GO_EFFECT_TYPE_SCOREEFF,
                                MkF3(
                                    objs[i]->pos.x,
                                    objs[i]->pos.y,
                                    objs[i]->pos.z + -ONE_BLOCK * 2.0f
                                )
                            );
                            if (go != nullptr)
                            {
                                go->scale = MkF3(
                                    20.0f,
                                    20.0f,
                                    20.0f
                                );
                                GameObjectSetLifeTime(go, 50);
                            }
                        }

                        break;
                    }
                }
            }
        }
    }
}


void GOE_Item::Draw()
{
    if (m_Obj->lifetimeFrame > 0 
        && m_Obj->lifetimeFrame < FRAME_ONE_SECOND * 3.0f
    )
    {
        if (m_Obj->frameActive % 6 < 3)
        {
            return;
        }
    }

    if (m_type == Type::Heal)
    {
        if (IsInStage())
        {
            DRAW_SPRITE_DAT dsdat = {};
            dsdat.x = m_Obj->pos.x;
            dsdat.y = m_Obj->pos.y;
            dsdat.z = -8.0f;
            //dsdat.z = -0.005f;
            dsdat.w = 400.0f * m_Obj->scale.x;
            dsdat.h = 400.0f * m_Obj->scale.y;

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
                200.0f,
                -200.0f,
                200.0f
            );

            drawDatTmp.rot = 0.0f;

            drawDatTmp.rot3D = MkF3(
                90.0f,
                0.0f + -20.0f,
                0.0f
            );

            drawDatTmp.color = MkF4(
                1.0f + 0.5f,
                1.0f + 0.1f,
                1.0f + 0.2f,
                1.0f
            );

            drawDatTmp.texNo = m_Tex_Heal;
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
            drawDatTmp.startPos3D.z += -abs(drawDatTmp.size.y) / 2.0f;

            drawDatTmp.startRot3D = MkF3(
                0.0f,
                0.0f,
                0.0f + 2.0f * m_Obj->frameActive
            );

            drawDatTmp.startScale = MkF2(1.0f, 1.0f);
            drawDatTmp.startScale3D = MkF3(1.0f, 1.0f, 1.0f);

            drawDatTmp.modelNo = m_Model_Heal;

            drawDatTmp.shaderOpt.outline = DPD_SHADER_OPT_OUTLINE_NORMAL;
            drawDatTmp.shaderOpt.outlineLayer = 0;

            DRAW_POLYGON_IN_CAMERA_EX_OPTION opt = {};
            opt.shaderType = DPD_SHADER_TYPE_METAL;
            opt.reverse = false;
            opt.fixed = false;
            opt.enableShade = true;
            opt.billboard = false;

            opt.priority = DRAW_POLYGON_PRIORITY_NORMAL;


            DrawPolygonInCameraEx(drawDatTmp, opt);
        }
        {
            DRAW_POLYGON_DAT drawDatTmp = {};
            drawDatTmp.size = MkF3(
                3600.0f,
                3600.0f,
                1.0f);

            drawDatTmp.rot = 0.0f;

            drawDatTmp.rot3D = MkF3(0.0f, 0.0f, 0.0f);
            drawDatTmp.color = MkF4(1.0f, 1.0f, 1.0f,
                //0.5f + 0.4f * (sin((float)m_Obj->frameActive / 16) / 2.0f + 0.5f)
                0.2f + 0.6f * (sin((float)m_Obj->frameActive / 10) / 2.0f + 0.5f)
                * 0.4f
            );
            drawDatTmp.size.x *= 0.6f + 0.4f * (sin((float)m_Obj->frameActive / 20) / 2.0f + 0.5f);
            drawDatTmp.size.y = drawDatTmp.size.x;

            drawDatTmp.texNo = m_Tex_Eff_loop;
            drawDatTmp.loadTexType = LOADTEXTURETYPE_MAIN;

            drawDatTmp.texUV = AnimationUV(
                std::min(((int)m_Obj->frameActive / 8), 4),
                //4 * (sin((float)m_Obj->frameActive / 8) / 2.0f + 0.5f),
                4, 4);
            drawDatTmp.texWH = { 1.0f / 4, 1.0f / 4 };

            drawDatTmp.startPos3D = m_Obj->pos;
            drawDatTmp.startPos3D.z += -150.0f;

            drawDatTmp.startRot3D = MkF3(0.0f, 0.0f, 0.0f);

            drawDatTmp.startScale = MkF2(1.0f, 1.0f);
            drawDatTmp.startScale3D = MkF3(1.0f, 1.0f, 1.0f);

            drawDatTmp.modelNo = 0;
            drawDatTmp.vertex.type = DRAW_POLYGON_TYPE_QUAD;

            drawDatTmp.shaderOpt.outline = DPD_SHADER_OPT_OUTLINE_NONE;
            drawDatTmp.shaderOpt.bloom = DPD_SHADER_OPT_BLOOM_NONE;

            drawDatTmp.shaderOpt.blendState = DPD_SHADER_OPT_BLEND_STATE_ADDCOLOR;

            DRAW_POLYGON_IN_CAMERA_EX_OPTION opt = {};
            opt.reverse = false;
            opt.fixed = false;
            opt.enableShade = false;
            opt.billboard = true;
            opt.priority = DRAW_POLYGON_PRIORITY_NML_u1;
            opt.depthBuffIgnore = true;

            DrawPolygonInCameraEx(drawDatTmp, opt);
        }
    }
}


void GOE_Item::Dead()
{
}


void GOE_Item::DamageAttacker(GAME_OBJECT_DAMAGE_RESULT dr)
{
}

void GOE_Item::DamageReciever(GAME_OBJECT_DAMAGE_RESULT dr)
{
}

void GOE_Item::BlockHit(GAME_OBJECT_POSMOD_RESULT pmr)
{
}

GOE_Item* GOE_Item::CreateItem(
    Type type,
    Float3 startPos
)
{
    GOE_Item* goe_pp = nullptr;
    {
        GAME_OBJECT co = GetGameObjectPrefab(GAME_OBJECT_TYPE_DUMMY);
        co.pos = startPos;

        GAME_OBJECT* coCrd = CreateGameObject(co);
        if (coCrd != nullptr)
        {
            goe_pp = new GOE_Item(coCrd);

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

