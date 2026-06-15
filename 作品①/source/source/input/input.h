#pragma once
#include <Windows.h>

//=============================================================================
// Input クラス (キーボード入力)
//=============================================================================
class Input
{
public:
    static void Init();
    static void Uninit();
    static void Update();

    // 押下状態
    static bool GetKey(BYTE KeyCode);
    // トリガー(押した瞬間)
    static bool GetKeyDown(BYTE KeyCode);
    // リリース(離した瞬間)
    static bool GetKeyUp(BYTE KeyCode);

private:
    static BYTE m_OldKeyState[256];
    static BYTE m_KeyState[256];
};
