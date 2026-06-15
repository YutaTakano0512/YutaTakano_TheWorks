// ===================================================
// player.cpp プレイヤー制御
// 
// hara sougo  2024/07/22
// ===================================================
#include "../../game_main.h"
#include "../Configuration/Param.h"
#include "../Sprite/sprite.h"
#include "../Texture/texture.h"
#include "player.h"
#include "../../Core/Input/controller_main.h"
#include "../Entities/Block/block.h"
#include "../Systems/collision.h"
#include "../Systems/easing.h"

#include "../Game Object/gameObjectGraph.h"
#include "../Game Object/gameObjectPrefab.h"

#include "../Game Object/Game Object Events/GOE_General.h"
#include "../Game Object/Game Object Events/GOE_Character_Player.h"

#include "../Managers/gameManager.h"

#include "../Managers/scene.h"
#include "../../scenes/SCENE_GAME.h"

#include "../Systems/camera.h"
#include "../Managers/soundManager.h"

#include "../Systems/ui.h"

#include "character.h"
#include "../../Scenes/SCENE_Character_Select.h"

#include "../Game Events/GE_Gaya.h"
#include "../Game Events/GE_Shoot.h"

#include "../Game Stage/Stage_Base.h"

#include "../../Core/Multi/winsock.h"
#include "../../Core/Multi/winsock_server.h"
#include "../../Core/Multi/winsock_client.h"
// ===================================================
// マクロ定義
// ===================================================

// ===================================================
// プロトタイプ宣言
// ===================================================

// ===================================================
// グローバル変数
// ===================================================

static PLAYER players[PLAYER_MAX] = {};
static bool g_loginEnable = false;
static int g_PlayerTotalDamage = 0;


