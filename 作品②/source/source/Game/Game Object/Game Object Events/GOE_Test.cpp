// ===================================================
// GOE_Test.cpp テストオブジェクト
// 
// hara sougo  2024/07/24
// ===================================================
#include "../../../game_main.h"
#include "../../Sprite/sprite.h"
#include "../../Texture/texture.h"
#include "../../../Game/Entities/Block/block.h"
#include "../../Systems/collision.h"
#include "../../Systems/easing.h"

#include "../gameObject.h"
#include "../gameObjectPrefab.h"

#include "../../Systems/ui.h"

#include "GOE_Test.h"

// ===================================================
// マクロ定義
// ===================================================

// ===================================================
// プロトタイプ宣言
// ===================================================

// ===================================================
// グローバル変数
// ===================================================
static bool testbool = false;

// ===================================================
// オブジェクトの初期化
// ===================================================
void GOE_Test::Init()
{
}

void GOE_Test::FirstFrame()
{
	if (!testbool)
	{
		//obj->pos.x = 0.0f;
		//obj->pos.y = 0.0f;
		//obj->scale = MkF2(2.0f, 2.0f);
		////ShowBossbar(index, true);
		//testbool = true;
	}
}

// ===================================================
// オブジェクトの終了
// ===================================================
void GOE_Test::Uninit()
{

}

// ===================================================
// オブジェクトの更新
// ===================================================
void GOE_Test::Update()
{

	if (m_Obj->status.hp <= 0)
	{
		m_Obj->status.hp = m_Obj->status.hpMax;
		//ShowBossbar(index, true);
	}
}

// ===================================================
// オブジェクトの更新(アクティブ)
// ===================================================
void GOE_Test::UpdateActive()
{
	m_Obj->rot -= 0.5f;
	m_Obj->graphic.graph.spriteAnim[1].rot += 1.0f;
}


void GOE_Test::DamageAttacker(GAME_OBJECT_DAMAGE_RESULT dr)
{
}

void GOE_Test::DamageReciever(GAME_OBJECT_DAMAGE_RESULT dr)
{
}

void GOE_Test::BlockHit(GAME_OBJECT_POSMOD_RESULT pmr)
{
}