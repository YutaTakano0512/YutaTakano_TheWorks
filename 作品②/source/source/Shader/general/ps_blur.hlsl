#include "common.hlsl"

//=============================================================================
// ピクセルシェーダ
//=============================================================================
void main(in PS_IN In, out float4 outDiffuse : SV_Target)
{
    //float2 center = float2(0.5, 0.5); // 画面の中心
    //float dist = length(In.TexCoord - center); // 中心からの距離
    //float blurAmount = dist * 1.0; // 距離に応じたブラー強度

    //float4 color = 0;
    //float totalWeight = 0;
    
    //for (int i = -1; i <= 1; i++)
    //{
    //    for (int j = -1; j <= 1; j++)
    //    {
    //        float2 offset = float2(i, j) * blurAmount * 0.01;
    //        float weight = exp(-dot(offset, offset) * 5.0);
    //        color += g_Texture.Sample(g_SamplerState, In.TexCoord + offset) * weight;
    //        totalWeight += weight;
    //    }
    //}
    
    //outDiffuse = color / totalWeight * TexColor;
    //outDiffuse.w = 1.0f;
    
    ////outDiffuse.rgb += -float3(1.0f, 1.0f, 1.0f) * (1.0f - TexColor.a);
    //outDiffuse.rgb *= TexColor.a;
    
    outDiffuse = Begin(In);
    
    float2 uv = In.TexCoord;
    // 手動でUVループを処理
    // 整数部分を取り除く
    uv = frac(uv);
    float4 px = g_Texture.Sample(g_SamplerState, uv) * TexColor;
    
    // 近傍ピクセルを取得してブレンド
    float blurAmount = 4.0f; // 距離に応じたブラー強度
    float4 colorLeft = g_Texture.Sample(g_SamplerState, clamp(uv + float2(-1.0f / ScreenSize.x, 0.0) * blurAmount, 0.0f, 0.999)) * TexColor;
    float4 colorRight = g_Texture.Sample(g_SamplerState, clamp(uv + float2(1.0f / ScreenSize.x, 0.0) * blurAmount, 0.0f, 0.999)) * TexColor;
    float4 colorUp = g_Texture.Sample(g_SamplerState, clamp(uv + float2(0.0, -1.0f / ScreenSize.y) * blurAmount, 0.0f, 0.999)) * TexColor;
    float4 colorDown = g_Texture.Sample(g_SamplerState, clamp(uv + float2(0.0, 1.0f / ScreenSize.y) * blurAmount, 0.0f, 0.999)) * TexColor;

    // 平均化してぼかし効果を適用
    px = (px + colorLeft + colorRight + colorUp + colorDown) / 5.0;
    
    outDiffuse = px;
    outDiffuse.rgb *= TexColor.a;
    outDiffuse.a = 1.0f;
}