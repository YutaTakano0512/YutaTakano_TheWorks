#pragma once
#include "../../core/scene.h"
#include "../../graphics/postProcess.h"
#include "../../graphics/textRenderer.h"
#include "../objects/mainCamera.h"

//=============================================================================
// GameClearScene
//=============================================================================
class GameClearScene : public Scene
{
private:
    PostProcess  m_PostProcess;
    MainCamera* m_Camera = nullptr;
    TextRenderer* m_ClearText = nullptr;
    TextRenderer* m_PressKeyText = nullptr;

    float m_StateTimer = 0.0f;
    float m_FadeAlpha = 1.0f; // フェードイン用

    enum class State {
        FadeIn,
        WaitInput,
        FadeOut
    };
    State m_State = State::FadeIn;

public:
    void Init() override;
    void Uninit() override;
    void Update(float dt) override;
    void Draw() override;
};
