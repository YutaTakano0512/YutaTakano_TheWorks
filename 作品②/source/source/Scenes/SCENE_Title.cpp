// ===================================================
// SCENE_Title.cpp タイトルシーン制御
// yahiro riki
// hara sougo  2024/07/22
// ===================================================
#include "../Game/Systems/sound.h"
#include "../main.h"
#include "../game_main.h"

#include "SCENE_Title.h"

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

#include "../Game/Systems/ui.h"
#include "../Game/Systems/movie.h"

#include "../Core/Multi/winsock.h"
#include "../Core/Multi/winsock_server.h"
#include "../Core/Multi/winsock_client.h"

static TEX_DAT g_tex_title = {};
static TEX_DAT g_tex_title_start = {};

static float g_in = 0;
static SPRITE_GRAPH_DAT g_titlePlayerGraph = {};

int constexpr mainMenuItemsSize = 4/*3*/;
int constexpr difficultyMenuItemsSize = 2;
int constexpr confirmMenuItemsSize = 2;

static bool stickTrigger_positive = false;
static bool stickTrigger_negative = false;

//タイトルロゴアニメーション
static float g_logo_anim_in = 0.0f;
static float g_logo_anim_rotate = 0.0f;

//ディレイ
static float g_start_delay;//時間の計測
static bool is_start;         //ディレイフラグ

//線形補間(枠の移動)
static float g_old_button;//古いポジション
static float g_Interpolation;//補間の割合を保存する変数
static float buttonZPositionY;//移動
static float buttonZPositionX;//移動

//ボタンのサイズ
static float buttonZScale;
static bool is_move;//移動フラグ
static float g_move;//ディレイ
static float scaleTime = 0.0f;//時間の計測

//プッシュ後のアニメーション
Float4  button_color;
static bool is_push;

// MP4が終わったらメインメニューを表示
static bool is_Spawn = false;
static float g_SpawnCount = 0;
static float g_fadeCount = 0;

static int g_titleHouchiCount = 0;//時間の計測

void Update_MainMenu(void);
void Update_DifficultyMenu(void);
void Update_ConfirmMenu(void);
//void Update_MultiMenu(void);
void Update_BossBattleMenu(void);
void Update_SoundRoomMenu(void);

void Draw_MainMenu(void);
void Draw_ConfirmMenu(void);
//void Draw_MultiMenu(void);
void Draw_SoundRoomMenu(void);
void Draw_DifficultyMenu(void);

struct s_menuItems
{
	Float2 position;
	// std::string text;
	TEX_TYPE texType;
};

struct s_menuItems_font
{
    Float2 position;
    // std::string text;
    std::string text;
};

enum e_switchingState
{
    SWITCHINGSTATE_MAINMENU,
    SWITCHINGSTATE_BOSSBATTLEMENU,
    SWITCHINGSTATE_SOUNDROOMMENU,
    //SWITCHINGSTATE_MULTIMENU,
	SWITCHINGSTATE_CONFIRMMENU,
    SWITCHINGSTATE_DIFFICULTY
};

enum e_menuItems
{
	MENUITEMS_START = 0,
    MENUITEMS_BOSSBATTLE,
    MENUITEMS_SOUNDROOM,
	//MENUITEMS_MULTI,// 2/26 髙野変更
	MENUITEMS_EXIT,
	MENUITEMS_MAX
};

enum e_confirmMenuItems
{
	CONFIRMMENU_YES = 0,
	CONFIRMMENU_NO,
	CONFIRMMENU_MAX
};

enum e_multiMenuItems
{
    MULTIMENU_YES = 0,
    MULTIMENU_NO,
    MULTIMENU_MAX
};

//選んでいる難易度の状態(2025/02/14 髙野追加)
//(2025/02/20 hara削除)
//typedef enum
//{
//    DIFFICULTY_NORMAL,//1
//    DIFFICULTY_HARD,//2
//    DIFFICULTY_MAX,
//}DIFFICULTY;

//タイトルテキスト設定
s_menuItems menuItems_ON[mainMenuItemsSize] =
{
	// {{SCREEN_WIDTH / 8, SCREEN_WIDTH * DX_WINDOW_SCREEN_MOD / 3}, "GAME START"},
	// {{SCREEN_WIDTH / 8, SCREEN_WIDTH * DX_WINDOW_SCREEN_MOD / 3 + 75}, "RANKING"},
	// {{SCREEN_WIDTH / 8, SCREEN_WIDTH * DX_WINDOW_SCREEN_MOD / 3 + 150}, "EXIT"}

	// {{-200, 50 }, "GAME START"},
	{{0, 175 }, TEX_TYPE_TITLE_GAMESTARTTEXT_ON},
    // {{-200, 175}, "RANKING"},
    //{{0, 310}, TEX_TYPE_TITLE_MULTITEXT_ON},
    {{0, 270}, TEX_TYPE_TITLE_BOSSBATTLETEXT_ON},
    // {{-200, 175}, "RANKING"},
    //{{0, 310}, TEX_TYPE_TITLE_MULTITEXT_ON},
    {{0, 365}, TEX_TYPE_TITLE_SOUNDROOMTEXT_ON},
    
	// {{-200, 300}, "EXIT"}
	{{0, 460}, TEX_TYPE_TITLE_EXITTEXT_ON}
};
s_menuItems menuItems_OFF[mainMenuItemsSize] =
{
    // {{SCREEN_WIDTH / 8, SCREEN_WIDTH * DX_WINDOW_SCREEN_MOD / 3}, "GAME START"},
    // {{SCREEN_WIDTH / 8, SCREEN_WIDTH * DX_WINDOW_SCREEN_MOD / 3 + 75}, "RANKING"},
    // {{SCREEN_WIDTH / 8, SCREEN_WIDTH * DX_WINDOW_SCREEN_MOD / 3 + 150}, "EXIT"}

    // {{-200, 50 }, "GAME START"},
    {{0, 175 }, TEX_TYPE_TITLE_GAMESTARTTEXT_OFF},
    // {{-200, 175}, "RANKING"},
    //{{0, 310}, TEX_TYPE_TITLE_MULTITEXT_ON},
    {{0, 270}, TEX_TYPE_TITLE_BOSSBATTLETEXT_OFF},
    // {{-200, 175}, "RANKING"},
    //{{0, 310}, TEX_TYPE_TITLE_MULTITEXT_OFF},
    {{0, 365}, TEX_TYPE_TITLE_SOUNDROOMTEXT_OFF},
    
    // {{-200, 300}, "EXIT"}
    {{0, 460}, TEX_TYPE_TITLE_EXITTEXT_OFF}
};

