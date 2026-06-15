#pragma once
#include "../math/vector2.h"

//=============================================================================
// MouseInput クラス
//=============================================================================
class MouseInput
{
public:
    static void Init();
    static void Uninit();
    static void Update();

    // 押下状態 (0:左, 1:右, 2:中)
    static bool GetButton(int button);
    static bool GetButtonDown(int button);
    static bool GetButtonUp(int button);

    // マウスカーソル座標 (スクリーン座標)
    static Vector2 GetPosition();
    static Vector2 GetDeltaPosition(); // 前フレームからの移動量

private:
    static bool m_OldButtonState[3];
    static bool m_ButtonState[3];
    static Vector2 m_Position;
    static Vector2 m_OldPosition;
};
