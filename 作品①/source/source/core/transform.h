#pragma once
#include "component.h"
#include "../math/vector2.h"
#include "../math/mathHelper.h"
#include <DirectXMath.h>
#include <cstdlib> // rand()用

//=============================================================================
// Transform コンポーネント (2D)
// GameObject に必ず1つ付与される。位置・回転・スケール・描画順を管理。
//=============================================================================
struct Transform : public Component
{
    Vector2 position  = { 0.0f, 0.0f };
    float   rotation  = 0.0f;          // ラジアン (Z軸回転)
    Vector2 scale     = { 1.0f, 1.0f };
    int     zOrder    = 0;             // 描画ソート用 (大きいほど手前)

    // シェイク（座標ブレ）用パラメータ
    Vector2 shakeOffset   = { 0.0f, 0.0f };
    float   shakeTimer     = 0.0f;
    float   shakeIntensity = 0.0f;

    // 毎フレームの更新処理でシェイクの計算を行う
    void Update(float dt) override
    {
        if (shakeTimer > 0.0f)
        {
            // TimeScaleが0 (ヒットストップ等) の時でもシェイクを進める
            float step = (dt > 0.0f) ? dt : (1.0f / 60.0f);
            shakeTimer -= step;
            if (shakeTimer <= 0.0f)
            {
                shakeTimer = 0.0f;
                shakeOffset = { 0.0f, 0.0f };
            }
            else
            {
                // -1.0 から 1.0 のランダム値を生成してブレ幅を計算
                float rx = (((float)rand() / RAND_MAX) * 2.0f - 1.0f) * shakeIntensity;
                float ry = (((float)rand() / RAND_MAX) * 2.0f - 1.0f) * shakeIntensity;
                shakeOffset = { rx, ry };
            }
        }
    }

    // シェイクの開始
    void Shake(float intensity, float duration)
    {
        shakeIntensity = intensity;
        shakeTimer = duration;
    }

    //-------------------------------------------------------------------------
    // ワールド行列取得 (Scale → Rotate → Translate)
    //-------------------------------------------------------------------------
    DirectX::XMMATRIX GetWorldMatrix() const
    {
        DirectX::XMMATRIX S = DirectX::XMMatrixScaling(scale.x, scale.y, 1.0f);
        DirectX::XMMATRIX R = DirectX::XMMatrixRotationZ(rotation);
        // position に shakeOffset を加算して描画座標をブレさせる
        DirectX::XMMATRIX T = DirectX::XMMatrixTranslation(position.x + shakeOffset.x, position.y + shakeOffset.y, static_cast<float>(zOrder) * 0.001f);
        return S * R * T;
    }

    //-------------------------------------------------------------------------
    // 便利関数
    //-------------------------------------------------------------------------
    // 前方向ベクトル (回転を考慮した +X 方向)
    Vector2 GetRight() const
    {
        return Vector2(cosf(rotation), sinf(rotation));
    }

    // 上方向ベクトル (回転を考慮した +Y 方向)
    Vector2 GetUp() const
    {
        return Vector2(-sinf(rotation), cosf(rotation));
    }

    // 向きを指定ベクトルに向ける
    void LookAt(const Vector2& target)
    {
        Vector2 dir = target - position;
        if (dir.lengthSq() > 1e-6f)
            rotation = atan2f(dir.y, dir.x);
    }

    void Translate(const Vector2& delta) { position += delta; }
    void Rotate(float rad)               { rotation += rad; }
};
