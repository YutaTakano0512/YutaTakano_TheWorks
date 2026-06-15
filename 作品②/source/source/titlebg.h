// ===================================================
// titlebg.h タイトル背景表示 (つかってないよ！)
// 
// hara sougo  2024/07/22
// ===================================================
#ifndef _TITLEBG_H_
#define _TITLEBG_H_

#include "Game/Systems/sys_general.h"

// ===================================================
// 構造体宣言
// ===================================================
struct TITLEBG {		// タイトル背景構造体
	Float2 pos;		// 座標
	Float2 size;	// サイズ
	bool use;		// 使用フラグ
};

// ===================================================
// プロトタイプ宣言
// ===================================================
void InitializeTitlebg(void);
void UpdateTitlebg(void);
void DrawTitlebg(void);
void FinalizeTitlebg(void);

#endif