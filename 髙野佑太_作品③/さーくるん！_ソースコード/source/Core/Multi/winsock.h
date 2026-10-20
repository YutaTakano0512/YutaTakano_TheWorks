//---------------------------------------------------------------------------
// winsock.h
// 
// sougo hara  2024/12/15
//---------------------------------------------------------------------------
#ifndef _MYWINSOCK_H_
#define _MYWINSOCK_H_

#include <winsock2.h>
#include <ws2tcpip.h>

//リンカ > 入力 > 追加する依存関係」にws2_32.libを追加
#pragma comment (lib, "ws2_32.lib")

#include "../../Game/Systems/sys_general.h"

#include "../Input/controller_main.h"
#include "../../Game/texture/texData.h"
#include "../../Game/Systems/camera.h"
#include "../../Game/Managers/scene.h"
#include "../../Game/Game Events/GE_Gaya.h"

#include <thread>

////仮
//static constexpr int WINSCK_SET_PORT = 12345;
//
//constexpr int WINSCK_CLIENT_MAX = (SYS_PLAYER_MAX - 1);
//
//constexpr int WINSOCK_SERVER_CLIENT_SEND_TEX_DATA_LOAD_MAX = TEX_DATA_LOAD_BUFF_MAX;
//constexpr int WINSOCK_SERVER_CLIENT_SEND_CAMERA_BUF_MAX = CAMERA_DRAW_BUF_MAX;
//
////constexpr int WINSOCK_BUF_SIZE = (1024 * 1024) * 16; //16MB
constexpr int WINSOCK_BUF_SIZE = (1024 * 1024) * 32; //32MB
//
//
enum class WINSOCK_CONNECT_TYPE : char
{
    OFFLINE = 0,

    SERVER,

    CLIENT,
};

//enum class WINSOCK_RECIV_FLAG : char
//{
//    CONNECT = 0, //接続
//    CONNECT_TEST, //接続テスト
//
//    INPUT_DAT, //入力情報
//
//    CAMERA_DAT, //カメラ情報,
//
//    TEX_LOAD, //テクスチャ読み込み
//
//    GAYA_REQUEST, //ガヤ
//
//    PLAYER_DAT, //プレイヤーの情報
//    PLAYER_DAT_CLEAR, //プレイヤーの情報クリア
//    PLAYER_DAT_CLIENT_LEAVE, //プレイヤーの情報クリア
//
//    SERVER_STATE, //鯖のステート
//
//    SERVER_CLOSE, //鯖を閉じる
//};
//
////
//// 構造体宣言
////
//struct WINSOCK_SERVER_CLIENTCONNECTED { //クライアントがサーバーに接続できたとき
//    int index;
//};
//struct WINSOCK_SERVER_CLIENTCONNECTED_REPLY { //クライアントがサーバーに接続できたとき
//    WINSOCK_SERVER_CLIENTCONNECTED cdat;
//    INT dummy;
//};
////struct WINSOCK_SERVER_CLIENT_SEND_INFO {
////    WINSOCK_RECIV_FLAG flag;
////    int dataSize;
////    WINSOCK_SERVER_CLIENTCONNECTED connectDat;
////};
//
////struct WINSOCK_SERVER_CLIENT_SEND_INPUTDAT { //クライアントがサーバーの通信で使う
////    WINSOCK_SERVER_CLIENT_SEND_INFO info;
////    CONTROLLER_INPUT_DAT inputdat;
////};
////
////struct WINSOCK_SERVER_CLIENT_SEND_CAMERADAT { //クライアントがサーバーの通信で使う
////    WINSOCK_SERVER_CLIENT_SEND_INFO info;
////    CAMERA_DRAW_BUF buf[WINSOCK_SERVER_CLIENT_SEND_CAMERA_BUF_MAX];
////};
//
//struct WINSOCK_SERVER_CLIENT_SEND_INFO {
//    WINSOCK_RECIV_FLAG flag;
//    WINSOCK_SERVER_CLIENTCONNECTED connectDat;
//};
//
//struct WINSOCK_SERVER_CLIENT_SEND_TEX_DATA_LOAD_BUF {
//    int bufSize;
//    TEX_DAT_LOAD_BUFF buf[WINSOCK_SERVER_CLIENT_SEND_TEX_DATA_LOAD_MAX];
//};
//struct WINSOCK_SERVER_CLIENT_SEND_CAMERA_DRAW_BUF {
//    DX_CAMERA_INFO info;
//    int postEffect;
//
//    int bufSize;
//    CAMERA_DRAW_BUF buf[WINSOCK_SERVER_CLIENT_SEND_CAMERA_BUF_MAX];
//};
//struct WINSOCK_SERVER_CLIENT_SEND_PLAYER_DAT {
//    int playerNo;
//};
//
//struct WINSOCK_SERVER_CLIENT_SEND_SERVER_STATE {
//    SCENE scene;
//    bool playerLogInFlag;
//};
//struct WINSOCK_SERVER_CLIENT_SEND { //クライアントがサーバーの通信で使う
//    WINSOCK_SERVER_CLIENT_SEND_INFO info;
//    union
//    {
//        CONTROLLER_INPUT_DAT inputdat;
//        WINSOCK_SERVER_CLIENT_SEND_TEX_DATA_LOAD_BUF texDataLoadBuf;
//        WINSOCK_SERVER_CLIENT_SEND_CAMERA_DRAW_BUF cameraBuf;
//        GAYA_REQUEST_BUF gayaRequestBuf;
//        WINSOCK_SERVER_CLIENT_SEND_PLAYER_DAT playerdat;
//        WINSOCK_SERVER_CLIENT_SEND_SERVER_STATE serverState;
//    };
//
//    //WINSOCK_SERVER_CLIENT_SEND() {};
//    //~WINSOCK_SERVER_CLIENT_SEND() {};
//};

//
// プロトタイプ宣言
//
void WinSockInitialize(void);
void WinSockFinalize(void);

bool WinSockIsEnable();

SOCKET* WinSockGetSock();

WINSOCK_CONNECT_TYPE  WinSockGetType();
void  WinSockSetType(WINSOCK_CONNECT_TYPE type);

#endif