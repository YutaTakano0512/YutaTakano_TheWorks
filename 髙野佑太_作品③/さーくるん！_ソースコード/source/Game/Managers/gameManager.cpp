// ===================================================
// gameManager.cpp ?Q?[???S??????
// 
// hara sougo  2024/07/22
// ===================================================

#include "../../Core/Multi/winsock.h"
#include "../../Core/Multi/winsock_server.h"
#include "../../Core/Multi/winsock_client.h"


#define IS_GAME_MANAGER

#include "../../Core/DirectX/dx.h"
#include "../../Game/Systems/system.h"

#include "../../game_main.h"
#include "../Sprite/sprite.h"
#include "../Texture/texture.h"

#include "../Systems/movie.h"

#include "../Systems/fade.h"
#include "../Managers/scene.h"

#include "../../Core/Input/controller_main.h"

#include "../Systems/camera.h"

#include "../Game Object/gameObject.h"
#include "../Game Object/gameObjectGraph.h"
#include "../Game Object/gameObjectPrefab.h"

#include "../Entities/Block/block.h"
#include "../Entities/Block/blockPrefab.h"
#include "../Game Map/gameMap.h"

#include "../Entities/character.h"

#include "gameManager.h"
#include "soundManager.h"

#include "gameEvent.h"

#include "bg.h"
#include "../Systems/ui.h"
#include "../Entities/player.h"

#include "../../Core/Multi/winsock.h"
#include "../../Core/Multi/winsock_server.h"
#include "../../Core/Multi/winsock_client.h"

// ===================================================
// ?\?????
// ===================================================
struct GAME_MANAGER_G_V {
    unsigned int gameActiveFrame;

	int gameStopFrame;

	int gameSlowFrame;
	int gameSlowRate;

	bool gamePause;

    int movieSkipFlagFrame;
    int movieSkipFlagStartFrame;
    int movieSkipFlagPushFrame;
    bool movieSkipTrigger;
    bool movieSkipEnable;
    bool movieFlag;
    bool moviePlayerHiddenFlag;
    bool playerCtrlStop;

	SCENE setSceneRequest;
	int setSceneRequestFrame;
	GAME_MANAGER_SAVE_RESULT saveResult;
    int scoreAddBuf; //ﾄﾉｲﾃ､ｷ､ｿ･ｹ･ｳ･｢､ﾎﾃﾍ､・ﾅｪ､ﾋﾊﾝﾂｸ


    int enemyDefeatNumber;
    unsigned int enemyDefeatLastFrame;
    float enemyDefeatSEPitch;
    bool enemyEnemyManyDefeatFlag;
};

// ===================================================
// ?O???[?o?????
// ===================================================
static GAME_MANAGER_G_V g_gameManager = {};

static SOUND_ID g_enemdefeatse[4];
// ===================================================
// ??????
// ===================================================
void InitGameManager(void)
{
	g_gameManager = {};
	g_gameManager.setSceneRequest = SCENE_INIT;

    g_enemdefeatse[0] = GetSoundData("SOUND/SE/player/attack/extinction");
    g_enemdefeatse[1] = GetSoundData("SOUND/SE/player/attack/extinction2");
    g_enemdefeatse[2] = GetSoundData("SOUND/SE/player/attack/extinction3");
    g_enemdefeatse[3] = GetSoundData("SOUND/SE/player/attack/extinction4");

	//?T?E???h?}?l?[?W???[???????
	InitSoundManager();

    InitMovie();

	//UI??????
	InitUI();

	//?J??????????
	InitCamera();

	//?Q?[???I?u?W?F?N?g???????
	InitGameObject();
	InitGameObjectGraph();
	InitGameObjectPrefab();

	Character::Init();

	//?w?i???????
	InitBg();

	InitBlock();
	InitBlockPrefab();
	InitGameMap();

	InitFade();

	//?V?[?????????
	InitScene();

	//?Q?[????C?x???g????
	InitGameEvent();


	// ?N??????V?[?????Z?b?g
	//SetScene(SCENE_TITLE);
    //SetSceneFade(SCENE_TITLE);
    SetSceneFade(SCENE_OP);

	//?^?C?g???p??BGM???娼｢?
	//SoundBGMPlayRequest(SOUND_TYPE_BGM_1, (60 * 60 * 1) + (60 * 42), 0.5);

	//PlayBGM(STRM_NIGHT_DRIVE);
	//PlayBGM(STRM_CYLINDER);
}

