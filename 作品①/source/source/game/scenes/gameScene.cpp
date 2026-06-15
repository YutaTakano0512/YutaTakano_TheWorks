#include "gameScene.h"
#include "../../core/manager.h"
#include "../objects/player.h"
#include "../objects/mainCamera.h"
#include "../objects/ground.h"
#include "../objects/enemy.h"
#include "../objects/enemyAttack.h"
#include "../objects/bossEventTrigger.h"
#include "../objects/bossIntroDirector.h"
#include "../objects/damageable.h"
#include "../UI/playerUI.h"
#include "../UI/enemyUI.h"
#include "../../graphics/spriteRenderer.h"
#include "../../graphics/renderer.h"
#include "../../physics/boxCollider2D.h"
#include "../../physics/rigidbody2D.h"
#include "../../physics/physicsManager.h"
#include "../../animation/animator.h"
#include "gameClearScene.h"
#include "titleScene.h"

//=============================================================================
// 初期化
//=============================================================================

void GameScene::Init()
{
    // PostProcessの初期化
    m_PostProcess.Init();

    // 背景オブジェクトを追加 (Layer::BACKGROUND = 最背面)
    m_Background = AddGameObject<Background>(Layer::BACKGROUND);

    // メインカメラを追加
    m_Camera = AddGameObject<MainCamera>(Layer::BACKGROUND);

    // ステージの範囲に合わせてカメラをクランプする
    // 地面: 総幅 1280*3=3840, 中心X=0 → 左端 -1920, 右端 +1920
    // Y方向: 上端 -500, 下端 +400 (地面下部を考慮)
    m_Camera->SetStageBounds(-1920.0f, 1920.0f, -500.0f, 400.0f);

    // フィールドの地面オブジェクトを追加
    AddGameObject<Ground>(Layer::FIELD);

    // プレイヤーオブジェクトを追加
    m_Player = AddGameObject<Player>(Layer::PLAYER);
    m_Player->SetTag("Player"); // コライダーの判定用にタグを設定
    m_Player->transform.position = { -1200.0f, -100.0f };

    // ボス（Enemy）オブジェクトを追加
    m_Boss = AddGameObject<Enemy>(Layer::ENEMY);
    m_Boss->SetTag("Enemy");
    m_Boss->transform.position = { 700.0f, -200.0f }; // ボスはプレイヤーの右側に配置

    // カットイン演出が終わるまでAIを停止しておく
    m_Boss->SetBehaviorLocked(true);
    m_Boss->SetDestroyOnDeath(false); // 死亡演出のために死体を残す

    // BossIntroDirector を生成し、参照をセット
    auto* director = AddGameObject<BossIntroDirector>(Layer::OBJECT);
    director->SetPlayer(m_Player);
    director->SetBoss(m_Boss);
    director->SetCamera(m_Camera);
    director->SetPostProcess(&m_PostProcess); // ポストプロセスを渡す

    // ボス部屋のイベントトリガーを配置
    // プレイヤー(200) → トリガー(550) → ボス(900) の順に一直線
    auto* bossTrigger = AddGameObject<BossEventTrigger>(Layer::FIELD);
    bossTrigger->transform.position = { 0.0f, -300.0f };
    bossTrigger->SetDirector(director); // DirectorをTriggerに渡す

    // プレイヤーUI (Canvas)
    auto* playerUI = AddGameObject<PlayerUI>(Layer::UI);
    playerUI->SetPlayer(m_Player);

    // 敵UI (Canvas): 画面中央上部にHPバー・名前を表示
    auto* enemyUI = AddGameObject<EnemyUI>(Layer::UI);
    enemyUI->SetEnemy(m_Boss);
    enemyUI->SetEnemyName(L"Dark Samurai");
	enemyUI->SetVisible(false);
	director->SetEnemyUI(enemyUI);

    m_State = State::FadeIn;
    m_StateTimer = 0.0f;
    m_FadeAlpha = 0.0f; // 0.0が真っ黒、1.0が透明
    m_PostProcess.AddEffect(PostEffectType::FADE, { m_FadeAlpha, 0.0f, 0.0f, 0.0f });
}

//=============================================================================
// 終了
//=============================================================================

void GameScene::Uninit()
{
    m_PostProcess.Uninit();
}

//=============================================================================
// 更新
//=============================================================================

