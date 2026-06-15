// ===================================================
// game_main.h ゲーム自体のメイン
// 
// hara sougo  2024/07/22
// ===================================================
#ifndef _GAME_MAIN_H_
#define _GAME_MAIN_H_

#include "Game/Systems/system.h"
#include <algorithm>

#include "Core/_systemModeSwitch.h"

#include "Game/Systems/sys_general.h"

#ifdef SWITCH_MODE
#else
#include "Core/debugPrintf.h"
#include "Core/debug_ostream.h"
#endif


// ===================================================
// マクロ定義
// ===================================================
#define GAME_DAT_PATH ("source/")

constexpr int FRAME_RATE = (60);
constexpr int FRAME_ONE_SECOND = (FRAME_RATE);
constexpr int FRAME_ONE_MINUTE = (FRAME_ONE_SECOND * 60);

constexpr int SCREEN_WIDTH = (1920);
constexpr int SCREEN_HEIGHT = (1080);
//#define SCREEN_WIDTH (1280)
//#define SCREEN_HEIGHT (720)

//#define SCREEN_WIDTH_WINDOW (1280)
//#define SCREEN_HEIGHT_WINDOW (720)

// SCREEN_WIDTH_MOD  ((float)SCREEN_WIDTH_WINDOW / SCREEN_WIDTH)
//#define SCREEN_HEIGHT_MOD  ((float)SCREEN_HEIGHT_WINDOW / SCREEN_HEIGHT)

//#define SCREEN_WIDTH ((float)16)
//#define SCREEN_HEIGHT ((float)9)
//#define WINDOW_SCREEN_WIDTH (1280)
//#define WINDOW_SCREEN_HEIGHT (720)

#define NUM_VERTEX_QUADS		(4)	// 頂点数(四角形)

#define BGM_VOLUME				(0.4f)
#define BGM_VOLUME_PAUSE_MOD	(0.5f)


// ===================================================
// 列挙体宣言
// ===================================================

// ===================================================
// 構造体宣言
// ===================================================
struct VERTEX_3D // 頂点情報
{
	Float3 Position;	// 座標
	Float4 Color;		// 色
	Float2 TexCoord;	// テクスチャ座標
};

// ===================================================
// プロトタイプ宣言
// ===================================================
void GameMainInitialize(void);
void GameMainUpdate(void);
void GameMainDraw(void);
void GameMainFinalize(void);

unsigned int GetMainGameFrame();

constexpr auto FILELOADADDBEFORE_TXT_TEX = "LOADING TEXTURE";
constexpr auto FILELOADADDBEFORE_TXT_FONT = "LOADING FONT";
constexpr auto FILELOADADDBEFORE_TXT_SOUND = "LOADING SOUND";
constexpr auto FILELOADADDBEFORE_TXT_MODEL = "LOADING SOUND";
constexpr auto FILELOADADDBEFORE_TXT_CACHE = "LOADING RESOURCE";
constexpr auto FILELOADADD_TXT_TEX = "LOADED TEXTURE";
constexpr auto FILELOADADD_TXT_FONT = "LOADED FONT";
constexpr auto FILELOADADD_TXT_SOUND = "LOADED SOUND";
constexpr auto FILELOADADD_TXT_MODEL = "LOADED MODEL";
constexpr auto FILELOADADD_TXT_CACHE = "LOADED RESOURCE";
constexpr auto FILELOADFINISHWAIT_TXT = "FINISH";
constexpr int FILELOADADD_TYPE_NORMAL = 0;
constexpr int FILELOADADD_TYPE_FINAL = 2;
void FileLoadAddBefore(int n, std::string txt, int type);
void FileLoadAdd(int n, std::string txt, int type);

// 60 ÷ n FPSになる
void SetMainGameDrawFPS(int n);
void SetMainGameDrawAutoFPSEnable(bool b);

#endif