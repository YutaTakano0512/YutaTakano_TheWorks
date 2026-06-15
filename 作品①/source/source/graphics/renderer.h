#pragma once
#include "../core/main.h"

//=============================================================================
// 頂点フォーマット(2D専用)
//=============================================================================
struct VERTEX_2D
{
    DirectX::XMFLOAT3 Position;   // XY座標 + Z(深度)
    DirectX::XMFLOAT4 Color;      // RGBA
    DirectX::XMFLOAT2 TexCoord;   // UV座標
};

//=============================================================================
// 定数バッファ構造体(HLSLと一致させること)
//=============================================================================
struct CB_World
{
    DirectX::XMFLOAT4X4 World;
};

struct CB_View
{
    DirectX::XMFLOAT4X4 View;
};

struct CB_Projection
{
    DirectX::XMFLOAT4X4 Projection;
};

struct CB_Material
{
    DirectX::XMFLOAT4 Color;         // RGBA 乗算色
    BOOL     TextureEnable; // テクスチャ使用フラグ
    float    Dummy[3];
};

struct CB_Time
{
    float Time;       // 経過時間(秒)
    float DeltaTime;
    float Dummy[2];
};

struct CB_Parameter
{
    DirectX::XMFLOAT4 Parameter; // シェーダー固有パラメータ
};

//=============================================================================
// ブレンドモード
//=============================================================================
enum class BlendMode
{
    NORMAL,    // 通常αブレンド
    ADD,       // 加算合成(エフェクト)
    MULTIPLY,  // 乗算
    MASK,      // 書き込みマスク
};

//=============================================================================
// Renderer クラス (DX11デバイス・ステート管理)
//=============================================================================
class Renderer
{
public:
    static void Init();
    static void Uninit();
    static void Begin();
    static void End();
    static void OnWindowResize(UINT clientW, UINT clientH);  // ウィンドウサイズ変化時に呼ぶ

    //-------------------------------------------------------------------
    // デバイスアクセス
    //-------------------------------------------------------------------
    static ID3D11Device*        GetDevice()        { return m_Device; }
    static ID3D11DeviceContext* GetDeviceContext()  { return m_DeviceContext; }
    static ID2D1Factory*        GetD2DFactory()    { return m_D2DFactory; }
    static IDWriteFactory*      GetDWriteFactory() { return m_DWriteFactory; }

    //-------------------------------------------------------------------
    // 定数バッファセット
    //-------------------------------------------------------------------
    static void SetWorldMatrix     (const DirectX::XMMATRIX& m);
    static void SetViewMatrix      (const DirectX::XMMATRIX& m);
    static void SetProjectionMatrix(const DirectX::XMMATRIX& m);
    static void SetMaterialColor   (const DirectX::XMFLOAT4& color, bool texEnable = true);
    static void SetParameter       (const DirectX::XMFLOAT4& param);
    static void UpdateTime         (float dt);

    // 2D用: 画面座標系 (左上原点, ピクセル単位 をNDCに変換するVP行列をセット
    static void SetWorldViewProjection2D();

    //-------------------------------------------------------------------
    // レンダリングステート
    //-------------------------------------------------------------------
    static void SetBlendMode   (BlendMode mode);
    static void SetDepthEnable (bool enable);
    static void SetSamplerState(bool isPixelArt);

    //-------------------------------------------------------------------
    // シェーダーロード
    //-------------------------------------------------------------------
    static void CreateVertexShader(
        ID3D11VertexShader** ppVS,
        ID3D11InputLayout**  ppLayout,
        const char* filePath);

    static void CreatePixelShader(
        ID3D11PixelShader** ppPS,
        const char* filePath);

    //-------------------------------------------------------------------
    // バックバッファ RTV / DSV 取得(PostProcess用)
    //-------------------------------------------------------------------
    static ID3D11RenderTargetView*  GetBackBufferRTV() { return m_RenderTargetView; }
    static ID3D11DepthStencilView*  GetDepthStencilView() { return m_DepthStencilView; }

private:
    static D3D_FEATURE_LEVEL        m_FeatureLevel;
    static ID3D11Device*            m_Device;
    static ID3D11DeviceContext*     m_DeviceContext;
    static IDXGISwapChain*          m_SwapChain;
    static ID3D11RenderTargetView*  m_RenderTargetView;
    static ID3D11DepthStencilView*  m_DepthStencilView;

    // 定数バッファ (レジスタ b0〜b5)
    static ID3D11Buffer* m_WorldBuffer;
    static ID3D11Buffer* m_ViewBuffer;
    static ID3D11Buffer* m_ProjectionBuffer;
    static ID3D11Buffer* m_MaterialBuffer;
    static ID3D11Buffer* m_TimeBuffer;
    static ID3D11Buffer* m_ParameterBuffer;

    // ブレンドステート
    static ID3D11BlendState* m_BlendNormal;
    static ID3D11BlendState* m_BlendAdd;
    static ID3D11BlendState* m_BlendMultiply;
    static ID3D11BlendState* m_BlendMask;

    // デプス/ステンシルステート
    static ID3D11DepthStencilState* m_DepthEnabled;
    static ID3D11DepthStencilState* m_DepthDisabled;

    // ラスタライザステート
    static ID3D11RasterizerState* m_RasterCullNone;

    // サンプラーステート
    static ID3D11SamplerState* m_SamplerLinear;
    static ID3D11SamplerState* m_SamplerPoint;

    // 時間
    static float m_TotalTime;

    // Direct2D / DirectWrite ファクトリ
    static ID2D1Factory*   m_D2DFactory;
    static IDWriteFactory* m_DWriteFactory;
};
