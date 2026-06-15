// ===================================================
// sprite.cpp スプライトの制御
// 
// hara sougo  2024/07/22
// ===================================================

#include "../../game_main.h"
#include "sprite.h"
#include "../Systems/collision.h"

#include "../../Core/_systemModeSwitch.h"

#ifdef SWITCH_MODE
#else
#include "../../Core/DirectX/dx.h"
#include "../../Core/DirectX/dx_camera.h"
#include <d3d11.h>
#include <DirectXMath.h>
#include "../../../DirectXTex.h"
using namespace DirectX;
#include "../../Core/DirectX/dx_shader.h"

#endif


#ifdef SWITCH_MODE
unsigned int	g_SpriteVertexArrayObject;
unsigned int	g_SpriteVertexBuffer;
#else
static ID3D11Buffer* g_VertexBuffer = NULL;			// 線用の頂点情報
static ID3D11Buffer* g_IndexBuffer = NULL;			//

// 注意！初期化で外部から設定されるもの。Release不要。
static ID3D11Device* g_Device = NULL;
static ID3D11DeviceContext* g_DeviceContext = NULL;

#endif


void InitSprite()
{
#ifdef SWITCH_MODE

	glGenVertexArrays(1, &g_SpriteVertexArrayObject);
	glBindVertexArray(g_SpriteVertexArrayObject);

	glEnableVertexArrayAttrib(g_SpriteVertexArrayObject, 0);
	glEnableVertexArrayAttrib(g_SpriteVertexArrayObject, 1);
	glEnableVertexArrayAttrib(g_SpriteVertexArrayObject, 2);

	glVertexAttribFormat(0, 3, GL_FLOAT, GL_FALSE, 0);
	glVertexAttribFormat(1, 4, GL_FLOAT, GL_FALSE, sizeof(Float3));
	glVertexAttribFormat(2, 2, GL_FLOAT, GL_FALSE, sizeof(Float3) + sizeof(Float4));

	glVertexAttribBinding(0, 0);
	glVertexAttribBinding(1, 0);
	glVertexAttribBinding(2, 0);


	glGenBuffers(1, &g_SpriteVertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, g_SpriteVertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(VERTEX_3D) * 4, 0, GL_DYNAMIC_DRAW);

	glBindVertexArray(0);
#else
	// デバイスとデバイスコンテキストの保存
	g_Device = DXGetDevice();
	g_DeviceContext = DXGetDeviceContext();
#endif

}


void UninitSprite()
{
#ifdef SWITCH_MODE

	glDeleteVertexArrays(1, &g_SpriteVertexArrayObject);
	glDeleteBuffers(1, &g_SpriteVertexBuffer);

#else

	SAFE_RELEASE(g_VertexBuffer);

#endif
}



// 四角形の描画
//in
// x,y スプライトの表示座標（中心点）
// w,h スプライトのサイズ　w横h縦
// texNo テクスチャ画像の番号
void DrawSpriteQuad(
	float x, float y, float w, float h,
	unsigned int texNo,
	DRAW_POLYGON_PRIORITY priority
)
{
#ifdef SWITCH_MODE

	VERTEX_3D vertexQuad[4];

	vertexQuad[0].Position = MakeFloat3(x - w * 0.5f, y - h * 0.5f, 0.0f);
	vertexQuad[1].Position = MakeFloat3(x + w * 0.5f, y - h * 0.5f, 0.0f);
	vertexQuad[2].Position = MakeFloat3(x - w * 0.5f, y + h * 0.5f, 0.0f);
	vertexQuad[3].Position = MakeFloat3(x + w * 0.5f, y + h * 0.5f, 0.0f);

	vertexQuad[0].Color = MakeFloat4(1.0f, 1.0f, 1.0f, 1.0f);
	vertexQuad[1].Color = MakeFloat4(1.0f, 1.0f, 1.0f, 1.0f);
	vertexQuad[2].Color = MakeFloat4(1.0f, 1.0f, 1.0f, 1.0f);
	vertexQuad[3].Color = MakeFloat4(1.0f, 1.0f, 1.0f, 1.0f);

	vertexQuad[0].TexCoord = MakeFloat2(0.0f, 0.0f);
	vertexQuad[1].TexCoord = MakeFloat2(1.0f, 0.0f);
	vertexQuad[2].TexCoord = MakeFloat2(0.0f, 1.0f);
	vertexQuad[3].TexCoord = MakeFloat2(1.0f, 1.0f);

	glBindBuffer(GL_ARRAY_BUFFER, g_SpriteVertexBuffer);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(VERTEX_3D) * 4, vertexQuad);


	glBindVertexArray(g_SpriteVertexArrayObject);
	glBindVertexBuffer(0, g_SpriteVertexBuffer, 0, sizeof(VERTEX_3D));

	SetTexture(texNo);

	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

	glBindVertexArray(0);
