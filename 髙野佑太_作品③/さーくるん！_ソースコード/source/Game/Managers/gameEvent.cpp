// ===================================================
// gameEvent.cpp ゲームイベントの管理
// 
// hara sougo  2024/11/14
// ===================================================
#define IS_GAME_EVENT_MANAGER
#include "../../game_main.h"
#include "../Sprite/sprite.h"
#include "../Texture/texture.h"
#include "../Entities/Block/block.h"
#include "../Systems/collision.h"
#include "../Systems/easing.h"

#include "../Game Object/gameObject.h"
#include "../Game Object/gameObjectPrefab.h"

#include "../Systems/ui.h"

#include "gameEvent.h"

//ゲームイベントのインクルード
#include "../Game Events/GE_Shoot.h"
#include "../Game Events/GE_Gaya.h"


// ===================================================
// マクロ定義
// ===================================================

// ===================================================
// プロトタイプ宣言
// ===================================================

// ===================================================
// グローバル変数
// ===================================================

void InitGameEvent(void)
{
    GE_InitShoot();
    GE_InitGaya();
}

void UninitGameEvent(void)
{
    GE_UninitShoot();
    GE_UninitGaya();
}

void UpdateGameEvent(void)
{
    GE_UpdateShoot();
    GE_UpdateGaya();
}

void DrawGameEvent(void)
{
	GE_DrawShoot();
    GE_DrawGaya();
}
