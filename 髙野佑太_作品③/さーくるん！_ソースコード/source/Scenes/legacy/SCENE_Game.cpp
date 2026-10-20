// ===================================================
// SCENE_Game.cpp ゲームシーン制御
// 
// hara sougo  2024/07/22
// ===================================================
#include "../game_main.h"

#include "SCENE_Title.h"
#include "SCENE_Game.h"

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

#include "../Game/Game Stage/Stage_Tutorial.h"
#include "../Game/Game Stage/Stage_Ruins.h"

#include "../Game/polygon/modelData.h"

#include "../Core/Multi/winsock.h"
#include "../Core/Multi/winsock_server.h"
#include "../Core/Multi/winsock_client.h"

// ===================================================
// 構造体宣言
// ===================================================
static constexpr int STAGE_MAIN_WAIT_END_FADE = FRAME_ONE_SECOND * 1;
struct SCENE_GAME_G_V {
	bool gamePause;


    Stage_Base* g_Stage = nullptr;

    BOOL g_Stage_Main_Wait_Flag = false; //待機中
    BOOL g_Stage_Main_Wait_EndFlag = false; //待機中終了フラグ
    int g_Stage_Main_Wait_EndFadeFlag = -1; //待機中終了フラグ

    //static DX_MODEL* g_testModel = nullptr;
    //static unsigned int g_testModel = 0;

    float g_pauseBeforeBgmVolume = 0;

    BOOL g_gameSceneEnableFlag = false;
};


//ゲームシーンのデータとか
static SCENE_GAME_G_V g_sceneGame = {};

