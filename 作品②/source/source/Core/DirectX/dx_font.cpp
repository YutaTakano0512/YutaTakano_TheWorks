//---------------------------------------------------------------------------
// dx_font.cpp
// 
// sougo hara  2024/11/17
//---------------------------------------------------------------------------

#define NODRAWTEXT
#include "../../Game/Configuration/configuration.h"
//#include <d3d11.h>
//#include <DirectXMath.h>
//#include "DirectXTex.h"

#include "../../game_main.h"
#include "dx_font.h"

//=============================================================================
//		カスタムファイルローダー
//=============================================================================
class CustomFontFileEnumerator : public IDWriteFontFileEnumerator
{
public:
	CustomFontFileEnumerator(IDWriteFactory* factory, const std::vector<std::wstring>& fontFilePaths)
		: refCount_(0), factory_(factory), fontFilePaths_(fontFilePaths), currentFileIndex_(-1)
	{
		factory_->AddRef();
	}

	~CustomFontFileEnumerator()
	{
		factory_->Release();
	}

	IFACEMETHODIMP QueryInterface(REFIID iid, void** ppvObject) override
	{
		if (iid == __uuidof(IUnknown) || iid == __uuidof(IDWriteFontCollectionLoader))
		{
			*ppvObject = this;
			AddRef();
			return S_OK;
		}
		else
		{
			*ppvObject = nullptr;
			return E_NOINTERFACE;
		}
	}

	IFACEMETHODIMP_(ULONG) AddRef() override
	{
		return InterlockedIncrement(&refCount_);
	}

	IFACEMETHODIMP_(ULONG) Release() override
	{
		ULONG newCount = InterlockedDecrement(&refCount_);
		if (newCount == 0)
			delete this;

		return newCount;
	}

	IFACEMETHODIMP MoveNext(OUT BOOL* hasCurrentFile) override {
		if (++currentFileIndex_ < static_cast<int>(fontFilePaths_.size())) {
			*hasCurrentFile = TRUE;
			return S_OK;
		}
		else {
			*hasCurrentFile = FALSE;
			return S_OK;
		}
	}

	IFACEMETHODIMP GetCurrentFontFile(OUT IDWriteFontFile** fontFile) override
	{
		// フォントファイルを読み込む
		return factory_->CreateFontFileReference(fontFilePaths_[currentFileIndex_].c_str(), nullptr, fontFile);
	}

private:
	ULONG refCount_;

	// DirectWriteファクトリ
	IDWriteFactory* factory_;

	// フォントファイルのパス
	std::vector<std::wstring> fontFilePaths_;

	// 現在のファイルインデックス
	int currentFileIndex_;
};

//=============================================================================
//		カスタムフォントコレクションローダー
//=============================================================================
class CustomFontCollectionLoader : public IDWriteFontCollectionLoader
{
public:
	// コンストラクタ
	CustomFontCollectionLoader(std::vector<std::wstring> setFontFilePaths) :
		refCount_(0),
		m_fontFilePaths(setFontFilePaths) {}

	// IUnknown メソッド
	IFACEMETHODIMP QueryInterface(REFIID iid, void** ppvObject) override
	{
		if (iid == __uuidof(IUnknown) || iid == __uuidof(IDWriteFontCollectionLoader))
		{
			*ppvObject = this;
			AddRef();
			return S_OK;
		}
		else
		{
			*ppvObject = nullptr;
			return E_NOINTERFACE;
		}
	}

	IFACEMETHODIMP_(ULONG) AddRef() override
	{
		return InterlockedIncrement(&refCount_);
	}

	IFACEMETHODIMP_(ULONG) Release() override
	{
		ULONG newCount = InterlockedDecrement(&refCount_);
		if (newCount == 0)
			delete this;

		return newCount;
	}

	// IDWriteFontCollectionLoader メソッド
	IFACEMETHODIMP CreateEnumeratorFromKey
	(
		IDWriteFactory* factory,
		void const* collectionKey,
		UINT32 collectionKeySize,
		OUT IDWriteFontFileEnumerator** fontFileEnumerator) override
	{
		// カスタムフォントファイル列挙子の作成
		*fontFileEnumerator = new (std::nothrow) CustomFontFileEnumerator(factory, m_fontFilePaths);

		// メモリ不足の場合はエラーを返す
		if (*fontFileEnumerator == nullptr) { return E_OUTOFMEMORY; }

		return S_OK;
	}

private:
	ULONG refCount_;
	std::vector<std::wstring> m_fontFilePaths; // 読み込むフォントファイルのパス
};

