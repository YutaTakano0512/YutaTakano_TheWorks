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
	
    outDiffuse = Begin(In);
	
    outDiffuse = Filter_MetalEnemy(In, px, outDiffuse,
        float3(0.0, 0.0, 0.0),
        0.8,
        float3(0.3, 0.3, 0.5),
        float3(0.4, 0.4, 0.3)
    );
    
    outDiffuse = End(outDiffuse);
}
