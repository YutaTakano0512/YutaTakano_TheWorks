// ===================================================
// SCENE_Op.cpp オープニングシーン制御
// 
// hara sougo  2025/09/27
// ===================================================
#include "../Game/Systems/sound.h"
#include "../main.h"
#include "../game_main.h"

#include "SCENE_Op.h"

#include "../Game/Texture/texData.h"
#include "../Game/Managers/bg.h"
#include "../Core/Input/controller_main.h"
#include "../Core/Input/keyboard.h"

#include "../Game/Systems/fade.h"
#include "../Game/Managers/scene.h"

#include "../Game/Sprite/sprite.h"
#include "../Game/Sprite/spriteGraph.h"
#include "../Game/Game Object/gameObject.h"
#include "../Game/Game Object/gameObjectGraph.h"
#include "../Game/Managers/gameManager.h"
#include "../Game/Managers/soundManager.h"
#include "../Game/Systems/camera.h"
#include "../Game/Systems/font.h"

#include "../Game/Systems/ui_Game.h"
#include "../Game/Systems/ui.h"
#include "../Game/Systems/movie.h"

#include "../Core/Multi/winsock.h"
#include "../Core/Multi/winsock_server.h"
#include "../Core/Multi/winsock_client.h"

static int g_frameCount = 0;

// ===================================================
// シーンの初期化
// ===================================================
void InitializeSceneOp(void)
{
    g_frameCount = 0;
    SetMainGameDrawAutoFPSEnable(false);

	//g_tex_title = GetTexData(TEX_TYPE_UI_TITLE);
	//g_tex_title_start = GetTexData(TEX_TYPE_UI_TITLE_START_TEXT);


    //マルチ終了処理
    if (WinSockGetType() == WINSOCK_CONNECT_TYPE::SERVER)
    {
        WinSockServerPlayerDatClear();
    }
    if (WinSockGetType() == WINSOCK_CONNECT_TYPE::CLIENT)
    {
        WinSockClient_ServerConnectCheck();
    }

	UIReset();
	//ShowGameFrame(true);

    //UI初期化
    InitUI_Game();

	ResetCamera();

	GameStopResetRequest();

    //
    GameManagerSetSceneRequestCancel();

    //シェーダを消す
    //SetPostEffectShader("");
    SetPostEffectShaderVS("");
    SetPostEffectShaderPS("");

    SetBg(BG_TYPE_NONE);

    //既存のBGMを消す
    SoundBGMStopRequest();

    //ムービーの再生
    LoadMovieSingle(
        "rom/movie/op_movie.mp4"
    );

    GameManagerSetMovieFlag(true, true);

    SoundBGMPlayRequest(
        GetSoundData("SOUND/BGM/op_movie"),
        false,
        1.85f);

}

// ===================================================
// タイトルシーンの更新
// ===================================================
void UpdateSceneOp(void)
{
    UpdateUI_Game();
    if (IsFading())
    {
        return;
    }

    g_frameCount++;

    if (g_frameCount == 60 * 57)
    {
        GameManagerSetMovieFlag(false);
    }

    if (g_frameCount >= 60 * 58)
    {
        SetScene(SCENE_TITLE);
    }
    else if (g_frameCount < 60 * 57
        && GameManagerIsMovieSkipTrigger())
    {
        GameManagerSetMovieFlag(false);
        SetSceneFade(SCENE_TITLE);
    }
}

// ===================================================
// タイトルシーンの描画
// ===================================================
void DrawSceneOp(void)
{
    DrawUI_Game();
}

// ===================================================
// タイトルシーンの終了
// ===================================================
void FinalizeSceneOp(void)
{
    //既存のBGMを消す
    SoundBGMStopRequest();

    UnloadMovie();
	//FinalizeTitlebg();

    UninitUI_Game();

    GameManagerSetMovieFlag(false);

    SetMainGameDrawAutoFPSEnable(true);
}