#else
	DRAW_SPRITE_BUFFER dsb = {};
	SpriteSetUp(&dsb);
	SpriteShape(&dsb,
		MkF2(x, y),
		0.0f,
		MkF2(w, h)
	);
	SpriteRelease(&dsb,
		MkF4(0.0f, 0.0f, 0.0f, 1.0f),
		texNo,
		MkF2(0.0f, 0.0f),
		MkF2(1.0f, 1.0f),
		LOADTEXTURETYPE_MAIN,
		true,
		priority
	);
#endif

}


// 四角形の描画 色つきバージョン
//in
// x,y スプライトの表示座標（中心点）
// w,h スプライトのサイズ　w横h縦
// texNo テクスチャ画像の番号
// color 頂点カラー
void DrawSpriteColorQuad(
	float x, float y, float w, float h,
	unsigned int texNo,
	Float4* color,
	DRAW_POLYGON_PRIORITY priority
)
{
#ifdef SWITCH_MODE

	VERTEX_3D vertexQuad[4];

	vertexQuad[0].Position = MakeFloat3(x - w * 0.5f, y - h * 0.5f, 0.0f);
	vertexQuad[1].Position = MakeFloat3(x + w * 0.5f, y - h * 0.5f, 0.0f);
	vertexQuad[2].Position = MakeFloat3(x - w * 0.5f, y + h * 0.5f, 0.0f);
	vertexQuad[3].Position = MakeFloat3(x + w * 0.5f, y + h * 0.5f, 0.0f);

	//引数のcolorをコピーする
	vertexQuad[0].Color = *color;
	vertexQuad[1].Color = *color;
	vertexQuad[2].Color = *color;
	vertexQuad[3].Color = *color;

	vertexQuad[0].TexCoord = MakeFloat2(0.0f, 0.0f);
	vertexQuad[1].TexCoord = MakeFloat2(1.0f, 0.0f);
	vertexQuad[2].TexCoord = MakeFloat2(0.0f, 1.0f);
	vertexQuad[3].TexCoord = MakeFloat2(1.0f, 1.0f);

	glBindBuffer(GL_ARRAY_BUFFER, g_SpriteVertexBuffer);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(VERTEX_3D) * 4, vertexQuad);


	glBindVertexArray(g_SpriteVertexArrayObject);
	glBindVertexBuffer(0, g_SpriteVertexBuffer, 0, sizeof(VERTEX_3D));

	SetTexture(texNo);

	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

	glBindVertexArray(0);
#else
	DRAW_SPRITE_BUFFER dsb = {};
	SpriteSetUp(&dsb);
	SpriteShape(&dsb,
		MkF2(x, y),
		0.0f,
		MkF2(w, h)
	);
	SpriteRelease(&dsb,
		*color,
		texNo,
		MkF2(0.0f, 0.0f),
		MkF2(1.0f, 1.0f),
		LOADTEXTURETYPE_MAIN,
		true,
		priority
	);
#endif

}


