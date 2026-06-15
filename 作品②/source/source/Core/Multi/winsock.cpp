
//---------------------------------------------------------------------------
// winsock.cpp
// 
// sougo hara  2024/12/15
//---------------------------------------------------------------------------
#include "winsock.h"

#include <iostream>

// inet_addr()関数で警告が出る場合は以下で警告を無効化する。
//#pragma warning(disable:4996) 

#include "../../Game/Configuration/configuration.h"
#include "../../main.h"

#include "../../game_main.h"


//グロ－バル変数

static SOCKET g_Sock = {}; //
static bool g_IsEnable = false;
static WINSOCK_CONNECT_TYPE  g_ConnectType = WINSOCK_CONNECT_TYPE::OFFLINE;

//
// 初期化
//
void WinSockInitialize(void)
{
    WSAData wsaData;
    // WinSockの初期化処理(Version 2.0)
    if (WSAStartup(MAKEWORD(2, 0), &wsaData) != 0) {
        MessageBox(NULL, std::string("Winsockの初期化失敗(WSAStartup)").c_str(), " ", MB_OK);
        return;
    }

    // socket作成
    // socketは通信の出入り口 ここを通してデータのやり取りをする
    // socket(アドレスファミリ, ソケットタイプ, プロトコル)
    g_Sock = socket(AF_INET, SOCK_DGRAM, 0);  //AF_INETはIPv4、SOCK_DGRAMはUDP通信
    if (g_Sock == INVALID_SOCKET) 
    {
        MessageBox(NULL, std::string("Socket creation failed.").c_str(), " ", MB_OK);
        WSACleanup();
        return;
    }

    // ブロッキング、ノンブロッキングの設定
    //（任意、必須コードではない（ソケットの初期設定はブロッキングモードなため）
    // val = 0 : ブロッキングモード データが受信されるまで待機
    // val = 1 : ノンブロッキング データが受信されなくても次の処理へ
    u_long val = 1;
    ioctlsocket(g_Sock, FIONBIO, &val);

    int buffSize = WINSOCK_BUF_SIZE;
    int ret = ::setsockopt(g_Sock, SOL_SOCKET, SO_RCVBUF, (const char*)&buffSize, sizeof(int));
    if (ret != 0) {
        MessageBox(NULL, std::string("割り当て失敗").c_str(), " ", MB_OK);
    }


    g_IsEnable = true;
}

//
// 終了処理
//
void WinSockFinalize(void)
{
    g_IsEnable = false;

    //for (int i = 0; i < g_Thread.size(); i++)
    //{
    //    g_Thread[i]->Terminate();
    //}

    // socketの破棄
    closesocket(g_Sock);

    // winsockの終了
    WSACleanup();
}


bool WinSockIsEnable()
{
    return g_IsEnable;
}

SOCKET* WinSockGetSock()
{
    return &g_Sock;
}

WINSOCK_CONNECT_TYPE WinSockGetType()
{
    return g_ConnectType;
}
void WinSockSetType(WINSOCK_CONNECT_TYPE type)
{
    g_ConnectType = type;
}
