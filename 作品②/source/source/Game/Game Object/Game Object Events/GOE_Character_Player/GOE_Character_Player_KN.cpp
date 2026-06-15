// ===================================================
// GOE_Character_Player_KN.cpp キャラクターオブジェクト
// 
// hara sougo  2025/01/19
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

#include "../../../Entities/player.h"


#include "../../../Managers/soundManager.h"

#include "../GOE_Effect.h"
#include "../GOE_Character.h"

#include "GOE_Character_PlayerProjectile.h"
#include "GOE_Character_Player_KN.h"

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
void GOE_Character_Player_KN::Init()
{
	SuperGOE::Init(); //初期化

    //アイコン
    m_TexIconMain_N = LoadTexData("TEX/ui/in_game_master/icon-assets/kontorius_icon");
    m_TexIconMain_Pinch = LoadTexData("TEX/ui/in_game_master/icon-assets/kontorius_pinch");
    m_TexIconMain_Out = LoadTexData("TEX/ui/in_game_master/icon-assets/kontorius_out");

    m_TexIconSub = LoadTexData("TEX/ui/in_game_master/contrius_icon");
    m_TexIconSkillOn = LoadTexData("TEX/ui/in_game_master/skill_bomber_on");
    m_TexIconSkillOff = LoadTexData("TEX/ui/in_game_master/skill_bomber_off");

    SetCharaSubGraphic();

    m_Tex_Beam1 = LoadTexData("TEX/effect/beam/tex_beam_01").textureId;
    m_Tex_Beam2 = LoadTexData("TEX/effect/beam/tex_beam_02").textureId;
    m_Tex_Beam3 = LoadTexData("TEX/effect/beam/tex_beam_03").textureId;
    //m_Tex_Beambg = LoadTexData("TEX/effect/tex_anim_02").textureId;
    m_Tex_Beambg = LoadTexData("TEX/effect/barrier/tex_br_wave").textureId;

    m_Model_Beam = GetModelDataIndex("MODEL/beam_01");

    m_Tex_ShockWave = LoadTexData("TEX/effect/kakopon/general/Glaytest").textureId;
    m_Model_ShockWave = GetModelDataIndex("MODEL/effect/shockwave");

    m_Tex_EnergyBall = LoadTexData("TEX/effect/tex_energy_01").textureId;

    m_Tex_AttackArea = LoadTexData("TEX/effect/tex_ex_attack").textureId;
    m_Tex_Wave1 = LoadTexData("TEX/effect/tex_ink_01").textureId;
    //m_Tex_Wave = LoadTexData("TEX/effect/kakopon/tex_add_01").textureId;
    //m_Tex_Wave2 = LoadTexData("TEX/effect/eff_wave_02").textureId;
    m_Model_Wave = GetModelDataIndex("MODEL/effect/legatos_wave01");

    GraphicSet(m_Tex_Main_Idle, 3, 3);

    m_BuffEfPos = MkF3(0.0f, -240.0f, 0.0f);

    m_MoveSpeedMod = MOVE_SPEED_MOD;
    m_MoveSpeedModKako = MOVE_SPEED_MOD_KAKO;

    m_Obj->friction *= MOVE_FRICTION_MOD;

    //ブロックを無視する条件
    m_Obj->blockCollisionIgnoreFunc = [](int objIndex, int blockIndex)->bool
    {
        //GAME_OBJECT* obj = GetGameObject(objIndex);
        BLOCK* block = GetBlocks() + blockIndex;

        //float BlockTop = block->pos.y - block->size.y / 2;

        //if (
        //    block->type != BLOCK_TYPE_ISEKI_OBJECT_06 //立っている柱
        //    && block->type != BLOCK_TYPE_ISEKI_OBJECT_09 //折れている立っている柱
        //    )
        if (block->sizeZ < BLOCK_HIGHT * 2)
        {
            return true;
        }

        return false;
    };

    m_IsHealIgnore = true;

    {
        GAME_OBJECT_COLLISION_RECIEVE_CHILD* col = m_Obj->collision.rec.collision + 0;
        col->pos.z += ONE_BLOCK * -4.0f;
        col->size.z *= 2.0f;
    }

    //かこポン初期化
    m_KakoDamName = ("COL_ATK_KAKOPON_KN");
    m_KakoDamAddMod = 1.2f;
    m_KakoInkCostMod = 1.25f;

    m_KakoAddDis = ONE_BLOCK * 8.0f;

    //m_ThemeColor = { 0.8f, 0.0f, 2.0f };
    m_ThemeColor = { 0.0f, 1.6f, 0.2f };

    //m_KakoDat_Main.graphic = KAKOPON_GRAPHIC::Main;
    //m_KakoDat_Main.type = KAKOPON_TYPE::Main;
    //m_KakoDat_Main.frameMax = GOE_Character_Player::KAKOPON_COOLDOWN_FRAME;

    //m_KakoDat_SubTest.graphic = KAKOPON_GRAPHIC::Main;
    //m_KakoDat_SubTest.type = KAKOPON_TYPE::Heal;
    //m_KakoDat_SubTest.frameMax = FRAME_ONE_SECOND * 5;

    m_KakoDatList[0].dat.graphic = KAKOPON_GRAPHIC::Main_Kn;
    m_KakoDatList[0].dat.type = KAKOPON_TYPE::Main;
    m_KakoDatList[0].dat.frameMax = KAKOPON_COOLDOWN_FRAME;

    m_KakoDatList[1] = {};

    m_BombAttackCol =
        Character::GetMotionColDataFindByName(
            Character::MOTION_COL_TYPE_ATK,
            SKILL_BOMB_COL_NAME
        ).atkCol;
    strcpy_s(m_BombAttackCol.tag, GAME_OBJECT_TXT_MAX,
        N_ATTACK_BOMBLASER_GENERAL_TAG);

}

// ===================================================
// FirstFrame
// 最初のフレームのみ実行
// ===================================================
void GOE_Character_Player_KN::FirstFrame()
{
	SuperGOE::FirstFrame();

	PLAYER* player = GetPlayer(m_Obj);
	if (player != nullptr)
	{
        //
        //↓処理
        //
        
		//return;
	}
    //最初のフレーム(初期化とか)

}

void GOE_Character_Player_KN::SetCharaSubGraphic()
{
    UnloadTexData(m_Tex_BeamCharge);
    UnloadTexData(m_Tex_Beam1);
    UnloadTexData(m_Tex_Beam2);
    UnloadTexData(m_Tex_Beam3);
    UnloadTexData(m_Tex_Beambg);

    UnloadTexData(m_Model_Beam);

    UnloadTexData(m_Tex_EnergyBall);
    UnloadTexData(m_Tex_Wave1);
    //UnloadTexData(m_Tex_Wave2);
    UnloadTexData(m_Tex_AttackArea);

    UnloadTexData(m_Tex_Main_Attack[0]);
    UnloadTexData(m_Tex_Main_Attack[1]);
    UnloadTexData(m_Tex_Main_Attack[2]);
    UnloadTexData(m_Tex_Main_Attack[3]);
    UnloadTexData(m_Tex_Main_Attack[4]);
    UnloadTexData(m_Tex_Main_Attack[5]);
    UnloadTexData(m_Tex_Main_Attack[6]);
    UnloadTexData(m_Tex_Main_Attack[7]);

    UnloadTexData(m_Tex_Main_Walk[0]);
    UnloadTexData(m_Tex_Main_Walk[1]);
    UnloadTexData(m_Tex_Main_Walk[2]);
    UnloadTexData(m_Tex_Main_Walk[3]);
    UnloadTexData(m_Tex_Main_Walk[4]);
    UnloadTexData(m_Tex_Main_Walk[5]);
    UnloadTexData(m_Tex_Main_Walk[6]);
    UnloadTexData(m_Tex_Main_Walk[7]);

    UnloadTexData(m_Tex_Main_Draw);
    UnloadTexData(m_Tex_Main_Idle);
    UnloadTexData(m_Tex_Main_Down);


    std::string pstr = m_OnajiCharaNum > 0 ? std::to_string(m_OnajiCharaNum + 1) : "";

    m_Tex_Main_Attack[0] = LoadTexData("TEX/player/kn" + pstr + "/attack/kn" + pstr + "_attack_01");
    m_Tex_Main_Attack[1] = LoadTexData("TEX/player/kn" + pstr + "/attack/kn" + pstr + "_attack_08");
    m_Tex_Main_Attack[2] = LoadTexData("TEX/player/kn" + pstr + "/attack/kn" + pstr + "_attack_07");
    m_Tex_Main_Attack[3] = LoadTexData("TEX/player/kn" + pstr + "/attack/kn" + pstr + "_attack_06");
    m_Tex_Main_Attack[4] = LoadTexData("TEX/player/kn" + pstr + "/attack/kn" + pstr + "_attack_05");
    m_Tex_Main_Attack[5] = LoadTexData("TEX/player/kn" + pstr + "/attack/kn" + pstr + "_attack_04");
    m_Tex_Main_Attack[6] = LoadTexData("TEX/player/kn" + pstr + "/attack/kn" + pstr + "_attack_03");
    m_Tex_Main_Attack[7] = LoadTexData("TEX/player/kn" + pstr + "/attack/kn" + pstr + "_attack_02");

    m_Tex_Main_Walk[0] = LoadTexData("TEX/player/kn" + pstr + "/move/kn" + pstr + "_walk_01");
    m_Tex_Main_Walk[1] = LoadTexData("TEX/player/kn" + pstr + "/move/kn" + pstr + "_walk_08");
    m_Tex_Main_Walk[2] = LoadTexData("TEX/player/kn" + pstr + "/move/kn" + pstr + "_walk_07");
    m_Tex_Main_Walk[3] = LoadTexData("TEX/player/kn" + pstr + "/move/kn" + pstr + "_walk_06");
    m_Tex_Main_Walk[4] = LoadTexData("TEX/player/kn" + pstr + "/move/kn" + pstr + "_walk_05");
    m_Tex_Main_Walk[5] = LoadTexData("TEX/player/kn" + pstr + "/move/kn" + pstr + "_walk_04");
    m_Tex_Main_Walk[6] = LoadTexData("TEX/player/kn" + pstr + "/move/kn" + pstr + "_walk_03");
    m_Tex_Main_Walk[7] = LoadTexData("TEX/player/kn" + pstr + "/move/kn" + pstr + "_walk_02");

    m_Tex_Main_Draw = LoadTexData("TEX/player/kn" + pstr + "/kako/kn" + pstr + "_naname");
    m_Tex_Main_Idle = LoadTexData("TEX/player/kn" + pstr + "/idle/kn" + pstr + "_idle");

    m_Tex_Main_Down = LoadTexData("TEX/player/kn" + pstr + "/kn" + pstr + "_damage_01");
    //m_Tex_Main_Down = LoadTexData("TEX/player/kai/kai_damage_01");
}