s_menuItems difficulty_menuItems_ON[mainMenuItemsSize] =
{
    // {{SCREEN_WIDTH / 8, SCREEN_WIDTH * DX_WINDOW_SCREEN_MOD / 3}, "GAME START"},
    // {{SCREEN_WIDTH / 8, SCREEN_WIDTH * DX_WINDOW_SCREEN_MOD / 3 + 75}, "RANKING"},
    // {{SCREEN_WIDTH / 8, SCREEN_WIDTH * DX_WINDOW_SCREEN_MOD / 3 + 150}, "EXIT"}

    // {{-200, 50 }, "GAME START"},
    {{0, -200 }, TEX_TYPE_TITLE_NORMAL_ON},
    // {{-200, 175}, "RANKING"},
    {{0, -50}, TEX_TYPE_TITLE_HARD_ON},
};
s_menuItems difficulty_menuItems_OFF[mainMenuItemsSize] =
{
    // {{SCREEN_WIDTH / 8, SCREEN_WIDTH * DX_WINDOW_SCREEN_MOD / 3}, "GAME START"},
    // {{SCREEN_WIDTH / 8, SCREEN_WIDTH * DX_WINDOW_SCREEN_MOD / 3 + 75}, "RANKING"},
    // {{SCREEN_WIDTH / 8, SCREEN_WIDTH * DX_WINDOW_SCREEN_MOD / 3 + 150}, "EXIT"}

    // {{-200, 50 }, "GAME START"},
    {{0, -200 }, TEX_TYPE_TITLE_NORMAL_OFF},
    // {{-200, 175}, "RANKING"},
    {{0, -50}, TEX_TYPE_TITLE_HARD_OFF},
};

//s_menuItems_font confirmMenuItems[confirmMenuItemsSize] =
//{
//	// {{SCREEN_WIDTH / 8, SCREEN_WIDTH * DX_WINDOW_SCREEN_MOD / 3}, "GAME START"},
//	// {{SCREEN_WIDTH / 8, SCREEN_WIDTH * DX_WINDOW_SCREEN_MOD / 3 + 75}, "RANKING"},
//	// {{SCREEN_WIDTH / 8, SCREEN_WIDTH * DX_WINDOW_SCREEN_MOD / 3 + 150}, "EXIT"}
//
//	{{0 - 150, 0}, "はい"},
//	{{0 + 150, 0}, "いいえ"},
//};


s_menuItems confirmMenuItems_ON[confirmMenuItemsSize] =
{
    {{-225.0f, 75.0f}, TEX_TYPE_TITLE_CONFIRM_YESTEXT_ON},
    {{225.0f, 75.0f}, TEX_TYPE_TITLE_CONFIRM_NOTEXT_ON},
};

s_menuItems confirmMenuItems_OFF[confirmMenuItemsSize] =
{
    {{-225.0f, 75.0f}, TEX_TYPE_TITLE_CONFIRM_YESTEXT_OFF},
    {{225.0f, 75.0f}, TEX_TYPE_TITLE_CONFIRM_NOTEXT_OFF},
};

e_switchingState switchingState = SWITCHINGSTATE_MAINMENU;
e_menuItems cursorMainMenu = MENUITEMS_START;
e_confirmMenuItems cursorConfirmMenu = CONFIRMMENU_NO;
e_multiMenuItems cursorMultiMenu = MULTIMENU_NO;
//選んでいる難易度のステート(2025/02/14 髙野追加)
//選んでいる難易度のステート(2025/02/20 hara列挙体変更)
Difficulty g_Difficulty = Difficulty::Normal;

//(2025/02/20 hara)
Difficulty GetDifficultySceneTitle()
{
    return g_Difficulty;
}
void SetDifficultySceneTitle(Difficulty diff)
{
    g_Difficulty = diff;
}

MULTI_FLAG g_MultiFlag = LOG_OUT;
static bool g_SingleBossBattleFlag = false; //シングルボスバトルフラグ(2025/07/09 hara追加)

void ResetTitleHouchiCount()
{
    g_titleHouchiCount = 0;
}

// ===================================================
// タイトルシーンの初期化
// ===================================================
void InitializeSceneTitle(void)
{
    SetMainGameDrawAutoFPSEnable(false);

    ResetTitleHouchiCount();

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


	g_in = 0;

	UIReset();
	//ShowGameFrame(true);

	ResetCamera();

	GameStopResetRequest();

    //
    GameManagerSetSceneRequestCancel();

    //シェーダを消す
    //SetPostEffectShader("");
    SetPostEffectShaderVS("");
    SetPostEffectShaderPS("");


    SetBg(BG_TYPE_TEST_TITLE);

    //既存のBGMを消す
    //SoundBGMStopRequest();

     /*SoundBGMPlayRequest(GetSoundData("SOUND/BGM/Test_BGM_Title2"),
         (FRAME_ONE_MINUTE * 1)
         + (FRAME_ONE_SECOND * 30),
         1.0f);*/

        //ムービーの再生
    LoadMovie(
        "rom/movie/title_movie_in.mp4", false,
        "rom/movie/title_movie_loop.mp4", true
    );
    {
        SOUND_ID bgm = GetSoundData("SOUND/BGM/Test_BGM_Title2");
        if (bgm != GetSoundBGMId())
        { 
            //異なるBGMが流れている、
            //またはBGMが流れていない場合再生する。
            SoundBGMPlayRequest(bgm,
                (FRAME_ONE_MINUTE * 1)
                + (FRAME_ONE_SECOND * 30),
                0.85f);
        }
    }

    stickTrigger_positive = false;
    stickTrigger_negative = false;

    //タイトルロゴ
    g_logo_anim_in = 0.0f;
    g_logo_anim_rotate = 0.0f;

    //ディレイ
    g_start_delay = 0.0f;
    is_start = false;

    //線形補間
    g_old_button = 0.0f;
    g_Interpolation = 1.0f;
    buttonZPositionY = 0.0f;
    buttonZPositionX = 0.0f;

    //サイズのアニメーション
    buttonZScale = 1.0f;
    is_move = false;
    g_move = 0.0f;

    button_color = { 1.0f,1.0f,1.0f,1.0f };//ボタンを押した後
    is_push = false;

    switchingState = SWITCHINGSTATE_MAINMENU;
    g_Difficulty = Difficulty::Normal;
    g_MultiFlag = LOG_OUT; //2025/02/27 hara
    g_SingleBossBattleFlag = false;

    // g_MultiFlag = LOG_OUT;

    // Keyboard_Initialize();

    ////デバッグログ
    //AllocConsole(); // コンソールを作成
    //FILE* a =  freopen("CONOUT$", "w", stdout); // stdoutをコンソールにリダイレクト
    //printf("Hello, World!\n"); // コンソールに出力
}

