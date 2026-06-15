#include "chakram.h"
#include "../../graphics/spriteRenderer.h"
#include "../../physics/rigidbody2D.h"
#include "../../physics/boxCollider2D.h"
#include "damageable.h" // 追加

//=============================================================================
// 初期化
//=============================================================================

void Chakram::Init()
{
    GameObject::Init();

    SetName("Chakram");
    SetTag("PlayerAttack");

    // 描画コンポーネント
    m_SpriteRenderer = AddComponent<SpriteRenderer>();
    m_SpriteRenderer->SetTexture("asset/texture/bullet/chakram.png");
    m_SpriteRenderer->size = { 64.0f, 64.0f }; 

    // 物理コンポーネント
    m_Rigidbody = AddComponent<Rigidbody2D>();
    m_Rigidbody->bodyType = Rigidbody2D::BodyType::Dynamic;
    m_Rigidbody->mass = 1.0f;
    m_Rigidbody->gravityScale = 0.0f; // 重力の影響を受けない

    m_Collider = AddComponent<BoxCollider2D>();
    m_Collider->size = { 64.0f, 64.0f };
    m_Collider->type = ColliderType::HITBOX;
    m_Collider->isTrigger = true;

    // 衝突時のコールバックを設定
    m_Collider->onCollisionEnter = [this](const CollisionInfo& info) {
        if (info.other && info.other->GetGameObject())
        {
            GameObject* target = info.other->GetGameObject();
            if (target->GetTag() == "Enemy")
            {
                Damageable* damageable = target->GetComponent<Damageable>();
                if (damageable)
                {
                    // チャクラムの移動方向（速度の正負）に合わせてノックバック方向を決定
                    float dir = (m_Rigidbody->velocity.x >= 0.0f) ? 1.0f : -1.0f;

                    DamageInfo dInfo;
                    dInfo.damage = 50.0f;
                    dInfo.knockback = { 0.0f, 0.0f };
                    dInfo.hitstopDuration = 0.1f; // 軽いヒットストップ
                    dInfo.shakeIntensity = 6.0f;
                    dInfo.shakeDuration = 0.15f;
                    dInfo.cameraShakeIntensity = 8.0f;
                    dInfo.cameraShakeDuration = 0.12f;

                    damageable->TakeDamage(dInfo);
                }
                // チャクラムを破棄
                SetDestroy();
            }
        }
    };

    // 挙動開始
    m_Rigidbody->velocity = Vector2(m_Speed, 0.0f); // 右方向に飛ばす (必要に応じて向きを調整)
}

//=============================================================================
// 更新
//=============================================================================

void Chakram::Update(float dt)
{
    GameObject::Update(dt);
    // 経過時間を更新
    m_Timer += dt;
    // 一定時間経過したら消える
    if (m_Timer >= m_Lifetime)
    {
        SetDestroy();
    }
}
