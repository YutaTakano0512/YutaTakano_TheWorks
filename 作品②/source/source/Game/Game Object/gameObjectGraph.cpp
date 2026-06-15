// ===================================================
// gameObjectGraph.cpp ゲームオブジェクトのグラフィック
// 
// hara sougo  2024/07/22
// ===================================================
#include "../../game_main.h"
#include "../Sprite/spriteAnim.h"
#include "../Sprite/spriteGraph.h"
#include "gameObject.h"
#include "gameObjectGraph.h"
#include "../../Game/Entities/Block/block.h"

static GAME_OBJECT_GRAPHIC gameObjectGraph[GAME_OBJECT_GRAPH_MAX] = {};

//メモリ効率が悪い
//static SPRITE_GRAPH_DAT sg[GAME_OBJECT_GRAPH_MAX] = {};

// ===================================================
// 初期化
// ===================================================
void InitGameObjectGraph(void)
{
	//一時的に書き込む領域
	SPRITE_GRAPH_DAT* sg = (SPRITE_GRAPH_DAT*)malloc(sizeof(SPRITE_GRAPH_DAT) * GAME_OBJECT_GRAPH_MAX);
	if (sg == nullptr)
	{
#ifdef SWITCH_MODE
#else
		MessageBox(NULL, "メモリが確保できなかった", "エラー", MB_OK);
#endif
		return;
	}

	for (int i = 0; i < GAME_OBJECT_GRAPH_MAX; i++)
	{
		InitSpriteGraph(sg + i);
	}

	
	//--------------------------------------------------
	// 無
	//--------------------------------------------------
	{
		SPRITE_GRAPH_DAT* sgdat = sg + GAME_OBJECT_GRAPH_VOID;
		{ //胴体
			SPRITE_ANIM_DAT* sadat = sgdat->spriteAnim + 0;
			sadat->isUse = true;
			sadat->texDat = GetTexData(TEX_TYPE_VOID);
			sadat->xcnt = 1;
			sadat->hcnt = 1;
			sadat->delay = 1;

			sadat->pos = MkF2( 0.0f, 0.0f );
			sadat->rot = 0.0f;
			sadat->size = MkF2( 1.0f, 1.0f );
		}
	}

	//--------------------------------------------------
	// プレイヤー
	//--------------------------------------------------
	{
		SPRITE_GRAPH_DAT* sgdat = sg + GAME_OBJECT_GRAPH_PLAYER;
		//sgdat->outlineWidth = 6.0f;

		Float2 sizetmp = MkF2(100.0f, 100.0f );
		{ //
			SPRITE_ANIM_DAT* sadat = sgdat->spriteAnim + 0;
			sadat->isUse = true;
			sadat->texDat = GetTexData(TEX_TYPE_EFFECT_FLASH_RING);

			sadat->pos = MkF2(0.0f, 0.0f);
			sadat->rot = 0.0f;
			sadat->size = MkF2(sizetmp.x, sizetmp.y);
		}
		{ //
			SPRITE_ANIM_DAT* sadat = sgdat->spriteAnim + 1;
			sadat->isUse = true;
			sadat->texDat = GetTexData(TEX_TYPE_EFFECT_FLASH_RING);

			sadat->pos = MkF2(70.0f, 0.0f);
			sadat->rot = 0.0f;
			sadat->size = MkF2(sizetmp.x * 0.3f, sizetmp.y * 0.3f);
		}
	}



	//--------------------------------------------------
	// 障害物
	//--------------------------------------------------
	SPRITE_GRAPH_DAT sgdatStageObjectTmp = {};
	InitSpriteGraph(&sgdatStageObjectTmp);
	{
		SPRITE_GRAPH_DAT* sgdat = &sgdatStageObjectTmp;
		sgdat->outlineWidth = 0.0f;
		{ //
			SPRITE_ANIM_DAT* sadat = sgdat->spriteAnim + 0;
			sadat->isUse = true;
			//sadat->texDat = {};
			sadat->texDat = GetTexData(TEX_TYPE_OBJECT_ISEKI_ZANGAI_0);
			sadat->xcnt = 1;
			sadat->hcnt = 1;
			sadat->delay = 1;

			sadat->pos = MkF2(0.0f, 0.0f);
			sadat->rot = 0.0f;
			//sadat->size = MkF2(96.0f, 96.0f);
			sadat->size = MkF2(1200.0f, 1200.0f);
			//sadat->size = MkF2(960.0f, 540.0f);
		}
	}
	//--------------------------------------------------
	// 残骸
	{
		SPRITE_GRAPH_DAT* sgdat = sg + GAME_OBJECT_GRAPH_ISEKI_ZANGAI_0;
		(*sgdat) = sgdatStageObjectTmp;
		{ //
			SPRITE_ANIM_DAT* sadat = sgdat->spriteAnim + 0;
			sadat->texDat = GetTexData(TEX_TYPE_OBJECT_ISEKI_ZANGAI_0);
		}
	}
	//--------------------------------------------------
	// 残骸
	{
		SPRITE_GRAPH_DAT* sgdat = sg + GAME_OBJECT_GRAPH_ISEKI_ZANGAI_1;
		(*sgdat) = sgdatStageObjectTmp;
		{ //
			SPRITE_ANIM_DAT* sadat = sgdat->spriteAnim + 0;
			sadat->texDat = GetTexData(TEX_TYPE_OBJECT_ISEKI_ZANGAI_1);
		}
	}
	//--------------------------------------------------
	// 残骸
	{
		SPRITE_GRAPH_DAT* sgdat = sg + GAME_OBJECT_GRAPH_ISEKI_ZANGAI_2;
		(*sgdat) = sgdatStageObjectTmp;
		{ //
			SPRITE_ANIM_DAT* sadat = sgdat->spriteAnim + 0;
			sadat->texDat = GetTexData(TEX_TYPE_OBJECT_ISEKI_ZANGAI_2);
		}
	}
	//--------------------------------------------------
	// 台座
	{
		SPRITE_GRAPH_DAT* sgdat = sg + GAME_OBJECT_GRAPH_ISEKI_DAIZA_0;
		(*sgdat) = sgdatStageObjectTmp;
		{ //
			SPRITE_ANIM_DAT* sadat = sgdat->spriteAnim + 0;
			sadat->texDat = GetTexData(TEX_TYPE_OBJECT_ISEKI_DAIZA_0);
		}
	}
	//--------------------------------------------------
	// 柱
	{
		SPRITE_GRAPH_DAT* sgdat = sg + GAME_OBJECT_GRAPH_ISEKI_HASHIRA_0;
		(*sgdat) = sgdatStageObjectTmp;
		{ //
			SPRITE_ANIM_DAT* sadat = sgdat->spriteAnim + 0;
			sadat->texDat = GetTexData(TEX_TYPE_OBJECT_ISEKI_HASHIRA_0);
		}
	}
	//--------------------------------------------------
	// 柱
	{
		SPRITE_GRAPH_DAT* sgdat = sg + GAME_OBJECT_GRAPH_ISEKI_HASHIRA_TAORETA_0;
		(*sgdat) = sgdatStageObjectTmp;
		{ //
			SPRITE_ANIM_DAT* sadat = sgdat->spriteAnim + 0;
			sadat->texDat = GetTexData(TEX_TYPE_OBJECT_ISEKI_HASHIRA_TAORETA_0);
		}
	}
	//--------------------------------------------------
	// 感圧版
	{
		SPRITE_GRAPH_DAT* sgdat = sg + GAME_OBJECT_GRAPH_ISEKI_KANATSUBAN_0;
		(*sgdat) = sgdatStageObjectTmp;
		{ //
			SPRITE_ANIM_DAT* sadat = sgdat->spriteAnim + 0;
			sadat->texDat = GetTexData(TEX_TYPE_OBJECT_ISEKI_KANATSUBAN_0);
		}
	}



	//--------------------------------------------------
	// テストオブジェクト
	//--------------------------------------------------
	{
		SPRITE_GRAPH_DAT* sgdat = sg + GAME_OBJECT_GRAPH_TEST;
		sgdat->outlineWidth = 12.0f;
		{ //胴体
			SPRITE_ANIM_DAT* sadat = sgdat->spriteAnim + 0;
			sadat->isUse = true;
			sadat->texDat = GetTexData(TEX_TYPE_EFFECT_FLASH_RING_ENEM);
			sadat->xcnt = 1;
			sadat->hcnt = 1;
			sadat->delay = 8;

			sadat->pos = MkF2(0.0f, 0.0f);
			sadat->rot = 45.0f;
			sadat->size = MkF2(80.0f, 80.0f);
		}
		//{ //
		//	SPRITE_ANIM_DAT* sadat = sgdat->spriteAnim + 1;
		//	sadat->isUse = true;
		//	sadat->texDat = GetTexData(TEX_TYPE_EFFECT_SPARK);
		//	sadat->xcnt = 1;
		//	sadat->hcnt = 1;
		//	sadat->delay = 8;

		//	sadat->pos = MkF2(200.0f, 0.0f );
		//	sadat->rot = 45.0f;
		//	sadat->size = MkF2(80.0f, 80.0f);
		//}
	}

	//--------------------------------------------------
	// テストプレイヤーオブジェクト
	//--------------------------------------------------
	{
		SPRITE_GRAPH_DAT* sgdat = sg + GAME_OBJECT_GRAPH_PLAYERTEST_DOWN;
		sgdat->outlineWidth = 10.0f;
		{ //
			SPRITE_ANIM_DAT* sadat = sgdat->spriteAnim + 0;
			sadat->isUse = true;
			sadat->texDat = GetTexData(TEX_TYPE_PLAYERTEST_DOWN);
			sadat->xcnt = 1;
			sadat->hcnt = 1;
			sadat->delay = 1;

			sadat->pos = MkF2(0.0f, 0.0f);
			sadat->rot = 0.0f;
			sadat->size = MkF2(960.0f, 540.0f);
		}
	}
	{
		SPRITE_GRAPH_DAT* sgdat = sg + GAME_OBJECT_GRAPH_PLAYERTEST_LEFT;
		sgdat->outlineWidth = 10.0f;
		{ //
			SPRITE_ANIM_DAT* sadat = sgdat->spriteAnim + 0;
			sadat->isUse = true;
			sadat->texDat = GetTexData(TEX_TYPE_PLAYERTEST_LEFT);
			sadat->xcnt = 1;
			sadat->hcnt = 1;
			sadat->delay = 1;

			sadat->pos = MkF2(0.0f, 0.0f);
			sadat->rot = 0.0f;
			sadat->size = MkF2(960.0f, 540.0f);
		}
	}


	//--------------------------------------------------
	// エフェクト
	//--------------------------------------------------
	{
		SPRITE_GRAPH_DAT* sgdat = sg + GAME_OBJECT_GRAPH_EFFECT_SHOCKWAVE;
		sgdat->outlineWidth = 0.0f;

		{
			SPRITE_ANIM_DAT* sadat = sgdat->spriteAnim + 0;
			sadat->isUse = true;
			sadat->texDat = GetTexData(TEX_TYPE_EFFECT_FLASH_RING);

			sadat->pos = MkF2(0.0f, 0.0f);
			sadat->rot = 0.0f;
			sadat->size = MkF2(200.0f, 200.0f);
		}
	}
	{
		SPRITE_GRAPH_DAT* sgdat = sg + GAME_OBJECT_GRAPH_EFFECT_FLASH_0;
		sgdat->outlineWidth = 0.0f;

		{
			SPRITE_ANIM_DAT* sadat = sgdat->spriteAnim + 0;
			sadat->isUse = true;
			sadat->texDat = GetTexData(TEX_TYPE_EFFECT_FLASH_GENERAL);

			sadat->pos = MkF2(0.0f, 0.0f);
			sadat->rot = 0.0f;
			sadat->size = MkF2(3.0f, 120.0f);
		}
		//for (int i = 1; i < SPRITE_GRAPH_ANIM_DAT_MAX; i++)
		//{
		//	SPRITE_ANIM_DAT* sadat = sgdat->spriteAnim + i;
		//	(*sadat) = sgdat->spriteAnim[0];
		//}

	}
	{
		SPRITE_GRAPH_DAT* sgdat = sg + GAME_OBJECT_GRAPH_EFFECT_TWINKLE;
		sgdat->outlineWidth = 0.0f;

		{
			SPRITE_ANIM_DAT* sadat = sgdat->spriteAnim + 0;
			sadat->isUse = true;
			sadat->texDat = GetTexData(TEX_TYPE_EFFECT_TWINKLE);

			sadat->pos = MkF2(0.0f, 0.0f);
			sadat->rot = 0.0f;
			sadat->size = MkF2(200.0f, 200.0f);
		}
	}
	{
		SPRITE_GRAPH_DAT* sgdat = sg + GAME_OBJECT_GRAPH_EFFECT_SPARK;
		sgdat->outlineWidth = 0.0f;

		{
			SPRITE_ANIM_DAT* sadat = sgdat->spriteAnim + 0;
			sadat->isUse = true;
			sadat->texDat = GetTexData(TEX_TYPE_EFFECT_SPARK);

			sadat->pos = MkF2(0.0f, 0.0f);
			sadat->rot = 0.0f;
			sadat->size = MkF2(200.0f, 200.0f);
		}
	}

    {
        SPRITE_GRAPH_DAT* sgdat = sg + GAME_OBJECT_GRAPH_EFFECT_SPARK_NEW;
        sgdat->outlineWidth = 0.0f;

        {
            SPRITE_ANIM_DAT* sadat = sgdat->spriteAnim + 0;
            sadat->isUse = true;
            sadat->texDat = LoadTexData("TEX/effect/spark/anim_th_01");

            sadat->pos = MkF2(0.0f, 0.0f);
            sadat->rot = 0.0f;
            sadat->size = MkF2(200.0f, 200.0f);
            sadat->anim = true;
            sadat->xcnt = 4;
            sadat->hcnt = 4;
            sadat->cntMax = sadat->xcnt * sadat->hcnt;
            sadat->delay = 1;
        }
    }



	//セット
	for (int i = 0; i < GAME_OBJECT_GRAPH_MAX; i++)
	{
		InitGameObjectGraphic(gameObjectGraph + i);
		gameObjectGraph[i].graph = sg[i];
	}

	free(sg);
}

// ===================================================
// 終了処理
// ===================================================
void UninitGameObjectGraph(void)
{
	for (int i = 0; i < GAME_OBJECT_GRAPH_MAX; i++)
	{
		UninitSpriteGraph(&gameObjectGraph[i].graph);
	}
}


GAME_OBJECT_GRAPHIC GetGameObjectGraph(GAME_OBJECT_GRAPH graph)
{
	return gameObjectGraph[graph];
}