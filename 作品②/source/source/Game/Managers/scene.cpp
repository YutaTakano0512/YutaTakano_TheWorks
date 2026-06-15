// ===================================================
// scene.cpp シ－ン制御
// 
// hara sougo  2024/07/22
// ===================================================
#include "../../game_main.h"
#include "../Sprite/sprite.h"
#include "../Texture/texture.h"
#include "scene.h"
#include "../../Core/Input/controller_main.h"

//シーンを追加する際に書き加える必要があるところへ
// ↓を書いておきます
//addpoint
#include "../../Scenes/SCENE_Title.h"
#include "../../Scenes/SCENE_Game.h"
#include "../../Scenes/SCENE_Result.h"
#include "../../Scenes/SCENE_Editor.h"
#include "../../Scenes/SCENE_StageSelect.h"
#include "../../Scenes/SCENE_Character_Select.h"
#include "../../Scenes/SCENE_SoundRoom.h"
#include "../../Scenes/SCENE_SingleBossBattle.h"
#include "../../Scenes/SCENE_Op.h"

#include "../../Scenes/SCENE_MultiClient.h"

// ===================================================
// グローバル変数
// ===================================================
static SCENE scene = SCENE_INIT;	// シーン情報
static SCENE sceneBefore = SCENE_INIT;	// シーン情報


// ===================================================
// プロトタイプ宣言
// ===================================================
void uninitScenePrivate(void);

//addpoint
void becomeTitle();
void becomeStageSelect();
void becomeGame();
void becomeResult();
void becomeEditor();
void becomeCharacterSelect();
void becomeSoundRoom();
void becomeSingleBossBattle();
void becomeMultiClient();
void becomeOp();

// ===================================================
// フェードの初期化
// ===================================================
void InitScene(void)
{
	scene = SCENE_INIT;
}

// ===================================================
// フェードの更新
// ===================================================
void UpdateScene(void)
{
	// 各シーンによって更新処理の分岐
	switch (scene)
	{
		//addpoint
	case SCENE_TITLE:
		UpdateSceneTitle();
		break;
	case SCENE_STAGESELECT:
		UpdateSceneStageSelect();
		break;
	case SCENE_CHARACTERSELECT:
		UpdateSceneCharacterSelect();
		break;
	case SCENE_GAME:
		UpdateSceneGame();
		break;
	case SCENE_RESULT:
		UpdateSceneResult();
		break;
    case SCENE_SOUNDROOM:
        UpdateSceneSoundRoom();
        break;
    case SCENE_SINGLEBOSSBATTLE:
        UpdateSceneSingleBossBattle();
        break;
    case SCENE_OP:
        UpdateSceneOp();
        break;
    case SCENE_MULTICLIENT:
        UpdateSceneMultiClient();
        break;
    case SCENE_EDITOR:
        UpdateSceneEditor();
		break;
	default:break;
	}
}

// ===================================================
// フェードの描画
// ===================================================
void DrawScene(void)
{
	// 各シーンによって更新処理の分岐
	switch (scene)
	{
		//addpoint
	case SCENE_TITLE:
		DrawSceneTitle();
		break;
	case SCENE_STAGESELECT:
		DrawSceneStageSelect();
		break;
	case SCENE_CHARACTERSELECT:
		DrawSceneCharacterSelect();
		break;
	case SCENE_GAME:
		DrawSceneGame();
		break;
    case SCENE_RESULT:
        DrawSceneResult();
        break;
    case SCENE_SOUNDROOM:
        DrawSceneSoundRoom();
        break;
    case SCENE_SINGLEBOSSBATTLE:
        DrawSceneSingleBossBattle();
        break;
    case SCENE_OP:
        DrawSceneOp();
        break;
    case SCENE_MULTICLIENT:
        DrawSceneMultiClient();
        break;
    case SCENE_EDITOR:
        DrawSceneEditor();
        break;
	default:
		break;
	}
}

// ===================================================
// フェードの終了処理
// ===================================================
void UninitScene(void)
{
	uninitScenePrivate();
}

void uninitScenePrivate(void)
{
	// 各シーンによって更新処理の分岐
	switch (scene)
	{
		//addpoint
	case SCENE_TITLE:
		FinalizeSceneTitle();
		break;
	case SCENE_STAGESELECT:
		FinalizeSceneStageSelect();
		break;
	case SCENE_CHARACTERSELECT:
		FinalizeSceneCharacterSelect();
		break;
	case SCENE_GAME:
		FinalizeSceneGame();
		break;
    case SCENE_RESULT:
        FinalizeSceneResult();
        break;
    case SCENE_SOUNDROOM:
        FinalizeSceneSoundRoom();
        break;
    case SCENE_SINGLEBOSSBATTLE:
        FinalizeSceneSingleBossBattle();
        break;
    case SCENE_OP:
        FinalizeSceneOp();
        break;
    case SCENE_MULTICLIENT:
        FinalizeSceneMultiClient();
        break;
    case SCENE_EDITOR:
        FinalizeSceneEditor();
		break;
	default:break;
	}

}

