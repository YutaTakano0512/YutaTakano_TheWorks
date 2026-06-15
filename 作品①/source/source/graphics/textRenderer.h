#pragma once
#include "../core/component.h"
#include "../math/vector2.h"
#include <string>
#include <d3d11.h>
#include <d2d1.h>
#include <dwrite.h>
#include <DirectXMath.h>
#include "sprite.h"
#include "renderer.h"

//=============================================================================
// TextRenderer コンポーネント
// UI等のテキスト描画を行う。Direct2D/DirectWriteを利用してテクスチャを生成する。
//=============================================================================
class TextRenderer : public Component
{
public:
    std::wstring text = L"";
    std::wstring fontFamily = L"Meiryo";
    float fontSize = 32.0f;
    DirectX::XMFLOAT4 color = { 1.0f, 1.0f, 1.0f, 1.0f }; // RGBA

    // 描画サイズ（文字列に合わせて自動計算される）
    Vector2 size = { 0.0f, 0.0f };

    // ブレンドモード
    BlendMode blendMode = BlendMode::NORMAL;

    TextRenderer() = default;
    virtual ~TextRenderer() = default;

    void Init() override;
    void Uninit() override;
    void Update(float dt) override;
    void Draw() override;

    // テキスト、フォント、サイズ、色などを変更した後にテクスチャを再生成する
    void UpdateTextTexture();

private:
    void ReleaseResources();

    ID3D11Texture2D* m_Texture = nullptr;
    ID3D11ShaderResourceView* m_SRV = nullptr;
    ID2D1RenderTarget* m_D2DRenderTarget = nullptr;

    IDWriteTextFormat* m_TextFormat = nullptr;
    ID2D1SolidColorBrush* m_Brush = nullptr;

    Sprite m_Sprite;

    // 変更検知用
    std::wstring m_PrevText;
    std::wstring m_PrevFontFamily;
    float m_PrevFontSize = 0.0f;
    DirectX::XMFLOAT4 m_PrevColor = { 0.0f, 0.0f, 0.0f, 0.0f };
};
