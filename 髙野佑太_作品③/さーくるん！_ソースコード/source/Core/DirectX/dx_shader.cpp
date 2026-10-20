///---------------------------------------------------------------------------
// shader.cpp
// シェーダーの利用
// sougo hara  2024/05/29
//---------------------------------------------------------------------------
#include "../../Game/Configuration/configuration.h"
#include "../../Game/Systems/sys_general.h"
#include "dx.h"
#include <d3d11.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>
using namespace DirectX;

#include "dx_shader.h"

#pragma comment(lib, "d3dcompiler.lib")

//
// 構造体宣言
//
struct ShaderDat
{
	std::string name;

    //ID3D11VertexShader* VertexShader;
    //ID3D11PixelShader* PixelShader;
    union {
        ID3D11VertexShader* VertexShader;
        ID3D11PixelShader* PixelShader;
        ID3D11GeometryShader* GeometryShader;
    };
    //ID3D11InputLayout* VertexLayout;
};

struct VertexLayout // 頂点構造体
{
    XMFLOAT4 Position;
    XMFLOAT4 Diffuse;
    XMFLOAT2 TexCoord;
    XMFLOAT4 Normal;
};

struct ConstantBuffer //コンスタントバッファ構造体
{
	XMFLOAT4X4 WorldViewProjection;

    XMMATRIX World;
    XMMATRIX View;
    XMMATRIX Projection;

	XMFLOAT4 Transition;
	XMFLOAT4 TexScale;
	XMFLOAT4 TexUV;
	XMFLOAT4 TexColor;

	XMFLOAT4 FixedColor;

    //XMINT4 EnableShade;
    INT ShaderType;
    //XMINT4 NoAlpha;
    BOOL NoAlpha;

    //XMFLOAT4 ScreenSize;
    XMFLOAT2 ScreenSize;
    XMFLOAT4 CameraPos;

    UINT GameFrame;
    XMFLOAT3 DUMMY;

    XMFLOAT4 FREE;
};

//
// グローバル変数
//
static ID3D11SamplerState* _pRenderTextureSampler = NULL;
static ID3D11SamplerState* g_SamplerState = NULL;
static ID3D11SamplerState* g_SamplerStateFinal = NULL;

static ID3D11InputLayout* g_VertexLayout = NULL;
static ID3D11Buffer* g_ConstantBuffer = NULL;

//static ShaderDat g_ShaderMain = {};
//static ShaderDat g_ShaderBloom = {};

//static ShaderDat g_ShaderOutlineBuf = {};
//static ShaderDat g_ShaderOutline = {};

// 注意！初期化で外部から設定されるもの。Release不要。
static ID3D11Device* g_Device = NULL;
static ID3D11DeviceContext* g_DeviceContext = NULL;


static std::vector<ShaderDat> g_ShaderDat = {};

