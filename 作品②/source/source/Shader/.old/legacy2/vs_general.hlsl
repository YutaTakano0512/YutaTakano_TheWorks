#include "common.hlsl"

//=============================================================================
// 頂点シェーダ
//=============================================================================
void main(in  float4 inPosition	: POSITION0,
		in  float4 inDiffuse	: COLOR0,
		out float4 outPosition	: SV_POSITION,
		out float4 outDiffuse	: COLOR,
		in float2 inTexCoord : TEXCOORD0,
        in float4 inNormal : NORMAL0,
		out float2 outTexCoord : TEXCOORD00,
		out float4 outNormal : NORMAL0)
{
	outPosition = mul( inPosition + Transition, WorldViewProjection );
	
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
