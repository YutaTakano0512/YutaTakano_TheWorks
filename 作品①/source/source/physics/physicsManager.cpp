#include "physicsManager.h"
#include "collider2D.h"
#include "rigidbody2D.h"
#include "../core/gameObject.h"
#include "../graphics/sprite.h"
#include "../graphics/renderer.h"
#include "../input/input.h"
#include "../game/objects/player.h"
#include <algorithm>
#include <cmath>

//=============================================================================
// 静的メンバ定義
//=============================================================================
Vector2 PhysicsManager::m_Gravity = { 0.0f, 980.0f }; // ピクセル単位/秒^2
std::vector<Collider2D*>  PhysicsManager::m_Colliders;
std::vector<Rigidbody2D*> PhysicsManager::m_Rigidbodies;
bool PhysicsManager::m_ShowDebug = false;
Sprite* PhysicsManager::m_DebugSprite = nullptr;

void PhysicsManager::Init()
{
    Clear();

    m_DebugSprite = new Sprite();
    m_DebugSprite->Init();
}

void PhysicsManager::Uninit()
{
    Clear();

    if (m_DebugSprite)
    {
        m_DebugSprite->Uninit();
        delete m_DebugSprite;
        m_DebugSprite = nullptr;
    }
}

void PhysicsManager::Clear()
{
    m_Colliders.clear();
    m_Rigidbodies.clear();
}

void PhysicsManager::AddCollider(Collider2D* col)
{
    m_Colliders.push_back(col);
}

void PhysicsManager::RemoveCollider(Collider2D* col)
{
    m_Colliders.erase(std::remove(m_Colliders.begin(), m_Colliders.end(), col), m_Colliders.end());
}

void PhysicsManager::AddRigidbody(Rigidbody2D* rb)
{
    m_Rigidbodies.push_back(rb);
}

void PhysicsManager::RemoveRigidbody(Rigidbody2D* rb)
{
    m_Rigidbodies.erase(std::remove(m_Rigidbodies.begin(), m_Rigidbodies.end(), rb), m_Rigidbodies.end());
}

//=============================================================================
// Update
//=============================================================================
void PhysicsManager::Update(float)
{
    // デバッグ表示の切り替え (F3キー)
    if (Input::GetKeyDown(VK_F3))
    {
        m_ShowDebug = !m_ShowDebug;
    }

    // O(N^2) の単純な総当たり判定 (必要なら空間分割等の最適化を入れる)
    size_t count = m_Colliders.size();
    for (size_t i = 0; i < count; ++i)
    {
        for (size_t j = i + 1; j < count; ++j)
        {
            Collider2D* colA = m_Colliders[i];
            Collider2D* colB = m_Colliders[j];

            if (!colA->enabled || !colB->enabled) continue;
            if (!colA->GetGameObject()->IsActive() || !colB->GetGameObject()->IsActive()) continue;

            Vector2 minA, maxA, minB, maxB;
            colA->GetAABB(minA, maxA);
            colB->GetAABB(minB, maxB);

            Vector2 normal;
            float depth;
            if (CheckCollisionAABB(minA, maxA, minB, maxB, normal, depth))
            {
                // スライディング中のすり抜け判定
                bool skipResolve = false;
                if (colA->GetGameObject()->GetTag() == "Player" && colB->GetGameObject()->GetTag() == "Enemy")
                {
                    Player* p = static_cast<Player*>(colA->GetGameObject());
                    if (p && p->IsSliding()) skipResolve = true;
                }
                else if (colB->GetGameObject()->GetTag() == "Player" && colA->GetGameObject()->GetTag() == "Enemy")
                {
                    Player* p = static_cast<Player*>(colB->GetGameObject());
                    if (p && p->IsSliding()) skipResolve = true;
                }

                // 押し出し解決
                if (!skipResolve)
                {
                    ResolveCollision(colA, colB, normal, depth);
                }

                // コールバック呼び出し
                CollisionInfo infoA = { colB, normal, depth };
                CollisionInfo infoB = { colA, -normal, depth };

                if (colA->onCollisionEnter) colA->onCollisionEnter(infoA);
                if (colB->onCollisionEnter) colB->onCollisionEnter(infoB);
            }
        }
    }
}

