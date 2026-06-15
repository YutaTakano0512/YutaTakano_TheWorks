#include "player.h"
#include "../../graphics/spriteRenderer.h"
#include "../../animation/animator.h"
#include "../../physics/rigidbody2D.h"
#include "../../physics/boxCollider2D.h"
#include "../../physics/physicsManager.h"
#include "../../input/input.h"
#include "../../input/gamepad.h"
#include "chakram.h"
#include "damageable.h" // 追加
#include "../../core/scene.h"
#include "../../core/manager.h"
#include "../../audio/audio.h"
#include <algorithm> // std::find用

//=============================================================================
// 初期化
//=============================================================================

void Player::Init()
{
    GameObject::Init();

    SetName("Player");
    SetTag("Player");

    // 描画コンポーネント
    m_SpriteRenderer = AddComponent<SpriteRenderer>();
    m_SpriteRenderer->SetTexture("asset/texture/character/player2.png");
    m_SpriteRenderer->size = { 69.0f * 2.0f, 44.0f * 2.0f };
    m_SpriteRenderer->pivot = { 0.4f, 0.6f };
    m_SpriteRenderer->pixelArt = true;

    // アニメーターコンポーネント
    m_Animator = AddComponent<Animator>();
    m_Animator->colCount = 6;
    m_Animator->rowCount = 17;
    
    // アニメーションのセットアップ
    SetupAnimations();

    m_Animator->Play("Idle");

    // 物理コンポーネント
    m_Rigidbody = AddComponent<Rigidbody2D>();
    m_Rigidbody->bodyType = Rigidbody2D::BodyType::Dynamic;
    m_Rigidbody->mass = 1.0f;
    m_Rigidbody->gravityScale = 1.0f;

    // 当たり判定コンポーネント (地形 兼 やられ判定)
    m_Collider = AddComponent<BoxCollider2D>();
    m_Collider->size = { 40.0f, 70.0f };
    m_Collider->offset = { 0.0f, 0.0f };
    m_Collider->type = ColliderType::HURTBOX;

    // 攻撃判定コンポーネント (Hitbox)
    m_Hitbox = AddComponent<BoxCollider2D>();
    m_Hitbox->size = { 70.0f, 60.0f };
    m_Hitbox->type = ColliderType::HITBOX;
    m_Hitbox->isTrigger = true;
    m_Hitbox->enabled = false;

    // 攻撃判定の衝突コールバックを設定
    m_Hitbox->onCollisionEnter = [this](const CollisionInfo& info) {
        if (info.other)
            OnHitboxTrigger(info);
    };

    // AnimatorにHitboxを登録
    m_Animator->SetHitbox(m_Hitbox);

    // 衝突コールバック
    m_Collider->onCollisionEnter = [this](const CollisionInfo& info) {
        if (info.normal.y == -1.0f)
        {
            m_IsGrounded = true;
        }
      };

    // ダメージを受けるコンポーネント
    m_Damageable = AddComponent<Damageable>();
    m_Damageable->SetMaxHp(20.0f);

	// 被ダメージ時のコールバック
    m_Damageable->SetOnTakeDamage([this](const DamageInfo& info) {
        m_Animator->Play("Hurt");

        if (info.knockback.y != 0.0f)
        {
            m_IsGrounded = false;
        }
        
        if (m_Hitbox)
        {
            m_Hitbox->enabled = false;
        }
        
        if (m_IsSliding)
        {
            m_IsSliding = false;
            m_Collider->size = { 40.0f, 70.0f };
            m_Collider->offset = { 0.0f, 0.0f };
            m_SlidingCooldownTimer = m_SlidingCooldown;
        }
        
        m_ComboStep = 0;
        m_ComboQueued = false;
        m_HitObjects.clear();
        });

    // 死亡時のコールバック
    m_Damageable->SetOnDeath([this]() {
        m_Animator->Play("Death");
        m_Collider->enabled = false;
        if (m_Hitbox)
        {
            m_Hitbox->enabled = false;
        }
        m_Rigidbody->velocity = { 0.0f, 0.0f };
        m_Rigidbody->bodyType = Rigidbody2D::BodyType::Static;
        });

    // SEのロード
    m_JumpSE = new Audio;
    m_JumpSE->Load("asset/audio/SE/Jump_SE.wav");
    m_SlidingSE = new Audio;
    m_SlidingSE->Load("asset/audio/SE/Sliding_SE.wav");
    m_PlayerRunSE = new Audio;
    m_PlayerRunSE->Load("asset/audio/SE/PlayerRun_SE.wav");
    m_PlayerDeathSE = new Audio;
    m_PlayerDeathSE->Load("asset/audio/SE/PlayerDeath_SE.wav");
    m_ComboAttackSE1 = new Audio;
    m_ComboAttackSE1->Load("asset/audio/SE/ComboAttack1_SE.wav");
    m_SwordSwingSE = new Audio(); m_SwordSwingSE->Load("asset/audio/SE/SwordSwing_SE.wav");
}

