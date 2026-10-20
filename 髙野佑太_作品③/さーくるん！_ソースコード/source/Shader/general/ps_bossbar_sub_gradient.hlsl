#include "common.hlsl"

float4 GageColor(PS_IN In, float4 basecolor)
{
    float4 nameInColor = basecolor; // 初期色
    //{
    //        // UV座標を時間に基づいて移動
    //    float2 uv = In.TexCoord;
    //    uv.x += (float)GameFrame / 60 * 0.1; // 矢印の流れる速度調整
    //    uv.y = max(min((In.TexCoord.y - 0.4) / 0.2, 1.0f), 0.0f);
        
    //    float stripe = step(frac(uv.x * 20), 0.7); // 横方向のストライプ
    //    float diagonal = step(abs(uv.y - (frac(uv.x * 20) * 0.8)), 0.3); // 斜めのライン

    //    nameInColor.rgb = 1.0 - stripe * diagonal;
    //    nameInColor.a = 1.0f;

    //}
    
    {
        float3 color1 = float3(0.3, 0.3, 1.0); // 
        float3 color2 = float3(0.01, 0.01, 0.01); // 
        float3 gradient = lerp(color1, color2,
        ((sin(In.Position.x / 20.0f + GameFrame * 0.2) + 1.0) / 2.0)
        ); // UVのX座標で補間
        //max(min((In.TexCoord.y - 0.4) / 0.2, 1.0f), 0.0f)
        
        nameInColor.rgb = gradient; // RGBに適用
    }
    
    {
        float4 lightColor = float4(1.0, 1.0, 1.0, 1.0); // 光の色
        
        float addpos = 0.1f * (float) GameFrame;

        float2 pos = In.TexCoord.xy * 100.0f; // テクスチャ座標を拡大
        
        // 回転行列を適用
        float angle = radians(25.0); //回転
        float2 rotatedPos;
        rotatedPos.x = pos.x * cos(angle) - pos.y * sin(angle) + addpos;
        rotatedPos.y = pos.x * sin(angle) + pos.y * cos(angle) + addpos;

        // チェック模様の判定
        int checkX = (int) (rotatedPos.x / 5) % 2;
        int checkY = (int) (rotatedPos.y / 5) % 2;
        int checker = (checkX + checkY) % 2;

        // 輝度の計算
        //float luminance = dot(nameInColor.xyz, float3(0.299, 0.587, 0.114) * 1.25f);
        //float luminance = length(nameInColor.rgb) / 1.0f;
        
        // 色の補間
        float4 brightColor =
            float4(lightColor.rgb, 1.0) * 0.6
            + nameInColor * float4(lightColor.rgb, 1.0) * 0.4; // 明るい部分の色
        brightColor.a = 1.0f;
        
        //float4 blendedColor = lerp(nameInColor, brightColor, luminance); // 輝度に応じた補間
        float4 blendedColor = brightColor; // 輝度に応じた補間
        
        //blendedColor.rgb *= 2.4f;
        nameInColor = checker ? blendedColor * 0.9 : blendedColor;
        
        //nameInColor =
        //    basecolor * 0.6
        //    + (nameInColor) * basecolor * 0.5;
        nameInColor =
            basecolor * 0.7
            + (nameInColor) * 0.7;
        nameInColor.a = basecolor.a;
        //nameInColor.a = 1;
        //nameInColor.rgb *= 1.25f;

    }
    
    return nameInColor;

}

//=============================================================================
// ピクセルシェーダ
//=============================================================================
void main(in PS_IN In, out float4 outDiffuse : SV_Target)
{
    outDiffuse = Begin(In);
    
    float4 result = outDiffuse;
    
    result = lerp(result,
        GageColor(In, outDiffuse * TexColor),
        outDiffuse.a > 0.1 && ((GameFrame % 6) < 3)
    );

    outDiffuse = result;
    
    outDiffuse = End(outDiffuse);
}
