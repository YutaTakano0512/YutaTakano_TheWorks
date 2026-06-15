// ===================================================
// GE_Shoot.h シュートの処理
// 
// hara sougo  2024/11/14
// ===================================================
#ifndef _GE_SHOOT_H_
#define _GE_SHOOT_H_


#include "../Systems/sys_general.h"
#include "../Game Object/gameObject.h"

// ===================================================
// マクロ定義
// ===================================================

// ===================================================
// 構造体宣言
// ===================================================


// ===================================================
// プロトタイプ宣言
// ===================================================
#ifdef IS_GAME_EVENT_MANAGER
void GE_InitShoot(void);
void GE_UninitShoot(void);
void GE_UpdateShoot(void);
void GE_DrawShoot(void);
#endif

bool GE_IsEnableShoot();
void GE_ShootStartRequest(GAME_OBJECT* startChara);


#endif