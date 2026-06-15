#pragma once
#include "../core/component.h"
#include "animationClip.h"
#include <unordered_map>
#include <string>

class SpriteRenderer;
class BoxCollider2D;

//=============================================================================
// Animator コンポーネント
// SpriteRendererと連携し、AnimationClipに従ってUV領域(フレーム)を切り替える
//=============================================================================
class Animator : public Component
{
public:
    // スプライトシートの分割数 (例: 4x4)
    int colCount = 1;
    int rowCount = 1;

    void Init() override;
    void Update(float dt) override;

    // クリップの追加
    void AddClip(const AnimationClip& clip);

    // アニメーション再生
    void Play(const std::string& clipName, bool forceRestart = false);

    // 現在再生中のクリップ名を取得
    std::string GetCurrentClipName() const;

    // 再生中かどうか
    bool IsPlaying() const { return m_IsPlaying; }

    // アニメーションが終了したか (ループしない場合)
    bool IsFinished() const;

    // フレーム単位のHitbox自動制御に使用するコライダーを登録する
    // flipX を参照するため SpriteRenderer も必要 (Animatorが内部で取得)
    void SetHitbox(BoxCollider2D* hitbox) { m_Hitbox = hitbox; }

private:
    SpriteRenderer*  m_SpriteRenderer = nullptr;
    BoxCollider2D*   m_Hitbox         = nullptr; // フレーム単位で制御するHitbox

    std::unordered_map<std::string, AnimationClip> m_Clips;
    AnimationClip* m_CurrentClip = nullptr;

    int   m_CurrentFrameIndex = 0;
    float m_Timer = 0.0f;
    bool  m_IsPlaying = false;

    void ApplyFrame();
};