// ===================================================
// プレイヤーの初期化
// ===================================================
void InitPlayer(void)
{

	for (int i = 0; i < PLAYER_MAX; i++)
	{
		players[i] = {};
		players[i].obj = nullptr;
		players[i].index = i;

	}

	//プレイヤーを生成
	{
		GAME_OBJECT* co = nullptr;
		//GAME_OBJECT* co2 = nullptr;

		//キャラ読み込み
		Character::MAIN* cd = nullptr;

        int playerNo = -1;
		//コードの追加（キャラクター選択）
		switch (Scene::Select::selected_id)
		{
		//case 0:
		//	// cd = Character::Load("PLAYER_TEST"); 

		//	// MEMORY !!!
		//	break;
		case 1:
			// cd = Character::Load("PLAYER_TEST2");

			// MEMORY !!!

			co = Character::CreateCharacterGameObject("PLAYER_NEO");
            playerNo = Scene::Select::selected_id;
			break;
        case 2:
            // cd = Character::Load("PLAYER_TEST3");

            // MEMORY !!!

            co = Character::CreateCharacterGameObject("PLAYER_KN");
            playerNo = Scene::Select::selected_id;
            break;
        case 3:
            // cd = Character::Load("PLAYER_TEST3");

            // MEMORY !!!

            co = Character::CreateCharacterGameObject("PLAYER_CA");
            playerNo = Scene::Select::selected_id;
            break;

		default:
            if (Keyboard_IsKeyDown(KK_D4))
            {
                co = Character::CreateCharacterGameObject("PLAYER_CA");
                playerNo = 3;
            }
            else if (Keyboard_IsKeyDown(KK_D3))
            {
                co = Character::CreateCharacterGameObject("PLAYER_KN");
                playerNo = 2;
            }
            else if (Keyboard_IsKeyDown(KK_D2))
            {
                co = Character::CreateCharacterGameObject("PLAYER_NEO");
                playerNo = 1;
            }
            else
            {
                co = Character::CreateCharacterGameObject("PLAYER_KAI");
                playerNo = 0;
            }
			break;
		}

		if (cd != nullptr)
		{
			//キャラオブジェクト生成
			// co = Character::CreateCharacterGameObject(cd);
			//co2 = Character::CreateCharacterGameObject(cd);

			//キャラデータアンロード
			// Character::Unload(cd);
		}

		if (co != nullptr)
		{
            co->pos = {};
			//co->pos.x = -500.0f;
			//co->pos.y = 0.0f;
			//co->pos.x = 0.0f;
			//co->pos.y = BLOCK_HIGHT * 36;

			//co->scale.x = 0.15f;
			//co->scale.y = 0.15f;

			//co->graphic.color = MkF4(2.0f, 0.4f, 0.4f, 1.0f);

			//DebugPrintf("seijouni");

            int onajiNamePl = 0;
            if (GetControllerMainPress(CONTROLLER_BUTTON_TYPE_LEFT)
                || GetControllerMainPress(CONTROLLER_BUTTON_TYPE_RIGHT))
            {
                onajiNamePl = 1;
            }
            if (GetControllerMainPress(CONTROLLER_BUTTON_TYPE_DOWN))
            {
                onajiNamePl = 2;
            }

            GOE_Character_Player* goe =
                dynamic_cast<GOE_Character_Player*>(co->events.source);  // ダウンキャスト
            if (goe != nullptr)
            {
                goe->SetOnajiCharaNum(onajiNamePl);
                goe->SetCharaSubGraphic();
            }

			players[0].obj = co;
            players[0].playerNo = playerNo;
		}
		//if (co2 != nullptr)
		//{
		//	co2->pos.x = 500.0f;
		//	//co2->pos.x = 0.0f;
		//	co2->pos.y = 0.0f;
		//	//co2->pos.y = 0.0f - 1000.0f;

		//	//co2->scale.x = 1.6f;
		//	//co2->scale.y = 1.6f;

		//	co2->scale.x = 0.75f;
		//	co2->scale.y = 0.75f;

		//	co2->graphic.color = MkF4(2.0f, 2.0f, 0.0f, 1.0f);

		//	//DebugPrintf("seijouni");
		//	players[1].obj = co2;
		//}
	}


    //{ //2P仮
    //    GAME_OBJECT* co = nullptr;

    //    //キャラクター選択
    //    co = Character::CreateCharacterGameObject("PLAYER_NEO");

    //    if (co != nullptr)
    //    {
    //        //co->pos.x = -500.0f;
    //        //co->pos.y = 0.0f;
    //        co->pos.x = 0.0f + BLOCK_WIDTH * 3;
    //        co->pos.y = BLOCK_HIGHT * 36;

    //        //co->scale.x *= 0.7f;
    //        //co->scale.y *= 0.7f;
    //        
    //        //co->scale.x = 0.15f;
    //        //co->scale.y = 0.15f;
    //        //DebugPrintf("seijouni");

    //        players[1].obj = co;
    //    }
    //}

    //{ //3P仮
    //    GAME_OBJECT* co = nullptr;

    //    //キャラクター選択
    //    co = Character::CreateCharacterGameObject("PLAYER_KN");

    //    if (co != nullptr)
    //    {
    //        //co->pos.x = -500.0f;
    //        //co->pos.y = 0.0f;
    //        co->pos.x = 0.0f + BLOCK_WIDTH * -3;
    //        co->pos.y = BLOCK_HIGHT * 36;


    //        //co->scale.x = 0.15f;
    //        //co->scale.y = 0.15f;
    //        //DebugPrintf("seijouni");

    //        players[2].obj = co;
    //    }
    //}

    //{ //4P仮
    //    GAME_OBJECT* co = nullptr;

    //    //キャラクター選択
    //    co = Character::CreateCharacterGameObject("PLAYER_CA");

    //    if (co != nullptr)
    //    {
    //        //co->pos.x = -500.0f;
    //        //co->pos.y = 0.0f;
    //        co->pos.x = 0.0f + BLOCK_WIDTH * 8;
    //        co->pos.y = BLOCK_HIGHT * 36;


    //        //co->scale.x = 0.15f;
    //        //co->scale.y = 0.15f;
    //        //DebugPrintf("seijouni");

    //        players[3].obj = co;
    //    }
    //}

	//players[0].obj = CreateGameObject(GetGameObjectPrefab(GAME_OBJECT_TYPE_PLAYER));
	//players[1].obj = CreateGameObject(GetGameObjectPrefab(GAME_OBJECT_TYPE_PLAYER));

	//if (players[0].obj != nullptr)
	//{
	//	SetCameraPos(MkF2(players[0].obj->pos.x, players[0].obj->pos.y));
	//}

	//テスト 同時に敵も出す
	//for (int i = 0; i < 1; i++)
	//{
	//	//players[i].obj = nullptr;
	//	GAME_OBJECT* e = CreateGameObject(GetGameObjectPrefab(GAME_OBJECT_TYPE_TEST));
	//	e->pos = MkF3(-800.0f + 200.0f * i, 0.0f, 0.0f);
	//}
	//GAME_OBJECT* e = CreateGameObject(GetGameObjectPrefab(GAME_OBJECT_TYPE_TEST));
	//e->pos = { 200.0f, 0.0f };

    ResetPlayers();
}

