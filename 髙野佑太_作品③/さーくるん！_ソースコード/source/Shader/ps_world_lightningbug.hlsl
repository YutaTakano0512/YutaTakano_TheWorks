
#include "common.hlsl"

//float GetGlowFactor(float time, uint instanceID)
//{
//    return 0.5 + 0.5 * sin(time * 0.016 + instanceID * 0.3); // 発光のゆらぎ
//}

//=============================================================================
// ピクセルシェーダ
//=============================================================================
void main(in PS_IN In, out float4 outDiffuse : SV_Target)
{
    int id = In.instanceID / 5;
    int index = In.instanceID % 5;
    
    float time = GameFrame * 0.1f; // 時間変化のスケール
    float glowIntensity = 0.5 + 0.5 * sin(time * 0.12 + id * 0.8)
     * (0.7f + sin(id * 7.3) * 0.3f); // 光のゆらぎ

    // UV座標を使用して、発光の中心からの距離を取得
    float2 uv = In.TexCoord;
    float distancemod = (0.55f + sin(id * 1.3) * 0.45f) 
    * (1.0 - (0.6f + 0.02f * index) );
    
    float distance = length(uv - float2(0.5, 0.5)) * distancemod; // テクスチャ中央からの距離

    // 発光の強度（中心ほど明るく、外側ほど暗くなる）
    //float glowFactor = exp(-distance * 8.0) * glowIntensity; // ぼやけた光
    float glowFactor = max(1.0 - min(distance / (0.45 * distancemod), 1.0),  0.01f); // ぼやけた光

    // 発光色（ホタルらしい黄色から白の色合い）
    float3 glowColor = TexColor.rgb * In.Diffuse.rgb * glowFactor;

    // 加算ブレンディング用の出力
    outDiffuse = float4(glowColor, glowFactor) 
     * TexColor.a * In.Diffuse.a * glowIntensity - (0.12 * index);
    
    //float4 px = g_Texture.Sample(g_SamplerState, In.TexCoord);
    
    //outDiffuse = px * TexColor;
    clip(outDiffuse.w - 0.01);
}