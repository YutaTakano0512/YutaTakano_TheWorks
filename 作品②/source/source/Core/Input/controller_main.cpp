// ===================================================
// controller_main.cpp  ゲームのコントローラー
// 
// hara sougo  2024/07/22
// ===================================================


#include "../_systemModeSwitch.h"

#include "../../game_main.h"
#include "controller_main.h"

#ifdef SWITCH_MODE
#else
#include "../../Game/Configuration/configuration.h"
#include "../../main.h"
#include "../DirectX/dx.h"
#endif


#ifdef SWITCH_MODE

#else

struct GAMEPADDAT {
    int keyPushFrame[CONTROLLER_BUTTON_TYPE_MAX];
    int gamePadButnPushFrame[CONTROLLER_BUTTON_TYPE_MAX];

    int gamePadVibrationFrameLeft ;
    int gamePadVibrationFrameRight ;
    int gamePadVibrationSpeedLeft ;
    int gamePadVibrationSpeedRight ;

    bool gamePadConnected;
    XINPUT_STATE gamePadState ;

    int stickLeftFrame;
    int stickRightFrame;
    Float2 stickLeft;
    Float2 stickRight;

    MouseState_tag gameMouseState ;
    //static const Keyboard_State* g_GameKeybordState = nullptr;

    CONTROLLER_INPUT_DAT inputDat;
};

static constexpr Keyboard_Keys keyboardNumList[] = {
    KK_RIGHT,
    KK_DOWN,
    KK_UP,
    KK_LEFT,

    KK_TAB,
    KK_PAGEDOWN,
    KK_TAB,
    KK_PAGEDOWN,

    KK_A,
    KK_D,
    KK_W,
    KK_S,

    KK_LEFTSHIFT,
    KK_RIGHTSHIFT,

    KK_ENTER,
    KK_BACK,
};
static constexpr int gamepadNumList[] = {
    /*XINPUT_GAMEPAD_A,
    XINPUT_GAMEPAD_B,
    XINPUT_GAMEPAD_X,
    XINPUT_GAMEPAD_Y,*/
    XINPUT_GAMEPAD_B, //xboxコンを考慮して逆に
    XINPUT_GAMEPAD_A,
    XINPUT_GAMEPAD_Y,
    XINPUT_GAMEPAD_X,

    XINPUT_GAMEPAD_LEFT_SHOULDER,
    XINPUT_GAMEPAD_RIGHT_SHOULDER,
    -1,
    -2,

    XINPUT_GAMEPAD_DPAD_LEFT,
    XINPUT_GAMEPAD_DPAD_RIGHT,
    XINPUT_GAMEPAD_DPAD_UP,
    XINPUT_GAMEPAD_DPAD_DOWN,

    XINPUT_GAMEPAD_LEFT_THUMB,
    XINPUT_GAMEPAD_RIGHT_THUMB,

    XINPUT_GAMEPAD_START,
    XINPUT_GAMEPAD_BACK,
};

struct InputDatFixed {
    bool enable;
    CONTROLLER_INPUT_DAT inputDat;
};

static InputDatFixed g_InputDatFixed[SYS_PLAYER_MAX] = {};


static GAMEPADDAT g_GamePadDat[CONTROLLER_DEVICE_MAX] = {};


static int mouseLPushFrame = 0;
static int mouseMPushFrame = 0;
static int mouseRPushFrame = 0;



//
// プロトタイプ宣言
//
void gamePadVibrationUpdate(void);

#endif


void ControllerMainInit(void)
{
#ifdef SWITCH_MODE
#else
    //g_GameKeybordState = Keyboard_GetState();
#endif
}