// 四角形の描画 回転＋色つきバージョン
//in
// x,y スプライトの表示座標（中心点）
// w,h スプライトのサイズ　w横h縦
// texNo テクスチャ画像の番号
// color 頂点カラー
// rot 回転角度（ラジアン）
void DrawSpriteColorRotateQuad(
	float x, float y, float w, float h,
	unsigned int texNo,
	Float4* color,
	float rot,
	DRAW_POLYGON_PRIORITY priority
)
{
#ifdef SWITCH_MODE

	VERTEX_3D vertexQuad[4];

	//原点(0,0)からの差分で座標を洗わす
	vertexQuad[0].Position = MakeFloat3(-w * 0.5f, -h * 0.5f, 0.0f);
	vertexQuad[1].Position = MakeFloat3(+w * 0.5f, -h * 0.5f, 0.0f);
	vertexQuad[2].Position = MakeFloat3(-w * 0.5f, +h * 0.5f, 0.0f);
	vertexQuad[3].Position = MakeFloat3(+w * 0.5f, +h * 0.5f, 0.0f);

	//sinΘとcosΘを作っておく
	float  co, si;
	co = cosf(rot);
	si = sinf(rot);

	//回転の計算
	float tempX, tempY;

	for (int i = 0; i < 4; i++)
	{
		tempX = (vertexQuad[i].Position.x *
			co - vertexQuad[i].Position.y * si)
			+ x;
		tempY = (vertexQuad[i].Position.x *
			si + vertexQuad[i].Position.y * co)
			+ y;
		vertexQuad[i].Position.x = tempX;
		vertexQuad[i].Position.y = tempY;
	}


	//引数のcolorをコピーする
	vertexQuad[0].Color = *color;
	vertexQuad[1].Color = *color;
	vertexQuad[2].Color = *color;
	vertexQuad[3].Color = *color;

	vertexQuad[0].TexCoord = MakeFloat2(0.0f, 0.0f);
	vertexQuad[1].TexCoord = MakeFloat2(1.0f, 0.0f);
	vertexQuad[2].TexCoord = MakeFloat2(0.0f, 1.0f);
	vertexQuad[3].TexCoord = MakeFloat2(1.0f, 1.0f);

	glBindBuffer(GL_ARRAY_BUFFER, g_SpriteVertexBuffer);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(VERTEX_3D) * 4, vertexQuad);


	glBindVertexArray(g_SpriteVertexArrayObject);
	glBindVertexBuffer(0, g_SpriteVertexBuffer, 0, sizeof(VERTEX_3D));

	SetTexture(texNo);

	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

	glBindVertexArray(0);
#else
	DRAW_SPRITE_BUFFER dsb = {};
	SpriteSetUp(&dsb);
	SpriteShape(&dsb,
		MkF2(x, y),
		rot,
		MkF2(w, h)
	);
	SpriteRelease(&dsb,
		*color,
		texNo,
		MkF2(0.0f, 0.0f),
		MkF2(1.0f, 1.0f),
		LOADTEXTURETYPE_MAIN,
		true,
		priority
	);
#endif

}

// 直線の描画 色つきバージョン
//in
// sx,sy 始点座標
// ex,ey 終点座標
// color 頂点カラー
void DrawLine(float sx, float sy, float ex, float ey, Float4 *color)
{
#ifdef SWITCH_MODE

	VERTEX_3D vertexLine[2];

	//原点(0,0)からの差分で座標を洗わす
	vertexLine[0].Position = MakeFloat3(sx, sy, 0.0f);
	vertexLine[1].Position = MakeFloat3(ex, ey, 0.0f);

	//引数のcolorをコピーする
	vertexLine[0].Color = *color;
	vertexLine[1].Color = *color;

	//テクスチャは使用しないが座標データは必要
	vertexLine[0].TexCoord = MakeFloat2(0.0f, 0.0f);
	vertexLine[1].TexCoord = MakeFloat2(1.0f, 0.0f);


	glBindBuffer(GL_ARRAY_BUFFER, g_SpriteVertexBuffer);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(VERTEX_3D) * 2, vertexLine);


	glBindVertexArray(g_SpriteVertexArrayObject);
	glBindVertexBuffer(0, g_SpriteVertexBuffer, 0, sizeof(VERTEX_3D));

	glDrawArrays(GL_LINES, 0, 2);

	glBindVertexArray(0);

#endif

}


