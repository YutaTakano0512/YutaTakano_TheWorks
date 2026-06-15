#include "gameClearScene.h"
#include "../../core/manager.h"
#include "../../input/input.h"
#include "../../input/gamepad.h"
#include "titleScene.h"
#include <cmath>

void GameClearScene::Init()
{
    m_PostProcess.Init();
    m_PostProcess.AddEffect(PostEffectType::FADE, { 1.0f, 0.0f, 0.0f, 0.0f });

    m_Camera = AddGameObject<MainCamera>(Layer::UI);

    // GAME CLEAR テキスト
    auto textObj1 = AddGameObject<GameObject>(Layer::UI);
    m_ClearText = textObj1->AddComponent<TextRenderer>();
    m_ClearText->fontFamily = L"Meiryo";
    m_ClearText->text = L"GAME CLEAR";
    m_ClearText->fontSize = 64.0f;
    m_ClearText->color = { 1.0f, 1.0f, 0.0f, 1.0f }; // 黄色
    textObj1->transform.position = { SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.4f };

    // PRESS ANY KEY
    auto textObj2 = AddGameObject<GameObject>(Layer::UI);
    m_PressKeyText = textObj2->AddComponent<TextRenderer>();
    m_PressKeyText->fontFamily = L"Meiryo";
    m_PressKeyText->text = L"PRESS ANY KEY";
    m_PressKeyText->fontSize = 32.0f;
    m_PressKeyText->color = { 1.0f, 1.0f, 1.0f, 1.0f };
    textObj2->transform.position = { SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.7f };

    m_State = State::FadeIn;
    m_FadeAlpha = 1.0f;
    m_StateTimer = 0.0f;
}

void GameClearScene::Uninit()
{
    m_PostProcess.Uninit();
}

void GameClearScene::Update(float dt)
{
    Scene::Update(dt);

    switch (m_State)
    {
    case State::FadeIn:
        m_FadeAlpha -= dt * 1.0f;
        if (m_FadeAlpha <= 0.0f)
        {
            m_FadeAlpha = 0.0f;
            m_State = State::WaitInput;
        }
        break;

    case State::WaitInput:
        m_StateTimer += dt * 3.0f;
        if(m_PressKeyText) m_PressKeyText->color.w = (std::sinf(m_StateTimer) * 0.5f) + 0.5f;

        {
            bool isAnyKeyDown = false;
            for (int i = 0; i < 256; i++) {
                if (Input::GetKeyDown(i)) { isAnyKeyDown = true; break; }
            }
            if (Gamepad::GetButtonDown(XINPUT_GAMEPAD_A) ||
                Gamepad::GetButtonDown(XINPUT_GAMEPAD_B) ||
                Gamepad::GetButtonDown(XINPUT_GAMEPAD_X) ||
                Gamepad::GetButtonDown(XINPUT_GAMEPAD_Y) ||
                Gamepad::GetButtonDown(XINPUT_GAMEPAD_START))
            {
                isAnyKeyDown = true;
            }

            if (isAnyKeyDown)
            {
                m_State = State::FadeOut;
                if(m_PressKeyText) m_PressKeyText->color.w = 1.0f;
            }
        }
        break;

    case State::FadeOut:
        m_FadeAlpha += dt * 1.5f;
        if (m_FadeAlpha >= 1.0f)
        {
            m_FadeAlpha = 1.0f;
            Manager::SetScene<TitleScene>();
        }
        break;
    }

    m_PostProcess.UpdateEffectParam(PostEffectType::FADE, { m_FadeAlpha, 0.0f, 0.0f, 0.0f });
}

void GameClearScene::Draw()
{
    m_PostProcess.Begin();
    if (m_Camera && m_Camera->GetCamera()) m_Camera->GetCamera()->Draw();
    Scene::Draw(0, static_cast<int>(Layer::UI));
    m_PostProcess.End();

    Renderer::SetViewMatrix(DirectX::XMMatrixIdentity());
    Scene::Draw(static_cast<int>(Layer::UI), static_cast<int>(Layer::LAYER_MAX));
}
