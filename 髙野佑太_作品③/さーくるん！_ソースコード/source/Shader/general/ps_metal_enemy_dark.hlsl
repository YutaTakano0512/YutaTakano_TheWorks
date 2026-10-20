#include "common.hlsl"

//=============================================================================
// ピクセルシェーダ
//=============================================================================
void main(in PS_IN In, out float4 outDiffuse : SV_Target)
{
    outDiffuse = Begin(In);
    float2 uv = In.TexCoord;
    // 手動でUVループを処理
    // 整数部分を取り除く
    uv = frac(uv);
    
    float4 px = g_Texture.Sample(g_SamplerState, uv) * TexColor;

    if (
            (px.x < 0.6 && px.y >= 0.9 && px.z < 0.6)
            ||
            (px.x >= 0.8 && px.y < 0.6 && px.z < 0.75)
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
        float3 ambientColor = float3(0.24, 0.24, 0.25);
        
        float3 lig;
        float3 lig2;
        {
            float3 lightDirection = normalize(float3(0.0, 0.0, 0.3));
                //float3 lightDirection = normalize(float3(0.3, 0.9, 0.3));
                //float3 lightDirection = normalize(float3(0.3, 0.5, 0.0));
            //float3 lightDirection = normalize(float3(0.0, 1.0, 0.7));
            //float3 lightDirection = normalize(float3(-0.0, -1.0, -1.0));
        
            float3 lightColor = float3(1.0, 1.0, 0.95); // ライトの色設定
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
            float3 toEye = CameraPos.xyz - In.Position.xyz;
            toEye = normalize(toEye);

        // 1と2で求めたベクトルの内積を使って、鏡面反射の強さを求める
            t = dot(refVec, toEye);
        // 内積の結果が0以下なら0にする
            if (t < 0.0f)
            {
                t = 0.0f;
            }

        // step-7 鏡面反射の強さを絞る
            t = pow(t, 0.4f);

        // 4. 3で求めた鏡面反射の強さを絞って、最終的な反射の強さを求める
            float3 specularLig = lightColor * t;
    

        // 拡散反射光と鏡面反射光を足し算して、最終的な光を求める
            lig2 = diffuseLig + specularLig;

        }
        
        // テクスチャカラーに求めた光を乗算して最終出力カラーを求める
        outDiffuse.xyz = px;
        outDiffuse.xyz *= ambientColor + lig2;
            
        outDiffuse.x += (outDiffuse.x - 0.5f) * 0.16f + 0.05f;
        outDiffuse.y += (outDiffuse.y - 0.5f) * 0.16f + 0.05f;
        outDiffuse.z += (outDiffuse.z - 0.5f) * 0.16f + 0.05f;
    }
    
    outDiffuse = End(outDiffuse);
}