// ===================================================
// Uninit
// オブジェクトが削除されるとき
// ===================================================
void GOE_Character_Player_KN::Uninit()
{

    UnloadTexData(m_Tex_BeamCharge);
    UnloadTexData(m_Tex_Beam1);
    UnloadTexData(m_Tex_Beam2);
    UnloadTexData(m_Tex_Beam3);
    UnloadTexData(m_Tex_Beambg);
    
    UnloadTexData(m_Model_Beam);

    UnloadTexData(m_Tex_EnergyBall);
    UnloadTexData(m_Tex_Wave1);
    //UnloadTexData(m_Tex_Wave2);
    UnloadTexData(m_Tex_AttackArea);

    UnloadTexData(m_Tex_Main_Attack[0]);
    UnloadTexData(m_Tex_Main_Attack[1]);
    UnloadTexData(m_Tex_Main_Attack[2]);
    UnloadTexData(m_Tex_Main_Attack[3]);
    UnloadTexData(m_Tex_Main_Attack[4]);
    UnloadTexData(m_Tex_Main_Attack[5]);
    UnloadTexData(m_Tex_Main_Attack[6]);
    UnloadTexData(m_Tex_Main_Attack[7]);

    UnloadTexData(m_Tex_Main_Walk[0]);
    UnloadTexData(m_Tex_Main_Walk[1]);
    UnloadTexData(m_Tex_Main_Walk[2]);
    UnloadTexData(m_Tex_Main_Walk[3]);
    UnloadTexData(m_Tex_Main_Walk[4]);
    UnloadTexData(m_Tex_Main_Walk[5]);
    UnloadTexData(m_Tex_Main_Walk[6]);
    UnloadTexData(m_Tex_Main_Walk[7]);

    UnloadTexData(m_Tex_Main_Draw);
    UnloadTexData(m_Tex_Main_Idle);
    UnloadTexData(m_Tex_Main_Down);
    
	//終了処理なのでこいつは最後に
	SuperGOE::Uninit(); //終了処理

	//CharaDat.charaData->privateVariable は
	// GOE_UninitCharacterObject の関数内で解放されるので
	//ここで解放しなくてもいいよ
}

// ===================================================
// Update
// 毎フレーム実行
// ===================================================
void GOE_Character_Player_KN::Update()
{
	SuperGOE::Update();


    m_IsDrawLaser = m_IsDrawing && m_InkGage > 0;
}

