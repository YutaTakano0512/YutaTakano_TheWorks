#pragma once
#include <math.h>
#include <algorithm>

//=============================================================================
// 数学ヘルパー
//=============================================================================
namespace MathHelper
{
    constexpr float PI        = 3.14159265358979323846f;
    constexpr float TWO_PI    = PI * 2.0f;
    constexpr float HALF_PI   = PI * 0.5f;
    constexpr float DEG_TO_RAD = PI / 180.0f;
    constexpr float RAD_TO_DEG = 180.0f / PI;

    inline float ToRadians(float deg) { return deg * DEG_TO_RAD; }
    inline float ToDegrees(float rad) { return rad * RAD_TO_DEG; }

    template<typename T>
    inline T Clamp(T v, T lo, T hi) { return std::max(lo, std::min(v, hi)); }

    inline float Lerp(float a, float b, float t) { return a + (b - a) * t; }

    // 0-1 にクランプしたLerp
    inline float LerpClamped(float a, float b, float t) { return Lerp(a, b, Clamp(t, 0.0f, 1.0f)); }

    // 値を指定範囲に折り返す (wrap)
    inline float Repeat(float t, float length) { return t - floorf(t / length) * length; }

    // PingPong (0→length→0→…)
    inline float PingPong(float t, float length)
    {
        t = Repeat(t, length * 2.0f);
        return length - fabsf(t - length);
    }

    // 符号
    template<typename T>
    inline T Sign(T v) { return (v > T(0)) ? T(1) : (v < T(0)) ? T(-1) : T(0); }

    // 滑らかなステップ
    inline float SmoothStep(float edge0, float edge1, float x)
    {
        float t = Clamp((x - edge0) / (edge1 - edge0), 0.0f, 1.0f);
        return t * t * (3.0f - 2.0f * t);
    }

    // 2つの値が近いか
    inline bool NearlyEqual(float a, float b, float eps = 1e-4f) { return fabsf(a - b) < eps; }
}

//=============================================================================
// イージング関数
//=============================================================================
namespace Easing
{
    // Linear
    inline float Linear(float t) { return t; }

    // Quad
    inline float InQuad(float t)    { return t * t; }
    inline float OutQuad(float t)   { return t * (2.0f - t); }
    inline float InOutQuad(float t)
    {
        if (t < 0.5f) return 2.0f * t * t;
        return -1.0f + (4.0f - 2.0f * t) * t;
    }

    // Cubic
    inline float InCubic(float t)   { return t * t * t; }
    inline float OutCubic(float t)  { float f = t - 1.0f; return f * f * f + 1.0f; }
    inline float InOutCubic(float t)
    {
        if (t < 0.5f) return 4.0f * t * t * t;
        float f = 2.0f * t - 2.0f; return 0.5f * f * f * f + 1.0f;
    }

    // Quart
    inline float InQuart(float t)   { return t * t * t * t; }
    inline float OutQuart(float t)  { float f = t - 1.0f; return 1.0f - f * f * f * f; }

    // Sine
    inline float InSine(float t)    { return 1.0f - cosf(t * MathHelper::HALF_PI); }
    inline float OutSine(float t)   { return sinf(t * MathHelper::HALF_PI); }
    inline float InOutSine(float t) { return 0.5f * (1.0f - cosf(MathHelper::PI * t)); }

    // Expo
    inline float InExpo(float t)    { return (t == 0.0f) ? 0.0f : powf(2.0f, 10.0f * (t - 1.0f)); }
    inline float OutExpo(float t)   { return (t == 1.0f) ? 1.0f : 1.0f - powf(2.0f, -10.0f * t); }

    // Back
    inline float InBack(float t)
    {
        const float s = 1.70158f;
        return t * t * ((s + 1.0f) * t - s);
    }
    inline float OutBack(float t)
    {
        const float s = 1.70158f;
        float f = t - 1.0f;
        return 1.0f + f * f * ((s + 1.0f) * f + s);
    }

    // Elastic
    inline float OutElastic(float t)
    {
        if (t == 0.0f || t == 1.0f) return t;
        return powf(2.0f, -10.0f * t) * sinf((t - 0.075f) * (2.0f * MathHelper::PI) / 0.3f) + 1.0f;
    }

    // Bounce
    inline float OutBounce(float t)
    {
        if (t < 1.0f / 2.75f) return 7.5625f * t * t;
        if (t < 2.0f / 2.75f) { t -= 1.5f   / 2.75f; return 7.5625f * t * t + 0.75f;  }
        if (t < 2.5f / 2.75f) { t -= 2.25f  / 2.75f; return 7.5625f * t * t + 0.9375f; }
        t -= 2.625f / 2.75f; return 7.5625f * t * t + 0.984375f;
    }
    inline float InBounce(float t) { return 1.0f - OutBounce(1.0f - t); }
}
