// ===================================================
// GOE_Character_Player_CA.cpp キャラクターオブジェクト
// 
// hara sougo  2025/01/19
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

#include "GOE_Character_Player_CA.h"

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
void GOE_Character_Player_CA::Init()
{
	SuperGOE::Init(); //初期化

    GraphicSet("TEX/player/character_test", 6, 8);

}

// ===================================================
// FirstFrame
// 最初のフレームのみ実行
// ===================================================
void GOE_Character_Player_CA::FirstFrame()
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
void GOE_Character_Player_CA::Uninit()
{


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
void GOE_Character_Player_CA::Update()
{
	SuperGOE::Update();


}

// ===================================================
// UpdateActive
// 毎フレーム実行
// ただし、ヒットストップ中などの
// 非アクティブ時は実行されない
// ===================================================
void GOE_Character_Player_CA::UpdateActive()
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

		//待機
		if (!m_CharaData->playingMotion.enable)
		{
		}

        //方向名
		std::string axisAnimName[AXIS_2D_MAX] = {};
		{
			axisAnimName[AXIS_2D_N] = "";
			axisAnimName[AXIS_2D_LEFT] = "LEFT";
			axisAnimName[AXIS_2D_RIGHT] = "RIGHT";
			axisAnimName[AXIS_2D_UP] = "UP";
			axisAnimName[AXIS_2D_DOWN] = "DOWN";
		}

		{ //方向処理
			//m_CurrentAxis = AXIS_2D_N;
			m_CurrentAxis = AXIS_2D_DOWN;
            m_AxisVec = MkF2(0.0f, 1.0f);

			if (m_BeforeVec.x < 0 && abs(m_BeforeVec.y) < 1.0f)
			{
				m_CurrentAxis = AXIS_2D_LEFT;
                m_AxisVec = MkF2( -1.0f, 0.0f );
			}
			if (m_BeforeVec.x > 0 && abs(m_BeforeVec.y) < 1.0f)
			{
				m_CurrentAxis = AXIS_2D_RIGHT;
                m_AxisVec = MkF2(1.0f, 0.0f);
			}
			if (m_BeforeVec.y < 0 && abs(m_BeforeVec.x) < 1.0f)
			{
				m_CurrentAxis = AXIS_2D_UP;
                m_AxisVec = MkF2(0.0f, -1.0f);
			}
			//if (m_BeforeVec.y > 0 && abs(m_BeforeVec.x) < 1.0f)
			//{
			//	m_CurrentAxis = AXIS_2D_DOWN;
			//}

		}

		if (!m_CharaData->playingMotion.enable)
		{
			//Character::PlayMotionCharacterGameObject(
			//	CharaDat.charaData,
			//	Character::GetCharacterMotionFindByName(Character::GetCharacterMainData(CharaDat.charaData->dataIndex), "IDLE"),
			//	false
			//);
		}

        //攻撃時の残像を生成
        if (m_IsNormalAttack && m_Obj->moveRequ.enable)
        {
            if (m_CharaData->playingMotion.frame >= ATTACK_START_FRAME
                && m_CharaData->playingMotion.frame < ATTACK_TOTAL_FRAME - ATTACK_FTH_FRAME * 0.78
                && m_CharaData->playingMotion.frame % 2 == 0
            )
            {
                GAME_OBJECT* go = GO_CreateAfterEffect(m_Obj, 30, MkF4(0.8f, 0.75f, 1.0f, 0.75f));
            }
        }

		//待機状態なら操作を受け付ける
		if (!m_CharaData->playingMotion.enable || m_IsIdling || m_IsWalkAnim)
		{
			{ //歩きの更新
				std::string animName = "IDLE";

				if (axisAnimName[m_CurrentAxis].size() > 0)
				{
					animName = "WALK_" + axisAnimName[m_CurrentAxis];
				}

				if (m_IsDrawing)
				{
					animName += "_DRAW";
				}

				if ((m_CharaData->playingMotion.motion->name) != animName)
				{
					Character::PlayMotionCharacterGameObject(
						m_CharaData,
						Character::GetCharacterMotionFindByName(Character::GetCharacterMainData(m_CharaData->dataIndex), animName),
						true,
						&m_WalkFrame,
						WALK_FRAME
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
						if (IsInput_NormalAttack())
						{
							Character::MOTION_COL_DAT motionColDetails =
								Character::GetMotionColDataFindByName(
									Character::MOTION_COL_TYPE_ATK,
									N_ATTACK_TEST_ATK_COL_NAME
								);

							GAME_OBJECT_COLLISION_ATTACK_CHILD attackColTmp = motionColDetails.atkCol;
                            strcpy_s(attackColTmp.tag, GAME_OBJECT_TXT_MAX, N_ATTACK_GENERAL_TAG);

                            //attackColTmp.subjectFamily[GAME_OBJECT_FAMILY_ALL] = true;
                            attackColTmp.subjectFamily[GAME_OBJECT_FAMILY_ENEMY] = true;

							attackColTmp.size = Float2ToFloat3(m_Obj->size);
							attackColTmp.size.z = (float)sqrt(pow(attackColTmp.size.x, 2) + pow(attackColTmp.size.y, 2));
							attackColTmp.pos.z = -attackColTmp.size.z / 2;

                            attackColTmp.pos.x += 540.0f / 2 * m_AxisVec.x;
                            attackColTmp.pos.y += 540.0f / 2 * m_AxisVec.y;
                            
							//攻撃判定を生成
							GameObjectAttackCollisionAdd(m_Obj, attackColTmp,
								ATTACK_START_FRAME,
								ATTACK_FRAME
							);

							//モーション再生
							Character::PlayMotionCharacterGameObject(
								m_CharaData,
								Character::GetCharacterMotionFindByName(Character::GetCharacterMainData(m_CharaData->dataIndex), "ATTACK_" + axisAnimName[m_CurrentAxis]),
								false,
								nullptr,
								ATTACK_TOTAL_FRAME
							);

                            if (m_IsWalking)
                            {
                                //無敵時間
                                m_Obj->status.invFrame = ATTACK_TOTAL_FRAME;

                                Float3 dashVec = NormalizeVector(attackColTmp.pos);
                                GameObjectMoveRequest(m_Obj, ATTACK_TOTAL_FRAME,
                                    MkF3(
                                        m_Obj->pos.x + dashVec.x * ONE_BLOCK * 16.0f,
                                        m_Obj->pos.y + dashVec.y * ONE_BLOCK * 16.0f,
                                        m_Obj->pos.z
                                    ),
                                    EasingInOutBack
                                );

                                //クールタイム設定
                                m_AttackCooldown = ATTACK_COOLDOWN_FRAME;
                            }
                            else
                            {
                                //クールタイム設定
                                m_AttackCooldown = (int)(ATTACK_COOLDOWN_FRAME * 0.4f);
                            }
						}
					}
				}
			}

			//かこポン
			{
                //反応するキー
                bool keyPushMain = IsInput_KakoMain();
                bool keyPushSubTest = IsInput_KakoSub() ;

                //適当
                //m_KakoDat_Main.graphic = KAKOPON_GRAPHIC::Main;
                //m_KakoDat_Main.type = KAKOPON_TYPE::Main;
                //m_KakoDat_Main.frameMax = GOE_Character_Player::KAKOPON_COOLDOWN_FRAME;

                //m_KakoDat_SubTest.graphic = KAKOPON_GRAPHIC::Main;
                //m_KakoDat_SubTest.type = KAKOPON_TYPE::Heal;
                //m_KakoDat_SubTest.frameMax = FRAME_ONE_SECOND * 5;
                m_KakoDatList[0].dat.graphic = KAKOPON_GRAPHIC::Main;
                m_KakoDatList[0].dat.type = KAKOPON_TYPE::Main;
                m_KakoDatList[0].dat.frameMax = KAKOPON_COOLDOWN_FRAME;

                m_KakoDatList[1].dat.graphic = KAKOPON_GRAPHIC::Main;
                m_KakoDatList[1].dat.type = KAKOPON_TYPE::Heal;
                m_KakoDatList[1].dat.frameMax = FRAME_ONE_SECOND * 5;

                //更新
                for (int i = 0; i < KAKOPON_DAT_NUM; i++)
                {
                    update_kakoponDat(&m_KakoDatList[i].dat);
                }
                //update_kakoponDat(&m_KakoDat_Main);
                //update_kakoponDat(&m_KakoDat_SubTest);


                { //プレイヤーの操作による処理

                    //for (int i = 0; i < KAKOPON_DAT_NUM; i++)
                    //{
                    //    update_kakopon_action(keyPushMain, &m_KakoDatList[i].dat, &m_KakoDatList[i].lineDraw);
                    //}
                    //update_kakopon_action(keyPushMain, &m_KakoDat_Main, &m_KakoponLineDraw_Main);
                    //update_kakopon_action(keyPushSubTest, &m_KakoDat_SubTest, &m_KakoponLineDraw_SubTest);
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
                            init_kakoponDat(&m_KakoDatList[i].dat);
                            //キャンセル時にゲージを戻す
                            m_InkGage = m_InkGage_DrawBefore;
                        }
                        else
                        {
                            cancel_All = false;
                        }
                    }
                    //bool cancel_Main = !m_KakoponLineDraw_Main.vertexDrawFlag && !m_KakoDat_Main.isConnected;
                    //if (cancel_Main)
                    //{
                    //    init_kakoponDat(&m_KakoDat_Main);
                    //}

                    //bool cancel_SubTest = !m_KakoponLineDraw_SubTest.vertexDrawFlag && !m_KakoDat_SubTest.isConnected;
                    //if (cancel_SubTest)
                    //{
                    //    init_kakoponDat(&m_KakoDat_SubTest);
                    //}

                    ////キャンセル時にゲージを戻す
                    //if (cancel_All)
                    //{
                    //    m_InkGage = m_InkGage_DrawBefore;
                    //}
                }

                //囲った時の判定
                if (m_KakoDatList[0].dat.objs.size() > 0 && !m_KakoDatList[0].dat.isFirstObjectCheck)
                {
                    m_KakoDatList[0].dat.isFirstObjectCheck = true;

                    //攻撃
                    kakopon_objects_attack(m_KakoDatList[0].dat);

                    //CameraZoomRequest(
                    //    player->index,
                    //    0.03f,
                    //    m_KakoDatList[0].dat.connectCentroid,
                    //    EasingInOutSine,
                    //    0.5f
                    //);

                    //SEを入れる
                    SoundEffectPlayRequest(SOUND_TYPE_SE_3, 4.0f, 0.0f);

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
                if (m_KakoDatList[1].dat.objs.size() > 0 && !m_KakoDatList[1].dat.isFirstObjectCheck)
                {
                    //m_KakoDat_SubTest.isFirstObjectCheck = true;

                    //オブジェクトを検証
                    for (int i = 0; i < m_KakoDatList[1].dat.objs.size(); i++)
                    {
                        GAME_OBJECT* ob = GetGameObjectFindByIdentifier(m_KakoDatList[1].dat.objs[i]);

                        if (
                            ob != nullptr
                            && IsBelongsFamilyGameObject(ob, GAME_OBJECT_FAMILY_PLAYER)
                            )
                        {
                            if (ob->frame % FRAME_ONE_SECOND == 0)
                            {
                                ob->status.hp += (int)(ob->status.hpMax * 0.1);
                                if (ob->status.hp > ob->status.hpMax)
                                {
                                    ob->status.hp = ob->status.hpMax;
                                }
                            }
                            //ob->status.hp += 1;
                        }
                    }
                }

                
			}
		}

	}

    kakopon_final_update();
}