// ===================================================
// UpdateActive
// 毎フレーム実行
// ただし、ヒットストップ中などの
// 非アクティブ時は実行されない
// ===================================================
void GOE_Character_Player_KN::UpdateActive()
{
	SuperGOE::UpdateActive();

	PLAYER* player = GetPlayer(m_Obj);
	if (player == nullptr)
	{
		return;
	}
	//
	//↓処理
	//

	{
		//入力処理　適当に

        m_AxisSpriteIndex = (8 - m_CurrentAxisEx + 1) % 8;
        //DebugPrintf("%d\n", m_AxisSpriteIndex);

		if (!m_CharaData->playingMotion.enable)
		{
			//Character::PlayMotionCharacterGameObject(
			//	CharaDat.charaData,
			//	Character::GetCharacterMotionFindByName(Character::GetCharacterMainData(CharaDat.charaData->dataIndex), "IDLE"),
			//	false
			//);
		}

        ////攻撃時の残像を生成
        //if (m_IsNormalAttack && m_Obj->moveRequ.enable)
        //{
        //    if (m_CharaData->playingMotion.frame >= ATTACK_START_FRAME
        //        && m_CharaData->playingMotion.frame < ATTACK_TOTAL_FRAME - ATTACK_FTH_FRAME * 0.78
        //        && m_CharaData->playingMotion.frame % 2 == 0
        //    )
        //    {
        //        GAME_OBJECT* go = GO_CreateAfterEffect(m_Obj, 30, MkF4(0.8f, 0.75f, 1.0f, 0.75f));
        //    }
        //}

        if (m_Obj->frame != 0 && m_IsDrawLaser && m_Obj->frame % 6 == 0)
        {
            CameraShakeRequest(
                player->index,
                CAMERA_SHAKE_SMALL_FRAME,
                CAMERA_SHAKE_MIDIUM_POWER * 0.75f,
                CAMERA_SHAKE_MIDIUM_POWER * 0.75f,
                true
            );
        }

        if (m_HammerAttackAnimFrame >= 0)
        {
            int chargeF = (m_HammerAttackChargeFrame - ATTACK_CHARGE_START_FRAME);
            int chargeMaxF = (ATTACK_CHARGE_MAX_FRAME - ATTACK_CHARGE_START_FRAME);
            float chargeMod = std::max(std::min(
                (float)(chargeF) / chargeMaxF,
                1.0f), 0.0f);

            if (!m_HammerAttackStartFlag)
            {
                if (IsInput_NormalAttackPress())
                {
                    Float3 bvecn = NormalizeVector(m_BeforeVecBuf);

                    //モーション再生
                    GraphicSpriteSet(
                        m_Tex_Main_Attack[
                            (int)round((CalculateRadToDeg(atan2(bvecn.y, bvecn.x)) - 90.0f + 360.0f) / (360.0f / 8)) % 8
                        ],
                        4, 3);
                    //GraphicSpriteSet(m_Tex_Main_Attack[std::max(m_CurrentAxisEx - 1, 0)], 4, 3);

                    m_Obj->graphic.graph.scale.x = 2.8f;
                    m_Obj->graphic.graph.scale.y = 2.8f;

                    m_Obj->graphic.pos3d = m_GraphPosDefault;
                    m_Obj->graphic.pos3d.x += cos(-1.2f * m_Obj->frame) * 60.0f * chargeMod;
                    m_Obj->graphic.pos3d.y += sin(-0.7f * m_Obj->frame) * 80.0f * chargeMod;

                    Character::PlayMotionCharacterGameObject(
                        m_CharaData,
                        Character::GetCharacterMotionFindByName(
                            Character::GetCharacterMainData(m_CharaData->dataIndex),
                            "ATTACK_CHARGE"),
                        true
                        //nullptr,
                        //ATTACK_TOTAL_FRAME
                    );
                    //m_CharaData->playingMotion.frame = 1;

                    if (m_HammerAttackChargeFrame >= ATTACK_CHARGE_START_FRAME)
                    {
                        SetCameraMoveActionFlag();

                        if (m_HammerAttackChargeFrame % (int)(16 + -8 * chargeMod) == 0)
                        {
                            SoundEffectPlayRequest(
                                GetSoundData("SOUND/SE/player/attack/ready2"),
                                3.0f, 0.0f, 1.0f + 1.0f * chargeMod);
                        }

                        if (m_HammerAttackChargeFrame % 10 == 0)
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
                                    6.0f * (1.0f + (1.4f * chargeMod)),
                                    6.0f * (1.0f + (1.4f * chargeMod)),
                                    6.0f * (1.0f + (1.4f * chargeMod))
                                );
                                GameObjectSetLifeTime(go, 30);
                            }
                        }
                    }

                    if (m_HammerAttackChargeFrame == (int)(ATTACK_CHARGE_MAX_FRAME * ATTACK_CHARGE_LV1_MOD))
                    {
                        SoundEffectPlayRequest(
                            GetSoundData("SOUND/SE/player/attack/ready1"),
                            3.0f, 0.0f, 0.8f);
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
                                    10.0f,
                                    10.0f,
                                    10.0f
                                );
                                GameObjectSetLifeTime(go, 40);
                            }
                        }
                    }
                    //if (m_HammerAttackChargeFrame == (int)(ATTACK_CHARGE_MAX_FRAME * ATTACK_CHARGE_LV2_MOD))
                    //{
                    //    SoundEffectPlayRequest(
                    //        GetSoundData("SOUND/SE/player/attack/ready1"),
                    //        3.0f, 0.0f, 1.0f);
                    //    {
                    //        Float3 postmp = m_Obj->pos;
                    //        postmp.z += ONE_BLOCK * -4.0f;

                    //        GAME_OBJECT* go = GO_CreateEffect(
                    //            GO_EFFECT_TYPE_HAMMER_CHARGE_END,
                    //            postmp);

                    //        if (go != nullptr)
                    //        {
                    //            go->graphic.color = MkF4(
                    //                1.0f,
                    //                1.0f,
                    //                1.0f,
                    //                0.8f
                    //            );
                    //            go->scale = MkF3(
                    //                15.0f,
                    //                15.0f,
                    //                15.0f
                    //            );
                    //            GameObjectSetLifeTime(go, 40);
                    //        }
                    //    }
                    //}
                    if (m_HammerAttackChargeFrame == (int)(ATTACK_CHARGE_MAX_FRAME * ATTACK_CHARGE_LV3_MOD))
                    {
                        SoundEffectPlayRequest(
                            GetSoundData("SOUND/SE/player/attack/ready1"),
                            3.0f, 0.0f, 1.2f);
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
                                    30.0f,
                                    30.0f,
                                    30.0f
                                );
                                GameObjectSetLifeTime(go, 40);
                            }
                        }
                    }
                    m_HammerAttackChargeFrame++;
                }
                else
                {
                    SetFlagTtrlAttack();

                    Float3 bvecn = NormalizeVector(m_BeforeVecBuf);

                    //モーション再生
                    GraphicSpriteSet(
                        m_Tex_Main_Attack[
                            (int)round((CalculateRadToDeg(atan2(bvecn.y, bvecn.x)) - 90.0f + 360.0f) / (360.0f / 8)) % 8
                        ],
                        4, 3);
                    //GraphicSpriteSet(m_Tex_Main_Attack[std::max(m_CurrentAxisEx - 1, 0)], 4, 3);

                    m_Obj->graphic.graph.scale.x = 2.8f;
                    m_Obj->graphic.graph.scale.y = 2.8f;
                    //m_Obj->graphic.pos3d.y = -100.0f;

                    Character::PlayMotionCharacterGameObject(
                        m_CharaData,
                        Character::GetCharacterMotionFindByName(
                            Character::GetCharacterMainData(m_CharaData->dataIndex),
                            "ATTACK"),
                        false,
                        nullptr,
                        ATTACK_TOTAL_FRAME
                    );
                    //m_CharaData->playingMotion.frame = 1;

                    m_HammerAttackStartFlag = true;
                }
            }

            if (m_HammerAttackAnimFrame == 1)
            {
                SoundEffectPlayRequest(
                    GetSoundData("SOUND/SE/player/attack/Normal_Attack"),
                    0.8f, 0.0f, 0.7f);

                {
                    //クールタイム設定
                    m_AttackCooldown = (int)(ATTACK_COOLDOWN_FRAME);
                }
            }

            if (m_HammerAttackAnimFrame == ATTACK_START_FRAME)
            {
                Character::MOTION_COL_DAT motionColDetails =
                    Character::GetMotionColDataFindByName(
                        Character::MOTION_COL_TYPE_ATK,
                        N_ATTACK_TEST_ATK_COL_NAME
                    );

                GAME_OBJECT_COLLISION_ATTACK_CHILD attackColTmp = motionColDetails.atkCol;
                strcpy_s(attackColTmp.tag, GAME_OBJECT_TXT_MAX, N_ATTACK_GENERAL_TAG);

                attackColTmp.subjectFamily[GAME_OBJECT_FAMILY_ALL] = true;
                //attackColTmp.subjectFamily[GAME_OBJECT_FAMILY_ENEMY] = true;
                //attackColTmp.groupNumber = 1;

                attackColTmp.size = Float2ToFloat3(m_Obj->size);
                attackColTmp.size.x *= 4.0f;
                attackColTmp.size.y *= 4.0f;
                attackColTmp.size.z = (float)sqrt(pow(attackColTmp.size.x, 2) + pow(attackColTmp.size.y, 2));
                attackColTmp.pos.z = -attackColTmp.size.z / 2;

                Float3 bvecn = NormalizeVector(m_BeforeVecBuf);
                attackColTmp.pos.x += 540.0f / 2 * bvecn.x + 250.0f * bvecn.x;
                attackColTmp.pos.y += 540.0f / 2 * bvecn.y + 250.0f * bvecn.y;

                //attackColTmp.power *= 1.0f + (7.0f * chargeMod);

                {
                    Float3 postmp = m_Obj->pos;
                    postmp.x += attackColTmp.pos.x;
                    postmp.y += attackColTmp.pos.y + ONE_BLOCK * 2.5f;
                    //postmp.z += ;

                    GAME_OBJECT* go = GO_CreateEffect(
                        GO_EFFECT_TYPE_HAMMER_SHOCKWAVE, 
                        postmp);
                    if (go != nullptr)
                    {
                        go->scale = MkF3(
                            18.0f * (1.0f + (0.5f * chargeMod)),
                            18.0f * (1.0f + (0.5f * chargeMod)),
                            18.0f * (1.0f + (0.5f * chargeMod))
                        );
                        GameObjectSetLifeTime(go, 32);
                    }
                }
                {
                    Float3 postmp = m_Obj->pos;
                    postmp.x += attackColTmp.pos.x;
                    postmp.y += attackColTmp.pos.y;

                    GAME_OBJECT* go = GO_CreateEffect(
                        GO_EFFECT_TYPE_HAMMER_SHOCKDECAL, 
                        postmp);
                    if (go != nullptr)
                    {
                        go->scale = MkF3(
                            14.0f * (1.0f + (1.0f * chargeMod)),
                            14.0f * (1.0f + (1.0f * chargeMod)),
                            14.0f * (1.0f + (1.0f * chargeMod))
                        );
                        GameObjectSetLifeTime(go, 30);
                    }
                }
                SoundEffectPlayRequest(
                    GetSoundData("SOUND/SE/enemy/attack/granade"),
                    1.8f, 0.0f, 0.8f);
                CameraShakeRequestAll(
                    CAMERA_SHAKE_MIDIUM_FRAME,
                    CAMERA_SHAKE_LARGE_POWER * 1.8f
                );

                int basepower = attackColTmp.power;

                if (chargeF >= chargeMaxF * ATTACK_CHARGE_LV1_MOD)
                {
                    attackColTmp.power = basepower * ATTACK_CHARGE_LV1_ATK_MOD;

                    attackColTmp.size.x *= 1.5f;
                    attackColTmp.size.y *= 1.5f;

                    //SEを再生
                    SoundEffectPlayRequest(
                        GetSoundData("SOUND/SE/enemy/attack/Big_Explosion"),
                        1.6f, 0, 1.2f);

                    Float3 postmp = m_Obj->pos;
                    postmp.x += attackColTmp.pos.x;
                    postmp.y += attackColTmp.pos.y + ONE_BLOCK * 3.0;
                    postmp.z += -200.0f;

                    for (int i = 0; i < 8; i++)
                    {
                        Float3 efPos2 = postmp;
                        efPos2.x += sin(2.0f * SYS_GENERAL_PI / 8 * i) * 900.0f;
                        efPos2.y += cos(2.0f * SYS_GENERAL_PI / 8 * i) * 900.0f;

                        GAME_OBJECT* go = GO_CreateEffect(GO_EFFECT_TYPE_EXPLOSION_1, efPos2);
                        if (go != nullptr)
                        {
                            go->scale = MkF3(
                                12.0f,
                                12.0f,
                                12.0f
                            );
                            GameObjectSetLifeTime(go, 40);
                        }
                    }

                    //if (chargeF < chargeMaxF * ATTACK_CHARGE_LV2_MOD)
                    if (chargeF < chargeMaxF * ATTACK_CHARGE_LV3_MOD)
                    {
                        float angle = 180.0f;
                        int bulletnum = 3;
                        for (int i = 0; i < bulletnum; i++)
                        {
                            float radtmp = CalculateDegToRad(
                                CalculateRadToDeg(atan2(bvecn.y, bvecn.x)) - 0.0f
                                - (angle / 2) + (angle / bulletnum) * (0.5f + (float)i)
                            );
                            {
                                Float3 nvec = NormalizeVector(
                                    MkF3(
                                        (float)cos(radtmp) * 3.0f,
                                        (float)sin(radtmp) * 3.0f,
                                        0.0f
                                    ));

                                GOE_Character_PlayerProjectile* goepro =
                                    GOE_Character_PlayerProjectile::CreateProjectile(
                                        GOE_Character_PlayerProjectile::Type::ContriusShockWave,
                                        MkF3(
                                            m_Obj->pos.x + nvec.x * ONE_BLOCK * 6.0f,
                                            m_Obj->pos.y + nvec.y * ONE_BLOCK * 6.0f,
                                            m_Obj->pos.z + nvec.z * ONE_BLOCK * 6.0f
                                        ),
                                        nvec
                                    );
                                if (goepro != nullptr)
                                {
                                    GAME_OBJECT* beamObj = goepro->GetGameObj();
                                    if (beamObj != nullptr)
                                    {
                                        beamObj->scale.x = 2.5f * m_Obj->scale.x;
                                        beamObj->scale.y = 2.5f * m_Obj->scale.y;
                                        beamObj->scale.z = 2.5f * m_Obj->scale.z;
                                        {
                                            Character::MOTION_COL_DAT motionColDetails =
                                                Character::GetMotionColDataFindByName(
                                                    Character::MOTION_COL_TYPE_ATK,
                                                    HAMMER_SHOCKWAVE_COL_NAME
                                                );

                                            GAME_OBJECT_COLLISION_ATTACK_CHILD attackColTmp = motionColDetails.atkCol;
                                            strcpy_s(attackColTmp.tag, GAME_OBJECT_TXT_MAX,
                                                N_ATTACK_SHOCKWAVE_GENERAL_TAG);
                                            attackColTmp.subjectFamily[GAME_OBJECT_FAMILY_ALL] = true;
                                            //attackColTmp.groupNumber = 1;

                                            attackColTmp.size = MkF3(
                                                ONE_BLOCK * 4,
                                                ONE_BLOCK * 4,
                                                ONE_BLOCK * 4
                                            );
                                            attackColTmp.pos.z = -attackColTmp.size.z / 2;
                                            attackColTmp.pos.y += attackColTmp.size.y / 2 * 1.0f;

                                            beamObj->collision.atk.enable = true;
                                            beamObj->collision.atk.ownerId = GetIdentifierGameObject(m_Obj);
                                            beamObj->collision.atk.collision[0] = attackColTmp;
                                            beamObj->collision.atk.collision[0].enable = true;
                                        }
                                        beamObj->blockCollisionIgnore = false;

                                        GameObjectSetLifeTime(beamObj, 70);
                                        GameObjectMoveRequest(beamObj, 70,
                                            MkF3(
                                                beamObj->pos.x + bvecn.x * ONE_BLOCK * 75.0f,
                                                beamObj->pos.y + bvecn.y * ONE_BLOCK * 75.0f,
                                                beamObj->pos.z
                                            ),
                                            nullptr
                                        );
                                    }
                                }
                            }
                        }
                    }
                }

                //if (chargeF >= chargeMaxF * ATTACK_CHARGE_LV2_MOD)
                if (chargeF >= chargeMaxF * ATTACK_CHARGE_LV3_MOD)
                {
                    //attackColTmp.power = basepower * ATTACK_CHARGE_LV2_ATK_MOD;

                    //attackColTmp.size.x *= 1.0f + (2.6f * chargeMod);
                    //attackColTmp.size.y *= 1.0f + (2.6f * chargeMod);

                    //attackColTmp.size = Float2ToFloat3(m_Obj->size);
                    //attackColTmp.size.x *= 9.0f;
                    //attackColTmp.size.y *= 9.0f;
                    //attackColTmp.size.z *= 4.0f;

                    //attackColTmp.size.x *= 1.75f;
                    //attackColTmp.size.y *= 1.75f;
                    attackColTmp.size.x *= 2.25f;
                    attackColTmp.size.y *= 2.25f;

                    //SEを再生
                    SoundEffectPlayRequest(
                        GetSoundData("SOUND/SE/enemy/attack/Big_Explosion"),
                        2.5f, 0, 0.7f);

                    CameraFlashRequest(
                        player->index,
                        30,
                        EasingInOutSine,
                        MkF4(10.0f, 10.0f, 5.0f, 0.6f),
                        DPD_SHADER_OPT_BLEND_STATE_ADDCOLOR
                    );

                    Float3 postmp = m_Obj->pos;
                    postmp.x += attackColTmp.pos.x;
                    postmp.y += attackColTmp.pos.y + ONE_BLOCK * 4.0;
                    postmp.z += -400.0f;

                    for (int i = 0; i < 8; i++)
                    {
                        Float3 efPos2 = postmp;
                        efPos2.x += sin(2.0f * SYS_GENERAL_PI / 8 * i) * 1500.0f;
                        efPos2.y += cos(2.0f * SYS_GENERAL_PI / 8 * i) * 1500.0f;

                        GAME_OBJECT* go = GO_CreateEffect(GO_EFFECT_TYPE_EXPLOSION_1, efPos2);
                        if (go != nullptr)
                        {
                            go->scale = MkF3(
                                18.0f,
                                18.0f,
                                18.0f
                            );
                            GameObjectSetLifeTime(go, 60);
                        }
                    }
                }

                if(chargeF >= chargeMaxF * ATTACK_CHARGE_LV3_MOD)
                {
                    attackColTmp.power = basepower * ATTACK_CHARGE_LV3_ATK_MOD;

                    SoundEffectPlayRequest(
                        GetSoundData("SOUND/SE/player/attack/activation"),
                        1.2f, 0.0f, 0.75f
                    );
                    SoundEffectPlayRequest(SOUND_TYPE_SE_9, 2.0f, 0, 0.8f);
                    {
                        float angle = 360.0f;
                        int bulletnum = 16;
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

                                GOE_Character_PlayerProjectile* goepro =
                                    GOE_Character_PlayerProjectile::CreateProjectile(
                                        GOE_Character_PlayerProjectile::Type::ContriusShockWave,
                                        MkF3(
                                            m_Obj->pos.x + attackColTmp.pos.x + nvec.x * ONE_BLOCK * 3.0f,
                                            m_Obj->pos.y + attackColTmp.pos.y + nvec.y * ONE_BLOCK * 3.0f,
                                            m_Obj->pos.z + nvec.z * ONE_BLOCK * 3.0f
                                        ),
                                        nvec
                                    );
                                if (goepro != nullptr)
                                {
                                    GAME_OBJECT* beamObj = goepro->GetGameObj();
                                    if (beamObj != nullptr)
                                    {
                                        beamObj->scale.x = 2.5f * m_Obj->scale.x;
                                        beamObj->scale.y = 2.5f * m_Obj->scale.y;
                                        beamObj->scale.z = 2.5f * m_Obj->scale.z;
                                        {
                                            Character::MOTION_COL_DAT motionColDetails =
                                                Character::GetMotionColDataFindByName(
                                                    Character::MOTION_COL_TYPE_ATK,
                                                    HAMMER_SHOCKWAVE_COL_NAME
                                                );

                                            GAME_OBJECT_COLLISION_ATTACK_CHILD attackColTmp = motionColDetails.atkCol;
                                            strcpy_s(attackColTmp.tag, GAME_OBJECT_TXT_MAX,
                                                N_ATTACK_SHOCKWAVE_GENERAL_TAG);
                                            attackColTmp.subjectFamily[GAME_OBJECT_FAMILY_ALL] = true;
                                            //attackColTmp.groupNumber = 1;

                                            attackColTmp.size = MkF3(
                                                ONE_BLOCK * 4,
                                                ONE_BLOCK * 4,
                                                ONE_BLOCK * 4
                                            );
                                            attackColTmp.pos.z = -attackColTmp.size.z / 2;
                                            attackColTmp.pos.y += attackColTmp.size.y / 2 * 1.0f;

                                            beamObj->collision.atk.enable = true;
                                            beamObj->collision.atk.ownerId = GetIdentifierGameObject(m_Obj);
                                            beamObj->collision.atk.collision[0] = attackColTmp;
                                            beamObj->collision.atk.collision[0].enable = true;
                                        }
                                        beamObj->blockCollisionIgnore = false;

                                        GameObjectSetLifeTime(beamObj, 100);
                                        GameObjectMoveRequest(beamObj, 100,
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

                //攻撃判定を生成
                GameObjectAttackCollisionAdd(m_Obj, attackColTmp,
                    0, //ATTACK_START_FRAME,
                    ATTACK_FRAME
                );


            }

            if (m_HammerAttackStartFlag)
            {
                m_HammerAttackAnimFrame++;
            }

            if (m_HammerAttackAnimFrame >= ATTACK_TOTAL_FRAME)
            {
                m_HammerAttackAnimFrame = -1;
            }
        }

		//待機状態なら操作を受け付ける
        {
            bool isNu =
                m_HammerAttackAnimFrame < 0 && (
                !m_CharaData->playingMotion.enable 
                || m_IsIdling 
                || m_IsWalkAnim
                    )
                ;

            if (isNu)
			{ //ループアニメーション更新

				std::string animName = "IDLE";

                int* animp = nullptr;
                int maxf = -1;

                if (m_IsDrawing)
                {
                    animName = "DRAW";
                    animp = &m_AxisSpriteIndex;
                    //if ((m_CharaData->playingMotion.motion->name) == animName)
                    {
                        m_Obj->graphic.pos3d = m_GraphPosDefault;
                        m_Obj->graphic.graph.scale.x = 1.0f;
                        m_Obj->graphic.graph.scale.y = 1.0f;
                        //m_Obj->graphic.pos3d.y = 0.0f;
                        GraphicSpriteSet(m_Tex_Main_Draw, 3, 3, 8);
                    }
                }
                else if (m_IsWalking)
                {
                    animName = "WALK";
                    animp = &m_WalkFrame;
                    maxf = WALK_FRAME;
                    //if ((m_CharaData->playingMotion.motion->name) == animName)
                    {
                        m_Obj->graphic.pos3d = m_GraphPosDefault;
                        m_Obj->graphic.graph.scale.x = 1.0f;
                        m_Obj->graphic.graph.scale.y = 1.0f;
                        //m_Obj->graphic.pos3d.y = 0.0f;
                        GraphicSpriteSet(m_Tex_Main_Walk[std::max(m_CurrentAxisEx - 1, 0)], 4, 3, 11);
                    }
                }
                else
                {
                    animp = &m_AxisSpriteIndex;
                    //if ((m_CharaData->playingMotion.motion->name) == animName)
                    {
                        m_Obj->graphic.pos3d = m_GraphPosDefault;
                        m_Obj->graphic.graph.scale.x = 1.0f;
                        m_Obj->graphic.graph.scale.y = 1.0f;
                        //m_Obj->graphic.pos3d.y = 0.0f;
                        GraphicSpriteSet(m_Tex_Main_Idle, 3, 3, 8);
                    }
                }

                if (!m_CharaData->playingMotion.enable || (m_CharaData->playingMotion.motion->name) != animName)
				{
					Character::PlayMotionCharacterGameObject(
						m_CharaData,
						Character::GetCharacterMotionFindByName(
                            Character::GetCharacterMainData(m_CharaData->dataIndex),
                            animName),
						true,
                        animp,
                        maxf
					);
				}
			}

			if (!m_IsDrawing)
			{ //描いている時ではない

				//通常攻撃
				{
					if (m_AttackCooldown <= 0) //クールタイム中ではない
					{
						//入力をとる
						if (isNu && IsInput_NormalAttackPress())
						{
                            m_HammerAttackStartFlag = false;
                            m_HammerAttackAnimFrame = 0;
                            m_HammerAttackChargeFrame = 0;

                            //if (m_IsWalking)
                            //{
                            //    //無敵時間
                            //    m_Obj->status.invFrame = GOE_Character_Player::ATTACK_TOTAL_FRAME;

                            //    Float3 dashVec = NormalizeVector(attackColTmp.pos);
                            //    GameObjectMoveRequest(m_Obj, GOE_Character_Player::ATTACK_TOTAL_FRAME,
                            //        MkF3(
                            //            m_Obj->pos.x + dashVec.x * ONE_BLOCK * 16.0f,
                            //            m_Obj->pos.y + dashVec.y * ONE_BLOCK * 16.0f,
                            //            m_Obj->pos.z
                            //        ),
                            //        EasingInOutBack
                            //    );

                            //    //クールタイム設定
                            //    m_AttackCooldown = GOE_Character_Player::ATTACK_COOLDOWN_FRAME;
                            //}
                            //else
                            //{
                            //    //クールタイム設定
                            //    m_AttackCooldown = (int)(ATTACK_COOLDOWN_FRAME);
                            //}
						}
					}
				}


                //スキル
                {
                    if (m_SkillBomb.objCreateFrame <= 0
                        && (m_CharaData->playingMotion.motion->name) != "BOMB_SHOOT"
                    )
                    {

                        //入力をとる
                        if (m_SkillCooldownFrame <= 0 && isNu && IsInput_KakoSub())
                        {
                            SetFlagTtrlSkill();

                            SoundEffectPlayRequest(
                                GetSoundData("SOUND/SE/enemy/attack/Small_Explosion"),
                                1.8f, 0.0f, 1.5f);

                            //SetSkillCooldownFrame(BOMB_SET_FRAME
                            //    + FRAME_ONE_SECOND * 5
                            //);
                            //SetSkillCooldownFrame(BOMB_SET_FRAME
                            //    + FRAME_ONE_SECOND * 12
                            //);

                            m_BombSetFrame = 0;
                            m_BombActiveFrame = 0;

                            {
                                Float3 bvecn = NormalizeVector(m_BeforeVecBuf);
                                bvecn.z = 0.3f;
                                bvecn = NormalizeVector(bvecn);


                                GOE_Character_PlayerProjectile* coGoe = GOE_Character_PlayerProjectile::CreateProjectile(
                                    GOE_Character_PlayerProjectile::Type::ContriusBomb,
                                    MkF3(
                                        m_Obj->pos.x + bvecn.x * 120.0f,
                                        m_Obj->pos.y + bvecn.y * 120.0f,
                                        m_Obj->pos.z + bvecn.z * 120.0f - 300.0f
                                    ),
                                    MkF3(
                                        bvecn.x * 70.0f,
                                        bvecn.y * 70.0f,
                                        bvecn.z * 70.0f
                                    )
                                );

                                GAME_OBJECT* coCrd = coGoe->GetGameObj();
                                if (coCrd != nullptr)
                                {
                                    InitGameObjectStatus(&coCrd->status);

                                    coCrd->friction = 0.06f;

                                    coCrd->collision.rec.enable = true;
                                    coCrd->collision.rec.collision[0].enable = true;
                                    coCrd->collision.rec.collision[0].size = {
                                        ONE_BLOCK * 4.0f,
                                        ONE_BLOCK * 4.0f,
                                        ONE_BLOCK * 4.0f,
                                    };
                                    coCrd->collision.rec.collision[0].pos = {};
                                    coCrd->collision.rec.collision[0].pos.z =
                                        -coCrd->collision.rec.collision[0].size.z / 2;
                                    coCrd->collision.rec.collision[0].guardMod = 1.0f;
                                    coCrd->collision.rec.collision[0].subjectFamily[GAME_OBJECT_FAMILY_ALL] = true; 

                                    {
                                        GAME_OBJECT* go = GO_CreateEffect(GO_EFFECT_TYPE_EXPLOSION_1,
                                            coCrd->pos);
                                        if (go != nullptr)
                                        {
                                            //go->vel = co.pos;

                                            go->scale = MkF3(
                                                3.0f,
                                                3.0f,
                                                3.0f
                                            );
                                            GameObjectSetLifeTime(go, 25);
                                        }
                                    }

                                    m_SkillBomb = GetIdentifierGameObject(coCrd);
                                }
                                //GAME_OBJECT co = GetGameObjectPrefab(GAME_OBJECT_TYPE_DUMMY);
                                //co.pos.x = m_Obj->pos.x + bvecn.x * 120.0f;
                                //co.pos.y = m_Obj->pos.y + bvecn.y * 120.0f;
                                //co.pos.z = m_Obj->pos.z + bvecn.z * 120.0f - 300.0f;

                                //co.vel.x = bvecn.x * 70.0f;
                                //co.vel.y = bvecn.y * 70.0f;
                                //co.vel.z = bvecn.z * 70.0f;

                                //co.friction = 0.1f;

                                //co.collision.rec.enable = true;
                                //co.collision.rec.collision[0].enable = true;
                                //co.collision.rec.collision[0].size = {
                                //    ONE_BLOCK * 3.0f,
                                //    ONE_BLOCK * 3.0f,
                                //    ONE_BLOCK * 3.0f,
                                //};
                                //co.collision.rec.collision[0].pos = {};
                                //co.collision.rec.collision[0].pos.z =
                                //    co.collision.rec.collision[0].size.z;
                                //co.collision.rec.collision[0].subjectFamily[GAME_OBJECT_FAMILY_ALL] = true;

                                //GAME_OBJECT* coCrd = CreateGameObject(co);
                                //if (coCrd != nullptr)
                                //{
                                //    m_SkillBomb = GetIdentifierGameObject(coCrd);
                                //}


                                m_Obj->vel.x = bvecn.x * -120.0f;
                                m_Obj->vel.y = bvecn.y * -120.0f;
                                //GameObjectMoveRequest(m_Obj, BOMB_SET_FRAME,
                                //    MkF3(
                                //        m_Obj->pos.x + bvecn.x * -230.0f,
                                //        m_Obj->pos.y + bvecn.y * -230.0f,
                                //        m_Obj->pos.z
                                //    ),
                                //    EasingOutSine
                                //);
                            }

                            m_Obj->graphic.graph.scale.x = 1.0f;
                            m_Obj->graphic.graph.scale.y = 1.0f;
                            GraphicSpriteSet(m_Tex_Main_Draw, 3, 3, 8);
                            Character::PlayMotionCharacterGameObject(
                                m_CharaData,
                                Character::GetCharacterMotionFindByName(
                                    Character::GetCharacterMainData(m_CharaData->dataIndex),
                                    "BOMB_SHOOT"),
                                true,
                                &m_AxisSpriteIndex
                            );

                            CameraShakeRequest(
                                player->index,
                                CAMERA_SHAKE_LARGE_FRAME,
                                CAMERA_SHAKE_LARGE_POWER
                            );
                        }
                    }
                }
			}

			//かこポン
			{
                //反応するキー
                bool keyPushMain = isNu && IsInput_KakoMain();
                //bool keyPushSubTest = IsInput_KakoSub();

                //更新
                for (int i = 0; i < KAKOPON_DAT_NUM; i++)
                {
                    //if (i == 1 && GameManagerIsMovieFlag())
                    //{
                    //    continue;
                    //}
                    update_kakoponDat(&m_KakoDatList[i]);
                }
                //update_kakoponDat(&m_KakoDat_Main);
                //update_kakoponDat(&m_KakoDat_SubTest);


                { //プレイヤーの操作による処理

                    update_kakopon_action(keyPushMain, &m_KakoDatList[0].dat, &m_KakoDatList[0].lineDraw);
                    //update_kakopon_action(keyPushSubTest, &m_KakoDatList[1].dat, &m_KakoDatList[1].lineDraw);

                    //操作キャンセル処理

                    bool cancel_All = true;
                    for (int i = 0; i < KAKOPON_DAT_NUM; i++)
                    {
                        if (!m_KakoDatList[i].lineDraw.vertexDrawFlag 
                            && !m_KakoDatList[i].dat.isConnected && m_KakoDatList[i].dat.vertexNum > 0
                        )
                        {
                            init_kakoponDat(&m_KakoDatList[i]);
                            //キャンセル時にゲージを戻す
                            m_InkGage = m_InkGage_DrawBefore;
                        }
                        else
                        {
                            cancel_All = false;
                        }
                    }
                }

                //囲った時の判定
                if (m_KakoDatList[0].dat.objs.size() > 0 && !m_KakoDatList[0].dat.isFirstObjectCheck)
                {
                    //攻撃
                    //kakopon_objects_attack(&m_KakoDatList[0].dat);

                    //SEを入れる
                    //SoundEffectPlayRequest(SOUND_TYPE_SE_3, 4.0f, 0.0f);
                    //if (m_KakoDatList[0].dat.IsSuccess)

                    //int tgtnum = 0;
                    ////オブジェクトを検証
                    //for (auto v : m_KakoDatList[0].dat.objs)
                    //{
                    //    GAME_OBJECT* ob = GetGameObjectFindByIdentifier(v);

                    //    if (is_kakopon_objects_attack_target(ob))
                    //    {
                    //        tgtnum++;
                    //    }
                    //}
                    //if(tgtnum > 0)
                    {
                        m_KakoDatList[0].dat.isFirstObjectCheck = true;

                        kakopon_attack_setBuf(&m_KakoDatList[0].dat);

                        Character::MOTION_COL_DAT motionColDetails =
                            Character::GetMotionColDataFindByName(
                                Character::MOTION_COL_TYPE_ATK,
                                KAKOPON_BEFORE_KN_COL_NAME
                            );

                        for (auto v : m_KakoDatList[0].dat.objs)
                        {
                            GAME_OBJECT* ob = GetGameObjectFindByIdentifier(v);

                            if (is_kakopon_objects_attack_target(ob))
                            {
                                if (ob->collision.rec.enable)
                                { //くらい判定が有効なら処理する

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
                                        //GameObjectHitStop(
                                        //    ob,
                                        //    (int)(m_KakoDatList[0].dat.frameMax)
                                        //);
                                        GameObjectDamageCollisionHit(ob, m_Obj,
                                            motionColDetails.atkCol,
                                            colRec
                                        );

                                        if (ob->hitStop.frame > 0)
                                        {
                                            ob->hitStop.frameMax = (int)(m_KakoDatList[0].dat.frameMax);
                                            ob->hitStop.frame = ob->hitStop.frameMax;
                                        }
                                    }
                                }
                            }
                        }

                        SoundEffectPlayRequest(
                            GetSoundData("SOUND/SE/player/attack/activation"),
                            3.0f, 0.0f);
                    }
                }

                if (
                    //m_KakoDatList[0].dat.isFirstObjectCheck &&
                    m_KakoDatList[0].dat.objs.size() > 0 
                    && m_KakoDatList[0].dat.frame
                    == (int)(m_KakoDatList[0].dat.frameMax * 0.5))
                {
                    //m_KakoDatBuf.dat.objs.clear();
                    //m_KakoDatBuf.dat.objs = m_KakoDatList[0].dat.objs;

                    int activenum = 0;
                    for (auto objId: m_KakoDatList[0].dat.objs)
                    {
                        GAME_OBJECT* obj = GetGameObjectFindByIdentifier(objId);
                        if (IsActiveGameObject(obj) 
                            && IsBelongsFamilyGameObject(obj, 
                                GAME_OBJECT_FAMILY_ENEMY)
                        )
                        {
                            activenum++;

                            Float3 efPos = obj->pos;
                            efPos.z += ONE_BLOCK * -2.5f;
                            float sca = 0.8f;
                            //GO_CreateExplosion(efPos, sca);

                            {
                                Float3 efPos1 = efPos;
                                efPos1.y += -10.0f;
                                efPos1.z += 96.0f * -1.5f * sca * 0.7f;

                                GAME_OBJECT* go = GO_CreateEffect(
                                    GO_EFFECT_TYPE_EXPLOSION_2, 
                                    efPos1);
                                if (go != nullptr)
                                {
                                    go->scale = MkF3(
                                        18.0f * sca,
                                        18.0f * sca,
                                        18.0f * sca
                                    );
                                    GameObjectSetLifeTime(go, 36);
                                }
                            }
                        }
                    }

                    //if (activenum > 0)
                    {
                        CameraFlashRequest(
                            player->index,
                            //30,
                            10,
                            EasingInOutSine,
                            //MkF4(0.9f, 0.7f, 0.5f, 0.8f),
                            MkF4(10.0f, 10.0f, 5.0f, 0.6f),
                            DPD_SHADER_OPT_BLEND_STATE_ADDCOLOR
                        );

                        //CameraShakeRequest(player->index, CAMERA_SHAKE_LARGE_FRAME, CAMERA_SHAKE_EXLARGE_POWER);
                    }

                    //攻撃
                    kakopon_objects_attack(&m_KakoDatList[0].dat);

                    //SEを入れる
                    //SoundEffectPlayRequest(SOUND_TYPE_SE_3, 4.0f, 0.0f);
                }


                
			}
		}

	}


    if (m_IsDrawLaser)
    {
        Character::MOTION_COL_DAT motionColDetails =
            Character::GetMotionColDataFindByName(
                Character::MOTION_COL_TYPE_ATK,
                KAKO_BEAM_COL_NAME
            );

        GAME_OBJECT_COLLISION_ATTACK_CHILD attackColTmp = motionColDetails.atkCol;
        strcpy_s(attackColTmp.tag, GAME_OBJECT_TXT_MAX,
            N_ATTACK_LASER_GENERAL_TAG);

       // attackColTmp.hitStopAttacker = 1;

        attackColTmp.subjectFamily[GAME_OBJECT_FAMILY_ALL] = true;
        //attackColTmp.subjectFamily[GAME_OBJECT_FAMILY_ENEMY] = true;

        attackColTmp.size = Float2ToFloat3(m_Obj->size);
        attackColTmp.size.x *= 0.75f;
        attackColTmp.size.y *= 0.75f;
        attackColTmp.size.z = (float)sqrt(pow(attackColTmp.size.x, 2) + pow(attackColTmp.size.y, 2));
        attackColTmp.pos.z = -attackColTmp.size.z / 2;

        Float2 vectmp = NormalizeVector(Float3ToFloat2(m_BeforeVec));
        {
            attackColTmp.pos.x += vectmp.x * (m_KakoAddDis - attackColTmp.size.x / 2);
            attackColTmp.pos.y += vectmp.y * (m_KakoAddDis - attackColTmp.size.y / 2);
        }

        m_Obj->collision.atk.collision[2] = attackColTmp;


        if (m_Obj->frameActive % 4 == 0)
        {
            {
                GAME_OBJECT* go = GO_CreateEffect(GO_EFFECT_TYPE_EXPLOSION_1,
                    MkF3(
                        m_Obj->pos.x + vectmp.x * ONE_BLOCK * 3.0f,
                        m_Obj->pos.y + vectmp.y * ONE_BLOCK * 3.0f + ONE_BLOCK * 1.0f,
                        m_Obj->pos.z + -ONE_BLOCK * 2.8f
                    )
                );
                if (go != nullptr)
                {
                    //go->vel = co.pos;

                    go->scale = MkF3(
                        2.0f,
                        2.0f,
                        2.0f
                    );
                    GameObjectSetLifeTime(go, 40);
                }
            }
            {
                GAME_OBJECT* go = GO_CreateEffect(GO_EFFECT_TYPE_EXPLOSION_1,
                    MkF3(
                        m_Obj->pos.x + vectmp.x * m_KakoAddDis + vectmp.x * 10.0f,
                        m_Obj->pos.y + vectmp.y * m_KakoAddDis + vectmp.y * 10.0f + ONE_BLOCK * 1.0f,
                        m_Obj->pos.z + ONE_BLOCK * -0.5f
                    )
                );
                if (go != nullptr)
                {
                    //go->vel = co.pos;

                    go->scale = MkF3(
                        2.5f,
                        2.5f,
                        2.5f
                    );
                    GameObjectSetLifeTime(go, 32);
                }
            }
        }

    }
    else
    {
        m_Obj->collision.atk.collision[2].enable = false;
    }

    //ボム設置のカウント
    if (m_BombSetFrame >= 0)
    {
        m_BombSetFrame++;
        if (m_BombSetFrame > BOMB_SET_FRAME)
        {
            m_BombSetFrame = -1;
            m_CharaData->playingMotion.enable = false; //無理矢理アニメーション停止
        }
    }

    //ボムの処理
    if (!GameManagerIsMovieFlag()
        && m_SkillBomb.objCreateFrame > 0)
    {
        GAME_OBJECT* obj = GetGameObjectFindByIdentifier(m_SkillBomb);
        if (obj != nullptr)
        {
            SetSkillCooldownFrame(
                FRAME_ONE_SECOND * 3.0f
            );

            if (m_BombActiveFrame == 30)
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

            bool isAttacking = m_BombActiveFrame >= BOMB_EXPLOSION_STARTING_FRAME * 0.25f + BOMB_EXPLOSION_START_FRAME
                && m_BombActiveFrame <= BOMB_EXPLOSION_START_FRAME + BOMB_EXPLOSION_ATTACK_FRAME
                ;

            if(!isAttacking)
            {
                if (m_BombActiveFrame % 14 == 0)
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
            if (m_BombActiveFrame == BOMB_EXPLOSION_START_BEFORE_FRAME)
            {
                //SEを再生
                SoundEffectPlayRequest(
                    GetSoundData("SOUND/SE/enemy/attack/Beam_Charge"), 
                    0.3f, 0, 1.2f);
            }
            if (m_BombActiveFrame >= BOMB_EXPLOSION_START_BEFORE_FRAME
                && m_BombActiveFrame < BOMB_EXPLOSION_START_FRAME)
            {
                int f = m_BombActiveFrame - BOMB_EXPLOSION_START_BEFORE_FRAME;
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

            if (m_BombActiveFrame == BOMB_EXPLOSION_START_FRAME - 20)
            {
                SoundEffectPlayRequest(GetSoundData("SOUND/SE/enemy/move/warpHigh"), 1.8f, 0);
            }
            if (m_BombActiveFrame == BOMB_EXPLOSION_START_FRAME)
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
                if (m_BombActiveFrame % 30 == 0)
                {
                    //SEを再生
                    SoundEffectPlayRequest(GetSoundData("SOUND/SE/player/attack/Kakopon_Contorius"),
                        0.32f, 0, 1.1f);
                }
                if (m_BombActiveFrame % 50 == 0)
                {
                    //SEを再生
                    SoundEffectPlayRequest(
                        GetSoundData("SOUND/SE/enemy/attack/Small_Explosion"),
                        4.0f, 0, 0.4);
                }

                if (m_BombActiveFrame % 15 == 0)
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
                if (m_BombActiveFrame % 12 == 0)
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
                if (m_BombActiveFrame % 8 == 0)
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
                if (m_BombActiveFrame % m_BombAttackCol.hitCycle == 0)
                {
                    for (int i = 0; i < GAME_OBJECT_MAX; i++)
                    {
                        GAME_OBJECT* ob = GetGameObject(i);
                        if (IsActiveGameObject(ob)
                            && (ob->type == GAME_OBJECT_TYPE_CHARACTER
                                || IsBelongsFamilyGameObject(ob, GAME_OBJECT_FAMILY_GIMIK))
                        )
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
                                            m_BombAttackCol,
                                            colRec
                                        );
                                    }
                                }
                            }
                        }
                    }
                }
            }

            if (m_BombActiveFrame > BOMB_EXPLOSION_TOTAL_FRAME)
            {
                DeleteGameObject(obj);
                m_SkillBomb = {};
            }

            m_BombActiveFrame++;
        }
        else
        {
            m_SkillBomb = {};
        }
    }

    //SE
    if (m_IsDrawLaser &&
        //m_WalkFrame % 3 == 0
        m_Obj->frameActive % 12 == 0
        )
    {
        SoundEffectPlayRequest(
            GetSoundData("SOUND/SE/player/attack/Kakopon_Contorius_New"),
            1.0f, 0.0f, 1.1f);
    }

    kakopon_final_update();

    UpdateActiveLate();
}