// ===================================================
// ?X?V
// ===================================================
void UpdateGameManager(void)
{
#if defined(_DEBUG) || defined(DEBUG)
	//?f?o?b?O 3D ?I???I?t

    if (GetScene() == SCENE_EDITOR)
    {
        static bool kp = false;
        if (GetAsyncKeyState(VK_INSERT))
        {
            if (!kp)
            {
                DXSetDepthStencilStateDepth(!DXDepthStencilStateDepthEnable());
                kp = true;
            }
        }
        else
        {
            kp = false;
        }
    }

    if ((Keyboard_IsKeyDown(KK_LEFTCONTROL) || Keyboard_IsKeyDown(KK_RIGHTCONTROL))
        && Keyboard_IsKeyDown(KK_R)
        && Keyboard_IsKeyTrigger(KK_S)
    )
    { // デバッグシェーダーリロード

        DX_ShaderReCompile();
    }

    //
    //{
    //    static bool isServer = false;
    //    //static bool isClient = false;
    //    if (!isServer/* && !isClient*/)
    //    {
    //        if ((Keyboard_IsKeyDown(KK_LEFTSHIFT) || Keyboard_IsKeyDown(KK_RIGHTSHIFT))
    //            && Keyboard_IsKeyDown(KK_S)
    //            && Keyboard_IsKeyDown(KK_A)
    //            && GetScene() == SCENE_TITLE
    //            )
    //        {  //?T?[?o?[??
    //            isServer = true;
    //            WinSockServer_Start();
    //        }
    //        //else if (GetAsyncKeyState(VK_SHIFT)
    //        //    && Keyboard_IsKeyDown(KK_C)
    //        //    && Keyboard_IsKeyDown(KK_A))
    //        //{  //?N???C?A???g??
    //        //    isClient = true;

    //        //    SetScene(SCENE_MULTICLIENT);
    //        //}
    //    }
    //}
#endif

    g_gameManager.movieSkipTrigger = false;
    if (g_gameManager.movieSkipFlagFrame > 0)
    {
        g_gameManager.movieSkipFlagFrame--;
    }
    if (g_gameManager.movieSkipFlagStartFrame > 0)
    {
        g_gameManager.movieSkipFlagStartFrame--;
    }
    if (g_gameManager.movieSkipFlagPushFrame > 0)
    {
        g_gameManager.movieSkipFlagPushFrame--;
    }


    //{

    //    if (WinSockGetType() == WINSOCK_CONNECT_TYPE::OFFLINE)
    //    {
    //        //ｻｪ
    //        if ((Keyboard_IsKeyDown(KK_LEFTSHIFT) || Keyboard_IsKeyDown(KK_RIGHTSHIFT))
    //            && Keyboard_IsKeyDown(KK_S)
    //            && Keyboard_IsKeyDown(KK_A)
    //            && GetScene() == SCENE_TITLE
    //            )
    //        {  //?T?[?o?[??
    //            WinSockServer_Start();
    //        }

    //        //ﾂ｢
    //        if ((Keyboard_IsKeyDown(KK_LEFTSHIFT) || Keyboard_IsKeyDown(KK_RIGHTSHIFT))
    //            && Keyboard_IsKeyDown(KK_C)
    //            && Keyboard_IsKeyDown(KK_A)
    //            && GetScene() == SCENE_TITLE
    //            )
    //        { // ･ﾇ･ﾐ･ﾃ･ｰ･ｷ｡ｼ･ｨ･ﾇ･｣･ｿ｡ｼ
    //            //･ｯ･鬣､･｢･ﾈ､ﾎ･ｹ･ｿ｡ｼ･ﾈ
    //            WinSockClient_Start();

    //        }
    //    }
    //}

	if (!g_gameManager.gamePause)
	{
		UpdateFade();
	}

	if (!FadeGetGameStop())
	{
		if (!g_gameManager.gamePause)
		{
            if (g_gameManager.movieFlag)
            {
                if (g_gameManager.movieSkipEnable)
                {
                    if (g_gameManager.movieSkipFlagFrame > 0)
                    {
                        if (GetControllerMainTrigger(CONTROLLER_BUTTON_TYPE_PLUS))
                        {
                            g_gameManager.movieSkipTrigger = true;

                            g_gameManager.movieSkipFlagFrame = 0;
                            g_gameManager.movieSkipFlagPushFrame = MOVIE_SKIP_FLAG_PUSH_FRAME;
                        }
                    }
                    else
                    {
                        bool pushtmp = false;
                        for (int i = 0; i < CONTROLLER_BUTTON_TYPE_MAX; i++)
                        {
                            pushtmp = pushtmp
                                || GetControllerMainTrigger((CONTROLLER_BUTTON_TYPE)i);
                        }
                        if (pushtmp)
                        {
                            g_gameManager.movieSkipFlagFrame = MOVIE_SKIP_FLAG_FRAME;
                            g_gameManager.movieSkipFlagStartFrame = MOVIE_SKIP_FLAG_START_FRAME;
                        }
                    }
                }
            }

			UpdateCamera();


			if (g_gameManager.gameStopFrame == -1)
			{
			}
			else if (g_gameManager.gameStopFrame > 0)
			{
				g_gameManager.gameStopFrame--;
			}
			else if (g_gameManager.gameSlowFrame > 0
				&& GetMainGameFrame() % g_gameManager.gameSlowRate != 0 )
			{
			}
			else
			{
				if (g_gameManager.setSceneRequest != SCENE_INIT
					&& g_gameManager.setSceneRequestFrame <=0)
				{
					SetSceneFade(g_gameManager.setSceneRequest,
                        FADE_TYPE_STAR_WIPE
                    );
					g_gameManager.gameStopFrame = -1;
					g_gameManager.setSceneRequest = SCENE_INIT;
				}
				else
				{
					if (g_gameManager.setSceneRequestFrame > 0)
					{
						g_gameManager.setSceneRequestFrame--;
					}

					UpdateGameMap();
					UpdateBlock();

					UpdateGameObject();

					UpdateBg();
				}


                { //たくさん倒したボ那須
                    if (abs((int)g_gameManager.gameActiveFrame - (int)g_gameManager.enemyDefeatLastFrame)
                         > FRAME_ONE_SECOND
                    )
                    {
                        GameManagerResultAddScore(
                            //(int)((0.3f * pow(g_gameManager.enemyDefeatNumber, 1.5f)) * 100),
                            (int)((0.3f * pow(g_gameManager.enemyDefeatNumber, 1.4f)) * 100),
                            RESULT_SCORE_EX_TYPE_MANY_ENEMY_DEFEAT
                        );

                        if (g_gameManager.enemyDefeatNumber >= 10
                            && !g_gameManager.enemyEnemyManyDefeatFlag)
                        {
                            g_gameManager.enemyEnemyManyDefeatFlag = true;

                            GE_GayaStartRequest(
                                "game_defeat_large_number_of_enemies",
                                FRAME_ONE_SECOND * 0.0f
                            );
                        }

                        g_gameManager.enemyDefeatSEPitch = 1.0f;

                        g_gameManager.enemyDefeatNumber = 0;
                    }
                }


				if (g_gameManager.gameSlowFrame > 0)
				{
					g_gameManager.gameSlowFrame--;
				}

                g_gameManager.gameActiveFrame++;
			}
		}

        UpdateGameEvent();

		UpdateScene();
	}

    //･爍ｼ･ﾓ｡ｼｹｹｿｷ
    UpdateMovie();

	UpdateUI();

	UpdateSoundManager();

    if (WinSockServerIsEnable())
    {
        WinSockServer_Update();
    }
    if (WinSockClientIsEnable())
    {
        WinSockClient_Update();
    }
}

