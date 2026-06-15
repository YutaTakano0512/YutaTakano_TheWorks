#pragma once
#include "../core/main.h"

//=============================================================================
// RenderTarget クラス
// オフスクリーンレンダリング用テクスチャ。
// ポストプロセスや中間バッファとして使用。
//=============================================================================
class RenderTarget
{
public:
    RenderTarget() = default;
    ~RenderTarget() { Uninit(); }

    // 作成 (指定解像度、デフォルトは画面サイズ)
    bool Init(UINT width = SCREEN_WIDTH, UINT height = SCREEN_HEIGHT,
              DXGI_FORMAT format = DXGI_FORMAT_R8G8B8A8_UNORM);
    void Uninit();

    // このRTへのレンダリング開始
    void Begin(const float clearColor[4] = nullptr);

    // バックバッファに戻す
    static void End();

    // SRV (シェーダーリソース) として取得
    ID3D11ShaderResourceView* GetSRV() const { return m_SRV; }

    UINT GetWidth()  const { return m_Width; }
    UINT GetHeight() const { return m_Height; }

private:
    ID3D11Texture2D*          m_Texture = nullptr;
    ID3D11RenderTargetView*   m_RTV     = nullptr;
    ID3D11ShaderResourceView* m_SRV     = nullptr;
    ID3D11DepthStencilView*   m_DSV     = nullptr;
    ID3D11Texture2D*          m_DSTexture = nullptr;

    UINT m_Width  = 0;
    UINT m_Height = 0;
};
