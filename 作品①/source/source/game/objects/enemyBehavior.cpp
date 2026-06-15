#include "enemyBehavior.h"
#include "enemy.h"
#include "enemyAttack.h"
#include "../../animation/animator.h"
#include "../../core/manager.h"
#include "../../core/scene.h"
#include "../../physics/rigidbody2D.h"
#include "player.h"
#include <cmath>
#include <random>

//=============================================================================
// 共通ヘルパー: 敵をプレイヤーの方向に向かせる
//=============================================================================
static void FacePlayer(Enemy* enemy)
{
    Player* player = Manager::GetScene()->GetGameObject<Player>();
    if (!player) return;

    float dirX  = player->transform.position.x - enemy->transform.position.x;
    if (dirX == 0.0f) return;

    float scaleX = std::abs(enemy->transform.scale.x);
    // 元画像がデフォルトで左向きのため、右 (dirX > 0) なら scale.x を負にする
    enemy->transform.scale.x = (dirX > 0.0f) ? -scaleX : scaleX;
}

//=============================================================================
// BehaviorSequence - 子ノードを順番に実行する
//=============================================================================
BehaviorResult BehaviorSequence::Update(Enemy* enemy, float dt)
{
    if (m_Child.empty()) return BehaviorResult::SUCCEED;

    BehaviorResult result = m_Child[m_Index]->Update(enemy, dt);
    if (result == BehaviorResult::SUCCEED)
    {
        m_Index++;
        if (m_Child.size() <= m_Index)
        {
            m_Index = 0;
            return BehaviorResult::SUCCEED;
        }
        return BehaviorResult::CONTINUE;
    }
    else if (result == BehaviorResult::FAILED)
    {
        m_Index = 0;
        return BehaviorResult::FAILED;
    }
    return BehaviorResult::CONTINUE;
}

//=============================================================================
// BehaviorSelector - 子ノードを順番に評価し、SUCCEEDかCONTINUEを返した時点で終了
//=============================================================================
BehaviorResult BehaviorSelector::Update(Enemy* enemy, float dt)
{
    if (m_Child.empty()) return BehaviorResult::FAILED;

    // 現在のインデックスから評価
    for (; m_Index < m_Child.size(); ++m_Index)
    {
        BehaviorResult result = m_Child[m_Index]->Update(enemy, dt);
        if (result == BehaviorResult::SUCCEED)
        {
            m_Index = 0; // 成功したので次回は最初から
            return BehaviorResult::SUCCEED;
        }
        else if (result == BehaviorResult::CONTINUE)
        {
            return BehaviorResult::CONTINUE; // 実行中なので抜ける
        }
        // FAILED の場合は次の子ノードを評価する
    }

    // 全てFAILEDだった
    m_Index = 0;
    return BehaviorResult::FAILED;
}

//=============================================================================
// BehaviorCondition - 条件を満たす場合のみ子ノード（1つ）を実行するデコレーター
//=============================================================================
BehaviorResult BehaviorCondition::Update(Enemy* enemy, float dt)
{
    if (m_Child.empty()) return BehaviorResult::FAILED;

    Player* player = Manager::GetScene()->GetGameObject<Player>();
    if (!player) return BehaviorResult::FAILED;

    float diffX = player->transform.position.x - enemy->transform.position.x;
    float dist  = std::abs(diffX);

    // 条件を満たしていれば子ノードを実行
    if (m_Index == 0) // 未実行状態
    {
        if (!m_Condition(enemy, dist))
        {
            return BehaviorResult::FAILED;
        }
        m_Index = 1; // 実行中フラグとして利用
    }

    BehaviorResult result = m_Child[0]->Update(enemy, dt);
    if (result != BehaviorResult::CONTINUE)
    {
        m_Index = 0; // 終了したらリセット
    }
    return result;
}

