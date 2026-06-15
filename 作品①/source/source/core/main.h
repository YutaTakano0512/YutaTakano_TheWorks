#pragma once

//=============================================================================
// ウィンドウ・画面定数
//=============================================================================
#define SCREEN_WIDTH  1280
#define SCREEN_HEIGHT  720

#define CLASS_NAME  "2DActionGame"
#define WINDOW_NAME "2D Action Game"

//=============================================================================
// Win32 / DirectX 共通インクルード
//=============================================================================
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <mmsystem.h>
#pragma comment(lib, "winmm.lib")

#include <d3d11.h>
#pragma comment(lib, "d3d11.lib")

#include <DirectXMath.h>

// Direct2D / DirectWrite
#include <d2d1.h>
#pragma comment(lib, "d2d1.lib")
#include <dwrite.h>
#pragma comment(lib, "dwrite.lib")

// DirectXTex
#include "../../DirectXTex.h"
#ifdef _DEBUG
#pragma comment(lib, "DirectXTex_Debug.lib")
#else
#pragma comment(lib, "DirectXTex_Release.lib")
#endif

// XAudio2
#include <xaudio2.h>
#pragma comment(lib, "xaudio2.lib")

// XInput (ゲームパッド)
#include <xinput.h>
#pragma comment(lib, "xinput.lib")

// プロジェクト共通数学
#include "../math/vector2.h"
#include "../math/mathHelper.h"

//=============================================================================
// ウィンドウハンドル取得
//=============================================================================
HWND GetWindowHandle();
