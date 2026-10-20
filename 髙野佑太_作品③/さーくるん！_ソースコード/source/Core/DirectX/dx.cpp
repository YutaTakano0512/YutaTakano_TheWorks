
//---------------------------------------------------------------------------
// dx.cpp
// 
// sougo hara  2024/05/14
//---------------------------------------------------------------------------

#include "../../Game/Configuration/configuration.h"
#include "../../main.h"

#include "dx.h"

#include "../../game_main.h"

//
// ライブラリのリンク
//
#pragma comment (lib, "d3d11.lib")
#pragma comment (lib, "xinput.lib")//---★追加---
#pragma comment(lib, "dxgi.lib")

//テクスチャ
#if _DEBUG

#include <dxgidebug.h>

#pragma comment(lib, "DirectXTex_Debug.lib")
#else
#pragma comment(lib, "DirectXTex_Release.lib")
#endif


//
// マクロ定義
//

//
// グローバル変数
//

static D3D_FEATURE_LEVEL g_FeatureLevel = D3D_FEATURE_LEVEL_11_0;

static ID3D11Device* g_Device = NULL; //マネージャー的な役割
static ID3D11DeviceContext* g_DeviceContext = NULL; //絵をかいたりすることを司る
static IDXGISwapChain* g_SwapChain[DX_WINDOW_NUM] = {}; //画面の切り替えを司る

//static IDXGIDevice1* g_DXGI      = NULL;
static IDXGIAdapter* g_Adapter = NULL;
static IDXGIFactory* g_Factory = NULL;

static ID3D11Texture2D* g_DepthStencilTexture = NULL;
static ID3D11RenderTargetView* g_RenderTargetView[DX_WINDOW_NUM] = {};
static ID3D11DepthStencilView* g_DepthStencilView = NULL;

static ID3D11RasterizerState* g_RasterizerState = NULL;

static ID3D11BlendState* g_BlendState = NULL;
static ID3D11BlendState* g_BlendState_AddColor = NULL;


static ID3D11DepthStencilState* g_DepthStencilStateDepthEnable = NULL;
static ID3D11DepthStencilState* g_DepthStencilStateDepthDisable = NULL;
static bool g_IsDepthStencilStateDepthEnable = false;


//ディスプレイのバッグバッファのテクスチャ
static ID3D11Texture2D* _pTexture = nullptr;

// オフスク用 RTV
static ID3D11RenderTargetView* _pRenderingTextureRTV = nullptr;
// オフスク用 SRV
static ID3D11ShaderResourceView* _pRenderingTextureSRV = nullptr;

static ID3D11Texture2D* g_DepthStencilTexture_OffScreen = NULL;
static ID3D11DepthStencilView* g_DepthStencilView_OffScreen = NULL;

//ディスプレイのバッグバッファのテクスチャ
static ID3D11Texture2D*				g_Texture_BloomBuf = nullptr;
// オフスク用 RTV
static ID3D11RenderTargetView*		g_RenderingTextureRTV_BloomBuf = nullptr;
// オフスク用 SRV
static ID3D11ShaderResourceView*	g_RenderingTextureSRV_BloomBuf = nullptr;


//ディスプレイのバッグバッファのテクスチャ
static ID3D11Texture2D* g_Texture_OutlineBuf[DX_OUTLINE_BUF_NUM] = {};
// オフスク用 RTV
static ID3D11RenderTargetView* g_RenderingTextureRTV_OutlineBuf[DX_OUTLINE_BUF_NUM] = {};
// オフスク用 SRV
static ID3D11ShaderResourceView* g_RenderingTextureSRV_OutlineBuf[DX_OUTLINE_BUF_NUM] = {};

//ディスプレイのバッグバッファのテクスチャ
static ID3D11Texture2D* g_Texture_MainDummyBuf = {};
// オフスク用 RTV
static ID3D11RenderTargetView* g_RenderingTextureRTV_MainDummyBuf = {};
// オフスク用 SRV
static ID3D11ShaderResourceView* g_RenderingTextureSRV_MainDummyBuf = {};

static XMFLOAT2 g_viewPortSizeCache = {};
static XMFLOAT2 g_WindowSizeCache = {};

static HWND* Save_hWnd = NULL;


