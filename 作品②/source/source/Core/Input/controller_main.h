// ===================================================
// controller_main.h  ゲームのコントローラー
// 
// hara sougo  2024/07/22
// ===================================================
#ifndef _CONTROLLER_MAIN_H_
#define _CONTROLLER_MAIN_H_

#include "../../Game/Systems/system.h"

#include "../_systemModeSwitch.h"

#include "../../Game/Systems/sys_general.h"

#include "keyboard.h"
#include "mouse.h"

// ===================================================
// マクロ定義
// ===================================================

// ===================================================
// 列挙体宣言
// ===================================================
enum CONTROLLER_BUTTON_TYPE {
	CONTROLLER_BUTTON_TYPE_A = 0,
	CONTROLLER_BUTTON_TYPE_B,
	CONTROLLER_BUTTON_TYPE_X,
	CONTROLLER_BUTTON_TYPE_Y,
    CONTROLLER_BUTTON_TYPE_L,
    CONTROLLER_BUTTON_TYPE_R,
    CONTROLLER_BUTTON_TYPE_ZL,
    CONTROLLER_BUTTON_TYPE_ZR,

    CONTROLLER_BUTTON_TYPE_LEFT,
    CONTROLLER_BUTTON_TYPE_RIGHT,
    CONTROLLER_BUTTON_TYPE_UP,
    CONTROLLER_BUTTON_TYPE_DOWN,

    CONTROLLER_BUTTON_TYPE_STICK_L,
    CONTROLLER_BUTTON_TYPE_STICK_R,

    CONTROLLER_BUTTON_TYPE_PLUS,
    CONTROLLER_BUTTON_TYPE_MINUS,

	CONTROLLER_BUTTON_TYPE_MAX,
};

//typedef int CONTROLLER_PUSH_FRAMES[CONTROLLER_BUTTON_TYPE_MAX];

// ===================================================
// 構造体宣言
// ===================================================
struct CONTROLLER_STICK_DAT {
    bool enable;
    Float2 stick;
    int frame;
};
struct CONTROLLER_INPUT_DAT {
    CONTROLLER_STICK_DAT stickLeft;
    CONTROLLER_STICK_DAT stickRight;
    int buttonPushFrame[CONTROLLER_BUTTON_TYPE_MAX];
};

// ===================================================
// プロトタイプ宣言
// ===================================================

#ifndef NO_CONTROLLER

void ControllerMainInit(void);
void ControllerMainUpdate(void);
void ControllerMainUninit(void);

void SetControllerMainInputDataFixed(int index, CONTROLLER_INPUT_DAT inputDat);
CONTROLLER_INPUT_DAT GetControllerMainInputData(int cIndex);
bool IsControllerMainGamePadConnected(int cIndex);

bool GetControllerMainPress(CONTROLLER_BUTTON_TYPE button, int cIndex = 0);
bool GetControllerMainTrigger(CONTROLLER_BUTTON_TYPE button, int cIndex = 0);
bool GetControllerMainRelease(CONTROLLER_BUTTON_TYPE button, int cIndex = 0);

Float2 GetControllerMainLeftStick(int cIndex = 0);
Float2 GetControllerMainRightStick(int cIndex = 0);

CONTROLLER_STICK_DAT GetControllerMainLeftStickTrigger(int cIndex = 0);
CONTROLLER_STICK_DAT GetControllerMainRightStickTrigger(int cIndex = 0);

CONTROLLER_STICK_DAT GetControllerMainLeftStickPress(int cIndex = 0);
CONTROLLER_STICK_DAT GetControllerMainRightStickPress(int cIndex = 0);

CONTROLLER_STICK_DAT GetControllerMainLeftStickRelease(int cIndex = 0);
CONTROLLER_STICK_DAT GetControllerMainRightStickRelease(int cIndex = 0);

Float3 GetControllerMainLeftAcceleration();
Float3 GetControllerMainRightAcceleration();

Float3 GetControllerMainLeftAngle();
Float3 GetControllerMainRightAngle();

bool GetControllerMainTouchScreen();
Float2 GetControllerMainTouchScreenPosition();


bool GetControllerMainMouseBtnLeftPress();
bool GetControllerMainMouseBtnLeftTrigger();
bool GetControllerMainMouseBtnLeftRelease();

bool GetControllerMainMouseBtnMiddlePress();
bool GetControllerMainMouseBtnMiddleTrigger();
bool GetControllerMainMouseBtnMiddleRelease();

bool GetControllerMainMouseBtnRightPress();
bool GetControllerMainMouseBtnRightTrigger();
bool GetControllerMainMouseBtnRightRelease();

#endif // !NO_CONTROLLER

void SetControllerMainLeftVibration(int frame, int cIndex = 0);
void SetControllerMainRightVibration(int frame, int cIndex = 0);


#endif