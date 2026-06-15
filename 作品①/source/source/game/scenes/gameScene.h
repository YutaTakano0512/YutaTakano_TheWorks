#pragma once
#include "../../core/scene.h"
#include "../../graphics/postProcess.h"
#include "../objects/mainCamera.h"
#include "../objects/background.h"
#include "../objects/player.h"
#include "../objects/enemy.h"

//=============================================================================
// GameScene
//=============================================================================
class GameScene : public Scene
{
private:
    PostProcess  m_PostProcess;
    Background*  m_Background = nullptr;
    MainCamera* m_Camera = nullptr;

    Player* m_Player = nullptr;
    Enemy* m_Boss = nullptr;

    enum class State {
        FadeIn,
        Playing,
        EnemyDeadSlow,
        EnemyDeadWait,
        EnemyDeadFade,
        PlayerDeadWait,
        PlayerDeadIrisOut
    };
    State m_State = State::FadeIn;

    float m_StateTimer = 0.0f;
    float m_FadeAlpha = 0.0f;
    float m_IrisRadius = 1.0f;

public:
    void Init() override;
    void Uninit() override;
    void Update(float dt) override;
    void Draw() override;
};
