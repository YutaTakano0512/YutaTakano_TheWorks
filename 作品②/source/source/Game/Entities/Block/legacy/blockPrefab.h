// ===================================================
// blockPrefab.h ブロックの設計図
// 
// hara sougo  2024/08/17
// ===================================================
#ifndef _BLOCK_PREFAB_H_
#define _BLOCK_PREFAB_H_


#include "../../Systems/sys_general.h"

// ===================================================
// プロトタイプ宣言
// ===================================================
void InitBlockPrefab(void);
void UninitBlockPrefab(void);


BLOCK GetBlockPrefab(BLOCK_TYPE type);

#endif