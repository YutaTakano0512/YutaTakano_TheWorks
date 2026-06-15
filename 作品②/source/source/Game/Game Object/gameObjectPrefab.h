// ===================================================
// gameObjectPrefab.h ゲームオブジェクトの設計図
// 
// hara sougo  2024/07/22
// ===================================================
#ifndef _GAME_OBJECT_PREFAB_H_
#define _GAME_OBJECT_PREFAB_H_


#include "../Systems/sys_general.h"

// ===================================================
// プロトタイプ宣言
// ===================================================
void InitGameObjectPrefab(void);
void UninitGameObjectPrefab(void);


GAME_OBJECT GetGameObjectPrefab(GAME_OBJECT_TYPE type);

#endif