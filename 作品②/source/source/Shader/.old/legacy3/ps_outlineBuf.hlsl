#include "common.hlsl"

//=============================================================================
// ピクセルシェーダ
//=============================================================================
void main(in PS_IN In, out float4 outDiffuse : SV_Target)
{ 
    // テクスチャ座標の処理を簡素化
    float2 coord = clamp(In.TexCoord + TexUV.xy, 0.0, 1.0);
    float4 px = g_Texture.Sample(g_SamplerState, coord) * TexColor;


    if (px.w > 0)
    {
        //outDiffuse.rgba = float4(1.0f, 1.0f, 1.0f, 1.0f);
        outDiffuse = px;
        //outDiffuse.x = 1.0f;
        //outDiffuse.y = 0.0f;
        //outDiffuse.z = 0.0f;
        //outDiffuse.w = 1.0f;
        if (FixedColor.x < 0 && FixedColor.y < 0 && FixedColor.z < 0 && FixedColor.w < 0)
        {
            outDiffuse.x = px.x;
            outDiffuse.y = px.y;
            outDiffuse.z = px.z;
            outDiffuse.x += (1.0f - (outDiffuse.x - 0.5f)) * -0.25f;
            outDiffuse.y += (1.0f - (outDiffuse.y - 0.5f)) * -0.25f;
            outDiffuse.z += (1.0f - (outDiffuse.z - 0.5f)) * -0.25f;
            
            if (outDiffuse.x <= 0.1 && outDiffuse.y <= 0.1 && outDiffuse.z <= 0.1)
            {
                outDiffuse.x = 0.6f;
                outDiffuse.y = 0.6f;
                outDiffuse.z = 0.6f;
            }
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
        clip(outDiffuse.w - 0.01);
        //outDiffuse.x = 1.0f;
        //outDiffuse.y = 1.0f;
        //outDiffuse.z = 1.0f;
        //outDiffuse.w = 1.0f;
    }
}
