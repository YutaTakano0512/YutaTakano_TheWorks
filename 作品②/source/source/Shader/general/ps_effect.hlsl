#include "common.hlsl"

//=============================================================================
// ピクセルシェーダ
//=============================================================================
void main(in PS_IN In, out float4 outDiffuse : SV_Target)
{
    outDiffuse =  Begin(In);
    
    float2 uv = In.TexCoord;
    // 手動でUVループを処理
    // 整数部分を取り除く
    uv = frac(uv);
    float4 px = g_Texture.Sample(g_SamplerState, uv) * TexColor;
    // 近傍ピクセルを取得してブレンド
    float blurAmount = 0.5f; // 距離に応じたブラー強度
    float4 colorLeft = g_Texture.Sample(g_SamplerState, clamp(uv + float2(-1.0f / ScreenSize.x, 0.0) * blurAmount, 0.0f, 0.999)) * TexColor;
    float4 colorRight = g_Texture.Sample(g_SamplerState, clamp(uv + float2(1.0f / ScreenSize.x, 0.0) * blurAmount, 0.0f, 0.999)) * TexColor;
    float4 colorUp = g_Texture.Sample(g_SamplerState, clamp(uv + float2(0.0, -1.0f / ScreenSize.y) * blurAmount, 0.0f, 0.999)) * TexColor;
    float4 colorDown = g_Texture.Sample(g_SamplerState, clamp(uv + float2(0.0, 1.0f / ScreenSize.y) * blurAmount, 0.0f, 0.999)) * TexColor;

    // 平均化してぼかし効果を適用
    px = (px + colorLeft + colorRight + colorUp + colorDown) / 5.0;
    
    //outDiffuse.rgb = px.rgb * 1.5;
    outDiffuse.rgb = px.rgb * 1.0;
    
    //float4 color = px;
    //float alpha = color.a; // アルファ値を取得

    //    // 境界部分をぼかすための補間
    //float blurFactor = smoothstep(0.0, 1.0, alpha); // アルファ値に基づく補間
    //color.rgb *= blurFactor; // 色をアルファ値に応じて調整

        
        
    //float luminance = dot(color.rgb, float3(0.1, 0.1, 0.1)); // 輝度を計算
    //    // にじみの強度を計算
    //float smudgeFactor = saturate((luminance - 0.5) * 2.0); // 閾値を調整

    //    // 周囲のピクセルをサンプリングして混ぜる
    //float2 texOffset = float2(1.0 / ScreenSize.x, 1.0 / ScreenSize.y);
    //float4 blurredColor = float4(0, 0, 0, 0);
    //blurredColor += g_Texture.Sample(g_SamplerState, uv + texOffset * float2(1, 0));
    //blurredColor += g_Texture.Sample(g_SamplerState, uv - texOffset * float2(1, 0));
    //blurredColor += g_Texture.Sample(g_SamplerState, uv + texOffset * float2(0, 1));
    //blurredColor += g_Texture.Sample(g_SamplerState, uv - texOffset * float2(0, 1));

    //blurredColor /= 4.0; // 平均化

    //    // 元の色とにじみ色を補間
    //color.rgb = lerp(color.rgb, blurredColor.rgb, smudgeFactor);
    
    //outDiffuse.rgb = color.rgb * 1.5;
    
    outDiffuse = End(outDiffuse);
 }
