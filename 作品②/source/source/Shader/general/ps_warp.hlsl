#include "common.hlsl"

//=============================================================================
// ピクセルシェーダ
//=============================================================================
void main(in PS_IN In, out float4 outDiffuse : SV_Target)
{
    outDiffuse =  Begin(In);
    
    float2 uv = In.TexCoord;
    // 手動でUVループを処理
    // 整数部分を取り除く
    uv = frac(uv);
    float4 px = g_Texture.Sample(g_SamplerState, uv) * TexColor;
    
    //// 近傍ピクセルを取得してブレンド
    //float blurAmount = 2.0f; // 距離に応じたブラー強度
    //float4 colorLeft = g_Texture.Sample(g_SamplerState,  clamp(uv + float2(-1.0f / ScreenSize.x, 0.0) * blurAmount, 0.0f, 0.999)) * TexColor;
    //float4 colorRight = g_Texture.Sample(g_SamplerState, clamp(uv + float2(1.0f / ScreenSize.x, 0.0) * blurAmount, 0.0f, 0.999)) * TexColor;
    //float4 colorUp = g_Texture.Sample(g_SamplerState,    clamp(uv + float2(0.0, -1.0f / ScreenSize.y) * blurAmount, 0.0f, 0.999)) * TexColor;
    //float4 colorDown = g_Texture.Sample(g_SamplerState,  clamp(uv + float2(0.0, 1.0f / ScreenSize.y) * blurAmount, 0.0f, 0.999)) * TexColor;

    //// 平均化してぼかし効果を適用
    //px = (px + colorLeft + colorRight + colorUp + colorDown) / 5.0;
    
    
	{
        px.x += (px.x - 0.5f) * 2.5f;
        px.y += (px.y - 0.5f) * 2.5f;
        px.z += (px.z - 0.5f) * 2.5f;
    }
        
    if (px.x <= 0.1f && px.y <= 0.1f && px.z <= 0.1f)
    {
        outDiffuse.w = 0;
    }
    else if (px.x <= 0.3f && px.y <= 0.3f && px.z <= 0.3f)
    {
        outDiffuse.x = 0.4f * 0.6;
        outDiffuse.y = 0.0f * 0.6;
        outDiffuse.z = 0.6f * 0.6;
    }
    else if (px.x <= 0.7f && px.y <= 0.7f && px.z <= 0.7f)
    {
        outDiffuse.x = 0.6f * 0.6;
        outDiffuse.y = 0.1f * 0.6;
        outDiffuse.z = 0.4f * 0.6;
    }
    outDiffuse.w *= px.w;
    
    //outDiffuse = px;
	
    outDiffuse = End(outDiffuse);
}
