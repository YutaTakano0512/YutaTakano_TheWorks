
#include "common.hlsl"


float3 GetRandomPosition(uint instanceID)
{
    //float3 stageSize = float3(
    //   +1.4f * 96.0f,
    //   +0.3f * 96.0f,
    //   +0.8f * 96.0f * 1000.0f
    //);

    float3 stageSize = float3(
       +0.45f * 96.0f,
       +0.03f * 96.0f,
       +0.45f * 96.0f * 1000.0f
    );

    float seed = instanceID * 123.456; // 固定のシード値
    float3 position;

    // X, Z座標は従来通り
    position.x = frac(sin(seed * 1.2) * 43758.5453) * stageSize.x - stageSize.x / 2;
    position.z = frac(sin(seed * 1.3) * 43758.5453) * stageSize.z - stageSize.z * 0.3;

    // Y座標: Yが大きいほど分布密度を高くする
    float baseY = frac(sin(seed * 1.1) * 43758.5453);
    //position.y = pow(baseY, 4.0) * stageSize.y - stageSize.y * 0.3; // 二次関数を適用
    position.y = (1.0f - pow(baseY, 2.0)) * stageSize.y - stageSize.y * 0.55; // 二次関数を適用

    return position;
}
//float3 GetRandomPosition(uint instanceID)
//{
//    float3 stageSize = float3(
//       +1.4f * 96.0f ,
//       +0.3f * 96.0f ,
//       +0.8f * 96.0f  * 1000.0f
//    );
    
//    float seed = instanceID * 123.456; // 固定のシード値
//    float3 position;
//    //position.x = frac(sin(seed) * 43758.5453) * 20.0 - 10.0;
//    //position.y = frac(sin(seed * 1.1) * 43758.5453) * 5.0;
//    //position.z = frac(sin(seed * 1.3) * 43758.5453) * 20.0 - 10.0; // 高さ調整
//    //position.x = frac(sin(seed * 1.2) * 43758.5453) * stageSize.x - stageSize.x / 2;
//    //position.y = frac(sin(seed * 1.1) * 43758.5453) * stageSize.y - stageSize.y * -0.5; // 高さ調整
//    //position.z = frac(sin(seed * 1.3) * 43758.5453) * stageSize.z - stageSize.z * 0.45;
//    position.x = frac(sin(seed * 1.2) * 43758.5453) * stageSize.x - stageSize.x / 2;
//    position.y = frac(sin(seed * 1.1) * 43758.5453) * stageSize.y - stageSize.y * 0.35; // 高さ調整
//    position.z = frac(sin(seed * 1.3) * 43758.5453) * stageSize.z - stageSize.z * 0.3;
//    return position;
//}
//float3 GetFireflyPosition(uint instanceID, float3 cameraPos)
//{
//    float seed = instanceID * 123.456 + cameraPos.x * 0.1; // カメラの座標をシードとして利用
//    float3 position;
//    position.x = frac(sin(seed) * 43758.5453) * 96.0f * (200.0f) - 96.0f * (100.0f) + cameraPos.x;
//    position.y = frac(sin(seed * 1.1) * 43758.5453) * 96.0f * (200.0f) - 96.0f * (100.0f) + cameraPos.y;
//    position.z = frac(sin(seed * 1.3) * 43758.5453) * 96.0f * -20.0f;
//    return position;
//}


float3 GetAnimatedPosition(uint instanceID, uint instanceIndex, float time)
{
    time += -120.0 * instanceIndex;
    
    float speed = 0.2 + 0.3 * frac(sin(instanceID * 123.566) * 43758.5453); // 移動速度
    float3 movement;
    movement.x = sin(time * 0.02 * speed + instanceID * 0.2) * 2.0f;
    movement.y = sin(time * 0.02 * speed * 1.5) * 0.1f - 0.002f * ((time * speed * 1.2 + instanceID * 17.0f) % 1000.0f);
    movement.z = cos(time * 0.02 * speed + instanceID * 0.1) * 2.0f * 1000.0f;
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
    
    float4 setp = inPosition + Transition + float4(animatedPosition, 1.0) + float4(randPosition, 1.0);
    //Out.Position = inPosition + float4(animatedPosition, 1.0);
    Out.Position = mul(setp, WorldViewProjection);
	
    Out.Diffuse = inDiffuse;
    //Out.Diffuse.a = min(max((setp.y + 0.5f) / 0.5f, 0.0f), 1.0f);
    Out.Diffuse.a = (1.0f - min(max((setp.y - 100.0f) / 1.0f, 0.0f), 1.0f))
    * min(max(length((CameraPos.y + 90.0f) - (setp.z / 16 * 1000.0f))
    / 1.0, 0.0), 1.0f);
    
    Out.TexCoord = inTexCoord;
//    // カメラの方向ベクトルを取得
//    float3 rightVector = normalize(float3(View._11, View._12, View._13)); // カメラの右方向
//    float3 forwardVector = normalize(float3(View._31, View._32, View._33)); // カメラの前方向
//    float3 upVector = normalize(cross(forwardVector, rightVector)); // 本来の上方向を計算

//// スプライトのオフセットを適用
//    float2 offsets[4] =
//    {
//        float2(-0.5, -0.5), float2(0.5, -0.5),
//        float2(0.5, 0.5), float2(-0.5, 0.5)
//    };

//// 90度回転補正を適用
//    float3 offset = offsets[vertexID].x * rightVector + offsets[vertexID].y * forwardVector;
//    float3 finalPosition = inPosition.xyz + Transition.xyz + animatedPosition + randPosition + offset;

//// ワールドビュー射影変換を適用
//    Out.Position = mul(float4(finalPosition, 1.0), WorldViewProjection);
    
    //float4 offset = float4(inPosition.x, inPosition.z, inPosition.y, inPosition.w);
    //Out.Position = mul(offset + Transition + float4(animatedPosition, 1.0) + float4(randPosition, 1.0), WorldViewProjection);
    
    Out.instanceID = instanceID;
}