// ===================================================
// プレイヤーの更新
// ===================================================
void UpdatePlayer(void)
{

    //マルチのプレイヤーを生成
    //if (WinSockGetType() == WINSOCK_CONNECT_TYPE::SERVER)
    //{
    //    if (g_loginEnable)
    //    {
    //        std::vector<WINSOCK_SERVER_SERVER_PLAYERDAT> pd = WinSockServerGetPlayerDat();

    //        for (int i = 0; i < std::min((int)pd.size(), PLAYER_MAX - 1); i++)
    //        {
    //            int pObjIndex = i + 1;

    //            if (players[pObjIndex].obj != nullptr)
    //            {
    //                if (!pd[i].enable)
    //                {
    //                    GameObjectSetLifeTime(players[pObjIndex].obj, 1);
    //                    //DeleteGameObject(players[pObjIndex].obj);
    //                    players[pObjIndex].obj = nullptr;
    //                    players[pObjIndex].playerNo = -1;
    //                }
    //            }
    //            else
    //            {
    //                if (pd[i].enable)
    //                {
    //                    std::string pn;
    //                    switch (pd[i].dat.playerdat.playerNo)
    //                    {
    //                    case 1:
    //                        pn = "PLAYER_NEO";
    //                        break;
    //                    case 2:
    //                        pn = "PLAYER_KN";
    //                        break;
    //                    case 3:
    //                        pn = "PLAYER_CA";
    //                        break;
    //                    default:
    //                        pn = "PLAYER_KAI";
    //                        break;
    //                    }

    //                    GAME_OBJECT* co = nullptr;

    //                    //キャラクター選択
    //                    co = Character::CreateCharacterGameObject(pn);

    //                    if (co != nullptr)
    //                    {

    //                        int onajiNamePl = 0;
    //                        for (int j = 0; j < PLAYER_MAX; j++)
    //                        {
    //                            PLAYER* pl = GetPlayer(j);
    //                            GAME_OBJECT* plObj = pl->obj;
    //                            if (plObj != nullptr)
    //                            {
    //                                GOE_Character_Player* goe =
    //                                    dynamic_cast<GOE_Character_Player*>(plObj->events.source);  // ダウンキャスト
    //                                if (goe != nullptr)
    //                                {
    //                                    Character::OBJECT_DAT* chaOb = goe->GetChara();
    //                                    if (chaOb != nullptr)
    //                                    {
    //                                        const Character::MAIN* chaMain = 
    //                                            Character::GetCharacterMainData(chaOb->dataIndex);
    //                                        if (chaMain != nullptr)
    //                                        {
    //                                            if (chaMain->name == pn)
    //                                            {
    //                                                onajiNamePl++;
    //                                            }
    //                                        }
    //                                    }
    //                                    
    //                                }
    //                            }
    //                        }


    //                        GOE_Character_Player* goe =
    //                            dynamic_cast<GOE_Character_Player*>(co->events.source);  // ダウンキャスト
    //                        if (goe != nullptr)
    //                        {
    //                            goe->SetOnajiCharaNum(onajiNamePl);
    //                            goe->SetCharaSubGraphic();
    //                        }

    //                        co->pos = {};
    //                        //co->pos.x = 0.0f + BLOCK_WIDTH * 3;
    //                        //co->pos.y = BLOCK_HIGHT * 36;

    //                        players[pObjIndex].obj = co;
    //                        players[pObjIndex].playerNo = pd[i].dat.playerdat.playerNo;
    //                    }
    //                }
    //            }
    //        }
    //    }
    //}


    for (int i = 0; i < PLAYER_MAX; i++)
    {
        PLAYER* player = players + i;
        GAME_OBJECT* playerObj = player->obj;
        if (playerObj != nullptr)
        {
            Character::OBJECT_DAT* pObjDat = Character::GetObjectDataCharacterGameObject(player->obj);
            if (pObjDat != nullptr)
            {
                if (GameManagerIsMovieFlag()
                    || GameManagerIsPlayerCtrlStop()
                    )
                {
                    playerObj->status.invFrame = 1;
                    pObjDat->inputDat = {};
                }
                else
                {
                    pObjDat->inputDat = GetControllerMainInputData(i);
                }
            }
        }
    }
}

// ===================================================
// プレイヤーの終了処理
// ===================================================
void UninitPlayer(void)
{
	for (int i = 0; i < PLAYER_MAX; i++)
	{ 
		if (players[i].obj != nullptr)
		{
			//プレイヤーの実体を削除
			DeleteGameObject(players[i].obj);
		}
	}
}

void SetPlayerLogInEnable(bool b)
{
    g_loginEnable = b;
}

bool GetPlayerLogInEnable()
{
    return g_loginEnable;
}

