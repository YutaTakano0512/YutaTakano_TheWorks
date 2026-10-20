// ===================================================
// Character_Enemy.cpp キャラクターオブジェクト
// 
// hara sougo  2024/11/25
// ===================================================
//#define NO_CONTROLLER

#include "../../../Game/Configuration/Param.h"
#include "../../../game_main.h"
#include "../../Sprite/sprite.h"
#include "../../Polygon/modelData.h"
#include "../../Texture/texture.h"
#include "../../../Game/Entities/Block/block.h"
#include "../../Systems/collision.h"
#include "../../Systems/easing.h"
#include "../../../Core/Input/controller_main.h"

#include "../../Managers/gameManager.h"
#include "../../Managers/scene.h"

#include "../gameObject.h"
#include "../gameObjectPrefab.h"

#include "../../Systems/camera.h"
#include "../../Systems/ui.h"
#include "../../Systems/ui_Game.h"

#include "../../Entities/character.h"
#include "../../Entities/player.h"

#include "../../Managers/soundManager.h"

#include "GOE_Effect.h"
#include "GOE_Character_Enemy.h"


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
void GOE_Character_Enemy::Init()
{
	SuperGOE::Init(); //初期化

    m_Tex_AppEf = LoadTexData("TEX/effect/kakopon/general/Glaytest").textureId;
    m_Model_AppEf = GetModelDataIndex("MODEL/beam_01");

	////初期アニメーションを再生
	//Character::PlayMotionCharacterGameObject(
	//	m_CharaData,
	//	Character::GetCharacterMotionFindByName(Character::GetCharacterMainData(m_CharaData->dataIndex), "IDLE"),
	//	true,
	//	&m_WalkFrame,
	//	36
	//);
	
    //m_Obj->graphic.shaderOpt.outline = DPD_SHADER_OPT_OUTLINE_PURPLE;
    m_Obj->graphic.shaderOpt.outline = DPD_SHADER_OPT_OUTLINE_NORMAL;
    m_Obj->family[GAME_OBJECT_FAMILY_ENEMY] = true;

    m_DefeatScoreType = RESULT_SCORE_EX_TYPE_ENEMY_DEFEAT;
}

// ===================================================
// FirstFrame
// 最初のフレームのみ実行
// ===================================================
void GOE_Character_Enemy::FirstFrame()
{
	SuperGOE::FirstFrame();
	
	//初期位置を保存
	m_SpawnPoint = m_Obj->pos;

    if (m_AppFlag)
    {
        EnemyApp();
    }
}

// ===================================================
// Uninit
// オブジェクトが削除されるとき
// ===================================================
void GOE_Character_Enemy::Uninit()
{
    UnloadTexData(m_Tex_AppEf);
	
	SuperGOE::Uninit(); //終了処理
}

// ===================================================
// Update
// 毎フレーム実行
// ===================================================
void GOE_Character_Enemy::Update()
{
	SuperGOE::Update();

}

