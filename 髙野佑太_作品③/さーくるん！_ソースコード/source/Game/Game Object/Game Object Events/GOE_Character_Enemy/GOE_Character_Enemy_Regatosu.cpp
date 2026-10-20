// ===================================================
// GOE_Character_Enemy_Regatosu.cpp キャラクターオブジェクト
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

#include "../GOE_Effect.h"
#include "../GOE_Character.h"

#include "../../../Game Events/GE_Gaya.h"

#include "GOE_Character_Enemy_Miresu.h"
#include "GOE_Character_Enemy_Boss_Inperiusu.h"
#include "GOE_Character_Enemy_Regatosu.h"

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
void GOE_Character_Enemy_Regatosu::Init()
{
	SuperGOE::Init();

    //気分転換
    //GraphicSet("TEX/enemy/regatosu_toshikoshi", 1, 1);
    //GraphicSet("TEX/enemy/regatosu", 1, 1);

    m_Tex_Main_Idle = LoadTexData("TEX/enemy/lega_idle");
    m_Tex_Main_Dam = LoadTexData("TEX/enemy/lega_damage");
    m_Tex_Main_Atk = LoadTexData("TEX/enemy/lega_attack");

    GraphicSet(m_Tex_Main_Idle, 1, 1);
    

    //必要なテクスチャを読み込み
    m_TexId_AreaEnemy = LoadTexData("TEX/effect/area_enemy").textureId;

    m_Model_Wave = GetModelDataIndex("MODEL/effect/legatos_wave01");
    m_Tex_EffWave1 = LoadTexData("TEX/effect/eff_wave_01");
    //m_Tex_EffWave2 = LoadTexData("TEX/effect/eff_wave_02");

    m_Tex_Defeat = LoadTexData("TEX/enemy/lega_diss");

	//Character::PlayMotionCharacterGameObject(
	//	goDat,
	//	Character::GetCharacterMotionFindByName(Character::GetCharacterMainData(goDat->dataIndex), "IDLE"),
	//	true,
	//	nullptr,
	//	60
	//);
	
    {
        GAME_OBJECT_COLLISION_RECIEVE_CHILD* col = m_Obj->collision.rec.collision + 0;
        col->size = MkF3(ONE_BLOCK * 3.0f, ONE_BLOCK * 3.0f, ONE_BLOCK * 16.0f);
    }

    m_DefeatScore = 50;
}

// ===================================================
// FirstFrame
// 最初のフレームのみ実行
// ===================================================
void GOE_Character_Enemy_Regatosu::FirstFrame()
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
        (int*)(&m_CurrentAxisEx) //スプライトの番号を向きに依存させる
    );
}

// ===================================================
// Uninit
// オブジェクトが削除されるとき
// ===================================================
void GOE_Character_Enemy_Regatosu::Uninit()
{
    UnloadTexData(m_TexId_AreaEnemy);

    UnloadTexData(m_Tex_Main_Idle);
    UnloadTexData(m_Tex_Main_Dam);
        
	SuperGOE::Uninit();
}

// ===================================================
// Update
// 毎フレーム実行
// ===================================================
void GOE_Character_Enemy_Regatosu::Update()
{
	SuperGOE::Update();

}

