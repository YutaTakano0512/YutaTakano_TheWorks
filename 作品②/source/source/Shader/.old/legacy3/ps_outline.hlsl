#include "common.hlsl"

#define SAMPLE_NUM 6
#define SAMPLE_INV (1.0 / SAMPLE_NUM)
#define PI2 6.2831852

#define BG_COLOR 0.1
//#define MAX_DELTA 0.008
//#define MAX_DELTA 0.006
//#define MAX_DELTA 0.007
//#define MAX_DELTA 0.003
#define MAX_DELTA 0.004
//#define DELTA 0.003
//#define DELTA 0.004
#define DELTA 0.001

#define EPSILON 0.001
#define BLOOM_COLOR float4(1.0f, 0.0f, 1.0, 1.0)

//=============================================================================
// ピクセルシェーダ
//=============================================================================
void main(in PS_IN In, out float4 outDiffuse : SV_Target)
{
    float4 px = g_Texture.Sample(g_SamplerState, In.TexCoord);
    
    float isBackground = min(step(BG_COLOR, px.x) + step(BG_COLOR, px.y) + step(BG_COLOR, px.z), 1.0f);
    
    float wSla = ScreenSize.y / ScreenSize.x;
    
    float4 pointPx = float4(0, 0, 0, 0);
        
    float delta = DELTA;
    
	[unroll]
    for (; pointPx.w <= EPSILON && delta < MAX_DELTA; delta += DELTA)
    {
	    [unroll]
        for (int j = 0; j < SAMPLE_NUM; j++)
        {
            float2 offset = float2(sin(SAMPLE_INV * j * PI2) * delta * wSla, cos(SAMPLE_INV * j * PI2) * delta);
            float2 coord = clamp(In.TexCoord + offset, float2(MAX_DELTA, MAX_DELTA), float2(1.0 - MAX_DELTA, 1.0 - MAX_DELTA));
            float4 px2 = g_Texture.Sample(g_SamplerState, coord);
            
            if (px2.x > 0.1f || px2.y > 0.1f || px2.z > 0.1f)
            {
                pointPx = px2;
                break; //ループ終了
            }
        }
    }
    
    float isEdge = step(EPSILON, pointPx.w);
    
    outDiffuse = lerp(
    float4(0.0f, 0.0f, 0.0f, 0.0f),
    pointPx, 
    (1.0f - isBackground) * isEdge);
    clip(outDiffuse.w - 0.01);
}
