// ===================================================
// GOE_Character_Player_NEO.cpp キャラクターオブジェクト
// 
// hara sougo  2025/01/26
// ===================================================
#define NO_CONTROLLER

#include "../../../../Game/Configuration/Param.h"
#include "../../../../game_main.h"
#include "../../../Sprite/sprite.h"
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

#include "GOE_Character_Player_NEO.h"

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
void GOE_Character_Player_NEO::Init()
{
	SuperGOE::Init(); //初期化

    //アイコン
    m_TexIconMain_Mod *= 1.2f;
    m_TexIconMain_N = LoadTexData("TEX/ui/in_game_master/icon-assets/neo_icon");
    m_TexIconMain_Pinch = LoadTexData("TEX/ui/in_game_master/icon-assets/neo_pinch");
    m_TexIconMain_Out = LoadTexData("TEX/ui/in_game_master/icon-assets/neo_out");

    m_TexIconSub = LoadTexData("TEX/ui/in_game_master/neo_icon");
    m_TexIconSkillOn = LoadTexData("TEX/ui/in_game_master/skill_heal_on");
    m_TexIconSkillOff = LoadTexData("TEX/ui/in_game_master/skill_heal_off");

    SetCharaSubGraphic();

    GraphicSet(m_Tex_Main_Idle, 3, 3);
    //m_Obj->graphic.pos3d.y += -140.0f;
    //m_GraphPosDefault = m_Obj->graphic.pos3d;
    //m_Obj->graphic.graph.spriteAnim->pos.y += -30.0f;
    //m_Obj->graphic.graph.spriteAnim->pos3D.z += -80.0f;

    m_MoveSpeedMod = MOVE_SPEED_MOD;
    m_MoveSpeedModKako = MOVE_SPEED_MOD_KAKO;

    m_Obj->friction *= MOVE_FRICTION_MOD;


    //かこぽん

    m_KakoAddDis = ONE_BLOCK * 0.5f;

    m_KakoDamName = ("COL_ATK_KAKOPON_NEO");
    //m_KakoDamAddMod = 1.0f;
    m_KakoDamAddMod = 1.2f;
    //m_KakoDamAddMod = 0.9f;
    m_KakoInkCostMod = 0.85f;

    m_ThemeColor = { 2.0f, 0.2f, 1.3f };
    
    //m_KakoDat_Main.graphic = KAKOPON_GRAPHIC::Main;
    //m_KakoDat_Main.type = KAKOPON_TYPE::Main;
    //m_KakoDat_Main.frameMax = GOE_Character_Player::KAKOPON_COOLDOWN_FRAME;

    //m_KakoDat_SubTest.graphic = KAKOPON_GRAPHIC::Main;
    //m_KakoDat_SubTest.type = KAKOPON_TYPE::Heal;
    //m_KakoDat_SubTest.frameMax = FRAME_ONE_SECOND * 5;
    m_KakoDatList[0].dat.graphic = KAKOPON_GRAPHIC::Main_Neo;
    m_KakoDatList[0].dat.type = KAKOPON_TYPE::Main;
    m_KakoDatList[0].dat.frameMax = KAKOPON_COOLDOWN_FRAME;

    m_KakoDatList[1].dat.graphic = KAKOPON_GRAPHIC::Heal;
    m_KakoDatList[1].dat.type = KAKOPON_TYPE::Heal;
    //m_KakoDatList[1].dat.frameMax = FRAME_ONE_SECOND * 5;
    m_KakoDatList[1].dat.frameMax = FRAME_ONE_SECOND * 4;

    ////仮
    //m_Obj->graphic.color = { 1.0f, 0.5f, 0.6f, 1.0f };
    //{
    //    GAME_OBJECT_COLLISION_RECIEVE_CHILD* col = m_Obj->collision.rec.collision + 0;
    //    //col->pos.z += ONE_BLOCK * -4.0f;
    //    col->size.x *= 2000.0f;
    //    col->size.y *= 2000.0f;
    //    col->size.z *= 1.0f;
    //}
}

