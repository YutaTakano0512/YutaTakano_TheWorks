//---------------------------------------------------------------------------
// spriteNumber.cpp
// 
// sougo hara  2024/09/03
//---------------------------------------------------------------------------

#include "../../game_main.h"
//#include "sprite.h"
#include "spriteNumber.h"

#include "../Systems/camera.h"

// ===================================================
// マクロ定義
// ===================================================
#define NUMBER_DIGIT			(5)								// スコアの桁
#define NUMBER_NUMBER_GAP		(-20.0f)							// スコアの数値と数値の隙間
#define NUMBER_PATTERN_NUM_W	(5)								// 横パターン数
#define NUMBER_PATTERN_NUM_H	(5)								// 縦パターン数
#define NUMBER_PATTERN_SIZE_W	(1.0f / NUMBER_PATTERN_NUM_W)	// 横パターンサイズ
#define NUMBER_PATTERN_SIZE_H	(1.0f / NUMBER_PATTERN_NUM_H)	// 縦パターンサイズ

// ===================================================
// グローバル変数
// ===================================================
static TEX_DAT g_numberTex = {};


void SpriteNumberInitialize(void)
{
	g_numberTex = GetTexData(TEX_TYPE_UI_NUMBER);
}

void SpriteNumberDraw(int v, Float2 pos, Float2 size, int digit, Float4 color, float gap, bool plus_minus)
{
	int value = (int)abs(v);


	int digittmp = digit;

	if (digittmp == -1)
	{
		digittmp = 0;

		int valuetmp = value;
		while (valuetmp != 0)
		{
			valuetmp /= 10;
			digittmp++;
		}
	}

	int digi = plus_minus ? digittmp + 1 : digittmp;
	for (int i = 0; i < digi; i++)
	{
		bool ispm = plus_minus ? i == digittmp : false;

		int num = 0;

		if (!ispm)
		{

			// １の位の数値を取り出す
			num = value % 10;

			// １桁右にずらす
			value /= 10;
		}

		//DRAW_SPRITE_DAT dsdat = {};
		//dsdat.x = pos.x - (size.x + gap) * i;
		//dsdat.y = pos.y;
		//dsdat.w = size.x;
		//dsdat.h = size.y;
		//dsdat.rot = 0.0f;
		//dsdat.color = color;

		//dsdat.texNo = g_numberTex.textureId;
		//dsdat.loadTexType = LOADTEXTURETYPE_MAIN;

		//if (ispm)
		//{
		//	dsdat.texUV = AnimationUV(v < 0 ? 11 : 10, NUMBER_PATTERN_NUM_W, NUMBER_PATTERN_NUM_H);
		//}
		//else
		//{
		//	dsdat.texUV = MkF2(
		//		num % NUMBER_PATTERN_NUM_W * NUMBER_PATTERN_SIZE_W,
		//		num / NUMBER_PATTERN_NUM_W * NUMBER_PATTERN_SIZE_H
		//	);
		//}

		//dsdat.texWH = MkF2(NUMBER_PATTERN_SIZE_W, NUMBER_PATTERN_SIZE_H);
		//dsdat.startPos = MkF2(0.0f, 0.0f);
		//dsdat.startRot = 0.0f;
		//dsdat.startScale = MkF2(1.0f, 1.0f);

		//DrawSpriteInCamera(dsdat, false, true, false);

        DrawSpriteInCamera(
			//表示座標 (四角形の真ん中)
			pos.x - (size.x + gap) * i,
			pos.y,

			//サイズ width hight
			size.x,
			size.y,

			//角度
			0.0f,

			//色
			color,

			g_numberTex.textureId,  //画像(テクスチャ)の番号

			ispm ?
				AnimationUV(v < 0 ? 11 : 10, NUMBER_PATTERN_NUM_W, NUMBER_PATTERN_NUM_H)
				:
				MkF2(
					num % NUMBER_PATTERN_NUM_W * NUMBER_PATTERN_SIZE_W,
					num / NUMBER_PATTERN_NUM_W * NUMBER_PATTERN_SIZE_H
				)
			,

			MkF2(NUMBER_PATTERN_SIZE_W, NUMBER_PATTERN_SIZE_H)
		);
	}
}

void SpriteNumberGapDraw(int v, Float2 pos, Float2 size, float gap)
{
	int value = v;

	for (int i = 0; i < NUMBER_DIGIT; i++)
	{
		int num = 0;

		// １の位の数値を取り出す
		num = value % 10;

		// １桁右にずらす
		value /= 10;

		DRAW_SPRITE_DAT dsdat = {};
		dsdat.x = pos.x - (size.x + gap) * i;
		dsdat.y = pos.y + (num / 5) * 8.0f;
		dsdat.w = size.x;
		dsdat.h = size.y;
		dsdat.rot = 0.0f;
		dsdat.color = MkF4(1.0f, 1.0f, 1.0f, 1.0f);

		dsdat.texNo = g_numberTex.textureId;
		dsdat.loadTexType = LOADTEXTURETYPE_MAIN;

		dsdat.texUV = MkF2(
			num % NUMBER_PATTERN_NUM_W * NUMBER_PATTERN_SIZE_W,
			num / NUMBER_PATTERN_NUM_W * NUMBER_PATTERN_SIZE_H
		);
		dsdat.texWH = MkF2(NUMBER_PATTERN_SIZE_W, NUMBER_PATTERN_SIZE_H);
		dsdat.startPos = MkF2(0.0f, 0.0f);
		dsdat.startRot = 0.0f;
		dsdat.startScale = MkF2(1.0f, 1.0f);

		DrawSpriteInCamera(dsdat, false, true);
	}
}




