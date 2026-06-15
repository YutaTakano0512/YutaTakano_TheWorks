

//---------------------------------------------------------------------------
// winsock.cpp
// 
// sougo hara  2024/12/15
//---------------------------------------------------------------------------

#include "winsock.h"
#include "winsock_server.h"

#include "../../Game/Configuration/configuration.h"
#include "../../main.h"

#include "../../game_main.h"

#include "../../Game/Managers/scene.h"
#include "../../Game/Entities/player.h"

void WinSockServer_Start(void)
{
};


void WinSockServer_Update(void)
{
};

void WinSockServer_Draw(void)
{
};

void WinSockServer_End(void)
{
};


bool WinSockServerIsEnable(void)
{
    return false;
};


void WinSockServerPlayerDatClear()
{
};


std::vector<WINSOCK_SERVER_SERVER_PLAYERDAT> WinSockServerGetPlayerDat()
{
    return {};
};



//仮
//#include "../../Game/Systems/font.h"
//
//struct CLIENTDAT {
//    sockaddr_in Addr;
//    int AddrSize;
//
//    WINSOCK_SERVER_CLIENTCONNECTED connectDat;
//};
//
//
//static std::vector<CLIENTDAT> g_ClientdatCache = {};
//static std::vector<WINSOCK_SERVER_SERVER_PLAYERDAT> g_ClientPlayerDat = {};
//
//static std::thread* g_Excute = nullptr;
//
//static WINSOCK_SERVER_CLIENT_SEND* g_RecvBuf = nullptr;
//
//static bool g_IsEnable = false;
//static bool g_TexDatLoadSendFlag = false;
//static bool g_GayaRequSendFlag = false;
//static bool g_CameraDrawSendFlag = false;
//static bool g_PlayerDatClearFlag = false;
//static bool g_ServerStateSendFlag = false;
//static bool g_EndFlag = false;
//static std::thread* g_CameraDrawSend = nullptr;
//
//static bool g_ClientScreenSendFlag = false;
//static TEX_DAT g_tex_client_wait_screen = {};
//
//
//static void server_execute();
//static void server_ExecuteClient(SOCKET* sock, int index);
//static void server_DrawDatSend( );
//
//void WinSockServer_Start(void)
//{
//    if (!WinSockIsEnable())
//    {
//        return;
//    }
//    WinSockSetType(WINSOCK_CONNECT_TYPE::SERVER);
//    g_EndFlag = false;
//
//    {
//        GAYA_REQUEST_OPT gayaopt = {};
//        //gayaopt.repdat.push_back({ L"%s", L"サーバー　オン" });
//        GE_GayaStartRequest("_system_general", 0, gayaopt);
//    }
//
//    //MessageBox(NULL, std::string("サーバー　オン").c_str(), " ", MB_OK);
//
//    g_tex_client_wait_screen = LoadTexData("TEX/ui/in_game_master/multi_client_wait");
//    g_ClientScreenSendFlag = false;
//
//    g_Excute = nullptr;
//    g_CameraDrawSend = nullptr;
//    g_RecvBuf = nullptr;
//    g_ClientdatCache = {};
//    g_CameraDrawSendFlag = false;
//
//    // アドレス等格納
//    struct sockaddr_in addr;
//    addr.sin_family = AF_INET;  //IPv4
//    addr.sin_port = htons(WINSCK_SET_PORT);   //通信ポート番号設定
//    addr.sin_addr.S_un.S_addr = INADDR_ANY; // INADDR_ANYはすべてのアドレスからのパケットを受信する
//
//    SOCKET* sock = WinSockGetSock();
//
//    //// ブロッキング、ノンブロッキングの設定
//    ////（任意、必須コードではない（ソケットの初期設定はブロッキングモードなため）
//    //// val = 0 : ブロッキングモード データが受信されるまで待機
//    //// val = 1 : ノンブロッキング データが受信されなくても次の処理へ
//    //u_long val = 1;
//    //ioctlsocket(*sock, FIONBIO, &val);
//
//    // バインド
//    // アドレス等の情報をsocketに登録する
//    if (bind(*sock, (struct sockaddr*)&addr, sizeof(addr)) == SOCKET_ERROR)
//    {
//        MessageBox(NULL, std::string("Bind failed.").c_str(), " ", MB_OK);
//        //closesocket(udpSocket);
//        //WSACleanup();
//        return;
//    };
//
//    // バッファ ここに受信したデータが入る サイズは自由に決められるが、char配列
//    //char buf[2048];
//    //memset(&g_buf, 0, sizeof(g_buf)); // 複数回受信するときは前回値が残らないようにこのように バッファを0でクリアするのが一般的 メモリー操作関数
//
//    //sockaddr_in senderAddr;
//    //int senderAddrSize = sizeof(senderAddr);
//
//    //サーバーのスレッド処理
//    g_Excute = new std::thread(server_execute);
//    g_CameraDrawSend = new std::thread(server_DrawDatSend);
//    g_IsEnable = true;
//}
//void WinSockServer_Update(void)
//{
//    g_TexDatLoadSendFlag = true;
//    g_GayaRequSendFlag = true;
//    g_ServerStateSendFlag = true;
//    //server_execute();
//}
//
//void WinSockServer_Draw(void)
//{
//    //仮
//    g_ClientScreenSendFlag =
//        GetScene() == SCENE_GAME
//        || GetScene() == SCENE_RESULT
//        ;
//
//    if (!g_ClientScreenSendFlag)
//    {
//        for (int i = 0; i < CAMERA_NUM - 1; i++)
//        {
//            DRAW_POLYGON_DAT dsdat = {};
//            dsdat.pos.x = 0.0f;
//            dsdat.pos.y = 0.0f;
//            dsdat.pos.z = 0.0f;
//
//            dsdat.size.x = SCREEN_WIDTH;
//            dsdat.size.y = SCREEN_HEIGHT;
//            dsdat.size.z = 1.0f;
//
//            dsdat.rot = 0.0f;
//
//            dsdat.color = MkF4(1.5f, 1.5f, 1.5f, 2.0f);
//
//            dsdat.texNo = g_tex_client_wait_screen.textureId;
//            dsdat.loadTexType = LOADTEXTURETYPE_MAIN;
//
//            dsdat.texUV = {
//                0.0f,
//                0.0f + 0.05f * (sinf((float)GetMainGameFrame() / 30) / 2.0f + 0.5f)
//            };
//            dsdat.texWH = { 1.0f, 1.0f };
//
//            dsdat.startPos = {
//                0.0f,
//                0.0f
//            };
//            dsdat.startRot = 0;
//            dsdat.startScale = { 1.0f, 1.0f };
//            dsdat.startScale3D = { 1.0f, 1.0f, 1.0f };
//
//            dsdat.shaderOpt.bloom = DPD_SHADER_OPT_BLOOM_NONE;
//
//            DRAW_POLYGON_IN_CAMERA_EX_OPTION opt = {};
//            opt.reverse = false;
//            opt.fixed = true;
//            opt.depthBuffIgnore = true;
//            opt.priority = DRAW_POLYGON_PRIORITY_FORCE_ALWAYS;
//
//            DrawPolygonInCameraSingle(i + 1, dsdat, opt);
//        }
//    }
//
//    g_CameraDrawSendFlag = true;
//}
//
//void WinSockServer_End(void)
//{
//    g_EndFlag = true;
//
//    while (
//        g_Excute != nullptr
//        || g_CameraDrawSend != nullptr
//    )
//    { //終了待ち
//        Sleep(0);
//    }
//
//    WinSockSetType(WINSOCK_CONNECT_TYPE::OFFLINE);
//    g_IsEnable = false;
//}
//
//
//bool WinSockServerIsEnable(void)
//{
//    return g_IsEnable;
//}
//
//void WinSockServerPlayerDatClear()
//{
//    g_PlayerDatClearFlag = true;
//}
//
//std::vector<WINSOCK_SERVER_SERVER_PLAYERDAT> WinSockServerGetPlayerDat()
//{
//    return g_ClientPlayerDat;
//}
//
//void server_execute()
//{
//    SOCKET* sock = WinSockGetSock();
//
//
//    //bool clientConnected = false;
//
//    //受信データ一時的
//    g_RecvBuf = new WINSOCK_SERVER_CLIENT_SEND;
//    ZeroMemory(g_RecvBuf, sizeof(*g_RecvBuf));
//
//    //int maxDataSize = sizeof(WINSOCK_SERVER_CLIENT_SEND_CAMERADAT);
//    //char* recvBuf = new char[maxDataSize];
//
//    while (WinSockIsEnable() && !g_EndFlag)
//    {
//        ZeroMemory(g_RecvBuf, sizeof(*g_RecvBuf));
//
//        //クライアントの情報格納場所
//        CLIENTDAT clientdat;
//        clientdat.AddrSize = sizeof(clientdat.Addr);
//
//        //まずデータを受信する
//        //int recvLen = recvfrom(*sock, recvBuf, sizeof(char) * maxDataSize, 0, (sockaddr*)&clientdat.Addr, &clientdat.AddrSize);
//        int recvLen = recvfrom(*sock, (char*)g_RecvBuf, sizeof(*g_RecvBuf), 0, (sockaddr*)&clientdat.Addr, &clientdat.AddrSize);
//        if (recvLen == SOCKET_ERROR)
//        { //失敗時
//            //MessageBox(NULL, std::string("recvfrom failed. 0").c_str(), " ", MB_OK);
//            continue;
//        }
//
//        {
//            if (g_RecvBuf->info.flag == WINSOCK_RECIV_FLAG::CONNECT)
//            { //最初の接続
//                //clientConnected = true;
//
//                // クライアントに返信
//                WINSOCK_SERVER_CLIENTCONNECTED_REPLY reply = {};
//                reply.cdat.index = g_ClientdatCache.size();
//
//                clientdat.connectDat = reply.cdat;
//
//
//                DebugPrintf("サーバー: クライアントからの接続 %d\n", g_ClientdatCache.size());
//                g_ClientdatCache.push_back(clientdat);
//
//                
//                {
//                    GAYA_REQUEST_OPT gayaopt = {};
//                    //gayaopt.repdat.push_back({ L"%d", std::to_wstring(reply.index) });
//                    GE_GayaStartRequest("_system_player_in", 0, gayaopt);
//                }
//
//                //どの番号に接続したかを返す
//                sendto(*sock, 
//                    (char*)&reply, 
//                    sizeof(reply),
//                    0, (sockaddr*)&clientdat.Addr, clientdat.AddrSize);
//                //MessageBox(NULL, std::string("サーバー: クライアントからの接続").c_str(), " ", MB_OK);
//            }
//            else
//            {
//                const WINSOCK_SERVER_CLIENTCONNECTED& connectDat = g_RecvBuf->info.connectDat;
//
//                if (g_RecvBuf->info.flag == WINSOCK_RECIV_FLAG::CONNECT_TEST)
//                { //最初の接続
//
//                    // クライアントに返信
//                    WINSOCK_SERVER_CLIENTCONNECTED_REPLY reply = {};
//                    reply.cdat = connectDat;
//
//                    //どの番号に接続したかを返す
//                    sendto(*sock, 
//                        (char*)&reply,
//                        sizeof(reply),
//                        0, 
//                        (sockaddr*)&clientdat.Addr, clientdat.AddrSize);
//                    //MessageBox(NULL, std::string("サーバー: クライアントからの接続").c_str(), " ", MB_OK);
//                }
//
//                if (g_RecvBuf->info.flag == WINSOCK_RECIV_FLAG::INPUT_DAT)
//                { //入力情報を受け取る
//
//                    //const WINSOCK_SERVER_CLIENT_SEND_INPUTDAT& recvInputDatTmp
//                    //    = (const WINSOCK_SERVER_CLIENT_SEND_INPUTDAT&)recvBuf;
//
//                    //DebugPrintf("入力情報 %d\n", connectDat.index);
//
//                    //プレイヤーとして接続済みのクライアントの中から探す
//                    if (g_ClientPlayerDat.size() > 0)
//                    {
//                        for (int i = 0; i < std::min((int)g_ClientPlayerDat.size(), SYS_PLAYER_MAX - 1); i++)
//                        {
//                            if (g_ClientPlayerDat[i].enable
//                                && connectDat.index == g_ClientPlayerDat[i].dat.info.connectDat.index)
//                            {
//                                //入力情報をセットする
//                                SetControllerMainInputDataFixed(
//                                    i + 1,
//                                    g_RecvBuf->inputdat
//                                );
//                                break;
//                            }
//                        }
//                    }
//
//
//                    ////入力情報をセットする
//                    //SetControllerMainInputDataFixed(
//                    //    g_RecvBuf->info.connectDat.index + 1,
//                    //    g_RecvBuf->inputdat
//                    //);
//                }
//
//                if (g_RecvBuf->info.flag == WINSOCK_RECIV_FLAG::PLAYER_DAT_CLIENT_LEAVE)
//                { //プレイヤーが去る
//
//                    //プレイヤーとして接続済みのクライアントの中から探す
//                    if (g_ClientPlayerDat.size() > 0)
//                    {
//                        for (int i = 0; i < std::min((int)g_ClientPlayerDat.size(), SYS_PLAYER_MAX - 1); i++)
//                        {
//                            if (g_ClientPlayerDat[i].enable
//                                && connectDat.index == g_ClientPlayerDat[i].dat.info.connectDat.index)
//                            {
//                                g_ClientPlayerDat[i].enable = false;
//                                break;
//                            }
//                        }
//                    }
//                }
//
//                if (g_RecvBuf->info.flag == WINSOCK_RECIV_FLAG::PLAYER_DAT)
//                { //プレイヤー情報を受け取る
//
//                    DebugPrintf("c: %d\np: %d\n\n", 
//                        g_RecvBuf->info.connectDat.index, 
//                        g_RecvBuf->playerdat.playerNo
//                    );
//
//                    bool kizon = false;
//                    //プレイヤーとして接続済みのクライアントの中から探す
//                    if (g_ClientPlayerDat.size() > 0)
//                    {
//                        for (int i = 0; i < std::min((int)g_ClientPlayerDat.size(), SYS_PLAYER_MAX - 1); i++)
//                        {
//                            if (!g_ClientPlayerDat[i].enable
//                                && connectDat.index == g_ClientPlayerDat[i].dat.info.connectDat.index)
//                            {
//                                g_ClientPlayerDat[i].enable = true;
//                                g_ClientPlayerDat[i].dat = *g_RecvBuf;
//
//                                kizon = true;
//                                break;
//                            }
//                        }
//                    }
//
//                    if (!kizon)
//                    {
//                        WINSOCK_SERVER_SERVER_PLAYERDAT* spd = new WINSOCK_SERVER_SERVER_PLAYERDAT;
//                        ZeroMemory(spd, sizeof(*spd));
//
//                        spd->enable = true;
//                        spd->dat = *g_RecvBuf;
//
//                        g_ClientPlayerDat.push_back(*spd);
//
//                        delete spd;
//                    }
//                }
//
//                //if (recvBuf->info.flag == WINSOCK_RECIV_FLAG::CAMERA_DAT)
//                //{ //情報を受け取る
//                //    WINSOCK_SERVER_CLIENT_SEND* sendDat = new WINSOCK_SERVER_CLIENT_SEND;
//                //    ZeroMemory(sendDat, sizeof(*sendDat));
//                //    sendDat->info = recvBuf->info;
//
//                //    {
//                //        int cIndex = connectDat.index + 1;
//
//                //        sendDat->cameraBuf.bufSize = 0;
//                //        sendDat->cameraBuf.info = DX_GetCameraInfo(cIndex);
//                //        //sendDat->cameraBuf.info = GetCameraInfo(cIndex);
//                //        CAMERA_DRAW_BUF* cdb = CameraGetDrawBuf(cIndex);
//                //        //DebugPrintf("%d\n", clientTmp.index);
//                //        {
//                //            for (int i = 0; i < WINSOCK_SERVER_CLIENT_SEND_CAMERA_BUF_MAX; i++)
//                //            {
//                //                if (cdb[i].enable)
//                //                {
//                //                    sendDat->cameraBuf.buf[i] = cdb[i];
//                //                    sendDat->cameraBuf.bufSize += sizeof(sendDat->cameraBuf.buf[i]);
//                //                }
//                //                //if (i < WINSOCK_SERVER_CLIENT_SEND_CAMERA_BUF_MAX)
//                //                //{
//                //                //    sendDat->cameraBuf.buf[i] = (*cdb)[i];
//                //                //    sendDat->cameraBuf.bufSize += sizeof(sendDat->cameraBuf.buf[i]);
//                //                //}
//                //                //else
//                //                //{
//                //                //    break;
//                //                //}
//                //            }
//                //            //DebugPrintf("%d\n", cdb->size());
//                //        }
//                //        //DebugPrintf("%d\n", (int)(sendDat->cameraBuf.bufSize / sizeof(sendDat->cameraBuf.buf[0])));
//                //    }
//
//                //    int senLen = sizeof(sendDat->info)
//                //        + sizeof(sendDat->cameraBuf.info)
//                //        + sizeof(sendDat->cameraBuf.bufSize)
//                //        + sendDat->cameraBuf.bufSize;
//                //    //DebugPrintf("%d\n", senLen);
//
//                //    //どの番号に接続したかを返す
//                //    sendto(*sock, 
//                //        (char*)sendDat,
//                //        //sizeof(*sendDat),
//                //        senLen,
//                //        0, (sockaddr*)&clientdat.Addr, clientdat.AddrSize);
//
//                //    delete sendDat;
//                //}
//            }
//
//        }
//    }
//
//    delete g_RecvBuf;
//    g_RecvBuf = nullptr;
//    //delete[] recvBuf;
//
//    ////想定されるクライアント分スレッドを立てる
//    //std::thread* thre[WINSCK_CLIENT_MAX] = {};
//
//    //for (int i = 0; i < WINSCK_CLIENT_MAX; i++)
//    //{
//    //    thre[i] = new std::thread(server_ExecuteClient, sock, i);
//    //}
//
//    ////スレッドの処理待ち
//    //for (int i = 0; i < WINSCK_CLIENT_MAX; i++)
//    //{
//    //    thre[i]->join();
//    //}
//
//    ////終了
//    //for (int i = 0; i < WINSCK_CLIENT_MAX; i++)
//    //{
//    //    delete thre[i];
//    //}
//
//    if (g_Excute != nullptr)
//    {
//        //delete g_Excute;
//        g_Excute = nullptr;
//    }
//}
//
//void server_DrawDatSend()
//{
//    SOCKET* sock = WinSockGetSock();
//
//    while (WinSockServerIsEnable())
//    {
//        if (g_ServerStateSendFlag)
//        {
//            g_ServerStateSendFlag = false;
//
//            if (sock != nullptr && g_ClientPlayerDat.size() > 0)
//            {
//                for (int i = 0; i < std::min((int)g_ClientPlayerDat.size(), SYS_PLAYER_MAX - 1); i++)
//                {
//                    if (!g_ClientPlayerDat[i].enable)
//                    {
//                        continue;
//                    }
//
//                    CLIENTDAT* cdtmp = nullptr;
//
//                    //接続済みのクライアントの中から探す
//                    if (g_ClientdatCache.size() > 0)
//                    {
//                        for (int j = 0; j < g_ClientdatCache.size(); j++)
//                        {
//                            if (g_ClientdatCache[j].connectDat.index
//                                == g_ClientPlayerDat[i].dat.info.connectDat.index
//                                )
//                            {
//                                cdtmp = &g_ClientdatCache[j];
//                                break;
//                            }
//                        }
//                    }
//
//                    if (cdtmp == nullptr)
//                    {
//                        continue;
//                    }
//
//                    WINSOCK_SERVER_CLIENT_SEND* sendDat = new WINSOCK_SERVER_CLIENT_SEND;
//                    ZeroMemory(sendDat, sizeof(*sendDat));
//
//                    sendDat->info.connectDat = cdtmp->connectDat;
//                    sendDat->info.flag = WINSOCK_RECIV_FLAG::SERVER_STATE;
//
//                    //サーバーの状況を伝達
//                    sendDat->serverState.scene = GetScene();
//                    sendDat->serverState.playerLogInFlag = GetPlayerLogInEnable();
//
//                    int senLen = sizeof(sendDat->info)
//                        + sizeof(sendDat->serverState)
//                        + sizeof(INT) * 8
//                        ;
//
//                    //返す
//                    sendto(*sock,
//                        (char*)sendDat,
//                        //sizeof(*sendDat),
//                        senLen,
//                        0, (sockaddr*)&cdtmp->Addr, cdtmp->AddrSize);
//
//                    delete sendDat;
//                }
//            }
//        }
//
//        if (g_TexDatLoadSendFlag)
//        {
//            g_TexDatLoadSendFlag = false;
//
//            if (sock != nullptr && g_ClientdatCache.size() > 0)
//            {
//                for (int i = 0; i < g_ClientdatCache.size(); i++)
//                {
//                    WINSOCK_SERVER_CLIENT_SEND* sendDat = new WINSOCK_SERVER_CLIENT_SEND;
//                    ZeroMemory(sendDat, sizeof(*sendDat));
//
//                    sendDat->info.connectDat = g_ClientdatCache[i].connectDat;
//                    sendDat->info.flag = WINSOCK_RECIV_FLAG::TEX_LOAD;
//
//                    {
//                        //int cIndex = connectDat.index + 1;
//                        int cIndex = g_ClientdatCache[i].connectDat.index + 1;
//
//                        sendDat->texDataLoadBuf.bufSize = 0;
//
//                        TEX_DAT_LOAD_BUFF* tdlb = GetTexDataBuff();
//
//                        {
//                            int setIndex = 0;
//                            for (int j = 0; j < WINSOCK_SERVER_CLIENT_SEND_TEX_DATA_LOAD_MAX; j++)
//                            {
//                                if (tdlb[j].enable)
//                                {
//                                    sendDat->texDataLoadBuf.buf[setIndex] = tdlb[j];
//                                    sendDat->texDataLoadBuf.bufSize += sizeof(sendDat->texDataLoadBuf.buf[0]);
//                                    setIndex++;
//                                }
//                            }
//                        }
//                    }
//
//                    if (sendDat->texDataLoadBuf.bufSize > 0)
//                    {
//                        sendDat->texDataLoadBuf.bufSize += sizeof(sendDat->texDataLoadBuf.buf[i]);
//
//                        int senLen = sizeof(sendDat->info)
//                            + sizeof(sendDat->texDataLoadBuf.bufSize)
//                            + sendDat->texDataLoadBuf.bufSize;
//                        //DebugPrintf("%d\n", senLen);
//                        //DebugPrintf("%d\n", senLen);
//
//                        //返す
//                        sendto(*sock,
//                            (char*)sendDat,
//                            //sizeof(*sendDat),
//                            senLen,
//                            0, (sockaddr*)&g_ClientdatCache[i].Addr, g_ClientdatCache[i].AddrSize);
//
//                    }
//
//                    delete sendDat;
//                }
//            }
//
//        }
//
//        //ガヤ
//        if (g_GayaRequSendFlag)
//        {
//            g_GayaRequSendFlag = false;
//
//            if (sock != nullptr && g_ClientPlayerDat.size() > 0)
//            {
//                GAYA_REQUEST_BUF* gayarequ = GE_GetGayaRequestBuf();
//
//                //DebugPrintf("gayacan: %s", gayarequ->name);
//                if (strlen(gayarequ->name) > 0 
//                    || gayarequ->cancelMode
//                    )
//                {
//                    for (int i = 0; i < std::min((int)g_ClientPlayerDat.size(), CAMERA_NUM - 1); i++)
//                    {
//                        if (!g_ClientPlayerDat[i].enable)
//                        {
//                            continue;
//                        }
//
//                        CLIENTDAT* cdtmp = nullptr;
//
//                        //接続済みのクライアントの中から探す
//                        if (g_ClientdatCache.size() > 0)
//                        {
//                            for (int j = 0; j < g_ClientdatCache.size(); j++)
//                            {
//                                if (g_ClientdatCache[j].connectDat.index
//                                    == g_ClientPlayerDat[i].dat.info.connectDat.index
//                                    )
//                                {
//                                    cdtmp = &g_ClientdatCache[j];
//                                    break;
//                                }
//                            }
//                        }
//
//                        //if (GetAsyncKeyState('O'))
//                        //{
//                        //    cdtmp = cdtmp;
//                        //}
//
//                        if (cdtmp == nullptr)
//                        {
//                            continue;
//                        }
//
//                        WINSOCK_SERVER_CLIENT_SEND* sendDat = new WINSOCK_SERVER_CLIENT_SEND;
//                        ZeroMemory(sendDat, sizeof(*sendDat));
//
//                        sendDat->info.connectDat = cdtmp->connectDat;
//                        sendDat->info.flag = WINSOCK_RECIV_FLAG::GAYA_REQUEST;
//
//                        {
//                            sendDat->gayaRequestBuf = *gayarequ;
//
//                            int senLen = sizeof(sendDat->info)
//                                + sizeof(sendDat->gayaRequestBuf)
//                                ;
//
//                            //返す
//                            sendto(*sock,
//                                (char*)sendDat,
//                                //sizeof(*sendDat),
//                                senLen,
//                                0, (sockaddr*)&cdtmp->Addr, cdtmp->AddrSize);
//
//                        }
//
//                        delete sendDat;
//                    }
//                }
//            }
//
//            GE_GayaRequestBufRelease();
//        }
//
//        //描画情報を送信する
//        if (g_CameraDrawSendFlag)
//        {
//            g_CameraDrawSendFlag = false;
//            
//
//            if (sock != nullptr && g_ClientPlayerDat.size() > 0)
//            {
//                for (int i = 0; i < std::min((int)g_ClientPlayerDat.size(), CAMERA_NUM - 1); i++)
//                {
//                    if (!g_ClientPlayerDat[i].enable)
//                    {
//                        continue;
//                    }
//
//                    CLIENTDAT* cdtmp = nullptr;
//
//                    //接続済みのクライアントの中から探す
//                    if (g_ClientdatCache.size() > 0)
//                    {
//                        for (int j = 0; j < g_ClientdatCache.size(); j++)
//                        {
//                            if (g_ClientdatCache[j].connectDat.index
//                                == g_ClientPlayerDat[i].dat.info.connectDat.index
//                            )
//                            {
//                                cdtmp = &g_ClientdatCache[j];
//                                break;
//                            }
//                        }
//                    }
//
//                    //if (GetAsyncKeyState('O'))
//                    //{
//                    //    cdtmp = cdtmp;
//                    //}
//
//                    if (cdtmp == nullptr)
//                    {
//                        continue;
//                    }
//
//                    WINSOCK_SERVER_CLIENT_SEND* sendDat = new WINSOCK_SERVER_CLIENT_SEND;
//                    ZeroMemory(sendDat, sizeof(*sendDat));
//
//                    sendDat->info.connectDat = cdtmp->connectDat;
//                    sendDat->info.flag = WINSOCK_RECIV_FLAG::CAMERA_DAT;
//
//                    {
//                        //int cIndex = connectDat.index + 1;
//                        //int cIndex = cdtmp->connectDat.index + 1;
//                        int cIndex = i + 1;
//
//                        sendDat->cameraBuf.bufSize = 0;
//                        sendDat->cameraBuf.info = DX_GetCameraInfo(cIndex);
//                        sendDat->cameraBuf.postEffect = GetPostEffectShader();
//                        //sendDat->cameraBuf.info = GetCameraInfo(cIndex);
//                        CAMERA_DRAW_BUF* cdb = CameraGetDrawBuf(cIndex);
//                        //DebugPrintf("%d\n", clientTmp.index);
//                        {
//                            int setIndex = 0;
//                            for (int j = 0; j < WINSOCK_SERVER_CLIENT_SEND_CAMERA_BUF_MAX; j++)
//                            {
//                                if (cdb[j].enable)
//                                {
//                                    //if (setIndex == 3)
//                                    //{
//                                    //    sendDat->cameraBuf.buf[setIndex] = {};
//                                    //    sendDat->cameraBuf.bufSize += sizeof(sendDat->cameraBuf.buf[i]);
//                                    //    setIndex++;
//                                    //}
//                                    sendDat->cameraBuf.buf[setIndex] = cdb[j];
//                                    //sendDat->cameraBuf.bufSize += sizeof(sendDat->cameraBuf.buf[i]);
//                                    sendDat->cameraBuf.bufSize++;
//                                    //sendDat->cameraBuf.bufSize += sizeof(sendDat->cameraBuf.buf[i]) * 2;
//                                    setIndex++;
//                                }
//                                //if (i < WINSOCK_SERVER_CLIENT_SEND_CAMERA_BUF_MAX)
//                                //{
//                                //    sendDat->cameraBuf.buf[i] = (*cdb)[i];
//                                //    sendDat->cameraBuf.bufSize += sizeof(sendDat->cameraBuf.buf[i]);
//                                //}
//                                //else
//                                //{
//                                //    break;
//                                //}
//                            }
//
//                            //if (GetAsyncKeyState('M'))
//                            //{
//                            //    setIndex = setIndex;
//                            //}
//                            //DebugPrintf("%d\n", cdb->size());
//                            //DebugPrintf("%d\n", setIndex);
//                        }
//                        //DebugPrintf("%d\n", (int)(sendDat->cameraBuf.bufSize / sizeof(sendDat->cameraBuf.buf[0])));
//                    }
//
//                    //if (sendDat->cameraBuf.bufSize > 0)
//                    //{
//                    //    sendDat->cameraBuf.bufSize += sizeof(sendDat->cameraBuf.buf[i]);
//                    //    //sendDat->cameraBuf.bufSize += sizeof(sendDat->cameraBuf.buf[i]) * 2;
//                    //    //sendDat->cameraBuf.bufSize += sizeof(char);
//                    //}
//                    if (sendDat->cameraBuf.bufSize > 0)
//                    {
//                        sendDat->cameraBuf.bufSize += 1;
//                    }
//
//                    //int senLen = sizeof(sendDat->info)
//                    //    + sizeof(sendDat->cameraBuf.info)
//                    //    + sizeof(sendDat->cameraBuf.postEffect)
//                    //    + sizeof(sendDat->cameraBuf.bufSize)
//                    //    + sendDat->cameraBuf.bufSize
//                    //    + sizeof(char);
//                    //DebugPrintf("%d\n", senLen);
//
//                    //int senLen = sizeof(sendDat)
//                    //    + sizeof(char);
//
//                    int senLen = sizeof(sendDat->info)
//                        + sizeof(sendDat->cameraBuf.info)
//                        + sizeof(sendDat->cameraBuf.postEffect)
//                        + sizeof(sendDat->cameraBuf.bufSize)
//                        + (sendDat->cameraBuf.bufSize * sizeof(sendDat->cameraBuf.buf[0]))
//                        ;
//
//                    //返す
//                    sendto(*sock,
//                        (char*)sendDat,
//                        //sizeof(*sendDat),
//                        senLen,
//                        0, (sockaddr*)&cdtmp->Addr, cdtmp->AddrSize);
//
//                    delete sendDat;
//                }
//            }
//            //if (sock != nullptr && g_ClientdatCache.size() > 0)
//            //{
//            //    for (int i = 0; i < g_ClientdatCache.size(); i++)
//            //    {
//            //        WINSOCK_SERVER_CLIENT_SEND* sendDat = new WINSOCK_SERVER_CLIENT_SEND;
//            //        ZeroMemory(sendDat, sizeof(*sendDat));
//
//            //        sendDat->info.connectDat = g_ClientdatCache[i].connectDat;
//            //        sendDat->info.flag = WINSOCK_RECIV_FLAG::CAMERA_DAT;
//
//            //        {
//            //            //int cIndex = connectDat.index + 1;
//            //            int cIndex = g_ClientdatCache[i].connectDat.index + 1;
//
//            //            sendDat->cameraBuf.bufSize = 0;
//            //            sendDat->cameraBuf.info = DX_GetCameraInfo(cIndex);
//            //            sendDat->cameraBuf.postEffect = GetPostEffectShader();
//            //            //sendDat->cameraBuf.info = GetCameraInfo(cIndex);
//            //            CAMERA_DRAW_BUF* cdb = CameraGetDrawBuf(cIndex);
//            //            //DebugPrintf("%d\n", clientTmp.index);
//            //            {
//            //                int setIndex = 0;
//            //                for (int i = 0; i < WINSOCK_SERVER_CLIENT_SEND_CAMERA_BUF_MAX; i++)
//            //                {
//            //                    if (cdb[i].enable)
//            //                    {
//            //                        //if (setIndex == 3)
//            //                        //{
//            //                        //    sendDat->cameraBuf.buf[setIndex] = {};
//            //                        //    sendDat->cameraBuf.bufSize += sizeof(sendDat->cameraBuf.buf[i]);
//            //                        //    setIndex++;
//            //                        //}
//            //                        sendDat->cameraBuf.buf[setIndex] = cdb[i];
//            //                        sendDat->cameraBuf.bufSize += sizeof(sendDat->cameraBuf.buf[i]);
//            //                        setIndex++;
//            //                    }
//            //                    //if (i < WINSOCK_SERVER_CLIENT_SEND_CAMERA_BUF_MAX)
//            //                    //{
//            //                    //    sendDat->cameraBuf.buf[i] = (*cdb)[i];
//            //                    //    sendDat->cameraBuf.bufSize += sizeof(sendDat->cameraBuf.buf[i]);
//            //                    //}
//            //                    //else
//            //                    //{
//            //                    //    break;
//            //                    //}
//            //                }
//
//            //                //if (GetAsyncKeyState('M'))
//            //                //{
//            //                //    setIndex = setIndex;
//            //                //}
//            //                //DebugPrintf("%d\n", cdb->size());
//            //            }
//            //            //DebugPrintf("%d\n", (int)(sendDat->cameraBuf.bufSize / sizeof(sendDat->cameraBuf.buf[0])));
//            //        }
//
//            //        if (sendDat->cameraBuf.bufSize > 0)
//            //        {
//            //            sendDat->cameraBuf.bufSize += sizeof(sendDat->cameraBuf.buf[i]);
//            //        }
//
//            //        int senLen = sizeof(sendDat->info)
//            //            + sizeof(sendDat->cameraBuf.info)
//            //            + sizeof(sendDat->cameraBuf.postEffect)
//            //            + sizeof(sendDat->cameraBuf.bufSize)
//            //            + sendDat->cameraBuf.bufSize;
//            //        //DebugPrintf("%d\n", senLen);
//
//            //        {
//            //            //int numtmp = 0;
//            //            //for (int i = 0; i < WINSOCK_SERVER_CLIENT_SEND_CAMERA_BUF_MAX; i++)
//            //            //{
//            //            //    if (sendDat->cameraBuf.buf[i].enable)
//            //            //    {
//            //            //        numtmp++;
//            //            //    }
//            //            //}
//            //            //DrawFont("" + senLen, FONT_TYPE_DEFAULT,
//            //            //    MkF2(600.0f, 0.0f),
//            //            //    50,
//            //            //    MkF4(1.0f, 0.0f, 1.0f, 1.0f),
//            //            //    FONT_TEXT_ALIGNMENT_CENTER
//            //            //);
//            //            //DebugPrintf("%d\n", numtmp);
//            //        }
//
//            //        //返す
//            //        sendto(*sock,
//            //            (char*)sendDat,
//            //            //sizeof(*sendDat),
//            //            senLen,
//            //            0, (sockaddr*)&g_ClientdatCache[i].Addr, g_ClientdatCache[i].AddrSize);
//
//            //        delete sendDat;
//            //    }
//            //}
//        }
//
//        //プレイヤーの情報を破棄する
//        if (g_PlayerDatClearFlag)
//        {
//            g_PlayerDatClearFlag = false;
//
//            if (sock != nullptr && g_ClientPlayerDat.size() > 0)
//            {
//                for (int i = 0; i < g_ClientPlayerDat.size(); i++)
//                {
//                    if (!g_ClientPlayerDat[i].enable)
//                    {
//                        continue;
//                    }
//
//                    CLIENTDAT* cdtmp = nullptr;
//
//                    //接続済みのクライアントの中から探す
//                    if (g_ClientdatCache.size() > 0)
//                    {
//                        for (int j = 0; j < g_ClientdatCache.size(); j++)
//                        {
//                            if (g_ClientdatCache[j].connectDat.index
//                                == g_ClientPlayerDat[i].dat.info.connectDat.index
//                                )
//                            {
//                                cdtmp = &g_ClientdatCache[j];
//                                break;
//                            }
//                        }
//                    }
//
//                    //if (GetAsyncKeyState('O'))
//                    //{
//                    //    cdtmp = cdtmp;
//                    //}
//
//                    if (cdtmp == nullptr)
//                    {
//                        continue;
//                    }
//
//                    WINSOCK_SERVER_CLIENT_SEND* sendDat = new WINSOCK_SERVER_CLIENT_SEND;
//                    ZeroMemory(sendDat, sizeof(*sendDat));
//
//                    sendDat->info.connectDat = cdtmp->connectDat;
//                    sendDat->info.flag = WINSOCK_RECIV_FLAG::PLAYER_DAT_CLEAR;
//
//                    int senLen = sizeof(sendDat->info) 
//                        + sizeof(INT) * 8 //余裕を持たせる
//                        ;
//
//                    //返す
//                    sendto(*sock,
//                        (char*)sendDat,
//                        //sizeof(*sendDat),
//                        senLen,
//                        0, (sockaddr*)&cdtmp->Addr, cdtmp->AddrSize);
//
//                    delete sendDat;
//                }
//            }
//
//            g_ClientPlayerDat.clear();
//            g_ClientPlayerDat.shrink_to_fit();
//        }
//
//
//        //サーバーを閉じる
//        if (g_EndFlag)
//        {
//            if (sock != nullptr && g_ClientdatCache.size() > 0)
//            {
//                for (int i = 0; i < g_ClientdatCache.size(); i++)
//                {
//
//                    WINSOCK_SERVER_CLIENT_SEND* sendDat = new WINSOCK_SERVER_CLIENT_SEND;
//                    ZeroMemory(sendDat, sizeof(*sendDat));
//
//                    sendDat->info.connectDat = g_ClientdatCache[i].connectDat;
//                    sendDat->info.flag = WINSOCK_RECIV_FLAG::SERVER_CLOSE;
//
//                    int senLen = sizeof(sendDat->info);
//
//                    //返す
//                    sendto(*sock,
//                        (char*)sendDat,
//                        //sizeof(*sendDat),
//                        senLen,
//                        0, (sockaddr*)&g_ClientdatCache[i].Addr, g_ClientdatCache[i].AddrSize);
//
//                    delete sendDat;
//                }
//            }
//
//            g_ClientPlayerDat.clear();
//            g_ClientPlayerDat.shrink_to_fit();
//
//            break;
//        }
//    }
//
//    if (g_CameraDrawSend != nullptr)
//    {
//        //delete g_Excute;
//        g_CameraDrawSend = nullptr;
//    }
//}
//
////クライアントごとの処理
//void server_ExecuteClient(SOCKET* sock, int index)
//{
//}