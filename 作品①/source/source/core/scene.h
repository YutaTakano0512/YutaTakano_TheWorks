#pragma once
#include <list>
#include <vector>
#include <memory>
#include <string>
#include "gameObject.h"

//=============================================================================
// レイヤー定義 (描画・更新順)
//=============================================================================
enum class Layer : int
{
    BACKGROUND = 0,   // 背景
    FIELD,            // フィールド・地面
    OBJECT,           // キャラクター・オブジェクト
    PLAYER,           // プレイヤー
    ENEMY,            // 敵
    EFFECT,           // エフェクト
    UI,               // UI (最前面)
    LAYER_MAX
};

//=============================================================================
// Scene 基底クラス
//=============================================================================
class Scene
{
private:
    // レイヤーごとにGameObjectを管理
    std::list<std::unique_ptr<GameObject>> m_GameObjects[static_cast<int>(Layer::LAYER_MAX)];

public:
    virtual ~Scene() = default;

    virtual void Init()   {}
    virtual void Uninit()
    {
        for (int i = 0; i < static_cast<int>(Layer::LAYER_MAX); ++i)
        {
            for (auto& obj : m_GameObjects[i])
            {
                if (obj) obj->Uninit();
            }
        }
    }
    virtual void Update(float dt);
    virtual void Draw();
    void Draw(int startLayer, int endLayer);

    //-------------------------------------------------------------------------
    // GameObject 操作
    //-------------------------------------------------------------------------
    // 型を指定してGameObjectを生成・追加
    template<typename T = GameObject>
    T* AddGameObject(Layer layer = Layer::OBJECT)
    {
        static_assert(std::is_base_of<GameObject, T>::value, "T must derive from GameObject");
        auto obj = std::make_unique<T>();
        obj->Init();
        T* ptr = obj.get();
        m_GameObjects[static_cast<int>(layer)].push_back(std::move(obj));
        return ptr;
    }

    // 型で最初のGameObjectを取得
    template<typename T>
    T* GetGameObject() const
    {
        for (int i = 0; i < static_cast<int>(Layer::LAYER_MAX); ++i)
        {
            for (const auto& obj : m_GameObjects[i])
            {
                if (auto p = dynamic_cast<T*>(obj.get()))
                    return p;
            }
        }
        return nullptr;
    }

    // 型で全GameObjectを取得
    template<typename T>
    std::vector<T*> GetGameObjects() const
    {
        std::vector<T*> result;
        for (int i = 0; i < static_cast<int>(Layer::LAYER_MAX); ++i)
        {
            for (const auto& obj : m_GameObjects[i])
            {
                if (auto p = dynamic_cast<T*>(obj.get()))
                    result.push_back(p);
            }
        }
        return result;
    }

    // 名前でGameObjectを取得
    GameObject* FindGameObject(const std::string& name) const;

    // タグで全GameObjectを取得
    std::vector<GameObject*> FindGameObjectsWithTag(const std::string& tag) const;

    // 全GameObjectを取得
    std::vector<GameObject*> GetAllGameObjects() const;
};