// ===================================================
// UpdateActive
// 毎フレーム実行
// ただし、ヒットストップ中などの
// 非アクティブ時は実行されない
// ===================================================
void GOE_Character_Enemy::UpdateActive()
{
	SuperGOE::UpdateActive();

    if(m_AppFrame >= 0)
    {
        //constexpr int appmax = 25;

        float mod = std::min(std::max((float)(m_AppFrame - (m_AppFrameMax - APP_OUT_FRAME)) / APP_OUT_FRAME, 0.0f), 1.0f);
        mod = (EasingOutQuad(mod));

        {
            float bsca = m_Obj->graphic.scale3d.y;
            m_Obj->graphic.scale3d.x = 0.02f + 0.88f * EasingInOutSine(mod);
            m_Obj->graphic.scale3d.y = 4.0f - 3.0f * EasingInOutSine(mod);
            m_Obj->graphic.scale3d.z = 0.02f + 0.88f * EasingInOutSine(mod);

            //m_Obj->graphic.pos3d.y =
            //    m_AppBeforeGra.pos3d.y
            //    + -1200.0f * (1.0f - mod);
            //m_Obj->graphic.pos3d.z =
            //    m_AppBeforeGra.pos3d.z
            //    + -200.0f * (1.0f - mod);
        }

        m_Obj->graphic.shaderOpt.outline = DPD_SHADER_OPT_OUTLINE_NONE;

        constexpr float coloutmod = 0.1f;
        float colmod = 0.45f + (0.55f + coloutmod) * mod;
        if (colmod > 1.0f)
        {
            float colmod2 = 1.0f - ((colmod - 1.0f) / coloutmod);
            float addcol = 10.0f * colmod2;

            m_Obj->graphic.color = MkF4(
                1.0f + addcol,
                1.0f + addcol,
                1.0f + addcol,
                1.0f);
        }
        else
        {
            //m_Obj->graphic.pos3d.y = m_AppBeforeGra.pos3d.y
            //    + (ONE_BLOCK * -80.0f - m_AppBeforeGra.pos3d.y) * (1.0f - colmod);

            m_Obj->graphic.color = MkF4(
                colmod,
                colmod,
                colmod,
                2.0f);

            m_Obj->graphic.shaderOpt.blendState = DPD_SHADER_OPT_BLEND_STATE_ADDCOLOR;

            //opt.priority = DRAW_POLYGON_PRIORITY_ALWAYS;

            m_Obj->graphic.shaderType = DPD_SHADER_TYPE_EFFECT_WARP;
        }

        m_AppFrame++;
        if (m_AppFrame >= m_AppFrameMax)
        {
            m_Obj->collision = m_AppBeforeCol;
            m_Obj->graphic = m_AppBeforeGra;
            m_AppFrame = -1;
        }
    }
}

// ===================================================
// Draw
// オブジェクトの描画イベント
// ===================================================
void GOE_Character_Enemy::Draw()
{
	SuperGOE::Draw();

    if (m_AppFrame >= 0)
    {
        float mod = std::min(std::max((float)(m_AppFrame) / APP_IN_FRAME, 0.0f), 1.0f);
        mod = (EasingInQuad(mod));

        float modout = 1.0f - std::min(std::max((float)(m_AppFrame - (APP_FRAME - APP_OUT_FRAME)) / APP_OUT_FRAME, 0.0f), 1.0f);
        modout = (EasingOutQuad(modout));

        {
            float siz = (m_Obj->size.x + m_Obj->size.y) / 2;
            float sca = (m_Obj->scale.x + m_Obj->scale.y + m_Obj->scale.z) / 3;

            DRAW_POLYGON_DAT drawDatTmp = {};
            //drawDatTmp.size = MkF3(
            //    siz * sca * mod * 0.5f,
            //    siz * sca * mod * 2.0f,
            //    1.0f
            //);
            drawDatTmp.size = MkF3(
                siz * sca * mod * 0.16f,
                siz * sca * mod * 0.16f,
                siz * sca * mod * 0.6f
            );
            //drawDatTmp.pos.y += -drawDatTmp.size.y / 2;
            //drawDatTmp.pos.z += -drawDatTmp.size.z / 2;
            drawDatTmp.pos.z += drawDatTmp.size.z * 0.8;
             
            drawDatTmp.rot = 0.0f;

            drawDatTmp.rot3D = MkF3(0.0f, 0.0f, 0.0f);

            drawDatTmp.texWH = {
                1.0f,//3.0f , 
                1.0f 
            };

            drawDatTmp.texUV = {
                0.03f * m_Obj->frame / 3.0f,
                0.0f
            };
            drawDatTmp.color = MkF4(0.7f, 0.2f, 0.9f, 0.8f * modout);

            drawDatTmp.texNo = m_Tex_AppEf;

            drawDatTmp.loadTexType = LOADTEXTURETYPE_MAIN;

            drawDatTmp.startPos3D = m_Obj->pos;
            drawDatTmp.startPos3D.z = 0.0f;

            //drawDatTmp.startRot3D = MkF3(80.0f, 0.0f, 0.0f);

            drawDatTmp.startScale = MkF2(1.0f, 1.0f);
            drawDatTmp.startScale3D = MkF3(1.0f, 1.0f, 1.0f);

            drawDatTmp.modelNo = m_Model_AppEf;

            drawDatTmp.shaderOpt.bloom = DPD_SHADER_OPT_BLOOM_NONE;
            drawDatTmp.shaderOpt.blendState = DPD_SHADER_OPT_BLEND_STATE_ADDCOLOR;

            DRAW_POLYGON_IN_CAMERA_EX_OPTION opt = {};
            opt.reverse = false;
            opt.fixed = false;
            opt.enableShade = true;
            opt.billboard = false;
            //opt.priority = DRAW_POLYGON_PRIORITY_NORMAL;
            //opt.priority = DRAW_POLYGON_PRIORITY_NML_d1;
            //opt.depthBuffIgnore = true;
            opt.priority = DRAW_POLYGON_PRIORITY_NML_ALWAYS;

            DrawPolygonInCameraEx(drawDatTmp, opt);
        }
    }
}

