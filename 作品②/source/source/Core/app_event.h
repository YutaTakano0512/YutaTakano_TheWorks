//---------------------------------------------------------------------------
// app_event.h
// 
// sougo hara  2025/05/08
//---------------------------------------------------------------------------
#ifndef _MY_APP_EVENT_H_
#define _MY_APP_EVENT_H_


//
// 構造体宣言
//
struct AppEventFileLoadAddResult {
    size_t loadfilesMax;
    size_t loadfilesCur;
};

//
// プロトタイプ宣言
//
void _AppEventInit();
void _AppEventUpdate();
void _AppEventUninit();

void AppEventFileLoadStartBefore();
void AppEventFileLoadStart(int maxnum);
AppEventFileLoadAddResult AppEventFileLoadAddBefore(int num, std::string txt, int type);
AppEventFileLoadAddResult AppEventFileLoadAdd(int num = 1, std::string txt="", int type=0);
void AppEventFileLoadEnd();

#endif