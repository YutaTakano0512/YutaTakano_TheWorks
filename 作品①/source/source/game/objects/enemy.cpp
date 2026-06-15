#include "enemy.h"
#include "../../graphics/spriteRenderer.h"
#include "../../animation/animator.h"
#include "../../physics/rigidbody2D.h"
#include "../../physics/boxCollider2D.h"
#include "damageable.h"
#include "enemyBehavior.h"
#include "../../core/manager.h"
#include "../../core/scene.h"
#include "player.h"

Enemy::~Enemy()
{
    if (m_AttackSE1) { delete m_AttackSE1; m_AttackSE1 = nullptr; }
    if (m_AttackSE2) { delete m_AttackSE2; m_AttackSE2 = nullptr; }

    if (m_BehaviorRoot)
    {
        delete m_BehaviorRoot;
        m_BehaviorRoot = nullptr;
    }
}

//=============================================================================
// 初期化
//=============================================================================

void Enemy::Init()
{
    GameObject::Init();

    SetName("Enemy");
    SetTag("Enemy");

    // 描画コンポーネント
    m_SpriteRenderer = AddComponent<SpriteRenderer>();
    m_SpriteRenderer->SetTexture("asset/texture/character/enemy1.png");
    m_SpriteRenderer->size  = { 140.0f * 3.0f, 93.0f * 3.0f };
    m_SpriteRenderer->pivot = { 0.75f, 0.66f };
    m_SpriteRenderer->pixelArt = true; // 黒フチ防止

    // アニメーターとアニメーション
    m_Animator = AddComponent<Animator>();
    m_Animator->colCount = 8;
    m_Animator->rowCount = 8;

    SetupAnimations();
    m_Animator->Play("Idle");

    // 物理・コライダー・コンポーネント
    SetupComponents();

    // ビヘイビアツリー
    m_BehaviorRoot = new BehaviorSelector();

    // 遠距離 (距離 250以上) -> 遠距離攻撃 または 急接近ダッシュ
    auto farCondition = new BehaviorCondition([](Enemy* e, float dist) { return dist >= 250.0f; });
    auto farAction = new BehaviorSequence();
    farAction->AddChild(new BehaviorRandomWait(0.1f, 0.4f));
    farAction->AddChild(new BehaviorGun());                      // 遠距離攻撃
    farAction->AddChild(new BehaviorRandomWait(0.3f, 0.8f));
    farAction->AddChild(new BehaviorDash(0.3f, 500.0f, true));   // プレイヤー側へダッシュ
    farCondition->AddChild(farAction);
    m_BehaviorRoot->AddChild(farCondition);

    // 近距離 (距離 120未満) -> 近接攻撃 または バックステップ
    auto nearCondition = new BehaviorCondition([](Enemy* e, float dist) { return dist < 120.0f; });
    auto nearAction = new BehaviorSequence();
    nearAction->AddChild(new BehaviorAttack());                  // 近接攻撃1
    nearAction->AddChild(new BehaviorRandomWait(0.1f, 0.3f));
    nearAction->AddChild(new BehaviorPunch());                   // 近接攻撃2
    nearAction->AddChild(new BehaviorDash(0.2f, 400.0f, false)); // バックステップ
    nearAction->AddChild(new BehaviorRandomWait(0.3f, 0.6f));
    nearCondition->AddChild(nearAction);
    m_BehaviorRoot->AddChild(nearCondition);

    // 中距離 (それ以外) -> じりじり距離を詰める
    auto midAction = new BehaviorSequence();
    midAction->AddChild(new BehaviorMoveToPlayer(100.0f));
    midAction->AddChild(new BehaviorRandomWait(0.1f, 0.4f));
    m_BehaviorRoot->AddChild(midAction);

    // SEのロード
    m_AttackSE1 = new Audio();
    m_AttackSE1->Load("asset/audio/SE/EnemyAttack1_SE.wav");
    m_AttackSE2 = new Audio(); 
    m_AttackSE2->Load("asset/audio/SE/EnemyAttack2_SE.wav");
}

