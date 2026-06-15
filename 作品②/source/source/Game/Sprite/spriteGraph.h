// ===================================================
// spriteGraph.h スプライトを使ったグラフィック制御
// 
// hara sougo  2024/07/22
// ===================================================
#ifndef _SPRITE_GRAPH_H_
#define _SPRITE_GRAPH_H_

#include "../Systems/sys_general.h"
#include "spriteAnim.h"

// ===================================================
// マクロ定義
// ===================================================
//#define SPRITE_GRAPH_ANIM_DAT_MAX (16)
#define SPRITE_GRAPH_ANIM_DAT_MAX (8)

// ===================================================
// 構造体宣言
// ===================================================

struct SPRITE_GRAPH_DAT {
	float alpha;
	bool reverse;
	float outlineWidth;
	Float2 scale;
	SPRITE_ANIM_DAT spriteAnim[SPRITE_GRAPH_ANIM_DAT_MAX];
};

struct DRAW_SPRITE_GRAPH_OPTION {
	Float3 startPos;
	Float3 startRot;
	Float2 startScale;
	Float4 colorAdd;
	Float4 colorMul;
	LOADTEXTURETYPE loadTexType;
    DRAW_POLYGON_PRIORITY priority;
    bool depthBuffIgnore;
	bool fixed;
	bool outlineMode;

	bool billboard;

	bool enableShade;

	DPD_SHADER_OPT shaderOpt;
    DPD_SHADER_TYPE shaderType;
};

// ===================================================
// プロトタイプ宣言
// ===================================================

void InitSpriteGraph(SPRITE_GRAPH_DAT* sgdat);
void UninitSpriteGraph(SPRITE_GRAPH_DAT* sgdat);
void UpdateSpriteGraph(SPRITE_GRAPH_DAT* sgdat);

void DrawSpriteGraph(SPRITE_GRAPH_DAT* sgdat,
	Float2 startPos, float startRot, Float2 startScale,
	Float4 colorAdd,
	Float4 colorMul,
	LOADTEXTURETYPE loadTexType,
	bool fixed,
	bool outlineMode = false
);
void DrawSpriteGraph(SPRITE_GRAPH_DAT* sgdat,
	Float3 startPos, Float3 startRot, Float2 startScale,
	Float4 colorAdd,
	Float4 colorMul,
	LOADTEXTURETYPE loadTexType,
	bool fixed,
	bool outlineMode = false
);

void DrawSpriteGraphEx(SPRITE_GRAPH_DAT* sgdat, DRAW_SPRITE_GRAPH_OPTION opt);

#endif