// ===================================================
// block.cpp ブロック
// 
// hara sougo  2024/07/22
// ===================================================
#include "../../../game_main.h"
#include "../../Texture/texture.h"
#include "../../Sprite/sprite.h"
#include "../../Managers/gameManager.h"
//#include "controller.h"
#include "block.h"
#include "../../../Game/Game Object/gameObject.h"
#include "../../../Game/Game Object/gameObjectPrefab.h"

#include "../../Systems/camera.h"
#include "../../Systems/collision.h"

static BLOCK blocks[BLOCK_MAX] = {};
//unsigned int BlockTextureId;

static TEX_DAT g_tex_spotlight = {};
static TEX_DAT g_tex_firelight = {};
static TEX_DAT g_tex_iseki_fire = {};

void InitBlock(BLOCK* block)
{
	(*block) = {};
	block->isUse = false;
	block->type = BLOCK_TYPE_VOID;
	block->pos = MkF2(0.0f, 0.0f);
	block->size = MkF2(BLOCK_WIDTH, BLOCK_HIGHT);
	//block->sprite = {};
	InitSpriteAnim(&block->sprite);
}
void InitBlock()
{
	for (int i = 0; i < BLOCK_MAX; i++)
	{
		InitBlock(blocks + i);
	}
	/*BlockTextureId = LoadTexture(
		"rom:/texture/block/test_block.tga"
	);*/

    g_tex_spotlight = LoadTexData("TEX/effect/spotlight_0");
    g_tex_firelight = LoadTexData("TEX/effect/firelight");
    
    g_tex_iseki_fire = LoadTexData("TEX/object/iseki/obj_image/fire");
}

void UninitBlock()
{
    UnloadTexData(g_tex_iseki_fire);
    UnloadTexData(g_tex_firelight);
    UnloadTexData(g_tex_spotlight);
}

void UpdateBlock()
{
	//for (int i = 0; i < BLOCK_MAX; i++)
	//{
	//	if (blocks[i].isUse == true)
	//	{
	//		//UpdateSpriteAnim(&blocks[i].sprite);
	//	}
	//}
}

void DrawBlock()
{
	for (int i = 0; i < BLOCK_MAX; i++)
	{
		if (blocks[i].isUse == true 
			/* && IsCameraInBlock(blocks + i)*/)
		{
			DrawBlockData(blocks + i);
		}
	}
}


static float BlockAlpha(int i, const Float3& pos)
{
    CAMERA camera = GetCameraInfo(i);
    //DX_CAMERA_INFO dxcamera = DX_GetCameraInfo(i);

    Float3 pos1 = camera.trans.pos;
    pos1.x += camera.camera3D.pos.x / -camera.camera3D.scale / camera.trans.zoom;
    pos1.y += camera.camera3D.pos.y / -camera.camera3D.scale / camera.trans.zoom;
    pos1.z += camera.camera3D.pos.z / camera.camera3D.scale / camera.trans.zoom;
    Float3 pos2 = pos;
    //pos1.z = 0;
    //pos2.z = 0;

    //float a = std::max(std::min(CalculateDistance(pos1, pos2) / 1200.0f, 1.0f), 0.0f);
    float a = std::max(std::min((pos1.y - pos2.y) / 1800.0f, 1.0f), 0.0f);
    //if (i == 0)
    //{
    //    DebugPrintf("%f\n", pos1.x);
    //    DebugPrintf("%f\n", pos1.y);
    //    DebugPrintf("%f\n", pos1.z);
    //}
    return a;
}