//=============================================================================
// アニメーションのセットアップ
//=============================================================================

void Enemy::SetupAnimations()
{
    // 待機
    AnimationClip idle("Idle");
    idle.AddFrames(0, 0, 8, 0.2f);
    m_Animator->AddClip(idle);

    // 走り
    AnimationClip run("Run");
    run.AddFrames(0, 1, 8, 0.2f);
    m_Animator->AddClip(run);

    // 被弾
    AnimationClip hurt("Hurt", false);
    hurt.AddFrames(2, 3, 3, 0.15f);
    m_Animator->AddClip(hurt);

    // 近接攻撃 (Attack1)
    AnimationClip attack1("Attack1", false);
    attack1.AddFrames(0, 2, 4, 0.2f);
    attack1.AddFramesWithHitbox(4, 2, 1, 0.1f, {280.0f, 186.0f}, {190.0f, 0.0f});
    attack1.AddFrames(5, 2, 3, 0.1f);                                              
    attack1.AddFrames(0, 3, 2, 0.1f);                                              
    m_Animator->AddClip(attack1);

    // 遠距離攻撃 (Attack2)
    AnimationClip attack2("Attack2", false);
    attack2.AddFrames(7, 4, 1, 0.15f);
    attack2.AddFrames(0, 5, 8, 0.15f);
    m_Animator->AddClip(attack2);

    // 死亡
    AnimationClip death("Death", false);
    death.AddFrames(5, 3, 3, 0.15f);
    death.AddFrames(0, 4, 7, 0.15f);
    m_Animator->AddClip(death);
}

//=============================================================================
// 物理・コライダー・ダメージコンポーネントのセットアップ
//=============================================================================
void Enemy::SetupComponents()
{
    // 物理
    m_Rigidbody = AddComponent<Rigidbody2D>();
    m_Rigidbody->bodyType    = Rigidbody2D::BodyType::Dynamic;
    m_Rigidbody->mass        = 1.0f;
    m_Rigidbody->gravityScale = 1.0f;

    // 地形・やられ判定
    m_Collider = AddComponent<BoxCollider2D>();
    m_Collider->size   = { 100.0f, 93.0f * 2.0f };
    m_Collider->offset = { 0.0f, 0.0f };
    m_Collider->type   = ColliderType::HURTBOX;
    m_Collider->onCollisionEnter = [this](const CollisionInfo& info)
    {
        if (info.normal.y == -1.0f)
        {
            m_IsGrounded = true;
        }
    };

    // 攻撃判定
    m_Hitbox = AddComponent<BoxCollider2D>();
    m_Hitbox->type      = ColliderType::HITBOX;
    m_Hitbox->isTrigger = true;
    m_Hitbox->enabled   = false;

    m_Animator->SetHitbox(m_Hitbox);

    // 攻撃ヒット時のコールバック
    m_Hitbox->onCollisionEnter = [this](const CollisionInfo& info)
    {
        if (!info.other || !info.other->GetGameObject()) return;

        GameObject* target = info.other->GetGameObject();
        if (target->GetTag() != "Player") return;

        Damageable* damageable = target->GetComponent<Damageable>();
        if (!damageable) return;

        float dir = (target->transform.position.x >= transform.position.x) ? 1.0f : -1.0f;

        DamageInfo dInfo;
        dInfo.damage = 5.0f;// 近接攻撃のダメージ値
        dInfo.knockback = { dir * 250.0f, -150.0f };
        dInfo.hitstopDuration = 0.08f;
        dInfo.shakeIntensity = 6.0f;
        dInfo.shakeDuration = 0.15f;
        dInfo.cameraShakeIntensity = 4.0f;
        dInfo.cameraShakeDuration = 0.12f;

        damageable->TakeDamage(dInfo);
    };

    // ダメージ受取コンポーネント
    m_Damageable = AddComponent<Damageable>();
    m_Damageable->SetMaxHp(25.0f);

    // 被弾
    m_Damageable->SetOnTakeDamage([this](const DamageInfo& info)
    {
        // スーパーアーマー中はのけぞらない（ダメージ値は Damageable 内部で処理済み）
        if (m_Damageable->IsSuperArmor())
        {
            // 必要ならエフェクトや音だけ鳴らす
            return;
        }

        m_Animator->Play("Hurt", true);

        if (info.knockback.y != 0.0f)
        {
            m_IsGrounded = false;
        }
        if (m_Hitbox)
        {
            m_Hitbox->enabled = false;
        }
        if (m_BehaviorRoot)
        {
            m_BehaviorRoot->Reset();
        }
    });

    // 死亡時
    m_Damageable->SetOnDeath([this]()
    {
        m_Animator->Play("Death", true);
        m_Collider->enabled = false;
        if (m_Hitbox)
        {
            m_Hitbox->enabled = false;
        }
        m_Rigidbody->velocity = { 0.0f, 0.0f };
        m_Rigidbody->bodyType = Rigidbody2D::BodyType::Static;
    });
}

