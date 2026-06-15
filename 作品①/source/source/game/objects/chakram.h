#pragma once
#include "../../core/gameObject.h"

class SpriteRenderer;
class Rigidbody2D;
class BoxCollider2D;

//=============================================================================
// Chakram オブジェクト
//=============================================================================
class Chakram : public GameObject
{
private:
    SpriteRenderer* m_SpriteRenderer = nullptr;
    Rigidbody2D* m_Rigidbody = nullptr;
    BoxCollider2D* m_Collider = nullptr;

    float m_Speed = 1000.0f; // 飛行速度
    float m_Lifetime = 1.0f; // 存在時間 (秒)
    float m_Timer = 0.0f;    // 経過時間

public:
    void Init() override;
    void Update(float dt) override;
};
