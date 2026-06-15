#include "renderer.h"
#include <io.h>
#include <fstream>

//=============================================================================
// 静的メンバ定義
//=============================================================================
D3D_FEATURE_LEVEL        Renderer::m_FeatureLevel    = D3D_FEATURE_LEVEL_11_0;
ID3D11Device*            Renderer::m_Device          = nullptr;
ID3D11DeviceContext*     Renderer::m_DeviceContext   = nullptr;
IDXGISwapChain*          Renderer::m_SwapChain       = nullptr;
ID3D11RenderTargetView*  Renderer::m_RenderTargetView= nullptr;
ID3D11DepthStencilView*  Renderer::m_DepthStencilView= nullptr;

ID3D11Buffer* Renderer::m_WorldBuffer      = nullptr;
ID3D11Buffer* Renderer::m_ViewBuffer       = nullptr;
ID3D11Buffer* Renderer::m_ProjectionBuffer = nullptr;
ID3D11Buffer* Renderer::m_MaterialBuffer   = nullptr;
ID3D11Buffer* Renderer::m_TimeBuffer       = nullptr;
ID3D11Buffer* Renderer::m_ParameterBuffer  = nullptr;

ID3D11BlendState* Renderer::m_BlendNormal   = nullptr;
ID3D11BlendState* Renderer::m_BlendAdd      = nullptr;
ID3D11BlendState* Renderer::m_BlendMultiply = nullptr;
ID3D11BlendState* Renderer::m_BlendMask     = nullptr;

ID3D11DepthStencilState* Renderer::m_DepthEnabled  = nullptr;
ID3D11DepthStencilState* Renderer::m_DepthDisabled = nullptr;

ID3D11RasterizerState* Renderer::m_RasterCullNone = nullptr;

ID3D11SamplerState* Renderer::m_SamplerLinear = nullptr;
ID3D11SamplerState* Renderer::m_SamplerPoint  = nullptr;

float Renderer::m_TotalTime = 0.0f;
ID2D1Factory*   Renderer::m_D2DFactory    = nullptr;
IDWriteFactory* Renderer::m_DWriteFactory = nullptr;

//=============================================================================
// ヘルパー: 定数バッファ作成
//=============================================================================
static ID3D11Buffer* CreateConstantBuffer(ID3D11Device* dev, UINT byteWidth)
{
    D3D11_BUFFER_DESC desc = {};
    desc.ByteWidth           = byteWidth;
    desc.Usage               = D3D11_USAGE_DEFAULT;
    desc.BindFlags           = D3D11_BIND_CONSTANT_BUFFER;
    desc.CPUAccessFlags      = 0;
    desc.StructureByteStride = sizeof(float);

    ID3D11Buffer* buf = nullptr;
    dev->CreateBuffer(&desc, nullptr, &buf);
    return buf;
}

