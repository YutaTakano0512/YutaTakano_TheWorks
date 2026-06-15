// ===================================================
// spriteAnim.h スプライトのアニメーション制御
// 
// hara sougo  2024/07/22
// ===================================================
#ifndef _SPRITE_ANIM_H_
#define _SPRITE_ANIM_H_

#include "../Systems/sys_general.h"
#include "../Texture/texData.h"
#include "../Polygon/polygon.h"

// ===================================================
// マクロ定義
// ===================================================
#define SPRITE_ANIM_TEXTURE_PATH_MAX (256)

// ===================================================
// 構造体宣言
// ===================================================
struct SPRITE_ANIM_DAT {
	bool isUse;

	TEX_DAT texDat;
	//char texturePath[SPRITE_ANIM_TEXTURE_PATH_MAX]; //使用しないこともある
	//unsigned int textureId;
	//unsigned int textureIdGray;

	Float4 color;
	int xcnt;
	int hcnt;
	int cntMax;
	int delay;

	bool anim;
	bool hide;
	unsigned int frame;
	int no;

	Float2 pos;
	Float3 pos3D;
	Float2 size;
	float rot;
	Float3 rot3D;

	bool reverse;
};

struct DRAW_SPRITE_ANIM_EX_OPTION {
	Float3 startPos;
	Float3 startRot;
	Float2 startScale;
	LOADTEXTURETYPE loadTexType;
    DRAW_POLYGON_PRIORITY priority;
    bool depthBuffIgnore;
	bool fixed;

	bool enableShade;

	bool billboard;

	DPD_SHADER_OPT shaderOpt;
    DPD_SHADER_TYPE shaderType;
    DRAW_POLYGON_TYPE vertex;
};

// ===================================================
// プロトタイプ宣言
// ===================================================


void InitSpriteAnim(SPRITE_ANIM_DAT* sadat);
void UninitSpriteAnim(SPRITE_ANIM_DAT* sadat);
void UpdateSpriteAnim(SPRITE_ANIM_DAT* sadat);

void DrawSpriteAnim(SPRITE_ANIM_DAT* sadat, Float2 startPos, float startRot, Float2 startScale,
	LOADTEXTURETYPE loadTexType, bool fixed);
void DrawSpriteAnim(SPRITE_ANIM_DAT* sadat, Float3 startPos, Float3 startRot, Float2 startScale,
	LOADTEXTURETYPE loadTexType, bool fixed);

void DrawSpriteAnimEx(SPRITE_ANIM_DAT* sadat, DRAW_SPRITE_ANIM_EX_OPTION opt);


#endif