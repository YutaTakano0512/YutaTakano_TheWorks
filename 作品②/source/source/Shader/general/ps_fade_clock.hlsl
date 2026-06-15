#include "common.hlsl"

//=============================================================================
// ピクセルシェーダ
//=============================================================================
void main(in PS_IN In, out float4 outDiffuse : SV_Target)
{
    outDiffuse = Begin(In);
    
    float4 result = outDiffuse;
    
        // テクスチャの中心座標を取得
    float2 center = float2(0.5, 0.5);
    
    // ピクセル座標から中心までの角度を計算
    float2 dir = In.TexCoord - center;
    //float angle = atan2(dir.y, dir.x); // -π ～ π の範囲
    float angle = atan2(dir.x, dir.y); // -π ～ π の範囲
    
    // 角度を 0 ～ 1 の範囲に正規化
    float normalizedAngle = (angle + 3.14159265359) / (2.0 * 3.14159265359);
    
    // フェード値と比較してアルファを設定
    float alpha = (normalizedAngle < Free.x) ? 1.0 : 0.0;

    result.a *= alpha; // 白色 + アルファ値

    outDiffuse = result;
    
    outDiffuse = End(outDiffuse);
}