static void DrawBlockSpriteAnim(BLOCK* block, SPRITE_ANIM_DAT* sadat, DRAW_SPRITE_ANIM_EX_OPTION opt)
{
    if (sadat->hide)
    {
        return;
    }

    for(int i = 0; i < CAMERA_NUM; i++)
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

        if (block->sizeZ > BLOCK_HIGHT * 5)
        {
            Float3 bpos = MkF3(
                drawDatTmp.startPos3D.x + drawDatTmp.pos.x,
                drawDatTmp.startPos3D.y + drawDatTmp.pos.y,
                drawDatTmp.startPos3D.z + drawDatTmp.pos.z
            );
            drawDatTmp.color.w *= BlockAlpha(i, bpos);
            if (drawDatTmp.color.w < 1.0)
            {
                ropt.shaderType = DPD_SHADER_TYPE_ALPHA_DISSOLVE;
            }
        }

        if (!block->isBackground)
        { //影
            DRAW_POLYGON_DAT drawDatTmp2 = drawDatTmp;
            drawDatTmp2.pos.z = -18.0f;
            drawDatTmp2.rot3D = {};
            drawDatTmp2.color = { 0.0f, 0.0f, 0.0f, 0.7f };
            drawDatTmp2.vertex.type = DRAW_POLYGON_TYPE_QUAD_OBJECT_SHADOW_0;
            drawDatTmp2.loadTexType = LOADTEXTURETYPE_GRAY;
            DRAW_POLYGON_IN_CAMERA_EX_OPTION ropt2 = ropt;
            ropt2.shaderType = DPD_SHADER_TYPE_NONE;
            DrawPolygonInCameraSingle(i, drawDatTmp2, ropt2);
        }

        DrawPolygonInCameraSingle(i, drawDatTmp, ropt);
    }
}