void ResetPlayers()
{
    g_PlayerTotalDamage = 0;

    //プレイヤーを定位置に移動する
    for (int i = 0; i < PLAYER_MAX; i++)
    {
        PLAYER* player = GetPlayer(i);
        GAME_OBJECT* playerObj = player->obj;
        if (playerObj != nullptr)
        {
            playerObj->status.hpMin = 0;
            playerObj->status.hp = playerObj->status.hpMax;
            playerObj->status.deadFlag = false;
            playerObj->pos = { 0.0f, BLOCK_HIGHT * 150.0f };

            playerObj->status.invFrame = 10;
            playerObj->hitStop = {};

            GOE_Character_Player* goe =
                dynamic_cast<GOE_Character_Player*>(playerObj->events.source);  // ダウンキャスト
            if (goe != nullptr)
            {
                goe->InitResetAction();
                goe->HpBarReset();
            }
        }
    }
}
void ResetActionPlayers()
{
    for (int i = 0; i < PLAYER_MAX; i++)
    {
        PLAYER* player = GetPlayer(i);
        GAME_OBJECT* playerObj = player->obj;
        if (playerObj != nullptr)
        {
            GOE_Character_Player* goe =
                dynamic_cast<GOE_Character_Player*>(playerObj->events.source);  // ダウンキャスト
            if (goe != nullptr)
            {
                goe->ResetAction();
            }
        }
    }
}

void TotalPlayerDamageAdd(int b)
{
    g_PlayerTotalDamage += b;
}

int GetTotalPlayerDamage()
{
    return g_PlayerTotalDamage;
}

// ===================================================
// プレイヤーの取得
// ===================================================
PLAYER* GetPlayer(int index)
{
	if (index < 0 || index >= PLAYER_MAX)
	{
		return nullptr;
	}
	return players + index;
}
PLAYER* GetPlayer(GAME_OBJECT* gobj)
{
	for (int i = 0; i < PLAYER_MAX; i++)
	{
		if (players[i].obj == gobj)
		{
			return players + i;
		}
	}
	return nullptr;
}

int GetActivePlayerNum()
{
    int n = 0;
    for (int i = 0; i < PLAYER_MAX; i++)
    {
        if (IsActiveGameObject(players[i].obj))
        {
            n++;
        }
    }
    return n;
}

int GetAlivePlayerNum()
{
    int n = 0;
    for (int i = 0; i < PLAYER_MAX; i++)
    {
        if (IsActiveGameObject(players[i].obj)
            && !IsDefeatGameObject(players[i].obj))
        {
            n++;
        }
    }
    return n;
}


std::vector<PLAYER> GetPlayesrFindByDistance(Float3 startPos, float distance)
{
	std::vector<PLAYER> players = {};

	for (int i = 0; i < PLAYER_MAX; i++)
	{
		PLAYER* player = GetPlayer(i);
		if (player != nullptr && player->obj != nullptr)
		{
			float distanceTmp = CalculateDistance(startPos, player->obj->pos);
			if (distanceTmp <= distance)
			{
				players.push_back(*player);
			}
		}
	}

	//std::sort(players.begin(), players.end(),
	//	[](const PLAYER& a, const PLAYER& b) {
	//		return a.;
	//	}
	//);
	return players;
}

void SetPlayerGameoverDecision()
{
    for (int i = 0; i < PLAYER_MAX; i++)
    {
        PLAYER* player = GetPlayer(i);
        if (player != nullptr && player->obj != nullptr)
        {
            GOE_Character_Player* goe =
                dynamic_cast<GOE_Character_Player*>(player->obj->events.source);  // ダウンキャスト

            if (goe != nullptr)
            { //
                goe->SetGameoverDecision(true);
            }
        }
    }
}

//PLAYER* GetPlayerOpponent(PLAYER* plyr)
//{
//	for (int i = 0; i < PLAYER_MAX; i++)
//	{
//		if ((players + i) == plyr)
//		{
//			if (i == 0)
//			{
//				return players + 1;
//			}
//			if (i == 1)
//			{
//				return players + 0;
//			}
//		}
//	}
//	return nullptr;
//}


