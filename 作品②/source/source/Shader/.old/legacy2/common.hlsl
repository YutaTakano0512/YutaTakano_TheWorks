
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

Texture2D g_Texture : register(t0);
SamplerState g_SamplerState : register(s0);