void GOE_Character_Player_NEO::SetCharaSubGraphic()
{
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

    UnloadTexData(m_Tex_Main_Draw[0]);
    UnloadTexData(m_Tex_Main_Draw[1]);
    UnloadTexData(m_Tex_Main_Draw[2]);
    UnloadTexData(m_Tex_Main_Draw[3]);
    UnloadTexData(m_Tex_Main_Draw[4]);
    UnloadTexData(m_Tex_Main_Draw[5]);
    UnloadTexData(m_Tex_Main_Draw[6]);
    UnloadTexData(m_Tex_Main_Draw[7]);

    UnloadTexData(m_Tex_Main_Idle);

    std::string pstr = m_OnajiCharaNum > 0 ? std::to_string(m_OnajiCharaNum + 1) : "";

    m_Tex_Main_Attack[0] = LoadTexData("TEX/player/neo" + pstr + "/attack/Neo" + pstr + "_attack_01");
    m_Tex_Main_Attack[1] = LoadTexData("TEX/player/neo" + pstr + "/attack/Neo" + pstr + "_attack_02");
    m_Tex_Main_Attack[2] = LoadTexData("TEX/player/neo" + pstr + "/attack/Neo" + pstr + "_attack_03");
    m_Tex_Main_Attack[3] = LoadTexData("TEX/player/neo" + pstr + "/attack/Neo" + pstr + "_attack_04");
    m_Tex_Main_Attack[4] = LoadTexData("TEX/player/neo" + pstr + "/attack/Neo" + pstr + "_attack_05");
    m_Tex_Main_Attack[5] = LoadTexData("TEX/player/neo" + pstr + "/attack/Neo" + pstr + "_attack_06");
    m_Tex_Main_Attack[6] = LoadTexData("TEX/player/neo" + pstr + "/attack/Neo" + pstr + "_attack_07");
    m_Tex_Main_Attack[7] = LoadTexData("TEX/player/neo" + pstr + "/attack/Neo" + pstr + "_attack_08");

    m_Tex_Main_Walk[0] = LoadTexData("TEX/player/neo" + pstr + "/move/Neo" + pstr + "_walk_01");
    m_Tex_Main_Walk[1] = LoadTexData("TEX/player/neo" + pstr + "/move/Neo" + pstr + "_walk_02");
    m_Tex_Main_Walk[2] = LoadTexData("TEX/player/neo" + pstr + "/move/Neo" + pstr + "_walk_03");
    m_Tex_Main_Walk[3] = LoadTexData("TEX/player/neo" + pstr + "/move/Neo" + pstr + "_walk_04");
    m_Tex_Main_Walk[4] = LoadTexData("TEX/player/neo" + pstr + "/move/Neo" + pstr + "_walk_05");
    m_Tex_Main_Walk[5] = LoadTexData("TEX/player/neo" + pstr + "/move/Neo" + pstr + "_walk_06");
    m_Tex_Main_Walk[6] = LoadTexData("TEX/player/neo" + pstr + "/move/Neo" + pstr + "_walk_07");
    m_Tex_Main_Walk[7] = LoadTexData("TEX/player/neo" + pstr + "/move/Neo" + pstr + "_walk_08");

    m_Tex_Main_Draw[0] = LoadTexData("TEX/player/neo" + pstr + "/kako/Neo" + pstr + "_skill_01");
    m_Tex_Main_Draw[1] = LoadTexData("TEX/player/neo" + pstr + "/kako/Neo" + pstr + "_skill_02");
    m_Tex_Main_Draw[2] = LoadTexData("TEX/player/neo" + pstr + "/kako/Neo" + pstr + "_skill_03");
    m_Tex_Main_Draw[3] = LoadTexData("TEX/player/neo" + pstr + "/kako/Neo" + pstr + "_skill_04");
    m_Tex_Main_Draw[4] = LoadTexData("TEX/player/neo" + pstr + "/kako/Neo" + pstr + "_skill_05");
    m_Tex_Main_Draw[5] = LoadTexData("TEX/player/neo" + pstr + "/kako/Neo" + pstr + "_skill_06");
    m_Tex_Main_Draw[6] = LoadTexData("TEX/player/neo" + pstr + "/kako/Neo" + pstr + "_skill_07");
    m_Tex_Main_Draw[7] = LoadTexData("TEX/player/neo" + pstr + "/kako/Neo" + pstr + "_skill_08");

    m_Tex_Main_Idle = LoadTexData("TEX/player/neo" + pstr + "/Neo" + pstr + "_idle");

    m_Tex_Main_Down = LoadTexData("TEX/player/neo" + pstr + "/Neo" + pstr + "_damage_01");
}
// ===================================================
// FirstFrame
// 最初のフレームのみ実行
// ===================================================
void GOE_Character_Player_NEO::FirstFrame()
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

