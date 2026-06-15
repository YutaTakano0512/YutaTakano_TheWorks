#include "mouseInput.h"
#include "../core/main.h"

//=============================================================================
// 静的メンバ定義
//=============================================================================
bool MouseInput::m_OldButtonState[3];
bool MouseInput::m_ButtonState[3];
Vector2 MouseInput::m_Position;
Vector2 MouseInput::m_OldPosition;

void MouseInput::Init()
{
    for (int i = 0; i < 3; i++)
    {
        m_OldButtonState[i] = false;
        m_ButtonState[i]    = false;
    }
    m_Position    = Vector2(0.0f, 0.0f);
    m_OldPosition = Vector2(0.0f, 0.0f);
}

void MouseInput::Uninit()
{
}

void MouseInput::Update()
{
    // ボタン状態保存
    for (int i = 0; i < 3; i++)
        m_OldButtonState[i] = m_ButtonState[i];

    // ボタン状態取得
    m_ButtonState[0] = (GetAsyncKeyState(VK_LBUTTON) & 0x8000) != 0;
    m_ButtonState[1] = (GetAsyncKeyState(VK_RBUTTON) & 0x8000) != 0;
    m_ButtonState[2] = (GetAsyncKeyState(VK_MBUTTON) & 0x8000) != 0;

    // マウス座標保存
    m_OldPosition = m_Position;

    // マウス座標取得 (スクリーン座標をクライアント座標に変換)
    POINT pt;
    GetCursorPos(&pt);
    ScreenToClient(GetWindowHandle(), &pt);
    m_Position.x = static_cast<float>(pt.x);
    m_Position.y = static_cast<float>(pt.y);
}

bool MouseInput::GetButton(int button)
{
    if (button < 0 || button >= 3) return false;
    return m_ButtonState[button];
}

bool MouseInput::GetButtonDown(int button)
{
    if (button < 0 || button >= 3) return false;
    return m_ButtonState[button] && !m_OldButtonState[button];
}

bool MouseInput::GetButtonUp(int button)
{
    if (button < 0 || button >= 3) return false;
    return !m_ButtonState[button] && m_OldButtonState[button];
}

Vector2 MouseInput::GetPosition()
{
    return m_Position;
}

Vector2 MouseInput::GetDeltaPosition()
{
    return m_Position - m_OldPosition;
}
