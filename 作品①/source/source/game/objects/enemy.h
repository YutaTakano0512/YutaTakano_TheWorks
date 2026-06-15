#pragma once
#include "../../core/gameObject.h"
#include "../../audio/audio.h"

class SpriteRenderer;
class Animator;
class Rigidbody2D;
class BoxCollider2D;
class Damageable;
class BehaviorNode;

//=============================================================================
// Enemy オブジェクト
//=============================================================================
class Enemy : public GameObject
{
private:
    SpriteRenderer* m_SpriteRenderer = nullptr;
    Animator*       m_Animator        = nullptr;
    Rigidbody2D*    m_Rigidbody       = nullptr;
    BoxCollider2D*  m_Collider        = nullptr;
    BoxCollider2D*  m_Hitbox          = nullptr;
    Damageable*     m_Damageable      = nullptr;

    Audio* m_AttackSE1 = nullptr;
    Audio* m_AttackSE2 = nullptr;

    float m_MoveSpeed   = 100.0f;
    bool  m_IsGrounded  = false;
    bool  m_BehaviorLocked = false;
    bool  m_DestroyOnDeath = true; // 死亡時に自動でDestroyするかどうか

    BehaviorNode* m_BehaviorRoot = nullptr;

    void SetupAnimations();
    void SetupComponents();

public:
    virtual ~Enemy() override;

    void Init()            override;
    void Update(float dt)  override;

    BoxCollider2D* GetHitbox()      const { return m_Hitbox; }
    float          GetMoveSpeed()   const { return m_MoveSpeed; }
    Damageable*    GetDamageable()  const { return m_Damageable; }

    void SetBehaviorLocked(bool locked);
    bool IsBehaviorLocked() const { return m_BehaviorLocked; }
    
    void SetDestroyOnDeath(bool b) { m_DestroyOnDeath = b; }

    void PlayAttackSE(int type);
};