//=============================================================================
// BehaviorWait - 指定時間だけ待機する
//=============================================================================
BehaviorResult BehaviorWait::Update(Enemy* enemy, float dt)
{
    Animator* animator = enemy->GetComponent<Animator>();
    if (animator && animator->GetCurrentClipName() != "Idle")
    {
        animator->Play("Idle");
    }

    m_Timer += dt;
    if (m_Timer >= m_WaitTime)
    {
        m_Timer = 0.0f;
        return BehaviorResult::SUCCEED;
    }
    return BehaviorResult::CONTINUE;
}

//=============================================================================
// BehaviorRandomWait - ランダムな時間だけ待機する
//=============================================================================
void BehaviorRandomWait::Reset()
{
    BehaviorNode::Reset();
    m_Timer = 0.0f;
    m_WaitTime = 0.0f;
}

BehaviorResult BehaviorRandomWait::Update(Enemy* enemy, float dt)
{
    if (m_WaitTime == 0.0f)
    {
        // 初回 Update 時にランダムな待機時間を決定する
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_real_distribution<float> dis(m_MinTime, m_MaxTime);
        m_WaitTime = dis(gen);
    }

    Animator* animator = enemy->GetComponent<Animator>();
    if (animator && animator->GetCurrentClipName() != "Idle")
    {
        animator->Play("Idle");
    }

    m_Timer += dt;
    if (m_Timer >= m_WaitTime)
    {
        m_Timer = 0.0f;
        m_WaitTime = 0.0f; // 次回のためにリセット
        return BehaviorResult::SUCCEED;
    }
    return BehaviorResult::CONTINUE;
}

//=============================================================================
// BehaviorAttack - 近接攻撃 (Attack1)
//=============================================================================
BehaviorResult BehaviorAttack::Update(Enemy* enemy, float dt)
{
    Animator* animator = enemy->GetComponent<Animator>();
    if (!animator) return BehaviorResult::FAILED;

    if (!m_Started)
    {
        FacePlayer(enemy);
        animator->Play("Attack1", true);
        enemy->PlayAttackSE(1);
        m_Started = true;
    }

    // アニメーションが終了したら成功
    if (animator->GetCurrentClipName() == "Attack1" && animator->IsFinished())
    {
        m_Started = false;
        return BehaviorResult::SUCCEED;
    }

    return BehaviorResult::CONTINUE;
}

//=============================================================================
// BehaviorGun - 遠距離攻撃 (Attack2 → プレイヤー頭上に EnemyAttack を生成)
//=============================================================================
BehaviorResult BehaviorGun::Update(Enemy* enemy, float dt)
{
    Animator* animator = enemy->GetComponent<Animator>();
    if (!animator) return BehaviorResult::FAILED;

    if (!m_Started)
    {
        FacePlayer(enemy);
        animator->Play("Attack2", true);
        m_Started = true;
        m_Spawned = false;
        m_Timer   = 0.0f;
    }

    m_Timer += dt;

    // 0.3 秒後にプレイヤーの頭上へ EnemyAttack を生成
    if (!m_Spawned && m_Timer >= 0.3f)
    {
        Player* player = Manager::GetScene()->GetGameObject<Player>();
        Scene*  scene  = Manager::GetScene();
        if (player && scene)
        {
            Vector2 spawnPos  = player->transform.position;
            spawnPos.y       -= 95.0f; // 頭上 95px

            EnemyAttack* effect = scene->AddGameObject<EnemyAttack>(Layer::EFFECT);
            if (effect)
            {
                effect->transform.position = spawnPos;
            }
        }
        m_Spawned = true;
    }

    // 0.7秒後に攻撃判定のSEを鳴らす
    if (!m_PlayedSE && m_Timer >= 0.7f)
    {
        enemy->PlayAttackSE(2);
        m_PlayedSE = true;
    }

    // アニメーションが終了したら成功
    if (animator->GetCurrentClipName() == "Attack2" && animator->IsFinished())
    {
        m_Started = false;
        m_Spawned = false;
        m_PlayedSE = false;
        m_Timer   = 0.0f;
        return BehaviorResult::SUCCEED;
    }

    return BehaviorResult::CONTINUE;
}

