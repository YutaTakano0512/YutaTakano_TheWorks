//---------------------------------------------------------------------------
// spriteNumber.h
// 
// sougo hara  2024/09/03
//---------------------------------------------------------------------------

#ifndef _SPRITE_NUMBER_H_
#define _SPRITE_NUMBER_H_

#include "../Systems/sys_general.h"
#include "../Texture/texData.h"
#include "sprite.h"


void SpriteNumberInitialize(void);
void SpriteNumberGapDraw(int v, Float2 pos, Float2 size, float gap);
void SpriteNumberDraw(int v, Float2 pos, Float2 size, int digit, Float4 color, float gap,
	bool plus_minus = false);

enum class SPRITE_NUMBER_DRAW_EX_ALIGN {
    Right = 0,
    Center,
    Left
};
struct SPRITE_NUMBER_DRAW_EX {
    int value;

    Float2 pos;
    Float2 size;
    int digit; //åÖêî
    Float4 color;
    float gap; //êîílÇÃä‘
    bool plus_minus;

    unsigned int texNo;
    int texCntX;
    int texCntY;

    SPRITE_NUMBER_DRAW_EX_ALIGN align;


    DPD_SHADER_TYPE shadertype;
    DPD_SHADER_TYPE_EX_OPT shadertypeExOpt;
};
void SpriteNumberDrawEx(SPRITE_NUMBER_DRAW_EX dat);

void SpriteNumberTimerDraw(SPRITE_NUMBER_DRAW_EX dat);

#endif