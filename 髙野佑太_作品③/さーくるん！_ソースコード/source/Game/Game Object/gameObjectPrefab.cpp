// ===================================================
// gameObjectPrefab.cpp ゲームオブジェクトの設計図
// 
// hara sougo  2024/07/22
// ===================================================
#include "../../game_main.h"
#include "../../Game/Configuration/Param.h"
#include "gameObject.h"
#include "gameObjectGraph.h"

//オブジェクトのインクルード
#include "../Entities/player.h"
#include "Game Object Events/GOE_Test.h"
#include "Game Object Events/GOE_Enemy.h"
#include "Game Object Events/GOE_General.h"


static GAME_OBJECT gameObjectPrefab[GAME_OBJECT_TYPE_MAX] = {};

// ===================================================
// 初期化
// ===================================================
void InitGameObjectPrefab(void)
{
	for (int i = 0; i < GAME_OBJECT_TYPE_MAX; i++)
	{
		InitGameObject(gameObjectPrefab + i);
	}


	GAME_OBJECT_EVENTS eventGeneral = {};
	{ //イベント汎用
		//eventGeneral.Init /*			*/ = GOE_InitGeneralObject;
		//eventGeneral.FirstFrame /*		*/ = GOE_FirstFrameGeneralObject;
		//eventGeneral.Uninit /*			*/ = GOE_UninitGeneralObject;
		//eventGeneral.Update /*			*/ = GOE_UpdateGeneralObject;
		//eventGeneral.UpdateActive /*	*/ = GOE_UpdateActiveGeneralObject;
		//eventGeneral.Draw /*			*/ = GOE_DrawGeneralObject;

		//eventGeneral.Dead /*			*/ = GOE_DeadGeneralObject;
		//eventGeneral.DamageAttacker /*	*/ = GOE_DamageAttackerGeneralObject;
		//eventGeneral.DamageReciever /*	*/ = GOE_DamageRecieverGeneralObject;
	}
	GAME_OBJECT_EVENTS eventEnemy = {};
	{ //イベント汎用
		//eventEnemy.Init /*				*/ = GOE_InitEnemyObject;
		//eventEnemy.FirstFrame /*		*/ = GOE_FirstFrameEnemyObject;
		//eventEnemy.Uninit /*			*/ = GOE_UninitEnemyObject;
		//eventEnemy.Update /*			*/ = GOE_UpdateEnemyObject;
		//eventEnemy.UpdateActive /*		*/ = GOE_UpdateActiveEnemyObject;
		//eventEnemy.Draw /*				*/ = GOE_DrawEnemyObject;

		//eventEnemy.Dead /*				*/ = GOE_DeadEnemyObject;
		//eventEnemy.DamageAttacker /*	*/ = GOE_DamageAttackerEnemyObject;
		//eventEnemy.DamageReciever /*	*/ = GOE_DamageRecieverEnemyObject;
	}

	//--------------------------------------------------
	// 無
	//--------------------------------------------------
	{
		GAME_OBJECT* go = gameObjectPrefab + GAME_OBJECT_TYPE_VOID;

		go->graphic = GetGameObjectGraph(GAME_OBJECT_GRAPH_VOID);
	}



	////--------------------------------------------------
	//// プレイヤー
	////--------------------------------------------------
	//{
	//	GAME_OBJECT* go = gameObjectPrefab + GAME_OBJECT_TYPE_PLAYER;

	//	go->family = GAME_OBJECT_FAMILY_PLAYER;

	//	go->zIndex /*	*/ = GAME_OBJECT_ZINDEX_PLAYER;

	//	go->size /*		*/ = MkF2(PARAM_GAME_OBJECT_PLAYER_SIZE_X, PARAM_GAME_OBJECT_PLAYER_SIZE_Y);
	//	go->cameraInKeep = true;
	//	go->alwaysUpdate = true;

	//	go->friction /*	*/ = PLAYER_FRICTION;
	//	go->gravity /*	*/ = PLAYER_GRAVITY;

	//	go->graphic /*	*/ = GetGameObjectGraph(GAME_OBJECT_GRAPH_PLAYER);

	//	{ //ステータス
	//		GAME_OBJECT_STATUS* s = &go->status;
	//		s->hpMax = PARAM_GAME_OBJECT_PLAYER_STATUS_HP;
	//		s->hp = s->hpMax;
	//		s->guardType = GAME_OBJECT_GUARD_TYPE_NORMAL;
	//	}

	//	{ //イベント
	//		GAME_OBJECT_EVENTS* e = &go->events;
	//		e->Init /*			*/ = GOE_InitPlayerObject;
	//		e->FirstFrame /*	*/ = GOE_FirstFramePlayerObject;
	//		e->Uninit /*		*/ = GOE_UninitPlayerObject;
	//		e->Update /*		*/ = GOE_UpdatePlayerObject;
	//		e->UpdateActive /*	*/ = GOE_UpdateActivePlayerObject;
	//		e->Draw /*			*/ = GOE_DrawPlayerObject;

	//		e->Dead /*			*/ = GOE_DeadPlayerObject;
	//		e->DamageAttacker /**/ = GOE_DamageAttackerPlayerObject;
	//		e->DamageReciever /**/ = GOE_DamageRecieverPlayerObject;
	//	}

	//	{ //当たり判定
	//		GAME_OBJECT_COLLISION* c = &go->collision;

	//		//くらい判定
	//		c->rec.enable = true;
	//		//{
	//		//	GAME_OBJECT_COLLISION_RECIEVE_CHILD* cc = c->rec.collision + 0;
	//		//	cc->enable = true;
	//		//	cc->pos = MkF3(0.0f, 0.0f, 0.0f);
	//		//	cc->size.x = PARAM_GAME_OBJECT_PLAYER_COL_REC_SIZE_X;
	//		//	cc->size.y = PARAM_GAME_OBJECT_PLAYER_COL_REC_SIZE_Y;
	//		//	cc->size.z = PARAM_GAME_OBJECT_PLAYER_COL_REC_SIZE_Z;
	//		//}
	//		
	//		//{
	//		//	GAME_OBJECT_COLLISION_RECIEVE_CHILD* cc = c->rec.collision + 0;
	//		//	cc->enable = true;
	//		//	cc->pos = MkF3(0.0f, 20.0f, 0.0f);
	//		//	cc->size.x = PARAM_GAME_OBJECT_PLAYER_COL_REC_SIZE_X * 1.5f;
	//		//	cc->size.y = PARAM_GAME_OBJECT_PLAYER_COL_REC_SIZE_Y * 0.7f;
	//		//	cc->size.z = PARAM_GAME_OBJECT_PLAYER_COL_REC_SIZE_Z;
	//		//}
	//		//{
	//		//	GAME_OBJECT_COLLISION_RECIEVE_CHILD* cc = c->rec.collision + 1;
	//		//	cc->enable = true;
	//		//	cc->pos = MkF3(50.0f, -30.0f, 0.0f);
	//		//	cc->size.x = PARAM_GAME_OBJECT_PLAYER_COL_REC_SIZE_X * 0.3f;
	//		//	cc->size.y = PARAM_GAME_OBJECT_PLAYER_COL_REC_SIZE_Y * 1.0f;
	//		//	cc->size.z = PARAM_GAME_OBJECT_PLAYER_COL_REC_SIZE_Z;
	//		//}
	//		//{
	//		//	GAME_OBJECT_COLLISION_RECIEVE_CHILD* cc = c->rec.collision + 2;
	//		//	cc->enable = true;
	//		//	cc->pos = MkF3(-50.0f, -30.0f, 0.0f);
	//		//	cc->size.x = PARAM_GAME_OBJECT_PLAYER_COL_REC_SIZE_X * 0.3f;
	//		//	cc->size.y = PARAM_GAME_OBJECT_PLAYER_COL_REC_SIZE_Y * 1.0f;
	//		//	cc->size.z = PARAM_GAME_OBJECT_PLAYER_COL_REC_SIZE_Z;
	//		//}

	//		{
	//			GAME_OBJECT_COLLISION_RECIEVE_CHILD* cc = c->rec.collision + 0;
	//			cc->enable = true;
	//			cc->pos = MkF3(0.0f, 0.0f, 0.0f);
	//			cc->size.x = PARAM_GAME_OBJECT_PLAYER_COL_REC_SIZE_X * 0.6f;
	//			cc->size.y = PARAM_GAME_OBJECT_PLAYER_COL_REC_SIZE_Y * 1.5f;
	//			cc->size.z = PARAM_GAME_OBJECT_PLAYER_COL_REC_SIZE_Z;
	//		}
	//		{
	//			GAME_OBJECT_COLLISION_RECIEVE_CHILD* cc = c->rec.collision + 1;
	//			cc->enable = true;
	//			cc->pos = MkF3(0.0f, 0.0f, 0.0f);
	//			cc->size.x = PARAM_GAME_OBJECT_PLAYER_COL_REC_SIZE_X * 1.6f;
	//			cc->size.y = PARAM_GAME_OBJECT_PLAYER_COL_REC_SIZE_Y * 0.3f;
	//			cc->size.z = PARAM_GAME_OBJECT_PLAYER_COL_REC_SIZE_Z;
	//		}
	//	}
	//}


	//--------------------------------------------------
	// 障害物
	//--------------------------------------------------
	GAME_OBJECT goStageObjectTmp = {};
	InitGameObject(&goStageObjectTmp);
	{
		GAME_OBJECT* go = &goStageObjectTmp;

		go->family[GAME_OBJECT_FAMILY_MISC] = true;

		go->zIndex /*	*/ = GAME_OBJECT_ZINDEX_OBJECT;

		//go->graphic = GetGameObjectGraph(GAME_OBJECT_GRAPH_PLAYERTEST_DOWN);
		//go->size /*		*/ = go->graphic.graph.spriteAnim[0].size;
		go->graphic = {};
		go->size = MkF2(96.0f, 96.0f);


		{ //ステータス
			GAME_OBJECT_STATUS* s = &go->status;
			s->hpMax = PARAM_GAME_OBJECT_TEST_STATUS_HP;
			s->hp = s->hpMax;
			s->guardType = GAME_OBJECT_GUARD_TYPE_NORMAL;
		}

		{ //イベント
			GAME_OBJECT_EVENTS* e = &go->events;
		}

		{ //当たり判定
			GAME_OBJECT_COLLISION* c = &go->collision;

			//攻撃判定
			c->atk.enable = false;

			//くらい判定
			c->rec.enable = false;
		}
	}

	//--------------------------------------------------
	// 残骸
	{
		GAME_OBJECT* go = gameObjectPrefab + GAME_OBJECT_TYPE_ISEKI_ZANGAI_0;
		(*go) = goStageObjectTmp;

		go->graphic = GetGameObjectGraph(GAME_OBJECT_GRAPH_ISEKI_ZANGAI_0);
		//go->size /*		*/ = go->graphic.graph.spriteAnim[0].size;
		go->graphic.rot3d.x = 40.0f;
		go->graphic.pos3d.z = -go->graphic.graph.spriteAnim[0].size.y * 0.18f;
	}
	//--------------------------------------------------
	// 残骸
	{
		GAME_OBJECT* go = gameObjectPrefab + GAME_OBJECT_TYPE_ISEKI_ZANGAI_1;
		(*go) = goStageObjectTmp;

		go->graphic = GetGameObjectGraph(GAME_OBJECT_GRAPH_ISEKI_ZANGAI_1);
		//go->size /*		*/ = go->graphic.graph.spriteAnim[0].size;
		go->graphic.rot3d.x = 5.0f;
		go->graphic.pos3d.z = -go->graphic.graph.spriteAnim[0].size.y * 0.06f;
	}
	//--------------------------------------------------
	// 残骸
	{
		GAME_OBJECT* go = gameObjectPrefab + GAME_OBJECT_TYPE_ISEKI_ZANGAI_2;
		(*go) = goStageObjectTmp;

		go->graphic = GetGameObjectGraph(GAME_OBJECT_GRAPH_ISEKI_ZANGAI_2);
		//go->size /*		*/ = go->graphic.graph.spriteAnim[0].size;
		go->graphic.rot3d.x = 40.0f;
		go->graphic.pos3d.z = -go->graphic.graph.spriteAnim[0].size.y * 0.18f;
	}
	//--------------------------------------------------
	// 台座
	{
		GAME_OBJECT* go = gameObjectPrefab + GAME_OBJECT_TYPE_ISEKI_DAIZA_0;
		(*go) = goStageObjectTmp;

		go->graphic = GetGameObjectGraph(GAME_OBJECT_GRAPH_ISEKI_DAIZA_0);
		//go->size /*		*/ = go->graphic.graph.spriteAnim[0].size;
		go->graphic.rot3d.x = 18.0f;
		go->graphic.pos3d.z = -go->graphic.graph.spriteAnim[0].size.y * 0.16f;
	}
	//--------------------------------------------------
	// 柱
	{
		GAME_OBJECT* go = gameObjectPrefab + GAME_OBJECT_TYPE_ISEKI_HASHIRA_0;
		(*go) = goStageObjectTmp;

		go->graphic = GetGameObjectGraph(GAME_OBJECT_GRAPH_ISEKI_HASHIRA_0);
		//go->size /*		*/ = go->graphic.graph.spriteAnim[0].size;
		go->graphic.rot3d.x = 50.0f;
		go->graphic.pos3d.z = -go->graphic.graph.spriteAnim[0].size.y * 0.4f;
	}
	//--------------------------------------------------
	// 柱
	{
		GAME_OBJECT* go = gameObjectPrefab + GAME_OBJECT_TYPE_ISEKI_HASHIRA_TAORETA_0;
		(*go) = goStageObjectTmp;

		go->graphic = GetGameObjectGraph(GAME_OBJECT_GRAPH_ISEKI_HASHIRA_TAORETA_0);
		//go->size /*		*/ = go->graphic.graph.spriteAnim[0].size;
		go->graphic.rot3d.x = 30.0f;
		go->graphic.pos3d.z = -go->graphic.graph.spriteAnim[0].size.y * 0.1f;
	}
	//--------------------------------------------------
	// 感圧版
	{
		GAME_OBJECT* go = gameObjectPrefab + GAME_OBJECT_TYPE_ISEKI_KANATSUBAN_0;
		(*go) = goStageObjectTmp;

		go->graphic = GetGameObjectGraph(GAME_OBJECT_GRAPH_ISEKI_KANATSUBAN_0);
		//go->size /*		*/ = go->graphic.graph.spriteAnim[0].size;
		go->graphic.rot3d.x = 3.0f;
		go->graphic.pos3d.z = -go->graphic.graph.spriteAnim[0].size.y * 0.05f;
	}




	//--------------------------------------------------
	// テストオブジェクト
	//--------------------------------------------------
	{
		GAME_OBJECT* go = gameObjectPrefab + GAME_OBJECT_TYPE_TEST;

		go->family[GAME_OBJECT_FAMILY_ENEMY] = true;

		go->zIndex /*	*/ = GAME_OBJECT_ZINDEX_ENEMY;

		go->size /*		*/ = MkF2(PARAM_GAME_OBJECT_GENERAL_SIZE_X, PARAM_GAME_OBJECT_GENERAL_SIZE_Y);

		go->graphic = GetGameObjectGraph(GAME_OBJECT_GRAPH_TEST);

		{ //ステータス
			GAME_OBJECT_STATUS* s = &go->status;
			s->hpMax = PARAM_GAME_OBJECT_TEST_STATUS_HP;
			s->hp = s->hpMax;
			s->guardType = GAME_OBJECT_GUARD_TYPE_NORMAL;
		}

		{ //イベント
			GAME_OBJECT_EVENTS* e = &go->events;
			//e->Init /*			*/ = GOE_InitTestObject;
			//e->FirstFrame /*	*/ = GOE_FirstFrameTestObject;
			//e->Uninit /*		*/ = GOE_UninitTestObject;
			//e->Update /*		*/ = GOE_UpdateTestObject;
			//e->UpdateActive /*	*/ = GOE_UpdateActiveTestObject;

			//e->DamageAttacker /**/ = GOE_DamageAttackerTestObject;
			//e->DamageReciever /**/ = GOE_DamageRecieverTestObject;
		}

		{ //当たり判定
			GAME_OBJECT_COLLISION* c = &go->collision;

			//攻撃判定
			c->atk.enable = true;
			{
				GAME_OBJECT_COLLISION_ATTACK_CHILD* cc = c->atk.collision + 0;
				cc->enable = true;
				cc->pos /*				*/ = MkF3(40.0f, 0.0f, 0.0f);
				cc->size.x /*			*/ = PARAM_GAME_OBJECT_TEST_COL_ATK_SIZE_X;
				cc->size.y /*			*/ = PARAM_GAME_OBJECT_TEST_COL_ATK_SIZE_Y;
				cc->size.z /*			*/ = PARAM_GAME_OBJECT_TEST_COL_ATK_SIZE_Z;
				cc->power /*			*/ = PARAM_GAME_OBJECT_TEST_COL_ATK_POWER;
				cc->hitStopAttacker /*	*/ = PARAM_GAME_OBJECT_TEST_COL_ATK_HITSTOP_ATK;
				cc->hitStopReciever/*	*/ = PARAM_GAME_OBJECT_TEST_COL_ATK_HITSTOP_REC;
				cc->hitCycle /*			*/ = PARAM_GAME_OBJECT_TEST_COL_ATK_HITCYCLE;
				cc->subjectFamily[GAME_OBJECT_FAMILY_PLAYER] = true;
			}
			{
				GAME_OBJECT_COLLISION_ATTACK_CHILD* cc = c->atk.collision + 1;
				(*cc) = c->atk.collision[0];
				cc->pos /*				*/ = MkF3(120.0f, -30.0f, 0.0f);
				cc->size.x /*			*/ = PARAM_GAME_OBJECT_TEST_COL_ATK_SIZE_X * 2.0f;
				cc->size.y /*			*/ = PARAM_GAME_OBJECT_TEST_COL_ATK_SIZE_Y * 2.0f;
				cc->size.z /*			*/ = PARAM_GAME_OBJECT_TEST_COL_ATK_SIZE_Z;
			}
			{
				GAME_OBJECT_COLLISION_ATTACK_CHILD* cc = c->atk.collision + 2;
				(*cc) = c->atk.collision[0];
				cc->pos /*				*/ = MkF3(160.0f, -120.0f, 0.0f);
				cc->size.x /*			*/ = PARAM_GAME_OBJECT_TEST_COL_ATK_SIZE_X * 1.0f;
				cc->size.y /*			*/ = PARAM_GAME_OBJECT_TEST_COL_ATK_SIZE_Y * 2.0f;
				cc->size.z /*			*/ = PARAM_GAME_OBJECT_TEST_COL_ATK_SIZE_Z;
			}

			//くらい判定
			c->rec.enable = true;
			{
				GAME_OBJECT_COLLISION_RECIEVE_CHILD* cc = c->rec.collision + 0;
				cc->enable = true;
				cc->pos = MkF3(0.0f, 0.0f, 0.0f);
				cc->size.x = PARAM_GAME_OBJECT_TEST_COL_REC_SIZE_X;
				cc->size.y = PARAM_GAME_OBJECT_TEST_COL_REC_SIZE_Y;
				cc->size.z = PARAM_GAME_OBJECT_TEST_COL_REC_SIZE_Z;
			}
		}
	}
	{
		GAME_OBJECT* go = gameObjectPrefab + GAME_OBJECT_TYPE_TEST_1;
		(*go) = gameObjectPrefab[GAME_OBJECT_TYPE_TEST];

		{ //当たり判定
			GAME_OBJECT_COLLISION* c = &go->collision;

			//攻撃判定
			c->atk.enable = true;
			for (int i = 0; i < GAME_OBJECT_COLLISION_NUM_MAX; i++)
			{
				c->atk.collision[i] = {};
			}
			{
				GAME_OBJECT_COLLISION_ATTACK_CHILD* cc = c->atk.collision + 0;
				cc->enable = true;
				cc->pos /*				*/ = MkF3(60.0f, 0.0f, 0.0f);
				cc->size.x /*			*/ = PARAM_GAME_OBJECT_TEST_1_COL_ATK_SIZE_X;
				cc->size.y /*			*/ = PARAM_GAME_OBJECT_TEST_1_COL_ATK_SIZE_Y;
				cc->size.z /*			*/ = PARAM_GAME_OBJECT_TEST_1_COL_ATK_SIZE_Z;
				cc->power /*			*/ = PARAM_GAME_OBJECT_TEST_1_COL_ATK_POWER;
				cc->hitStopAttacker /*	*/ = PARAM_GAME_OBJECT_TEST_1_COL_ATK_HITSTOP_ATK;
				cc->hitStopReciever/*	*/ = PARAM_GAME_OBJECT_TEST_1_COL_ATK_HITSTOP_REC;
				cc->hitCycle /*			*/ = PARAM_GAME_OBJECT_TEST_1_COL_ATK_HITCYCLE;
				cc->subjectFamily[GAME_OBJECT_FAMILY_PLAYER] = true;
			}

			//くらい判定
			c->rec.enable = true;
			for (int i = 0; i < GAME_OBJECT_COLLISION_NUM_MAX; i++)
			{
				c->rec.collision[i] = {};
			}
			{
				GAME_OBJECT_COLLISION_RECIEVE_CHILD* cc = c->rec.collision + 0;
				cc->enable = true;
				cc->pos /*				*/ = MkF3(0.0f, 0.0f, 0.0f);
				cc->size.x = PARAM_GAME_OBJECT_TEST_COL_REC_SIZE_X;
				cc->size.y = PARAM_GAME_OBJECT_TEST_COL_REC_SIZE_Y;
				cc->size.z = PARAM_GAME_OBJECT_TEST_COL_REC_SIZE_Z;
			}
		}
	}
	{
		GAME_OBJECT* go = gameObjectPrefab + GAME_OBJECT_TYPE_TEST_2;
		(*go) = gameObjectPrefab[GAME_OBJECT_TYPE_TEST];

		{ //当たり判定
			GAME_OBJECT_COLLISION* c = &go->collision;

			//攻撃判定
			c->atk.enable = true;
			for (int i = 0; i < GAME_OBJECT_COLLISION_NUM_MAX; i++)
			{
				c->atk.collision[i] = {};
			}
			{
				GAME_OBJECT_COLLISION_ATTACK_CHILD* cc = c->atk.collision + 0;
				cc->enable = true;
				cc->pos /*				*/ = MkF3(0.0f, 0.0f, 0.0f);
				cc->size.x /*			*/ = PARAM_GAME_OBJECT_TEST_2_COL_ATK_SIZE_X;
				cc->size.y /*			*/ = PARAM_GAME_OBJECT_TEST_2_COL_ATK_SIZE_Y;
				cc->size.z /*			*/ = PARAM_GAME_OBJECT_TEST_2_COL_ATK_SIZE_Z;
				cc->power /*			*/ = PARAM_GAME_OBJECT_TEST_2_COL_ATK_POWER;
				cc->hitStopAttacker /*	*/ = PARAM_GAME_OBJECT_TEST_2_COL_ATK_HITSTOP_ATK;
				cc->hitStopReciever/*	*/ = PARAM_GAME_OBJECT_TEST_2_COL_ATK_HITSTOP_REC;
				cc->hitCycle /*			*/ = PARAM_GAME_OBJECT_TEST_2_COL_ATK_HITCYCLE;
				cc->subjectFamily[GAME_OBJECT_FAMILY_PLAYER] = true;
			}

			//くらい判定
			c->rec.enable = true;
			for (int i = 0; i < GAME_OBJECT_COLLISION_NUM_MAX; i++)
			{
				c->rec.collision[i] = {};
			}
			{
				GAME_OBJECT_COLLISION_RECIEVE_CHILD* cc = c->rec.collision + 0;
				cc->enable = true;
				cc->pos /*				*/ = MkF3(0.0f, 0.0f, 0.0f);
				cc->size.x = PARAM_GAME_OBJECT_TEST_COL_REC_SIZE_X;
				cc->size.y = PARAM_GAME_OBJECT_TEST_COL_REC_SIZE_Y;
				cc->size.z = PARAM_GAME_OBJECT_TEST_COL_REC_SIZE_Z;
			}
		}
	}

	//--------------------------------------------------
	// テストオブジェクト
	//--------------------------------------------------
	{
		GAME_OBJECT* go = gameObjectPrefab + GAME_OBJECT_TYPE_PLAYERTEST_DOWN;

		go->family[GAME_OBJECT_FAMILY_ENEMY] = true;

		go->zIndex /*	*/ = GAME_OBJECT_ZINDEX_ENEMY;

		go->graphic = GetGameObjectGraph(GAME_OBJECT_GRAPH_PLAYERTEST_DOWN);
		go->size /*		*/ = go->graphic.graph.spriteAnim[0].size;

		go->graphic.rot3d.x = 50;
		go->graphic.pos3d.z = -go->size.y / 2;


		{ //ステータス
			GAME_OBJECT_STATUS* s = &go->status;
			s->hpMax = PARAM_GAME_OBJECT_TEST_STATUS_HP;
			s->hp = s->hpMax;
			s->guardType = GAME_OBJECT_GUARD_TYPE_NORMAL;
		}

		{ //イベント
			GAME_OBJECT_EVENTS* e = &go->events;
		}

		{ //当たり判定
			GAME_OBJECT_COLLISION* c = &go->collision;

			//攻撃判定
			c->atk.enable = false;

			//くらい判定
			c->rec.enable = true;
			{
				GAME_OBJECT_COLLISION_RECIEVE_CHILD* cc = c->rec.collision + 0;
				cc->enable = true;
				cc->pos = MkF3(0.0f, 0.0f, 0.0f);
				cc->size.x = PARAM_GAME_OBJECT_TEST_COL_REC_SIZE_X;
				cc->size.y = PARAM_GAME_OBJECT_TEST_COL_REC_SIZE_Y;
				cc->size.z = PARAM_GAME_OBJECT_TEST_COL_REC_SIZE_Z;
			}
		}
	}
	{
		GAME_OBJECT* go = gameObjectPrefab + GAME_OBJECT_TYPE_PLAYERTEST_LEFT;
		(*go) = gameObjectPrefab[GAME_OBJECT_TYPE_PLAYERTEST_DOWN];

		go->graphic = GetGameObjectGraph(GAME_OBJECT_GRAPH_PLAYERTEST_LEFT);
		go->graphic.rot3d.x = 50;
		go->graphic.pos3d.z = -go->size.y / 2;
	}

	//--------------------------------------------------
	// エフェクト
	//--------------------------------------------------
	{
		GAME_OBJECT* go = gameObjectPrefab + GAME_OBJECT_TYPE_EFFECT;

		GameObjectSetLifeTime(go, 60);

		go->family[GAME_OBJECT_FAMILY_MISC] = true;

		go->zIndex /*	*/ = GAME_OBJECT_ZINDEX_EFFECT;

		go->size /*		*/ = MkF2(PARAM_GAME_OBJECT_GENERAL_SIZE_X, PARAM_GAME_OBJECT_GENERAL_SIZE_Y);
		go->blockCollisionIgnore = true;
		go->alwaysUpdate = true;

		go->friction /*	*/ = 0.0f;
		go->gravity /*	*/ = 0.0f;

		go->graphic = {};
		go->graphic.hidden = true;

		{ //ステータス
			GAME_OBJECT_STATUS* s = &go->status;
			s->hpMax = PARAM_GAME_OBJECT_GENERAL_STATUS_HP;
			s->hp = s->hpMax;
			s->guardType = GAME_OBJECT_GUARD_TYPE_INV;
		}

		{ //イベント
			GAME_OBJECT_EVENTS* e = &go->events;
			(*e) = eventGeneral;
		}
	}

    //--------------------------------------------------
    // ダミー
    //--------------------------------------------------
    {
        GAME_OBJECT* go = gameObjectPrefab + GAME_OBJECT_TYPE_DUMMY;

        go->family[GAME_OBJECT_FAMILY_MISC] = true;

        go->zIndex /*	*/ = GAME_OBJECT_ZINDEX_EFFECT;

        go->size /*		*/ = MkF2(PARAM_GAME_OBJECT_GENERAL_SIZE_X, PARAM_GAME_OBJECT_GENERAL_SIZE_Y);
        go->blockCollisionIgnore = true;
        go->alwaysUpdate = true;

        go->friction /*	*/ = 0.0f;
        go->gravity /*	*/ = 0.0f;

        go->graphic = {};
        go->graphic.hidden = true;

        { //ステータス
            GAME_OBJECT_STATUS* s = &go->status;
            s->hpMax = PARAM_GAME_OBJECT_GENERAL_STATUS_HP;
            s->hp = s->hpMax;
            s->guardType = GAME_OBJECT_GUARD_TYPE_INV;
        }

        { //イベント
            GAME_OBJECT_EVENTS* e = &go->events;
        }
    }

	for (int i = 0; i < GAME_OBJECT_TYPE_MAX; i++)
	{
		gameObjectPrefab[i].use = true;
		gameObjectPrefab[i].type = (GAME_OBJECT_TYPE)i;
	}
}

// ===================================================
// 終了処理
// ===================================================
void UninitGameObjectPrefab(void)
{
}



GAME_OBJECT GetGameObjectPrefab(GAME_OBJECT_TYPE type)
{
	return gameObjectPrefab[type];
}