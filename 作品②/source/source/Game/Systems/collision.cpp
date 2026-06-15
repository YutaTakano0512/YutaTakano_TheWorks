// ===================================================
// collision.cpp 当たり判定
// 
// hara sougo  2024/07/22
// ===================================================
#include "../../game_main.h"
#include "collision.h"

// ===================================================
// バウンディングボックスの当たり判定
// 
// 引数:
// 	矩形Ａの中心座標
// 	矩形Ｂの中心座標
// 	矩形Ａのサイズ
// 	矩形Ｂのサイズ
//
// 戻り値
//  true：当たっている
// 	false：当たっていない
// ===================================================
bool CheckBoxCollider(Float2 PosA, Float2 PosB, Float2 SizeA, Float2 SizeB)
{
	float ATop		= PosA.y - SizeA.y / 2;
	float ABottom	= PosA.y + SizeA.y / 2;
	float ARight	= PosA.x + SizeA.x / 2;
	float ALeft		= PosA.x - SizeA.x / 2;

	float BTop		= PosB.y - SizeB.y / 2;
	float BBottom	= PosB.y + SizeB.y / 2;
	float BRight	= PosB.x + SizeB.x / 2;
	float BLeft		= PosB.x - SizeB.x / 2;

	if (
		(ARight >= BLeft)
		&&
		(ALeft <= BRight)
		&&
		(ABottom >= BTop)
		&&
		(ATop <= BBottom)
	)
	{
		// 当たっている
		return true;
	}

	// 当たっていない
	return false;
}

// ===================================================
// バウンディングサークルの当たり判定
// 
// 引数:
// 	円１の中心座標
// 	円２の中心座標
// 	円１の半径
// 	円２の半径
//
// 戻り値
//  true：当たっている
// 	false：当たっていない
// ===================================================
bool CheckCircleCollider(Float2 PosA, Float2 PosB, float rA, float rB)
{
	//距離の二乗
	float distance = 0.0f;
	//半径の合計の二乗
	float rSunm = -1.0f;

	{
		//位置の差
		float disX = (PosA.x - PosB.x);
		float disY = (PosA.y - PosB.y);
		distance = disX * disX + disY * disY;
	}
	{
		//半径の合計
		float rSum = (rA + rB);
		//半径の合計の二乗
		rSunm = rSum * rSum;
	}

	if (distance <= rSunm)
	{
		// 当たっている
		return true;
	}

	// 当たっていない
	return false;
}


//線の判定
bool CheckLineCollider(Float2 a, Float2 b, Float2 c, Float2 d)
{
	double s, t;
	s = (a.x - b.x) * (c.y - a.y) - (a.y - b.y) * (c.x - a.x);
	t = (a.x - b.x) * (d.y - a.y) - (a.y - b.y) * (d.x - a.x);
	if (s * t > 0)
		return false;

	s = (c.x - d.x) * (a.y - c.y) - (c.y - d.y) * (a.x - c.x);
	t = (c.x - d.x) * (b.y - c.y) - (c.y - d.y) * (b.x - c.x);
	if (s * t > 0)
		return false;
	return true;
}

//線と箱の判定
bool CheckLineBoxCollider(Float2 a, Float2 b, Float2 pos, Float2 size)
{
    if (CheckBoxCollider(a, pos, MkF2(0.0f, 0.0f), size)
        && CheckBoxCollider(b, pos, MkF2(0.0f, 0.0f), size) )
    {
        return true;
    }

	Float2 p[4][2] = {
		{ MkF2(pos.x, pos.y), MkF2(pos.x + size.x, pos.y) },
		{ MkF2(pos.x, pos.y), MkF2(pos.x, pos.y + size.y) },
		{ MkF2(pos.x + size.x, pos.y), MkF2(pos.x + size.x, pos.y + size.y) },
		{ MkF2(pos.x, pos.y + size.y), MkF2(pos.x + size.x, pos.y + size.y) }
	};
	for (int i = 0; i < 4; i++)
	{
		if (CheckLineCollider(a, b, p[i][0], p[i][1]))
		{
			return true;
		}
	}
	return false;
}

// 点が三角形内にあるか判定する関数（バリセントリック座標を使用）
bool CheckPointTriangleCollider(Float3 p, Float3 v0, Float3 v1, Float3 v2) 
{
    // 三角形の各辺のベクトル
    Float3 v0v1 = { v1.x - v0.x, v1.y - v0.y, v1.z - v0.z };
    Float3 v0v2 = { v2.x - v0.x, v2.y - v0.y, v2.z - v0.z };
    Float3 v0p = { p.x - v0.x, p.y - v0.y, p.z - v0.z };

    // 面積（クロスプロダクトを利用）
    float d00 = v0v1.x * v0v1.x + v0v1.y * v0v1.y + v0v1.z * v0v1.z;
    float d01 = v0v1.x * v0v2.x + v0v1.y * v0v2.y + v0v1.z * v0v2.z;
    float d11 = v0v2.x * v0v2.x + v0v2.y * v0v2.y + v0v2.z * v0v2.z;
    float d20 = v0p.x * v0v1.x + v0p.y * v0v1.y + v0p.z * v0v1.z;
    float d21 = v0p.x * v0v2.x + v0p.y * v0v2.y + v0p.z * v0v2.z;

    // バリセントリック座標を計算
    float denom = d00 * d11 - d01 * d01;
    float v = (d11 * d20 - d01 * d21) / denom;
    float w = (d00 * d21 - d01 * d20) / denom;

    // 点が三角形の内部なら true を返す
    return (v >= 0) && (w >= 0) && (v + w <= 1);
}