// ===================================================
// Uninit
// オブジェクトが削除されるとき
// ===================================================
void GOE_Character_Player_NEO::Uninit()
{

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

    UnloadTexData(m_Tex_Main_Draw[0]);
    UnloadTexData(m_Tex_Main_Draw[1]);
    UnloadTexData(m_Tex_Main_Draw[2]);
    UnloadTexData(m_Tex_Main_Draw[3]);
    UnloadTexData(m_Tex_Main_Draw[4]);
    UnloadTexData(m_Tex_Main_Draw[5]);
    UnloadTexData(m_Tex_Main_Draw[6]);
    UnloadTexData(m_Tex_Main_Draw[7]);

    UnloadTexData(m_Tex_Main_Idle);

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
void GOE_Character_Player_NEO::Update()
{
	SuperGOE::Update();
}

// ===================================================
// UpdateActive
// 毎フレーム実行
// ただし、ヒットストップ中などの
// 非アクティブ時は実行されない
// ===================================================
void GOE_Character_Player_NEO::UpdateActive()
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

        //m_AxisSpriteIndex = (8 - m_CurrentAxisEx + 1) % 8;
        m_AxisSpriteIndex = (m_CurrentAxisEx - 1) % 8;
        //DebugPrintf("%d\n", m_AxisSpriteIndex);

		if (!m_CharaData->playingMotion.enable)
		{
			//Character::PlayMotionCharacterGameObject(
			//	CharaDat.charaData,
			//	Character::GetCharacterMotionFindByName(Character::GetCharacterMainData(CharaDat.charaData->dataIndex), "IDLE"),
			//	false
			//);
		}


        //攻撃時の残像を生成
        if (m_IsNormalAttack /* && m_Obj->moveRequ.enable */)
        {
            if (m_CharaData->playingMotion.frame == ATTACK_START_FRAME)
            {
                //無敵時間
                if (m_Obj->status.invFrame < ATTACK_TOTAL_FRAME)
                {
                    m_Obj->status.invFrame = ATTACK_TOTAL_FRAME;
                }
            }

            if (m_CharaData->playingMotion.frame >= ATTACK_START_FRAME
                && m_CharaData->playingMotion.frame < ATTACK_TOTAL_FRAME - ATTACK_FTH_FRAME * 0.78
                && m_CharaData->playingMotion.frame % 3 == 0
            )
            {
                GAME_OBJECT* go = GO_CreateAfterEffect(m_Obj, 30, MkF4(0.8f, 0.75f, 1.0f, 0.75f));
            }

            if (m_CharaData->playingMotion.frame == ATTACK_TOTAL_FRAME * 0.5f)
            {
                SoundEffectPlayRequest(
                    GetSoundData("SOUND/SE/player/attack/Normal_Attack"),
                    0.8f, 0.0f, 1.0f);
                CameraShakeRequestAll(
                    CAMERA_SHAKE_MIDIUM_FRAME,
                    CAMERA_SHAKE_LARGE_POWER * 1.5f
                );

                Character::MOTION_COL_DAT motionColDetails =
                    Character::GetMotionColDataFindByName(
                        Character::MOTION_COL_TYPE_ATK,
                        N_ATTACK_TEST_ATK_COL2_NAME
                    );

                GAME_OBJECT_COLLISION_ATTACK_CHILD attackColTmp = motionColDetails.atkCol;
                strcpy_s(attackColTmp.tag, GAME_OBJECT_TXT_MAX, N_ATTACK_GENERAL_TAG);

                //attackColTmp.subjectFamily[GAME_OBJECT_FAMILY_ALL] = true;
                attackColTmp.subjectFamily[GAME_OBJECT_FAMILY_ENEMY] = true;
                attackColTmp.subjectFamily[GAME_OBJECT_FAMILY_GIMIK] = true;

                //attackColTmp.size = Float2ToFloat3(m_Obj->size);
                //attackColTmp.size.x *= 2.0f;
                //attackColTmp.size.y *= 2.0f;
                //attackColTmp.size.z = (float)sqrt(pow(attackColTmp.size.x, 2) + pow(attackColTmp.size.y, 2));

                attackColTmp.size.x = 1000.0f * m_Obj->scale.x;
                attackColTmp.size.y = 1000.0f * m_Obj->scale.y;
                attackColTmp.size.z = 500.0f * m_Obj->scale.z;
                attackColTmp.pos.z = -attackColTmp.size.z / 2;

                Float3 bvecn = NormalizeVector(m_BeforeVecBuf);
                attackColTmp.pos.x += 540.0f / 4 * bvecn.x + 0.0f * bvecn.x;
                attackColTmp.pos.y += 540.0f / 4 * bvecn.y + 0.0f * bvecn.y;


                //攻撃判定を生成
                GameObjectAttackCollisionAdd(m_Obj, attackColTmp,
                    8,
                    (ATTACK_TOTAL_FRAME - m_CharaData->playingMotion.frame)
                );
            }
        }
        else
        {
            //ぶりんくの残像を生成
            if (m_AttackCooldown > 0 && m_Obj->moveRequ.enable)
            {
                if (m_Obj->status.invFrame < 2)
                {
                    m_Obj->status.invFrame = 2;
                }

                if (m_Obj->moveRequ.frame % 3 == 0)
                {
                    //GAME_OBJECT* go = GO_CreateAfterEffect(m_Obj, 30, MkF4(0.8f, 0.75f, 1.0f, 0.75f));
                    GAME_OBJECT* go = GO_CreateAfterEffect(m_Obj, 30, MkF4(2.0f, 1.5f, 1.6f, 0.7f));
                }
            }
        }

        //歩きの煙
        if (m_WalkFrame > 0 &&  m_WalkFrame % 8 == 0)
        {
            {
                Float3 efPos2 = m_Obj->pos;
                efPos2.z += BLOCK_HIGHT * -0.5f;

                GAME_OBJECT* go = GO_CreateEffect(GO_EFFECT_TYPE_SMOKE, efPos2);
                if (go != nullptr)
                {
                    //go->graphic.rot3d.x = 90.0f;
                    go->graphic.color.w = 1.0f;
                    go->scale = MkF3(
                        0.8f,
                        0.8f,
                        0.8f
                    );
                    GameObjectSetLifeTime(go, 12);
                }
            }
        }
        if (m_IsDrawing)
        {
            if (m_WalkFrame > 0 && m_WalkFrame % 6 == 0)
            {
                GAME_OBJECT* go = GO_CreateAfterEffect(m_Obj, 30, MkF4(1.0f, 1.0f, 1.0f, 0.8f));
            }
        }
        //if (m_WalkFrame > 0 && m_WalkFrame % 8 == 0)
        //{
        //    SoundEffectPlayRequest(
        //        GetSoundData("SOUND/SE/player/move/Neo_Walk"),
        //        0.8f, 0.0f);
        //}

		//待機状態なら操作を受け付ける
        {
            bool isNu = !m_CharaData->playingMotion.enable || m_IsIdling || m_IsWalkAnim;
            if (isNu)
			{ //ループアニメーション更新
                constexpr float scaledefault = 1.35f;
				std::string animName = "IDLE";

                int* animp = nullptr;
                int maxf = -1;

                if (m_IsDrawing)
                {
                    animName = "DRAW";
                    animp = &m_WalkFrame;
                    maxf = WALK_FRAME * 0.8f;
                    if ((m_CharaData->playingMotion.motion->name) == animName)
                    {
                        m_Obj->graphic.graph.scale.x = scaledefault;
                        m_Obj->graphic.graph.scale.y = scaledefault;
                        m_Obj->graphic.pos3d = m_GraphPosDefault;
                        GraphicSpriteSet(m_Tex_Main_Draw[std::max(m_CurrentAxisEx - 1, 0)], 4, 3);
                    }
                }
                else if (m_IsWalking)
                {
                    animName = "WALK";
                    animp = &m_WalkFrame;
                    maxf = WALK_FRAME * 1.5f;
                    if ((m_CharaData->playingMotion.motion->name) == animName)
                    {
                        m_Obj->graphic.graph.scale.x = scaledefault;
                        m_Obj->graphic.graph.scale.y = scaledefault;
                        m_Obj->graphic.pos3d = m_GraphPosDefault;
                        GraphicSpriteSet(m_Tex_Main_Walk[std::max(m_CurrentAxisEx - 1, 0)], 4, 3, 14);
                    }
                }
                else
                {
                    animp = &m_AxisSpriteIndex;
                    if (m_CharaData->playingMotion.enable 
                        && (m_CharaData->playingMotion.motion->name) == animName)
                    {
                        m_Obj->graphic.graph.scale.x = scaledefault;
                        m_Obj->graphic.graph.scale.y = scaledefault;
                        m_Obj->graphic.pos3d = m_GraphPosDefault;
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

			{ //描いている時ではない

				//通常攻撃
				{
					if (m_AttackCooldown <= 0) //クールタイム中ではない
					{

						//入力をとる
						if (isNu && IsInput_NormalAttack())
						{
                            if (!m_IsDrawing)
                            {
                                SetFlagTtrlAttack();

                                SoundEffectPlayRequest(
                                    GetSoundData("SOUND/SE/player/attack/Normal_Attack"),
                                    0.8f, 0.0f, 0.75f);
                                CameraShakeRequestAll(
                                    CAMERA_SHAKE_MIDIUM_FRAME,
                                    CAMERA_SHAKE_SMALL_POWER * 1.5f
                                );

                                Character::MOTION_COL_DAT motionColDetails =
                                    Character::GetMotionColDataFindByName(
                                        Character::MOTION_COL_TYPE_ATK,
                                        N_ATTACK_TEST_ATK_COL_NAME
                                    );

                                GAME_OBJECT_COLLISION_ATTACK_CHILD attackColTmp = motionColDetails.atkCol;
                                strcpy_s(attackColTmp.tag, GAME_OBJECT_TXT_MAX, N_ATTACK_GENERAL_TAG);

                                //attackColTmp.subjectFamily[GAME_OBJECT_FAMILY_ALL] = true;
                                attackColTmp.subjectFamily[GAME_OBJECT_FAMILY_ENEMY] = true;
                                attackColTmp.subjectFamily[GAME_OBJECT_FAMILY_GIMIK] = true;

                                //attackColTmp.size = Float2ToFloat3(m_Obj->size);
                                //attackColTmp.size.x *= 1.0f;
                                //attackColTmp.size.y *= 1.0f;
                                //attackColTmp.size.z = (float)sqrt(pow(attackColTmp.size.x, 2) + pow(attackColTmp.size.y, 2));

                                attackColTmp.size.x = 600.0f * m_Obj->scale.x;
                                attackColTmp.size.y = 600.0f * m_Obj->scale.y;
                                attackColTmp.size.z = 500.0f * m_Obj->scale.z;
                                attackColTmp.pos.z = -attackColTmp.size.z / 2;

                                Float3 bvecn = NormalizeVector(m_BeforeVecBuf);
                                attackColTmp.pos.x += 540.0f / 4 * bvecn.x + 0.0f * bvecn.x;
                                attackColTmp.pos.y += 540.0f / 4 * bvecn.y + 0.0f * bvecn.y;

                                //攻撃判定を生成
                                GameObjectAttackCollisionAdd(m_Obj, attackColTmp,
                                    ATTACK_START_FRAME,
                                    //ATTACK_FRAME
                                    ATTACK_TOTAL_FRAME / 2
                                );

                                //モーション再生
                                GraphicSpriteSet(
                                    m_Tex_Main_Attack[
                                        (int)round((CalculateRadToDeg(atan2(bvecn.y, bvecn.x)) - 90.0f + 360.0f) / (360.0f / 8)) % 8
                                    ],
                                    3, 2);
                                //GraphicSpriteSet(m_Tex_Main_Attack[std::max(m_CurrentAxisEx - 1, 0)], 3, 2);
                                //m_Obj->graphic.graph.scale.x = 1.5f;
                                //m_Obj->graphic.graph.scale.y = 1.5f;
                                //m_Obj->graphic.pos3d = m_GraphPosDefault;
                                //m_Obj->graphic.pos3d.y += -60.0f;
                                //m_Obj->graphic.pos3d.z += -20.0f;

                                Character::PlayMotionCharacterGameObject(
                                    m_CharaData,
                                    Character::GetCharacterMotionFindByName(
                                        Character::GetCharacterMainData(m_CharaData->dataIndex),
                                        "ATTACK"),
                                    false,
                                    nullptr,
                                    ATTACK_TOTAL_FRAME
                                );
                                {
                                    //Float3 dashVec = NormalizeVector(attackColTmp.pos);
                                    Float2 dashVec = NormalizeVector(MkF2(attackColTmp.pos.x, attackColTmp.pos.y));
                                    GameObjectMoveRequest(m_Obj, ATTACK_TOTAL_FRAME,
                                        MkF3(
                                            m_Obj->pos.x + dashVec.x * ONE_BLOCK * 18.0f,
                                            m_Obj->pos.y + dashVec.y * ONE_BLOCK * 18.0f,
                                            m_Obj->pos.z
                                        ),
                                        EasingOutSine
                                    );

                                    //クールタイム設定
                                    m_AttackCooldown = ATTACK_COOLDOWN_FRAME;
                                }
                            }
                            else
                            {
                                SoundEffectPlayRequest(
                                    GetSoundData("SOUND/SE/player/attack/Normal_Attack"),
                                    0.8f, 0.0f, 0.75f);

                                CameraShakeRequestAll(
                                    CAMERA_SHAKE_MIDIUM_FRAME,
                                    CAMERA_SHAKE_SMALL_POWER * 1.5f
                                );

                                Float3 bvecn = NormalizeVector(m_BeforeVecBuf);
                                {
                                    GameObjectMoveRequest(m_Obj, ATTACK_TOTAL_FRAME * 0.75,
                                        MkF3(
                                            m_Obj->pos.x + bvecn.x * ONE_BLOCK * 20.0f,
                                            m_Obj->pos.y + bvecn.y * ONE_BLOCK * 20.0f,
                                            m_Obj->pos.z
                                        ),
                                        EasingOutSine
                                    );

                                    //クールタイム設定
                                    m_AttackCooldown = ATTACK_COOLDOWN_FRAME;
                                }
                            }
						}
					}
				}
			}

			//かこポン
			{
                //反応するキー
                bool keyPushMain = isNu && IsInput_KakoMain();
                bool keyPushSubTest = isNu &&
                    m_SkillCooldownFrame <= 0 && IsInput_KakoSub() ;

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
                    update_kakopon_action(keyPushSubTest, &m_KakoDatList[1].dat, &m_KakoDatList[1].lineDraw);

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
                if (m_KakoDatList[0].dat.objs.size() > 0
                    && !m_KakoDatList[0].dat.isFirstObjectCheck)
                {
                    m_KakoDatList[0].dat.isFirstObjectCheck = true;

                    //攻撃
                    kakopon_attack_setBuf(&m_KakoDatList[0].dat);
                    kakopon_objects_attack(&m_KakoDatList[0].dat);

                    //CameraZoomRequest(
                    //    player->index,
                    //    0.03f,
                    //    m_KakoDatList[0].dat.connectCentroid,
                    //    EasingInOutSine,
                    //    0.5f
                    //);

                    //SEを入れる
                    //SoundEffectPlayRequest(SOUND_TYPE_SE_3, 4.0f, 0.0f);
                    if (m_KakoDatList[0].dat.IsSuccess)
                    {
                        m_KakoponBuffPoint += (int)(
                            m_KakoDatList[0].dat.atkBuf.level
                            * KAKOPON_BUFFPOINT_ADDMOD
                        );
                        if (m_KakoponBuffPoint > KAKOPON_BUFFPOINT_MAX)
                        {
                            m_KakoponBuffPoint = KAKOPON_BUFFPOINT_MAX;
                        }
                        //m_KakoponBuffPointCostCoolDown
                        //    += 12 * pow(m_KakoDatList[0].dat.atkBuf.level,
                        //        2.0f);
                        m_KakoponBuffPointCostCoolDown
                            += FRAME_ONE_SECOND * m_KakoDatList[0].dat.atkBuf.level;
                        if (m_KakoponBuffPointCostCoolDown > KAKOPON_BUFFPOINT_COST_COOLDOWM_MAX)
                        {
                            m_KakoponBuffPointCostCoolDown = KAKOPON_BUFFPOINT_COST_COOLDOWM_MAX;
                        }
                        
                        //m_KakoDatList[0].dat.atkBuf.power
                        SoundEffectPlayRequest(
                            GetSoundData("SOUND/SE/player/attack/activation"),
                            3.0f, 0.0f);
                    }

                    //constexpr int stopframe = 60;
                    ////GameStopRequest(stopframe);

                    //CameraZoomResetRequest(
                    //    player->index,
                    //    0.02f,
                    //    EasingInOutSine,
                    //    stopframe
                    //);

                    //g_LineDraw.afterTimerFrame += stopframe;
                }

                //囲った時の判定テスト
                if (m_KakoDatList[1].dat.isConnected)
                 //if (m_KakoDatList[1].dat.objs.size() > 0)
                {
                    if (!m_KakoDatList[1].dat.isFirstObjectCheck)
                    {
                        SetFlagTtrlSkill();

                        m_KakoDatList[1].dat.isFirstObjectCheck = true;
                        SoundEffectPlayRequest(
                            GetSoundData("SOUND/SE/player/attack/ready1"),
                            2.8f, 0.0f, 1.0f);
                    }
                    //SetSkillCooldownFrame(m_KakoDatList[1].dat.frameMax
                    //    + FRAME_ONE_SECOND * 10
                    //    );
                    SetSkillCooldownFrame(FRAME_ONE_SECOND * 10);

                    m_KakoDatList[1].dat.IsSuccess = true;

                    //オブジェクトを検証
                    for (int i = 0; i < m_KakoDatList[1].dat.objs.size(); i++)
                    {
                        GAME_OBJECT* ob = GetGameObjectFindByIdentifier(m_KakoDatList[1].dat.objs[i]);

                        if (
                            ob != nullptr
                            && IsBelongsFamilyGameObject(ob, GAME_OBJECT_FAMILY_PLAYER)
                            )
                        {
                            {
                                GOE_Character_Player* goe =
                                    dynamic_cast<GOE_Character_Player*>(ob->events.source);  // ダウンキャスト

                                if (goe != nullptr)
                                { //
                                    
                                    if (
                                        !goe->IsHealIgnore()
                                        && !goe->IsDefeat()
                                        && ob->frame % (FRAME_ONE_SECOND / 3) == 0
                                     )
                                    {
                                        //ob->status.hp += (int)(ob->status.hpMax * 0.1);
                                        ob->status.hp += (int)(ob->status.hpMax * 0.065);
                                        if (ob->status.hp > ob->status.hpMax)
                                        {
                                            ob->status.hp = ob->status.hpMax;
                                        }
                                        goe->HealingFlag();

                                        SoundEffectPlayRequest(
                                            GetSoundData("SOUND/SE/player/heal_sml"),
                                            0.2f, 0.0f, 1.0f);

                                    }
                                }
                            }
                            //ob->status.hp += 1;
                        }
                    }
                }
			}
		}

	}

    //SE
    if (m_IsDrawing &&
        m_InkGage > 0 &&
        //m_WalkFrame % 3 == 0
        m_Obj->frameActive % 16 == 0
        )
    {
        //SoundEffectPlayRequest(
        //    GetSoundData("SOUND/SE/player/attack/ready_single"),
        //    3.0f, 0.0f);
        SoundEffectPlayRequest(
            GetSoundData("SOUND/SE/player/attack/ready2"),
            3.0f, 0.0f);
    }

    kakopon_final_update();

    //{
    //    int lv = (int)(
    //            m_KakoponBuffPoint 
    //            / (KAKOPON_BUFFPOINT_MAX / KAKOPON_BUFFPOINT_MAXLV)
    //        );

    //    float mod = 1.0f 
    //        + KAKOPON_BUFFPOINT_LVADDMOD * std::min(lv, KAKOPON_BUFFPOINT_MAXLV - 1)
    //        ;

    //    DebugPrintf("p: %d  mod: %f \n", m_KakoponBuffPoint, mod);
    //}

    if (!GameManagerIsMovieFlag())
    {
        if (m_KakoponBuffPointCostCoolDown > 0)
        {
            m_KakoponBuffPointCostCoolDown--;
        }
        else if (m_KakoponBuffPoint > 0)
        {
            m_KakoponBuffPoint += -KAKOPON_BUFFPOINT_COST;
            if (m_KakoponBuffPoint < 0)
            {
                m_KakoponBuffPoint = 0;
            }
        }
    }

    UpdateActiveLate();
}

void GOE_Character_Player_NEO::UpdateActiveLate()
{
    SuperGOE::UpdateActiveLate();
}

void GOE_Character_Player_NEO::calc_kakopon_damage(
    GAME_OBJECT_COLLISION_ATTACK_CHILD* output
)
{
    if (m_KakoponBuffPoint > 0)
    {
        int lv = (int)(
                m_KakoponBuffPoint 
                / (KAKOPON_BUFFPOINT_MAX / KAKOPON_BUFFPOINT_MAXLV)
            );

        float mod = 1.0f 
            + KAKOPON_BUFFPOINT_LVADDMOD * std::min(lv, KAKOPON_BUFFPOINT_MAXLV - 1)
            ;

        output->power *= mod;
    }
}

// ===================================================
// Draw
// オブジェクトの描画イベント
// ===================================================
void GOE_Character_Player_NEO::Draw()
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
        {
            {
                float rott = m_AxisSpriteIndex * 360.0f / 8 - 90.0f;

                //調整用
                //if (m_CurrentAxis == AXIS_2D_LEFT) rott += -50.0f;
                //if (m_CurrentAxis == AXIS_2D_RIGHT) rott += 60.0f;
                //if (m_CurrentAxis == AXIS_2D_UP) rott += -100.0f;

                DRAW_SPRITE_DAT dsdat = {};
                dsdat.x = 0.0f;
                dsdat.y = 540.0f * 0.02f;

                dsdat.w = 540.0f * 1.8f * 1.f;
                dsdat.h = 540.0f * 1.8f * -1.f;

                dsdat.rot = -90.0f;
                dsdat.color = { 1.0f, 0.5f, 0.8f, 1.0f * m_MovieVisibleAlpha };
                //dsdat.color = { 0.4f, 0.1f, 0.3f, 1.0f };

                dsdat.texNo = m_TexId_AttackEffect_s;
                dsdat.loadTexType = LOADTEXTURETYPE_MAIN;

                //dsdat.texUV = { 0.0f, 0.0f };
                //dsdat.texWH = { 1.0f, 1.0f };
                dsdat.texUV = AnimationUV(
                    (int)(
                        m_AnimCount_AttackEffect_s.x * m_AnimCount_AttackEffect_s.y
                        * std::max((float)(m_CharaData->playingMotion.frame - ATTACK_TOTAL_FRAME * 0.6f) / (ATTACK_TOTAL_FRAME * 0.4f), 0.0f)
                    ),
                    (int)m_AnimCount_AttackEffect_s.x,
                    (int)m_AnimCount_AttackEffect_s.y
                );
                dsdat.texWH = { 1.0f / m_AnimCount_AttackEffect_s.x, 1.0f / m_AnimCount_AttackEffect_s.y };

                dsdat.startPos3D = m_Obj->pos;
                dsdat.startPos3D.z += -abs(dsdat.h) * 0.12f;

                dsdat.startRot = rott;
                dsdat.startScale = { 1.0f, 1.0f };

                //dsdat.shaderOpt.bloom = true;

                dsdat.shaderOpt.blendState = DPD_SHADER_OPT_BLEND_STATE_ADDCOLOR;

                DrawSpriteInCamera(dsdat);
            }
        }
    }


}

