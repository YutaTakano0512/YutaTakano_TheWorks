#include "common.hlsl"

//=============================================================================
// ピクセルシェーダ
//=============================================================================
//void main(in float4 inPosition : POSITION0,
//		 in float4 inDiffuse : COLOR0,
//		 in float2 inTexCoord : TEXCOORD0,
//         in float4 inNormal : NORMAL0,
//		 out float4 outDiffuse : SV_Target)

void main(in PS_IN In, out float4 outDiffuse : SV_Target)
{
    float2 uv = In.TexCoord;
    // 手動でUVループを処理
    // 整数部分を取り除く
    uv = frac(uv);
    float4 px = g_Texture.Sample(g_SamplerState, uv) * TexColor;
	
  //  if (px.w > 0
		//&& FixedColor.x >= 0 && FixedColor.y >= 0 && FixedColor.z >= 0 && FixedColor.w >= 0)
  //  {
  //      outDiffuse = float4(FixedColor.x, FixedColor.y, FixedColor.z, FixedColor.w);
		////outDiffuse = float4(0.0f, 0.0f, 0.0f, 1.0f);
  //  }
  //  else
  //  {
  //      outDiffuse = px;
  //  }
    outDiffuse = Begin(In);
	
    outDiffuse = Filter_MetalEnemy(In, px, outDiffuse,
        float3(0.8, 0.6, 0.75),
        0.4,
        float3(0.35, 0.35, 0.55),
        float3(0.65, 0.65, 0.5)
    );
    
    
	////アルファテスト
 //   clip(outDiffuse.w - 0.01);
	
 //   if (NoAlpha == 1)
 //   {
 //       outDiffuse.w = 1.0f;
 //   }
    outDiffuse = End(outDiffuse);
}
