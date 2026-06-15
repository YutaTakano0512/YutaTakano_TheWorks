#pragma once
#include "collider2D.h"

//=============================================================================
// BoxCollider2D コンポーネント (AABB)
//=============================================================================
class BoxCollider2D : public Collider2D
{
public:
    Vector2 size = { 1.0f, 1.0f }; // 幅と高さ

    void GetAABB(Vector2& outMin, Vector2& outMax) const override;
};