void Player::Uninit()
{
    if (m_ComboAttackSE1) { delete m_ComboAttackSE1; m_ComboAttackSE1 = nullptr; }
    if (m_JumpSE) { delete m_JumpSE; m_JumpSE = nullptr; }
    if (m_PlayerDeathSE) { delete m_PlayerDeathSE; m_PlayerDeathSE = nullptr; }
    if (m_PlayerRunSE) { delete m_PlayerRunSE; m_PlayerRunSE = nullptr; }
    if (m_SlidingSE) { delete m_SlidingSE; m_SlidingSE = nullptr; }
    if (m_SwordSwingSE) { delete m_SwordSwingSE; m_SwordSwingSE = nullptr; }

    GameObject::Uninit();
}

Player::~Player()
{
    Uninit();
}

//=============================================================================
// 更新
//=============================================================================

void Player::Update(float dt)
{
    GameObject::Update(dt);

    if (m_SwordSwingDelayTimer > 0.0f)
    {
        m_SwordSwingDelayTimer -= dt;
        if (m_SwordSwingDelayTimer <= 0.0f)
        {
            if (m_SwordSwingSE) m_SwordSwingSE->Play();
        }
    }

    if (IsDead())
    {
        m_Rigidbody->velocity = { 0.0f, 0.0f };
        HandleAnimationState(false, 0.0f);
        return;
    }

    // 入力取得
    float moveX = 0.0f;
    bool meleeInput = false;
    bool jumpInput = false;
    bool slidingInput = false;

    if (!IsHurt() && !m_InputLocked)
    {
        moveX = GetMoveInput();
        meleeInput = GetMeleeInput();
        jumpInput = GetJumpInput();
        slidingInput = GetSlidingInput();
    }

    if (!m_InputLocked)
    {
		// 遠距離攻撃は一旦保留
        //HandleRangedAttack(dt);
    }
    HandleSliding(slidingInput, dt);
    HandleMovement(moveX, dt);
    HandleJump(jumpInput);
    
    if (!m_InputLocked)
    {
        HandleComboStateMachine(meleeInput, moveX);
        HandleAnimationState(meleeInput, moveX);
    }

    // コンボタイマーの更新
    if (m_ComboStep > 0)
    {
        m_ComboTimer += dt;
    }
    else
    {
        m_ComboTimer = 0.0f;
    }

    // スライディング中の無敵設定
    if (m_Damageable)
    {
        m_Damageable->SetInvincible(m_IsSliding);
    }

    // 足音ループの制御 (m_IsGroundedのバタつきを考慮し、Runアニメーション中なら鳴らす)
    if (m_Animator && m_Animator->GetCurrentClipName() == "Run" && !m_IsSliding)
    {
        if (m_PlayerRunSE && !m_PlayerRunSE->IsPlaying()) m_PlayerRunSE->Play(true);
    }
    else
    {
        if (m_PlayerRunSE && m_PlayerRunSE->IsPlaying()) m_PlayerRunSE->Stop();
    }
}

//=============================================================================
// 入力取得
//=============================================================================

