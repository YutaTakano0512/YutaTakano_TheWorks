// ===================================================
// spriteGraph.cpp スプライトを使ったグラフィック制御
// 
// hara sougo  2024/07/22
// ===================================================
#include "../../game_main.h"
#include "spriteGraph.h"

// ===================================================
// マクロ定義
// ===================================================

// ===================================================
// プロトタイプ宣言
// ===================================================

// ===================================================
// グローバル変数
// ===================================================


void InitSpriteGraph(SPRITE_GRAPH_DAT* sgdat)
{
	(*sgdat) = {};
	sgdat->alpha = 1.0f;
	sgdat->scale = MkF2(1.0f, 1.0f);
	sgdat->outlineWidth = 6.0f;
	for (int i = 0; i < SPRITE_GRAPH_ANIM_DAT_MAX; i++)
	{
		InitSpriteAnim(&sgdat->spriteAnim[i]);
	}
}

void UninitSpriteGraph(SPRITE_GRAPH_DAT* sgdat)
{
	for (int i = 0; i < SPRITE_GRAPH_ANIM_DAT_MAX; i++)
	{
		UninitSpriteAnim(&sgdat->spriteAnim[i]);
	}
}

void UpdateSpriteGraph(SPRITE_GRAPH_DAT* sgdat)
{
	for (int i = 0; i < SPRITE_GRAPH_ANIM_DAT_MAX; i++)
	{
		if (sgdat->spriteAnim[i].isUse)
		{
			UpdateSpriteAnim(&sgdat->spriteAnim[i]);
		}
	}
}

void DrawSpriteGraph(SPRITE_GRAPH_DAT* sgdat,
	Float2 startPos, float startRot, Float2 startScale,
	Float4 colorAdd,
	Float4 colorMul,
	LOADTEXTURETYPE loadTexType,
	bool fixed,
	bool outlineMode
)
{
	DrawSpriteGraph( sgdat,
		Float2ToFloat3(startPos), MkF3(0.0f, 0.0f, startRot), startScale,
		colorAdd,
		colorMul,
		loadTexType,
		fixed,
		outlineMode
	);
}

void DrawSpriteGraph(SPRITE_GRAPH_DAT* sgdat,
	Float3 startPos, Float3 startRot, Float2 startScale,
	Float4 colorAdd,
	Float4 colorMul,
	LOADTEXTURETYPE loadTexType,
	bool fixed,
	bool outlineMode
)
{
	DRAW_SPRITE_GRAPH_OPTION opt = {};
	opt.startPos = startPos;
	opt.startRot = startRot;
	opt.startScale = startScale;
	opt.colorAdd = colorAdd;
	opt.colorMul = colorMul;
	opt.loadTexType = loadTexType;
	opt.fixed = fixed;
	opt.outlineMode = outlineMode;
    opt.priority = DRAW_POLYGON_PRIORITY_NORMAL;

	DrawSpriteGraphEx(sgdat, opt);
}

