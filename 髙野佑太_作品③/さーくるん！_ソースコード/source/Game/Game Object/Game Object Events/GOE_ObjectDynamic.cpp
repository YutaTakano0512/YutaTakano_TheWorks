// ===================================================
// GOE_ObjectDynamic.cpp オブジェクトイベントエフェクト
// 
// hara sougo  2025/07/01
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
#include "GOE_ObjectDynamic.h"

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
void GOE_ObjectDynamic::Init()
{
    m_tex_block[0] = LoadTexData("TEX/object/iseki/obj_image/Block");
    m_tex_block[1] = LoadTexData("TEX/object/iseki/obj_image/Daiza");
    m_tex_block[2] = LoadTexData("TEX/object/iseki/obj_image/HasiraS3");
    m_tex_block[3] = LoadTexData("TEX/object/iseki/obj_image/HasiraYoko1");
    m_tex_block[4] = LoadTexData("TEX/object/iseki/obj_image/HasiraYoko2");
    m_Tex_Shadow = LoadTexData("TEX/effect/shadow").textureId;
}

void GOE_ObjectDynamic::FirstFrame()
{
    //if (m_type == Type::)
    //{
    //}
}

// ===================================================
// オブジェクトの終了
// ===================================================
void GOE_ObjectDynamic::Uninit()
{
    for(int i = 0; i < ARRAYSIZE(m_tex_block); i++)
    {
        UnloadTexData(m_tex_block[i]);
    }
    UnloadTexData(m_Tex_Shadow);
}

// ===================================================
// オブジェクトの更新
// ===================================================
void GOE_ObjectDynamic::Update()
{
}

