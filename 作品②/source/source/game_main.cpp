// ===================================================
// game_main.cpp ゲーム自体のメイン
// 
// hara sougo  2024/07/22
// ===================================================
#define IS_MAIN
#include "main.h"
#include "Game/Systems/sound.h"
#include "Game/Sound/soundData.h"
#include "game_main.h"
#include "Game/Texture/texture.h"
#include "Game/Texture/texData.h"
#include "Game/Systems/font.h"
#include "Game/Polygon/polygon.h"
#include "Game/Polygon/modelData.h"
#include "Game/Sprite/sprite.h"
#include "Game/Sprite/spriteNumber.h"

#include "Core/app_event.h"
#include "Core/Input/controller_main.h"
#include "Game/Managers/gameManager.h"

#include "Core/Multi/winsock.h"
#include "Core/Multi/winsock_server.h"
#include "Core/Multi/winsock_client.h"

// ===================================================
// プロトタイプ宣言
// ===================================================

// ===================================================
// グローバル宣言
// ===================================================
static unsigned int g_GameFrame = 0;

//static bool g_GameDrawSkipFlag = false;

// ===================================================
// 初期化
// ===================================================
void GameMainInitialize(void)
{
	setlocale(LC_CTYPE, "jpn");

    //InitPolygon();

    AppEventFileLoadStartBefore();

    int lfmax = 0;
    lfmax += (int)InitSoundDataFiles();
    lfmax += (int)InitTexDataFiles() 
        * 2
        ;
    lfmax += (int)InitFontDataFiles();
    lfmax += (int)InitModelDataFiles();

    AppEventFileLoadStart(lfmax);

	// システム系の初期化
	InitSound();
	InitSoundData();

	InitSystem();
	InitTexture();
	InitTexData();
	InitFont();
	InitPolygon();
    InitModelData();
	InitSprite();
	SpriteNumberInitialize();
	ControllerMainInit();

    WinSockInitialize();

    //キャッシュするために一度描画処理をしておく
    PolygonTextureDrawAll(GetTexIdAll());
    FileLoadAddBefore(0, FILELOADFINISHWAIT_TXT, FILELOADADD_TYPE_FINAL);

	InitGameManager();

    SetMainGameDrawFPS(1);

    AppEventFileLoadEnd();
    SetWinMainStartGame();
}

// ===================================================
// 更新
// ===================================================
void GameMainUpdate(void)
{
    TexDataBuffClear();

	// システム系更新;
	ControllerMainUpdate();
	UpdateGameManager();

	UpdateSound();

	g_GameFrame++;
}

// ===================================================
// 描画
// ===================================================
void GameMainDraw(void)
{
    if (GetWinMainLoopDrawThroughFlag())
    {
        DrawGameManager(false);

        DrawCameraBufferReleaseOnly();

        PolygonBufferReleaseOnly();

        DrawFontBufferReleaseOnly();
    }
    else
    {
        // 画面クリア処理
        SystemClearB();

        DrawGameManager(true);

        ////ポリゴンの描画情報をメインバッファに描きこむ
        PolygonBufferRelease();

        ////フォントの描画情報をメインバッファに描きこむ
        DrawFontBufferRelease();

        ////ポリゴンのメインバッファを描きこむ
        PolygonMainBufRelease();

        // バッファ切り替え
        SystemSwapBuffers();
    }
}

// ===================================================
// 終了処理
// ===================================================
void GameMainFinalize(void)
{
	// システム系の終了処理
	UninitGameManager();

    if (WinSockGetType() == WINSOCK_CONNECT_TYPE::SERVER)
    {
        WinSockServer_End();
    }
    else if (WinSockGetType() == WINSOCK_CONNECT_TYPE::CLIENT)
    {
        WinSockClient_End();
    }

    WinSockFinalize();

	ControllerMainUninit();
	UninitSprite();
    UninitModelData();
    UninitPolygon();
	UninitFont();
	UninitTexData();
	UninitTexture();
	UninitSystem();

	UninitSoundData();
	UninitSound();
}

unsigned int GetMainGameFrame()
{
	return g_GameFrame;
}

void FileLoadAddBefore(int n, std::string txt, int type)
{
    DWORD time = timeGetTime(); // システム起動からの経過時間（ミリ秒）

    std::string txt2 = txt;
    for (int i = 0; i < (int)((time / 150) % 4); i++) txt2 += ".";

    AppEventFileLoadAddBefore(n, txt2, type);
}

void FileLoadAdd(int n, std::string txt, int type)
{
    //AppEventFileLoadAddResult r = 
        AppEventFileLoadAdd(n, txt, type);

    //進捗表示
    //{
    //    // 画面クリア処理
    //    SystemClearB();

    //    PolygonDrawLoadProgress(r.loadfilesCur, r.loadfilesMax);

    //    // バッファ切り替え
    //    SystemSwapBuffers();
    //}
}

void SetMainGameDrawFPS(int n)
{
    //g_GameDrawFps = std::max(n, 1);
    SetWinMainLoopDrawFPS(n);

}

void SetMainGameDrawAutoFPSEnable(bool b)
{
    //g_GameDrawAutoFps = b;
    SetWinMainLoopDrawAutoFPSEnable(b);
}
