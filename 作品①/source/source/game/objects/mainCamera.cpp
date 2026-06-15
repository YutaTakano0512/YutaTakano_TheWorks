#include "mainCamera.h"
#include "../../core/manager.h"
#include "../../core/main.h"
#include <algorithm>

// =============================================================================
// 初期化
// =============================================================================

void MainCamera::Init()
{
    GameObject::Init();

    SetName("MainCamera");
    SetTag("Camera");

    // カメラコンポーネントを追加
    m_Camera = AddComponent<Camera2D>();

    // 初期位置
    transform.position = { 1280.0f / 2.0f, 720.0f / 2.0f }; // 先ほど720に変更したため
}

// =============================================================================
// 更新
// =============================================================================

void MainCamera::Update(float dt)
{
    GameObject::Update(dt);

    // 固定カメラモード
    if (m_IsFixed)
    {
        transform.position = m_FixedPosition;
        return;
    }

    auto players = Manager::GetScene()->FindGameObjectsWithTag("Player");
    if (!players.empty())
    {
        Vector2 playerPos = players[0]->transform.position;
        float cameraOffsetY = -150.0f;
        transform.position = { playerPos.x, playerPos.y + cameraOffsetY }; 
    }

    if (m_ClampEnabled)
    {
        const float hw = static_cast<float>(SCREEN_WIDTH)  * 0.5f;
        const float hh = static_cast<float>(SCREEN_HEIGHT) * 0.5f;

        const float clampMinX = m_StageMinX + hw;
        const float clampMaxX = m_StageMaxX - hw;
        const float clampMinY = m_StageMinY + hh;
        const float clampMaxY = m_StageMaxY - hh;

        // ステージ幅が画面より狭い場合は中央に固定
        if (clampMinX <= clampMaxX)
            transform.position.x = std::clamp(transform.position.x, clampMinX, clampMaxX);

        if (clampMinY <= clampMaxY)
            transform.position.y = std::clamp(transform.position.y, clampMinY, clampMaxY);
    }
}
