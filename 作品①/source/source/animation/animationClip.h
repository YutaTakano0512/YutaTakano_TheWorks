#pragma once
#include <string>
#include <vector>
#include "../math/vector2.h"

//=============================================================================
// アニメーションフレーム定義
//=============================================================================
struct AnimationFrame
{
    int col;        // スプライトシートの列インデックス
    int row;        // スプライトシートの行インデックス
    float duration; // 表示時間(秒)

    // フレーム単位のHitbox設定
    bool    hitboxEnabled = false;      // このフレームでHitboxを有効にするか
    Vector2 hitboxSize    = {0.0f, 0.0f}; // Hitboxのサイズ (有効時のみ使用)
    Vector2 hitboxOffset  = {0.0f, 0.0f}; // Hitboxのオフセット (右向き基準、左向き時は自動反転)
};

//=============================================================================
// AnimationClip クラス
// 1つのアニメーション(例: "Run", "Jump")のフレーム群を管理
//=============================================================================
class AnimationClip
{
public:
    std::string name;
    std::vector<AnimationFrame> frames;
    bool loop = true;

    AnimationClip() = default;
    AnimationClip(const std::string& name, bool loop = true)
        : name(name), loop(loop) {}

    // フレームを1つ追加するヘルパー (Hitboxなし)
    void AddFrame(int col, int row, float duration)
    {
        frames.push_back({ col, row, duration });
    }

    // まとめて追加するヘルパー (Hitboxなし)
    void AddFrames(int startCol, int startRow, int count, float durationPerFrame, bool horizontal = true)
    {
        for (int i = 0; i < count; i++)
        {
            AnimationFrame f;
            if (horizontal)
                f = { startCol + i, startRow, durationPerFrame };
            else
                f = { startCol, startRow + i, durationPerFrame };
            frames.push_back(f);
        }
    }

    // フレームを1つ追加するヘルパー (Hitboxあり)
    void AddFrameWithHitbox(int col, int row, float duration,
                            const Vector2& size, const Vector2& offset)
    {
        AnimationFrame f;
        f.col           = col;
        f.row           = row;
        f.duration      = duration;
        f.hitboxEnabled = true;
        f.hitboxSize    = size;
        f.hitboxOffset  = offset;
        frames.push_back(f);
    }

    // まとめて追加するヘルパー (Hitboxあり、全フレーム同じHitbox設定)
    void AddFramesWithHitbox(int startCol, int startRow, int count, float durationPerFrame,
                             const Vector2& size, const Vector2& offset, bool horizontal = true)
    {
        for (int i = 0; i < count; i++)
        {
            AnimationFrame f;
            if (horizontal)
                f = { startCol + i, startRow, durationPerFrame, true, size, offset };
            else
                f = { startCol, startRow + i, durationPerFrame, true, size, offset };
            frames.push_back(f);
        }
    }
};
