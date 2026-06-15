#include "enemyUI.h"
#include "../../core/scene.h"
#include "../../core/manager.h"
#include "../objects/damageable.h"
#include <sstream>
#include <cmath>

//=============================================================================
// 定数
//=============================================================================
static constexpr float HP_BAR_WIDTH  = 500.0f;  // HPバーの最大幅
static constexpr float HP_BAR_HEIGHT = 24.0f;   // HPバーの高さ

static constexpr float UI_CENTER_X = SCREEN_WIDTH  * 0.5f;
static constexpr float UI_TOP_Y    = 75.0f;

//=============================================================================
// 初期化
//=============================================================================

void EnemyUI::Init()
{
    Scene* scene = Manager::GetScene();
    if (!scene) return;

    // HPバー背景
    {
        m_HpBarBgObj = scene->AddGameObject<GameObject>(Layer::UI);
        m_HpBarBg = m_HpBarBgObj->AddComponent<SpriteRenderer>();
        m_HpBarBg->SetTexture("asset/texture/ui/hpBar_bg.png");
        m_HpBarBg->size  = { HP_BAR_WIDTH, HP_BAR_HEIGHT };
        m_HpBarBg->pivot = { 0.5f, 0.5f };
        m_HpBarBgObj->transform.position = { UI_CENTER_X, UI_TOP_Y };
        m_HpBarBgObj->transform.zOrder   = 10;
    }

    // HPバー本体
    {
        m_HpBarObj = scene->AddGameObject<GameObject>(Layer::UI);
        m_HpBar = m_HpBarObj->AddComponent<SpriteRenderer>();
        m_HpBar->SetTexture("asset/texture/ui/hpBar.png");
        m_HpBar->size  = { HP_BAR_WIDTH, HP_BAR_HEIGHT };
        m_HpBar->color = { 0.85f, 0.15f, 0.15f, 1.0f };
        m_HpBar->pivot = { 0.0f, 0.5f }; // 左端基準
        m_HpBarObj->transform.position = { UI_CENTER_X - HP_BAR_WIDTH * 0.5f, UI_TOP_Y };
        m_HpBarObj->transform.zOrder   = 11;
    }

    // 敵の名前テキスト (HPバーの上)
    {
        m_NameTextObj  = scene->AddGameObject<GameObject>(Layer::UI);
        m_NameTextComp = m_NameTextObj->AddComponent<TextRenderer>();
        m_NameTextComp->fontFamily = L"Meiryo";
        m_NameTextComp->fontSize   = 26.0f;
        m_NameTextComp->color      = { 0.95f, 0.85f, 0.6f, 1.0f }; // ゴールド
        m_NameTextComp->text       = m_EnemyNameStr;
        m_NameTextObj->transform.position =
        {
            UI_CENTER_X,
            UI_TOP_Y - HP_BAR_HEIGHT * 0.5f - 28.0f
        };
        m_NameTextObj->transform.zOrder = 11;
    }

    // HP数値テキスト (HPバー中央)
    {
        m_HpTextObj = scene->AddGameObject<GameObject>(Layer::UI);
        m_HpText    = m_HpTextObj->AddComponent<TextRenderer>();
        m_HpText->fontFamily = L"Meiryo";
        m_HpText->fontSize   = 20.0f;
        m_HpText->color      = { 1.0f, 1.0f, 1.0f, 0.9f };
        m_HpTextObj->transform.position = { UI_CENTER_X, UI_TOP_Y };
        m_HpTextObj->transform.zOrder = 11;
    }

    // 自身のGameObjectはzOrder管理のみ（SpriteRendererは持たない）
    transform.zOrder = 9;
}

//=============================================================================
// 更新
//=============================================================================

void EnemyUI::Update(float dt)
{
    GameObject::Update(dt);

    if (!m_Enemy) return;

    Damageable* dmg = m_Enemy->GetDamageable();
    if (!dmg) return;

    if(dmg->IsDead())
    {
        SetVisible(false);
	}

    if (!m_IsVisible)
    {
        return;
    }

    const float hp    = dmg->GetHp();
    const float maxHp = dmg->GetMaxHp();
    const float ratio = (maxHp > 0.0f) ? (hp / maxHp) : 0.0f;

    // HPバー幅をHP割合に応じて更新
    if (m_HpBar)
    {
        m_HpBar->size.x = HP_BAR_WIDTH * ratio;
    }

    // HP数値テキスト更新
    if (m_HpText)
    {
        std::wstringstream wss;
        wss << (int)hp << L" / " << (int)maxHp;
        m_HpText->text = wss.str();
    }
}

void EnemyUI::SetVisible(bool visible)
{
	m_IsVisible = visible;
	if (m_HpBar) m_HpBar->enabled = visible;
	if (m_HpBarBg) m_HpBarBg->enabled = visible;
	if (m_HpText) m_HpText->enabled = visible;
	if (m_NameTextComp) m_NameTextComp->enabled = visible;
}