// 色付き回転ありUV指定可能四角形の描画
void DrawSpriteQuadColorRotateTexUv(
	float x, float y, float w, float h,
	float rot,
	Float4 color,
	unsigned int texNo,
	Float2 texUV, Float2 texWH,
	DRAW_POLYGON_PRIORITY priority
)
{
#ifdef SWITCH_MODE

	VERTEX_3D vertexQuad[4];

	vertexQuad[0].Position = MakeFloat3(-w * 0.5f, -h * 0.5f, 0.0f);
	vertexQuad[1].Position = MakeFloat3(+w * 0.5f, -h * 0.5f, 0.0f);
	vertexQuad[2].Position = MakeFloat3(-w * 0.5f, +h * 0.5f, 0.0f);
	vertexQuad[3].Position = MakeFloat3(+w * 0.5f, +h * 0.5f, 0.0f);

	float rotRad = DegreeToRadian(rot);
	float si = sinf(rotRad),
		co = cosf(rotRad);
	for (int i = 0; i < 4; i++)
	{
		float tmpX, tmpY;
		//x = (xcosθ - ysinθ) + posX
		//y = (xsinθ + ycosθ) + posY
		tmpX = (vertexQuad[i].Position.x * co - vertexQuad[i].Position.y * si) + x;
		tmpY = (vertexQuad[i].Position.x * si + vertexQuad[i].Position.y * co) + y;

		vertexQuad[i].Position.x = tmpX;
		vertexQuad[i].Position.y = tmpY;
	}

	vertexQuad[0].Color = color;
	vertexQuad[1].Color = color;
	vertexQuad[2].Color = color;
	vertexQuad[3].Color = color;

	vertexQuad[0].TexCoord = MakeFloat2(texUV.x, texUV.y);
	vertexQuad[1].TexCoord = MakeFloat2(texUV.x + texWH.x, texUV.y);
	vertexQuad[2].TexCoord = MakeFloat2(texUV.x, texUV.y + texWH.y);
	vertexQuad[3].TexCoord = MakeFloat2(texUV.x + texWH.x, texUV.y + texWH.y);

	glBindBuffer(GL_ARRAY_BUFFER, g_SpriteVertexBuffer);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(VERTEX_3D) * 4, vertexQuad);


	glBindVertexArray(g_SpriteVertexArrayObject);
	glBindVertexBuffer(0, g_SpriteVertexBuffer, 0, sizeof(VERTEX_3D));

	SetTexture(texNo);

	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

	glBindVertexArray(0);
#else

	DRAW_SPRITE_BUFFER dsb = {};
	SpriteSetUp(&dsb);
	SpriteShape(&dsb,
		MkF2(x, y),
		rot,
		MkF2(w, h)
	);
	SpriteRelease(&dsb,
		color,
		texNo,
		texUV,
		texWH,
		LOADTEXTURETYPE_MAIN,
		true,
		priority
	);

#endif

}


// 四角形の描画 拡張版
void DrawSpriteEx(DRAW_SPRITE_DAT drawDat)
{
#ifdef SWITCH_MODE

	VERTEX_3D vertexQuad[4];

	vertexQuad[0].Position = MakeFloat3(-drawDat.w * 0.5f * drawDat.startScale.x,
		-drawDat.h * 0.5f * drawDat.startScale.y, 0.0f);
	vertexQuad[1].Position = MakeFloat3(+drawDat.w * 0.5f * drawDat.startScale.x,
		-drawDat.h * 0.5f * drawDat.startScale.y, 0.0f);
	vertexQuad[2].Position = MakeFloat3(-drawDat.w * 0.5f * drawDat.startScale.x,
		+drawDat.h * 0.5f * drawDat.startScale.y, 0.0f);
	vertexQuad[3].Position = MakeFloat3(+drawDat.w * 0.5f * drawDat.startScale.x,
		+drawDat.h * 0.5f * drawDat.startScale.y, 0.0f);

	{
		float rotRad = DegreeToRadian(drawDat.rot);
		float si = sinf(rotRad),
			co = cosf(rotRad);
		for (int i = 0; i < 4; i++)
		{
			float tmpX, tmpY;
			//x = (xcosθ - ysinθ) + posX
			//y = (xsinθ + ycosθ) + posY
			tmpX = (vertexQuad[i].Position.x * co - vertexQuad[i].Position.y * si) + drawDat.x * drawDat.startScale.x;
			tmpY = (vertexQuad[i].Position.x * si + vertexQuad[i].Position.y * co) + drawDat.y * drawDat.startScale.y;

			vertexQuad[i].Position.x = tmpX;
			vertexQuad[i].Position.y = tmpY;
		}
	}

	{
		float rotRad = DegreeToRadian(drawDat.startRot);
		float si = sinf(rotRad),
			co = cosf(rotRad);

		for (int i = 0; i < 4; i++)
		{
			float tmpX, tmpY;
			//x = (xcosθ - ysinθ) + posX
			//y = (xsinθ + ycosθ) + posY
			tmpX = (vertexQuad[i].Position.x * co - vertexQuad[i].Position.y * si) + drawDat.startPos.x;
			tmpY = (vertexQuad[i].Position.x * si + vertexQuad[i].Position.y * co) + drawDat.startPos.y;

			vertexQuad[i].Position.x = tmpX;
			vertexQuad[i].Position.y = tmpY;
		}
	}

	vertexQuad[0].Color = drawDat.color;
	vertexQuad[1].Color = drawDat.color;
	vertexQuad[2].Color = drawDat.color;
	vertexQuad[3].Color = drawDat.color;

	vertexQuad[0].TexCoord = MakeFloat2(drawDat.texUV.x, drawDat.texUV.y);
	vertexQuad[1].TexCoord = MakeFloat2(drawDat.texUV.x + drawDat.texWH.x, drawDat.texUV.y);
	vertexQuad[2].TexCoord = MakeFloat2(drawDat.texUV.x, drawDat.texUV.y + drawDat.texWH.y);
	vertexQuad[3].TexCoord = MakeFloat2(drawDat.texUV.x + drawDat.texWH.x, drawDat.texUV.y + drawDat.texWH.y);

	glBindBuffer(GL_ARRAY_BUFFER, g_SpriteVertexBuffer);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(VERTEX_3D) * 4, vertexQuad);


	glBindVertexArray(g_SpriteVertexArrayObject);
	glBindVertexBuffer(0, g_SpriteVertexBuffer, 0, sizeof(VERTEX_3D));

	SetTexture(drawDat.texNo);

	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

	glBindVertexArray(0);
