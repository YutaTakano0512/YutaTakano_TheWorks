#include "common.hlsl"

//=============================================================================
// ピクセルシェーダ
//=============================================================================
void main(in PS_IN In, out float4 outDiffuse : SV_Target)
{
    float2 uv = In.TexCoord;
    float4 px = g_Texture.Sample(g_SamplerState, uv) * TexColor;
    
    // FixedColorがすべて非負かどうかを判定（非負なら1.0、それ以外は0.0）
    float isFixedColorValid = step(EPSILON, px.w) * step(0, FixedColor.x) * step(0, FixedColor.y) * step(0, FixedColor.z) * step(0, FixedColor.w);
    
    // `lerp()` を活用して条件分岐を削減
    outDiffuse = lerp(px, FixedColor, isFixedColorValid);
    
    outDiffuse = End(outDiffuse);
}
