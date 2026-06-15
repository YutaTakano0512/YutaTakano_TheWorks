#include "gamepad.h"
#include <cmath>
#include <algorithm>

#pragma comment(lib, "xinput.lib")

//=============================================================================
// 静的メンバ定義
//=============================================================================
XINPUT_STATE Gamepad::m_State;
XINPUT_STATE Gamepad::m_OldState;
bool Gamepad::m_Connected = false;

void Gamepad::Init()
{
    ZeroMemory(&m_State, sizeof(XINPUT_STATE));
    ZeroMemory(&m_OldState, sizeof(XINPUT_STATE));
    m_Connected = false;
}

void Gamepad::Uninit()
{
    SetVibration(0.0f, 0.0f);
}

void Gamepad::Update()
{
    m_OldState = m_State;

    // プレイヤー0 (インデックス0) の状態を取得
    DWORD dwResult = XInputGetState(0, &m_State);

    if (dwResult == ERROR_SUCCESS)
    {
        m_Connected = true;
    }
    else
    {
        m_Connected = false;
        ZeroMemory(&m_State, sizeof(XINPUT_STATE));
    }
}

bool Gamepad::GetButton(WORD buttonMask)
{
    if (!m_Connected) return false;
    return (m_State.Gamepad.wButtons & buttonMask) != 0;
}

bool Gamepad::GetButtonDown(WORD buttonMask)
{
    if (!m_Connected) return false;
    return ((m_State.Gamepad.wButtons & buttonMask) != 0) &&
           ((m_OldState.Gamepad.wButtons & buttonMask) == 0);
}

bool Gamepad::GetButtonUp(WORD buttonMask)
{
    if (!m_Connected) return false;
    return ((m_State.Gamepad.wButtons & buttonMask) == 0) &&
           ((m_OldState.Gamepad.wButtons & buttonMask) != 0);
}

float Gamepad::ApplyDeadzone(SHORT value, SHORT deadzone)
{
    if (value < -deadzone)
    {
        return (float)(value + deadzone) / (float)(32768 - deadzone);
    }
    else if (value > deadzone)
    {
        return (float)(value - deadzone) / (float)(32767 - deadzone);
    }
    return 0.0f;
}

Vector2 Gamepad::GetLeftStick()
{
    if (!m_Connected) return Vector2(0.0f, 0.0f);
    float x = ApplyDeadzone(m_State.Gamepad.sThumbLX, XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE);
    float y = ApplyDeadzone(m_State.Gamepad.sThumbLY, XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE);
    return Vector2(x, y);
}

Vector2 Gamepad::GetRightStick()
{
    if (!m_Connected) return Vector2(0.0f, 0.0f);
    float x = ApplyDeadzone(m_State.Gamepad.sThumbRX, XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE);
    float y = ApplyDeadzone(m_State.Gamepad.sThumbRY, XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE);
    return Vector2(x, y);
}

float Gamepad::GetLeftTrigger()
{
    if (!m_Connected) return 0.0f;
    if (m_State.Gamepad.bLeftTrigger > XINPUT_GAMEPAD_TRIGGER_THRESHOLD)
    {
        return (float)(m_State.Gamepad.bLeftTrigger - XINPUT_GAMEPAD_TRIGGER_THRESHOLD) /
               (float)(255 - XINPUT_GAMEPAD_TRIGGER_THRESHOLD);
    }
    return 0.0f;
}

float Gamepad::GetRightTrigger()
{
    if (!m_Connected) return 0.0f;
    if (m_State.Gamepad.bRightTrigger > XINPUT_GAMEPAD_TRIGGER_THRESHOLD)
    {
        return (float)(m_State.Gamepad.bRightTrigger - XINPUT_GAMEPAD_TRIGGER_THRESHOLD) /
               (float)(255 - XINPUT_GAMEPAD_TRIGGER_THRESHOLD);
    }
    return 0.0f;
}

void Gamepad::SetVibration(float leftMotor, float rightMotor)
{
    if (!m_Connected) return;

    leftMotor  = std::max(0.0f, std::min(1.0f, leftMotor));
    rightMotor = std::max(0.0f, std::min(1.0f, rightMotor));

    XINPUT_VIBRATION vibration;
    ZeroMemory(&vibration, sizeof(XINPUT_VIBRATION));
    vibration.wLeftMotorSpeed  = (WORD)(leftMotor * 65535.0f);   // 低周波モーター
    vibration.wRightMotorSpeed = (WORD)(rightMotor * 65535.0f);  // 高周波モーター

    XInputSetState(0, &vibration);
}
