#include "common.hlsl"

float4 GageColor(PS_IN In, float4 basecolor)
{
    float4 nameInColor = basecolor; // 初期色
    {
        float3 color1 = float3(1.0, 1.0, 1.0); // 赤
        float3 color2 = float3(0.0, 0.0, 0.0); // 青
        float3 gradient = lerp(color1, color2,
        (sin(max(min(
        (In.TexCoord.y) / 1.0, 1.0f), 0.0f) * 3.1415 * 2.0 * 0.75 - GameFrame * -0.3
        ) + 1.0 / 2.0)
        ); // UVのX座標で補間
        //max(min((In.TexCoord.y - 0.4) / 0.2, 1.0f), 0.0f)
        
        nameInColor.rgb = gradient; // RGBに適用
    }
    
    {
        float4 lightColor = TexColor; // 光の色
        
        float addpos = 1.0f * (float) GameFrame;

        float2 pos = In.TexCoord.xy * 100.0f; // テクスチャ座標を拡大
        
        // 回転行列を適用
        float angle = radians(25.0); //回転
        float2 rotatedPos;
        rotatedPos.x = pos.x * cos(angle) - pos.y * sin(angle) + addpos;
        rotatedPos.y = pos.x * sin(angle) + pos.y * cos(angle) + addpos;

        // チェック模様の判定
        int checkX = (int) (rotatedPos.x / 40) % 2;
        int checkY = (int) (rotatedPos.y / 40) % 2;
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
        
        blendedColor.rgb *= 1.6f;
        nameInColor = checker ? blendedColor * 0.75 : blendedColor;
    }
    
    return nameInColor;

}

//=============================================================================
// ピクセルシェーダ
//=============================================================================
void main(in PS_IN In, out float4 outDiffuse : SV_Target)
{
    outDiffuse = Begin(In);
    
    //float luminance = dot(outDiffuse.rgb, float3(0.3, 0.59, 0.11)); // 輝度を計算
    float4 result = outDiffuse;
    
    result = lerp(result, 
    GageColor(In, outDiffuse),  outDiffuse.a > 0.01);
    //result = GageColor(In, outDiffuse);

    outDiffuse = result;
    
    outDiffuse = End(outDiffuse);
}