// ===================================================
// ?`??
// ===================================================
void DrawGameManager(bool enable)
{
    if (
        WinSockClientIsEnable()
        && GetScene() == SCENE_MULTICLIENT
    )
    {
        WinSockClient_Draw();

        DrawGameEvent();

        DrawScene();
        DrawFade();

        if (enable)
        {
            //?J???????`?????
            DrawCameraBufferRelease();
            //DrawCameraBufferDrawOnly();
        }
    }
    else
    {
        DrawBg();

        DrawBlock();

        DrawGameObject();

        DrawCamera();

        DrawGameEvent();

        DrawScene();
        DrawFade();

        DrawUI();

        //･爍ｼ･ﾓ｡ｼﾉﾁｲ・
        DrawMovie();

        if (enable)
        {
            //ｻｪ
            if (WinSockGetType() == WINSOCK_CONNECT_TYPE::SERVER)
            {
                WinSockServer_Draw();
            }

            //?J???????`?????
            DrawCameraBufferRelease();
        }
    }
}

// ===================================================
// ?I??????
// ===================================================
void UninitGameManager(void)
{
	UninitGameEvent();


	UninitGameMap();
	UninitBlockPrefab();
	UninitBlock();

	Character::Uninit();

	UninitGameObjectPrefab();
	UninitGameObjectGraph();
	UninitGameObject();

	UninitScene();
	UninitFade();

	UninitBg();

	UninitCamera();

	UninitUI();

    UninitMovie();

	UninitSoundManager();
}

