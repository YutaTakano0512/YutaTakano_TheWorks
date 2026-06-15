#include "textRenderer.h"
#include "../core/gameObject.h"

void TextRenderer::Init()
{
    m_Sprite.Init();
    UpdateTextTexture();
}

void TextRenderer::Uninit()
{
    m_Sprite.Uninit();
    ReleaseResources();
}

void TextRenderer::Update(float dt)
{
    bool changed = false;
    if (text != m_PrevText) changed = true;
    if (fontFamily != m_PrevFontFamily) changed = true;
    if (fontSize != m_PrevFontSize) changed = true;
    if (color.x != m_PrevColor.x || color.y != m_PrevColor.y || color.z != m_PrevColor.z || color.w != m_PrevColor.w) changed = true;

    if (changed)
    {
        UpdateTextTexture();
    }
}

void TextRenderer::ReleaseResources()
{
    if (m_Brush) { m_Brush->Release(); m_Brush = nullptr; }
    if (m_TextFormat) { m_TextFormat->Release(); m_TextFormat = nullptr; }
    if (m_D2DRenderTarget) { m_D2DRenderTarget->Release(); m_D2DRenderTarget = nullptr; }
    if (m_SRV) { m_SRV->Release(); m_SRV = nullptr; }
    if (m_Texture) { m_Texture->Release(); m_Texture = nullptr; }
}

void TextRenderer::UpdateTextTexture()
{
    ReleaseResources();

    m_PrevText = text;
    m_PrevFontFamily = fontFamily;
    m_PrevFontSize = fontSize;
    m_PrevColor = color;

    if (text.empty())
    {
        size = { 0.0f, 0.0f };
        return;
    }

    IDWriteFactory* dwriteFactory = Renderer::GetDWriteFactory();
    ID2D1Factory* d2dFactory = Renderer::GetD2DFactory();
    ID3D11Device* device = Renderer::GetDevice();

    if (!dwriteFactory || !d2dFactory || !device) return;

    HRESULT hr = dwriteFactory->CreateTextFormat(
        fontFamily.c_str(),
        nullptr,
        DWRITE_FONT_WEIGHT_NORMAL,
        DWRITE_FONT_STYLE_NORMAL,
        DWRITE_FONT_STRETCH_NORMAL,
        fontSize,
        L"ja-jp",
        &m_TextFormat
    );
    if (FAILED(hr)) return;

    IDWriteTextLayout* textLayout = nullptr;
    hr = dwriteFactory->CreateTextLayout(
        text.c_str(),
        (UINT32)text.length(),
        m_TextFormat,
        4096.0f,
        4096.0f,
        &textLayout
    );
    if (FAILED(hr)) return;

    DWRITE_TEXT_METRICS metrics;
    textLayout->GetMetrics(&metrics);
    textLayout->Release();

    int texWidth = (int)(metrics.width + 2.0f);
    int texHeight = (int)(metrics.height + 2.0f);
    
    if (texWidth <= 0) texWidth = 1;
    if (texHeight <= 0) texHeight = 1;

    size = { (float)texWidth, (float)texHeight };

    D3D11_TEXTURE2D_DESC texDesc = {};
    texDesc.Width = texWidth;
    texDesc.Height = texHeight;
    texDesc.MipLevels = 1;
    texDesc.ArraySize = 1;
    texDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
    texDesc.SampleDesc.Count = 1;
    texDesc.Usage = D3D11_USAGE_DEFAULT;
    texDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
    texDesc.MiscFlags = D3D11_RESOURCE_MISC_GDI_COMPATIBLE;

    hr = device->CreateTexture2D(&texDesc, nullptr, &m_Texture);
    if (FAILED(hr)) return;

    hr = device->CreateShaderResourceView(m_Texture, nullptr, &m_SRV);
    if (FAILED(hr)) return;

    IDXGISurface* dxgiSurface = nullptr;
    hr = m_Texture->QueryInterface(__uuidof(IDXGISurface), (void**)&dxgiSurface);
    if (FAILED(hr)) return;

    D2D1_RENDER_TARGET_PROPERTIES props = D2D1::RenderTargetProperties(
        D2D1_RENDER_TARGET_TYPE_DEFAULT,
        D2D1::PixelFormat(DXGI_FORMAT_UNKNOWN, D2D1_ALPHA_MODE_PREMULTIPLIED),
        96.0f, 96.0f
    );
    hr = d2dFactory->CreateDxgiSurfaceRenderTarget(dxgiSurface, &props, &m_D2DRenderTarget);
    dxgiSurface->Release();
    if (FAILED(hr)) return;

    hr = m_D2DRenderTarget->CreateSolidColorBrush(
        D2D1::ColorF(color.x, color.y, color.z, color.w),
        &m_Brush
    );
    if (FAILED(hr)) return;

    m_D2DRenderTarget->BeginDraw();
    m_D2DRenderTarget->Clear(D2D1::ColorF(0.0f, 0.0f, 0.0f, 0.0f));

    IDWriteTextLayout* finalLayout = nullptr;
    hr = dwriteFactory->CreateTextLayout(
        text.c_str(),
        (UINT32)text.length(),
        m_TextFormat,
        (float)texWidth,
        (float)texHeight,
        &finalLayout
    );
    if (SUCCEEDED(hr))
    {
        m_D2DRenderTarget->DrawTextLayout(D2D1::Point2F(0.0f, 0.0f), finalLayout, m_Brush);
        finalLayout->Release();
    }

    m_D2DRenderTarget->EndDraw();
}

void TextRenderer::Draw()
{
    if (!m_SRV || text.empty()) return;

    Renderer::SetBlendMode(blendMode);
    
    // UIとして綺麗に描画するためにリニアサンプリング
    Sprite::SetPointSampling(false);

    // Transformを適用して描画
    m_Sprite.DrawWithMatrix(
        m_SRV, 
        m_GameObject->transform.GetWorldMatrix(), 
        size, 
        DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),
        Vector2(0.0f, 0.0f), 
        Vector2(1.0f, 1.0f)
    );
}
