//---------------------------------------------------------------------------
// dx_game.h
// 
// sougo hara  2024/05/08
//---------------------------------------------------------------------------
#ifndef _DX_GAME_H_
#define _DX_GAME_H_

//
// 構造体宣言
//
struct GAME_INIT_SET { //ゲーム初期化時に受け渡しするデータ
	HWND* hWnd;
};

//
// プロトタイプ宣言
//
void DX_GameInitialize(GAME_INIT_SET set);
void DX_GameFinalize(void);

void DX_GameUpdate(void);
void DX_GameDraw(void);


void DX_GameLoop(void);


unsigned int DX_GameGetFrame();

#endif