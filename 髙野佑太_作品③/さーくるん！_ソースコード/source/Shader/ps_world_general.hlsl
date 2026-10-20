#include "common.hlsl"

//#define COLORBORDERMULMOD 0.2
//#define COLORBORDERADDMOD 0.08
#define COLORBORDERMULMOD 0.002
#define COLORBORDERADDMOD 0.06
#define COLORBORDER 0.5
//=============================================================================
// ピクセルシェーダ
//=============================================================================
void main(in PS_IN In, out float4 outDiffuse : SV_Target)
{
    float4 px = g_Texture.Sample(g_SamplerState, In.TexCoord + float2(TexUV.x, TexUV.y)) * TexColor;

    outDiffuse = px;
	
    {
        float2 _Distortion = 0.003; // 歪みの強さ
        float2 _ChromaticOffset = 0.005; // 色収差の最大強さ

// 画面中心からの距離を計算
        float2 center = float2(0.5, 0.4);
        float2 offset = (In.TexCoord + float2(TexUV.x, TexUV.y)) - center;
        float dist = length(offset);

// 歪みを適用
        float2 distortedUV = (In.TexCoord + float2(TexUV.x, TexUV.y)) + offset * (_Distortion * dist * dist);

// 色収差の強さを距離に応じて調整
        float2 chromaticEffect = _ChromaticOffset * smoothstep(0.0, 0.5, pow(dist, 1.5f)); // 中央では弱く、外側で強く

// 色収差を適用するUV座標を計算
        float2 safeOffsetR = distortedUV + float2(chromaticEffect.x, 0);
        float2 safeOffsetB = distortedUV - float2(chromaticEffect.x, 0);

// UV座標が範囲外なら黒を返す
        if (any(safeOffsetR < float2(0.0, 0.0)) || any(safeOffsetR > float2(1.0, 1.0)) ||
    any(safeOffsetB < float2(0.0, 0.0)) || any(safeOffsetB > float2(1.0, 1.0)) ||
    any(distortedUV < float2(0.0, 0.0)) || any(distortedUV > float2(1.0, 1.0)))
        {
            outDiffuse = float4(0.1, 0.1, 0.1, 1); // 黒で塗りつぶす
        }
        else
        {
            float4 colR = g_Texture.Sample(g_SamplerState, safeOffsetR) * TexColor;
            float4 colG = g_Texture.Sample(g_SamplerState, distortedUV) * TexColor;
            float4 colB = g_Texture.Sample(g_SamplerState, safeOffsetB) * TexColor;

            outDiffuse = float4(colR.r, colG.g, colB.b, 1.0);
            outDiffuse.rgb *= 1.25f;
        }

    }
    outDiffuse.rgb += ScreenFilter_Bloom(In) * 0.15;
    
    //clip(outDiffuse.w - 0.01);
	
    outDiffuse.w = 1.0f;
}
