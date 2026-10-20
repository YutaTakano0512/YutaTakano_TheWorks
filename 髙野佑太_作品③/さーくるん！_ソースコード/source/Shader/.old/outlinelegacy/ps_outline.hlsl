#include "common.hlsl"

//#define SAMPLE_NUM 6
#define SAMPLE_NUM 24
#define SAMPLE_INV (1.0 / SAMPLE_NUM)
#define PI2 6.2831852f

#define BG_COLOR 0.1
//#define MAX_DELTA 0.008
//#define MAX_DELTA 0.006
//#define MAX_DELTA 0.007
//#define MAX_DELTA 0.003
//#define MAX_DELTA 0.004
//#define MAX_DELTA 0.006
#define MAX_DELTA 8.0f
//#define DELTA 0.003
//#define DELTA 0.004
//#define DELTA 0.001
#define DELTA 1.0f

#define BLOOM_COLOR float4(1.0f, 0.0f, 1.0, 1.0)

//=============================================================================
// ピクセルシェーダ
//=============================================================================
void main(in PS_IN In, out float4 outDiffuse : SV_Target)
{
 //   float4 px = g_Texture.Sample(g_SamplerState, In.TexCoord);
    
 //   float isBackground = min(step(BG_COLOR, px.x) + step(BG_COLOR, px.y) + step(BG_COLOR, px.z), 1.0f);
    
 //   //float wSla = ScreenSize.y / ScreenSize.x;
    
 //   float4 pointPx = float4(0, 0, 0, 0);
    
 //   float delta = DELTA;
    
	//[unroll]
 //   for (; pointPx.w <= EPSILON && delta < MAX_DELTA; delta += DELTA)
 //   {
	//    [unroll]
 //       for (int j = 0; j < SAMPLE_NUM; j++)
 //       {
 //           float2 offset = float2(
 //               sin(SAMPLE_INV * j * PI2) * delta / ScreenSize.x,
 //               cos(SAMPLE_INV * j * PI2) * delta / ScreenSize.y
 //           );
 //           //float2 coord = clamp(
 //           //    In.TexCoord + offset,
 //           //    //float2(MAX_DELTA, MAX_DELTA),
 //           //    //float2(1.0 - MAX_DELTA, 1.0 - MAX_DELTA)
 //           //    float2(MAX_DELTA / ScreenSize.x, MAX_DELTA / ScreenSize.y),
 //           //    float2(1.0 - MAX_DELTA / ScreenSize.x, 1.0 - MAX_DELTA / ScreenSize.y)
 //           //);
 //           float2 coord = In.TexCoord + offset;
 //           float2 minBound = MAX_DELTA / ScreenSize;
 //           float2 maxBound = 1.0 - minBound;

 //           if (any(coord < minBound) || any(coord >= maxBound))
 //           {
 //               continue;
 //           }
            
 //           float4 px2 = g_Texture.Sample(g_SamplerState, coord);
            
 //           //if (px2.x > 0.1f || px2.y > 0.1f || px2.z > 0.1f)
 //           if (dot(px2.rgb, float3(0.299, 0.587, 0.114)) > 0.1f)
 //           {
 //               pointPx = px2;
 //               break; //ループ終了
 //           }
 //       }
 //   }
    
 //   float isEdge = step(EPSILON, pointPx.w);
    
 //   outDiffuse = lerp(
 //   float4(0.0f, 0.0f, 0.0f, 0.0f),
 //   pointPx, 
 //   (1.0f - isBackground) * isEdge);
 //   clip(outDiffuse.w - 0.01);
    
    float4 colorraw = g_Texture.Sample(g_SamplerState, In.TexCoord);
    float3 color = colorraw.rgb;
    
    // オフセットサンプリング
    //float2 offsets[8] =
    //{
    //    float2(-1, 0), float2(1, 0),
    //    float2(0, -1), float2(0, 1),
    //    float2(-1, -1), float2(1, 1),
    //    float2(-1, 1), float2(1, -1)
    //};

    float4 outline = float4(0, 0, 0, 0);
    for (int i = 0; i < SAMPLE_NUM; i++)
    {
        // オフセット座標を計算
        float2 offset = float2(
            sin(SAMPLE_INV * i * PI2),
            cos(SAMPLE_INV * i * PI2)
        );

        // 画面外判定用のサンプリング座標
        float2 sampleCoord = In.TexCoord + (offset * MAX_DELTA) / ScreenSize;
        
        // UV座標の範囲チェックを数値ベースで処理
        float isInsideScreen = all(sampleCoord >= float2(0.0, 0.0)) * all(sampleCoord <= float2(1.0, 1.0));

        float4 neighborColor = g_Texture.Sample(g_SamplerState, sampleCoord) * isInsideScreen;
        
        // 黒色かどうかを判定（輝度が閾値以下なら黒とみなす）
        //float isNotNone = step(BG_COLOR / 3, length(neighborColor));
        
        float isNotNone = neighborColor.a;

        // 画面外でない場合のみ加算
        outline += isInsideScreen * isNotNone * neighborColor;
    }
    
    outDiffuse = float4(outline.r, outline.g, outline.b, 1.0); // エッジを表示
    
    //// 黒色の場合は透明にする（アルファ値を0）
    //outDiffuse.w = (1.0 - step( // 黒色判定（色の合計が低い場合）
    //    length(outDiffuse.rgb),
    //    BG_COLOR / 3 // 黒とみなす閾値
    //)) * step( //自身の元ピクセルが黒であるか
    //    length(color.rgb),
    //    BG_COLOR / 3 // 黒とみなす閾値
    //);
    
    outDiffuse.w = outline.a * (1.0f - colorraw.a);
    clip(outDiffuse.w - 0.1);
}
