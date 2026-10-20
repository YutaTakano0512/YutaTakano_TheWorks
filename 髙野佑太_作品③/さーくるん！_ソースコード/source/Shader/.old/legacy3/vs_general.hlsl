//#include "common.hlsl"

////=============================================================================
//// 頂点シェーダ
////=============================================================================
//void main(in VS_IN In, out PS_IN Out)
//{
//    Out.Position = mul(In.Position + Transition, WorldViewProjection);
	
//    Out.Diffuse = In.Diffuse * TexColor;
//	//outDiffuse = inDiffuse;
//    Out.TexCoord = In.TexCoord * float2(TexScale.x, TexScale.y) + float2(TexUV.x, TexUV.y);
    
//    //float2 texc = inTexCoord;
//    //texc.x *= TexScale.x;
//    //texc.y *= TexScale.y;
    
//    //outTexCoord = texc + float2(TexUV.x, TexUV.y);
//    //float2 texc = inTexCoord;
//    //float texScaX = 1.0f / TexScale.x;
//    //float texScaY = 1.0f / TexScale.y;
    
//    //outTexCoord = inTexCoord + float2(TexUV.x / TexScale.x, TexUV.y / TexScale.y);
	
//    Out.Normal = In.Normal;
//}
#include "common.hlsl"

//=============================================================================
// 頂点シェーダ
//=============================================================================
void main(in float4 inPosition : POSITION0,
		in float4 inDiffuse : COLOR0,
		in float2 inTexCoord : TEXCOORD0,
        in float4 inNormal : NORMAL0,
        out PS_IN Out
)
{
    Out.Position = mul(inPosition + Transition, WorldViewProjection);
	
    Out.Diffuse = inDiffuse * TexColor;
	//outDiffuse = inDiffuse;
    Out.TexCoord = inTexCoord * float2(TexScale.x, TexScale.y) + float2(TexUV.x, TexUV.y);
    
    //float2 texc = inTexCoord;
    //texc.x *= TexScale.x;
    //texc.y *= TexScale.y;
    
    //outTexCoord = texc + float2(TexUV.x, TexUV.y);
    //float2 texc = inTexCoord;
    //float texScaX = 1.0f / TexScale.x;
    //float texScaY = 1.0f / TexScale.y;
    
    //outTexCoord = inTexCoord + float2(TexUV.x / TexScale.x, TexUV.y / TexScale.y);
	
    Out.Normal = inNormal;
}