void GameStopRequest(int frame)
{
	if (frame > 0)
	{
		g_gameManager.gameStopFrame = frame;
	}
}
void GameStopResetRequest()
{
	g_gameManager.gameStopFrame = 0;
}
void GamePause(bool enable)
{
	g_gameManager.gamePause = enable;
}
void GameSlowRequest(int frame, int rate)
{
	if (frame > 0)
	{
		g_gameManager.gameSlowFrame = frame;
		g_gameManager.gameSlowRate = rate;
	}
}

bool IsGamePause()
{
	return g_gameManager.gamePause;
}

bool IsGameStop()
{
	return g_gameManager.gameStopFrame > 0;
}

void GameManagerSetSceneRequest(int sce, int delay, bool isClear)
{
	g_gameManager.setSceneRequestFrame = delay;
	g_gameManager.setSceneRequest = (SCENE)sce;

    if (delay <= 0)
    {
        SetSceneFade(g_gameManager.setSceneRequest,
            FADE_TYPE_STAR_WIPE
        );
        g_gameManager.gameStopFrame = -1;
        g_gameManager.setSceneRequest = SCENE_INIT;
    }

	g_gameManager.saveResult.isClear = isClear;
}
void GameManagerSetSceneRequestCancel()
{
    g_gameManager.setSceneRequestFrame = 0;
    g_gameManager.setSceneRequest = SCENE_INIT;
}
GAME_MANAGER_SAVE_RESULT GameManagerGetResult()
{
    return g_gameManager.saveResult;
}
void GameManagerResetResult()
{
    g_gameManager.saveResult = {};
}
void GameManagerResetResultScore()
{
    g_gameManager.saveResult.score = 0;
    for (int i = 0; i < RESULT_SCORE_EX_TYPE_MAX; i++)
    {
        g_gameManager.saveResult.scoreEx[i] = 0;
    }
}

