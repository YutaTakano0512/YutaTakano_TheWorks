#include "common.hlsl"

float4 NameInColor(PS_IN In, float4 basecolor)
{
    float4 nameInColor = basecolor; // 初期色
    {
        float3 color1 = float3(1.0, 1.0, 1.0); // 赤
        float3 color2 = float3(0.0, 0.0, 0.0); // 青
        float3 gradient = lerp(color1, color2,
        (sin(max(min(
        (In.TexCoord.y - 0.4) / 0.2, 1.0f), 0.0f) * 3.1415 * 2.0 * 3.0 - GameFrame * 0.5
        ) + 1.0 / 2.0)
        ); // UVのX座標で補間
        //max(min((In.TexCoord.y - 0.4) / 0.2, 1.0f), 0.0f)
        
        nameInColor.rgb = gradient; // RGBに適用
    }
    
    {
        float4 lightColor = float4(1.0, 0.15, 0.5, 1.0); // 光の色
        
        float addpos = 1.0f * (float) GameFrame;

        // 回転行列を適用
        float angle = radians(25.0); //回転
        float2 rotatedPos;
        rotatedPos.x = In.Position.x * cos(angle) - In.Position.y * sin(angle) + addpos;
        rotatedPos.y = In.Position.x * sin(angle) + In.Position.y * cos(angle) + addpos;

        // チェック模様の判定
        int checkX = (int) (rotatedPos.x / 8) % 2;
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
        
        nameInColor = checker ? blendedColor - float4(0.15, 0.1, 0.08, 0) : blendedColor;
        nameInColor.rgb += nameInColor.rgb * 1.6;
    }
    
    return nameInColor;

}

float4 NameOutColor(PS_IN In, float4 basecolor)
{
    float4 nameOutColor = basecolor;
    
    float2 center = float2(0.5, 0.5); // 画面の中央
    float dist = length(In.TexCoord - center); // 中心からの距離（半径）

    float wave = sin(dist * 100.0f - GameFrame * 0.75); // 波の周期とオフセットを調整
    wave = (wave + 1) * 0.5; // -1~1の範囲を0~1に調整
    
    
    float4 baseColor = float4(0.2, 0.0, 1.0, 1.0);
    nameOutColor =
        baseColor * 0.05
        + float4(0.5, 0.7, 1.0, 1.0) * float4(wave, wave, wave, 1.0) * 0.4;
    nameOutColor.a = 1.0f;
    
    return nameOutColor;

}

//=============================================================================
// ピクセルシェーダ
//=============================================================================
void main(in PS_IN In, out float4 outDiffuse : SV_Target)
{
    outDiffuse = Begin(In);
    
    float4 nameInColor = NameInColor(In, outDiffuse);
    float4 nameOutColor = NameOutColor(In, outDiffuse);
    
    
    //float luminance = dot(outDiffuse.rgb, float3(0.3, 0.59, 0.11)); // 輝度を計算
    float4 result = outDiffuse;

    // ほぼ黒なら nameOutColor に、ほぼ白なら nameInColor にする
    result = lerp(result, nameOutColor, outDiffuse.a > 0.01 && all(outDiffuse.rgb < 0.2));
    result = lerp(result, nameInColor, outDiffuse.a > 0.01 && all(outDiffuse.rgb > 0.8));

    outDiffuse = result;
    
    outDiffuse = End(outDiffuse);
}
