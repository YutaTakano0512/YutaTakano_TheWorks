#include "boxCollider2D.h"
#include "../core/gameObject.h"
#include "physicsManager.h"

//=============================================================================
// Collider2D 基底実装
//=============================================================================
void Collider2D::Init()
{
    PhysicsManager::AddCollider(this);
}

void Collider2D::Uninit()
{
    PhysicsManager::RemoveCollider(this);
}

//=============================================================================
// BoxCollider2D 実装
//=============================================================================
void BoxCollider2D::GetAABB(Vector2& outMin, Vector2& outMax) const
{
    if (!m_GameObject) return;

    Vector2 center = m_GameObject->transform.position + offset;
    Vector2 halfSize = size * 0.5f;

    // TODO: 回転を考慮する場合はここで AABB を拡張する (現在は軸並行(AABB)前提)
    
    outMin = center - halfSize;
    outMax = center + halfSize;
}