// ===================================================
// Draw
// オブジェクトの描画イベント
// ===================================================
void GOE_Character_Player_CA::Draw()
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

    if (m_HealingFrame > 0)
    {
        DrawCharaEff(CharaEffType::Heal);
    }
    if (m_BuffAttackFrame > 0)
    {
        DrawCharaEff(CharaEffType::Buff);
    }

    //通常攻撃エフェクト
    if (m_IsNormalAttack)
    { 
        {
            {
                float rott = CalculateRadToDeg(atan2(m_AxisVec.y, m_AxisVec.x)) - 90.0f;

                //調整用
                //if (m_CurrentAxis == AXIS_2D_LEFT) rott += -50.0f;
                //if (m_CurrentAxis == AXIS_2D_RIGHT) rott += 60.0f;
                //if (m_CurrentAxis == AXIS_2D_UP) rott += -100.0f;

                DRAW_SPRITE_DAT dsdat = {};
                dsdat.x = 0.0f;
                dsdat.y = 540.0f * 0.02f;

                dsdat.w = 540.0f * 2.4f;
                dsdat.h = 540.0f * 2.4f * 1.f;

                dsdat.rot = 0.0f;
                dsdat.color = { 1.0f, 1.0f, 0.0f, 1.0f };

                dsdat.texNo = m_TexId_AttackEffect_s;
                dsdat.loadTexType = LOADTEXTURETYPE_MAIN;

                //dsdat.texUV = { 0.0f, 0.0f };
                //dsdat.texWH = { 1.0f, 1.0f };
                dsdat.texUV = AnimationUV(
                    (int)(
                        m_AnimCount_AttackEffect_s.x * m_AnimCount_AttackEffect_s.y
                        * ((float)m_CharaData->playingMotion.frame / ATTACK_TOTAL_FRAME)
                    ),
                    (int)m_AnimCount_AttackEffect_s.x,
                    (int)m_AnimCount_AttackEffect_s.y
                );
                dsdat.texWH = { 1.0f / m_AnimCount_AttackEffect_s.x, 1.0f / m_AnimCount_AttackEffect_s.y };

                dsdat.startPos3D = m_Obj->pos;
                dsdat.startPos3D.z += -dsdat.h * 0.12f;

                dsdat.startRot = rott;
                dsdat.startScale = { 1.0f, 1.0f };

                dsdat.shaderOpt.bloom = DPD_SHADER_OPT_BLOOM_NORMAL;

                DrawSpriteInCamera(dsdat);
            }
        }
    }


}

// ===================================================
// Dead
// オブジェクトのHPが0になったとき
// ===================================================
void GOE_Character_Player_CA::Dead()
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

}

// ===================================================
// DamageAttacker
// オブジェクトの攻撃判定がヒットしたとき
// ===================================================
void GOE_Character_Player_CA::DamageAttacker(GAME_OBJECT_DAMAGE_RESULT dr)
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

	}
}

// ===================================================
// DamageReciever
// オブジェクトが
// 別のオブジェクトの攻撃判定にヒットしたとき
// ===================================================
void GOE_Character_Player_CA::DamageReciever(GAME_OBJECT_DAMAGE_RESULT dr)
{
	SuperGOE::DamageReciever(dr);

	if (dr.damage > 0 || dr.colAtk.isGrab)
	{

	}
}

// ===================================================
// BlockHit
// オブジェクトがブロックに触れたとき
// ===================================================
void GOE_Character_Player_CA::BlockHit(GAME_OBJECT_POSMOD_RESULT pmr)
{
	SuperGOE::BlockHit(pmr);
}
