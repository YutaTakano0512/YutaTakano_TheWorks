// ===================================================
// gameMap.cpp マップの制御
// 
// hara sougo  2024/07/29
// ===================================================
#include "../../game_main.h"
#include "../Texture/texture.h"
#include "../Sprite/sprite.h"
#include "../Entities/Block/block.h"
#include "../Entities/Block/blockPrefab.h"
#include "../Game Object/gameObject.h"
#include "../Game Object/gameObjectPrefab.h"

#include "../Systems/camera.h"

#include "gameMap.h"

// ===================================================
// マクロ定義
// ===================================================
//#define BLOCK_NUM_X	(20)
//#define BLOCK_NUM_Y	(16)

// ===================================================
// 構造体宣言
// ===================================================

static GAME_MAP map = {};


void InitGameMap(void)
{
	map = {};
	//map.mapDat.map = (int*)testMap;
	//map.mapDat.width = BLOCK_NUM_X;
	//map.mapDat.hight = BLOCK_NUM_Y;

	//for (int y = 0; y < map.mapDat.hight; y++)
	//{
	//	for (int x = 0; x < map.mapDat.width; x++)
	//	{
	//		//int block = testMap[y][x];
	//		int block = *(map.mapDat.map + y * map.mapDat.width + x);

	//		Float2 posBase = {};
	//		posBase.x = (-SCREEN_WIDTH / 2) + (BLOCK_WIDTH / 2);
	//		posBase.y = (-SCREEN_HEIGHT / 2) + (BLOCK_HIGHT / 2);

	//		Float2 posBlock = {};
	//		posBlock.x = posBase.x + (x * BLOCK_WIDTH);
	//		posBlock.y = posBase.y + (y * BLOCK_HIGHT);

	//		BLOCK blockDatTmp = {};
	//		blockDatTmp.type = BLOCK_TYPE_VOID;
	//		blockDatTmp.pos = MkF2( posBlock.x, posBlock.y );
	//		blockDatTmp.size = MkF2( BLOCK_WIDTH, BLOCK_HIGHT );

	//		blockDatTmp.type = (BLOCK_TYPE)block;

	//		if (blockDatTmp.type != BLOCK_TYPE_VOID)
	//		{
	//			SetBlock(blockDatTmp);
	//		}
	//	}
	//}
}

void UpdateGameMap(void)
{
}

void UninitGameMap(void)
{
}

//マップを読み込む
void GameMapLoad(GAME_MAP_DATA_ID id,
    std::vector<BLOCK*>* outBlocks,
    std::vector<GAME_OBJECT*>* outObjects
)
{
	GAME_MAP maptmp = GameMapDataLoad(id);

	for (int i = 0; i < maptmp.blockDat.count; i++)
	{
		GAME_MAP_BLOCK_DAT dat = maptmp.blockDat.blocks[i];

		BLOCK b = GetBlockPrefab((BLOCK_TYPE)dat.block);
		b.pos = MkF2((float)dat.x, (float)dat.y);

        BLOCK* newBlock = SetBlock(b);
        if (outBlocks != nullptr && newBlock != nullptr)
        {
            outBlocks->push_back(newBlock);
        }
	}
	for (int i = 0; i < maptmp.objDat.count; i++)
	{
		GAME_MAP_OBJ_DAT dat = maptmp.objDat.objects[i];

		GAME_OBJECT o = GetGameObjectPrefab((GAME_OBJECT_TYPE)dat.obj);
		o.pos = MkF3((float)dat.x, (float)dat.y, 0.0f);

        GAME_OBJECT* newObj = CreateGameObject(o);
        if (outObjects != nullptr && newObj != nullptr)
        {
            outObjects->push_back(newObj);
        }
	}

	GameMapFree(&maptmp);
}

void GameMapFree(GAME_MAP* map)
{
	free(map->objDat.objects);
	free(map->blockDat.blocks);
}

