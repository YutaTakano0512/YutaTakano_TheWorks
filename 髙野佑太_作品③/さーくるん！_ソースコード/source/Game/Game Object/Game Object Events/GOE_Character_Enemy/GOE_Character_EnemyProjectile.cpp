// ===================================================
// GOE_Character_EnemyProjectile.cpp オブジェクトイベントエフェクト
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
#include "GOE_Character_EnemyProjectile.h"
#include "../GOE_ObjectDynamic.h"

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
void GOE_Character_EnemyProjectile::Init()
{
	m_TexId_AreaEnemy = LoadTexData("TEX/effect/area_enemy").textureId;

    m_Tex_Beam1 = LoadTexData("TEX/effect/beam/sozai_01").textureId;
    m_Tex_Beam2 = LoadTexData("TEX/effect/beam/sozai_02").textureId;
    m_Tex_Beam3 = LoadTexData("TEX/effect/beam/sozai_03").textureId;
    m_Tex_Beam = LoadTexData("TEX/effect/beam/sozai_02").textureId;
    m_Tex_Beambg = LoadTexData("TEX/effect/barrier/tex_br_wave").textureId;
    m_Model_Beam = GetModelDataIndex("MODEL/beam_01");
    m_Model_Beam_Misile = GetModelDataIndex("MODEL/beam_misile02");

    m_Tex_EnergyBall = LoadTexData("TEX/effect/tex_energy_01").textureId;

    m_Model_Fire_Misile = GetModelDataIndex("MODEL/Re_misile_fire");
    m_Tex_Misile_Fire_Model = LoadTexData("TEX/enemy/misile/misile_fire2").textureId;
    m_Model_Misile = GetModelDataIndex("MODEL/Re_misile_01");
    m_Tex_Misile_Model = LoadTexData("TEX/enemy/misile/misile_color").textureId;

    m_Tex_BladeWave = LoadTexData("TEX/effect/boss_sl_front").textureId;
    m_Tex_BladeWaveNew = LoadTexData("TEX/effect/boss_sl_jet").textureId;

    m_Tex_AttackArea = LoadTexData("TEX/effect/tex_ex_attack").textureId;
    m_Tex_Wave1 = LoadTexData("TEX/effect/tex_ink_01").textureId;
    //m_Tex_Wave = LoadTexData("TEX/effect/kakopon/tex_add_01").textureId;
    //m_Tex_Wave2 = LoadTexData("TEX/effect/eff_wave_02").textureId;
    m_Model_Wave = GetModelDataIndex("MODEL/effect/legatos_wave01");

    m_Tex_EfBuff_Flow = LoadTexData("TEX/effect/buff_flow").textureId;
    m_Tex_EfBuff_Flare = LoadTexData("TEX/effect/buff_flare").textureId;

    m_Tex_cursor_000 = LoadTexData("TEX/ui/in_game_master/cursor/cursor_000").textureId;
    m_Tex_cursor_010 = LoadTexData("TEX/ui/in_game_master/cursor/cursor_010").textureId;
    m_Tex_cursor_020 = LoadTexData("TEX/ui/in_game_master/cursor/cursor_020").textureId;

    m_Tex_Shadow = LoadTexData("TEX/effect/shadow").textureId;
	//m_Obj->graphic.hidden = true;


    m_Tex_ShockWaveSprite = LoadTexData("TEX/effect/Ps_tex_sprite").textureId;
    m_Tex_ShockWaveFlow = LoadTexData("TEX/effect/Ps_tex_mesh").textureId;
    m_Tex_ShockWaveSprite_Gray = LoadTexData("TEX/effect/Ps_tex_sprite_gray").textureId;
    m_Tex_ShockWaveFlow_Gray = LoadTexData("TEX/effect/Ps_tex_mesh_gray").textureId;
    m_Model_ShockWaveFlow = GetModelDataIndex("MODEL/beam_01");

    m_Tex_EbBomb_ShockWave = LoadTexData("TEX/effect/kakopon/general/Glaytest").textureId;
    m_Model_EbBomb_ShockWave = GetModelDataIndex("MODEL/effect/shockwave");

	m_InitVel = NormalizeVector(m_Obj->vel);

    if (m_type == Type::Beam)
    {
        //m_Obj->size.x = ONE_BLOCK * 0.01f;
        //m_Obj->size.y = ONE_BLOCK * 0.01f;
    }
    if (m_type == Type::ExplosionShockWave)
    {
        //m_Obj->size.x = ONE_BLOCK * 0.01f;
        //m_Obj->size.y = ONE_BLOCK * 0.01f;
    }
}

void GOE_Character_EnemyProjectile::FirstFrame()
{
    if (m_type == Type::Beam)
    {
        m_Obj->blockCollisionIgnore = true;
    }
    if (m_type == Type::ExplosionShockWave)
    {
        m_Obj->blockCollisionIgnore = true;
    }

    if (m_type == Type::Granade)
    {
        
        GameObjectMoveRequest(m_Obj, 60,
            MkF3(
                m_Obj->pos.x,
                m_Obj->pos.y,
                m_Obj->pos.z + BLOCK_WIDTH * -60.0f
            ),
            EasingOutSine
        );
        m_Init = true;

        //{
        //    Character::MOTION_COL_DAT motionColDetails =
        //        Character::GetMotionColDataFindByName(
        //            Character::MOTION_COL_TYPE_ATK,
        //            ATK_COL_NAME_0
        //        );

        //    GAME_OBJECT_COLLISION_ATTACK_CHILD attackColTmp = motionColDetails.atkCol;

        //    attackColTmp.subjectFamily[GAME_OBJECT_FAMILY_PLAYER] = true;

        //    attackColTmp.size.x = BLOCK_WIDTH * 3.0f;
        //    attackColTmp.size.y = BLOCK_HIGHT * 3.0f;
        //    attackColTmp.size.z = BLOCK_HIGHT * 3.0f;
        //    //attackColTmp.pos.y = -attackColTmp.size.y / 2;

        //    attackColTmp.power = 20;
        //    attackColTmp.hitStopAttacker = 4;
        //    attackColTmp.hitStopReciever = 35;

        //    //攻撃判定を生成
        //    GameObjectAttackCollisionAdd(m_Obj, attackColTmp,
        //        0,
        //        180
        //    );
        //}
        GameObjectSetLifeTime(m_Obj, 60 * 10 + m_Obj->freeInt);
    }
	//m_Obj->freeInt
}

// ===================================================
// オブジェクトの終了
// ===================================================
void GOE_Character_EnemyProjectile::Uninit()
{
    //UnloadTexData(m_Tex_Wave2);
    UnloadTexData(m_Tex_Wave1);

    UnloadTexData(m_Tex_EnergyBall);
    
    UnloadTexData(m_Tex_Misile_Fire_Model);
    UnloadTexData(m_Tex_Misile_Model);

    UnloadTexData(m_Tex_AttackArea);
    UnloadTexData(m_Tex_BladeWave);
    UnloadTexData(m_Tex_BladeWaveNew);

    UnloadTexData(m_Tex_Beam);
    UnloadTexData(m_Tex_Beam1);
    UnloadTexData(m_Tex_Beam2);
    UnloadTexData(m_Tex_Beam3);
    UnloadTexData(m_Tex_Beambg);
    
    UnloadTexData(m_Tex_EfBuff_Flow);
    UnloadTexData(m_Tex_EfBuff_Flare);

    UnloadTexData(m_Tex_cursor_020);
    UnloadTexData(m_Tex_cursor_010);
    UnloadTexData(m_Tex_cursor_000);

    UnloadTexData(m_Tex_Shadow);

    UnloadTexData(m_Tex_ShockWaveSprite);
    UnloadTexData(m_Tex_ShockWaveFlow);

    UnloadTexData(m_Tex_EbBomb_ShockWave);
}

// ===================================================
// オブジェクトの更新
// ===================================================
void GOE_Character_EnemyProjectile::Update()
{
}