//
// マクロ定義
//

//
// 構造体宣言
//


//
// プロトタイプ宣言
//
HRESULT FontLoader();
HRESULT GetFontFamilyName(IDWriteFontCollection* customFontCollection, const WCHAR* locale = L"en-us");
std::wstring GetFontFileNameWithoutExtension(const std::wstring& filePath);

//
// グローバル変数
//

static ID2D1Factory*				g_D2DFactory = nullptr;		// Direct2Dリソース
static ID2D1RenderTarget*			g_RenderTarget = nullptr;	// Direct2Dレンダーターゲット
//static ID2D1SolidColorBrush*		g_Brush = nullptr;			// Direct2Dブラシ設定
static IDWriteFactory*				g_DWriteFactory = nullptr;	// DirectWriteリソース
//static IDWriteTextFormat*			g_TextFormat = nullptr;		// DirectWriteテキスト形式
static IDWriteTextLayout*			g_TextLayout = nullptr;		// DirectWriteテキスト書式
static IDXGISurface*				g_BackBuffer = nullptr;		// サーフェス情報


//ディスプレイのバッグバッファのテクスチャ
//static ID3D11Texture2D* g_Texture_FontBuf = nullptr;
// オフスク用 RTV
//static ID3D11RenderTargetView* g_RenderingTextureRTV_FontBuf = nullptr;
// オフスク用 SRV
//static ID3D11ShaderResourceView* g_RenderingTextureSRV_FontBuf = nullptr;



static D2D1_RENDER_TARGET_PROPERTIES g_Props = {};

static CustomFontCollectionLoader* g_FontCollectionLoader = nullptr;

// カスタムフォントコレクション
static IDWriteFontCollection* g_FontCollection = nullptr;
// フォントファイルリスト
static std::vector<IDWriteFontFile*> g_FontFileList = {};
// フォント名リスト
static std::vector<std::wstring> g_FontNamesList = {};

static FontData  g_FontDataSetting = {};



