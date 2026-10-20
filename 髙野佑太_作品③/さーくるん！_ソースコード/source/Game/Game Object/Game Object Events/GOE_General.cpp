// ===================================================
// GOE_General.cpp テストオブジェクト
// 
// hara sougo  2024/08/25
// ===================================================
#include "../../../game_main.h"
#include "../../Sprite/sprite.h"
#include "../../Texture/texture.h"
#include "../../../Game/Entities/Block/block.h"
#include "../../Systems/collision.h"
#include "../../Systems/easing.h"

#include "../gameObject.h"
#include "../gameObjectGraph.h"
#include "../gameObjectPrefab.h"

#include "../../Systems/ui.h"

#include "GOE_General.h"

// ===================================================
// マクロ定義
// ===================================================

// ===================================================
// プロトタイプ宣言
// ===================================================

// ===================================================
// グローバル変数
// ===================================================

// ===================================================
// オブジェクトの初期化
// ===================================================
void GOE_General::Init()
{
	if (m_Obj->type == GAME_OBJECT_TYPE_EFFECT)
	{
		m_Obj->graphic.hidden = true;
	}
}

void GOE_General::FirstFrame()
{
	//エフェクト
	if (m_Obj->type == GAME_OBJECT_TYPE_EFFECT)
	{
	}
}

// ===================================================
// オブジェクトの終了
// ===================================================
void GOE_General::Uninit()
{
    if (m_Obj->type == GAME_OBJECT_TYPE_EFFECT)
    {
    }
}

// ===================================================
// オブジェクトの更新
// ===================================================
void GOE_General::Update()
{

}

// ===================================================
// オブジェクトの更新(アクティブ)
// ===================================================
void GOE_General::UpdateActive()
{
	GAME_OBJECT* objOwner = nullptr;

	if (m_Obj->ownerObjectIndex >= 0)
	{
		objOwner = GetGameObject(m_Obj->ownerObjectIndex);
	}

	//エフェクト
	if (m_Obj->type == GAME_OBJECT_TYPE_EFFECT)
	{
	}
}


void GOE_General::Draw()
{
}


void GOE_General::Dead()
{
}


void GOE_General::DamageAttacker(GAME_OBJECT_DAMAGE_RESULT dr)
{
}

void GOE_General::DamageReciever(GAME_OBJECT_DAMAGE_RESULT dr)
{
}

void GOE_General::BlockHit(GAME_OBJECT_POSMOD_RESULT pmr)
{
}
