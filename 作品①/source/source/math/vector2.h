#pragma once
#include <math.h>

//=============================================================================
// Vector2 クラス
//=============================================================================
class Vector2
{
public:
    float x, y;

    Vector2() : x(0.0f), y(0.0f) {}
    Vector2(const Vector2& a) : x(a.x), y(a.y) {}
    Vector2(float nx, float ny) : x(nx), y(ny) {}

    Vector2& operator=(const Vector2& a)       { x = a.x; y = a.y; return *this; }
    bool     operator==(const Vector2& a) const { return x == a.x && y == a.y; }
    bool     operator!=(const Vector2& a) const { return x != a.x || y != a.y; }

    void    zero()                    { x = y = 0.0f; }
    Vector2 operator-() const         { return Vector2(-x, -y); }
    Vector2 operator+(const Vector2& a) const { return Vector2(x + a.x, y + a.y); }
    Vector2 operator-(const Vector2& a) const { return Vector2(x - a.x, y - a.y); }
    Vector2 operator*(float a) const  { return Vector2(x * a, y * a); }
    Vector2 operator/(float a) const  { float inv = 1.0f / a; return Vector2(x * inv, y * inv); }

    Vector2& operator+=(const Vector2& a) { x += a.x; y += a.y; return *this; }
    Vector2& operator-=(const Vector2& a) { x -= a.x; y -= a.y; return *this; }
    Vector2& operator*=(float a)          { x *= a;   y *= a;   return *this; }
    Vector2& operator/=(float a)          { float inv = 1.0f / a; x *= inv; y *= inv; return *this; }

    // 正規化
    void normalize()
    {
        float magSq = x * x + y * y;
        if (magSq > 0.0f)
        {
            float inv = 1.0f / sqrtf(magSq);
            x *= inv; y *= inv;
        }
    }

    Vector2 normalized() const
    {
        Vector2 v = *this;
        v.normalize();
        return v;
    }

    // 大きさ
    float length()    const { return sqrtf(x * x + y * y); }
    float lengthSq()  const { return x * x + y * y; }

    // 内積
    static float dot(const Vector2& a, const Vector2& b) { return a.x * b.x + a.y * b.y; }

    // 外積 (Z成分のスカラー)
    static float cross(const Vector2& a, const Vector2& b) { return a.x * b.y - a.y * b.x; }

    // 距離
    static float distance(const Vector2& a, const Vector2& b) { return (a - b).length(); }

    // 線形補間
    static Vector2 lerp(const Vector2& a, const Vector2& b, float t)
    {
        return Vector2(a.x + (b.x - a.x) * t, a.y + (b.y - a.y) * t);
    }

    // 反射ベクトル
    static Vector2 reflect(const Vector2& v, const Vector2& n)
    {
        Vector2 nn = n;
        nn.normalize();
        return v - nn * (2.0f * dot(v, nn));
    }

    // 垂直ベクトル (左90度回転)
    Vector2 perpendicular() const { return Vector2(-y, x); }
};

// スカラー * Vector2
inline Vector2 operator*(float s, const Vector2& v) { return Vector2(v.x * s, v.y * s); }
