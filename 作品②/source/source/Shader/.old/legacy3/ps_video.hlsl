#include "common.hlsl"

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
void main(in PS_IN In, out float4 outDiffuse : SV_Target)
{
    
    float y = g_Texture.Sample(g_SamplerState, float2(In.TexCoord.x, In.TexCoord.y * 0.5)).r;
    float u = g_Texture.Sample(g_SamplerState, float2(In.TexCoord.x * 0.5, 0.50 + In.TexCoord.y * 0.25)).r;
    float v = g_Texture.Sample(g_SamplerState, float2(In.TexCoord.x * 0.5, 0.75 + In.TexCoord.y * 0.25)).r;
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