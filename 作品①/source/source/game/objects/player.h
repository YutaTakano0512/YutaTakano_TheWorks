#pragma once
#include "../../core/gameObject.h"
#include "../../audio/audio.h"

class SpriteRenderer;
class Animator;
class Rigidbody2D;
class BoxCollider2D;
class Damageable;

//=============================================================================
// Player オブジェクト
//=============================================================================
class Player : public GameObject
{
private:
    SpriteRenderer* m_SpriteRenderer = nullptr;
    Animator* m_Animator = nullptr;
    Rigidbody2D* m_Rigidbody = nullptr;
    BoxCollider2D* m_Collider = nullptr;
    BoxCollider2D* m_Hitbox = nullptr; // 攻撃判定
    Damageable* m_Damageable = nullptr;

    Audio* m_ComboAttackSE1 = nullptr;
    Audio* m_JumpSE = nullptr;
    Audio* m_PlayerDeathSE = nullptr;
    Audio* m_PlayerRunSE = nullptr;
    Audio* m_SlidingSE = nullptr;
    Audio* m_SwordSwingSE = nullptr;

    float m_SwordSwingDelayTimer = 0.0f; // 素振りSEのディレイ用タイマー

    bool  m_InputLocked = false; // 演出中の入力ロック

    float m_MoveSpeed = 300.0f;
    float m_JumpForce = -500.0f; // 上方向はマイナス
    float m_SlidingForce = 300.0f;
    bool  m_IsGrounded = false;

    //float m_AttackCooldown = 20.0f; // 攻撃のクールダウン時間 (秒)
    //float m_AttackTimer = 0.0f;    // 攻撃の経過時間

    // スライディング管理
    bool  m_IsSliding = false;
    float m_SlidingTimer = 0.0f;
    float m_SlidingDir = 1.0f;
    float m_SlidingDuration = 0.5f;
    float m_SlidingCooldown = 0.8f;
    float m_SlidingCooldownTimer = 0.0f;

    // コンボ管理
    int  m_ComboStep = 0;     // 0:未攻撃, 1:1段目中, 2:2段目中, 3:3段目中
    bool m_ComboQueued = false; // 1段目中に2段目の入力を受け付けたか
    float m_ComboTimer = 0.0f;  // 近接コンボ攻撃の経過時間タイマー (攻撃判定のタイミング制御用)

    // 攻撃がヒットしたオブジェクトの追跡（多段ヒット防止用）
    std::vector<GameObject*> m_HitObjects;

public:
    virtual ~Player() override;
    void Init() override;
    void Uninit() override;
    void Update(float dt) override;

    //float GetAttackTimer() const { return m_AttackTimer; }
    //float GetAttackCooldown() const { return m_AttackCooldown; }

    Damageable* GetDamageable() const { return m_Damageable; }
    bool IsSliding() const { return m_IsSliding; }

    // 演出中の入力ロック
    void SetInputLocked(bool locked) { m_InputLocked = locked; }
    bool IsInputLocked() const { return m_InputLocked; }

private:
    // 攻撃判定（Hitbox）の衝突コールバック
    void OnHitboxTrigger(const struct CollisionInfo& info);

    bool IsHurt() const;
    bool IsDead() const;
	void SetupAnimations();// アニメーションのセットアップ

    float GetMoveInput()  const;// 移動入力値を取得
    bool GetMeleeInput() const;// 近接攻撃入力を取得
    bool GetRangedInput() const;// 遠距離攻撃入力を取得
    bool GetJumpInput()  const;// ジャンプ入力を取得
    bool GetSlidingInput() const;// スライディング入力を取得

    //void HandleRangedAttack(float dt);// 遠距離攻撃 (チャクラム発射)
    void HandleMovement(float moveX, float dt);// 横移動・向き反転 (dtを追加)
    void HandleJump(bool jumpInput);// ジャンプ・接地判定
    void HandleSliding(bool slidingInput, float dt);// スライディング
    void HandleComboStateMachine(bool input, float moveX);// コンボ攻撃 ステートマシン
    void HandleAnimationState(bool meleeInput, float moveX);// アニメーション遷移
};