SCENE GetSceneBefore(void)
{
    return sceneBefore;
}

// ===================================================
// シーンの取得
// ===================================================
SCENE GetScene(void)
{
	return scene;
}

// ===================================================
// シーンのセット
// 引数
//  s ：遷移先のシーン
// ===================================================
void SetScene(SCENE s)
{
	// 現在のシーンの終了
	uninitScenePrivate();

    //以前のシーンを保存
    sceneBefore = scene;
	// 現在のシーンの更新
	scene = s;

	// 次のシーンの初期化
	switch (scene)
	{
		//addpoint
	case SCENE_TITLE:
		InitializeSceneTitle();
		break;
	case SCENE_STAGESELECT:
		InitializeSceneStageSelect();
		break;
	case SCENE_CHARACTERSELECT:
		InitializeSceneCharacterSelect();
		break;
	case SCENE_GAME:
		InitializeSceneGame();
		break;
    case SCENE_RESULT:
        InitializeSceneResult();
        break;
    case SCENE_SOUNDROOM:
        InitializeSceneSoundRoom();
        break;
    case SCENE_SINGLEBOSSBATTLE:
        InitializeSceneSingleBossBattle();
        break;
    case SCENE_OP:
        InitializeSceneOp();
        break;
	case SCENE_MULTICLIENT:
		InitializeSceneMultiClient();
		break;
    case SCENE_EDITOR:
        InitializeSceneEditor();
        break;
	default:break;
	}
}

bool SetSceneFade(SCENE setScene, 
    FADE_TYPE fadetype,
    std::function<float(float)> easing,
    int speed
)
{
	switch (setScene)
	{

		//addpoint
	case SCENE_TITLE:
	{
		bool r = SetFade(fadetype, becomeTitle,
            easing, speed
		);
		return r;
		//break;
	}

	case SCENE_STAGESELECT:
	{
		bool r = SetFade(fadetype, becomeStageSelect,
            easing, speed
		);
		return r;
		//break;
	}

	case SCENE_CHARACTERSELECT:
	{
		bool r = SetFade(fadetype, becomeCharacterSelect,
            easing, speed
		);
		return r;
		//break;
	}

	case SCENE_GAME:
	{
		bool r = SetFade(fadetype, becomeGame,
            easing, speed
		);
		return r;
		//break;
	}

    case SCENE_RESULT:
    {
        bool r = SetFade(fadetype, becomeResult,
            easing, speed
        );
        return r;
        //break;
    }
    case SCENE_SOUNDROOM:
    {
        bool r = SetFade(fadetype, becomeSoundRoom,
            easing, speed
        );
        return r;
        //break;
    }
    case SCENE_SINGLEBOSSBATTLE:
    {
        bool r = SetFade(fadetype, becomeSingleBossBattle,
            easing, speed
        );
        return r;
        //break;
    }
    case SCENE_OP:
    {
        bool r = SetFade(fadetype, becomeOp,
            easing, speed
        );
        return r;
        //break;
    }

	case SCENE_MULTICLIENT:
	{
		bool r = SetFade(fadetype, becomeMultiClient,
            easing, speed
		);
		return r;
		//break;
	}

    case SCENE_EDITOR:
    {
        bool r = SetFade(fadetype, becomeEditor,
            easing, speed
        );
        return r;
        //break;
    }
	default:break;
	}
	return false;
}

//addpoint
void becomeTitle()
{
	SetScene(SCENE_TITLE);
}

void becomeStageSelect()
{
	SetScene(SCENE_STAGESELECT);
}

void becomeCharacterSelect()
{
	SetScene(SCENE_CHARACTERSELECT);
}

void becomeGame()
{
	SetScene(SCENE_GAME);
}

void becomeResult()
{
    SetScene(SCENE_RESULT);
}
void becomeSoundRoom()
{
    SetScene(SCENE_SOUNDROOM);
}
void becomeSingleBossBattle()
{
    SetScene(SCENE_SINGLEBOSSBATTLE);
}
void becomeOp()
{
    SetScene(SCENE_OP);
}

void becomeMultiClient()
{
    SetScene(SCENE_MULTICLIENT);
}


void becomeEditor()
{
    SetScene(SCENE_EDITOR);
}

