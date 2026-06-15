#include "manager.h"
#include "../graphics/renderer.h"
#include "../input/input.h"
#include "../input/mouseInput.h"
#include "../input/gamepad.h"
#include "../audio/audio.h"
#include "../physics/physicsManager.h"
#include "../game/scenes/titleScene.h"
#include <algorithm>

//=============================================================================
// 静的メンバ定義
//=============================================================================
std::unique_ptr<Scene> Manager::m_Scene;
std::unique_ptr<Scene> Manager::m_SceneNext;
float Manager::m_TimeScale    = 1.0f;
float Manager::m_HitStopTimer = 0.0f;
bool  Manager::m_IsPaused     = false;

//=============================================================================
// Init / Uninit
//=============================================================================
void Manager::Init()
{
    Renderer::Init();
    Input::Init();
    MouseInput::Init();
    Gamepad::Init();
    Audio::InitMaster();
    PhysicsManager::Init();

    // 最初のシーンをセット
    m_Scene = std::make_unique<TitleScene>();
    m_Scene->Init();
}

void Manager::Uninit()
{
    if (m_Scene)
    {
        m_Scene->Uninit();
        m_Scene.reset();
    }
    if (m_SceneNext)
    {
        m_SceneNext.reset();
    }

    PhysicsManager::Uninit();
    Audio::UninitMaster();
    Gamepad::Uninit();
    MouseInput::Uninit();
    Input::Uninit();
    Renderer::Uninit();
}

//=============================================================================
// Update
//=============================================================================
void Manager::Update()
{
    // 入力更新
    Input::Update();
    MouseInput::Update();
    Gamepad::Update();

    // HitStop タイマー
    if (m_HitStopTimer > 0.0f)
    {
        m_HitStopTimer -= GetUnscaledDeltaTime();
        if (m_HitStopTimer < 0.0f) m_HitStopTimer = 0.0f;
    }

    float dt = GetDeltaTime();
    if (!m_IsPaused && m_HitStopTimer <= 0.0f)
    {
        if (m_Scene)
        {
            m_Scene->Update(dt);
        }

        // 物理更新
        PhysicsManager::Update(dt);
    }

    // シーン遷移チェック
    TransitionScene();
}

//=============================================================================
// Draw
//=============================================================================
void Manager::Draw()
{
    Renderer::Begin();

    if (m_Scene)
        m_Scene->Draw();

    Renderer::End();
}

//=============================================================================
// HitStop
//=============================================================================
void Manager::SetHitStop(float duration)
{
    m_HitStopTimer = std::max(m_HitStopTimer, duration);
}

//=============================================================================
// シーン遷移
//=============================================================================
void Manager::TransitionScene()
{
    if (!m_SceneNext) return;

    if (m_Scene)
        m_Scene->Uninit();

    PhysicsManager::Clear();

    m_Scene     = std::move(m_SceneNext);
    m_SceneNext = nullptr;
    m_Scene->Init();
}
