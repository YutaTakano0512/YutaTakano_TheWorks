// ===================================================
// bg.cpp 背景制御
// 
// hara sougo  2024/07/30
// ===================================================
#include "../../game_main.h"
#include "../Texture/texData.h"
#include "../Sprite/sprite.h"
#include "bg.h"
#include "../Systems/camera.h"

#define BG_LAYER_NUM_GENERAL_0 (8)

//
static TEX_DAT g_tex_test_none = {};
static TEX_DAT g_tex_test_title = {};
static TEX_DAT g_tex_test_game = {};
static TEX_DAT g_tex_test_result = {};

//追加
static TEX_DAT g_tex_test_result_clear = {};
static TEX_DAT g_tex_test_result_gameover = {};


static TEX_DAT g_tex_iseki = {};
//static TEX_DAT g_tex_iseki_mut[5] = {};

static TEX_DAT g_tex_general[8] = {};

static TEX_DAT g_tex_tutorial_base = {};
static int g_ShaderIndex_TutorialBg_Vertex = 0;
static int g_ShaderIndex_TutorialBg_Pixel  = 0;


static BG_DAT bgDat = {};

void InitBg()
{
	bgDat = {};
	g_tex_test_none = GetTexData(TEX_TYPE_VOID);
	//g_tex_test_title = GetTexData(TEX_TYPE_BG_TESTTITLE);
	g_tex_test_title = GetTexData(TEX_TYPE_TITLE_COMSEPTART);
	g_tex_test_game = GetTexData(TEX_TYPE_BG_TESTN);
	g_tex_test_result = GetTexData(TEX_TYPE_BG_TESTRESULT);
	

	g_tex_iseki = GetTexData(TEX_TYPE_BG_ISEKI_TEST);

    //g_tex_iseki_mut[0] = LoadTexData("TEX/effect/boss/tex_inp_bk_01");
    //g_tex_iseki_mut[1] = LoadTexData("TEX/effect/boss/tex_inp_bk_02");
    //g_tex_iseki_mut[2] = LoadTexData("TEX/effect/barrier/tex_br_noise");
    //g_tex_iseki_mut[3] = LoadTexData("TEX/effect/barrier/tex_br_wave");
    //g_tex_iseki_mut[4] = LoadTexData("TEX/effect/barrier/tex_br_wave02");

    g_tex_general[0] = LoadTexData("TEX/bg/tutorial/bg0_000");
    g_tex_general[1] = LoadTexData("TEX/bg/tutorial/bg0_010");
    g_tex_general[2] = LoadTexData("TEX/bg/tutorial/bg0_020");
    g_tex_general[3] = LoadTexData("TEX/bg/tutorial/bg0_030");
    g_tex_general[4] = LoadTexData("TEX/bg/tutorial/bg0_040");
    g_tex_general[5] = LoadTexData("TEX/bg/tutorial/bg0_050");
    g_tex_general[6] = LoadTexData("TEX/bg/tutorial/bg0_060");
    g_tex_general[7] = LoadTexData("TEX/bg/tutorial/bg0_070");

    g_tex_tutorial_base = LoadTexData("TEX/bg/tutorial/base");
    g_ShaderIndex_TutorialBg_Vertex = DX_GetShaderIndex("SHADER/vs_general");
    g_ShaderIndex_TutorialBg_Pixel = DX_GetShaderIndex("SHADER/ps_bg_tutorial");

	bgDat.type = BG_TYPE_NONE;
}
void UninitBg()
{
    UnloadTexData(g_tex_general[0]);
    UnloadTexData(g_tex_general[1]);
    UnloadTexData(g_tex_general[2]);
    UnloadTexData(g_tex_general[3]);
    UnloadTexData(g_tex_general[4]);
    UnloadTexData(g_tex_general[5]);
    UnloadTexData(g_tex_general[6]);
    UnloadTexData(g_tex_general[7]);
    UnloadTexData(g_tex_tutorial_base);

    //UnloadTexData(g_tex_iseki_mut[0]);
    //UnloadTexData(g_tex_iseki_mut[1]);
    //UnloadTexData(g_tex_iseki_mut[2]);
    //UnloadTexData(g_tex_iseki_mut[3]);
    //UnloadTexData(g_tex_iseki_mut[4]);
}

