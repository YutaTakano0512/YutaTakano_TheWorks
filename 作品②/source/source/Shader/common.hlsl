
#define __hlsl__
#include "commonBindings.h"
#include "commonConstantBuffer.hlsl"

// 頂点シェーダー入力されるデータを構造体の形で表現
// これは頂点バッファの内容そのもの

struct VS_IN
{
    float4 Position : POSITION0;
    float4 Diffuse : COLOR0;
    float2 TexCoord : TEXCOORD0;
    float4 Normal : NORMAL0;
};

// ピクセルシェーダーへ入力されるデータを構造体の形で表現
struct PS_IN
{
    float4 Position : SV_POSITION;
    float4 Diffuse : COLOR;
    float2 TexCoord : TEXCOORD0;
    float4 Normal : NORMAL0;
    
    uint instanceID : SV_InstanceID;
};

Texture2D g_Texture : register(t0);
SamplerState g_SamplerState : register(s0);

#define EPSILON 0.0001f


float3 ScreenFilter_Bloom(PS_IN In)
{
    float3 bloomColor = float3(0, 0, 0);
    //float kernel[5] = { 0.05, 0.1, 0.2, 0.1, 0.05 }; // ぼかしカーネル
    float kernel[7] = { 0.02, 0.05, 0.1, 0.2, 0.1, 0.05, 0.02 }; // より広範囲のぼかし
    //float2 texOffset = 1.0f / ScreenSize; // ピクセルオフセット
    float2 texOffset = 2.0f / ScreenSize; // 拡張したピクセルオフセット

    //for (int i = -2; i <= 2; i++)
    //{
    //    float3 color = g_Texture.Sample(g_SamplerState, In.TexCoord + float2(i, 0) * texOffset).rgb;

    //    // 明るい部分のみ抽出
    //    float brightness = dot(color, float3(0.299, 0.587, 0.114)); // 輝度計算
    //    float threshold = 0.6; // 明るさの閾値
    //    float3 filteredColor = brightness > threshold ? color : float3(0, 0, 0);

    //    bloomColor += filteredColor * kernel[i + 2];
    //}
    for (int i = -3; i <= 3; i++)
    {
        float3 colorX = g_Texture.Sample(g_SamplerState, In.TexCoord + float2(i, 0) * texOffset).rgb;
        float3 colorY = g_Texture.Sample(g_SamplerState, In.TexCoord + float2(0, i) * texOffset).rgb;
    
    // 輝度を計算
        float brightnessX = dot(colorX, float3(0.299, 0.587, 0.114));
        float brightnessY = dot(colorY, float3(0.299, 0.587, 0.114));

    // 閾値を設定
        float threshold = 0.6;

    // 閾値に近いほど強く加算（明るさに応じたスケール）
        float weightX = smoothstep(threshold - 0.2, threshold, brightnessX);
        float weightY = smoothstep(threshold - 0.2, threshold, brightnessY);

        bloomColor += colorX * kernel[i + 3] * weightX;
        bloomColor += colorY * kernel[i + 3] * weightY;
    }
    
    return bloomColor;

}
