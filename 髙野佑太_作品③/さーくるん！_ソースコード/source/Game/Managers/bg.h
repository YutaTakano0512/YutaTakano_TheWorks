// ===================================================
// bg.h 背景制御
// 
// hara sougo  2024/07/30
// ===================================================

#ifndef _BG_H_
#define _BG_H_


// ===================================================
// 列挙体宣言
// ===================================================
enum BG_TYPE {
	BG_TYPE_NONE = 0,

	BG_TYPE_TEST_TITLE,
	BG_TYPE_TEST_STAGESELECT,
	BG_TYPE_TEST_GAME,
	//リザルト画面
	BG_TYPE_TEST_RESULT,
	BG_TYPE_TEST_RESULT_CLER,
	BG_TYPE_TEST_RESULT_GAMEOVER,

	BG_TYPE_ISEKI,
    BG_TYPE_ISEKI_MUTATION,

	

    BG_TYPE_TUTORIAL_0,
    BG_TYPE_TUTORIAL_1,
    BG_TYPE_GENERAL_0,

	BG_TYPE_MAX
};
// ===================================================
// 構造体宣言
// ===================================================
struct BG_DAT {		// 背景構造体
	BG_TYPE type;

	unsigned int frame;
};


// ===================================================
// プロトタイプ宣言
// ===================================================

void InitBg();
void UninitBg();
void UpdateBg();
void DrawBg();

void SetBg(BG_TYPE bg);
BG_DAT GetBg();

#endif