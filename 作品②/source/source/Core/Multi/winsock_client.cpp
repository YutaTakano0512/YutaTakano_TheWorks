
//---------------------------------------------------------------------------
// winsock_client.cpp
// 
// sougo hara  2024/12/15
//---------------------------------------------------------------------------

#include "winsock.h"
#include "winsock_client.h"

#include "../../Game/Configuration/configuration.h"
#include "../../main.h"

#include "../../game_main.h"

#include "../../Game/Managers/scene.h"
#include "../../Scenes/SCENE_Character_Select.h"

//クライアントの開始
void WinSockClient_Start(void)
{
}

void WinSockClient_Update(void)
{
}
void WinSockClient_Draw(void)
{
}
//クライアントの終了
void WinSockClient_End(void)
{
}

void WinSockClient_DrawBufClear()
{
}

void WinSockClient_ServerConnectCheck()
{
}

bool WinSockClientIsEnable(void)
{
    return false;
}

//プレイヤーのデータを送る
void WinSockClientSend_PlayerDat(void)
{
}
//プレイヤーが去る
void WinSockClientSend_PlayerLeave(void)
{
}

struct WINSOCK_SERVER_CLIENT_SEND_SERVER_STATE {};
WINSOCK_SERVER_CLIENT_SEND_SERVER_STATE WinSockClientGetServerState(void)
{
    return {};
}