#else

	DRAW_SPRITE_BUFFER dsb = {};
	SpriteSetUp(&dsb);
	SpriteShape(&dsb,
		MkF3(drawDat.x, drawDat.y, drawDat.z),
		MkF3(0.0f, 0.0f, drawDat.rot),
		MkF2(drawDat.w, drawDat.h)
	);
	SpriteShape(&dsb,
		MkF3(0.0f, 0.0f, 0.0f),
		MkF3(drawDat.rot3D.x, drawDat.rot3D.y, drawDat.rot3D.z),
		MkF2(1.0f, 1.0f)
	);

	SpriteShape(&dsb,
		MkF2(drawDat.startPos.x, drawDat.startPos.y),
		drawDat.startRot,
		MkF2(drawDat.startScale.x, drawDat.startScale.y)
	);
	SpriteShape(&dsb,
		MkF3(drawDat.startPos3D.x, drawDat.startPos3D.y, drawDat.startPos3D.z),
		MkF3(drawDat.startRot3D.x, drawDat.startRot3D.y, drawDat.startRot3D.z),
		MkF2(1.0f, 1.0f)
	);

	SpriteRelease(&dsb,
		drawDat.color,
		drawDat.texNo,
		drawDat.texUV,
		drawDat.texWH,
		drawDat.loadTexType,
		true,
		DRAW_POLYGON_PRIORITY_ALWAYS
	);

#endif

}


//----------------------------------------
//処理を小分けにしたもの
//より複雑な変形が可能
//

//セットアップ
void SpriteSetUp(DRAW_SPRITE_BUFFER* dsBuffer, Vertex_DX vertex[SPRITE_NUM_VERTEX_SQUARE])
{
#ifdef SWITCH_MODE

	dsBuffer->vertexQuad[0].Position = MakeFloat3(-0.5f, -0.5f, 0.0f);
	dsBuffer->vertexQuad[1].Position = MakeFloat3(+0.5f, -0.5f, 0.0f);
	dsBuffer->vertexQuad[2].Position = MakeFloat3(-0.5f, +0.5f, 0.0f);
	dsBuffer->vertexQuad[3].Position = MakeFloat3(+0.5f, +0.5f, 0.0f);

#else
	(*dsBuffer) = {};

	DPD_VERTEX dpdVertex = {};
	dpdVertex.type = DRAW_POLYGON_TYPE_QUAD;

	PolygonSetUp(&dsBuffer->buf, dpdVertex);
#endif
}
//変形
void SpriteShape(DRAW_SPRITE_BUFFER* dsBuffer,
	Float2 pos, float rot, Float2 scale)
{
	SpriteShape(dsBuffer,
		Float2ToFloat3(pos), MkF3(0.0f, 0.0f, rot), scale);
}