void DX_FontInitialize(std::vector<std::wstring> setFontFilePaths)
{
	HRESULT result = S_OK;

	g_FontDataSetting = FontData();

	// Direct2Dファクトリ情報の初期化
	result = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &g_D2DFactory);
	if (FAILED(result))
	{

	}

	// バックバッファの取得
	//result = DXGetSwapChain()->GetBuffer(0, IID_PPV_ARGS(&g_BackBuffer));
	//if (FAILED(result))
	//{

	//}

    ////-----------------------------
    //// メインダミー
    ////-----------------------------
    //{
    //    {
    //        // テクスチャ設定 オフスク用
    //        D3D11_TEXTURE2D_DESC rtDesc;
    //        memset(&rtDesc, 0, sizeof(rtDesc));
    //        rtDesc.Width = SCREEN_WIDTH;
    //        rtDesc.Height = SCREEN_HEIGHT;
    //        rtDesc.MipLevels = 1;
    //        rtDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    //        //rtDesc.Format = DXGI_FORMAT_R16G16B16A16_FLOAT; // 高精度フォーマット

    //        rtDesc.SampleDesc.Count = 1;
    //        rtDesc.Usage = D3D11_USAGE_DEFAULT;
    //        rtDesc.ArraySize = 1;
    //        rtDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
    //        rtDesc.CPUAccessFlags = 0;

    //        DXGetDevice()->CreateTexture2D(&rtDesc, 0, &g_Texture_FontBuf);


    //        //	SRV設定 オフスク用
    //        D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
    //        ZeroMemory(&srvDesc, sizeof(srvDesc));
    //        srvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    //        srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
    //        srvDesc.Texture2D.MostDetailedMip = 0;
    //        srvDesc.Texture2D.MipLevels = rtDesc.MipLevels;
    //        if (g_Texture_FontBuf != NULL) //警告回避
    //        {
    //            DXGetDevice()->CreateShaderResourceView(g_Texture_FontBuf, &srvDesc, &g_RenderingTextureSRV_FontBuf);
    //        }

    //        //if (g_Texture_FontBuf != NULL) //警告回避
    //        //{
    //        //    DXGetDevice()->CreateRenderTargetView(g_Texture_FontBuf, NULL, & );
    //        //}
    //    }
    //}

    //result = g_Texture_FontBuf->QueryInterface(IID_PPV_ARGS(&g_BackBuffer));
    //if (FAILED(result)) { /* エラー処理 */ }

    result = GetTexture2D_MainDummyBuf()->QueryInterface(IID_PPV_ARGS(&g_BackBuffer));
    if (FAILED(result)) { /* エラー処理 */ }
    

	//// dpiの設定
	//FLOAT dpiX;
	//FLOAT dpiY;
	////pD2DFactory->GetDesktopDpi(&dpiX, &dpiY);
 //   
	////{
	////	HDC hdc = GetDC(NULL);
	////	dpiX = (FLOAT)GetDeviceCaps(hdc, LOGPIXELSX);
	////	dpiY = (FLOAT)GetDeviceCaps(hdc, LOGPIXELSY);
	////	ReleaseDC(NULL, hdc);
	////}
 //   // 
 //   
    //g_D2DFactory->GetDesktopDpi(&dpiX, &dpiY);

    //dpiX = SCREEN_WIDTH;
    //dpiY = SCREEN_HEIGHT;

    {
        //// ID3D11Texture2Dの作成
        //D3D11_TEXTURE2D_DESC desc = {};
        //desc.Width = SCREEN_WIDTH;
        //desc.Height = SCREEN_HEIGHT;
        //desc.MipLevels = 1;
        //desc.ArraySize = 1;
        //desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
        //desc.SampleDesc.Count = 1;
        //desc.Usage = D3D11_USAGE_DEFAULT;
        //desc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;

        //DXGetDevice()->CreateTexture2D(&desc, nullptr, &g_Texture_FontBuf);

        //result = g_Texture_FontBuf->QueryInterface(__uuidof(IDXGISurface),
        //    reinterpret_cast<void**>(&g_BackBuffer));

        //if (FAILED(result)) {
        //    // エラーハンドリング
        //    return;
        //}

        //// Direct2Dレンダリングターゲットの設定
        //D2D1_RENDER_TARGET_PROPERTIES props = D2D1::RenderTargetProperties();
        //props.pixelFormat = D2D1::PixelFormat(
        //    DXGI_FORMAT_R8G8B8A8_UNORM,
        //    D2D1_ALPHA_MODE_PREMULTIPLIED); // ピクセル形式の設定
        //result = g_D2DFactory->CreateDxgiSurfaceRenderTarget(g_BackBuffer, &props, &g_RenderTarget);
        //if (FAILED(result)) {
        //    // エラーハンドリング
        //    return;
        //}

        //// Shader Resource Viewの作成
        //D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
        //srvDesc.Format = desc.Format;
        //srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
        //srvDesc.Texture2D.MostDetailedMip = 0;
        //srvDesc.Texture2D.MipLevels = 1;

        //if (g_Texture_FontBuf == nullptr)
        //{
        //    return;
        //}
        //DXGetDevice()->CreateShaderResourceView(g_Texture_FontBuf, &srvDesc, 
        //    &g_RenderingTextureSRV_FontBuf);
    }

	//// レンダーターゲットの作成
 //   g_Props = D2D1::RenderTargetProperties(D2D1_RENDER_TARGET_TYPE_DEFAULT,
 //       D2D1::PixelFormat(
 //           //DXGI_FORMAT_R8G8B8A8_UNORM,
 //           DXGI_FORMAT_UNKNOWN, 
 //           D2D1_ALPHA_MODE_PREMULTIPLIED
 //       ), dpiX, dpiY);

	////
	//// DirectX 初期化 時に D3D11CreateDeviceAndSwapChain の第4引数を
	//// D3D11_CREATE_DEVICE_BGRA_SUPPORT にしないと失敗するので要注意
	//// anchor: sougo hara
	////

	//
 //   
 //   // サーフェスに描画するレンダーターゲットを作成
	//result = g_D2DFactory->CreateDxgiSurfaceRenderTarget(g_BackBuffer, &g_Props, &g_RenderTarget);
	//if (FAILED(result))
	//{

	//}
    
    //g_RenderTarget->Resize(D2D1::SizeU(LOWORD(lParam), HIWORD(lParam)));

        // Direct2Dレンダリングターゲットの設定
    D2D1_RENDER_TARGET_PROPERTIES props = D2D1::RenderTargetProperties();
    props.pixelFormat = D2D1::PixelFormat(
        DXGI_FORMAT_UNKNOWN,
        D2D1_ALPHA_MODE_PREMULTIPLIED); // ピクセル形式の設定
    result = g_D2DFactory->CreateDxgiSurfaceRenderTarget(g_BackBuffer, &props, &g_RenderTarget);
    if (FAILED(result)) {
        // エラーハンドリング
        return;
    }

	// アンチエイリアシングモードの設定
	g_RenderTarget->SetTextAntialiasMode(D2D1_TEXT_ANTIALIAS_MODE_CLEARTYPE);

	// IDWriteFactoryの作成
	result = DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED,
		__uuidof(IDWriteFactory), reinterpret_cast<IUnknown**>(&g_DWriteFactory));
	if (FAILED(result))
	{

	}

	// カスタムフォントコレクションローダー
	g_FontCollectionLoader = new CustomFontCollectionLoader(setFontFilePaths);

	// カスタムフォントコレクションローダーの作成
	result = g_DWriteFactory->RegisterFontCollectionLoader(g_FontCollectionLoader);
	if (FAILED(result))
	{

	}

	// フォントファイルの読み込み
	result = FontLoader();
	if (FAILED(result))
	{

	}

	//// フォントを設定
	//result = DX_SetFont(g_FontDataSetting);
	//if (FAILED(result))
	//{

	//}

}

