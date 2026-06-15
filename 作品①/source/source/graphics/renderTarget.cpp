#include "renderTarget.h"
#include "renderer.h"

//=============================================================================
// RenderTarget 実装
//=============================================================================

bool RenderTarget::Init(UINT width, UINT height, DXGI_FORMAT format)
{
    m_Width  = width;
    m_Height = height;
    auto* dev = Renderer::GetDevice();

    // カラーテクスチャ
    {
        D3D11_TEXTURE2D_DESC td = {};
        td.Width              = width;
        td.Height             = height;
        td.MipLevels          = 1;
        td.ArraySize          = 1;
        td.Format             = format;
        td.SampleDesc.Count   = 1;
        td.Usage              = D3D11_USAGE_DEFAULT;
        td.BindFlags          = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;

        if (FAILED(dev->CreateTexture2D(&td, nullptr, &m_Texture))) return false;
        if (FAILED(dev->CreateRenderTargetView(m_Texture, nullptr, &m_RTV))) return false;
        if (FAILED(dev->CreateShaderResourceView(m_Texture, nullptr, &m_SRV))) return false;
    }

    // デプステクスチャ
    {
        D3D11_TEXTURE2D_DESC td = {};
        td.Width              = width;
        td.Height             = height;
        td.MipLevels          = 1;
        td.ArraySize          = 1;
        td.Format             = DXGI_FORMAT_D24_UNORM_S8_UINT;
        td.SampleDesc.Count   = 1;
        td.Usage              = D3D11_USAGE_DEFAULT;
        td.BindFlags          = D3D11_BIND_DEPTH_STENCIL;

        if (FAILED(dev->CreateTexture2D(&td, nullptr, &m_DSTexture))) return false;
        if (FAILED(dev->CreateDepthStencilView(m_DSTexture, nullptr, &m_DSV))) return false;
    }

    return true;
}

void RenderTarget::Uninit()
{
    auto SafeRelease = [](auto*& p){ if(p){ p->Release(); p=nullptr; } };
    SafeRelease(m_SRV);
    SafeRelease(m_RTV);
    SafeRelease(m_Texture);
    SafeRelease(m_DSV);
    SafeRelease(m_DSTexture);
}

void RenderTarget::Begin(const float clearColor[4])
{
    auto* ctx = Renderer::GetDeviceContext();

    ctx->OMSetRenderTargets(1, &m_RTV, m_DSV);

    static const float defaultClear[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
    const float* cc = clearColor ? clearColor : defaultClear;
    ctx->ClearRenderTargetView(m_RTV, cc);
    ctx->ClearDepthStencilView(m_DSV, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

    // ビューポート
    D3D11_VIEWPORT vp = {};
    vp.Width    = static_cast<float>(m_Width);
    vp.Height   = static_cast<float>(m_Height);
    vp.MaxDepth = 1.0f;
    ctx->RSSetViewports(1, &vp);
}

void RenderTarget::End()
{
    auto* ctx = Renderer::GetDeviceContext();
    auto* rtv = Renderer::GetBackBufferRTV();
    auto* dsv = Renderer::GetDepthStencilView();
    ctx->OMSetRenderTargets(1, &rtv, dsv);

    D3D11_VIEWPORT vp = {};
    vp.Width    = static_cast<float>(SCREEN_WIDTH);
    vp.Height   = static_cast<float>(SCREEN_HEIGHT);
    vp.MaxDepth = 1.0f;
    ctx->RSSetViewports(1, &vp);
}
