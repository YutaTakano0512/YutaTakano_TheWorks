#include "titleScene.h"
#include "../../core/manager.h"
#include "../../input/input.h"
#include "../../input/gamepad.h"
#include "gameScene.h"
#include <cmath>

void TitleScene::Init()
{
    m_PostProcess.Init();
	m_PostProcess.AddEffect(PostEffectType::FADE, { 0.0f,0.0f,0.0f,0.0f });

    // メインカメラの生成
	m_Camera = AddGameObject<MainCamera>(Layer::UI);

	auto bgObj = AddGameObject<GameObject>(Layer::BACKGROUND);
	auto bgSprite = bgObj->AddComponent<SpriteRenderer>();
	bgSprite->SetTexture("asset/texture/title/title_bg.png");
	bgSprite->size = { SCREEN_WIDTH, SCREEN_HEIGHT };
	bgObj->transform.position = { SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f };

	auto textObj = AddGameObject<GameObject>(Layer::UI);
	m_PressKeyText = textObj->AddComponent<TextRenderer>();
	m_PressKeyText->fontFamily = L"Meiryo";
	m_PressKeyText->text = L"PRESS ANY KEY";
	m_PressKeyText->fontSize = 32.0f;
	m_PressKeyText->color = { 1.0f, 1.0f, 1.0f, 1.0f };
	textObj->transform.position = { SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.7f };

	m_State = State::FadeIn;
	m_FadeAlpha = 0.0f;
	m_BlinkTimer = 0.0f;
	m_StateTimer = 0.0f;
}

void TitleScene::Uninit()
{
	m_PostProcess.Uninit();
}

void TitleScene::Update(float dt)
{
    Scene::Update(dt);

	switch (m_State)
	{
	case State::FadeIn:
		m_FadeAlpha += dt * 1.5f;
		if (m_FadeAlpha >= 1.0f)
		{
			m_FadeAlpha = 1.0f;
			m_State = State::WaitInput;
		}
		if (m_PressKeyText)
		{
			m_PressKeyText->color.w = m_FadeAlpha;
		}
		break;

	case State::WaitInput:
		m_BlinkTimer += dt * 3.0f;
		m_PressKeyText->color.w = (std::sinf(m_BlinkTimer) * 0.5f) + 0.5f;
		{
			bool isAnyKeyDown = false;
			for (int i = 0; i < 256; i++)
			{
				if (Input::GetKeyDown(i))
				{
					isAnyKeyDown = true;
					break;
				}
			}

			if (isAnyKeyDown)
			{
				m_State = State::Blinking;
				m_BlinkTimer = 0.0f;
				m_StateTimer = 0.0f;
			}
		}
		break;

	case State::Blinking:
		m_BlinkTimer += dt * 25.0f;
		m_PressKeyText->color.w = (std::sinf(m_BlinkTimer) * 0.5f) + 0.5f;

		m_StateTimer += dt;
		if (m_StateTimer >= 1.0f)
		{
			m_State = State::FadeOut;
		}
		break;

	case State::FadeOut:
		m_FadeAlpha -= dt * 1.5f;
		if (m_FadeAlpha <= 0.0f)
		{
			m_FadeAlpha = 0.0f;
			Manager::SetScene<GameScene>();
		}
		if (m_PressKeyText)
		{
			m_PressKeyText->color.w = m_FadeAlpha;
		}
		break;

	default:
		break;
	}

	m_PostProcess.UpdateEffectParam(PostEffectType::FADE, { m_FadeAlpha,0.0f,0.0f,0.0f });
}

void TitleScene::Draw()
{
	m_PostProcess.Begin();

    // メインカメラの描画
	auto* mainCamera = GetGameObject<MainCamera>();
    if(mainCamera && mainCamera->GetCamera())
    {
        mainCamera->GetCamera()->Draw();
	}

	// ゲームオブジェクトの描画
	Scene::Draw(0, static_cast<int>(Layer::UI));

    m_PostProcess.End();

    Renderer::SetViewMatrix(DirectX::XMMatrixIdentity());
    Scene::Draw(static_cast<int>(Layer::UI), static_cast<int>(Layer::LAYER_MAX));
}
