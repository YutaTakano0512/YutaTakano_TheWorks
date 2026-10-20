#include "common.hlsl"

//Copilotにつけてもらったコメント付き

//#define SAMPLE_NUM 6
#define SAMPLE_NUM 32  // 友達は多いほうがいい（サンプル数）
#define SAMPLE_INV (1.0 / SAMPLE_NUM)  // 逆数は偉大
#define PI2 6.2831852f  // 2π！円の人生！

#define BG_COLOR 0.1  // ちょっと薄暗いけど、これは演出だ
#define BLOOM_COLOR float4(1.0f, 0.0f, 1.0f, 1.0f)  // 眩しいピンク、主役級の輝き！

void main(in PS_IN In, out float4 outDiffuse : SV_Target)
{
    float4 colorraw = g_Texture.Sample(g_SamplerState, In.TexCoord);  
    float3 color = colorraw.rgb;  // 元の色、すべての始まり

    float4 outline = float4(0, 0, 0, 0);  // エッジ発見ミッション開始！
    for (int i = 0; i < SAMPLE_NUM; i++)
    {
        // ランダムに見えるけど、しっかり計算されたサークル
        float2 offset = float2(
            sin(SAMPLE_INV * i * PI2),  // X方向のスピン
            cos(SAMPLE_INV * i * PI2)   // Y方向のスピン
        );

        // 画面を飛び出さないように注意しながらチェックポイント作成
        float2 sampleCoord = In.TexCoord + (offset * Free.x) / ScreenSize;

        // 画面の範囲チェック、無許可の脱出は許さない！
        float isInsideScreen = all(sampleCoord >= float2(0.0, 0.0)) * all(sampleCoord <= float2(1.0, 1.0));

        float4 neighborColor = g_Texture.Sample(g_SamplerState, sampleCoord) * isInsideScreen;  // 隣人とのご挨拶
        float isNotNone = neighborColor.a;  // 透明じゃないならカウント

        outline += isInsideScreen * isNotNone * neighborColor;  // 画面にいるならアウトラインとして加算

        if (outline.a > 0.1f)  
            break;  // 最初の有効な色を見つけたら旅はここで終了
    }

    outDiffuse = float4(outline.r, outline.g, outline.b, 1.0);  // エッジの輪郭をくっきり表示！

    // 差が小さいならちょっとくすませてシャープさを演出
    outDiffuse.rgb += step(length(outDiffuse.rgb - colorraw.rgb), 0.2) * -0.4;  

    outDiffuse.w = outline.a * (1.0f - colorraw.a);  // 元の透明度と相談しながら描画
    clip(outDiffuse.w - 0.1);  // ぼんやりした部分は潔くカット！
}