// ===================================================
// オブジェクトの更新(アクティブ)
// ===================================================
void GOE_ObjectDynamic::UpdateActive()
{
    if (m_clearFrame >= 0)
    {
        m_Obj->pos.z += 20.0f;

        m_clearFrame++;

        if (m_clearFrame > FRAME_ONE_SECOND * 1.0f)
        {
            DeleteGameObject(m_Obj);
        }
        return;
    }

    if (GameManagerIsMovieFlag())
    { //ムービー中は止める
        return;
    }

    if (m_isFalling)
    {
        if (!m_Obj->moveRequ.enable)
        {
            m_isFalling = false;

            GAME_OBJECT* ownerObj = GetGameObjectFindByIdentifier(m_ObjOwner);

            //SEを再生
            SoundEffectPlayRequest(GetSoundData("SOUND/SE/etc/gareki"),
                0.5f, 0, 0.9f);

            //SEを再生
            SoundEffectPlayRequest(SOUND_TYPE_SE_9,
                1.6f, 0.0f, 2.6f);
            SoundEffectPlayRequest(
                GetSoundData("SOUND/SE/enemy/attack/Small_Explosion"),
                1.6f, 0, 1.5f);

            CameraShakeRequestAll(
                CAMERA_SHAKE_LARGE_FRAME,
                CAMERA_SHAKE_EXLARGE_POWER
            );

            float length = std::max(
                CalculateDistance({}, m_Obj->collider.capsule.pointA),
                CalculateDistance({}, m_Obj->collider.capsule.pointB)
            ) + m_Obj->collider.capsule.radius * 2;
            {
                Float3 efPos = m_Obj->pos;

                for (int i = 0; i < 7; i++)
                {
                    Float3 efPos2 = efPos;
                    efPos2.x += sin(2.0f * SYS_GENERAL_PI / 5 * i) * length * 2.0f * 0.8f;
                    efPos2.y += cos(2.0f * SYS_GENERAL_PI / 5 * i) * length * 2.0f * 0.8f;

                    GAME_OBJECT* go = GO_CreateEffect(GO_EFFECT_TYPE_SMOKE, efPos2);
                    if (go != nullptr)
                    {
                        //go->graphic.rot3d.x = 90.0f;
                        go->graphic.color = MkF4(
                            0.6f,
                            0.6f,
                            0.6f,
                            1.0f
                        );
                        go->scale = MkF3(
                            m_Obj->collider.capsule.radius / 100.0f * (length / 180.0f) * 0.35f,
                            m_Obj->collider.capsule.radius / 100.0f * (length / 180.0f) * 0.35f,
                            m_Obj->collider.capsule.radius / 100.0f * (length / 180.0f) * 0.35f
                        );
                        GameObjectSetLifeTime(go, 30);
                    }
                }
            }
            {
                Float3 scatmp = MkF3(
                    m_Obj->collider.capsule.radius / 100.0f * (length / 180.0f) * 1.5f,
                    m_Obj->collider.capsule.radius / 100.0f * (length / 180.0f) * 1.5f,
                    m_Obj->collider.capsule.radius / 100.0f * (length / 180.0f) * 1.5f
                );
                Float3 postmp = m_Obj->pos;
                postmp.y += scatmp.y * 0.2f;
                //postmp.z += ;

                GAME_OBJECT* go = GO_CreateEffect(
                    GO_EFFECT_TYPE_HAMMER_SHOCKWAVE,
                    postmp);
                if (go != nullptr)
                {
                    go->scale = scatmp;
                    GameObjectSetLifeTime(go, 32);
                }
            }
            {
                Float3 postmp = m_Obj->pos;

                GAME_OBJECT* go = GO_CreateEffect(
                    GO_EFFECT_TYPE_HAMMER_SHOCKDECAL,
                    postmp);
                if (go != nullptr)
                {
                    go->scale = MkF3(
                        m_Obj->collider.capsule.radius / 100.0f * (length / 180.0f) * 1.2f,
                        m_Obj->collider.capsule.radius / 100.0f * (length / 180.0f) * 1.2f,
                        m_Obj->collider.capsule.radius / 100.0f * (length / 180.0f) * 1.2f
                    );
                    GameObjectSetLifeTime(go, 30);
                }
            }
            m_Obj->collision.rec.enable = true;
            m_Obj->status.guardType = GAME_OBJECT_GUARD_TYPE_NORMAL;


            Character::MOTION_COL_DAT motionColDetails =
                Character::GetMotionColDataFindByName(
                    Character::MOTION_COL_TYPE_ATK,
                    "COL_ATK_OBJECT_DYNAMIC_FALL"
                );

            GAME_OBJECT_COLLISION_ATTACK_CHILD attackColTmp = motionColDetails.atkCol;
            attackColTmp.subjectFamily[GAME_OBJECT_FAMILY_PLAYER] = true;
            attackColTmp.subjectFamily[GAME_OBJECT_FAMILY_ENEMY] = true;

            //判定サイズ
            attackColTmp.size = MkF3(
               length * 1.0f,
               length * 1.0f,
               ONE_BLOCK * 4.0f
            );
            attackColTmp.pos.z = -attackColTmp.size.z / 2;

            if (IsActiveGameObject(ownerObj))
            {
                m_Obj->collision.atk.ownerId = GetIdentifierGameObject(ownerObj);
            }

            //攻撃判定を生成
            GameObjectAttackCollisionAdd(m_Obj, attackColTmp,
                1,
                5
            );
        }
    }

    if (m_hitcooldown > 0)
    {
        m_hitcooldown--;
    }
}