// ===================================================
// オブジェクトの更新(アクティブ)
// ===================================================
void GOE_Character_EnemyProjectile::UpdateActive()
{
    if (GameManagerIsMovieFlag())
    { //ムービー中は止める
        return;
    }

    if (m_type == Type::BladeWave)
    {
        //if(m_Obj->frameActive % 8 == 0)
        //{
        //    float angle = 360.0f;
        //    int bulletnum = 2;
        //    for (int i = 0; i < bulletnum; i++)
        //    {
        //        float radtmp = CalculateDegToRad(
        //            CalculateRadToDeg(atan2(m_InitVel.y, m_InitVel.x)) - 135.0f
        //            + 180.0f + (angle - 180.0f) / 4 - (angle / bulletnum * i)
        //        );
        //        {
        //            Float3 nvec = NormalizeVector(
        //                MkF3(
        //                    (float)cos(radtmp) * 3.0f,
        //                    (float)sin(radtmp) * 3.0f,
        //                    0.0f
        //                ));

        //            GOE_Character_EnemyProjectile* goepro =
        //                GOE_Character_EnemyProjectile::CreateProjectile(
        //                    GOE_Character_EnemyProjectile::Type::ExplosionShockWave,
        //                    MkF3(
        //                        m_Obj->pos.x + nvec.x * ONE_BLOCK * 3.0f,
        //                        m_Obj->pos.y + nvec.y * ONE_BLOCK * 3.0f,
        //                        m_Obj->pos.z + nvec.z * ONE_BLOCK * 3.0f
        //                    ),
        //                    nvec
        //                );
        //            if (goepro != nullptr)
        //            {
        //                GAME_OBJECT* beamObj = goepro->GetGameObj();
        //                if (beamObj != nullptr)
        //                {
        //                    float sca = (m_Obj->scale.x + m_Obj->scale.y + m_Obj->scale.z) / 3;
        //                    beamObj->scale.x = 0.8f * sca;
        //                    beamObj->scale.y = 0.8f * sca;
        //                    beamObj->scale.z = 0.8f * sca;
        //                    {
        //                        Character::MOTION_COL_DAT motionColDetails =
        //                            Character::GetMotionColDataFindByName(
        //                                Character::MOTION_COL_TYPE_ATK,
        //                                ATK_COL_NAME_0
        //                            );

        //                        GAME_OBJECT_COLLISION_ATTACK_CHILD attackColTmp = motionColDetails.atkCol;
        //                        attackColTmp.subjectFamily[GAME_OBJECT_FAMILY_PLAYER] = true;

        //                        attackColTmp.size = MkF3(
        //                            ONE_BLOCK * 3,
        //                            ONE_BLOCK * 3,
        //                            ONE_BLOCK * 3
        //                        );
        //                        attackColTmp.pos.z = -attackColTmp.size.z / 2;
        //                        attackColTmp.pos.y += attackColTmp.size.y / 2 * 1.0f;

        //                        beamObj->collision.atk.enable = true;
        //                        beamObj->collision.atk.collision[0] = attackColTmp;
        //                        beamObj->collision.atk.collision[0].enable = true;
        //                    }
        //                    beamObj->blockCollisionIgnore = false;

        //                    GameObjectSetLifeTime(beamObj, 200);
        //                    GameObjectMoveRequest(beamObj, 200,
        //                        MkF3(
        //                            beamObj->pos.x + nvec.x * ONE_BLOCK * 70.0f,
        //                            beamObj->pos.y + nvec.y * ONE_BLOCK * 70.0f,
        //                            beamObj->pos.z + nvec.z * ONE_BLOCK * 70.0f
        //                        ),
        //                        nullptr
        //                    );
        //                }
        //            }
        //        }
        //    }
        //}

        if (m_Obj->frameActive % 5 == 0)
        {
            for (int i = 0; i < 2; i++)
            {
                Float3 posTmp = m_Obj->pos;
                posTmp.x += sin((float)m_Obj->frame * 13.0f + (0.7f * i)) * ONE_BLOCK * 6.0f * m_Obj->scale.x;
                posTmp.y += cos((float)m_Obj->frame * +3.0f + (1.3f * i)) * ONE_BLOCK * 6.0f * m_Obj->scale.y;
                posTmp.z += cos((float)m_Obj->frame * 13.0f + (1.3f * i)) * ONE_BLOCK * 6.0f * m_Obj->scale.z;

                posTmp.x += m_InitVel.x * 100.0f;
                posTmp.y += m_InitVel.y * 100.0f;
                //posTmp.z += -80.0f;

                GAME_OBJECT* go = GO_CreateEffect(GO_EFFECT_TYPE_SPARK, posTmp);
                if (go != nullptr)
                {
                    if (i == 1)
                    {
                        go->graphic.color = MkF4(
                            0.45f,
                            0.0f,
                            0.05f,
                            -0.8f
                        );
                    }
                    else
                    {
                        go->graphic.color = MkF4(
                            0.4f,
                            0.0f,
                            0.5f,
                            0.8f
                        );
                    }
                    go->rot = CalculateRadToDeg(atan2(m_InitVel.y, m_InitVel.x)) - 45.0f
                        + 55.0f * m_Obj->frame * (i + 1);
                    go->scale = MkF3(
                        12.0f,
                        12.0f,
                        12.0f
                    );
                    GameObjectSetLifeTime(go, 28);
                }
            }
        }
    }

    if (m_type == Type::EnergyBallBitKakusan)
    {

        if (m_Obj->lifetimeFrame == 1)
        {
            { //うつよ
                //SEを再生
                SoundEffectPlayRequest(
                    GetSoundData("SOUND/SE/enemy/attack/Laser"),
                    1.9f, 0, 0.8f);
                //SEを再生
                SoundEffectPlayRequest(
                    GetSoundData("SOUND/SE/enemy/attack/Small_Explosion"),
                    4.5f, 0);

                CameraShakeRequestAll(CAMERA_SHAKE_MIDIUM_FRAME, CAMERA_SHAKE_MIDIUM_POWER);


                float angle = 360.0f;
                int bulletnum = 7;
                for (int i = 0; i < bulletnum; i++)
                {
                    float radtmp = CalculateDegToRad(
                        180.0f + (angle - 180.0f) / 4 - (angle / bulletnum * i)
                    );
                    {
                        Float3 nvec = NormalizeVector(
                            MkF3(
                                (float)cos(radtmp) * 3.0f,
                                (float)sin(radtmp) * 3.0f,
                                0.0f
                            ));

                        GOE_Character_EnemyProjectile* goepro =
                            GOE_Character_EnemyProjectile::CreateProjectile(
                                GOE_Character_EnemyProjectile::Type::Beam,
                                MkF3(
                                    m_Obj->pos.x + nvec.x * ONE_BLOCK * 3.0f,
                                    m_Obj->pos.y + nvec.y * ONE_BLOCK * 3.0f,
                                    m_Obj->pos.z + nvec.z * ONE_BLOCK * 3.0f
                                ),
                                nvec
                            );
                        if (goepro != nullptr)
                        {
                            GAME_OBJECT* beamObj = goepro->GetGameObj();
                            if (beamObj != nullptr)
                            {
                                beamObj->scale.x = m_Obj->scale.x;
                                beamObj->scale.y = m_Obj->scale.y;
                                beamObj->scale.z = m_Obj->scale.z;
                                {
                                    Character::MOTION_COL_DAT motionColDetails =
                                        Character::GetMotionColDataFindByName(
                                            Character::MOTION_COL_TYPE_ATK,
                                            ATK_COL_NAME_0
                                        );

                                    GAME_OBJECT_COLLISION_ATTACK_CHILD attackColTmp = motionColDetails.atkCol;
                                    attackColTmp.subjectFamily[GAME_OBJECT_FAMILY_PLAYER] = true;
                                    attackColTmp.subjectFamily[GAME_OBJECT_FAMILY_GIMIK] = true;

                                    attackColTmp.size = MkF3(
                                        ONE_BLOCK * 3,
                                        ONE_BLOCK * 3,
                                        ONE_BLOCK * 5
                                    );
                                    attackColTmp.pos.y += attackColTmp.size.y / 2 * 1.0f;

                                    beamObj->collision.atk.enable = true;
                                    beamObj->collision.atk.collision[0] = attackColTmp;
                                    beamObj->collision.atk.collision[0].enable = true;
                                }
                                beamObj->blockCollisionIgnore = false;

                                GameObjectSetLifeTime(beamObj, 200);
                                GameObjectMoveRequest(beamObj, 200,
                                    MkF3(
                                        beamObj->pos.x + nvec.x * ONE_BLOCK * 90.0f,
                                        beamObj->pos.y + nvec.y * ONE_BLOCK * 90.0f,
                                        beamObj->pos.z + nvec.z * ONE_BLOCK * 90.0f
                                    ),
                                    nullptr
                                );
                            }
                        }
                    }
                }
            }
        }

    }

    if (m_type == Type::EnergyBallBit)
    {
        bool isH = m_NumFlag[0] == 1;
        if (m_Obj->frameActive != 0 && m_Obj->frameActive % (isH ? 100: 60) == 0)
        {
            GAME_OBJECT* targetObject = GetGameObjectFindByIdentifier(m_ObjTarget);
            if (targetObject != nullptr)
            {
                Float3 vec = NormalizeVector(MkF3(
                    targetObject->pos.x - m_Obj->pos.x,
                    targetObject->pos.y - m_Obj->pos.y,
                    targetObject->pos.z - m_Obj->pos.z
                ));
                CameraShakeRequestAll(CAMERA_SHAKE_MIDIUM_FRAME, CAMERA_SHAKE_MIDIUM_POWER);

                //SEを再生
                SoundEffectPlayRequest(
                    GetSoundData("SOUND/SE/enemy/attack/Laser"),
                    0.6f, 0);

                GOE_Character_EnemyProjectile* goepro =
                    GOE_Character_EnemyProjectile::CreateProjectile(
                        isH ?
                        GOE_Character_EnemyProjectile::Type::EnergyBallBitKakusan:
                        GOE_Character_EnemyProjectile::Type::Beam,
                        MkF3(
                            m_Obj->pos.x + vec.x * ONE_BLOCK * 1.5f,
                            m_Obj->pos.y + vec.y * ONE_BLOCK * 1.5f - 80.0f,
                            m_Obj->pos.z + vec.z * ONE_BLOCK * 1.5f
                        ),
                        vec
                    );
                if (goepro != nullptr)
                {
                    GAME_OBJECT* beamObj = goepro->GetGameObj();
                    if (beamObj != nullptr)
                    {
                        beamObj->blockCollisionIgnore = false;
                        beamObj->scale = MkF3(
                            1.0f,
                            1.0f,
                            1.1f
                        );
                        {
                            Character::MOTION_COL_DAT motionColDetails =
                                Character::GetMotionColDataFindByName(
                                    Character::MOTION_COL_TYPE_ATK,
                                    "COL_ATK_ENEMY_BOSS_INPERIUSU_MACHINEGUN"
                                );

                            GAME_OBJECT_COLLISION_ATTACK_CHILD attackColTmp = motionColDetails.atkCol;
                            attackColTmp.subjectFamily[GAME_OBJECT_FAMILY_PLAYER] = true;
                            attackColTmp.subjectFamily[GAME_OBJECT_FAMILY_GIMIK] = true;

                            attackColTmp.size = MkF3(
                                ONE_BLOCK * 3.5,
                                ONE_BLOCK * 3.5,
                                ONE_BLOCK * 3.5
                            );
                            attackColTmp.pos.z = -attackColTmp.size.z / 2;
                            attackColTmp.pos.y += attackColTmp.size.y / 2 * 1.0f;

                            beamObj->collision.atk.enable = true;
                            beamObj->collision.atk.collision[0] = attackColTmp;
                            beamObj->collision.atk.collision[0].enable = true;
                        }
                        GameObjectSetLifeTime(beamObj, 70);

                        Float3 targetpos = MkF3(
                            beamObj->pos.x + vec.x * ONE_BLOCK * 100.0f,
                            beamObj->pos.y + vec.y * ONE_BLOCK * 100.0f,
                            beamObj->pos.z + vec.z * ONE_BLOCK * 100.0f
                        );

                        if (isH)
                        {
                            targetpos.z = -50.0f;
                        }

                        GameObjectMoveRequest(beamObj, 70,
                            targetpos,
                            EasingInSine
                        );
                    }
                }
            }
        }
        //m_MachineGunShootVec = m_TargetVec;

    }

    if (m_type == Type::EnergyBallBitLaser)
    {
        if (m_Obj->frame == 0)
        {
            if (m_NumFlag[0] > 0)
            {
                SoundEffectPlayRequest(GetSoundData("SOUND/SE/enemy/move/warpLow"),
                    3.5f, 0);
            }

            Character::MOTION_COL_DAT motionColDetails =
                Character::GetMotionColDataFindByName(
                    Character::MOTION_COL_TYPE_ATK,
                    ATK_COL_NAME_0
                );
            GAME_OBJECT_COLLISION_ATTACK_CHILD attackColTmp = motionColDetails.atkCol;
            attackColTmp.subjectFamily[GAME_OBJECT_FAMILY_PLAYER] = true;
            attackColTmp.subjectFamily[GAME_OBJECT_FAMILY_GIMIK] = true;

            //判定サイズ
            attackColTmp.size = MkF3(
                ONE_BLOCK * 2.0f,
                ONE_BLOCK * 5000.0f,
                ONE_BLOCK * 8.0f
            );

            //攻撃判定を生成
            GameObjectAttackCollisionAdd(m_Obj, attackColTmp,
                m_NumFlag[0],
                m_Obj->lifetimeFrameMax - BEAM_SHOOT_AFTER - m_NumFlag[0]
            );

        }

        if (m_Obj->frame == m_NumFlag[0])
        {
            //SEを再生
            SoundEffectPlayRequest(
                GetSoundData("SOUND/SE/enemy/attack/Laser"),
                1.5f, 0, 0.5f);
            SoundEffectPlayRequest(
                GetSoundData("SOUND/SE/enemy/attack/Big_Explosion"),
                2.2f, 0, 0.75f);
            //SEを再生
            SoundEffectPlayRequest(
                GetSoundData("SOUND/SE/enemy/attack/Small_Explosion"),
                2.2f, 0, 0.9f);

            CameraShakeRequestAll(CAMERA_SHAKE_LARGE_FRAME, CAMERA_SHAKE_EXLARGE_POWER);

            //{
            //    GAME_OBJECT_COLLIDER& collider = m_Obj->collider;

            //    collider.type = GAME_OBJECT_COLLIDER_TYPE_CAPSULE;
            //    collider.capsule.radius = BLOCK_WIDTH * 2.0f;
            //    collider.capsule.pointA.x = 0.0f;
            //    collider.capsule.pointA.y = BLOCK_WIDTH * -5000.0f;
            //    collider.capsule.pointA.z = -m_Obj->pos.z + BLOCK_WIDTH * 1.5f;
            //    collider.capsule.pointB.x = 0.0f;
            //    collider.capsule.pointB.y = BLOCK_WIDTH * 5000.0f;
            //    collider.capsule.pointB.z = -m_Obj->pos.z + BLOCK_WIDTH * 1.5f;
            //}
        }
    }

    if (m_type == Type::Beam)
    {
        if (m_Obj->frameActive % 8 == 0)
        {
            for (int i = 0; i < 2; i++)
            {
                Float3 posTmp = m_Obj->pos;
                posTmp.x += sin((float)m_Obj->frame * 13.0f + (0.7f * i)) * ONE_BLOCK * 0.8f * m_Obj->scale.x;
                posTmp.y += cos((float)m_Obj->frame * +3.0f + (1.3f * i)) * ONE_BLOCK * 0.8f * m_Obj->scale.y;
                posTmp.z += cos((float)m_Obj->frame * 13.0f + (1.3f * i)) * ONE_BLOCK * 0.8f * m_Obj->scale.z;

                posTmp.x += m_InitVel.x * 100.0f;
                posTmp.y += m_InitVel.y * 100.0f;
                //posTmp.z += -80.0f;

                GAME_OBJECT* go = GO_CreateEffect(GO_EFFECT_TYPE_SPARK, posTmp);
                if (go != nullptr)
                {
                    //go->graphic.color = MkF4(
                    //    1.0f,
                    //    1.0f,
                    //    2.0f,
                    //    1.0f
                    //);
                    if (i == 1)
                    {
                        go->graphic.color = MkF4(
                            0.4f,
                            0.0f,
                            0.05f,
                            -0.8f
                        );
                    }
                    else
                    {
                        //go->graphic.color = MkF4(
                        //    1.2f,
                        //    0.6f,
                        //    1.5f,
                        //    0.95f
                        //);
                        go->graphic.color = MkF4(
                            0.8f,
                            0.0f,
                            0.1f,
                            0.8f
                        );
                    }
                    go->rot = CalculateRadToDeg(atan2(m_InitVel.y, m_InitVel.x)) - 45.0f
                        + 55.0f * m_Obj->frame * (i + 1);
                    go->scale = MkF3(
                        4.2f,
                        4.2f,
                        4.2f
                    );
                    GameObjectSetLifeTime(go, 28);
                }
            }
        }

        if (m_Obj->pos.z > 0)
        {
            {
                Float3 posTmp = m_Obj->pos;
                posTmp.z = 0.0f;

                GAME_OBJECT* go = GO_CreateEffect(GO_EFFECT_TYPE_EXPLOSION_1_GROUND, posTmp);
                if (go != nullptr)
                {
                    go->rot = CalculateRadToDeg(atan2(m_InitVel.y, m_InitVel.x)) - 45.0f
                        + 55.0f * m_Obj->frame;
                    float sca = (m_Obj->scale.x + m_Obj->scale.y + m_Obj->scale.z) / 3.0f;
                    go->scale = MkF3(
                        5.0f * sca,
                        5.0f * sca,
                        5.0f * sca
                    );
                    GameObjectSetLifeTime(go, 30);
                }
            }

            DeleteGameObject(m_Obj);
        }
    }

    if (m_type == Type::Granade)
    {
        float sca = (m_Obj->scale.x + m_Obj->scale.y + m_Obj->scale.z) / 3.0f;

        if (m_Init && !m_Init2 && m_Obj->moveRequ.frame == 1)
        {
            if (m_Obj->freeInt > 0)
            {
                GameObjectHitStop(m_Obj, m_Obj->freeInt);
            }

            if (m_NumFlag[0] == 2)
            {
                m_Obj->scale.x *= 1.5f;
                m_Obj->scale.y *= 1.5f;
                m_Obj->scale.z *= 1.5f;
            }
        }
        if (m_Init && !m_Init2)
        {
            if (m_Obj->moveRequ.frame == 1)
            {
                if (m_Obj->freeInt > 0)
                {
                    GameObjectHitStop(m_Obj, m_Obj->freeInt);
                }
            }
            if(m_Obj->moveRequ.frame <= 0)
            {
                m_Init2 = true;

                {
                    GAME_OBJECT* targetObject = GetGameObjectFindByIdentifier(m_ObjTarget);
                    if (targetObject != nullptr)
                    {
                        m_HitPos = targetObject->pos;

                        {
                            if (m_NumFlag[0] != 2 && m_NumFlag[0] != 4)
                            {
                                float rad = CalculateDegToRad(5.0f) * m_Obj->freeInt
                                    + CalculateDegToRad(180.0f);
                                //m_HitPos.x += sin(rad) * ONE_BLOCK * 25.0f;
                                //m_HitPos.y += cos(rad) * ONE_BLOCK * 25.0f;
                                m_HitPos.x += sin(rad) * ONE_BLOCK * 18.0f;
                                m_HitPos.y += cos(rad) * ONE_BLOCK * 18.0f;
                            }

                            m_HitPos = InStageGameObjectPos(
                                m_HitPos,
                                MkF3(
                                    GRANADE_EXPLO_AREA * sca * 2.0f * 0.6f,
                                    GRANADE_EXPLO_AREA * sca * 2.0f * 0.6f,
                                    0.0f
                                )
                            );
                        }
                    }
                }

                m_Obj->pos.x = m_HitPos.x;
                m_Obj->pos.y = m_HitPos.y;

                GameObjectMoveRequest(m_Obj, 90,
                    m_HitPos,
                    EasingInSine
                );

                //SEを再生
                SoundEffectPlayRequest(
                    GetSoundData("SOUND/SE/inperiusu/lockon"),
                    0.24f, 0, 0.9f);

                //反転
                //m_Obj->graphic.graph.polygonModel[0].size.y *= -1;
            }
        }


        //if (m_Obj->frameActive % 8 == 0)
        //{
        //    GAME_OBJECT* go = GO_CreateAfterEffect(m_Obj, 30, MkF4(1.0f, 1.0f, 1.0f, 0.75f));
        //}

        if (m_Init && m_Init2 && m_Obj->moveRequ.frame <= 0)
        {
            {
                CameraFlashRequestAll(
                    25,
                    EasingInOutSine,
                    //MkF4(0.9f, 0.7f, 0.5f, 0.8f),
                    MkF4(10.0f, 10.0f, 5.0f, 0.6f),
                    DPD_SHADER_OPT_BLEND_STATE_ADDCOLOR
                );

                //SoundEffectPlayRequest(SOUND_TYPE_GENERAL_CLASH, 0.25f, 0);
                CameraShakeRequestAll(CAMERA_SHAKE_MIDIUM_FRAME, CAMERA_SHAKE_EXLARGE_POWER);

                //SEを再生
                SoundEffectPlayRequest(GetSoundData("SOUND/SE/enemy/attack/Big_Explosion"),
                    3.0f, 0);
                SoundEffectPlayRequest(SOUND_TYPE_SE_9, 2.2f, 0, 1.1f);

                {
                    Float3 efPos = m_Obj->pos;
                    efPos.z += ONE_BLOCK * -4.0f;

                    GO_CreateExplosion(efPos, sca * 0.75f);
                }
                //{
                //    GAME_OBJECT beamObj = GetGameObjectPrefab(GAME_OBJECT_TYPE_DUMMY);
                //    beamObj.pos = m_Obj->pos;
                //    Character::MOTION_COL_DAT motionColDetails =
                //        Character::GetMotionColDataFindByName(
                //            Character::MOTION_COL_TYPE_ATK,
                //            ATK_COL_NAME_0
                //        );

                //    GAME_OBJECT_COLLISION_ATTACK_CHILD attackColTmp = motionColDetails.atkCol;

                //    attackColTmp.subjectFamily[GAME_OBJECT_FAMILY_PLAYER] = true;

                //    attackColTmp.size.x = BLOCK_WIDTH * 12.0f;
                //    attackColTmp.size.y = BLOCK_HIGHT * 12.0f;
                //    attackColTmp.size.z = BLOCK_HIGHT * 12.0f;
                //    //attackColTmp.pos.y = -attackColTmp.size.y / 2;

                //    attackColTmp.power = 20;
                //    attackColTmp.hitStopAttacker = 4;
                //    attackColTmp.hitStopReciever = 35;

                //    beamObj.collision.atk.enable = true;
                //    beamObj.collision.atk.collision[0] = attackColTmp;
                //    beamObj.collision.atk.collision[0].enable = true;

                //    GAME_OBJECT* beamObjCreated = CreateGameObject(beamObj);
                //    GameObjectSetLifeTime(beamObjCreated, 10);
                //}
                {
                    Float3 posTmp = m_Obj->pos;
                    posTmp.z = 0.0f;

                    GAME_OBJECT* go = GO_CreateEffect(GO_EFFECT_TYPE_EXPLOSION_1_GROUND, posTmp);
                    if (go != nullptr)
                    {
                        //go->rot = CalculateRadToDeg(atan2(m_InitVel.y, m_InitVel.x)) - 45.0f
                        //    + 55.0f * m_Obj->frame;
                        go->scale = MkF3(
                            22.0f * sca,
                            22.0f * sca,
                            22.0f * sca
                        );
                        GameObjectSetLifeTime(go, 60);
                    }
                }

                { //自爆の判定
                    std::map<int, GAME_OBJECT*> objs = GetGameObjectFindByDistance(m_Obj->pos,
                        GRANADE_EXPLO_AREA * sca
                    );
                    for (int i = 0; i < objs.size(); i++)
                    {
                        if (IsBelongsFamilyGameObject(objs[i], GAME_OBJECT_FAMILY_PLAYER)
                            || IsBelongsFamilyGameObject(objs[i], GAME_OBJECT_FAMILY_GIMIK))
                        {
                            //有効なくらい判定を探す
                            GAME_OBJECT_COLLISION_RECIEVE_CHILD colRec = {};
                            for (int j = 0; j < GAME_OBJECT_COLLISION_NUM_MAX; j++)
                            {
                                if (objs[i]->collision.rec.collision[j].enable)
                                {
                                    colRec = objs[i]->collision.rec.collision[j];
                                    break;
                                }
                            }

                            if (colRec.enable)
                            { //判定が有効なら処理する

                                Character::MOTION_COL_DAT motionColDetails =
                                    Character::GetMotionColDataFindByName(
                                        Character::MOTION_COL_TYPE_ATK,
                                        "COL_ATK_ENEMY_BOSS_INPERIUSU_GRANADE"
                                    );

                                GameObjectDamageCollisionHit(objs[i], m_Obj,
                                    motionColDetails.atkCol,
                                    colRec
                                );

                            }
                        }
                    }
                }

                if (m_NumFlag[0] == 1 || m_NumFlag[0] == 2 
                    || m_NumFlag[0] == 3 || m_NumFlag[0] == 4
                    )
                {
                    float angle = 360.0f;
                    //int bulletnum = 6;
                    int bulletnum = 8;

                    if (m_NumFlag[2] > 0)
                    {
                        bulletnum = m_NumFlag[2];
                    }
                    else
                    {
                        if (m_NumFlag[0] == 2)
                        {
                            //bulletnum = 16;
                            bulletnum = 24;
                            //bulletnum = 12;
                        }

                        if (m_NumFlag[0] == 3)
                        {
                            bulletnum = 3;
                        }
                        if (m_NumFlag[0] == 4)
                        {
                            bulletnum = 5;
                        }
                    }

                    for (int i = 0; i < bulletnum; i++)
                    {
                        float radtmp = CalculateDegToRad(
                            180.0f + (angle - 180.0f) / 4 - (angle / bulletnum * i)
                        );
                        {
                            Float3 nvec = NormalizeVector(
                                MkF3(
                                    (float)cos(radtmp) * 3.0f,
                                    (float)sin(radtmp) * 3.0f,
                                    0.0f
                                ));

                            GOE_Character_EnemyProjectile* goepro =
                                GOE_Character_EnemyProjectile::CreateProjectile(
                                    GOE_Character_EnemyProjectile::Type::ExplosionShockWave,
                                    MkF3(
                                        //m_Obj->pos.x + nvec.x * ONE_BLOCK * 3.0f,
                                        //m_Obj->pos.y + nvec.y * ONE_BLOCK * 3.0f,
                                        //m_Obj->pos.z + nvec.z * ONE_BLOCK * 3.0f
                                        m_Obj->pos.x + nvec.x * ONE_BLOCK * 5.0f,
                                        m_Obj->pos.y + nvec.y * ONE_BLOCK * 5.0f,
                                        m_Obj->pos.z + nvec.z * ONE_BLOCK * 5.0f
                                    ),
                                    nvec
                                );
                            if (goepro != nullptr)
                            {
                                if (m_NumFlag[1] == 1)
                                {
                                    goepro->SetColor(MkF4(1.7f, 1.0f, 2.0f, 1.0f));
                                }

                                GAME_OBJECT* beamObj = goepro->GetGameObj();
                                if (beamObj != nullptr)
                                {
                                    beamObj->scale.x = 1.25f * m_Obj->scale.x;
                                    beamObj->scale.y = 1.25f * m_Obj->scale.y;
                                    beamObj->scale.z = 1.25f * m_Obj->scale.z;
                                    {
                                        Character::MOTION_COL_DAT motionColDetails =
                                            Character::GetMotionColDataFindByName(
                                                Character::MOTION_COL_TYPE_ATK,
                                                ATK_COL_NAME_0
                                            );

                                        GAME_OBJECT_COLLISION_ATTACK_CHILD attackColTmp = motionColDetails.atkCol;
                                        attackColTmp.subjectFamily[GAME_OBJECT_FAMILY_PLAYER] = true;
                                        attackColTmp.subjectFamily[GAME_OBJECT_FAMILY_GIMIK] = true;

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

                                    GameObjectSetLifeTime(beamObj, 150);
                                    GameObjectMoveRequest(beamObj, 150,
                           /*             GameObjectSetLifeTime(beamObj, 200);
                                    GameObjectMoveRequest(beamObj, 200,*/
                                        MkF3(
                                            beamObj->pos.x + nvec.x * ONE_BLOCK * 70.0f,
                                            beamObj->pos.y + nvec.y * ONE_BLOCK * 70.0f,
                                            beamObj->pos.z + nvec.z * ONE_BLOCK * 70.0f
                                        ),
                                        nullptr
                                    );
                                }
                            }
                        }
                    }
                }

                //SoundEffectPlayRequest(SOUND_TYPE_PLAYER_SHOOT_LARGE, 0.5f, 0);
                m_Obj->graphic.hidden = true;
                DeleteGameObject(m_Obj);
                //GameObjectSetLifeTime(m_Obj, 10);
            }
        }

        if (m_Init2)
        {
            m_Init2Frame++;
        }

    }


    if (m_type == Type::ExplosionShockWave)
    {
        float sca = (m_Obj->scale.x + m_Obj->scale.y) / 2.0f;

        if (m_Obj->frameActive % 10 == 0)
        //if (m_Obj->frameActive % 5 == 0)
        {
            {
                Float3 posTmp = m_Obj->pos;

                GAME_OBJECT* go = GO_CreateEffect(GO_EFFECT_TYPE_EXPLOSION_1_GROUND, posTmp);
                if (go != nullptr)
                {
                    go->rot = CalculateRadToDeg(atan2(m_InitVel.y, m_InitVel.x)) - 45.0f
                        + 55.0f * m_Obj->frame;
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

    //ボムの処理
    if (m_type == Type::EnergyBallBomb)
    {
        int BOMB_EXPLOSION_START_FRAME = m_NumFlag[0];
        int BOMB_AREA = m_NumFlag[1];
        int BOMB_EXPLOSION_START_BEFORE_FRAME = m_NumFlag[2];
        int BOMB_EXPLOSION_ATTACK_FRAME = m_NumFlag[3];
        int BOMB_EXPLOSION_STARTING_FRAME = m_NumFlag[4];
        int BOMB_EXPLOSION_AFTER_FRAME = m_NumFlag[5];
        int BOMB_VARIANT = m_NumFlag[6];
        
        int BOMB_EXPLOSION_TOTAL_FRAME =
            BOMB_EXPLOSION_START_FRAME
            + BOMB_EXPLOSION_ATTACK_FRAME
            + BOMB_EXPLOSION_AFTER_FRAME
            ;

        auto bombAttackCol =
            Character::GetMotionColDataFindByName(
                Character::MOTION_COL_TYPE_ATK,
                ATK_COL_NAME_BEAM_LARGE
            ).atkCol;

        bombAttackCol.hitCycle = 6; //ヒット間隔のオーバーライド

        int bombFrame = m_Obj->frameActive;
        GAME_OBJECT* obj = m_Obj;
        if (obj != nullptr)
        {
            //引き寄せ
            float vacuumPower = 1.0f / 100.f;

            if (bombFrame == 30)
            {
                GameObjectMoveRequest(obj, 70,
                    MkF3(
                        obj->pos.x,
                        obj->pos.y,
                        obj->pos.z + ONE_BLOCK * -2.0f
                    ),
                    EasingInOutSine
                );
            }

            bool isAttacking = bombFrame >= BOMB_EXPLOSION_STARTING_FRAME * 0.25f + BOMB_EXPLOSION_START_FRAME
                && bombFrame <= BOMB_EXPLOSION_START_FRAME + BOMB_EXPLOSION_ATTACK_FRAME
                ;

            auto isAttackTarget = [this](GAME_OBJECT* ob)
            {
                return IsActiveGameObject(ob)
                    && ob->type == GAME_OBJECT_TYPE_CHARACTER
                    && ob != m_Obj
                    && (ob->tags.size() > 0
                        && (
                            ob->tags[0].find("PLAYER") == 0
                            || !ob->tags[0].find("ENEMY_BOSS") == 0
                        )
                    )
                    && ob->status.statusType
                        != GAME_OBJECT_STATUS_TYPE_ENEMY_BOSS
                    ;
            };


            if (!isAttacking)
            {
                vacuumPower = 1.0f / 500.f * ((float)bombFrame / 60.0f);

                if (bombFrame % 14 == 0)
                {
                    Float3 postmp = obj->pos;
                    //postmp.x += sin((float)obj->frame * 13.0f) * ONE_BLOCK * 3.2f * obj->scale.x;
                    //postmp.y += cos((float)obj->frame * 03.0f) * ONE_BLOCK * 3.2f * obj->scale.y;
                    //postmp.z += cos((float)obj->frame * 13.0f) * ONE_BLOCK * 3.2f * obj->scale.z;
                    //postmp.z += -150.0f * m_Obj->scale.y;
                    GAME_OBJECT* go = GO_CreateEffect(GO_EFFECT_TYPE_SPARK, postmp);
                    if (go != nullptr)
                    {
                        //if (obj->frameActive % 4 == 0)
                        //{
                        //    go->graphic.color = MkF4(
                        //        0.2f,
                        //        0.2f,
                        //        0.1f,
                        //        1.0f
                        //    );
                        //}
                        //else
                        {
                            go->graphic.color = MkF4(
                                2.0f,
                                2.0f,
                                0.1f,
                                1.0f
                            );
                        }
                        go->rot = -33.0f * obj->frame + m_Obj->frame;
                        go->scale = MkF3(
                            14.0f,
                            14.0f,
                            14.0f
                        );
                        GameObjectSetLifeTime(go, 6);
                    }
                }
            }
            if (bombFrame == BOMB_EXPLOSION_START_BEFORE_FRAME)
            {
                //SEを再生
                SoundEffectPlayRequest(
                    GetSoundData("SOUND/SE/enemy/attack/Beam_Charge"),
                    0.3f, 0, 1.2f);
            }
            if (bombFrame >= BOMB_EXPLOSION_START_BEFORE_FRAME
                && bombFrame < BOMB_EXPLOSION_START_FRAME)
            {
                int f = bombFrame - BOMB_EXPLOSION_START_BEFORE_FRAME;
                float fmod = (float)f / (BOMB_EXPLOSION_START_FRAME - BOMB_EXPLOSION_START_BEFORE_FRAME);
                if (f % 10 == 0)
                {
                    Float3 postmp = obj->pos;
                    //postmp.z += ONE_BLOCK * -4.0f;

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
                            12.0f * (1.0f + (1.5f * fmod)),
                            12.0f * (1.0f + (1.5f * fmod)),
                            12.0f * (1.0f + (1.5f * fmod))
                        );
                        GameObjectSetLifeTime(go, 30);
                    }
                }
            }

            if (bombFrame == BOMB_EXPLOSION_START_FRAME - 20)
            {
                SoundEffectPlayRequest(GetSoundData("SOUND/SE/enemy/move/warpHigh"), 1.8f, 0);
            }
            if (bombFrame == BOMB_EXPLOSION_START_FRAME)
            {
                obj->status.guardType = GAME_OBJECT_GUARD_TYPE_INV;

                SoundEffectPlayRequest(GetSoundData("SOUND/SE/enemy/attack/Small_Explosion"), 4.5f, 0);
                SoundEffectPlayRequest(GetSoundData("SOUND/SE/enemy/attack/Explosion"), 0.4f, 0);
                //CameraFlashRequestAll(
                //    30,
                //    EasingInOutSine,
                //    MkF4(0.7f, 0.7f, 0.5f, 0.6f),
                //    DPD_SHADER_OPT_BLEND_STATE_ADDCOLOR
                //);
                CameraFlashRequestAll(
                    30,
                    EasingInOutSine,
                    MkF4(10.0f, 10.0f, 5.0f, 0.6f),
                    DPD_SHADER_OPT_BLEND_STATE_ADDCOLOR
                );
            }

            if (isAttacking)
            {
                if (BOMB_VARIANT == 1)
                {
                    vacuumPower = 1.0f / 100.f;

                    if (bombFrame % 8 == 0)
                    {
                        float angle = 360.0f;
                        int bulletnum = 3;

                        Character::MOTION_COL_DAT motionColDetails =
                            Character::GetMotionColDataFindByName(
                                Character::MOTION_COL_TYPE_ATK,
                                ATK_COL_NAME_BEAM_LARGE
                            );

                        for (int i = 0; i < bulletnum; i++)
                        {
                            float radtmp = CalculateDegToRad(
                                180.0f + (angle - 180.0f) / 4 - (angle / bulletnum * i)
                                + bombFrame * 2.5f
                            );
                            {
                                Float3 nvec = NormalizeVector(
                                    MkF3(
                                        (float)cos(radtmp) * 3.0f,
                                        (float)sin(radtmp) * 3.0f,
                                        0.0f
                                    ));

                                GOE_Character_EnemyProjectile* goepro =
                                    GOE_Character_EnemyProjectile::CreateProjectile(
                                        GOE_Character_EnemyProjectile::Type::ExplosionShockWave,
                                        MkF3(
                                            m_Obj->pos.x + nvec.x * ONE_BLOCK * 5.0f,
                                            m_Obj->pos.y + nvec.y * ONE_BLOCK * 5.0f,
                                            0.0f
                                        ),
                                        nvec
                                    );
                                if (goepro != nullptr)
                                {
                                    goepro->SetColor(MkF4(1.7f, 1.0f, 2.0f, 1.0f));

                                    GAME_OBJECT* beamObj = goepro->GetGameObj();
                                    if (beamObj != nullptr)
                                    {
                                        beamObj->scale.x = 2.0f;
                                        beamObj->scale.y = 2.0f;
                                        beamObj->scale.z = 2.0f;
                                        {
                                            GAME_OBJECT_COLLISION_ATTACK_CHILD attackColTmp = motionColDetails.atkCol;
                                            attackColTmp.subjectFamily[GAME_OBJECT_FAMILY_PLAYER] = true;
                                            attackColTmp.subjectFamily[GAME_OBJECT_FAMILY_GIMIK] = true;

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

                                        GameObjectSetLifeTime(beamObj, 120);
                                        GameObjectMoveRequest(beamObj, 120,
                                            MkF3(
                                                beamObj->pos.x + nvec.x * ONE_BLOCK * 70.0f,
                                                beamObj->pos.y + nvec.y * ONE_BLOCK * 70.0f,
                                                beamObj->pos.z + nvec.z * ONE_BLOCK * 70.0f
                                            ),
                                            nullptr
                                        );
                                    }
                                }
                            }
                        }
                    }
                }
                else
                {
                    vacuumPower = 1.0f / 60.f;
                }

                if (bombFrame % 30 == 0)
                {
                    //SEを再生
                    SoundEffectPlayRequest(GetSoundData("SOUND/SE/player/attack/Kakopon_Contorius"),
                        0.32f, 0, 1.1f);
                }
                if (bombFrame % 50 == 0)
                {
                    //SEを再生
                    SoundEffectPlayRequest(
                        GetSoundData("SOUND/SE/enemy/attack/Small_Explosion"),
                        4.0f, 0, 0.4);
                }

                if (bombFrame % 15 == 0)
                {
                    {
                        Float3 posTmp = obj->pos;
                        posTmp.z = -5.0f;

                        GAME_OBJECT* go = GO_CreateEffect(
                            GO_EFFECT_TYPE_EXPLOSION_1_GROUND2,
                            posTmp
                        );
                        if (go != nullptr)
                        {
                            go->rot = 55.0f * obj->frame + m_Obj->frame;
                            go->scale = MkF3(
                                45.0f,
                                45.0f,
                                45.0f
                            );
                            GameObjectSetLifeTime(go, 120);
                        }
                    }
                    {
                        Float3 posTmp = obj->pos;
                        posTmp.z = -10.0f;

                        GAME_OBJECT* go = GO_CreateEffect(
                            GO_EFFECT_TYPE_HAMMER_CHARGE_END_GROUND,
                            posTmp
                        );
                        if (go != nullptr)
                        {
                            go->rot = 55.0f * obj->frame + m_Obj->frame;
                            go->scale = MkF3(
                                36.0f,
                                36.0f,
                                36.0f
                            );
                            GameObjectSetLifeTime(go, 30);
                        }
                    }
                }
                if (bombFrame % 12 == 0)
                {
                    Float3 postmp = obj->pos;
                    //postmp.x += sin((float)obj->frame * 13.0f) * ONE_BLOCK * 3.2f * obj->scale.x;
                    //postmp.y += cos((float)obj->frame * 03.0f) * ONE_BLOCK * 3.2f * obj->scale.y;
                    //postmp.z += cos((float)obj->frame * 13.0f) * ONE_BLOCK * 3.2f * obj->scale.z;
                    //postmp.z += -150.0f * m_Obj->scale.y;
                    GAME_OBJECT* go = GO_CreateEffect(GO_EFFECT_TYPE_SPARK, postmp);
                    if (go != nullptr)
                    {
                        //if (obj->frameActive % 4 == 0)
                        //{
                        //    go->graphic.color = MkF4(
                        //        0.2f,
                        //        0.2f,
                        //        0.1f,
                        //        1.0f
                        //    );
                        //}
                        //else
                        go->rot = -33.0f * obj->frame + m_Obj->frame;
                        go->graphic.color = MkF4(
                            0.1f,
                            0.0f,
                            0.3f,
                            -1.0f
                        );
                        go->scale.x = 40.0f;
                        go->scale.y = 40.0f;
                        go->scale.z = 40.0f;
                        GameObjectSetLifeTime(go, 10);
                    }
                }
                if (bombFrame % 8 == 0)
                {
                    //CameraShakeRequest(
                    //    player->index,
                    //    CAMERA_SHAKE_SMALL_FRAME,
                    //    CAMERA_SHAKE_MIDIUM_POWER * 0.75f,
                    //    CAMERA_SHAKE_MIDIUM_POWER * 0.75f,
                    //    true
                    //);
                    CameraShakeRequestAll(
                        CAMERA_SHAKE_SMALL_FRAME,
                        CAMERA_SHAKE_EXLARGE_POWER * 1.2f,
                        CAMERA_SHAKE_EXLARGE_POWER * 1.2f
                        //, true
                    );
                }
                if (bombFrame % bombAttackCol.hitCycle == 0)
                {
                    for (int i = 0; i < GAME_OBJECT_MAX; i++)
                    {
                        GAME_OBJECT* ob = GetGameObject(i);
                        if (isAttackTarget(ob))
                        {
                            float dis = CalculateDistance(
                                Float3ToFloat2(ob->pos),
                                Float3ToFloat2(obj->pos)
                            );
                            if (dis <= BOMB_AREA)
                            {
                                if (ob->collision.rec.enable)
                                {
                                    //有効なくらい判定を探す
                                    GAME_OBJECT_COLLISION_RECIEVE_CHILD colRec = {};
                                    for (int j = 0; j < GAME_OBJECT_COLLISION_NUM_MAX; j++)
                                    {
                                        if (ob->collision.rec.collision[j].enable)
                                        {
                                            colRec = ob->collision.rec.collision[j];
                                            break;
                                        }
                                    }

                                    if (colRec.enable)
                                    { //判定が有効なら処理する

                                        GameObjectDamageCollisionHit(ob, m_Obj,
                                            bombAttackCol,
                                            colRec
                                        );
                                    }
                                }
                            }
                        }
                    }
                }

            }

            if (bombFrame > BOMB_EXPLOSION_TOTAL_FRAME)
            {
                DeleteGameObject(obj);
            }

            {
                float startMod = 1.0f;
                float endMod = 1.0f;
                {
                    int f = bombFrame - BOMB_EXPLOSION_START_FRAME - BOMB_EXPLOSION_ATTACK_FRAME;
                    if (f >= 0)
                    {
                        endMod =
                            (1.0f - std::max(std::min((float)f / BOMB_EXPLOSION_AFTER_FRAME, 1.0f), 0.0f));

                    }
                }
                {
                    int f = bombFrame;
                    if (f >= 0)
                    {
                        startMod =
                            EasingOutElastic(std::max(std::min((float)f / BOMB_EXPLOSION_START_FRAME, 1.0f), 0.0f));
                        startMod *= bombFrame >= BOMB_EXPLOSION_START_FRAME ? 1.0f : 0.1f;
                    }
                }

                if (bombFrame % 10 == 0)
                {
                    {
                        Float3 posTmp = obj->pos;
                        posTmp.z = -10.0f;

                        GAME_OBJECT* go = GO_CreateEffect(
                            GO_EFFECT_TYPE_HAMMER_CHARGE_LOOP_GROUND,
                            posTmp
                        );
                        if (go != nullptr)
                        {
                            go->rot = 55.0f * obj->frame + m_Obj->frame;
                            go->graphic.color = MkF4(
                                1.0f,
                                1.0f,
                                1.0f,
                                0.5f * startMod * endMod
                            );
                            go->scale = MkF3(
                                150.0f,
                                150.0f,
                                150.0f
                            );
                            GameObjectSetLifeTime(go, 30);
                        }
                    }
                }

                for (int i = 0; i < GAME_OBJECT_MAX; i++)
                {
                    GAME_OBJECT* ob = GetGameObject(i);
                    if (isAttackTarget(ob))
                    {
                        float dis = CalculateDistance(
                            Float3ToFloat2(ob->pos),
                            Float3ToFloat2(obj->pos)
                        );
                        if (dis <= ONE_BLOCK * 200.0f)
                        {
                            if (ob->collision.rec.enable)
                            {
                                //有効なくらい判定を探す
                                GAME_OBJECT_COLLISION_RECIEVE_CHILD colRec = {};
                                for (int j = 0; j < GAME_OBJECT_COLLISION_NUM_MAX; j++)
                                {
                                    if (ob->collision.rec.collision[j].enable)
                                    {
                                        colRec = ob->collision.rec.collision[j];
                                        break;
                                    }
                                }

                                if (colRec.enable)
                                { //判定が有効なら処理する

                                    Float2 vectmp = {
                                        obj->pos.x - ob->pos.x,
                                        obj->pos.y - ob->pos.y,
                                    };
                                    vectmp = NormalizeVector(vectmp);

                                    float vacuumpowertmp = CalculateDistance(
                                        Float3ToFloat2(ob->pos),
                                        Float3ToFloat2(obj->pos)
                                    ) * vacuumPower;

                                    vacuumpowertmp = std::max(
                                        vacuumpowertmp,
                                        10.0f
                                    );

                                    ob->pos.x += vectmp.x * vacuumpowertmp;
                                    ob->pos.y += vectmp.y * vacuumpowertmp;
                                }
                            }
                        }
                    }
                }
            }
            //m_BombActiveFrame++;
        }
        else
        {
        }
    }
}


void GOE_Character_EnemyProjectile::Draw()
{
    if (m_type == Type::BladeWave)
    {
        {
            DRAW_POLYGON_DAT dsdat = {};
            //dsdat.y = 120.0f * ((m_Obj->scale.x + m_Obj->scale.y) / 2);
            dsdat.pos.y = -80.0f * ((m_Obj->scale.x + m_Obj->scale.y) / 2);

            //dsdat.w = (516.0f * 1.9f) * m_Obj->scale.x;
            //dsdat.h = (516.0f * 1.9f) * m_Obj->scale.y;
            dsdat.size.x = (516.0f * 2.0f) * m_Obj->scale.x;
            dsdat.size.y = (516.0f * 2.0f) * m_Obj->scale.y;
            dsdat.size.z = 1.0f;

            dsdat.rot = 0.0f;
            //dsdat.color = { 1.5f, 0.2f, 1.5f, 1.0f };
            if (m_NumFlag[0] == 1)
            {
                dsdat.color = { 2.0f, 1.0f, 0.9f, 1.0f };
            }
            else
            {
                dsdat.color = { 1.0f, 1.0f, 1.0f, 1.0f };
            }

            //dsdat.texNo = m_Tex_BladeWave;
            dsdat.texNo = m_Tex_BladeWaveNew;
            dsdat.loadTexType = LOADTEXTURETYPE_MAIN;

            //dsdat.texUV = { 0.0f, 0.0f };
            //dsdat.texWH = { 1.0f, 1.0f };
            //dsdat.texUV = AnimationUV(
            //    (int)(5 + 9 * (sin((float)m_Obj->frameActive * 0.6) / 2.0f + 0.5f)),
            //    5,
            //    5
            //);
            dsdat.texUV = AnimationUV(
                (int)(m_Obj->frameActive),
                5,
                5
            );
            dsdat.texWH = { 1.0f / 5, 1.0f / 5 };

            dsdat.startPos3D = m_Obj->pos;
            //dsdat.startPos3D.z += -dsdat.h * 0.12f - 250.0f;
            //dsdat.startPos3D.z += -850.0f;

            dsdat.startRot = CalculateRadToDeg(atan2(m_InitVel.y, m_InitVel.x)) - 90.0f;
            dsdat.startScale = { 1.0f, 1.0f };
            dsdat.startScale3D = { 1.0f, 1.0f, 1.0f };

            dsdat.shaderOpt.bloom = DPD_SHADER_OPT_BLOOM_NORMAL;
            //dsdat.shaderOpt.blendState = DPD_SHADER_OPT_BLEND_STATE_ADDCOLOR;
            //if (m_StartMotionShaderFlag)
            //{
            //    //dsdat.s
            //}

            DRAW_POLYGON_IN_CAMERA_EX_OPTION optBaseTmp = {};
            optBaseTmp.reverse = false;
            optBaseTmp.fixed = false;
            optBaseTmp.enableShade = true;
            optBaseTmp.billboard = false;
            optBaseTmp.priority = DRAW_POLYGON_PRIORITY_NORMAL;
            optBaseTmp.shaderType = DPD_SHADER_TYPE_EFFECT;

            if(IsInStage())
            {
                DRAW_POLYGON_DAT dsdat2 = dsdat;
                dsdat2.startPos3D.z = -10.0f;
                dsdat2.color = { 0.4f, 0.0f, 0.4f, 1.0f };
                dsdat2.shaderOpt.blendState = DPD_SHADER_OPT_BLEND_STATE_ADDCOLOR;
                DrawPolygonInCameraEx(dsdat2, optBaseTmp);
            }

            DrawPolygonInCameraEx(dsdat, optBaseTmp);
        }
    }

    if (
        m_type == Type::EnergyBallBit
        || m_type == Type::EnergyBallBitKakusan
        || m_type == Type::EnergyBallBitLaser
    )
    {
        //laser

        float baseEnterMod = EasingOutBack(std::min(
            (float)m_Obj->frame / (BEAM_SHOOT_AFTER * 2),
            1.0f));
        float baseAfterMod = EasingInSine(std::min(
            (float)m_Obj->lifetimeFrame / BEAM_SHOOT_AFTER,
            1.0f));
        if (m_type == Type::EnergyBallBitKakusan)
        {
            baseAfterMod = 1.0f;
        }

        if(m_type == Type::EnergyBallBitLaser)
        {
            for (int beamIndex = 0; beamIndex < 2; beamIndex++)
            {
                Float3 beamVec = NormalizeVector({0.0f, 1.0f, 0.0f});
                if (beamIndex == 1)
                {
                    beamVec.x *= -1;
                    beamVec.y *= -1;
                    beamVec.z *= -1;
                }

                {
                    DRAW_POLYGON_DAT drawDatBaseTmp = {};
                    //drawDatBaseTmp.size.z =
                    //    CalculateDistance(
                    //        targetObject->pos,
                    //        m_Obj->pos
                    //    ) * 0.5f * 0.67f
                    //    ;

                    drawDatBaseTmp.size.z = 5000.0f;
                    //CalculateDistance(
                    //    m_BeamTargetPos,
                    //    m_Obj->pos
                    //) * 0.5f * 0.67f
                    //;

                    //drawDatBaseTmp.pos.z = drawDatBaseTmp.size.z / 2 * 0.75f;
                    drawDatBaseTmp.pos.z = drawDatBaseTmp.size.z / 2 * 0.9f;

                    drawDatBaseTmp.rot = 0.0f;

                    drawDatBaseTmp.rot3D = MkF3(0.0f, 0.0f, 0.0f);

                    drawDatBaseTmp.color = MkF4(0.6f, 0.0f, 0.0f, 
                        0.9f * baseEnterMod * baseAfterMod
                    );

                    drawDatBaseTmp.texNo = 0;
                    drawDatBaseTmp.loadTexType = LOADTEXTURETYPE_MAIN;

                    drawDatBaseTmp.texUV = { 0.0f, 0.0f };
                    drawDatBaseTmp.texWH = { 1.0f, 1.0f };

                    drawDatBaseTmp.startPos3D = m_Obj->pos;

                    drawDatBaseTmp.startRot = CalculateRadToDeg(atan2(beamVec.y, beamVec.x)) - 90.0f;
                    drawDatBaseTmp.startRot3D = MkF3(90.0f + 45.0f * beamVec.z, 0.0f, 0.0f);

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
                    optBaseTmp.depthBuffIgnore = true;


                    //予測エフェクト
                    {
                        //int startbeforef = 30;
                        int startbeforef = 0;
                        if (m_Obj->frame < m_NumFlag[0])
                        {
                            //float startMod = EasingInOutElastic(m_Anim_Mod);
                            float startMod = 1.0f;

                            DRAW_POLYGON_DAT drawDatTmp2 = drawDatBaseTmp;
                            drawDatTmp2.texNo = 0;
                            drawDatTmp2.size.x = 8.0f * startMod;
                            drawDatTmp2.size.y = 8.0f * startMod;
                            drawDatTmp2.size.z *= baseEnterMod;

                            //drawDatTmp2.color = MkF4(2.0f, 0.2f, 0.2f, 0.75f);
                            drawDatTmp2.color = MkF4(1.0f, 0.1f, 0.1f, 0.75f);
                            drawDatTmp2.shaderOpt.blendState = DPD_SHADER_OPT_BLEND_STATE_ADDCOLOR;
                            //drawDatTmp2.color = MkF4(2.0f, 0.5f, 0.5f, 0.75f);
                            DRAW_POLYGON_IN_CAMERA_EX_OPTION opt2 = optBaseTmp;
                            opt2.priority = DRAW_POLYGON_PRIORITY_NORMAL;
                            DrawPolygonInCameraEx(drawDatTmp2, opt2);
                        }
                    }
                }


                {
                    int beamShootStartFrame = m_NumFlag[0];
                    int beamShootEnd = m_Obj->lifetimeFrameMax - BEAM_SHOOT_AFTER;
                    bool isBeamStateActive =
                        m_Obj->frame >= beamShootStartFrame
                        && m_Obj->frame < m_Obj->lifetimeFrameMax;

                    {
                        int beamShootFrame = m_Obj->frame - beamShootStartFrame;

                        float beamMotionStartMod = 1.0f;
                        float beamStartMod = 1.0f;
                        float beamEndMod = baseAfterMod;

                        {
                            int endf = 24;
                            int f = m_Obj->frame - beamShootStartFrame;
                            if (f < endf)
                            {
                                beamMotionStartMod = EasingInSine(
                                    std::max((float)(f) / endf, 0.0f)
                                );
                            }
                        }
                        {
                            beamStartMod = EasingOutBack(beamMotionStartMod);
                            //int endf = 30;
                            //if (beamShootFrame < endf)
                            //{
                            //    beamStartMod = EasingOutElastic((float)beamShootFrame / endf);
                            //}
                        }
                        //{
                        //    int beamShootEndStartFrame = beamShootEnd;
                        //    if (m_Obj->frame >= beamShootEndStartFrame)
                        //    {
                        //        int f = m_Obj->frame - beamShootEndStartFrame;
                        //        int fMax = BEAM_SHOOT_AFTER;
                        //        beamEndMod = 1.0f - (float)f / fMax;
                        //    }
                        //}

                        {
                            float addOffsetZ = 0.0f;

                            if (isBeamStateActive)
                            {
                                {
                                    DRAW_POLYGON_DAT drawDatBaseTmp = {};
                                    drawDatBaseTmp.size = MkF3(
                                        20.0f,
                                        20.0f,
                                        1.0f);

                                    drawDatBaseTmp.size.z = 5000.0f;
                                    //CalculateDistance(
                                    //    m_BeamTargetPos,
                                    //    m_Obj->pos
                                    //) * 0.5f * 0.67f
                                    //;
                                    drawDatBaseTmp.size.z += drawDatBaseTmp.size.x / 2;

                                    float sizeZScale = 750.0f / drawDatBaseTmp.size.z;

                                    drawDatBaseTmp.pos.z = drawDatBaseTmp.size.z / 2 * 0.9f;

                                    //drawDatBaseTmp.pos.z += -(drawDatBaseTmp.size.z + 0.0f) * i + 1500.0f;
                                    drawDatBaseTmp.pos.z += -addOffsetZ;

                                    //drawDatBaseTmp.pos.z += -1000.0f;

                                    drawDatBaseTmp.rot = 0.0f;

                                    drawDatBaseTmp.rot3D = MkF3(0.0f, 0.0f, 0.0f);

                                    drawDatBaseTmp.color = MkF4(1.0f, 0.9f, 1.0f, 0.8f);

                                    drawDatBaseTmp.texNo = 0;
                                    drawDatBaseTmp.loadTexType = LOADTEXTURETYPE_MAIN;

                                    drawDatBaseTmp.texUV = { 0.0f, 0.0f };
                                    drawDatBaseTmp.texWH = { 1.0f, 1.0f };

                                    drawDatBaseTmp.startPos3D = m_Obj->pos;

                                    drawDatBaseTmp.startRot = CalculateRadToDeg(atan2(
                                        beamVec.y,
                                        beamVec.x
                                    )) - 90.0f;
                                    drawDatBaseTmp.startRot3D = MkF3(
                                        90.0f + 45.0f * beamVec.z,
                                        0.0f, 0.0f);


                                    drawDatBaseTmp.startScale = MkF2(1.0f, 1.0f);
                                    drawDatBaseTmp.startScale3D = MkF3(1.0f, 1.0f, 1.0f);

                                    drawDatBaseTmp.modelNo = m_Model_Beam;

                                    drawDatBaseTmp.shaderOpt.blendState = DPD_SHADER_OPT_BLEND_STATE_ADDCOLOR;

                                    DRAW_POLYGON_IN_CAMERA_EX_OPTION optBaseTmp = {};
                                    optBaseTmp.reverse = false;
                                    optBaseTmp.fixed = false;
                                    optBaseTmp.enableShade = true;
                                    optBaseTmp.priority = DRAW_POLYGON_PRIORITY_NML_u3;
                                    optBaseTmp.depthBuffIgnore = true;
                                    optBaseTmp.shaderType = DPD_SHADER_TYPE_EFFECT;

                                    if (m_Obj->frame >= beamShootStartFrame)
                                    {

                                        DRAW_POLYGON_DAT drawDatTmp = drawDatBaseTmp;

                                        Float2 scaleAddTmp = MkF2(0.0f, 0.0f);
                                        Float2 scaleMulTmp = MkF2(1.0f, 1.0f);
                                        {
                                            float addSizeTmp = sin((float)(m_Obj->frame) * 0.8f) * 2.0f;
                                            scaleAddTmp.x += addSizeTmp;
                                            scaleAddTmp.y += addSizeTmp;
                                        }

                                        {
                                            if (beamStartMod < 1.0f)
                                            {
                                                scaleMulTmp.x *= beamStartMod;
                                                scaleMulTmp.y *= beamStartMod;
                                            }
                                        }
                                        {
                                            if (beamEndMod < 1.0f)
                                            {
                                                scaleMulTmp.x *= beamEndMod;
                                                scaleMulTmp.y *= beamEndMod;
                                            }
                                        }
                                        drawDatTmp.size.x += scaleAddTmp.x;
                                        drawDatTmp.size.y += scaleAddTmp.y;
                                        drawDatTmp.size.x *= scaleMulTmp.x;
                                        drawDatTmp.size.y *= scaleMulTmp.y;

                                        DRAW_POLYGON_DAT drawDatTmp2 = drawDatTmp;
                                        drawDatTmp2.size.x *= 3.2f;
                                        drawDatTmp2.size.y *= 3.2f;
                                        drawDatTmp2.shaderOpt.blendState = DPD_SHADER_OPT_BLEND_STATE_DEFAULT;
                                        drawDatTmp2.color = MkF4(0.25f, 0.1f, 0.45f, 0.6f);
                                        DRAW_POLYGON_IN_CAMERA_EX_OPTION opt2 = optBaseTmp;
                                        opt2.priority = DRAW_POLYGON_PRIORITY_NML_d2;

                                        {
                                            DRAW_POLYGON_DAT drawDatTmp3 = drawDatTmp;
                                            drawDatTmp3.texNo = m_Tex_Beam1;
                                            drawDatTmp3.size.x *= 2.4f;
                                            drawDatTmp3.size.y *= 2.4f;
                                            drawDatTmp3.color = MkF4(2.0f, 0.5f, 1.6f, 0.75f);
                                            drawDatTmp3.texUV = {
                                                0.0f + 0.01f * (float)(m_Obj->frame / 1),
                                                0.0f + 0.05f * (float)(m_Obj->frame / 1)
                                            };
                                            drawDatTmp3.texWH = {
                                                3.0f,
                                                10.0f * 5.0f * sizeZScale
                                            };
                                            drawDatTmp3.shaderOpt.blendState = DPD_SHADER_OPT_BLEND_STATE_DEFAULT;
                                            DRAW_POLYGON_IN_CAMERA_EX_OPTION opt3 = optBaseTmp;
                                            opt3.priority = DRAW_POLYGON_PRIORITY_NML_u2;
                                            DrawPolygonInCameraEx(drawDatTmp3, opt3);
                                        }
                                        {
                                            DRAW_POLYGON_DAT drawDatTmp3 = drawDatTmp;
                                            drawDatTmp3.texNo = m_Tex_Beam2;
                                            drawDatTmp3.size.x *= 1.7f;
                                            drawDatTmp3.size.y *= 1.7f;
                                            drawDatTmp3.color = MkF4(3.0f, 1.0f, 7.0f, 0.75f);
                                            drawDatTmp3.texUV = {
                                                0.0f + 0.06f * (float)(m_Obj->frame / 1),
                                                0.0f + 0.05f * (float)(m_Obj->frame / 1)
                                            };
                                            drawDatTmp3.texWH = {
                                                1.0f,
                                                10.0f * 3.0f * sizeZScale
                                            };
                                            drawDatTmp3.shaderOpt.blendState = DPD_SHADER_OPT_BLEND_STATE_DEFAULT;
                                            DRAW_POLYGON_IN_CAMERA_EX_OPTION opt3 = optBaseTmp;
                                            opt3.priority = DRAW_POLYGON_PRIORITY_NORMAL;
                                            DrawPolygonInCameraEx(drawDatTmp3, opt3);
                                        }
                                        {
                                            DRAW_POLYGON_DAT drawDatTmp3 = drawDatTmp;
                                            drawDatTmp3.texNo = m_Tex_Beam3;
                                            drawDatTmp3.size.x *= 3.5f;
                                            drawDatTmp3.size.y *= 3.5f;
                                            drawDatTmp3.color = MkF4(0.13f * 2, 0.f, 0.5f * 2, 0.75f);
                                            drawDatTmp3.texUV = {
                                                0.0f,
                                                0.0f + 0.05f * (float)(m_Obj->frame / 1)
                                            };
                                            drawDatTmp3.texWH = {
                                                1.0f,
                                                10.0f * 4.5f * sizeZScale
                                            };
                                            drawDatTmp3.shaderOpt.blendState = DPD_SHADER_OPT_BLEND_STATE_DEFAULT;
                                            DRAW_POLYGON_IN_CAMERA_EX_OPTION opt3 = optBaseTmp;
                                            opt3.priority = DRAW_POLYGON_PRIORITY_NML_d1;
                                            DrawPolygonInCameraEx(drawDatTmp3, opt3);
                                        }

                                        DrawPolygonInCameraEx(drawDatTmp2, opt2);
                                        DrawPolygonInCameraEx(drawDatTmp, optBaseTmp);
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }

        if (IsInStage())
        {
            DRAW_SPRITE_DAT dsdat = {};
            dsdat.x = m_Obj->pos.x;
            dsdat.y = m_Obj->pos.y;
            dsdat.z = -8.0f;
            //dsdat.z = -0.005f;
            dsdat.w = 320.0f * m_Obj->scale.x * 1.0f* baseEnterMod * baseAfterMod;
            dsdat.h = 320.0f * m_Obj->scale.y * 1.0f* baseEnterMod * baseAfterMod;

            {
                float mo = 1.0f - std::max(std::min(abs(m_Obj->pos.z) / (ONE_BLOCK * 80.0f), 1.0f), 0.0f);
                dsdat.w *= mo;
                dsdat.h *= mo;
            }

            dsdat.rot = 0.0f;
            dsdat.color = { 0.5f, 0.0f, 1.0f, 0.75f };
            //dsdat.color = { 2.0f, 0.0f, 1.2f, 0.35f };

            dsdat.texNo = m_Tex_Shadow;
            //dsdat.loadTexType = LOADTEXTURETYPE_MAIN;
            dsdat.loadTexType = LOADTEXTURETYPE_GRAY;
            dsdat.shaderOpt.blendState = DPD_SHADER_OPT_BLEND_STATE_ADDCOLOR;

            dsdat.texUV = { 0.0f, 0.0f };
            dsdat.texWH = { 1.0f, 1.0f };
            dsdat.startPos = { 0.0f, 0.0f };
            dsdat.startRot = 0.0f;
            dsdat.startScale = { 1.0f, 1.0f };

            DrawSpriteInCamera(dsdat, false, false,
                true
            );
        }

        DRAW_POLYGON_DAT drawDatBaseTmp = {};
        drawDatBaseTmp.size = MkF3(
            940.0f * m_Obj->scale.x * baseEnterMod * baseAfterMod,
            940.0f * m_Obj->scale.y * baseEnterMod * baseAfterMod,
            940.0f * m_Obj->scale.z * baseEnterMod * baseAfterMod
        );

        drawDatBaseTmp.rot = 0.0f;

        drawDatBaseTmp.rot3D = MkF3(0.0f, 0.0f, 0.0f);

        drawDatBaseTmp.color = MkF4(
            1.0f,
            1.0f,
            1.0f,
            1.0f * baseEnterMod * baseAfterMod
        );

        drawDatBaseTmp.texNo = m_Tex_EnergyBall;
        drawDatBaseTmp.loadTexType = LOADTEXTURETYPE_MAIN;

        drawDatBaseTmp.texUV = AnimationUV(m_Obj->frameActive / 3, 4, 4);
        drawDatBaseTmp.texWH = { 1.0f / 4, 1.0f / 4 };

        drawDatBaseTmp.startPos3D = m_Obj->pos;
        //drawDatBaseTmp.startPos3D.z += -80.0f;

        drawDatBaseTmp.startScale = MkF2(1.0f, 1.0f);
        drawDatBaseTmp.startScale3D = MkF3(1.0f, 1.0f, 1.0f);

        drawDatBaseTmp.modelNo = 0;

        //drawDatBaseTmp.shaderOpt.bloom = DPD_SHADER_OPT_BLOOM_NORMAL;
        //drawDatBaseTmp.shaderOpt.blendState = DPD_SHADER_OPT_BLEND_STATE_ADDCOLOR;

        DRAW_POLYGON_IN_CAMERA_EX_OPTION optBaseTmp = {};
        optBaseTmp.reverse = false;
        optBaseTmp.fixed = false;
        optBaseTmp.enableShade = true;
        optBaseTmp.billboard = true;
        //optBaseTmp.priority = DRAW_POLYGON_PRIORITY_NML_u3;
        //optBaseTmp.priority = DRAW_POLYGON_PRIORITY_NORMAL;
        optBaseTmp.priority = DRAW_POLYGON_PRIORITY_NML_d1;
        optBaseTmp.depthBuffIgnore = true;
        //opt.priority = DRAW_POLYGON_PRIORITY_FORCE_NORMAL;
        optBaseTmp.shaderType = DPD_SHADER_TYPE_EFFECT;

        DrawPolygonInCameraEx(drawDatBaseTmp, optBaseTmp);
    }

    if (m_type == Type::Beam)
    {
        float startMod = 1.0f;
        float endMod = 1.0f;
        {
            int endf = 120;
            if (m_Obj->frameActive < endf)
            {
                startMod = EasingOutElastic((float)m_Obj->frameActive / endf);
            }
        }
        {
            int endf = 40;
            if (m_Obj->frameActive >= m_Obj->lifetimeFrameMax - endf)
            {
                int f = m_Obj->frameActive - (m_Obj->lifetimeFrameMax - endf);
                endMod = 1.0f - (float)f / endf;
            }
        }
        DRAW_POLYGON_DAT drawDatBaseTmp = {};
        //drawDatBaseTmp.size = MkF3(
        //    12.0f * m_Obj->scale.x,
        //    12.0f * m_Obj->scale.y,
        //    1800.0f * m_Obj->scale.z
        //);
        drawDatBaseTmp.size = MkF3(
            12.0f   * m_Obj->scale.x,
            12.0f   * m_Obj->scale.y,
            1800.0f * m_Obj->scale.z
        );

        //drawDatBaseTmp.pos.y += -120.0f - drawDatBaseTmp.size.y / 2;
        //drawDatBaseTmp.pos.z += 30.0f + drawDatBaseTmp.size.z / 2 - (m_Obj->size.x * m_Obj->scale.x + m_Obj->size.y * m_Obj->scale.y) / 4;
        //drawDatBaseTmp.pos.z += 30.0f + drawDatBaseTmp.size.z / 2 - (m_Obj->size.x * m_Obj->scale.x + m_Obj->size.y * m_Obj->scale.y) / 4;

        drawDatBaseTmp.pos.z = drawDatBaseTmp.size.z / 2;

        drawDatBaseTmp.rot = 0.0f;

        drawDatBaseTmp.rot3D = MkF3(0.0f, 0.0f, 0.0f);

        //drawDatTmp.color = MkF4(1.0f, 0.8f, 1.0f, 0.9f);
        drawDatBaseTmp.color = MkF4(1.0f * 2, 0.9f * 2, 1.0f * 2, 0.9f);
        //drawDatTmp.color = MkF4(3.0f, 1.0f, 7.0f, 0.9f);

        //drawDatTmp.texNo = LoadTexture("rom/texture/ui/in_game/charactor_icon.png");
        drawDatBaseTmp.texNo = 0;
        drawDatBaseTmp.loadTexType = LOADTEXTURETYPE_MAIN;

        drawDatBaseTmp.texUV = { 0.0f, 0.0f };
        drawDatBaseTmp.texWH = { 1.0f, 1.0f };

        drawDatBaseTmp.startPos3D = m_Obj->pos;
        //drawDatBaseTmp.startPos3D.z += -80.0f;

        drawDatBaseTmp.startRot = CalculateRadToDeg(atan2(m_InitVel.y, m_InitVel.x)) - 90.0f;
        drawDatBaseTmp.startRot3D = MkF3(90.0f - 45.0f * m_InitVel.z, 0.0f, 0.0f);

        drawDatBaseTmp.startScale = MkF2(1.0f, 1.0f);
        drawDatBaseTmp.startScale3D = MkF3(1.0f, 1.0f, 1.0f);

        drawDatBaseTmp.modelNo = m_Model_Beam_Misile;
        //drawDatBaseTmp.modelNo = m_Model_Beam;

        drawDatBaseTmp.shaderOpt.bloom = DPD_SHADER_OPT_BLOOM_NORMAL;
        //drawDatBaseTmp.shaderOpt.blendState = DPD_SHADER_OPT_BLEND_STATE_ADDCOLOR;

        DRAW_POLYGON_IN_CAMERA_EX_OPTION optBaseTmp = {};
        optBaseTmp.reverse = false;
        optBaseTmp.fixed = false;
        optBaseTmp.enableShade = true;
        optBaseTmp.priority = DRAW_POLYGON_PRIORITY_NML_u3;
        //opt.priority = DRAW_POLYGON_PRIORITY_NORMAL;
        optBaseTmp.depthBuffIgnore = true;
        //opt.priority = DRAW_POLYGON_PRIORITY_FORCE_NORMAL;
        optBaseTmp.shaderType = DPD_SHADER_TYPE_EFFECT;

        if (IsInStage())
        {
            DRAW_SPRITE_DAT dsdat = {};
            dsdat.x = m_Obj->pos.x;
            dsdat.y = m_Obj->pos.y;
            dsdat.z = -8.0f;
            //dsdat.z = -0.005f;
            dsdat.w = 140.0f * m_Obj->scale.x;
            dsdat.h = 140.0f * m_Obj->scale.y;

            {
                Float3 addposvec = NormalizeVector(m_InitVel);

                Float3 addpos = {};
                addpos.x = dsdat.w * 0.33f * addposvec.x;
                addpos.y = dsdat.w * 0.33f * addposvec.y;
                addpos.z = dsdat.w * 0.33f * addposvec.z;

                dsdat.x += addpos.x;
                dsdat.y += addpos.y;
            }

            {
                float mo = 1.0f - std::max(std::min(abs(m_Obj->pos.z) / (ONE_BLOCK * 80.0f), 1.0f), 0.0f);
                dsdat.w *= mo;
                dsdat.h *= mo;
            }

            {
                float hmod = 1.0f - (float)((int)drawDatBaseTmp.startRot3D.x % 90) / 90.0f;
                dsdat.h *= (1.0f + 2.0f * hmod) * startMod * endMod;
            }

            dsdat.rot = drawDatBaseTmp.startRot;
            dsdat.color = { 1.0f, 0.0f, 0.5f, 0.75f };
            //dsdat.color = { 2.0f, 0.0f, 1.2f, 0.35f };

            dsdat.texNo = m_Tex_Shadow;
            //dsdat.loadTexType = LOADTEXTURETYPE_MAIN;
            dsdat.loadTexType = LOADTEXTURETYPE_GRAY;
            dsdat.shaderOpt.blendState = DPD_SHADER_OPT_BLEND_STATE_ADDCOLOR;

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
            GAME_OBJECT* beamObj = m_Obj;
            if (beamObj != nullptr
                && (beamObj->vel.x != 0.0f || beamObj->vel.y != 0.0f))
            {

                {

                    DRAW_POLYGON_DAT drawDatTmp = drawDatBaseTmp;
                    drawDatTmp.size = MkF3(
                        12.0f  * m_Obj->scale.x * 2.5f,
                        12.0f  * m_Obj->scale.y * 2.5f,
                        160.0f * m_Obj->scale.z * 0.3f
                    );

                    drawDatTmp.pos = {};
                    drawDatTmp.pos.z += -drawDatTmp.size.z;

                    drawDatTmp.startRot = CalculateRadToDeg(atan2(beamObj->vel.y, beamObj->vel.x)) + 90.0f;
                    drawDatTmp.startPos3D = beamObj->pos;
                    //drawDatTmp.startPos3D.y += -80.0f;

                    {
                        {
                            float addSizeTmp = sin((float)(beamObj->frameActive) * 0.8f) * 2.0f;
                            drawDatTmp.size.x += addSizeTmp;
                            drawDatTmp.size.y += addSizeTmp;
                        }

                        drawDatTmp.size.z *= startMod;
                        drawDatTmp.size.z *= endMod;

                        DRAW_POLYGON_DAT drawDatTmp2 = drawDatTmp;
                        drawDatTmp2.size.x *= 1.3f;
                        drawDatTmp2.size.y *= 1.3f;
                        //drawDatTmp2.color = MkF4(1.0f, 0.3f, 1.0f, 0.6f);
                        drawDatTmp2.color = MkF4(3.0f, 1.0f, 7.0f, 0.75f);
                        //drawDatTmp2.color = MkF4(3.0f * 2, 1.0f * 2, 7.0f * 2, 0.75f);

                        DRAW_POLYGON_IN_CAMERA_EX_OPTION opt2 = optBaseTmp;
                        opt2.priority = DRAW_POLYGON_PRIORITY_NML_u2;
                        //opt2.priority = DRAW_POLYGON_PRIORITY_NML_d1;


                        DRAW_POLYGON_DAT drawDatTmp3 = drawDatTmp;
                        drawDatTmp3.texNo = m_Tex_Beam;
                        drawDatTmp3.texUV = {
                            0.0f,
                            0.0f + -0.06f * (float)(m_Obj->frameActive / 1)
                        };
                        drawDatTmp3.texWH = {
                            1.0f,
                            1.0f
                        };
                        drawDatTmp3.size.x *= 5.0f;
                        drawDatTmp3.size.y *= 5.0f;
                        drawDatTmp3.color = MkF4(1.8f, 0.f, 2.0f, 0.5f);
                        DRAW_POLYGON_IN_CAMERA_EX_OPTION opt3 = optBaseTmp;
                        opt3.priority = DRAW_POLYGON_PRIORITY_NML_u1;


                        DRAW_POLYGON_DAT drawDatTmp4 = drawDatTmp;

                        drawDatTmp4.size.x *= 3.0f;
                        drawDatTmp4.size.y *= 3.0f;
                        //drawDatTmp3.color = MkF4(0.03f, 0.f, 0.5f, 0.4f);
                        //drawDatTmp4.color = MkF4(0.03f * 2, 0.f, 0.5f * 2, 0.75f);
                        //drawDatTmp3.color = MkF4(0.45f * 2, 0.4f, 1.0f, 0.8f);
                        drawDatTmp4.color = MkF4(0.7f, 0.0f, 0.1f, 0.75f);

                        DRAW_POLYGON_IN_CAMERA_EX_OPTION opt4 = optBaseTmp;
                        opt4.priority = DRAW_POLYGON_PRIORITY_NORMAL;
                        opt4.depthBuffIgnore = true;


                        DrawPolygonInCameraEx(drawDatTmp3, opt3);
                        DrawPolygonInCameraEx(drawDatTmp4, opt4);
                        DrawPolygonInCameraEx(drawDatTmp2, opt2);
                        DrawPolygonInCameraEx(drawDatTmp, optBaseTmp);
                    }
                }

            }
        }
    }

    if (m_type == Type::Granade)
    {
        float sca = (m_Obj->scale.x + m_Obj->scale.y + m_Obj->scale.z) / 3.0f;
        if (m_Init2)
        {
            float enterf = std::min((float)m_Init2Frame / GRANADE_AREA_ENTER_FRAME, 1.0f);
            //DRAW_SPRITE_DAT dsdat = {};
            //dsdat.x = 0.0f;
            //dsdat.z = -5.0f;

            //dsdat.w = GRANADE_EXPLO_AREA * 2;
            //dsdat.h = GRANADE_EXPLO_AREA * 2;

            //dsdat.rot = 0.0f;
            //dsdat.color = { 1.0f, 1.0f, 1.0f, 0.1f };

            //{
            //    float amod = (sin((float)m_Obj->frameActive / 10) + 1.0f) / 2;
            //    dsdat.color.x += amod * 1.5f;
            //    dsdat.color.y += amod * 1.5f;
            //    dsdat.color.z += amod * 1.5f;
            //    dsdat.color.w += amod * 0.2f;
            //}

            //dsdat.texNo = m_TexId_AreaEnemy;
            //dsdat.loadTexType = LOADTEXTURETYPE_MAIN;

            ////dsdat.texUV = { 0.0f, 0.0f };
            ////dsdat.texWH = { 1.0f, 1.0f };
            //dsdat.texUV = AnimationUV(
            //    (int)((m_Obj->frame / 4) % 8),
            //    3,
            //    3
            //);
            //dsdat.texWH = { 1.0f / 3, 1.0f / 3 };

            //dsdat.startPos3D = m_HitPos;
            //dsdat.startPos3D.z += 0.0f;

            //dsdat.startRot = 0.0f;
            //dsdat.startScale = { 1.0f, 1.0f };

            //dsdat.shaderOpt.bloom = DPD_SHADER_OPT_BLOOM_NONE;
            //dsdat.shaderOpt.blendState = DPD_SHADER_OPT_BLEND_STATE_ADDCOLOR;

            //DrawSpriteInCamera(dsdat);

            //for (int i = 0; i < 2; i++)
            {
                DRAW_POLYGON_DAT drawDatTmp = {};
                drawDatTmp.size = MkF3(
                    GRANADE_EXPLO_AREA * 2.0f / 100 * sca * enterf,
                    GRANADE_EXPLO_AREA * 2.0f / 100 * sca * enterf,
                    GRANADE_EXPLO_AREA * 2.0f / 100 * sca * enterf
                );

                drawDatTmp.rot = 0.0f;

                drawDatTmp.rot3D = MkF3(0.0f, 0.0f, 0.0f);

                drawDatTmp.texWH = { 1.0f , 1.0f };

                drawDatTmp.texUV = {
                    0.0f,
                    0.03f * m_Obj->frame
                };
                drawDatTmp.color = MkF4(0.9f, 0.0f, 0.2f, 0.9f);

                //if (i == 1)
                //{
                //    drawDatTmp.texNo = m_Tex_Wave2;
                //    drawDatTmp.texUV.y *= 0.75f;
                //}
                //else
                {
                    drawDatTmp.texNo = m_Tex_AttackArea;

                }

                drawDatTmp.loadTexType = LOADTEXTURETYPE_MAIN;

                //drawDatTmp.texUV = AnimationUV(
                //    (int)((m_Obj->frame / 4) % m_AnimCountMax_AreaEnemy),
                //    (int)m_AnimCount_AreaEnemy.x,
                //    (int)m_AnimCount_AreaEnemy.y
                //);
                //drawDatTmp.texWH = { 1.0f / m_AnimCount_AreaEnemy.x, 1.0f / m_AnimCount_AreaEnemy.y };

                drawDatTmp.startPos3D = m_Obj->pos;
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

                DrawPolygonInCameraEx(drawDatTmp, opt);
            }

            for (int i = 0; i < 3; i++)
            {
                DRAW_POLYGON_DAT drawDatTmp = {};
                drawDatTmp.size = MkF3(
                    400.0f * sca * enterf,
                    400.0f * sca * enterf,
                    400.0f * sca * enterf
                );

                if (i == 2)
                {
                    drawDatTmp.rot = 3.0f * m_Obj->frame;
                    //drawDatTmp.color = MkF4(1.9f, 1.3f, 0.5f, 0.5f);
                    //drawDatTmp.color = MkF4(1.3f, 1.4f, 1.3f, 0.3f);
                    drawDatTmp.color = MkF4(1.7f, 1.7f, 0.5f, 0.7f);
                    //drawDatTmp.shaderOpt.blendState = DPD_SHADER_OPT_BLEND_STATE_DEFAULT;
                    drawDatTmp.shaderOpt.blendState = DPD_SHADER_OPT_BLEND_STATE_ADDCOLOR;
                }
                else if (i == 1)
                {
                    drawDatTmp.rot = -5.0f * m_Obj->frame;
                    //drawDatTmp.color = MkF4(2.0f, 0.2f, 0.1f, 0.7f);
                    //drawDatTmp.color = MkF4(1.7f, 1.7f, 0.5f, 0.5f);
                    //drawDatTmp.color = MkF4(0.9f, 0.2f, 0.1f, 0.7f);
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

                int texno[3] = {
                    m_Tex_cursor_000 ,
                    m_Tex_cursor_010 ,
                    m_Tex_cursor_020 ,
                };
                drawDatTmp.texNo = texno[i];

                drawDatTmp.loadTexType = LOADTEXTURETYPE_MAIN;

                drawDatTmp.startPos3D = m_Obj->pos;
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
        {
            DRAW_POLYGON_DAT drawDatBaseTmp = {};
            drawDatBaseTmp.size = MkF3(
                60.0f * m_Obj->scale.x,
                60.0f * m_Obj->scale.y,
                60.0f * m_Obj->scale.z
            );

            //drawDatBaseTmp.pos.z = drawDatBaseTmp.size.z / 2;

            drawDatBaseTmp.startRot3D = MkF3(0.0f, 0.0f, 0.0f);
            drawDatBaseTmp.startRot3D.x = -90.0f;
            drawDatBaseTmp.startRot3D.y = 0.0f + 180.0f * (m_Init2 ? 1.0f : 0.0f);
            drawDatBaseTmp.startRot3D.z = (m_Init2 ? 70.0f: -70.0f);

            drawDatBaseTmp.color = MkF4(1.0f, 1.0f, 1.0f, 1.0f);
            //drawDatBaseTmp.color.x += 8.0f * (1.0f - sin((float)m_Obj->frame * 0.7f));
                    //drawDatBaseTmp.color.x += 8.0f * (sin((float)m_Obj->frame * 0.7f) / 2.0f + 0.5f);
            drawDatBaseTmp.color.x += 8.0f * (sin((float)m_Obj->frame / 8) / 2.0f + 0.5f);
            drawDatBaseTmp.color.y = drawDatBaseTmp.color.x;
            drawDatBaseTmp.color.z = drawDatBaseTmp.color.x;

            drawDatBaseTmp.texNo = m_Tex_Misile_Model;
            drawDatBaseTmp.loadTexType = LOADTEXTURETYPE_MAIN;

            drawDatBaseTmp.texUV = { 0.0f, 0.0f };
            drawDatBaseTmp.texWH = { 1.0f, 1.0f };

            drawDatBaseTmp.startPos3D = m_Obj->pos;
            //drawDatBaseTmp.startPos3D.z += -80.0f;

            //drawDatBaseTmp.startRot = CalculateRadToDeg(atan2(m_InitVel.y, m_InitVel.x)) - 90.0f;
            //drawDatBaseTmp.startRot3D = MkF3(90.0f - 45.0f * m_InitVel.z, 0.0f, 0.0f);

            drawDatBaseTmp.startScale = MkF2(1.0f, 1.0f);
            drawDatBaseTmp.startScale3D = MkF3(1.0f, 1.0f, 1.0f);

            drawDatBaseTmp.modelNo = m_Model_Misile;
            //drawDatBaseTmp.modelNo = m_Model_Beam;

            drawDatBaseTmp.shaderOpt.bloom = DPD_SHADER_OPT_BLOOM_ALWAYS;
            //drawDatBaseTmp.shaderOpt.blendState = DPD_SHADER_OPT_BLEND_STATE_ADDCOLOR;
            drawDatBaseTmp.shaderOpt.outline = DPD_SHADER_OPT_OUTLINE_NORMAL;

            DRAW_POLYGON_IN_CAMERA_EX_OPTION optBaseTmp = {};
            optBaseTmp.reverse = false;
            optBaseTmp.fixed = false;
            optBaseTmp.priority = DRAW_POLYGON_PRIORITY_NORMAL;
            optBaseTmp.shaderType = DPD_SHADER_TYPE_METAL;

            {
                DRAW_POLYGON_DAT drawDatTmp = drawDatBaseTmp;
                drawDatTmp.texNo = m_Tex_Misile_Fire_Model;
                drawDatTmp.modelNo = m_Model_Fire_Misile;
                drawDatTmp.pos.y += drawDatBaseTmp.size.y * -6.0f;
                drawDatTmp.texUV.y += 0.1f * m_Obj->frameActive;
                drawDatTmp.shaderOpt.blendState = DPD_SHADER_OPT_BLEND_STATE_ADDCOLOR;
                drawDatTmp.shaderOpt.outline = DPD_SHADER_OPT_OUTLINE_NONE;
                DRAW_POLYGON_IN_CAMERA_EX_OPTION optTmp = optBaseTmp;
                optTmp.shaderType = DPD_SHADER_TYPE_NONE;
                if (m_NumFlag[1] == 1)
                {
                    drawDatTmp.color = MkF4(0.5f, 0.6f, 2.0f, 1.0f);
                }
                DrawPolygonInCameraEx(drawDatTmp, optTmp);
            }

            int drawNum = 1;
            if (m_NumFlag[1] == 1)
            {
                drawNum = 2;
            }

            for (int i = 0; i < drawNum; i++)
            {
                DRAW_POLYGON_DAT drawDatTmp = drawDatBaseTmp;
                DRAW_POLYGON_IN_CAMERA_EX_OPTION optTmp = optBaseTmp;
                if (m_NumFlag[1] == 1)
                {
                    switch (i)
                    {
                    case 0:
                        drawDatTmp.texNo = 0;
                        drawDatTmp.shaderOpt.blendState = DPD_SHADER_OPT_BLEND_STATE_DEFAULT;
                        drawDatTmp.color = MkF4(
                            0.2f,
                            0.0f,
                            0.1f,
                            0.7f);
                        optTmp.priority = (DRAW_POLYGON_PRIORITY)(optTmp.priority - 1);
                        optTmp.depthBuffIgnore = true;
                        optTmp.shaderType = DPD_SHADER_TYPE_EFFECT;

                        break;
                    default:
                        drawDatTmp.texUV.x = (float)m_Obj->frameActive * 0.012f;
                        drawDatTmp.texUV.y = (float)m_Obj->frameActive * 0.02f;
                        drawDatTmp.texWH.x = 0.7f;
                        drawDatTmp.texWH.y = 0.7f;
                        drawDatTmp.texNo = m_Tex_Beam;
                        drawDatTmp.shaderOpt.blendState = DPD_SHADER_OPT_BLEND_STATE_ADDCOLOR;

                        drawDatTmp.color = MkF4(
                            1.00f,
                            0.30f,
                            1.30f,
                            1.0f);
                        optTmp.priority = (DRAW_POLYGON_PRIORITY)(optTmp.priority + 1);
                        optTmp.depthBuffIgnore = false;
                        optTmp.shaderType = DPD_SHADER_TYPE_EFFECT;
                        break;
                    }
                }
                DrawPolygonInCameraEx(drawDatTmp, optTmp);
            }

            //{
            //    DRAW_POLYGON_DAT drawDat2Tmp = drawDatBaseTmp;
            //    drawDat2Tmp.size = MkF3(
            //        20.0f * m_Obj->scale.x,
            //        20.0f * m_Obj->scale.y,
            //        150.0f * m_Obj->scale.z
            //    );
            //    drawDat2Tmp.modelNo = m_Model_Beam;
            //    drawDat2Tmp.pos.z = -drawDat2Tmp.size.z * 3.5f;
            //    drawDat2Tmp.texNo = m_Tex_Misile_Fire;
            //    drawDat2Tmp.texUV = AnimationUV(
            //        m_Obj->frameActive / 2,
            //        4,
            //        4
            //    );
            //    drawDat2Tmp.texWH = {
            //        +1.0f / 4,
            //        -1.0f / 4
            //    };

            //    DRAW_POLYGON_IN_CAMERA_EX_OPTION opt2Tmp = optBaseTmp;
            //    DrawPolygonInCameraEx(drawDat2Tmp, opt2Tmp);
            //}
        }

        //{
        //    DRAW_POLYGON_DAT drawDatBaseTmp = {};
        //    drawDatBaseTmp.size = MkF3(
        //        640.0f * m_Obj->scale.x,
        //        640.0f * m_Obj->scale.y,
        //        640.0f * m_Obj->scale.z
        //    );

        //    drawDatBaseTmp.rot = -90.0f + 180.0f * (m_Init2 ? 1.0f: 0.0f);

        //    drawDatBaseTmp.rot3D = MkF3(0.0f, 0.0f, 0.0f);

        //    drawDatBaseTmp.color = MkF4(1.0f, 1.0f, 1.0f, 1.0f);

        //    drawDatBaseTmp.texNo = m_Tex_Misile_Main;
        //    drawDatBaseTmp.loadTexType = LOADTEXTURETYPE_MAIN;

        //    drawDatBaseTmp.texUV = AnimationUV(0, 3, 3);
        //    drawDatBaseTmp.texWH = { 1.0f / 3, 1.0f / 3 };

        //    drawDatBaseTmp.startPos3D = m_Obj->pos;
        //    //drawDatBaseTmp.startPos3D.z += -80.0f;

        //    drawDatBaseTmp.startRot3D.x = 80.0f;

        //    drawDatBaseTmp.startScale = MkF2(1.0f, 1.0f);
        //    drawDatBaseTmp.startScale3D = MkF3(1.0f, 1.0f, 1.0f);

        //    drawDatBaseTmp.modelNo = 0;

        //    //drawDatBaseTmp.shaderOpt.bloom = DPD_SHADER_OPT_BLOOM_NORMAL;
        //    //drawDatBaseTmp.shaderOpt.blendState = DPD_SHADER_OPT_BLEND_STATE_ADDCOLOR;

        //    DRAW_POLYGON_IN_CAMERA_EX_OPTION optBaseTmp = {};
        //    optBaseTmp.reverse = false;
        //    optBaseTmp.fixed = false;
        //    optBaseTmp.enableShade = true;
        //    optBaseTmp.billboard = false;
        //    //optBaseTmp.priority = DRAW_POLYGON_PRIORITY_NML_u3;
        //    optBaseTmp.priority = DRAW_POLYGON_PRIORITY_NORMAL;
        //    //optBaseTmp.depthBuffIgnore = true;
        //    //opt.priority = DRAW_POLYGON_PRIORITY_FORCE_NORMAL;

        //    DrawPolygonInCameraEx(drawDatBaseTmp, optBaseTmp);
        //}
        
        //Float3 velRot = {};
        //{
        //	Float3 vec = m_InitVel;

        //	Float2 vecH = NormalizeVector(Float2(vec.x, vec.z));
        //	float vecHRad = atan2(vecH.x, vecH.y);

        //	velRot = Float3(0.0f);
        //	velRot.y = CalculateRadToDeg(vecHRad) + 0.0f;
        //	velRot.x = -90.0f * vec.y + 90.0f;
        //}

        //DRAW_POLYGON_DAT drawDatBaseTmp = {};
        //drawDatBaseTmp.rot = 0.0f;

        ////drawDatBaseTmp.rot3D = MkF3(0.0f, 0.0f, 0.0f);
        ////drawDatBaseTmp.rot3D = velRot;
        ////drawDatBaseTmp.rot3D.x = velRot.x;

        ////drawDatTmp.color = MkF4(1.0f, 0.8f, 1.0f, 0.9f);
        //drawDatBaseTmp.color = MkF4(1.0f * 2, 0.9f * 2, 1.0f * 2, 0.9f);
        ////drawDatTmp.color = MkF4(3.0f, 1.0f, 7.0f, 0.9f);

        ////drawDatTmp.texNo = LoadTexture("rom/texture/ui/in_game/charactor_icon.png");
        //drawDatBaseTmp.texNo = m_Tex_Beam;
        ////drawDatBaseTmp.texNo = 0;
        //drawDatBaseTmp.loadTexType = LOADTEXTURETYPE_MAIN;

        //drawDatBaseTmp.texUV = { 0.0f, 0.0f };
        //drawDatBaseTmp.texWH = { 1.0f, 1.0f };

        //drawDatBaseTmp.startPos3D = m_Obj->pos;
        ////drawDatBaseTmp.startPos3D.y += -80.0f;

        ////drawDatBaseTmp.startRot = CalculateRadToDeg(atan2(m_BeamShootVec.y, m_BeamShootVec.x)) - 90.0f;
        //drawDatBaseTmp.startRot3D = velRot;

        //drawDatBaseTmp.startScale = MkF2(1.0f, 1.0f);
        //drawDatBaseTmp.startScale3D = MkF3(1.0f, 1.0f, 1.0f);

        //drawDatBaseTmp.modelNo = m_Model_Beam;

        //drawDatBaseTmp.shaderOpt.bloom = true;
        ////drawDatBaseTmp.shaderOpt.blendState = DPD_SHADER_OPT_BLEND_STATE_ADDCOLOR;

        //DRAW_POLYGON_IN_CAMERA_EX_OPTION optBaseTmp = {};
        //optBaseTmp.reverse = false;
        //optBaseTmp.fixed = false;
        //optBaseTmp.enableShade = true;
        //optBaseTmp.priority = DRAW_POLYGON_PRIORITY_NML_u3;
        ////opt.priority = DRAW_POLYGON_PRIORITY_NORMAL;
        //optBaseTmp.depthBuffIgnore = true;
        ////opt.priority = DRAW_POLYGON_PRIORITY_FORCE_NORMAL;

        //{
        //	{
        //		DRAW_POLYGON_DAT drawDatTmp = drawDatBaseTmp;
        //		drawDatTmp.size = MkF3(18.0f, 240.0f, 18.0f);

        //		drawDatTmp.pos = Float3(0.0f);
        //		drawDatTmp.pos.y += drawDatTmp.size.y;

        //		{
        //			//{
        //			//	float addSizeTmp = sin((float)(beamObj->frameActive) * 0.8f) * 2.0f;
        //			//	drawDatTmp.size.x += addSizeTmp;
        //			//	drawDatTmp.size.y += addSizeTmp;
        //			//}

        //			//{
        //			//	int endf = 120;
        //			//	if (beamObj->frameActive < endf)
        //			//	{
        //			//		float startMod = EasingOutElastic((float)beamObj->frameActive / endf);
        //			//		//drawDatTmp.size.x *= startMod;
        //			//		//drawDatTmp.size.y *= startMod;
        //			//		drawDatTmp.size.z *= startMod;
        //			//	}
        //			//}
        //			//{
        //			//	int endf = 40;
        //			//	if (beamObj->frameActive >= FRAME_BEAM_LIFETIME - endf)
        //			//	{
        //			//		int f = beamObj->frameActive - (FRAME_BEAM_LIFETIME - endf);
        //			//		float endMod = 1.0f - (float)f / endf;
        //			//		//drawDatTmp.size.x *= endMod;
        //			//		//drawDatTmp.size.y *= endMod;
        //			//		drawDatTmp.size.z *= endMod;
        //			//	}
        //			//}

        //			DRAW_POLYGON_DAT drawDatTmp2 = drawDatTmp;
        //			drawDatTmp2.size.x *= 1.3f;
        //			drawDatTmp2.size.z *= 1.3f;
        //			//drawDatTmp2.color = MkF4(1.0f, 0.3f, 1.0f, 0.6f);
        //			drawDatTmp2.color = MkF4(3.0f, 1.0f, 7.0f, 0.75f);
        //			//drawDatTmp2.color = MkF4(3.0f * 2, 1.0f * 2, 7.0f * 2, 0.75f);
        //			DRAW_POLYGON_IN_CAMERA_EX_OPTION opt2 = optBaseTmp;
        //			opt2.priority = DRAW_POLYGON_PRIORITY_NML_u2;
        //			//opt2.priority = DRAW_POLYGON_PRIORITY_NML_d1;


        //			DRAW_POLYGON_DAT drawDatTmp3 = drawDatTmp;
        //			//drawDatTmp3.texNo = m_Tex_Beam;
        //			///drawDatTmp3.texNo = 0;
        //			drawDatTmp3.texUV = {
        //				0.0f,
        //				0.0f + -0.06f * (float)(m_Obj->frameActive / 1)
        //			};
        //			drawDatTmp3.texWH = {
        //				1.0f * 0.5f,
        //				800.0f
        //			};
        //			drawDatTmp3.size.x *= 5.0f;
        //			drawDatTmp3.size.z *= 5.0f;
        //			drawDatTmp3.color = MkF4(1.8f, 0.f, 2.0f, 0.5f);
        //			DRAW_POLYGON_IN_CAMERA_EX_OPTION opt3 = optBaseTmp;
        //			opt3.priority = DRAW_POLYGON_PRIORITY_NML_u1;


        //			DRAW_POLYGON_DAT drawDatTmp4 = drawDatTmp;

        //			drawDatTmp4.size.x *= 3.0f;
        //			drawDatTmp4.size.z *= 3.0f;
        //			//drawDatTmp3.color = MkF4(0.03f, 0.f, 0.5f, 0.4f);
        //			drawDatTmp4.color = MkF4(0.03f * 2, 0.f, 0.5f * 2, 0.75f);
        //			//drawDatTmp3.color = MkF4(0.45f * 2, 0.4f, 1.0f, 0.8f);
        //			DRAW_POLYGON_IN_CAMERA_EX_OPTION opt4 = optBaseTmp;
        //			opt4.priority = DRAW_POLYGON_PRIORITY_NORMAL;


        //			DrawPolygonInCameraEx(drawDatTmp3, opt3);
        //			DrawPolygonInCameraEx(drawDatTmp4, opt4);
        //			DrawPolygonInCameraEx(drawDatTmp2, opt2);
        //			DrawPolygonInCameraEx(drawDatTmp, optBaseTmp);
        //		}
        //	}
        //}

        //if (m_Beam_Objects.size() > 0)
        //{
        //	for (const auto& beamObjIde : m_Beam_Objects)
        //	{
        //		GAME_OBJECT* beamObj = GetGameObjectFindByIdentifier(beamObjIde.second);
        //		if (beamObj != nullptr
        //			&& (beamObj->vel.x != 0.0f || beamObj->vel.y != 0.0f))
        //		{


        //		}
        //	}
        //}
    }


    if (m_type == Type::ExplosionShockWave)
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
        optBaseTmp.shaderType = DPD_SHADER_TYPE_EFFECT;

        {
            GAME_OBJECT* beamObj = m_Obj;
            if (beamObj != nullptr
                && (beamObj->vel.x != 0.0f || beamObj->vel.y != 0.0f))
            {
                Float2 sizesca = { 1.0f, 1.0f };
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
                    drawDatTmp.texNo = m_ColorSetFlag ? m_Tex_ShockWaveFlow_Gray : m_Tex_ShockWaveFlow;
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
                    drawDatTmp2.texNo = m_ColorSetFlag ? m_Tex_ShockWaveSprite_Gray : m_Tex_ShockWaveSprite;
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
                    //drawDatTmp2.pos.z = drawDatTmp2.size.z * -0.48f;
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

    if (m_type == Type::EnergyBallBomb)
    {
        int BOMB_EXPLOSION_START_FRAME = m_NumFlag[0];
        int BOMB_AREA = m_NumFlag[1];
        int BOMB_EXPLOSION_START_BEFORE_FRAME = m_NumFlag[2];
        int BOMB_EXPLOSION_ATTACK_FRAME = m_NumFlag[3];
        int BOMB_EXPLOSION_STARTING_FRAME = m_NumFlag[4];
        int BOMB_EXPLOSION_AFTER_FRAME = m_NumFlag[5];
        

        int bombFrame = m_Obj->frameActive;

        float enterf = std::min(std::max(
            (float)(bombFrame - 30) / 12,
            0.0f), 1.0f);
        float endf = 1.0f;

        if (bombFrame <= BOMB_EXPLOSION_START_FRAME)
        {
            {
                DRAW_POLYGON_DAT drawDatBaseTmp = {};
                drawDatBaseTmp.size = MkF3(
                    1000.0f * m_Obj->scale.x,
                    1000.0f * m_Obj->scale.y,
                    1000.0f * m_Obj->scale.z
                );

                drawDatBaseTmp.rot = 0.0f;

                drawDatBaseTmp.rot3D = MkF3(0.0f, 0.0f, 0.0f);

                drawDatBaseTmp.color = MkF4(1.0f, 1.0f, 1.0f, 1.0f 
                    //* m_MovieVisibleAlpha
                );

                drawDatBaseTmp.texNo = m_Tex_EnergyBall;
                drawDatBaseTmp.loadTexType = LOADTEXTURETYPE_MAIN;

                drawDatBaseTmp.texUV = AnimationUV(m_Obj->frameActive / 3, 4, 4);
                drawDatBaseTmp.texWH = { 1.0f / 4, 1.0f / 4 };

                drawDatBaseTmp.startPos3D = m_Obj->pos;

                drawDatBaseTmp.startScale = MkF2(1.0f, 1.0f);
                drawDatBaseTmp.startScale3D = MkF3(1.0f, 1.0f, 1.0f);

                drawDatBaseTmp.modelNo = 0;

                //drawDatBaseTmp.shaderOpt.bloom = DPD_SHADER_OPT_BLOOM_NORMAL;
                //drawDatBaseTmp.shaderOpt.blendState = DPD_SHADER_OPT_BLEND_STATE_ADDCOLOR;

                DRAW_POLYGON_IN_CAMERA_EX_OPTION optBaseTmp = {};
                optBaseTmp.reverse = false;
                optBaseTmp.fixed = false;
                optBaseTmp.enableShade = true;
                optBaseTmp.billboard = true;
                //optBaseTmp.priority = DRAW_POLYGON_PRIORITY_NML_u3;
                optBaseTmp.priority = DRAW_POLYGON_PRIORITY_NORMAL;
                //optBaseTmp.depthBuffIgnore = true;
                //opt.priority = DRAW_POLYGON_PRIORITY_FORCE_NORMAL;
                optBaseTmp.shaderType = DPD_SHADER_TYPE_EFFECT;

                DrawPolygonInCameraEx(drawDatBaseTmp, optBaseTmp);

                { //ショックウェーブ
                    DRAW_POLYGON_DAT ddshok = drawDatBaseTmp;
                    ddshok.pos = {};
                    ddshok.rot3D.x = -90.0f;
                    ddshok.size = MkF3(
                        drawDatBaseTmp.size.x * 0.75f,
                        drawDatBaseTmp.size.z * 0.15, //0.35f,
                        drawDatBaseTmp.size.y * 0.75f
                    );

                    ddshok.startPos3D.z = 0;
                    ddshok.texNo = m_Tex_EbBomb_ShockWave;
                    ddshok.modelNo = m_Model_EbBomb_ShockWave;
                    ddshok.texUV = {
                        0.0f + -0.002f * m_Obj->frameActive,
                        0.1f
                    };
                    //ddshok.texWH = {
                    //    1.0f,
                    //    3.0f
                    //};
                    //ddshok.texWH = {
                    //    1.0f,
                    //    2.0f
                    //};
                    ddshok.texWH = {
                        2.0f,
                        3.0f
                    };
                    ddshok.color = MkF4(0.5f, 0.2f, 0.7f, 1.0f
                        //* m_MovieVisibleAlpha
                    );
                    ddshok.shaderOpt.blendState = DPD_SHADER_OPT_BLEND_STATE_ADDCOLOR;
                    DRAW_POLYGON_IN_CAMERA_EX_OPTION optshok = optBaseTmp;
                    optshok.billboard = false;
                    optshok.priority = DRAW_POLYGON_PRIORITY_NML_u1;
                    DrawPolygonInCameraEx(ddshok, optshok);
                }
            }
            //for (int i = 0; i < 2; i++)
            {
                DRAW_POLYGON_DAT drawDatTmp = {};
                drawDatTmp.size = MkF3(
                    BOMB_AREA * 2.0f / 100 * enterf,
                    BOMB_AREA * 2.0f / 100 * enterf,
                    BOMB_AREA * 2.0f / 100 * enterf
                );

                drawDatTmp.rot = 0.0f;

                drawDatTmp.rot3D = MkF3(0.0f, 0.0f, 0.0f);

                drawDatTmp.texWH = { 1.0f , 1.0f };

                drawDatTmp.texUV = {
                    0.0f,
                    0.03f * m_Obj->frame
                };
                //drawDatTmp.color = MkF4(0.9f, 0.0f, 0.2f, 0.9f);
                drawDatTmp.color = MkF4(1.0f, 0.0f, 1.2f, 1.0f
                    //* m_MovieVisibleAlpha
                    );

                //if (i == 1)
                //{
                //    drawDatTmp.texNo = m_Tex_Wave2;
                //    drawDatTmp.texUV.y *= 0.75f;
                //}
                //else
                {
                    drawDatTmp.texNo = m_Tex_AttackArea;

                }

                drawDatTmp.loadTexType = LOADTEXTURETYPE_MAIN;

                //drawDatTmp.texUV = AnimationUV(
                //    (int)((m_Obj->frame / 4) % m_AnimCountMax_AreaEnemy),
                //    (int)m_AnimCount_AreaEnemy.x,
                //    (int)m_AnimCount_AreaEnemy.y
                //);
                //drawDatTmp.texWH = { 1.0f / m_AnimCount_AreaEnemy.x, 1.0f / m_AnimCount_AreaEnemy.y };

                drawDatTmp.startPos3D = m_Obj->pos;
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

                DrawPolygonInCameraEx(drawDatTmp, opt);
            }
        }

        //if (obj->frameActive <= BOMB_EXPLOSION_FRAME)
        {
            DRAW_POLYGON_DAT drawDatBaseTmp = {};
            drawDatBaseTmp.size = MkF3(
                BOMB_AREA / 3 * 0.7f,
                BOMB_AREA / 3 * 0.7f,
                4000.0f
            );

            drawDatBaseTmp.rot = 0.0f;

            drawDatBaseTmp.rot3D = MkF3(0.0f, 0.0f, 0.0f);

            drawDatBaseTmp.color = MkF4(
                1.2f, 0.8f, 1.2f, 
                1.0f
                //* m_MovieVisibleAlpha
            );
            drawDatBaseTmp.texNo = 0;
            drawDatBaseTmp.loadTexType = LOADTEXTURETYPE_MAIN;

            drawDatBaseTmp.texUV = { 0.0f, 0.0f };
            drawDatBaseTmp.texWH = { 1.0f, 1.0f };

            drawDatBaseTmp.startPos3D = m_Obj->pos;
            //drawDatBaseTmp.startPos3D.z = drawDatBaseTmp.size.z * 0.465f;
            drawDatBaseTmp.startPos3D.z = drawDatBaseTmp.size.z * 0.466f;

            drawDatBaseTmp.startRot3D = MkF3(0.0f, 0.0f, 0.0f);

            drawDatBaseTmp.startScale = MkF2(1.0f, 1.0f);
            drawDatBaseTmp.startScale3D = MkF3(1.0f, 1.0f, 1.0f);

            drawDatBaseTmp.modelNo = m_Model_Beam;

            //drawDatBaseTmp.shaderOpt.bloom = DPD_SHADER_OPT_BLOOM_NORMAL;
            drawDatBaseTmp.shaderOpt.blendState = DPD_SHADER_OPT_BLEND_STATE_ADDCOLOR;

            DRAW_POLYGON_IN_CAMERA_EX_OPTION optBaseTmp = {};
            optBaseTmp.reverse = false;
            optBaseTmp.fixed = false;
            optBaseTmp.priority = DRAW_POLYGON_PRIORITY_NML_u1;
            optBaseTmp.shaderType = DPD_SHADER_TYPE_EFFECT;

            if (bombFrame >= BOMB_EXPLOSION_START_BEFORE_FRAME)
            {

                DRAW_POLYGON_DAT drawDatTmp = drawDatBaseTmp;

                Float2 scaleAddTmp = MkF2(0.0f, 0.0f);
                Float2 scaleMulTmp = MkF2(1.0f, 1.0f);

                bool isStart = bombFrame >= BOMB_EXPLOSION_START_FRAME;
                if (isStart)
                {
                    {
                        //float addSizeTmp = sin((float)(m_Obj->frameActive) * 0.8f) * 0.05f;
                        float addSizeTmp = sin((float)(m_Obj->frameActive) * 0.8f) * 20.0f;
                        scaleAddTmp.x += addSizeTmp;
                        scaleAddTmp.y += addSizeTmp;
                        //drawDatTmp.size.z += addSizeTmp;
                    }

                    {
                        int f = bombFrame - BOMB_EXPLOSION_START_FRAME - BOMB_EXPLOSION_ATTACK_FRAME;
                        if (f >= 0)
                        {
                            float mod =
                                (1.0f - std::max(std::min((float)f / BOMB_EXPLOSION_AFTER_FRAME, 1.0f), 0.0f));
                            scaleMulTmp.x *= mod;
                            scaleMulTmp.y *= mod;

                            endf = mod;
                            //drawDatTmp.size.z *= startMod;
                        }
                    }
                    {
                        int f = bombFrame - BOMB_EXPLOSION_START_FRAME;
                        //if (f >= 0)
                        {
                            float mod =
                                EasingOutElastic(std::max(std::min((float)f / BOMB_EXPLOSION_STARTING_FRAME, 1.0f), 0.0f));
                            scaleMulTmp.x *= mod;
                            scaleMulTmp.y *= mod;
                            //drawDatTmp.size.z *= endMod;
                        }
                        //else
                        //{
                        //    //仮
                        //    if (beamFrame % 15 == 0)
                        //    {
                        //        CameraShakeRequestAll(CAMERA_SHAKE_MIDIUM_FRAME, CAMERA_SHAKE_LARGE_POWER);
                        //    }
                        //}
                    }
                }

                {
                    int f = bombFrame - BOMB_EXPLOSION_START_BEFORE_FRAME;
                    if (f >= 0 && f < (BOMB_EXPLOSION_START_FRAME - BOMB_EXPLOSION_START_BEFORE_FRAME))
                    {
                        float mod = std::max(std::min((float)f / 16, 1.0f), 0.0f);
                        drawDatTmp.size.x = 20.0f;
                        drawDatTmp.size.y = 20.0f;
                        drawDatTmp.size.x *= mod;
                        drawDatTmp.size.y *= mod;
                    }
                    //else
                    //{
                    //    //仮
                    //    if (beamFrame % 15 == 0)
                    //    {
                    //        CameraShakeRequestAll(CAMERA_SHAKE_MIDIUM_FRAME, CAMERA_SHAKE_LARGE_POWER);
                    //    }
                    //}
                }
                drawDatTmp.size.x += scaleAddTmp.x;
                drawDatTmp.size.y += scaleAddTmp.y;
                drawDatTmp.size.x *= scaleMulTmp.x;
                drawDatTmp.size.y *= scaleMulTmp.y;
                //DRAW_POLYGON_DAT drawDatTmp2 = drawDatTmp;
                //drawDatTmp2.size.x *= 1.3f;
                //drawDatTmp2.size.y *= 1.3f;
                //drawDatTmp2.color = MkF4(0.3f, 0.0f, 0.5f, 0.5f);
                //DRAW_POLYGON_IN_CAMERA_EX_OPTION opt2 = optBaseTmp;
                //opt2.priority = DRAW_POLYGON_PRIORITY_NML_u1;

                if (isStart)
                {
                    {
                        DRAW_POLYGON_DAT drawDatTmp3 = drawDatBaseTmp;
                        drawDatTmp3.texNo = m_Tex_EbBomb_ShockWave;
                        drawDatTmp3.texUV = {
                            0.0f + -0.003f * m_Obj->frameActive,
                            0.0f// + 0.022f * (float)(obj->frameActive / 1)
                        };
                        //drawDatTmp3.texWH = {
                        //    2.0f,
                        //    2.0f
                        //};
                        drawDatTmp3.texWH = {
                            0.5f,
                            0.5f
                        };
                        {
                            //drawDatTmp3.size.x = BOMB_AREA / 3 * 0.6f;
                            //drawDatTmp3.size.y = BOMB_AREA / 3 * 0.6f;
                            drawDatTmp3.size.x = BOMB_AREA / 3 * 0.5f;
                            drawDatTmp3.size.y = BOMB_AREA / 3 * 0.5f;
                            drawDatTmp3.size.x += scaleAddTmp.x;
                            drawDatTmp3.size.y += scaleAddTmp.y;
                            drawDatTmp3.size.x *= scaleMulTmp.x;
                            drawDatTmp3.size.y *= scaleMulTmp.y;

                        }
                        drawDatTmp3.color = MkF4(0.55f, 0.25f, 0.7f, 1.0f 
                            //* m_MovieVisibleAlpha
                        );
                        DRAW_POLYGON_IN_CAMERA_EX_OPTION opt3 = optBaseTmp;
                        //opt3.priority = DRAW_POLYGON_PRIORITY_NML_u3;
                        opt3.depthBuffIgnore = true;
                        opt3.priority = DRAW_POLYGON_PRIORITY_NML_d1;


                        DRAW_POLYGON_DAT drawDatTmp4 = drawDatBaseTmp;
                        drawDatTmp4.texNo = m_Tex_Beam1;
                        drawDatTmp4.texUV = {
                            0.0f + -0.0002f * m_Obj->frameActive,
                            0.0f + 0.026f * m_Obj->frameActive
                        };
                        //drawDatTmp4.texWH = {
                        //    2.0f,
                        //    2.0f
                        //};
                        drawDatTmp4.texWH = {
                            1.0f,
                            2.0f
                        };
                        {
                            //drawDatTmp4.size.x = BOMB_AREA / 3 * 0.8f;
                            //drawDatTmp4.size.y = BOMB_AREA / 3 * 0.8f;
                            drawDatTmp4.size.x = BOMB_AREA / 3 * 0.9f;
                            drawDatTmp4.size.y = BOMB_AREA / 3 * 0.9f;
                            drawDatTmp4.size.x += scaleAddTmp.x;
                            drawDatTmp4.size.y += scaleAddTmp.y;
                            drawDatTmp4.size.x *= scaleMulTmp.x;
                            drawDatTmp4.size.y *= scaleMulTmp.y;
                        }
                        //drawDatTmp4.color = MkF4(0.5f, 0.1f, 0.4f, 1.0f);
                        drawDatTmp4.color = MkF4(0.6f, 0.2f, 0.45f, 1.0f 
                            //* m_MovieVisibleAlpha
                        );
                        DRAW_POLYGON_IN_CAMERA_EX_OPTION opt4 = optBaseTmp;
                        opt4.priority = DRAW_POLYGON_PRIORITY_NML_u2;


                        DRAW_POLYGON_DAT drawDatTmp5 = drawDatBaseTmp;
                        //drawDatTmp5.texNo = m_Tex_Beam3;
                        drawDatTmp5.texNo = m_Tex_Beambg;
                        drawDatTmp5.texUV = {
                            0.0f,
                            0.0f + 0.036f * m_Obj->frameActive
                        };
                        //drawDatTmp5.texWH = {
                        //    3.0f,
                        //    2.0f
                        //};
                        //drawDatTmp5.texWH = {
                        //    2.0f,
                        //    2.0f
                        //};
                        //drawDatTmp5.texWH = {
                        //    1.0f,
                        //    2.0f
                        //};
                        drawDatTmp5.texWH = {
                            2.0f,
                            3.0f
                        };
                        {
                            drawDatTmp5.size.x = BOMB_AREA / 3 * 1.1f;
                            drawDatTmp5.size.y = BOMB_AREA / 3 * 1.1f;
                            drawDatTmp5.size.x += scaleAddTmp.x;
                            drawDatTmp5.size.y += scaleAddTmp.y;
                            drawDatTmp5.size.x *= scaleMulTmp.x;
                            drawDatTmp5.size.y *= scaleMulTmp.y;
                        }
                        //drawDatTmp5.color = MkF4(0.6f, 0.1f, 0.9f, 1.0f);
                        //drawDatTmp5.color = MkF4(0.3f, 0.1f, 0.5f, 1.0f);
                        //drawDatTmp5.color = MkF4(0.75f, 0.2f, 0.4f, 0.9f);
                        drawDatTmp5.color = MkF4(0.3f, 0.1f, 0.55f, 1.0f 
                            //* m_MovieVisibleAlpha
                        );
                        DRAW_POLYGON_IN_CAMERA_EX_OPTION opt5 = optBaseTmp;
                        opt5.priority = DRAW_POLYGON_PRIORITY_NML_u4;

                        { //ショックウェーブ
                            DRAW_POLYGON_DAT ddshok = drawDatBaseTmp;
                            ddshok.pos = {};
                            ddshok.rot3D.x = -90.0f;
                            //ddshok.size = MkF3(
                            //    BOMB_AREA * 0.9f,
                            //    300.0f,
                            //    BOMB_AREA * 0.9f
                            //);
                            ddshok.size = MkF3(
                                BOMB_AREA * 1.0f,
                                300.0f,
                                BOMB_AREA * 1.0f
                            );
                            ddshok.size.x += scaleAddTmp.x;
                            ddshok.size.z += scaleAddTmp.y;
                            ddshok.size.x *= scaleMulTmp.x;
                            ddshok.size.z *= scaleMulTmp.y;

                            ddshok.startPos3D.z = 0;
                            ddshok.texNo = m_Tex_EbBomb_ShockWave;
                            ddshok.modelNo = m_Model_EbBomb_ShockWave;
                            ddshok.texUV = {
                                0.0f + -0.003f * m_Obj->frameActive,
                                0.1f
                            };
                            ddshok.texWH = {
                                2.0f,
                                3.0f
                            };
                            ddshok.color = MkF4(0.4f, 0.1f, 1.0f, 1.0f 
                                //* m_MovieVisibleAlpha
                                );
                            DRAW_POLYGON_IN_CAMERA_EX_OPTION optshok = optBaseTmp;
                            optshok.priority = DRAW_POLYGON_PRIORITY_NML_u1;
                            DrawPolygonInCameraEx(ddshok, optshok);
                        }
                        {
                            DRAW_POLYGON_DAT drawDatTmp6 = drawDatTmp5;
                            drawDatTmp6.size.x = BOMB_AREA / 3 * 1.2f;
                            drawDatTmp6.size.y = BOMB_AREA / 3 * 1.2f;
                            drawDatTmp6.size.x += scaleAddTmp.x;
                            drawDatTmp6.size.y += scaleAddTmp.y;
                            drawDatTmp6.size.x *= scaleMulTmp.x;
                            drawDatTmp6.size.y *= scaleMulTmp.y;

                            //drawDatTmp6.texNo = m_Tex_Beambg;
                            drawDatTmp6.texNo = 0;
                            //drawDatTmp6.color = MkF4(0.2f, 0.0f, 0.5f, 0.7f);
                            //drawDatTmp6.color = MkF4(0.7f, 0.7f, 0.7f, 0.9f);
                            //drawDatTmp6.color = MkF4(0.15f, 0.0f, 0.2f, 0.9f);
                            //drawDatTmp6.color = MkF4(0.05f, 0.0f, 0.1f, 1.0f);
                            drawDatTmp6.color = MkF4(0.1f, 0.0f, 0.3f, 1.0f 
                                //* m_MovieVisibleAlpha
                            );
                            //drawDatTmp6.texUV = {
                            //    0.0f + 0.01f * obj->frameActive,
                            //    0.0f + 0.02f * obj->frameActive
                            //};
                            drawDatTmp6.texUV = {
                                0.0f + 0.006f * m_Obj->frameActive,
                                0.0f + 0.008f * m_Obj->frameActive
                            };
                            //drawDatTmp6.texWH = {
                            //    5.0f,
                            //    3.0f
                            //};
                            drawDatTmp6.texWH = {
                                3.0f,
                                2.0f
                            };
                            drawDatTmp6.shaderOpt.blendState = DPD_SHADER_OPT_BLEND_STATE_DEFAULT;
                            DRAW_POLYGON_IN_CAMERA_EX_OPTION opt6 = optBaseTmp;
                            opt6.depthBuffIgnore = true;
                            opt6.priority = DRAW_POLYGON_PRIORITY_NML_d1;
                            DrawPolygonInCameraEx(drawDatTmp6, opt6);
                        }

                        DrawPolygonInCameraEx(drawDatTmp5, opt5);
                        DrawPolygonInCameraEx(drawDatTmp4, opt4);
                        DrawPolygonInCameraEx(drawDatTmp3, opt3);

                        //DrawPolygonInCameraEx(drawDatTmp2, opt2);
                    }
                }
                //drawDatTmp.texNo = m_Tex_Beambg;
                drawDatTmp.texNo = m_Tex_Beam2;
                //drawDatTmp.color = MkF4(0.75f, 0.2f, 0.4f, 0.9f);
                //drawDatTmp.color = MkF4(0.3f, 0.1f, 0.5f, 1.0f);
                drawDatTmp.color = MkF4(0.95f, 0.4f, 0.6f, 0.9f 
                    //* m_MovieVisibleAlpha
                );
                drawDatTmp.texUV = {
                    0.0f + 0.012f * m_Obj->frameActive,
                    0.0f + 0.01f * m_Obj->frameActive
                };
                //drawDatTmp.texWH = {
                //    2.0f,
                //    4.0f
                //};
                //drawDatTmp.texWH = {
                //    1.0f,
                //    3.0f
                //};
                drawDatTmp.texWH = {
                    0.5f,
                    2.0f
                };
                optBaseTmp.depthBuffIgnore = true;
                optBaseTmp.priority = DRAW_POLYGON_PRIORITY_NML_d1;
                DrawPolygonInCameraEx(drawDatTmp, optBaseTmp);
            }
        }


        {
            DRAW_POLYGON_DAT dsdat = {};
            dsdat.pos.x = 0.0f;
            dsdat.pos.y = 0.0f;
            dsdat.pos.z = 0.0f;

            dsdat.size.x = SCREEN_WIDTH;
            dsdat.size.y = SCREEN_HEIGHT;
            dsdat.size.z = 1.0f;

            dsdat.rot = 0.0f;

            //dsdat.color = { 0.5f, 0.0f, 0.3f, 0.25f };
            dsdat.color = {
                0.1f,
                0.0f,
                0.3f,
                0.6f
            };
            dsdat.color.w *= enterf * endf;

            dsdat.texNo = 0;
            dsdat.loadTexType = LOADTEXTURETYPE_MAIN;

            dsdat.texUV = {
                0.0f,
                0.0f
            };
            dsdat.texWH = {
                1.0f,
                1.0f
            };

            dsdat.startPos = {
                0.0f,
                0.0f
            };
            dsdat.startRot = 0;
            dsdat.startScale = { 1.0f, 1.0f };
            dsdat.startScale3D = { 1.0f, 1.0f, 1.0f };

            dsdat.shaderOpt.bloom = DPD_SHADER_OPT_BLOOM_NONE;
            //dsdat.shaderOpt.blendState = DPD_SHADER_OPT_BLEND_STATE_ADDCOLOR;

            DRAW_POLYGON_IN_CAMERA_EX_OPTION opt = {};
            opt.reverse = false;
            opt.fixed = true;
            opt.depthBuffIgnore = true;
            //opt.priority = DRAW_POLYGON_PRIORITY_NML_d1;
            opt.priority = DRAW_POLYGON_PRIORITY_NML_d1;

            DrawPolygonInCameraEx(dsdat, opt);
        }
    }
}


void GOE_Character_EnemyProjectile::Dead()
{
}


void GOE_Character_EnemyProjectile::DamageAttacker(GAME_OBJECT_DAMAGE_RESULT dr)
{
    GAME_OBJECT* other = GetGameObject(dr.recObjIndex);

    //if (dynamic_cast<GOE_ObjectDynamic*>(other->events.source) != nullptr)
    if (other->collider.type != GAME_OBJECT_COLLIDER_TYPE_NONE)
    {
        if (
            m_type == Type::Beam
            || m_type == Type::ExplosionShockWave
        )
        {
            DeleteGameObject(m_Obj);
        }
    }
}

void GOE_Character_EnemyProjectile::DamageReciever(GAME_OBJECT_DAMAGE_RESULT dr)
{
}

void GOE_Character_EnemyProjectile::BlockHit(GAME_OBJECT_POSMOD_RESULT pmr)
{
    //if (m_type == Type::Beam)
    //{
    //    DeleteGameObject(m_Obj);
    //}
    //if (m_type == Type::ExplosionShockWave)
    //{
    //    DeleteGameObject(m_Obj);
    //}
}

GOE_Character_EnemyProjectile* GOE_Character_EnemyProjectile::CreateProjectile(Type type,
    Float3 startPos,
    Float3 startVel
)
{
    GOE_Character_EnemyProjectile* goe_pp = nullptr;
    {
        GAME_OBJECT co = GetGameObjectPrefab(GAME_OBJECT_TYPE_DUMMY);
        co.pos = startPos;
        co.vel = startVel;

        GAME_OBJECT* coCrd = CreateGameObject(co);
        if (coCrd != nullptr)
        {
            goe_pp = new GOE_Character_EnemyProjectile(coCrd);

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

