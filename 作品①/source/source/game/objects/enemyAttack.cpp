#include "enemyAttack.h"
#include "../../graphics/spriteRenderer.h"
#include "../../physics/boxCollider2D.h"
#include "../../animation/animator.h"
#include "damageable.h"

//=============================================================================
// 初期化
//=============================================================================

void EnemyAttack::Init()
{
    GameObject::Init();

    SetName("EnemyAttack");
    SetTag("EnemyAttack");

    // 描画コンポーネント
    m_SpriteRenderer = AddComponent<SpriteRenderer>();
    m_SpriteRenderer->SetTexture("asset/texture/character/enemy1.png");
    m_SpriteRenderer->size  = { 140.0f * 4.0f, 93.0f * 4.0f };
    m_SpriteRenderer->pivot = { 0.48f, 0.65f };
    m_SpriteRenderer->pixelArt = true; // 黒フチ防止

    // アニメーターとアニメーション
    m_Animator = AddComponent<Animator>();
    m_Animator->colCount = 8;
    m_Animator->rowCount = 8;
    SetupAnimations();
    m_Animator->Play("Attack");

    // 当たり判定・ダメージコンポーネント
    SetupComponents();
}

//=============================================================================
// アニメーションのセットアップ
//=============================================================================

void EnemyAttack::SetupAnimations()
{
    AnimationClip attack("Attack", false);
    attack.AddFrames(0, 6, 7, 0.1f); 
    attack.AddFramesWithHitbox(7, 6, 1, 0.1f, {150.0f, 225.0f}, {0.0f, 25.0f});
    attack.AddFrames(0, 7, 8, 0.1f);
    m_Animator->AddClip(attack);
}

//=============================================================================
// コライダー・ダメージコンポーネントのセットアップ
//=============================================================================

void EnemyAttack::SetupComponents()
{
    // 攻撃判定
    m_Hitbox = AddComponent<BoxCollider2D>();
    m_Hitbox->type      = ColliderType::HITBOX;
    m_Hitbox->isTrigger = true;

    m_Animator->SetHitbox(m_Hitbox);

    // ヒット時のコールバック
    m_Hitbox->onCollisionEnter = [this](const CollisionInfo& info)
    {
        if (!info.other || !info.other->GetGameObject()) return;

        GameObject* target = info.other->GetGameObject();
        if (target->GetTag() != "Player") return;

        Damageable* damageable = target->GetComponent<Damageable>();
        if (!damageable) return;

        float dir = (target->transform.position.x >= transform.position.x) ? 1.0f : -1.0f;

        DamageInfo dInfo;
        dInfo.damage               = 2.0f;
        dInfo.knockback            = { dir * 250.0f, -150.0f };
        dInfo.hitstopDuration      = 0.08f;
        dInfo.shakeIntensity       = 6.0f;
        dInfo.shakeDuration        = 0.15f;
        dInfo.cameraShakeIntensity = 4.0f;
        dInfo.cameraShakeDuration  = 0.12f;

        damageable->TakeDamage(dInfo);
    };
}

//=============================================================================
// 更新
//=============================================================================

void EnemyAttack::Update(float dt)
{
    GameObject::Update(dt);

    if (m_Animator && m_Animator->GetCurrentClipName() == "Attack" && m_Animator->IsFinished())
    {
        SetDestroy();
    }
}