float Player::GetMoveInput() const
{
    float moveX = 0.0f;
    if (Input::GetKey(VK_LEFT)  || Input::GetKey('A'))  moveX -= 1.0f;
    if (Input::GetKey(VK_RIGHT) || Input::GetKey('D'))  moveX += 1.0f;
    if (Gamepad::IsConnected())
    {
        moveX += Gamepad::GetLeftStick().x;
        if (Gamepad::GetButton(XINPUT_GAMEPAD_DPAD_LEFT))  moveX -= 1.0f;
        if (Gamepad::GetButton(XINPUT_GAMEPAD_DPAD_RIGHT)) moveX += 1.0f;
    }
    return moveX;
}

bool Player::GetMeleeInput() const
{
    return Input::GetKeyDown('K') ||
           (Gamepad::IsConnected() && Gamepad::GetButtonDown(XINPUT_GAMEPAD_B));
}

bool Player::GetRangedInput() const
{
    return Input::GetKeyDown('J') ||
           (Gamepad::IsConnected() && Gamepad::GetButtonDown(XINPUT_GAMEPAD_X));
}

bool Player::GetJumpInput() const
{
    return Input::GetKeyDown(VK_SPACE) ||
           (Gamepad::IsConnected() && Gamepad::GetButtonDown(XINPUT_GAMEPAD_A));
}

bool Player::GetSlidingInput() const
{
    return Input::GetKeyDown(VK_LSHIFT) ||
        (Gamepad::IsConnected() && Gamepad::GetButtonDown(XINPUT_GAMEPAD_Y));
}

//=============================================================================
// 遠距離攻撃 (チャクラム発射)
//=============================================================================
/*
void Player::HandleRangedAttack(float dt)
{
    // クールダウン更新
    if (m_AttackTimer > 0.0f)
        m_AttackTimer -= dt;

    if (!GetRangedInput() || m_AttackTimer > 0.0f) return;

    m_AttackTimer = m_AttackCooldown;

    Scene* scene = Manager::GetScene();
    if (!scene) return;

    Chakram* chakram = scene->AddGameObject<Chakram>(Layer::EFFECT);
    if (!chakram) return;

    // 発射位置をプレイヤーの前方に設定
    Vector2 spawnOffset = { 40.0f, 0.0f };
    if (m_SpriteRenderer->flipX)
    {
        spawnOffset.x = -spawnOffset.x;
    }
    chakram->transform.position = transform.position + spawnOffset;

    // 向きに合わせて速度・描画を反転
    Rigidbody2D* crb = chakram->GetComponent<Rigidbody2D>();
    if (crb && m_SpriteRenderer->flipX)
    {
        crb->velocity.x = -crb->velocity.x;
    }

    SpriteRenderer* csr = chakram->GetComponent<SpriteRenderer>();
    if (csr)
    {
        csr->flipX = m_SpriteRenderer->flipX;
    }
}
*/

//=============================================================================
// 横移動・向き反転
//=============================================================================

void Player::HandleMovement(float moveX, float dt)
{
    if (IsDead()) return;

    // 被弾中は操作不能
    if (IsHurt())
    {
        float friction = m_IsGrounded ? 8.0f : 2.0f;
        m_Rigidbody->velocity.x -= m_Rigidbody->velocity.x * friction * dt;
        return;
    }

    // スライディング中は入力を無効化
    if (m_IsSliding)
    {
        return;
    }

    // コンボ攻撃中は移動を受け付けない
    if (m_ComboStep > 0)
    {
        m_Rigidbody->velocity.x = 0.0f;
        return;
    }

    if (moveX != 0.0f)
    {
        m_Rigidbody->velocity.x   = moveX * m_MoveSpeed;
        m_SpriteRenderer->flipX   = (moveX < 0.0f);
    }
    else
    {
        m_Rigidbody->velocity.x = 0.0f;
    }
}

//=============================================================================
// ジャンプ・接地判定
//=============================================================================

