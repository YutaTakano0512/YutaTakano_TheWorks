#include "common.hlsl"

//=============================================================================
// ピクセルシェーダ
//=============================================================================
void main(in PS_IN In, out float4 outDiffuse : SV_Target)
{ 
    // テクスチャ座標の処理を簡素化
    float2 coord = clamp(In.TexCoord + TexUV.xy, 0.0, 1.0);
    float4 px = g_Texture.Sample(g_SamplerState, coord) * TexColor;

    
    //if (px.w > 0)
    //if (px.w > 0.95f)
    if (px.w > 0.975f)
    //if (px.w > 0.75f)
    {
        outDiffuse = px;
        if (all(FixedColor.agba < 0))
        {
            outDiffuse.rgb = px.rgb;
            
            //float3 condition = step(0.5, outDiffuse.rgb); // 0.5より大きければ1、小さければ0
            //outDiffuse.rgb = lerp(float3(0.5, 0.5, 0.5), outDiffuse.rgb, 0.5);
            //outDiffuse.rgb += (1.0 - condition) * -0.2 - condition * -0.2;
            ////outDiffuse.rgb += -0.5f;
            ////outDiffuse.rgb = lerp(float3(0.5, 0.5, 0.5), outDiffuse.rgb, 0.5);
            ////outDiffuse.rgb += -0.3f;
            //outDiffuse.rgb = saturate(outDiffuse.rgb);
            
            float3 condition = step(0.5, outDiffuse.rgb); // 0.5より大きければ1、小さければ0
            outDiffuse.rgb = lerp(float3(0.5, 0.5, 0.5), outDiffuse.rgb, 0.6);
            outDiffuse.rgb += -0.3f;
            outDiffuse.rgb = saturate(outDiffuse.rgb);
            outDiffuse.rgb += 1.0f;
            outDiffuse.rgb = pow(outDiffuse.rgb, 1.4f);
            outDiffuse.rgb += -1.0f;
            outDiffuse.rgb = saturate(outDiffuse.rgb);
            outDiffuse.rgb += 0.5f;
        }
        else
        {
            outDiffuse.x = FixedColor.x;
            outDiffuse.y = FixedColor.y;
            outDiffuse.z = FixedColor.z;
        }
        //outDiffuse.w = 1.0f;
        outDiffuse.w = max(px.w, 0.1);
    }
    else
    {
        outDiffuse.x = 0.0f;
        outDiffuse.y = 0.0f;
        outDiffuse.z = 0.0f;
        outDiffuse.w = 0.0f;
        clip(outDiffuse.w - 0.1);
    }
}
