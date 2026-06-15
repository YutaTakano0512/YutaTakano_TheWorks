#pragma once
#include <memory>
#include "scene.h"

//=============================================================================
// Manager クラス (シングルトン的static管理クラス)
// シーンの切り替え・時間管理・グローバルアクセスを提供
//=============================================================================
class Manager
{
public:
    static void Init();
    static void Uninit();
    static void Update();
    static void Draw();

    //-------------------------------------------------------------------------
    // シーン管理
    //-------------------------------------------------------------------------
    static Scene* GetScene() { return m_Scene.get(); }

    template<typename T>
    static void SetScene()
    {
        m_SceneNext = std::make_unique<T>();
    }

    //-------------------------------------------------------------------------
    // 時間管理
    //-------------------------------------------------------------------------
    // TimeScale適用済みdeltaTime (ポーズ・スローモー考慮)
    static float GetDeltaTime()         { return (1.0f / 60.0f) * m_TimeScale; }
    // TimeScale非依存 (ポーズ中のUIなど)
    static float GetUnscaledDeltaTime() { return 1.0f / 60.0f; }

    static float GetTimeScale()               { return m_TimeScale; }
    static void  SetTimeScale(float t)        { m_TimeScale = t; }

    // ヒットストップ (指定秒数だけTimeScaleを0に近づける)
    static void  SetHitStop(float duration);
    static bool  IsHitStopping() { return m_HitStopTimer > 0.0f; }

    //-------------------------------------------------------------------------
    // ポーズ
    //-------------------------------------------------------------------------
    static bool IsPaused()             { return m_IsPaused; }
    static void SetPaused(bool paused) { m_IsPaused = paused; }
    static void TogglePause()          { m_IsPaused = !m_IsPaused; }

private:
    static std::unique_ptr<Scene> m_Scene;
    static std::unique_ptr<Scene> m_SceneNext;

    static float m_TimeScale;
    static float m_HitStopTimer;
    static bool  m_IsPaused;

    // シーン遷移の実行
    static void TransitionScene();
};
