// ===================================================
// gameObject.h オブジェクトの種類の定義
// 
// hara sougo  2024/10/30
// ===================================================
#ifndef _GAME_OBJECT_TYPES_H_
#define _GAME_OBJECT_TYPES_H_


#include "../Systems/sys_general.h"

// ===================================================
// マクロ定義
// ===================================================

// ===================================================
// 列挙体定義
// ===================================================


//
// オブジェクトの種類
//
enum GAME_OBJECT_TYPE {
	GAME_OBJECT_TYPE_VOID = 0, //無

	//GAME_OBJECT_TYPE_PLAYER,

	GAME_OBJECT_TYPE_CHARACTER,

	GAME_OBJECT_TYPE_TEST,
	GAME_OBJECT_TYPE_TEST_1,
	GAME_OBJECT_TYPE_TEST_2,

	GAME_OBJECT_TYPE_PLAYERTEST_DOWN,
	GAME_OBJECT_TYPE_PLAYERTEST_LEFT,


	GAME_OBJECT_TYPE_ISEKI_ZANGAI_0,
	GAME_OBJECT_TYPE_ISEKI_ZANGAI_1,
	GAME_OBJECT_TYPE_ISEKI_ZANGAI_2,
	GAME_OBJECT_TYPE_ISEKI_DAIZA_0,
	GAME_OBJECT_TYPE_ISEKI_HASHIRA_0,
	GAME_OBJECT_TYPE_ISEKI_HASHIRA_TAORETA_0,
	GAME_OBJECT_TYPE_ISEKI_KANATSUBAN_0,


    GAME_OBJECT_TYPE_EFFECT,
    GAME_OBJECT_TYPE_DUMMY,

	GAME_OBJECT_TYPE_MAX, //最大値
};


//
// オブジェクトの分類
//

#define STR__GAME_OBJECT_FAMILY ("GAME_OBJECT_FAMILY_")

enum GAME_OBJECT_FAMILY {
	GAME_OBJECT_FAMILY_ALL = 0, //無

	GAME_OBJECT_FAMILY_PLAYER,
	GAME_OBJECT_FAMILY_PLAYER_ATTACK,
	GAME_OBJECT_FAMILY_ENEMY,
	GAME_OBJECT_FAMILY_ENEMY_ATTACK,
	GAME_OBJECT_FAMILY_GIMIK,
	GAME_OBJECT_FAMILY_MISC,

	GAME_OBJECT_FAMILY_MAX,  //最大値
};


#endif