void GameManagerResultAddScore(int addscore, RESULT_SCORE_EX_TYPE type)
{
    int sc = g_gameManager.saveResult.score;
    int scEx = g_gameManager.saveResult.scoreEx[type];

    sc += addscore;
    scEx += addscore;

    if (sc < 0)
    {
        sc = 0;
    }
    if (sc > 99999)
    {
        sc = 99999;
    }

    if (scEx < 0)
    {
        scEx = 0;
    }
    if (scEx > 99999)
    {
        scEx = 99999;
    }

    g_gameManager.scoreAddBuf += addscore;

    g_gameManager.saveResult.score = sc;
    g_gameManager.saveResult.scoreEx[type] = scEx;
}

int GameManagerGetResultAddScoreBuf()
{
    return g_gameManager.scoreAddBuf;
}
void GameManagerResetResultAddScoreBuf()
{
    g_gameManager.scoreAddBuf = 0;
}

void GameManagerSetResultClearTime(int time)
{
    g_gameManager.saveResult.clearTimeFrame = time;
}

void GameManagerSetResultRank(int rank)
{
    g_gameManager.saveResult.rank = rank;
}

void GameManagerResetResultAddScoreEvent()
{
    g_gameManager.enemyDefeatNumber = 0;
}

void GameManagerResetSetDiff(Difficulty diff)
{
    g_gameManager.saveResult.difficulty = diff;
}

void GameManagerSetPlayerNum(int num)
{
    g_gameManager.saveResult.playersNum = num;
}

void GameManagerSetPlayer(int index, PLAYER_CHARACTER_TYPE type)
{
    g_gameManager.saveResult.players[index] = type;
}


bool GameManagerIsPlayerCtrlStop(void)
{
    return g_gameManager.playerCtrlStop;
}
void GameManagerSetPlayerCtrlStop(bool b)
{
    g_gameManager.playerCtrlStop = b;
}

bool GameManagerIsMovieFlag(void)
{
    return g_gameManager.movieFlag;
}
void GameManagerSetMovieFlag(bool b, bool skip)
{
    g_gameManager.movieFlag = b;
    g_gameManager.movieSkipEnable = skip;
    if (g_gameManager.movieSkipEnable)
    {
        ResetActionPlayers();
    }
    g_gameManager.moviePlayerHiddenFlag = false;
}

bool GameManagerIsMoviePlayerHiddenFlag(void)
{
    return g_gameManager.moviePlayerHiddenFlag;
}

void GameManagerSetMoviePlayerHiddenFlag(bool b)
{
    g_gameManager.moviePlayerHiddenFlag = b;
}

int GameManagerGetMovieSkipFlagPushFrame(void)
{
    return g_gameManager.movieSkipFlagPushFrame;
}

int GameManagerGetMovieSkipFlagStartFrame(void)
{
    return g_gameManager.movieSkipFlagStartFrame;
}

int GameManagerGetMovieSkipFlagFrame(void)
{
    return g_gameManager.movieSkipFlagFrame;
}
bool GameManagerIsMovieSkipTrigger(void)
{
    return g_gameManager.movieSkipTrigger;
}

void GameManagerAddEnemyDefeatCall()
{
    int num = g_gameManager.enemyDefeatNumber % 4;

    SoundEffectPlayRequest(
        g_enemdefeatse[num],
        2.0f, 0.0f, g_gameManager.enemyDefeatSEPitch);

    g_gameManager.enemyDefeatSEPitch += 0.122f;
    g_gameManager.enemyDefeatSEPitch = std::min(g_gameManager.enemyDefeatSEPitch, 2.0f);

    g_gameManager.enemyDefeatLastFrame = g_gameManager.gameActiveFrame;

    g_gameManager.enemyDefeatNumber++;
}