// ===================================================
// タイトルシーンの更新
// ===================================================
void UpdateSceneTitle(void)
{
    if (IsFading())
    {
        return;
    }

    g_titleHouchiCount++;
    if (g_titleHouchiCount > FRAME_ONE_SECOND * 20) 
    {
        //一定時間操作が無ければオープニングに戻る
        SetSceneFade(SCENE_OP);
        return;
    }

    {
        bool isButn = false;
        for (int i = 0; i < CONTROLLER_BUTTON_TYPE_MAX; i++)
        {
            isButn |= GetControllerMainTrigger((CONTROLLER_BUTTON_TYPE)i);
        }
        {
            CONTROLLER_STICK_DAT stickTrigger_Dat = GetControllerMainLeftStickTrigger();
            //Lスティックの入力を受け取る
            if (stickTrigger_Dat.enable)
            {
                if (abs(stickTrigger_Dat.stick.x) > 0.4f
                    || abs(stickTrigger_Dat.stick.y) > 0.4f)
                {
                    isButn |= true;
                }
            }
        }
        {
            CONTROLLER_STICK_DAT stickTrigger_Dat = GetControllerMainRightStickTrigger();
            //Rスティックの入力を受け取る
            if (stickTrigger_Dat.enable)
            {
                if (abs(stickTrigger_Dat.stick.x) > 0.4f
                    || abs(stickTrigger_Dat.stick.y) > 0.4f)
                {
                    isButn |= true;
                }
            }
        }

        if (isButn)
        {
            ResetTitleHouchiCount();
        }
    }

    //UpdateTitlebg();

    // シーン遷移

#ifdef SWITCH_MODE
#else

#if defined(_DEBUG) || defined(DEBUG)
    if ((Keyboard_IsKeyDown(KK_LEFTSHIFT) || Keyboard_IsKeyDown(KK_RIGHTSHIFT))
        && Keyboard_IsKeyDown(KK_LEFTCONTROL))
    { // デバッグシーンエディター
        SetSceneFade(SCENE_EDITOR);
    }

    //if ((Keyboard_IsKeyDown(KK_LEFTSHIFT) || Keyboard_IsKeyDown(KK_RIGHTSHIFT))
    //    && Keyboard_IsKeyDown(KK_C)
    //    && Keyboard_IsKeyDown(KK_A))
    //{ // デバッグシーンエディター
    //    SetScene(SCENE_MULTICLIENT);
    //}

    if ((Keyboard_IsKeyDown(KK_LEFTSHIFT) || Keyboard_IsKeyDown(KK_RIGHTSHIFT))
        && Keyboard_IsKeyDown(KK_UP)
        && Keyboard_IsKeyDown(KK_DOWN))
    { //
        SetScene(SCENE_GAME);
    }
#endif

#endif

    if (g_in < 1.0f)
    {
        g_in += (1.0f / 120);
        if (g_in >= 1.0f)
        {
            g_in = 1.0f;
        }
    }

    if (!is_Spawn)
    {
        bool isStartButn = false;
        for (int i = 0; i < CONTROLLER_BUTTON_TYPE_MAX; i++)
        {
            isStartButn |= GetControllerMainTrigger((CONTROLLER_BUTTON_TYPE)i);
        }

        if (isStartButn && !IsFading())
        {
            SoundEffectPlayRequest(SOUND_TYPE_SE_UI_OK, 4.0f, 0.0f);
            is_Spawn = true;
        }


        g_SpawnCount += 1.0f / 60.0f;
    }

    else if (is_Spawn)
    {
        g_fadeCount += (1.0f / 60.0f);
        if (g_fadeCount >= 1.0f)
        {
            g_fadeCount = 1.0f;
        }

        switch (switchingState)
        {
        case SWITCHINGSTATE_MAINMENU:
            Update_MainMenu();
            break;
        case SWITCHINGSTATE_CONFIRMMENU:
            Update_ConfirmMenu();
            break;
        case SWITCHINGSTATE_SOUNDROOMMENU:
            Update_SoundRoomMenu();
            break;
        case SWITCHINGSTATE_BOSSBATTLEMENU:
            Update_BossBattleMenu();
            break;
        //case SWITCHINGSTATE_MULTIMENU:
        //    Update_MultiMenu();
        //    break;
        case SWITCHINGSTATE_DIFFICULTY:
            Update_DifficultyMenu();
        default:
            break;
        }
    }
}

// ===================================================
// タイトルシーンの描画
// ===================================================
void DrawSceneTitle(void)
{
    if (!is_Spawn)
    {
        float alpha = (cosf(g_SpawnCount * 3.14159f) + 1.0f) / 2.0f + 0.1f;
        Float4 color = { 1.0f, 1.0f, 1.0f, alpha };
        DrawSpriteColorQuad(0, 310.0f, GetTexData(TEX_TYPE_TITLE_PRESS_ANY_BUTTON).width, GetTexData(TEX_TYPE_TITLE_PRESS_ANY_BUTTON).height, GetTexData(TEX_TYPE_TITLE_PRESS_ANY_BUTTON).textureId, &color);
    }

	switch (switchingState)
	{
	case SWITCHINGSTATE_MAINMENU:
		Draw_MainMenu();
		break;
	case SWITCHINGSTATE_CONFIRMMENU:
		Draw_MainMenu();
		Draw_ConfirmMenu();
		break;
    //case SWITCHINGSTATE_MULTIMENU:
    //    Draw_MainMenu();
    //    Draw_MultiMenu();
    //    break;
    case SWITCHINGSTATE_SOUNDROOMMENU:
        Draw_MainMenu();
        Draw_SoundRoomMenu();
        break;
    case SWITCHINGSTATE_DIFFICULTY:
        Draw_MainMenu();
        Draw_DifficultyMenu();
        break;
	default:
		break;
	}
}

