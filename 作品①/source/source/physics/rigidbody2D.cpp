#include "rigidbody2D.h"
#include "../core/gameObject.h"
#include "physicsManager.h"

void Rigidbody2D::Init()
{
    PhysicsManager::AddRigidbody(this);
}

void Rigidbody2D::Uninit()
{
    PhysicsManager::RemoveRigidbody(this);
}

void Rigidbody2D::AddForce(const Vector2& force)
{
    if (bodyType != BodyType::Dynamic) return;
    m_ForceAccum += force;
}

void Rigidbody2D::AddVelocity(const Vector2& v)
{
    if (bodyType != BodyType::Dynamic) return;
    velocity += v;
}

void Rigidbody2D::Update(float dt)
{
    if (bodyType == BodyType::Static) return;

    if (bodyType == BodyType::Dynamic)
    {
        // 重力
        if (useGravity)
        {
            m_ForceAccum += PhysicsManager::GetGravity() * gravityScale * mass;
        }

        // a = F / m
        Vector2 accel = m_ForceAccum / mass;
        
        // v = v0 + at
        velocity += accel * dt;

        // 空気抵抗 v = v * (1 - drag * dt)
        velocity *= (1.0f - drag * dt);

        m_ForceAccum.zero();
    }

    // p = p0 + vt
    m_GameObject->transform.position += velocity * dt;
}