void ControllerMainUpdate(void)
{
#ifdef SWITCH_MODE
#else
    Keyboard_Update();
    Mouse_GetState(&g_GamePadDat[0].gameMouseState);

    for (int i = 0; i < 3; i++)
    {
        bool btn = g_GamePadDat[0].gameMouseState.leftButton;
        int* ftmp = &mouseLPushFrame;
        if (i == 1)
        {
            btn = g_GamePadDat[0].gameMouseState.middleButton;
            ftmp = &mouseMPushFrame;
        }
        if (i == 2)
        {
            btn = g_GamePadDat[0].gameMouseState.rightButton;
            ftmp = &mouseRPushFrame;
        }

        if ((*ftmp) < 0)
        {
            (*ftmp) = 0;
        }

        if (btn)
        {
            (*ftmp)++;
        }
        else
        {
            if ((*ftmp) > 0)
            {
                (*ftmp) = -1;
            }
        }
    }

    for (int i = 0; i < CONTROLLER_BUTTON_TYPE_MAX; i++)
    {
        if (g_GamePadDat[0].keyPushFrame[i] < 0)
        {
            g_GamePadDat[0].keyPushFrame[i] = 0;
        }

        if (Keyboard_IsKeyDown(keyboardNumList[i]))
        {
            g_GamePadDat[0].keyPushFrame[i]++;
        }
        else
        {
            if (g_GamePadDat[0].keyPushFrame[i] > 0)
            {
                g_GamePadDat[0].keyPushFrame[i] = -1;
            }
        }
    }

    for (int i = 0; i < CONTROLLER_DEVICE_MAX; i++)
    {
        DWORD xsr =  XInputGetState(i, &g_GamePadDat[i].gamePadState);
        if (xsr == ERROR_SUCCESS)
        {
            g_GamePadDat[i].gamePadConnected = true;
        }
        else
        {
            g_GamePadDat[i].gamePadConnected = false;
        }

        //ゲームパッドアナログスティックのデッドゾーン処理
        if ((g_GamePadDat[i].gamePadState.Gamepad.sThumbLX < XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE && g_GamePadDat[i].gamePadState.Gamepad.sThumbLX > -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE) &&
            (g_GamePadDat[i].gamePadState.Gamepad.sThumbLY < XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE && g_GamePadDat[i].gamePadState.Gamepad.sThumbLY > -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE))
        {
            g_GamePadDat[i].gamePadState.Gamepad.sThumbLX = 0;
            g_GamePadDat[i].gamePadState.Gamepad.sThumbLY = 0;
        }
        {
            g_GamePadDat[i].stickLeft.x = g_GamePadDat[i].gamePadState.Gamepad.sThumbLX / 32767.0f;
            g_GamePadDat[i].stickLeft.y = g_GamePadDat[i].gamePadState.Gamepad.sThumbLY / 32767.0f;

            g_GamePadDat[i].stickRight.x = g_GamePadDat[i].gamePadState.Gamepad.sThumbRX / 32767.0f;
            g_GamePadDat[i].stickRight.y = g_GamePadDat[i].gamePadState.Gamepad.sThumbRY / 32767.0f;
        }

        if (g_GamePadDat[i].stickLeftFrame < 0)
        {
            g_GamePadDat[i].stickLeftFrame = 0;
        }
        if (g_GamePadDat[i].stickRightFrame < 0)
        {
            g_GamePadDat[i].stickRightFrame = 0;
        }

        if (abs(g_GamePadDat[i].stickLeft.x) >= 0.05f
            || abs(g_GamePadDat[i].stickLeft.y) >= 0.05f)
        {
            g_GamePadDat[i].stickLeftFrame++;
        }
        else
        {
            g_GamePadDat[i].stickLeftFrame = -1;
        }

        if (abs(g_GamePadDat[i].stickRight.x) >= 0.05f
            || abs(g_GamePadDat[i].stickRight.y) >= 0.05f)
        {
            g_GamePadDat[i].stickRightFrame++;
        }
        else
        {
            g_GamePadDat[i].stickRightFrame = -1;
        }

        for (int j = 0; j < CONTROLLER_BUTTON_TYPE_MAX; j++)
        {
            if (g_GamePadDat[i].gamePadButnPushFrame[j] < 0)
            {
                g_GamePadDat[i].gamePadButnPushFrame[j] = 0;
            }

            if (gamepadNumList[j] == -1 && g_GamePadDat[i].gamePadState.Gamepad.bLeftTrigger > 255 / 4)
            { //ZL
                g_GamePadDat[i].gamePadButnPushFrame[j]++;
            }
            else if (gamepadNumList[j] == -2 && g_GamePadDat[i].gamePadState.Gamepad.bRightTrigger > 255 / 4)
            { //ZR
                g_GamePadDat[i].gamePadButnPushFrame[j]++;
            }
            else if (gamepadNumList[j] != -1 && gamepadNumList[j] != -2
                && (g_GamePadDat[i].gamePadState.Gamepad.wButtons & gamepadNumList[j]))
            {
                g_GamePadDat[i].gamePadButnPushFrame[j]++;
            }
            else
            {
                if (g_GamePadDat[i].gamePadButnPushFrame[j] > 0)
                {
                    g_GamePadDat[i].gamePadButnPushFrame[j] = -1;
                }
            }
        }

        { //バイブレーション処理
            bool gpVibUpdate = false;
            if (g_GamePadDat[i].gamePadVibrationFrameLeft > 0)
            {
                g_GamePadDat[i].gamePadVibrationFrameLeft--;
                if (g_GamePadDat[i].gamePadVibrationFrameLeft <= 0)
                {
                    g_GamePadDat[i].gamePadVibrationSpeedLeft = 0;
                    gpVibUpdate = true;
                }
            }
            if (g_GamePadDat[i].gamePadVibrationFrameRight > 0)
            {
                g_GamePadDat[i].gamePadVibrationFrameRight--;
                if (g_GamePadDat[i].gamePadVibrationFrameRight <= 0)
                {
                    g_GamePadDat[i].gamePadVibrationSpeedRight = 0;
                    gpVibUpdate = true;
                }
            }
            if (gpVibUpdate)
            {
                gamePadVibrationUpdate();
            }
        }
    }

    //最終結果
    for (int i = 0; i < CONTROLLER_DEVICE_MAX; i++)
    {
        if (g_GamePadDat[i].gamePadConnected)
        { //コントローラーが接続されていたら

            for (int j = 0; j < CONTROLLER_BUTTON_TYPE_MAX; j++)
            { //ボタンの入力
                g_GamePadDat[i].inputDat.buttonPushFrame[j] = g_GamePadDat[i].gamePadButnPushFrame[j];
            }
            { //ゲームパッドアナログスティック入力時処理
                g_GamePadDat[i].inputDat.stickLeft.stick = g_GamePadDat[i].stickLeft;
                g_GamePadDat[i].inputDat.stickLeft.frame = g_GamePadDat[i].stickLeftFrame;
                g_GamePadDat[i].inputDat.stickLeft.enable = true;

                g_GamePadDat[i].inputDat.stickRight.stick = g_GamePadDat[i].stickRight;
                g_GamePadDat[i].inputDat.stickRight.frame = g_GamePadDat[i].stickRightFrame;
                g_GamePadDat[i].inputDat.stickRight.enable = true;
            }
        }
        else
        { //キーボード

            for (int j = 0; j < CONTROLLER_BUTTON_TYPE_MAX; j++)
            { //ボタンの入力
                g_GamePadDat[i].inputDat.buttonPushFrame[j] = g_GamePadDat[i].keyPushFrame[j];
            }
            { //ゲームパッドアナログスティック入力時処理
                g_GamePadDat[i].inputDat.stickLeft.stick.x = 0;
                g_GamePadDat[i].inputDat.stickLeft.stick.y = 0;
                g_GamePadDat[i].inputDat.stickLeft.frame = 0;
                g_GamePadDat[i].inputDat.stickLeft.enable = false;

                g_GamePadDat[i].inputDat.stickRight.stick.x = 0;
                g_GamePadDat[i].inputDat.stickRight.stick.y = 0;
                g_GamePadDat[i].inputDat.stickRight.frame = 0;
                g_GamePadDat[i].inputDat.stickRight.enable = false;
            }
        }
    }
#endif
}

