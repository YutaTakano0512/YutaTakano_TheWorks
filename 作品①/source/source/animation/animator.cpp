#include "animator.h"
#include "../graphics/spriteRenderer.h"
#include "../core/gameObject.h"
#include "../physics/boxCollider2D.h"

//=============================================================================
// Animator 実装
//=============================================================================

void Animator::Init()
{
    // 同一GameObjectのSpriteRendererを取得
    m_SpriteRenderer = m_GameObject->GetComponent<SpriteRenderer>();
}

void Animator::AddClip(const AnimationClip& clip)
{
    m_Clips[clip.name] = clip;
}

void Animator::Play(const std::string& clipName, bool forceRestart)
{
    auto it = m_Clips.find(clipName);
    if (it == m_Clips.end()) return;

    if (!forceRestart && m_CurrentClip == &it->second && m_IsPlaying)
    {
        return; // 既に同じクリップを再生中
    }

    m_CurrentClip = &it->second;
    m_CurrentFrameIndex = 0;
    m_Timer = 0.0f;
    m_IsPlaying = true;

    ApplyFrame();
}

void Animator::Update(float dt)
{
    if (!m_IsPlaying || !m_CurrentClip || m_CurrentClip->frames.empty())
        return;

    m_Timer += dt;

    const auto& currentFrame = m_CurrentClip->frames[m_CurrentFrameIndex];

    if (m_Timer >= currentFrame.duration)
    {
        m_Timer -= currentFrame.duration;
        m_CurrentFrameIndex++;

        if (m_CurrentFrameIndex >= (int)m_CurrentClip->frames.size())
        {
            if (m_CurrentClip->loop)
            {
                m_CurrentFrameIndex = 0;
            }
            else
            {
                m_CurrentFrameIndex = (int)m_CurrentClip->frames.size() - 1;
                m_IsPlaying = false; // ループなしの終端
            }
        }

        ApplyFrame();
    }
}

void Animator::ApplyFrame()
{
    if (!m_SpriteRenderer || !m_CurrentClip || m_CurrentClip->frames.empty())
        return;

    const auto& frame = m_CurrentClip->frames[m_CurrentFrameIndex];

    // SpriteRendererにUV領域を指示
    m_SpriteRenderer->SetTextureRegion(frame.col, frame.row, colCount, rowCount);

    // フレームに紐づくHitbox設定を自動適用
    if (m_Hitbox)
    {
        m_Hitbox->enabled = frame.hitboxEnabled;
        if (frame.hitboxEnabled)
        {
            // ダウンキャストしてサイズも更新
            if (BoxCollider2D* box = dynamic_cast<BoxCollider2D*>(m_Hitbox))
            {
                box->size = frame.hitboxSize;
            }
            // flipX に応じてオフセットX を反転（右向き基準で定義したoffsetを使う）
            float dirSign = (m_SpriteRenderer->flipX) ? -1.0f : 1.0f;
            m_Hitbox->offset = { frame.hitboxOffset.x * dirSign, frame.hitboxOffset.y };
        }
    }
}

std::string Animator::GetCurrentClipName() const
{
    return m_CurrentClip ? m_CurrentClip->name : "";
}

bool Animator::IsFinished() const
{
    if (!m_CurrentClip || m_CurrentClip->frames.empty()) return true;
    if (m_CurrentClip->loop) return false;

    return !m_IsPlaying && (m_CurrentFrameIndex == (int)m_CurrentClip->frames.size() - 1);
}