//
// シェーダ初期化
//
void DX_ShaderInitialize(ID3D11Device* device, ID3D11DeviceContext* device_context)
{
	HRESULT hr;

	// 取得するdeviceとdevice_contextのどちらかがNULLだったら正しく初期化できないので、エラー処理した方が良い
	if (!device || !device_context) {
		// MessageBoxなどでエラー処理すべし
		MessageBox(DXGetWindowHandle(), "deviceかdevice_contextがない", "kakunin", MB_OK);
	}

	// デバイスとデバイスコンテキストの保存
	g_Device = device;
	g_DeviceContext = device_context;

	// サンプラーステート設定
	//D3D11_SAMPLER_DESC samplerDesc = {};
	//samplerDesc.Filter = D3D11_FILTER_COMPARISON_MIN_MAG_MIP_POINT; //フィルタリング きれい
	////samplerDesc.Filter = D3D11_FILTER_COMPARISON_MIN_MAG_MIP_LINEAR; //フィルタリング 画像の解像度に関する
	//samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	//samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	//samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	//samplerDesc.MipLODBias = 0;
	//samplerDesc.MaxAnisotropy = 16;
	//samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	//samplerDesc.MinLOD = 0;
	//samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

	D3D11_SAMPLER_DESC samplerDesc = {};
    {
        D3D11_SAMPLER_DESC& sd = samplerDesc;
        //sd.Filter = D3D11_FILTER_COMPARISON_ANISOTROPIC;
        sd.Filter = D3D11_FILTER_COMPARISON_MIN_MAG_MIP_LINEAR;

        //sd.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
        //sd.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
        //sd.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
        sd.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
        sd.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
        sd.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
        sd.BorderColor[0] = 0.0f;
        sd.BorderColor[1] = 0.0f;
        sd.BorderColor[2] = 0.0f;
        sd.BorderColor[3] = 0.0f;
        sd.MipLODBias = 0;
        //sd.MaxAnisotropy = 16;
        sd.MaxAnisotropy = 8;
        //sd.MaxAnisotropy = 4;
        //sd.MaxAnisotropy = 1;
        sd.ComparisonFunc = D3D11_COMPARISON_NEVER;
        sd.MinLOD = 0;
        sd.MaxLOD = 1.0f; //最大ミップマップ数
    }


    D3D11_SAMPLER_DESC samplerDescFinal = {};
    {
        D3D11_SAMPLER_DESC& sd = samplerDescFinal;
        sd.Filter = D3D11_FILTER_COMPARISON_MIN_MAG_MIP_LINEAR;
        //sd.Filter = D3D11_FILTER_MIN_MAG_POINT_MIP_LINEAR;
        //sd.Filter = D3D11_FILTER_MIN_MAG_LINEAR_MIP_POINT;
        //sd.Filter = D3D11_FILTER_ANISOTROPIC;

        //sd.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
        //sd.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
        //sd.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
        sd.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
        sd.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
        sd.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
        
        sd.BorderColor[0] = 0.0f;
        sd.BorderColor[1] = 0.0f;
        sd.BorderColor[2] = 0.0f;
        sd.BorderColor[3] = 0.0f;
        sd.MipLODBias = 0;
        //sd.MaxAnisotropy = 16;
        sd.MaxAnisotropy = 8;
        //sd.MaxAnisotropy = 4;
        //sd.MaxAnisotropy = 1;
        sd.ComparisonFunc = D3D11_COMPARISON_NEVER;
        //sd.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
        sd.MinLOD = 0;
        //sd.MaxLOD = D3D11_FLOAT32_MAX;
        sd.MaxLOD = 1.0f; //最大ミップマップ数
    }
    D3D11_SAMPLER_DESC samplerDescLinear = {};
    {
        D3D11_SAMPLER_DESC& sd = samplerDescLinear;
        sd.Filter = D3D11_FILTER_MIN_MAG_POINT_MIP_LINEAR;

        sd.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
        sd.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
        sd.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
        sd.BorderColor[0] = 0.0f;
        sd.BorderColor[1] = 0.0f;
        sd.BorderColor[2] = 0.0f;
        sd.BorderColor[3] = 0.0f;
        sd.MipLODBias = 0;
        //sd.MaxAnisotropy = 16;
        sd.MaxAnisotropy = 4;
        sd.ComparisonFunc = D3D11_COMPARISON_NEVER;
        //sd.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
        sd.MinLOD = 0;
        //sd.MaxLOD = D3D11_FLOAT32_MAX;
        sd.MaxLOD = 1.0f; //最大ミップマップ数
    }

	//ID3D11SamplerState* samplerState = NULL;
	g_Device->CreateSamplerState(&samplerDesc, &g_SamplerState);
    //g_Device->CreateSamplerState(&samplerDesc, &_pRenderTextureSampler);
    g_Device->CreateSamplerState(&samplerDescLinear, &_pRenderTextureSampler);

    g_Device->CreateSamplerState(&samplerDescFinal, &g_SamplerStateFinal);
	


	// 入力レイアウト生成
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,		0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR",    0, DXGI_FORMAT_R32G32B32A32_FLOAT,	0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,			0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },

		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT,
			0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },

        // インスタンスデータ（位置オフセット）
        { "INSTANCE_POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,
        1, 0, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
	};
    //D3D11_INPUT_ELEMENT_DESC layout[] =
    //{
    //	{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,		0, offsetof(VertexLayout, Position), D3D11_INPUT_PER_VERTEX_DATA, 0 },
    //	{ "COLOR",    0, DXGI_FORMAT_R32G32B32A32_FLOAT,	0, offsetof(VertexLayout, Diffuse), D3D11_INPUT_PER_VERTEX_DATA, 0 },
    //	{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,			0, offsetof(VertexLayout, TexCoord), D3D11_INPUT_PER_VERTEX_DATA, 0 },

    //	{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT,
    //		0, offsetof(VertexLayout, Normal), D3D11_INPUT_PER_VERTEX_DATA, 0 }
    //};

	UINT numElements = ARRAYSIZE(layout);


	//---------------------------------------------------------
	// メインシェーダ
	//---------------------------------------------------------

	{
		//自動読み込み
		std::vector<FILE_AUTO_LOAD_DAT> files = FileScanDirectory(SHADER_FOLDER_PATH, "SHADER");

        bool firstSetInputLayout = false;

		//セット
		for (int i = 0; i < files.size(); i++)
		{
			ShaderDat dat = {};
			dat.name = files[i].name;

			{
                std::string rStr = files[i].fPath;
                rStr.erase(remove(rStr.begin(), rStr.end(), '\r'), rStr.end());
                rStr.erase(remove(rStr.begin(), rStr.end(), '\n'), rStr.end());

                {// シェーダファイル読み込み

                    FILE* fp = nullptr;
                    fopen_s(&fp, rStr.c_str(), "rb");

                    if (fp != nullptr)
                    {
                        char* datatmp;
                        long sizetmp;

                        fseek(fp, 0, SEEK_END);
                        sizetmp = ftell(fp);
                        fseek(fp, 0, SEEK_SET);

                        datatmp = new char[sizetmp];
                        fread_s(datatmp, sizetmp, sizetmp, 1, fp);

                        if (files[i].lPath[0] == 'g' && files[i].lPath[1] == 's')
                        {
                            g_Device->CreateGeometryShader(
                                datatmp, sizetmp,
                                NULL, &dat.GeometryShader);
                        }
                        if (files[i].lPath[0] == 'p' && files[i].lPath[1] == 's')
                        {
                            g_Device->CreatePixelShader(
                                datatmp, sizetmp,
                                NULL, &dat.PixelShader);
                        }
                        if (files[i].lPath[0] == 'v' && files[i].lPath[1] == 's')
                        {
                            g_Device->CreateVertexShader(
                                datatmp, sizetmp,
                                NULL, &dat.VertexShader);

                            if (!firstSetInputLayout)
                            {
                                firstSetInputLayout = true;

                                // 入力レイアウト生成
                                g_Device->CreateInputLayout(layout,
                                    numElements,
                                    datatmp,
                                    sizetmp,
                                    &g_VertexLayout);

                                // 入力レイアウト設定
                                g_DeviceContext->IASetInputLayout(g_VertexLayout);
                            }

                        }


                        fclose(fp);
                        delete[] datatmp;
                    }
                }

                //std::wstring rStr = {};
                //{
                //    std::string ostr = files[i].fPath;
                //    ostr.erase(remove(ostr.begin(), ostr.end(), '\r'), ostr.end());
                //    ostr.erase(remove(ostr.begin(), ostr.end(), '\n'), ostr.end());

                //    size_t sizetmp = ostr.size() + 1;

                //    wchar_t* wcFileName = new wchar_t[sizetmp];

                //    size_t convertedChars = 0;
                //    mbstowcs_s(&convertedChars, wcFileName, sizetmp, ostr.c_str(), _TRUNCATE);

                //    if (convertedChars > 0)
                //    {
                //        rStr = wcFileName;
                //    }

                //    delete[] wcFileName;
                //}
				//// 頂点シェーダコンパイル・生成
				//ID3DBlob* pErrorBlob;
				//ID3DBlob* pVSBlob = NULL;
				//hr = D3DCompileFromFile(rStr.c_str(), NULL, NULL, "VS", "vs_4_0", D3DCOMPILE_ENABLE_STRICTNESS, 0, &pVSBlob, &pErrorBlob);
				//if (FAILED(hr))
				//{
				//	MessageBox(NULL, (char*)pErrorBlob->GetBufferPointer(), "VS", MB_OK | MB_ICONERROR);
				//}

				//g_Device->CreateVertexShader(pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize(), NULL, &dat.VertexShader);


				//// 入力レイアウト生成
				//g_Device->CreateInputLayout(layout,
				//	numElements,
				//	pVSBlob->GetBufferPointer(),
				//	pVSBlob->GetBufferSize(),
				//	&g_VertexLayout);

				//pVSBlob->Release();

				//// 入力レイアウト設定
				//g_DeviceContext->IASetInputLayout(g_VertexLayout);

				//// ピクセルシェーダコンパイル・生成
				//ID3DBlob* pPSBlob = NULL;
				//hr = D3DCompileFromFile(rStr.c_str(), NULL, NULL, "PS", "ps_4_0", D3DCOMPILE_ENABLE_STRICTNESS, 0, &pPSBlob, &pErrorBlob);
				//if (FAILED(hr))
				//{
				//	MessageBox(NULL, (char*)pErrorBlob->GetBufferPointer(), "PS", MB_OK | MB_ICONERROR);
				//}

				//g_Device->CreatePixelShader(pPSBlob->GetBufferPointer(), pPSBlob->GetBufferSize(), NULL, &dat.PixelShader);

				//pPSBlob->Release();
			}

			g_ShaderDat.push_back(dat);
		}
	}


	// 定数バッファ生成
	D3D11_BUFFER_DESC hBufferDesc;
	hBufferDesc.ByteWidth = sizeof(ConstantBuffer); //コンスタントバッファの容量
	hBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	hBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	hBufferDesc.CPUAccessFlags = 0;
	hBufferDesc.MiscFlags = 0;
	hBufferDesc.StructureByteStride = sizeof(float);

	g_Device->CreateBuffer(&hBufferDesc, NULL, &g_ConstantBuffer);

	g_DeviceContext->VSSetConstantBuffers(0, 1, &g_ConstantBuffer); //Vertexシェーダで使えるように
    g_DeviceContext->PSSetConstantBuffers(0, 1, &g_ConstantBuffer); //Pixelシェーダで使えるように
    g_DeviceContext->GSSetConstantBuffers(0, 1, &g_ConstantBuffer); //Geometryシェーダで使えるように

}

