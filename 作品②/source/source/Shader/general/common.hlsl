#include "../common.hlsl"

float4 Begin(PS_IN In)
{
    float2 uv = In.TexCoord;
    // 手動でUVループを処理
    // 整数部分を取り除く
    uv = frac(uv);
    float4 px = g_Texture.Sample(g_SamplerState, uv) * TexColor;
    //float4 px = g_Texture.Sample(g_SamplerState, clamp(In.TexCoord, 0.0f, 0.999)) * TexColor;
    
//    float2 texSize;
//    g_Texture.GetDimensions(texSize.x, texSize.y);
//    // 近傍ピクセルを取得してブレンド
//    float blurAmount = 1.0f; // 距離に応じたブラー強度
//    float4 colorLeft = g_Texture.Sample(g_SamplerState, clamp(In.TexCoord + float2(-1.0f / texSize.x, 0.0) * blurAmount, 0.0f, 0.99)) * TexColor;
//    float4 colorRight = g_Texture.Sample(g_SamplerState, clamp(In.TexCoord + float2(1.0f / texSize.x, 0.0) * blurAmount, 0.0f, 0.99)) * TexColor;
//    float4 colorUp = g_Texture.Sample(g_SamplerState, clamp(In.TexCoord + float2(0.0, -1.0f / texSize.y) * blurAmount, 0.0f, 0.99)) * TexColor;
//    float4 colorDown = g_Texture.Sample(g_SamplerState, clamp(In.TexCoord + float2(0.0, 1.0f / texSize.y) * blurAmount, 0.0f, 0.99)) * TexColor;

//// 平均化してぼかし効果を適用
//    px.a = (px.a + colorLeft.a + colorRight.a + colorUp.a + colorDown.a) / 5.0;
    
    //float sdfValue = px.r; // SDF値を取得
    //float edgeFactor = smoothstep(0.45, 0.55, sdfValue); // エッジの滑らかさを調整

    // FixedColorがすべて非負かどうかを判定（非負なら1.0、それ以外は0.0）
    float isFixedColorValid = step(EPSILON, px.w) * step(0, FixedColor.x) * step(0, FixedColor.y) * step(0, FixedColor.z) * step(0, FixedColor.w);

    
    // `lerp()` を活用して条件分岐を削減
    
    return lerp(px, FixedColor, isFixedColorValid);
    //return lerp(lerp(px, FixedColor, isFixedColorValid), float4(0, 0, 0, 0), edgeFactor);
}

float4 End(float4 outDiffuse)
{
    // クリップ処理
    clip(outDiffuse.w - 0.01);

    // NoAlphaが1の場合、アルファ値を強制設定
    outDiffuse.w = lerp(outDiffuse.w, 1.0f, NoAlpha);
    
    return outDiffuse;
}

float ColorMatch(float3 color1, float3 color2, float threshold)
{
    float dist = length(color1 - color2);
    return (dist <= threshold) ? 1.0 : 0.0;
}

