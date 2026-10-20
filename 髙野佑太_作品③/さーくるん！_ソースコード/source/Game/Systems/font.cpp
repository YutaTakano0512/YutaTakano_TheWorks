// ===================================================
// font.cpp フォントの制御
// 
// hara sougo  2024/11/18
// ===================================================

#include "../../Core/_systemModeSwitch.h"

#include "../../game_main.h"
#include "font.h"
#include "../Polygon/polygon.h"


//    //1. OpenMPの有効化
//    //　Visual Studioでは、OpenMPを利用するためにプロジェクト設定を変更する必要がある。
//    //　- プロジェクトのプロパティを開く:-メニューからプロジェクト > プロパティを選択。
//    //　- OpenMPを有効化:-「構成プロパティ > C / C++ > 言語」セクションで、OpenMP サポートを有効に設定。
//    //　- 設定を保存してプロジェクトを再ビルドします。
//#include <omp.h>

#ifdef SWITCH_MODE
#else

//構造体
//struct FontDat_DX {
//	//DirectWriteCustomFont* write;
//};
//
//static FontDat_DX g_FontData[FONT_TYPE_MAX] = {};

static int g_font_shader = 0;
static IDWriteTextFormat* g_TextFormat[FONT_TYPE_MAX] = {};		// DirectWriteテキスト形式
static ID2D1SolidColorBrush* g_Brush[FONT_TEXT_COLOR_MAX] = {};

static std::vector<DRAW_FONT_DAT> g_fontRelease_buffers = {};
#endif

size_t InitFontDataFiles()
{
    return 1;
}
ID2D1SolidColorBrush* GetFontColorBrush(FONT_TEXT_COLOR c)
{
    return g_Brush[c];
}

void InitFont(void)
{
//#ifdef SWITCH_MODE
//#else
//
//	std::string fontPath[FONT_TYPE_MAX] = {};
//
//	FontDataSet(fontPath);
//
//	//セット
//	for (int i = 0; i < FONT_TYPE_MAX; i++)
//	{
//		FontDat_DX* pFontData = g_FontData + i;
//
//		{
//			FontData fd = {};
//
//			// DirectWrite用コンポーネントを作成
//			pFontData->write = new DirectWriteCustomFont(&fd);
//
//			// 初期化
//			pFontData->write->Init(DXGetSwapChain(), DirectWriteCustomFont::StringToWString(fontPath[i]));
//
//			//DebugPrintf("%d", g_Write->GetFontName(0).size());
//
//			// フォントをセット
//			pFontData->write->SetFont(fd);
//		}
//	}
//
//#endif

    FileLoadAddBefore(1, FILELOADADDBEFORE_TXT_FONT, FILELOADADD_TYPE_NORMAL);

    g_font_shader = DX_GetShaderIndex("SHADER/shader");

	std::string fontPath[FONT_TYPE_MAX] = {};
	FontDataSet(fontPath);

	std::vector<std::wstring> setFontFilePaths = {};
	//セット
	for (int i = 0; i < FONT_TYPE_MAX; i++)
	{
		setFontFilePaths.push_back(StringToWString(fontPath[i].c_str()));
	}

	DX_FontInitialize(setFontFilePaths);


// OpenMP並列処理
//#pragma omp parallel for
    for (int i = 0; i < FONT_TYPE_MAX; i++)
    {
        FontData fd = FontData();

        fd.font = DX_GetFontName(i);
        fd.fontSize = 1;
        fd.fontWeight = DWRITE_FONT_WEIGHT_ULTRA_BLACK;
        {
            //RGB → uint32
            uint32_t colortmp = RGBToUINT32(MkF4(1.0f, 1.0f, 1.0f, 1.0f));
            fd.Color = D2D1::ColorF(colortmp, 1.0f);
        }
        DX_CreateFont(fd, &g_TextFormat[i]);
    }

    Float3 colorstmp[FONT_TEXT_COLOR_MAX] = {};
    colorstmp[FONT_TEXT_COLOR_WHITE] = MkF3(1.0f, 1.0f, 1.0f);
    colorstmp[FONT_TEXT_COLOR_BLACK] = MkF3(0.0f, 0.0f, 0.0f);
    colorstmp[FONT_TEXT_COLOR_RED] = MkF3(1.0f, 0.0f, 0.0f);
    colorstmp[FONT_TEXT_COLOR_ORANGE] = MkF3(1.0f, 0.5f, 0.0f);
    colorstmp[FONT_TEXT_COLOR_YELLOW] = MkF3(1.0f, 1.0f, 0.0f);
    colorstmp[FONT_TEXT_COLOR_GREEN] = MkF3(0.0f, 1.0f, 0.0f);
    colorstmp[FONT_TEXT_COLOR_LIGHTBLUE] = MkF3(0.0f, 1.0f, 1.0f);
    colorstmp[FONT_TEXT_COLOR_BLUE] = MkF3(0.0f, 0.0f, 1.0f);
    colorstmp[FONT_TEXT_COLOR_PURPLE] = MkF3(1.0f, 0.0f, 1.0f);

// OpenMP並列処理
//#pragma omp parallel for
    for (int i = 0; i < FONT_TEXT_COLOR_MAX; i++)
    {
        D2D1_COLOR_F color = {};

        //RGB → uint32
        uint32_t colortmp = RGBToUINT32(colorstmp[i]);
        color = D2D1::ColorF(colortmp, 1.0f);

        DX_CreateBrush(color, &g_Brush[i]);
    }

    FileLoadAdd(1, FILELOADADD_TXT_FONT, FILELOADADD_TYPE_NORMAL);
}