// ===================================================
// Dead
// オブジェクトのHPが0になったとき
// ===================================================
void GOE_Character_Enemy::Dead()
{
	SuperGOE::Dead();

	m_DeadFlag = true;

	//DeleteGameObject(index);
}

// ===================================================
// DamageAttacker
// オブジェクトの攻撃判定がヒットしたとき
// ===================================================
void GOE_Character_Enemy::DamageAttacker(GAME_OBJECT_DAMAGE_RESULT dr)
{
	SuperGOE::DamageAttacker(dr);

	//DebugPrintf("ateta\n");
	//if (dr.colAtk.isGrab)
	//{
	//	//GAME_OBJECT* recOb = GetGameObject(dr.recObjIndex);
	//	//GameObjectMoveRequest(recOb,
	//	//	20,
	//	//	MkF3(recOb->pos.x, recOb->pos.y - 600.0f, recOb->pos.z),
	//	//	EasingInOutSine);
	//}
}

// ===================================================
// DamageReciever
// オブジェクトが
// 別のオブジェクトの攻撃判定にヒットしたとき
// ===================================================
void GOE_Character_Enemy::DamageReciever(GAME_OBJECT_DAMAGE_RESULT dr)
{
	SuperGOE::DamageReciever(dr);

	if (dr.damage > 0)
	{
        if (IsDefeatGameObject(m_Obj))
        { //死んだんじゃないの～？

            GAME_OBJECT* attackerObj = GetGameObject(dr.atkObjIndex);
            if (attackerObj != nullptr)
            {
                if (GetPlayer(attackerObj) != nullptr)
                { //アタッカーがプレイヤーである
                    m_DefeatedByPlayerFlag = true;
                }
            }
        }
	}
}

// ===================================================
// BlockHit
// オブジェクトがブロックに触れたとき
// ===================================================
void GOE_Character_Enemy::BlockHit(GAME_OBJECT_POSMOD_RESULT pmr)
{
	SuperGOE::BlockHit(pmr);
}

