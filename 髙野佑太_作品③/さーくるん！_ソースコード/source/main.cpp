
//===========================================================================
// ################ この作業を毎度必ずやる ###############
//===========================================================================
// 
// プロジェクト > プロパティ
//
// -すべての構成-
//   詳細
//     文字セット → マルチ バイト文字セットを使用する
//   C/C++
//     全般
//       複数プロセッサによるコンパイル → はい
//
// -構成 Debug-
//   C / C++
//     コード生成
//       ランタイム ライブラリ → マルチスレッド デバッグ
//
// -構成 Release-
//   C / C++
//     コード生成
//       ランタイム ライブラリ → マルチスレッド
//===========================================================================


//---------------------------------------------------------------------------
// main.cpp
// 
// sougo hara  2024/07/04
//---------------------------------------------------------------------------
#include "Core/resource.h"
//#define _CRTDBG_MAP_ALLOC 
#include "main.h"

#include "Core/Input/mouse.h"
#include "Core/Input/keyboard.h"

#include "Core/DirectX/dx_game.h"
#include "Core/DirectX/dx.h"

#include "Core/debugPrintf.h"

#include "Core/app_event.h"

#include "game_main.h"
#include "Scenes/SCENE_Editor.h"

#include <iostream>
#include <filesystem>

//
// マクロ定義
// 
#define CLASS_NAME_EDIT		("GameWindow_Edit")

constexpr LONG WINDOW_STYLE_DEFAULT = WS_OVERLAPPEDWINDOW;

//
// グローバル変数
//
static DWORD g_CountFps;				// FPSカウンタ
static float g_game_loop_amount_of_delay = 0.0f; //ゲームループの遅延量

static int g_GameDrawFps = 1;
static int g_GameDrawWaitF = 0;
static bool g_GameDrawAutoFps = true;
static int g_GameDrawAutoFpsAddWaitF = 0;
static int g_GameDrawAutoFpsAddWaitFCooldown = 0;
static bool g_GameDrawThroughFlag = false;

static HWND hWnd_Main[DX_WINDOW_NUM] = {};
static HWND hWnd_Edit = NULL;

static HINSTANCE hInstanceSave;
static int nCmdShowSave;

static EditorWindowHWND editorWindowHWND = {};