void UninitFont(void)
{
	DX_FontFinalize();
    for (int i = 0; i < FONT_TYPE_MAX; i++)
    {
        SAFE_RELEASE(g_TextFormat[i]);
    }

    for (int i = 0; i < FONT_TEXT_COLOR_MAX; i++)
    {
        SAFE_RELEASE(g_Brush[i]);
    }
    

//#ifdef SWITCH_MODE
//#else
//
//	for (int i = 0; i < FONT_TYPE_MAX; i++)
//	{ //解放
//		FontDat_DX* pFontData = g_FontData + i;
//		if (pFontData->write != nullptr)
//		{
//			delete pFontData->write;
//			pFontData->write = nullptr;
//		}
//	}
//
//#endif
}

void DrawFont(std::string txt, FONT_TYPE fonttype,
	Float2 pos,
	int fontSize,
    FONT_TEXT_COLOR color,
	FONT_TEXT_ALIGNMENT textAlign
)
{
	DRAW_FONT_DAT dat = {};
	dat.txt = StringToWString(txt);
	dat.fonttype = fonttype;
	dat.pos = pos;
	dat.fontSize = fontSize;
	dat.color = color;
	dat.textAlign = textAlign;

	DrawFont(dat);
}
void DrawFont(std::wstring txt, FONT_TYPE fonttype,
	Float2 pos,
	int fontSize,
    FONT_TEXT_COLOR color,
	FONT_TEXT_ALIGNMENT textAlign
)
{
	DRAW_FONT_DAT dat = {};
	dat.txt = txt;
	dat.fonttype = fonttype;
	dat.pos = pos;
	dat.fontSize = fontSize;
	dat.color = color;
	dat.textAlign = textAlign;

	DrawFont(dat);
}

void DrawFont(DRAW_FONT_DAT dat)
{
	g_fontRelease_buffers.push_back(dat);
}