//=============================================================================
// Init
//=============================================================================
void Renderer::Init()
{
    HRESULT hr;

    // スワップチェーン記述
    DXGI_SWAP_CHAIN_DESC scDesc = {};
    scDesc.BufferCount                        = 1;
    scDesc.BufferDesc.Width                   = SCREEN_WIDTH;
    scDesc.BufferDesc.Height                  = SCREEN_HEIGHT;
    scDesc.BufferDesc.Format                  = DXGI_FORMAT_R8G8B8A8_UNORM;
    scDesc.BufferDesc.RefreshRate.Numerator   = 60;
    scDesc.BufferDesc.RefreshRate.Denominator = 1;
    scDesc.BufferUsage                        = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    scDesc.OutputWindow                       = GetWindowHandle();
    scDesc.SampleDesc.Count                   = 1;  // MSAA無し (2Dでは不要)
    scDesc.SampleDesc.Quality                 = 0;
    scDesc.Windowed                           = TRUE;

    UINT flags = 0;
#ifdef _DEBUG
    flags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

    flags |= D3D11_CREATE_DEVICE_BGRA_SUPPORT; // Direct2D 連携用
    hr = D3D11CreateDeviceAndSwapChain(
        nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, flags,
        nullptr, 0, D3D11_SDK_VERSION,
        &scDesc, &m_SwapChain, &m_Device, &m_FeatureLevel, &m_DeviceContext);
    if (FAILED(hr)) return;
    // Direct2D / DirectWrite ファクトリ初期化
    D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &m_D2DFactory);
    DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(IDWriteFactory), reinterpret_cast<IUnknown**>(&m_DWriteFactory));

    // レンダーターゲットビュー
    {
        ID3D11Texture2D* backBuffer = nullptr;
        m_SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&backBuffer));
        m_Device->CreateRenderTargetView(backBuffer, nullptr, &m_RenderTargetView);
        backBuffer->Release();
    }

    // デプスステンシルバッファ
    {
        D3D11_TEXTURE2D_DESC dsDesc = {};
        dsDesc.Width              = SCREEN_WIDTH;
        dsDesc.Height             = SCREEN_HEIGHT;
        dsDesc.MipLevels          = 1;
        dsDesc.ArraySize          = 1;
        dsDesc.Format             = DXGI_FORMAT_D24_UNORM_S8_UINT;
        dsDesc.SampleDesc.Count   = 1;
        dsDesc.SampleDesc.Quality = 0;
        dsDesc.Usage              = D3D11_USAGE_DEFAULT;
        dsDesc.BindFlags          = D3D11_BIND_DEPTH_STENCIL;

        ID3D11Texture2D* dsTex = nullptr;
        m_Device->CreateTexture2D(&dsDesc, nullptr, &dsTex);

        D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc = {};
        dsvDesc.Format        = dsDesc.Format;
        dsvDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
        m_Device->CreateDepthStencilView(dsTex, &dsvDesc, &m_DepthStencilView);
        dsTex->Release();
    }

    m_DeviceContext->OMSetRenderTargets(1, &m_RenderTargetView, m_DepthStencilView);

    // ビューポート
    {
        D3D11_VIEWPORT vp = {};
        vp.Width    = static_cast<float>(SCREEN_WIDTH);
        vp.Height   = static_cast<float>(SCREEN_HEIGHT);
        vp.MinDepth = 0.0f;
        vp.MaxDepth = 1.0f;
        m_DeviceContext->RSSetViewports(1, &vp);
    }

    // ラスタライザ (カリングなし, 2Dでは表裏関係ない)
    {
        D3D11_RASTERIZER_DESC rsDesc = {};
        rsDesc.FillMode        = D3D11_FILL_SOLID;
        rsDesc.CullMode        = D3D11_CULL_NONE;
        rsDesc.DepthClipEnable = TRUE;
        m_Device->CreateRasterizerState(&rsDesc, &m_RasterCullNone);
        m_DeviceContext->RSSetState(m_RasterCullNone);
    }

    // ブレンドステート
    {
        auto CreateBS = [&](D3D11_BLEND src, D3D11_BLEND dst, ID3D11BlendState** ppBS)
        {
            D3D11_BLEND_DESC bd = {};
            bd.RenderTarget[0].BlendEnable           = TRUE;
            bd.RenderTarget[0].SrcBlend              = src;
            bd.RenderTarget[0].DestBlend             = dst;
            bd.RenderTarget[0].BlendOp               = D3D11_BLEND_OP_ADD;
            bd.RenderTarget[0].SrcBlendAlpha         = D3D11_BLEND_ONE;
            bd.RenderTarget[0].DestBlendAlpha        = D3D11_BLEND_ZERO;
            bd.RenderTarget[0].BlendOpAlpha          = D3D11_BLEND_OP_ADD;
            bd.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
            m_Device->CreateBlendState(&bd, ppBS);
        };

        CreateBS(D3D11_BLEND_SRC_ALPHA,    D3D11_BLEND_INV_SRC_ALPHA, &m_BlendNormal);
        CreateBS(D3D11_BLEND_SRC_ALPHA,    D3D11_BLEND_ONE,           &m_BlendAdd);
        CreateBS(D3D11_BLEND_DEST_COLOR,   D3D11_BLEND_ZERO,          &m_BlendMultiply);

        // マスク: 色書き込みなし
        D3D11_BLEND_DESC bdMask = {};
        bdMask.RenderTarget[0].BlendEnable = FALSE;
        bdMask.RenderTarget[0].RenderTargetWriteMask = 0;
        m_Device->CreateBlendState(&bdMask, &m_BlendMask);

        float bf[4] = {};
        m_DeviceContext->OMSetBlendState(m_BlendNormal, bf, 0xffffffff);
    }

    // デプスステンシルステート
    {
        D3D11_DEPTH_STENCIL_DESC dsDesc = {};
        dsDesc.DepthEnable    = TRUE;
        dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
        dsDesc.DepthFunc      = D3D11_COMPARISON_LESS_EQUAL;
        m_Device->CreateDepthStencilState(&dsDesc, &m_DepthEnabled);

        dsDesc.DepthEnable    = FALSE;
        dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
        m_Device->CreateDepthStencilState(&dsDesc, &m_DepthDisabled);

        m_DeviceContext->OMSetDepthStencilState(m_DepthDisabled, 0); // 2Dはデプスなし
    }

    // サンプラーステート
    {
        auto CreateSS = [&](D3D11_FILTER filter, ID3D11SamplerState** ppSS)
        {
            D3D11_SAMPLER_DESC sd = {};
            sd.Filter         = filter;
            sd.AddressU       = D3D11_TEXTURE_ADDRESS_WRAP;
            sd.AddressV       = D3D11_TEXTURE_ADDRESS_WRAP;
            sd.AddressW       = D3D11_TEXTURE_ADDRESS_WRAP;
            sd.MaxAnisotropy  = 4;
            sd.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
            sd.MaxLOD         = D3D11_FLOAT32_MAX;
            m_Device->CreateSamplerState(&sd, ppSS);
        };

        CreateSS(D3D11_FILTER_MIN_MAG_MIP_LINEAR, &m_SamplerLinear);
        CreateSS(D3D11_FILTER_MIN_MAG_MIP_POINT,  &m_SamplerPoint);

        m_DeviceContext->PSSetSamplers(0, 1, &m_SamplerLinear);
        m_DeviceContext->PSSetSamplers(1, 1, &m_SamplerPoint);
    }

    // 定数バッファ作成 & レジスタ設定
    {
        m_WorldBuffer      = CreateConstantBuffer(m_Device, sizeof(CB_World));
        m_ViewBuffer       = CreateConstantBuffer(m_Device, sizeof(CB_View));
        m_ProjectionBuffer = CreateConstantBuffer(m_Device, sizeof(CB_Projection));
        m_MaterialBuffer   = CreateConstantBuffer(m_Device, sizeof(CB_Material));
        m_TimeBuffer       = CreateConstantBuffer(m_Device, sizeof(CB_Time));
        m_ParameterBuffer  = CreateConstantBuffer(m_Device, sizeof(CB_Parameter));

        // b0〜b5 を VS/PS 両方にバインド
        ID3D11Buffer* cbs[] = {
            m_WorldBuffer, m_ViewBuffer, m_ProjectionBuffer,
            m_MaterialBuffer, m_TimeBuffer, m_ParameterBuffer
        };
        m_DeviceContext->VSSetConstantBuffers(0, 6, cbs);
        m_DeviceContext->PSSetConstantBuffers(0, 6, cbs);
    }

    // デフォルトで2D投影をセット
    SetWorldViewProjection2D();
}