// ===================================================
// タイトルシーンの終了
// ===================================================
void FinalizeSceneTitle(void)
{
    UnloadMovie();
	//FinalizeTitlebg();
    SetMainGameDrawAutoFPSEnable(true);
}

MULTI_FLAG GetMultiFlag()
{
    return g_MultiFlag;
}

bool IsSingleBossBattleFlag()
{
    return g_SingleBossBattleFlag;
}

//枠の移動
void Update_MainMenu(void)
{
    CONTROLLER_STICK_DAT stickTrigger_Dat = GetControllerMainLeftStickTrigger();

    stickTrigger_positive = false;
    stickTrigger_negative = false;

    //Lスティックの入力を受け取る
    if (stickTrigger_Dat.enable)
    {
        if (stickTrigger_Dat.stick.y > 0.4f)
        {
            stickTrigger_positive = true;
        }
        if (stickTrigger_Dat.stick.y < -0.4f)
        {
            stickTrigger_negative = true;
        }
    }
    if (!is_push)
    {
        if ((GetControllerMainTrigger(CONTROLLER_BUTTON_TYPE_UP) || stickTrigger_positive) && !IsFading())
        {
            SoundEffectPlayRequest(SOUND_TYPE_SE_UI_SELECT, 4.0f, 0.0f);


            cursorMainMenu = e_menuItems((cursorMainMenu - 1 + MENUITEMS_MAX) % MENUITEMS_MAX);
            g_old_button = menuItems_ON[static_cast<int>(cursorMainMenu)].position.y;//現在の位置を保存する
            g_Interpolation = 0.0f;
            is_move = true;
        }

        if ((GetControllerMainTrigger(CONTROLLER_BUTTON_TYPE_DOWN) || stickTrigger_negative) && !IsFading())
        {
            SoundEffectPlayRequest(SOUND_TYPE_SE_UI_SELECT, 4.0f, 0.0f);


            cursorMainMenu = e_menuItems((cursorMainMenu + 1) % MENUITEMS_MAX);
            g_old_button = buttonZPositionY;
            g_Interpolation = 0.0f;
            is_move = true;
        }
    }
    //イージング
    float t = EasingInOutSine(g_Interpolation);

    //線形補間
    buttonZPositionY = g_old_button * (1.0f - t) + menuItems_ON[static_cast<int>(cursorMainMenu)].position.y * t;

    if (is_move==true)
    {//trueになったら数秒語にfalse
        g_move++;

        if (g_move>20.0f)
        {
            is_move = false;
            g_move = 0.0f;
        }
    }

    g_Interpolation += 0.3f;//速度の設定

    if (g_Interpolation > 1.0f)
    {
        g_Interpolation = 1.0f;
    }

	if (GetControllerMainTrigger(CONTROLLER_BUTTON_TYPE_A) && !IsFading())
	{
		switch (cursorMainMenu)
		{
		case MENUITEMS_START:
			SoundEffectPlayRequest(SOUND_TYPE_SE_UI_OK, 4.0f, 0.0f);
            switchingState = SWITCHINGSTATE_DIFFICULTY;
			break;
		//case MENUITEMS_MULTI:
  //          SoundEffectPlayRequest(SOUND_TYPE_SE_UI_OK, 4.0f, 0.0f);
  //          switchingState = SWITCHINGSTATE_MULTIMENU;
		//	//SetSceneFade(SCENE_RANKING);
		//	break;
        case MENUITEMS_BOSSBATTLE:
            SoundEffectPlayRequest(SOUND_TYPE_SE_UI_OK, 4.0f, 0.0f);
            //switchingState = SWITCHINGSTATE_SOUNDROOMMENU;

            g_SingleBossBattleFlag = true; //シングルボスバトルフラグを立てる
            SetSceneFade(SCENE_SINGLEBOSSBATTLE, FADE_TYPE_STAR_WIPE);

            //SetSceneFade(SCENE_RANKING);
            break;
		case MENUITEMS_SOUNDROOM:
            SoundEffectPlayRequest(SOUND_TYPE_SE_UI_OK, 4.0f, 0.0f);
            //switchingState = SWITCHINGSTATE_SOUNDROOMMENU;

            SetSceneFade(SCENE_SOUNDROOM, FADE_TYPE_STAR_WIPE);

			//SetSceneFade(SCENE_RANKING);
			break;
		case MENUITEMS_EXIT:
			switchingState = SWITCHINGSTATE_CONFIRMMENU;
			SoundEffectPlayRequest(SOUND_TYPE_SE_UI_OK, 4.0f, 0.0f);
			/*if (MessageBox(NULL, "本当に終了しますか？", "確認", MB_OKCANCEL | MB_DEFBUTTON2) == IDOK)
			{
				exit(0);
			}*/
			break;
		default:
			break;
		}
	}

    if (is_start)
    {//ディレイ
        g_start_delay++;
        buttonZScale = 0.9f;        //へこませる

        if (g_start_delay > 15.0f)
        {
            buttonZScale = 1.0f;
        }
        if (g_start_delay > 45.0f)
        {
            is_start = false;
            is_push = false;
            g_start_delay = 0.0f;
            button_color = { 1.0f,1.0f,1.0f,1.0f };

            switch (cursorMainMenu)
            {
            case MENUITEMS_START:
                SetSceneFade(SCENE_STAGESELECT, FADE_TYPE_STAR_WIPE);
                break;
            //case MENUITEMS_MULTI:
            //    // SetSceneFade(SCENE_RANKING);
            //    break;
            default:
                break;
            }
        }
        switch (cursorMainMenu)
        {//ディレイなし
            case MENUITEMS_EXIT:
                switchingState = SWITCHINGSTATE_CONFIRMMENU;
                is_start = false;
                is_push = false;
                g_start_delay = 0.0f;
                break;
        default:
            break;
        }
    }
    else
    {
        if (!is_move)
        {//ボタン動かす
            scaleTime += 0.05f;
            buttonZScale = 1.0f + 0.05f * sin(scaleTime);
        }
    }

}

