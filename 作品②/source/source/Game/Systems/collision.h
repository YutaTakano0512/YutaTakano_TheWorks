// ===================================================
// collision.h 当たり判定
// 
// hara sougo  2024/07/22
// ===================================================
#ifndef _COLLISION_H_
#define _COLLISION_H_


#include "sys_general.h"


// ===================================================
// プロトタイプ宣言
// ===================================================
bool CheckBoxCollider(Float2 PosA, Float2 PosB, Float2 SizeA, Float2 SizeB);	// バウンディングボックスの当たり判定
bool CheckCircleCollider(Float2 PosA, Float2 PosB, float rA, float rB);		// バウンディングサークルの当たり判定

bool CheckLineCollider(Float2 a, Float2 b, Float2 c, Float2 d);
bool CheckLineBoxCollider(Float2 a, Float2 b, Float2 pos, Float2 size);

bool CheckPointTriangleCollider(Float3 p, Float3 v0, Float3 v1, Float3 v2);


// 球とカプセルの衝突判定関数
// sphereCenter : 球体の中心座標
// sphereRadius : 球体の半径
// capsuleA, capsuleB : カプセルの線分の両端の座標（カプセルの中心線）
// capsuleRadius : カプセルの半径（両端の半球と円筒部分の外側の半径）
bool CheckSphereCapsuleCollision(
    const Float3& sphereCenter, float sphereRadius,
    FloatCapsule capsule
);

// 衝突解決：球がカプセルに衝突している場合、球の中心を補正し、衝突していない状態にする
bool ResolveSphereCapsuleCollision(
    Float3& sphereCenter, float sphereRadius,
    FloatCapsule capsule
);

#endif