void DX_FontFinalize()
{
	SAFE_RELEASE(g_D2DFactory);		// Direct2Dリソース
	SAFE_RELEASE(g_RenderTarget);	// Direct2Dレンダーターゲット
	//SAFE_RELEASE(g_Brush);			// Direct2Dブラシ設定
	SAFE_RELEASE(g_DWriteFactory);	// DirectWriteリソース
	//SAFE_RELEASE(g_TextFormat);		// DirectWriteテキスト形式
	SAFE_RELEASE(g_TextLayout);		// DirectWriteテキスト書式
	SAFE_RELEASE(g_BackBuffer);		// サーフェス情報

    //ディスプレイのバッグバッファのテクスチャ
    //SAFE_RELEASE(g_Texture_FontBuf);
    //SAFE_RELEASE(g_RenderingTextureRTV_FontBuf);
    //SAFE_RELEASE(g_RenderingTextureSRV_FontBuf);


	delete g_FontCollectionLoader;

	// カスタムフォントコレクション
	SAFE_RELEASE(g_FontCollection);
	// フォントファイルリスト
	for (int i = 0; i < g_FontFileList.size(); i++)
	{
		SAFE_RELEASE(g_FontFileList[i]);
	}
	g_FontFileList.clear();
	g_FontFileList.shrink_to_fit();

	// フォント名リスト
	g_FontNamesList.clear();
	g_FontNamesList.shrink_to_fit();
}