void Update_DifficultyMenu(void)
{
    CONTROLLER_STICK_DAT stickTrigger_Dat = GetControllerMainLeftStickTrigger();

    stickTrigger_positive = false;
    stickTrigger_negative = false;

    //Lスティックの入力を受け取る
    if (stickTrigger_Dat.enable)
    {
        if (stickTrigger_Dat.stick.y > 0.4f)
        {
            stickTrigger_positive = true;
        }
        if (stickTrigger_Dat.stick.y < -0.4f)
        {
            stickTrigger_negative = true;
        }
    }

    if (GetControllerMainTrigger(CONTROLLER_BUTTON_TYPE_B) && !IsFading())
    {
        SoundEffectPlayRequest(SOUND_TYPE_SE_UI_CANCEL, 4.0f, 0);
        cursorConfirmMenu = CONFIRMMENU_NO;
        switchingState = SWITCHINGSTATE_MAINMENU;
    }

    if ((GetControllerMainTrigger(CONTROLLER_BUTTON_TYPE_UP) || stickTrigger_positive) && !IsFading())
    {
        SoundEffectPlayRequest(SOUND_TYPE_SE_UI_SELECT, 4.0f, 0.0f);

        g_Difficulty = Difficulty(((int)g_Difficulty - 1 + (int)Difficulty::Max) % (int)Difficulty::Max);

        g_old_button = difficulty_menuItems_ON[static_cast<int>(g_Difficulty)].position.y;//現在の位置を保存する
        g_Interpolation = 0.0f;
        is_move = true;
    }

    if ((GetControllerMainTrigger(CONTROLLER_BUTTON_TYPE_DOWN) || stickTrigger_negative) && !IsFading())
    {
        SoundEffectPlayRequest(SOUND_TYPE_SE_UI_SELECT, 4.0f, 0.0f);

        g_Difficulty = Difficulty(((int)g_Difficulty + 1) % (int)Difficulty::Max);

        g_old_button = buttonZPositionY;
        g_Interpolation = 0.0f;
        is_move = true;
    }

    if (!is_move)
    {//ボタン動かす
        scaleTime += 0.05f;
        buttonZScale = 1.0f + 0.05f * sin(scaleTime);
    }

    //イージング
    float t = EasingInOutSine(g_Interpolation);

    //線形補間
    buttonZPositionY = g_old_button * (1.0f - t) + difficulty_menuItems_ON[static_cast<int>(g_Difficulty)].position.y * t;

    if (is_move == true)
    {//trueになったら数秒語にfalse
        g_move++;

        if (g_move > 20.0f)
        {
            is_move = false;
            g_move = 0.0f;
        }
    }

    g_Interpolation += 0.3f;//速度の設定

    if (g_Interpolation > 1.0f)
    {
        g_Interpolation = 1.0f;
    }


    if (GetControllerMainTrigger(CONTROLLER_BUTTON_TYPE_A) && !IsFading())
    {
        switch (g_Difficulty)
        {
        case Difficulty::Normal:
            SoundEffectPlayRequest(SOUND_TYPE_SE_UI_OK, 4.0f, 0.0f);
            printf("[DEBUG]選択された難易度はNORMALです。(%d)", g_Difficulty);
            SetSceneFade(SCENE_STAGESELECT, FADE_TYPE_STAR_WIPE);
            break;
        case Difficulty::Hard:
            SoundEffectPlayRequest(SOUND_TYPE_SE_UI_OK, 4.0f, 0.0f);
            printf("[DEBUG]選択された難易度はHARDです。(%d)",g_Difficulty);
            SetSceneFade(SCENE_STAGESELECT, FADE_TYPE_STAR_WIPE);
            break;
        default:
            break;
        }
    }

    //printf("[DEBUG]難易度(%d)\n", g_Difficulty);
}

void Update_ConfirmMenu(void)
{
    if (GetControllerMainTrigger(CONTROLLER_BUTTON_TYPE_B) && !IsFading())
    {
        SoundEffectPlayRequest(SOUND_TYPE_SE_UI_CANCEL, 4.0f, 0);
        cursorConfirmMenu = CONFIRMMENU_NO;
        switchingState = SWITCHINGSTATE_MAINMENU;
    }
	if (GetControllerMainTrigger(CONTROLLER_BUTTON_TYPE_LEFT) && !IsFading())
	{
        SoundEffectPlayRequest(SOUND_TYPE_SE_UI_SELECT, 4.0f, 0.0f);

        cursorConfirmMenu = e_confirmMenuItems((cursorConfirmMenu - 1 + CONFIRMMENU_MAX) % CONFIRMMENU_MAX);

        g_old_button = confirmMenuItems_ON[static_cast<int>(cursorConfirmMenu)].position.x;//現在の位置を保存する
        g_Interpolation = 0.0f;
        is_move = true;
	}

	if (GetControllerMainTrigger(CONTROLLER_BUTTON_TYPE_RIGHT) && !IsFading())
	{
        SoundEffectPlayRequest(SOUND_TYPE_SE_UI_SELECT, 4.0f, 0.0f);

		cursorConfirmMenu = e_confirmMenuItems((cursorConfirmMenu + 1) % CONFIRMMENU_MAX);

        g_old_button = buttonZPositionX;
        g_Interpolation = 0.0f;
        is_move = true;
	}

    if (!is_move)
    {//ボタン動かす
        scaleTime += 0.05f;
        buttonZScale = 1.0f + 0.05f * sin(scaleTime);
    }

    //イージング
    float t = EasingInOutSine(g_Interpolation);

    //線形補間
    buttonZPositionX = g_old_button * (1.0f - t) + confirmMenuItems_ON[static_cast<int>(cursorConfirmMenu)].position.x * t;

    if (is_move == true)
    {//trueになったら数秒語にfalse
        g_move++;

        if (g_move > 20.0f)
        {
            is_move = false;
            g_move = 0.0f;
        }
    }

    g_Interpolation += 0.3f;//速度の設定

    if (g_Interpolation > 1.0f)
    {
        g_Interpolation = 1.0f;
    }

	if (GetControllerMainTrigger(CONTROLLER_BUTTON_TYPE_A) && !IsFading())
	{
		switch (cursorConfirmMenu)
		{
        case CONFIRMMENU_YES:
            //exit(0);
            {
                DestroyWindow(GetMainHWND());
            }
			break;
		case CONFIRMMENU_NO:
            SoundEffectPlayRequest(SOUND_TYPE_SE_UI_CANCEL, 4.0f, 0);
			switchingState = SWITCHINGSTATE_MAINMENU;
			break;
		default:
			break;
		}
	}
}

