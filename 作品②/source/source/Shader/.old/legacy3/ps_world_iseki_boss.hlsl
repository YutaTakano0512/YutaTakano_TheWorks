#include "common.hlsl"


#define COLORBORDERMULMOD 0.2
#define COLORBORDERADDMOD 0.1
#define COLORBORDER 0.5
//=============================================================================
// ピクセルシェーダ
//=============================================================================
void main(in PS_IN In, out float4 outDiffuse : SV_Target)
{
    float4 px = g_Texture.Sample(g_SamplerState, In.TexCoord + float2(TexUV.x, TexUV.y)) * TexColor;
	
    outDiffuse = px;
    {
		 // ランバート拡散証明モデル
        In.Normal.xyz = normalize(In.Normal.xyz); // 正規化(長さ1にする)
		
		{
            float3 lightDirection = normalize(float3(0.0, 0.0, -1.0));
		
            float3 lightColor = float3(-0.5, -0.5, -0.4); // ライトの色設定
            float3 ambientColor = float3(1.0, 1.0, 1.0); // 環境光の色設定
    
            float3 light = -dot(In.Normal.xyz, lightDirection);
            light = saturate(light) * lightColor + ambientColor; // ライトの色処理

    
            outDiffuse.rgb *= light;
        }
    }
	{
        outDiffuse.x += (outDiffuse.x - COLORBORDER) * COLORBORDERMULMOD + COLORBORDERADDMOD;
        outDiffuse.y += (outDiffuse.y - COLORBORDER) * COLORBORDERMULMOD + COLORBORDERADDMOD;
        outDiffuse.z += (outDiffuse.z - COLORBORDER) * COLORBORDERMULMOD + COLORBORDERADDMOD;
    }
	
    //clip(outDiffuse.w - 0.01);
	
    outDiffuse.w = 1.0f;
	
 }