void GOE_Character_Player_KN::UpdateActiveLate()
{
    SuperGOE::UpdateActiveLate();
}


// ===================================================
// Draw
// オブジェクトの描画イベント
// ===================================================
void GOE_Character_Player_KN::Draw()
{
	SuperGOE::Draw();

    PLAYER* player = GetPlayer(m_Obj);
    if (player == nullptr)
    {
        return;
    }
    //
    //↓処理
    //

    //通常攻撃エフェクト
    if (m_IsNormalAttack)
    { 
        //{
        //    {
        //        float rott = CalculateRadToDeg(atan2(m_AxisVec.y, m_AxisVec.x)) - 90.0f;

        //        //調整用
        //        //if (m_CurrentAxis == AXIS_2D_LEFT) rott += -50.0f;
        //        //if (m_CurrentAxis == AXIS_2D_RIGHT) rott += 60.0f;
        //        //if (m_CurrentAxis == AXIS_2D_UP) rott += -100.0f;

        //        DRAW_SPRITE_DAT dsdat = {};
        //        dsdat.x = 0.0f;
        //        dsdat.y = m_Obj->size.y * 0.02f;

        //        dsdat.w = m_Obj->size.x * 2.4f;
        //        dsdat.h = m_Obj->size.y * 2.4f * 1.f;

        //        dsdat.rot = 0.0f;
        //        dsdat.color = { 1.0f, 1.0f, 0.0f, 1.0f };

        //        dsdat.texNo = m_TexId_AttackEffect_s;
        //        dsdat.loadTexType = LOADTEXTURETYPE_MAIN;

        //        //dsdat.texUV = { 0.0f, 0.0f };
        //        //dsdat.texWH = { 1.0f, 1.0f };
        //        dsdat.texUV = AnimationUV(
        //            (int)(
        //                m_AnimCount_AttackEffect_s.x * m_AnimCount_AttackEffect_s.y
        //                * ((float)m_CharaData->playingMotion.frame / ATTACK_TOTAL_FRAME)
        //            ),
        //            (int)m_AnimCount_AttackEffect_s.x,
        //            (int)m_AnimCount_AttackEffect_s.y
        //        );
        //        dsdat.texWH = { 1.0f / m_AnimCount_AttackEffect_s.x, 1.0f / m_AnimCount_AttackEffect_s.y };

        //        dsdat.startPos3D = m_Obj->pos;
        //        dsdat.startPos3D.z += -dsdat.h * 0.12f;

        //        dsdat.startRot = rott;
        //        dsdat.startScale = { 1.0f, 1.0f };

        //        dsdat.shaderOpt.bloom = true;

        //        DrawSpriteInCamera(dsdat);
        //    }
        //}
    }

    if (m_IsDrawLaser)
    {
        DRAW_POLYGON_DAT drawDatBaseTmp = {};
        drawDatBaseTmp.size = MkF3(1.0f, 1.0f, 240.0f);

        drawDatBaseTmp.pos.z = drawDatBaseTmp.size.z;

        drawDatBaseTmp.rot = 0.0f;

        drawDatBaseTmp.rot3D = MkF3(0.0f, 0.0f, 0.0f);

        //drawDatTmp.color = MkF4(1.0f, 0.8f, 1.0f, 0.9f);
        //drawDatBaseTmp.color = MkF4(1.0f * 2, 0.9f * 2, 1.0f * 2, 0.9f);
        //drawDatBaseTmp.color = MkF4(0.9f, 0.3f, 0.3f, 0.9f);
        //drawDatTmp.color = MkF4(3.0f, 1.0f, 7.0f, 0.9f);

        //drawDatTmp.texNo = LoadTexture("rom/texture/ui/in_game/charactor_icon.png");
        drawDatBaseTmp.texNo = 0;
        drawDatBaseTmp.loadTexType = LOADTEXTURETYPE_MAIN;

        drawDatBaseTmp.texUV = { 0.0f, 0.0f };
        drawDatBaseTmp.texWH = { 1.0f, 1.0f };

        drawDatBaseTmp.startPos3D = m_Obj->pos;

        drawDatBaseTmp.startRot = CalculateRadToDeg(atan2(
            m_BeforeVec.y,
            m_BeforeVec.x)) - 90.0f;
        drawDatBaseTmp.startRot3D = MkF3(90.0f + 32.0f, 0.0f, 0.0f);

        drawDatBaseTmp.startScale = MkF2(1.0f, 1.0f);
        drawDatBaseTmp.startScale3D = MkF3(1.0f, 1.0f, 1.0f);

        drawDatBaseTmp.modelNo = m_Model_Beam;

        //drawDatBaseTmp.shaderOpt.bloom = DPD_SHADER_OPT_BLOOM_NORMAL;
        //drawDatBaseTmp.shaderOpt.blendState = DPD_SHADER_OPT_BLEND_STATE_ADDCOLOR;

        DRAW_POLYGON_IN_CAMERA_EX_OPTION optBaseTmp = {};
        optBaseTmp.reverse = false;
        optBaseTmp.fixed = false;
        optBaseTmp.enableShade = true;
        //optBaseTmp.priority = DRAW_POLYGON_PRIORITY_NML_d1;
        optBaseTmp.priority = DRAW_POLYGON_PRIORITY_NORMAL;
        //optBaseTmp.depthBuffIgnore = true;
        //opt.priority = DRAW_POLYGON_PRIORITY_FORCE_NORMAL;
        optBaseTmp.shaderType = DPD_SHADER_TYPE_EFFECT;

        {
            {
                //float startMod = EasingInOutElastic((float)(beamFrameTmp + startbeforef) / startbeforef);
                float startMod = 1.0f;

                DRAW_POLYGON_DAT drawDatTmp2 = drawDatBaseTmp;
                //drawDatTmp2.texNo = m_Tex_Beam;
                drawDatTmp2.texNo = 0;
                drawDatTmp2.size.x = 16.0f * startMod;
                drawDatTmp2.size.y = 16.0f * startMod;

                {
                    float rotTmp = CalculateDegToRad(drawDatBaseTmp.startRot + 180.0f);
                    drawDatTmp2.pos.x += cos(rotTmp) * ONE_BLOCK * 0.4f;
                    drawDatTmp2.pos.y += sin(rotTmp) * ONE_BLOCK * 0.4f;
                }

                drawDatTmp2.pos.z += drawDatTmp2.size.z * -1.6f;

                drawDatTmp2.startPos3D.z += -440.0f;
                //drawDatTmp2.rot3D.x += 35.0f;

                //drawDatTmp2.color = MkF4(1.0f, 0.2f, 0.3f, 0.75f);
                drawDatTmp2.color = MkF4(0.7f, 0.0f, 0.1f, 0.7f * m_MovieVisibleAlpha);
                drawDatTmp2.shaderOpt.blendState = DPD_SHADER_OPT_BLEND_STATE_ADDCOLOR;
                //drawDatTmp2.color = MkF4(2.0f, 0.5f, 0.5f, 0.75f);
                DRAW_POLYGON_IN_CAMERA_EX_OPTION opt2 = optBaseTmp;
                //opt2.depthBuffIgnore = true;
                opt2.priority = DRAW_POLYGON_PRIORITY_NORMAL;
                DrawPolygonInCameraEx(drawDatTmp2, opt2);


                DRAW_POLYGON_DAT drawDatTmp3 = drawDatTmp2;
                drawDatTmp3.size.x *= 0.4f;
                drawDatTmp3.size.y *= 0.4f;
                drawDatTmp3.color = MkF4(1.5f, 1.2f, 1.2f, 1.0f * m_MovieVisibleAlpha);
                DRAW_POLYGON_IN_CAMERA_EX_OPTION opt3 = opt2;
                opt3.priority = (DRAW_POLYGON_PRIORITY)(opt3.priority - 1);
                //opt3.priority = DRAW_POLYGON_PRIORITY_NML_u1;
                DrawPolygonInCameraEx(drawDatTmp3, opt3);
            }
        }
    }



    if(!GameManagerIsMovieFlag()
        && m_SkillBomb.objCreateFrame > 0)
    {
        GAME_OBJECT* obj = GetGameObjectFindByIdentifier(m_SkillBomb);
        if (obj != nullptr)
        {
            if (m_BombActiveFrame <= BOMB_EXPLOSION_START_FRAME)
            {
                float enterf = std::min(std::max(
                    (float)(m_BombActiveFrame - 30) / 12,
                    0.0f), 1.0f);
                {
                    DRAW_POLYGON_DAT drawDatBaseTmp = {};
                    drawDatBaseTmp.size = MkF3(
                        1000.0f * obj->scale.x,
                        1000.0f * obj->scale.y,
                        1000.0f * obj->scale.z
                    );

                    drawDatBaseTmp.rot = 0.0f;

                    drawDatBaseTmp.rot3D = MkF3(0.0f, 0.0f, 0.0f);

                    drawDatBaseTmp.color = MkF4(1.0f, 1.0f, 1.0f, 1.0f * m_MovieVisibleAlpha);

                    drawDatBaseTmp.texNo = m_Tex_EnergyBall;
                    drawDatBaseTmp.loadTexType = LOADTEXTURETYPE_MAIN;

                    drawDatBaseTmp.texUV = AnimationUV(obj->frameActive / 3, 4, 4);
                    drawDatBaseTmp.texWH = { 1.0f / 4, 1.0f / 4 };

                    drawDatBaseTmp.startPos3D = obj->pos;

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
                        ddshok.texNo = m_Tex_ShockWave;
                        ddshok.modelNo = m_Model_ShockWave;
                        ddshok.texUV = {
                            0.0f + -0.002f * obj->frameActive,
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
                        ddshok.color = MkF4(0.5f, 0.2f, 0.7f, 1.0f * m_MovieVisibleAlpha);
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
                        0.03f * obj->frame
                    };
                    //drawDatTmp.color = MkF4(0.9f, 0.0f, 0.2f, 0.9f);
                    drawDatTmp.color = MkF4(1.0f, 0.0f, 1.2f, 1.0f * m_MovieVisibleAlpha);

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

                    drawDatTmp.startPos3D = obj->pos;
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

                drawDatBaseTmp.color = MkF4(1.2f, 0.8f, 1.2f, 1.0f * m_MovieVisibleAlpha);
                drawDatBaseTmp.texNo = 0;
                drawDatBaseTmp.loadTexType = LOADTEXTURETYPE_MAIN;

                drawDatBaseTmp.texUV = { 0.0f, 0.0f };
                drawDatBaseTmp.texWH = { 1.0f, 1.0f };

                drawDatBaseTmp.startPos3D = obj->pos;
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

                if (m_BombActiveFrame >= BOMB_EXPLOSION_START_BEFORE_FRAME)
                {

                    DRAW_POLYGON_DAT drawDatTmp = drawDatBaseTmp;

                    Float2 scaleAddTmp = MkF2(0.0f, 0.0f);
                    Float2 scaleMulTmp = MkF2(1.0f, 1.0f);

                    bool isStart = m_BombActiveFrame >= BOMB_EXPLOSION_START_FRAME;
                    if (isStart)
                    {
                        {
                            //float addSizeTmp = sin((float)(m_Obj->frameActive) * 0.8f) * 0.05f;
                            float addSizeTmp = sin((float)(obj->frameActive) * 0.8f) * 20.0f;
                            scaleAddTmp.x += addSizeTmp;
                            scaleAddTmp.y += addSizeTmp;
                            //drawDatTmp.size.z += addSizeTmp;
                        }

                        {
                            int f = m_BombActiveFrame - BOMB_EXPLOSION_START_FRAME - BOMB_EXPLOSION_ATTACK_FRAME;
                            if (f >= 0)
                            {
                                float mod =
                                    (1.0f - std::max(std::min((float)f / BOMB_EXPLOSION_AFTER_FRAME, 1.0f), 0.0f));
                                scaleMulTmp.x *= mod;
                                scaleMulTmp.y *= mod;
                                //drawDatTmp.size.z *= startMod;
                            }
                        }
                        {
                            int f = m_BombActiveFrame - BOMB_EXPLOSION_START_FRAME;
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
                        int f = m_BombActiveFrame - BOMB_EXPLOSION_START_BEFORE_FRAME;
                        if (f >= 0 && f < (BOMB_EXPLOSION_START_FRAME - BOMB_EXPLOSION_START_BEFORE_FRAME))
                        {
                            float mod = std::max(std::min((float)f / 16, 1.0f), 0.0f);
                            drawDatTmp.size.x = 20.0f;
                            drawDatTmp.size.y = 20.0f;
                            drawDatTmp.size.x *= mod ;
                            drawDatTmp.size.y *= mod ;
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
                            drawDatTmp3.texNo = m_Tex_ShockWave;
                            drawDatTmp3.texUV = {
                                0.0f + -0.003f * obj->frameActive,
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
                            drawDatTmp3.color = MkF4(0.55f, 0.25f, 0.7f, 1.0f * m_MovieVisibleAlpha);
                            DRAW_POLYGON_IN_CAMERA_EX_OPTION opt3 = optBaseTmp;
                            //opt3.priority = DRAW_POLYGON_PRIORITY_NML_u3;
                            opt3.depthBuffIgnore = true;
                            opt3.priority = DRAW_POLYGON_PRIORITY_NML_d1;


                            DRAW_POLYGON_DAT drawDatTmp4 = drawDatBaseTmp;
                            drawDatTmp4.texNo = m_Tex_Beam1;
                            drawDatTmp4.texUV = {
                                0.0f + -0.0002f * obj->frameActive,
                                0.0f + 0.026f * obj->frameActive
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
                            drawDatTmp4.color = MkF4(0.6f, 0.2f, 0.45f, 1.0f * m_MovieVisibleAlpha);
                            DRAW_POLYGON_IN_CAMERA_EX_OPTION opt4 = optBaseTmp;
                            opt4.priority = DRAW_POLYGON_PRIORITY_NML_u2;


                            DRAW_POLYGON_DAT drawDatTmp5 = drawDatBaseTmp;
                            //drawDatTmp5.texNo = m_Tex_Beam3;
                            drawDatTmp5.texNo = m_Tex_Beambg;
                            drawDatTmp5.texUV = {
                                0.0f,
                                0.0f + 0.036f * obj->frameActive
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
                            drawDatTmp5.color = MkF4(0.3f, 0.1f, 0.55f, 1.0f * m_MovieVisibleAlpha);
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
                                ddshok.texNo = m_Tex_ShockWave;
                                ddshok.modelNo = m_Model_ShockWave;
                                ddshok.texUV = {
                                    0.0f + -0.003f * obj->frameActive,
                                    0.1f
                                };
                                ddshok.texWH = {
                                    2.0f,
                                    3.0f
                                };
                                ddshok.color = MkF4(0.4f, 0.1f, 1.0f, 1.0f * m_MovieVisibleAlpha);
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
                                drawDatTmp6.color = MkF4(0.1f, 0.0f, 0.3f, 1.0f * m_MovieVisibleAlpha);
                                //drawDatTmp6.texUV = {
                                //    0.0f + 0.01f * obj->frameActive,
                                //    0.0f + 0.02f * obj->frameActive
                                //};
                                drawDatTmp6.texUV = {
                                    0.0f + 0.006f * obj->frameActive,
                                    0.0f + 0.008f * obj->frameActive
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
                    drawDatTmp.color = MkF4(0.95f, 0.4f, 0.6f, 0.9f * m_MovieVisibleAlpha);
                    drawDatTmp.texUV = {
                        0.0f + 0.012f * obj->frameActive,
                        0.0f + 0.01f * obj->frameActive
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
        }
    }
}

// ===================================================
// Dead
// オブジェクトのHPが0になったとき
// ===================================================
void GOE_Character_Player_KN::Dead()
{
	SuperGOE::Dead();

    PLAYER* player = GetPlayer(m_Obj);
    if (player == nullptr)
    {
        return;
    }
    //
    //↓処理
    //

    m_HammerAttackAnimFrame = -1;

    {
        GAME_OBJECT* obj = GetGameObjectFindByIdentifier(m_SkillBomb);
        if (obj != nullptr)
        {
            DeleteGameObject(obj);
        }
    }

    SetDownPoseStart();
    SetDownPose();
}

// ===================================================
// DamageAttacker
// オブジェクトの攻撃判定がヒットしたとき
// ===================================================
void GOE_Character_Player_KN::DamageAttacker(GAME_OBJECT_DAMAGE_RESULT dr)
{
	SuperGOE::DamageAttacker(dr);

	//DebugPrintf("ateta\n");
	//if (dr.colAtk.isGrab)
		//GameObjectMoveRequest(recOb,
		//	20,
		//	MkF3(recOb->pos.x, recOb->pos.y - 600.0f, recOb->pos.z),
		//	EasingInOutSine);

    if(dr.damage > 0)
	{
        if (std::string(dr.colAtk.tag) == N_ATTACK_GENERAL_TAG)
        {
            //SEを再生
            SoundEffectPlayRequest(
                GetSoundData("SOUND/SE/player/attack/atkhit_mid"),
                1.1f);

            if (m_Obj->status.invFrame < 40)
            {
                m_Obj->status.invFrame = 40;
            }
        }
        else if (std::string(dr.colAtk.tag) == N_ATTACK_SHOCKWAVE_GENERAL_TAG)
        {
            //SEを再生
            SoundEffectPlayRequest(
                GetSoundData("SOUND/SE/player/attack/atkhit_mid"),
                1.0f);
        }
        else if (std::string(dr.colAtk.tag) == N_ATTACK_LASER_GENERAL_TAG)
        {
            //SEを再生
            SoundEffectPlayRequest(
                GetSoundData("SOUND/SE/player/attack/atkhit_spk"),
                0.15f);

        }
        else if (std::string(dr.colAtk.tag) == N_ATTACK_BOMBLASER_GENERAL_TAG)
        {
            //SEを再生
            SoundEffectPlayRequest(
                GetSoundData("SOUND/SE/player/attack/atkhit_spk"),
                0.2f);

        }
	}
}

// ===================================================
// DamageReciever
// オブジェクトが
// 別のオブジェクトの攻撃判定にヒットしたとき
// ===================================================
void GOE_Character_Player_KN::DamageReciever(GAME_OBJECT_DAMAGE_RESULT dr)
{
	SuperGOE::DamageReciever(dr);

    if (dr.damage > 0)
    {
        if (IsDefeat())
        {
        }
        else
        {
            if (m_DamageSternFrame < 0)
            {
                if (m_HammerAttackAnimFrame >= 0 
                    && m_HammerAttackChargeFrame >= ATTACK_CHARGE_START_FRAME
                )
                {
                    SetStern(60);
                    m_Obj->status.invFrame += 60;
                }
            }
        }
    }

    if (m_DamageSternFrame >= 0)
    {
        m_HammerAttackAnimFrame = -1;
    }
	//if (dr.damage > 0 || dr.colAtk.isGrab)
	//{

	//}
}

// ===================================================
// BlockHit
// オブジェクトがブロックに触れたとき
// ===================================================
void GOE_Character_Player_KN::BlockHit(GAME_OBJECT_POSMOD_RESULT pmr)
{
	SuperGOE::BlockHit(pmr);
}

void GOE_Character_Player_KN::SetDownPoseStart()
{
    m_DefeatFrameAnimMax = 6;
}

void GOE_Character_Player_KN::SetDownPose()
{
    //モーション再生
    GraphicSpriteSet(
        m_Tex_Main_Down,
        3, 2);
    //m_Obj->graphic.graph.scale.x = 1.5f;
    //m_Obj->graphic.graph.scale.y = 1.5f;
    //m_Obj->graphic.pos3d = m_GraphPosDefault;
    //m_Obj->graphic.pos3d.y += -60.0f;
    //m_Obj->graphic.pos3d.z += -20.0f;
    m_Obj->graphic.pos3d = m_GraphPosDefault;
    m_Obj->graphic.pos3d.y += 140.0f;
    m_Obj->graphic.pos3d.z += 220.0f;
    m_Obj->graphic.graph.scale.x = 1.0f;
    m_Obj->graphic.graph.scale.y = 1.0f;

    Character::PlayMotionCharacterGameObject(
        m_CharaData,
        Character::GetCharacterMotionFindByName(
            Character::GetCharacterMainData(m_CharaData->dataIndex),
            "DOWN"),
        true,
        &m_DefeatFrameAnim,
        -1
    );
}
