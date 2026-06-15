// ===================================================
// fade.cpp フェード制御
// 
// hara sougo  2024/07/30
// ===================================================
#include "../../game_main.h"
#include "../Sprite/sprite.h"
#include "../Texture/texture.h"
#include "../Texture/texData.h"
#include "fade.h"
#include "../Managers/scene.h"
#include "../Systems/camera.h"

// ===================================================
// グローバル変数
// ===================================================
static FADE fade[FADE_MAX] = {};	// フェード実体
static unsigned int FadeTextureId;	// フェードテクスチャID

static unsigned int g_FadeTextureId_StarWipe;


//ぷ
void setFade(FADE* fad, FADE_TYPE type,
	std::function<float(float)> easing,
	int speed,
	bool isGameStop
);
void fadeStart(FADE* fad);
void fadeUpdate(FADE* fad);
void fadeDraw(FADE* fad);

// ===================================================
// フェードの初期化
// ===================================================
void InitFade(void)
{
	//初期化
	for (int i = 0; i < FADE_MAX; i++)
	{
		fade[i] = {};
		fade[i].pos = MkF2(0.0f, 0.0f);
		fade[i].size = MkF2(SCREEN_WIDTH, SCREEN_HEIGHT);
		fade[i].alpha = 0.0f;
		fade[i].mode = MODE_FADE_NONE;
		fade[i].use = false;
	}

	// テクスチャの読み込み
	FadeTextureId = 0;

    g_FadeTextureId_StarWipe = LoadTexData("TEX/effect/star_wipe").textureId;

}

// ===================================================
// フェードの更新
// ===================================================
void UpdateFade(void)
{
	for (int i = 0; i < FADE_MAX; i++)
	{
		if (fade[i].use)
		{
			switch (fade[i].mode)
			{
			case MODE_FADE_NONE:
				break;

			case MODE_FADE_OUT:
				fade[i].alpha += fade[i].speed;//不透明へ近づける

				if (fade[i].alpha >= 1.0f)//完全不透明状態
				{
					fade[i].mode = MODE_FADE_IN;
					fade[i].alpha = 1.0f;

					// 設定された関数を実行する
					if (fade[i].func != nullptr)
					{
						fade[i].func();
					}
				}
				break;

			case MODE_FADE_IN:
				fade[i].alpha -= fade[i].speed;//透明へ近づける
				if (fade[i].alpha <= 0.0f)//完全透明
				{
					fade[i].mode = MODE_FADE_NONE;
					fade[i].alpha = 0.0f;
					fade[i].use = false;
				}

				break;
			default:break;
			}
			fadeUpdate(fade + i);
		}
	}
}

// ===================================================
// フェードの描画
// ===================================================
void DrawFade(void)
{
	for (int i = 0; i < FADE_MAX; i++)
	{
		if (fade[i].use)
		{
			float easTmp = fade[i].alpha;
			if (fade[i].easing != nullptr)
			{
				easTmp = fade[i].easing(easTmp);
			}
			fade[i].color.w = easTmp;

			fadeDraw(fade + i);
		}
	}
}

// ===================================================
// フェードの終了処理
// ===================================================
void UninitFade(void)
{
    UnloadTexData(g_FadeTextureId_StarWipe);
}

// ===================================================
// フェードのセット（フェードアウトの開始）
// ===================================================

void setFade(FADE* fad, FADE_TYPE type,
	std::function<float(float)> easing,
	int speed,
	bool isGameStop
)
{
	fad->isGameStop = isGameStop;
	fad->easing = easing;

	fad->type = type;

	fad->mode = MODE_FADE_OUT;
	fad->speed = 0.1f;
	fad->color = MkF4( 0.0f, 0.0f, 0.0f, 0.0f );
	fad->alpha = 0.0f;
	fad->use = true;
	fadeStart(fad);
	if (speed > 0)
	{
		fade->speed = 1.0f / speed;
	}
}

bool SetFade(FADE_TYPE type,
	std::function<void(void)> func,
	std::function<float(float)> easing,
	int speed,
	bool isGameStop
)
{
	/*if (fade.mode == MODE_FADE_NONE)
	{
		fade.func = func;
		setFade(type, easing);
	}*/

	for (int i = 0; i < FADE_MAX; i++)
	{
		if (!fade[i].use)
		{
			fade[i].func = func;
			setFade(fade + i, type, easing, speed, isGameStop);

			return true;
			//break;
		}
	}
	return false;
}

bool FadeGetGameStop()
{
	for (int i = 0; i < FADE_MAX; i++)
	{
		if (fade[i].use && fade[i].isGameStop)
		{
			return true;
		}
	}
	return false;
}
bool IsFading()
{
	for (int i = 0; i < FADE_MAX; i++)
	{
		if (fade[i].use)
		{
			return true;
		}
	}
	return false;
}


void fadeStart(FADE* fad)
{
	switch (fad->type)
	{

	case FADE_TYPE_COLOR_BLACK:
		fad->speed = 1.0f / 40;
		fad->color = MkF4( 0.0f, 0.0f, 0.0f, 0.0f );
		break;

	default:
        fad->speed = 1.0f / 24;
        fad->color = MkF4(0.0f, 0.0f, 0.0f, 0.0f);
        break;
	}
}

