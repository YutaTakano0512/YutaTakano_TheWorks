// ===================================================
// GE_Shoot.cpp シュートの処理
// 
// hara sougo  2024/11/14
// ===================================================
#include "../../game_main.h"
#include "../Sprite/sprite.h"
#include "../Texture/texture.h"
#include "../Entities/Block/block.h"
#include "../Systems/collision.h"
#include "../Systems/easing.h"

#include "../Managers/gameManager.h"

#include "../Game Object/gameObject.h"
#include "../Game Object/gameObjectPrefab.h"

#include "../Systems/ui.h"

#include "../Entities/player.h"
#include "../Systems/camera.h"

#include "../Entities/character.h"

#include "GE_Shoot.h"

// ===================================================
// マクロ定義
// ===================================================
constexpr int SHOOT_CHARA_MAX = 2;

constexpr int SHOOT_PUSH_COUNT_MAX = 25;

// ===================================================
// 構造体宣言
// ===================================================
struct GE_SHOOT_DAT {
	bool enable;


	bool mashOk;

	int frame;

	bool finished;
	int finishedFrame;
	int winCharaIndex;

	GAME_OBJECT* characters[SHOOT_CHARA_MAX];
	Character::OBJECT_DAT* charactersObjDat[SHOOT_CHARA_MAX];

	int pushCount[SHOOT_CHARA_MAX];
};

// ===================================================
// プロトタイプ宣言
// ===================================================

// ===================================================
// グローバル変数
// ===================================================
static GE_SHOOT_DAT shootDat = {};


void GE_InitShoot(void)
{
	shootDat = {};
}

void GE_UninitShoot(void)
{
}

void GE_UpdateShoot(void)
{
	//if (shootDat.enable)
	//{
	//	int frame = shootDat.frame;

	//	GAME_OBJECT* startChara = shootDat.characters[0];
	//	GAME_OBJECT* oppChara = shootDat.characters[1];
	//	Character::OBJECT_DAT* startCharaObjDat = shootDat.charactersObjDat[0];
	//	Character::OBJECT_DAT* oppCharaObjDat = shootDat.charactersObjDat[1];

	//	if (startChara != nullptr && oppChara != nullptr)
	//	{
	//		//即席で

	//		if (frame == 0)
	//		{
	//			GameStopRequest(60);

	//			CameraZoomRequest(0.03f, GetGameObjectIndex(startChara), EasingInOutSine);
	//			CameraZoomResetRequest(0.03f, EasingInOutSine, 60);
	//		}

	//		if (frame == 60)
	//		{
	//			shootDat.mashOk = true;
	//		}


	//		if (shootDat.finished)
	//		{ //勝者が決まったとき
	//			if (shootDat.finishedFrame > 25)
	//			{
	//				shootDat.enable = false;
	//			}

	//			shootDat.finishedFrame++;
	//		}
	//		else if(shootDat.mashOk)
	//		{ //連打Ok

	//			{ //連打中
	//				if (Character::GetCtrlTrigger(startCharaObjDat, CONTROLLER_BUTTON_TYPE_A))
	//				{
	//					shootDat.pushCount[0]++;
	//					CameraShakeRequest(CAMERA_SHAKE_SMALL_FRAME, CAMERA_SHAKE_SMALL_FRAME);
	//				}

	//				if (Character::GetCtrlTrigger(oppCharaObjDat, CONTROLLER_BUTTON_TYPE_A))
	//				{
	//					shootDat.pushCount[1]++;
	//					CameraShakeRequest(CAMERA_SHAKE_SMALL_FRAME, CAMERA_SHAKE_SMALL_FRAME);
	//				}
	//			}

	//			//終了処理 (後で関数化するか)
	//			if (shootDat.pushCount[0] > SHOOT_PUSH_COUNT_MAX)
	//			{
	//				//Character::OBJECT_DAT* oppCobD = Character::GetObjectDataCharacterGameObject(oppChara);
	//				//Character::PlayMotionCharacterGameObject(oppCobD,
	//				//	Character::GetCharacterMotionFindByName(oppCobD->data, "DAMAGE"),
	//				//	false,
	//				//	30
	//				//);
	//				GAME_OBJECT_COLLISION_ATTACK_CHILD atkCol = {};
	//				atkCol.enable = true;
	//				atkCol.hitStern = 30;

	//				GameObjectDamage(oppChara, startChara, 500, atkCol, {});
	//				CameraZoomRequest(0.03f, GetGameObjectIndex(oppChara), EasingOutSine);

	//				shootDat.winCharaIndex = 0;

	//				shootDat.finished = true;
	//			}
	//			else if (shootDat.pushCount[1] > SHOOT_PUSH_COUNT_MAX)
	//			{
	//				//Character::OBJECT_DAT* stCobD = Character::GetObjectDataCharacterGameObject(startChara);
	//				//Character::PlayMotionCharacterGameObject(stCobD,
	//				//	Character::GetCharacterMotionFindByName(stCobD->data, "DAMAGE"),
	//				//	false,
	//				//	180
	//				//);
	//				GAME_OBJECT_COLLISION_ATTACK_CHILD atkCol = {};
	//				atkCol.enable = true;
	//				atkCol.hitStern = 300;

	//				GameObjectDamage(startChara, oppChara, 250, atkCol, {});
	//				CameraZoomRequest(0.03f, GetGameObjectIndex(startChara), EasingOutSine);

	//				shootDat.winCharaIndex = 1;

	//				shootDat.finished = true;
	//			}
	//			if (shootDat.finished)
	//			{
	//				GameSlowRequest(40, 2);
	//				CameraZoomResetRequest(0.02f, EasingInOutSine, 60);

	//				//GameStopRequest(40);
	//				CameraShakeRequest(CAMERA_SHAKE_EXLARGE_FRAME, CAMERA_SHAKE_EXLARGE_POWER);
	//			}
	//		}
	//	}

	//	shootDat.frame++;
	//}
}

