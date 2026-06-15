
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
    outTexCoord = inTexCoord * float2(TexScale.x, TexScale.y);
	
    outNormal = inNormal;
}

Texture2D g_Texture : register(t0);
SamplerState g_SamplerState : register(s0);

//=============================================================================
// ピクセルシェーダ
//=============================================================================
void PS(in float4 inPosition : POSITION0,
		 in float4 inDiffuse : COLOR0,
		 in float2 inTexCoord : TEXCOORD0,
         in float4 inNormal : NORMAL0,
		 out float4 outDiffuse : SV_Target)
{
    float2 coord = inTexCoord + float2(TexUV.x, TexUV.y);
    coord.x = min(max(coord.x, 0.0), 1.0);
    coord.y = min(max(coord.y, 0.0), 1.0);
    float4 px = g_Texture.Sample(g_SamplerState, coord) * TexColor;
    //float4 px = g_Texture.Sample(g_SamplerState, inTexCoord + float2(TexUV.x, TexUV.y)) * TexColor;
    
    if (px.w > 0)
    {
        //outDiffuse.rgba = float4(1.0f, 1.0f, 1.0f, 1.0f);
        outDiffuse = px;
        //outDiffuse.x = 1.0f;
        //outDiffuse.y = 0.0f;
        //outDiffuse.z = 0.0f;
        //outDiffuse.w = 1.0f;
        if (FixedColor.x < 0 && FixedColor.y < 0 && FixedColor.z < 0 && FixedColor.w < 0)
        {
            outDiffuse.x = px.x;
            outDiffuse.y = px.y;
            outDiffuse.z = px.z;
            outDiffuse.x += (1.0f - (outDiffuse.x - 0.5f)) * -0.25f;
            outDiffuse.y += (1.0f - (outDiffuse.y - 0.5f)) * -0.25f;
            outDiffuse.z += (1.0f - (outDiffuse.z - 0.5f)) * -0.25f;
            
            if (outDiffuse.x <= 0.1 && outDiffuse.y <= 0.1 && outDiffuse.z <= 0.1)
            {
                outDiffuse.x = 0.6f;
                outDiffuse.y = 0.6f;
                outDiffuse.z = 0.6f;
            }
        }
        else
        {
            outDiffuse.x = FixedColor.x;
            outDiffuse.y = FixedColor.y;
            outDiffuse.z = FixedColor.z;
        }
        outDiffuse.w = 1.0f;
    }
    else
    {
        outDiffuse.x = 0.0f;
        outDiffuse.y = 0.0f;
        outDiffuse.z = 0.0f;
        outDiffuse.w = 0.0f;
        clip(outDiffuse.w - 0.01);
        //outDiffuse.x = 1.0f;
        //outDiffuse.y = 1.0f;
        //outDiffuse.z = 1.0f;
        //outDiffuse.w = 1.0f;
    }
}
