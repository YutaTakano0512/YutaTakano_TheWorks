#pragma once
#include "../../core/gameObject.h"
#include "../objects/enemy.h"
#include "../../graphics/spriteRenderer.h"
#include "../../graphics/textRenderer.h"
#include <string>

//=============================================================================
// EnemyUI クラス
// 敵のHPバーと名前を画面中央上部に表示するCanvas UI
// 各要素は別々のGameObjectとして生成し、Layer::UI に登録する
//=============================================================================
class EnemyUI : public GameObject
{
public:
    void Init() override;
    void Update(float dt) override;

    void SetEnemy(Enemy* enemy)             { m_Enemy = enemy; }
    void SetEnemyName(const wchar_t* name)  { m_EnemyNameStr = name; }

    void SetVisible(bool visible);

private:
    Enemy*       m_Enemy        = nullptr;
    std::wstring m_EnemyNameStr = L"Enemy";
    float m_BlinkTimer   = 0.0f;

	bool m_IsVisible = true;

    // HPバー背景
    GameObject*     m_HpBarBgObj    = nullptr;
    SpriteRenderer* m_HpBarBg       = nullptr;

    // HPバー本体
    GameObject*     m_HpBarObj      = nullptr;
    SpriteRenderer* m_HpBar         = nullptr;

    // 敵の名前テキスト
    GameObject*  m_NameTextObj  = nullptr;
    TextRenderer* m_NameTextComp = nullptr;

    // HP数値テキスト
    GameObject*  m_HpTextObj = nullptr;
    TextRenderer* m_HpText   = nullptr;
};
