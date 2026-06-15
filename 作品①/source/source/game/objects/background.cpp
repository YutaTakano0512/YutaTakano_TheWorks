#include "background.h"
#include "../../core/scene.h"
#include "../../core/manager.h"
#include <cmath>

static const char* g_TexturePaths[BG_LAYER_COUNT] =
{
    "asset/texture/backgrounds/layer0.png", // 空
    "asset/texture/backgrounds/layer1.png", // 最遠景
    "asset/texture/backgrounds/layer2.png", // 遠景
    "asset/texture/backgrounds/layer3.png", // 中景
    "asset/texture/backgrounds/layer4.png", // 中近景
    "asset/texture/backgrounds/layer5.png", // 近景
    "asset/texture/backgrounds/layer6.png", // 最前景
};

static const float g_ParallaxRatesX[BG_LAYER_COUNT] =
{
    0.00f, // layer0: 空
    0.05f, // layer1: 最遠景
    0.10f, // layer2: 遠景
    0.20f, // layer3: 中景
    0.35f, // layer4: 中近景
    0.55f, // layer5: 近景
    0.80f, // layer6: 最前景
};

static const float g_ParallaxRatesY[BG_LAYER_COUNT] =
{
    0.00f, // layer0: 空
    0.01f, // layer1: 最遠景
    0.02f, // layer2: 遠景
    0.04f, // layer3: 中景
    0.06f, // layer4: 中近景
    0.08f, // layer5: 近景
    0.10f, // layer6: 最前景
};

//=============================================================================
// 初期化
//=============================================================================
void Background::Init()
{
    for (int i = 0; i < BG_LAYER_COUNT; i++)
    {
        // スプライトコンポーネントを追加
        m_Sprite[i] = AddComponent<SpriteRenderer>();
        m_Sprite[i]->SetTexture(g_TexturePaths[i]);

        // 画面全体を覆うサイズ
        m_Sprite[i]->size  = { (float)SCREEN_WIDTH, (float)SCREEN_HEIGHT };
        m_Sprite[i]->pivot = { 0.5f, 0.5f };
        m_Sprite[i]->pixelArt = true; // ドット絵モード（拡大してもギザギザを維持）

        // 視差率とUVオフセットを初期化
        m_ParallaxRateX[i] = g_ParallaxRatesX[i];
        m_ParallaxRateY[i] = g_ParallaxRatesY[i];
        m_UVOffsetX[i]     = 0.0f;
        m_UVOffsetY[i]     = 0.0f;
    }

    // オブジェクトを画面中心に配置（スクロールしない固定位置）
    transform.position = { (float)SCREEN_WIDTH * 0.5f, -(float)SCREEN_HEIGHT * 0.5f };
    transform.zOrder   = -100.0f;
}

//=============================================================================
// 更新
//=============================================================================
void Background::Update(float dt)
{
    GameObject::Update(dt);
}

//=============================================================================
// 描画
//=============================================================================
void Background::Draw()
{
    if (!IsActive()) return;

    for (int i = 0; i < BG_LAYER_COUNT; i++)
    {
        m_Sprite[i]->uvMin = { m_UVOffsetX[i], m_UVOffsetY[i] };
        m_Sprite[i]->uvMax = { m_UVOffsetX[i] + 1.0f, m_UVOffsetY[i] + 1.0f };

        m_Sprite[i]->Draw();
    }
}

void Background::SetCameraPosition(float cameraX, float cameraY)
{
    for (int i = 0; i < BG_LAYER_COUNT; i++)
    {
        float uvX = cameraX * m_ParallaxRateX[i] / (float)SCREEN_WIDTH;
        m_UVOffsetX[i] = fmodf(uvX, 1.0f);
        if (m_UVOffsetX[i] < 0.0f)
        {
            m_UVOffsetX[i] += 1.0f;
        }

        float uvY = cameraY * m_ParallaxRateY[i] / (float)SCREEN_HEIGHT;
        m_UVOffsetY[i] = fmodf(uvY, 1.0f);
        if (m_UVOffsetY[i] < 0.0f)
        {
            m_UVOffsetY[i] += 1.0f;
        }
    }
}
