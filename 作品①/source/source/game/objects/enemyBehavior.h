#pragma once
#include <vector>
#include <functional>

class Enemy;

enum class BehaviorResult
{
    SUCCEED,
    FAILED,
    CONTINUE
};

//=============================================================================
// BehaviorNode 基底クラス
//=============================================================================
class BehaviorNode
{
protected:
    std::vector<BehaviorNode*> m_Child;
    int m_Index = 0;

public:
    virtual ~BehaviorNode()
    {
        for (auto child : m_Child)
        {
            delete child;
        }
        m_Child.clear();
    }

    void AddChild(BehaviorNode* child)
    {
        m_Child.push_back(child);
    }

    virtual void Reset()
    {
        m_Index = 0;
        for (auto child : m_Child)
        {
            child->Reset();
        }
    }

    virtual BehaviorResult Update(Enemy* enemy, float dt) = 0;
};

//=============================================================================
// BehaviorSequence - 子ノードを順番に実行する（すべてSUCCEEDでSUCCEED）
//=============================================================================
class BehaviorSequence : public BehaviorNode
{
public:
    virtual BehaviorResult Update(Enemy* enemy, float dt) override;
};

//=============================================================================
// BehaviorSelector - 子ノードを順番に評価し、SUCCEEDかCONTINUEを返した時点で終了
//=============================================================================
class BehaviorSelector : public BehaviorNode
{
public:
    virtual BehaviorResult Update(Enemy* enemy, float dt) override;
};

//=============================================================================
// BehaviorCondition - 条件を満たす場合のみ子ノード（1つ）を実行するデコレーター
//=============================================================================
class BehaviorCondition : public BehaviorNode
{
private:
    std::function<bool(Enemy*, float)> m_Condition;

public:
    BehaviorCondition(std::function<bool(Enemy*, float)> condition) : m_Condition(condition) {}

    virtual BehaviorResult Update(Enemy* enemy, float dt) override;
};

//=============================================================================
// BehaviorWait - 指定時間だけ待機する
//=============================================================================
class BehaviorWait : public BehaviorNode
{
private:
    float m_WaitTime = 0.0f;
    float m_Timer    = 0.0f;

public:
    BehaviorWait(float waitTime) : m_WaitTime(waitTime) {}

    virtual void Reset() override
    {
        BehaviorNode::Reset();
        m_Timer = 0.0f;
    }

    virtual BehaviorResult Update(Enemy* enemy, float dt) override;
};

//=============================================================================
// BehaviorRandomWait - ランダムな時間だけ待機する
//=============================================================================
class BehaviorRandomWait : public BehaviorNode
{
private:
    float m_MinTime;
    float m_MaxTime;
    float m_WaitTime = 0.0f;
    float m_Timer    = 0.0f;

public:
    BehaviorRandomWait(float minTime, float maxTime) : m_MinTime(minTime), m_MaxTime(maxTime) {}

    virtual void Reset() override;
    virtual BehaviorResult Update(Enemy* enemy, float dt) override;
};

//=============================================================================
// BehaviorAttack - 近接攻撃 (Attack1)
//=============================================================================
class BehaviorAttack : public BehaviorNode
{
private:
    bool m_Started = false;

public:
    virtual void Reset() override
    {
        BehaviorNode::Reset();
        m_Started = false;
    }

    virtual BehaviorResult Update(Enemy* enemy, float dt) override;
};

//=============================================================================
// BehaviorGun - 遠距離攻撃 (Attack2 → プレイヤー頭上に EnemyAttack を生成)
//=============================================================================
class BehaviorGun : public BehaviorNode
{
private:
    bool  m_Started = false;
    bool  m_Spawned = false;
    bool  m_PlayedSE = false;
    float m_Timer   = 0.0f;

public:
    virtual void Reset() override
    {
        BehaviorNode::Reset();
        m_Started = false;
        m_Spawned = false;
        m_PlayedSE = false;
        m_Timer   = 0.0f;
    }

    virtual BehaviorResult Update(Enemy* enemy, float dt) override;
};

//=============================================================================
// BehaviorPunch - パンチ攻撃 (Attack1 を流用)
//=============================================================================
class BehaviorPunch : public BehaviorNode
{
private:
    bool m_Started = false;

public:
    virtual void Reset() override
    {
        BehaviorNode::Reset();
        m_Started = false;
    }

    virtual BehaviorResult Update(Enemy* enemy, float dt) override;
};

//=============================================================================
// BehaviorMoveToPlayer - プレイヤーとの距離を一定に保ちながら追跡する
//=============================================================================
class BehaviorMoveToPlayer : public BehaviorNode
{
private:
    float m_TargetDistance = 0.0f;

public:
    BehaviorMoveToPlayer(float targetDistance) : m_TargetDistance(targetDistance) {}

    virtual BehaviorResult Update(Enemy* enemy, float dt) override;
};

//=============================================================================
// BehaviorDash - 短時間高速で移動する（バックステップや飛び込み用）
//=============================================================================
class BehaviorDash : public BehaviorNode
{
private:
    float m_DashTime;
    float m_DashSpeed;
    bool  m_Forward;   // trueなら前進、falseなら後退
    float m_Timer = 0.0f;

public:
    // forward=trueならプレイヤー側へ、falseならプレイヤーと逆側へダッシュ
    BehaviorDash(float dashTime, float dashSpeed, bool forward) 
        : m_DashTime(dashTime), m_DashSpeed(dashSpeed), m_Forward(forward) {}

    virtual void Reset() override
    {
        BehaviorNode::Reset();
        m_Timer = 0.0f;
    }

    virtual BehaviorResult Update(Enemy* enemy, float dt) override;
};