//void Update_MultiMenu(void)
//{
//    if (GetControllerMainTrigger(CONTROLLER_BUTTON_TYPE_B))
//    {
//        SoundEffectPlayRequest(SOUND_TYPE_SE_UI_CANCEL, 4.0f, 0);
//        cursorMultiMenu = MULTIMENU_NO;
//        switchingState = SWITCHINGSTATE_MAINMENU;
//    }
//	if (GetControllerMainTrigger(CONTROLLER_BUTTON_TYPE_LEFT))
//	{
//        SoundEffectPlayRequest(SOUND_TYPE_SE_UI_SELECT, 4.0f, 0.0f);
//
//        cursorMultiMenu = e_multiMenuItems((cursorMultiMenu - 1 + CONFIRMMENU_MAX) % CONFIRMMENU_MAX);
//
//        g_old_button = confirmMenuItems_ON[static_cast<int>(cursorMultiMenu)].position.x;//現在の位置を保存する
//        g_Interpolation = 0.0f;
//        is_move = true;
//	}
//
//	if (GetControllerMainTrigger(CONTROLLER_BUTTON_TYPE_RIGHT))
//	{
//        SoundEffectPlayRequest(SOUND_TYPE_SE_UI_SELECT, 4.0f, 0.0f);
//
//        cursorMultiMenu = e_multiMenuItems((cursorMultiMenu + 1) % CONFIRMMENU_MAX);
//
//        g_old_button = buttonZPositionX;
//        g_Interpolation = 0.0f;
//        is_move = true;
//	}
//
//
//    if (!is_move)
//    {//ボタン動かす
//        scaleTime += 0.05f;
//        buttonZScale = 1.0f + 0.05f * sin(scaleTime);
//    }
//
//    //イージング
//    float t = EasingInOutSine(g_Interpolation);
//
//    //線形補間
//    buttonZPositionX = g_old_button * (1.0f - t) + confirmMenuItems_ON[static_cast<int>(cursorMultiMenu)].position.x * t;
//
//    if (is_move == true)
//    {//trueになったら数秒語にfalse
//        g_move++;
//
//        if (g_move > 20.0f)
//        {
//            is_move = false;
//            g_move = 0.0f;
//        }
//    }
//
//    g_Interpolation += 0.3f;//速度の設定
//
//    if (g_Interpolation > 1.0f)
//    {
//        g_Interpolation = 1.0f;
//    }
//
//	if (GetControllerMainTrigger(CONTROLLER_BUTTON_TYPE_A))
//	{
//		switch (cursorMultiMenu)
//		{
//		case MULTIMENU_YES:
//            //フラグだけへんこうできればおｋ
//            g_MultiFlag = MULTI_FLAG::LOG_IN;
//            printf("[DEBUG]g_MultiFlag = %d \n", g_MultiFlag);
//            SetSceneFade(SCENE_CHARACTERSELECT, FADE_TYPE_STAR_WIPE);
//			break;
//		case MULTIMENU_NO:
//            SoundEffectPlayRequest(SOUND_TYPE_SE_UI_CANCEL, 4.0f, 0);
//			switchingState = SWITCHINGSTATE_MAINMENU;
//            break;
//		default:
//			break;
//		}
//	}
//}
void Update_BossBattleMenu(void)
{
}
void Update_SoundRoomMenu(void)
{
}

void Draw_MainMenu(void)
{
    if (is_Spawn)
    {
        float alpha = EasingOutExpo(g_fadeCount);
        Float4 color = { 1.0f, 1.0f, 1.0f, alpha};
        //ボタン
        DrawSpriteColorQuad(menuItems_ON[static_cast<int>(cursorMainMenu)].position.x - GetTexData(menuItems_ON[static_cast<int>(cursorMainMenu)].texType).width * 0.01f, buttonZPositionY,
            GetTexData(TEX_TYPE_TITLE_BUTTON_Z).width * 0.9f * buttonZScale, GetTexData(TEX_TYPE_TITLE_BUTTON_Z).height * buttonZScale, GetTexData(TEX_TYPE_TITLE_BUTTON_Z).textureId, &button_color);

        for (int i = 0; i < mainMenuItemsSize; ++i)
        {//タイトルテキストの設定

            float textScale = 0.8f;

            if (i == static_cast<int>(cursorMainMenu))
            {
                textScale = buttonZScale;
            }

            DrawSpriteColorQuad(menuItems_OFF[i].position.x, menuItems_OFF[i].position.y, GetTexData(menuItems_OFF[i].texType).width * 1.2f, GetTexData(menuItems_OFF[i].texType).height * 1.2f, GetTexData(menuItems_OFF[i].texType).textureId, &color);
            if (i == static_cast<int>(cursorMainMenu))
            {
                DrawSpriteColorQuad(menuItems_ON[i].position.x, menuItems_ON[i].position.y, GetTexData(menuItems_ON[i].texType).width * 1.4f, GetTexData(menuItems_ON[i].texType).height * 1.4f, GetTexData(menuItems_ON[i].texType).textureId, &color);
            }
        }
    }
    // g_logo_anim_rotate += 1.0 / 480.0f;

    // DrawSpriteColorRotateQuad(0.0f, -210.0f, GetTexData(TEX_TYPE_TITLE_LOGO_Y).width * 0.34f, GetTexData(TEX_TYPE_TITLE_LOGO_Y).height * 0.34f, GetTexData(TEX_TYPE_TITLE_LOGO_Y).textureId, &color,0.0f);
    // DrawSpriteColorRotateQuad(0.0f, -210.0f, GetTexData(TEX_TYPE_TITLE_LOGO_Z).width * 0.355f, GetTexData(TEX_TYPE_TITLE_LOGO_Z).height * 0.355f, GetTexData(TEX_TYPE_TITLE_LOGO_Z).textureId, &color, -360 * g_logo_anim_rotate);
    // DrawSpriteColorQuad(0.0f, -230.0f, GetTexData(TEX_TYPE_TITLE_LOGO).width * 0.5f, GetTexData(TEX_TYPE_TITLE_LOGO).height * 0.5f, GetTexData(TEX_TYPE_TITLE_LOGO).textureId, &color);

    // //ボタン
    // DrawSpriteColorQuad(menuItems_ON[static_cast<int>(cursorMainMenu)].position.x - GetTexData(menuItems_ON[static_cast<int>(cursorMainMenu)].texType).width * 0.01f, buttonZPositionY,
    // GetTexData(TEX_TYPE_TITLE_BUTTON_Z).width * 0.9f* buttonZScale, GetTexData(TEX_TYPE_TITLE_BUTTON_Z).height* buttonZScale, GetTexData(TEX_TYPE_TITLE_BUTTON_Z).textureId, &button_color);

	// for (int i = 0; i < mainMenuItemsSize; ++i)
	// {//タイトルテキストの設定

    //     float textScale = 0.8f;

    //     if (i == static_cast<int>(cursorMainMenu))
    //     {
    //         textScale = buttonZScale;
    //     }

    //    // DrawSpriteQuad(menuItems_ON[i].position.x, menuItems_ON[i].position.y, GetTexData(menuItems_ON[i].texType).width * 0.3f* textScale, GetTexData(menuItems_ON[i].texType).height * 0.3f* textScale, GetTexData(menuItems_ON[i].texType).textureId);
    //     DrawSpriteColorQuad(menuItems_OFF[i].position.x, menuItems_OFF[i].position.y, GetTexData(menuItems_OFF[i].texType).width * 1.2f, GetTexData(menuItems_OFF[i].texType).height * 1.2f , GetTexData(menuItems_OFF[i].texType).textureId, &color);
    //     if (i == static_cast<int>(cursorMainMenu))
    //     {
    //         DrawSpriteColorQuad(menuItems_ON[i].position.x, menuItems_ON[i].position.y, GetTexData(menuItems_ON[i].texType).width * 1.4f, GetTexData(menuItems_ON[i].texType).height * 1.4f , GetTexData(menuItems_ON[i].texType).textureId, &color);
    //     }
	// }
}

