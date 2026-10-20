#include "common.hlsl"

float4 GageColor(PS_IN In, float4 basecolor)
{
    float resultAlpha = 1.0f;
    
    {
        float color1 = 1.0; // 赤
        float color2 = 0.0; // 青
        float gradient = lerp(color1, color2,
        (sin(max(min(In.Position.y / 1000.0, 1.0f), 0.0f) * 3.1415 * 2.0 * 30.0 - GameFrame * -0.05
        ) + 1.0 / 2.0)
        ); // UVのX座標で補間
        //max(min((In.TexCoord.y - 0.4) / 0.2, 1.0f), 0.0f)
        
        resultAlpha = gradient; // RGBに適用
    }
    
    {
        float addpos = 0.2f * (float) GameFrame;

        float2 pos = In.Position.xy; // テクスチャ座標を拡大
        
        // 回転行列を適用
        float angle = radians(25.0); //回転
        float2 rotatedPos;
        rotatedPos.x = pos.x * cos(angle) - pos.y * sin(angle) + addpos;
        rotatedPos.y = pos.x * sin(angle) + pos.y * cos(angle) + addpos;

        // チェック模様の判定
        int checkX = (int) (rotatedPos.x / 160) % 2;
        int checkY = (int) (rotatedPos.y / 160) % 2;
        int checker = (checkX + checkY) % 2;
        
        // 色の補間
        float alpha = basecolor.a * 0.75
        + basecolor.a * 0.15 * resultAlpha;
        
        //blendedColor.rgb *= 1.6f;
        resultAlpha = checker ? alpha * 0.5 : alpha;
        
        basecolor.a = resultAlpha;
    }
    
    
    return basecolor;

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
        outDiffuse.a < 0.8 && outDiffuse.a > 0.01
    );

    outDiffuse = result;
    
    outDiffuse = End(outDiffuse);
}