float4 Filter_MetalEnemy(in PS_IN In, float4 px, float4 outDiffuse,
    float3 safecolor,
    float reflectPower,
    float3 ambientColor,
    float3 lightColor
)
{
    //METAL_ENEMY
    {
        if (
            ColorMatch(safecolor, px.xyz, 0.24f)
        )
        {
            outDiffuse.xyz = px;
        }
        else
        {
		    // Phong鏡面反射モデル
        
            In.Normal.xyz = normalize(In.Normal.xyz); // 正規化(長さ1にする)
		
            //float3 ambientColor = float3(0.2, 0.2, 0.6);
            //float3 ambientColor = float3(0.35, 0.35, 0.65);
            //float3 ambientColor = float3(0.25, 0.25, 0.45);
            //float3 ambientColor = float3(0.32, 0.32, 0.64);
            //float3 ambientColor = float3(0.4, 0.4, 0.65);
        
            float3 lig;
            float3 lig2;
            {
                float3 lightDirection = normalize(float3(0.0, -0.01, -0.3));
                //float3 lightDirection = normalize(float3(0.3, 0.9, 0.3));
                //float3 lightDirection = normalize(float3(0.3, 0.5, 0.0));
            //float3 lightDirection = normalize(float3(0.0, 1.0, 0.7));
            //float3 lightDirection = normalize(float3(-0.0, -1.0, -1.0));
        
                //float3 lightColor = float3(1.0, 1.0, 0.8); // ライトの色設定
                //float3 lightColor = float3(0.75, 0.75, 0.65); // ライトの色設定
        //float3 lightColor = float3(0.5, 0.5, 1.0); // ライトの色設定
        
        // 1. ライトの方向と面の法線とで内積を計算する
                float t = dot(In.Normal.xyz, lightDirection);
        // 1.で求めた結果に-1を乗算してライトの強さを求める
                t *= -1.0f;

        // ライトの強さにマイナスの値は必要ないため
                if (t < 0.0f)
                    t = 0.0f;

        
        // 拡散反射光を求める
                float3 diffuseLig = lightColor * t;

        // 1. ライトが面に入射して、反射したベクトルを求める
                float3 refVec = reflect(lightDirection, In.Normal.xyz);

        //2. 光が入射した面から視点に向かって伸びるベクトルを求める
                //float3 toEye = CameraPos.xyz - In.Position.xyz;
                float3 toEye = -CameraPos.xyz;
                toEye = normalize(toEye);

        // 1と2で求めたベクトルの内積を使って、鏡面反射の強さを求める
                t = dot(refVec, toEye);
        // 内積の結果が0以下なら0にする
                if (t < 0.0f)
                {
                    t = 0.0f;
                }

        // step-7 鏡面反射の強さを絞る
                t = pow(t, reflectPower);

        // 4. 3で求めた鏡面反射の強さを絞って、最終的な反射の強さを求める
                float3 specularLig = lightColor * t;
    

        // 拡散反射光と鏡面反射光を足し算して、最終的な光を求める
                lig2 = diffuseLig + specularLig;

            }
        
        
        // テクスチャカラーに求めた光を乗算して最終出力カラーを求める
            outDiffuse.xyz = px;
            outDiffuse.xyz *= ambientColor + lig2;
        }
    }
    
    return outDiffuse;

}


