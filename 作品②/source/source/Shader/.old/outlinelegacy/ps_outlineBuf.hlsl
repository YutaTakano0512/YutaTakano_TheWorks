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
    if (px.w > 0.8f)
    {
        //outDiffuse.rgba = float4(1.0f, 1.0f, 1.0f, 1.0f);
        outDiffuse = px;
        //outDiffuse.x = 1.0f;
        //outDiffuse.y = 0.0f;
        //outDiffuse.z = 0.0f;
        //outDiffuse.w = 1.0f;
        if (all(FixedColor.agba < 0))
        {
            outDiffuse.rgb = px.rgb;
            //outDiffuse.rgb += (1.0f - (outDiffuse.rgb - 0.5f)) * -0.25f;
            float3 condition = step(0.5, outDiffuse.rgb); // 0.5より大きければ1、小さければ0
            outDiffuse.rgb += (1.0 - condition) * 0.4 - condition * 0.4;
            outDiffuse.rgb = saturate(outDiffuse.rgb);
            
            //if (outDiffuse.x <= 0.1 && outDiffuse.y <= 0.1 && outDiffuse.z <= 0.1)
            //{
            //    outDiffuse.x = 0.8f;
            //    outDiffuse.y = 0.8f;
            //    outDiffuse.z = 0.8f;
            //}
            //if (all(outDiffuse.rgb <= 0.1))
            //{
            //    outDiffuse.x += 0.1f;
            //    outDiffuse.y += 0.1f;
            //    outDiffuse.z += 0.1f;
            //}
        }
        else
        {
            outDiffuse.x = FixedColor.x;
            outDiffuse.y = FixedColor.y;
            outDiffuse.z = FixedColor.z;
        }
        outDiffuse.w = 1.0f;
    }
    else
    {
        outDiffuse.x = 0.0f;
        outDiffuse.y = 0.0f;
        outDiffuse.z = 0.0f;
        outDiffuse.w = 0.0f;
        clip(outDiffuse.w - 0.1);
        //outDiffuse.x = 1.0f;
        //outDiffuse.y = 1.0f;
        //outDiffuse.z = 1.0f;
        //outDiffuse.w = 1.0f;
    }
}
