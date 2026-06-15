#include "bossIntroDirector.h"
#include "../../graphics/postProcess.h"
#include "player.h"
#include "enemy.h"
#include "mainCamera.h"
#include "../../core/scene.h"
#include "../../core/manager.h"
#include "../../core/main.h"
#include "../../animation/animator.h"
#include "../../physics/rigidbody2D.h"
#include "../../physics/boxCollider2D.h"
#include "../../graphics/spriteRenderer.h"
#include "../UI/enemyUI.h"
#include <cmath>

//=============================================================================
// 初期化
//=============================================================================

void BossIntroDirector::Init()
{
    GameObject::Init();
    SetName("BossIntroDirector");
}

//=============================================================================
// 更新
//=============================================================================

void BossIntroDirector::Update(float dt)
{
    GameObject::Update(dt);

    switch (m_State)
    {
    case STATE_LETTERBOX_IN:
        UpdateLetterboxIn(dt);
        break;
    case STATE_PLAYER_WALK:
        UpdatePlayerWalk(dt);
        break;
    case STATE_BOSS_TAUNT:
        UpdateBossTaunt(dt);
        break;
    case STATE_LETTERBOX_OUT:
        UpdateLetterboxOut(dt);
        break;
    default:
        break;
    }
}

//=============================================================================
// 演出開始
//=============================================================================

void BossIntroDirector::StartEvent()
{
    if (m_State != STATE_IDLE) return;

    // プレイヤーを入力ロック
    LockPlayer();

    // ボスのAIをロック（演出中は動かない）
    if (m_Boss)
    {
        m_Boss->SetBehaviorLocked(true);
    }

    // プレイヤーの目標X座標 = ボスの300px手前
    if (m_Boss && m_Player)
    {
        float dir = (m_Boss->transform.position.x > m_Player->transform.position.x) ? 1.0f : -1.0f;
        m_PlayerTargetX = m_Boss->transform.position.x - dir * 500.0f;

        // 演出開始時点でカメラを最終的な中間位置（プレイヤー目標位置とボスの間）に固定する
        if (m_Camera)
        {
            float camX = (m_PlayerTargetX + m_Boss->transform.position.x) / 2.0f;
            m_Camera->SetFixed(true, { camX, m_Camera->transform.position.y });
        }
    }

    // レターボックスエフェクトを追加（厚み0からスタート）
    if (m_PostProcess)
    {
        m_LetterboxThickness = 0.0f;
        m_PostProcess->AddEffect(PostEffectType::LETTERBOX, { m_LetterboxThickness, 0.0f, 0.0f, 0.0f });
    }

    m_State = STATE_LETTERBOX_IN;
    m_Timer = 0.0f;
}

//=============================================================================
// プレイヤーの入力ロック
//=============================================================================

void BossIntroDirector::LockPlayer()
{
    if (!m_Player) return;
    m_Player->SetInputLocked(true);

    // 速度も即座にゼロに（慣性で滑らないように）
    Rigidbody2D* rb = m_Player->GetComponent<Rigidbody2D>();
    if (rb)
    {
        rb->velocity.x = 0.0f;
    }
}

//=============================================================================
// プレイヤーのロック解除
//=============================================================================

void BossIntroDirector::UnlockPlayer()
{
    if (!m_Player) return;
    m_Player->SetInputLocked(false);
}

//=============================================================================
// 黒帯スライドイン待ち → プレイヤー歩行へ
//=============================================================================

void BossIntroDirector::UpdateLetterboxIn(float dt)
{
    m_Timer += dt;
    
    // イージングで厚みを増やす (0.0 から 0.15 くらいまで)
    float t = std::min(m_Timer / LETTERBOX_IN_DURATION, 1.0f);
    // easeOutQuad: t * (2 - t)
    float easeT = t * (2.0f - t);
    m_LetterboxThickness = 0.15f * easeT;
    
    if (m_PostProcess)
    {
        m_PostProcess->UpdateEffectParam(PostEffectType::LETTERBOX, { m_LetterboxThickness, 0.0f, 0.0f, 0.0f });
    }

    if (m_Timer >= LETTERBOX_IN_DURATION)
    {
        // 黒帯が出たらプレイヤーをRunアニメーションで歩かせる
        if (m_Player)
        {
            Animator* anim = m_Player->GetComponent<Animator>();
            if (anim) anim->Play("Run");
        }
        m_State = STATE_PLAYER_WALK;
        m_Timer = 0.0f;
    }
}

//=============================================================================
// プレイヤーが目標位置まで自動歩行
//=============================================================================