//=============================================================================
// 更新
//=============================================================================

void Enemy::Update(float dt)
{
    GameObject::Update(dt);

    if (m_Damageable && m_Damageable->IsDead())
    {
        if (m_Animator->GetCurrentClipName() == "Death" && m_Animator->IsFinished())
        {
            if (m_DestroyOnDeath)
            {
                SetDestroy();
            }
        }
        return;
    }

    // 向きの反転
    if (m_Animator->GetCurrentClipName() != "Hurt")
    {
        Player* player = Manager::GetScene()->GetGameObject<Player>();
        if (player)
        {
            float dirX  = player->transform.position.x - transform.position.x;
            float scaleX = std::abs(transform.scale.x);
            if (dirX != 0.0f)
            {
                transform.scale.x = (dirX > 0.0f) ? -scaleX : scaleX;
            }
        }
    }

    // スーパーアーマー
    if (m_Damageable)
    {
        bool isAttacking = (m_Animator->GetCurrentClipName().find("Attack") != std::string::npos);
        m_Damageable->SetSuperArmor(isAttacking);
    }

    if (m_Hitbox)
    {
        m_Hitbox->offset.x = (transform.scale.x < 0.0f) ? 80.0f : -80.0f;
    }

    if (m_Animator->GetCurrentClipName() == "Hurt" && m_Animator->IsFinished())
    {
        m_Animator->Play("Idle");
        if (m_Rigidbody)
        {
            m_Rigidbody->velocity.x = 0.0f;
        }
        if (m_BehaviorRoot)
        {
            m_BehaviorRoot->Reset();
        }
    }

    if (m_Animator->GetCurrentClipName() == "Hurt")
    {
        if (m_Rigidbody)
        {
            float friction = m_IsGrounded ? 8.0f : 2.0f;
            m_Rigidbody->velocity.x -= m_Rigidbody->velocity.x * friction * dt;
        }
        return;
    }

    if (m_BehaviorRoot && !m_BehaviorLocked)
    {
        m_BehaviorRoot->Update(this, dt);
    }
}

//=============================================================================
// ビヘイビアロック設定
//=============================================================================

void Enemy::SetBehaviorLocked(bool locked)
{
    m_BehaviorLocked = locked;

    if (locked)
    {
        if (m_Rigidbody)
        {
            m_Rigidbody->velocity.x = 0.0f;
        }
        if (m_BehaviorRoot)
        {
            m_BehaviorRoot->Reset();
        }
        if (m_Animator)
        {
            m_Animator->Play("Idle");
        }
    }
    else
    {
        if (m_BehaviorRoot)
        {
            m_BehaviorRoot->Reset();
        }
    }
}

void Enemy::PlayAttackSE(int type)
{
    if (type == 1 && m_AttackSE1) m_AttackSE1->Play();
    else if (type == 2 && m_AttackSE2) m_AttackSE2->Play();
}
