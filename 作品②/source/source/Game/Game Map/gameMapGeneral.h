// ===================================================
// gameMapGeneral.h マップデータの定義
// 
// hara sougo  2024/08/14
// ===================================================
#ifndef _GAME_MAP_GENERAL_H_
#define _GAME_MAP_GENERAL_H_


#include "../Systems/sys_general.h"

// ===================================================
// マクロ定義
// ===================================================
#define GAME_MAP_DAT_H_PATH ("gameMap_dataFile.h")
#define GAME_MAP_DAT_C_PATH ("gameMap_dataFile.cpp")

// ===================================================
// 構造体宣言
// ===================================================

struct GAME_MAP_OBJ_DAT {		// マップに配置されたオブジェクトのデータ構造体
	int x;
	int y;
	int obj;
};

struct GAME_MAP_OBJ_LIST_DAT {		// マップに配置されたオブジェクトのデータ構造体
	int count;

	GAME_MAP_OBJ_DAT* objects;
};

struct GAME_MAP_BLOCK_DAT {		// マップに配置されたブロックのデータ構造体
	int x;
	int y;
	int block;
};
struct GAME_MAP_BLOCK_LIST_DAT {		// マップデータ構造体
	/*int width;
	int hight;

	int* map;*/
	int count;

	GAME_MAP_BLOCK_DAT* blocks;
};

struct GAME_MAP {		// マップ構造体
	GAME_MAP_OBJ_LIST_DAT objDat;
	GAME_MAP_BLOCK_LIST_DAT blockDat;
};


#endif