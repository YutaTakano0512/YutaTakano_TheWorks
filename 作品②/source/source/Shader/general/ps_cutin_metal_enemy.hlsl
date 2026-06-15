#include "common.hlsl"

//=============================================================================
// ピクセルシェーダ
//=============================================================================

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
    //    float3(0.3, 0.8, 0.3),
    //    0.16,
    //    float3(0.4, 0.4, 0.6),
    //    float3(0.95, 0.95, 0.75)
    //);
    //outDiffuse = Filter_CutIn(In, px, outDiffuse,
    //    float3(-0.40f, -0.50f, -0.10f),
    //    float3(0.4f, 0.1f, 0.2f)
    //);
    outDiffuse = Filter_MetalEnemy(In, px, outDiffuse,
        float3(0.3, 0.8, 0.3),
        //0.12,
        0.06,
        float3(0.24, 0.24, 0.42),
        float3(0.95, 0.95, 0.75)
    );
    outDiffuse = Filter_CutIn(In, px, outDiffuse,
        //float3(-0.20f, -0.30f, -0.10f),
        //float3(0.4f, 0.1f, 0.2f)

        float3(-0.35f, -0.50f, -0.01f),
        float3(0.25f, 0.6f, 0.15f)
    );
    //outDiffuse.xyz += 0.1f;
    outDiffuse.xyz += 0.14f;
    //outDiffuse.rgb *= 1.1;
    
	////アルファテスト
 //   clip(outDiffuse.w - 0.01);
	
 //   if (NoAlpha == 1)
 //   {
 //       outDiffuse.w = 1.0f;
 //   }
    outDiffuse = End(outDiffuse);
}