//=============================================================================
// Uninit
//=============================================================================
void Renderer::Uninit()
{
    if (m_DWriteFactory) { m_DWriteFactory->Release(); m_DWriteFactory = nullptr; }
    if (m_D2DFactory) { m_D2DFactory->Release(); m_D2DFactory = nullptr; }
    auto SafeRelease = [](auto*& p) { if (p) { p->Release(); p = nullptr; } };

    SafeRelease(m_ParameterBuffer);
    SafeRelease(m_TimeBuffer);
    SafeRelease(m_MaterialBuffer);
    SafeRelease(m_ProjectionBuffer);
    SafeRelease(m_ViewBuffer);
    SafeRelease(m_WorldBuffer);

    SafeRelease(m_BlendNormal);
    SafeRelease(m_BlendAdd);
    SafeRelease(m_BlendMultiply);
    SafeRelease(m_BlendMask);

    SafeRelease(m_DepthEnabled);
    SafeRelease(m_DepthDisabled);
    SafeRelease(m_RasterCullNone);

    SafeRelease(m_SamplerLinear);
    SafeRelease(m_SamplerPoint);

    SafeRelease(m_DepthStencilView);
    SafeRelease(m_RenderTargetView);
    SafeRelease(m_SwapChain);
    SafeRelease(m_DeviceContext);
    SafeRelease(m_Device);
}

