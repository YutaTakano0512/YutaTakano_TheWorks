//===========================================================================
// Param.h
// パラメータ
// sougo hara  2024/07/24
//===========================================================================
#ifndef _PARAM_H_
#define _PARAM_H_


//---------------------------------------------------------------------------
// オブジェクトのサイズ(ブロックに当たる判定)
//---------------------------------------------------------------------------

//汎用
#define PARAM_GAME_OBJECT_GENERAL_SIZE_X				(96.0f)
#define PARAM_GAME_OBJECT_GENERAL_SIZE_Y				(96.0f)
//汎用
#define PARAM_GAME_OBJECT_PLAYER_SIZE_X				(60.0f)
#define PARAM_GAME_OBJECT_PLAYER_SIZE_Y				(60.0f)


//---------------------------------------------------------------------------
// オブジェクトのHP
//---------------------------------------------------------------------------

//汎用
#define PARAM_GAME_OBJECT_GENERAL_STATUS_HP				(1)

//プレイヤー
#define PARAM_GAME_OBJECT_PLAYER_STATUS_HP				(100)

//キャラ
#define PARAM_GAME_OBJECT_CHARACTER_STATUS_HP			(1000)

//テストオブジェクト
#define PARAM_GAME_OBJECT_TEST_STATUS_HP				(1000)

//実質無敵
#define PARAM_GAME_OBJECT_INV_STATUS_HP					(99999)


//---------------------------------------------------------------------------
// オブジェクトの攻撃判定
//---------------------------------------------------------------------------

//汎用
//#define PARAM_GAME_OBJECT_GENERAL_COL_ATK_SIZER			(10.0f)	//判定の半径
#define PARAM_GAME_OBJECT_GENERAL_COL_ATK_SIZE_X			(20.0f)	//判定の幅
#define PARAM_GAME_OBJECT_GENERAL_COL_ATK_SIZE_Y			(20.0f)	//判定の高さ
#define PARAM_GAME_OBJECT_GENERAL_COL_ATK_SIZE_Z			(20.0f)	//判定の厚さ
#define PARAM_GAME_OBJECT_GENERAL_COL_ATK_POWER			(1)		//攻撃力
#define PARAM_GAME_OBJECT_GENERAL_COL_ATK_HITSTOP_ATK	(3)		//ヒットストップのフレーム数(攻撃側)
#define PARAM_GAME_OBJECT_GENERAL_COL_ATK_HITSTOP_REC	(3)		//ヒットストップのフレーム数(受け側)
#define PARAM_GAME_OBJECT_GENERAL_COL_ATK_HITCYCLE		(60)	//判定周期(一度当たってから再度ヒットするようになるまでのフレーム数)

//テストオブジェクト
#define PARAM_GAME_OBJECT_TEST_COL_ATK_SIZE_X			(50.0f)
#define PARAM_GAME_OBJECT_TEST_COL_ATK_SIZE_Y			(50.0f)
#define PARAM_GAME_OBJECT_TEST_COL_ATK_SIZE_Z			(50.0f)
#define PARAM_GAME_OBJECT_TEST_COL_ATK_POWER			(5)
#define PARAM_GAME_OBJECT_TEST_COL_ATK_HITSTOP_ATK		(12)
#define PARAM_GAME_OBJECT_TEST_COL_ATK_HITSTOP_REC		(12)
#define PARAM_GAME_OBJECT_TEST_COL_ATK_HITCYCLE			(PARAM_GAME_OBJECT_GENERAL_COL_ATK_HITCYCLE)

#define PARAM_GAME_OBJECT_TEST_1_COL_ATK_SIZE_X			(50.0f)
#define PARAM_GAME_OBJECT_TEST_1_COL_ATK_SIZE_Y			(100.0f)
#define PARAM_GAME_OBJECT_TEST_1_COL_ATK_SIZE_Z			(50.0f)
#define PARAM_GAME_OBJECT_TEST_1_COL_ATK_POWER			(1)
#define PARAM_GAME_OBJECT_TEST_1_COL_ATK_HITSTOP_ATK	(8)
#define PARAM_GAME_OBJECT_TEST_1_COL_ATK_HITSTOP_REC	(8)
#define PARAM_GAME_OBJECT_TEST_1_COL_ATK_HITCYCLE		(10)

#define PARAM_GAME_OBJECT_TEST_2_COL_ATK_SIZE_X			(50.0f)
#define PARAM_GAME_OBJECT_TEST_2_COL_ATK_SIZE_Y			(90.0f)
#define PARAM_GAME_OBJECT_TEST_2_COL_ATK_SIZE_Z			(50.0f)
#define PARAM_GAME_OBJECT_TEST_2_COL_ATK_POWER			(20)
#define PARAM_GAME_OBJECT_TEST_2_COL_ATK_HITSTOP_ATK	(30)
#define PARAM_GAME_OBJECT_TEST_2_COL_ATK_HITSTOP_REC	(30)
#define PARAM_GAME_OBJECT_TEST_2_COL_ATK_HITCYCLE		(180)


//エネミー汎用
#define PARAM_GAME_OBJECT_ENEMY_GENERAL_COL_ATK_SIZER				(40.0f)
#define PARAM_GAME_OBJECT_ENEMY_GENERAL_COL_ATK_POWER				(4)
//#define PARAM_GAME_OBJECT_ENEMY_GENERAL_COL_ATK_POWER				(50)
#define PARAM_GAME_OBJECT_ENEMY_GENERAL_COL_ATK_HITSTOP_ATK			(12)
#define PARAM_GAME_OBJECT_ENEMY_GENERAL_COL_ATK_HITSTOP_REC			(12)
#define PARAM_GAME_OBJECT_ENEMY_GENERAL_COL_ATK_HITCYCLE			(PARAM_GAME_OBJECT_GENERAL_COL_ATK_HITCYCLE)

//---------------------------------------------------------------------------
// オブジェクトのくらい判定
//---------------------------------------------------------------------------

//汎用
//#define PARAM_GAME_OBJECT_GENERAL_COL_REC_SIZER				(10.0f)	//判定の半径
#define PARAM_GAME_OBJECT_GENERAL_COL_REC_SIZE_X				(20.0f)
#define PARAM_GAME_OBJECT_GENERAL_COL_REC_SIZE_Y				(20.0f)
#define PARAM_GAME_OBJECT_GENERAL_COL_REC_SIZE_Z				(20.0f)

//プレイヤー
#define PARAM_GAME_OBJECT_PLAYER_COL_REC_SIZE_X				(60.0f)
#define PARAM_GAME_OBJECT_PLAYER_COL_REC_SIZE_Y				(60.0f)
#define PARAM_GAME_OBJECT_PLAYER_COL_REC_SIZE_Z				(60.0f)

//テストオブジェクト
#define PARAM_GAME_OBJECT_TEST_COL_REC_SIZE_X				(60.0f)
#define PARAM_GAME_OBJECT_TEST_COL_REC_SIZE_Y				(60.0f)
#define PARAM_GAME_OBJECT_TEST_COL_REC_SIZE_Z				(60.0f)

//エネミー汎用
#define PARAM_GAME_OBJECT_ENEMY_GENERAL_COL_REC_SIZE_X		(120.0f)
#define PARAM_GAME_OBJECT_ENEMY_GENERAL_COL_REC_SIZE_Y		(120.0f)
#define PARAM_GAME_OBJECT_ENEMY_GENERAL_COL_REC_SIZE_Z		(120.0f)





#endif