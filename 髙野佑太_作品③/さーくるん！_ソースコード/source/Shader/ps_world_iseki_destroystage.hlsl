
#include "common.hlsl"

// 疑似ランダム関数
float random(float2 seed)
{
    return frac(sin(dot(seed, float2(12.9898, 78.233))) * 43758.5453);
}






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
	
    float day1Mod = clamp((GameFrame - Free.x) / (60.0f * ISEKI_DAY_CHANGE_TIME), 0.0f, 1.0f) * Free.y;
    float day2Mod = clamp((GameFrame - Free.z) / (60.0f * ISEKI_DAY_CHANGE_TIME), 0.0f, 1.0f) * Free.w;
	
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
    {
		 // ランバート拡散証明モデル
        In.Normal.xyz = normalize(In.Normal.xyz); // 正規化(長さ1にする)
		
		{
            float3 lightDirection = normalize(float3(0.0, 0.0, -1.0));
		
            //float3 lightColor = float3(0.64, 0.64, 0.60); // ライトの色設定
            //float3 ambientColor = float3(0.55, 0.56, 0.56); // 環境光の色設定
            
            float3 lightColor = float3(0.6, 0.55, 0.5); // ライトの色設定
            float3 ambientColor = float3(0.4, 0.3, 0.3); // 環境光の色設定
    
            lightColor = lerp(lightColor, float3(0.9, 0.42, 0.14), day1Mod);
            ambientColor = lerp(ambientColor, float3(0.64, 0.36, 0.14), day1Mod);
            
            lightColor = lerp(lightColor, float3(0.49, 0.5, 0.62), day2Mod);
            ambientColor = lerp(ambientColor, float3(0.19, 0.16, 0.24), day2Mod);
            
    
            float3 light = -dot(In.Normal.xyz, lightDirection);
            light = saturate(light) * lightColor + ambientColor; // ライトの色処理

    
            outDiffuse.rgb *= light;
            outDiffuse.rgb *= 1.2f;
        }
    }
    
//    {
//        uint fireflyCount = 20; // ホタルの最大数
//        float intensity = 0.0;
    
//        float2 pixelPos = In.TexCoord * ScreenSize * 0.75f;
    
//        for (uint i = 0; i < fireflyCount; i++)
//        {
//            float2 fireflyPos = getFireflyPosition(i);
//            intensity += applyGlowEffect(pixelPos, fireflyPos, (float)GameFrame * 0.002);
//        }
    
//        intensity = min(intensity, 1.0); // 最大輝度を調整
        
//        // ピクセルの明るさを計算 (標準的な輝度計算)
//        float luminance = dot(px.rgb, float3(0.2126, 0.7152, 0.0722));
        
//        // 画面中心の座標 (標準化されたUV空間で 0.5, 0.5)
//        float2 center = float2(0.5, 0.5);

//// 輝度が一定値以下の場合のみ加算 (step関数を使用)
//        float factor = step(luminance, 0.5); // 0.4 以下なら 1.0、それ以外は 0.0
//        // 現在のピクセル座標と中心との距離を計算
//        float distanceFromCenter = length(In.TexCoord - center);
        
//// 減衰係数 (距離が大きいほど光が強く、小さいほど弱くする)
//        float fadeFactor = pow(smoothstep(0.0, 0.9, distanceFromCenter), 1.8f);



//        outDiffuse += float4(1.0, 1.0, 0.5, 1.0f) * intensity * 0.8f * factor * fadeFactor;
//    }
    //clip(outDiffuse.w - 0.01);
	
    outDiffuse.rgb += ScreenFilter_Bloom(In) * 0.4;
    //outDiffuse.rgb += float3(0.05f, 0.03f, 0.02f);
    outDiffuse.rgb += float3(-0.01f, -0.02f, -0.05f);
    
    outDiffuse.w = 1.0f;
}
