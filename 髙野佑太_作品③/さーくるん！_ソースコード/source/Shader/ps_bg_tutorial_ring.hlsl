#include "common.hlsl"

float rand(float2 co)
{
    return frac(sin(dot(co, float2(12.9898, 78.233))) * 43758.5453);
}

float gridPattern(float2 uv, float time)
{
    float gridSize = 20.0; // グリッドの密度
    float2 gridUV = uv * gridSize;
    float2 gridLines = abs(frac(gridUV) - 0.5) * 2.0;

    float glow = exp(-10.0 * min(gridLines.x, gridLines.y)); // グリッドの発光
    glow *= 0.5 + 0.5 * sin(time * 2.0 + uv.x * 60.0); // 時間変化による発光のゆらぎ

    return glow;
}
float blockNoise(float2 uv, float scale)
{
    return rand(floor(uv * scale)); // UV座標を整数化してランダム値を適用
}



//=============================================================================
// ピクセルシェーダ
//=============================================================================
void main(in PS_IN In, out float4 outDiffuse : SV_Target)
{
    //float time = frac(GameFrame / 100 * 0.5);
    float time = (float)GameFrame / 30;
    float2 uv = float2(
        In.TexCoord.x * 3,
        In.TexCoord.y * 3
    );
    float glowFactor = gridPattern(uv, time);
    float noiseFactor = blockNoise(uv, 20.0); // ブロックサイズを調整

    float4 color = float4(1.0,1.0,1.0, 1.0)
    * glowFactor * noiseFactor
    ; // 発光
    outDiffuse = color * TexColor;

    clip(outDiffuse.w - 0.01);
}
