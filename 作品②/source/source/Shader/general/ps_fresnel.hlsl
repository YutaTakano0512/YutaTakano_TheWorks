#include "common.hlsl"

//=============================================================================
// ピクセルシェーダ
//=============================================================================
void main(in PS_IN In, out float4 outDiffuse : SV_Target)
{
    outDiffuse =  Begin(In);
    
    half _F0 = 0.12f;
    
    half3 viewDir = normalize(half3(
            (In.Position.x - CameraPos.x) * 1,
            (In.Position.y - CameraPos.y) * 1,
            (In.Position.z - CameraPos.z) * 1
        ));
    
    half fresnel = _F0 + (1.0h - _F0) * pow(1.0h - dot(viewDir, In.Normal.xyz), 2);
        
    float4 colo = fresnel;
    colo.x = 1.0f - colo.x;
    colo.y = 1.0f - colo.y;
    colo.z = 1.0f - colo.z;
    if ((colo.x < 0.2 && colo.y < 0.2 && colo.z < 0.2))
    {
        outDiffuse.w = 0;
    }
    else
    {
        outDiffuse.w = 1.0f - (colo.x / 0.8f);
        outDiffuse.w *= pow((1.0f - outDiffuse.w) + 1.0f, 3);
    }
    
    outDiffuse = End(outDiffuse);
 }