//=============================================================================
// Begin / End
//=============================================================================
void Renderer::Begin()
{
    const float clearColor[4] = { 0.1f, 0.1f, 0.15f, 1.0f };
    m_DeviceContext->ClearRenderTargetView(m_RenderTargetView, clearColor);
    m_DeviceContext->ClearDepthStencilView(m_DepthStencilView,
        D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
    m_DeviceContext->OMSetRenderTargets(1, &m_RenderTargetView, m_DepthStencilView);
}

void Renderer::End()
{
    m_SwapChain->Present(1, 0);  // VSync ON
}

//=============================================================================
// OnWindowResize: ウィンドウサイズ変化後に呼ぶ
// レターボックス方式で常に 16:9 を維持する
//=============================================================================
void Renderer::OnWindowResize(UINT clientW, UINT clientH)
{
    if (!m_SwapChain || clientW == 0 || clientH == 0) return;

    // RTV / DSV を一旦解放しないと ResizeBuffers が失敗する
    m_DeviceContext->OMSetRenderTargets(0, nullptr, nullptr);
    if (m_RenderTargetView) { m_RenderTargetView->Release(); m_RenderTargetView = nullptr; }
    if (m_DepthStencilView) { m_DepthStencilView->Release(); m_DepthStencilView = nullptr; }

    // スワップチェーンのバッファをリサイズ (SCREEN_WIDTH x SCREEN_HEIGHT 固定
    m_SwapChain->ResizeBuffers(1, SCREEN_WIDTH, SCREEN_HEIGHT,
        DXGI_FORMAT_R8G8B8A8_UNORM, 0);

    // RTV 再作成
    {
        ID3D11Texture2D* backBuffer = nullptr;
        m_SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D),
            reinterpret_cast<void**>(&backBuffer));
        m_Device->CreateRenderTargetView(backBuffer, nullptr, &m_RenderTargetView);
        backBuffer->Release();
    }

    // DSV 再作成
    {
        D3D11_TEXTURE2D_DESC dsDesc = {};
        dsDesc.Width              = SCREEN_WIDTH;
        dsDesc.Height             = SCREEN_HEIGHT;
        dsDesc.MipLevels          = 1;
        dsDesc.ArraySize          = 1;
        dsDesc.Format             = DXGI_FORMAT_D24_UNORM_S8_UINT;
        dsDesc.SampleDesc.Count   = 1;
        dsDesc.SampleDesc.Quality = 0;
        dsDesc.Usage              = D3D11_USAGE_DEFAULT;
        dsDesc.BindFlags          = D3D11_BIND_DEPTH_STENCIL;

        ID3D11Texture2D* dsTex = nullptr;
        m_Device->CreateTexture2D(&dsDesc, nullptr, &dsTex);

        D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc = {};
        dsvDesc.Format        = dsDesc.Format;
        dsvDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
        m_Device->CreateDepthStencilView(dsTex, &dsvDesc, &m_DepthStencilView);
        dsTex->Release();
    }

    m_DeviceContext->OMSetRenderTargets(1, &m_RenderTargetView, m_DepthStencilView);

    // レターボックス: クライアント領域内で SCREEN_WIDTH:SCREEN_HEIGHT を保った最大矩形
    float scaleX = static_cast<float>(clientW) / SCREEN_WIDTH;
    float scaleY = static_cast<float>(clientH) / SCREEN_HEIGHT;
    float scale  = (scaleX < scaleY) ? scaleX : scaleY;   // 小さい方に合わせる

    UINT vpW = static_cast<UINT>(SCREEN_WIDTH  * scale);
    UINT vpH = static_cast<UINT>(SCREEN_HEIGHT * scale);
    UINT vpX = (clientW - vpW) / 2;
    UINT vpY = (clientH - vpH) / 2;

    D3D11_VIEWPORT vp = {};
    vp.TopLeftX = static_cast<float>(vpX);
    vp.TopLeftY = static_cast<float>(vpY);
    vp.Width    = static_cast<float>(vpW);
    vp.Height   = static_cast<float>(vpH);
    vp.MinDepth = 0.0f;
    vp.MaxDepth = 1.0f;
    m_DeviceContext->RSSetViewports(1, &vp);
}

//=============================================================================
// 定数バッファ更新
//=============================================================================
void Renderer::SetWorldMatrix(const DirectX::XMMATRIX& m)
{
    CB_World cb;
    DirectX::XMStoreFloat4x4(&cb.World, DirectX::XMMatrixTranspose(m));
    m_DeviceContext->UpdateSubresource(m_WorldBuffer, 0, nullptr, &cb, 0, 0);
}

void Renderer::SetViewMatrix(const DirectX::XMMATRIX& m)
{
    CB_View cb;
    DirectX::XMStoreFloat4x4(&cb.View, DirectX::XMMatrixTranspose(m));
    m_DeviceContext->UpdateSubresource(m_ViewBuffer, 0, nullptr, &cb, 0, 0);
}

void Renderer::SetProjectionMatrix(const DirectX::XMMATRIX& m)
{
    CB_Projection cb;
    DirectX::XMStoreFloat4x4(&cb.Projection, DirectX::XMMatrixTranspose(m));
    m_DeviceContext->UpdateSubresource(m_ProjectionBuffer, 0, nullptr, &cb, 0, 0);
}

