// ===================================================
// GOE_Character_Enemy_Miresu_Hyper.cpp キャラクターオブジェクト
// 
// hara sougo  2024/12/12
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

#include "../../../Entities/character.h"
#include "../../../Entities/player.h"

#include "../../../Managers/soundManager.h"

#include "../GOE_Effect.h"
#include "../GOE_Character.h"


#include "GOE_Character_Enemy_Miresu_Hyper.h"

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
void GOE_Character_Enemy_Miresu_Hyper::Init()
{
	SuperGOE::Init();

    GraphicSet("TEX/enemy/miresu_sprite_hyp", 3, 3);


	//Character::PlayMotionCharacterGameObject(
	//	goDat,
	//	Character::GetCharacterMotionFindByName(Character::GetCharacterMainData(goDat->dataIndex), "IDLE"),
	//	true,
	//	nullptr,
	//	60
	//);
	
}

// ===================================================
// FirstFrame
// 最初のフレームのみ実行
// ===================================================
void GOE_Character_Enemy_Miresu_Hyper::FirstFrame()
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
void GOE_Character_Enemy_Miresu_Hyper::Uninit()
{

	SuperGOE::Uninit();
}

// ===================================================
// Update
// 毎フレーム実行
// ===================================================
void GOE_Character_Enemy_Miresu_Hyper::Update()
{
	SuperGOE::Update();

}

// ===================================================
// UpdateActive
// 毎フレーム実行
// ただし、ヒットストップ中などの
// 非アクティブ時は実行されない
// ===================================================
void GOE_Character_Enemy_Miresu_Hyper::UpdateActive()
{
	SuperGOE::UpdateActive();


	//行動パターン
	{
		if (m_TargetObjIdentifier.objCreateFrame > 0)
		{ //対象がいるとき

			GAME_OBJECT* targetObject = GetGameObjectFindByIdentifier(m_TargetObjIdentifier);
			if (targetObject != nullptr)
			{
				float distance = CalculateDistance(targetObject->pos, m_Obj->pos);
				Float3 vec = NormalizeVector(MkF3(
					targetObject->pos.x - m_Obj->pos.x,
					targetObject->pos.y - m_Obj->pos.y,
					0.0f
				));

				constexpr unsigned int dashOneCycle = (60 * 1.5f);
				unsigned int dashF = m_TargetFrame % dashOneCycle;

				//突進
				if (m_TargetFrame > dashOneCycle)
				{
					if (dashF == 1 && distance < ONE_BLOCK * 15.0f)
					{ //ため
						GameObjectMoveRequest(m_Obj, 30,
							MkF3(
								m_Obj->pos.x + vec.x * -ONE_BLOCK * 1.2f,
								m_Obj->pos.y + vec.y * -ONE_BLOCK * 1.2f,
								m_Obj->pos.z + vec.z * -ONE_BLOCK * 1.2f
							),
							EasingInOutSine
						);
					}

					if (m_Obj->moveRequ.enable)
					{
						if (dashF > 0 && dashF < 30)
						{ //ためエフェクト
							if (dashF % 8 == 0)
							{
								{
									Float3 postmp = m_Obj->pos;
									postmp.z += -150.0f * m_Obj->scale.y;
									GAME_OBJECT* go = GO_CreateEffect(GO_EFFECT_TYPE_FLASH_1_ENEM, postmp);
									if (go != nullptr)
									{
										go->scale = MkF3(
											3.0f,
                                            3.0f,
                                            3.0f
										);
										GameObjectSetLifeTime(go, 32);
									}
								}
								//CameraShakeRequest(CAMERA_SHAKE_SMALL_FRAME, CAMERA_SHAKE_SMALL_POWER);
							}
						}

						if (dashF == 30)
						{ //突進
							GameObjectMoveRequest(m_Obj, 30,
								MkF3(
									m_Obj->pos.x + vec.x * ONE_BLOCK * 9.0f,
									m_Obj->pos.y + vec.y * ONE_BLOCK * 9.0f,
									m_Obj->pos.z + vec.z * ONE_BLOCK * 9.0f
								),
								EasingOutBack
							);
						}

						//突進の残像
						if (dashF >= 30 && dashF < 30 + 9)
						{
							if (dashF % 3 == 0)
							{
								GAME_OBJECT* go = GO_CreateAfterEffect(m_Obj, 30, MkF4(0.8f, 0.75f, 1.0f, 0.75f));
							}
						}

					}
				}

				if (dashF > 100 && distance > ONE_BLOCK * 6.0f)
				{ //接近
					m_Obj->vel.x += vec.x * ONE_BLOCK / 20;
					m_Obj->vel.y += vec.y * ONE_BLOCK / 20;
					m_Obj->vel.z += vec.z * ONE_BLOCK / 20;
					//if (privateDat->targetFrame % 40 == 0)
					//{
					//	GameObjectMoveRequest(obj, 60,
					//		MkF3(
					//			obj->pos.x + vec.x * OneBlock * 2.0f,
					//			obj->pos.y + vec.y * OneBlock * 2.0f,
					//			obj->pos.z + vec.z * OneBlock * 2.0f
					//		),
					//		EasingInOutSine
					//	);
					//}
				}

				//対象への向きを入れる
				m_BeforeVec = vec;
			}

			//タゲはいってからのカウント
			m_TargetFrame++;
		}
		else //対象がいないとき
		{
			//非戦闘時
			{
				//マス内のプレイヤーを検索
				std::vector<PLAYER> players = GetPlayesrFindByDistance(m_Obj->pos, ONE_BLOCK * 20);

				//マス以内にプレイヤーがいない
				if (players.size() <= 0)
				{
					//巡回
					if ((m_Obj->frameActive + GetGameObjectIndex(m_Obj) * 37) % 240 == 0)
					{
						Float3 movepos = {};
						movepos.x = sin((float)GetMainGameFrame()) * ONE_BLOCK * 4.0f;
						movepos.y = cos((float)GetMainGameFrame()) * ONE_BLOCK * 4.0f;

						GameObjectMoveRequest(m_Obj, 180,
							MkF3(
								m_SpawnPoint.x + movepos.x,
								m_SpawnPoint.y + movepos.y,
								m_SpawnPoint.z + movepos.z
							),
							EasingInOutSine
						);
					}
				}
			}

			//索敵
			{
				//マス内のプレイヤーを検索
				std::vector<PLAYER> players = GetPlayesrFindByDistance(m_Obj->pos, ONE_BLOCK * 10);
				players.reserve(PLAYER_MAX);

				//マス以内にプレイヤーがいる
				if (players.size() > 0)
				{
					//ターゲッティング
					m_TargetObjIdentifier = GetIdentifierGameObject(players[0].obj);

					{ //えふぇくと
						Float3 postmp = m_Obj->pos;
						postmp.y += -200.0f * m_Obj->scale.y;
						postmp.z += -550.0f * m_Obj->scale.y;
						GAME_OBJECT* go = GO_CreateEffect(GO_EFFECT_TYPE_EXCLAMATION, postmp);
						if (go != nullptr)
						{
							go->scale = MkF3(
								1.0f,
                                1.0f,
                                1.0f
							);
							GameObjectSetLifeTime(go, 50);
						}
					}
				}
			}

			m_BeforeVec = m_Obj->vel;
		}
	}


    //フラグがたって居たら削除する
	if (m_DeadFlag)
	{
        Float3 addPos = {};
        addPos.z = -320.0f;
        EnemyVanish(addPos);
	}
}