float4 Filter_CutIn(in PS_IN In, float4 px, float4 outDiffuse,
    float3 blindColor,
    float3 lightColor)
{
    //outDiffuse.x += (outDiffuse.x - 0.5f) * 0.4f + 0.2f;
    //outDiffuse.y += (outDiffuse.y - 0.5f) * 0.4f + 0.2f;
    //outDiffuse.z += (outDiffuse.z - 0.5f) * 0.4f + 0.2f;
    
    //int pX = (int) (In.Position.x * 2);
    //int pY = (int) (In.Position.y * 1);
    //if (
    //        (
    //            (pX % 2 == 0 && pY % 2 != 0)
    //            || (pX % 2 != 0 && pY % 2 == 0)
    //            //|| ((int) inPosition.z) % 2 == 0
    //        )
    //    )
    //{
    //    outDiffuse.x += max(min(outDiffuse.x, 1.0f), 0.0f) * 0.3f * (outDiffuse.x < 0.5f ? -1 : 1);
    //    outDiffuse.y += max(min(outDiffuse.y, 1.0f), 0.0f) * 0.3f * (outDiffuse.y < 0.5f ? -1 : 1);
    //    outDiffuse.z += max(min(outDiffuse.z, 1.0f), 0.0f) * 0.3f * (outDiffuse.z < 0.5f ? -1 : 1);
    //}
    
    //return outDiffuse;
    
    //int checkX = (int) (In.Position.x / 12) % 2; // X方向のチェック判定
    //int checkY = (int) (In.Position.y / 12) % 2; // Y方向のチェック判定
    //int checker = (checkX + checkY) % 2; // チェック模様の生成

    //float4 color2 = outDiffuse;
    //color2.xyz += max(min(outDiffuse.xyz, 1.0f), 0.0f) * 0.3f * (outDiffuse.xyz < 0.5f ? -1 : 1);

    
    //return checker ? outDiffuse : color2;
    
    //{
    //    float addposX = -0.4f * (float) GameFrame;
    //    float addposY = 0.4f * (float) GameFrame;
    //// 回転行列を適用
    //    float angle = radians(45.0); //回転
    //    float2 rotatedPos;
    //    rotatedPos.x = In.Position.x * cos(angle) - In.Position.y * sin(angle) + addposX;
    //    rotatedPos.y = In.Position.x * sin(angle) + In.Position.y * cos(angle) + addposY;

    //// チェック模様の判定
    //    int checkX = (int) (rotatedPos.x / 5) % 2;
    //    int checkY = (int) (rotatedPos.y / 5) % 2;
    //    int checker = (checkX + checkY) % 2;

    //// 色の調整
    //    float4 color2 = outDiffuse;
    //    color2.xyz += max(min(color2.xyz, 1.0f), 0.0f) * -0.5f;

    //    outDiffuse = checker? outDiffuse: color2;
    //}
    //{
    //    float addpos = 1.0f * (float) GameFrame;
    //// 回転行列を適用
    //    float angle = radians(25.0); //回転
    //    float2 rotatedPos;
    //    rotatedPos.x = In.Position.x * cos(angle) - In.Position.y * sin(angle) + addpos;
    //    rotatedPos.y = In.Position.x * sin(angle) + In.Position.y * cos(angle) + addpos;

    //// チェック模様の判定
    //    int checkX = (int) (rotatedPos.x / 15) % 2;
    //    int checkY = (int) (rotatedPos.y / 15) % 2;
    //    int checker = (checkX + checkY) % 2;

    //// 色の調整
    //    float4 color2 = outDiffuse;
    //    //color2.xyz += max(min(color2.xyz, 1.0f), 0.0f) * 0.45f;
    //    color2.x += 0.11f;
    //    color2.y += 0.00f;
    //    color2.z += 0.15f;

    //    outDiffuse = checker ? outDiffuse : color2;
    //}
    {
        float addposX = -0.4f * (float) GameFrame;
        float addposY = 0.4f * (float) GameFrame;

    // 回転行列を適用
        float angle = radians(45.0); //回転
        float2 rotatedPos;
        rotatedPos.x = In.Position.x * cos(angle) - In.Position.y * sin(angle) + addposX;
        rotatedPos.y = In.Position.x * sin(angle) + In.Position.y * cos(angle) + addposY;

    // チェック模様の判定
        //int checkX = (int) (rotatedPos.x / 5) % 2;
        //int checkY = (int) (rotatedPos.y / 5) % 2;
        int checkX = (int) (rotatedPos.x / 12) % 2;
        int checkY = (int) (rotatedPos.y / 12) % 2;
        int checker = (checkX + checkY) % 2;

    // 輝度の計算
        float luminance = dot(outDiffuse.xyz, float3(0.299, 0.587, 0.114) * 1.5f);
        
        // 色の補間
        float4 brightColor = outDiffuse + float4(blindColor.x, blindColor.y, blindColor.z, 0.0); // 暗い部分の色
        float4 blendedColor = lerp(outDiffuse, brightColor, 1.0f - luminance); // 輝度に応じた補間
        
        outDiffuse = checker ? outDiffuse : blendedColor;
    }

    {
        float addpos = 1.0f * (float) GameFrame;

    // 回転行列を適用
        float angle = radians(25.0); //回転
        float2 rotatedPos;
        rotatedPos.x = In.Position.x * cos(angle) - In.Position.y * sin(angle) + addpos;
        rotatedPos.y = In.Position.x * sin(angle) + In.Position.y * cos(angle) + addpos;

    // チェック模様の判定
        //int checkX = (int) (rotatedPos.x / 15) % 2;
        //int checkY = (int) (rotatedPos.y / 15) % 2;
        int checkX = (int) (rotatedPos.x / 40) % 2;
        int checkY = (int) (rotatedPos.y / 40) % 2;
        int checker = (checkX + checkY) % 2;

    // 輝度の計算
        float luminance = dot(outDiffuse.xyz, float3(0.299, 0.587, 0.114) * 1.25f);
        
        // 色の補間
        float4 brightColor = outDiffuse + float4(lightColor.x, lightColor.y, lightColor.z, 0.0); // 明るい部分の色
        float4 blendedColor = lerp(outDiffuse, brightColor, luminance); // 輝度に応じた補間
        
        outDiffuse = checker ? outDiffuse : blendedColor;
    }
    return outDiffuse;

}