void Player::HandleJump(bool jumpInput)
{
    if (jumpInput && m_IsGrounded)
    {
        if (m_IsSliding)
        {
            Vector2 checkSize = { 38.0f, 35.0f };
            Vector2 checkOffset = { 0.0f, -17.5f };
            Vector2 center = transform.position + checkOffset;
            Vector2 halfSize = checkSize * 0.5f;
            Vector2 min = center - halfSize;
            Vector2 max = center + halfSize;

            if (!PhysicsManager::CheckOverlap(min, max, m_Collider, true))
            {
                m_IsSliding = false;
                m_Collider->size = { 40.0f, 70.0f };
                m_Collider->offset = { 0.0f, 0.0f };
                m_SlidingCooldownTimer = m_SlidingCooldown;

                m_Rigidbody->velocity.y = m_JumpForce;
                m_IsGrounded = false;
                if (m_JumpSE) m_JumpSE->Play();
            }
        }
        else
        {
            m_Rigidbody->velocity.y = m_JumpForce;
            m_IsGrounded = false;
            if (m_JumpSE) m_JumpSE->Play();
        }
    }

    if (abs(m_Rigidbody->velocity.y) > 20.0f)
        m_IsGrounded = false;
}

//=============================================================================
// スライディング
//=============================================================================
void Player::HandleSliding(bool slidingInput, float dt)
{
    // クールダウンタイマーの更新
    if (m_SlidingCooldownTimer > 0.0f)
    {
        m_SlidingCooldownTimer -= dt;
    }

    // スライディング中
    if (m_IsSliding)
    {
        m_SlidingTimer -= dt;
        if (m_SlidingTimer <= 0.0f)
        {
            Vector2 checkSize = { 38.0f, 35.0f };
            Vector2 checkOffset = { 0.0f, -17.5f };
            Vector2 center = transform.position + checkOffset;
            Vector2 halfSize = checkSize * 0.5f;
            Vector2 min = center - halfSize;
            Vector2 max = center + halfSize;

            if (!PhysicsManager::CheckOverlap(min, max, m_Collider, true))
            {
                m_IsSliding = false;
                m_Collider->size = { 40.0f, 70.0f };
                m_Collider->offset = { 0.0f, 0.0f };
                m_SlidingCooldownTimer = m_SlidingCooldown;
            }
            else
            {
                m_Rigidbody->velocity.x = m_SlidingDir * (m_SlidingForce * 0.3f);
            }
        }
        else
        {
            m_Rigidbody->velocity.x = m_SlidingDir * m_SlidingForce;
        }
    }
    else if (slidingInput && m_IsGrounded && m_SlidingCooldownTimer <= 0.0f)
    {
        m_IsSliding = true;
        m_SlidingTimer = m_SlidingDuration;
        m_SlidingDir = m_SpriteRenderer->flipX ? -1.0f : 1.0f;
        if (m_SlidingSE) m_SlidingSE->Play();

        if (m_ComboStep > 0)
        {
            m_ComboStep = 0;
            m_ComboQueued = false;
            m_HitObjects.clear();
        }

        m_Collider->size = { 40.0f, 35.0f };
        m_Collider->offset = { 0.0f, 17.5f };

        m_Rigidbody->velocity.x = m_SlidingDir * m_SlidingForce;
    }
}

//=============================================================================
// コンボ攻撃 ステートマシン
//=============================================================================

