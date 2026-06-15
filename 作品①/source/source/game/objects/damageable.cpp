#include "damageable.h"
#include "../../core/gameObject.h"
#include "../../core/manager.h"
#include "../../physics/rigidbody2D.h"
#include "../../camera/camera2D.h"

//=============================================================================
// 初期化
//=============================================================================

void Damageable::Init()
{
    m_Hp = m_MaxHp;
    m_InvincibleTimer = 0.0f;
}

//=============================================================================
// 更新
//=============================================================================

void Damageable::Update(float dt)
{
    // 無敵時間のカウントダウン
    if (m_InvincibleTimer > 0.0f)
    {
        m_InvincibleTimer -= dt;
        if (m_InvincibleTimer < 0.0f)
        {
            m_InvincibleTimer = 0.0f;
        }
    }
}

void Damageable::TakeDamage(const DamageInfo& info)
{
    // 既に死亡しているか、無敵時間中の場合はダメージを受けない
    if (IsDead() || IsInvincible())
    {
        return;
    }

    // HPの減少
    m_Hp -= info.damage;
    if (m_Hp < 0.0f)
    {
        m_Hp = 0.0f;
    }

    // 無敵時間を開始
    m_InvincibleTimer = m_InvincibleDuration;

    // 1. ヒットストップ（シーンの更新を一時停止）の適用
    if (info.hitstopDuration > 0.0f)
    {
        Manager::SetHitStop(info.hitstopDuration);
    }

    // 2. 被弾したオブジェクトのヒットシェイク（ブレ）の適用
    if (!m_IsSuperArmorState && info.shakeDuration > 0.0f && m_GameObject)
    {
        m_GameObject->transform.Shake(info.shakeIntensity, info.shakeDuration);
    }

    // 3. カメラシェイクの適用
    if (info.cameraShakeDuration > 0.0f)
    {
        // "Camera" タグのゲームオブジェクトを検索
        auto cameras = Manager::GetScene()->FindGameObjectsWithTag("Camera");
        for (size_t i = 0; i < cameras.size(); ++i)
        {
            if (cameras[i])
            {
                Camera2D* camera2D = cameras[i]->GetComponent<Camera2D>();
                if (camera2D)
                {
                    camera2D->Shake(info.cameraShakeIntensity, info.cameraShakeDuration);
                }
            }
        }
    }

    // 4. ノックバック（吹き飛び）の適用
    if (!m_IsSuperArmorState && m_GameObject)
    {
        Rigidbody2D* rb = m_GameObject->GetComponent<Rigidbody2D>();
        if (rb)
        {
            // ノックバックベクトルを速度に直接代入
            rb->velocity = info.knockback;
        }
    }

    // コールバック関数が登録されていれば呼び出す
    if (m_OnTakeDamageCallback)
    {
        m_OnTakeDamageCallback(info);
    }

    if (IsDead())
    {
        if (m_OnDeathCallback)
        {
            m_OnDeathCallback();
        }
    }
}