void fadeUpdate(FADE* fad)
{
	switch (fad->type)
	{

	case FADE_TYPE_COLOR_BLACK:

		break;

	default:break;
	}
}

void fadeDraw(FADE* fad)
{
	switch (fad->type)
	{

	case FADE_TYPE_COLOR_BLACK:

		//DrawSpriteColorQuad(
		//	fad->pos.x, fad->pos.y,
		//	fad->size.x, fad->size.y,
		//	FadeTextureId,
		//	&fad->color,
		//	DRAW_POLYGON_PRIORITY_ALWAYS
		//);
	{
		Float4 color = fad->color;
		//DRAW_SPRITE_BUFFER dsb = {};
		//SpriteSetUp(&dsb);
		//SpriteShape(&dsb,
		//	MkF2(fad->pos.x, fad->pos.y),
		//	0.0f,
		//	MkF2(fad->size.x, fad->size.y)
		//);
		//SpriteRelease(&dsb,
		//	color,
		//	FadeTextureId,
		//	MkF2(0.0f, 0.0f),
		//	MkF2(1.0f, 1.0f),
		//	LOADTEXTURETYPE_MAIN,
		//	true,
		//	(DRAW_POLYGON_PRIORITY)(DRAW_POLYGON_PRIORITY_FORCE_ALWAYS - 1)
		//);
        {
            DRAW_POLYGON_DAT dsdat = {};
            dsdat.pos.x = fad->pos.x;
            dsdat.pos.y = fad->pos.y;
            dsdat.pos.z = 0.0f;

            dsdat.size.x = fad->size.x;
            dsdat.size.y = fad->size.y;
            dsdat.size.z = 1.0f;

            dsdat.rot = 0.0f;

            dsdat.color = color;

            dsdat.texNo = FadeTextureId;
            dsdat.loadTexType = LOADTEXTURETYPE_MAIN;

            dsdat.texUV = {
                0.0f,
                0.0f
            };
            dsdat.texWH = { 1.0f, 1.0f };

            dsdat.startPos = {
                0.0f,
                0.0f
            };
            dsdat.startRot = 0;
            dsdat.startScale = { 1.0f, 1.0f };
            dsdat.startScale3D = { 1.0f, 1.0f, 1.0f };

            dsdat.shaderOpt.bloom = DPD_SHADER_OPT_BLOOM_NONE;
            //dsdat.shaderOpt.blendState = DPD_SHADER_OPT_BLEND_STATE_ADDCOLOR;

            DRAW_POLYGON_IN_CAMERA_EX_OPTION opt = {};
            opt.reverse = false;
            opt.fixed = true;
            opt.depthBuffIgnore = true;
            opt.priority = (DRAW_POLYGON_PRIORITY)(DRAW_POLYGON_PRIORITY_FORCE_ALWAYS - 1);


            DrawPolygonInCameraEx(dsdat, opt);
        }
	}

		break;
    case FADE_TYPE_STAR_WIPE:

    {
        Float4 color = fad->color;

        {
            DRAW_POLYGON_DAT dsdat = {};
            dsdat.pos.x = fad->pos.x;
            dsdat.pos.y = fad->pos.y;
            dsdat.pos.z = 0.0f;

            dsdat.size.x = fad->size.x;
            dsdat.size.y = fad->size.y;
            dsdat.size.z = 1.0f;

            dsdat.rot = 0.0f;

            dsdat.color = {1.0f, 1.0f, 1.0, 1.0f};

            dsdat.texNo = g_FadeTextureId_StarWipe;
            dsdat.loadTexType = LOADTEXTURETYPE_MAIN;

            //float mod = 1.0f - fad->alpha;
            float mod = fad->alpha;

            int animi = 0;
            if (fad->mode == MODE_FADE_OUT)
            //if (fad->mode == MODE_FADE_IN)
            {
                animi = 12 * mod;
            }
            //if (fad->mode == MODE_FADE_OUT)
            if (fad->mode == MODE_FADE_IN)
            {
                animi = 12 + 12 * (1.0f - mod);
            }
            dsdat.texUV = AnimationUV(
                animi,
                5, 5
            );
            dsdat.texWH = {
                1.0f / 5,
                1.0f / 5
            };

            dsdat.startPos = {
                0.0f,
                0.0f
            };
            dsdat.startRot = 0;
            dsdat.startScale = { 1.0f, 1.0f };
            dsdat.startScale3D = { 1.0f, 1.0f, 1.0f };

            dsdat.shaderOpt.bloom = DPD_SHADER_OPT_BLOOM_NONE;
            //dsdat.shaderOpt.blendState = DPD_SHADER_OPT_BLEND_STATE_ADDCOLOR;

            DRAW_POLYGON_IN_CAMERA_EX_OPTION opt = {};
            opt.reverse = false;
            opt.fixed = true;
            opt.depthBuffIgnore = true;
            opt.priority = (DRAW_POLYGON_PRIORITY)(DRAW_POLYGON_PRIORITY_FORCE_ALWAYS - 1);


            DrawPolygonInCameraEx(dsdat, opt);
        }
    }

    break;

	default:break;
	}
}


FADE GetFadeInfo()
{
	return fade[0];
}