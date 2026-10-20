// ===================================================
// sprite.h スプライトの制御
// 
// hara sougo  2024/07/22
// ===================================================

#ifndef _SPRITE_H_
#define _SPRITE_H_

#include "../Systems/sys_general.h"

#include "../../Core/_systemModeSwitch.h"

#include "../Texture/texture.h"
#include "../Polygon/polygon.h"

#ifdef SWITCH_MODE
#else
#include <d3d11.h>
#include <DirectXMath.h>
using namespace DirectX;
#endif

//static constexpr int SPRITE_NUM_VERTEX_SQUARE_EX = (4 * 3);	//四角の頂点の数
//static constexpr int NUM_VERTEX_SQUARE_INDEXED_MAX = 6;	//必要な頂点の数

static constexpr int SPRITE_NUM_VERTEX_SQUARE = NUM_VERTEX_SQUARE;	//四角の頂点の数


// ===================================================
// 列挙体定義
// ===================================================


// ===================================================
// 構造体宣言
// ===================================================
struct DRAW_SPRITE_DAT {		// 描画データ
	float x;
	float y;
	float z;

	float w;
	float h;

	float rot;
	Float3 rot3D;

	Float4 color;

	unsigned int texNo;
	LOADTEXTURETYPE loadTexType;

	Float2 texUV;
	Float2 texWH;

	Float2 startPos;
	Float3 startPos3D;

	float startRot;
	Float3 startRot3D;

	Float2 startScale;

	DPD_SHADER_OPT shaderOpt;
};

struct DRAW_SPRITE_BUFFER {
#ifdef SWITCH_MODE
	VERTEX_3D vertexQuad[4];
#else
	DRAW_POLYGON_BUFFER buf;
	//XMMATRIX pro;
	
	//Float3 position;
	//float* gameCameraDistance;
#endif
};


// ===================================================
// プロトタイプ宣言
// ===================================================

#ifdef IS_MAIN
//メイン以外からは呼び出せないようにしておく
//メインから呼び出すのを忘れずに
void InitSprite();
void UninitSprite();
#endif


void DrawSpriteQuad(
	float x, float y, float w, float h,
	unsigned int texNo,
	DRAW_POLYGON_PRIORITY priority = DRAW_POLYGON_PRIORITY_FORCE_NORMAL
);

void DrawSpriteColorQuad(
	float x, float y, float w, float h,
	unsigned int texNo,
	Float4* color,
	DRAW_POLYGON_PRIORITY priority = DRAW_POLYGON_PRIORITY_FORCE_NORMAL
);

void DrawSpriteColorRotateQuad(
	float x, float y, float w, float h,
	unsigned int texNo,
	Float4* color,
	float rot,
	DRAW_POLYGON_PRIORITY priority = DRAW_POLYGON_PRIORITY_FORCE_NORMAL
);


void DrawLine(float sx, float sy, float ex, float ey, Float4 *color);


// 色付き回転ありUV指定可能四角形の描画
void DrawSpriteQuadColorRotateTexUv(
	float x, float y, float w, float h,
	float rot,
	Float4 color,
	unsigned int texNo,
	Float2 texUV, Float2 texWH,
	DRAW_POLYGON_PRIORITY priority= DRAW_POLYGON_PRIORITY_FORCE_NORMAL
);

//スプライトの描画 より細かく
void DrawSpriteEx(DRAW_SPRITE_DAT drawDat);

//スプライト描画の処理を小分けにしたもの
void SpriteSetUp(DRAW_SPRITE_BUFFER* dsBuffer,
	Vertex_DX vertex[SPRITE_NUM_VERTEX_SQUARE] = nullptr
);

void SpriteShape(DRAW_SPRITE_BUFFER* dsBuffer,
	Float2 pos, float rot, Float2 scale);
void SpriteShape(DRAW_SPRITE_BUFFER* dsBuffer,
	Float3 pos, Float3 rot, Float2 scale);

void SpriteRelease(DRAW_SPRITE_BUFFER* dsBuffer,
	Float4 color, unsigned int texNo, Float2 texUV, Float2 texWH, LOADTEXTURETYPE loadTexType,
	bool fixed=false,
	DRAW_POLYGON_PRIORITY priority= DRAW_POLYGON_PRIORITY_NORMAL
);
void SpriteReleaseEx(DRAW_SPRITE_BUFFER* dsBuffer, POLYGON_RELEASE_OPTION opt);


Float2 AnimationUV(int no, int xcnt, int hcnt);

#endif //_SPRITE_H_