////////////////////////////////////////////////////
////
//// プレイヤー個々の処理
//// ゲームオブジェクトに設定するイベント
////
////
//
//// ===================================================
//// プレイヤーオブジェクトの初期化
//// ===================================================
//void GOE_InitPlayerObject(int index)
//{
//}
//
//void GOE_FirstFramePlayerObject(int index)
//{
//	GAME_OBJECT* playerObj = GetGameObject(index);
//	PLAYER* player = GetPlayer(playerObj);
//
//	//DebugPrintf("%d\n", player);
//	//DebugPrintf("%d\n", players);
//	//DebugPrintf("%d", (int)(player - players));
//
//	ShowHpbar((int)(player - players), true);
//
//	//テスト
//	//ShowBossbar(index, true);
//
//	//DebugPrintf("GOE_InitPlayerObject");
//}
//
//// ===================================================
//// プレイヤーオブジェクトの終了
//// ===================================================
//void GOE_UninitPlayerObject(int index)
//{
//	GAME_OBJECT* playerObj = GetGameObject(index);
//	PLAYER* player = GetPlayer(playerObj);
//
//	//ShowHpbar((int)(player - players), false);
//
//	//DebugPrintf("GOE_UninitPlayerObject");
//}
//
//// ===================================================
//// プレイヤーオブジェクトの更新
//// ===================================================
//void GOE_UpdatePlayerObject(int index)
//{
//	GAME_OBJECT* playerObj = GetGameObject(index);
//	PLAYER* player = GetPlayer(playerObj);
//	if (playerObj->status.deadFlag && !player->gameoverflag)
//	{
//		player->gameoverflag = true;
//
//		//GameStopRequest(9999);
//		//SetSceneFade(SCENE_RESULT);
//	}
//
//	if (!IsCameraInGameObject(playerObj))
//	{
//		CAMERA cinfo = GetCameraInfo();
//
//		//playerObj->pos.x += (cinfo.trans.pos.x - playerObj->pos.x) / 24.0f;
//		//playerObj->pos.y += (cinfo.trans.pos.y - playerObj->pos.y) / 24.0f;
//		//GameObjectMoveRequest(playerObj, 30, MkF3(cinfo.trans.pos.x, cinfo.trans.pos.y, 0.0f), EasingInOutSine);
//	}
//}
//
//// ===================================================
//// プレイヤーオブジェクトの更新(アクティブ)
//// ===================================================
//void GOE_UpdateActivePlayerObject(int index)
//{
//	GAME_OBJECT* playerObj = GetGameObject(index);
//	PLAYER* player = GetPlayer(playerObj);
//	int playerIndex = player->index;
//
//	GAME_OBJECT_STATUS* playerStatus = &playerObj->status;
//
//	SPRITE_ANIM_DAT* playerSpriteAnim = playerObj->graphic.graph.spriteAnim;
//
//	if (playerObj->status.deadFlag)
//	{ //やられたら実行しない
//		return;
//	}
//
//	// 十字キー移動
//	float moveModTmp = 1.0f;
//	//if (GetControllerMainPress(CONTROLLER_BUTTON_TYPE_Y))
//	//{
//	//	moveModTmp = 1.8f;
//	//}
//
//	bool isMoveControl = false;
//	bool isJuujikey = false;
//
//	Float2 stickAngle = {};
//	Float3 moveVel = playerObj->vel;
//
//	{ //移動
//		Float2 slickL = GetControllerMainLeftStick(playerIndex);
//		bool isMoving = false;
//		if (slickL.x != 0.0f || slickL.y != 0.0f)
//		{
//			moveVel.x += slickL.x * PLAYER_MOVE_AXCEL * moveModTmp;
//			moveVel.y += -slickL.y * PLAYER_MOVE_AXCEL * moveModTmp;
//
//			stickAngle = slickL;
//
//			isMoving = true;
//			isMoveControl = true;
//		}
//		else
//		{
//			isJuujikey = true;
//			if (GetControllerMainPress(CONTROLLER_BUTTON_TYPE_LEFT, playerIndex))
//			{
//				moveVel.x -= PLAYER_MOVE_AXCEL * moveModTmp;
//
//				isMoving = true;
//				isMoveControl = true;
//
//				stickAngle.x = -1.0f;
//			}
//			if (GetControllerMainPress(CONTROLLER_BUTTON_TYPE_RIGHT, playerIndex))
//			{
//				moveVel.x += PLAYER_MOVE_AXCEL * moveModTmp;
//
//				isMoving = true;
//				isMoveControl = true;
//
//				stickAngle.x = 1.0f;
//			}
//			if (GetControllerMainPress(CONTROLLER_BUTTON_TYPE_UP, playerIndex))
//			{
//				moveVel.y -= PLAYER_MOVE_AXCEL * moveModTmp;
//
//				isMoving = true;
//				isMoveControl = true;
//				stickAngle.y = 1.0f;
//			}
//			if (GetControllerMainPress(CONTROLLER_BUTTON_TYPE_DOWN, playerIndex))
//			{
//				moveVel.y += PLAYER_MOVE_AXCEL * moveModTmp;
//
//				isMoving = true;
//				isMoveControl = true;
//				stickAngle.y = -1.0f;
//			}
//		}
//
//		if (isMoving)
//		{
//			player->angle = stickAngle;
//
//			playerObj->vel = moveVel;
//		}
//
//		//反転
//		//playerObj->graphic.graph.reverse = player->reverse;
//	}
//
//	{ // ジャンプ
//		CONTROLLER_BUTTON_TYPE jumpKey = CONTROLLER_BUTTON_TYPE_B;
//		CONTROLLER_BUTTON_TYPE jumpKey2 = CONTROLLER_BUTTON_TYPE_X;
//		bool jumpKeyTrigger = GetControllerMainTrigger(jumpKey, playerIndex) || GetControllerMainTrigger(jumpKey2, playerIndex);
//		bool jumpKeyPress = GetControllerMainPress(jumpKey, playerIndex) || GetControllerMainPress(jumpKey2, playerIndex);
//
//		if (player->jumpF >= 1)
//		{
//			playerObj->gravityVel = 0.0f; //ジャンプ中重力を無効
//			playerObj->velImpact.y =
//				-((float)PLAYER_JUMP_FORCE
//					- (float)PLAYER_JUMP_FRICTION * ((float)player->jumpF )) ;
//			if (playerObj->velImpact.y > 0)
//			{
//				playerObj->velImpact.y = 0;
//			}
//			player->jumpF++;
//
//			if ((player->jumpF > PLAYER_JUMP_FRAME_MIN && !jumpKeyPress)
//				|| (player->jumpF > PLAYER_JUMP_FRAME_MAX ))
//			{
//				player->jumpF = 0;
//			}
//		}
//		else
//		{
//			if (playerObj->isGround && jumpKeyTrigger)
//			{
//				//SoundEffectPlayRequest(SOUND_TYPE_PLAYER_SHOOT_SMALL, 0.8f, 0);
//				player->jumpF = 1;
//			}
//		}
//		//playerObj->friction = PLAYER_FRICTION * playerChargingSlow;
//	}
//	// 重力
//	//player->vel.y += PLAYER_GRAVITY;
//
//	// 抵抗力
//	//playerObj->vel.x += -1 * playerObj->vel.x * PLAYER_FRICTION;
//	//playerObj->vel.y += -1 * playerObj->vel.y * PLAYER_FRICTION;
//}
//
//void GOE_DeadPlayerObject(int index)
//{
//	GAME_OBJECT* playerObj = GetGameObject(index);
//	PLAYER* player = GetPlayer(playerObj);
//
//	{
//		//GameStopRequest(9999);
//		GameManagerSetSceneRequest((int)SCENE_RESULT, 0, false);
//
//		playerObj->status.invFrame = 0;
//		playerObj->hitReaction = {};
//		playerObj->status.guardType = GAME_OBJECT_GUARD_TYPE_INV;
//
//		
//		{
//		}
//
//		SoundBGMStopRequest();
//		GameStopRequest(90);
//		CameraFlashRequest(40, nullptr, MkF4(10.0f, 0.0f, 0.0f, 0.5f));
//		CameraShakeRequest(CAMERA_SHAKE_LARGE_FRAME, CAMERA_SHAKE_LARGE_POWER);
//		CameraZoomRequest(0.05f, index, EasingInSine);
//
//		//SoundEffectPlayRequest(SOUND_TYPE_PLAYER_CLASH, 0.8f, 0);
//	}
//}
//
//void GOE_DamageAttackerPlayerObject(int index, GAME_OBJECT_DAMAGE_RESULT dr)
//{
//}
//
//void GOE_DamageRecieverPlayerObject(int index, GAME_OBJECT_DAMAGE_RESULT dr)
//{
//	GAME_OBJECT* playerObj = GetGameObject(index);
//	PLAYER* player = GetPlayer(playerObj);
//
//	if (dr.damage > 0)
//	{
//		CameraShakeRequest(CAMERA_SHAKE_MIDIUM_FRAME, CAMERA_SHAKE_MIDIUM_POWER);
//		//SoundEffectPlayRequest(SOUND_TYPE_PLAYER_DAMAGE, 1.0f, 0);
//
//	}
//}
//
//
//void GOE_DrawPlayerObject(int index)
//{
//}
//
//
////
//// プレイヤーのたま
////
//void GOE_InitPlayerBulletObject(int index)
//{
//	GAME_OBJECT* obj = GetGameObject(index);
//}
//
//void GOE_FirstFramePlayerBulletObject(int index)
//{
//	GAME_OBJECT* obj = GetGameObject(index);
//}
//
//void GOE_UninitPlayerBulletObject(int index)
//{
//	GAME_OBJECT* obj = GetGameObject(index);
//}
//
//void GOE_UpdatePlayerBulletObject(int index)
//{
//	GAME_OBJECT* obj = GetGameObject(index);
//
//	float radtmp = atan2(obj->vel.y, obj->vel.x);
//	float degtmp = CalculateRadToDeg(radtmp);
//
//	obj->graphic.graph.spriteAnim[0].rot = degtmp;
//}
//
//void GOE_UpdateActivePlayerBulletObject(int index)
//{
//	GAME_OBJECT* obj = GetGameObject(index);
//
//
//
//	float radtmp = atan2(obj->vel.y, obj->vel.x);
//	float degtmp = CalculateRadToDeg(radtmp);
//
//	//if (CheckBoxCollider(CameraPosToWorldPos(MkF2(0.0f, 0.0f)), MkF2(obj->pos.x, obj->pos.y),
//	//	GetCameraSize(), MkF2(obj->size.x, obj->size.y)))
//	if(IsCameraInGameObject(obj))
//	{
//	}
//
//}
//
//
//void GOE_DamageAttackerPlayerBulletObject(int index, GAME_OBJECT_DAMAGE_RESULT dr)
//{
//	GAME_OBJECT* obj = GetGameObject(index);
//
//
//	if (dr.damage > 0)
//	{
//
//	}
//	DeleteGameObject(index);
//
//}
//
//void GOE_DamageRecieverPlayerBulletObject(int index, GAME_OBJECT_DAMAGE_RESULT dr)
//{
//	GAME_OBJECT* obj = GetGameObject(index);
//}
//
//
//void GOE_BlockHitPlayerBulletObject(int index, GAME_OBJECT_POSMOD_RESULT pmr)
//{
//	GAME_OBJECT* obj = GetGameObject(index);
//
//	{
//		GAME_OBJECT* go = GO_CreateEffect(GO_EFFECT_TYPE_SHOCKWAVE, obj->pos);
//		if (go != nullptr)
//		{
//			go->scale = MkF2(0.5f, 0.5f);
//			GameObjectSetLifeTime(go, 16);
//		}
//	}
//	DeleteGameObject(index);
//}
































