
#include "common.hlsl"

float FBM(float2 uv)
{
    float value = 0.0;
    float amplitude = 0.5;
    float frequency = 1.0;

    for (int i = 0; i < 4; i++)
    {
        value += amplitude * sin(uv.x * frequency) * cos(uv.y * frequency);
        frequency *= 1.2;
        amplitude *= 0.3;
    }

    return value;
}




float CheckerPattern(float2 uv, float alpha)
{
    //float scale = lerp(15, 15.0, alpha); // alphaが小さいほどスケールを大きく
    //float2 scaledUV = uv * scale;

    //return fmod(floor(scaledUV.x) + floor(scaledUV.y), 2.0) == 0 ? 0.0 : 1.0; // 交互に透明度を設定
    float scale = lerp(1, 150.0, 1.0 - alpha); // alphaが小さいほどスケールを大きく

    // チェック模様の判定
    int checkX = (int) (uv.x / 10) % scale;
    int checkY = (int) (uv.y / 10) % scale;
    int checker = (checkX + checkY) % scale;
    
    return checker == 0;
}


float SoftEdge(float2 uv)
{
    float edgeFactor = smoothstep(0.0, 1.0, length(uv - float2(0.5, 0.5))); // 中心からの距離で透明度を調整
    return edgeFactor;
}


//=============================================================================
// ピクセルシェーダ
//=============================================================================
void main(in PS_IN In, out float4 outDiffuse : SV_Target)
{
    int id = In.instanceID / 3;
    
    int index = In.instanceID % 3;
    
    
    float2 uv = In.TexCoord * 6.0;
    float noise = FBM(uv);
    float alpha = CheckerPattern(In.Position.xy, noise * TexColor.a * In.Diffuse.a);
    ///float alpha = noise * TexColor.a * In.Diffuse.a;

    outDiffuse = float4(1, 1, 1, alpha) * float4(TexColor.rgb, 1.0f); // チェック柄の透明度を適用
    clip(outDiffuse.a - 0.01);

}