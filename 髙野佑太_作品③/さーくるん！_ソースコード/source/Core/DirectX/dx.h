//---------------------------------------------------------------------------
// dx.h
// 
// sougo hara  2024/05/14
//---------------------------------------------------------------------------
#ifndef _DX_H_
#define _DX_H_

#include <d3d11.h>
#include <xinput.h>//---★追加---

#include <d3d11.h>
#include <DirectXMath.h>
#include "../../../DirectXTex.h"
using namespace DirectX;

//
// マクロ定義
//
#define SAFE_RELEASE(o) if (o) { (o)->Release(); o = NULL; }
constexpr int DX_OUTLINE_BUF_NUM = 2;

//
// 構造体宣言
//
struct DX_INIT_SET { //初期化時に受け渡しするデータ
	HWND* hWnd;
};
struct DX_INIT_RESULT { //初期化終了時に外部に渡すデータ
	ID3D11Device* device ;
	ID3D11DeviceContext* deviceContext ;
};

struct Vertex_DX { // 頂点構造体
    XMFLOAT3 pos;	// 座標
    XMFLOAT4 color;	// 頂点カラー
    XMFLOAT2 texcoord; // テクスチャ座標(UV)
    XMFLOAT3 normal;	// 法線
};


//
// プロトタイプ宣言
//
void DXInitialize(DX_INIT_SET set, DX_INIT_RESULT* result);
void DXFinalize(void);

void DXSetViewport(int n);
void DXUpdateViewport(int width, int height);
void DXUpdateWindowSize(int width, int height);
XMFLOAT2 DXGetViewportSize(void);
XMFLOAT2 DXGetWindowSize(void);

void DXClearBackBuffer(void);
void DXClearTextureRTV(void);

void DXSwapBuffer(void);

HWND DXGetWindowHandle(void);
ID3D11Device* DXGetDevice(void);
ID3D11DeviceContext* DXGetDeviceContext(void);
IDXGISwapChain* DXGetSwapChain(void);

bool DXDepthStencilStateDepthEnable(void);
void DXSetDepthStencilStateDepth(bool en);

void SetBlendState_Main();
void SetBlendState_AddColor();


void SetRenderTargetView_Final();
void SetRenderTargetView_Main();
void SetRenderTargetView_OffScreen();
void SetRenderTargetView_BloomBuf();
void SetRenderTargetView_OutlineBuf(int index);

ID3D11ShaderResourceView* GetSRV_OffScreen();
ID3D11ShaderResourceView* GetSRV_BloomBuf();
ID3D11ShaderResourceView* GetSRV_OutlineBuf(int index);
ID3D11ShaderResourceView* GetSRV_MainDummyBuf();

ID3D11Texture2D* GetTexture2D_MainDummyBuf();

void DXTest(float r, float g, float b, float a);

#endif