void BossIntroDirector::UpdatePlayerWalk(float dt)
{
    m_Timer += dt;

    if (!m_Player)
    {
        m_State = STATE_BOSS_TAUNT;
        m_Timer = 0.0f;
        return;
    }

    Rigidbody2D*    rb = m_Player->GetComponent<Rigidbody2D>();
    SpriteRenderer* sr = m_Player->GetComponent<SpriteRenderer>();

    float dx        = m_PlayerTargetX - m_Player->transform.position.x;
    float walkSpeed = 200.0f;

    if (rb)
    {
        if (std::abs(dx) > 5.0f)
        {
            // 目標方向へ速度をセット
            float dir = (dx > 0.0f) ? 1.0f : -1.0f;
            rb->velocity.x = dir * walkSpeed;
            if (sr) sr->flipX = (dir < 0.0f);
        }
        else
        {
            // 目標位置に到達 → 停止してボス方向を向く
            rb->velocity.x = 0.0f;

            // ボスの方を向く
            if (m_Boss && sr)
            {
                sr->flipX = (m_Boss->transform.position.x < m_Player->transform.position.x);
            }

            Animator* anim = m_Player->GetComponent<Animator>();
            if (anim) anim->Play("Idle");

            m_State = STATE_BOSS_TAUNT;
            m_Timer = 0.0f;
            return;
        }
    }

    // タイムアウト
    if (m_Timer >= PLAYER_WALK_TIMEOUT)
    {
        if (rb) rb->velocity.x = 0.0f;
        m_State = STATE_BOSS_TAUNT;
        m_Timer = 0.0f;
    }
}

//=============================================================================
// ボスが威嚇アニメーションを再生
//=============================================================================

void BossIntroDirector::UpdateBossTaunt(float dt)
{
    m_Timer += dt;

    // 少し間を置いてから威嚇アニメを再生する
    if (m_Timer >= TAUNT_WAIT_BEFORE && m_Timer < TAUNT_WAIT_BEFORE + 0.02f)
    {
        // ボスがプレイヤーの方を向く
        if (m_Boss && m_Player)
        {
            SpriteRenderer* sr = m_Boss->GetComponent<SpriteRenderer>();
            if (sr)
            {
                sr->flipX = (m_Player->transform.position.x > m_Boss->transform.position.x);
            }
        }

        // 威嚇アニメーション再生
        if (m_Boss)
        {
            Animator* anim = m_Boss->GetComponent<Animator>();
            if (anim) anim->Play("Attack1", true);
        }
    }

    // 最低待機時間が経過したら黒帯スライドアウトへ
    if (m_Timer >= TAUNT_WAIT_BEFORE + TAUNT_ANIM_DURATION)
    {
        m_State = STATE_LETTERBOX_OUT;
        m_Timer = 0.0f;
    }
}

//=============================================================================
// 黒帯スライドアウト → ボス戦開始
//=============================================================================
void BossIntroDirector::UpdateLetterboxOut(float dt)
{
    m_Timer += dt;
    
    // イージングで厚みを減らす (0.15 から 0.0 まで)
    float t = std::min(m_Timer / LETTERBOX_OUT_DURATION, 1.0f);
    // easeInQuad: t * t
    float easeT = t * t;
    m_LetterboxThickness = 0.15f * (1.0f - easeT);
    
    if (m_PostProcess)
    {
        m_PostProcess->UpdateEffectParam(PostEffectType::LETTERBOX, { m_LetterboxThickness, 0.0f, 0.0f, 0.0f });
    }

    if (m_Timer >= LETTERBOX_OUT_DURATION)
    {
        if (m_PostProcess)
        {
            m_PostProcess->RemoveEffect(PostEffectType::LETTERBOX);
        }

        // ボス戦開始処理
        BeginBossBattle();

        m_State = STATE_FINISHED;
        m_Timer = 0.0f;

        // Directorとしての役割終了
        SetDestroy();
    }
}

//=============================================================================
// ボス戦開始処理
//=============================================================================

void BossIntroDirector::BeginBossBattle()
{
    // プレイヤーの入力を解放
    UnlockPlayer();

    // ボスのAIロックを解除（ここから戦闘開始）
    if (m_Boss)
    {
        m_Boss->SetBehaviorLocked(false);
    }

    // UI
    if (m_EnemyUI)
    {
        m_EnemyUI->SetVisible(true);
	}

    // カメラはStartEventで既に固定済み。そのまま境界壁を生成。
    SpawnBoundaryWalls();
}

//=============================================================================
// ボスエリア境界壁を生成（透明な静的コライダー）
//=============================================================================

void BossIntroDirector::SpawnBoundaryWalls()
{
    if (!m_Boss) return;

    Scene* scene = Manager::GetScene();
    if (!scene) return;

    const float wallHeight = 800.0f; // 十分な高さ
    const float wallWidth  = 40.0f;
    const float bossY      = m_Boss->transform.position.y;

    // カメラのX座標（プレイヤー目標とボスの中間）
    float camX = m_Boss->transform.position.x;
    if (m_Player)
    {
        camX = (m_PlayerTargetX + m_Boss->transform.position.x) / 2.0f;
    }

    // 画面幅の半分（1280 / 2 = 640）
    const float halfScreenWidth = 640.0f;

    // 左壁 (画面の左端より外側)
    m_WallLeft = scene->AddGameObject<GameObject>(Layer::FIELD);
    {
        BoxCollider2D* col = m_WallLeft->AddComponent<BoxCollider2D>();
        col->size          = { wallWidth, wallHeight };
        col->isTrigger     = false;
        m_WallLeft->transform.position = { camX - halfScreenWidth - (wallWidth * 0.5f), bossY };
    }

    // 右壁 (画面の右端より外側)
    m_WallRight = scene->AddGameObject<GameObject>(Layer::FIELD);
    {
        BoxCollider2D* col = m_WallRight->AddComponent<BoxCollider2D>();
        col->size          = { wallWidth, wallHeight };
        col->isTrigger     = false;
        m_WallRight->transform.position = { camX + halfScreenWidth + (wallWidth * 0.5f), bossY };
    }
}
