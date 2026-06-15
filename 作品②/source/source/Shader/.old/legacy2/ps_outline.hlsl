#include "common.hlsl"

#define SAMPLE_NUM 6
#define SAMPLE_INV (1.0 / SAMPLE_NUM)
#define PI2 6.2831852

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
void main(in float4 inPosition : POSITION0,
		 in float4 inDiffuse : COLOR0,
		 in float2 inTexCoord : TEXCOORD0,
         in float4 inNormal : NORMAL0,
		 out float4 outDiffuse : SV_Target)
{
    float4 px = g_Texture.Sample(g_SamplerState, inTexCoord);
    
    
    //outDiffuse = px;
    if (ScreenSize.x < 1.0 || ScreenSize.y < 1.0)
    {
        outDiffuse = px;
    }
    else if (px.x > 0.1 || px.y > 0.1 || px.z > 0.1) //(distance(px, BLOOM_COLOR) < EPSILON)
    //else if (px.x <= 0.9 || px.y <= 0.9 || px.z <= 0.9) //(distance(px, BLOOM_COLOR) < EPSILON)
    {
        outDiffuse.w = 0.0f;
        clip(outDiffuse.w - 0.01);
    }
    else
    {
        float wSla = ScreenSize.y / ScreenSize.x;
        {
            //float depthtmp = 0;
            float4 pxOutl;
            int edge = 0;
        
            float delta = DELTA;
            
	        [unroll]
            for (; edge == 0 && delta < MAX_DELTA;)
            {
	            [unroll]
                for (int j = 0; j < SAMPLE_NUM; j++)
                {
		            // オフセット分ずらしてサンプリング
                    float2 coord = inTexCoord + float2(sin(SAMPLE_INV * j * PI2) * delta * wSla, cos(SAMPLE_INV * j * PI2) * delta);
                    coord.x = min(max(coord.x, 0.0 + MAX_DELTA), 1.0 - MAX_DELTA);
                    coord.y = min(max(coord.y, 0.0 + MAX_DELTA), 1.0 - MAX_DELTA);
                    
                    float4 px2 = g_Texture.Sample(g_SamplerState, coord);
                    
                    if (px2.x > 0.1 || px2.y > 0.1 || px2.z > 0.1) // (distance(px2, BLOOM_COLOR) < EPSILON)
                    //if (px2.x <= 0.9 || px2.y <= 0.9 || px2.z <= 0.9) // (distance(px2, BLOOM_COLOR) < EPSILON)
                    {
                        //depthtmp = px2.z;
                        pxOutl = px2;
                        edge++;
                        break; //処理速度 はやめる
                    }
                }
            
                delta += DELTA;

            }
        
            if (edge > 0)
            {
                //if (depthtmp >= 0.2)
                //{
                //    outDiffuse = float4(0.2f, 0.2f, 0.3f, 1.0f);
                //}
                //else
                //{
                //    outDiffuse = float4(0.5f, 0.5f, 0.6f, 1.0f);
                //}
                outDiffuse = pxOutl;
                //outDiffuse = float4(0.5f, 0.5f, 0.6f, 1.0f);
                
                outDiffuse.w = 1.0f;
            }
            else
            {
                outDiffuse.w = 0.0f;
                clip(outDiffuse.w - 0.01);
            }
        }
    }
}