//=============================================================================
// BehaviorPunch - パンチ攻撃 (Attack1 を流用)
//=============================================================================
BehaviorResult BehaviorPunch::Update(Enemy* enemy, float dt)
{
    Animator* animator = enemy->GetComponent<Animator>();
    if (!animator) return BehaviorResult::FAILED;

    if (!m_Started)
    {
        FacePlayer(enemy);
        animator->Play("Attack1", true);
        enemy->PlayAttackSE(1);
        m_Started = true;
    }

    // アニメーションが終了したら成功
    if (animator->GetCurrentClipName() == "Attack1" && animator->IsFinished())
    {
        m_Started = false;
        return BehaviorResult::SUCCEED;
    }

    return BehaviorResult::CONTINUE;
}

//=============================================================================
// BehaviorMoveToPlayer - プレイヤーとの距離を一定に保ちながら追跡する
//=============================================================================
BehaviorResult BehaviorMoveToPlayer::Update(Enemy* enemy, float dt)
{
    Player* player = Manager::GetScene()->GetGameObject<Player>();
    if (!player) return BehaviorResult::FAILED;

    float diffX = player->transform.position.x - enemy->transform.position.x;
    float dist  = std::abs(diffX);

    Animator*    animator = enemy->GetComponent<Animator>();
    Rigidbody2D* rb       = enemy->GetComponent<Rigidbody2D>();

    const float tolerance = 30.0f;

    // 目標距離に到達したら停止して成功
    if (dist >= m_TargetDistance - tolerance && dist <= m_TargetDistance + tolerance)
    {
        if (rb)
        {
            rb->velocity.x = 0.0f;
        }
        if (animator && animator->GetCurrentClipName() != "Idle")
        {
            animator->Play("Idle");
        }
        return BehaviorResult::SUCCEED;
    }

    // 近すぎる → 離れる / 遠すぎる → 近づく
    float dir = (dist < m_TargetDistance - tolerance)
        ? ((diffX > 0.0f) ? -1.0f : 1.0f)   // 離れる
        : ((diffX > 0.0f) ?  1.0f : -1.0f);  // 近づく

    if (rb)
    {
        rb->velocity.x = dir * enemy->GetMoveSpeed();
    }
    if (animator && animator->GetCurrentClipName() != "Run")
    {
        animator->Play("Run");
    }

    return BehaviorResult::CONTINUE;
}

//=============================================================================
// BehaviorDash - 短時間高速で移動する（バックステップや飛び込み用）
//=============================================================================
BehaviorResult BehaviorDash::Update(Enemy* enemy, float dt)
{
    Player* player = Manager::GetScene()->GetGameObject<Player>();
    if (!player) return BehaviorResult::FAILED;

    float diffX = player->transform.position.x - enemy->transform.position.x;
    float dirToPlayer = (diffX > 0.0f) ? 1.0f : -1.0f;
    
    // 前進ならプレイヤー側へ、後退ならプレイヤーと逆側へ
    float moveDir = m_Forward ? dirToPlayer : -dirToPlayer;

    Rigidbody2D* rb = enemy->GetComponent<Rigidbody2D>();
    Animator* animator = enemy->GetComponent<Animator>();

    if (m_Timer == 0.0f)
    {
        FacePlayer(enemy);
        if (animator)
        {
            animator->Play("Run");
        }
    }

    m_Timer += dt;
    
    if (rb)
    {
        rb->velocity.x = moveDir * m_DashSpeed;
    }

    if (m_Timer >= m_DashTime)
    {
        m_Timer = 0.0f;
        if (rb) rb->velocity.x = 0.0f;
        return BehaviorResult::SUCCEED;
    }

    return BehaviorResult::CONTINUE;
}
