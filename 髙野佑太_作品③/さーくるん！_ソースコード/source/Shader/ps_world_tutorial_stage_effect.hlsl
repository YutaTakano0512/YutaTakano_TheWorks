
#include "common.hlsl"


float rand(float2 co)
{
    return frac(sin(dot(co, float2(12.9898, 78.233))) * 43758.5453);
}

float isLine(float2 uv, float2 p1, float2 p2, float width)
{
    //float2 dir = normalize(p2 - p1);
    //float2 perpDir = float2(-dir.y, dir.x); // 垂直方向
    //float dist = abs(dot(uv - p1, perpDir)); // 線からの距離
    //return step(dist, width); // 線の幅を明確に判定
    float2 dir = normalize(p2 - p1);
    float dist = length(p2 - p1); // 線からの距離
    for (float t = 0.0; t <= 1.0; t += 0.005) // 線分上の点をサンプリング
    {
        float2 pointOnLine = p1 + t * dir * dist; // 線分上の点
        if (length(uv - pointOnLine) < width) // 線の幅を考慮
        {
            return 1.0; // 線上にある
        }
    }
    return 0.0; // 線上にない
}


float4 isInsideHexagon(float2 uv, float2 center, float size, float time, float id, float index)
{
    int numRand = (int) (rand(id + index) * 6) + 1; // 確実に整数化

    float timetmp = GameFrame * (1.2f + 0.15 * (id + index)) / 1000.0f;
    
    float4 result1 = float4(0.0f, 0.0f, 0.0f, 0.0f);
    float4 result2 = float4(0.0f, 0.0f, 0.0f, 0.0f);
    for (int i = 0; i < numRand; i++)
    {
        float2 pos = float2(
            sin(2.0f * 3.1415 / numRand * i + timetmp),
            cos(2.0f * 3.1415 / numRand * i + timetmp)
        ) * 0.5f * size + center;

        float2 pos2 = float2(
            sin(2.0f * 3.1415 / numRand * ((i + 1) % numRand) + timetmp), // 最後の頂点と最初の頂点を接続
            cos(2.0f * 3.1415 / numRand * ((i + 1) % numRand) + timetmp)
        ) * 0.5f * size + center;



        float2 edge = pos;
        float miwidth = 0.09f; // ワイヤーフレームの線幅
        
        if (length(uv - edge) < miwidth)// ワイヤーフレームの線幅
        {
            float glowIntensity = 0.5 + 0.5 * sin(time * 0.12 + id * 0.8)
                * (0.7f + sin(id * 7.3) * 0.3f); // 光のゆらぎ

            // UV座標を使用して、発光の中心からの距離を取得
            float distancemod = ((0.55f + sin(id * 1.3) * 0.45f)) * miwidth;
    
            float distance = length(uv - edge) * distancemod; // テクスチャ中央からの距離

            // 発光の強度（中心ほど明るく、外側ほど暗くなる）
                //float glowFactor = exp(-distance * 8.0) * glowIntensity; // ぼやけた光
            float glowFactor = max(1.0 - min(distance / (miwidth * distancemod), 1.0), 0.01f); // ぼやけた光

            // 発光色（ホタルらしい黄色から白の色合い）
            float3 glowColor = glowFactor;

            // 加算ブレンディング用の出力
            result1 = float4(glowColor, glowFactor)
                 * glowIntensity - (0.001 * index);
    
        }
        
        {
            float width = 0.004; // 線の幅
            float lineFactor = isLine(uv, pos, pos2, width);
            
            //result2 += float4(lineFactor, lineFactor, lineFactor, 1.0) * 0.5
            //    * (1.0f - lineFactor) * (1.0f - result2.a);
            
            if (lineFactor > 0.01)
                result2 = float4(lineFactor, lineFactor, lineFactor, 1.0) * 0.5;
        }
    }
    
    return result1 + result2;
}




//=============================================================================
// ピクセルシェーダ
//=============================================================================
void main(in PS_IN In, out float4 outDiffuse : SV_Target)
{
    int id = In.instanceID / 3;
    int index = In.instanceID % 3;
    
    //float time = frac(GameFrame * 0.01f);
    //float time = index * 0.016 + GameFrame * (0.003f + 0.001 * In.instanceID);
    float time = 0;
    
    //float2 center = float2(rand(float2(id, time)) * 0.8, rand(float2(id + 1, time)) * 0.8);
    float2 center = float2(0.5, 0.5);
    float size = (0.2 + rand(float2(id + index, time)) * 0.5);
    //float size = 1.0;
    
    float3 randcolor = float3(
        1.0 * rand(float2(id + index, time)),
        1.0 * rand(float2(id + index + 1, time)),
        1.0 * rand(float2(id + index + 2, time))
    );

    float4 isHex = isInsideHexagon(In.TexCoord, center, size,  time, id, index);
    float4 color = isHex.a > 0.01 ?
        (TexColor + float4(randcolor, 0.0)) * isHex
        : float4(0.0, 0.0, 0.0, 0.0)
    ;

    outDiffuse = color;

    clip(outDiffuse.w - 0.01);
}