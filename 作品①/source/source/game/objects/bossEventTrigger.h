#pragma once
#include "../../core/gameObject.h"
#include "../../physics/collider2D.h"

class BossIntroDirector;

//=============================================================================
// ボス部屋入室イベントを制御するオブジェクト
// トリガーコライダーにプレイヤーが触れると BossIntroDirector の演出を開始する。
//=============================================================================
class BossEventTrigger : public GameObject
{
private:
    bool m_HasTriggered = false; // イベントが2回以上走らないようにするフラグ
    BossIntroDirector* m_Director = nullptr; // 演出コントローラへの参照

public:
    void Init() override;

    // GameScene側でDirectorへの参照を渡す
    void SetDirector(BossIntroDirector* director) { m_Director = director; }

    void OnTriggerEnter(const CollisionInfo& info);
    void StartBossEvent();
};
