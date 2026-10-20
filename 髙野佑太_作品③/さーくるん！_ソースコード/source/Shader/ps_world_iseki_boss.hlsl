#include "common.hlsl"


#define COLORBORDERMULMOD 0.2
#define COLORBORDERADDMOD 0.1
#define COLORBORDER 0.5
//=============================================================================
// ピクセルシェーダ
//=============================================================================
void main(in PS_IN In, out float4 outDiffuse : SV_Target)
{
    float4 px = g_Texture.Sample(g_SamplerState, In.TexCoord + float2(TexUV.x, TexUV.y)) * TexColor;
	
    outDiffuse = px;
    {
		 // ランバート拡散証明モデル
        In.Normal.xyz = normalize(In.Normal.xyz); // 正規化(長さ1にする)
		
		{
            float3 lightDirection = normalize(float3(0.0, 0.0, -1.0));
		
            float3 lightColor = float3(-0.5, -0.5, -0.4); // ライトの色設定
            float3 ambientColor = float3(1.0, 1.0, 1.0); // 環境光の色設定
    
            float3 light = -dot(In.Normal.xyz, lightDirection);
            light = saturate(light) * lightColor + ambientColor; // ライトの色処理

    
            outDiffuse.rgb *= light;
        }
    }
	{
        outDiffuse.x += (outDiffuse.x - COLORBORDER) * COLORBORDERMULMOD + COLORBORDERADDMOD;
        outDiffuse.y += (outDiffuse.y - COLORBORDER) * COLORBORDERMULMOD + COLORBORDERADDMOD;
        outDiffuse.z += (outDiffuse.z - COLORBORDER) * COLORBORDERMULMOD + COLORBORDERADDMOD;
    }
	
    {
        float2 _Distortion = 0.1; // 歪みの強さ
        float2 _ChromaticOffset = 0.005; // 色収差の強さ
        
// 画面中心からの距離を計算
        float2 center = float2(0.5, 0.4);
        float2 offset = (In.TexCoord + float2(TexUV.x, TexUV.y)) - center;
        float dist = length(offset);

// 歪みを適用
        float2 distortedUV = (In.TexCoord + float2(TexUV.x, TexUV.y)) + offset * (_Distortion * dist * dist);

// 色収差の強さを距離に応じて調整
        float2 chromaticEffect = _ChromaticOffset * smoothstep(0.0, 0.5, pow(dist, 0.8f)); // 中央では弱く、外側で強く

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
            outDiffuse.rgb *= 0.95f;
        }

    }
    //clip(outDiffuse.w - 0.01);
	
    outDiffuse.w = 1.0f;
	
 }