void DrawBlockData(BLOCK* block)
{
	//GAME_OBJECT o = GetGameObjectPrefab(GAME_OBJECT_TYPE_TEST);
	//o.pos = MkF3(od->x, od->y, 0.0f);
	//DrawSpriteAnim(&o.graphic.graph.spriteAnim[0], block->pos, 0.0f, MkF2(1.0f, 1.0f), LOADTEXTURETYPE_MAIN);
	{
		SPRITE_ANIM_DAT sa = block->sprite;
		//float adc = (float)((sin((float)GetMainGameFrame() / 40) * 1.0f + 1.0f) / 2 * 1.5f);
		//sa.color.x += adc;
		//sa.color.y += adc;
		//sa.color.z += adc;
		
		//DrawSpriteAnim(&sa, block->pos, 0.0f, MkF2(1.0f, 1.0f), LOADTEXTURETYPE_MAIN, false);

		if (sa.isUse && !sa.hide)
		{
			DRAW_SPRITE_ANIM_EX_OPTION opttmp = {};
			opttmp.startPos = Float2ToFloat3(block->pos, block->posZ);
			opttmp.startRot = MkF3(0.0f, 0.0f, 0.0f);
			opttmp.startScale = MkF2(1.0f, 1.0f);
			opttmp.loadTexType = LOADTEXTURETYPE_MAIN;
			opttmp.fixed = false;

			//opttmp.enableShade = true;
			opttmp.enableShade = false;

            opttmp.shaderOpt.bloom = DPD_SHADER_OPT_BLOOM_NONE;
            //opttmp.shaderOpt.bloom = DPD_SHADER_OPT_BLOOM_NORMAL;


            if (block->isBackground)
            {
                opttmp.priority =
                    (DRAW_POLYGON_PRIORITY)(DRAW_POLYGON_PRIORITY_DEEPEST + 1);
                //opttmp.depthBuffIgnore = true;
            }
            else if (block->sizeZ <= 10.0f)
            {
                opttmp.priority = 
                    (DRAW_POLYGON_PRIORITY)(DRAW_POLYGON_PRIORITY_NML_DEEPEST + 1);
                //opttmp.depthBuffIgnore = true;
            }
            else
            {
                //opttmp.priority = DRAW_POLYGON_PRIORITY_NORMAL;
                opttmp.priority = DRAW_POLYGON_PRIORITY_NML_u1;
            }

            if (
                (block->type == BLOCK_TYPE_ISEKI_0_Taimatsu
                || block->type == BLOCK_TYPE_ISEKI_0_Dark_Taimatsu)
                )
            {
                //opttmp.priority = (DRAW_POLYGON_PRIORITY)(opttmp.priority - 1);
                { //炎
                    DRAW_POLYGON_DAT drawDatTmp = {};
                    drawDatTmp.pos = Float2ToFloat3(block->pos);

                    drawDatTmp.size = MkF3(
                        opttmp.startScale.x * BLOCK_WIDTH * 5,
                        opttmp.startScale.y * BLOCK_HIGHT * 5,
                        1.0f);


                    drawDatTmp.rot = 0.0f;

                    drawDatTmp.rot3D = MkF3(70.0f, 0.0f, 0.0f);

                    drawDatTmp.color = MkF4(1.0f, 1.0f, 1.0f, 1.0f);

                    drawDatTmp.loadTexType = LOADTEXTURETYPE_MAIN;

                    drawDatTmp.startScale = MkF2(1.0f, 1.0f);
                    drawDatTmp.startScale3D = MkF3(1.0f, 1.0f, 1.0f);

                    drawDatTmp.modelNo = 0;

                    drawDatTmp.texWH = { 1.0f, 1.0f };

                    DRAW_POLYGON_IN_CAMERA_EX_OPTION opt = {};
                    opt.reverse = false;
                    opt.fixed = false;
                    opt.enableShade = false;
                    //opt.priority = DRAW_POLYGON_PRIORITY_NML_DEEPEST;
                    opt.priority =
                        (DRAW_POLYGON_PRIORITY)(DRAW_POLYGON_PRIORITY_NML_DEEPEST + 2);
                    opt.depthBuffIgnore = false;

                    float fsize = sin(GetMainGameFrame() / 5);
                    {
                        DRAW_POLYGON_DAT dd = drawDatTmp;
                        dd.pos.z += -10.0f;
                        dd.size.x *= 1.8f;
                        dd.size.y *= 1.8f;
                        dd.size.x += fsize * 30.0f;
                        dd.size.y += fsize * 30.0f;
                        dd.rot3D = {};
                        dd.texNo = g_tex_spotlight.textureId;
                        dd.color = MkF4(2.0f, 2.0f, 0.8f, 0.2f);
                        dd.shaderOpt.blendState = DPD_SHADER_OPT_BLEND_STATE_ADDCOLOR;
                        DRAW_POLYGON_IN_CAMERA_EX_OPTION opt2 = opt;
                        opt2.priority =
                            (DRAW_POLYGON_PRIORITY)(DRAW_POLYGON_PRIORITY_NML_DEEPEST + 2);
                        opt2.depthBuffIgnore = true;
                        DrawPolygonInCameraEx(dd, opt2);
                    }

                    float ady = sin(CalculateDegToRad(sa.rot3D.x - 45.0f)) * BLOCK_HIGHT * -6.0f - sin(CalculateDegToRad(sa.rot3D.x)) * 24.0f;
                    float adz = cos(CalculateDegToRad(sa.rot3D.x - 45.0f)) * BLOCK_HIGHT * -6.0f - cos(CalculateDegToRad(sa.rot3D.x)) * 24.0f;
                    drawDatTmp.pos.y += ady;
                    drawDatTmp.pos.z += adz;

                    if(!GameManagerIsMovieFlag())
                    {
                        DRAW_POLYGON_DAT dd = drawDatTmp;
                        dd.pos.y += sin(CalculateDegToRad(sa.rot3D.x)) * 80.0f;
                        dd.pos.z += cos(CalculateDegToRad(sa.rot3D.x)) * -80.0f;
                        dd.pos.z += 15.0f;
                        dd.size.x *= 1.8f;
                        dd.size.y *= 1.8f;
                        dd.size.x += fsize * 10.0f;
                        dd.size.y += fsize * 10.0f;
                        dd.rot3D = {};
                        dd.texNo = g_tex_firelight.textureId;
                        //dd.color = MkF4(2.0f, 0.4f, 0.0f, 0.3f);
                        dd.color = MkF4(4.0f, 1.2f, 0.1f, 0.35f);
                        dd.shaderOpt.blendState = DPD_SHADER_OPT_BLEND_STATE_ADDCOLOR;
                        DRAW_POLYGON_IN_CAMERA_EX_OPTION opt2 = opt;
                        opt2.billboard = true;
                        //opt2.priority =
                        //    (DRAW_POLYGON_PRIORITY)(DRAW_POLYGON_PRIORITY_NML_DEEPEST + 2);
                        opt2.priority = DRAW_POLYGON_PRIORITY_NML_u2;
                        DrawPolygonInCameraEx(dd, opt2);
                    }

                    drawDatTmp.texNo = g_tex_iseki_fire.textureId;
                    drawDatTmp.texUV = AnimationUV(GetMainGameFrame() / 5, 4, 3);
                    drawDatTmp.texWH = { 1.0f / 4, 1.0f / 3 };

                    drawDatTmp.shaderOpt.bloom = DPD_SHADER_OPT_BLOOM_NORMAL;
                    DrawPolygonInCameraEx(drawDatTmp, opt);
                }
                
            }

            DrawBlockSpriteAnim(block, &sa, opttmp);

		}
	}

	
	//DRAW_SPRITE_DAT dsdat = {};
	//dsdat.x = block->pos.x;
	//dsdat.y = block->pos.y;
	//dsdat.w = BLOCK_WIDTH;
	//dsdat.h = BLOCK_HIGHT;
	//dsdat.rot = 0.0f;
	//dsdat.color = MkF4(1.0f, 1.0f, 1.0f, 1.0f);
	//dsdat.texNo = BlockTextureId;
	////dsdat.texUV = AnimationUV(blocks[i].type, 8, 4);
	//dsdat.texUV = AnimationUV(block->type, 6, 6);
	////dsdat.texWH = { 1.0f / 8, 1.0f / 4 };
	//dsdat.texWH = MkF2(1.0f / 6, 1.0f / 6);
	//dsdat.startPos = MkF2(0.0f, 0.0f);
	//dsdat.startRot = 0.0f;
	//dsdat.startScale = MkF2(1.0f, 1.0f);

	//DrawSpriteInCamera(dsdat);

	//DrawSpriteQuadColorRotateTexUv(
	//	//表示座標 (四角形の真ん中)
	//	blocks[i].pos.x,
	//	blocks[i].pos.y,

	//	//サイズ width hight
	//	BLOCK_WIDTH,
	//	BLOCK_HIGHT,

	//	//角度
	//	0.0f,

	//	//色
	//	nn::util::MakeFloat4(1.0f, 1.0f, 1.0f, 1.0f),

	//	BlockTextureId,  //画像(テクスチャ)の番号
	//	AnimationUV(blocks[i].type, 8, 4),
	//	nn::util::MakeFloat2(
	//		1.0f / 8,
	//		1.0f / 4
	//	)
	//);
}