//ポーズの画像
static TEX_DAT g_tex_pause_text = {};
static TEX_DAT g_tex_pause_retire_button = {};


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

    if (WinSockGetType() == WINSOCK_CONNECT_TYPE::CLIENT)
    {
        SetScene(SCENE_MULTICLIENT);
        return;
    }

    {
        { //読み込みとか変数の初期化

            //g_testModel = DX_ModelLoad("rom/model/hashira1.fbx", false);
            //g_testModel = GetModelDataIndex("MODEL/cube");

            g_tex_pause_text = GetTexData(TEX_TYPE_UI_PAUSE_TEXT);
        }

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

        { //まず待機ステージへ移動{
            g_sceneGame.g_Stage_Main_Wait_Flag = true;
            g_sceneGame.g_Stage_Main_Wait_EndFlag = false;
            g_sceneGame.g_Stage_Main_Wait_EndFadeFlag = -2;

            g_sceneGame.g_Stage = new Stage_Tutorial(GetDifficultySceneTitle());
            g_sceneGame.g_Stage->Init();
        }

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
	if (GetAsyncKeyState(VK_DELETE))
	{
        //マルチ終了処理
        if (WinSockGetType() == WINSOCK_CONNECT_TYPE::SERVER)
        {
            WinSockServerPlayerDatClear();
        }
		SetSceneFade(SCENE_TITLE);
	}

#if defined(_DEBUG) || defined(DEBUG)
    if (GetAsyncKeyState(VK_RCONTROL))
    {
        SetSceneFade(SCENE_RESULT);
    }
#endif

#endif

    if (!IsGamePause())
    { //ポーズ中ではない
        if (g_sceneGame.g_Stage != nullptr)
        { //ステージの更新
            g_sceneGame.g_Stage->Update();
        }
    }

    //UIの更新
	UpdateUI_Game();

    //プレイヤーの更新
	UpdatePlayer();


    //DebugPrintf("%d\n", g_Stage_Main_Wait ? 1: 0);

    //待機所の処理
    if (g_sceneGame.g_Stage_Main_Wait_Flag)
    {
        if (g_sceneGame.g_Stage_Main_Wait_EndFadeFlag >= 0)
        {
            g_sceneGame.g_Stage_Main_Wait_EndFadeFlag++;

            if (g_sceneGame.g_Stage_Main_Wait_EndFadeFlag
                == STAGE_MAIN_WAIT_END_FADE / 2)
            {
                //プレイヤー以外を削除する
                for (int i = 0; i < GAME_OBJECT_MAX; i++)
                {
                    GAME_OBJECT* ob = GetGameObject(i);
                    if (IsActiveGameObject(ob)
                        && (ob->tags.size() <= 0 || !ob->tags[0].find("PLAYER"))
                        )
                    {

                        DeleteGameObject(ob);
                    }
                }

                //元々あったステージを削除
                if (g_sceneGame.g_Stage != nullptr)
                { //�X�e�[�W�̏I��
                    g_sceneGame.g_Stage->Uninit();
                    delete g_sceneGame.g_Stage;
                    g_sceneGame.g_Stage = nullptr;
                }

                Stage_Base* stgbuff = new Stage_Ruins(GetDifficultySceneTitle());
                g_sceneGame.g_Stage = stgbuff;
                g_sceneGame.g_Stage->Init();

                GameManagerResetResult();
                GameManagerResetResultAddScoreBuf();


                //プレイヤーを定位置に移動する
                for (int i = 0; i < PLAYER_MAX; i++)
                {
                    PLAYER* player = GetPlayer(i);
                    GAME_OBJECT* playerObj = player->obj;
                    if (playerObj != nullptr)
                    {
                        playerObj->status.hp = playerObj->status.hpMax;
                        playerObj->pos = { 0.0f, BLOCK_HIGHT * 150.0f };
                    }
                }
            }

            //フラグを折る
            if (g_sceneGame.g_Stage_Main_Wait_EndFadeFlag > STAGE_MAIN_WAIT_END_FADE)
            {
                g_sceneGame.g_Stage_Main_Wait_Flag = FALSE;

                g_sceneGame.g_Stage_Main_Wait_EndFadeFlag = -2;
            }

            DebugPrintf("%d\n", g_sceneGame.g_Stage_Main_Wait_EndFadeFlag);

            //return;
        }

        else
        {
            //戻る
            if (
                GetControllerMainTrigger(CONTROLLER_BUTTON_TYPE_L)
                )
            {
                g_sceneGame.g_Stage_Main_Wait_EndFlag = true;

                SetSceneFade(SCENE_CHARACTERSELECT);
            }
            else if (
                GetControllerMainTrigger(CONTROLLER_BUTTON_TYPE_R)
                )
            {
                //進
                g_sceneGame.g_Stage_Main_Wait_EndFlag = true;

                g_sceneGame.g_Stage_Main_Wait_EndFadeFlag = 0;
            }
        }
    }

    if (!g_sceneGame.g_Stage_Main_Wait_Flag)
    {
        //ポーズの処理
        { //ポーズ
            if (g_sceneGame.gamePause)
            {
                if (GetControllerMainTrigger(CONTROLLER_BUTTON_TYPE_PLUS)
                    || GetControllerMainTrigger(CONTROLLER_BUTTON_TYPE_MINUS)
                    || GetControllerMainTrigger(CONTROLLER_BUTTON_TYPE_B)
                    )
                {
                    g_sceneGame.gamePause = false;
                    GamePause(g_sceneGame.gamePause);

                    //ポーズ解除時にBGMの音量を戻す
                    SoundBGMSetVolumeRequest(g_sceneGame.g_pauseBeforeBgmVolume, 0);

                    //SoundEffectPlayRequest(SOUND_TYPE_PAUSE_CANCEL, 0.4f, 0);
                }
            }
            else if (!IsFading()) //�|�[�Y
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

                        //SoundEffectPlayRequest(SOUND_TYPE_PAUSE, 0.4f, 0);
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

    {
        static bool change = false;
        if (g_sceneGame.gamePause)
        {
            static int modMouseMidScroll = 0;

            constexpr int S_EDIT_CAMERA_ZOOM_SPEED = 0.3f;
            constexpr int clickokframe = 5;
            static MouseState_tag l_GameMouseState = {};
            Mouse_GetState(&l_GameMouseState);
            //DebugPrintf("%d", l_GameMouseState.scrollWheelValue);

            CAMERA cinfo = GetCameraInfo(0);

            {
                {
                    static bool crekeypush = false;
                    if (l_GameMouseState.middleButton
                        && !crekeypush)
                    {
                        //if (isForegroundWindowFrame > clickokframe)
                        {
                            modMouseMidScroll = l_GameMouseState.scrollWheelValue;
                        }

                        crekeypush = true;
                    }
                    else
                    {
                        crekeypush = false;
                    }
                }

                {
                    //Float2 ls = GetControllerMainLeftStick();
                    Float2 ls = {};
                    if (GetControllerMainPress(CONTROLLER_BUTTON_TYPE_LEFT, 0))
                    {
                        ls.x += -1.0f;
                        change = true;
                    }
                    if (GetControllerMainPress(CONTROLLER_BUTTON_TYPE_RIGHT, 0))
                    {
                        ls.x += 1.0f;
                        change = true;
                    }
                    if (GetControllerMainPress(CONTROLLER_BUTTON_TYPE_UP, 0))
                    {
                        ls.y += -1.0f;
                        change = true;
                    }
                    if (GetControllerMainPress(CONTROLLER_BUTTON_TYPE_DOWN, 0))
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

                    if (change)
                    {
                        g_debugcameraflag = true;
                        SetCameraZoom(0, (float)(1.0f + (float)(l_GameMouseState.scrollWheelValue - modMouseMidScroll) / 1000 * S_EDIT_CAMERA_ZOOM_SPEED));

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

    if (g_sceneGame.g_Stage != nullptr)
    { //�X�e�[�W�̕`��
        g_sceneGame.g_Stage->Draw();
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
        //opt.shaderType = DX_SHADER_TYPE_EFFECT_FRESNEL;

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
        //opt.shaderType = DX_SHADER_TYPE_METAL_ENEMY;

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

    if (g_sceneGame.g_Stage_Main_Wait_EndFadeFlag >= 0)
    {
        {
            DRAW_POLYGON_DAT dsdat = {};
            dsdat.pos.x = -1.0f;
            dsdat.pos.y = -1.0f;
            dsdat.pos.z = 0.0f;

            dsdat.size.x = SCREEN_WIDTH + 2.0f;
            dsdat.size.y = SCREEN_HEIGHT + 2.0f;
            dsdat.size.z = 1.0f;

            dsdat.rot = 0.0f;

            dsdat.color = MkF4(-10.0f, -10.0f, -10.0f, 2.0f);
            dsdat.color.w *= 1.0f - (cos(2.0f * SYS_GENERAL_PI
                * ((float)g_sceneGame.g_Stage_Main_Wait_EndFadeFlag / STAGE_MAIN_WAIT_END_FADE)) 
                / 2.0f + 0.5f);

            dsdat.texNo = 0;
            dsdat.loadTexType = LOADTEXTURETYPE_MAIN;

            dsdat.texUV = {
                0.0f,
                0.0f
            };
            dsdat.texWH = { 1.0f, 1.0f };

            dsdat.startPos = {
                0.0f,
                0.0f
            };
            dsdat.startRot = 0;
            dsdat.startScale = { 1.0f, 1.0f };
            dsdat.startScale3D = { 1.0f, 1.0f, 1.0f };

            dsdat.shaderOpt.bloom = DPD_SHADER_OPT_BLOOM_NONE;

            DRAW_POLYGON_IN_CAMERA_EX_OPTION opt = {};
            opt.reverse = false;
            opt.fixed = true;
            opt.depthBuffIgnore = true;
            opt.priority = DRAW_POLYGON_PRIORITY_FORCE_ALWAYS;

            DrawPolygonInCameraEx(dsdat, opt);
        }
    }

	if (IsGamePause())
	{ //ポーズの表示

        {
            DRAW_POLYGON_DAT dsdat = {};
            dsdat.pos.x = -1.0f;
            dsdat.pos.y = -1.0f;
            dsdat.pos.z = 0.0f;

            dsdat.size.x = SCREEN_WIDTH + 2.0f;
            dsdat.size.y = SCREEN_HEIGHT + 2.0f;
            dsdat.size.z = 1.0f;

            dsdat.rot = 0.0f;

            dsdat.color = MkF4(-10.0f, -10.0f, -10.0f, 0.3f);

            dsdat.texNo = 0;
            dsdat.loadTexType = LOADTEXTURETYPE_MAIN;

            dsdat.texUV = {
                0.0f,
                0.0f
            };
            dsdat.texWH = { 1.0f, 1.0f };

            dsdat.startPos = {
                0.0f,
                0.0f
            };
            dsdat.startRot = 0;
            dsdat.startScale = { 1.0f, 1.0f };
            dsdat.startScale3D = { 1.0f, 1.0f, 1.0f };

            dsdat.shaderOpt.bloom = DPD_SHADER_OPT_BLOOM_NONE;

            DRAW_POLYGON_IN_CAMERA_EX_OPTION opt = {};
            opt.reverse = false;
            opt.fixed = true;
            opt.depthBuffIgnore = true;
            opt.priority = DRAW_POLYGON_PRIORITY_FORCE_ALWAYS;

            DrawPolygonInCameraEx(dsdat, opt);
            
            dsdat.pos = MkF3(
                (float)(SCREEN_WIDTH * 0.4f),
                (float)(SCREEN_HEIGHT * 0.4f + sin((float)GetMainGameFrame() / 60) * 20.0f),
                0.0f
            );
            dsdat.size = MkF3(
                400.0f,
                400.0f,
                1.0f
            );
            dsdat.texNo = g_tex_pause_text.textureId;
            dsdat.color = MkF4(1.0f, 1.0f, 1.0f, 1.0f);
            DrawPolygonInCameraEx(dsdat, opt);
        }

		//{
		//	Float4 color = MkF4(-10.0f, -10.0f, -10.0f, 0.3f);
		//	DRAW_SPRITE_BUFFER dsb = {};
		//	SpriteSetUp(&dsb);
		//	SpriteShape(&dsb,
		//		MkF2(-1.0f, -1.0f),
		//		0.0f,
		//		MkF2((float)SCREEN_WIDTH + 2.0f, (float)SCREEN_HEIGHT + 2.0f)
		//	);
		//	SpriteRelease(&dsb,
		//		color,
		//		0,
		//		MkF2(0.0f, 0.0f),
		//		MkF2(1.0f, 1.0f),
		//		LOADTEXTURETYPE_MAIN,
		//		true,
		//		DRAW_POLYGON_PRIORITY_FORCE_ALWAYS
		//	);
		//}
		//{
		//	Float4 color = MkF4(1.0f, 1.0f, 1.0f, 1.0f);
		//	DRAW_SPRITE_BUFFER dsb = {};
		//	SpriteSetUp(&dsb);
		//	SpriteShape(&dsb,
		//		MkF2((float)(SCREEN_WIDTH * 0.4f), (float)(SCREEN_HEIGHT * 0.4f + sin((float)GetMainGameFrame() / 60) * 20.0f)),
		//		0.0f,
		//		MkF2(400.0f, 400.0f)
		//	);
		//	SpriteRelease(&dsb,
		//		color,
		//		g_tex_pause_text.textureId,
		//		MkF2(0.0f, 0.0f),
		//		MkF2(1.0f, 1.0f),
		//		LOADTEXTURETYPE_MAIN,
		//		true,
		//		DRAW_POLYGON_PRIORITY_FORCE_ALWAYS
		//	);
		//}
        //{
        //    Float4 color = MkF4(1.0f, 1.0f, 1.0f, 1.0f);
        //    DRAW_SPRITE_BUFFER dsb = {};
        //    SpriteSetUp(&dsb);
        //    SpriteShape(&dsb,
        //        MkF2(0.0f, 0.0f),
        //        0.0f,
        //        MkF2(500.0f, 200.0f)
        //    );
        //    SpriteRelease(&dsb,
        //        color,
        //        g_tex_pause_retire_button.textureId,
        //        MkF2(0.0f, 0.0f),
        //        MkF2(1.0f, 1.0f),
        //        LOADTEXTURETYPE_MAIN,
        //        true,
        //        DRAW_POLYGON_PRIORITY_FORCE_ALWAYS
        //    );
        //}
	}
	else
	{
		if (GetScene() == SCENE_GAME)
		{
			//Float4 color = MkF4(1.0f, 1.0f, 1.0f, 0.75f);
			//DRAW_SPRITE_BUFFER dsb = {};
			//SpriteSetUp(&dsb);
			//SpriteShape(&dsb,
			//	MkF2((float)(SCREEN_WIDTH * 0.37f), (float)(SCREEN_HEIGHT * 0.39f)),
			//	0.0f,
			//	MkF2(600.0f, 600.0f)
			//);
			//SpriteRelease(&dsb,
			//	color,
			//	g_tex_pause_gui.textureId,
			//	MkF2(0.0f, 0.0f),
			//	MkF2(1.0f, 1.0f),
			//	LOADTEXTURETYPE_MAIN
			//);
		}
	}
}

// ===================================================
// ゲームシーンの終了処理
// ===================================================
void FinalizeSceneGame(void)
{
    if (g_sceneGame.g_Stage != nullptr)
    { //�X�e�[�W�̏I��
        g_sceneGame.g_Stage->Uninit();
        delete g_sceneGame.g_Stage;
        g_sceneGame.g_Stage = nullptr;
    }

	UninitPlayer();

	Character::CharacterMainDataClearCache();

	//シェーダを消す
	SetPostEffectShader("");

	//既存のブロックとオブジェクトとBGMを消す
	SoundBGMStopRequest();
	DeleteBlockAll();
	DeleteGameObjectAll();

    UninitUI_Game();

    g_sceneGame.g_gameSceneEnableFlag = false;

	//(void)_heapmin();
}


int GetStageCountFrameSceneGame(void)
{
    if (g_sceneGame.g_Stage != nullptr)
    {
        return g_sceneGame.g_Stage->GetCountFrame();
    }
    return -1;
}

bool IsStageCountStopSceneGame(void)
{
    if (g_sceneGame.g_Stage != nullptr)
    {
        return g_sceneGame.g_Stage->IsCountStop();
    }
    return false;
}
void SetStageCountStopSceneGame(bool b)
{
    if (g_sceneGame.g_Stage != nullptr)
    {
        g_sceneGame.g_Stage->SetCountStop(b);
    }
}
Stage_Base* GetCurrentStageSceneGame()
{
    return g_sceneGame.g_Stage;
}
