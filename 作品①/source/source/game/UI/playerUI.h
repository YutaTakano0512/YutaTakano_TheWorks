#pragma once
#include "../../core/gameObject.h"
#include "../objects/player.h"
#include "../../graphics/spriteRenderer.h"
#include "../../graphics/textRenderer.h"

//=============================================================================
// PlayerUI クラス
// プレイヤーのスキルクールダウン・HPバーを画面左下に表示するCanvas UI
//=============================================================================
class PlayerUI : public GameObject
{
public:
    void Init() override;
    void Update(float dt) override;

    void SetPlayer(Player* player) { m_Player = player; }

private:
    Player* m_Player    = nullptr;
    float   m_BlinkTimer = 0.0f;

	// HPバー背景
    GameObject*     m_HpBarBgObj    = nullptr;
    SpriteRenderer* m_HpBarBg       = nullptr;

	// HPバー本体
    GameObject*     m_HpBarObj      = nullptr;
    SpriteRenderer* m_HpBar         = nullptr;

	// HPテキスト
    GameObject*  m_HpLabelObj  = nullptr;
    TextRenderer* m_HpLabelComp = nullptr;

	// HP数値テキスト
    GameObject*  m_HpTextObj = nullptr;
    TextRenderer* m_HpText   = nullptr;
};