void Player::HandleComboStateMachine(bool input, float moveX)
{
    if (m_IsSliding)
    {
        return;
    }

    // 1段目終了 → 2段目への連携 or リセット
    if (m_ComboStep == 1 && m_Animator->IsFinished())
    {
        if (m_ComboQueued)
        {
            if (moveX != 0.0f) m_SpriteRenderer->flipX = (moveX < 0.0f);
            m_Animator->Play("ComboAttack2", true);
            if (m_SwordSwingSE) m_SwordSwingSE->Play();
            m_ComboStep   = 2;
            m_ComboQueued = false;
            m_ComboTimer  = 0.0f;
            m_HitObjects.clear();
        }
        else
        {
            m_ComboStep = 0;
            m_HitObjects.clear();
        }
        return;
    }

    // 2段目終了 → 3段目への連携 or リセット
    if (m_ComboStep == 2 && m_Animator->IsFinished())
    {
        if (m_ComboQueued)
        {
            if (moveX != 0.0f) m_SpriteRenderer->flipX = (moveX < 0.0f);
            m_Animator->Play("ComboAttack3", true);
            if (m_SwordSwingSE) m_SwordSwingSE->Play();
            m_ComboStep   = 3;
            m_ComboQueued = false;
            m_ComboTimer  = 0.0f;
            m_HitObjects.clear();
        }
        else
        {
            m_ComboStep = 0;
            m_HitObjects.clear();
        }
        return;
    }

    // 3段目終了 → リセット
    if (m_ComboStep == 3 && m_Animator->IsFinished())
    {
        m_ComboStep = 0;
        m_HitObjects.clear();
        return;
    }

    // 入力受付
    if (!input) return;

    if (m_ComboStep == 0)
    {
        if (moveX != 0.0f) m_SpriteRenderer->flipX = (moveX < 0.0f);
        m_Animator->Play("ComboAttack1", true);
        
        // 初段は少し遅れて素振り音を鳴らす
        m_SwordSwingDelayTimer = 0.2f; // 0.15秒のディレイ（必要に応じて調整可能）
        
        m_ComboStep   = 1;
        m_ComboQueued = false;
        m_ComboTimer  = 0.0f;
        m_HitObjects.clear();
    }
    else if (m_ComboStep == 1 && !m_Animator->IsFinished())
    {
        // 1段目再生中に入力 → 2段目をキューに積む
        m_ComboQueued = true;
    }
    else if (m_ComboStep == 2 && !m_Animator->IsFinished())
    {
        // 2段目再生中に入力 → 3段目をキューに積む
        m_ComboQueued = true;
    }
}

//=============================================================================
// アニメーション遷移
//=============================================================================

void Player::HandleAnimationState(bool meleeInput, float moveX)
{
    // 死亡時
    if (IsDead())
    {
        if (m_Animator->GetCurrentClipName() != "Death")
        {
            m_Animator->Play("Death");
            if (m_PlayerDeathSE) m_PlayerDeathSE->Play();
        }
        return;
    }

    // 被弾時
    if (IsHurt())
    {
        return;
    }

    // 空中
    if (!m_IsGrounded)
    {
        m_Animator->Play("Jump");
        m_ComboStep   = 0;
        m_ComboQueued = false;
        if (m_IsSliding)
        {
            m_IsSliding = false;
            m_Collider->size = { 40.0f, 70.0f };
            m_Collider->offset = { 0.0f, 0.0f };
        }
        return;
    }

    // スライディング中
    if (m_IsSliding)
    {
        m_Animator->Play("Sliding");
        return;
    }

    // コンボ攻撃中
    if (m_ComboStep > 0) return;

    // 走り
    if (moveX != 0.0f)
    {
        m_Animator->Play("Run");
        return;
    }

    // 待機
    m_Animator->Play("Idle");
}

//=============================================================================
// 攻撃判定（Hitbox）の衝突コールバック
//=============================================================================

void Player::OnHitboxTrigger(const CollisionInfo& info)
{
    // 衝突相手が有効かチェック
    if (!info.other || !info.other->GetGameObject())
    {
        return;
    }

    GameObject* target = info.other->GetGameObject();

    if (target->GetTag() != "Enemy")
    {
        return;
    }

	// 多段ヒット防止
    auto it = std::find(m_HitObjects.begin(), m_HitObjects.end(), target);
    if (it != m_HitObjects.end())
    {
        return;
    }

    // ヒットリストに登録
    m_HitObjects.push_back(target);

    // 敵の Damageable コンポーネントを取得してダメージを与える
    Damageable* damageable = target->GetComponent<Damageable>();
    if (damageable)
    {
        // Hit SEの再生 (ボスの音と重なるためAttack1に統一)
        if (m_ComboAttackSE1) m_ComboAttackSE1->Play();

        // プレイヤーの向きに合わせてノックバックの方向を決定
        float dir = m_SpriteRenderer->flipX ? -1.0f : 1.0f;

        DamageInfo info;
        if (m_ComboStep == 1 || m_ComboStep == 2)
        {
            info.damage = 1.0f;
            info.knockback = { dir * 100.0f, -10.0f }; // 軽く吹き飛ばす
            info.hitstopDuration = 0.1f;               // 0.1秒ヒットストップ
            info.shakeIntensity = 8.0f;                 // 敵のヒットシェイク強度
            info.shakeDuration = 0.15f;                 // ヒットシェイク時間
            info.cameraShakeIntensity = 3.0f;           // カメラシェイク強度
            info.cameraShakeDuration = 0.1f;            // カメラシェイク時間
        }
        else if (m_ComboStep == 3) // コンボフィニッシュ
        {
            info.damage = 2.0f;
            info.knockback = { dir * 200.0f, -50.0f }; // 大きく吹き飛ばす
            info.hitstopDuration = 0.18f;               // 長めのヒットストップ
            info.shakeIntensity = 15.0f;                // 激しくヒットシェイク
            info.shakeDuration = 0.25f;
            info.cameraShakeIntensity = 8.0f;           // カメラも大きく揺らす
            info.cameraShakeDuration = 0.2f;
        }

        // ダメージ適用
        damageable->TakeDamage(info);
    }
}