//
// DirectX 初期化
//
void DXInitialize(DX_INIT_SET set, DX_INIT_RESULT* result)
{
	Save_hWnd = set.hWnd;

    DXGI_SWAP_CHAIN_DESC sd = {};
    //ZeroMemory(&sd, sizeof(sd));
    sd.BufferCount = 1;
    sd.BufferDesc.Width = SCREEN_WIDTH;
    sd.BufferDesc.Height = SCREEN_HEIGHT;
    sd.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
    sd.BufferDesc.RefreshRate.Numerator = 60;
    sd.BufferDesc.RefreshRate.Denominator = 1;
    sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    sd.SampleDesc.Count = 1;
    sd.SampleDesc.Quality = 0;
    sd.Windowed = true;

//    //デバイス　スワップチェーン　コンテキスト設定
//    HRESULT hr = D3D11CreateDeviceAndSwapChain(NULL,
//        D3D_DRIVER_TYPE_HARDWARE,
//        NULL,
//#if defined(DEBUG) || defined(_DEBUG)
//        //D3D10_CREATE_DEVICE_DEBUG |
//        D3D11_CREATE_DEVICE_BGRA_SUPPORT, //Direct2Dに対応する 元0
//#else
//        D3D11_CREATE_DEVICE_BGRA_SUPPORT, //Direct2Dに対応する 元0
//#endif
//        NULL,
//        0,
//        D3D11_SDK_VERSION,
//        &sd,
//        &g_SwapChain,
//        &g_Device,
//        &g_FeatureLevel,
//        &g_DeviceContext);
//    if (FAILED(hr))
//    {
//        //エラー
//        MessageBox(Save_hWnd[0], "D3D11CreateDeviceAndSwapChain の失敗", "kakunin", MB_OK | MB_ICONHAND);
//
//        return;
//    }
// 
// 
	// GPU情報取得
    IDXGIFactory* factory = nullptr;
    HRESULT hr = CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&factory);
    if (FAILED(hr)) return;

    // アダプター列挙
    std::vector<IDXGIAdapter*> adapters;
    for (UINT i = 0; factory->EnumAdapters(i, &g_Adapter) != DXGI_ERROR_NOT_FOUND; ++i)
    {
        adapters.emplace_back(g_Adapter);
    }
    factory->Release();

    // VRAM容量が最大のGPU選択
    SIZE_T maxMemory = 0;
    for (auto ad : adapters)
    {
        DXGI_ADAPTER_DESC desc;
        ad->GetDesc(&desc);
        if (desc.DedicatedVideoMemory > maxMemory)
        {
            maxMemory = desc.DedicatedVideoMemory;
            g_Adapter = ad;
        }
    }

    //// GPU名保存
    //{
    //    DXGI_ADAPTER_DESC desc;
    //    adapter->GetDesc(&desc);
    //    gpu_name_ = ToString(desc.Description);
    //}

    // 選ばれなかったアダプタは解放
    for (auto ad : adapters)
    {
        if (ad != g_Adapter)
            ad->Release();
    }

    if (!g_Adapter) return;

    hr = D3D11CreateDevice(g_Adapter, D3D_DRIVER_TYPE_UNKNOWN,
        NULL, D3D11_CREATE_DEVICE_BGRA_SUPPORT, NULL, 0, D3D11_SDK_VERSION,
        &g_Device, &g_FeatureLevel, &g_DeviceContext);
    if (FAILED(hr))
    {
        //エラー
        MessageBox(NULL, "D3D11CreateDevice の失敗", "kakunin", MB_OK | MB_ICONHAND);

        return;
    }
    //g_Device->QueryInterface(__uuidof(IDXGIDevice1), (void**)&g_DXGI);
    //g_DXGI->GetAdapter(&g_Adapter);
    g_Adapter->GetParent(__uuidof(IDXGIFactory), (void**)&g_Factory);

    for (int i = 0; i < DX_WINDOW_NUM; i++)
    {
        sd.OutputWindow = Save_hWnd[i];

        g_Factory->CreateSwapChain(g_Device, &sd, &g_SwapChain[i]);
        if (FAILED(hr))
        {
            //エラー
            MessageBox(Save_hWnd[i], "D3D11CreateDeviceAndSwapChain の失敗", "kakunin", MB_OK | MB_ICONHAND);

            return;
        }

    }

    //-----------------------------
    // メインダミー
    //-----------------------------
    {
        {
            // テクスチャ設定 オフスク用
            D3D11_TEXTURE2D_DESC rtDesc;
            memset(&rtDesc, 0, sizeof(rtDesc));
            rtDesc.Width = SCREEN_WIDTH;
            rtDesc.Height = SCREEN_HEIGHT;
            rtDesc.MipLevels = 1;
            rtDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
            //rtDesc.Format = DXGI_FORMAT_R16G16B16A16_FLOAT; // 高精度フォーマット

            rtDesc.SampleDesc.Count = 1;
            rtDesc.Usage = D3D11_USAGE_DEFAULT;
            rtDesc.ArraySize = 1;
            rtDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
            rtDesc.CPUAccessFlags = 0;

            g_Device->CreateTexture2D(&rtDesc, 0, &g_Texture_MainDummyBuf);


            //	SRV設定 オフスク用
            D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
            ZeroMemory(&srvDesc, sizeof(srvDesc));
            srvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
            srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
            srvDesc.Texture2D.MostDetailedMip = 0;
            srvDesc.Texture2D.MipLevels = rtDesc.MipLevels;
            if (g_Texture_MainDummyBuf != NULL) //警告回避
            {
                g_Device->CreateShaderResourceView(g_Texture_MainDummyBuf, &srvDesc, &g_RenderingTextureSRV_MainDummyBuf);
            }

            if (g_Texture_MainDummyBuf != NULL) //警告回避
            {
                g_Device->CreateRenderTargetView(g_Texture_MainDummyBuf, NULL, &g_RenderingTextureRTV_MainDummyBuf);
            }
        }
    }
	//-----------------------------
	// アウトライン
	//-----------------------------
	{
        for (int i = 0; i < DX_OUTLINE_BUF_NUM; i++)
        {
            // テクスチャ設定 オフスク用
            D3D11_TEXTURE2D_DESC rtDesc;
            memset(&rtDesc, 0, sizeof(rtDesc));
            rtDesc.Width = SCREEN_WIDTH;
            rtDesc.Height = SCREEN_HEIGHT;
            rtDesc.MipLevels = 1;
            rtDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
            rtDesc.SampleDesc.Count = 1;
            rtDesc.Usage = D3D11_USAGE_DEFAULT;
            rtDesc.ArraySize = 1;
            rtDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
            rtDesc.CPUAccessFlags = 0;

            g_Device->CreateTexture2D(&rtDesc, 0, &g_Texture_OutlineBuf[i]);


            //	SRV設定 オフスク用
            D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
            ZeroMemory(&srvDesc, sizeof(srvDesc));
            srvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
            srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
            srvDesc.Texture2D.MostDetailedMip = 0;
            srvDesc.Texture2D.MipLevels = rtDesc.MipLevels;
            if (g_Texture_OutlineBuf[i] != NULL) //警告回避
            {
                g_Device->CreateShaderResourceView(g_Texture_OutlineBuf[i], &srvDesc, &g_RenderingTextureSRV_OutlineBuf[i]);
            }

            if (g_Texture_OutlineBuf[i] != NULL) //警告回避
            {
                g_Device->CreateRenderTargetView(g_Texture_OutlineBuf[i], NULL, &g_RenderingTextureRTV_OutlineBuf[i]);
            }
        }
	}
	//-----------------------------
	// ブルーム
	//-----------------------------
	{
		// テクスチャ設定 オフスク用
		D3D11_TEXTURE2D_DESC rtDesc;
		memset(&rtDesc, 0, sizeof(rtDesc));
		rtDesc.Width = SCREEN_WIDTH;
		rtDesc.Height = SCREEN_HEIGHT;
		rtDesc.MipLevels = 1;
		rtDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		rtDesc.SampleDesc.Count = 1;
		rtDesc.Usage = D3D11_USAGE_DEFAULT;
		rtDesc.ArraySize = 1;
		rtDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
		rtDesc.CPUAccessFlags = 0;

		g_Device->CreateTexture2D(&rtDesc, 0, &g_Texture_BloomBuf);


		//	SRV設定 オフスク用
		D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
		ZeroMemory(&srvDesc, sizeof(srvDesc));
		srvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		srvDesc.Texture2D.MostDetailedMip = 0;
		srvDesc.Texture2D.MipLevels = rtDesc.MipLevels;
		if (g_Texture_BloomBuf != NULL) //警告回避
		{
			g_Device->CreateShaderResourceView(g_Texture_BloomBuf, &srvDesc, &g_RenderingTextureSRV_BloomBuf);
		}

		if (g_Texture_BloomBuf != NULL) //警告回避
		{
			g_Device->CreateRenderTargetView(g_Texture_BloomBuf, NULL, &g_RenderingTextureRTV_BloomBuf);
		}
	}
	//-----------------------------
	// オフスク
	//-----------------------------
	{
		// テクスチャ設定 オフスク用
		D3D11_TEXTURE2D_DESC rtDesc;
		memset(&rtDesc, 0, sizeof(rtDesc));
		rtDesc.Width = SCREEN_WIDTH;
		rtDesc.Height = SCREEN_HEIGHT;
		rtDesc.MipLevels = 1;
		rtDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		rtDesc.SampleDesc.Count = 1;
		rtDesc.Usage = D3D11_USAGE_DEFAULT;
		rtDesc.ArraySize = 1;
		rtDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
		rtDesc.CPUAccessFlags = 0;

		g_Device->CreateTexture2D(&rtDesc, 0, &_pTexture);


		//	SRV設定 オフスク用
		D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
		ZeroMemory(&srvDesc, sizeof(srvDesc));
		srvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		srvDesc.Texture2D.MostDetailedMip = 0;
		srvDesc.Texture2D.MipLevels = rtDesc.MipLevels;
		if (_pTexture != NULL) //警告回避
		{
			g_Device->CreateShaderResourceView(_pTexture, &srvDesc, &_pRenderingTextureSRV);
		}

		// RTV 作成 オフスク用

		//ID3D11Texture2D* p_back_buffer = NULL;
		//g_SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&p_back_buffer);
		//if (p_back_buffer != NULL) //警告回避
		//{
		//	g_Device->CreateRenderTargetView(p_back_buffer, NULL, &_pRenderingTextureRTV);
		//	p_back_buffer->Release();
		//}

		if (_pTexture != NULL) //警告回避
		{
			g_Device->CreateRenderTargetView(_pTexture, NULL, &_pRenderingTextureRTV);
		}
	}

    for (int i = 0; i < DX_WINDOW_NUM; i++)
    {
        // レンダーターゲットビュー生成
        ID3D11Texture2D* p_back_buffer = NULL;
        g_SwapChain[i]->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&p_back_buffer);
        if (p_back_buffer != NULL) //警告回避
        {
            g_Device->CreateRenderTargetView(p_back_buffer, NULL, &g_RenderTargetView[i]);
            p_back_buffer->Release();
        }
    }

	// デプスステンシル使用テクスチャー作成
	{
		D3D11_TEXTURE2D_DESC td = {};
		td.Width = sd.BufferDesc.Width;
		td.Height = sd.BufferDesc.Height;
		td.MipLevels = 1;
		td.ArraySize = 1;
		td.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		td.SampleDesc = sd.SampleDesc;
		td.Usage = D3D11_USAGE_DEFAULT;
		td.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		td.CPUAccessFlags = 0;
		td.MiscFlags = 0;
		g_Device->CreateTexture2D(&td, NULL, &g_DepthStencilTexture);
		// デプスステンシルターゲット作成
		D3D11_DEPTH_STENCIL_VIEW_DESC dsvd = {};
		dsvd.Format = td.Format;
		dsvd.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		dsvd.Flags = 0;
		if (g_DepthStencilTexture != NULL) //警告回避
		{
			g_Device->CreateDepthStencilView(g_DepthStencilTexture, &dsvd,
				&g_DepthStencilView);
		}
	}
	{
		D3D11_TEXTURE2D_DESC td = {};
		td.Width = sd.BufferDesc.Width;
		td.Height = sd.BufferDesc.Height;
		td.MipLevels = 1;
		td.ArraySize = 1;
		td.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		td.SampleDesc = sd.SampleDesc;
		td.Usage = D3D11_USAGE_DEFAULT;
		td.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		td.CPUAccessFlags = 0;
		td.MiscFlags = 0;
		g_Device->CreateTexture2D(&td, NULL, &g_DepthStencilTexture_OffScreen);
		// デプスステンシルターゲット作成
		D3D11_DEPTH_STENCIL_VIEW_DESC dsvd = {};
		dsvd.Format = td.Format;
		dsvd.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		dsvd.Flags = 0;
		if (g_DepthStencilTexture_OffScreen != NULL) //警告回避
		{
			g_Device->CreateDepthStencilView(g_DepthStencilTexture_OffScreen, &dsvd,
				&g_DepthStencilView_OffScreen);
		}
	}


	//ビューポートの設定
	//D3D11_VIEWPORT vp;
	//vp.Width = (FLOAT)SCREEN_WIDTH;
	//vp.Height = (FLOAT)SCREEN_HEIGHT;
	//vp.MinDepth = 0.0f;
	//vp.MaxDepth = 1.0f;
	//vp.TopLeftX = 0;
	//vp.TopLeftY = 0;

	//g_DeviceContext->RSSetViewports(1, &vp);
    DXUpdateViewport(SCREEN_WIDTH, SCREEN_HEIGHT);
    //DXSetViewport(2);

	//ラスタライザステート
	D3D11_RASTERIZER_DESC rd = {};
	rd.FillMode = D3D11_FILL_SOLID;
	//rd.CullMode = D3D11_CULL_BACK;
	rd.CullMode = D3D11_CULL_NONE; //ポリゴンの裏を描画する

	//rd.DepthClipEnable = true;
	rd.DepthClipEnable = false;

	rd.MultisampleEnable = false;
	//rd.MultisampleEnable = true;

    rd.DepthBias = 1000;            // 深度バイアス
    rd.SlopeScaledDepthBias = 1.0f; // スロープスケールバイアス
    rd.DepthBiasClamp = 0.0f;       // バイアスのクランプ値

	g_Device->CreateRasterizerState(&rd, &g_RasterizerState);

	//デバイスコンテキストにラスタライザーステートを設定
	g_DeviceContext->RSSetState(g_RasterizerState);

	//ブレンドステート設定
	D3D11_BLEND_DESC bd = {};
	bd.AlphaToCoverageEnable = false;
	//bd.AlphaToCoverageEnable = true;
	bd.IndependentBlendEnable = false;
	{
		D3D11_RENDER_TARGET_BLEND_DESC* bdrt = &bd.RenderTarget[0];
		bdrt->BlendEnable = true;
		bdrt->SrcBlend = D3D11_BLEND_SRC_ALPHA; //D3D11_BLEND_SRC_ALPHA
		bdrt->DestBlend = D3D11_BLEND_INV_SRC_ALPHA; //D3D11_BLEND_INV_SRC_ALPHA
		bdrt->BlendOp = D3D11_BLEND_OP_ADD;
		bdrt->SrcBlendAlpha = D3D11_BLEND_ONE;
		bdrt->DestBlendAlpha = D3D11_BLEND_ZERO;
		bdrt->BlendOpAlpha = D3D11_BLEND_OP_ADD;
		bdrt->RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
	}
	//{
	//	D3D11_RENDER_TARGET_BLEND_DESC* bdrt = &bd.RenderTarget[1];
	//	bdrt->BlendEnable = true;
	//	bdrt->SrcBlend = D3D11_BLEND_SRC_ALPHA; //D3D11_BLEND_SRC_ALPHA
	//	bdrt->DestBlend = D3D11_BLEND_INV_SRC_ALPHA; //D3D11_BLEND_INV_SRC_ALPHA
	//	bdrt->BlendOp = D3D11_BLEND_OP_ADD;
	//	bdrt->SrcBlendAlpha = D3D11_BLEND_ONE;
	//	bdrt->DestBlendAlpha = D3D11_BLEND_ZERO;
	//	bdrt->BlendOpAlpha = D3D11_BLEND_OP_ADD;
	//	bdrt->RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
	//}

	D3D11_BLEND_DESC bdAddColor = bd;
	{
		D3D11_RENDER_TARGET_BLEND_DESC* bdrt = &bdAddColor.RenderTarget[0];
        bdrt->BlendEnable = true;
        bdrt->SrcBlend = D3D11_BLEND_SRC_ALPHA;
        bdrt->DestBlend = D3D11_BLEND_ONE;
        bdrt->BlendOp = D3D11_BLEND_OP_ADD;
        bdrt->SrcBlendAlpha = D3D11_BLEND_ONE;
        bdrt->DestBlendAlpha = D3D11_BLEND_ZERO;
        bdrt->BlendOpAlpha = D3D11_BLEND_OP_ADD;
        bdrt->RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
		//bdrt->SrcBlend = D3D11_BLEND_ONE;
		//bdrt->DestBlend = D3D11_BLEND_ONE;
		//bdrt->SrcBlendAlpha = D3D11_BLEND_ONE;
		//bdrt->DestBlendAlpha = D3D11_BLEND_ONE;
	}

	g_Device->CreateBlendState(&bd, &g_BlendState);
	g_Device->CreateBlendState(&bdAddColor, &g_BlendState_AddColor);


	//深度ステンシルステート設定
	D3D11_DEPTH_STENCIL_DESC dsd = {};

	//ステンシルは今は使わないので無効にしておく

	//深度有効ステート
	dsd.DepthEnable = true;
	dsd.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	dsd.DepthFunc = D3D11_COMPARISON_LESS;
	dsd.StencilEnable = false;

    //dsd.DepthEnable = true;  // 深度バッファを有効化
    //dsd.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;  // 深度値を書き込まない
    //dsd.DepthFunc = D3D11_COMPARISON_LESS;  // 通常の深度比較

	g_Device->CreateDepthStencilState(&dsd, &g_DepthStencilStateDepthEnable);

	//深度無効ステート
	dsd.DepthEnable = false;
	dsd.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;

	g_Device->CreateDepthStencilState(&dsd, &g_DepthStencilStateDepthDisable);

	//とりあえず深度無効にしておく
	//DXSetDepthStencilStateDepth(false);

	// とりあえず深度有効にしておく
	DXSetDepthStencilStateDepth(true);


