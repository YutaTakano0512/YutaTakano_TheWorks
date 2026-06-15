// ===================================================
// gameMap.h マップの制御
// 
// hara sougo  2024/07/29
// ===================================================
#ifndef _GAME_MAP_H_
#define _GAME_MAP_H_


#include "../Systems/sys_general.h"

#include "gameMapGeneral.h"

//マップデータ
#include "gameMap_dataFile.h"

// ===================================================
// マクロ定義
// ===================================================

// ===================================================
// 構造体宣言
// ===================================================

// ===================================================
// プロトタイプ宣言
// ===================================================
void InitGameMap(void);

void UpdateGameMap(void);

void UninitGameMap(void);

struct GAME_OBJECT;
struct BLOCK;
void GameMapLoad(
    GAME_MAP_DATA_ID id,
    std::vector<BLOCK*>* outBlocks = nullptr,
    std::vector<GAME_OBJECT*>* outObjects = nullptr
);
void GameMapFree(GAME_MAP* map);


#endif