//#include "common.hlsl"

////=============================================================================
//// 頂点シェーダ
////=============================================================================
//void main(in VS_IN In, out PS_IN Out)
//{
//    Out.Position = mul(In.Position + Transition, WorldViewProjection);
	
//    Out.Diffuse = In.Diffuse * TexColor;
//	//outDiffuse = inDiffuse;
//    Out.TexCoord = In.TexCoord * float2(TexScale.x, TexScale.y);
	
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

        uint instanceID : SV_InstanceID,
        out PS_IN Out
)
{
    Out.Position = mul(inPosition + Transition, WorldViewProjection);
	
    Out.Diffuse = inDiffuse * TexColor;
	//outDiffuse = inDiffuse;
    //Out.TexCoord = inTexCoord * float2(TexScale.x, TexScale.y);
    //Out.TexCoord = clamp(inTexCoord * float2(TexScale.x, TexScale.y), 0.01, 0.99);
    //Out.TexCoord = clamp(inTexCoord, 0.001, 0.999) * float2(TexScale.x, TexScale.y);
	
    float2 tc = inTexCoord;
    //tc.x = tc.x - floor(tc.x); // X方向のUVをループさせる
    //tc.y = tc.y - floor(tc.y); // Y方向のUVをループさせる
    Out.TexCoord = tc * float2(TexScale.x, TexScale.y);
    Out.instanceID = instanceID;


    
    Out.Normal = inNormal;
}