//
// シェーダ終了
//
void DX_ShaderFinalize()
{
	for (int i = 0; i < g_ShaderDat.size(); i++)
	{
		SAFE_RELEASE(g_ShaderDat[i].PixelShader);
        SAFE_RELEASE(g_ShaderDat[i].VertexShader);
        SAFE_RELEASE(g_ShaderDat[i].GeometryShader);
	}
	//SAFE_RELEASE(g_ShaderOutlineBuf.PixelShader2);
	//SAFE_RELEASE(g_ShaderOutlineBuf.PixelShader);
	//SAFE_RELEASE(g_ShaderOutlineBuf.VertexShader);

	//SAFE_RELEASE(g_ShaderOutline.PixelShader);
	//SAFE_RELEASE(g_ShaderOutline.VertexShader);

	//SAFE_RELEASE(g_ShaderBloom.PixelShader);
	//SAFE_RELEASE(g_ShaderBloom.VertexShader);

	//SAFE_RELEASE(g_ShaderMain.PixelShader);
	//SAFE_RELEASE(g_ShaderMain.VertexShader);

	SAFE_RELEASE(g_ConstantBuffer);
	SAFE_RELEASE(g_VertexLayout);
    SAFE_RELEASE(g_SamplerState);
    SAFE_RELEASE(g_SamplerStateFinal);
	SAFE_RELEASE(_pRenderTextureSampler); 
}



