#include "playerUI.h"
#include "../../core/scene.h"
#include "../../core/manager.h"
#include "../objects/damageable.h"
#include <iomanip>
#include <sstream>
#include <cmath>

//=============================================================================
// 定数
//=============================================================================
static constexpr float PLAYER_HP_BAR_WIDTH  = 280.0f;
static constexpr float PLAYER_HP_BAR_HEIGHT = 20.0f;

static constexpr float PLAYER_UI_X   = 60.0f + PLAYER_HP_BAR_WIDTH * 0.5f;
static constexpr float PLAYER_UI_Y   = SCREEN_HEIGHT - 55.0f;

//=============================================================================
// 初期化
//=============================================================================
void PlayerUI::Init()
{
    Scene* scene = Manager::GetScene();

    if (scene)
    {
        // HPバー背景
        m_HpBarBgObj = scene->AddGameObject<GameObject>(Layer::UI);
        m_HpBarBg = m_HpBarBgObj->AddComponent<SpriteRenderer>();
        m_HpBarBg->SetTexture("asset/texture/ui/hpBar_bg.png");
        m_HpBarBg->size  = { PLAYER_HP_BAR_WIDTH, PLAYER_HP_BAR_HEIGHT };
        m_HpBarBg->pivot = { 0.5f, 0.5f };
        m_HpBarBgObj->transform.position = { PLAYER_UI_X, PLAYER_UI_Y };
        m_HpBarBgObj->transform.zOrder   = 10;

        // HPバー本体
        m_HpBarObj = scene->AddGameObject<GameObject>(Layer::UI);
        m_HpBar = m_HpBarObj->AddComponent<SpriteRenderer>();
        m_HpBar->SetTexture("asset/texture/ui/hpBar.png");
        m_HpBar->size  = { PLAYER_HP_BAR_WIDTH, PLAYER_HP_BAR_HEIGHT };
        m_HpBar->color = { 0.15f, 0.85f, 0.25f, 1.0f }; // 緑系
        m_HpBar->pivot = { 0.0f, 0.5f }; // 左端基準
        m_HpBarObj->transform.position = { PLAYER_UI_X - PLAYER_HP_BAR_WIDTH * 0.5f, PLAYER_UI_Y };
        m_HpBarObj->transform.zOrder = 11;

        // "HP" ラベルテキスト
        m_HpLabelObj  = scene->AddGameObject<GameObject>(Layer::UI);
        m_HpLabelComp = m_HpLabelObj->AddComponent<TextRenderer>();
        m_HpLabelComp->fontFamily = L"Meiryo";
        m_HpLabelComp->fontSize   = 18.0f;
        m_HpLabelComp->color      = { 0.7f, 1.0f, 0.7f, 1.0f };
        m_HpLabelComp->text       = L"HP";
        m_HpLabelObj->transform.position = { PLAYER_UI_X - PLAYER_HP_BAR_WIDTH * 0.5f + 15.0f, PLAYER_UI_Y + 1.0f };
        m_HpLabelObj->transform.zOrder = 11;

        // HP数値テキスト
        m_HpTextObj = scene->AddGameObject<GameObject>(Layer::UI);
        m_HpText    = m_HpTextObj->AddComponent<TextRenderer>();
        m_HpText->fontFamily = L"Meiryo";
        m_HpText->fontSize   = 18.0f;
        m_HpText->color      = { 1.0f, 1.0f, 1.0f, 0.95f };
        m_HpTextObj->transform.position = { PLAYER_UI_X + PLAYER_HP_BAR_WIDTH * 0.5f - 40.0f, PLAYER_UI_Y + 1.0f };
        m_HpTextObj->transform.zOrder = 11;
    }
}

//=============================================================================
// 更新
//=============================================================================
void PlayerUI::Update(float dt)
{
    GameObject::Update(dt);

    if (!m_Player) return;

    Damageable* dmg = m_Player->GetDamageable();
    if (dmg && m_HpBar)
    {
        const float hp    = dmg->GetHp();
        const float maxHp = dmg->GetMaxHp();
        const float ratio = (maxHp > 0.0f) ? (hp / maxHp) : 0.0f;

        // バー幅を更新
        m_HpBar->size.x = PLAYER_HP_BAR_WIDTH * ratio;

        // HP割合で色変化
        if (ratio > 0.5f)
        {
            m_HpBar->color = { 0.15f, 0.85f, 0.25f, 1.0f }; // 緑
        }
        else if (ratio > 0.25f)
        {
            m_HpBar->color = { 0.95f, 0.75f, 0.10f, 1.0f }; // 黄
        }
        else
        {
            // 低HP: 赤く点滅
            m_BlinkTimer += dt * 5.0f;
            float alpha = 0.75f + 0.25f * sinf(m_BlinkTimer);
            m_HpBar->color = { 1.0f, 0.15f, 0.15f, alpha };
        }

        // HP数値テキスト更新
        if (m_HpText)
        {
            std::wstringstream wss;
            wss << (int)hp << L"/" << (int)maxHp;
            m_HpText->text = wss.str();
        }
    }
}