#if _DEBUG

	//typedef HRESULT(__stdcall* fPtr)(const IID&, void**);
	//HMODULE hDll = GetModuleHandleW(L"dxgidebug.dll");
	//fPtr DXGIGetDebugInterface = (fPtr)GetProcAddress(hDll, "DXGIGetDebugInterface");
	//IDXGIDebug* pDxgiDebug;
	//DXGIGetDebugInterface(__uuidof(IDXGIDebug), (void**)&pDxgiDebug);
	//pDxgiDebug->ReportLiveObjects(DXGI_DEBUG_D3D11, DXGI_DEBUG_RLO_DETAIL);

#endif
	if (result != NULL)
	{
		//結果を渡す
		//このデータは 呼び出し元で別の初期化処理に利用する
		result->device = g_Device;
		result->deviceContext = g_DeviceContext;
	}
}

//
// DirectX 終了処理
//
void DXFinalize(void)
{
	//安全に終了するために開放する

    SAFE_RELEASE(g_Texture_MainDummyBuf);
    SAFE_RELEASE(g_RenderingTextureRTV_MainDummyBuf);
    SAFE_RELEASE(g_RenderingTextureSRV_MainDummyBuf);

	SAFE_RELEASE(g_Texture_BloomBuf);
	SAFE_RELEASE(g_RenderingTextureRTV_BloomBuf);
	SAFE_RELEASE(g_RenderingTextureSRV_BloomBuf);

    for (int i = 0; i < DX_OUTLINE_BUF_NUM; i++)
    {
        SAFE_RELEASE(g_Texture_OutlineBuf[i]);
        SAFE_RELEASE(g_RenderingTextureRTV_OutlineBuf[i]);
        SAFE_RELEASE(g_RenderingTextureSRV_OutlineBuf[i]);
    }

	SAFE_RELEASE(g_Texture_BloomBuf);
	SAFE_RELEASE(g_RenderingTextureRTV_BloomBuf);
	SAFE_RELEASE(g_RenderingTextureSRV_BloomBuf);

	SAFE_RELEASE(g_DepthStencilTexture_OffScreen);
	SAFE_RELEASE(g_DepthStencilView_OffScreen);

	SAFE_RELEASE(_pTexture);
	SAFE_RELEASE(_pRenderingTextureRTV);
	SAFE_RELEASE(_pRenderingTextureSRV);


	SAFE_RELEASE(g_DepthStencilStateDepthDisable);
	SAFE_RELEASE(g_DepthStencilStateDepthEnable);

	SAFE_RELEASE(g_BlendState);
	SAFE_RELEASE(g_BlendState_AddColor);
	

	SAFE_RELEASE(g_RasterizerState);

	SAFE_RELEASE(g_DepthStencilView);
	SAFE_RELEASE(g_DepthStencilTexture);
    for (int i = 0; i < DX_WINDOW_NUM; i++)
    {
        SAFE_RELEASE(g_RenderTargetView[i]);
        SAFE_RELEASE(g_SwapChain[i]);
    }

    SAFE_RELEASE(g_Factory);
    SAFE_RELEASE(g_Adapter);
    //SAFE_RELEASE(g_DXGI);


	SAFE_RELEASE(g_DeviceContext);
	SAFE_RELEASE(g_Device);
}