//
// プロトタイプ宣言
// 
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK WndProcSub(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK WndProcEdit(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

//
// main
//
int APIENTRY WinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPSTR lpCmdLine,
	_In_ int nCmdShow)
{
    { //romフォルダがなかったら即終了
        DWORD attributes = GetFileAttributes("./rom");
        if (!(attributes != INVALID_FILE_ATTRIBUTES && (attributes & FILE_ATTRIBUTE_DIRECTORY)))
        {
            return 0;
        }
    }

    {
        HRESULT hr = CoInitializeEx(nullptr, COINIT_MULTITHREADED);
        if (FAILED(hr))
        {
            return 0;
        }
    }

	//_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	//使用しない変数を明示
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	hInstanceSave = hInstance;
	nCmdShowSave = nCmdShow;

    {
        WNDCLASS wc = {}; //すべて0で初期化
        wc.lpfnWndProc = WndProc; //ウィンドウプロシージャの指定
        wc.lpszClassName = CLASS_NAME;
        wc.hInstance = hInstance;
        wc.hCursor = LoadCursor(NULL, IDC_ARROW);
        wc.hbrBackground = (HBRUSH)(COLOR_BACKGROUND + 1);
        wc.hIcon = (HICON)LoadImage(wc.hInstance, 
            MAKEINTRESOURCE(IDI_ICON1),
            IMAGE_ICON,
            GetSystemMetrics(SM_CXICON), 
            GetSystemMetrics(SM_CYICON), 0);

        //登録する
        RegisterClass(&wc);
    }
    {
        WNDCLASS wc = {}; //すべて0で初期化
        wc.lpfnWndProc = WndProcSub; //ウィンドウプロシージャの指定
        wc.lpszClassName = CLASS_NAME_SUB;
        wc.hInstance = hInstance;
        wc.hCursor = LoadCursor(NULL, IDC_ARROW);
        wc.hbrBackground = (HBRUSH)(COLOR_BACKGROUND + 1);

        //登録する
        RegisterClass(&wc);
    }


	RECT winRect = {
		0,
		0,
		(LONG)(SCREEN_WIDTH * DX_WINDOW_SCREEN_MOD),
		(LONG)(SCREEN_HEIGHT * DX_WINDOW_SCREEN_MOD),
	};

    for (int i = 0; i < DX_WINDOW_NUM; i++)
    {
        while (1)
        {
            //DWORD winStyle = WS_OVERLAPPEDWINDOW ^ WS_THICKFRAME; //XOR ^
            DWORD winStyle = 
                WINDOW_STYLE_DEFAULT //WS_POPUP
                //& ~(WS_THICKFRAME | WS_MAXIMIZEBOX)
                ;
            DWORD winStyle2 = WS_BORDER |
                WS_HSCROLL | WS_VSCROLL | ES_AUTOHSCROLL | ES_AUTOVSCROLL |
                ES_LEFT | ES_MULTILINE;

            AdjustWindowRect(&winRect, winStyle, false);
            int winWid = winRect.right - winRect.left;
            int winHig = winRect.bottom - winRect.top;

            int desWid = GetSystemMetrics(SM_CXSCREEN);
            int desHig = GetSystemMetrics(SM_CYSCREEN);

            int winX = std::max((desWid - winWid) / 2, 0); //CW_USEDEFAULT
            int winY = std::max((winHig - desHig) / 2, 0);

            hWnd_Main[i] = CreateWindow(
                i == 0 ? CLASS_NAME: CLASS_NAME_SUB,
                WINDOW_CAPTION,
                i == 0 ? winStyle: winStyle2,

                //サイズと位置
                winX,
                winY,
                winWid,
                winHig,

                NULL,
                NULL,
                hInstance,
                NULL
            );
            if (hWnd_Main[i] == NULL)
            {
                if (MessageBox(hWnd_Main[i], "ウィンドウを作成できませんでした。", "kakunin", MB_RETRYCANCEL) == IDCANCEL)
                {
                    return 0;
                }
            }
            else
            {
                break;
            }
        }

        //SetWindowLong(hWnd_Main, GWL_STYLE, WS_POPUP);

        ShowWindow(hWnd_Main[i], nCmdShow);
    }


	//エディタウィンドウの生成
	{
		WNDCLASS wc = {}; //すべて0で初期化
		wc.lpfnWndProc = WndProcEdit; //ウィンドウプロシージャの指定
		wc.lpszClassName = CLASS_NAME_EDIT;
		wc.hInstance = hInstance;
		wc.hCursor = LoadCursor(NULL, IDC_ARROW);
		wc.hbrBackground = (HBRUSH)(COLOR_BACKGROUND + 1);

		//登録する
		RegisterClass(&wc);

		//エディットボックスの定義
		hWnd_Edit = CreateWindow(
			CLASS_NAME_EDIT,             //ウィンドウクラス名
			NULL,                   //キャプション
			WS_BORDER |

            WS_HSCROLL | WS_VSCROLL | ES_AUTOHSCROLL | ES_AUTOVSCROLL |
            ES_LEFT | ES_MULTILINE,         //スタイル指定
			10, 10,                  //位置 ｘ、ｙ
			SCREEN_EDIT_WIDTH, SCREEN_EDIT_HIGHT,                //幅、高さ
			NULL,                   //親ウィンドウ
			NULL /*(HMENU)1*/,               // メニューハンドルまたは子ウィンドウID
			hInstance,                  //インスタンスハンドル
			NULL);                  //その他の作成データ
		//ShowWindow(hWnd_Edit, SW_HIDE);
	}

    _AppEventInit();

	//マウス初期化
	Mouse_Initialize(hWnd_Main[0]);
	//Mouse_SetMode(MOUSE_POSITION_MODE_RELATIVE);
	//キー入力
	Keyboard_Initialize();

	//初期化
	GAME_INIT_SET gInitSet = {};
	gInitSet.hWnd = hWnd_Main;
	DX_GameInitialize(gInitSet);

	//ゲーム起動
	// 分解能を設定
	timeBeginPeriod(1);
	////fps計測用変数
	//DWORD ExecLastTime;
	//DWORD FPSLastTime;
 //   DWORD CurrentTime;
 //   DWORD DeltaTime;
	DWORD FrameCount = 0;

	//ExecLastTime =
	//	FPSLastTime = timeGetTime();
	//CurrentTime =
	//	FrameCount = 0;

	//ShowCursor(true);

    LONGLONG last_tick = 0;
    LONGLONG frequency = 0;
    float delta_time = 0;
    float max_delta_time = 0.1f;

    float game_loop_duration = 0.0f; // `DX_GameLoop` の実行時間


    // Timer関連初期化
    LARGE_INTEGER freq;
    QueryPerformanceFrequency(&freq);
    frequency = freq.QuadPart;

    LARGE_INTEGER now;
    //QueryPerformanceCounter(&now);
    //last_tick = now.QuadPart;


	MSG msg = {};
	while (msg.message != WM_QUIT)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
//			//ゲーム処理
//			//GameLoop();
//			CurrentTime = timeGetTime();
//
//            DeltaTime = (CurrentTime - ExecLastTime);
//
//			//fpsの計測
//			if ((CurrentTime - FPSLastTime) >= 500)	// 0.5秒ごとに実行
//			{
//				g_CountFps = FrameCount * 1000 / (CurrentTime - FPSLastTime);
//				FPSLastTime = CurrentTime;
//				FrameCount = 0;
//			}
//
//			//ゲームの処理
//			if (DeltaTime >= (1000 / FRAME_RATE))	// 60fpsでフレーム管理
//			{
//				ExecLastTime = CurrentTime;
//
//#ifdef _DEBUG
//				//std::stringstream caption = {};
//				//caption << WINDOW_CAPTION << " FPS:" << g_CountFPS;
//				char t[256] = {};
//				sprintf_s(t, "FPS: %d", g_CountFps);
//				SetWindowTextA(hWnd_Main[0], t);
//#endif
//				/*{
//					MouseState_tag ms = {};
//					Mouse_GetState(&ms);
//					if (ms.middleButton)
//					{
//						MoveWindow(
//							hWnd_Main,
//							ms.x,
//							ms.y,
//							winRect.right,
//							winRect.bottom,
//							false
//						);
//					}
//				}*/
//
//				//ゲーム処理
//				DX_GameLoop();
//
//				FrameCount++;
//			}


            QueryPerformanceCounter(&now);

            // Time更新
            LONGLONG elapsed_ticks = now.QuadPart - last_tick;

            float elapsed_sec = (float)elapsed_ticks / frequency;
            delta_time = std::min(elapsed_sec, max_delta_time); // 最大dtでClampする

            if (delta_time > 1.0f / FRAME_RATE)
            {   
                //開始時間を記録
                LARGE_INTEGER start_event_time;
                QueryPerformanceCounter(&start_event_time);


                last_tick = now.QuadPart;

                g_CountFps = 1.0f / delta_time;
                int drawFps = g_CountFps / (g_GameDrawFps + g_GameDrawAutoFpsAddWaitF);

#ifdef _DEBUG
                char t[256] = {};
                sprintf_s(t, "UpdateFPS: %d | DrawFPS: %d | OneFrameTime: %.4f sec | Late: %.4f sec",
                    g_CountFps, 
                    drawFps,
                    game_loop_duration, g_game_loop_amount_of_delay);
                SetWindowTextA(hWnd_Main[0], t);
#endif
                _AppEventUpdate();

                DX_GameLoop();

                FrameCount++;

                //終了時間を記録
                LARGE_INTEGER end_event_time;
                QueryPerformanceCounter(&end_event_time);
                game_loop_duration = (float)(end_event_time.QuadPart - start_event_time.QuadPart) / frequency;
                
                //フレームレートとの差分を計算
                g_game_loop_amount_of_delay += game_loop_duration - (1.0f / FRAME_RATE);
                g_game_loop_amount_of_delay = std::min(std::max(g_game_loop_amount_of_delay, 0.0f), 1.0f);

                if (g_GameDrawAutoFps)
                {
                    if (g_game_loop_amount_of_delay > 0.0f)
                    {
                        if (drawFps > FRAME_RATE / 6)
                        {
                            g_GameDrawAutoFpsAddWaitF +=
                                (int)(g_game_loop_amount_of_delay / (1.0f / FRAME_RATE))
                                + 1;

                            g_GameDrawWaitF = g_GameDrawFps - 1 + g_GameDrawAutoFpsAddWaitF;
                        }
                        g_GameDrawAutoFpsAddWaitFCooldown = FRAME_RATE / 10;
                    }
                    else
                    {
                        if (g_GameDrawAutoFpsAddWaitFCooldown > 0)
                        {
                            g_GameDrawAutoFpsAddWaitFCooldown--;
                        }
                        else if (g_GameDrawAutoFpsAddWaitF > 0)
                        {
                            g_GameDrawAutoFpsAddWaitF--;
                        }
                    }
                }
                else
                {
                    g_GameDrawAutoFpsAddWaitF = 0;
                }

                if (g_GameDrawWaitF > 0)
                {
                    g_GameDrawThroughFlag = true;

                    g_GameDrawWaitF--;
                }
                else
                {
                    g_GameDrawThroughFlag = false;

                    if (g_GameDrawFps > 0)
                    {
                        g_GameDrawWaitF = g_GameDrawFps - 1 + g_GameDrawAutoFpsAddWaitF;
                    }
                }
            }
		}
	}

	Mouse_Finalize();

	//終了処理
	DX_GameFinalize();

    _AppEventUninit();

    CoUninitialize();

	//_CrtDumpMemoryLeaks();

	return (int)msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg) {
		//キーボード
	case WM_ACTIVATEAPP:
	case WM_KEYDOWN:
	case WM_SYSKEYDOWN:
	case WM_KEYUP:
	case WM_SYSKEYUP:
		Keyboard_ProcessMessage(uMsg, wParam, lParam);
		break;
	}

	switch (uMsg) {

		//マウス
	case WM_ACTIVATEAPP:
	case WM_INPUT:
	case WM_MOUSEMOVE:
	case WM_LBUTTONDOWN:
	case WM_LBUTTONUP:
	case WM_RBUTTONDOWN:
	case WM_RBUTTONUP:
	case WM_MBUTTONDOWN:
	case WM_MBUTTONUP:
	case WM_MOUSEWHEEL:
	case WM_XBUTTONDOWN:
	case WM_XBUTTONUP:
	case WM_MOUSEHOVER:
		Mouse_ProcessMessage(uMsg, wParam, lParam);
		break;

	}

	switch (uMsg) {

    case WM_SIZE:
    {
        int width = LOWORD(lParam);  // 新しい幅
        int height = HIWORD(lParam); // 新しい高さ
        // ここでサイズ変更時の処理を行う
        //DXUpdateViewport(width, height);
        DXUpdateWindowSize(width, height);

        //{
        //    RECT rect;
        //    GetClientRect(hWnd_Main[0], &rect);
        //    float aspectRatio = (float)SCREEN_WIDTH / SCREEN_HEIGHT;
        //    int newHeight = static_cast<int>(rect.right / aspectRatio);

        //    SetWindowPos(hWnd_Main[0], NULL, rect.left, rect.top, rect.right, newHeight, SWP_NOZORDER | SWP_NOMOVE);
        //}
    }
        break;

	/*case WM_CREATE:

		return 0;*/

	//case WM_NCHITTEST: {
	//	LRESULT hit = DefWindowProc(hWnd, uMsg, wParam, lParam);
	//	if (hit == HTCLIENT) {
	//		return HTCAPTION; // クライアント領域をドラッグ可能にする
	//	}
	//	return hit;
	//}
	//case WM_MOUSEWHEEL: {
	//	return HTCAPTION; // クライアント領域をドラッグ可能にする
	//}

	case WM_KEYDOWN:
		//if (wParam == VK_SPACE)
		//{
		//	std::string test0 = "a";
		//	std::string test1 = "b";
		//	std::string test2 = test0 + test1;
		//	TextOut(hdc, 10, 10, test2.c_str(), 10);
		//	//MessageBox(hWnd, "a", "b", MB_OKCANCEL | MB_DEFBUTTON2);
		//}

		if (wParam == VK_ESCAPE)
		{
			SendMessage(hWnd, WM_CLOSE, 0, 0);
		}
		break;
	case WM_CLOSE:
		if (MessageBox(hWnd, "終了しますか?", "確認", MB_OKCANCEL | MB_DEFBUTTON2) == IDOK)
		{
			DestroyWindow(hWnd);
		}
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}

	//処理していないものを肩代わり
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}
LRESULT CALLBACK WndProcSub(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    //処理していないものを肩代わり
    return DefWindowProc(hWnd, uMsg, wParam, lParam);
}


