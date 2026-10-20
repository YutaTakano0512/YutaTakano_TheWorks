#include "common.hlsl"

float CheckerPattern(float2 uv, float alpha)
{
    //float scale = lerp(15, 15.0, alpha); // alphaが小さいほどスケールを大きく
    //float2 scaledUV = uv * scale;

    //return fmod(floor(scaledUV.x) + floor(scaledUV.y), 2.0) == 0 ? 0.0 : 1.0; // 交互に透明度を設定
    float scale = lerp(1, 8.0, pow(1.0f - alpha, 3.0f)); // alphaが小さいほどスケールを大きく

    // チェック模様の判定
    int checkX = (int) (uv.x / 1) % scale;
    int checkY = (int) (uv.y / 1) % scale;
    
    return checkX == 0 && checkY == 0;
}
//=============================================================================
// ピクセルシェーダ
//=============================================================================
void main(in PS_IN In, out float4 outDiffuse : SV_Target)
{
    outDiffuse =  Begin(In);
    
    //if (outDiffuse.w > 0.01)
    //{
    //    int pX = (int) (In.Position.x * 2);
    //    int pY = (int) (In.Position.y * 2);
    //    if (
    //            (
    //                (pX % 2 == 0 && pY % 2 != 0)
    //                || (pX % 2 != 0 && pY % 2 == 0)
    //                //|| ((int) inPosition.z) % 2 == 0
    //            )
    //        )
    //    {
    //        outDiffuse.w *= outDiffuse.w;
    //    }
    //}
    //if (outDiffuse.w > EPSILON)
    //{
    //    float alpha = outDiffuse.w;
        
    //    float addpos = -0.2f * (float) GameFrame;

    //// 回転行列を適用
    //    float angle = radians(45.0); //回転
    //    float2 rotatedPos;
    //    rotatedPos.x = In.Position.x * cos(angle) - In.Position.y * sin(angle) + addpos;
    //    rotatedPos.y = In.Position.x * sin(angle) + In.Position.y * cos(angle) + addpos;

    //// チェック模様の判定
    //    int checkX = (int) (rotatedPos.x / 15) % 2;
    //    int checkY = (int) (rotatedPos.y / 15) % 2;
    //    int checker = (checkX + checkY) % 2;
        
    //    // 色の補間
    //    float blendedAlpha = outDiffuse.w * outDiffuse.w;
        
    //    outDiffuse.w = checker ? outDiffuse.w : blendedAlpha;
    //}
    
    outDiffuse.w = outDiffuse.w > 0.01 ?
    CheckerPattern(In.Position.xy, outDiffuse.w)
    : 0;
    
    outDiffuse = End(outDiffuse);
 }
