#include "common.hlsl"

float rand(float2 co)
{
    return frac(sin(dot(co, float2(12.9898, 78.233))) * 43758.5453);
}

float2 glitchOffset(float2 uv, float time)
{
    float glitchStrength = 0.02;
    float noise = rand(float2(uv.y * 10.0, time));
    return uv + float2(noise * glitchStrength, 0);
}


float4 fColor(PS_IN In, float4 basecolor)
{
    float4 nameInColor = basecolor; // 初期色
    {
        float3 color1 = float3(1.0, 1.0, 1.0); // 赤
        float3 color2 = float3(0.0, 0.0, 0.0); // 青
        float3 gradient = lerp(color1, color2,
        (sin(max(min(
        (In.TexCoord.y) / 1.0, 1.0f), 0.0f) * 3.1415 * 2.0 * 1.25 - GameFrame * -0.05
        ) + 1.0 / 2.0)
        ); // UVのX座標で補間
        //max(min((In.TexCoord.y - 0.4) / 0.2, 1.0f), 0.0f)
        
        nameInColor.rgb = gradient; // RGBに適用
    }
    
    {
        float4 lightColor = TexColor; // 光の色
        {
            float tmp = lightColor.y;
            lightColor.y = lightColor.z;
            lightColor.z = tmp;
        }
        
        float addpos = 0.4f * (float) GameFrame;

        float2 pos = In.TexCoord.xy * 100.0f; // テクスチャ座標を拡大
        
        // 回転行列を適用
        float angle = radians(25.0); //回転
        float2 rotatedPos;
        rotatedPos.x = pos.x * cos(angle) - pos.y * sin(angle) + addpos;
        rotatedPos.y = pos.x * sin(angle) + pos.y * cos(angle) + addpos;

        // チェック模様の判定
        int checkX = (int) (rotatedPos.x / 60) % 2;
        int checkY = (int) (rotatedPos.y / 60) % 2;
        int checker = (checkX + checkY) % 2;

        // 輝度の計算
        //float luminance = dot(nameInColor.xyz, float3(0.299, 0.587, 0.114) * 1.25f);
        //float luminance = length(nameInColor.rgb) / 1.0f;
        
        // 色の補間
        float4 brightColor =
            float4(lightColor.rgb, 1.0) * 0.45
            + nameInColor * float4(lightColor.rgb, 1.0) * 0.3; // 明るい部分の色
        //brightColor.rgb *= 2.0f;
        brightColor.rgb = max(brightColor.rgb, lightColor.rgb * 0.2);
        brightColor.a = 1.0f;
        
        //float4 blendedColor = lerp(nameInColor, brightColor, luminance); // 輝度に応じた補間
        float4 blendedColor = brightColor; // 輝度に応じた補間
        
        blendedColor.rgb *= 0.25f;
        nameInColor = checker ? blendedColor * 0.75 : blendedColor;
    }
    
    return nameInColor;

}

float4 lColor(PS_IN In, float4 basecolor)
{
    float4 nameInColor = basecolor; // 初期色
    {
        float3 color1 = float3(1.0, 1.0, 1.0); // 赤
        float3 color2 = float3(0.0, 0.0, 0.0); // 青
        float3 gradient = lerp(color1, color2,
        (sin(max(min(
        (In.TexCoord.y) / 1.0, 1.0f), 0.0f) * 3.1415 * 2.0 * 4.0 - GameFrame * 0.08
        ) + 1.0 / 2.0)
        ); // UVのX座標で補間
        //max(min((In.TexCoord.y - 0.4) / 0.2, 1.0f), 0.0f)
        
        nameInColor.rgb = gradient; // RGBに適用
    }
    
    {
        float4 lightColor = TexColor; // 光の色
        
        float addpos = -0.2f * (float) GameFrame;

        float2 pos = In.TexCoord.xy * 100.0f; // テクスチャ座標を拡大
        
        // 回転行列を適用
        float angle = radians(60.0); //回転
        float2 rotatedPos;
        rotatedPos.x = pos.x * cos(angle) - pos.y * sin(angle) + addpos;
        rotatedPos.y = pos.x * sin(angle) + pos.y * cos(angle) + addpos;

        // チェック模様の判定
        int checkX = (int) (rotatedPos.x / 40) % 2;
        int checkY = (int) (rotatedPos.y / 8) % 2;
        int checker = (checkX + checkY) % 2;

        // 輝度の計算
        //float luminance = dot(nameInColor.xyz, float3(0.299, 0.587, 0.114) * 1.25f);
        //float luminance = length(nameInColor.rgb) / 1.0f;
        
        // 色の補間
        float4 brightColor =
            float4(lightColor.rgb, 1.0) * 0.45
            + nameInColor * float4(lightColor.rgb, 1.0) * 0.3; // 明るい部分の色
        //brightColor.rgb *= 2.0f;
        brightColor.rgb = max(brightColor.rgb, lightColor.rgb * 0.2);
        brightColor.a = 1.0f;
        
        //float4 blendedColor = lerp(nameInColor, brightColor, luminance); // 輝度に応じた補間
        float4 blendedColor = brightColor; // 輝度に応じた補間
        
        nameInColor = checker ? blendedColor * 0.75 : blendedColor;
    }
    
    return nameInColor;

}

//=============================================================================
// ピクセルシェーダ
//=============================================================================
void main(in PS_IN In, out float4 outDiffuse : SV_Target)
{
    //float2 grid = abs(frac(In.TexCoord * 10.0) - 0.5); // グリッドの間隔を調整
    //float2 grid = abs(frac(In.TexCoord * 10.0)); // グリッドの間隔を調整
    float2 grid = abs(frac(In.TexCoord * 6.0 + 0.5) - 0.5);
    float linetmp = min(grid.x, grid.y); // 最小値を取得して線を描画
    float alpha = smoothstep(0, 0 + 0.02, linetmp); // 太さを調整
    
    outDiffuse = TexColor; // グリッドを描画
    //outDiffuse.rgb = TexColor.rgb * (1.0 - alpha); // グリッドを描画
    //outDiffuse.a = (1.0 - alpha); // グリッドを描画
    //outDiffuse.a = (1.0 - alpha) > 0.01; // グリッドを描画
    outDiffuse = lerp(
        fColor(In, outDiffuse),
        lColor(In, outDiffuse),
        (1.0 - alpha) > 0.01
    );
    
    {
        float time = frac(GameFrame * 0.05);
        float2 distortedUV = glitchOffset(In.TexCoord * 1.0f, time);
        float4 color = g_Texture.Sample(g_SamplerState, distortedUV);

    // RGBチャンネルをずらしてグリッチ効果を強調
        float4 glitchColor;
        glitchColor.r = g_Texture.Sample(g_SamplerState, distortedUV + float2(0.005, 0)).r;
        glitchColor.g = g_Texture.Sample(g_SamplerState, distortedUV).g;
        glitchColor.b = g_Texture.Sample(g_SamplerState, distortedUV - float2(0.005, 0)).b;
        glitchColor.a = color.a;
        
        outDiffuse += float4(0.0f, 0.0, 1.0f, 1.0f) * glitchColor * 0.25;

    }
    
    clip(outDiffuse.w - 0.01);

}
