#pragma once
#include "../../core/gameObject.h"

class Player;
class Enemy;
class BossIntroLetterbox;
class MainCamera;
class BoxCollider2D;
class PostProcess;
class EnemyUI;

//=============================================================================
// BossIntroDirector
// ボスイベント演出のシーケンス全体を管理するコントローラ
//
// 演出フロー:
//   IDLE → LETTERBOX_IN → PLAYER_WALK → BOSS_TAUNT → LETTERBOX_OUT → FINISHED
//=============================================================================
class BossIntroDirector : public GameObject
{
private:
    enum State
    {
        STATE_IDLE,           // 待機状態
        STATE_LETTERBOX_IN,   // 黒帯スライドイン
        STATE_PLAYER_WALK,    // プレイヤーが定位置まで移動
        STATE_BOSS_TAUNT,     // ボスが威嚇アニメーション
        STATE_LETTERBOX_OUT,  // 黒帯スライドアウト
        STATE_FINISHED        // 演出終了・操作解放済み
    };

    State  m_State = STATE_IDLE;
    float  m_Timer = 0.0f;

    // --- 参照 ---
    PostProcess*        m_PostProcess     = nullptr;
    Player*             m_Player          = nullptr;
    Enemy*              m_Boss            = nullptr;
    MainCamera*         m_Camera          = nullptr;
	EnemyUI* m_EnemyUI = nullptr;

    // ボスエリアの境界壁（ボス戦開始後に生成）
    GameObject*         m_WallLeft        = nullptr;
    GameObject*         m_WallRight       = nullptr;

    // プレイヤーの目標X座標
    float m_PlayerTargetX = 0.0f;
    float m_LetterboxThickness = 0.0f; // レターボックスの現在の厚み (0.0～0.2)

    // --- 各フェーズのパラメータ ---
    static constexpr float LETTERBOX_IN_DURATION  = 0.8f;  // 黒帯スライドインの時間
    static constexpr float PLAYER_WALK_TIMEOUT    = 5.0f;  // プレイヤー移動のタイムアウト
    static constexpr float TAUNT_WAIT_BEFORE      = 0.4f;  // 威嚇アニメ開始前の溜め秒数
    static constexpr float TAUNT_ANIM_DURATION    = 1.2f;  // 威嚇アニメの最低再生時間
    static constexpr float LETTERBOX_OUT_DURATION = 0.6f;  // 黒帯スライドアウトの時間
    // ボスエリアの幅（固定カメラ範囲の半幅）
    static constexpr float BOSS_AREA_HALF_WIDTH = 640.0f;

public:
    void Init()           override;
    void Update(float dt) override;

    // 外部セッター（GameScene側でセットする）
    void SetPostProcess(PostProcess* pp){ m_PostProcess = pp; }
    void SetPlayer(Player* p)     { m_Player = p; }
    void SetBoss(Enemy* b)        { m_Boss   = b; }
    void SetCamera(MainCamera* c) { m_Camera = c; }
	void SetEnemyUI(EnemyUI* ui) { m_EnemyUI = ui; }

    // 演出開始
    void StartEvent();

    // 演出が完全に終わったか
    bool IsFinished() const { return m_State == STATE_FINISHED; }

private:
    // プレイヤーの入力・物理を無効化する
    void LockPlayer();
    // プレイヤーの入力・物理を有効化する
    void UnlockPlayer();

    // 各フェーズ更新ヘルパー
    void UpdateLetterboxIn(float dt);
    void UpdatePlayerWalk(float dt);
    void UpdateBossTaunt(float dt);
    void UpdateLetterboxOut(float dt);

    // ボス戦開始処理（固定カメラ・境界壁の生成）
    void BeginBossBattle();

    // ボスエリア境界壁を生成
    void SpawnBoundaryWalls();
};
