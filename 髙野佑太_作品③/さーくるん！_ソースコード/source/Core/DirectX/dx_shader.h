//---------------------------------------------------------------------------
// dx_shader.h
// シェーダーの利用
// sougo hara  2024/05/14
//---------------------------------------------------------------------------
#ifndef _DX_SHADER_H_
#define _DX_SHADER_H_

#include <d3d11.h>
#include <DirectXMath.h>
using namespace DirectX;


#define SHADER_FOLDER_PATH ("rom/shader")
#define SHADER_COMMON_PATH (L"rom/shader/common.hlsl")

//#define SHADER_PATH (L"rom/shader/shader.hlsl")
//#define SHADER_PATH_BLOOM (L"rom/shader/shader_bloom.hlsl")
//
//#define SHADER_PATH_OUTLINE_BUF (L"rom/shader/shader_outlineBuf.hlsl")
//#define SHADER_PATH_OUTLINE		(L"rom/shader/shader_outline.hlsl")

//#define SHADER_PATH_TEST (L"rom/shader/shader_test.hlsl")
//
//#define SHADER_PATH (L"rom/shader/shader.hlsl")
//#define SHADER_PATH_BLOOM (SHADER_PATH)
//
//#define SHADER_PATH_OUTLINE_BUF (SHADER_PATH)
//#define SHADER_PATH_OUTLINE		(SHADER_PATH)
//
//enum DX_SHADER_TYPE : char {
//    DPD_SHADER_TYPE_NONE = 0,
//
//    DPD_SHADER_TYPE_METAL,
//    DPD_SHADER_TYPE_METAL_ENEMY,
//    DPD_SHADER_TYPE_METAL_ENEMY_2,
//    DPD_SHADER_TYPE_CUTIN,
//
//    DPD_SHADER_TYPE_EFFECT_DIS,
//    DPD_SHADER_TYPE_EFFECT_WARP,
//    DPD_SHADER_TYPE_EFFECT_FRESNEL,
//
//    DPD_SHADER_TYPE_ALPHA_DISSOLVE,
//
//    DPD_SHADER_TYPE_METAL_ENEMY_DARK,
//    DPD_SHADER_TYPE_EFFECT,
//
//    DPD_SHADER_TYPE_MAX
//};

//現在未使用
//enum DX_SHADER_TYPE : char {
//    DPD_SHADER_TYPE_NONE = 0
//};

//
// 構造体宣言
//
struct SHADER_VERTEX_SET_MATRIX_EX_DAT {
    XMMATRIX World;
    XMMATRIX View;
    XMMATRIX Projection;

	XMFLOAT3 Transition;
	XMFLOAT2 TexScale;
	XMFLOAT2 TexUV;
	XMFLOAT4 TexColor;

	XMFLOAT4 FixedColor;

	//bool EnableShade;
    //DX_SHADER_TYPE ShaderType;
	bool NoAlpha;

	XMFLOAT2 ScreenSize;
    XMFLOAT3 CameraPos;
    unsigned int GameFrame;

    XMFLOAT4 Free;
};

void DX_ShaderInitialize(ID3D11Device* device, ID3D11DeviceContext* device_context);
void DX_ShaderFinalize(void);

void DX_ShaderVertexSetMatrixEx(const XMFLOAT4X4* matrix, SHADER_VERTEX_SET_MATRIX_EX_DAT dat);
void DX_ShaderVertexSetMatrix(const XMFLOAT4X4* matrix);

void DX_SetSampler_Main();
void DX_SetSampler_OffScreen();
void DX_SetSampler_Final();


int DX_GetShaderIndex(std::string name);
//void DX_SetShader(std::string name);
//void DX_SetShader(int index);
void DX_SetShaderPS(std::string name);
void DX_SetShaderPS(int index);
void DX_SetShaderVS(std::string name);
void DX_SetShaderVS(int index);
void DX_SetShaderGS(std::string name);
void DX_SetShaderGS(int index);

void DX_ShaderReCompile();

#endif // _SHADER_H_