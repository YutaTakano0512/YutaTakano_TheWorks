#pragma once
#include <memory>

//=============================================================================
// Component 基底クラス
//=============================================================================
class GameObject;

class Component
{
protected:
    GameObject* m_GameObject = nullptr;

public:
    bool enabled = true;

    Component() {}
    explicit Component(GameObject* obj) : m_GameObject(obj) {}
    virtual ~Component() {}

    // ライフサイクル
    virtual void Init()           {}
    virtual void Uninit()         {}
    virtual void Update(float dt) {}
    virtual void Draw()           {}

    GameObject* GetGameObject() const { return m_GameObject; }
    void        SetGameObject(GameObject* obj) { m_GameObject = obj; }
};