void Draw_ConfirmMenu(void)
{
	Float4 color = { 1.0f, 1.0f, 1.0f, 1.0f };
    DrawSpriteColorQuad(0, 0, GetTexData(TEX_TYPE_TITLE_CONFIRM_CHECK_Z).width, GetTexData(TEX_TYPE_TITLE_CONFIRM_CHECK_Z).height, GetTexData(TEX_TYPE_TITLE_CONFIRM_CHECK_Z).textureId, &color);
    DrawSpriteColorQuad(0.0f, -100.0f, GetTexData(TEX_TYPE_TITLE_EXIT_TEXT).width, GetTexData(TEX_TYPE_TITLE_EXIT_TEXT).height, GetTexData(TEX_TYPE_TITLE_EXIT_TEXT).textureId,&color);

    //ボタン
    DrawSpriteColorQuad(buttonZPositionX, confirmMenuItems_ON[static_cast<int>(cursorConfirmMenu)].position.y,
        GetTexData(TEX_TYPE_TITLE_BUTTON_Z).width * buttonZScale, GetTexData(TEX_TYPE_TITLE_BUTTON_Z).height * buttonZScale, GetTexData(TEX_TYPE_TITLE_BUTTON_Z).textureId, &button_color);

    for (int i = 0; i < confirmMenuItemsSize; i++)
    {
        float textScale = 0.8f;

        if (i == static_cast<int>(cursorConfirmMenu))
        {
            textScale = buttonZScale;
        }
        DrawSpriteColorQuad(confirmMenuItems_OFF[i].position.x, confirmMenuItems_OFF[i].position.y, GetTexData(confirmMenuItems_OFF[i].texType).width * 0.8f, GetTexData(confirmMenuItems_OFF[i].texType).height * 0.8f, GetTexData(confirmMenuItems_OFF[i].texType).textureId, &color);

        if (i == static_cast<int>(cursorConfirmMenu))
        {
            DrawSpriteColorQuad(confirmMenuItems_ON[i].position.x, confirmMenuItems_ON[i].position.y, GetTexData(confirmMenuItems_ON[i].texType).width * 1.1f, GetTexData(confirmMenuItems_ON[i].texType).height * 1.1f, GetTexData(confirmMenuItems_ON[i].texType).textureId, &color);
        }
    }
}

//void Draw_MultiMenu(void)
//{
//    
//    Float4 color = { 1.0f, 1.0f, 1.0f, 1.0f };
//	DrawSpriteColorQuad(0, 0, GetTexData(TEX_TYPE_TITLE_CONFIRM_CHECK_Z).width, GetTexData(TEX_TYPE_TITLE_CONFIRM_CHECK_Z).height, GetTexData(TEX_TYPE_TITLE_CONFIRM_CHECK_Z).textureId, &color);
//    DrawSpriteColorQuad(0, -100, GetTexData(TEX_TYPE_TITLE_MULTI_TEXT).width, GetTexData(TEX_TYPE_TITLE_MULTI_TEXT).height, GetTexData(TEX_TYPE_TITLE_MULTI_TEXT).textureId, &color);
//
//    //ボタン
//    DrawSpriteColorQuad(buttonZPositionX, confirmMenuItems_ON[static_cast<int>(cursorMultiMenu)].position.y,
//        GetTexData(TEX_TYPE_TITLE_BUTTON_Z).width * buttonZScale, GetTexData(TEX_TYPE_TITLE_BUTTON_Z).height * buttonZScale, GetTexData(TEX_TYPE_TITLE_BUTTON_Z).textureId, &button_color);
//
//    for (int i = 0; i < confirmMenuItemsSize; i++)
//    {
//        float textScale = 0.8f;
//
//        if (i == static_cast<int>(cursorMultiMenu))
//        {
//            textScale = buttonZScale;
//        }
//        DrawSpriteColorQuad(confirmMenuItems_OFF[i].position.x, confirmMenuItems_OFF[i].position.y, GetTexData(confirmMenuItems_OFF[i].texType).width * 0.8f , GetTexData(confirmMenuItems_OFF[i].texType).height * 0.8f, GetTexData(confirmMenuItems_OFF[i].texType).textureId, &color);
//
//        if (i == static_cast<int>(cursorMultiMenu))
//        {
//            DrawSpriteColorQuad(confirmMenuItems_ON[i].position.x, confirmMenuItems_ON[i].position.y , GetTexData(confirmMenuItems_ON[i].texType).width * 1.1f, GetTexData(confirmMenuItems_ON[i].texType).height * 1.1f, GetTexData(confirmMenuItems_ON[i].texType).textureId, &color);
//        }
//    }
//}
void Draw_SoundRoomMenu(void)
{

    Float4 color = { 1.0f, 1.0f, 1.0f, 1.0f };
    DrawSpriteColorQuad(0, 0, GetTexData(TEX_TYPE_TITLE_CONFIRM_CHECK_Z).width, GetTexData(TEX_TYPE_TITLE_CONFIRM_CHECK_Z).height, GetTexData(TEX_TYPE_TITLE_CONFIRM_CHECK_Z).textureId, &color);
    DrawSpriteColorQuad(0, -100, GetTexData(TEX_TYPE_TITLE_MULTI_TEXT).width, GetTexData(TEX_TYPE_TITLE_MULTI_TEXT).height, GetTexData(TEX_TYPE_TITLE_MULTI_TEXT).textureId, &color);

    //ボタン
    DrawSpriteColorQuad(buttonZPositionX, confirmMenuItems_ON[static_cast<int>(cursorMultiMenu)].position.y,
        GetTexData(TEX_TYPE_TITLE_BUTTON_Z).width * buttonZScale, GetTexData(TEX_TYPE_TITLE_BUTTON_Z).height * buttonZScale, GetTexData(TEX_TYPE_TITLE_BUTTON_Z).textureId, &button_color);

    for (int i = 0; i < confirmMenuItemsSize; i++)
    {
        float textScale = 0.8f;

        if (i == static_cast<int>(cursorMultiMenu))
        {
            textScale = buttonZScale;
        }
        DrawSpriteColorQuad(confirmMenuItems_OFF[i].position.x, confirmMenuItems_OFF[i].position.y, GetTexData(confirmMenuItems_OFF[i].texType).width * 0.8f, GetTexData(confirmMenuItems_OFF[i].texType).height * 0.8f, GetTexData(confirmMenuItems_OFF[i].texType).textureId, &color);

        if (i == static_cast<int>(cursorMultiMenu))
        {
            DrawSpriteColorQuad(confirmMenuItems_ON[i].position.x, confirmMenuItems_ON[i].position.y, GetTexData(confirmMenuItems_ON[i].texType).width * 1.1f, GetTexData(confirmMenuItems_ON[i].texType).height * 1.1f, GetTexData(confirmMenuItems_ON[i].texType).textureId, &color);
        }
    }
}