void GOE_ObjectDynamic::Draw()
{
    if (m_isFalling && IsInStage())
    {
        float length = std::max(
                CalculateDistance({}, m_Obj->collider.capsule.pointA),
                CalculateDistance({}, m_Obj->collider.capsule.pointB)
            );

        DRAW_SPRITE_DAT dsdat = {};
        dsdat.x = m_Obj->pos.x;
        dsdat.y = m_Obj->pos.y;
        dsdat.z = -8.0f;
        //dsdat.z = -0.005f;
        dsdat.w = length * m_Obj->scale.x * 4.0f;
        dsdat.h = length * m_Obj->scale.y * 4.0f;
        {
            float mo = 1.0f - std::max(std::min(abs(m_Obj->pos.z) / (ONE_BLOCK * 100.0f), 1.0f), 0.0f);
            dsdat.w *= mo;
            dsdat.h *= mo;
        }

        dsdat.rot = 0.0f;
        dsdat.color = { 1.0f, 1.0f, 1.0f, 0.4f };

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
        shakePos.x += sin(1.2f * m_Obj->hitReaction.frame) * hrModTmp * 300.0f * shakemod;
    }

    DRAW_POLYGON_DAT drawDatBaseTmp = {};

    drawDatBaseTmp.rot = 0.0f;

    drawDatBaseTmp.rot3D = MkF3(0.0f, 0.0f, 0.0f);

    drawDatBaseTmp.color = {
        1.0f,
        1.0f,
        1.0f,
        1.0f
    };

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
    optBaseTmp.priority = DRAW_POLYGON_PRIORITY_NML_d1;
    //optBaseTmp.depthBuffIgnore = true;
    //optBaseTmp.shaderType = DPD_SHADER_TYPE_EFFECT;

    {
        DRAW_POLYGON_DAT drawDatTmp2 = drawDatBaseTmp;
        drawDatTmp2.texNo = 0;
        drawDatTmp2.modelNo = 0;
        //drawDatTmp2.color = {
        //    0.75f,
        //    0.75f,
        //    0.75f,
        //    1.0f
        //};
        drawDatTmp2.color = {
            0.52f,
            0.46f,
            0.56f,
            1.0f
        };
        drawDatTmp2.size = MkF3(
            900.0f * m_Obj->scale.x * 1.0f,
            900.0f * m_Obj->scale.z * 1.0f,
            900.0f * m_Obj->scale.y * 1.0f
        );

        drawDatTmp2.startRot3D.x = 50.0f;
        drawDatTmp2.pos = {};
        drawDatTmp2.pos.z = drawDatTmp2.size.y * -0.2f;
        drawDatTmp2.startPos3D = m_Obj->pos;

        drawDatTmp2.shaderOpt.blendState = DPD_SHADER_OPT_BLEND_STATE_DEFAULT;

        DRAW_POLYGON_IN_CAMERA_EX_OPTION opt2 = optBaseTmp;
        opt2.priority = DRAW_POLYGON_PRIORITY_NORMAL;
        //opt2.priority = DRAW_POLYGON_PRIORITY_NML_u1;
        //opt2.depthBuffIgnore = false;
        //opt2.depthBuffIgnore = true;


        Float3 setvec = NormalizeVector(
            m_Obj->collider.capsule.pointB
            - m_Obj->collider.capsule.pointA
        );

        Float2 setsize = {
            m_Obj->collider.capsule.radius * 2.0f,
            m_Obj->collider.capsule.radius * 1.0f
        };

        float objlength1 = CalculateDistance(
            m_Obj->collider.capsule.pointA,
            m_Obj->collider.capsule.pointB
        );

        float objlength2 = (objlength1
            + m_Obj->collider.capsule.radius * 2);

        Int2 setnumraw = {
            (int)(objlength1 / (setsize.x + 0.0f)),
            (int)(objlength2 / (setsize.y + 0.0f)),
        };

        //setnum = std::max(setnum, 1);
        int setnum = std::max(setnumraw.x, setnumraw.y);

        {
            for (int i = 0; i < setnum; i++)
            {
                Float3 posTmp = m_Obj->collider.capsule.pointA;
                posTmp.x += objlength1 / setnum * setvec.x * i;
                posTmp.y += objlength2 / setnum * setvec.y * i - setvec.y * m_Obj->collider.capsule.radius;

                //Float3 posTmp = m_Obj->collider.capsule.pointA
                //    + setvec * (setsize * setnumraw / setnum * 1.8f) * i 
                //    + setvec * (setsize * setnumraw / setnum * -0.4f);
                //Float3 posTmp = m_Obj->collider.capsule.pointA
                //    + setvec * (setsize - 96.0f + (m_Obj->collider.capsule.radius * 2.0f / setnum)) * i
                //    + setvec * m_Obj->collider.capsule.radius * -4.0f;
                posTmp.z = 0;

                drawDatTmp2.startPos3D = m_Obj->pos + posTmp;

                float randtmp = tan(sin((
                    drawDatTmp2.startPos3D.x * 12.9898
                    + drawDatTmp2.startPos3D.y * 78.233
                    ) * cos((float)m_Obj->createFrame / 672.5f)
                )) * 437.5854f;

                int indextmp = (int)(randtmp * 654.7f) % ARRAYSIZE(m_tex_block);

                drawDatTmp2.texNo = m_tex_block[indextmp].textureId;

                drawDatTmp2.rot = -4.0f + 8.0f * (fmod(randtmp * 46.3f, 100.0f) / 100.0f);
                drawDatTmp2.size = MkF3(
                    setsize.x * m_Obj->scale.x * (2.3f + 0.2f * fmod(randtmp * 99.2f, 100.0f) / 100.0f),
                    setsize.x * m_Obj->scale.z * (2.1f + 0.8f * fmod(randtmp * 91.4f, 100.0f) / 100.0f),
                    setsize.x * m_Obj->scale.y * (2.3f + 0.2f * fmod(randtmp * 94.6f, 100.0f) / 100.0f)
                );
                if (fmod((float)i + randtmp / 9999.9f, 1.0f) < 0.5f)
                {
                    drawDatTmp2.size.x *= -1;
                }

                switch (indextmp)
                {
                case 0:
                    drawDatTmp2.pos.x = drawDatTmp2.size.x * -0.08f;
                    drawDatTmp2.pos.y = drawDatTmp2.size.y * 0.01f;
                    drawDatTmp2.pos.z = drawDatTmp2.size.y * -0.16f;
                    break;
                case 1:
                case 2:
                    drawDatTmp2.size.x *= 0.7f;
                    drawDatTmp2.size.y = abs(drawDatTmp2.size.x);
                    drawDatTmp2.pos.x = 0.0f;
                    drawDatTmp2.pos.y = drawDatTmp2.size.y * 0.01f;
                    drawDatTmp2.pos.z = drawDatTmp2.size.y * -0.19f;
                    break;
                case 3:
                case 4:
                    drawDatTmp2.size.x *= 0.6f;
                    drawDatTmp2.size.y = abs(drawDatTmp2.size.x);
                    drawDatTmp2.pos.x = 0.0f;
                    drawDatTmp2.pos.y = drawDatTmp2.size.y * 0.01f;
                    drawDatTmp2.pos.z = drawDatTmp2.size.y * -0.09f;
                    break;
                default:break;
                }

                drawDatTmp2.startPos3D = drawDatTmp2.startPos3D + shakePos;

                DrawPolygonInCameraEx(drawDatTmp2, opt2);
            }
        }

        //{
        //    drawDatTmp2.size = MkF3(
        //        50.0f,
        //        50.0f,
        //        50.0f
        //    );
        //    drawDatTmp2.texNo = 0;
        //    drawDatTmp2.pos = {};
        //    drawDatTmp2.rot3D.x = 90.0f;
        //    Float3 posTmp = m_Obj->collider.capsule.pointA;

        //    drawDatTmp2.startPos3D = m_Obj->pos + posTmp + shakePos;
        //    DrawPolygonInCameraEx(drawDatTmp2, opt2);
        //}
        //{
        //    drawDatTmp2.size = MkF3(
        //        50.0f,
        //        50.0f,
        //        50.0f
        //    );
        //    drawDatTmp2.texNo = 0;
        //    drawDatTmp2.pos = {};
        //    drawDatTmp2.rot3D.x = 90.0f;
        //    Float3 posTmp = m_Obj->collider.capsule.pointB;

        //    drawDatTmp2.startPos3D = m_Obj->pos + posTmp + shakePos;
        //    DrawPolygonInCameraEx(drawDatTmp2, opt2);
        //}
    }
}


