#include "common.hlsl"


//=============================================================================
// ピクセルシェーダ
//=============================================================================
void main(in float4 inPosition : POSITION0,
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
        outDiffuse.x = 0.0f;
        outDiffuse.y = 0.0f;
        outDiffuse.z = 1.0f;
        outDiffuse.w = 0.3f;
    }
    else
    {
        outDiffuse.w = 0.0f;
        clip(outDiffuse.w - 0.01);
    }
}