void SpriteNumberDrawEx(SPRITE_NUMBER_DRAW_EX dat)
{
    int value = (int)abs(dat.value);


    int digittmp = dat.digit;

    if (digittmp == -1)
    {
        digittmp = 0;

        int valuetmp = value;
        while (valuetmp != 0)
        {
            valuetmp /= 10;
            digittmp++;
        }
    }

    float alignmod = 0;
    if (dat.align == SPRITE_NUMBER_DRAW_EX_ALIGN::Center)
    {
        alignmod = ((dat.size.x + dat.gap) * (digittmp - 1) / 2);
    }
    else if (dat.align == SPRITE_NUMBER_DRAW_EX_ALIGN::Left)
    {
        alignmod = ((dat.size.x + dat.gap) * (digittmp - 1));
    }

    int digi = dat.plus_minus ? digittmp + 1 : digittmp;
    for (int i = 0; i < digi; i++)
    {
        bool ispm = dat.plus_minus ? i == digittmp : false;

        int num = 0;

        if (!ispm)
        {

            // １の位の数値を取り出す
            num = value % 10;

            // １桁右にずらす
            value /= 10;
        }

        DrawSpriteInCamera(
            //表示座標 (四角形の真ん中)
            dat.pos.x - (dat.size.x + dat.gap) * i + alignmod,
            dat.pos.y,

            //サイズ width hight
            dat.size.x,
            dat.size.y,

            //角度
            0.0f,

            //色
            dat.color,

            dat.texNo,  //画像(テクスチャ)の番号

            ispm ?
            AnimationUV(dat.value < 0 ? 11 : 10, dat.texCntX, dat.texCntY)
            :
            MkF2(
                num % dat.texCntX * (1.0f / dat.texCntX),
                num / dat.texCntX * (1.0f / dat.texCntY)
            )
            ,

            MkF2(1.0f / dat.texCntX, 1.0f / dat.texCntY),
            DRAW_POLYGON_PRIORITY_FORCE_NORMAL,
            dat.shadertype,
            dat.shadertypeExOpt
        );
    }
}

void SpriteNumberTimerDraw(SPRITE_NUMBER_DRAW_EX dat)
{
    //int value = (int)abs(dat.value);

    constexpr double e = 1.0 / FRAME_ONE_SECOND;
    double sec = ((double)dat.value * e);

    int ms = (int)(sec * 100) % 100;
    int s = (int)(sec) % 60;
    int m = (int)(sec) / 60;

    int values[3] = {};
    if (dat.digit == 1)
    {
        values[0] = s;
    }
    if (dat.digit == 2)
    {
        values[0] = ms;
        values[1] = s;
    }
    if (dat.digit == 3)
    {
        values[0] = ms;
        values[1] = s;
        values[2] = m;
    }

    float addOfX = 0;
    for (int j = 0; j < dat.digit; j++)
    {
        int value = values[j];
        //int digittmp = dat.digit;

        //if (digittmp == -1)
        //{
        //    digittmp = 0;

        //    int valuetmp = value;
        //    while (valuetmp != 0)
        //    {
        //        valuetmp /= 10;
        //        digittmp++;
        //    }
        //}

        float alignmod = 0;
        //if (dat.align == SPRITE_NUMBER_DRAW_EX_ALIGN::Center)
        //{
        //    alignmod = ((dat.size.x + dat.gap) * (digittmp - 1) / 2);
        //}
        //else if (dat.align == SPRITE_NUMBER_DRAW_EX_ALIGN::Left)
        //{
        //    alignmod = ((dat.size.x + dat.gap) * (digittmp - 1));
        //}

        int digi = ((dat.digit - 1) == j) ? 2 : 3;
        for (int i = 0; i < digi; i++)
        {
            bool issplit = (i == (digi - 1) && digi > 2);

            int num = 0;

            if (!issplit)
            {

                // １の位の数値を取り出す
                num = value % 10;

                // １桁右にずらす
                value /= 10;
            }

            Float2 pos = {};
            Float2 sca = {1.0f, 1.0f};
            Float2 uv = {};
            Float2 wh = {};

            uv = issplit ?
                AnimationUV(10, dat.texCntX, dat.texCntY)
                :
                MkF2(
                    num % dat.texCntX * (1.0f / dat.texCntX),
                    num / dat.texCntX * (1.0f / dat.texCntY)
                )
                ;
            wh = MkF2(1.0f / dat.texCntX, 1.0f / dat.texCntY);

            if (issplit && 0 == j)
            {
                sca.y *= 0.5f;
                pos.y += dat.size.y / 2 * sca.y;
                wh.y *= sca.y;
            }

            addOfX += (dat.size.x + dat.gap);
            DrawSpriteInCamera(
                //表示座標 (四角形の真ん中)
                dat.pos.x +  pos.x  - addOfX + alignmod,
                dat.pos.y +  pos.y ,

                //サイズ width hight
                dat.size.x * sca.x,
                dat.size.y * sca.y,

                //角度
                0.0f,

                //色
                dat.color,

                dat.texNo,  //画像(テクスチャ)の番号

                uv,

                wh,
                DRAW_POLYGON_PRIORITY_FORCE_NORMAL,
                dat.shadertype,
                dat.shadertypeExOpt
            );
        }
    }

}