void GOE_ObjectDynamic::Dead()
{
    Destroy();
}


void GOE_ObjectDynamic::DamageAttacker(GAME_OBJECT_DAMAGE_RESULT dr)
{
}

void GOE_ObjectDynamic::DamageReciever(GAME_OBJECT_DAMAGE_RESULT dr)
{
    //SEを再生
    SoundEffectPlayRequest(
        GetSoundData("SOUND/SE/player/attack/atkhit_mid"),
        0.8f, 0.7f);

}

void GOE_ObjectDynamic::BlockHit(GAME_OBJECT_POSMOD_RESULT pmr)
{
}

void GOE_ObjectDynamic::CalcDamage(
    GAME_OBJECT_DAMAGE_RESULT* output,
    const GAME_OBJECT* obj,
    const GAME_OBJECT* objAttacker,
    int damage,
    GAME_OBJECT_COLLISION_ATTACK_CHILD colAtk,
    GAME_OBJECT_COLLISION_RECIEVE_CHILD colRec
)
{
    GAME_OBJECT* ownerObj = GetGameObjectFindByIdentifier(m_ObjOwner);
    if (objAttacker == ownerObj)
    {
        output->damage *= 10.0f; //ダメージを10倍にする
    }
}


void GOE_ObjectDynamic::PlayerHit(GOE_Character_Player* player)
{
    if (m_clearFrame >= 0)
    {
        return;
    }

    if (m_hitcooldown > 0)
    {
        return;
    }
    //m_hitcooldown = 3;
    m_hitcooldown = 30;

    Character::MOTION_COL_DAT motionColDetails =
        Character::GetMotionColDataFindByName(
            Character::MOTION_COL_TYPE_ATK,
            "COL_ATK_OBJECT_DYNAMIC_HIT"
        );
    motionColDetails.atkCol.hitStopAttacker = 0;
    motionColDetails.atkCol.hitStopReciever = 0;
    motionColDetails.atkCol.power = m_Obj->status.hpMax / 3 + 1;

    //有効なくらい判定を探す
    GAME_OBJECT_COLLISION_RECIEVE_CHILD colRec = {};
    for (int j = 0; j < GAME_OBJECT_COLLISION_NUM_MAX; j++)
    {
        if (m_Obj->collision.rec.collision[j].enable)
        {
            colRec = m_Obj->collision.rec.collision[j];
            break;
        }
    }

    if (colRec.enable)
    { //判定が有効なら処理する

        GameObjectDamageCollisionHit(m_Obj, player->GetGameObj(),
            motionColDetails.atkCol,
            colRec
        );
    }

    //Destroy();
}

