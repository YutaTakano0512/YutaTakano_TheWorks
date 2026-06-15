#pragma once
#include "../core/component.h"
#include "../math/vector2.h"
#include <DirectXMath.h>


//=============================================================================
// Camera2D コンポーネント
//=============================================================================
class Camera2D : public Component
{
public:
    float zoom = 1.0f;

    // シェイク用
    float shakeIntensity = 0.0f;
    float shakeDuration  = 0.0f;

    void Init() override;
    void Update(float dt) override;
    void Draw() override; // RendererにView行列を送信

    void Shake(float intensity, float duration);

    DirectX::XMMATRIX GetViewMatrix() const;

private:
    Vector2 m_ShakeOffset = { 0.0f, 0.0f };
};