//=============================================================================
// 状態ヘルパー
//=============================================================================
bool Player::IsHurt() const
{
    return m_Animator && m_Animator->GetCurrentClipName() == "Hurt" && !m_Animator->IsFinished();
}

bool Player::IsDead() const
{
    return m_Damageable && m_Damageable->IsDead();
}

//=============================================================================
// アニメーションセットアップ
//=============================================================================
void Player::SetupAnimations()
{
    if (!m_Animator) return;

    // 待機
    AnimationClip idle("Idle");
    idle.AddFrames(0, 0, 6, 0.2f);
    m_Animator->AddClip(idle);

    // 走り
    AnimationClip run("Run");
    run.AddFrames(0, 1, 6, 0.1f);
    run.AddFrames(0, 2, 2, 0.1f);
    m_Animator->AddClip(run);

    // ジャンプ
    AnimationClip jump("Jump", false);
    jump.AddFrames(5, 6, 1, 0.15f);
    jump.AddFrames(0, 7, 6, 0.15f);
    jump.AddFrames(0, 8, 1, 0.15f);
    m_Animator->AddClip(jump);

    // コンボ1段目: 予備動作4フレーム → 攻撃判定4フレーム
    AnimationClip attack1("ComboAttack1", false);
    attack1.AddFrames(2, 2, 4, 0.1f);
    attack1.AddFramesWithHitbox(0, 3, 4, 0.1f, {70.0f, 60.0f}, {40.0f, 0.0f});
    m_Animator->AddClip(attack1);

    // コンボ2段目: 予備動作2フレーム → 攻撃判定2フレーム
    AnimationClip attack2("ComboAttack2", false);
    attack2.AddFrames(4, 3, 2, 0.1f);
    attack2.AddFramesWithHitbox(0, 4, 2, 0.1f, {70.0f, 60.0f}, {40.0f, 0.0f});
    m_Animator->AddClip(attack2);

    // コンボ3段目(フィニッシュ): 予備動作1フレーム → 攻撃判定6フレーム
    AnimationClip attack3("ComboAttack3", false);
    attack3.AddFrames(5, 12, 1, 0.1f);
    attack3.AddFramesWithHitbox(0, 13, 6, 0.1f, {90.0f, 70.0f}, {45.0f, 0.0f});
    m_Animator->AddClip(attack3);

    // 被弾
    AnimationClip hurt("Hurt", false);
    hurt.AddFrames(1, 6, 4, 0.1f);
    m_Animator->AddClip(hurt);

    // 死亡
    AnimationClip death("Death", false);
    death.AddFrames(2, 4, 4, 0.1f);
    death.AddFrames(0, 5, 6, 0.1f);
    death.AddFrames(0, 6, 1, 0.1f);
    m_Animator->AddClip(death);

    // スライディング
    AnimationClip sliding("Sliding", false);
    sliding.AddFrames(0, 14, 6, 0.1f);
    sliding.AddFrames(0, 15, 1, 0.1f);
    m_Animator->AddClip(sliding);
}
