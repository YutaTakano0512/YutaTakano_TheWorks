// ===================================================
// texture.h テクスチャの制御
// 
// hara sougo  2024/11/08
// ===================================================
#ifndef _TEXTURE_H_
#define _TEXTURE_H_

#include "../Systems/sys_general.h"

#include "../../Core/_systemModeSwitch.h"

#ifdef SWITCH_MODE
#else
#include <d3d11.h>
#endif

// ===================================================
// 列挙体定義
// ===================================================
enum LOADTEXTURETYPE : char
{
	LOADTEXTURETYPE_MAIN = 0,
	LOADTEXTURETYPE_GRAY,

	LOADTEXTURETYPE_MAX,
};

// ===================================================
// 構造体宣言
// ===================================================
struct TEXTURE_DAT {
	unsigned int main;
	unsigned int gray;
};

// ===================================================
// プロトタイプ宣言
// ===================================================
void InitTexture(void);
void UninitTexture(void);

#ifdef IS_TEXDATA
unsigned int LoadTextureExOpt(const char* FileName, LOADTEXTURETYPE opt);
TEXTURE_DAT LoadTextureEx(const char* FileName);

unsigned int LoadTexture(const char* FileName, bool force);
void UnloadTexture(unsigned int Texture);
void SetTexture(unsigned int Texture);
void WaitLoadTexture();
#endif

//DX用
#ifdef SWITCH_MODE
#else
ID3D11ShaderResourceView* GetTexture_DX(unsigned int Texture);
int GetTextureWidth(unsigned int Texture);
int GetTextureHeight(unsigned int Texture);
#endif

#endif