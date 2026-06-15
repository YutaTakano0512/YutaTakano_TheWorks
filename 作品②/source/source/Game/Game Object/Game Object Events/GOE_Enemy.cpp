// ===================================================
// GOE_Enemy.cpp エネミーオブジェクト
// 
// hara sougo  2024/07/24
// ===================================================
#include "../../../game_main.h"
#include "../../Sprite/sprite.h"
#include "../../Texture/texture.h"
#include "../../../Game/Entities/Block/block.h"
#include "../../Systems/collision.h"
#include "../../Systems/easing.h"
#include "../../../Core/Input/controller_main.h"

#include "../../Managers/gameManager.h"
#include "../../Managers/scene.h"
#include "../../Systems/camera.h"

#include "../gameObject.h"
#include "../gameObjectGraph.h"
#include "../gameObjectPrefab.h"

#include "../../Systems/ui.h"

#include "GOE_Enemy.h"
#include "GOE_General.h"

#include "../../Managers/soundManager.h"
#include "../../Entities/player.h"

// ===================================================
// マクロ定義
// ===================================================

// ===================================================
// プロトタイプ宣言
// ===================================================

// ===================================================
// グローバル変数
// ===================================================
//static ENEMY_DATA enemyData[GAME_OBJECT_MAX] = {};

// ===================================================
// オブジェクトの初期化
// ===================================================
void GOE_InitEnemyObject(int index)
{
	GAME_OBJECT* obj = GetGameObject(index);
	obj->privateVariable = malloc(sizeof(ENEMY_DATA));
	//enemyData[index] = {};

	ENEMY_DATA* enemyDat = (ENEMY_DATA*)obj->privateVariable;

	if (enemyDat != nullptr)
	{
		(*enemyDat) = {}; //中身を初期化
	}

}
// ===================================================
// オブジェクトの最初のフレーム
// ===================================================
void GOE_FirstFrameEnemyObject(int index)
{
	GAME_OBJECT* obj = GetGameObject(index);

	ENEMY_DATA* enemyDat = (ENEMY_DATA*)obj->privateVariable;

	if (enemyDat == nullptr)
	{
		return;
	}

	//ターゲットのプレイヤーをセット
	enemyDat->target = GetPlayer(0)->obj;

}

// ===================================================
// オブジェクトの終了
// ===================================================
void GOE_UninitEnemyObject(int index)
{
	GAME_OBJECT* obj = GetGameObject(index);

}

// ===================================================
// オブジェクトの更新
// ===================================================
void GOE_UpdateEnemyObject(int index)
{
	GAME_OBJECT* obj = GetGameObject(index);

	ENEMY_DATA* enemyDat = (ENEMY_DATA*)obj->privateVariable;
	if (enemyDat == nullptr)
	{
		return;
	}


}

// ===================================================
// オブジェクトの更新(アクティブ)
// ===================================================
void GOE_UpdateActiveEnemyObject(int index)
{
	GAME_OBJECT* obj = GetGameObject(index);

	ENEMY_DATA* enemyDat = (ENEMY_DATA*)obj->privateVariable;
	if (enemyDat == nullptr)
	{
		return;
	}

	GAME_OBJECT* targetPlayer = nullptr;
	float targetAngleRad = 0;
	if (enemyDat != nullptr)
	{ //ターゲットのプレイヤーをセット
		targetPlayer = enemyDat->target;

		if (targetPlayer != nullptr)
		{
			targetAngleRad = atan2(targetPlayer->pos.y - obj->pos.y, targetPlayer->pos.x - obj->pos.x);
		}
	}
	Float3 targetPos = {};
	if (targetPlayer != nullptr)
	{
		targetPos = targetPlayer->pos;
	}

	float hpRate = (float)obj->status.hp / obj->status.hpMax;

	SPRITE_ANIM_DAT* graSad = obj->graphic.graph.spriteAnim;

}

void GOE_DrawEnemyObject(int index)
{
	GAME_OBJECT* obj = GetGameObject(index);
}


void GOE_DeadEnemyObject(int index)
{
	GAME_OBJECT* obj = GetGameObject(index);

	ENEMY_DATA* enemyDat = (ENEMY_DATA*)obj->privateVariable;

}


void GOE_DamageAttackerEnemyObject(int index, GAME_OBJECT_DAMAGE_RESULT dr)
{
}

void GOE_DamageRecieverEnemyObject(int index, GAME_OBJECT_DAMAGE_RESULT dr)
{
	GAME_OBJECT* obj = GetGameObject(index);

	ENEMY_DATA* enemyDat = (ENEMY_DATA*)obj->privateVariable;
	if (enemyDat == nullptr)
	{
		return;
	}

}

void GOE_BlockHitEnemyObject(int index, GAME_OBJECT_POSMOD_RESULT pmr)
{

}



//
// エネミーのたま
//
void GOE_InitEnemyBulletObject(int index)
{

}
void GOE_FirstFrameEnemyBulletObject(int index)
{
	GAME_OBJECT* obj = GetGameObject(index);

}
void GOE_UninitEnemyBulletObject(int index)
{

}
void GOE_UpdateEnemyBulletObject(int index)
{

}
void GOE_UpdateActiveEnemyBulletObject(int index)
{
	GAME_OBJECT* obj = GetGameObject(index);

	obj->graphic.graph.spriteAnim[0].rot += 8.0f;

	if (obj->frame % 24 == 0)
	//	if (obj->frame % 12 == 0)
	{
		GAME_OBJECT* go = GO_CreateAfterEffect(obj, 20, MkF4(1.5f, 1.0f, 2.0f, 0.5f));
	}
}

void GOE_DamageAttackerEnemyBulletObject(int index, GAME_OBJECT_DAMAGE_RESULT dr)
{
	GAME_OBJECT* obj = GetGameObject(index);
	{
		GAME_OBJECT* go = GO_CreateEffect(GO_EFFECT_TYPE_TWINKLE, obj->pos);
		if (go != nullptr)
		{
			go->scale = MkF2(0.8f, 0.8f);
			GameObjectSetLifeTime(go, 24);
		}
	}
	{
		GAME_OBJECT* go = GO_CreateEffect(GO_EFFECT_TYPE_SHOCKWAVE_ENEM, obj->pos);
		if (go != nullptr)
		{
			go->size = MkF2(0.2f, 0.2f);
			go->scale = MkF2(1.8f, 1.8f);
			GameObjectSetLifeTime(go, 25);
		}
	}

	{
		//SoundEffectPlayRequest(SOUND_TYPE_HIT_SMALL, 1.0f, 0);
	}
	DeleteGameObject(index);
}
void GOE_DamageRecieverEnemyBulletObject(int index, GAME_OBJECT_DAMAGE_RESULT dr)
{

}

void GOE_BlockHitEnemyBulletObject(int index, GAME_OBJECT_POSMOD_RESULT pmr)
{
	GAME_OBJECT* obj = GetGameObject(index);
	{
		GAME_OBJECT* go = GO_CreateEffect(GO_EFFECT_TYPE_SHOCKWAVE_ENEM, obj->pos);
		go->scale = MkF2(0.5f, 0.5f);
		GameObjectSetLifeTime(go, 16);
	}
	DeleteGameObject(index);
}