void ControllerMainUninit(void)
{
#ifdef SWITCH_MODE
#else
#endif
}


void SetControllerMainInputDataFixed(int index, CONTROLLER_INPUT_DAT inputDat)
{
    if (index < 0 || index >= SYS_PLAYER_MAX)
    {
        return;
    }

    g_InputDatFixed[index].enable = true;
    g_InputDatFixed[index].inputDat = inputDat;
}

CONTROLLER_INPUT_DAT GetControllerMainInputData(int cIndex)
{
    if (g_InputDatFixed[cIndex].enable)
    {
        //g_InputDatFixed[cIndex].enable = false;
        return g_InputDatFixed[cIndex].inputDat;
    }

    if (cIndex >= CONTROLLER_DEVICE_MAX)
    {
        return {};
    }
    return g_GamePadDat[cIndex].inputDat;
}

bool IsControllerMainGamePadConnected(int cIndex)
{
    return g_GamePadDat[cIndex].gamePadConnected;
}

bool GetControllerMainPress(CONTROLLER_BUTTON_TYPE button, int cIndex)
{
#ifdef SWITCH_MODE
#else
    CONTROLLER_INPUT_DAT result = GetControllerMainInputData(cIndex);
    return result.buttonPushFrame[button] > 0;
#endif
}

