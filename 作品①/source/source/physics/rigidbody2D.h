#pragma once
#include "../core/component.h"
#include "../math/vector2.h"

//=============================================================================
// Rigidbody2D コンポーネント
//=============================================================================
class Rigidbody2D : public Component
{
public:
    enum class BodyType { Dynamic, Kinematic, Static };

    BodyType bodyType = BodyType::Dynamic;

    Vector2 velocity = { 0.0f, 0.0f };
    float   mass     = 1.0f;
    float   gravityScale = 1.0f; // 重力の影響度
    float   drag     = 0.0f;     // 空気抵抗

    bool    useGravity = true;

    // 追加する力
    void AddForce(const Vector2& force);
    void AddVelocity(const Vector2& v);

    void Init() override;
    void Uninit() override;
    void Update(float dt) override;

private:
    Vector2 m_ForceAccum = { 0.0f, 0.0f };
};