void GameScene::Update(float dt)
{
    // 通常のUpdate (タイムスケールを考慮したdtが渡される)
    Scene::Update(dt);

    // Unscaled dtを取得（演出タイマー等に使用）
    float unscaledDt = Manager::GetUnscaledDeltaTime();

    switch (m_State)
    {
    case State::FadeIn:
        m_FadeAlpha += unscaledDt * 1.5f; // 0.0(黒)から1.0(透明)へ
        if (m_FadeAlpha >= 1.0f)
        {
            m_FadeAlpha = 1.0f;
            m_State = State::Playing;
        }
        m_PostProcess.UpdateEffectParam(PostEffectType::FADE, { m_FadeAlpha, 0.0f, 0.0f, 0.0f });
        break;

    case State::Playing:
        if (m_Boss && m_Boss->GetDamageable() && m_Boss->GetDamageable()->IsDead())
        {
            m_State = State::EnemyDeadSlow;
            m_StateTimer = 0.0f;
            Manager::SetTimeScale(0.2f); // スローモーション開始
            
            // 無敵化で余計なダメージを防ぐ
            m_Boss->GetDamageable()->SetInvincible(true);
            if (m_Player && m_Player->GetDamageable()) m_Player->GetDamageable()->SetInvincible(true);
        }
        else if (m_Player && m_Player->GetDamageable() && m_Player->GetDamageable()->IsDead())
        {
            m_State = State::PlayerDeadWait;
            m_StateTimer = 0.0f;
            m_Boss->SetBehaviorLocked(true); // ボスの動きを止める
            m_Player->GetDamageable()->SetInvincible(true);
        }
        break;

    case State::EnemyDeadSlow:
        m_StateTimer += unscaledDt; // 実際の時間で計測
        {
            bool playerIsIdle = false;
            if (m_Player)
            {
                auto* animator = m_Player->GetComponent<Animator>();
                // Playerが攻撃を終えてIdleに戻ったかをチェック
                if (animator && animator->GetCurrentClipName() == "Idle")
                {
                    playerIsIdle = true;
                }
            }

            // Playerの攻撃アニメーションが終了するか、または最大で3.0秒経過したらスローを解除
            if (playerIsIdle || m_StateTimer >= 3.0f)
            {
                m_State = State::EnemyDeadWait;
                m_StateTimer = 0.0f;
                Manager::SetTimeScale(1.0f); // スロー終了解除（Enemyも通常速度になる）
            }
        }
        break;

    case State::EnemyDeadWait:
        m_StateTimer += unscaledDt; // 実際の時間で計測
        if (m_StateTimer >= 1.2f) // 通常速度で敵が倒れるのを1.2秒見守る
        {
            m_State = State::EnemyDeadFade;
            m_FadeAlpha = 1.0f; // 透明からスタート
            m_PostProcess.AddEffect(PostEffectType::FADE, { m_FadeAlpha, 0.0f, 0.0f, 0.0f });
        }
        break;

    case State::EnemyDeadFade:
        m_FadeAlpha -= unscaledDt * 1.0f; // 1.0(透明)から0.0(黒)へ
        m_PostProcess.UpdateEffectParam(PostEffectType::FADE, { m_FadeAlpha, 0.0f, 0.0f, 0.0f });
        if (m_FadeAlpha <= 0.0f)
        {
            Manager::SetScene<GameClearScene>();
        }
        break;

    case State::PlayerDeadWait:
        m_StateTimer += unscaledDt;
        if (m_StateTimer >= 1.5f) // 1.5秒待ってからアイリスアウト開始
        {
            m_State = State::PlayerDeadIrisOut;
            m_IrisRadius = 1.0f;
            m_PostProcess.AddEffect(PostEffectType::IRIS_OUT, { 1.0f, 0.5f, 0.5f, 0.0f });
        }
        break;

    case State::PlayerDeadIrisOut:
        m_IrisRadius -= unscaledDt * 0.8f; // アイリスアウトの速度

        // プレイヤーのスクリーン座標を計算してシェーダーに渡す
        if (m_Camera && m_Player)
        {
            // 2D座標の差分と画面中央のオフセットから画面上の座標を算出
            float screenX = (m_Player->transform.position.x - m_Camera->transform.position.x) + SCREEN_WIDTH * 0.5f;
            float screenY = (m_Player->transform.position.y - m_Camera->transform.position.y) + SCREEN_HEIGHT * 0.5f;
            
            // UV (0 ~ 1) へ変換
            float uvX = screenX / SCREEN_WIDTH;
            float uvY = screenY / SCREEN_HEIGHT;

            m_PostProcess.UpdateEffectParam(PostEffectType::IRIS_OUT, { m_IrisRadius, uvX, uvY, 0.0f });
        }

        if (m_IrisRadius <= 0.0f)
        {
            Manager::SetScene<TitleScene>();
        }
        break;
    }

    // 背景のパララックススクロール
    if (m_Camera)
    {
        m_Background->SetCameraPosition(m_Camera->transform.position.x, m_Camera->transform.position.y);
        m_Background->transform.position = m_Camera->transform.position;
    }
}

//=============================================================================
// 描画
//=============================================================================

void GameScene::Draw()
{
    // ゲーム描画をオフスクリーンにリダイレクト開始
    m_PostProcess.Begin();

    // メインカメラの描画
    auto* mainCamera = GetGameObject<MainCamera>();
    if (mainCamera && mainCamera->GetCamera())
    {
        mainCamera->GetCamera()->Draw();
    }

    // ゲームオブジェクトの描画
    Scene::Draw(0, static_cast<int>(Layer::UI));

    // デバッグ描画
    PhysicsManager::DrawDebug();

    // ゲーム描画をオフスクリーンから戻す
    m_PostProcess.End();

    // Canvasの描画: オフスクリーンの影響を受けない
    Renderer::SetViewMatrix(DirectX::XMMatrixIdentity());

    // UI描画: オフスクリーンの影響を受けない
    Scene::Draw(static_cast<int>(Layer::UI), static_cast<int>(Layer::LAYER_MAX));
}