// ===================================================
// UpdateActive
// 毎フレーム実行
// ただし、ヒットストップ中などの
// 非アクティブ時は実行されない
// ===================================================
void GOE_Character_Enemy_Regatosu::UpdateActive()
{
	SuperGOE::UpdateActive();


    //フラグがたって居たら削除する
    if (m_DeadFlag)
    {
        //for (int i = 0; i < m_BuffTargets.size(); i++)
        //{
        //    if (IsActiveGameObject(m_BuffTargets[i]))
        //    {
        //        GOE_Character_Enemy_Miresu* goeTmp = (GOE_Character_Enemy_Miresu*)(m_BuffTargets[i]->events.source);
        //        if (goeTmp != nullptr && goeTmp->GetIsBuff())
        //        {
        //            goeTmp->SetBuffBreak();
        //        }
        //    }
        //}

        Float3 addPos = {};
        //addPos.z = -320.0f;
        {
            GAME_OBJECT* go = GO_CreateAfterEffect(m_Obj, 90, MkF4(1.0f, 1.0f, 1.0f, 1.0f));
            if (go != nullptr)
            {
                go->graphic.stop = false;
                go->graphic.graph.spriteAnim[0].texDat = m_Tex_Defeat;
                go->graphic.graph.spriteAnim[0].anim = true;
                go->graphic.graph.spriteAnim[0].no = 0;
                go->graphic.graph.spriteAnim[0].frame = 0;
                go->graphic.graph.spriteAnim[0].xcnt = 5;
                go->graphic.graph.spriteAnim[0].hcnt = 5;
                go->graphic.graph.spriteAnim[0].cntMax = 5 * 5;
                go->graphic.graph.spriteAnim[0].delay = go->lifetimeFrameMax / 30;
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

    if (m_Anim_AttackLoop)
    {
        if (!m_CharaData->playingMotion.enable
            || std::string(m_CharaData->playingMotion.motion->name) == "IDLE")
        {
            GraphicSpriteSet(m_Tex_Main_Atk, 3, 2);

            //アニメーションを再生する
            Character::PlayMotionCharacterGameObject(
                m_CharaData,
                Character::GetCharacterMotionFindByName(Character::GetCharacterMainData(m_CharaData->dataIndex), "ATTACK"),
                true,
                nullptr,
                40
            );
        }
    }
    else
    {
        if (!m_CharaData->playingMotion.enable)
        {
            GraphicSpriteSet(m_Tex_Main_Idle, 1, 1);

            //アニメーションを再生する
            Character::PlayMotionCharacterGameObject(
                m_CharaData,
                Character::GetCharacterMotionFindByName(Character::GetCharacterMainData(m_CharaData->dataIndex), "IDLE"),
                true,
                (int*)(&m_CurrentAxisEx) //スプライトの番号を向きに依存させる
            );
        }
    }

	//行動パターン
	{
        if (!m_IsNoBuffMode
            && m_Obj->frameActive > 1
            && !IsDefeatGameObject(m_Obj)
        )
        {
            if (GetIsBuffSuper())
            {
                m_BuffSuperFrame++;

                //バフ
                m_Obj->collision.atk.collision[0].power = m_CollisionDefault.atk.collision[0].power * BUFF_SUPER_MOD_ATTACK;
                m_Obj->collision.rec.collision[0].guardMod = m_CollisionDefault.rec.collision[0].guardMod * BUFF_SUPER_MOD_GUARD;
            }
            else
            {
                m_Obj->collision.atk.collision[0].power = m_CollisionDefault.atk.collision[0].power;
                m_Obj->collision.rec.collision[0].guardMod = m_CollisionDefault.rec.collision[0].guardMod;
            }

            //int mbuffTargetsIndex = 0;
            //m_BuffTargets.clear();

            std::map<int, GAME_OBJECT*> miresu = GetGameObjectFindByDistance(m_Obj->pos, MIRESU_BUFF_AREA);
            for (int i = 0; i < miresu.size(); i++)
            {
                if (
                    HasTagGameObject(miresu[i], GOE_Character_Enemy_Miresu::CHARACTER_NAME_ATTACKER)
                    || HasTagGameObject(miresu[i], GOE_Character_Enemy_Miresu::CHARACTER_NAME_SHOOTER)
                    )
                {
                    //m_BuffTargets[mbuffTargetsIndex] = miresu[i];
                    //mbuffTargetsIndex++;

                    GOE_Character_Enemy_Miresu* goeTmp = (GOE_Character_Enemy_Miresu*)(miresu[i]->events.source);
                    if (goeTmp != nullptr
                        && !goeTmp->IsApp()
                        && !goeTmp->GetIsBuff()
                        && !goeTmp->GetIsBuffBreak()
                        && !goeTmp->GetIsBuffSuper()
                    )
                    {
                        goeTmp->StartCharaEffBuff(
                            30, 
                            MkF3(
                                ONE_BLOCK * 5.0f * (m_Obj->scale.x + m_Obj->scale.y + m_Obj->scale.z) / 3,
                                ONE_BLOCK * 5.0f * (m_Obj->scale.x + m_Obj->scale.y + m_Obj->scale.z) / 3,
                                ONE_BLOCK * 5.0f * (m_Obj->scale.x + m_Obj->scale.y + m_Obj->scale.z) / 3
                            ),
                            {}
                        );
                        goeTmp->SetBuff(m_Obj);
                    }
                }
            }
        }
	}


    //バフエフェクト
    if (GetIsBuffSuper())
    {
        int indextmp = GetGameObjectIndex(m_Obj);
        if ((m_Obj->frameActive + indextmp * 13) % 20 == 0)
        {
            Float3 postmp = m_Obj->pos;
            postmp.x += sin((float)m_Obj->frame * 13.0f + (0.7f * indextmp)) * ONE_BLOCK * 1.4f * m_Obj->scale.x;
            postmp.y += cos((float)m_Obj->frame * 3.0f + (1.3f * indextmp)) * ONE_BLOCK * 1.4f * m_Obj->scale.y + 0.0f;
            postmp.z += cos((float)m_Obj->frame * 13.0f + (1.3f * indextmp)) * ONE_BLOCK * 1.8f * m_Obj->scale.y + -250.0f;
            //postmp.z += -150.0f * m_Obj->scale.y;
            GAME_OBJECT* go = GO_CreateEffect(GO_EFFECT_TYPE_SPARK, postmp);
            if (go != nullptr)
            {
                go->rot = -50.0f * m_Obj->frame * indextmp;
                go->scale = MkF3(
                    2.0f,
                    2.0f,
                    2.0f
                );
                GameObjectSetLifeTime(go, 12);
            }
        }
    }

    
    if(!m_DeadFlag)
    { //自爆

        if (m_BuffSuperFrame == BUFF_SUPER_EXPLODE_BEFORE_FRAME - FRAME_ONE_SECOND * 2)
        {
            GAYA_REQUEST_OPT opt = {};
            opt.exclusive = true;
            m_ExploGayaId = GE_GayaStartRequest(
                "iseki_legatos_explosion",
                0.0f,
                opt
            );
        }

        if (m_BuffSuperFrame == BUFF_SUPER_EXPLODE_BEFORE_FRAME)
        {
            GameSlowRequest(10, 5);
            CameraFlashRequestAll(
                50,
                EasingInOutSine,
                MkF4(0.3f, 0.0f, 0.0f, 0.2f)
            );
        }

        if (m_BuffSuperFrame >= BUFF_SUPER_EXPLODE_FRAME)
        {
            GE_GayaCancelRequest(m_ExploGayaId);

            { //自爆の判定
                std::map<int, GAME_OBJECT*> objs = GetGameObjectFindByDistance(m_Obj->pos, MIRESU_BUFF_AREA);
                for (int i = 0; i < objs.size(); i++)
                {
                    if ( !HasTagGameObject(objs[i], GOE_Character_Enemy_Boss_Inperiusu::CHARACTER_NAME) )
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
                                    EXPLODE_ATK_COL_NAME
                                );

                            GameObjectDamageCollisionHit(objs[i], m_Obj,
                                motionColDetails.atkCol,
                                colRec
                            );

                        }
                    }
                }
            }

            Float3 postmp = m_Obj->pos;
            postmp.y += 180.0f * m_Obj->scale.y;
            postmp.z += -220.0f * m_Obj->scale.y;

            GO_CreateExplosion(postmp, 1.8f);

            CameraFlashRequestAll(
                30,
                EasingInOutSine,
                MkF4(0.9f, 0.7f, 0.5f, 0.9f),
                DPD_SHADER_OPT_BLEND_STATE_ADDCOLOR
            );

            CameraShakeRequestAll(
                CAMERA_SHAKE_EXLARGE_FRAME * 0.75f, 
                CAMERA_SHAKE_EXLARGE_POWER * 3.0f
            );

            //SEを再生
            SoundEffectPlayRequest(SOUND_TYPE_SE_9, 4.0f, 0.0f, 0.6f);

            DeleteGameObject(m_Obj);
        }
    }
}