void UpdateBg()
{
	bgDat.frame++;
}

void DrawBg()
{
	TEX_DAT texTmp = {};
	Float4 colorTmp = MkF4(1.0f, 1.0f, 1.0f, 1.0f);

    float framef = (float)bgDat.frame;
	if (bgDat.type == BG_TYPE_GENERAL_0)
	{
	}
    else if (bgDat.type == BG_TYPE_TUTORIAL_0)
    {
        DRAW_POLYGON_DAT drawDatTmp = {};
        //drawDatTmp.pos.x = dxCinfo.CameraVector.x * -8000.0f;
        //drawDatTmp.pos.y = dxCinfo.CameraVector.y * -8000.0f;
        //drawDatTmp.pos.z = dxCinfo.CameraVector.z * -8000.0f;
        //drawDatTmp.pos.z = -2000.0f;
        drawDatTmp.size = MkF3(
            +SCREEN_WIDTH,
            +SCREEN_HEIGHT,
            +1.0f);


        drawDatTmp.rot = 0.0f;

        drawDatTmp.rot3D = MkF3(0.0f, 40.0f, 0.0f);

        //drawDatTmp.color = { 0.6f, 0.6f, 0.75f, 1.0f };

        drawDatTmp.loadTexType = LOADTEXTURETYPE_MAIN;

        drawDatTmp.texUV = { 0.0f, 0.0f };
        drawDatTmp.texWH = { 10.0f, 1.0f };
        drawDatTmp.startScale = MkF2(1.0f, 1.0f);
        drawDatTmp.startScale3D = MkF3(1.0f, 1.0f, 1.0f);

        drawDatTmp.shaderOpt.outline = DPD_SHADER_OPT_OUTLINE_NONE;
        drawDatTmp.shaderOpt.bloom = DPD_SHADER_OPT_BLOOM_NONE;

        DRAW_POLYGON_IN_CAMERA_EX_OPTION opt = {};
        opt.reverse = false;
        opt.fixed = false;
        opt.enableShade = false;
        opt.priority = DRAW_POLYGON_PRIORITY_DEEPEST;
        opt.depthBuffIgnore = true;

        drawDatTmp.modelNo = 0;

        //{
        //    DRAW_POLYGON_DAT drawDatCpyTmp = drawDatTmp;
        //    drawDatCpyTmp.size = MkF3(
        //        +SCREEN_WIDTH * 100.0f,
        //        +SCREEN_HEIGHT * 100.0f,
        //        +1.0f);
        //    drawDatCpyTmp.startRot3D.z = 0.2f * framef;
        //    drawDatCpyTmp.color = MkF4(1.0f, 1.0f, 1.0f, 0.12f);
        //    drawDatCpyTmp.texUV = { 0.0f, 0.0f };
        //    drawDatCpyTmp.texWH = { 30.0f, 30.0f };
        //    drawDatCpyTmp.texNo = g_tex_general[1].textureId;
        //    drawDatCpyTmp.loadTexType = LOADTEXTURETYPE_GRAY;
        //    drawDatCpyTmp.shaderOpt.blendState = DPD_SHADER_OPT_BLEND_STATE_ADDCOLOR;
        //    DrawPolygonInCameraEx(drawDatCpyTmp, opt);
        //}
        for (int i = 0; i < 3; i++)
        {
            DRAW_POLYGON_DAT drawDatCpyTmp = drawDatTmp;
            drawDatCpyTmp.size = MkF3(
                +SCREEN_WIDTH * 100.0f,
                +SCREEN_HEIGHT * 10.0f,
                +1.0f);
            drawDatCpyTmp.startRot3D.z = 80.0f * i;
            drawDatCpyTmp.color = MkF4(1.0f, 1.0f, 1.0f, 0.5f);
            drawDatCpyTmp.texUV = { 0.0f + framef * -0.002f, 0.0f };
            drawDatCpyTmp.texWH = { 20.0f, 2.0f };
            drawDatCpyTmp.texNo = g_tex_general[6].textureId;
            drawDatCpyTmp.shaderOpt.blendState = DPD_SHADER_OPT_BLEND_STATE_ADDCOLOR;
            DrawPolygonInCameraEx(drawDatCpyTmp, opt);
        }
        for (int i = 0; i < 3; i++)
        {
            DRAW_POLYGON_DAT drawDatCpyTmp = drawDatTmp;
            drawDatCpyTmp.size = MkF3(
                +SCREEN_WIDTH * 100.0f,
                +SCREEN_HEIGHT * 10.0f,
                +1.0f);
            drawDatCpyTmp.startRot3D.z = 80.0f * i;
            drawDatCpyTmp.color = MkF4(0.0f, 1.8f, 1.2f, 0.2f);
            drawDatCpyTmp.texUV = { 0.0f + framef * 0.003f, 0.0f };
            drawDatCpyTmp.texWH = { 60.0f, 6.0f };
            drawDatCpyTmp.texNo = g_tex_general[4].textureId;
            drawDatCpyTmp.loadTexType = LOADTEXTURETYPE_GRAY;
            drawDatCpyTmp.shaderOpt.blendState = DPD_SHADER_OPT_BLEND_STATE_ADDCOLOR;
            DrawPolygonInCameraEx(drawDatCpyTmp, opt);
        }
    }
    else if (bgDat.type == BG_TYPE_TUTORIAL_1)
    {
        DRAW_POLYGON_DAT drawDatTmp = {};
        drawDatTmp.size = MkF3(
            +SCREEN_WIDTH,
            +SCREEN_HEIGHT,
            +1.0f);


        drawDatTmp.rot = 0.0f;

        drawDatTmp.rot3D = MkF3(0.0f, 0.0f, 0.0f);

        drawDatTmp.loadTexType = LOADTEXTURETYPE_MAIN;

        drawDatTmp.texUV = { 0.0f, 0.0f };
        drawDatTmp.texWH = { 1.0f, 1.0f };
        drawDatTmp.startScale = MkF2(1.0f, 1.0f);
        drawDatTmp.startScale3D = MkF3(1.0f, 1.0f, 1.0f);

        drawDatTmp.shaderOpt.outline = DPD_SHADER_OPT_OUTLINE_NONE;
        drawDatTmp.shaderOpt.bloom = DPD_SHADER_OPT_BLOOM_NONE;

        DRAW_POLYGON_IN_CAMERA_EX_OPTION opt = {};
        opt.reverse = false;
        opt.fixed = true;
        opt.enableShade = false;
        opt.priority = DRAW_POLYGON_PRIORITY_DEEPEST;
        opt.depthBuffIgnore = true;

        drawDatTmp.modelNo = 0;
        //{
        //    drawDatTmp.texNo = g_tex_iseki.textureId;
        //    drawDatTmp.color = MkF4(0.2f, 0.0f, 1.5f, 0.75f);
        //    DrawPolygonInCameraEx(drawDatTmp, opt);
        //}
        {
            opt.shaderType = DPD_SHADER_TYPE_EX;
            opt.shaderTypeEx.vs = g_ShaderIndex_TutorialBg_Vertex;
            opt.shaderTypeEx.ps = g_ShaderIndex_TutorialBg_Pixel;

            drawDatTmp.shaderOpt.blendState = DPD_SHADER_OPT_BLEND_STATE_ADDCOLOR;
            drawDatTmp.texNo = g_tex_tutorial_base.textureId;
            drawDatTmp.color = MkF4(0.0f, 0.5f, 1.0f, 0.35f);
            DrawPolygonInCameraEx(drawDatTmp, opt);
        }
    }
    else if (bgDat.type == BG_TYPE_ISEKI_MUTATION)
    {
        //DRAW_POLYGON_DAT drawDatTmp = {};
        ////drawDatTmp.pos.x = dxCinfo.CameraVector.x * -8000.0f;
        ////drawDatTmp.pos.y = dxCinfo.CameraVector.y * -8000.0f;
        ////drawDatTmp.pos.z = dxCinfo.CameraVector.z * -8000.0f;
        ////drawDatTmp.pos.z = -2000.0f;
        //drawDatTmp.size = MkF3(
        //    +SCREEN_WIDTH,
        //    +SCREEN_HEIGHT,
        //    +1.0f);


        //drawDatTmp.rot = 0.0f;

        //drawDatTmp.rot3D = MkF3(0.0f, 40.0f, 0.0f);

        ////drawDatTmp.color = { 0.6f, 0.6f, 0.75f, 1.0f };

        //drawDatTmp.loadTexType = LOADTEXTURETYPE_MAIN;

        //drawDatTmp.texUV = { 0.0f, 0.0f };
        //drawDatTmp.texWH = { 20.0f, 1.0f };
        //drawDatTmp.startScale = MkF2(1.0f, 1.0f);
        //drawDatTmp.startScale3D = MkF3(1.0f, 1.0f, 1.0f);

        //drawDatTmp.shaderOpt.outline = DPD_SHADER_OPT_OUTLINE_NONE;
        //drawDatTmp.shaderOpt.bloom = DPD_SHADER_OPT_BLOOM_NONE;

        //DRAW_POLYGON_IN_CAMERA_EX_OPTION opt = {};
        //opt.reverse = false;
        //opt.fixed = false;
        //opt.enableShade = false;
        //opt.priority = DRAW_POLYGON_PRIORITY_DEEPEST;
        //opt.depthBuffIgnore = true;

        //drawDatTmp.modelNo = 0;

        ////for (int i = 0; i < 2; i++)
        ////{
        ////    DRAW_POLYGON_DAT drawDatCpyTmp = drawDatTmp;
        ////    drawDatTmp.rot3D = MkF3(0.0f, -45.0f + 90.0f * i, 0.0f);
        ////    drawDatCpyTmp.size = MkF3(
        ////        +SCREEN_WIDTH * 100.0f,
        ////        +SCREEN_HEIGHT * 100.0f,
        ////        +1.0f);
        ////    drawDatCpyTmp.startPos.y += -SCREEN_HEIGHT;
        ////    drawDatCpyTmp.startRot3D.z = 0.25f * framef;
        ////    drawDatCpyTmp.color = MkF4(0.5f, 0.0f, 1.0f, 0.9f);
        ////    drawDatCpyTmp.texUV = {
        ////        0.0f + framef * 0.02f,
        ////        0.0f,
        ////    };
        ////    drawDatCpyTmp.texWH = { 8.0f, 8.0f };
        ////    drawDatCpyTmp.texNo = g_tex_iseki_mut[0].textureId;
        ////    //drawDatCpyTmp.loadTexType = LOADTEXTURETYPE_GRAY;
        ////    drawDatCpyTmp.shaderOpt.blendState = DPD_SHADER_OPT_BLEND_STATE_ADDCOLOR;
        ////    DrawPolygonInCameraEx(drawDatCpyTmp, opt);
        ////}

        //{
        //    DRAW_POLYGON_DAT drawDatCpyTmp = drawDatTmp;
        //    drawDatCpyTmp.pos = {};
        //    drawDatCpyTmp.size = MkF3(
        //        +SCREEN_WIDTH * 2,
        //        +SCREEN_HEIGHT * 2,
        //        +1.0f);
        //    drawDatCpyTmp.color = MkF4(
        //        1.0f,
        //        0.0f,
        //        0.0f,
        //        0.3f + 0.2f * sin(framef / 30)
        //    );
        //    drawDatCpyTmp.startPos = {};
        //    drawDatCpyTmp.texUV = { 0.0f, 0.0f };
        //    drawDatCpyTmp.texWH = { 1.0f, 1.0f };
        //    drawDatCpyTmp.texNo = 0;
        //    drawDatCpyTmp.shaderOpt.blendState = DPD_SHADER_OPT_BLEND_STATE_ADDCOLOR;
        //    DRAW_POLYGON_IN_CAMERA_EX_OPTION optCpyTmp = opt;
        //    optCpyTmp.fixed = true;
        //    DrawPolygonInCameraEx(drawDatCpyTmp, optCpyTmp);
        //}
        //{
        //    DRAW_POLYGON_DAT drawDatCpyTmp = drawDatTmp;
        //    drawDatCpyTmp.pos = {};
        //    drawDatCpyTmp.size = MkF3(
        //        +SCREEN_WIDTH * 2,
        //        +SCREEN_HEIGHT * 2,
        //        +1.0f);
        //    drawDatCpyTmp.color = MkF4(
        //        1.0f,
        //        0.0f,
        //        1.0f,
        //        0.4f + 0.2f * sin(framef / 20)
        //    );
        //    drawDatCpyTmp.startPos = {};
        //    drawDatCpyTmp.texUV = {
        //        0.0f,
        //        0.0f + framef * 0.008f,
        //    };
        //    drawDatCpyTmp.texWH = {
        //        2.0f,
        //        2.0f
        //    };
        //    drawDatCpyTmp.texNo = g_tex_iseki_mut[3].textureId;
        //    drawDatCpyTmp.shaderOpt.blendState = DPD_SHADER_OPT_BLEND_STATE_ADDCOLOR;
        //    DRAW_POLYGON_IN_CAMERA_EX_OPTION optCpyTmp = opt;
        //    optCpyTmp.fixed = true;
        //    DrawPolygonInCameraEx(drawDatCpyTmp, optCpyTmp);
        //}
    }
	else if (bgDat.type == BG_TYPE_ISEKI)
	{
		DRAW_SPRITE_BUFFER dsb = {};
		SpriteSetUp(&dsb);

		SpriteShape(&dsb,
			MkF2(0.0f, 0.0f),
			0.0,
			MkF2(SCREEN_WIDTH, SCREEN_HEIGHT)
		);

		POLYGON_RELEASE_OPTION opt = {};
		opt.color = MkF4(0.3f, 0.3f, 0.3f, 1.0f);

		opt.texNo = g_tex_iseki.textureId;
		opt.texUV = MkF2(0.0f, 0.0f);
		opt.texWH = MkF2(1.0f, 1.0f);
		opt.loadTexType = LOADTEXTURETYPE_MAIN;
		opt.fixed = true;
		opt.priority = DRAW_POLYGON_PRIORITY_DEEPEST;
        opt.depthBuffIgnore = true;

        //opt.enableShade = true;
        opt.shaderType = DPD_SHADER_TYPE_NONE;

		SpriteReleaseEx(&dsb, opt);
	}
	else
	{
		switch (bgDat.type)
		{
		case BG_TYPE_TEST_TITLE:
			texTmp = g_tex_test_title;
			break;
		case BG_TYPE_TEST_GAME:
			texTmp = g_tex_test_game;
			break;
		case BG_TYPE_TEST_RESULT:
			texTmp = g_tex_test_result;
			break;
		case BG_TYPE_TEST_RESULT_CLER:
			texTmp = g_tex_test_result_clear;
			break;
		case BG_TYPE_TEST_RESULT_GAMEOVER:
			texTmp = g_tex_test_result_gameover;
			break;
		default:
			texTmp = g_tex_test_none;
			colorTmp = MkF4(0.0f, 0.0f, 0.0f, 1.0f);
			break;
		}

		DrawSpriteQuadColorRotateTexUv(
			//表示座標 (四角形の真ん中)
			0.0f,
			0.0f,

			//サイズ width hight
			SCREEN_WIDTH,
			SCREEN_HEIGHT,

			//角度
			0.0f,

			//色
			colorTmp,

			texTmp.textureId,  //画像(テクスチャ)の番号
			MkF2(0.0f, 0.0f),
			MkF2(1.0f, 1.0f),
			DRAW_POLYGON_PRIORITY_DEEPEST
		);
	}
}


void SetBg(BG_TYPE bg)
{
	bgDat.frame = 0;
	bgDat.type = bg;
}

BG_DAT GetBg()
{
    return bgDat;
}