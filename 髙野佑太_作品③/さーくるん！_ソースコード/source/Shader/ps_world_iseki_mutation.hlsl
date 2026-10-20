#include "common.hlsl"

// 疑似ランダム関数
float random(float2 seed)
{
    return frac(sin(dot(seed, float2(12.9898, 78.233))) * 43758.5453);
}

// ホタルの座標を生成（画面サイズに基づく）
float2 getFireflyPosition(uint id)
{
    float2 seed = float2(id * 0.73, id * 1.37);
    
        // ランダムな3D位置を生成
    float3 fireflyPos = float3(
        (random(seed) - 0.5) * 10.0,  // -5.0～5.0 の範囲でX座標生成
        (random(seed + float2(0.2, 0.5)) - 0.5) * 6.0,  // Y座標
        (random(seed * 1.5) - 0.5) * 8.0   // Z座標（奥行き）
    );

    // カメラ方向への影響を加味
    float3 relativePos = fireflyPos - CameraPos.xyz / 100.0f;
    


    // 疑似3Dの座標を2Dスクリーン座標に変換
    float2 projectedPos = float2(
        ScreenSize.x * (0.5 + relativePos.x / (relativePos.z + 5.0)), // カメラの視点方向に合わせる
        ScreenSize.y * (0.5 + relativePos.y / (relativePos.z + 5.0))
    );

    return projectedPos;
}

// ホタルの光の強度を決定
float applyGlowEffect(float2 pixelPos, float2 fireflyPos, float time)
{
    float distance = length(pixelPos - fireflyPos);
    float flicker = 0.5 + 0.5 * sin(time * 2.0 + fireflyPos.x * 0.1);
    return exp(-distance * 0.08) * flicker;
}



//#define COLORBORDERMULMOD 0.2
//#define COLORBORDERADDMOD 0.08
#define COLORBORDERMULMOD 0.1
#define COLORBORDERADDMOD 0.05
#define COLORBORDER 0.5
//=============================================================================
// ピクセルシェーダ
//=============================================================================
void main(in PS_IN In, out float4 outDiffuse : SV_Target)
{
    float4 px = g_Texture.Sample(g_SamplerState, In.TexCoord + float2(TexUV.x, TexUV.y)) * TexColor;
	
    float dayMod = Free.w <= 0.0f ? 0.0f : (pow(clamp(
    (GameFrame - Free.x) / (60.0f * Free.w),
    0.0f, 1.0f) * Free.y, Free.z));
    
    outDiffuse = px;
    
    {
        float2 _Distortion = 0.003; // 歪みの強さ
        float2 _ChromaticOffset = 0.007; // 色収差の最大強さ
        
// 画面中心からの距離を計算
        float2 center = float2(0.5, 0.4);
        float2 offset = (In.TexCoord + float2(TexUV.x, TexUV.y)) - center;
        float dist = length(offset);

// 歪みを適用
        float2 distortedUV = (In.TexCoord + float2(TexUV.x, TexUV.y)) + offset * (_Distortion * dist * dist);

// 色収差の強さを距離に応じて調整
        float2 chromaticEffect = _ChromaticOffset * smoothstep(0.0, 0.5, pow(dist, 1.15f)); // 中央では弱く、外側で強く

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
            outDiffuse.rgb *= 1.0f;
        }

    }
    {
		 // ランバート拡散証明モデル
        In.Normal.xyz = normalize(In.Normal.xyz); // 正規化(長さ1にする)
		
		{
            float3 lightDirection = normalize(float3(0.0, 0.0, -1.0));
		
            //float3 lightColor = float3(0.9, 0.6, 0.85); // ライトの色設定
            //float3 ambientColor = float3(0.75, 0.4, 0.5); // 環境光の色設定
    
            float3 lightColor = float3(0.75, 0.5, 0.7); // ライトの色設定
            float3 ambientColor = float3(0.3, 0.2, 0.2); // 環境光の色設定
    
            lightColor = lerp(lightColor, float3(0.9, 0.6, 0.75), dayMod);
            ambientColor = lerp(ambientColor, float3(0.4, 0.2, 0.3), dayMod);
            
            float3 light = -dot(In.Normal.xyz, lightDirection);
            light = saturate(light) * lightColor + ambientColor; // ライトの色処理

    
            outDiffuse.rgb *= light;
            outDiffuse.rgb *= 1.45f;
        }
    }
    
    outDiffuse.rgb += ScreenFilter_Bloom(In) * 0.46;
    outDiffuse.g += -0.08f;
    
    //clip(outDiffuse.w - 0.01);
	
    outDiffuse.w = 1.0f;
}
