// ===================================================
// player.h プレイヤー制御
// 
// hara sougo  2024/07/22
// ===================================================
#ifndef _PLAYER_H_
#define _PLAYER_H_


#include "../Systems/sys_general.h"

#include "../Game Object/gameObject.h"

// ===================================================
// マクロ定義
// ===================================================
#define PLAYER_MAX (SYS_PLAYER_MAX)
// ===================================================
// 列挙体定義
// ===================================================

// ===================================================
// 構造体宣言
// ===================================================
struct PLAYER {		// プレイヤー構造体
	int index;
	GAME_OBJECT* obj;	//プレイヤーのオブジェクト
    int playerNo;
};


// ===================================================
// プロトタイプ宣言
// ===================================================
void InitPlayer(void);

void UpdatePlayer(void);

void UninitPlayer(void);

void SetPlayerLogInEnable(bool b);
bool GetPlayerLogInEnable();


void ResetPlayers();
void ResetActionPlayers();
void TotalPlayerDamageAdd(int b);
int GetTotalPlayerDamage();

PLAYER* GetPlayer(int index);
PLAYER* GetPlayer(GAME_OBJECT* gobj);
int GetActivePlayerNum();
int GetAlivePlayerNum();

std::vector<PLAYER> GetPlayesrFindByDistance(Float3 startPos, float distance);

void SetPlayerGameoverDecision();
////引数に渡したプレイヤーの対戦相手を返す
////見つからなかった場合は ぬるぽ
//PLAYER* GetPlayerOpponent(PLAYER* plyr);


//void GOE_InitPlayerObject(int index);
//void GOE_FirstFramePlayerObject(int index);
//void GOE_UninitPlayerObject(int index);
//void GOE_UpdatePlayerObject(int index);
//void GOE_UpdateActivePlayerObject(int index);
//void GOE_DrawPlayerObject(int index);
//void GOE_DeadPlayerObject(int index);
//
//void GOE_DamageAttackerPlayerObject(int index, GAME_OBJECT_DAMAGE_RESULT dr);
//void GOE_DamageRecieverPlayerObject(int index, GAME_OBJECT_DAMAGE_RESULT dr);
//
//
//
//void GOE_InitPlayerBulletObject(int index);
//void GOE_FirstFramePlayerBulletObject(int index);
//void GOE_UninitPlayerBulletObject(int index);
//void GOE_UpdatePlayerBulletObject(int index);
//void GOE_UpdateActivePlayerBulletObject(int index);
//
//void GOE_DamageAttackerPlayerBulletObject(int index, GAME_OBJECT_DAMAGE_RESULT dr);
//void GOE_DamageRecieverPlayerBulletObject(int index, GAME_OBJECT_DAMAGE_RESULT dr);
//
//void GOE_BlockHitPlayerBulletObject(int index, GAME_OBJECT_POSMOD_RESULT pmr);

#endif