// ===================================================
// titlebg.cpp タイトル背景表示
// 
// hara sougo  2024/07/22
// ===================================================
#include "game_main.h"
#include "Game/Sprite/sprite.h"
#include "Game/Texture/texData.h"
#include "titlebg.h"
//#include "controller.h"

// ===================================================
// グローバル変数
// ===================================================
TITLEBG titlebg;	// タイトル背景実体
//unsigned int TitlebgTextureId;	// タイトル背景テクスチャID

// ===================================================
// タイトル背景の初期化
// ===================================================
void InitializeTitlebg(void)
{
	titlebg.pos = MkF2(0.0f, 0.0f );
	titlebg.size = MkF2(SCREEN_WIDTH, SCREEN_HEIGHT );
	titlebg.use = true;

	// テクスチャの読み込み
	//TitlebgTextureId = LoadTexture("rom:/texture/bg/test_title.tga");
	//TitlebgTextureId = LoadTexture("rom:/texture/bg_01.tga");
	
}

// ===================================================
// タイトル背景の更新
// ===================================================
void UpdateTitlebg(void)
{
	if (titlebg.use)
	{
	}
}

// ===================================================
// タイトル背景の描画
// ===================================================
void DrawTitlebg(void)
{
	if (titlebg.use)
	{
		/*DrawSpriteQuad(
			titlebg.pos.x, titlebg.pos.y,
			titlebg.size.x, titlebg.size.y,
			TitlebgTextureId
		);*/
	}
}

// ===================================================
// タイトル背景の終了処理
// ===================================================
void FinalizeTitlebg(void)
{
	//UnloadTexture(TitlebgTextureId);
}