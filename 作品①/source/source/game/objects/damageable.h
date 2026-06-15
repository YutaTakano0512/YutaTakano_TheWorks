#pragma once
#include "../../core/component.h"
#include "damageInfo.h"
#include <functional>

//=============================================================================
// Damageable コンポーネント
// オブジェクトにHPと被ダメージ処理、無敵時間、ヒットストップ、シェイク等を提供する汎用コンポーネント
//=============================================================================
class Damageable : public Component
{
public:
    Damageable() = default;
    virtual ~Damageable() override = default;

    void Init() override;
    void Update(float dt) override;

    // ダメージを受ける処理
    void TakeDamage(const DamageInfo& info);

    // HP設定
    void SetMaxHp(float maxHp) { m_MaxHp = maxHp; m_Hp = maxHp; }
    void SetHp(float hp) { m_Hp = hp; }

    float GetHp() const { return m_Hp; }
    float GetMaxHp() const { return m_MaxHp; }

    bool IsDead() const { return m_Hp <= 0.0f; }
    bool IsInvincible() const { return m_InvincibleTimer > 0.0f || m_IsInvincibleState; }
    bool IsSuperArmor() const { return m_IsSuperArmorState; }

    // 手動で無敵状態を切り替える
    void SetInvincible(bool invincible) { m_IsInvincibleState = invincible; }
    
    // 手動でスーパーアーマー状態を切り替える
    void SetSuperArmor(bool superArmor) { m_IsSuperArmorState = superArmor; }

    // コールバック登録
    void SetOnTakeDamage(std::function<void(const DamageInfo&)> callback) { m_OnTakeDamageCallback = callback; }
    void SetOnDeath(std::function<void()> callback) { m_OnDeathCallback = callback; }

private:
    float m_MaxHp = 10.0f; // 最大HP
    float m_Hp = 10.0f; // 現在HP

    float m_InvincibleDuration = 0.3f; // 被弾後の無敵時間 (秒)
    float m_InvincibleTimer = 0.0f;    // 無敵残り時間

    bool m_IsInvincibleState = false;  // 手動で設定する無敵状態 (スライディング中など)
    bool m_IsSuperArmorState = false;  // 手動で設定するスーパーアーマー状態 (攻撃中など、被ダメージは受けるが怯まない)

    std::function<void(const DamageInfo&)> m_OnTakeDamageCallback; // ダメージを受けたときのコールバック (DamageInfoを引数に取る)
    std::function<void()> m_OnDeathCallback; // 死亡したときのコールバック (引数なし)
};