//=============================================================================
// デバッグ描画
//=============================================================================
void PhysicsManager::DrawDebug()
{
    if (!m_ShowDebug || !m_DebugSprite) return;

    Renderer::SetBlendMode(BlendMode::NORMAL);

    for (Collider2D* col : m_Colliders)
    {
        if (!col->enabled || !col->GetGameObject()->IsActive()) continue;

        DirectX::XMFLOAT4 color = { 1.0f, 1.0f, 1.0f, 0.5f }; // 白 (デフォルト)
        if (col->type == ColliderType::HURTBOX)
            color = { 0.0f, 1.0f, 0.0f, 0.9f }; // 緑
        else if (col->type == ColliderType::HITBOX)
            color = { 1.0f, 0.0f, 0.0f, 0.9f }; // 赤
        else if (col->type == ColliderType::PHYSICS)
            color = { 0.0f, 0.0f, 1.0f, 0.9f }; // 青
        Vector2 min, max;
        col->GetAABB(min, max);
        Vector2 size = { max.x - min.x, max.y - min.y };
        Vector2 center = { min.x + size.x * 0.5f, min.y + size.y * 0.5f };

        m_DebugSprite->Draw(nullptr, center, size, color, { 0, 0 }, { 1, 1 }, 0.0f, { 0.5f, 0.5f }, false, false);
    }
}

//=============================================================================
// AABB vs AABB 衝突判定 (Minkowski Differenceベース)
//=============================================================================
bool PhysicsManager::CheckCollisionAABB(
    const Vector2& minA, const Vector2& maxA,
    const Vector2& minB, const Vector2& maxB,
    Vector2& outNormal, float& outDepth)
{
    if (maxA.x < minB.x || minA.x > maxB.x) return false;
    if (maxA.y < minB.y || minA.y > maxB.y) return false;

    // 衝突している場合、重なり量を計算して押し出しベクトルを求める
    float overlapX1 = maxA.x - minB.x;
    float overlapX2 = maxB.x - minA.x;
    float overlapY1 = maxA.y - minB.y;
    float overlapY2 = maxB.y - minA.y;

    float minOverlapX = std::min(overlapX1, overlapX2);
    float minOverlapY = std::min(overlapY1, overlapY2);

    if (minOverlapX < minOverlapY)
    {
        outDepth = minOverlapX;
        outNormal = (overlapX1 < overlapX2) ? Vector2(-1, 0) : Vector2(1, 0);
    }
    else
    {
        outDepth = minOverlapY;
        outNormal = (overlapY1 < overlapY2) ? Vector2(0, -1) : Vector2(0, 1);
    }

    return true;
}

//=============================================================================
// 衝突解決 (押し出し)
//=============================================================================
void PhysicsManager::ResolveCollision(
    Collider2D* colA, Collider2D* colB,
    const Vector2& normal, float depth)
{
    if (colA->isTrigger || colB->isTrigger) return;

    Rigidbody2D* rbA = colA->GetGameObject()->GetComponent<Rigidbody2D>();
    Rigidbody2D* rbB = colB->GetGameObject()->GetComponent<Rigidbody2D>();

    bool dynA = rbA && rbA->bodyType == Rigidbody2D::BodyType::Dynamic;
    bool dynB = rbB && rbB->bodyType == Rigidbody2D::BodyType::Dynamic;

    if (dynA && dynB)
    {
        // 質量による比率分配
        float totalMass = rbA->mass + rbB->mass;
        float ratioA = rbB->mass / totalMass;
        float ratioB = rbA->mass / totalMass;

        colA->GetGameObject()->transform.position += normal * (depth * ratioA);
        colB->GetGameObject()->transform.position -= normal * (depth * ratioB);

        // 速度の法線成分を0にする
        float vA = Vector2::dot(rbA->velocity, normal);
        float vB = Vector2::dot(rbB->velocity, normal);
        if (vA < 0) rbA->velocity -= normal * vA;
        if (vB > 0) rbB->velocity -= normal * vB;
    }
    else if (dynA)
    {
        colA->GetGameObject()->transform.position += normal * depth;
        float vA = Vector2::dot(rbA->velocity, normal);
        if (vA < 0) rbA->velocity -= normal * vA;
    }
    else if (dynB)
    {
        colB->GetGameObject()->transform.position -= normal * depth;
        float vB = Vector2::dot(rbB->velocity, normal);
        if (vB > 0) rbB->velocity -= normal * vB;
    }
}

//=============================================================================
// AABBと他のPHYSICSコライダーの重複チェック
//=============================================================================
bool PhysicsManager::CheckOverlap(const Vector2& min, const Vector2& max, Collider2D* ignoreCollider, bool checkEnemy)
{
    for (Collider2D* col : m_Colliders)
    {
        if (col == ignoreCollider) continue;
        if (!col->enabled || !col->GetGameObject()->IsActive()) continue;

        bool isObstacle = (col->type == ColliderType::PHYSICS);
        if (checkEnemy && col->GetGameObject()->GetTag() == "Enemy" && col->type == ColliderType::HURTBOX)
        {
            isObstacle = true;
        }

        if (!isObstacle) continue;

        Vector2 minB, maxB;
        col->GetAABB(minB, maxB);

        // AABB 交差判定
        if (max.x > minB.x && min.x < maxB.x &&
            max.y > minB.y && min.y < maxB.y)
        {
            return true; // 重なっている
        }
    }
    return false; // 重なっていない
}
