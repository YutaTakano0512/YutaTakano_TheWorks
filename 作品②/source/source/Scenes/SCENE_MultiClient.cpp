// ===================================================
// SCENE_MultiClient.cpp マルチ クライアント制御
// 
// hara sougo  2024/12/15
// ===================================================
#include "../game_main.h"

#include "SCENE_MultiClient.h"

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

#include "../Core/Multi/winsock.h"
#include "../Core/Multi/winsock_server.h"
#include "../Core/Multi/winsock_client.h"

void InitializeSceneMultiClient(void) {}
void UpdateSceneMultiClient(void) {}
void DrawSceneMultiClient(void) {}
void FinalizeSceneMultiClient(void) {}
//// ===================================================
//// 構造体宣言
//// ===================================================
//
//static constexpr int STAGE_MAIN_WAIT_END_FADE_MCT = 60;
//
//static bool g_ModoruFlag = false;
//static int g_TutorialOutPushBtn_mct = 0;
//
//static TEX_DAT g_Tex_menuback = {};
//static TEX_DAT g_Tex_waitplease = {};
//
//// ===================================================
//// シーンの初期化
//// ===================================================
//void InitializeSceneMultiClient(void)
//{
//    //既存のブロックとオブジェクトを消す
//    DeleteBlockAll();
//    DeleteGameObjectAll();
//    SetBg(BG_TYPE_NONE);
//
//    WinSockClient_DrawBufClear();
//    //プレイヤーの情報を送信（参加リクエスト）
//    WinSockClientSend_PlayerDat();
//
//    g_ModoruFlag = false;
//    g_TutorialOutPushBtn_mct = 0;
//
//    g_Tex_menuback = LoadTexData("TEX/ui/in_game_master/stagebefore/back");
//    g_Tex_waitplease = LoadTexData("TEX/ui/in_game_master/stagebefore/hostwait");
//
//	(void)_heapmin();
//}
//
//// ===================================================
//// シーンの更新
//// ===================================================
//void UpdateSceneMultiClient(void)
//{
//#ifdef SWITCH_MODE
//#else
//
//	//// シーン遷移(デバッグテスト用)
//	//if (GetAsyncKeyState(VK_DELETE))
//	//{
//	//	SetScene(SCENE_TITLE);
//	//}
//
//#endif
//
//
//    WINSOCK_SERVER_CLIENT_SEND_SERVER_STATE serverState
//        = WinSockClientGetServerState();
//
//    if (!g_ModoruFlag)
//    {
//        if (serverState.scene == SCENE_GAME
//            && serverState.playerLogInFlag
//        )
//        {
//            ////戻る
//            //if (GetControllerMainTrigger(CONTROLLER_BUTTON_TYPE_L))
//            //{
//            //    g_ModoruFlag = true;
//
//            //    //プレイヤーの情報を送信（去るリクエスト）
//            //    WinSockClientSend_PlayerLeave();
//            //    SetSceneFade(SCENE_CHARACTERSELECT);
//            //}
//            
//            //戻る
//            if (GetControllerMainPress(CONTROLLER_BUTTON_TYPE_L))
//            {
//                g_TutorialOutPushBtn_mct++;
//                if (g_TutorialOutPushBtn_mct > STAGE_MAIN_WAIT_END_FADE_MCT)
//                {
//                    g_ModoruFlag = true;
//
//                    //プレイヤーの情報を送信（去るリクエスト）
//                    WinSockClientSend_PlayerLeave();
//                    SetSceneFade(SCENE_CHARACTERSELECT);
//                }
//            }
//            else
//            {
//                g_TutorialOutPushBtn_mct = 0;
//            }
//        }
//    }
//}
//
//// ===================================================
//// シーンの描画
//// ===================================================
//void DrawSceneMultiClient(void)
//{
//    WINSOCK_SERVER_CLIENT_SEND_SERVER_STATE serverState
//        = WinSockClientGetServerState();
//
//    if (serverState.scene == SCENE_GAME
//        && serverState.playerLogInFlag
//        )
//    {
//        DrawSpriteInCamera(
//            //表示座標 (四角形の真ん中)
//            -SCREEN_WIDTH / 2 + 150.0f,
//            -SCREEN_HEIGHT / 2 + 320.0f,
//
//            //サイズ width hight
//            260.0f,
//            260.0f * ((float)g_Tex_menuback.height / g_Tex_menuback.width),
//
//            //角度
//            0.0f,
//
//            //色
//            MkF4(1.0f, 1.0f, 1.0f, 1.0f),
//
//            g_Tex_menuback.textureId,  //画像(テクスチャ)の番号
//            MkF2(0.0f, 0.0f),
//            MkF2(1.0f, 1.0f)
//        );
//        DrawSpriteInCamera(
//            //表示座標 (四角形の真ん中)
//            -SCREEN_WIDTH / 2 + 150.0f,
//            -SCREEN_HEIGHT / 2 + 360.0f,
//
//            //サイズ width hight
//            1.0f * 260.0f * std::min((float)g_TutorialOutPushBtn_mct / STAGE_MAIN_WAIT_END_FADE_MCT, 1.0f),
//            10.0f,
//
//            //角度
//            0.0f,
//
//            //色
//            MkF4(2.0f, 2.0f, 2.0f, 0.75f),
//
//            0,  //画像(テクスチャ)の番号
//            MkF2(0.0f, 0.0f),
//            MkF2(1.0f, 1.0f)
//        );
//        DrawSpriteInCamera(
//            //表示座標 (四角形の真ん中)
//            80.0f,
//            SCREEN_HEIGHT / 2 - 80.0f,
//
//            //サイズ width hight
//            700.0f,
//            700.0f,
//
//            //角度
//            0.0f,
//
//            //色
//            MkF4(1.0f, 1.0f, 1.0f, 1.0f),
//
//            g_Tex_waitplease.textureId,  //画像(テクスチャ)の番号
//            MkF2(0.0f, 0.0f),
//            MkF2(1.0f, 1.0f)
//        );
//    }
//}
//
//// ===================================================
//// シーンの終了処理
//// ===================================================
//void FinalizeSceneMultiClient(void)
//{
//    //WinSockClient_End();
//
//	Character::CharacterMainDataClearCache();
//
//    //既存のブロックとオブジェクトを消す
//	DeleteBlockAll();
//	DeleteGameObjectAll();
//
//	(void)_heapmin();
//}