void DXUpdateViewport(int width, int height) 
{
    if (g_DeviceContext == nullptr)
    {
        return;
    }

    float aspectRatioWindow = (float)width / height; // ウィンドウアスペクト比
    float aspectRatioBase = (float)SCREEN_WIDTH / SCREEN_HEIGHT; // 固定アスペクト比
    //float newWidth = width;
    //float newHeight = newWidth / aspectRatio;

    //if (newHeight > height) {
    //    newHeight = height;
    //    newWidth = newHeight * aspectRatio;
    //}

    //D3D11_VIEWPORT viewport = {};
    //viewport.Width = newWidth;
    //viewport.Height = newHeight;
    //viewport.TopLeftX = (width - newWidth) / 2;
    //viewport.TopLeftY = (height - newHeight) / 2;
    //viewport.MinDepth = 0.0f;
    //viewport.MaxDepth = 1.0f;

    D3D11_VIEWPORT viewport;
    if (aspectRatioWindow > aspectRatioBase)
    {
        viewport.Width = (FLOAT)SCREEN_WIDTH * (aspectRatioBase / aspectRatioWindow);
        viewport.Height = (FLOAT)SCREEN_HEIGHT;
        //viewport.TopLeftX = SCREEN_WIDTH / (((float)width / height) - ((float)SCREEN_WIDTH / SCREEN_HEIGHT));
        viewport.TopLeftX = SCREEN_WIDTH * -(((float)height / width) - ((float)SCREEN_HEIGHT / SCREEN_WIDTH)) * aspectRatioBase / 2.0f;
        viewport.TopLeftY = 0;
    }
    else
    {
        viewport.Width = (FLOAT)SCREEN_WIDTH;
        viewport.Height = (FLOAT)SCREEN_HEIGHT * (((float)SCREEN_HEIGHT / SCREEN_WIDTH) / ((float)height / width));
        viewport.TopLeftX = 0;
        //viewport.TopLeftY = SCREEN_HEIGHT / (((float)height / width) - ((float)SCREEN_HEIGHT / SCREEN_WIDTH));
        viewport.TopLeftY = SCREEN_HEIGHT * -(((float)width / height) - ((float)SCREEN_WIDTH / SCREEN_HEIGHT)) / aspectRatioBase / 2.0f;
    }
    viewport.MinDepth = 0.0f;
    viewport.MaxDepth = 1.0f;

    g_viewPortSizeCache.x = viewport.Width;
    g_viewPortSizeCache.y = viewport.Height;

    g_DeviceContext->RSSetViewports(1, &viewport);

    g_SwapChain[0]->ResizeBuffers(0, width, height, DXGI_FORMAT_R8G8B8A8_UNORM, 0);
}
void DXUpdateWindowSize(int width, int height)
{
    g_WindowSizeCache.x = (float)width;
    g_WindowSizeCache.y = (float)height;
}

