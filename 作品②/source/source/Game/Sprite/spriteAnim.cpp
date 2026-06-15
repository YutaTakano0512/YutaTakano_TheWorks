// ===================================================
// spriteAnim.cpp スプライトのアニメーション制御
// 
// hara sougo  2024/07/22
// ===================================================
#include "../../game_main.h"
#include "sprite.h"
#include "spriteAnim.h"

#include "../Systems/camera.h"

// ===================================================
// マクロ定義
// ===================================================

// ===================================================
// プロトタイプ宣言
// ===================================================

// ===================================================
// グローバル変数
// ===================================================


void InitSpriteAnim(SPRITE_ANIM_DAT* sadat)
{
	(*sadat) = {};
	sadat->isUse = false;
	sadat->texDat = {};
	sadat->color = MkF4(1.0f, 1.0f, 1.0f, 1.0f );
	sadat->hcnt = 1;
	sadat->xcnt = 1;
	sadat->cntMax = 1;
	sadat->delay = 0;

	sadat->anim = false;
	sadat->hide = false;
	sadat->frame = 0;
	sadat->no = 0;

	sadat->pos = MkF2(0.0f, 0.0f);
	sadat->rot = 0.0f;
	sadat->pos3D = MkF3(0.0f, 0.0f, 0.0f);
	sadat->rot3D = MkF3(0.0f, 0.0f, 0.0f);

	sadat->size = MkF2(0.0f, 0.0f );
}

void UninitSpriteAnim(SPRITE_ANIM_DAT* sadat)
{
	sadat->isUse = false;
}

void UpdateSpriteAnim(SPRITE_ANIM_DAT* sadat)
{
	//sadat->no = (sadat->frame / sadat->delay) % (sadat->xcnt * sadat->hcnt);
	if (sadat->anim)
	{
		sadat->no = (sadat->frame / std::max(sadat->delay, 1)) % std::max(sadat->cntMax, 1);
	}

	sadat->frame++;
}

void DrawSpriteAnim(SPRITE_ANIM_DAT* sadat, Float2 startPos, float startRot, Float2 startScale,
	LOADTEXTURETYPE loadTexType, bool fixed)
{
	DrawSpriteAnim(sadat, Float2ToFloat3(startPos), MkF3(0.0f, 0.0f, startRot), startScale,
		loadTexType, fixed);
}

void DrawSpriteAnim(SPRITE_ANIM_DAT* sadat, Float3 startPos, Float3 startRot, Float2 startScale,
	LOADTEXTURETYPE loadTexType, bool fixed)
{
	DRAW_SPRITE_ANIM_EX_OPTION opt = {};
	opt.startPos = startPos;
	opt.startRot = startRot;
	opt.startScale = startScale;
	opt.loadTexType = loadTexType;
	opt.fixed = fixed;
    opt.priority = DRAW_POLYGON_PRIORITY_NORMAL;

	DrawSpriteAnimEx(sadat, opt);
}
void DrawSpriteAnimEx(SPRITE_ANIM_DAT* sadat, DRAW_SPRITE_ANIM_EX_OPTION opt)
{
	if (sadat->hide)
	{
		return;
	}

    {
        DRAW_POLYGON_DAT drawDatTmp = {};
        drawDatTmp.pos = MkF3(
            sadat->pos.x + sadat->pos3D.x,
            sadat->pos.y + sadat->pos3D.y,
            sadat->pos3D.z
        );
        drawDatTmp.size = MkF3(sadat->size.x, sadat->size.y, 1.0f);

        drawDatTmp.rot = sadat->rot;
        drawDatTmp.rot3D = sadat->rot3D;

        drawDatTmp.color = sadat->color;

        drawDatTmp.texNo = sadat->texDat.textureId;
        drawDatTmp.loadTexType = opt.loadTexType;

        drawDatTmp.texUV = AnimationUV(sadat->no, sadat->xcnt, sadat->hcnt);
        drawDatTmp.texWH = MkF2(1.0f / (float)sadat->xcnt, 1.0f / (float)sadat->hcnt);

        drawDatTmp.startPos3D = opt.startPos;

        drawDatTmp.startRot3D = opt.startRot;

        drawDatTmp.startScale = opt.startScale;
        drawDatTmp.startScale3D = MkF3(1.0f, 1.0f, 1.0f);

        drawDatTmp.vertex.type = opt.vertex;

        //drawDatTmp.model = nullptr;
        drawDatTmp.modelNo = 0;

        drawDatTmp.shaderOpt = opt.shaderOpt;

        DRAW_POLYGON_IN_CAMERA_EX_OPTION ropt = {};
        ropt.reverse = sadat->reverse;
        ropt.fixed = opt.fixed;
        ropt.billboard = opt.billboard;
        ropt.enableShade = opt.enableShade;
        ropt.priority = opt.priority;
        ropt.depthBuffIgnore = opt.depthBuffIgnore;
        ropt.shaderType = opt.shaderType;

        DrawPolygonInCameraEx(drawDatTmp, ropt);
    }
}