void DX_SetSampler_Main()
{
	//ここを実行しなおすと切り替えられる
    g_DeviceContext->PSSetSamplers(0, 1, &g_SamplerState);
    

}
void DX_SetSampler_OffScreen()
{
	//ここを実行しなおすと切り替えられる
	g_DeviceContext->PSSetSamplers(0, 1, &_pRenderTextureSampler);
}

void DX_SetSampler_Final()
{
    g_DeviceContext->PSSetSamplers(0, 1, &g_SamplerStateFinal);
}


int DX_GetShaderIndex(std::string name)
{
	for (int i = 0; i < g_ShaderDat.size(); i++)
	{
		if (g_ShaderDat[i].name == name)
		{
			return i;
		}
	}
	return -1;
}

//void DX_SetShader(int index)
//{
//	// シェーダ設定
//	g_DeviceContext->VSSetShader(g_ShaderDat[index].VertexShader, NULL, 0);
//	g_DeviceContext->PSSetShader(g_ShaderDat[index].PixelShader, NULL, 0);
//}
//void DX_SetShader(std::string name)
//{
//	DX_SetShader(DX_GetShaderIndex(name));
//}
void DX_SetShaderPS(int index)
{
    // シェーダ設定
    g_DeviceContext->PSSetShader(g_ShaderDat[index].PixelShader, NULL, 0);
}
void DX_SetShaderPS(std::string name)
{
    DX_SetShaderPS(DX_GetShaderIndex(name));
}