void SpriteShape(DRAW_SPRITE_BUFFER* dsBuffer,
	Float3 pos, Float3 rot, Float2 scale)
{
#ifdef SWITCH_MODE

	for (int i = 0; i < 4; i++)
	{
		dsBuffer->vertexQuad[i].Position.x *= scale.x;
		dsBuffer->vertexQuad[i].Position.y *= scale.y;
	}

	float rotRad = DegreeToRadian(rot);
	float si = sinf(rotRad),
		co = cosf(rotRad);
	for (int i = 0; i < 4; i++)
	{
		float tmpX, tmpY;
		//x = (xcosθ - ysinθ) + posX
		//y = (xsinθ + ycosθ) + posY
		tmpX = (dsBuffer->vertexQuad[i].Position.x * co - dsBuffer->vertexQuad[i].Position.y * si) + pos.x;
		tmpY = (dsBuffer->vertexQuad[i].Position.x * si + dsBuffer->vertexQuad[i].Position.y * co) + pos.y;

		dsBuffer->vertexQuad[i].Position.x = tmpX;
		dsBuffer->vertexQuad[i].Position.y = tmpY;
	}

#else
	PolygonShape(&dsBuffer->buf, pos, rot, Float2ToFloat3(scale, 1.0f));
#endif
}
//解放(描画)
void SpriteRelease(DRAW_SPRITE_BUFFER* dsBuffer,
	Float4 color, unsigned int texNo, Float2 texUV, Float2 texWH, LOADTEXTURETYPE loadTexType,
	bool fixed, DRAW_POLYGON_PRIORITY priority
)
{
#ifdef SWITCH_MODE

	dsBuffer->vertexQuad[0].Color = color;
	dsBuffer->vertexQuad[1].Color = color;
	dsBuffer->vertexQuad[2].Color = color;
	dsBuffer->vertexQuad[3].Color = color;

	dsBuffer->vertexQuad[0].TexCoord = MakeFloat2(texUV.x, texUV.y);
	dsBuffer->vertexQuad[1].TexCoord = MakeFloat2(texUV.x + texWH.x, texUV.y);
	dsBuffer->vertexQuad[2].TexCoord = MakeFloat2(texUV.x, texUV.y + texWH.y);
	dsBuffer->vertexQuad[3].TexCoord = MakeFloat2(texUV.x + texWH.x, texUV.y + texWH.y);

	//bool draw = false;
	//for (int i = 0; i < 4; i++)
	//{
	//}

	if (CheckBoxCollider(MkF2(0.0f, 0.0f),
		(MkF2(dsBuffer->vertexQuad[0].Position.x, dsBuffer->vertexQuad[0].Position.y)),
		(MkF2(SCREEN_WIDTH, SCREEN_HEIGHT)),
		(MkF2(dsBuffer->vertexQuad[3].Position.x - dsBuffer->vertexQuad[0].Position.x,
			dsBuffer->vertexQuad[3].Position.y - dsBuffer->vertexQuad[0].Position.y))
	)
		|| CheckBoxCollider(MkF2(0.0f, 0.0f),
			(MkF2(dsBuffer->vertexQuad[0].Position.x, dsBuffer->vertexQuad[0].Position.y)),
			(MkF2(SCREEN_WIDTH, SCREEN_HEIGHT)), (MkF2(1.0f, 1.0f))
		)
		|| CheckBoxCollider(MkF2(0.0f, 0.0f),
			(MkF2(dsBuffer->vertexQuad[1].Position.x, dsBuffer->vertexQuad[1].Position.y)),
			(MkF2(SCREEN_WIDTH, SCREEN_HEIGHT)), (MkF2(1.0f, 1.0f))
		)
		|| CheckBoxCollider(MkF2(0.0f, 0.0f),
			(MkF2(dsBuffer->vertexQuad[2].Position.x, dsBuffer->vertexQuad[2].Position.y)),
			(MkF2(SCREEN_WIDTH, SCREEN_HEIGHT)), (MkF2(1.0f, 1.0f))
		)
		|| CheckBoxCollider(MkF2(0.0f, 0.0f),
			(MkF2(dsBuffer->vertexQuad[3].Position.x, dsBuffer->vertexQuad[3].Position.y)),
			(MkF2(SCREEN_WIDTH, SCREEN_HEIGHT)), (MkF2(1.0f, 1.0f))
		)
		)
	{ //画面外のものは描画処理をしない
		glBindBuffer(GL_ARRAY_BUFFER, g_SpriteVertexBuffer);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(VERTEX_3D) * 4, dsBuffer->vertexQuad);


		glBindVertexArray(g_SpriteVertexArrayObject);
		glBindVertexBuffer(0, g_SpriteVertexBuffer, 0, sizeof(VERTEX_3D));

		SetTexture(texNo);

		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

		glBindVertexArray(0);

	}