//====================================
// 文字描画
// string：文字列
// pos：描画ポジション
// options：テキストの整形
//====================================
HRESULT DX_DrawString(
    std::wstring str, float x, float y,
    float maxwidth, float maxhight,
    float LineSpacingwidth, float LineSpacinghight,
    D2D1_DRAW_TEXT_OPTIONS options,
    float shadowX, float shadowY,

    IDWriteTextFormat* format,
    ID2D1SolidColorBrush* brush,
    ID2D1SolidColorBrush* brushShadow,

    std::vector<DX_DrawString_SetSize> setsize,
    std::vector<DX_DrawString_SetDrawingEffect> setdrawingeffect
)
{
	HRESULT result = S_OK;

	// 文字列の変換
	//std::wstring wstr = StringToWString(str.c_str());
	std::wstring wstr = str;

	// ターゲットサイズの取得
	D2D1_SIZE_F TargetSize = g_RenderTarget->GetSize();
    //XMFLOAT2 vpsize = DXGetViewportSize();
    //{
    //    XMFLOAT2 vpsize = DXGetWindowSize();

    //    float aspectRatioWindow = (float)vpsize.x / vpsize.y; // ウィンドウアスペクト比
    //    float aspectRatioBase = (float)TargetSize.width / TargetSize.height; // 固定アスペクト比

    //    if (aspectRatioWindow > aspectRatioBase)
    //    {
    //        TargetSize.width = +TargetSize.width * (aspectRatioBase / aspectRatioWindow);
    //        TargetSize.height = TargetSize.height * 1.0f;
    //    }
    //    else
    //    {
    //        TargetSize.width = +TargetSize.width * 1.0f;
    //        TargetSize.height = TargetSize.height * (((float)TargetSize.height / SCREEN_WIDTH) / ((float)vpsize.y / vpsize.x));
    //    }
    //}

	// テキストレイアウトを作成
	SAFE_RELEASE(g_TextLayout);
	result = g_DWriteFactory->CreateTextLayout(
		wstr.c_str(),
		(UINT32)wstr.size(),
        format,
        TargetSize.width,
        TargetSize.height,
        //vpsize.x,
        //vpsize.y,
		&g_TextLayout
	);

	if (FAILED(result)) { return result; }

    {
        // 行間の設定
        format->SetLineSpacing(DWRITE_LINE_SPACING_METHOD_UNIFORM, LineSpacingwidth, LineSpacinghight);
        //{
        //    DWRITE_TEXT_RANGE dtr;
        //    dtr.startPosition = 0;//配列何番目から
        //    dtr.length = wstr.size();//何文字分

        //    g_TextLayout->SetFontStretch(
        //        DWRITE_FONT_STRETCH_CONDENSED,
        //        dtr
        //    );
        //}

        //{
        //    //まず、構造体で対象文字列の設定をします
        //    DWRITE_TEXT_RANGE dtr;
        //    dtr.startPosition = 0;//配列何番目から
        //    dtr.length = wstr.size();//何文字分
        //    g_TextLayout->SetFontSize(100, dtr);//TRUEで下線を引く
        //}
        if (setsize.size() > 0)
        {
            for (auto v : setsize)
            {
                //まず、構造体で対象文字列の設定をします
                DWRITE_TEXT_RANGE dtr;
                dtr.startPosition = v.start;//配列何番目から
                dtr.length = v.length;//何文字分
                g_TextLayout->SetFontSize(v.size, dtr);//TRUEで下線を引く
            }
        }

        if (maxwidth > 0)
        {
            g_TextLayout->SetMaxWidth(maxwidth);
        }

        if (maxhight > 0)
        {
            g_TextLayout->SetMaxHeight(maxhight);
        }

        // 必要に応じて、テキストレイアウトのプロパティを設定
        g_TextLayout->SetWordWrapping(DWRITE_WORD_WRAPPING_WRAP); // 改行を有効にする
    }

	// 描画位置の確定
	D2D1_POINT_2F pounts;
	pounts.x = x;
	pounts.y = y;

	// 描画の開始
	g_RenderTarget->BeginDraw();

    if (brushShadow != nullptr && (shadowX != 0.0f || shadowY != 0.0f))
    { //影

        D2D1_POINT_2F pounts2 = pounts;
        pounts2.x += shadowX;
        pounts2.y += shadowY;

        // 描画処理
        g_RenderTarget->DrawTextLayout(pounts2, g_TextLayout, brushShadow, options);
    }

    {
        if (setdrawingeffect.size() > 0)
        {
            for (auto v : setdrawingeffect)
            {
                //まず、構造体で対象文字列の設定をします
                DWRITE_TEXT_RANGE dtr;
                dtr.startPosition = v.start;//配列何番目から
                dtr.length = v.length;//何文字分
                g_TextLayout->SetDrawingEffect(v.drawingEffect, dtr);//TRUEで下線を引く
            }
        }
    }

	// 描画処理
    g_RenderTarget->DrawTextLayout(pounts, g_TextLayout, brush, options);
    //g_RenderTarget->DrawText(
    //    wstr.c_str(),
    //    (UINT32)wstr.size(),
    //    format,
    //    { TargetSize.width, TargetSize.height },
    //    brush,
    //    options
    //);

	// 描画の終了
	result = g_RenderTarget->EndDraw();

	if (FAILED(result)) { return result; }

	return S_OK;
}

//ID3D11ShaderResourceView* DX_GetFontSRV()
//{
//    return g_RenderingTextureSRV_FontBuf;
//}

