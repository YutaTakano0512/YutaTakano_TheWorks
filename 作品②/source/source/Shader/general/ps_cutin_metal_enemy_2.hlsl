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
	
    outDiffuse.xyz += 2.65f;
    //outDiffuse = Filter_MetalEnemy(In, px, outDiffuse,
    //    float3(0.8, 0.6, 0.75),
    //    0.16,
    //    float3(0.4, 0.4, 0.6),
    //    float3(0.95, 0.95, 0.75)
    //);
    outDiffuse = Filter_MetalEnemy(In, px, outDiffuse,
        float3(0.8, 0.6, 0.75),
        //0.12,
        0.05,
        //float3(0.30, 0.30, 0.5),
        //float3(0.28, 0.28, 0.35),
        float3(0.2, 0.2, 0.3),
        float3(1.25, 1.25, 1.00)
    );
    outDiffuse = Filter_CutIn(In, px, outDiffuse,
        //float3(-0.35f, -0.25f, -0.20f),
        //float3(-0.26f, -0.16f, -0.05f),
        float3(-0.03f, -0.11f, -0.01f),
        //float3(0.3f, 0.2f, 0.4f)
        float3(0.45f, 0.75f, 0.4f)
    );
    //outDiffuse.xyz += 0.08f;
    //outDiffuse.xyz += 0.12f;
    //outDiffuse.xyz += 0.12f;
    outDiffuse.xyz += 0.16f;
    //outDiffuse.x += 0.15f;
    //outDiffuse.y += 0.15f;
    //outDiffuse.z += 0.1f;
    //outDiffuse.rgb *= 1.1;
    
    
	////アルファテスト
 //   clip(outDiffuse.w - 0.01);
	
 //   if (NoAlpha == 1)
 //   {
 //       outDiffuse.w = 1.0f;
 //   }
    outDiffuse = End(outDiffuse);
}
