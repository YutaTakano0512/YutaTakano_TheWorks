//---------------------------------------------------------------------------
// winsock_server.h
// 
// sougo hara  2024/12/15
//---------------------------------------------------------------------------
#ifndef _WINSOCK_SERVER_H_
#define _WINSOCK_SERVER_H_

#include "../../Game/Systems/sys_general.h"

//
// 構造体宣言
//

//
// プロトタイプ宣言
//

//サーバーの開始
void WinSockServer_Start(void);

void WinSockServer_Update(void);
void WinSockServer_Draw(void);
void WinSockServer_End(void);

bool WinSockServerIsEnable(void);

void WinSockServerPlayerDatClear();

struct WINSOCK_SERVER_CLIENT_SEND;
struct WINSOCK_SERVER_SERVER_PLAYERDAT {
    //bool enable;
    //WINSOCK_SERVER_CLIENT_SEND dat;
};
std::vector<WINSOCK_SERVER_SERVER_PLAYERDAT> WinSockServerGetPlayerDat();

#endif