void GOE_Character_Enemy::EnemyVanish(Float3 addPos)
{

    if (m_DefeatedByPlayerFlag)
    {
        GameManagerResultAddScore(
            m_DefeatScore,
            (RESULT_SCORE_EX_TYPE)m_DefeatScoreType
        );
        GameManagerAddEnemyDefeatCall();
    }

    Float3 postmp = m_Obj->pos;
    postmp.x += addPos.x * m_Obj->scale.x;
    postmp.y += addPos.y * m_Obj->scale.y;
    postmp.z += addPos.z * m_Obj->scale.y;

    //for (int i = 0; i < 2; i++)
    //{
    //    GAME_OBJECT* go = GO_CreateEffect(GO_EFFECT_TYPE_SHOCKWAVE_ENEM, postmp);
    //    if (go != nullptr)
    //    {
    //        go->size = MkF2(1.0f, 0.5f);
    //        go->scale = MkF2(8.0f, 4.0f);
    //        go->rot = -45.0f + 90.0f * i;

    //        GameObjectSetLifeTime(go, 26);
    //    }
    //}
    //{
    //    GAME_OBJECT* go = GO_CreateEffect(GO_EFFECT_TYPE_TWINKLE, postmp);
    //    if (go != nullptr)
    //    {
    //        go->scale = MkF2(8.5f, 8.5f);
    //        GameObjectSetLifeTime(go, 12);
    //    }
    //}


    {
        Float3 postmp = m_Obj->pos;
        postmp.x += (m_Obj->graphic.pos3d.x + m_Obj->graphic.graph.spriteAnim[0].pos.x + m_Obj->graphic.graph.spriteAnim[0].pos3D.x) * m_Obj->scale.x;
        postmp.y += (m_Obj->graphic.pos3d.y + m_Obj->graphic.graph.spriteAnim[0].pos.y + m_Obj->graphic.graph.spriteAnim[0].pos3D.y) * m_Obj->scale.y;
        postmp.z += (m_Obj->graphic.pos3d.z + m_Obj->graphic.graph.spriteAnim[0].pos3D.z) * m_Obj->scale.z + ONE_BLOCK;

        GAME_OBJECT* go = GO_CreateEffect(GO_EFFECT_TYPE_HITSTAR_VANISH, postmp);
        if (go != nullptr)
        {
            float sca = (m_Obj->scale.x + m_Obj->scale.y + m_Obj->scale.z) / 3;
            go->scale = MkF3(
                12.0f * sca,
                12.0f * sca,
                12.0f * sca
            );
            GameObjectSetLifeTime(go, 25);
        }
    }


    CameraShakeRequestAll(
        CAMERA_SHAKE_MIDIUM_FRAME,
        CAMERA_SHAKE_EXLARGE_POWER * 2.0f
    );
    SetControllerMainLeftVibration(30, 0);
    SetControllerMainRightVibration(30, 0);


    //SEを再生
    //SoundEffectPlayRequest(SOUND_TYPE_SE_10, 4.0f, 0.0f);

    DeleteGameObject(m_Obj);
}

void GOE_Character_Enemy::EnemyApp()
{
    //位置を戻す
    m_Obj->pos = m_AppPos;
    m_SpawnPoint = m_Obj->pos;

    m_AppBeforeCol = m_Obj->collision;
    m_AppBeforeGra = m_Obj->graphic;

    m_Obj->collision.atk.enable = false;
    m_Obj->collision.rec.enable = false;
    m_Obj->graphic.shadow = false;
    //m_Obj->graphic = {};

    m_AppFrame = 0;
}

bool GOE_Character_Enemy::IsApp()
{
    return m_AppFrame >= 0;
}

void GOE_Character_Enemy::BecomeApp(int appmaxf)
{
    m_AppFlag = true;

    //一旦下に移動
    m_AppPos = m_Obj->pos;
    m_Obj->pos = {};
    m_Obj->pos.z += 32767.0f;
    m_AppFrameMax = appmaxf;
}

int GOE_Character_Enemy::CalcRecvDamageMultiPlay(int damage)
{
    int pnum = GetAlivePlayerNum();
    int dam = damage;
    for (int i = 0; i < (pnum - 1); i++)
    {
        dam *= 0.75f;
    }

    return dam;
}
void GOE_Character_Enemy::CalcRecvDamageGuardMod(
    float mod,
    GAME_OBJECT_COLLISION_ATTACK_CHILD colAtk,
    GAME_OBJECT_COLLISION_RECIEVE_CHILD colRec,
    GAME_OBJECT_DAMAGE_RESULT* output
)
{
    float modr = mod;
    float ignoremod  = colAtk.guardIgnoreMod; //防御貫通率
    if (ignoremod > 0.0f)
    {
        modr = (modr * (1.0f - ignoremod) + ignoremod);
    }

    output->damage *= modr;
    output->mod *= modr;
}

void GOE_Character_Enemy::CalcHpMod(GAME_OBJECT* obj, int retryCount)
{
    float mod = 1.0f;
    if (retryCount == 1)
    {
        mod *= 0.95f;
    }
    else if (retryCount == 2)
    {
        mod *= 0.90f;
    }
    else if (retryCount == 3)
    {
        mod *= 0.85f;
    }
    else if (retryCount == 4)
    {
        mod *= 0.80f;
    }
    else if (retryCount == 5)
    {
        mod *= 0.75f;
    }
    else if (retryCount == 6)
    {
        mod *= 0.70f;
    }
    else if (retryCount >= 7)
    {
        mod *= 0.65f;
    }

    if (mod != 1.0f)
    {
        obj->status.hpMax *= mod;
        obj->status.hp = obj->status.hpMax;
    }
}
