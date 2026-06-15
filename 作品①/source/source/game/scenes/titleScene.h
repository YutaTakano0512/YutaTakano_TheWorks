#pragma once
#include "../../core/scene.h"
#include "../../graphics/postProcess.h"
#include "../../graphics/textRenderer.h"
#include "../../graphics/spriteRenderer.h"
#include "../objects/mainCamera.h"

//=============================================================================
// TitleScene
//=============================================================================
class TitleScene : public Scene
{
private:
    PostProcess  m_PostProcess;
    MainCamera* m_Camera = nullptr;

	TextRenderer* m_PressKeyText = nullptr;
    SpriteRenderer* m_FadeSprite = nullptr;

    // ÉVÅ[ÉìÇÃèÛë‘
    enum class State
    {
        FadeIn,
		WaitInput,
        Blinking,
        FadeOut
    };
	State m_State = State::FadeIn;

	float m_FadeAlpha = 0.0f;
	float m_BlinkTimer = 0.0f;
	float m_StateTimer = 0.0f;

public:
    void Init() override;
    void Uninit() override;
    void Update(float dt) override;
    void Draw() override;
};