XMFLOAT2 DXGetViewportSize(void)
{
    return g_viewPortSizeCache;
}

XMFLOAT2 DXGetWindowSize(void)
{
    return g_WindowSizeCache;
}

//
// 未使用
//
void DXSetViewport(int n)
{
	//ビューポートの設定
	D3D11_VIEWPORT vp[2];

	{
		D3D11_VIEWPORT* vpt = &vp[0];
		vpt->Width = (FLOAT)SCREEN_WIDTH / 2;
		vpt->Height = (FLOAT)SCREEN_HEIGHT;
		vpt->MinDepth = 0.0f;
		vpt->MaxDepth = 1.0f;
		vpt->TopLeftX = 0;
		vpt->TopLeftY = 0;
	}
	{
		D3D11_VIEWPORT* vpt = &vp[1];
		vpt->Width = (FLOAT)SCREEN_WIDTH / 2;
		vpt->Height = (FLOAT)SCREEN_HEIGHT;
		vpt->MinDepth = 0.0f;
		vpt->MaxDepth = 1.0f;
		vpt->TopLeftX = SCREEN_WIDTH / 2;
		vpt->TopLeftY = 0;
	}

	g_DeviceContext->RSSetViewports(1, &vp[n]);
}




//
// バックバッファクリア
//
void DXClearBackBuffer(void)
{
	//黒
	float clear_color[4] = { 0.0f, 0.0f, 0.0f, 1.0f };

	DXClearTextureRTV();

	//色の塗りつぶし
    for (int i = 0; i < DX_WINDOW_NUM; i++)
    {
        g_DeviceContext->ClearRenderTargetView(g_RenderTargetView[i], clear_color);
    }

	//深度 のクリア
	g_DeviceContext->ClearDepthStencilView(g_DepthStencilView, D3D11_CLEAR_DEPTH,
		1.0f, 0);
}
void DXClearTextureRTV(void)
{
	float clear_color[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
    float clear_color2[4] = { 0.0f, 0.0f, 0.0f, 0.0f };

    g_DeviceContext->ClearRenderTargetView(g_RenderingTextureRTV_MainDummyBuf, clear_color);
	g_DeviceContext->ClearRenderTargetView(_pRenderingTextureRTV, clear_color);
    for (int i = 0; i < DX_OUTLINE_BUF_NUM; i++)
    {
        g_DeviceContext->ClearRenderTargetView(g_RenderingTextureRTV_OutlineBuf[i], clear_color2);
    }
	g_DeviceContext->ClearRenderTargetView(g_RenderingTextureRTV_BloomBuf, clear_color);

	//深度 のクリア
	g_DeviceContext->ClearDepthStencilView(g_DepthStencilView_OffScreen, D3D11_CLEAR_DEPTH,
		1.0f, 0);
}

//
// バックバッファとフロントバッファの交換
//
void DXSwapBuffer(void)
{
    for (int i = 0; i < DX_WINDOW_NUM; i++)
    {
        g_SwapChain[i]->Present(0, 0);
        //g_SwapChain[i]->Present(1, 0);
        //第一引数を1にするとディスプレイの最大フレーム数で回すようになる
    }
}


//
// ウィンドウハンドルを返す
//
HWND DXGetWindowHandle(void)
{
	return Save_hWnd[0];
}

ID3D11Device* DXGetDevice(void)
{
	return g_Device;
}
ID3D11DeviceContext* DXGetDeviceContext(void)
{
	return g_DeviceContext;
}
IDXGISwapChain* DXGetSwapChain(void)
{
	return g_SwapChain[0];
}

bool DXDepthStencilStateDepthEnable(void)
{
	return g_IsDepthStencilStateDepthEnable;
}
void DXSetDepthStencilStateDepth(bool en)
{
	if (en)
	{
		g_DeviceContext->OMSetDepthStencilState(g_DepthStencilStateDepthEnable, NULL);
		g_IsDepthStencilStateDepthEnable = true;
	}
	else
	{
		g_DeviceContext->OMSetDepthStencilState(g_DepthStencilStateDepthDisable, NULL);
		g_IsDepthStencilStateDepthEnable = false;
	}
}


void SetRenderTargetView_Final()
{
	// デバイスコンテキストへレンダーターゲットビューとデプスステンシルビューを設定
	//絵を描く場所を指定してあげる
	g_DeviceContext->OMSetRenderTargets(1, &g_RenderTargetView[0], g_DepthStencilView);
}
void SetRenderTargetView_Main()
{
    g_DeviceContext->OMSetRenderTargets(1, &g_RenderingTextureRTV_MainDummyBuf, g_DepthStencilView_OffScreen);
}
void SetRenderTargetView_OffScreen()
{
	g_DeviceContext->OMSetRenderTargets(1, &_pRenderingTextureRTV, g_DepthStencilView_OffScreen);
}
void SetRenderTargetView_OutlineBuf(int index)
{
	g_DeviceContext->OMSetRenderTargets(1, &g_RenderingTextureRTV_OutlineBuf[index], g_DepthStencilView_OffScreen);
}
void SetRenderTargetView_BloomBuf()
{
	g_DeviceContext->OMSetRenderTargets(1, &g_RenderingTextureRTV_BloomBuf, g_DepthStencilView_OffScreen);
}


ID3D11ShaderResourceView* GetSRV_OffScreen()
{
	return _pRenderingTextureSRV;
}
ID3D11ShaderResourceView* GetSRV_BloomBuf()
{
	return g_RenderingTextureSRV_BloomBuf;
}
ID3D11ShaderResourceView* GetSRV_OutlineBuf(int index)
{
	return g_RenderingTextureSRV_OutlineBuf[index];
}

ID3D11ShaderResourceView* GetSRV_MainDummyBuf()
{
    return g_RenderingTextureSRV_MainDummyBuf;
}

ID3D11Texture2D* GetTexture2D_MainDummyBuf()
{
    return g_Texture_MainDummyBuf;
}


void SetBlendState_Main()
{
	float blend_factor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
	g_DeviceContext->OMSetBlendState(g_BlendState, blend_factor, 0xffffffff);
}
void SetBlendState_AddColor()
{
	float blend_factor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
	g_DeviceContext->OMSetBlendState(g_BlendState_AddColor, blend_factor, 0xffffffff);
}





//
//テスト用
//
void DXTest(float r, float g, float b, float a)
{

	//// バックバッファクリア
	//float clear_color[4] = { r, g, b, a };
	//g_DeviceContext->ClearRenderTargetView(g_RenderTargetView, clear_color);
	//g_DeviceContext->ClearDepthStencilView(g_DepthStencilView, D3D11_CLEAR_DEPTH,
	//	1.0f, 0);

	//// バックバッファとフロントバッファの交換
	//g_SwapChain->Present(0, 0);
}
