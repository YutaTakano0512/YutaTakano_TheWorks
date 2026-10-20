#include "common.hlsl"

float rand(float2 co)
{
    return frac(sin(dot(co, float2(1092.9898, 78.233))) * 43758.5453);
}

float2 glitchOffset(float2 uv, float time)
{
    float glitchStrength = 0.02;
    float noise = rand(float2(uv.y * 10.0, time));
    return uv + float2(noise * glitchStrength, 0);
}


//=============================================================================
// ピクセルシェーダ
//=============================================================================
void main(in PS_IN In, out float4 outDiffuse : SV_Target)
{
    float2 uv = In.TexCoord * 2.0;
    uv.x += (float) GameFrame / 1500.0f;
    uv.y += (float) GameFrame / 2000.0f;
    uv.xy = fmod(uv.xy, 1.0f);
    
    float4 px = g_Texture.Sample(g_SamplerState, uv);
    
    float4 color = px;
    //color.rgb += (sin((float) GameFrame / 20) + 1.0f) / 2 * 2.0f;
    //color.rgb += sin((float) GameFrame / 20) * 0.3f - 0.5;
    color.rgb += (float) GameFrame / 500;
    color.rgb += (color.rgb - 0.5) * 0.5f;
    //color.rgb = saturate(color.rgb);
    color.rgb = fmod(color.rgb, 1.0f);
    
    outDiffuse = lerp(
        0,
        px * TexColor,
        saturate(length(color.rgb) / 1.0)
    );
    outDiffuse.a = color.a > 0.01;
    outDiffuse.rgb *= TexColor.a;
    //outDiffuse = px;
    
    {
        float time = frac(GameFrame * 0.05);
        float2 distortedUV = glitchOffset(uv, time);
        float4 color = g_Texture.Sample(g_SamplerState, distortedUV);

    // RGBチャンネルをずらしてグリッチ効果を強調
        float4 glitchColor;
        glitchColor.r = g_Texture.Sample(g_SamplerState, distortedUV + float2(0.005, 0)).r;
        glitchColor.g = g_Texture.Sample(g_SamplerState, distortedUV).g;
        glitchColor.b = g_Texture.Sample(g_SamplerState, distortedUV - float2(0.005, 0)).b;
        glitchColor.a = color.a;
        
        outDiffuse += float4(0.5f, 0.5, 0.6f, 1.0f) * glitchColor * 0.55;
    }
    
    clip(outDiffuse.w - 0.01);

}
