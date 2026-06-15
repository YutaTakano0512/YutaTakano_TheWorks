#include "input.h"

//=============================================================================
// 静的メンバ定義
//=============================================================================
BYTE Input::m_OldKeyState[256];
BYTE Input::m_KeyState[256];

void Input::Init()
{
    for (int i = 0; i < 256; i++)
    {
        m_OldKeyState[i] = 0;
        m_KeyState[i]    = 0;
    }
}

void Input::Uninit()
{
}

void Input::Update()
{
    // 前回の入力を保存
    memcpy(m_OldKeyState, m_KeyState, sizeof(m_KeyState));

    // 現在の入力を取得
    GetKeyboardState(m_KeyState);
}

bool Input::GetKey(BYTE KeyCode)
{
    return (m_KeyState[KeyCode] & 0x80) != 0;
}

bool Input::GetKeyDown(BYTE KeyCode)
{
    return ((m_KeyState[KeyCode] & 0x80) != 0) && ((m_OldKeyState[KeyCode] & 0x80) == 0);
}

bool Input::GetKeyUp(BYTE KeyCode)
{
    return ((m_KeyState[KeyCode] & 0x80) == 0) && ((m_OldKeyState[KeyCode] & 0x80) != 0);
}