void Renderer::SetMaterialColor(const DirectX::XMFLOAT4& color, bool texEnable)
{
    CB_Material cb;
    cb.Color         = color;
    cb.TextureEnable = texEnable ? TRUE : FALSE;
    cb.Dummy[0] = cb.Dummy[1] = cb.Dummy[2] = 0.0f;
    m_DeviceContext->UpdateSubresource(m_MaterialBuffer, 0, nullptr, &cb, 0, 0);
}

void Renderer::SetParameter(const DirectX::XMFLOAT4& param)
{
    CB_Parameter cb;
    cb.Parameter = param;
    m_DeviceContext->UpdateSubresource(m_ParameterBuffer, 0, nullptr, &cb, 0, 0);
}

void Renderer::UpdateTime(float dt)
{
    m_TotalTime += dt;
    CB_Time cb;
    cb.Time      = m_TotalTime;
    cb.DeltaTime = dt;
    cb.Dummy[0] = cb.Dummy[1] = 0.0f;
    m_DeviceContext->UpdateSubresource(m_TimeBuffer, 0, nullptr, &cb, 0, 0);
}

//=============================================================================
// 2D投影設定 画面座標 -> NDC
// 左上(0,0) 右下(SCREEN_WIDTH, SCREEN_HEIGHT)
//=============================================================================
void Renderer::SetWorldViewProjection2D()
{
    // World = 単位行列
    SetWorldMatrix(DirectX::XMMatrixIdentity());

    // View = 単位行列
    SetViewMatrix(DirectX::XMMatrixIdentity());

    // Projection: 画面座標 -> [-1,1] に変換 (Y軸反転あり)
    DirectX::XMMATRIX proj = DirectX::XMMatrixOrthographicOffCenterLH(
        0.0f,                            // left
        static_cast<float>(SCREEN_WIDTH), // right
        static_cast<float>(SCREEN_HEIGHT),// bottom
        0.0f,                            // top (Y軸反転)
        -1.0f, 1.0f);                    // near, far

    SetProjectionMatrix(proj);
}

//=============================================================================
// レンダリングステート
//=============================================================================
void Renderer::SetBlendMode(BlendMode mode)
{
    float bf[4] = {};
    switch (mode)
    {
    case BlendMode::NORMAL:   m_DeviceContext->OMSetBlendState(m_BlendNormal,   bf, 0xffffffff); break;
    case BlendMode::ADD:      m_DeviceContext->OMSetBlendState(m_BlendAdd,      bf, 0xffffffff); break;
    case BlendMode::MULTIPLY: m_DeviceContext->OMSetBlendState(m_BlendMultiply, bf, 0xffffffff); break;
    case BlendMode::MASK:     m_DeviceContext->OMSetBlendState(m_BlendMask,     bf, 0xffffffff); break;
    }
}

void Renderer::SetDepthEnable(bool enable)
{
    m_DeviceContext->OMSetDepthStencilState(
        enable ? m_DepthEnabled : m_DepthDisabled, 0);
}

void Renderer::SetSamplerState(bool isPixelArt)
{
    ID3D11SamplerState* sampler = isPixelArt ? m_SamplerPoint : m_SamplerLinear;
    m_DeviceContext->PSSetSamplers(0, 1, &sampler);
}

//=============================================================================
// シェーダーロード (CSO バイナリ)
//=============================================================================
static std::vector<char> LoadCSO(const char* path)
{
    std::ifstream ifs(path, std::ios::binary | std::ios::ate);
    if (!ifs) return {};
    std::streamsize size = ifs.tellg();
    ifs.seekg(0, std::ios::beg);
    std::vector<char> buf(static_cast<size_t>(size));
    ifs.read(buf.data(), size);
    return buf;
}

void Renderer::CreateVertexShader(
    ID3D11VertexShader** ppVS,
    ID3D11InputLayout**  ppLayout,
    const char* filePath)
{
    auto code = LoadCSO(filePath);
    if (code.empty()) return;

    m_Device->CreateVertexShader(code.data(), code.size(), nullptr, ppVS);

    // 入力レイアウト VERTEX_2D に対応
    D3D11_INPUT_ELEMENT_DESC layout[] = {
        { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,    0, 0,                            D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "COLOR",    0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,       0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
    };
    m_Device->CreateInputLayout(layout, 3, code.data(), code.size(), ppLayout);
}

void Renderer::CreatePixelShader(ID3D11PixelShader** ppPS, const char* filePath)
{
    auto code = LoadCSO(filePath);
    if (code.empty()) return;
    m_Device->CreatePixelShader(code.data(), code.size(), nullptr, ppPS);
}