void DX_SetShaderVS(int index)
{
    // シェーダ設定
    g_DeviceContext->VSSetShader(g_ShaderDat[index].VertexShader, NULL, 0);
}
void DX_SetShaderVS(std::string name)
{
    DX_SetShaderVS(DX_GetShaderIndex(name));
}
void DX_SetShaderGS(int index)
{
    if (index < 0)
    {
        // シェーダ設定
        g_DeviceContext->GSSetShader(NULL, NULL, 0);
    }
    else
    {
        // シェーダ設定
        g_DeviceContext->GSSetShader(g_ShaderDat[index].GeometryShader, NULL, 0);
    }
}
void DX_SetShaderGS(std::string name)
{
    DX_SetShaderGS(DX_GetShaderIndex(name));
}




void DX_ShaderVertexSetMatrixEx(const XMFLOAT4X4* matrix, SHADER_VERTEX_SET_MATRIX_EX_DAT dat)
{
	//float 4つずつしか送れないのは注意
	ConstantBuffer cb;
    cb.WorldViewProjection = *matrix;
    cb.World = dat.World;
    cb.View = dat.View;
    cb.Projection = dat.Projection;

	cb.Transition = { dat.Transition.x, dat.Transition.y, dat.Transition.z, 0.0f };
	cb.TexScale = { dat.TexScale.x, dat.TexScale.y, 0.0, 0.0 };
	cb.TexUV = { dat.TexUV.x, dat.TexUV.y, 0.0, 0.0 };
	cb.TexColor = { dat.TexColor.x, dat.TexColor.y, dat.TexColor.z, dat.TexColor.w };
	cb.FixedColor = { dat.FixedColor.x, dat.FixedColor.y, dat.FixedColor.z, dat.FixedColor.w };
    //cb.EnableShade = { (dat.EnableShade ? 1 : 0), 0, 0, 0 };
    //cb.ShaderType = dat.ShaderType;
    cb.ShaderType = 0;
    //cb.NoAlpha = { (dat.NoAlpha ? 1 : 0), 0, 0, 0 };
    cb.NoAlpha = dat.NoAlpha;

    //cb.ScreenSize = { dat.ScreenSize.x, dat.ScreenSize.y, 0.0, 0.0 };
    cb.ScreenSize = { dat.ScreenSize.x, dat.ScreenSize.y };
    cb.CameraPos = { dat.CameraPos.x, dat.CameraPos.y, dat.CameraPos.z, 0.0f };

    cb.GameFrame = dat.GameFrame;
    cb.DUMMY = {};
    cb.FREE = dat.Free;

	//コンスタントバッファに情報を送る
	g_DeviceContext->UpdateSubresource(g_ConstantBuffer, 0, NULL, &cb, 0, 0);
}

void DX_ShaderVertexSetMatrix(const XMFLOAT4X4* matrix)
{
	//static float a = 0.0f;
	//a++;

	//float 4つずつしか送れないのは注意
	ConstantBuffer cb;
	cb.WorldViewProjection = *matrix;
	cb.Transition = { 0.0f, 0.0f, 0.0f, 0.0f };

	//コンスタントバッファに情報を送る
	g_DeviceContext->UpdateSubresource(g_ConstantBuffer, 0, NULL, &cb, 0, 0);
}


