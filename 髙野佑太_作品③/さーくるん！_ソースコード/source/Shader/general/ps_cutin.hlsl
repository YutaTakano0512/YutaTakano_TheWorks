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
		 
    if (px.w > 0
		&& FixedColor.x >= 0 && FixedColor.y >= 0 && FixedColor.z >= 0 && FixedColor.w >= 0)
    {
        outDiffuse = float4(FixedColor.x, FixedColor.y, FixedColor.z, FixedColor.w);
		//outDiffuse = float4(0.0f, 0.0f, 0.0f, 1.0f);
    }
    else
    {
        outDiffuse = px;
    }
    
    outDiffuse = Filter_CutIn(In, px, outDiffuse,
        float3(-0.40f, -0.50f, -0.10f),
        float3(0.4f, 0.1f, 0.2f)
    );
    
	//アルファテスト
    clip(outDiffuse.w - 0.01);
	
    if (NoAlpha == 1)
    {
        outDiffuse.w = 1.0f;
    }
}
