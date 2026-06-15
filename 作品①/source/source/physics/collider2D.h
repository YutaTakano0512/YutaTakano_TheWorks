#pragma once
#include "../core/component.h"
#include "../math/vector2.h"
#include <functional>

//=============================================================================
// 衝突情報
//=============================================================================
struct CollisionInfo
{
    class Collider2D* other;   // 衝突相手のコライダー
    Vector2 normal;            // 衝突法線
    float depth;               // めり込み量
};

enum class ColliderType
{
    PHYSICS, // 地形や壁との物理衝突用
    HURTBOX, // 当たり判定（やられ判定）
    HITBOX   // 攻撃判定
};

//=============================================================================
// Collider2D 基底クラス
//=============================================================================
class Collider2D : public Component
{
public:
    ColliderType type = ColliderType::PHYSICS; // デフォルトはPHYSICS
    bool isTrigger = false;    // trueなら物理的な押し出しをせず、判定だけ行う
    Vector2 offset = {0, 0};   // Transformからのローカルオフセット

    // コールバック
    std::function<void(const CollisionInfo&)> onCollisionEnter;
    std::function<void(const CollisionInfo&)> onCollisionStay;
    std::function<void(const CollisionInfo&)> onCollisionExit;

    virtual ~Collider2D() override = default;

    void Init() override;
    void Uninit() override;

    // ワールド座標系でのAABB取得
    virtual void GetAABB(Vector2& outMin, Vector2& outMax) const = 0;
};
