// ===================================================
// SCENE_Game.cpp ゲームシーン制御
// 
// hara sougo  2024/07/22
// ===================================================
#include "../game_main.h"

#include "SCENE_Title.h"
#include "SCENE_Game.h"
#include "SCENE_SingleBossBattle.h"

#include "../Core/Input/controller_main.h"
#include "../Game/Entities/player.h"
#include "../Game/Entities/Block/block.h"
#include "../Game/Game Object/gameObjectPrefab.h"
#include "../Game/Entities/Block/blockPrefab.h"

#include "../Game/Managers/bg.h"

#include "../Game/Systems/fade.h"
#include "../Game/Managers/scene.h"

#include "../Game/Game Map/gameMap.h"
#include "../Game/Managers/gameManager.h"
#include "../Game/Managers/soundManager.h"

#include "../Game/Systems/camera.h"
#include "../Game/Entities/character.h"

#include "../Game/Systems/font.h"

#include "../Game/Systems/ui.h"
#include "../Game/Systems/ui_Game.h"

#include "../Game/Managers/StageManager.h"
#include "../Game/Game Stage/Stage_Tutorial.h"
#include "../Game/Game Stage/Stage_Ruins.h"
#include "../Game/Game Events/GE_Gaya.h"

#include "../Game/polygon/modelData.h"

#include "../Core/Multi/winsock.h"
#include "../Core/Multi/winsock_server.h"
#include "../Core/Multi/winsock_client.h"

#include "../Game/Systems/ui_Confirm.h"
#include "../Game/Systems/ui_PauseMenu.h"
#include "../Game/Systems/ui_ContinueMenu.h"

#if defined(_DEBUG) || defined(DEBUG)
#include "../Game/Game Object/Game Object Events/GOE_Item.h"
#include "../Game/Game Object/Game Object Events/GOE_ItemTransporter.h"
#endif


// ===================================================
// 構造体宣言
// ===================================================
struct SCENE_GAME_G_V {
    bool gamePause;
    bool gamePauseCancelFlag;


    StageManager* g_StageManager = nullptr;

    UI_PauseMenu pauseMenu;
    UI_ContinueMenu continueMenu;

    BOOL g_gameSceneEnableFlag = false;
    float g_pauseBeforeBgmVolume = 1.0f;
};

//ゲームシーンのデータとか
static SCENE_GAME_G_V g_sceneGame = {};



#if defined(_DEBUG) || defined(DEBUG)
static bool g_debugcameraflag = false;

#endif

// ===================================================
// ゲームシーンの初期化
// ===================================================
void InitializeSceneGame(void)
{
    g_sceneGame = {};

    g_sceneGame.g_gameSceneEnableFlag = false;

    if (GetMultiFlag() == MULTI_FLAG::LOG_IN
        && WinSockGetType() == WINSOCK_CONNECT_TYPE::CLIENT)
    {
        SetScene(SCENE_MULTICLIENT);
        return;
    }

    g_sceneGame.pauseMenu.init();
    g_sceneGame.continueMenu.init();

    //SetMainGameDrawFPS(2);
    SetMainGameDrawFPS(1);

    {
        GameManagerResetResult();
        GameManagerResetResultAddScoreBuf();

        //ムービーフラグ
        GameManagerSetMovieFlag(false);

        UIReset();
        //ShowGameFrame(true);

        ResetCamera();

        GameStopResetRequest();

        //UI初期化
        InitUI_Game();


        //既存のブロックとオブジェクトを消す
        DeleteBlockAll();
        DeleteGameObjectAll();

        //プレイヤーの初期化
        InitPlayer();

        {
            //CAMERA cinfo = GetCameraInfo();
            //SetCameraZoom(1.75f);

            SetCameraModeAll(CAMERA_MODE_PLAYER_CHASE);

            //SetCameraPos(MkF2(cinfo.trans.pos.x, cinfo.trans.pos.y + BLOCK_HIGHT * 2));
            //SetCameraPos(MkF2(0.0f, 0.0f + BLOCK_HIGHT * 2));

            //SetCameraZoom(1.4f);
            //SetCameraPos(MkF2(0.0f + BLOCK_HIGHT * -6, 0.0f));

            //SetCameraZoom(1.0f);
            //SetCameraPos(MkF2(0.0f, 0.0f));
        }

        //ステージの読み込み

//#if defined(_DEBUG) || defined(DEBUG)
//        if (Keyboard_IsKeyDown(KK_H)) //仮
//        {
//            g_Stage = new Stage_Ruins(Difficulty::Hard);
//        }
//        else
//#endif
//        {
//            //g_Stage = new Stage_Ruins(Difficulty::Normal);
//            g_Stage = new Stage_Ruins(GetDifficultySceneTitle());
//        }
//
//        g_Stage->Init();


        //{
        //    //g_Stage = new Stage_Ruins(Difficulty::Normal);
        //    g_Stage = new Stage_Tutorial(GetDifficultySceneTitle());
        //    g_Stage->Init();
        //}

        g_sceneGame.g_StageManager = new StageManager(
            GetDifficultySceneTitle(),
            IsSingleBossBattleFlag() ? 
                GetSelectNumberSingleBossBattle(): -1
        );
        g_sceneGame.g_StageManager->Init();
    }

    g_sceneGame.g_gameSceneEnableFlag = true;

	//(void)_heapmin();
}