void GOE_ObjectDynamic::Destroy()
{
    if (m_clearFrame >= 0)
    {
        return;
    }

    ////SEを再生
    //SoundEffectPlayRequest(GetSoundData("SOUND/SE/etc/kuzureru"),
    //    0.7f, 0, 1.2f);

    //SEを再生
    SoundEffectPlayRequest(SOUND_TYPE_SE_9,
        2.0f, 0.0f, 2.4f);
    SoundEffectPlayRequest(
        GetSoundData("SOUND/SE/enemy/attack/Small_Explosion"),
        2.0f, 0, 1.7f);
    ////SEを再生
    //SoundEffectPlayRequest(GetSoundData("SOUND/SE/etc/zusyan"),
    //    0.7f, 0, 2.0f);


    float length = std::max(
        CalculateDistance({}, m_Obj->collider.capsule.pointA),
        CalculateDistance({}, m_Obj->collider.capsule.pointB)
    );
    {
        Float3 efPos = m_Obj->pos;

        for (int i = 0; i < 7; i++)
        {
            Float3 efPos2 = efPos;
            efPos2.x += sin(2.0f * SYS_GENERAL_PI / 5 * i) * length * 2.0f * 0.6f;
            efPos2.y += cos(2.0f * SYS_GENERAL_PI / 5 * i) * length * 2.0f * 0.6f;

            GAME_OBJECT* go = GO_CreateEffect(GO_EFFECT_TYPE_SMOKE, efPos2);
            if (go != nullptr)
            {
                //go->graphic.rot3d.x = 90.0f;
                go->graphic.color = MkF4(
                    0.6f,
                    0.6f,
                    0.6f,
                    1.0f
                );
                go->scale = MkF3(
                    m_Obj->collider.capsule.radius / 100.0f * (length / 180.0f) * 1.0f,
                    m_Obj->collider.capsule.radius / 100.0f * (length / 180.0f) * 1.0f,
                    m_Obj->collider.capsule.radius / 100.0f * (length / 180.0f) * 1.0f
                );
                GameObjectSetLifeTime(go, 30);
            }
        }
    }
    {
        Float3 efPos = m_Obj->pos;

        for (int i = 0; i < 1; i++)
        {
            GAME_OBJECT* go = GO_CreateEffect(GO_EFFECT_TYPE_EXPLOSION_4, efPos);
            if (go != nullptr)
            {
                go->scale = MkF3(
                    m_Obj->collider.capsule.radius / 100.0f * (length / 180.0f) * 1.75f,
                    m_Obj->collider.capsule.radius / 100.0f * (length / 180.0f) * 1.75f,
                    m_Obj->collider.capsule.radius / 100.0f * (length / 180.0f) * 1.75f
                );
                GameObjectSetLifeTime(go, 50);
            }
        }
    }

    DeleteGameObject(m_Obj);
}
void GOE_ObjectDynamic::Clear()
{
    //m_Obj->collider.type = GAME_OBJECT_COLLIDER_TYPE_NONE;

    m_Obj->collision.rec.enable = false;
    m_Obj->status.guardType = GAME_OBJECT_GUARD_TYPE_INV;

    m_Obj->moveRequ.enable = false;

    m_clearFrame = 0;
}


