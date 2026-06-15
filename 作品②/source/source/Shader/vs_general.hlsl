//#include "common.hlsl"

////=============================================================================
//// 頂点シェーダ
////=============================================================================
#include "common.hlsl"

//=============================================================================
// 頂点シェーダ
//=============================================================================
void main(in float4 inPosition : POSITION0,
		in float4 inDiffuse : COLOR0,
		in float2 inTexCoord : TEXCOORD0,
        in float4 inNormal : NORMAL0,

        uint instanceID : SV_InstanceID,
        out PS_IN Out
)
{
    Out.Position = mul(inPosition + Transition, WorldViewProjection);
	
    Out.Diffuse = inDiffuse * TexColor;
	//outDiffuse = inDiffuse;
    //Out.TexCoord = inTexCoord * float2(TexScale.x, TexScale.y) + float2(TexUV.x, TexUV.y);
    //Out.TexCoord = clamp(inTexCoord * float2(TexScale.x, TexScale.y) + float2(TexUV.x, TexUV.y), 0.0, 1.0);
    //Out.TexCoord = clamp(inTexCoord, 0.001, 0.999) * float2(TexScale.x, TexScale.y) + float2(TexUV.x, TexUV.y);
    
    //float2 texSize;
    //g_Texture.GetDimensions(texSize.x, texSize.y);
    //float2 tcepsilo = 1.0f / ScreenSize;
    float2 tc = inTexCoord;
    //tc.x = tc.x - floor(tc.x); // X方向のUVをループさせる
    //tc.y = tc.y - floor(tc.y); // Y方向のUVをループさせる
    Out.TexCoord = tc * float2(TexScale.x, TexScale.y) + float2(TexUV.x, TexUV.y);
    Out.instanceID = instanceID;

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