// ===================================================
// ゲームシーンの更新
// ===================================================
void UpdateSceneGame(void)
{
    if (!g_sceneGame.g_gameSceneEnableFlag)
    {
        return;
    }

	// シーン遷移(デバッグテスト用)
	//if (GetControllerMainTrigger(CONTROLLER_BUTTON_TYPE_PLUS))
	//{
	//	SetSceneFade(SCENE_RESULT);
	//	//SetScene(SCENE_RESULT);//リザルトシーンへ移行
	//}

#ifdef SWITCH_MODE
#else
	// シーン遷移(デバッグテスト用)
	if (Keyboard_IsKeyTrigger(KK_DELETE))
	{
        //マルチ終了処理
        if (WinSockGetType() == WINSOCK_CONNECT_TYPE::SERVER)
        {
            WinSockServerPlayerDatClear();
        }
		SetSceneFade(SCENE_TITLE);
	}

#if defined(_DEBUG) || defined(DEBUG)
    if (Keyboard_IsKeyTrigger(KK_RIGHTCONTROL))
    {
        if (Keyboard_IsKeyDown(KK_D4))
        {//SS
            GameManagerSetResultRank(4);
        }
        else if (Keyboard_IsKeyDown(KK_D3))
        {//S
            GameManagerSetResultRank(3);
        }
        else if (Keyboard_IsKeyDown(KK_D2))
        {//A
            GameManagerSetResultRank(2);
        }
        else if (Keyboard_IsKeyDown(KK_D1))
        {//B
            GameManagerSetResultRank(1);
        }
        else
        {//C 
            GameManagerSetResultRank(0);
        }
        SetSceneFade(SCENE_RESULT);
    }

    if (Keyboard_IsKeyTrigger(KK_I))
    {
        {
            GOE_Item* item = GOE_Item::CreateItem(
                GOE_Item::Type::Heal,
                {
                    96.0f * -200.0f,
                    0.0f,
                    96.0f * -200.0f,
                }
            );

            GOE_ItemTransporter* tsptr = GOE_ItemTransporter::Create(
                {
                    96.0f * -200.0f,
                    0.0f,
                    96.0f * -14.0f,
                },
                item->GetGameObj(),
                GetPlayer(0)->obj
            );
            tsptr->StartTransport();
        }
    }
#endif

#endif

    if (g_sceneGame.gamePauseCancelFlag)
    { //ポーズ解除
        g_sceneGame.gamePauseCancelFlag = false;

        g_sceneGame.gamePause = false;
        GamePause(g_sceneGame.gamePause);

        SoundEffectPlayRequest(SOUND_TYPE_SE_UI_CANCEL, 4.0f, 0.0f);

        //ポーズ解除時にBGMの音量を戻す
        SoundBGMSetVolumeRequest(g_sceneGame.g_pauseBeforeBgmVolume, 0);

        //SoundEffectPlayRequest(SOUND_TYPE_PAUSE_CANCEL, 0.4f, 0);
    }


    //UIの更新
	UpdateUI_Game();

    //プレイヤーの更新
	UpdatePlayer();

    if (!IsGamePause())
    { //ポーズ中ではない
        if (g_sceneGame.g_StageManager != nullptr)
        { //ステージの更新
            g_sceneGame.g_StageManager->Update();
        }
    }

    if (g_sceneGame.g_StageManager != nullptr
        && g_sceneGame.g_StageManager->IsGameoverConfirm())
    {
        g_sceneGame.gamePause = false;
        GamePause(false);
        GameStopRequest(60);

        //ポーズ解除時にBGMの音量を戻す
        SoundBGMSetVolumeRequest(g_sceneGame.g_pauseBeforeBgmVolume, 0);

        if (!IsFading())
        {
            bool menuExit = g_sceneGame.continueMenu.update();
            if (menuExit)
            {
            }
        }
    }
    else
    {
        //ゲーム中のポーズ
        if (
            g_sceneGame.g_StageManager == nullptr
            || !g_sceneGame.g_StageManager->GetWaitFlag()
            )
        {
            //ポーズの処理
            if (!IsFading())
            { //ポーズ
                if (g_sceneGame.gamePause)
                {
                    //if (g_sceneGame.pauseMenu.get_state()
                    //    != SCENE_GAME_PauseMenu::State::Default
                    //)
                    //{
                    //}
                    //else
                    //{
                    //    //ポーズ解除
                    //    if (GetControllerMainTrigger(CONTROLLER_BUTTON_TYPE_PLUS)
                    //        || GetControllerMainTrigger(CONTROLLER_BUTTON_TYPE_MINUS)
                    //        || GetControllerMainTrigger(CONTROLLER_BUTTON_TYPE_B)
                    //        )
                    //    {
                    //        g_sceneGame.gamePause = false;
                    //        GamePause(g_sceneGame.gamePause);

                    //        //ポーズ解除時にBGMの音量を戻す
                    //        SoundBGMSetVolumeRequest(g_sceneGame.g_pauseBeforeBgmVolume, 0);

                    //        //SoundEffectPlayRequest(SOUND_TYPE_PAUSE_CANCEL, 0.4f, 0);
                    //    }
                    //}

                    //if (g_sceneGame.gamePause)
                    //{
                    //    g_sceneGame.pauseMenu.update();
                    //}

                    bool menuExit = g_sceneGame.pauseMenu.update();
                    if (menuExit)
                    {
                        g_sceneGame.gamePauseCancelFlag = true;
                    }

                }
                else// if (!IsFading()) //�|�[�Y
                {
                    if (GetControllerMainTrigger(CONTROLLER_BUTTON_TYPE_PLUS)
                        || GetControllerMainTrigger(CONTROLLER_BUTTON_TYPE_MINUS))
                    {
                        if (!GameManagerIsMovieFlag()) //ムービー中はポーズ無効
                        {
                            g_sceneGame.gamePause = true;
                            GamePause(g_sceneGame.gamePause);

                            g_sceneGame.g_pauseBeforeBgmVolume = GetSoundBGMVolume();
                            //�|�[�Y��BGM�̉��ʂ�������
                            SoundBGMSetVolumeRequest(g_sceneGame.g_pauseBeforeBgmVolume
                                * BGM_VOLUME_PAUSE_MOD, 0);

                            SoundEffectPlayRequest(SOUND_TYPE_SE_UI_OK, 4.0f, 0.0f);

                            //SoundEffectPlayRequest(SOUND_TYPE_PAUSE, 0.4f, 0);
                        }
                    }
                }
            }
        }
    }

    //if (GetControllerMainTrigger(CONTROLLER_BUTTON_TYPE_B) && g_sceneGame.gamePause)
    //{
    //    g_sceneGame.gamePause = false;
    //    GamePause(g_sceneGame.gamePause);
    //}

#if defined(_DEBUG) || defined(DEBUG)

    static bool kpCamera = false;
    //if(!WinSockClientIsEnable())
    {
        if (Keyboard_IsKeyTrigger(KK_C))
        {
            if (!kpCamera)
            {
                kpCamera = true;
                SetCameraMode(0, CAMERA_MODE_NORMAL);
            }
            else
            {
                kpCamera = false;
                SetCameraMode(0, CAMERA_MODE_PLAYER_CHASE);
            }
        }
    }

    {
        static bool change = false;
        if (kpCamera)
        //if (g_sceneGame.gamePause)
        {
            //static int modMouseMidScroll = 0;

            //constexpr int S_EDIT_CAMERA_ZOOM_SPEED = 0.3f;
            //constexpr int clickokframe = 5;
            //static MouseState_tag l_GameMouseState = {};
            //Mouse_GetState(&l_GameMouseState);
            ////DebugPrintf("%d", l_GameMouseState.scrollWheelValue);

            CAMERA cinfo = GetCameraInfo(0);

            {
                //{
                //    static bool crekeypush = false;
                //    if (l_GameMouseState.middleButton
                //        && !crekeypush)
                //    {
                //        //if (isForegroundWindowFrame > clickokframe)
                //        {
                //            modMouseMidScroll = l_GameMouseState.scrollWheelValue;
                //        }

                //        crekeypush = true;
                //    }
                //    else
                //    {
                //        crekeypush = false;
                //    }
                //}

                {
                    //Float2 ls = GetControllerMainRightZStick();
                    ////Float2 ls = GetControllerMainLeftStick();
                    Float2 ls = {};
                    //if (GetControllerMainPress(CONTROLLER_BUTTON_TYPE_LEFT, 0))
                        if (Keyboard_IsKeyDown(KK_F))
                    {
                        ls.x += -1.0f;
                        change = true;
                    }
                    //if (GetControllerMainPress(CONTROLLER_BUTTON_TYPE_RIGHT, 0))
                        if (Keyboard_IsKeyDown(KK_H))
                    {
                        ls.x += 1.0f;
                        change = true;
                    }
                    //if (GetControllerMainPress(CONTROLLER_BUTTON_TYPE_UP, 0))
                        if (Keyboard_IsKeyDown(KK_T))
                    {
                        ls.y += -1.0f;
                        change = true;
                    }
                    //if (GetControllerMainPress(CONTROLLER_BUTTON_TYPE_DOWN, 0))
                        if (Keyboard_IsKeyDown(KK_G))
                    {
                        ls.y += 1.0f;
                        change = true;
                    }

                    Float2 ls2 = {};
                    if (Keyboard_IsKeyDown(KK_Q))
                    {
                        ls2.x += -1.0f;
                        change = true;
                    }
                    if (Keyboard_IsKeyDown(KK_E))
                    {
                        ls2.x += 1.0f;
                        change = true;
                    }

                    float zm = 0.0f;
                    if (Keyboard_IsKeyDown(KK_Z))
                    {
                        if (Keyboard_IsKeyDown(KK_LEFTSHIFT))
                        {
                            zm = -0.004f;
                        }
                        else
                        {
                            zm = 0.004f;
                        }
                        change = true;
                    }

                    if (change)
                    {
                        g_debugcameraflag = true;

                        //SetCameraZoom(0, (float)(1.0f + (float)(l_GameMouseState.scrollWheelValue - modMouseMidScroll) / 1000 * S_EDIT_CAMERA_ZOOM_SPEED));
                        SetCameraZoom(0,
                            std::min(std::max(
                                cinfo.trans.zoom + zm,
                                0.15f), 2.0f)
                        );

                        SetCameraPos(0,
                            MkF3(cinfo.trans.pos.x + ls.x * 25.0f,
                                cinfo.trans.pos.y + ls.y * 25.0f,
                                0)
                        );
                        SetCameraRot(0,
                            cinfo.trans.rot + ls2.x * 1.5f
                        );
                    }
                }

                /*{
                    Float2 rs = GetControllerMainRightStick();
                    SetCameraRot(cinfo.trans.rot + rs.x * 1.0f);
                }*/
            }

        }
        else
        {
            if (change)
            {
                g_debugcameraflag = false;
                SetCameraRot(0, 0.0f);
            }
            change = false;
        }
    }
#endif

	//テスト
	// 定期的に敵を出す
	//if (GetMainGameFrame() % 60 == 0)
	//{
	//	CreateGameObject(GetGameObjectPrefab(GAME_OBJECT_TYPE_TEST));
	//}
}

