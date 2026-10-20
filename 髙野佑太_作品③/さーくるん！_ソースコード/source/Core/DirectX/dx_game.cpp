
//---------------------------------------------------------------------------
// game.cpp
// 
// sougo hara  2024/05/14
//---------------------------------------------------------------------------

#include "../../Game/Configuration/configuration.h"
#include "../../main.h"

#include "../Input/keyboard.h"
#include "../Input/mouse.h"

#include "dx.h"
#include "dx_game.h"
#include "dx_camera.h"
#include "dx_sound.h"

#include "dx_shader.h"
#include "dx_model.h"

#include "../../game_main.h"

static unsigned int g_GameFrame = 0;


//
// ゲーム 初期化
//
void DX_GameInitialize(GAME_INIT_SET set)
{
	DX_INIT_SET dx_set = {};
	dx_set.hWnd = set.hWnd;

	DX_INIT_RESULT dx_init_result = {};
	DXInitialize(dx_set, &dx_init_result);

	//TextureInitialize(dx_init_result.device);

	DX_ShaderInitialize(dx_init_result.device, dx_init_result.deviceContext);
	//SpriteInitialize(dx_init_result.device, dx_init_result.deviceContext);
	DX_SoundInitialize();
	DX_CameraInitialize();

    DX_ModelInitialize();

    GameMainInitialize();
}

//
// ゲーム 終了処理
//
void DX_GameFinalize(void)
{
    GameMainFinalize();

    DX_ModelFinalize();

	DX_CameraFinalize();
	DX_SoundFinalize();
	//SpriteFinalize();
	DX_ShaderFinalize();

	//TextureFinalize();

	DXFinalize();
}

//
// ゲーム 更新
//
void DX_GameUpdate(void)
{
    GameMainUpdate();

	g_GameFrame++;
}

//
// ゲーム 描画
//
void DX_GameDraw(void)
{
    GameMainDraw();
}


//
// ゲーム ループ
//
void DX_GameLoop(void)
{
	DX_GameUpdate();
	DX_GameDraw();
}


unsigned int DX_GameGetFrame()
{
	return g_GameFrame;
}