bool GetControllerMainTrigger(CONTROLLER_BUTTON_TYPE button, int cIndex)
{
#ifdef SWITCH_MODE
#else
    CONTROLLER_INPUT_DAT result = GetControllerMainInputData(cIndex);
    return result.buttonPushFrame[button] == 1;
#endif
}

bool GetControllerMainRelease(CONTROLLER_BUTTON_TYPE button, int cIndex)
{
#ifdef SWITCH_MODE
#else
    CONTROLLER_INPUT_DAT result = GetControllerMainInputData(cIndex);
    return result.buttonPushFrame[button] == -1;
#endif
}


Float2 GetControllerMainLeftStick(int cIndex)
{
#ifdef SWITCH_MODE
#else
    //Float2 r = {};

    //if (g_GamePadDat[cIndex].gamePadState.Gamepad.sThumbLX != 0 || g_GamePadDat[cIndex].gamePadState.Gamepad.sThumbLY != 0)//ゲームパッドアナログスティック入力時処理
    //{
    //    r.x = g_GamePadDat[cIndex].gamePadState.Gamepad.sThumbLX / 32767.0f;
    //    r.y = g_GamePadDat[cIndex].gamePadState.Gamepad.sThumbLY / 32767.0f;
    //}
    //else
    //{
    //    if (keyPushFrame[CONTROLLER_BUTTON_TYPE_LEFT] > 0)
    //    {
    //        r.x = -1.0f;
    //    }
    //    if (keyPushFrame[CONTROLLER_BUTTON_TYPE_RIGHT] > 0)
    //    {
    //        r.x = 1.0f;
    //    }
    //    if (keyPushFrame[CONTROLLER_BUTTON_TYPE_UP] > 0)
    //    {
    //        r.y = -1.0f;
    //    }
    //    if (keyPushFrame[CONTROLLER_BUTTON_TYPE_DOWN] > 0)
    //    {
    //        r.y = 1.0f;
    //    }
    //}
    //return r;

    CONTROLLER_INPUT_DAT result = GetControllerMainInputData(cIndex);
    return result.stickLeft.stick;
#endif
}

