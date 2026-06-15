#pragma once
#include <Windows.h>
#include <xinput.h>
#include "../math/vector2.h"

//=============================================================================
// Gamepad クラス (XInputベース)
// 1プレイヤー(Index 0)のみを想定した簡易ラッパー
//=============================================================================
class Gamepad
{
public:
    static void Init();
    static void Uninit();
    static void Update();

    // 接続状態
    static bool IsConnected() { return m_Connected; }

    // ボタン状態
    // 例: XINPUT_GAMEPAD_A, XINPUT_GAMEPAD_DPAD_UP など
    static bool GetButton(WORD buttonMask);
    static bool GetButtonDown(WORD buttonMask);
    static bool GetButtonUp(WORD buttonMask);

    // スティック入力 (-1.0 ～ 1.0)
    static Vector2 GetLeftStick();
    static Vector2 GetRightStick();

    // トリガー入力 (0.0 ～ 1.0)
    static float GetLeftTrigger();
    static float GetRightTrigger();

    // 振動 (0.0 ～ 1.0)
    static void SetVibration(float leftMotor, float rightMotor);

private:
    static XINPUT_STATE m_State;
    static XINPUT_STATE m_OldState;
    static bool m_Connected;

    // デッドゾーン処理用
    static float ApplyDeadzone(SHORT value, SHORT deadzone);
};