// ===================================================
// Draw
// オブジェクトの描画イベント
// ===================================================
void GOE_Character_Enemy_Miresu_Hyper::Draw()
{
	SuperGOE::Draw();
}

// ===================================================
// Dead
// オブジェクトのHPが0になったとき
// ===================================================
void GOE_Character_Enemy_Miresu_Hyper::Dead()
{
	SuperGOE::Dead();

	m_Obj->collision.atk.enable = false;
	m_Obj->collision.rec.enable = false;

	//まだ消さない
	//DeleteGameObject(m_Obj);
}

// ===================================================
// DamageAttacker
// オブジェクトの攻撃判定がヒットしたとき
// ===================================================
void GOE_Character_Enemy_Miresu_Hyper::DamageAttacker(GAME_OBJECT_DAMAGE_RESULT dr)
{
	SuperGOE::DamageAttacker(dr);
}

// ===================================================
// DamageReciever
// オブジェクトが
// 別のオブジェクトの攻撃判定にヒットしたとき
// ===================================================
void GOE_Character_Enemy_Miresu_Hyper::DamageReciever(GAME_OBJECT_DAMAGE_RESULT dr)
{
	SuperGOE::DamageReciever(dr);


}

// ===================================================
// BlockHit
// オブジェクトがブロックに触れたとき
// ===================================================
void GOE_Character_Enemy_Miresu_Hyper::BlockHit(GAME_OBJECT_POSMOD_RESULT pmr)
{
	SuperGOE::BlockHit(pmr);
}