// ===================================================
// ゲームシーンの描画
// ===================================================
void DrawSceneGame(void)
{
    if (!g_sceneGame.g_gameSceneEnableFlag)
    {
        return;
    }

    if (g_sceneGame.g_StageManager != nullptr)
    { //�X�e�[�W�̕`��
        g_sceneGame.g_StageManager->Draw();
    }

	DrawUI_Game();

	{
        //DRAW_POLYGON_DAT drawDatTmp = {};
        //drawDatTmp.pos = MkF3(0.0f, 0.0f, 0.0f);

        //drawDatTmp.rot = 0.0f;
        ////drawDatTmp.rot3D = MkF3(0.0f, 0.0f + (float)GetMainGameFrame() * 2.2f, 0.0f);

        //drawDatTmp.color = MkF4(1.0f, 1.0f, 1.0f, 1.0f);

        ////drawDatTmp.texNo = LoadTexture("rom/texture/ui/in_game/charactor_icon.png");
        //drawDatTmp.texNo = 0;
        //drawDatTmp.loadTexType = LOADTEXTURETYPE_MAIN;

        //drawDatTmp.texUV = { 0.0f, 0.0f };
        //drawDatTmp.texWH = { 1.0f, 1.0f };
        //drawDatTmp.startPos3D = MkF3(0.0f, 2500.0f, -600.0f);
        //drawDatTmp.startRot3D = MkF3(0.0f, 0.0f, 0.0f);
        //drawDatTmp.startScale = MkF2(1.0f, 1.0f);
        //drawDatTmp.startScale3D = MkF3(1.0f, 1.0f, 1.0f);

        //drawDatTmp.size = MkF3(500.0f, 500.0f, 500.0f);
        //drawDatTmp.modelNo = GetModelDataIndex("MODEL/ball");
        ////drawDatTmp.size = MkF3(9.0f, -9.0f, 9.0f);
        ////drawDatTmp.modelNo = GetModelDataIndex("MODEL/boss_inperiusu/inperiusu");

        ////drawDatTmp.shaderOpt.outline = DPD_SHADER_OPT_OUTLINE_PURPLE;

        //DRAW_POLYGON_IN_CAMERA_EX_OPTION opt = {};
        //opt.reverse = false;
        //opt.fixed = false;
        //opt.enableShade = true;
        //opt.shaderType = DPD_SHADER_TYPE_EFFECT_FRESNEL;

        ////opt.priority = DRAW_POLYGON_PRIORITY_FORCE_ALWAYS;
        //opt.priority = DRAW_POLYGON_PRIORITY_NORMAL;
        ////opt.priority = DRAW_POLYGON_PRIORITY_FORCE_NORMAL;
        //DrawPolygonInCameraEx(drawDatTmp, opt);


		//DRAW_POLYGON_BUFFER dpb = {};
		//PolygonSetUp(&dpb, g_testModel);

		//PolygonShape(&dpb,
		//	MkF3(-200.0f, 0.0f, 20.0f),
		//	MkF3(0.0f, 0.0f, 0.0f),
		//	MkF3(40.0f, 40.0f, 40.0f)
		//);

		//POLYGON_RELEASE_OPTION opt = {};
		//opt.color = MkF4(1.0f, 1.0f, 1.0f, 1.0f);
		//opt.texNo = 0;
		//opt.texUV = { 0.0f, 0.0f };
		//opt.texWH = { 1.0f, 1.0f };
		//opt.loadTexType = LOADTEXTURETYPE_MAIN;
		//opt.fixed = false;
		//opt.priority = DRAW_POLYGON_PRIORITY_ALWAYS;

		//opt.enableShade = true;

		//PolygonRelease(&dpb, opt);


        //DRAW_POLYGON_DAT drawDatTmp = {};
        //drawDatTmp.pos = MkF3(0.0f, 0.0f, 0.0f);
        //drawDatTmp.size = MkF3(9.0f, -9.0f, 9.0f);

        //drawDatTmp.rot = 0.0f;
        ////drawDatTmp.rot3D = MkF3(0.0f, 0.0f + (float)GetMainGameFrame() * 2.2f, 0.0f);

        //drawDatTmp.color = MkF4(1.0f, 1.0f, 1.0f, 1.0f);

        ////drawDatTmp.texNo = LoadTexture("rom/texture/ui/in_game/charactor_icon.png");
        //drawDatTmp.texNo = 0;
        //drawDatTmp.loadTexType = LOADTEXTURETYPE_MAIN;

        //drawDatTmp.texUV = { 0.0f, 0.0f };
        //drawDatTmp.texWH = { 1.0f, 1.0f };
        //drawDatTmp.startPos3D = MkF3(0.0f, 2500.0f, -600.0f);
        //drawDatTmp.startRot3D = MkF3(80.0f, 0.0f, 0.0f);
        //drawDatTmp.startScale = MkF2(1.0f, 1.0f);
        //drawDatTmp.startScale3D = MkF3(1.0f, 1.0f, 1.0f);

        //drawDatTmp.modelNo = GetModelDataIndex("MODEL/boss_inperiusu/inperiusu");

        //drawDatTmp.shaderOpt.outline = DPD_SHADER_OPT_OUTLINE_PURPLE;

        //DRAW_POLYGON_IN_CAMERA_EX_OPTION opt = {};
        //opt.reverse = false;
        //opt.fixed = false;
        //opt.enableShade = true;
        //opt.shaderType = DPD_SHADER_TYPE_METAL_ENEMY;

        ////opt.priority = DRAW_POLYGON_PRIORITY_FORCE_ALWAYS;
        //opt.priority = DRAW_POLYGON_PRIORITY_NORMAL;
        ////opt.priority = DRAW_POLYGON_PRIORITY_FORCE_NORMAL;
        //DrawPolygonInCameraEx(drawDatTmp, opt);

        //CAMERA cInfo = GetCameraInfo(0);
        //
        //DRAW_POLYGON_DAT drawDatTmp = {};
        //drawDatTmp.pos = MkF3(
        //    0.0f, 
        //    0.0f, 
        //    0.0f
        //);
        //drawDatTmp.size = MkF3(SCREEN_WIDTH, SCREEN_HEIGHT, 1.0f);

        //drawDatTmp.rot = 0.0f;
        //drawDatTmp.rot3D = MkF3(0.0f, 0.0f, 0.0f);

        //drawDatTmp.color = MkF4(1.0f, 1.0f, 1.0f, 1.0f);

        //drawDatTmp.texNo = LoadTexture("rom/texture/effect/beam/sozai_02.png");
        //drawDatTmp.loadTexType = LOADTEXTURETYPE_MAIN;

        //drawDatTmp.texUV = { 0.0f, 0.0f };
        //drawDatTmp.texWH = { 1.0f, 10.0f };
        //drawDatTmp.startScale = MkF2(1.0f, 1.0f);
        //drawDatTmp.startScale3D = MkF3(1.0f, 1.0f, 1.0f);

        //drawDatTmp.modelNo = 0;

        //{
        //    static bool kp2 = false;
        //    static bool kp = false;
        //    if (GetAsyncKeyState('Q'))
        //    {
        //        if (!kp)
        //        {
        //            kp2 = !kp2;
        //            kp = true;
        //        }
        //    }
        //    else
        //    {
        //        kp = false;
        //    }

        //    drawDatTmp.shaderOpt.blendState = kp2 ? DPD_SHADER_OPT_BLEND_STATE_ADDCOLOR: DPD_SHADER_OPT_BLEND_STATE_DEFAULT;
        //}

        //DRAW_POLYGON_IN_CAMERA_EX_OPTION opt = {};
        //opt.reverse = false;
        //opt.fixed = false;
        //opt.enableShade = true;
        //opt.billboard = true;
        //opt.depthBuffIgnore = true;
        //opt.priority = DRAW_POLYGON_PRIORITY_FORCE_ALWAYS;
        ////opt.priority = DRAW_POLYGON_PRIORITY_FORCE_NORMAL;
        //DrawPolygonInCameraEx(drawDatTmp, opt);
	}

	//return;

	//{
	//	static unsigned int g_GameTime = 0;

	//	constexpr double e = 1.0 / 60.0;
	//	double sec = ((double)g_GameTime * e);

	//	unsigned int ms = (int)(sec * 100) % 100;
	//	unsigned int s = (int)(sec) % 60;
	//	unsigned int m = (int)(sec) / 60;

	//	char txt[256] = {};
	//	sprintf_s(txt, 256, "time: %02d:%02d.%02d", (int)(m), (int)(s), (int)(ms));

	//	DrawFont(std::string(txt), FONT_TYPE_DEFAULT,
	//		MkF2(SCREEN_WIDTH * -0.5f, SCREEN_HEIGHT * -0.3f),
	//		40,
	//		MkF4(0.9f, 0.9f, 0.9f, 0.75f),
	//		FONT_TEXT_ALIGNMENT_LEFT
	//	);

	//	g_GameTime++;
	//}


#if defined(_DEBUG) || defined(DEBUG)
    if (g_debugcameraflag)
    {
        return;
    }
#endif


    if (!IsFading())
    {
        if (g_sceneGame.g_StageManager != nullptr
            && g_sceneGame.g_StageManager->IsGameoverConfirm())
        {
            g_sceneGame.continueMenu.draw();
        }
        else
        {
            g_sceneGame.pauseMenu.draw();
        }
    }
}

