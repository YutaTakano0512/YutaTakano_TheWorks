// ===================================================
// gameObjectGraph.h ゲームオブジェクトのグラフィック
// 
// hara sougo  2024/07/22
// ===================================================
#ifndef _GAME_OBJECT_GRAPH_H_
#define _GAME_OBJECT_GRAPH_H_


#include "../Systems/sys_general.h"

#include "../Texture/texture.h"

// ===================================================
// 列挙体定義
// ===================================================

//グラフィックの種類
enum GAME_OBJECT_GRAPH {
	GAME_OBJECT_GRAPH_VOID = 0,
	GAME_OBJECT_GRAPH_PLAYER,

	GAME_OBJECT_GRAPH_TEST,
	GAME_OBJECT_GRAPH_PLAYERTEST_DOWN,
	GAME_OBJECT_GRAPH_PLAYERTEST_LEFT,

	GAME_OBJECT_GRAPH_ISEKI_ZANGAI_0,
	GAME_OBJECT_GRAPH_ISEKI_ZANGAI_1,
	GAME_OBJECT_GRAPH_ISEKI_ZANGAI_2,
	GAME_OBJECT_GRAPH_ISEKI_DAIZA_0,
	GAME_OBJECT_GRAPH_ISEKI_HASHIRA_0,
	GAME_OBJECT_GRAPH_ISEKI_HASHIRA_TAORETA_0,
	GAME_OBJECT_GRAPH_ISEKI_KANATSUBAN_0,

	GAME_OBJECT_GRAPH_EFFECT_SHOCKWAVE,
	GAME_OBJECT_GRAPH_EFFECT_FLASH_0,
	GAME_OBJECT_GRAPH_EFFECT_TWINKLE,
	GAME_OBJECT_GRAPH_EFFECT_SPARK,
    GAME_OBJECT_GRAPH_EFFECT_SPARK_NEW,

	GAME_OBJECT_GRAPH_MAX,
};

// ===================================================
// 構造体宣言
// ===================================================

// ===================================================
// プロトタイプ宣言
// ===================================================
void InitGameObjectGraph(void);
void UninitGameObjectGraph(void);

GAME_OBJECT_GRAPHIC GetGameObjectGraph(GAME_OBJECT_GRAPH graph);

#endif