//// ???
//
///*
//
//// ===================================================
//// player.cpp プレイヤー制御
////
//// ===================================================
//#include "main.h"
//#include "sprite.h"
//#include "texture.h"
//#include "player.h"
//#include "controller.h"
//#include "block.h"
//#include "collision.h"
//
//// ===================================================
//// マクロ定義
//// ===================================================
//#define	PLAYER_MOVE_AXCEL (1.5f)	//加速度
////#define	PLAYER_MOVE_SPEED (20.0f)
//#define	PLAYER_JUMP_FORCE (-30.0f)	//ジャンプ力
//#define	PLAYER_GRAVITY		(1.8f)	//重力加速度
//#define	PLAYER_FRICTION		(0.15f)	//摩擦力
//
//// ===================================================
//// プロトタイプ宣言
//// ===================================================
//void playerPosModification(void);
//
//
//// ===================================================
//// グローバル変数
//// ===================================================
//PLAYER player;	// プレイヤー実体
//unsigned int PlayerTextureId;	// プレイヤーテクスチャID
//
//// ===================================================
//// プレイヤーの初期化
//// ===================================================
//void InitPlayer(void)
//{
//	player.pos = MakeFloat2(0.0f, 0.0f);
//	player.oldpos = MakeFloat2(0.0f, 0.0f);
//	player.vel = MakeFloat2(0.0f, 0.0f);
//	player.size = MakeFloat2(96.0f, 96.0f);
//	player.jump = false;
//	player.use = true;
//
//	// テクスチャの読み込み
//	PlayerTextureId = LoadTexture("rom:/texture1.tga");
//}
//
//// ===================================================
//// プレイヤーの更新
//// ===================================================
//void UpdatePlayer(void)
//{
//	if (player.use)
//	{
//		// 変更前の座標を格納
//		player.oldpos = player.pos;
//
//
//
//		// 十字キー移動
//
//		float moveModTmp = 1.0f;
//		if (GetControllerPress(NpadButton::Y::Index))
//		{
//			moveModTmp = 1.8f;
//		}
//
//		Float2 slickL = GetControllerLeftStick();
//		if (slickL.x != 0.0f || slickL.y != 0.0f)
//		{
//			player.vel.x +=  slickL.x * PLAYER_MOVE_AXCEL * moveModTmp;
//			player.vel.y += -slickL.y * PLAYER_MOVE_AXCEL * moveModTmp;
//		}
//		else
//		{
//			if (GetControllerPress(NpadButton::Left::Index))
//			{
//				player.vel.x -= PLAYER_MOVE_AXCEL * moveModTmp;
//			}
//			if (GetControllerPress(NpadButton::Right::Index))
//			{
//				player.vel.x += PLAYER_MOVE_AXCEL * moveModTmp;
//			}
//			if (GetControllerPress(NpadButton::Up::Index))
//			{
//				player.vel.y -= PLAYER_MOVE_AXCEL * moveModTmp;
//			}
//			if (GetControllerPress(NpadButton::Down::Index))
//			{
//				player.vel.y += PLAYER_MOVE_AXCEL * moveModTmp;
//			}
//		}
//
//		// ジャンプ
//		/*bool oldjump = player.jump;
//
//		if (GetControllerTrigger(NpadButton::B::Index))
//		{
//			if (!player.jump)
//			{
//				player.vel.y = PLAYER_JUMP_FORCE;
//				player.jump = true;
//
//			}
//		}*/
//
//		// 重力
//		//player.vel.y += PLAYER_GRAVITY;
//
//		// 抵抗力
//player.vel.x += -1 * player.vel.x * PLAYER_FRICTION;
//player.vel.y += -1 * player.vel.y * PLAYER_FRICTION;
//
//// 移動
//player.pos.x += player.vel.x;
//playerPosModification();
//
//player.pos.y += player.vel.y;
//playerPosModification();
//
//
//	}
//}
//
////
//// プレイヤーの座標変更後 位置の修正を行う関数
////
//void playerPosModification(void)
//{
//	// ブロックの先頭アドレスを取得
//	BLOCK* pBlock = GetBlocks();
//
//	// ブロックの個数分チェック
//	for (int BlockCnt = 0; BlockCnt < BLOCK_MAX; BlockCnt++)
//	{
//		BLOCK* pBlockThis = &pBlock[BlockCnt];
//
//		if (pBlockThis->isUse)
//		{
//			// 上下左右の当たり方向を検出するため、oldを用いたチェックを行う
//			// playerの4辺を計算
//			float PlayerTop = player.pos.y - player.size.y / 2;
//			float PlayerBottom = player.pos.y + player.size.y / 2;
//			float PlayerRight = player.pos.x + player.size.x / 2;
//			float PlayerLeft = player.pos.x - player.size.x / 2;
//
//			// playerのoldposの4辺を計算
//			float PlayerOldTop = player.oldpos.y - player.size.y / 2;
//			float PlayerOldBottom = player.oldpos.y + player.size.y / 2;
//			float PlayerOldRight = player.oldpos.x + player.size.x / 2;
//			float PlayerOldLeft = player.oldpos.x - player.size.x / 2;
//
//
//			// blockの4辺を計算
//			float BlockTop = pBlockThis->pos.y - pBlockThis->size.y / 2;
//			float BlockBottom = pBlockThis->pos.y + pBlockThis->size.y / 2;
//			float BlockRight = pBlockThis->pos.x + pBlockThis->size.x / 2;
//			float BlockLeft = pBlockThis->pos.x - pBlockThis->size.x / 2;
//
//
//			// 当たっている（上下判定用）
//			if (CheckBoxCollider(player.pos, pBlockThis->pos, player.size, pBlockThis->size))
//			{
//
//				// 下から上に当たった
//				if (PlayerOldTop >= BlockBottom && PlayerTop <= BlockBottom)
//				{// プレイヤーの上とブロックの下を比べ、プレイヤーの上がoldの時は下側にあり、現在は上側にある場合
//					// 止める
//					player.vel.y = 0.0f;
//
//					// プレイヤーの場所を固定
//					player.pos.y = BlockBottom + player.size.y / 2 + 1;
//
//				}
//
//				// 上から下に当たった（乗った）
//				if (PlayerOldBottom <= BlockTop && PlayerBottom >= BlockTop)
//				{// プレイヤーの下とブロックの上を比べ、プレイヤーの下がoldの時は上側にあり、現在は下側にある場合
//					// 着地
//					player.vel.y = 0.0f;
//
//					// プレイヤーの場所を固定（地面に引っかからないようにするため1.0fだけ上げる）
//					player.pos.y = BlockTop - player.size.y / 2 - 1.0f;
//
//					// 着地した場合ジャンプフラグを折る
//					/*if (oldjump)
//					{
//						player.jump = false;
//					}*/
//				}
//			}
//
//			// 当たっている（左右判定用）
//			if (CheckBoxCollider(player.pos, pBlockThis->pos, player.size, pBlockThis->size))
//			{
//				// 左から右に当たった
//				if (PlayerOldRight <= BlockLeft && PlayerRight >= BlockLeft)
//				{// プレイヤーの右とブロックの左を比べ、プレイヤーの右がoldの時は左側にあり、現在は右側にある場合
//					// 止める
//					player.vel.x = 0.0f;
//
//					// プレイヤーの場所を固定 
//					player.pos.x = BlockLeft - player.size.x / 2 - 1;
//				}
//
//				// 右から左に当たった
//				if (PlayerOldLeft >= BlockRight && PlayerLeft <= BlockRight)
//				{// プレイヤーの左とブロックの右を比べ、プレイヤーの左がoldの時は右側にあり、現在は左側にある場合
//					// 止める
//					player.vel.x = 0.0f;
//
//					// プレイヤーの場所を固定 
//					player.pos.x = BlockRight + player.size.x / 2 + 1;
//				}
//			}
//
//		}
//	}
//}
//
//
//
//
//// ===================================================
//// プレイヤーの描画
//// ===================================================
//void DrawPlayer(void)
//{
//	if (player.use)
//	{
//		DrawSpriteQuad(
//			player.pos.x, player.pos.y,
//			player.size.x, player.size.y,
//			PlayerTextureId
//		);
//	}
//}
//
//// ===================================================
//// プレイヤーの終了処理
//// ===================================================
//void UninitPlayer(void)
//{
//	UnloadTexture(PlayerTextureId);
//}
//
//// ===================================================
//// プレイヤーの取得
//// ===================================================
//PLAYER* GetPlayer(void)
//{
//	return &player;
//}
//
//*/