//
// エディタウィンドウの処理
//
LRESULT CALLBACK WndProcEdit(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	//static const char* comboitem[] = { "Apple", "Peach", "Banana" };

	HDC hdc;
	PAINTSTRUCT ps;

	switch (uMsg) {
	case WM_CREATE:
	{
		int menuNum = 1;
		editorWindowHWND.addMapTextBox = CreateWindowA(
			"EDIT",
			"",
			WS_CHILD | WS_VISIBLE | WS_BORDER | ES_LEFT,
			200, 2, 300, 20,
			hWnd, (HMENU)EditorWindowHMENU_addMapTextBox, ((LPCREATESTRUCT)(lParam))->hInstance, NULL); menuNum++;
		editorWindowHWND.addMapAddBtn = CreateWindowA(
			"BUTTON",
			"ADD",
			WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
			440, 25, 60, 20,
			hWnd, (HMENU)EditorWindowHMENU_addMapAddBtn, ((LPCREATESTRUCT)(lParam))->hInstance, NULL); menuNum++;

		editorWindowHWND.mapSelectList = CreateWindowA(
			"COMBOBOX", NULL,
			WS_CHILD | WS_VISIBLE | WS_VSCROLL | CBS_DROPDOWNLIST,
			200, 200, 300, 300,
			hWnd, (HMENU)EditorWindowHMENU_mapSelectList, ((LPCREATESTRUCT)(lParam))->hInstance, NULL); menuNum++;

		editorWindowHWND.blockSelectList = CreateWindowA(
			"COMBOBOX", NULL,
			WS_CHILD | WS_VISIBLE | WS_VSCROLL | CBS_DROPDOWNLIST,
			200, 300, 300, 300,
			hWnd, (HMENU)EditorWindowHMENU_blockSelectList, ((LPCREATESTRUCT)(lParam))->hInstance, NULL); menuNum++;

		editorWindowHWND.objSelectList = CreateWindowA(
			"COMBOBOX", NULL,
			WS_CHILD | WS_VISIBLE | WS_VSCROLL | CBS_DROPDOWNLIST,
			200, 330, 300, 300,
			hWnd, (HMENU)EditorWindowHMENU_objSelectList, ((LPCREATESTRUCT)(lParam))->hInstance, NULL); menuNum++;

		editorWindowHWND.setModeSelectBlockBtn = CreateWindowA(
			"BUTTON", "BLOCK",
			WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON | WS_GROUP,
			200, 360, 100, 20,
			hWnd, (HMENU)EditorWindowHMENU_setModeSelectBlockBtn, ((LPCREATESTRUCT)(lParam))->hInstance, NULL);
		editorWindowHWND.setModeSelectObjBtn = CreateWindowA(
			"BUTTON", "OBJECT",
			WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON,
			300, 360, 100, 20,
			hWnd, (HMENU)EditorWindowHMENU_setModeSelectObjBtn, ((LPCREATESTRUCT)(lParam))->hInstance, NULL);
		//先頭をオンにしておく
		SendMessage(editorWindowHWND.setModeSelectBlockBtn, BM_SETCHECK, BST_CHECKED, 0);

		editorWindowHWND.viewSizeCheckBox = CreateWindowA(
			"BUTTON", "VIEW SIZE",
			WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX,
			3, 500, 100, 20,
			hWnd, (HMENU)EditorWindowHMENU_viewSizeCheckBox, ((LPCREATESTRUCT)(lParam))->hInstance, NULL);
	}
		return 0;

	case WM_PAINT: //ウィンドウの描画
	{
		hdc = BeginPaint(hWnd, &ps);

		{ char txt[] = "MAP ADD"; TextOut(hdc, 2, 2, txt, lstrlen(txt)); }
		{ char txt[] = "MAP SELECT"; TextOut(hdc, 3, 200, txt, lstrlen(txt)); }
		{ char txt[] = "SET BLOCK"; TextOut(hdc, 3, 300, txt, lstrlen(txt)); }
		{ char txt[] = "SET OBJECT"; TextOut(hdc, 3, 330, txt, lstrlen(txt)); }
		{ char txt[] = "SET MODE"; TextOut(hdc, 3, 360, txt, lstrlen(txt)); }

		EndPaint(hWnd, &ps);
	}
		break;

	case WM_COMMAND:
	{
		OnWindowEventEditor(LOWORD(wParam), HIWORD(wParam));
		/*if (editorEventFunc != nullptr)
		{
			editorEventFunc(LOWORD(wParam), HIWORD(wParam));
		}*/
	}
		return 0;
	case WM_NCHITTEST: {
		LRESULT hit = DefWindowProc(hWnd, uMsg, wParam, lParam);
		if (hit == HTCLIENT) {
			return HTCAPTION; // クライアント領域をドラッグ可能にする
		}
		return hit;
	}
	case WM_KEYDOWN:
		if (wParam == VK_ESCAPE)
		{
			SendMessage(hWnd, WM_CLOSE, 0, 0);
		}
		break;
	case WM_CLOSE:
		if (MessageBox(hWnd, "end?", "kakunin", MB_OKCANCEL | MB_DEFBUTTON2) == IDOK)
		{
			DestroyWindow(hWnd);
		}
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	//処理していないものを肩代わり
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}




//int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
//{
//	//MessageBox(NULL, txt, "QUESTION", MB_YESNO | MB_DEFBUTTON3 | MB_ICONQUESTION);
//	UNREFERENCED_PARAMETER(hInstance);
//
//	DebugPrintf("hInstanceの値　%p\n");
//	MessageBox(NULL, "txt", "QUESTION", MB_YESNO | MB_DEFBUTTON3 | MB_ICONQUESTION);
//
//	return 0;
//}


//HINSTANCE Get_hInstance(void)
//{
//	return hInstanceSave;
//}
//
//int Get_nCmdShow(void)
//{
//	return nCmdShowSave;
//}

void EditorWindowShow(bool bl)
{
	ShowWindow(hWnd_Edit, bl ? SW_SHOW:SW_HIDE);
}

EditorWindowHWND EditorGetWindowHWND(void)
{
	return editorWindowHWND;
}

HWND GetMainHWND(void)
{
	return hWnd_Main[0];
}



//void GetIconBitmap(HICON hIcon) {
//    ICONINFO iconInfo;
//    if (GetIconInfo(hIcon, &iconInfo)) {
//        // iconInfo.hbmColor にアイコンのビットマップが格納されている
//        // 必要に応じて `GetObject()` でサイズを取得
//        BITMAP bmp;
//        GetObject(iconInfo.hbmColor, sizeof(BITMAP), &bmp);
//        // ここで `bmp.bmWidth` や `bmp.bmHeight` を取得可能
//        DeleteObject(iconInfo.hbmColor);
//        DeleteObject(iconInfo.hbmMask);
//    }
//}

void LoadProgressPaint(size_t cur, size_t max, std::string txt, int type)
{
    if (type == 2)
    {
        cur = max;
    }
    //SendMessage(hWnd_Main[0], WM_PAINT, cur, max);

    HWND hwnd = GetMainHWND();

    // ウィンドウサイズを取得
    RECT clientRect;
    GetClientRect(hwnd, &clientRect);
    int width = clientRect.right;
    int height = clientRect.bottom;

    HDC hdc = GetDC(hwnd);

    HDC hdcMem0;
    HBITMAP hbmMem0;
    // メモリDCを作成
    hdcMem0 = CreateCompatibleDC(hdc);
    hbmMem0 = CreateCompatibleBitmap(hdc, width, height);
    ::SelectObject(hdcMem0, hbmMem0);

    // 画面へ転送
    BitBlt(hdcMem0, 0, 0, width, height, hdc, 0, 0, SRCCOPY);


    SelectObject(hdcMem0, GetStockObject(BLACK_BRUSH));
    // 長方形を描画
    Rectangle(hdcMem0,
        0,
        0,
        width,
        height
    );

    float mod = ((float)cur / max);
    float circR = height * 0.25f;
    float circW = height * 0.01f;

    //SelectObject(hdcMem0, GetStockObject(DKGRAY_BRUSH));
    //Ellipse(hdcMem0,
    //    width  / 2.0f - circR,
    //    height / 2.0f - circR,
    //    width  / 2.0f + circR,
    //    height / 2.0f + circR
    //);

    {
        HPEN hPen = CreatePen(PS_SOLID, circW, RGB(60, 60, 60));

        SelectObject(hdcMem0, hPen);
        Arc(
            hdcMem0,   // 描画対象のデバイスコンテキスト
            width / 2.0f - circR,    // 外接する矩形の左上隅の x 座標
            height / 2.0f - circR,    // 外接する矩形の左上隅の y 座標
            width / 2.0f + circR,    // 外接する矩形の右下隅の x 座標
            height / 2.0f + circR,    // 外接する矩形の右下隅の y 座標
            width / 2.0f,    // 円弧の始点を定義する放射状線の終点の x 座標
            height / 2.0f,    // 円弧の始点を定義する放射状線の終点の y 座標
            width / 2.0f,    // 円弧の終点を定義する放射状線の終点の x 座標
            height / 2.0f     // 円弧の終点を定義する放射状線の終点の y 座標
        );
        DeleteObject(hPen);
    }
    {
        //HBRUSH hBrush = CreateSolidBrush(RGB(40, 210, 240));
        HPEN hPen = CreatePen(PS_DASH, circW, RGB(40, 210, 240));


        //SelectObject(hdcMem0, hBrush);
        SelectObject(hdcMem0, hPen);
        Arc(
            hdcMem0,   // 描画対象のデバイスコンテキスト
            width / 2.0f - circR,    // 外接する矩形の左上隅の x 座標
            height / 2.0f - circR,    // 外接する矩形の左上隅の y 座標
            width / 2.0f + circR,    // 外接する矩形の右下隅の x 座標
            height / 2.0f + circR,    // 外接する矩形の右下隅の y 座標
            width / 2.0f,    // 円弧の始点を定義する放射状線の終点の x 座標
            height / 2.0f - circR,    // 円弧の始点を定義する放射状線の終点の y 座標
            width / 2.0f + (int)(sin(2.0 * SYS_GENERAL_PI * mod - SYS_GENERAL_PI) * circR),    // 円弧の終点を定義する放射状線の終点の x 座標
            height / 2.0f + (int)(cos(2.0 * SYS_GENERAL_PI * mod - SYS_GENERAL_PI) * circR)     // 円弧の終点を定義する放射状線の終点の y 座標
        );
        DeleteObject(hPen);
        //DeleteObject(hBrush);
    }

    if (txt.size() > 0)
    {
        {
            // テキストの幅を取得
            SIZE textSize;
            GetTextExtentPoint32(hdc, txt.c_str(), lstrlen(txt.c_str()), &textSize);

            // 中央座標を計算
            int x = (clientRect.right - textSize.cx) / 2;
            int y = (clientRect.bottom - textSize.cy) / 2;



            // 文字色と背景色を設定
            SetTextColor(hdcMem0, RGB(255, 255, 255)); // 赤色
            SetBkColor(hdcMem0, RGB(0, 0, 0)); // 白背景

            // 文字を描画
            TextOut(hdcMem0,
                x,
                y - height * 0.02f,
                TEXT(txt.c_str()),
                txt.size()
            );
        }

        {
            std::string ptxt = std::to_string((int)(((float)cur / max) * 100)) + "%";

            // テキストの幅を取得
            SIZE textSize;
            GetTextExtentPoint32(hdc, ptxt.c_str(), lstrlen(ptxt.c_str()), &textSize);

            // 中央座標を計算
            int x = (clientRect.right - textSize.cx) / 2;
            int y = (clientRect.bottom - textSize.cy) / 2;


            // 文字を描画
            TextOut(hdcMem0,
                x,
                y + height * 0.02f,
                TEXT(ptxt.c_str()),
                ptxt.size()
            );
        }
    }

    {
        HICON hIcon = (HICON)LoadImage(hInstanceSave,
            MAKEINTRESOURCE(IDI_ICON1),
            IMAGE_ICON,
            GetSystemMetrics(SM_CXICON),
            GetSystemMetrics(SM_CYICON),
            0); // アイコンを取得

        ICONINFO iconInfo;
        GetIconInfo(hIcon, &iconInfo);

        HDC hdcMem = CreateCompatibleDC(hdcMem0);
        SelectObject(hdcMem, iconInfo.hbmColor);

        int sw = height * 0.08;
        int sh = height * 0.08;

        // 回転角度（ラジアン）
        float angle = CalculateDegToRad(-360.0f * mod);

        // 回転後の座標を計算
        POINT points[3];
        points[0].x = width  / 2.0f + (int)(sin(2.0 * SYS_GENERAL_PI * mod - SYS_GENERAL_PI) * (circR + circW + sw * 0.6));
        points[0].y = height / 2.0f + (int)(cos(2.0 * SYS_GENERAL_PI * mod - SYS_GENERAL_PI) * (circR + circW + sh * 0.6));
        points[1].x = points[0].x + (int)(cos(angle) * sw);
        points[1].y = points[0].y + (int)(sin(angle) * sw);
        points[2].x = points[0].x + (int)(-sin(angle) * sh);
        points[2].y = points[0].y + (int)(cos(angle) * sh);

        // `PlgBlt` を使用して画像を回転
        PlgBlt(hdcMem0, points, hdcMem, 0, 0, 64, 64, NULL, 0, 0);

        DeleteDC(hdcMem);
        DeleteObject(iconInfo.hbmColor);
        DeleteObject(iconInfo.hbmMask);
    }
    
    //SelectObject(hdcMem0, GetStockObject(BLACK_BRUSH));
    //Ellipse(hdcMem0,
    //    width  / 2.0f - circR * 0.95f,
    //    height / 2.0f - circR * 0.95f,
    //    width  / 2.0f + circR * 0.95f,
    //    height / 2.0f + circR * 0.95f
    //);

    //{
    //    SelectObject(hdcMem0, GetStockObject(DKGRAY_BRUSH));
    //    // 長方形を描画
    //    Rectangle(hdcMem0,
    //        width * 0.05f,
    //        height * 0.8f,
    //        width * 0.95f,
    //        height * 0.815f
    //    );
    //}

    //{
    //    HBRUSH hBrush = CreateSolidBrush(RGB(40, 210, 240));

    //    SelectObject(hdcMem0, hBrush);
    //    //SelectObject(hdc, GetStockObject(WHITE_BRUSH));
    //    // 長方形を描画
    //    Rectangle(hdcMem0,
    //        width * 0.05f,
    //        height * 0.8f,
    //        width * 0.05f + width * ((float)cur / max) * 0.9f,
    //        height * 0.815f
    //    );
    //    DeleteObject(hBrush);
    //}

    //if (txt.size() > 0)
    //{
    //    // 文字色と背景色を設定
    //    SetTextColor(hdcMem0, RGB(255, 255, 255)); // 赤色
    //    SetBkColor(hdcMem0, RGB(0, 0, 0)); // 白背景

    //    // 文字を描画
    //    TextOut(hdcMem0,
    //        //width * 0.05f,
    //        //height * 0.75f,
    //        width * 0.125f,
    //        height * 0.815f,
    //        TEXT(txt.c_str()),
    //        txt.size()
    //    );

    //    std::string ptxt = std::to_string((int)(((float)cur / max) * 100)) + "%";
    //    // 文字を描画
    //    TextOut(hdcMem0,
    //        width * 0.05f,
    //        height * 0.815f,
    //        TEXT(ptxt.c_str()),
    //        ptxt.size()
    //    );
    //    //std::wstring wtext(text.begin(), text.end()); // UTF-16へ変換
    //    //TextOutW(
    //    //    hdc, x, y,
    //    //    wtext.c_str(), 
    //    //    wtext.length()
    //    //);

    //}

    //{
    //    HICON hIcon = //LoadIcon(NULL, MAKEINTRESOURCE(IDI_ICON1));
    //    (HICON)LoadImage(hInstanceSave,
    //        MAKEINTRESOURCE(IDI_ICON1),
    //        IMAGE_ICON,
    //        GetSystemMetrics(SM_CXICON),
    //        GetSystemMetrics(SM_CYICON),
    //        0); // アイコンを取得
    //    ////DrawIcon(hdc,
    //    ////    width * 0.05f + width * ((float)cur / max) * 0.9f - 32,
    //    ////    height * 0.65,
    //    ////    hIcon); // 指定座標に描画
    //    //DrawIconEx(hdc,
    //    //    width * 0.05f + width * ((float)cur / max) * 0.9f - 32,
    //    //    height * 0.65,
    //    //    hIcon,
    //    //    height * 0.05f,
    //    //    height * 0.05f,
    //    //    0, NULL, DI_IMAGE | DI_MASK);

    //    ICONINFO iconInfo;
    //    GetIconInfo(hIcon, &iconInfo);

    //    HDC hdcMem = CreateCompatibleDC(hdcMem0);
    //    SelectObject(hdcMem, iconInfo.hbmColor);

    //    int sw = height * -0.08;
    //    int sh = height * 0.08;
    //    // 水平方向に反転
    //    StretchBlt(hdcMem0,
    //        width * 0.05f + ((float)cur / max) * width * 0.9 + abs(sw) / 2,
    //        height * 0.75,
    //        sw,
    //        sh,

    //        hdcMem, 

    //        0,
    //        0, 
    //        64,
    //        64,

    //        SRCCOPY);

    //    DeleteDC(hdcMem);
    //    DeleteObject(iconInfo.hbmColor);
    //    DeleteObject(iconInfo.hbmMask);
    //}


    // 円を描画
    //Ellipse(hdc, 250, 50, 400, 200);

    // 最後に画面へ転送
    BitBlt(hdc, 0, 0, width, height, hdcMem0, 0, 0, SRCCOPY);

    // メモリDCとビットマップを削除
    if (hbmMem0 != nullptr)
    {
        DeleteObject(hbmMem0);
    }
    if (hdcMem0 != nullptr)
    {
        DeleteDC(hdcMem0);
    }
    ReleaseDC(hwnd, hdc);
}

float GetGameLoopAmountOfDelay(void)
{
    return g_game_loop_amount_of_delay;
}
void SetWinMainLoopDrawFPS(int n)
{
    g_GameDrawFps = std::max(n, 1);
}

void SetWinMainLoopDrawAutoFPSEnable(bool b)
{
    g_GameDrawAutoFps = b;
}

bool GetWinMainLoopDrawThroughFlag(void)
{
    return g_GameDrawThroughFlag;
}

void SetWinMainStartGame()
{
    //SetWindowLong(hWnd_Main[0], GWL_STYLE, WINDOW_STYLE_DEFAULT);
    //SetWindowPos(hWnd_Main[0], NULL, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE | SWP_FRAMECHANGED);
}