void DrawFontBufferRelease(void)
{

	//for (const DRAW_FONT_DAT& dat : g_fontRelease_buffers)
	//{
	//	FontDat_DX* fdDx = g_FontData + dat.fonttype;

	//	//現在のフォント設定を取得
	//	FontData fdSave = fdDx->write->GetFont();
	//	FontData fd = fdSave;

	//	fd.fontSize = (FLOAT)dat.fontSize;
	//	fd.fontWeight = DWRITE_FONT_WEIGHT_ULTRA_BLACK;
	//	//fd.Color = D2D1::ColorF(D2D1::ColorF::Red);
	//	//fd.Color = D2D1::ColorF(0xFF00FF, 0.75f);
	//	{
	//		//RGB → uint32
	//		uint32_t colortmp = RGBToUINT32(dat.color);
	//		fd.Color = D2D1::ColorF(colortmp, dat.color.w);
	//	}

	//	{
	//		//RGB → uint32
	//		uint32_t colortmp = RGBToUINT32(dat.shadowColor);
	//		fd.shadowColor = D2D1::ColorF(colortmp, dat.color.w);
	//	}
	//	fd.shadowOffset = D2D1::Point2F(dat.shadowPos.x, dat.shadowPos.y);


	//	fdDx->write->SetFont(fd);

	//	//描画
	//	float fsizeX = fd.fontSize * dat.txt.size();
	//	D2D1_RECT_F rect = {};

	//	if (dat.textAlign == FONT_TEXT_ALIGNMENT_RIGHT)
	//	{
	//		rect = {
	//			dat.pos.x - fsizeX / 2,
	//			dat.pos.y,
	//			dat.pos.x + fsizeX / 2,
	//			dat.pos.y + fd.fontSize,
	//		};
	//	}
	//	else if (dat.textAlign == FONT_TEXT_ALIGNMENT_CENTER)
	//	{
	//		rect = {
	//			dat.pos.x - fsizeX / 4,
	//			dat.pos.y,
	//			dat.pos.x + fsizeX / 4 * 3,
	//			dat.pos.y + fd.fontSize,
	//		};
	//	}
	//	else
	//	{
	//		rect = {
	//			dat.pos.x,
	//			dat.pos.y,
	//			dat.pos.x + fsizeX,
	//			dat.pos.y + fd.fontSize,
	//		};
	//	}
	//	rect.left += SCREEN_WIDTH / 2;
	//	rect.top += SCREEN_HEIGHT / 2;
	//	rect.right += SCREEN_WIDTH / 2;
	//	rect.bottom += SCREEN_HEIGHT / 2;

	//	fdDx->write->DrawString(
	//		dat.txt,
	//		rect,
	//		D2D1_DRAW_TEXT_OPTIONS_NONE,
	//		dat.shadow
	//	);

	//	//戻す
	//	fdDx->write->SetFont(fdSave);
	//}


	for (const DRAW_FONT_DAT& dat : g_fontRelease_buffers)
	{
		//描画

		//FontData fd = FontData();

		//fd.font = DX_GetFontName(dat.fonttype);
		//fd.fontSize = (FLOAT)dat.fontSize;
		//fd.fontWeight = DWRITE_FONT_WEIGHT_ULTRA_BLACK;
		//{
		//	//RGB → uint32
		//	uint32_t colortmp = RGBToUINT32(dat.color);
		//	fd.Color = D2D1::ColorF(colortmp, dat.color.w);
		//}

        float fsizeX = (float)dat.fontSize * dat.txt.size();
        //float fsizeX = fd.fontSize * dat.txt.size();
		Float2 f2 = {};

		if (dat.textAlign == FONT_TEXT_ALIGNMENT_RIGHT)
		{
			f2 = {
				dat.pos.x - fsizeX / 2,
				dat.pos.y
			};
		}
		else if (dat.textAlign == FONT_TEXT_ALIGNMENT_CENTER)
		{
			f2 = {
				dat.pos.x - fsizeX / 4,
				dat.pos.y
			};
		}
		else
		{
			f2 = {
				dat.pos.x,
				dat.pos.y
			};
		}
		f2.x += SCREEN_WIDTH / 2;
		f2.y += SCREEN_HEIGHT / 2;

        std::vector<DX_DrawString_SetSize> sizeEx = {};
        std::vector<DX_DrawString_SetDrawingEffect> drawingEffect = dat.drawingEffect;

        {
            {
                DX_DrawString_SetSize sstmp = {};
                sstmp.start = 0;
                sstmp.length = dat.txt.size();
                sstmp.size = (float)dat.fontSize;
                sizeEx.push_back(sstmp);
            }
            if (dat.sizeEx.size() > 0)
            {
                for (auto v : dat.sizeEx)
                {
                    sizeEx.push_back(v);
                }
            }
        }

		//DX_SetFont(fd);
		DX_DrawString(
			dat.txt,
			f2.x, f2.y,
            dat.maxWidth,
            dat.maxHight,
            dat.LineSpacingWidth,
            dat.LineSpacingHight,
			D2D1_DRAW_TEXT_OPTIONS_NONE,
            dat.posShadow.x,
            dat.posShadow.y,

            g_TextFormat[dat.fonttype],
            g_Brush[dat.color],

            dat.colorShadow == FONT_TEXT_COLOR_NONE 
            ? nullptr : g_Brush[dat.colorShadow],

            sizeEx,
            drawingEffect
        );
	}

    

    //PolygonSRVDraw(DX_GetFontSRV(),
    //    MkF3(
    //        0.0f,
    //        0.0f,
    //        0.0f),
    //    {},
    //    MkF3(
    //        +SCREEN_WIDTH,
    //        SCREEN_HEIGHT,
    //        1.0f),
    //    g_font_shader
    //);

	//バッファをクリア
    //DrawFontBufferReleaseOnly();
    g_fontRelease_buffers.clear();
    g_fontRelease_buffers.shrink_to_fit();

}
void DrawFontBufferReleaseOnly(void)
{
    g_fontRelease_buffers.clear();
    g_fontRelease_buffers.shrink_to_fit();
}