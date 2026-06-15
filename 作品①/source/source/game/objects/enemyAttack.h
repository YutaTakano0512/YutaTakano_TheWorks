#pragma once
#include "../../core/gameObject.h"

class SpriteRenderer;
class Animator;
class BoxCollider2D;

//=============================================================================
// EnemyAttack オブジェクト
// 敵の遠距離攻撃エフェクト。アニメーション終了と同時に自身を消去する。
//=============================================================================
class EnemyAttack : public GameObject
{
private:
    SpriteRenderer* m_SpriteRenderer = nullptr;
    Animator*       m_Animator       = nullptr;
    BoxCollider2D*  m_Hitbox         = nullptr;

    void SetupAnimations();
    void SetupComponents();

public:
    void Init()           override;
    void Update(float dt) override;
};
