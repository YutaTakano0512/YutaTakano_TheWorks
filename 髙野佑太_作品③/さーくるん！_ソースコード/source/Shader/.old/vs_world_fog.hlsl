
#include "common.hlsl"


float3 GetRandomPosition(uint instanceID)
{
    //float3 stageSize = float3(
    //   +1.4f * 96.0f,
    //   +0.3f * 96.0f,
    //   +0.8f * 96.0f * 1000.0f
    //);

    float3 stageSize = float3(
       +0.8f * 96.0f,
       +0.15f * 96.0f,
       +0.65f * 96.0f * 1000.0f
    );

    float seed = instanceID * 123.456; // 固定のシード値
    float3 position;

    // X, Z座標は従来通り
    position.x = frac(sin(seed * 1.2) * 43758.5453) * stageSize.x - stageSize.x / 2;
    position.z = frac(sin(seed * 1.3) * 43758.5453) * stageSize.z - stageSize.z * 0.3;

    // Y座標: Yが大きいほど分布密度を高くする
    float baseY = frac(sin(seed * 1.1) * 43758.5453);
    //position.y = pow(baseY, 4.0) * stageSize.y - stageSize.y * 0.3; // 二次関数を適用
    position.y = (1.0f - pow(baseY, 2.0)) * stageSize.y + stageSize.y * 0.1; // 二次関数を適用

    return position;
}
//float3 GetRandomPosition(uint instanceID)
//{
//    float3 stageSize = float3(
//       +300.0f  * 96.0f,
//       +200.0f * 96.0f,
//       +20.0f * 96.0f
//    );

//    float seed = instanceID * 123.456; // 固定のシード値
//    float3 position;
    
//    position.x = frac(sin(seed * 1.2) * 43758.5453) * stageSize.x - stageSize.x / 2;
//    position.y = frac(sin(seed * 1.3) * 43758.5453) * stageSize.y - stageSize.y * 0.3;

//    // Y座標: Yが大きいほど分布密度を高くする
//    float baseY = frac(sin(seed * 1.1) * 43758.5453);
//    position.z = (1.0f - pow(baseY, 2.0)) * stageSize.z + stageSize.z * 0.25; // 二次関数を適用

//    return position;
//}

//float3 GetAnimatedPosition(uint instanceID, uint instanceIndex, float time)
//{
//    time += -20.0 * instanceIndex;
    
//    float speed = 0.2 + 0.3 * frac(sin(instanceID * 123.566) * 43758.5453); // 移動速度
//    float3 movement;
//    movement.x = sin(time * 0.004 * speed + instanceID * 0.2) * 20.0f;
//    movement.y = cos(time * 0.004 * speed + instanceID * 0.1) * 20.0f;
//    movement.z = sin(time * 0.005 * speed * 1.5) * 0.1f - 3.0f * ((time * speed * 1.2 + instanceID * 17.0f) % 1000.0f) / 1000;
//    return movement;
//}

float3 GetAnimatedPosition(uint instanceID, uint instanceIndex, float time)
{
    time += -80.0 * instanceIndex;
    
    float speed = 0.2 + 0.3 * frac(sin(instanceID * 123.566) * 43758.5453); // 移動速度
    float3 movement;
    movement.x = sin(time * 0.004 * speed + instanceID * 0.2) * 5.2f;
    movement.y = sin(time * 0.004 * speed * 1.5) * 0.1f - 0.006f * ((time * speed * 1.2 + instanceID * 17.0f) % 1000.0f);
    movement.z = cos(time * 0.005 * speed + instanceID * 0.1) * 5.2f * 1000.0f;
    return movement;
}


//=============================================================================
// 頂点シェーダ
//=============================================================================
void main(in float4 inPosition : POSITION0,
		in float4 inDiffuse : COLOR0,
		in float2 inTexCoord : TEXCOORD0,
        in float4 inNormal : NORMAL0,

        uint vertexID : SV_VertexID,
        uint instanceID : SV_InstanceID,

        out PS_IN Out
)
{
    int id = instanceID / 3;
    int index = instanceID % 3;
    
    // ランダムな位置を適用
    float time = (float)GameFrame; // GPU側で時間管理
    float3 randPosition = GetRandomPosition(id);
    float3 animatedPosition = GetAnimatedPosition(id, index, time);
    
    //float4 setp = inPosition + Transition;
    //Out.Position = mul(setp, WorldViewProjection)
    // + float4(animatedPosition, 1.0) + float4(randPosition, 1.0);
	
    float4 setp = inPosition + Transition + float4(animatedPosition, 1.0) + float4(randPosition, 1.0);
    Out.Position = mul(setp, WorldViewProjection);
	
    Out.Diffuse = inDiffuse;
    Out.Diffuse.a = min(max(length((CameraPos.y + 90.0f) - (setp.y / 16 * 1.0f))
    / 1.0, 0.0), 1.0f)
    * max(pow(min(max(length(setp.xy) / (96.0f * 80), 0.0), 1.0f), 5.0f), 0.3f);
    
    Out.TexCoord = inTexCoord;
    
    Out.instanceID = instanceID;
}
