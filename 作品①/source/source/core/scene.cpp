#include "scene.h"
#include <algorithm>

//=============================================================================
// Scene 実装
//=============================================================================

void Scene::Update(float dt)
{
    for (int i = 0; i < static_cast<int>(Layer::LAYER_MAX); ++i)
    {
        for (auto& obj : m_GameObjects[i])
        {
            if (!obj->IsDestroy())
                obj->Update(dt);
        }
    }

    // 破棄フラグが立っているオブジェクトを削除
    for (int i = 0; i < static_cast<int>(Layer::LAYER_MAX); ++i)
    {
        m_GameObjects[i].remove_if([](const std::unique_ptr<GameObject>& obj)
        {
            if (obj->IsDestroy())
            {
                obj->Uninit();
                return true;
            }
            return false;
        });
    }
}

void Scene::Draw()
{
    Draw(0, static_cast<int>(Layer::LAYER_MAX));
}

void Scene::Draw(int startLayer, int endLayer)
{
    // 指定された範囲のレイヤーを描画
    for (int i = startLayer; i < endLayer; ++i)
    {
        for (auto& obj : m_GameObjects[i])
        {
            if (!obj->IsDestroy() && obj->IsActive())
                obj->Draw();
        }
    }
}

GameObject* Scene::FindGameObject(const std::string& name) const
{
    for (int i = 0; i < static_cast<int>(Layer::LAYER_MAX); ++i)
    {
        for (const auto& obj : m_GameObjects[i])
        {
            if (obj->GetName() == name)
                return obj.get();
        }
    }
    return nullptr;
}

std::vector<GameObject*> Scene::FindGameObjectsWithTag(const std::string& tag) const
{
    std::vector<GameObject*> result;
    for (int i = 0; i < static_cast<int>(Layer::LAYER_MAX); ++i)
    {
        for (const auto& obj : m_GameObjects[i])
        {
            if (obj->GetTag() == tag)
                result.push_back(obj.get());
        }
    }
    return result;
}

std::vector<GameObject*> Scene::GetAllGameObjects() const
{
    std::vector<GameObject*> result;
    for (int i = 0; i < static_cast<int>(Layer::LAYER_MAX); ++i)
    {
        for (const auto& obj : m_GameObjects[i])
            result.push_back(obj.get());
    }
    return result;
}
