#include "bossEventTrigger.h"
#include "bossIntroDirector.h"
#include "../../physics/boxCollider2D.h"
#include <iostream>

//=============================================================================
// 初期化
//=============================================================================

void BossEventTrigger::Init()
{
    // 自身にBoxCollider2Dを追加（または取得）
    auto* collider = GetComponent<BoxCollider2D>();
    if (!collider)
    {
        collider = AddComponent<BoxCollider2D>();
    }

    collider->isTrigger = true;

    // プレイヤーが通り抜けられるサイズに調整
    collider->size = { 64.0f, 1000.0f };

    // 衝突時のコールバックを登録（ラムダ式でクラス内の関数を呼ぶ）
    collider->onCollisionEnter = [this](const CollisionInfo& info)
    {
        this->OnTriggerEnter(info);
    };
}

void BossEventTrigger::OnTriggerEnter(const CollisionInfo& info)
{
    // 既にイベント発生済みなら処理を抜ける
    if (m_HasTriggered) return;

    // 衝突した相手のGameObjectを取得
    GameObject* hitObj = info.other->GetGameObject();

    // プレイヤーとの接触かどうかを判定
    if (hitObj && hitObj->CompareTag("Player"))
    {
        m_HasTriggered = true;
        StartBossEvent();
    }
}

void BossEventTrigger::StartBossEvent()
{
    if (m_Director)
    {
        m_Director->StartEvent();
    }

    SetDestroy();
}