Float2 GetControllerMainRightStick(int cIndex)
{
#ifdef SWITCH_MODE
#else
    //Float2 r = {};
    //if (g_GamePadDat[cIndex].gamePadState.Gamepad.sThumbRX != 0 || g_GamePadDat[cIndex].gamePadState.Gamepad.sThumbRY != 0)//ゲームパッドアナログスティック入力時処理
    //{
    //    r.x = g_GamePadDat[cIndex].gamePadState.Gamepad.sThumbRX / 32767.0f;
    //    r.y = g_GamePadDat[cIndex].gamePadState.Gamepad.sThumbRY / 32767.0f;
    //}
    //else
    //{
    //    if (Keyboard_IsKeyDown(KK_J))
    //    {
    //        r.x = -1.0f;
    //    }
    //    if (Keyboard_IsKeyDown(KK_L))
    //    {
    //        r.x = 1.0f;
    //    }
    //    if (Keyboard_IsKeyDown(KK_I))
    //    {
    //        r.y = -1.0f;
    //    }
    //    if (Keyboard_IsKeyDown(KK_K))
    //    {
    //        r.y = 1.0f;
    //    }
    //}
    //return r;

    CONTROLLER_INPUT_DAT result = GetControllerMainInputData(cIndex);
    return result.stickRight.stick;
#endif
}


CONTROLLER_STICK_DAT GetControllerMainLeftStickTrigger(int cIndex)
{
#ifdef SWITCH_MODE
#else
    CONTROLLER_INPUT_DAT result = GetControllerMainInputData(cIndex);
    CONTROLLER_STICK_DAT stickDat = result.stickLeft;
    stickDat.enable = (stickDat.frame == 1);
    return stickDat;
#endif
}

CONTROLLER_STICK_DAT GetControllerMainRightStickTrigger(int cIndex)
{
#ifdef SWITCH_MODE
#else
    CONTROLLER_INPUT_DAT result = GetControllerMainInputData(cIndex);
    CONTROLLER_STICK_DAT stickDat = result.stickRight;
    stickDat.enable = (stickDat.frame == 1);
    return stickDat;
#endif
}

CONTROLLER_STICK_DAT GetControllerMainLeftStickPress(int cIndex)
{
#ifdef SWITCH_MODE
#else
    CONTROLLER_INPUT_DAT result = GetControllerMainInputData(cIndex);
    CONTROLLER_STICK_DAT stickDat = result.stickLeft;
    stickDat.enable = (stickDat.frame > 0);
    return stickDat;
#endif
}

CONTROLLER_STICK_DAT GetControllerMainRightStickPress(int cIndex)
{
#ifdef SWITCH_MODE
#else
    CONTROLLER_INPUT_DAT result = GetControllerMainInputData(cIndex);
    CONTROLLER_STICK_DAT stickDat = result.stickRight;
    stickDat.enable = (stickDat.frame > 0);
    return stickDat;
#endif
}

CONTROLLER_STICK_DAT GetControllerMainLeftStickRelease(int cIndex)
{
#ifdef SWITCH_MODE
#else
    CONTROLLER_INPUT_DAT result = GetControllerMainInputData(cIndex);
    CONTROLLER_STICK_DAT stickDat = result.stickLeft;
    stickDat.enable = (stickDat.frame == -1);
    return stickDat;
#endif
}

CONTROLLER_STICK_DAT GetControllerMainRightStickRelease(int cIndex)
{
#ifdef SWITCH_MODE
#else
    CONTROLLER_INPUT_DAT result = GetControllerMainInputData(cIndex);
    CONTROLLER_STICK_DAT stickDat = result.stickRight;
    stickDat.enable = (stickDat.frame == -1);
    return stickDat;
#endif
}


void SetControllerMainLeftVibration(int frame, int cIndex)
{
#ifdef SWITCH_MODE
#else
    if (frame > 0)
    {
        g_GamePadDat[cIndex].gamePadVibrationFrameLeft = frame;
        //gamePadVibrationSpeedLeft = 65535;
        g_GamePadDat[cIndex].gamePadVibrationSpeedLeft = 32767;
        gamePadVibrationUpdate();
    }
#endif
}