// ===================================================
// ゲームシーンの終了処理
// ===================================================
void FinalizeSceneGame(void)
{
    if (g_sceneGame.g_StageManager != nullptr)
    { //�X�e�[�W�̏I��
        g_sceneGame.g_StageManager->Uninit();
        delete g_sceneGame.g_StageManager;
        g_sceneGame.g_StageManager = nullptr;
    }

    GE_GayaCancelRequestAll();

	UninitPlayer();

	Character::CharacterMainDataClearCache();

	//シェーダを消す
    //SetPostEffectShader("");
    SetPostEffectShaderVS("");
    SetPostEffectShaderPS("");

	//SoundBGMStopRequest();
	//既存のブロックとオブジェクトを消す
	DeleteBlockAll();
	DeleteGameObjectAll();

    UninitUI_Game();
    
    g_sceneGame.pauseMenu.uninit();
    g_sceneGame.continueMenu.uninit();

    g_sceneGame.g_gameSceneEnableFlag = false;

    SetMainGameDrawFPS(1);
	//(void)_heapmin();
}


Int2 GetStageCountFrameSceneGame(void)
{
    if (g_sceneGame.g_StageManager != nullptr)
    {
        if (g_sceneGame.g_StageManager->GetStage() != nullptr)
        {
            return {
                g_sceneGame.g_StageManager->GetStage()->GetCountFrame(),
                g_sceneGame.g_StageManager->GetStage()->GetCountFrameMax()
            };
        }
    }
    return { -1, -1 };
}

bool IsStageCountStopSceneGame(void)
{
    if (g_sceneGame.g_StageManager != nullptr)
    {
        if (g_sceneGame.g_StageManager->GetStage() != nullptr)
        {
            return g_sceneGame.g_StageManager->GetStage()->IsCountStop();
        }
    }
    return false;
}
void SetStageCountStopSceneGame(bool b)
{
    if (g_sceneGame.g_StageManager != nullptr)
    {
        if (g_sceneGame.g_StageManager->GetStage() != nullptr)
        {
            g_sceneGame.g_StageManager->GetStage()->SetCountStop(b);
        }
    }
}
Stage_Base* GetCurrentStageSceneGame()
{
    if (g_sceneGame.g_StageManager != nullptr)
    {
        return  g_sceneGame.g_StageManager->GetStage();
    }
    return nullptr;
}
