#include "common.hlsl"

//#define SAMPLE_NUM 6
#define SAMPLE_NUM 16
#define SAMPLE_INV (1.0 / SAMPLE_NUM)
#define PI2 6.2831852

//#define MAX_DELTA 0.026
#define MAX_DELTA 0.016
#define DELTA 0.003

//#define BLOOM_COLOR 0.96
//#define BLOOM_COLOR 0.6
//#define BLOOM_COLOR 0.0
#define BLOOM_COLOR 0.5

//#define BG_COLOR 0.001
#define BG_COLOR 0.01

#define BLOOM_ADDCOLOR 0.3
#define BLOOM_MODCOLOR 0.5

//=============================================================================
// ピクセルシェーダ
//=============================================================================
void main(in PS_IN In, out float4 outDiffuse : SV_Target)
{
    float4 px = g_Texture.Sample(g_SamplerState, In.TexCoord);
    
    float isBackground = step(BG_COLOR, px.x) * step(BG_COLOR, px.y) * step(BG_COLOR, px.z);
    
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
            
            if (px2.x > BLOOM_COLOR && px2.y > BLOOM_COLOR && px2.z > BLOOM_COLOR)
            {
                pointPx = px2;
                break; //ループ終了
            }
        }
    }
    
    float isEdge = step(EPSILON, pointPx.w);

    float alpha = 1.0 - (delta / MAX_DELTA);
    
    outDiffuse = float4(
        pointPx.x + BLOOM_ADDCOLOR,
        pointPx.y + BLOOM_ADDCOLOR,
        pointPx.z + BLOOM_ADDCOLOR,
        BLOOM_MODCOLOR * alpha * (1.0f - isBackground) * isEdge
    );
    clip(outDiffuse.w - 0.01);

}