void DrawSpriteGraphEx(SPRITE_GRAPH_DAT* sgdat, DRAW_SPRITE_GRAPH_OPTION opt)
{
	if (opt.outlineMode && sgdat->outlineWidth <= 0.0f)
	{
		return;
	}

	for (int i = 0; i < SPRITE_GRAPH_ANIM_DAT_MAX; i++)
	{
		if (sgdat->spriteAnim[i].isUse)
		{
			SPRITE_ANIM_DAT sgdatTmp = sgdat->spriteAnim[i];
			sgdatTmp.color.x += opt.colorAdd.x;
			sgdatTmp.color.y += opt.colorAdd.y;
			sgdatTmp.color.z += opt.colorAdd.z;
			sgdatTmp.color.w += opt.colorAdd.w;

			sgdatTmp.color.x *= opt.colorMul.x;
			sgdatTmp.color.y *= opt.colorMul.y;
			sgdatTmp.color.z *= opt.colorMul.z;
			sgdatTmp.color.w *= opt.colorMul.w * sgdat->alpha;
			sgdatTmp.reverse = sgdat->reverse;

			Float2 scaleTmp = opt.startScale;
			scaleTmp.x *= sgdat->scale.x;
			scaleTmp.y *= sgdat->scale.y;

			if (opt.outlineMode)
			{
				Float4 colorMulTmp = MkF4(1.6f, 1.6f, 1.6f, 2.0f);

				sgdatTmp.color.x *= colorMulTmp.x;
				sgdatTmp.color.y *= colorMulTmp.y;
				sgdatTmp.color.z *= colorMulTmp.z;
				sgdatTmp.color.w *= colorMulTmp.w;

				//DRAW_SPRITE_ANIM_EX_OPTION opttmp = {};
				//opttmp.startPos = opt.startPos;
				//opttmp.startRot = opt.startRot;
				//opttmp.startScale = scaleTmp;

				//opttmp.startScale.x *= 1.1f;
				//opttmp.startScale.y *= 1.1f;

				//opttmp.loadTexType = opt.loadTexType;
				//opttmp.fixed = opt.fixed;

				//opttmp.enableShade = opt.enableShade;

				//DrawSpriteAnimEx(&sgdatTmp, opttmp);

				constexpr int fn = 4;
				for (int j = 0; j < fn; j++)
				{
					Float3 posTmp = {};
					posTmp.x += (float)sin(SYS_GENERAL_PI * 2.0f / fn * j);
					posTmp.y += (float)cos(SYS_GENERAL_PI * 2.0f / fn * j);

					float rotRad = CalculateDegToRad(opt.startRot.x);
					float si = sinf(rotRad),
						co = cosf(rotRad);

					{
						float tmpX, tmpY;
						//x = (xcosθ - ysinθ) + posX
						//y = (xsinθ + ycosθ) + posY
						tmpX = (posTmp.y * co - posTmp.z * si);
						tmpY = (posTmp.y * si + posTmp.z * co);

						posTmp.y = tmpX;
						posTmp.z = tmpY;
					}
					posTmp.x *= sgdat->outlineWidth;
					posTmp.y *= sgdat->outlineWidth;
					posTmp.z *= sgdat->outlineWidth;

					posTmp.x += opt.startPos.x;
					posTmp.y += opt.startPos.y;
					posTmp.z += opt.startPos.z;


					DRAW_SPRITE_ANIM_EX_OPTION opttmp = {};
					opttmp.startPos = posTmp;
					opttmp.startRot = opt.startRot;
					opttmp.startScale = scaleTmp;
					opttmp.loadTexType = opt.loadTexType;
					opttmp.fixed = opt.fixed;
					opttmp.billboard = opt.billboard;

					opttmp.enableShade = opt.enableShade;
					opttmp.shaderOpt = opt.shaderOpt;

					DrawSpriteAnimEx(&sgdatTmp, opttmp);

					//DrawSpriteAnim(&sgdatTmp, posTmp, opt.startRot, scaleTmp, opt.loadTexType, opt.fixed);
				}

				//Float4 colorAddTmp = MkF4( 0.0f, 0.0f, 0.0f, 0.0f);
				////Float4 colorMulTmp = MkF4(0.4f, 0.4f, 0.4f, 0.75f);
				//Float4 colorMulTmp = MkF4(1.4f, 1.4f, 1.4f, 0.75f);
				////Float2 posTmp = startPos;
				//Float3 posTmp = startPos;
				//posTmp.x += 1.0f * sgdat->outlineWidth;
				//posTmp.y += 1.0f * sgdat->outlineWidth;

				//sgdatTmp.color.x *= colorMulTmp.x;
				//sgdatTmp.color.y *= colorMulTmp.y;
				//sgdatTmp.color.z *= colorMulTmp.z;
				//sgdatTmp.color.w *= colorMulTmp.w;

				//DrawSpriteAnim(&sgdatTmp, posTmp, startRot, scaleTmp, loadTexType, fixed);
			}
			else
			{
				DRAW_SPRITE_ANIM_EX_OPTION opttmp = {};
				opttmp.startPos = opt.startPos;
				opttmp.startRot = opt.startRot;
				opttmp.startScale = scaleTmp;
				opttmp.loadTexType = opt.loadTexType;
				opttmp.fixed = opt.fixed;
				opttmp.billboard = opt.billboard;

				opttmp.enableShade = opt.enableShade;

				opttmp.shaderOpt = opt.shaderOpt;

                opttmp.priority = opt.priority;
                opttmp.depthBuffIgnore = opt.depthBuffIgnore;
                opttmp.shaderType = opt.shaderType;

				DrawSpriteAnimEx(&sgdatTmp, opttmp);

				//DrawSpriteAnim(&sgdatTmp, startPos, startRot, scaleTmp, loadTexType, fixed);
			}
		}
	}
}
