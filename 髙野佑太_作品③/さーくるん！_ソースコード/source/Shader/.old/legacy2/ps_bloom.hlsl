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
void main(in float4 inPosition : POSITION0,
		 in float4 inDiffuse : COLOR0,
		 in float2 inTexCoord : TEXCOORD0,
         in float4 inNormal : NORMAL0,
		 out float4 outDiffuse : SV_Target)
{
    float4 px = g_Texture.Sample(g_SamplerState, inTexCoord);
    
    if (ScreenSize.x < 1.0 || ScreenSize.y < 1.0)
    {
        outDiffuse = px;
    }
    else if (px.x > BG_COLOR && px.y > BG_COLOR && px.z > BG_COLOR) 
    {
        outDiffuse.w = 0.0f;
        clip(outDiffuse.w - 0.01);
    }
    else
    {
        float wSla = ScreenSize.y / ScreenSize.x;
        //if (px.x > 0.8f && px.y > 0.8f && px.z > 0.8f)
        //{
        //    outDiffuse = px;
        //}
        //else
        {
            int edge = 0;
            float4 pointPx;
            //float2 deltaBase = float2(1.0 / ScreenSize.x, 1.0 / ScreenSize.y);
        
            float delta = DELTA;
            
	        [unroll]
            for (; edge == 0 && delta < MAX_DELTA;)
            {
	            [unroll]
                for (int j = 0; j < SAMPLE_NUM; j++)
                {
		            // オフセット分ずらしてサンプリング
                    float2 coord = inTexCoord + float2(sin(SAMPLE_INV * j * PI2) * delta * wSla, cos(SAMPLE_INV * j * PI2) * delta);
                    //if (
                    //    coord.x < 0.0f || coord.y < 0.0f
                    //    || coord.x >= 1.0f || coord.y >= 1.0f
                    //)
                    //{
                    //    continue;
                    //}
                    //coord.x = min(max(coord.x, 0.0), 1.0);
                    //coord.y = min(max(coord.y, 0.0), 1.0);
                    coord.x = min(max(coord.x, 0.0 + MAX_DELTA), 1.0 - MAX_DELTA);
                    coord.y = min(max(coord.y, 0.0 + MAX_DELTA), 1.0 - MAX_DELTA);
                    
                    float4 px2 = g_Texture.Sample(g_SamplerState, coord);

                    if (px2.x > BLOOM_COLOR && px2.y > BLOOM_COLOR && px2.z > BLOOM_COLOR)
                    {
                        pointPx = px2;
                        edge++;
                        break; //処理速度 はやめる
                    }
                }
            
                delta += DELTA;

            }
        
            if (edge > 0)
            {
                //outDiffuse = inDiffuse + float4(1.0f, 1.0f, 1.0f, 1.0 - (delta / MAX_DELTA));
                float alpha = 1.0 - (delta / MAX_DELTA);
                //float color = (pointPx.x + pointPx.y + pointPx.z) / 3;
            
                //float4 addPx = px;
                //addPx.x = BLOOM_ADDCOLOR * alpha;
                //addPx.y = BLOOM_ADDCOLOR * alpha;
                //addPx.z = BLOOM_ADDCOLOR * alpha;
            
                //outDiffuse = px + addPx;
                
                //outDiffuse = float4(
                //    pointPx.x + FixedColor.x,
                //    pointPx.y + FixedColor.y,
                //    pointPx.z + FixedColor.z,
                //    BLOOM_MODCOLOR * alpha
                //);
                outDiffuse = float4(
                    pointPx.x + BLOOM_ADDCOLOR,
                    pointPx.y + BLOOM_ADDCOLOR,
                    pointPx.z + BLOOM_ADDCOLOR,
                    BLOOM_MODCOLOR * alpha
                );

                if (outDiffuse.a < 0.1f)
                {
                    outDiffuse.w = 0.0f;
                    clip(outDiffuse.w - 0.01);
                }

            }
            else
            {
                //outDiffuse = float4(0.0f, 0.0f, 0.0f, 1.0f);
                //outDiffuse = px;
                outDiffuse.w = 0.0f;
                clip(outDiffuse.w - 0.01);
            }
        }
    }
    //outDiffuse.w = 1.0f;
}
