// ===================================================
// font.h フォントの制御
// 
// hara sougo  2024/11/18
// ===================================================
#ifndef _FONT_H_
#define _FONT_H_

#include "sys_general.h"

#include "../../Core/_systemModeSwitch.h"

#ifdef SWITCH_MODE
#else

#include "../../Core/DirectX/dx_font.h"

#endif

// ===================================================
// フォントの種類を宣言
// ===================================================
enum FONT_TYPE {
	FONT_TYPE_DEFAULT = 0,
    FONT_TYPE_KANJI,
    FONT_TYPE_STICK,

	FONT_TYPE_MAX,
};

//インライン関数
inline void FontDataSet(std::string* t)
{
	t[FONT_TYPE_DEFAULT]/*						*/ = "rom/font/Kazesawa-Extrabold.ttf";
    t[FONT_TYPE_KANJI]/*						*/ = "rom/font/tamanegi-v7re.ttf";
    t[FONT_TYPE_STICK]/*						*/ = "rom/font/Stick-Regular.ttf";
}


// ===================================================
// 列挙体定義
// ===================================================
enum FONT_TEXT_ALIGNMENT
{
    FONT_TEXT_ALIGNMENT_LEFT = 0,
    FONT_TEXT_ALIGNMENT_RIGHT,
    FONT_TEXT_ALIGNMENT_CENTER,
};
enum FONT_TEXT_COLOR
{
    FONT_TEXT_COLOR_NONE = -1,
    FONT_TEXT_COLOR_WHITE = 0,
    FONT_TEXT_COLOR_BLACK,
    FONT_TEXT_COLOR_RED,
    FONT_TEXT_COLOR_ORANGE,
    FONT_TEXT_COLOR_YELLOW,
    FONT_TEXT_COLOR_GREEN,
    FONT_TEXT_COLOR_LIGHTBLUE,
    FONT_TEXT_COLOR_BLUE,
    FONT_TEXT_COLOR_PURPLE,

    FONT_TEXT_COLOR_MAX
};

// ===================================================
// 構造体宣言
// ===================================================
struct DRAW_FONT_DAT {
	std::wstring txt;
	FONT_TYPE fonttype;
    Float2 pos;
    Float2 posShadow;
	int fontSize;
	//Float4 color;
    FONT_TEXT_COLOR color;
    FONT_TEXT_COLOR colorShadow;
	FONT_TEXT_ALIGNMENT textAlign;

    float maxWidth;
    float maxHight;

    float LineSpacingWidth;
    float LineSpacingHight;

    std::vector<DX_DrawString_SetSize> sizeEx;
    std::vector<DX_DrawString_SetDrawingEffect> drawingEffect;
	//bool shadow;
	//Float4 shadowColor;
	//Float2 shadowPos;
};

// ===================================================
// プロトタイプ宣言
// ===================================================

#ifdef IS_MAIN
size_t InitFontDataFiles();
//メイン以外からは呼び出せないようにしておく
//メインから呼び出すのを忘れずに
void InitFont(void);
void UninitFont(void);

//保存した書き込み情報を出力
void DrawFontBufferRelease(void);
void DrawFontBufferReleaseOnly(void);
#endif

ID2D1SolidColorBrush* GetFontColorBrush(FONT_TEXT_COLOR c);

void DrawFont(std::string txt, FONT_TYPE fonttype,
	Float2 pos,
	int fontSize,
    FONT_TEXT_COLOR color,
	FONT_TEXT_ALIGNMENT textAlign
);
void DrawFont(std::wstring txt, FONT_TYPE fonttype,
	Float2 pos,
	int fontSize,
    FONT_TEXT_COLOR color,
	FONT_TEXT_ALIGNMENT textAlign
);
void DrawFont(DRAW_FONT_DAT dat);


#endif