#include "common.hlsl"

[maxvertexcount(4)] // 出力する最大頂点数
void main(point PS_IN input[1], inout TriangleStream<PS_IN> output)
{
    float2 offsets[4] =
    {
        float2(-0.5, -0.5), float2(0.5, -0.5),
        float2(-0.5, 0.5), float2(0.5, 0.5)
    };

    for (int i = 0; i < 4; i++)
    {
        PS_IN newVertex;
        newVertex.Position = input[0].Position + float4(offsets[i] * float2(Free.x, Free.y), 0, 0);
        newVertex.TexCoord = offsets[i] + float2(0.5, 0.5); // テクスチャ座標
        newVertex.Diffuse = input[0].Diffuse;
        newVertex.Normal = input[0].Normal;
        newVertex.instanceID = input[0].instanceID;
        output.Append(newVertex);
    }
}