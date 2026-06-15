
#include "common.hlsl"

float FBM(float2 uv)
{
    float value = 0.0;
    float amplitude = 0.5;
    float frequency = 1.0;

    for (int i = 0; i < 4; i++)
    {
        value += amplitude * sin(uv.x * frequency) * cos(uv.y * frequency);
        frequency *= 3.4;
        amplitude *= 0.2;
    }

    return value;
}




float CheckerPattern(float2 uv, float dis, float alpha)
{
    //float scale = lerp(15, 15.0, alpha); // alphaが小さいほどスケールを大きく
    //float2 scaledUV = uv * scale;

    //return fmod(floor(scaledUV.x) + floor(scaledUV.y), 2.0) == 0 ? 0.0 : 1.0; // 交互に透明度を設定
    float scale = lerp(1, 8.0, pow(1.0f - alpha, 2.0f)); // alphaが小さいほどスケールを大きく

    float mod = min(max(dis, 1), 20);
    // チェック模様の判定
    int checkX = (int) (uv.x / mod) % scale;
    int checkY = (int) (uv.y / mod) % scale;
    
    return checkX == 0 && checkY == 0;
}

float SoftEdge(float2 uv, float4 pos, int i)
{
    return pow(lerp(1.0, 0.0, min(length(uv - float2(0.5, 0.5))
    / ((0.5 - 0.1 * i) * (1.0f + sin(uv.x + uv.y + pos.x + pos.y + pos.z + i) * 0.4))
    , 1.0f)), 0.5f); // 中心からの距離で透明度を調整
}


//=============================================================================
// ピクセルシェーダ
//=============================================================================
void main(in PS_IN In, out float4 outDiffuse : SV_Target)
{
    int id = In.instanceID / 3;
    
    int index = In.instanceID % 3;
    
    float2 uv = In.TexCoord;
    // 手動でUVループを処理
    // 整数部分を取り除く
    uv = frac(uv);
    float4 px = g_Texture.Sample(g_SamplerState, uv) * TexColor * In.Diffuse;
    
    outDiffuse = px;
    
    //float randValue = frac(sin(dot(uv, float2(12.9898, 78.233))) * 43758.5453);
    ////float randValue = fmod(floor(uv.x * 100.0) + floor(uv.y * 100.0), 2.0) / 100;
    
    ////float rvmod = 1.0f;
    ////float rvx = fmod(uv.x * 1000.0f, rvmod) / rvmod / 1000.0f;
    ////float rvy = fmod(uv.y * 1000.0f, rvmod) / rvmod / 1000.0f;
    ////float randValue = (rvx + rvy) / 2;
    
    ////float randValue = frac(sin(dot(uv, float2(6.9898, 32.233))) * 43758.5453);
    ////float randValue = frac(sin(dot(fmod(In.Position.xy * 0.000001, 1.0f), float2(12.9898, 78.233))) * 43758.5453);

    //// 透明度に応じてドットを減らす（アルファ値が低いほど打つ）
    //float threshold = 1.0 - outDiffuse.a; // 透明度が高いほど閾値が小さくなる
    //outDiffuse.a = randValue > threshold;

    //float baseAlpha = px.a * TexColor.a * In.Diffuse.a 
    //* SoftEdge(In.TexCoord, In.Position, index) * (sin(In.instanceID * 0.17) * 0.2 + 1.0);
    //float alpha = CheckerPattern(
    //    In.Position.xy,
    //    1,
    //    baseAlpha
    //);

    //outDiffuse = baseAlpha <= EPSILON ? 0 : float4(px.rgb, alpha) * float4(In.Diffuse.rgb, 1.0f); // チェック柄の透明度を適用
    
    clip(outDiffuse.a - 0.01);

}