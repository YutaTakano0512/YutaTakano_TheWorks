#pragma once
#include <vector>
#include <memory>
#include <string>
#include <type_traits>
#include "component.h"
#include "transform.h"

//=============================================================================
// GameObject クラス
// Unityライクなコンポーネントベース設計。
// transform には直接アクセス可能 (obj->transform.position など)
//=============================================================================
class GameObject
{
public:
    Transform transform;   // 必ず存在するTransform (直接アクセス用)

    //-------------------------------------------------------------------------
    // 生成・破棄
    //-------------------------------------------------------------------------
    GameObject();
    virtual ~GameObject() = default;

    virtual void Init()           {}
    virtual void Uninit()
    {
        for (auto& comp : m_Components)
        {
            if (comp) comp->Uninit();
        }
    }
    virtual void Update(float dt);  // 全コンポーネントのUpdateを呼ぶ
    virtual void Draw();            // 全コンポーネントのDrawを呼ぶ

    //-------------------------------------------------------------------------
    // コンポーネント操作
    //-------------------------------------------------------------------------
    // コンポーネントを追加して返す
    template<typename T, typename... Args>
    T* AddComponent(Args&&... args)
    {
        static_assert(std::is_base_of<Component, T>::value, "T must derive from Component");
        auto comp = std::make_unique<T>(std::forward<Args>(args)...);
        comp->SetGameObject(this);
        comp->Init();
        T* ptr = comp.get();
        m_Components.push_back(std::move(comp));
        return ptr;
    }

    // 型でコンポーネントを検索 (最初に見つかったものを返す)
    template<typename T>
    T* GetComponent() const
    {
        for (const auto& c : m_Components)
        {
            if (auto p = dynamic_cast<T*>(c.get()))
                return p;
        }
        return nullptr;
    }

    // 型でコンポーネントを全検索
    template<typename T>
    std::vector<T*> GetComponents() const
    {
        std::vector<T*> result;
        for (const auto& c : m_Components)
        {
            if (auto p = dynamic_cast<T*>(c.get()))
                result.push_back(p);
        }
        return result;
    }

    // コンポーネントを削除
    template<typename T>
    void RemoveComponent()
    {
        m_Components.erase(
            std::remove_if(m_Components.begin(), m_Components.end(),
                [](const std::unique_ptr<Component>& c)
                {
                    if (dynamic_cast<T*>(c.get()) != nullptr)
                    {
                        c->Uninit();
                        return true;
                    }
                    return false;
                }),
            m_Components.end());
    }

    //-------------------------------------------------------------------------
    // プロパティ
    //-------------------------------------------------------------------------
    bool        IsActive()  const { return m_Active; }
    bool        IsDestroy() const { return m_Destroy; }
    void        SetActive(bool v)  { m_Active = v; }
    void        SetDestroy()       { m_Destroy = true; }

    const std::string& GetName() const { return m_Name; }
    void               SetName(const std::string& name) { m_Name = name; }

    const std::string& GetTag() const { return m_Tag; }
    void               SetTag(const std::string& tag)   { m_Tag  = tag; }

    bool CompareTag(const std::string& tag) const { return m_Tag == tag; }

private:
    std::vector<std::unique_ptr<Component>> m_Components;
    bool        m_Active   = true;
    bool        m_Destroy  = false;
    std::string m_Name;
    std::string m_Tag;
};