void Draw_DifficultyMenu(void)
{
    Float4 color = { 1.0f, 1.0f, 1.0f, 1.0f };
    DrawSpriteColorQuad(0, 0, GetTexData(TEX_TYPE_TITLE_CONFIRM_DIFFICULTY).width, GetTexData(TEX_TYPE_TITLE_CONFIRM_DIFFICULTY).height, GetTexData(TEX_TYPE_TITLE_CONFIRM_DIFFICULTY).textureId, &color);
    DrawSpriteColorQuad(390, 330, GetTexData(TEX_TYPE_TITLE_BUTTON_A).width, GetTexData(TEX_TYPE_TITLE_BUTTON_A).height, GetTexData(TEX_TYPE_TITLE_BUTTON_A).textureId, &color);
    DrawSpriteColorQuad(230, 330, GetTexData(TEX_TYPE_TITLE_BUTTON_B).width, GetTexData(TEX_TYPE_TITLE_BUTTON_B).height, GetTexData(TEX_TYPE_TITLE_BUTTON_B).textureId, &color);


    //ボタン
    DrawSpriteColorQuad(difficulty_menuItems_ON[static_cast<int>(g_Difficulty)].position.x, buttonZPositionY,
        GetTexData(TEX_TYPE_TITLE_BUTTON_Z2).width * buttonZScale, GetTexData(TEX_TYPE_TITLE_BUTTON_Z2).height * buttonZScale, GetTexData(TEX_TYPE_TITLE_BUTTON_Z2).textureId, &button_color);

    for (int i = 0; i < difficultyMenuItemsSize; ++i)
    {//タイトルテキストの設定

        float textScale = 0.8f;

        if (i == static_cast<int>(g_Difficulty))
        {
            textScale = buttonZScale;
        }

        // DrawSpriteQuad(menuItems_ON[i].position.x, menuItems_ON[i].position.y, GetTexData(menuItems_ON[i].texType).width * 0.3f* textScale, GetTexData(menuItems_ON[i].texType).height * 0.3f* textScale, GetTexData(menuItems_ON[i].texType).textureId);
        DrawSpriteColorQuad(difficulty_menuItems_OFF[i].position.x, difficulty_menuItems_OFF[i].position.y, GetTexData(difficulty_menuItems_OFF[i].texType).width * 0.8f, GetTexData(difficulty_menuItems_OFF[i].texType).height * 0.8f, GetTexData(difficulty_menuItems_OFF[i].texType).textureId, &color);
        if (i == static_cast<int>(g_Difficulty))
        {
            DrawSpriteColorQuad(difficulty_menuItems_ON[i].position.x, difficulty_menuItems_ON[i].position.y, GetTexData(difficulty_menuItems_ON[i].texType).width, GetTexData(difficulty_menuItems_ON[i].texType).height, GetTexData(difficulty_menuItems_ON[i].texType).textureId, &color);
            //DrawSpriteColorQuad(difficulty_menuItems_ON[i].position.x, difficulty_menuItems_ON[i].position.y, GetTexData(TEX_TYPE_TITLE_BUTTON_Z2).width, GetTexData(TEX_TYPE_TITLE_BUTTON_Z2).height * 1.1f, GetTexData(TEX_TYPE_TITLE_BUTTON_Z2).textureId, &color);
        }
    }

    //難易度の説明
    if (g_Difficulty == Difficulty::Normal)
    {
        DrawSpriteColorQuad(0, 175, GetTexData(TEX_TYPE_TITLE_NORMAL_TEXT).width * 1.1f , GetTexData(TEX_TYPE_TITLE_NORMAL_TEXT).height * 1.1f, GetTexData(TEX_TYPE_TITLE_NORMAL_TEXT).textureId, &color);
    }
    else if (g_Difficulty == Difficulty::Hard)
    {
        DrawSpriteColorQuad(0, 175, GetTexData(TEX_TYPE_TITLE_HARD_TEXT).width * 1.1f, GetTexData(TEX_TYPE_TITLE_HARD_TEXT).height * 1.1f, GetTexData(TEX_TYPE_TITLE_HARD_TEXT).textureId, &color);
    }
}