//
////仮
////#include "../../Game/Systems/font.h"
//
////仮
//static std::string g_Addr = "";
//
//static std::thread* g_Excute = nullptr;
//static bool g_IsEnable = false;
//
////static bool g_TexDatLoadBufRecvFlag = false;
//static bool g_CameraDatRecvFlag = false;
//static bool g_CameraDatDrawFlag = false;
//static bool g_InputDatSendFlag = false;
//static bool g_PlayerDatSendFlag = false;
//static bool g_PlayerLeaveSendFlag = false;
//static bool g_ServerConnectCheckFlag = false;
//static bool g_EndFlag = false;
//
////static WINSOCK_SERVER_CLIENT_SEND sendDat_TexDatLoadBuf = {};
//static WINSOCK_SERVER_CLIENT_SEND sendDat_CameraDraw[2] = {};
//static bool sendDat_CameraDrawCurrentBuf = 0;
//
//static WINSOCK_SERVER_CLIENT_SEND_SERVER_STATE g_saveServerState = {};
//
//
//static void WinSockClient_Private_draw();
//static void client_execute();
//
//void WinSockClient_Start(void)
//{
//    if (!WinSockIsEnable())
//    {
//        return;
//    }
//    WinSockSetType(WINSOCK_CONNECT_TYPE::CLIENT);
//    g_EndFlag = false;
//
//    {
//        GAYA_REQUEST_OPT gayaopt = {};
//        //gayaopt.repdat.push_back({ L"%s", L"クライアント　オン" });
//        GE_GayaStartRequest("_system_general", 0, gayaopt);
//    }
//
//    {
//        std::string pathstr = "rom/serverAddr.txt";
//
//        // ファイルを開く
//        FILE* fp = nullptr;
//
//        fopen_s(&fp, pathstr.c_str(), "rb");
//
//        if (fp != nullptr)
//        {
//            char buf[128] = {};
//            while (fgets(buf, sizeof(buf), fp))
//            {
//                g_Addr = NormalizeString(buf);
//                break;
//            }
//
//            // ファイルを閉じる
//            fclose(fp);
//        }
//    }
//
//    //クライアントのスレッド処理
//    g_Excute = new std::thread(client_execute);
//    g_IsEnable = true;
//}
//
//void WinSockClient_Update(void)
//{
//    if (GetScene() == SCENE_MULTICLIENT)
//    {
//        g_InputDatSendFlag = true;
//    }
//    //client_execute();
//}
//void WinSockClient_Draw(void)
//{
//    //if (g_CameraDatRecvFlag)
//    //{
//    //    g_CameraDatRecvFlag = false;
//    //}
//    
//    //g_CameraDatDrawFlag = true;
//    WinSockClient_Private_draw();
//
//
//    g_CameraDatRecvFlag = false;
//}
//
//
//void WinSockClient_End(void)
//{
//    g_EndFlag = true;
//
//    while (
//        g_Excute != nullptr
//        )
//    { //終了待ち
//        Sleep(0);
//    }
//
//    WinSockSetType(WINSOCK_CONNECT_TYPE::OFFLINE);
//    g_IsEnable = false;
//}
//
//void WinSockClient_DrawBufClear()
//{
//    ZeroMemory(&sendDat_CameraDraw[0], sizeof(sendDat_CameraDraw[0]));
//    ZeroMemory(&sendDat_CameraDraw[1], sizeof(sendDat_CameraDraw[1]));
//}
//
//void WinSockClient_ServerConnectCheck()
//{
//    g_ServerConnectCheckFlag = true;
//}
//
//bool WinSockClientIsEnable(void)
//{
//    return g_IsEnable;
//}
//
//void client_execute()
//{
//    // アドレス等格納
//    struct sockaddr_in addr;
//    addr.sin_family = AF_INET;  //IPv4
//    addr.sin_port = htons(WINSCK_SET_PORT);   //通信ポート番号設定
//    addr.sin_addr.S_un.S_addr = inet_addr(g_Addr.c_str()); // 送信アドレスを設定
//
//    SOCKET* sock = WinSockGetSock();
//
//    //通信情報
//    WINSOCK_SERVER_CLIENTCONNECTED connectDat = {};
//    connectDat.index = -1;
//
//    { //初回通信
//        WINSOCK_SERVER_CLIENT_SEND_INFO* sendDat = new WINSOCK_SERVER_CLIENT_SEND_INFO;
//
//        sendDat->flag = WINSOCK_RECIV_FLAG::CONNECT;
//
//        sendto(*sock, (char*)sendDat, sizeof(*sendDat), 0, (struct sockaddr*)&addr, sizeof(addr));
//
//        delete sendDat;
//
//        DWORD stime = timeGetTime();
//
//        //通信情報を受け取る
//        while (WinSockIsEnable())
//        {
//            WINSOCK_SERVER_CLIENTCONNECTED_REPLY replyget = {};
//
//            int recvLen = recv(*sock, 
//                (char*)&replyget,
//                sizeof(replyget)
//                ,
//                0);
//            if (recvLen == SOCKET_ERROR)
//            { //失敗時
//                if ((stime + 10000) < timeGetTime())
//                {
//                    MessageBox(NULL, std::string("サーバーからの応答がありません").c_str(), " ", MB_OK);
//
//                    if (g_Excute != nullptr)
//                    {
//                        //delete g_Excute;
//                        g_Excute = nullptr;
//                    }
//
//                    if (GetScene() == SCENE_MULTICLIENT)
//                    {
//                        SetSceneFade(SCENE_TITLE);
//                    }
//
//                    //WinSockClient_End();
//                    WinSockSetType(WINSOCK_CONNECT_TYPE::OFFLINE);
//                    g_IsEnable = false;
//
//                    return;
//                }
//                //DebugPrintf("SOCKET_ERROR");
//            }
//            else
//            {
//                connectDat = replyget.cdat;
//                break;
//            }
//        }
//        //int recvLen = recv(*sock, (char*)&connectDat, sizeof(connectDat), 0);
//        //if (recvLen == SOCKET_ERROR)
//        //{ //失敗時
//        //    MessageBox(NULL, std::string("クライアント: そけっとえらー  ").c_str(), " ", MB_OK);
//        //    return;
//        //}
//    }
//
//    if (connectDat.index < 0)
//    {
//        MessageBox(NULL, std::string("クライアント: うまく接続ができませんでした").c_str(), " ", MB_OK);
//
//        if (g_Excute != nullptr)
//        {
//            //delete g_Excute;
//            g_Excute = nullptr;
//        }
//
//        if (GetScene() == SCENE_MULTICLIENT)
//        {
//            SetSceneFade(SCENE_TITLE);
//        }
//
//        //WinSockClient_End();
//        WinSockSetType(WINSOCK_CONNECT_TYPE::OFFLINE);
//        g_IsEnable = false;
//
//        return;
//    }
//    //MessageBox(NULL, std::string("クライアント: 接続完了").c_str(), " ", MB_OK);
//
//    //ループ
//    while (WinSockIsEnable() && !g_EndFlag)
//    {
//        //鯖との接続チェック
//        if (g_ServerConnectCheckFlag)
//        {
//            g_ServerConnectCheckFlag = false;
//
//            { //通信
//                WINSOCK_SERVER_CLIENT_SEND_INFO* sendDat = new WINSOCK_SERVER_CLIENT_SEND_INFO;
//
//                sendDat->flag = WINSOCK_RECIV_FLAG::CONNECT_TEST;
//
//                sendto(*sock, (char*)sendDat, sizeof(*sendDat), 0, (struct sockaddr*)&addr, sizeof(addr));
//
//                delete sendDat;
//
//                DWORD stime = timeGetTime();
//
//                //通信情報を受け取る
//                while (WinSockIsEnable())
//                {
//                    WINSOCK_SERVER_CLIENTCONNECTED_REPLY replyget = {};
//
//                    int recvLen = recv(*sock,
//                        (char*)&replyget,
//                        sizeof(replyget)
//                        ,
//                        0);
//                    if (recvLen == SOCKET_ERROR)
//                    { //失敗時
//                        if ((stime + 1000) < timeGetTime())
//                        {
//                            MessageBox(NULL, std::string("サーバーからの応答がありません").c_str(), " ", MB_OK);
//
//                            if (g_Excute != nullptr)
//                            {
//                                //delete g_Excute;
//                                g_Excute = nullptr;
//                            }
//
//                            if (GetScene() == SCENE_MULTICLIENT)
//                            {
//                                SetSceneFade(SCENE_TITLE);
//                            }
//
//                            //WinSockClient_End();
//                            WinSockSetType(WINSOCK_CONNECT_TYPE::OFFLINE);
//                            g_IsEnable = false;
//
//                            return;
//                        }
//                        //DebugPrintf("SOCKET_ERROR");
//                    }
//                    else
//                    {
//                        break;
//                    }
//                }
//            }
//        }
//
//        //プレイヤーが去る
//        if (g_PlayerLeaveSendFlag)
//        {
//            g_PlayerLeaveSendFlag = false;
//
//            WINSOCK_SERVER_CLIENT_SEND* sendDat = new WINSOCK_SERVER_CLIENT_SEND;
//            ZeroMemory(sendDat, sizeof(*sendDat));
//
//            sendDat->info.flag = WINSOCK_RECIV_FLAG::PLAYER_DAT_CLIENT_LEAVE;
//            sendDat->info.connectDat = connectDat;
//
//            sendto(*sock, (char*)sendDat,
//                sizeof(sendDat->info),
//
//                //sizeof(*sendDat),
//                0, (struct sockaddr*)&addr, sizeof(addr));
//
//            delete sendDat;
//        }
//
//        //プレイヤー情報送信
//        if (g_PlayerDatSendFlag)
//        {
//            g_PlayerDatSendFlag = false;
//
//            WINSOCK_SERVER_CLIENT_SEND* sendDat = new WINSOCK_SERVER_CLIENT_SEND;
//            ZeroMemory(sendDat, sizeof(*sendDat));
//
//            sendDat->info.flag = WINSOCK_RECIV_FLAG::PLAYER_DAT;
//            sendDat->info.connectDat = connectDat;
//
//            sendDat->playerdat.playerNo = Scene::Select::selected_id; //プレイヤーの番号
//
//            sendto(*sock, (char*)sendDat,
//                //sizeof(sendDat->info) + sizeof(sendDat->playerdat),
//                //sizeof(sendDat->info) + sizeof(sendDat->inputdat),
//                sizeof(sendDat->info) + sizeof(sendDat->playerdat)
//                + sizeof(INT) * 8 //余裕を持たせる
//                ,
//
//                //sizeof(*sendDat),
//                0, (struct sockaddr*)&addr, sizeof(addr));
//
//            delete sendDat;
//        }
//
//        //入力情報送信
//        if(g_InputDatSendFlag)
//        {
//            g_InputDatSendFlag = false;
//
//            WINSOCK_SERVER_CLIENT_SEND* sendDat = new WINSOCK_SERVER_CLIENT_SEND;
//            ZeroMemory(sendDat, sizeof(*sendDat));
//
//            sendDat->info.flag = WINSOCK_RECIV_FLAG::INPUT_DAT;
//            sendDat->info.connectDat = connectDat;
//
//            sendDat->inputdat = GetControllerMainInputData(0);
//
//            sendto(*sock, (char*)sendDat,
//                sizeof(sendDat->info) + sizeof(sendDat->inputdat) ,
//                //sizeof(*sendDat),
//                0, (struct sockaddr*)&addr, sizeof(addr));
//
//            delete sendDat;
//        }
//
//        //描画情報の受信
//        {
//            //{
//            //    WINSOCK_SERVER_CLIENT_SEND* sendDat = new WINSOCK_SERVER_CLIENT_SEND;
//            //    sendDat->info.flag = WINSOCK_RECIV_FLAG::CAMERA_DAT;
//            //    sendDat->info.connectDat = connectDat;
//
//            //    sendto(*sock, (char*)sendDat, sizeof(sendDat->info), 0, (struct sockaddr*)&addr, sizeof(addr));
//            //    delete sendDat;
//            //}
//
//            {
//                WINSOCK_SERVER_CLIENT_SEND* sendDat = new WINSOCK_SERVER_CLIENT_SEND;
//                ZeroMemory(sendDat, sizeof(*sendDat));
//
//                //通信情報を受け取る
//
//                int recvLen = recv(*sock,
//                    (char*)sendDat,
//                    sizeof(*sendDat),
//                    //senLen,
//                    //sizeof(sendDat->info) + sizeof(sendDat->cameraBuf.info) + sizeof(sendDat->cameraBuf),
//                    0);
//                //if (recvLen == SOCKET_ERROR)
//                //{ //失敗時
//                //    MessageBox(NULL, std::string("クライアント: そけっとえらー  CAMERA_DAT").c_str(), " ", MB_OK);
//                //    delete sendDat;
//                //    return;
//                //}
//                if (recvLen == SOCKET_ERROR)
//                { //失敗時
//                }
//                else
//                {
//                    if (sendDat->info.flag == WINSOCK_RECIV_FLAG::TEX_LOAD)
//                    {
//                        //描画情報読み込み
//                        int drawNum = (int)(sendDat->texDataLoadBuf.bufSize
//                            / sizeof(sendDat->texDataLoadBuf.buf[0]));
//
//                        if (drawNum > 0)
//                        {
//                            for (int i = 0; i < drawNum; i++)
//                            {
//                                if (sendDat->texDataLoadBuf.buf[i].enable)
//                                {
//                                    if (sendDat->texDataLoadBuf.buf[i].unload)
//                                    {
//                                        UnloadTexData(sendDat->texDataLoadBuf.buf[i].textureId);
//                                    }
//                                    else
//                                    {
//                                        PolygonTextureCache(LoadTexData(sendDat->texDataLoadBuf.buf[i].textureId).textureId_DX);
//                                    }
//                                }
//                            }
//                        }
//                        //if (sendDat->texDataLoadBuf.bufSize > 0 && !g_TexDatLoadBufRecvFlag)
//                        //{
//                        //    ZeroMemory(
//                        //        &sendDat_TexDatLoadBuf,
//                        //        sizeof(sendDat_TexDatLoadBuf)
//                        //    );
//                        //    sendDat_TexDatLoadBuf = (*sendDat);
//
//                        //    g_TexDatLoadBufRecvFlag = true;
//                        //}
//                    }
//
//                    
//                    //鯖すて
//                    if (sendDat->info.flag == WINSOCK_RECIV_FLAG::SERVER_STATE)
//                    {
//                        g_saveServerState = sendDat->serverState;
//
//                        //DebugPrintf("scene: %d\nplayerLogInFlag: %d\n",
//                        //    sendDat->serverState.scene, 
//                        //    sendDat->serverState.playerLogInFlag);
//                    }
//
//
//                    //サーバーが閉じた
//                    if (sendDat->info.flag == WINSOCK_RECIV_FLAG::SERVER_CLOSE)
//                    {
//                        if (GetScene() == SCENE_MULTICLIENT)
//                        {
//                            SetSceneFade(SCENE_TITLE);
//                        }
//
//                        WinSockSetType(WINSOCK_CONNECT_TYPE::OFFLINE);
//                        g_IsEnable = false;
//                        //WinSockClient_End();
//                        break;
//                    }
//
//                    //プレイヤーのお情報がサーバーから破棄された
//                    if (sendDat->info.flag == WINSOCK_RECIV_FLAG::PLAYER_DAT_CLEAR)
//                    {
//                        if (GetScene() == SCENE_MULTICLIENT)
//                        {
//                            SetSceneFade(SCENE_TITLE);
//                        }
//                    }
//
//                    //ガヤのリクエスト
//                    if (sendDat->info.flag == WINSOCK_RECIV_FLAG::GAYA_REQUEST)
//                    {
//                        if (sendDat->gayaRequestBuf.cancelMode)
//                        {
//                            if (sendDat->gayaRequestBuf.name[0] == 0)
//                            {
//                                GE_GayaCancelRequestAll();
//                            }
//                            else
//                            {
//                                GE_GayaCancelRequest(
//                                    sendDat->gayaRequestBuf.name
//                                );
//                            }
//                            DebugPrintf("gayacan: %s", sendDat->gayaRequestBuf.name);
//                        }
//                        else
//                        {
//                            GE_GayaStartRequest(
//                                sendDat->gayaRequestBuf.name,
//                                0,
//                                sendDat->gayaRequestBuf.opt
//                            );
//                        }
//                    }
//
//                    //描画情報
//                    if (sendDat->info.flag == WINSOCK_RECIV_FLAG::CAMERA_DAT)
//                    {
//                        if (sendDat->cameraBuf.bufSize > 0 && !g_CameraDatRecvFlag)
//                        {
//                            {
//                                /*  int numtmp = 0;
//                                  for (int i = 0; i < WINSOCK_SERVER_CLIENT_SEND_CAMERA_BUF_MAX; i++)
//                                  {
//                                      if (sendDat->cameraBuf.buf[i].enable)
//                                      {
//                                          numtmp++;
//                                      }
//                                  }*/
//                                  //DrawFont("" + senLen, FONT_TYPE_DEFAULT,
//                                  //    MkF2(600.0f, 0.0f),
//                                  //    50,
//                                  //    MkF4(1.0f, 0.0f, 1.0f, 1.0f),
//                                  //    FONT_TEXT_ALIGNMENT_CENTER
//                                  //);
//
//                                  //DebugPrintf("sendDat %d\n", numtmp);
//
//                                  //DrawFont("" + sendDat->cameraBuf.bufSize, FONT_TYPE_DEFAULT,
//                                  //    MkF2(600.0f, 0.0f),
//                                  //    50,
//                                  //    MkF4(1.0f, 0.0f, 1.0f, 1.0f),
//                                  //    FONT_TEXT_ALIGNMENT_CENTER
//                                  //);
//                            }
//
//                            bool nextBuf = !sendDat_CameraDrawCurrentBuf;
//                            ZeroMemory(
//                                &sendDat_CameraDraw[nextBuf],
//                                sizeof(sendDat_CameraDraw[nextBuf])
//                            );
//                            sendDat_CameraDraw[nextBuf] = (*sendDat);
//                            sendDat_CameraDrawCurrentBuf = nextBuf;
//
//                            g_CameraDatRecvFlag = true;
//                        }
//                    }
//
//                }
//
//                delete sendDat;
//            }
//        }
//
//        //if (g_CameraDatDrawFlag)
//        //{
//        //    draw();
//        //    g_CameraDatDrawFlag = false;
//        //}
//
//        
//        //Sleep(16);
//        //Sleep(60);
//
//        //std::rewind(stdin);
//        //std::cin.get();
//
//        //// 送信
//        //// sendto(ソケット, 送信するデータ, データのバイト数, フラグ, アドレス情報, アドレス情報のサイズ);
//        //// 送信するデータに直接文字列 "HELLO" 等を入れることもできる
//        //// バインドしている場合は send(sock, buf, 5, 0); でもOK？
//        //sendto(sock, (char*)&g_buf, sizeof(g_buf), 0, (struct sockaddr*)&addr, sizeof(addr));
//
//        //Sleep(100);
//    }
//
//    if (g_Excute != nullptr)
//    {
//        //delete g_Excute;
//        g_Excute = nullptr;
//    }
//}
//
//void WinSockClient_Private_draw()
//{
//    //SOCKET* sock = WinSockGetSock();
//    {
//        WINSOCK_SERVER_CLIENT_SEND* cameraDrawdatTmp = new WINSOCK_SERVER_CLIENT_SEND;
//        (*cameraDrawdatTmp) = sendDat_CameraDraw[sendDat_CameraDrawCurrentBuf];
//
//        //描画情報読み込み
//        //int cIndex = cameraDrawdatTmp->info.connectDat.index + 1;
//
//        //int drawNum = (int)(cameraDrawdatTmp->cameraBuf.bufSize
//        //    / sizeof(cameraDrawdatTmp->cameraBuf.buf[0]));
//        int drawNum = cameraDrawdatTmp->cameraBuf.bufSize;
//
//        if (drawNum > 0)
//        {
//            //深度設定を保持
//            bool deSave = DXDepthStencilStateDepthEnable();
//            if (deSave)
//            {
//                DX_SetCameraInfo(0, cameraDrawdatTmp->cameraBuf.info);
//            }
//
//            if (GetPostEffectShader() != cameraDrawdatTmp->cameraBuf.postEffect)
//            {
//                SetPostEffectShader(cameraDrawdatTmp->cameraBuf.postEffect);
//            }
//            // SetCameraInfo(cIndex, sendDat_CameraDraw.cameraBuf.info);
//
//             //std::sort(sendDat_CameraDraw.cameraBuf.buf,
//             //    sendDat_CameraDraw.cameraBuf.buf + WINSOCK_SERVER_CLIENT_SEND_CAMERA_BUF_MAX,
//             //    [](const CAMERA_DRAW_BUF& a, const CAMERA_DRAW_BUF& b) {
//             //        return a.pro.priority > b.pro.priority;
//             //    }
//             //);
//            //int numtmp = 0;
//
//            //DebugPrintf("%d\n", drawNum);
//            //for (int i = 0; i < WINSOCK_SERVER_CLIENT_SEND_CAMERA_BUF_MAX; i++)
//            for (int i = 0; i < drawNum; i++)
//            {
//                //CameraAddDrawBuf(cIndex, sendDat_CameraDraw.cameraBuf.buf[i]);
//
//                if (cameraDrawdatTmp->cameraBuf.buf[i].enable)
//                {
//                    cameraDrawdatTmp->cameraBuf.buf[i].dpb.cameraIndex = 0;
//                    //sendDat_CameraDraw[sendDat_CameraDrawCurrentBuf].cameraBuf.buf[i].pro.texNo = 0;
//                    PolygonRelease(
//                        &cameraDrawdatTmp->cameraBuf.buf[i].dpb,
//                        cameraDrawdatTmp->cameraBuf.buf[i].pro,
//                        true
//                    );
//
//                    //if (sendDat_CameraDraw.cameraBuf.buf[i].pro.priority == DRAW_POLYGON_PRIORITY_FORCE_ALWAYS)
//                    //{
//                    //    DebugPrintf("%d\n", sendDat_CameraDraw.cameraBuf.buf[i].dpb.modelNo);
//                    //}
//                    //if (sendDat_CameraDraw.cameraBuf.buf[i].dpb.modelNo != 0)
//                    //{
//                    //    DebugPrintf("%d\n", sendDat_CameraDraw.cameraBuf.buf[i].dpb.modelNo);
//                    //}
//
//                    //CameraAddDrawBuf(0, sendDat_CameraDraw.cameraBuf.buf[i]);
//
//                    //numtmp++;
//                }
//            }
//
// /*           if (GetAsyncKeyState('M'))
//            {
//                drawNum = drawNum;
//            }*/
//            //DebugPrintf("cameraDrawdatTmp %d\n", numtmp);
//        }
//        delete cameraDrawdatTmp;
//    }
//}
//
//void WinSockClientSend_PlayerDat(void)
//{
//    g_PlayerDatSendFlag = true;
//}
//
//void WinSockClientSend_PlayerLeave(void)
//{
//    g_PlayerLeaveSendFlag = true;
//}
//
//WINSOCK_SERVER_CLIENT_SEND_SERVER_STATE WinSockClientGetServerState(void)
//{
//    return g_saveServerState;
//}
