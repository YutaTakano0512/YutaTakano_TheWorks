//このファイルはエディタ(sceneEditor.cpp)のプログラムで生成しています
//可読性は考慮していません

#ifndef _GAME_MAP_DAT_H_
#define _GAME_MAP_DAT_H_ 
#include "gameMapGeneral.h" 

enum GAME_MAP_DATA_ID {
	GAME_MAP_DATA_ID_TEST_MAP0 = 0,
	GAME_MAP_DATA_ID_TESTMAP2,
	GAME_MAP_DATA_ID_AAAAAA,
	GAME_MAP_DATA_ID_STAGE_ISEKI,
	GAME_MAP_DATA_ID_STAGE_ISEKI_TILE,
	GAME_MAP_DATA_ID_STAGE_ISEKI_00,
	GAME_MAP_DATA_ID_STAGE_ISEKI_01,
	GAME_MAP_DATA_ID_MAX,
};

GAME_MAP GameMapDataLoad(GAME_MAP_DATA_ID id);
std::string GameMapDataGetIdSring(int id);
#endif