//
//bool CapsuleCapsuleIntersect(COLLIDER_CAPSULE cap1, COLLIDER_CAPSULE cap2)
//{
//    // ここで【2線分間の最短距離】を求めるためのパラメータ s, t と最短距離の二乗を算出する。
//    // 具体的な数式は「線分同士の最短距離求めるアルゴリズム」を参照してください。
//    float s, t;
//    float sqrDist = ComputeSquaredDistanceBetweenSegments(cap1.pointA, cap1.pointB,
//        cap2.pointA, cap2.pointB, s, t);
//    float radiusSum = cap1.radius + cap2.radius;
//    return (sqrDist <= radiusSum * radiusSum);
//}


// 内積を求める関数
//static float Float3Dot(const Float3& a, const Float3& b) {
//    return a.x * b.x + a.y * b.y + a.z * b.z;
//}
//// ベクトルの引き算
//static Float3 operator-(const Float3& a, const Float3& b) {
//    return { a.x - b.x, a.y - b.y, a.z - b.z };
//}
//
//// ベクトルの加算
//static Float3 operator+(const Float3& a, const Float3& b) {
//    return { a.x + b.x, a.y + b.y, a.z + b.z };
//}
//
//// ベクトルとスカラーの乗算
//static Float3 operator*(const Float3& v, float t) {
//    return { v.x * t, v.y * t, v.z * t };
//}





// カプセルの線分上の最も近い点を返す関数
static Float3 ClosestPointOnSegment(const Float3& point, const Float3& A, const Float3& B)
{
    Float3 AB = B - A;
    Float3 AP = point - A;
    float dotAP_AB = Float3Dot(AP, AB);
    float dotAB_AB = Float3Dot(AB, AB);
    float t = dotAP_AB / dotAB_AB;
    // 0～1にクランプ
    t = std::max(0.0f, std::min(t, 1.0f));
    return A + AB * t;
}



// 線分 AB 上の最も近い点と点 point との二乗距離を計算する関数
static float SquaredDistancePointToSegment(const Float3& point, const Float3& A, const Float3& B)
{
    // AB = B - A, AP = point - A
    Float3 AB = B - A;
    Float3 AP = point - A;

    // t = (AP ・ AB) / (AB ・ AB)
    float dotAP_AB = Float3Dot(AP, AB);
    float dotAB_AB = Float3Dot(AB, AB);

    float t = dotAP_AB / dotAB_AB;
    // t を 0～1 の範囲にクランプ
    t = std::max(0.0f, std::min(t, 1.0f));

    // 線分上の最も近い点 Q = A + t * AB
    Float3 closestPoint = A + (AB * t);

    // 点 point と Q の差を求め、その二乗距離を計算
    Float3 diff = point - closestPoint;
    return Float3Dot(diff, diff);
}


// 球とカプセルの衝突判定関数
// sphereCenter : 球体の中心座標
// sphereRadius : 球体の半径
// capsuleA, capsuleB : カプセルの線分の両端の座標（カプセルの中心線）
// capsuleRadius : カプセルの半径（両端の半球と円筒部分の外側の半径）
bool CheckSphereCapsuleCollision(
    const Float3& sphereCenter, float sphereRadius,
    FloatCapsule capsule
)
{
    // 球の中心からカプセルの中心線までの最短距離の二乗を求める
    float sqDist = SquaredDistancePointToSegment(sphereCenter, capsule.pointA, capsule.pointB);

    // 衝突判定のためのしきい値は、球体とカプセルの半径の和の二乗
    float combinedRadius = sphereRadius + capsule.radius;
    return sqDist <= (combinedRadius * combinedRadius);
}

// 衝突解決：球がカプセルに衝突している場合、球の中心を補正し、衝突していない状態にする
bool ResolveSphereCapsuleCollision(
    Float3& sphereCenter, float sphereRadius,
    FloatCapsule capsule
)
{
    // 衝突していなければ何もせず返す
    if (!CheckSphereCapsuleCollision(sphereCenter, sphereRadius, capsule))
        return false;

    // カプセルの線分上の球の中心に最も近い点を求める
    Float3 closestPoint = ClosestPointOnSegment(sphereCenter, capsule.pointA, capsule.pointB);
    Float3 diff = sphereCenter - closestPoint;
    float sqrDist = Float3Dot(diff, diff);
    float dist = std::sqrt(sqrDist);
    float combinedRadius = sphereRadius + capsule.radius;

    // 球の中心とカプセルの最も近い点との距離が 0 の場合、方向が定まらないので任意の方向に補正する
    if (dist == 0.0f) {
        // 例として x 方向にずらす
        diff = { 1.0f, 0.0f, 0.0f };
        dist = 1.0f;
    }

    //dist *= 10.0f;

    // 不足している（押し出すべき）距離の計算
    float penetrationDepth = combinedRadius - dist;
    // 補正方向：球の中心から最近接点へ向かうベクトルの正規化したものの逆方向
    Float3 direction = diff * (1.0f / dist);

    // 球の中心を補正。これにより球がカプセルの外に出る（接してちょうど外側になる）
    Float3 beforepos = sphereCenter;
    sphereCenter = sphereCenter + direction * penetrationDepth;
    sphereCenter.z = beforepos.z;

    return true;
}