// ===================================================
// Dead
// オブジェクトのHPが0になったとき
// ===================================================
void GOE_Character_Player_NEO::Dead()
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

    SetDownPoseStart();
    SetDownPose();
}

// ===================================================
// DamageAttacker
// オブジェクトの攻撃判定がヒットしたとき
// ===================================================
void GOE_Character_Player_NEO::DamageAttacker(GAME_OBJECT_DAMAGE_RESULT dr)
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
                GetSoundData("SOUND/SE/player/attack/atkhit_sml"),
                1.0f);
        }
	}
}

// ===================================================
// DamageReciever
// オブジェクトが
// 別のオブジェクトの攻撃判定にヒットしたとき
// ===================================================
void GOE_Character_Player_NEO::DamageReciever(GAME_OBJECT_DAMAGE_RESULT dr)
{
	SuperGOE::DamageReciever(dr);

	//if (dr.damage > 0 || dr.colAtk.isGrab)
	//{

	//}
    if (dr.damage > 0)
    {
        if (m_KakoponBuffPoint > 0)
        {
            m_KakoponBuffPoint += -KAKOPON_BUFFPOINT_DAMAGE_COST;
            if (m_KakoponBuffPoint < 0)
            {
                m_KakoponBuffPoint = 0;
            }
        }
    }
}

// ===================================================
// BlockHit
// オブジェクトがブロックに触れたとき
// ===================================================
void GOE_Character_Player_NEO::BlockHit(GAME_OBJECT_POSMOD_RESULT pmr)
{
	SuperGOE::BlockHit(pmr);
}

void GOE_Character_Player_NEO::SetDownPoseStart()
{
    m_DefeatFrameAnimMax = 6;
}

void GOE_Character_Player_NEO::SetDownPose()
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
    m_Obj->graphic.pos3d.y += 80.0f;
    m_Obj->graphic.pos3d.z += -140.0f;
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

void GOE_Character_Player_NEO::InitResetAction()
{
    //SuperGOE::InitResetAction();

    ResetAction();

    m_CharaData->playingMotion.enable = false;

    m_DamageFrame = -1;
    m_DefeatFrame = 0;
    m_DefeatFrameAnim = 0;
    m_DefeatFrameAnimMax = 0;
    m_DamageSternFrame = -1;
    m_DamageSternFrameMax = 0;
    m_DeadNum = 0;
    m_GameoverDecision = false;

    m_KakoponBuffPoint = 0;
    m_KakoponBuffPointCostCoolDown = 0;
}
