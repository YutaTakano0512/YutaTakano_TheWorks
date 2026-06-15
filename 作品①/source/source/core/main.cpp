#include "main.h"
#include "manager.h"
#include "../graphics/renderer.h"

//=============================================================================
// グローバル変数
//=============================================================================
static HWND     g_hWnd       = nullptr;
static bool     g_Fullscreen = false;
static RECT     g_WindowRect = {};

HWND GetWindowHandle() { return g_hWnd; }

//=============================================================================
// ボーダレスフルスクリーン切り替え
//=============================================================================
static void ToggleBorderlessFullscreen()
{
    if (!g_Fullscreen)
    {
        GetWindowRect(g_hWnd, &g_WindowRect);
        SetWindowLongPtr(g_hWnd, GWL_STYLE, WS_POPUP | WS_VISIBLE);

        HMONITOR hMon = MonitorFromWindow(g_hWnd, MONITOR_DEFAULTTONEAREST);
        MONITORINFO mi = { sizeof(mi) };
        GetMonitorInfo(hMon, &mi);
        SetWindowPos(g_hWnd, HWND_TOP,
            mi.rcMonitor.left, mi.rcMonitor.top,
            mi.rcMonitor.right  - mi.rcMonitor.left,
            mi.rcMonitor.bottom - mi.rcMonitor.top,
            SWP_FRAMECHANGED | SWP_NOACTIVATE);
    }
    else
    {
        SetWindowLongPtr(g_hWnd, GWL_STYLE, WS_OVERLAPPEDWINDOW | WS_VISIBLE);
        SetWindowPos(g_hWnd, HWND_NOTOPMOST,
            g_WindowRect.left, g_WindowRect.top,
            g_WindowRect.right  - g_WindowRect.left,
            g_WindowRect.bottom - g_WindowRect.top,
            SWP_FRAMECHANGED | SWP_NOACTIVATE);
    }
    g_Fullscreen = !g_Fullscreen;

    // サイズ変更後にレンダラーに通知してレターボックスを再計算
    RECT clientRect = {};
    GetClientRect(g_hWnd, &clientRect);
    Renderer::OnWindowResize(
        static_cast<UINT>(clientRect.right  - clientRect.left),
        static_cast<UINT>(clientRect.bottom - clientRect.top));
}

//=============================================================================
// ウィンドウプロシージャ
//=============================================================================
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_DESTROY:
        PostQuitMessage(0);
        break;

    case WM_SIZE:
        // ウィンドウの手動リサイズに対応 (最小化時は 0x0 なのでガードする)
        if (wParam != SIZE_MINIMIZED)
        {
            UINT w = LOWORD(lParam);
            UINT h = HIWORD(lParam);
            if (w > 0 && h > 0)
                Renderer::OnWindowResize(w, h);
        }
        break;

    case WM_KEYDOWN:
        switch (wParam)
        {
        case VK_F11:
            ToggleBorderlessFullscreen();
            break;
        case VK_ESCAPE:
            DestroyWindow(hWnd);
            break;
        }
        break;

    default:
        break;
    }
    return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

//=============================================================================
// WinMain
//=============================================================================
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int nCmdShow)
{
    // ウィンドウクラス登録
    WNDCLASSEX wcex = {};
    wcex.cbSize        = sizeof(WNDCLASSEX);
    wcex.style         = 0;
    wcex.lpfnWndProc   = WndProc;
    wcex.hInstance     = hInstance;
    wcex.hCursor       = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = nullptr;
    wcex.lpszClassName = CLASS_NAME;
    RegisterClassEx(&wcex);

    // クライアント領域が SCREEN_WIDTH x SCREEN_HEIGHT になるよう調整
    RECT rc = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
    AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);

    g_hWnd = CreateWindowEx(
        0, CLASS_NAME, WINDOW_NAME,
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT,
        rc.right - rc.left, rc.bottom - rc.top,
        nullptr, nullptr, hInstance, nullptr);

    if (!g_hWnd) return -1;

    CoInitializeEx(nullptr, COINIT_MULTITHREADED);

    Manager::Init();

    ShowWindow(g_hWnd, nCmdShow);
    UpdateWindow(g_hWnd);

    // タイマー精度を1msに
    timeBeginPeriod(1);
    DWORD lastTime    = timeGetTime();
    DWORD currentTime = 0;

    MSG msg = {};
    while (true)
    {
        if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
        {
            if (msg.message == WM_QUIT) break;
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        else
        {
            currentTime = timeGetTime();
            if ((currentTime - lastTime) >= (1000 / 60))
            {
                lastTime = currentTime;
                Manager::Update();
                Manager::Draw();
            }
        }
    }

    timeEndPeriod(1);
    Manager::Uninit();
    CoUninitialize();
    UnregisterClass(CLASS_NAME, hInstance);

    return static_cast<int>(msg.wParam);
}
