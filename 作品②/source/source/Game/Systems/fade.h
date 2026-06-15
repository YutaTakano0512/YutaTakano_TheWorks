// ===================================================
// fade.h フェード制御
// 
// hara sougo  2024/07/30
// ===================================================
#ifndef _FADE_H_
#define _FADE_H_

#include "sys_general.h"
#include "easing.h"

// ===================================================
// マクロ定義
// ===================================================
#define FADE_MAX (1)

#define FADE_SPEED_DEFAULT (40.0f)
// ===================================================
// 列挙体宣言
// ===================================================

enum MODE_FADE {
	MODE_FADE_NONE = 0,
	MODE_FADE_OUT,
	MODE_FADE_IN,
	MODE_FADE_MAX
};

enum FADE_TYPE {
    FADE_TYPE_COLOR_BLACK = 0,

    FADE_TYPE_STAR_WIPE,

	FADE_TYPE_MAX
};

// ===================================================
// 構造体宣言
// ===================================================
struct FADE {		// フェード構造体
	Float2 pos;		// 座標
	Float2 size;	// サイズ
	Float4 color;	// カラー
	float alpha;	// 透過度
	float speed;// フェード速度
	MODE_FADE mode;	// 現在のフェードモード
	bool use;		// 使用フラグ

	bool isGameStop;
	FADE_TYPE type; //フェードの種類

	//暗転中に実行する関数を指定する
	std::function<void(void)> func;

	//イージング関数を指定する
	std::function<float(float)> easing;
};

// ===================================================
// プロトタイプ宣言
// ===================================================

void InitFade(void);
void UpdateFade(void);
void DrawFade(void);
void UninitFade(void);

bool SetFade(FADE_TYPE type,
	std::function<void(void)> func,
	std::function<float(float)> easing = nullptr,
	int speed = 0,
	bool isGameStop = false
);

bool FadeGetGameStop();
bool IsFading();

FADE GetFadeInfo();

#endif