#else
	POLYGON_RELEASE_OPTION opt = {};
	opt.color = color;
	opt.texNo = texNo;
	opt.texUV = texUV;
	opt.texWH = texWH;
	opt.loadTexType = loadTexType;
	opt.fixed = fixed;
	opt.priority = priority;

	SpriteReleaseEx(dsBuffer, opt);
#endif
}

void SpriteReleaseEx(DRAW_SPRITE_BUFFER* dsBuffer, POLYGON_RELEASE_OPTION opt)
{
	PolygonRelease(&dsBuffer->buf, opt);
}


//
//----------------------------------------

Float2 AnimationUV(int no, int xcnt, int hcnt)
{
	Float2 uv = {};

	if (xcnt <= 0 || hcnt <= 0)
	{
		return uv;
	}

	float w = 1.0f / xcnt;
	float h = 1.0f / hcnt;

	uv.x = no % xcnt * w;
	uv.y = no / xcnt * h;

	return uv;
}


//{
//	/*{ XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), XMFLOAT2(0.0f, 0.0f) },
//	{ XMFLOAT3(1.0f, 0.0f, 0.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), XMFLOAT2(1.0f, 0.0f) },
//	{ XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), XMFLOAT2(0.0f, 1.0f) },
//	{ XMFLOAT3(1.0f, 1.0f, 0.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), XMFLOAT2(1.0f, 1.0f) },*/
//
//
//	//{ XMFLOAT3(-0.5f, -0.5f, 0.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), XMFLOAT2(0.0f, 0.0f), XMFLOAT3(+1.0f, 0.0f, 0.0f) },
//	//{ XMFLOAT3(+0.5f, -0.5f, 0.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), XMFLOAT2(1.0f, 0.0f), XMFLOAT3(-1.0f, 0.0f, 0.0f) },
//	//{ XMFLOAT3(-0.5f, +0.5f, 0.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), XMFLOAT2(0.0f, 1.0f), XMFLOAT3(+1.0f, 0.0f, 0.0f) },
//	//{ XMFLOAT3(+0.5f, +0.5f, 0.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), XMFLOAT2(1.0f, 1.0f), XMFLOAT3(-1.0f, 0.0f, 0.0f) },
//
//	//{ XMFLOAT3(-0.5f, -0.5f, 0.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), XMFLOAT2(0.0f, 0.0f), XMFLOAT3(0.0f, -1.0f, 0.0f) },
//	//{ XMFLOAT3(+0.5f, -0.5f, 0.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), XMFLOAT2(1.0f, 0.0f), XMFLOAT3(0.0f, -1.0f, 0.0f) },
//	//{ XMFLOAT3(-0.5f, +0.5f, 0.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), XMFLOAT2(0.0f, 1.0f), XMFLOAT3(0.0f, +1.0f, 0.0f) },
//	//{ XMFLOAT3(+0.5f, +0.5f, 0.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), XMFLOAT2(1.0f, 1.0f), XMFLOAT3(0.0f, +1.0f, 0.0f) },
//
//	//{ XMFLOAT3(-0.5f, -0.5f, 0.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), XMFLOAT2(0.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, -1.0f) },
//	//{ XMFLOAT3(+0.5f, -0.5f, 0.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), XMFLOAT2(1.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, -1.0f) },
//	//{ XMFLOAT3(-0.5f, +0.5f, 0.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), XMFLOAT2(0.0f, 1.0f), XMFLOAT3(0.0f, 0.0f, -1.0f) },
//	//{ XMFLOAT3(+0.5f, +0.5f, 0.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), XMFLOAT2(1.0f, 1.0f), XMFLOAT3(0.0f, 0.0f, -1.0f) },
//
//
//	{ XMFLOAT3(-0.5f, -0.5f, 0.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), XMFLOAT2(0.0f, 0.0f), XMFLOAT3(-1.0f, -1.0f, 0.1f) },
//	{ XMFLOAT3(+0.5f, -0.5f, 0.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), XMFLOAT2(1.0f, 0.0f), XMFLOAT3(+1.0f, -1.0f, 0.1f) },
//	{ XMFLOAT3(-0.5f, +0.5f, 0.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), XMFLOAT2(0.0f, 1.0f), XMFLOAT3(-1.0f, +1.0f, 0.1f) },
//	{ XMFLOAT3(+0.5f, +0.5f, 0.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), XMFLOAT2(1.0f, 1.0f), XMFLOAT3(+1.0f, +1.0f, 0.1f) },
//}