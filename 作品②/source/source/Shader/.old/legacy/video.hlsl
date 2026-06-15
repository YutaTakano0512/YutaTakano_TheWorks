
//*****************************************************************************
// 定数バッファ
//*****************************************************************************
cbuffer ConstantBuffer : register(b0)
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


//=============================================================================
// 頂点シェーダ
//=============================================================================
void VS(in float4 inPosition : POSITION0,
		in float4 inDiffuse : COLOR0,
		out float4 outPosition : SV_POSITION,
		out float4 outDiffuse : COLOR,
		in float2 inTexCoord : TEXCOORD0,
        in float4 inNormal : NORMAL0,
		out float2 outTexCoord : TEXCOORD00,
		out float4 outNormal : NORMAL0)
{
    outPosition = mul(inPosition + Transition, WorldViewProjection);
	
    outDiffuse = inDiffuse * TexColor;
	//outDiffuse = inDiffuse;
    outTexCoord = inTexCoord * float2(TexScale.x, TexScale.y) + float2(TexUV.x, TexUV.y);
    
    //float2 texc = inTexCoord;
    //texc.x *= TexScale.x;
    //texc.y *= TexScale.y;
    
    //outTexCoord = texc + float2(TexUV.x, TexUV.y);
    //float2 texc = inTexCoord;
    //float texScaX = 1.0f / TexScale.x;
    //float texScaY = 1.0f / TexScale.y;
    
    //outTexCoord = inTexCoord + float2(TexUV.x / TexScale.x, TexUV.y / TexScale.y);
	
    outNormal = inNormal;
}


Texture2D g_Texture : register(t0);
SamplerState g_SamplerState : register(s0);

float3 YUVToRGB(float3 yuv)
{
  // BT.601 coefs
    static const float3 yuvCoef_r = { 1.164f, 0.000f, 1.596f };
    static const float3 yuvCoef_g = { 1.164f, -0.392f, -0.813f };
    static const float3 yuvCoef_b = { 1.164f, 2.017f, 0.000f };
    yuv -= float3(0.0625f, 0.5f, 0.5f);
    return saturate(float3(
    dot(yuv, yuvCoef_r),
    dot(yuv, yuvCoef_g),
    dot(yuv, yuvCoef_b)
    ));
}

//=============================================================================
// ピクセルシェーダ
//=============================================================================
void PS(in float4 inPosition : POSITION0,
		 in float4 inDiffuse : COLOR0,
		 in float2 inTexCoord : TEXCOORD0,
         in float4 inNormal : NORMAL0,
		 out float4 outDiffuse : SV_Target)
{
    
    float y = g_Texture.Sample(g_SamplerState, float2(inTexCoord.x, inTexCoord.y * 0.5)).r;
    float u = g_Texture.Sample(g_SamplerState, float2(inTexCoord.x * 0.5, 0.50 + inTexCoord.y * 0.25)).r;
    float v = g_Texture.Sample(g_SamplerState, float2(inTexCoord.x * 0.5, 0.75 + inTexCoord.y * 0.25)).r;
    outDiffuse = float4(YUVToRGB(float3(y, u, v)), 1.f);
}

////--------------------------------------------------------------------------------------
//cbuffer ConstantBuffer : register(b0)
//{
//    //matrix World;
//    //matrix View;
//    //matrix Projection;
    
//	//唯一CPU側から書き込める場所
//    matrix WorldViewProjection;
//    matrix World;
//    matrix View;
//    matrix Projection;
    
//    float4 Transition;
//    float4 TexScale;
//    float4 TexUV;
//    float4 TexColor;
//    float4 FixedColor;
//    //int4 EnableShade;
//    int ShaderType;
//    //int4 NoAlpha;
//    int NoAlpha;
    
//    //float4 ScreenSize;
//    float2 ScreenSize;
//    float4 CameraPos;
//}

////--------------------------------------------------------------------------------------
//struct VS_INPUT
//{
//    float4 Pos : POSITION;
//    float2 Tex : TEXCOORD0;
//    float4 Color : COLOR;
//    float4 Normal : NORMAL0;
//};

//struct v2f
//{
//    float4 Pos : SV_POSITION;
//    float2 Tex : TEXCOORD0;
//    float4 Color : COLOR;
//    float4 Normal : NORMAL0;
//};

////--------------------------------------------------------------------------------------
//Texture2D    txYUV        : register(t0);
//SamplerState samLinear    : register(s0);

////--------------------------------------------------------------------------------------
//// Nothing special here
//v2f VS(VS_INPUT input) {
//  v2f output;
//  output.Pos = mul(input.Pos, World);
//  output.Pos = mul(output.Pos, View);
//  output.Pos = mul(output.Pos, Projection);
//  output.Tex = input.Tex;
//  output.Color = input.Color;
//  output.Normal = input.Normal;
//  return output;
//}

////--------------------------------------------------------------------------------------
//// Pixel Shader
////--------------------------------------------------------------------------------------

//float3 YUVToRGB(float3 yuv) {
//  // BT.601 coefs
//  static const float3 yuvCoef_r = { 1.164f, 0.000f, 1.596f };
//  static const float3 yuvCoef_g = { 1.164f, -0.392f, -0.813f };
//  static const float3 yuvCoef_b = { 1.164f, 2.017f, 0.000f };
//  yuv -= float3(0.0625f, 0.5f, 0.5f);
//  return saturate(float3( 
//    dot( yuv, yuvCoef_r ),
//    dot( yuv, yuvCoef_g ),
//    dot( yuv, yuvCoef_b )
//    ));
//}

//float4 PS(v2f input) : SV_Target {
//  float y = txYUV.Sample(samLinear, float2(input.Tex.x, input.Tex.y * 0.5)).r;
//  float u = txYUV.Sample(samLinear, float2(input.Tex.x * 0.5, 0.50 + input.Tex.y * 0.25)).r;
//  float v = txYUV.Sample(samLinear, float2(input.Tex.x * 0.5, 0.75 + input.Tex.y * 0.25)).r;
//  return float4(YUVToRGB(float3(y,u,v)), 1.f);
//}