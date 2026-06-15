//#pragma once

//---------------------------------------------------------------------------
// main.h
// 
// sougo hara  2024/05/08
//---------------------------------------------------------------------------
#ifndef _MAIN_H_
#define _MAIN_H_

#include "Game/Configuration/configuration.h"

#define DX_WINDOW_NUM (1)
#define DX_WINDOW_SCREEN_MOD (0.5)

enum EditorWindowHMENU {
	EditorWindowHMENU_addMapTextBox = 0,
	EditorWindowHMENU_addMapAddBtn,
	EditorWindowHMENU_mapSelectList,
	EditorWindowHMENU_blockSelectList,
	EditorWindowHMENU_objSelectList,

	EditorWindowHMENU_setModeSelectBlockBtn,
	EditorWindowHMENU_setModeSelectObjBtn,

	EditorWindowHMENU_viewSizeCheckBox,

	EditorWindowHMENU_MAX

};

struct EditorWindowHWND {
	HWND addMapTextBox;
	HWND addMapAddBtn;
	HWND mapSelectList;
	HWND blockSelectList;
	HWND objSelectList;

	HWND setModeSelectBlockBtn;
	HWND setModeSelectObjBtn;

	HWND viewSizeCheckBox;
};

//HINSTANCE Get_hInstance(void);
//int Get_nCmdShow(void);
void EditorWindowShow(bool bl);

EditorWindowHWND EditorGetWindowHWND(void);

HWND GetMainHWND(void);
void LoadProgressPaint(size_t cur, size_t max, std::string txt, int type);

float GetGameLoopAmountOfDelay(void);
void SetWinMainLoopDrawFPS(int n);
void SetWinMainLoopDrawAutoFPSEnable(bool b);
bool GetWinMainLoopDrawThroughFlag(void);

void SetWinMainStartGame();


#endif