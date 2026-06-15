#include "camera2D.h"
#include "../core/gameObject.h"
#include "../core/main.h"
#include "../graphics/renderer.h"
#include <cstdlib>

void Camera2D::Init()
{
}

void Camera2D::Update(float dt)
{
    if (shakeDuration > 0.0f)
    {
        // TimeScaleが0 (ヒットストップ等) の時でもシェイクを進める
        float step = (dt > 0.0f) ? dt : (1.0f / 60.0f);
        shakeDuration -= step;
        if (shakeDuration <= 0.0f)
        {
            shakeDuration = 0.0f;
            m_ShakeOffset.zero();
        }
        else
        {
            // -1.0 〜 1.0 のランダム値
            float rx = ((float)rand() / RAND_MAX) * 2.0f - 1.0f;
            float ry = ((float)rand() / RAND_MAX) * 2.0f - 1.0f;
            m_ShakeOffset.x = rx * shakeIntensity;
            m_ShakeOffset.y = ry * shakeIntensity;
        }
    }
}

void Camera2D::Draw()
{
    Renderer::SetViewMatrix(GetViewMatrix());
}

void Camera2D::Shake(float intensity, float duration)
{
    shakeIntensity = intensity;
    shakeDuration  = duration;
}

DirectX::XMMATRIX Camera2D::GetViewMatrix() const
{
    Vector2 pos = m_GameObject->transform.position + m_ShakeOffset;
    float   rot = m_GameObject->transform.rotation;

    // カメラの Transform は「世界を逆方向に動かす」ためのもの
    // つまりカメラが (X,Y) にいるなら、世界を (-X, -Y) に移動する
    // また画面中心が原点になるようにオフセットを加える
    
    float hw = (float)SCREEN_WIDTH * 0.5f;
    float hh = (float)SCREEN_HEIGHT * 0.5f;

    DirectX::XMMATRIX t1 = DirectX::XMMatrixTranslation(-pos.x, -pos.y, 0.0f);
    DirectX::XMMATRIX r  = DirectX::XMMatrixRotationZ(-rot);
    DirectX::XMMATRIX s  = DirectX::XMMatrixScaling(zoom, zoom, 1.0f);
    DirectX::XMMATRIX t2 = DirectX::XMMatrixTranslation(hw, hh, 0.0f); // 画面中心へ

    return t1 * r * s * t2;
}
