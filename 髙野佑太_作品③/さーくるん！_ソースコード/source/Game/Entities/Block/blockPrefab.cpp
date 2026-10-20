// ===================================================
// blockPrefab.cpp ゲームオブジェクトの設計図
// 
// hara sougo  2024/08/17
// ===================================================
#include "../../../game_main.h"
#include "../../../Game/Configuration/Param.h"
#include "block.h"
#include "blockPrefab.h"

static BLOCK blockPrefab[BLOCK_TYPE_MAX] = {};

// ===================================================
// 初期化
// ===================================================
void InitBlockPrefab(void)
{
	for (int i = 0; i < BLOCK_TYPE_MAX; i++)
	{
		InitBlock(blockPrefab + i);

		{
			SPRITE_ANIM_DAT* sadat = &blockPrefab[i].sprite;
			sadat->isUse = true;
			sadat->texDat = GetTexData(TEX_TYPE_BLOCK_TEST);
			sadat->xcnt = 6;
			sadat->hcnt = 6;
			sadat->cntMax = 1;
			sadat->no = 0;
			sadat->anim = false;
			sadat->delay = 1;

			sadat->pos = MkF2(0.0f, 0.0f);
			sadat->rot = 0.0f;
			sadat->size = blockPrefab[i].size;

			sadat->hide = false;
		}
	}

	//--------------------------------------------------
	// 無
	//--------------------------------------------------
	{
		BLOCK* b = blockPrefab + BLOCK_TYPE_VOID;
	}

	//--------------------------------------------------
	// TESTブロック
	//--------------------------------------------------
	for (int i = 0; i < BLOCK_TYPE_10 - BLOCK_TYPE_01 + 1; i++) //1 ~ 10
	{
		BLOCK* b = blockPrefab + BLOCK_TYPE_01 + i;

		{
			SPRITE_ANIM_DAT* sadat = &b->sprite;
			sadat->no = i;
		}
	}
	//--------------------------------------------------
	// 汎用ブロック
	//--------------------------------------------------
	{
		BLOCK* b = blockPrefab + BLOCK_TYPE_GENERAL_0;

		{
			SPRITE_ANIM_DAT* sadat = &b->sprite;
			sadat->texDat = GetTexData(TEX_TYPE_BLOCK_GENERAL);
			sadat->no = 0;
		}
	}
	{
		BLOCK* b = blockPrefab + BLOCK_TYPE_GENERAL_1;

		{
			SPRITE_ANIM_DAT* sadat = &b->sprite;
			sadat->texDat = GetTexData(TEX_TYPE_BLOCK_GENERAL);
			sadat->no = 1;
		}
	}

	//--------------------------------------------------
	// 判定のみ
	//--------------------------------------------------
	{
		BLOCK* b = blockPrefab + BLOCK_TYPE_DUMMY;
		{
			SPRITE_ANIM_DAT* sadat = &b->sprite;
			sadat->isUse = false;
			sadat->hide = true;
		}
	}
	//--------------------------------------------------
	// オブジェクト
	//--------------------------------------------------
	BLOCK BlockStageObjectTmp = {}; //テンプレを作る
	
	{
		//InitBlock(&BlockStageObjectTmp);
		BlockStageObjectTmp = blockPrefab[BLOCK_TYPE_VOID];

		BLOCK* b = &BlockStageObjectTmp;

		{
			SPRITE_ANIM_DAT* sadat = &b->sprite;
			sadat->isUse = true;
			//sadat->texDat = {};
			sadat->texDat = GetTexData(TEX_TYPE_OBJECT_ISEKI_ZANGAI_0);
			sadat->xcnt = 1;
			sadat->hcnt = 1;
			sadat->delay = 1;

			sadat->pos = MkF2(0.0f, 0.0f);
			sadat->rot = 0.0f;
			//sadat->size = MkF2(96.0f, 96.0f);
            //sadat->size = MkF2(1200.0f, 1200.0f);
            sadat->size = MkF2(500.0f, 500.0f);
			//sadat->size = MkF2(960.0f, 540.0f);
		}
	}
	//α版
	{
		{
			BLOCK* b = blockPrefab + BLOCK_TYPE_ISEKI_ZANGAI_0;
			(*b) = BlockStageObjectTmp; //テンプレをコピー
			b->size = MkF2(
				BLOCK_WIDTH * 7,
				BLOCK_HIGHT * 2
			);
			{
				SPRITE_ANIM_DAT* sadat = &b->sprite;

				sadat->pos3D = MkF3(0.0f, BLOCK_HIGHT * 0.5f,
					-BLOCK_HIGHT * 1.5f);
				sadat->rot3D = MkF3(40.0f, 0.0f, 0.0f);

				sadat->texDat = GetTexData(TEX_TYPE_OBJECT_ISEKI_ZANGAI_0);
			}
		}

		{
			BLOCK* b = blockPrefab + BLOCK_TYPE_ISEKI_ZANGAI_1;
			(*b) = BlockStageObjectTmp; //テンプレをコピー
			b->noCollision = true;
			{
				SPRITE_ANIM_DAT* sadat = &b->sprite;

				sadat->pos3D = MkF3(0.0f, BLOCK_HIGHT * 0.0f,
					-BLOCK_HIGHT * 0.1f);
				sadat->rot3D = MkF3(5.0f, 0.0f, 0.0f);

				sadat->texDat = GetTexData(TEX_TYPE_OBJECT_ISEKI_ZANGAI_1);
			}
		}

		{
			BLOCK* b = blockPrefab + BLOCK_TYPE_ISEKI_ZANGAI_2;
			(*b) = BlockStageObjectTmp; //テンプレをコピー
			b->size = MkF2(
				BLOCK_WIDTH * 11,
				BLOCK_HIGHT * 1
			);
			{
				SPRITE_ANIM_DAT* sadat = &b->sprite;

				sadat->pos3D = MkF3(0.0f, BLOCK_HIGHT * 0.5f,
					-BLOCK_HIGHT * 1.5f);
				sadat->rot3D = MkF3(40.0f, 0.0f, 0.0f);

				sadat->texDat = GetTexData(TEX_TYPE_OBJECT_ISEKI_ZANGAI_2);
			}
		}

		{
			BLOCK* b = blockPrefab + BLOCK_TYPE_ISEKI_DAIZA_0;
			(*b) = BlockStageObjectTmp; //テンプレをコピー
			b->size = MkF2(
				BLOCK_WIDTH * 5,
				BLOCK_HIGHT * 5
			);
			{
				SPRITE_ANIM_DAT* sadat = &b->sprite;

				sadat->pos3D = MkF3(0.0f, BLOCK_HIGHT * 0.5f,
					-BLOCK_HIGHT * 1.5f);
				sadat->rot3D = MkF3(16.0f, 0.0f, 0.0f);

				sadat->texDat = GetTexData(TEX_TYPE_OBJECT_ISEKI_DAIZA_0);
			}
		}

		{
			BLOCK* b = blockPrefab + BLOCK_TYPE_ISEKI_HASHIRA_0;
			(*b) = BlockStageObjectTmp; //テンプレをコピー
			b->size = MkF2(
				BLOCK_WIDTH * 3,
				BLOCK_HIGHT * 2
			);
			{
				SPRITE_ANIM_DAT* sadat = &b->sprite;

				//sadat->pos3D = MkF3(0.0f, BLOCK_HIGHT * 0.5f, -BLOCK_HIGHT * 4.5f);
				sadat->pos3D = MkF3(0.0f, -BLOCK_HIGHT * 5.0f, -BLOCK_HIGHT * 7.0f);

				sadat->rot3D = MkF3(50.0f, 0.0f, 0.0f);

				sadat->size.x *= 1.5f;
				sadat->size.y *= 1.5f;

				sadat->texDat = GetTexData(TEX_TYPE_OBJECT_ISEKI_HASHIRA_0);
			}
		}

		{
			BLOCK* b = blockPrefab + BLOCK_TYPE_ISEKI_HASHIRA_TAORETA_0;
			(*b) = BlockStageObjectTmp; //テンプレをコピー
			b->size = MkF2(
				BLOCK_WIDTH * 12,
				BLOCK_HIGHT * 1
			);
			{
				SPRITE_ANIM_DAT* sadat = &b->sprite;

				sadat->pos3D = MkF3(0.0f, BLOCK_HIGHT * -0.5f,
					-BLOCK_HIGHT * 1.8f);
				sadat->rot3D = MkF3(30.0f, 0.0f, 0.0f);

				sadat->size.x *= 1.5f;
				sadat->size.y *= 1.5f;

				sadat->texDat = GetTexData(TEX_TYPE_OBJECT_ISEKI_HASHIRA_TAORETA_0);
			}
		}

		{
			BLOCK* b = blockPrefab + BLOCK_TYPE_ISEKI_HASHIRA_TAORETA_1;
			(*b) = BlockStageObjectTmp; //テンプレをコピー
			b->size = MkF2(
				BLOCK_WIDTH * 2,
				BLOCK_HIGHT * 13
			);
			{
				SPRITE_ANIM_DAT* sadat = &b->sprite;

				sadat->pos3D = MkF3(BLOCK_WIDTH * 1.0f, -BLOCK_HIGHT * 0.0f,
					-BLOCK_HIGHT * 1.5f);
				sadat->rot3D = MkF3(0.0f, -5.0f, 96.0f);

				sadat->size.x *= 1.5f;
				sadat->size.y *= 1.5f;

				sadat->texDat = GetTexData(TEX_TYPE_OBJECT_ISEKI_HASHIRA_TAORETA_0);
			}
		}

		{
			BLOCK* b = blockPrefab + BLOCK_TYPE_ISEKI_KANATSUBAN_0;
			(*b) = BlockStageObjectTmp; //テンプレをコピー
			b->noCollision = true;
			b->size = MkF2(
				BLOCK_WIDTH * 1,
				BLOCK_HIGHT * 1
			);
			{
				SPRITE_ANIM_DAT* sadat = &b->sprite;

				sadat->pos3D = MkF3(0.0f, BLOCK_HIGHT * 0.0f,
					-BLOCK_HIGHT * 0.2f);
				sadat->rot3D = MkF3(1.0f, 0.0f, 0.0f);

				sadat->size.x *= 0.4f;
				sadat->size.y *= 0.4f;

				sadat->texDat = GetTexData(TEX_TYPE_OBJECT_ISEKI_KANATSUBAN_0);
			}
		}
	}


    //β版
    //{
    //    {
    //        BLOCK* b = blockPrefab + BLOCK_TYPE_ISEKI_OBJECT_01;
    //        (*b) = BlockStageObjectTmp; //テンプレをコピー
    //        b->noCollision = true;
    //        {
    //            SPRITE_ANIM_DAT* sadat = &b->sprite;

    //            sadat->pos3D = MkF3(0.0f, BLOCK_HIGHT * 0.0f,
    //                -BLOCK_HIGHT * 0.1f);
    //            sadat->rot3D = MkF3(5.0f, 0.0f, 0.0f);

    //            //sadat->texDat = GetTexData(TEX_TYPE_OBJECT_ISEKI_ZANGAI_1);
    //            //sadat->texDat = GetTexData("TEX/object/iseki/obj_image/object_01");
    //            sadat->texDat = LoadTexData("TEX/object/iseki/obj_image/object_01");
    //            sadat->size = MkF2(1200.0f, 1200.0f);
    //        }
    //    }
    //    {
    //        BLOCK* b = blockPrefab + BLOCK_TYPE_ISEKI_OBJECT_02;
    //        (*b) = BlockStageObjectTmp; //テンプレをコピー
    //        b->noCollision = true;
    //        {
    //            SPRITE_ANIM_DAT* sadat = &b->sprite;

    //            sadat->pos3D = MkF3(0.0f, BLOCK_HIGHT * 0.0f,
    //                -BLOCK_HIGHT * 0.1f);
    //            sadat->rot3D = MkF3(5.0f, 0.0f, 0.0f);

    //            //sadat->texDat = GetTexData(TEX_TYPE_OBJECT_ISEKI_ZANGAI_1);
    //            sadat->texDat = LoadTexData("TEX/object/iseki/obj_image/object_02");
    //            sadat->size = MkF2(1200.0f, 1200.0f);
    //        }
    //    }
    //    {
    //        BLOCK* b = blockPrefab + BLOCK_TYPE_ISEKI_OBJECT_03;
    //        (*b) = BlockStageObjectTmp; //テンプレをコピー
    //        b->noCollision = true;
    //        {
    //            SPRITE_ANIM_DAT* sadat = &b->sprite;

    //            sadat->pos3D = MkF3(0.0f, BLOCK_HIGHT * 0.0f,
    //                -BLOCK_HIGHT * 0.1f);
    //            sadat->rot3D = MkF3(5.0f, 0.0f, 0.0f);

    //            //sadat->texDat = GetTexData(TEX_TYPE_OBJECT_ISEKI_ZANGAI_1);
    //            //sadat->texDat = LoadTexData("TEX/object/iseki/obj_image/object_03");
    //            sadat->texDat = LoadTexData("TEX/object/iseki/obj_image/Block");
    //        }
    //    }

    //    {
    //        BLOCK* b = blockPrefab + BLOCK_TYPE_ISEKI_OBJECT_04;
    //        (*b) = BlockStageObjectTmp; //テンプレをコピー
    //        b->size = MkF2(
    //            BLOCK_WIDTH * 11,
    //            BLOCK_HIGHT * 1
    //        );
    //        {
    //            SPRITE_ANIM_DAT* sadat = &b->sprite;

    //            sadat->pos3D = MkF3(BLOCK_WIDTH * 1.0f, BLOCK_HIGHT * 0.5f,
    //                -BLOCK_HIGHT * 2.0f);
    //            sadat->rot3D = MkF3(40.0f, 0.0f, 0.0f);

    //            sadat->size.x *= 2.0f;
    //            sadat->size.y *= 2.0f;

    //            //sadat->texDat = GetTexData(TEX_TYPE_OBJECT_ISEKI_ZANGAI_2);
    //            //sadat->texDat = LoadTexData("TEX/object/iseki/obj_image/object_04");
    //            sadat->texDat = LoadTexData("TEX/object/iseki/obj_image/HasiraYoko2");
    //        }
    //    }

    //    {
    //        BLOCK* b = blockPrefab + BLOCK_TYPE_ISEKI_OBJECT_05;
    //        (*b) = BlockStageObjectTmp; //テンプレをコピー
    //        b->size = MkF2(
    //            BLOCK_WIDTH * 2,
    //            BLOCK_HIGHT * 8
    //        );
    //        {
    //            SPRITE_ANIM_DAT* sadat = &b->sprite;

    //            sadat->pos3D = MkF3(BLOCK_WIDTH * 1.0f, -BLOCK_HIGHT * 1.0f,
    //                -BLOCK_HIGHT * 2.5f);
    //            sadat->rot3D = MkF3(20.0f, 0.0f, 0.0f);

    //            sadat->size.x *= 2.0f;
    //            sadat->size.y *= 2.0f;

    //            //sadat->texDat = GetTexData(TEX_TYPE_OBJECT_ISEKI_HASHIRA_TAORETA_0);
    //            //sadat->texDat = LoadTexData("TEX/object/iseki/obj_image/object_05");
    //            sadat->texDat = LoadTexData("TEX/object/iseki/obj_image/HasiraS1");
    //        }
    //    }

    //    {
    //        BLOCK* b = blockPrefab + BLOCK_TYPE_ISEKI_OBJECT_06;
    //        (*b) = BlockStageObjectTmp; //テンプレをコピー
    //        b->size = MkF2(
    //            BLOCK_WIDTH * 3,
    //            BLOCK_HIGHT * 2
    //        );
    //        {
    //            SPRITE_ANIM_DAT* sadat = &b->sprite;

    //            //sadat->pos3D = MkF3(0.0f, BLOCK_HIGHT * 0.5f, -BLOCK_HIGHT * 4.5f);
    //            sadat->pos3D = MkF3(BLOCK_WIDTH * 0.5f, -BLOCK_HIGHT * 4.0f, -BLOCK_HIGHT * 5.0f);

    //            sadat->rot3D = MkF3(40.0f, 0.0f, 0.0f);

    //            sadat->size.x *= 3.0f;
    //            sadat->size.y *= 3.0f;

    //            //sadat->texDat = GetTexData(TEX_TYPE_OBJECT_ISEKI_HASHIRA_0);
    //            //sadat->texDat = LoadTexData("TEX/object/iseki/obj_image/object_06");
    //            sadat->texDat = LoadTexData("TEX/object/iseki/obj_image/Hasira3");
    //        }
    //    }
    //    {
    //        BLOCK* b = blockPrefab + BLOCK_TYPE_ISEKI_OBJECT_07;
    //        (*b) = BlockStageObjectTmp; //テンプレをコピー
    //        b->size = MkF2(
    //            BLOCK_WIDTH * 12,
    //            BLOCK_HIGHT * 1
    //        );
    //        {
    //            SPRITE_ANIM_DAT* sadat = &b->sprite;

    //            sadat->pos3D = MkF3(BLOCK_WIDTH * 2.0f, -BLOCK_HIGHT * 1.0f,
    //                -BLOCK_HIGHT * 3.0f);
    //            sadat->rot3D = MkF3(40.0f, 0.0f, 0.0f);

    //            sadat->size.x *= 3.0f;
    //            sadat->size.y *= 3.0f;

    //            //sadat->texDat = GetTexData(TEX_TYPE_OBJECT_ISEKI_ZANGAI_0);
    //            //sadat->texDat = LoadTexData("TEX/object/iseki/obj_image/object_07");
    //            sadat->texDat = LoadTexData("TEX/object/iseki/obj_image/HasiraYoko1");
    //        }
    //    }
    //    {
    //        BLOCK* b = blockPrefab + BLOCK_TYPE_ISEKI_OBJECT_08;
    //        (*b) = BlockStageObjectTmp; //テンプレをコピー
    //        b->size = MkF2(
    //            BLOCK_WIDTH * 2,
    //            BLOCK_HIGHT * 9
    //        );
    //        {
    //            SPRITE_ANIM_DAT* sadat = &b->sprite;

    //            sadat->pos3D = MkF3(BLOCK_WIDTH * 1.0f, -BLOCK_HIGHT * 3.0f,
    //                -BLOCK_HIGHT * 2.5f);
    //            sadat->rot3D = MkF3(10.0f, 0.0f, 0.0f);

    //            sadat->size.x *= 3.0f;
    //            sadat->size.y *= 3.0f;

    //            //sadat->texDat = GetTexData(TEX_TYPE_OBJECT_ISEKI_HASHIRA_TAORETA_0);
    //            //sadat->texDat = LoadTexData("TEX/object/iseki/obj_image/object_08");
    //            sadat->texDat = LoadTexData("TEX/object/iseki/obj_image/HasiraS4");
    //        }
    //    }
    //    {
    //        BLOCK* b = blockPrefab + BLOCK_TYPE_ISEKI_OBJECT_09;
    //        (*b) = BlockStageObjectTmp; //テンプレをコピー
    //        b->size = MkF2(
    //            BLOCK_WIDTH * 3,
    //            BLOCK_HIGHT * 2
    //        );
    //        {
    //            SPRITE_ANIM_DAT* sadat = &b->sprite;

    //            //sadat->pos3D = MkF3(0.0f, BLOCK_HIGHT * 0.5f, -BLOCK_HIGHT * 4.5f);
    //            sadat->pos3D = MkF3(BLOCK_WIDTH * 2.0f, -BLOCK_HIGHT * 4.0f, -BLOCK_HIGHT * 5.2f);

    //            sadat->rot3D = MkF3(40.0f, 0.0f, 0.0f);

    //            sadat->size.x *= 3.0f;
    //            sadat->size.y *= 3.0f;

    //            //sadat->texDat = GetTexData(TEX_TYPE_OBJECT_ISEKI_HASHIRA_0);
    //            //sadat->texDat = LoadTexData("TEX/object/iseki/obj_image/object_09");
    //            sadat->texDat = LoadTexData("TEX/object/iseki/obj_image/HasiraS1");
    //        }
    //    }
    //    {
    //        BLOCK* b = blockPrefab + BLOCK_TYPE_ISEKI_OBJECT_10;
    //        (*b) = BlockStageObjectTmp; //テンプレをコピー
    //        b->size = MkF2(
    //            BLOCK_WIDTH * 5,
    //            BLOCK_HIGHT * 4
    //        );
    //        {
    //            SPRITE_ANIM_DAT* sadat = &b->sprite;

    //            sadat->pos3D = MkF3(BLOCK_WIDTH * 0.5f, BLOCK_HIGHT * 0.5f,
    //                -BLOCK_HIGHT * 3.0f);
    //            sadat->rot3D = MkF3(35.0f, 0.0f, 0.0f);

    //            sadat->size.x *= 2.0f;
    //            sadat->size.y *= 2.0f;

    //            //sadat->texDat = GetTexData(TEX_TYPE_OBJECT_ISEKI_DAIZA_0);
    //            //sadat->texDat = LoadTexData("TEX/object/iseki/obj_image/object_10");
    //            sadat->texDat = LoadTexData("TEX/object/iseki/obj_image/Daiza");
    //        }
    //    }
    //    {
    //        BLOCK* b = blockPrefab + BLOCK_TYPE_ISEKI_OBJECT_Trap;
    //        (*b) = BlockStageObjectTmp; //テンプレをコピー
    //        b->noCollision = true;
    //        b->size = MkF2(
    //            BLOCK_WIDTH * 1,
    //            BLOCK_HIGHT * 1
    //        );
    //        {
    //            SPRITE_ANIM_DAT* sadat = &b->sprite;

    //            sadat->pos3D = MkF3(0.0f, BLOCK_HIGHT * 0.0f,
    //                -BLOCK_HIGHT * 0.5f);
    //            sadat->rot3D = MkF3(10.0f, 0.0f, 0.0f);

    //            sadat->size.x *= 1.0f;
    //            sadat->size.y *= 1.0f;

    //            //sadat->texDat = GetTexData(TEX_TYPE_OBJECT_ISEKI_KANATSUBAN_0);
    //            //sadat->texDat = LoadTexData("TEX/object/iseki/obj_image/trap");
    //            sadat->texDat = LoadTexData("TEX/object/iseki/obj_image/Kanatuban");
    //        }
    //    }
    //}

    //master
    {
        {
            BLOCK* b = blockPrefab + BLOCK_TYPE_ISEKI_0_Block;
            (*b) = BlockStageObjectTmp; //テンプレをコピー
            b->size = MkF2(
                BLOCK_WIDTH * 3,
                BLOCK_HIGHT * 1
            );
            b->sizeZ = BLOCK_HIGHT * 1;
            {
                SPRITE_ANIM_DAT* sadat = &b->sprite;

                sadat->pos3D = MkF3(0.0f, BLOCK_HIGHT * 0.0f,
                    -BLOCK_HIGHT * 0.74f
                );
                sadat->rot3D = MkF3(30.0f, 0.0f, 0.0f);

                sadat->texDat = LoadTexData("TEX/object/iseki/obj_image/Block");
            }
        }

        {
            BLOCK* b = blockPrefab + BLOCK_TYPE_ISEKI_0_Daiza;
            (*b) = BlockStageObjectTmp; //テンプレをコピー
            b->size = MkF2(
                BLOCK_WIDTH * 3,
                BLOCK_HIGHT * 1
            );
            b->sizeZ = BLOCK_HIGHT * 1.5f;
            {
                SPRITE_ANIM_DAT* sadat = &b->sprite;

                sadat->pos3D = MkF3(BLOCK_WIDTH * 0.0f, BLOCK_HIGHT * 0.0f,
                    -BLOCK_HIGHT * 1.5f);
                sadat->rot3D = MkF3(55.0f, 0.0f, 0.0f);

                sadat->texDat = LoadTexData("TEX/object/iseki/obj_image/Daiza");
            }
        }

        {
            BLOCK* b = blockPrefab + BLOCK_TYPE_ISEKI_0_Hasira1;
            (*b) = BlockStageObjectTmp; //テンプレをコピー
            b->size = MkF2(
                BLOCK_WIDTH * 3,
                BLOCK_HIGHT * 2
            );
            b->sizeZ = BLOCK_HIGHT * 10;
            {
                SPRITE_ANIM_DAT* sadat = &b->sprite;

                sadat->pos3D = MkF3(BLOCK_WIDTH * 0.0f, BLOCK_HIGHT * -2.0f,
                    -BLOCK_HIGHT * 2.5f * 3
                );
                sadat->rot3D = MkF3(70.0f, 0.0f, 0.0f);
                sadat->size.x *= 3.0f;
                sadat->size.y *= 3.0f;

                sadat->texDat = LoadTexData("TEX/object/iseki/obj_image/Hasira1");
            }
        }

        {
            BLOCK* b = blockPrefab + BLOCK_TYPE_ISEKI_0_Hasira2;
            (*b) = BlockStageObjectTmp; //テンプレをコピー
            b->size = MkF2(
                BLOCK_WIDTH * 3,
                BLOCK_HIGHT * 2
            );
            b->sizeZ = BLOCK_HIGHT * 10;
            {
                SPRITE_ANIM_DAT* sadat = &b->sprite;

                sadat->pos3D = MkF3(BLOCK_WIDTH * 0.0f, BLOCK_HIGHT * -2.0f,
                    -BLOCK_HIGHT * 2.5f * 3
                );
                sadat->rot3D = MkF3(70.0f, 0.0f, 0.0f);
                sadat->size.x *= 3.0f;
                sadat->size.y *= 3.0f;

                sadat->texDat = LoadTexData("TEX/object/iseki/obj_image/Hasira2");
            }
        }

        {
            BLOCK* b = blockPrefab + BLOCK_TYPE_ISEKI_0_Hasira3;
            (*b) = BlockStageObjectTmp; //テンプレをコピー
            b->size = MkF2(
                BLOCK_WIDTH * 3,
                BLOCK_HIGHT * 2
            );
            b->sizeZ = BLOCK_HIGHT * 10;
            {
                SPRITE_ANIM_DAT* sadat = &b->sprite;

                sadat->pos3D = MkF3(BLOCK_WIDTH * 0.0f, BLOCK_HIGHT * -2.0f,
                    -BLOCK_HIGHT * 2.5f * 3
                );
                sadat->size.x *= 3.0f;
                sadat->size.y *= 3.0f;

                sadat->rot3D = MkF3(70.0f, 0.0f, 0.0f);

                sadat->texDat = LoadTexData("TEX/object/iseki/obj_image/Hasira3");
            }
        }

        {
            BLOCK* b = blockPrefab + BLOCK_TYPE_ISEKI_0_HasiraS1;
            (*b) = BlockStageObjectTmp; //テンプレをコピー
            b->size = MkF2(
                BLOCK_WIDTH * 3,
                BLOCK_HIGHT * 2
            );
            b->sizeZ = BLOCK_HIGHT * 4;
            {
                SPRITE_ANIM_DAT* sadat = &b->sprite;

                sadat->pos3D = MkF3(BLOCK_WIDTH * 0.0f, BLOCK_HIGHT * -1.0f, -BLOCK_HIGHT * 3.5f);
                sadat->size.x *= 1.5f;
                sadat->size.y *= 1.5f;

                sadat->rot3D = MkF3(60.0f, 0.0f, 0.0f);

                sadat->texDat = LoadTexData("TEX/object/iseki/obj_image/HasiraS1");
            }
        }
        {
            BLOCK* b = blockPrefab + BLOCK_TYPE_ISEKI_0_HasiraS2;
            (*b) = BlockStageObjectTmp; //テンプレをコピー
            b->size = MkF2(
                BLOCK_WIDTH * 3,
                BLOCK_HIGHT * 2
            );
            b->sizeZ = BLOCK_HIGHT * 4;
            {
                SPRITE_ANIM_DAT* sadat = &b->sprite;

                sadat->pos3D = MkF3(BLOCK_WIDTH * 0.0f, BLOCK_HIGHT * -1.0f, -BLOCK_HIGHT * 3.4f);
                sadat->size.x *= 1.5f;
                sadat->size.y *= 1.5f;

                sadat->rot3D = MkF3(60.0f, 0.0f, 0.0f);

                sadat->texDat = LoadTexData("TEX/object/iseki/obj_image/HasiraS2");
            }
        }
        {
            BLOCK* b = blockPrefab + BLOCK_TYPE_ISEKI_0_HasiraS3;
            (*b) = BlockStageObjectTmp; //テンプレをコピー
            b->size = MkF2(
                BLOCK_WIDTH * 5,
                BLOCK_HIGHT * 2
            );
            b->sizeZ = BLOCK_HIGHT * 4;
            {
                SPRITE_ANIM_DAT* sadat = &b->sprite;

                sadat->pos3D = MkF3(BLOCK_WIDTH * 0.0f, BLOCK_HIGHT * -1.0f, -BLOCK_HIGHT * 1.7f);

                sadat->rot3D = MkF3(45.0f, 0.0f, 0.0f);
                sadat->size.x *= 1.5f;
                sadat->size.y *= 1.5f;

                sadat->texDat = LoadTexData("TEX/object/iseki/obj_image/HasiraS3");
            }
        }
        {
            BLOCK* b = blockPrefab + BLOCK_TYPE_ISEKI_0_HasiraS4;
            (*b) = BlockStageObjectTmp; //テンプレをコピー
            b->size = MkF2(
                BLOCK_WIDTH * 3,
                BLOCK_HIGHT * 2
            );
            b->sizeZ = BLOCK_HIGHT * 3;
            {
                SPRITE_ANIM_DAT* sadat = &b->sprite;

                sadat->pos3D = MkF3(BLOCK_WIDTH * 0.0f, BLOCK_HIGHT * -1.0f, -BLOCK_HIGHT * 1.9f);

                sadat->rot3D = MkF3(30.0f, 0.0f, 0.0f);
                sadat->size.x *= 1.5f;
                sadat->size.y *= 1.5f;

                sadat->texDat = LoadTexData("TEX/object/iseki/obj_image/HasiraS4");
            }
        }
        {
            BLOCK* b = blockPrefab + BLOCK_TYPE_ISEKI_0_HasiraYoko1;
            (*b) = BlockStageObjectTmp; //テンプレをコピー
            b->size = MkF2(
                BLOCK_WIDTH * 8,
                BLOCK_HIGHT * 1
            );
            b->sizeZ = BLOCK_HIGHT * 1.5f;
            {
                SPRITE_ANIM_DAT* sadat = &b->sprite;

                sadat->pos3D = MkF3(BLOCK_WIDTH * 0.0f, BLOCK_HIGHT * -1.0f,
                    -BLOCK_HIGHT * 1.4f);
                sadat->rot3D = MkF3(60.0f, 0.0f, 0.0f);
                sadat->size.x *= 1.75f;
                sadat->size.y *= 1.75f;

                sadat->texDat = LoadTexData("TEX/object/iseki/obj_image/HasiraYoko1");
            }
        }
        {
            BLOCK* b = blockPrefab + BLOCK_TYPE_ISEKI_0_HasiraYoko2;
            (*b) = BlockStageObjectTmp; //テンプレをコピー
            b->size = MkF2(
                BLOCK_WIDTH * 8,
                BLOCK_HIGHT * 1
            );
            b->sizeZ = BLOCK_HIGHT * 1.5f;
            {
                SPRITE_ANIM_DAT* sadat = &b->sprite;

                sadat->pos3D = MkF3(BLOCK_WIDTH * 0.0f, BLOCK_HIGHT * -1.0f,
                    -BLOCK_HIGHT * 1.4f);
                sadat->rot3D = MkF3(60.0f, 0.0f, 0.0f);
                sadat->size.x *= 1.75f;
                sadat->size.y *= 1.75f;

                sadat->texDat = LoadTexData("TEX/object/iseki/obj_image/HasiraYoko2");
            }
        }
        {
            BLOCK* b = blockPrefab + BLOCK_TYPE_ISEKI_0_Kanatuban;
            (*b) = BlockStageObjectTmp; //テンプレをコピー
            b->noCollision = true;
            b->size = MkF2(
                BLOCK_WIDTH * 1,
                BLOCK_HIGHT * 1
            );
            b->sizeZ = BLOCK_HIGHT * 0.1;
            {
                SPRITE_ANIM_DAT* sadat = &b->sprite;

                sadat->pos3D = MkF3(0.0f, BLOCK_HIGHT * 0.0f,
                    -BLOCK_HIGHT * 0.6f);
                sadat->rot3D = MkF3(30.0f, 0.0f, 0.0f);

                sadat->size.x *= 1.0f;
                sadat->size.y *= 1.0f;

                sadat->texDat = LoadTexData("TEX/object/iseki/obj_image/Kanatuban");
            }
        }
        {
            BLOCK* b = blockPrefab + BLOCK_TYPE_ISEKI_0_Taimatsu;
            (*b) = BlockStageObjectTmp; //テンプレをコピー
            b->size = MkF2(
                BLOCK_WIDTH * 1,
                BLOCK_HIGHT * 1
            );
            b->sizeZ = BLOCK_HIGHT * 3;
            {
                SPRITE_ANIM_DAT* sadat = &b->sprite;

                sadat->pos3D = MkF3(BLOCK_WIDTH * 0.0f, BLOCK_HIGHT * 0.0f,
                    -BLOCK_HIGHT * 2.5f);
                sadat->rot3D = MkF3(60.0f, 0.0f, 0.0f);

                sadat->size.x *= 1.0f;
                sadat->size.y *= 1.0f;

                sadat->texDat = LoadTexData("TEX/object/iseki/obj_image/Taimatsu");
            }
        }

        for (int i = BLOCK_TYPE_ISEKI_0_Dark_Block; i <= BLOCK_TYPE_ISEKI_0_Dark_Taimatsu; i++)
        {
            int motoIndex = (int)(BLOCK_TYPE_ISEKI_0_Block + (i - BLOCK_TYPE_ISEKI_0_Dark_Block));
            blockPrefab[i] = blockPrefab[motoIndex];

            //blockPrefab[motoIndex].sprite.color = MkF4(
            //    1.00f * 1.5f,
            //    0.70f * 1.5f,
            //    0.52f * 1.5f,
            //    1.0f);
            //blockPrefab[i].sprite.color = MkF4(
            //    1.00f * 1.05f,
            //    0.58f * 1.05f,
            //    0.60f * 1.05f,
            //    1.0f);
            blockPrefab[motoIndex].sprite.color = MkF4(
                1.0f,
                1.0f,
                1.0f,
                1.0f);
            blockPrefab[i].sprite.color = MkF4(
                0.75f,
                0.75f,
                0.75f,
                1.0f);
        }
    }


    float haikeiSizeMod = 7.0f;
    float haikeiYPosMod = BLOCK_HIGHT * -8.0f;
    {
        BLOCK* b = blockPrefab + BLOCK_TYPE_ISEKI_HAIKEI_01;
        (*b) = BlockStageObjectTmp; //テンプレをコピー
        b->isBackground = true;
        b->noCollision = true;
        b->size = MkF2(
            BLOCK_WIDTH * 1,
            BLOCK_HIGHT * 1
        );
        {
            SPRITE_ANIM_DAT* sadat = &b->sprite;

            sadat->pos3D = MkF3(0.0f, 0.0f, BLOCK_HIGHT * 20.0f + haikeiYPosMod);
            sadat->rot3D = MkF3(80.0f, 0.0f, 0.0f);

            sadat->size.x = 999.0f * haikeiSizeMod;
            sadat->size.y = 1013.0f * haikeiSizeMod;

            sadat->color = MkF4(0.3f, 0.3f, 0.3f, 1.0f);

            sadat->texDat = LoadTexData("TEX/bg/iseki/haikei/haikei1");
        }
    }
    {
        BLOCK* b = blockPrefab + BLOCK_TYPE_ISEKI_HAIKEI_02;
        (*b) = BlockStageObjectTmp; //テンプレをコピー
        b->isBackground = true;
        b->noCollision = true;
        b->size = MkF2(
            BLOCK_WIDTH * 1,
            BLOCK_HIGHT * 1
        );
        {
            SPRITE_ANIM_DAT* sadat = &b->sprite;

            sadat->pos3D = MkF3(0.0f, 0.0f, BLOCK_HIGHT * 20.0f + haikeiYPosMod);
            sadat->rot3D = MkF3(80.0f, 0.0f, 0.0f);

            sadat->size.x = 252.0f * haikeiSizeMod;
            sadat->size.y = 705.0f * haikeiSizeMod;

            sadat->color = MkF4(0.3f, 0.3f, 0.3f, 1.0f);

            sadat->texDat = LoadTexData("TEX/bg/iseki/haikei/haikei2");
        }
    }
    {
        BLOCK* b = blockPrefab + BLOCK_TYPE_ISEKI_HAIKEI_03;
        (*b) = BlockStageObjectTmp; //テンプレをコピー
        b->isBackground = true;
        b->noCollision = true;
        b->size = MkF2(
            BLOCK_WIDTH * 1,
            BLOCK_HIGHT * 1
        );
        {
            SPRITE_ANIM_DAT* sadat = &b->sprite;

            sadat->pos3D = MkF3(0.0f, 0.0f, BLOCK_HIGHT * 20.0f + haikeiYPosMod);
            sadat->rot3D = MkF3(80.0f, 0.0f, 0.0f);

            sadat->size.x = 961.0f * haikeiSizeMod;
            sadat->size.y = 880.0f * haikeiSizeMod;

            sadat->color = MkF4(0.3f, 0.3f, 0.3f, 1.0f);

            sadat->texDat = LoadTexData("TEX/bg/iseki/haikei/haikei3");
        }
    }
    {
        BLOCK* b = blockPrefab + BLOCK_TYPE_ISEKI_HAIKEI_04;
        (*b) = BlockStageObjectTmp; //テンプレをコピー
        b->isBackground = true;
        b->noCollision = true;
        b->size = MkF2(
            BLOCK_WIDTH * 1,
            BLOCK_HIGHT * 1
        );
        {
            SPRITE_ANIM_DAT* sadat = &b->sprite;

            sadat->pos3D = MkF3(0.0f, 0.0f, BLOCK_HIGHT * -5.0f + haikeiYPosMod);
            sadat->rot3D = MkF3(80.0f, 0.0f, 0.0f);

            sadat->size.x = 560.0f * haikeiSizeMod;
            sadat->size.y = 791.0f * haikeiSizeMod;

            sadat->color = MkF4(0.3f, 0.3f, 0.3f, 1.0f);

            sadat->texDat = LoadTexData("TEX/bg/iseki/haikei/haikei4");
        }
    }
    {
        BLOCK* b = blockPrefab + BLOCK_TYPE_ISEKI_HAIKEI_05;
        (*b) = BlockStageObjectTmp; //テンプレをコピー
        b->isBackground = true;
        b->noCollision = true;
        b->size = MkF2(
            BLOCK_WIDTH * 1,
            BLOCK_HIGHT * 1
        );
        {
            SPRITE_ANIM_DAT* sadat = &b->sprite;

            sadat->pos3D = MkF3(0.0f, 0.0f, BLOCK_HIGHT * 20.0f + haikeiYPosMod);
            sadat->rot3D = MkF3(80.0f, 0.0f, 0.0f);

            sadat->size.x = 429.0f * haikeiSizeMod;
            sadat->size.y = 297.0f * haikeiSizeMod;

            sadat->color = MkF4(0.3f, 0.3f, 0.3f, 1.0f);

            sadat->texDat = LoadTexData("TEX/bg/iseki/haikei/haikei5");
        }
    }
    {
        BLOCK* b = blockPrefab + BLOCK_TYPE_ISEKI_HAIKEI_06;
        (*b) = BlockStageObjectTmp; //テンプレをコピー
        b->isBackground = true;
        b->noCollision = true;
        b->size = MkF2(
            BLOCK_WIDTH * 1,
            BLOCK_HIGHT * 1
        );
        {
            SPRITE_ANIM_DAT* sadat = &b->sprite;

            sadat->pos3D = MkF3(0.0f, 0.0f, BLOCK_HIGHT * 20.0f + haikeiYPosMod);
            sadat->rot3D = MkF3(80.0f, 0.0f, 0.0f);

            sadat->size.x = 203.0f * haikeiSizeMod;
            sadat->size.y = 91.0f * haikeiSizeMod;

            sadat->color = MkF4(0.3f, 0.3f, 0.3f, 1.0f);

            sadat->texDat = LoadTexData("TEX/bg/iseki/haikei/haikei6");
        }
    }
    {
        BLOCK* b = blockPrefab + BLOCK_TYPE_ISEKI_HAIKEI_07;
        (*b) = BlockStageObjectTmp; //テンプレをコピー
        b->isBackground = true;
        b->noCollision = true;
        b->size = MkF2(
            BLOCK_WIDTH * 1,
            BLOCK_HIGHT * 1
        );
        {
            SPRITE_ANIM_DAT* sadat = &b->sprite;

            sadat->pos3D = MkF3(0.0f, 0.0f, BLOCK_HIGHT * -30.0f + haikeiYPosMod);
            sadat->rot3D = MkF3(80.0f, 0.0f, 0.0f);

            sadat->size.x = 208.0f * haikeiSizeMod * 0.75;
            sadat->size.y = 635.0f * haikeiSizeMod * 0.75;

            sadat->color = MkF4(0.3f, 0.3f, 0.3f, 1.0f);

            sadat->texDat = LoadTexData("TEX/bg/iseki/haikei/haikei7");
        }
    }

	////--------------------------------------------------
	//// 遺跡タイルブロック
	////--------------------------------------------------
	//for (int i = 0; i < BLOCK_TYPE_tile4 - BLOCK_TYPE_tile_center + 1; i++) //1 ~ 10
	//{
	//	BLOCK* b = blockPrefab + BLOCK_TYPE_tile_center + i;
	//	(*b) = BlockStageObjectTmp; //テンプレをコピー

	//	b->noCollision = true;
	//	{
	//		SPRITE_ANIM_DAT* sadat = &b->sprite;
	//		sadat->texDat = GetTexData((TEX_TYPE)(TEX_TYPE_BLOCK_tile_center + i));
	//		sadat->size = MkF2(BLOCK_WIDTH * 1,
	//			BLOCK_HIGHT * 1);
	//	}
	//}

    for (int i = 0; i < BLOCK_TYPE_MAX; i++)
    {
        blockPrefab[i].isUse = true;
        blockPrefab[i].type = (BLOCK_TYPE)i;
    }
}

// ===================================================
// 終了処理
// ===================================================
void UninitBlockPrefab(void)
{
}



BLOCK GetBlockPrefab(BLOCK_TYPE type)
{
	return blockPrefab[type];
}