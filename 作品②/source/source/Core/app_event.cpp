
//---------------------------------------------------------------------------
// app_event.cpp
// 
// sougo hara  2025/05/08
//---------------------------------------------------------------------------

//#include "../Game/Configuration/configuration.h"
#include "../main.h"
#include "../game_main.h"
#include "app_event.h"

#include <shobjidl.h>
#include <thread>
#include <atomic>


static HWND g_hwnd_save = nullptr;

//static std::atomic<bool> g_LoadfilesRunning(false);
//
//static std::atomic<size_t> g_LoadfilesMax(0);
//static std::atomic<size_t> g_LoadfilesCur(0);
//static std::string g_LoadfilesTxt;
//static std::atomic<std::string*> g_LoadfilesTxtPointer(&g_LoadfilesTxt);
//static std::atomic<int> g_LoadfilesType(0);

static size_t g_LoadfilesMax = 0;
static size_t g_LoadfilesCur = 0;
//static std::atomic<size_t> g_LoadfilesCur(0);

static ITaskbarList3* g_pTaskbar = nullptr;

//void _SetTaskBarState(TBPFLAG tbpFlags)
//{
//    ITaskbarList3* pTaskbar = nullptr;
//    HRESULT hr = CoCreateInstance(CLSID_TaskbarList, NULL, CLSCTX_ALL, IID_PPV_ARGS(&pTaskbar));
//    if (SUCCEEDED(hr)) {
//        HWND hwnd = g_hwnd_save;
//
//        pTaskbar->SetProgressState(hwnd, tbpFlags); // 進行状態解除
//
//
//        pTaskbar->Release();
//    }
//}
//
//void _SetTaskBarProgress(int Now, int Max)
//{
//    ITaskbarList3* pTaskbar = nullptr;
//    HRESULT hr = CoCreateInstance(CLSID_TaskbarList, NULL, CLSCTX_ALL, IID_PPV_ARGS(&pTaskbar));
//    if (SUCCEEDED(hr)) {
//        HWND hwnd = g_hwnd_save;
//
//        pTaskbar->SetProgressValue(hwnd, Now, Max);
//
//        pTaskbar->Release();
//    }
//}

void _SetTaskBarCreateInstance()
{
    HRESULT hr = CoCreateInstance(CLSID_TaskbarList, NULL, CLSCTX_ALL, IID_PPV_ARGS(&g_pTaskbar));
    if (SUCCEEDED(hr)) {
    }
}
void _SetTaskBarState(TBPFLAG tbpFlags)
{
    if (g_pTaskbar != nullptr && g_hwnd_save != nullptr)
    {
        g_pTaskbar->SetProgressState(g_hwnd_save, tbpFlags); // 進行状態解除
    }
}

void _SetTaskBarProgress(int Now, int Max)
{
    if (g_pTaskbar != nullptr && g_hwnd_save != nullptr)
    {
        g_pTaskbar->SetProgressValue(g_hwnd_save, Now, Max);
    }
}

void _AppEventInit()
{
    //g_hwnd_save = (HWND)hwnd;
    g_hwnd_save = GetMainHWND();

    _SetTaskBarCreateInstance();
}
void _AppEventUpdate()
{
}
void _AppEventUninit()
{
    if (g_pTaskbar != nullptr)
    {
        g_pTaskbar->Release();
    }
}



//void _AppEventFileLoadWork()
//{
//    LONGLONG last_tick = 0;
//    LONGLONG frequency = 0;
//    float delta_time = 0;
//    float max_delta_time = 0.1f;
//
//    // Timer関連初期化
//    LARGE_INTEGER freq;
//    QueryPerformanceFrequency(&freq);
//    frequency = freq.QuadPart;
//
//    LARGE_INTEGER now;
//
//    while (g_LoadfilesRunning)
//    {
//        QueryPerformanceCounter(&now);
//
//        // Time更新
//        LONGLONG elapsed_ticks = now.QuadPart - last_tick;
//
//        float elapsed_sec = (float)elapsed_ticks / frequency;
//        delta_time = std::min(elapsed_sec, max_delta_time); // 最大dtでClampする
//
//        if (delta_time > 1.0f / FRAME_RATE)
//        {
//            LoadProgressPaint(
//                g_LoadfilesCur, 
//                g_LoadfilesMax, 
//                *g_LoadfilesTxtPointer, 
//                g_LoadfilesType);
//        }
//    }
//    
//}

void AppEventFileLoadStartBefore()
{
    _SetTaskBarState(TBPF_INDETERMINATE);
}
void AppEventFileLoadStart(int maxnum)
{
    g_LoadfilesCur = 0;
    g_LoadfilesMax = maxnum;

    _SetTaskBarState(TBPF_NORMAL);
    _SetTaskBarProgress(g_LoadfilesCur, g_LoadfilesMax);

    //g_LoadfilesRunning = true;
    //std::thread th(_AppEventFileLoadWork);
    //th.detach(); // メインスレッドとは独立して動作
}
AppEventFileLoadAddResult AppEventFileLoadAddBefore(int num, std::string txt, int type)
{
    AppEventFileLoadAddResult r = {};
    if (g_LoadfilesMax > 0)
    {
        //g_LoadfilesTxt = txt;
        //strcpy_s(g_LoadfilesTxt, 256, txt.c_str());
        //g_LoadfilesType = type;
        LoadProgressPaint(g_LoadfilesCur, g_LoadfilesMax, txt, type);
    }
    r.loadfilesCur = g_LoadfilesCur;
    r.loadfilesMax = g_LoadfilesMax;

    return r;
}
AppEventFileLoadAddResult AppEventFileLoadAdd(int num, std::string txt, int type)
{
    AppEventFileLoadAddResult r = {};
    if (g_LoadfilesMax > 0)
    {
        g_LoadfilesCur += num;
        _SetTaskBarProgress(g_LoadfilesCur, g_LoadfilesMax);

        //g_LoadfilesTxt = txt;
        //strcpy_s(g_LoadfilesTxt, 256, txt.c_str());
        //g_LoadfilesType = type;
        LoadProgressPaint(g_LoadfilesCur, g_LoadfilesMax, txt, type);
    }
    r.loadfilesCur = g_LoadfilesCur;
    r.loadfilesMax = g_LoadfilesMax;

    return r;
}
void AppEventFileLoadEnd()
{
    g_LoadfilesMax = 0;
    _SetTaskBarState(TBPF_NOPROGRESS);
    //g_LoadfilesRunning = false;
}