void SetControllerMainRightVibration(int frame, int cIndex)
{
#ifdef SWITCH_MODE
#else
    if (frame > 0)
    {
        g_GamePadDat[cIndex].gamePadVibrationFrameRight = frame;
        //gamePadVibrationSpeedRight = 65535;
        g_GamePadDat[cIndex].gamePadVibrationSpeedRight = 32767;
        gamePadVibrationUpdate();
    }
#endif
}


Float3 GetControllerMainLeftAcceleration()
{
#ifdef SWITCH_MODE
#else
    return { 0.0f, 0.0f, 0.0f };
#endif
}

Float3 GetControllerMainRightAcceleration()
{
#ifdef SWITCH_MODE
#else
    return { 0.0f, 0.0f, 0.0f };
#endif
}


Float3 GetControllerMainLeftAngle()
{
#ifdef SWITCH_MODE
#else
    return { 0.0f, 0.0f, 0.0f };
#endif
}

Float3 GetControllerMainRightAngle()
{
#ifdef SWITCH_MODE
#else
    return { 0.0f, 0.0f, 0.0f };
#endif
}


bool GetControllerMainTouchScreen()
{
#ifdef SWITCH_MODE
#else
    return g_GamePadDat[0].gameMouseState.leftButton;
#endif
}

Float2 GetControllerMainTouchScreenPosition()
{
#ifdef SWITCH_MODE
#else
    return MkF2( (float)((float)g_GamePadDat[0].gameMouseState.x * (1.0f / DX_WINDOW_SCREEN_MOD) - (SCREEN_WIDTH / 2)),
        (float)((float)g_GamePadDat[0].gameMouseState.y * (1.0f / DX_WINDOW_SCREEN_MOD) - (SCREEN_HEIGHT / 2)));
#endif
}


bool GetControllerMainMouseBtnLeftPress()
{
#ifdef SWITCH_MODE
#else
    return mouseLPushFrame > 0;
#endif
}
bool GetControllerMainMouseBtnLeftTrigger()
{
#ifdef SWITCH_MODE
#else
    return mouseLPushFrame == 1;
#endif
}
bool GetControllerMainMouseBtnLeftRelease()
{
#ifdef SWITCH_MODE
#else
    return mouseLPushFrame == -1;
#endif
}


bool GetControllerMainMouseBtnMiddlePress()
{
#ifdef SWITCH_MODE
#else
    return mouseMPushFrame > 0;
#endif
}
bool GetControllerMainMouseBtnMiddleTrigger()
{
#ifdef SWITCH_MODE
#else
    return mouseMPushFrame == 1;
#endif
}
bool GetControllerMainMouseBtnMiddleRelease()
{
#ifdef SWITCH_MODE
#else
    return mouseMPushFrame == -1;
#endif
}

bool GetControllerMainMouseBtnRightPress()
{
#ifdef SWITCH_MODE
#else
    return mouseRPushFrame > 0;
#endif
}
bool GetControllerMainMouseBtnRightTrigger()
{
#ifdef SWITCH_MODE
#else
    return mouseRPushFrame == 1;
#endif
}
bool GetControllerMainMouseBtnRightRelease()
{
#ifdef SWITCH_MODE
#else
    return mouseRPushFrame == -1;
#endif
}


#ifdef SWITCH_MODE
#else
void gamePadVibrationUpdate(void)
{
    for (int i = 0; i < CONTROLLER_DEVICE_MAX; i++)
    {
        XINPUT_VIBRATION vibration = {};
        vibration.wLeftMotorSpeed = g_GamePadDat[i].gamePadVibrationSpeedLeft;
        vibration.wRightMotorSpeed = g_GamePadDat[i].gamePadVibrationSpeedRight;
        XInputSetState(i, &vibration);
    }
}
#endif