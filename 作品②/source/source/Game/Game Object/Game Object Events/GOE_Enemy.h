// ===================================================
// go_enemy.h エネミーオブジェクト
// 
// hara sougo  2024/07/24
// ===================================================
#ifndef _GOE_ENEMY_H_
#define _GOE_ENEMY_H_


#include "../../Systems/sys_general.h"

// ===================================================
// マクロ定義
// ===================================================
#define BOSS_DEFEAT_VIB_FRAME			(12)

// ===================================================
// 構造体宣言
// ===================================================

struct ENEMY_DATA {		// エネミーデータ構造体
	GAME_OBJECT* target;

	unsigned int phase2StartFrame;
	bool deadEventEndFlag;

	GAME_OBJECT* owner;
};

// ===================================================
// プロトタイプ宣言
// ===================================================
void GOE_InitEnemyObject(int index);
void GOE_FirstFrameEnemyObject(int index);
void GOE_UninitEnemyObject(int index);
void GOE_UpdateEnemyObject(int index);
void GOE_UpdateActiveEnemyObject(int index);
void GOE_DrawEnemyObject(int index);

void GOE_DeadEnemyObject(int index);

void GOE_DamageAttackerEnemyObject(int index, GAME_OBJECT_DAMAGE_RESULT dr);
void GOE_DamageRecieverEnemyObject(int index, GAME_OBJECT_DAMAGE_RESULT dr);

void GOE_BlockHitEnemyObject(int index, GAME_OBJECT_POSMOD_RESULT pmr);



void GOE_InitEnemyBulletObject(int index);
void GOE_FirstFrameEnemyBulletObject(int index);
void GOE_UninitEnemyBulletObject(int index);
void GOE_UpdateEnemyBulletObject(int index);
void GOE_UpdateActiveEnemyBulletObject(int index);

void GOE_DamageAttackerEnemyBulletObject(int index, GAME_OBJECT_DAMAGE_RESULT dr);
void GOE_DamageRecieverEnemyBulletObject(int index, GAME_OBJECT_DAMAGE_RESULT dr);

void GOE_BlockHitEnemyBulletObject(int index, GAME_OBJECT_POSMOD_RESULT pmr);

#endif