void GE_DrawShoot(void)
{
	//if (shootDat.enable)
	//{
	//	int maxf = shootDat.pushCount[0] + shootDat.pushCount[1] + 20;
	//	//int maxf = SHOOT_PUSH_COUNT_MAX * 2;

	//	float wtmp = SCREEN_WIDTH * 0.8f;
	//	float htmp = SCREEN_HEIGHT * 0.05f;

	//	float m0 = (float)(shootDat.pushCount[0] + 10) / (float)maxf;
	//	float m1 = (float)(shootDat.pushCount[1] + 10) / (float)maxf;

	//	if(shootDat.finished)
	//	{ //かった時の演出
	//		float mfini = (float)shootDat.finishedFrame / 10.0f;
	//		if (mfini > 1.0f)
	//		{
	//			mfini = 1.0f;
	//		}
	//		mfini = EasingInOutBack(mfini);

	//		if (shootDat.winCharaIndex == 1)
	//		{
	//			m1 += (1.0f - m1) * mfini;

	//			m0 = 1.0f - m1;
	//		}
	//		else
	//		{
	//			m0 += (1.0f - m0) * mfini;

	//			m1 = 1.0f - m0;
	//		}
	//	}

	//	{
	//		DRAW_SPRITE_DAT dsdat = {};
	//		dsdat.x = wtmp * m1 / -2;
	//		dsdat.y = SCREEN_HEIGHT * 0.4f;
	//		dsdat.w = wtmp * m0;
	//		dsdat.h = htmp;
	//		dsdat.rot = 0.0f;
	//		dsdat.color = { 2.0f, 0.2f, 0.2f, 0.8f };

	//		dsdat.texNo = 0;
	//		dsdat.loadTexType = LOADTEXTURETYPE_MAIN;

	//		dsdat.texUV = { 0.0f, 0.0f };
	//		dsdat.texWH = { 1.0f, 1.0f };
	//		dsdat.startPos = { 0.0f, 0.0f };
	//		dsdat.startRot = 0.0f;
	//		dsdat.startScale = { 1.0f, 1.0f };

	//		DrawSpriteInCamera(dsdat, false, true);
	//	}
	//	{
	//		DRAW_SPRITE_DAT dsdat = {};
	//		dsdat.x = wtmp * m0 / 2;
	//		dsdat.y = SCREEN_HEIGHT * 0.4f;
	//		dsdat.w = wtmp * m1;
	//		dsdat.h = htmp;
	//		dsdat.rot = 0.0f;
	//		dsdat.color = { 0.2f, 0.2f, 2.0f, 0.8f };

	//		dsdat.texNo = 0;
	//		dsdat.loadTexType = LOADTEXTURETYPE_MAIN;

	//		dsdat.texUV = { 0.0f, 0.0f };
	//		dsdat.texWH = { 1.0f, 1.0f };
	//		dsdat.startPos = { 0.0f, 0.0f };
	//		dsdat.startRot = 0.0f;
	//		dsdat.startScale = { 1.0f, 1.0f };

	//		DrawSpriteInCamera(dsdat, false, true);
	//	}
	//}
}

bool GE_IsEnableShoot()
{
	return shootDat.enable;
}

void GE_ShootStartRequest(GAME_OBJECT* startChara)
{
	//GAME_OBJECT* oppChara = nullptr;

	////仕掛けたキャラの対戦相手を探す
	//PLAYER* plyrOpp = GetPlayerOpponent(GetPlayer(startChara));
	//if (plyrOpp != nullptr)
	//{
	//	oppChara = plyrOpp->obj;
	//}

	////プレイヤーがそろっていたら開始する
	//if (startChara != nullptr
	//	&& oppChara != nullptr)
	//{
	//	Character::OBJECT_DAT* startCharaObjDat = Character::GetObjectDataCharacterGameObject(startChara);
	//	Character::OBJECT_DAT* oppCharaObjDat = Character::GetObjectDataCharacterGameObject(oppChara);

	//	if (startCharaObjDat != nullptr
	//		&& oppCharaObjDat != nullptr)
	//	{
	//		shootDat = {}; //初期化
	//		shootDat.enable = true; //有効化

	//		//仕掛けたキャラをセット
	//		shootDat.characters[0] = startChara;
	//		shootDat.charactersObjDat[0] = startCharaObjDat;
	//		//仕掛けたキャラの対戦相手をセット
	//		shootDat.characters[1] = oppChara;
	//		shootDat.charactersObjDat[1] = oppCharaObjDat;
	//	}
	//}

}