void GOE_ObjectDynamic::StartFall(int inframe)
{
    if (m_clearFrame >= 0)
    {
        return;
    }

    //SEを再生
    SoundEffectPlayRequest(GetSoundData("SOUND/SE/etc/kuzureru"),
        0.6f, 0, 0.8f);
    CameraShakeRequestAll(
        CAMERA_SHAKE_EXLARGE_FRAME * 3.0f,
        CAMERA_SHAKE_MIDIUM_POWER
    );


    GameObjectMoveRequest(m_Obj,
        inframe,
        MkF3(
            m_Obj->pos.x,
            m_Obj->pos.y,
            0.0f
        ),
        EasingInQuad
    );

    m_Obj->collision.rec.enable = false;
    m_Obj->status.guardType = GAME_OBJECT_GUARD_TYPE_INV;

    m_isFalling = true;
}

void GOE_ObjectDynamic::SetOwner(GAME_OBJECT_IDENTIFIER id)
{
    m_ObjOwner = id;
}



GOE_ObjectDynamic* GOE_ObjectDynamic::CreateObjectDynamic(
    Type type,
    Float3 startPos,
    const GAME_OBJECT_COLLIDER& collider
)
{
    GOE_ObjectDynamic* goe_pp = nullptr;
    {
        GAME_OBJECT co = GetGameObjectPrefab(GAME_OBJECT_TYPE_DUMMY);
        co.pos = startPos;
        co.collider = collider;

        co.status.hpMax = 100;
        co.status.hp = co.status.hpMax;
        co.status.guardType = GAME_OBJECT_GUARD_TYPE_NORMAL;

        co.family[GAME_OBJECT_FAMILY_GIMIK] = true;

        co.collision.rec.enable = true;
        co.collision.rec.collision[0].enable = true;

        float length = CalculateDistance({}, collider.capsule.pointA) 
            + collider.capsule.radius * 2;
        co.collision.rec.collision[0].size = {
            length,
            length,
            ONE_BLOCK * 4.0f,
        };
        co.collision.rec.collision[0].pos = {};
        co.collision.rec.collision[0].pos.z =
            -co.collision.rec.collision[0].size.z / 2;
        co.collision.rec.collision[0].guardMod = 1.0f;
        co.collision.rec.collision[0].subjectFamily[GAME_OBJECT_FAMILY_ALL] = true;


        GAME_OBJECT* coCrd = CreateGameObject(co);
        if (coCrd != nullptr)
        {
            goe_pp = new GOE_ObjectDynamic(coCrd);

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

