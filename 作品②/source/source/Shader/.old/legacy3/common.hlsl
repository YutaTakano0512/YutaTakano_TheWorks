
//*****************************************************************************
// 定数バッファ
//*****************************************************************************
cbuffer ConstantBuffer : register( b0 )
{
	//唯一CPU側から書き込める場所
    matrix WorldViewProjection;
    matrix World;
    matrix View;
    matrix Projection;
    
	float4 Transition;
	float4 TexScale;
	float4 TexUV;
    float4 TexColor;
    float4 FixedColor;
    //int4 EnableShade;
    int ShaderType;
    //int4 NoAlpha;
    int NoAlpha;
    
    //float4 ScreenSize;
    float2 ScreenSize;
    float4 CameraPos;
}

// 頂点シェーダー入力されるデータを構造体の形で表現
// これは頂点バッファの内容そのもの

struct VS_IN
{
    float4 Position : POSITION0;
    float4 Diffuse : COLOR0;
    float2 TexCoord : TEXCOORD0;
    float4 Normal : NORMAL0;
};

// ピクセルシェーダーへ入力されるデータを構造体の形で表現
struct PS_IN
{
    float4 Position : SV_POSITION;
    float4 Diffuse : COLOR;
    float2 TexCoord : TEXCOORD0;
    float4 Normal : NORMAL0;
};

Texture2D g_Texture : register(t0);
SamplerState g_SamplerState : register(s0);

#define EPSILON 0.0001f