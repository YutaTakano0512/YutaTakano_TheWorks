#include "gameObject.h"

//=============================================================================
// GameObject 実装
//=============================================================================

GameObject::GameObject()
{
    // transform は直接メンバなので明示的な追加不要
    transform.SetGameObject(this);
}

void GameObject::Update(float dt)
{
    if (!m_Active) return;

    // transform は直接更新
    if (transform.enabled)
        transform.Update(dt);

    // 追加コンポーネントを更新
    for (auto& comp : m_Components)
    {
        if (comp->enabled)
            comp->Update(dt);
    }
}

void GameObject::Draw()
{
    if (!m_Active) return;

    for (auto& comp : m_Components)
    {
        if (comp->enabled)
            comp->Draw();
    }
}