BLOCK* GetBlocks()
{
	return blocks;
}

BLOCK* SetBlock(BLOCK dat)
{
	for (int i = 0; i < BLOCK_MAX; i++)
	{
		if (blocks[i].isUse == false)
		{
			blocks[i] = dat;
			blocks[i].isUse = true;
            return blocks + i;
		}
	}
    return nullptr;
}

bool DeleteBlock(BLOCK* block)
{
	if (block->isUse)
	{
		block->isUse = false;
		return true;
	}

	return false;
}
bool DeleteBlock(int index)
{
	if (index >= 0 && index < BLOCK_MAX)
	{
		return DeleteBlock(blocks + index);
	}

	return false;
}
bool DeleteBlockAll()
{
	bool r = false;
	for (int i = 0; i < BLOCK_MAX; i++)
	{
		bool rc = DeleteBlock(i);
		if (rc)
		{
			r = true;
		}
	}

	return r;
}

bool IsCameraInBlock(BLOCK* block)
{
    if (block == nullptr)
    {
        return false;
    }
    for (int i = 0; i < CAMERA_NUM; i++)
    {
        if (IsCameraInBlock(i, block))
        {
            return true;
        }
    }
    return false;
}

bool IsCameraInBlock(int cameraIndex, BLOCK* block)
{
	if (block == nullptr)
	{
		return false;
	}
	return CheckBoxCollider(CameraPosToWorldPos(cameraIndex, MkF2(0.0f, 0.0f)), MkF2(block->pos.x, block->pos.y), GetCameraSize(cameraIndex), MkF2(1.0f, 1.0f));
}
