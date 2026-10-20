//---------------------------------------------------------------------------
// winsock_client.h
// 
// sougo hara  2024/12/15
//---------------------------------------------------------------------------
#ifndef _WINSOCK_CLIENT_H_
#define _WINSOCK_CLIENT_H_

#include "../../Game/Systems/sys_general.h"

//
// 構造体宣言
//

//
// プロトタイプ宣言
//

//クライアントの開始
void WinSockClient_Start(void);

void WinSockClient_Update(void);
void WinSockClient_Draw(void);
//クライアントの終了
void WinSockClient_End(void);

void WinSockClient_DrawBufClear();

void WinSockClient_ServerConnectCheck();

bool WinSockClientIsEnable(void);

//プレイヤーのデータを送る
void WinSockClientSend_PlayerDat(void);
//プレイヤーが去る
void WinSockClientSend_PlayerLeave(void);

struct WINSOCK_SERVER_CLIENT_SEND_SERVER_STATE;
WINSOCK_SERVER_CLIENT_SEND_SERVER_STATE WinSockClientGetServerState(void);

#endif