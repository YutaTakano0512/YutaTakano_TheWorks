#pragma once
#include "../math/vector2.h"
#include <vector>

class Collider2D;
class Rigidbody2D;
class Sprite;

//=============================================================================
// PhysicsManager クラス (AABBベースの簡易物理エンジン)
//=============================================================================
class PhysicsManager
{
public:
    static void Init();
    static void Uninit();
    static void Clear();
    static void Update(float dt);
    static void DrawDebug();


    // 指定したAABBが、ignoreCollider以外のPHYSICSコライダーと重なっているか判定する
    // checkEnemy=trueにすると、Enemyタグを持つHURTBOXコライダーもチェック対象にする
    static bool CheckOverlap(const Vector2& min, const Vector2& max, Collider2D* ignoreCollider, bool checkEnemy = false);

    // グローバル重力
    static void SetGravity(const Vector2& g) { m_Gravity = g; }
    static Vector2 GetGravity() { return m_Gravity; }

    // コライダー / リジッドボディの登録
    static void AddCollider(Collider2D* col);
    static void RemoveCollider(Collider2D* col);
    
    static void AddRigidbody(Rigidbody2D* rb);
    static void RemoveRigidbody(Rigidbody2D* rb);

private:
    static bool m_ShowDebug;
    static Vector2 m_Gravity;

    static std::vector<Collider2D*>  m_Colliders;
    static std::vector<Rigidbody2D*> m_Rigidbodies;

    static Sprite* m_DebugSprite;

    // 当たり判定 (AABB vs AABB)
    static bool CheckCollisionAABB(
        const Vector2& minA, const Vector2& maxA,
        const Vector2& minB, const Vector2& maxB,
        Vector2& outNormal, float& outDepth);

    // 衝突解決
    static void ResolveCollision(
        Collider2D* colA, Collider2D* colB,
        const Vector2& normal, float depth);
};