void DX_ShaderReCompile()
{
#if defined(_DEBUG) || defined(DEBUG)
    for (int i = 0; i < g_ShaderDat.size(); i++)
    {
        SAFE_RELEASE(g_ShaderDat[i].PixelShader);
        SAFE_RELEASE(g_ShaderDat[i].VertexShader);
        SAFE_RELEASE(g_ShaderDat[i].GeometryShader);
    }
    g_ShaderDat.clear();

    {
        //自動読み込み
        std::vector<FILE_AUTO_LOAD_DAT> files
            = FileScanDirectory("source/Shader", "SHADER");

        //セット
        for (int i = 0; i < files.size(); i++)
        {
            ShaderDat dat = {};
            dat.name = files[i].name;

            bool isCompile = false;
            {
                std::string rStr = files[i].fPath;
                rStr.erase(remove(rStr.begin(), rStr.end(), '\r'), rStr.end());
                rStr.erase(remove(rStr.begin(), rStr.end(), '\n'), rStr.end());

                {// シェーダファイル読み込み

                    FILE* fp = nullptr;
                    fopen_s(&fp, rStr.c_str(), "rb");

                    if (fp != nullptr)
                    {
                        std::wstring rStr2 = {};
                        {
                            std::string ostr = files[i].fPath;
                            ostr.erase(remove(ostr.begin(), ostr.end(), '\r'), ostr.end());
                            ostr.erase(remove(ostr.begin(), ostr.end(), '\n'), ostr.end());

                            size_t sizetmp = ostr.size() + 1;

                            wchar_t* wcFileName = new wchar_t[sizetmp];

                            size_t convertedChars = 0;
                            mbstowcs_s(&convertedChars, wcFileName, sizetmp, ostr.c_str(), _TRUNCATE);

                            if (convertedChars > 0)
                            {
                                rStr2 = wcFileName;
                            }

                            delete[] wcFileName;
                        }

                        char* datatmp;
                        long sizetmp;

                        fseek(fp, 0, SEEK_END);
                        sizetmp = ftell(fp);
                        fseek(fp, 0, SEEK_SET);

                        datatmp = new char[sizetmp];
                        fread_s(datatmp, sizetmp, sizetmp, 1, fp);

                        if (files[i].lPath[0] == 'g' && files[i].lPath[1] == 's')
                        {
                            // Pixelシェーダコンパイル・生成
                            ID3DBlob* pErrorBlob;
                            ID3DBlob* pVSBlob = NULL;
                            HRESULT hr = D3DCompileFromFile(rStr2.c_str(), NULL, D3D_COMPILE_STANDARD_FILE_INCLUDE, "GS", "vs_5_0", D3DCOMPILE_ENABLE_STRICTNESS, 0, &pVSBlob, &pErrorBlob);
                            if (FAILED(hr))
                            {
                                MessageBox(NULL, (char*)pErrorBlob->GetBufferPointer(), "GS", MB_OK | MB_ICONERROR);
                            }

                            g_Device->CreateGeometryShader(
                                pVSBlob->GetBufferPointer(),
                                pVSBlob->GetBufferSize(),
                                NULL, &dat.GeometryShader);

                            pVSBlob->Release();

                            isCompile = true;
                        }
                        if (files[i].lPath[0] == 'p' && files[i].lPath[1] == 's')
                        {
                            // Pixelシェーダコンパイル・生成
                            ID3DBlob* pErrorBlob;
                            ID3DBlob* pVSBlob = NULL;
                            HRESULT hr = D3DCompileFromFile(rStr2.c_str(), NULL, D3D_COMPILE_STANDARD_FILE_INCLUDE, "PS", "vs_5_0", D3DCOMPILE_ENABLE_STRICTNESS, 0, &pVSBlob, &pErrorBlob);
                            if (FAILED(hr))
                            {
                                MessageBox(NULL, (char*)pErrorBlob->GetBufferPointer(), "PS", MB_OK | MB_ICONERROR);
                            }

                            g_Device->CreatePixelShader(
                                pVSBlob->GetBufferPointer(),
                                pVSBlob->GetBufferSize(),
                                NULL, &dat.PixelShader);

                            pVSBlob->Release();

                            isCompile = true;
                        }
                        if (files[i].lPath[0] == 'v' && files[i].lPath[1] == 's')
                        {
                            // 頂点シェーダコンパイル・生成
                            ID3DBlob* pErrorBlob;
                            ID3DBlob* pVSBlob = NULL;
                            HRESULT hr = D3DCompileFromFile(rStr2.c_str(), NULL, D3D_COMPILE_STANDARD_FILE_INCLUDE, "VS", "vs_5_0", D3DCOMPILE_ENABLE_STRICTNESS, 0, &pVSBlob, &pErrorBlob);
                            if (FAILED(hr))
                            {
                                MessageBox(NULL, (char*)pErrorBlob->GetBufferPointer(), "VS", MB_OK | MB_ICONERROR);
                            }

                            g_Device->CreateVertexShader(
                                pVSBlob->GetBufferPointer(),
                                pVSBlob->GetBufferSize(),
                                NULL, &dat.VertexShader);

                            pVSBlob->Release();

                            isCompile = true;
                        }


                        fclose(fp);
                        delete[] datatmp;
                    }
                }
            }

            if (isCompile)
            {
                g_ShaderDat.push_back(dat);
            }
        }
    }
#endif
}