//DX_StringTexture DX_CreateStringTexture(std::wstring str, float x, float y, D2D1_DRAW_TEXT_OPTIONS options)
//{
//    HRESULT result = S_OK;
//
//    DX_StringTexture resultDat = {};
//
//    // サーフェスに描画するレンダーターゲットを作成
//    //result = g_D2DFactory->CreateDxgiSurfaceRenderTarget(g_BackBuffer, &g_Props, &g_RenderTarget);
//    //if (FAILED(result))
//    //{
//
//    //}
//
//    // 文字列の変換
//    //std::wstring wstr = StringToWString(str.c_str());
//    std::wstring wstr = str;
//
//    // ターゲットサイズの取得
//    D2D1_SIZE_F TargetSize = g_RenderTarget->GetSize();
//
//    // テキストレイアウトを作成
//    SAFE_RELEASE(g_TextLayout);
//    result = g_DWriteFactory->CreateTextLayout(
//        wstr.c_str(),
//        (UINT32)wstr.size(),
//        g_TextFormat,
//        TargetSize.width,
//        TargetSize.height,
//        &g_TextLayout
//    );
//
//    if (FAILED(result)) { return {}; }
//
//    // 描画位置の確定
//    D2D1_POINT_2F pounts;
//    pounts.x = x;
//    pounts.y = y;
//
//    // 描画の開始
//    g_RenderTarget->BeginDraw();
//
//    // 描画処理
//    g_RenderTarget->DrawTextLayout(pounts, g_TextLayout, g_Brush, options);
//
//    // 描画の終了
//    result = g_RenderTarget->EndDraw();
//
//    if (FAILED(result)) { return {}; }
//
//    return resultDat;
//}



// 指定されたパスのフォントを読み込む
HRESULT FontLoader()
{
	HRESULT result = S_OK;

	// カスタムフォントコレクションの作成
	result = g_DWriteFactory->CreateCustomFontCollection
	(
		g_FontCollectionLoader,
		g_FontFileList.data(),
		(UINT32)g_FontFileList.size(),
		&g_FontCollection
	);
	if (FAILED(result)) { return result; }

	// フォント名を取得
	result = GetFontFamilyName(g_FontCollection);
	if (FAILED(result)) { return result; }

	return S_OK;
}

// フォント名を取得
HRESULT GetFontFamilyName(IDWriteFontCollection* customFontCollection, const WCHAR* locale)
{
	HRESULT result = S_OK;

	// フォントファミリー名一覧をリセット
	std::vector<std::wstring>().swap(g_FontNamesList);

	// フォントの数を取得
	UINT32 familyCount = customFontCollection->GetFontFamilyCount();

	for (UINT32 i = 0; i < familyCount; i++)
	{
		// フォントファミリーの取得
		IDWriteFontFamily* fontFamily = nullptr;
		result = customFontCollection->GetFontFamily(i, &fontFamily);
		if (FAILED(result))
		{
			SAFE_RELEASE(fontFamily);
			return result;
		}

		// フォントファミリー名の一覧を取得
		IDWriteLocalizedStrings* familyNames = nullptr;
		result = fontFamily->GetFamilyNames(&familyNames);
		if (FAILED(result))
		{
			SAFE_RELEASE(fontFamily);
			SAFE_RELEASE(familyNames);
			return result;
		}

		// 指定されたロケールに対応するインデックスを検索
		UINT32 index = 0;
		BOOL exists = FALSE;
		result = familyNames->FindLocaleName(locale, &index, &exists);

		if (FAILED(result))
		{
			SAFE_RELEASE(fontFamily);
			SAFE_RELEASE(familyNames);
			return result;
		}

		// 指定されたロケールが見つからなかった場合は、デフォルトのロケールを使用
		if (!exists)
		{
			result = familyNames->FindLocaleName(L"en-us", &index, &exists);

			if (FAILED(result))
			{
				SAFE_RELEASE(fontFamily);
				SAFE_RELEASE(familyNames);
				return result;
			}
		}

		// フォントファミリー名の長さを取得
		UINT32 length = 0;
		result = familyNames->GetStringLength(index, &length);

		if (FAILED(result))
		{
			SAFE_RELEASE(fontFamily);
			SAFE_RELEASE(familyNames);
			return result;
		}

		// フォントファミリー名の取得
		std::wstring name = {};
		{
			WCHAR* nameTmp = new WCHAR[length + 1];
			result = familyNames->GetString(index, nameTmp, length + 1);
			name = nameTmp;
			delete[] nameTmp;
		}

		if (FAILED(result)) 
		{
			SAFE_RELEASE(fontFamily);
			SAFE_RELEASE(familyNames);
			return result; 
		}

		// フォントファミリー名を追加
		g_FontNamesList.push_back(name);

		SAFE_RELEASE(fontFamily);
		SAFE_RELEASE(familyNames);
	}

	return result;
}