// ===================================================
// Draw
// オブジェクトの描画イベント
// ===================================================
void GOE_Character_Enemy_Regatosu::Draw()
{
	SuperGOE::Draw();

    if (IsApp())
    { //登場演出中は止める
        return;
    }

    //{
    //    DRAW_SPRITE_DAT dsdat = {};
    //    dsdat.x = 0.0f;
    //    dsdat.z = 0.0f;
    //    //dsdat.y = -0.01f - (0.1f / GAME_OBJECT_MAX) * GetGameObjectIndex(m_Obj);
    //    dsdat.z = -5.0f - (0.1f / GAME_OBJECT_MAX) * GetGameObjectIndex(m_Obj);

    //    dsdat.w = MIRESU_BUFF_AREA * 2.0f;
    //    dsdat.h = MIRESU_BUFF_AREA * 2.0f;

    //    dsdat.rot = 0.0f;
    //    if (GetIsBuffSuper())
    //    {
    //        dsdat.color = { 1.5f, 0.0f, 0.0f, 0.2f };
    //    }
    //    else
    //    {
    //        dsdat.color = { 1.0f, 1.0f, 1.0f, 0.1f };
    //    }
    //    {
    //        float amod = (sin((float)m_Obj->frameActive / 10) + 1.0f) / 2;
    //        dsdat.color.x += amod * 1.5f;
    //        dsdat.color.y += amod * 1.5f;
    //        dsdat.color.z += amod * 1.5f;
    //        dsdat.color.w += amod * 0.2f;
    //    }

    //    dsdat.texNo = m_TexId_AreaEnemy;
    //    dsdat.loadTexType = LOADTEXTURETYPE_MAIN;

    //    //dsdat.texUV = { 0.0f, 0.0f };
    //    //dsdat.texWH = { 1.0f, 1.0f };
    //    dsdat.texUV = AnimationUV(
    //        (int)((m_Obj->frame / 4) % m_AnimCountMax_AreaEnemy),
    //        (int)m_AnimCount_AreaEnemy.x,
    //        (int)m_AnimCount_AreaEnemy.y
    //    );
    //    dsdat.texWH = { 1.0f / m_AnimCount_AreaEnemy.x, 1.0f / m_AnimCount_AreaEnemy.y };

    //    dsdat.startPos3D = m_Obj->pos;
    //    dsdat.startPos3D.z += 0.0f;

    //    dsdat.startRot = 0.0f;
    //    dsdat.startScale = { 1.0f, 1.0f };

    //    dsdat.shaderOpt.bloom = DPD_SHADER_OPT_BLOOM_NORMAL;
    //    dsdat.shaderOpt.blendState = DPD_SHADER_OPT_BLEND_STATE_ADDCOLOR;

    //    DrawSpriteInCamera(dsdat);
    //}

    if (!m_IsNoBuffMode)
    {
        //for (int i = 0; i < 2; i++)
        {
            DRAW_POLYGON_DAT drawDatTmp = {};
            drawDatTmp.size = MkF3(
                MIRESU_BUFF_AREA * 2.0f / 100,
                MIRESU_BUFF_AREA * 2.0f / 100,
                MIRESU_BUFF_AREA * 2.0f / 100
            );
            //drawDatTmp.size = MkF3(
            //    10.0f,
            //    10.0f,
            //    10.0f
            //);

            //drawDatTmp.pos.y += m_Obj->size.y / 2;
            //drawDatTmp.pos.z = -5.0f;

            drawDatTmp.rot = 0.0f;

            drawDatTmp.rot3D = MkF3(0.0f, 0.0f, 0.0f);

            drawDatTmp.texWH = { 1.0f / 1, 1.0f / 1 };

            //if (i == 1)
            //{
            //    drawDatTmp.texUV = {
            //        0.0f,
            //        0.003f * m_Obj->frame
            //    };
            //    drawDatTmp.color = MkF4(0.7f, 0.1f, 0.7f, 1.0f);

            //    drawDatTmp.texNo = m_Tex_EffWave2.textureId;
            //}
            //else

            {
                drawDatTmp.texUV = {
                    0.0f,
                    0.006f * m_Obj->frame
                };
                //drawDatTmp.color = MkF4(0.4f, 0.1f, 0.7f, 1.0f);
                drawDatTmp.color = MkF4(
                    1.0f, 
                    0.1f,
                    0.0f, 
                    1.0f * (GetIsBuffSuper() ? 1.0f:0.35f)
                );

                drawDatTmp.texNo = m_Tex_EffWave1.textureId;
            }

            drawDatTmp.loadTexType = LOADTEXTURETYPE_MAIN;

            //drawDatTmp.texUV = AnimationUV(
            //    (int)((m_Obj->frame / 4) % m_AnimCountMax_AreaEnemy),
            //    (int)m_AnimCount_AreaEnemy.x,
            //    (int)m_AnimCount_AreaEnemy.y
            //);
            //drawDatTmp.texWH = { 1.0f / m_AnimCount_AreaEnemy.x, 1.0f / m_AnimCount_AreaEnemy.y };

            drawDatTmp.startPos3D = m_Obj->pos;
            //drawDatTmp.startPos3D.z = -5.0f + -4.0 * i;
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
            opt.priority = DRAW_POLYGON_PRIORITY_NML_d3;
            opt.depthBuffIgnore = true;

            DrawPolygonInCameraEx(drawDatTmp, opt);
        }
    }

    if (m_BuffSuperFrame >= BUFF_SUPER_EXPLODE_BEFORE_FRAME)
    {
        m_Obj->graphic.color = MkF4(
            1000.0f,
            1000.0f,
            1000.0f,
            1.0f
        );
    }
    else if (GetIsBuffSuper())
    {
        //DrawCharaEff(CharaEffType::Buff, MkF2(ONE_BLOCK * 12.0f, ONE_BLOCK * 12.0f));

        float gmod = (sin((float)m_Obj->frameActive / 4) + 1.0f) / 2 * -0.75f;
        float bmod = (cos((float)m_Obj->frameActive / 4) + 1.0f) / 2 * -0.75f;
        m_Obj->graphic.color = MkF4(
            1.0f,
            1.0f + gmod,
            1.0f + bmod,
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
}

// ===================================================
// Dead
// オブジェクトのHPが0になったとき
// ===================================================
void GOE_Character_Enemy_Regatosu::Dead()
{
	SuperGOE::Dead();

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
void GOE_Character_Enemy_Regatosu::DamageAttacker(GAME_OBJECT_DAMAGE_RESULT dr)
{
	SuperGOE::DamageAttacker(dr);
}

// ===================================================
// DamageReciever
// オブジェクトが
// 別のオブジェクトの攻撃判定にヒットしたとき
// ===================================================
void GOE_Character_Enemy_Regatosu::DamageReciever(GAME_OBJECT_DAMAGE_RESULT dr)
{
	SuperGOE::DamageReciever(dr);

    if (dr.damage > 0)
    {
        GraphicSpriteSet(m_Tex_Main_Dam, 4, 3, 11);

        //アニメーションを再生する
        Character::PlayMotionCharacterGameObject(
            m_CharaData,
            Character::GetCharacterMotionFindByName(Character::GetCharacterMainData(m_CharaData->dataIndex), "DAMAGE"),
            false,
            nullptr,
            40
        );
    }
}

// ===================================================
// BlockHit
// オブジェクトがブロックに触れたとき
// ===================================================
void GOE_Character_Enemy_Regatosu::BlockHit(GAME_OBJECT_POSMOD_RESULT pmr)
{
	SuperGOE::BlockHit(pmr);
}
