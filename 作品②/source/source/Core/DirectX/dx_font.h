//---------------------------------------------------------------------------
// dx_font.h
// 
// sougo hara  2024/12/07
// 前のがひどかったので作り直し
//---------------------------------------------------------------------------

#ifndef _DX_FONT_H_
#define _DX_FONT_H_

#include "dx.h"
#include <string>						// 文字列
#include <vector>						// 動的配列

#include <d2d1.h>						// Direct2D
#include <DWrite.h>						// DirectWrite
#pragma comment(lib,"d2d1.lib")			// Direct2D
#pragma comment(lib,"Dwrite.lib")		// DirectWrite


//=============================================================================
//		フォント設定
//=============================================================================
struct FontData
{
	std::wstring font; //フォント名
	IDWriteFontCollection* fontCollection;		// フォントコレクション
	DWRITE_FONT_WEIGHT fontWeight;				// フォントの太さ
	DWRITE_FONT_STYLE fontStyle;				// フォントスタイル
	DWRITE_FONT_STRETCH fontStretch;			// フォントの幅
	FLOAT fontSize;								// フォントサイズ
	WCHAR const* localeName;					// ロケール名
	DWRITE_TEXT_ALIGNMENT textAlignment;		// テキストの配置
	D2D1_COLOR_F Color;							// フォントの色


	// デフォルト設定
	FontData()
	{
		font = L"";
		fontCollection = nullptr;
		fontWeight = DWRITE_FONT_WEIGHT::DWRITE_FONT_WEIGHT_NORMAL;
		fontStyle = DWRITE_FONT_STYLE::DWRITE_FONT_STYLE_NORMAL;
		fontStretch = DWRITE_FONT_STRETCH::DWRITE_FONT_STRETCH_NORMAL;
		fontSize = 20;
		localeName = L"ja-jp";
		textAlignment = DWRITE_TEXT_ALIGNMENT::DWRITE_TEXT_ALIGNMENT_LEADING;
		Color = D2D1::ColorF(D2D1::ColorF::White);
	}
};

struct DX_StringTexture {
};

struct DX_DrawString_SetDrawingEffect {
    UINT32 start;
    UINT32 length;
    IUnknown* drawingEffect;
};
struct DX_DrawString_SetSize {
    UINT32 start;
    UINT32 length;
    float size;
};
//=============================================================================
//	プロトタイプ宣言
//=============================================================================
void DX_FontInitialize(std::vector<std::wstring> setFontFilePaths);
void DX_FontFinalize();

std::wstring DX_GetFontName(int num);
int DX_GetFontNameNum();

HRESULT DX_CreateFont(FontData set,
    IDWriteTextFormat** textFormat
);
HRESULT DX_CreateBrush(
    D2D1_COLOR_F color,
    ID2D1SolidColorBrush** brush
);

HRESULT DX_DrawString(
    std::wstring str, float x, float y,
    float maxwidth, float maxhight,
    float LineSpacingwidth, float LineSpacinghight,
    D2D1_DRAW_TEXT_OPTIONS options,
    float shadowX, float shadowY,

    IDWriteTextFormat* format,
    ID2D1SolidColorBrush* brush,
    ID2D1SolidColorBrush* brushShadow,

    std::vector<DX_DrawString_SetSize> setsize,
    std::vector<DX_DrawString_SetDrawingEffect> setdrawingeffect
);

//DX_StringTexture DX_CreateStringTexture(std::wstring str, D2D1_DRAW_TEXT_OPTIONS options);
//ID3D11ShaderResourceView* DX_GetFontSRV();



#endif // _DX_FONT_H_