// フォント名を取得する
std::wstring DX_GetFontName(int num)
{
	// フォント名のリストが空だった場合
	if (g_FontNamesList.empty())
	{
		//return nullptr;
		return L"";
	}

	// リストのサイズを超えていた場合
	if (num >= static_cast<int>(g_FontNamesList.size()))
	{
		return g_FontNamesList[0];
	}

	return g_FontNamesList[num];
}

// 読み込んだフォント名の数を取得する
int DX_GetFontNameNum()
{
	return (int)g_FontNamesList.size();
}

HRESULT DX_CreateFont(FontData set,
    IDWriteTextFormat** textFormat
)
{
	HRESULT result = S_OK;

	// 設定をコピー
	g_FontDataSetting = set;

	//関数CreateTextFormat()
	//第1引数：フォント名（L"メイリオ", L"Arial", L"Meiryo UI"等）
	//第2引数：フォントコレクション（nullptr）
	//第3引数：フォントの太さ（DWRITE_FONT_WEIGHT_NORMAL, DWRITE_FONT_WEIGHT_BOLD等）
	//第4引数：フォントスタイル（DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STYLE_OBLIQUE, DWRITE_FONT_STYLE_ITALIC）
	//第5引数：フォントの幅（DWRITE_FONT_STRETCH_NORMAL,DWRITE_FONT_STRETCH_EXTRA_EXPANDED等）
	//第6引数：フォントサイズ（20, 30等）
	//第7引数：ロケール名（L""）
	//第8引数：テキストフォーマット（&g_pTextFormat）

    SAFE_RELEASE(*textFormat);
	result = g_DWriteFactory->CreateTextFormat
	(
		GetFontFileNameWithoutExtension(g_FontDataSetting.font.c_str()).c_str(),
		g_FontCollection,
		g_FontDataSetting.fontWeight,
		g_FontDataSetting.fontStyle,
		g_FontDataSetting.fontStretch,
		g_FontDataSetting.fontSize,
		g_FontDataSetting.localeName,
		textFormat
	);
	if (FAILED(result)) { 
        return result;
    }

	//関数SetTextAlignment()
	//第1引数：テキストの配置（DWRITE_TEXT_ALIGNMENT_LEADING：前, DWRITE_TEXT_ALIGNMENT_TRAILING：後, DWRITE_TEXT_ALIGNMENT_CENTER：中央,
	//                         DWRITE_TEXT_ALIGNMENT_JUSTIFIED：行いっぱい）
	result = (*textFormat)->SetTextAlignment(g_FontDataSetting.textAlignment);
	if (FAILED(result)) { 
        return result;
    }

	return result;
}
HRESULT DX_CreateBrush(
    D2D1_COLOR_F color,
    ID2D1SolidColorBrush** brush
)
{
    HRESULT result = S_OK;

    //関数CreateSolidColorBrush()
    //第1引数：フォント色（D2D1::ColorF(D2D1::ColorF::Black)：黒, D2D1::ColorF(D2D1::ColorF(0.0f, 0.2f, 0.9f, 1.0f))：RGBA指定）
    SAFE_RELEASE(*brush);
    result = g_RenderTarget->CreateSolidColorBrush(color, brush);
    //result = g_RenderTarget->CreateSolidColorBrush(g_FontDataSetting.Color, brush);
    if (FAILED(result)) {
        return result; 
    }

    //// 影用のブラシを作成
    //result = g_RenderTarget->CreateSolidColorBrush(g_FontDataSetting.shadowColor, g_ShadowBrush);
    //if (FAILED(result)) { return result; }

    return result;
}



// 拡張子を除くファイル名を返す
std::wstring GetFontFileNameWithoutExtension(const std::wstring& filePath)
{
	// 末尾から検索してファイル名と拡張子の位置を取得
	size_t start = filePath.find_last_of(L"/\\") + 1;
	size_t end = filePath.find_last_of(L'.');

	// ファイル名を取得
	std::wstring fileNameWithoutExtension = filePath.substr(start, end - start).c_str();

	std::wstring rname = {};
	{
		// 新しいWCHAR配列を作成
		WCHAR* fileName = new WCHAR[fileNameWithoutExtension.length() + 1];

		// 文字列をコピー
		wcscpy_s(fileName, fileNameWithoutExtension.length() + 1, fileNameWithoutExtension.c_str());

		//セット
		rname = fileName;

		//消す
		delete[] fileName;
	}

	// ファイル名を返す
	return rname;
}


