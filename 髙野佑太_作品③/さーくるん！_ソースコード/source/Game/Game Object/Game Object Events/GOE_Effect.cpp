// ===================================================
// GOE_Effect.cpp オブジェクトイベントエフェクト
// 
// hara sougo  2025/01/06
// ===================================================
#include "../../../game_main.h"
#include "../../Sprite/sprite.h"
#include "../../Polygon/modelData.h"
#include "../../Texture/texture.h"
#include "../../../Game/Entities/Block/block.h"
#include "../../Systems/collision.h"
#include "../../Systems/easing.h"

#include "../gameObject.h"
#include "../gameObjectGraph.h"
#include "../gameObjectPrefab.h"

#include "../../Systems/ui.h"

#include "GOE_Effect.h"

// ===================================================
// マクロ定義
// ===================================================

// ===================================================
// プロトタイプ宣言
// ===================================================
GAME_OBJECT* gO_CreateEffect(GO_EFFECT_TYPE et, GAME_OBJECT* owner);

// ===================================================
// グローバル変数
// ===================================================

// ===================================================
// オブジェクトの初期化
// ===================================================
void GOE_Effect::Init()
{
	DRAW_POLYGON_DAT drawDatTmp = {};
	drawDatTmp.size = MkF3(200.0f, 200.0f, 1.0f);

	drawDatTmp.rot = 0.0f;

	drawDatTmp.rot3D = MkF3(0.0f, 0.0f, 0.0f);
    //drawDatTmp.rot3D.x = 90.0f;
	drawDatTmp.color = MkF4(1.0f, 1.0f, 1.0f, 1.0f);

	drawDatTmp.texNo = 0;
	drawDatTmp.loadTexType = LOADTEXTURETYPE_MAIN;

	drawDatTmp.texUV = {
		0.0f,
		0.0f
	};
	drawDatTmp.texWH = {
		1.0f,
		1.0f
	};

	//drawDatTmp.startPos3D = m_Obj->pos;

	drawDatTmp.startRot3D = MkF3(0.0f, 0.0f, 0.0f);

	drawDatTmp.startScale = MkF2(1.0f, 1.0f);
	drawDatTmp.startScale3D = MkF3(1.0f, 1.0f, 1.0f);

	drawDatTmp.modelNo = 0;
	drawDatTmp.vertex.type = DRAW_POLYGON_TYPE_QUAD;

	drawDatTmp.shaderOpt.outline = DPD_SHADER_OPT_OUTLINE_NONE;
	drawDatTmp.shaderOpt.bloom = DPD_SHADER_OPT_BLOOM_NONE;

	DRAW_POLYGON_IN_CAMERA_EX_OPTION opt = {};
	opt.reverse = false;
	opt.fixed = false;
	opt.enableShade = false;
	opt.billboard = true;
	opt.priority = DRAW_POLYGON_PRIORITY_NORMAL;


	GO_EFFECT_TYPE et = (GO_EFFECT_TYPE)m_Obj->freeInt;

	if (et == GO_EFFECT_TYPE_SHOCKWAVE)
	{
		m_Dpd_Num = 1;
		m_Dpd = new DRAW_POLYGON_DAT[m_Dpd_Num];
		m_DpdOpt = new DRAW_POLYGON_IN_CAMERA_EX_OPTION[m_Dpd_Num];

		m_Dpd[0] = drawDatTmp;
		m_Dpd[0].texNo = LoadTexData("TEX/effect/shockwave2d2").textureId;
		m_Dpd[0].color = { 0.0f, 1.0f, 1.0f, 3.75f };
		//m_Dpd[0].rot3D.x = 90.0f;
		m_Dpd[0].shaderOpt.blendState = DPD_SHADER_OPT_BLEND_STATE_ADDCOLOR;
		m_DpdOpt[0] = opt;
		m_DpdOpt[0].billboard = false;
	}
	if (et == GO_EFFECT_TYPE_SHOCKWAVE_ENEM)
	{
		m_Dpd_Num = 1;
		m_Dpd = new DRAW_POLYGON_DAT[m_Dpd_Num];
		m_DpdOpt = new DRAW_POLYGON_IN_CAMERA_EX_OPTION[m_Dpd_Num];

		m_Dpd[0] = drawDatTmp;
		m_Dpd[0].texNo = LoadTexData("TEX/effect/shockwave2d2").textureId;
		m_Dpd[0].color = { 1.0f, 0.0f, 1.0f, 3.75f };
		//m_Dpd[0].rot3D.x = 90.0f;
		m_Dpd[0].shaderOpt.blendState = DPD_SHADER_OPT_BLEND_STATE_ADDCOLOR;
		m_DpdOpt[0] = opt;
		m_DpdOpt[0].billboard = false;
	}

	if (et == GO_EFFECT_TYPE_FLASH_0)
	{
		m_Dpd_Num = 3;
		m_Dpd = new DRAW_POLYGON_DAT[m_Dpd_Num];
		m_DpdOpt = new DRAW_POLYGON_IN_CAMERA_EX_OPTION[m_Dpd_Num];

		for (int i = 0; i < m_Dpd_Num; i++)
		{
			m_Dpd[i] = drawDatTmp;
            m_Dpd[i].rot3D.x = 90.0f;
			m_Dpd[i].texNo = LoadTexData("TEX/effect/flashGeneralGrayHishi").textureId;
			m_Dpd[i].color = { 0.0f, 1.0f, 1.0f, 0.75f };
			m_Dpd[i].shaderOpt.blendState = DPD_SHADER_OPT_BLEND_STATE_ADDCOLOR;
			m_Dpd[i].size.x /= 22;

			m_DpdOpt[i] = opt;
		}
	}

	if (et == GO_EFFECT_TYPE_FLASH_0_ENEM)
	{
		m_Dpd_Num = 3;
		m_Dpd = new DRAW_POLYGON_DAT[m_Dpd_Num];
		m_DpdOpt = new DRAW_POLYGON_IN_CAMERA_EX_OPTION[m_Dpd_Num];

		for (int i = 0; i < m_Dpd_Num; i++)
		{
			m_Dpd[i] = drawDatTmp;
            m_Dpd[i].rot3D.x = 90.0f;
			m_Dpd[i].texNo = LoadTexData("TEX/effect/flashGeneralGrayHishi").textureId;
			m_Dpd[i].color = { 1.0f, 0.0f, 1.0f, 0.75f };
			m_Dpd[i].shaderOpt.blendState = DPD_SHADER_OPT_BLEND_STATE_ADDCOLOR;
			m_Dpd[i].size.x /= 22;

			m_DpdOpt[i] = opt;
		}
	}

	if (et == GO_EFFECT_TYPE_FLASH_1)
	{
		m_Dpd_Num = 3;
		m_Dpd = new DRAW_POLYGON_DAT[m_Dpd_Num];
		m_DpdOpt = new DRAW_POLYGON_IN_CAMERA_EX_OPTION[m_Dpd_Num];

		for (int i = 0; i < m_Dpd_Num; i++)
		{
			m_Dpd[i] = drawDatTmp;
            m_Dpd[i].rot3D.x = 90.0f;
			m_Dpd[i].texNo = LoadTexData("TEX/effect/flashGeneralGrayHishi").textureId;
			m_Dpd[i].color = { 0.0f, 1.0f, 1.0f, 0.75f };
			m_Dpd[i].shaderOpt.blendState = DPD_SHADER_OPT_BLEND_STATE_ADDCOLOR;
			m_Dpd[i].size.x /= 22;

			m_DpdOpt[i] = opt;
		}
	}

	if (et == GO_EFFECT_TYPE_FLASH_1_ENEM)
	{
		m_Dpd_Num = 3;
		m_Dpd = new DRAW_POLYGON_DAT[m_Dpd_Num];
		m_DpdOpt = new DRAW_POLYGON_IN_CAMERA_EX_OPTION[m_Dpd_Num];

		for (int i = 0; i < m_Dpd_Num; i++)
		{
			m_Dpd[i] = drawDatTmp;
            m_Dpd[i].rot3D.x = 90.0f;
			m_Dpd[i].texNo = LoadTexData("TEX/effect/flashGeneralGrayHishi").textureId;
			m_Dpd[i].color = { 1.0f, 0.0f, 1.0f, 0.75f };
			m_Dpd[i].shaderOpt.blendState = DPD_SHADER_OPT_BLEND_STATE_ADDCOLOR;
			m_Dpd[i].size.x /= 22;

			m_DpdOpt[i] = opt;
			//m_DpdOpt[i].billboard = false;
		}
	}

	if (et == GO_EFFECT_TYPE_TWINKLE)
	{

        m_Dpd_Num = 1;
        m_Dpd = new DRAW_POLYGON_DAT[m_Dpd_Num];
        m_DpdOpt = new DRAW_POLYGON_IN_CAMERA_EX_OPTION[m_Dpd_Num];

        m_Dpd[0] = drawDatTmp;
        m_Dpd[0].texNo = LoadTexData("TEX/effect/twinkle").textureId;
        m_Dpd[0].color = { 1.0f, 1.0f, 1.0f, 1.75f };
        m_Dpd[0].shaderOpt.blendState = DPD_SHADER_OPT_BLEND_STATE_ADDCOLOR;

        m_DpdOpt[0] = opt;
	}
	if (et == GO_EFFECT_TYPE_SPARK)
	{
		m_Dpd_Num = 1;
		m_Dpd = new DRAW_POLYGON_DAT[m_Dpd_Num];
		m_DpdOpt = new DRAW_POLYGON_IN_CAMERA_EX_OPTION[m_Dpd_Num];

		m_Dpd[0] = drawDatTmp;
        //m_Dpd[0].texNo = LoadTexData("TEX/effect/spark").textureId;
        m_Dpd[0].texNo = LoadTexData("TEX/effect/spark/anim_th_01").textureId;
        //m_Dpd[0].shaderOpt.blendState = DPD_SHADER_OPT_BLEND_STATE_ADDCOLOR;
        m_Dpd[0].color = { 1.0f, 1.0f, 0.0f, 1.0f };
        

		m_DpdOpt[0] = opt;
        m_DpdOpt[0].shaderType = DPD_SHADER_TYPE_EFFECT;
	}
    if (et == GO_EFFECT_TYPE_EXCLAMATION)
    {
        m_Dpd_Num = 1;
        m_Dpd = new DRAW_POLYGON_DAT[m_Dpd_Num];
        m_DpdOpt = new DRAW_POLYGON_IN_CAMERA_EX_OPTION[m_Dpd_Num];

        m_Dpd[0] = drawDatTmp;
        m_Dpd[0].texNo = LoadTexData("TEX/effect/exclamation2").textureId;

        m_DpdOpt[0] = opt;

        m_DpdOpt[0].priority = DRAW_POLYGON_PRIORITY_NML_ALWAYS;
        m_DpdOpt[0].depthBuffIgnore = true;
    }
    if (et == GO_EFFECT_TYPE_QUESTION)
    {
        m_Dpd_Num = 1;
        m_Dpd = new DRAW_POLYGON_DAT[m_Dpd_Num];
        m_DpdOpt = new DRAW_POLYGON_IN_CAMERA_EX_OPTION[m_Dpd_Num];

        m_Dpd[0] = drawDatTmp;
        m_Dpd[0].texNo = LoadTexData("TEX/effect/question").textureId;

        m_DpdOpt[0] = opt;

        m_DpdOpt[0].priority = DRAW_POLYGON_PRIORITY_NML_ALWAYS;
        m_DpdOpt[0].depthBuffIgnore = true;
    }

	if (et == GO_EFFECT_TYPE_SHOCKWAVE_3D)
	{
		m_Dpd_Num = 1;
		m_Dpd = new DRAW_POLYGON_DAT[m_Dpd_Num];
		m_DpdOpt = new DRAW_POLYGON_IN_CAMERA_EX_OPTION[m_Dpd_Num];

		m_Dpd[0] = drawDatTmp;
		m_Dpd[0].texNo = LoadTexData("TEX/effect/shockwave2").textureId;
		m_Dpd[0].modelNo = GetModelDataIndex("MODEL/effect/shockwave");
		m_Dpd[0].color = { 1.0f, 1.0f, 1.0f, 3.0f };
		m_Dpd[0].size = MkF3(200.0f, -200.0f, 200.0f);
		//m_Dpd[0].texUV = MkF2(0.25f, 0.0f);
		m_Dpd[0].texWH = MkF2(0.5f, 1.0f);
		m_Dpd[0].shaderOpt.blendState = DPD_SHADER_OPT_BLEND_STATE_ADDCOLOR;

		m_DpdOpt[0] = opt;
		m_DpdOpt[0].billboard = false;
	}
	if (et == GO_EFFECT_TYPE_HITMARK)
	{
		m_Dpd_Num = 1;
		m_Dpd = new DRAW_POLYGON_DAT[m_Dpd_Num];
		m_DpdOpt = new DRAW_POLYGON_IN_CAMERA_EX_OPTION[m_Dpd_Num];

		m_Dpd[0] = drawDatTmp;
		m_Dpd[0].texNo = LoadTexData("TEX/effect/shockwave2d2").textureId;
		m_Dpd[0].color = { 1.0f, 0.0f, 1.0f, 3.75f };
		//m_Dpd[0].rot3D.x = 90.0f;
		m_Dpd[0].shaderOpt.blendState = DPD_SHADER_OPT_BLEND_STATE_ADDCOLOR;
		m_DpdOpt[0] = opt;
		//m_DpdOpt[0].billboard = false;
		m_DpdOpt[0].priority = DRAW_POLYGON_PRIORITY_NML_u1;

	}
    if (et == GO_EFFECT_TYPE_N_ATTACK_HIT)
    {
        m_Dpd_Num = 1;
        m_Dpd = new DRAW_POLYGON_DAT[m_Dpd_Num];
        m_DpdOpt = new DRAW_POLYGON_IN_CAMERA_EX_OPTION[m_Dpd_Num];

        m_Dpd[0] = drawDatTmp;
        m_Dpd[0].texNo = LoadTexData("TEX/effect/tex_hit_01").textureId;
        m_Dpd[0].shaderOpt.blendState = DPD_SHADER_OPT_BLEND_STATE_ADDCOLOR;

        m_DpdOpt[0] = opt;
        m_DpdOpt[0].shaderType = DPD_SHADER_TYPE_EFFECT;
    }
    if (et == GO_EFFECT_TYPE_HITSTAR_VANISH)
    {
        m_Dpd_Num = 1;
        m_Dpd = new DRAW_POLYGON_DAT[m_Dpd_Num];
        m_DpdOpt = new DRAW_POLYGON_IN_CAMERA_EX_OPTION[m_Dpd_Num];

        m_Dpd[0] = drawDatTmp;
        m_Dpd[0].texNo = LoadTexData("TEX/effect/Hit_star_01").textureId;
        m_Dpd[0].shaderOpt.blendState = DPD_SHADER_OPT_BLEND_STATE_ADDCOLOR;

        m_DpdOpt[0] = opt;
        m_DpdOpt[0].priority = DRAW_POLYGON_PRIORITY_NML_u1;
        m_DpdOpt[0].depthBuffIgnore = true;
        m_DpdOpt[0].shaderType = DPD_SHADER_TYPE_EFFECT;
    }
    if (et == GO_EFFECT_TYPE_EXPLOSION_1)
    {
        m_Dpd_Num = 1;
        m_Dpd = new DRAW_POLYGON_DAT[m_Dpd_Num];
        m_DpdOpt = new DRAW_POLYGON_IN_CAMERA_EX_OPTION[m_Dpd_Num];

        m_Dpd[0] = drawDatTmp;
        m_Dpd[0].texNo = LoadTexData("TEX/effect/explosion/EX_atlas_01").textureId;
        //m_Dpd[0].shaderOpt.blendState = DPD_SHADER_OPT_BLEND_STATE_ADDCOLOR;

        m_DpdOpt[0] = opt;
        m_DpdOpt[0].shaderType = DPD_SHADER_TYPE_EFFECT;
    }
    if (et == GO_EFFECT_TYPE_EXPLOSION_2)
    {
        m_Dpd_Num = 1;
        m_Dpd = new DRAW_POLYGON_DAT[m_Dpd_Num];
        m_DpdOpt = new DRAW_POLYGON_IN_CAMERA_EX_OPTION[m_Dpd_Num];

        m_Dpd[0] = drawDatTmp;
        m_Dpd[0].texNo = LoadTexData("TEX/effect/explosion/EX_atlas_02").textureId;
        //m_Dpd[0].shaderOpt.blendState = DPD_SHADER_OPT_BLEND_STATE_ADDCOLOR;

        m_DpdOpt[0] = opt;
        m_DpdOpt[0].shaderType = DPD_SHADER_TYPE_EFFECT;
    }
    if (et == GO_EFFECT_TYPE_EXPLOSION_3)
    {
        m_Dpd_Num = 1;
        m_Dpd = new DRAW_POLYGON_DAT[m_Dpd_Num];
        m_DpdOpt = new DRAW_POLYGON_IN_CAMERA_EX_OPTION[m_Dpd_Num];

        m_Dpd[0] = drawDatTmp;
        m_Dpd[0].texNo = LoadTexData("TEX/effect/explosion/EX_atlas_03").textureId;
        m_Dpd[0].shaderOpt.blendState = DPD_SHADER_OPT_BLEND_STATE_ADDCOLOR;

        m_DpdOpt[0] = opt;
        m_DpdOpt[0].shaderType = DPD_SHADER_TYPE_EFFECT;
    }
    if (et == GO_EFFECT_TYPE_EXPLOSION_4)
    {
        m_Dpd_Num = 1;
        m_Dpd = new DRAW_POLYGON_DAT[m_Dpd_Num];
        m_DpdOpt = new DRAW_POLYGON_IN_CAMERA_EX_OPTION[m_Dpd_Num];

        m_Dpd[0] = drawDatTmp;
        m_Dpd[0].texNo = LoadTexData("TEX/effect/explosion/EX_atlas_04").textureId;
        m_Dpd[0].shaderOpt.blendState = DPD_SHADER_OPT_BLEND_STATE_ADDCOLOR;

        m_DpdOpt[0] = opt;
        //m_DpdOpt[0].priority = DRAW_POLYGON_PRIORITY_FORCE_NML_u1;
        m_DpdOpt[0].priority = DRAW_POLYGON_PRIORITY_NML_u1;
        m_DpdOpt[0].depthBuffIgnore = true;
        m_DpdOpt[0].shaderType = DPD_SHADER_TYPE_EFFECT;
    }
    if (et == GO_EFFECT_TYPE_EXPLOSION_SMOKE)
    {
        m_Dpd_Num = 1;
        m_Dpd = new DRAW_POLYGON_DAT[m_Dpd_Num];
        m_DpdOpt = new DRAW_POLYGON_IN_CAMERA_EX_OPTION[m_Dpd_Num];

        m_Dpd[0] = drawDatTmp;
        m_Dpd[0].texNo = LoadTexData("TEX/effect/explosion/Smoke_01").textureId;
        //m_Dpd[0].shaderOpt.blendState = DPD_SHADER_OPT_BLEND_STATE_ADDCOLOR;

        m_DpdOpt[0] = opt;
        m_DpdOpt[0].shaderType = DPD_SHADER_TYPE_EFFECT;
    }
    if (et == GO_EFFECT_TYPE_SMOKE)
    {
        m_Dpd_Num = 1;
        m_Dpd = new DRAW_POLYGON_DAT[m_Dpd_Num];
        m_DpdOpt = new DRAW_POLYGON_IN_CAMERA_EX_OPTION[m_Dpd_Num];

        m_Dpd[0] = drawDatTmp;
        m_Dpd[0].texNo = LoadTexData("TEX/effect/smoke_anim_01").textureId;

        m_DpdOpt[0] = opt;
        m_DpdOpt[0].shaderType = DPD_SHADER_TYPE_EFFECT;
    }
    if (et == GO_EFFECT_TYPE_EXPLOSION_1_GROUND)
    {
        m_Dpd_Num = 1;
        m_Dpd = new DRAW_POLYGON_DAT[m_Dpd_Num];
        m_DpdOpt = new DRAW_POLYGON_IN_CAMERA_EX_OPTION[m_Dpd_Num];

        m_Dpd[0] = drawDatTmp;
        m_Dpd[0].texNo = LoadTexData("TEX/effect/explosion/EX_atlas_01").textureId;
        //m_Dpd[0].shaderOpt.blendState = DPD_SHADER_OPT_BLEND_STATE_ADDCOLOR;
        m_Dpd[0].color.w *= 0.9f;
        m_Dpd[0].pos.y += 0.16f * m_Obj->scale.y;

        m_DpdOpt[0] = opt;
        m_DpdOpt[0].billboard = false;
        m_DpdOpt[0].depthBuffIgnore = true;
        m_DpdOpt[0].priority = DRAW_POLYGON_PRIORITY_NML_d1;
        m_DpdOpt[0].shaderType = DPD_SHADER_TYPE_EFFECT;
    }
    if (et == GO_EFFECT_TYPE_EXPLOSION_1_GROUND2)
    {
        m_Dpd_Num = 1;
        m_Dpd = new DRAW_POLYGON_DAT[m_Dpd_Num];
        m_DpdOpt = new DRAW_POLYGON_IN_CAMERA_EX_OPTION[m_Dpd_Num];

        m_Dpd[0] = drawDatTmp;
        m_Dpd[0].texNo = LoadTexData("TEX/effect/explosion/EX_atlas_01").textureId;
        //m_Dpd[0].shaderOpt.blendState = DPD_SHADER_OPT_BLEND_STATE_ADDCOLOR;
        m_Dpd[0].shaderOpt.blendState = DPD_SHADER_OPT_BLEND_STATE_DEFAULT;
        m_Dpd[0].color.w *= 0.9f;
        m_Dpd[0].pos.y += 0.16f * m_Obj->scale.y;

        m_DpdOpt[0] = opt;
        m_DpdOpt[0].billboard = false;
        m_DpdOpt[0].depthBuffIgnore = true;
        m_DpdOpt[0].priority = DRAW_POLYGON_PRIORITY_NML_d1;
        m_DpdOpt[0].shaderType = DPD_SHADER_TYPE_EFFECT;
    }
    if (et == GO_EFFECT_TYPE_HAMMER_SHOCKWAVE)
    {
        m_Dpd_Num = 1;
        m_Dpd = new DRAW_POLYGON_DAT[m_Dpd_Num];
        m_DpdOpt = new DRAW_POLYGON_IN_CAMERA_EX_OPTION[m_Dpd_Num];

        m_Dpd[0] = drawDatTmp;
        m_Dpd[0].texNo = LoadTexData("TEX/effect/shock_wave").textureId;
        m_Dpd[0].shaderOpt.blendState = DPD_SHADER_OPT_BLEND_STATE_ADDCOLOR;
        m_Dpd[0].pos.y = m_Dpd[0].size.y * -0.3;
        m_Dpd[0].startRot3D.x = 70.0f;

        m_DpdOpt[0] = opt;
        m_DpdOpt[0].billboard = false;
        m_DpdOpt[0].depthBuffIgnore = true;
        //m_DpdOpt[0].priority = DRAW_POLYGON_PRIORITY_FORCE_NML_d1;;
        m_DpdOpt[0].priority = DRAW_POLYGON_PRIORITY_NML_d1;
        m_DpdOpt[0].shaderType = DPD_SHADER_TYPE_EFFECT;
    }
    if (et == GO_EFFECT_TYPE_HAMMER_SHOCKDECAL)
    {
        m_Dpd_Num = 1;
        m_Dpd = new DRAW_POLYGON_DAT[m_Dpd_Num];
        m_DpdOpt = new DRAW_POLYGON_IN_CAMERA_EX_OPTION[m_Dpd_Num];

        m_Dpd[0] = drawDatTmp;
        m_Dpd[0].texNo = LoadTexData("TEX/effect/shock_decal").textureId;
        m_Dpd[0].shaderOpt.blendState = DPD_SHADER_OPT_BLEND_STATE_ADDCOLOR;
        //m_Dpd[0].startRot3D.x = 90.0f;
        m_Dpd[0].startPos3D.y = -10.0f;

        m_DpdOpt[0] = opt;
        m_DpdOpt[0].billboard = false;
        m_DpdOpt[0].depthBuffIgnore = true;
        m_DpdOpt[0].priority = DRAW_POLYGON_PRIORITY_NML_d1;
        m_DpdOpt[0].shaderType = DPD_SHADER_TYPE_EFFECT;
    }
    if (et == GO_EFFECT_TYPE_SCOREEFF)
    {
        m_Dpd_Num = 1;
        m_Dpd = new DRAW_POLYGON_DAT[m_Dpd_Num];
        m_DpdOpt = new DRAW_POLYGON_IN_CAMERA_EX_OPTION[m_Dpd_Num];

        m_Dpd[0] = drawDatTmp;
        m_Dpd[0].texNo = LoadTexData("TEX/effect/eff_score").textureId;
        m_Dpd[0].shaderOpt.blendState = DPD_SHADER_OPT_BLEND_STATE_ADDCOLOR;

        m_DpdOpt[0] = opt;
        m_DpdOpt[0].priority = DRAW_POLYGON_PRIORITY_NML_u1;
        m_DpdOpt[0].depthBuffIgnore = true;
    }
    if (et == GO_EFFECT_TYPE_SCOREEFF_REV)
    {
        m_Dpd_Num = 1;
        m_Dpd = new DRAW_POLYGON_DAT[m_Dpd_Num];
        m_DpdOpt = new DRAW_POLYGON_IN_CAMERA_EX_OPTION[m_Dpd_Num];

        m_Dpd[0] = drawDatTmp;
        m_Dpd[0].texNo = LoadTexData("TEX/effect/eff_score").textureId;
        m_Dpd[0].shaderOpt.blendState = DPD_SHADER_OPT_BLEND_STATE_ADDCOLOR;

        m_DpdOpt[0] = opt;
        m_DpdOpt[0].priority = DRAW_POLYGON_PRIORITY_NML_u1;
        m_DpdOpt[0].depthBuffIgnore = true;
    }
    if (et == GO_EFFECT_TYPE_HITSTAR_VANISH_REV)
    {
        m_Dpd_Num = 1;
        m_Dpd = new DRAW_POLYGON_DAT[m_Dpd_Num];
        m_DpdOpt = new DRAW_POLYGON_IN_CAMERA_EX_OPTION[m_Dpd_Num];

        m_Dpd[0] = drawDatTmp;
        m_Dpd[0].texNo = LoadTexData("TEX/effect/Hit_star_01").textureId;
        m_Dpd[0].shaderOpt.blendState = DPD_SHADER_OPT_BLEND_STATE_ADDCOLOR;

        m_DpdOpt[0] = opt;
        m_DpdOpt[0].priority = DRAW_POLYGON_PRIORITY_NML_u1;
        m_DpdOpt[0].depthBuffIgnore = true;
    }

    if (et == GO_EFFECT_TYPE_HAMMER_CHARGE_LOOP)
    {
        m_Dpd_Num = 1;
        m_Dpd = new DRAW_POLYGON_DAT[m_Dpd_Num];
        m_DpdOpt = new DRAW_POLYGON_IN_CAMERA_EX_OPTION[m_Dpd_Num];

        m_Dpd[0] = drawDatTmp;
        m_Dpd[0].texNo = LoadTexData("TEX/effect/eff_kn_loop").textureId;
        m_Dpd[0].shaderOpt.blendState = DPD_SHADER_OPT_BLEND_STATE_ADDCOLOR;

        m_DpdOpt[0] = opt;
        m_DpdOpt[0].priority = DRAW_POLYGON_PRIORITY_NML_u1;
        m_DpdOpt[0].depthBuffIgnore = true;
        m_DpdOpt[0].shaderType = DPD_SHADER_TYPE_EFFECT;
    }
    if (et == GO_EFFECT_TYPE_HAMMER_CHARGE_LOOP_GROUND)
    {
        m_Dpd_Num = 1;
        m_Dpd = new DRAW_POLYGON_DAT[m_Dpd_Num];
        m_DpdOpt = new DRAW_POLYGON_IN_CAMERA_EX_OPTION[m_Dpd_Num];

        m_Dpd[0] = drawDatTmp;
        m_Dpd[0].texNo = LoadTexData("TEX/effect/eff_kn_loop").textureId;
        m_Dpd[0].shaderOpt.blendState = DPD_SHADER_OPT_BLEND_STATE_DEFAULT;

        m_DpdOpt[0] = opt;
        m_DpdOpt[0].billboard = false;
        m_DpdOpt[0].depthBuffIgnore = true;
        m_DpdOpt[0].priority = DRAW_POLYGON_PRIORITY_NML_d1;
        m_DpdOpt[0].shaderType = DPD_SHADER_TYPE_EFFECT;
    }
    if (et == GO_EFFECT_TYPE_HAMMER_CHARGE_END)
    {
        m_Dpd_Num = 1;
        m_Dpd = new DRAW_POLYGON_DAT[m_Dpd_Num];
        m_DpdOpt = new DRAW_POLYGON_IN_CAMERA_EX_OPTION[m_Dpd_Num];

        m_Dpd[0] = drawDatTmp;
        m_Dpd[0].texNo = LoadTexData("TEX/effect/eff_kn_end").textureId;
        m_Dpd[0].shaderOpt.blendState = DPD_SHADER_OPT_BLEND_STATE_ADDCOLOR;

        m_DpdOpt[0] = opt;
        m_DpdOpt[0].priority = DRAW_POLYGON_PRIORITY_NML_u1;
        m_DpdOpt[0].depthBuffIgnore = true;
        m_DpdOpt[0].shaderType = DPD_SHADER_TYPE_EFFECT;
    }
    if (et == GO_EFFECT_TYPE_HAMMER_CHARGE_END_GROUND)
    {
        m_Dpd_Num = 1;
        m_Dpd = new DRAW_POLYGON_DAT[m_Dpd_Num];
        m_DpdOpt = new DRAW_POLYGON_IN_CAMERA_EX_OPTION[m_Dpd_Num];

        m_Dpd[0] = drawDatTmp;
        //m_Dpd[0].texNo = LoadTexData("TEX/effect/eff_kn_loop").textureId;
        m_Dpd[0].texNo = LoadTexData("TEX/effect/eff_kn_end").textureId;
        m_Dpd[0].shaderOpt.blendState = DPD_SHADER_OPT_BLEND_STATE_DEFAULT;

        m_DpdOpt[0] = opt;
        m_DpdOpt[0].billboard = false;
        m_DpdOpt[0].depthBuffIgnore = true;
        m_DpdOpt[0].priority = DRAW_POLYGON_PRIORITY_NML_d1;
        m_DpdOpt[0].shaderType = DPD_SHADER_TYPE_EFFECT;
    }
    if (et == GO_EFFECT_TYPE_HAMMER_CHARGE_END_NON_BILLBOARD)
    {
        m_Dpd_Num = 1;
        m_Dpd = new DRAW_POLYGON_DAT[m_Dpd_Num];
        m_DpdOpt = new DRAW_POLYGON_IN_CAMERA_EX_OPTION[m_Dpd_Num];

        m_Dpd[0] = drawDatTmp;
        //m_Dpd[0].texNo = LoadTexData("TEX/effect/eff_kn_loop").textureId;
        m_Dpd[0].texNo = LoadTexData("TEX/effect/eff_kn_end").textureId;
        m_Dpd[0].shaderOpt.blendState = DPD_SHADER_OPT_BLEND_STATE_ADDCOLOR;

        m_DpdOpt[0] = opt;
        m_DpdOpt[0].billboard = false;
        m_DpdOpt[0].depthBuffIgnore = true;
        m_DpdOpt[0].priority = DRAW_POLYGON_PRIORITY_NML_u1;
        m_DpdOpt[0].shaderType = DPD_SHADER_TYPE_EFFECT;
    }
    if (et == GO_EFFECT_TYPE_WARP_IN)
    {
        m_Dpd_Num = 2;
        m_Dpd = new DRAW_POLYGON_DAT[m_Dpd_Num];
        m_DpdOpt = new DRAW_POLYGON_IN_CAMERA_EX_OPTION[m_Dpd_Num];

        m_Dpd[0] = drawDatTmp;
        m_Dpd[0].texNo = LoadTexData("TEX/effect/eff_kn_loop").textureId;
        m_Dpd[0].shaderOpt.blendState = DPD_SHADER_OPT_BLEND_STATE_ADDCOLOR;

        m_DpdOpt[0] = opt;
        m_DpdOpt[0].billboard = false;
        m_DpdOpt[0].priority = DRAW_POLYGON_PRIORITY_NML_u1;
        m_DpdOpt[0].depthBuffIgnore = true;
        m_DpdOpt[0].shaderType = DPD_SHADER_TYPE_EFFECT;

        m_Dpd[1] = m_Dpd[0];
        m_DpdOpt[1] = m_DpdOpt[0];
    }
    if (et == GO_EFFECT_TYPE_WARP_OUT)
    {
        m_Dpd_Num = 2;
        m_Dpd = new DRAW_POLYGON_DAT[m_Dpd_Num];
        m_DpdOpt = new DRAW_POLYGON_IN_CAMERA_EX_OPTION[m_Dpd_Num];

        m_Dpd[0] = drawDatTmp;
        m_Dpd[0].texNo = LoadTexData("TEX/effect/eff_kn_end").textureId;
        m_Dpd[0].shaderOpt.blendState = DPD_SHADER_OPT_BLEND_STATE_ADDCOLOR;

        m_DpdOpt[0] = opt;
        m_DpdOpt[0].billboard = false;
        m_DpdOpt[0].priority = DRAW_POLYGON_PRIORITY_NML_u1;
        m_DpdOpt[0].depthBuffIgnore = true;
        m_DpdOpt[0].shaderType = DPD_SHADER_TYPE_EFFECT;

        m_Dpd[1] = m_Dpd[0];
        m_DpdOpt[1] = m_DpdOpt[0];
    }
    if (et == GO_EFFECT_TYPE_OVERLAY_BALL)
    {
        m_Dpd_Num = 1;
        m_Dpd = new DRAW_POLYGON_DAT[m_Dpd_Num];
        m_DpdOpt = new DRAW_POLYGON_IN_CAMERA_EX_OPTION[m_Dpd_Num];

        m_Dpd[0] = drawDatTmp;
        m_Dpd[0].texNo = 0;
        m_Dpd[0].modelNo = GetModelDataIndex("MODEL/ball");

        m_DpdOpt[0] = opt;
        m_DpdOpt[0].priority = DRAW_POLYGON_PRIORITY_NML_ALWAYS;
        m_DpdOpt[0].depthBuffIgnore = true;
        m_DpdOpt[0].shaderType = DPD_SHADER_TYPE_EFFECT;
    }

	m_Obj->graphic.hidden = true;
}

void GOE_Effect::FirstFrame()
{
	GO_EFFECT_TYPE et = (GO_EFFECT_TYPE)m_Obj->freeInt;

	if (m_Dpd_Num <= 0)
	{
		m_Obj->graphic.hidden = false;
	}
}

// ===================================================
// オブジェクトの終了
// ===================================================
void GOE_Effect::Uninit()
{
	//for (int i = 0; i < m_Dpd_Num; i++)
	//{
	//	delete (m_Dpd + i);
	//	delete (m_DpdOpt + i);
	//}

	if (m_Dpd != nullptr)
	{
        for (int i = 0; i < m_Dpd_Num; i++)
        {
            DRAW_POLYGON_DAT dpd = m_Dpd[i];
            UnloadTexData(dpd.texNo);
        }
		delete[] m_Dpd;
	}
	if (m_DpdOpt != nullptr)
	{
		delete[] m_DpdOpt;
	}
}

// ===================================================
// オブジェクトの更新
// ===================================================
void GOE_Effect::Update()
{

}

// ===================================================
// オブジェクトの更新(アクティブ)
// ===================================================
void GOE_Effect::UpdateActive()
{
	GAME_OBJECT* objOwner = nullptr;

	if (m_Obj->ownerObjectIndex >= 0)
	{
		objOwner = GetGameObject(m_Obj->ownerObjectIndex);
	}

	GO_EFFECT_TYPE et = (GO_EFFECT_TYPE)m_Obj->freeInt;

	float alpha = (float)m_Obj->lifetimeFrame / m_Obj->lifetimeFrameMax;


	if (et == GO_EFFECT_TYPE_AFTEREFFECT)
	{
		//for (int i = 0; i < POLYGON_GRAPH_MODEL_DAT_MAX; i++)
		//{
		//	m_Obj->graphic.graph.polygonModel[i].color.w = alpha;
		//}
		m_Obj->graphic.graph.alpha = alpha;
	}


	if (m_Dpd == nullptr || m_DpdOpt == nullptr)
	{
		return;
	}

	if (et == GO_EFFECT_TYPE_SHOCKWAVE || et == GO_EFFECT_TYPE_SHOCKWAVE_ENEM)
	{
		m_Dpd[0].startRot3D.x = m_Obj->graphic.rot3d.x;
		m_Dpd[0].startRot3D.y = m_Obj->graphic.rot3d.y;
		m_Dpd[0].startRot3D.z = m_Obj->graphic.rot3d.z;
		m_Dpd[0].color.w = alpha * m_Obj->graphic.color.w;
		m_Dpd[0].size.x = m_Obj->size.x + (1.0f - alpha);
		m_Dpd[0].size.y = m_Obj->size.y + (1.0f - alpha);
	}

	if (et == GO_EFFECT_TYPE_FLASH_0 || et == GO_EFFECT_TYPE_FLASH_0_ENEM)
	{
		int index = 0;
		for (int i = 0; i < m_Dpd_Num; i++)
		{
			{
				if (index % 2 == 0 && m_Obj->frame == 0)
				{
					m_Dpd[i].size.y *= 0.6f;
				}
				float rad = (float)(2.0f * SYS_GENERAL_PI / m_Dpd_Num * index + ((float)m_Obj->createFrame / 80));

				//sadat->pos.x = sin(rad) * 8.0f * (1.0f - alpha);
				//sadat->pos.y = cos(rad) * 8.0f * (1.0f - alpha);
				m_Dpd[i].pos.x = sin(rad) * -(40.0f + 40.0f * (1.0f - alpha));
				m_Dpd[i].pos.z = cos(rad) * (40.0f + 40.0f * (1.0f - alpha));
				m_Dpd[i].rot = CalculateRadToDeg(rad);
				index++;
			}

			m_Dpd[i].color.w = alpha;
		}
	}

	if (et == GO_EFFECT_TYPE_FLASH_1 || et == GO_EFFECT_TYPE_FLASH_1_ENEM)
	{
		int index = 0;
		for (int i = 0; i < m_Dpd_Num; i++)
		{
			{
				if (m_Obj->frame == 0)
				{
					m_Dpd[i].size.y *= 0.65f;
				}
				float rad = (float)(2.0f * SYS_GENERAL_PI / m_Dpd_Num * index + ((float)m_Obj->createFrame / 3));

				m_Dpd[i].pos.x = sin(rad) * -(160.0f * alpha);
				m_Dpd[i].pos.z = cos(rad) * (160.0f * alpha);
				m_Dpd[i].rot = CalculateRadToDeg(rad);
				index++;
			}

			m_Dpd[i].color.w = alpha;
		}
	}


	if (et == GO_EFFECT_TYPE_TWINKLE)
	{
		//m_Dpd[0].startRot3D.x = m_Obj->graphic.rot3d.x;
		//m_Dpd[0].startRot3D.y = m_Obj->graphic.rot3d.y;
		//m_Dpd[0].startRot3D.z = m_Obj->graphic.rot3d.z;
		m_Dpd[0].color.w = alpha * m_Obj->graphic.color.w;
		//m_Dpd[0].size.x = m_Obj->size.x + (1.0f - alpha);
		//m_Dpd[0].size.y = m_Obj->size.y + (1.0f - alpha);
	}

	if (et == GO_EFFECT_TYPE_SPARK)
	{
		m_Dpd[0].startRot3D.x = m_Obj->graphic.rot3d.x;
		m_Dpd[0].startRot3D.y = m_Obj->graphic.rot3d.y;
		m_Dpd[0].startRot3D.z = m_Obj->graphic.rot3d.z + m_Obj->rot;
        m_Dpd[0].color.x = m_Obj->graphic.color.x;
        m_Dpd[0].color.y = m_Obj->graphic.color.y;
        m_Dpd[0].color.z = m_Obj->graphic.color.z;

		m_Dpd[0].color.w = alpha * abs(m_Obj->graphic.color.w);
        if (m_Obj->graphic.color.w < 0)
        {
            m_Dpd[0].shaderOpt.blendState = DPD_SHADER_OPT_BLEND_STATE_DEFAULT;
        }
        else
        {
            m_Dpd[0].shaderOpt.blendState = DPD_SHADER_OPT_BLEND_STATE_ADDCOLOR;
        }

		m_Dpd[0].size.x = m_Obj->size.x + (1.0f - alpha);
		m_Dpd[0].size.y = m_Obj->size.y + (1.0f - alpha);

        m_Dpd[0].texUV = AnimationUV(16 * (1.0f - alpha), 4, 4);
        m_Dpd[0].texWH = { 1.0f / 4, 1.0f / 4 };
	}

    if (et == GO_EFFECT_TYPE_EXCLAMATION)
    {
        m_Dpd[0].color.w = alpha;
    }
    if (et == GO_EFFECT_TYPE_QUESTION)
    {
        m_Dpd[0].color.w = alpha;
    }

	if (et == GO_EFFECT_TYPE_SHOCKWAVE_3D)
	{
		m_Dpd[0].startRot3D.x = m_Obj->graphic.rot3d.x;
		m_Dpd[0].startRot3D.y = m_Obj->graphic.rot3d.y;
		m_Dpd[0].startRot3D.z = m_Obj->graphic.rot3d.z;
		m_Dpd[0].color.x = m_Obj->graphic.color.x;
		m_Dpd[0].color.y = m_Obj->graphic.color.y;
		m_Dpd[0].color.z = m_Obj->graphic.color.z;
		m_Dpd[0].color.w = alpha;
		m_Dpd[0].startScale3D.x = m_Obj->size.x + (1.0f - alpha) * (1.0f - m_Obj->size.x);
		m_Dpd[0].startScale3D.z = m_Obj->size.y + (1.0f - alpha) * (1.0f - m_Obj->size.y);
	}
	if (et == GO_EFFECT_TYPE_HITMARK)
	{
		m_Dpd[0].startRot3D.x = m_Obj->graphic.rot3d.x;
		m_Dpd[0].startRot3D.y = m_Obj->graphic.rot3d.y;
		m_Dpd[0].startRot3D.z = m_Obj->graphic.rot3d.z;
		m_Dpd[0].color.w = alpha * m_Obj->graphic.color.w;
	}


    if (et == GO_EFFECT_TYPE_N_ATTACK_HIT)
    {
        m_Dpd[0].texUV = AnimationUV((1.0f - alpha) * 9, 3, 3);
        m_Dpd[0].texWH = { 1.0f / 3, 1.0f / 3 };
        //m_Dpd[0].color.w = alpha;
    }
    if (et == GO_EFFECT_TYPE_HITSTAR_VANISH)
    {
        m_Dpd[0].texUV = AnimationUV((1.0f - alpha) * 16, 4, 4);
        m_Dpd[0].texWH = { 1.0f / 4, 1.0f / 4 };
        //m_Dpd[0].color.w = alpha;
    }

    if (et == GO_EFFECT_TYPE_EXPLOSION_1)
    {
        constexpr int n = 5;
        m_Dpd[0].texUV = AnimationUV((1.0f - alpha) * (n * n), n, n);
        m_Dpd[0].texWH = { 1.0f / n, 1.0f / n };

        float lm = (float)m_Obj->lifetimeFrame / m_Obj->lifetimeFrameMax;
        if (lm > 0.7f)
        {
            m_Dpd[0].shaderOpt.blendState = DPD_SHADER_OPT_BLEND_STATE_ADDCOLOR;
        }
        else
        {
            m_Dpd[0].shaderOpt.blendState = DPD_SHADER_OPT_BLEND_STATE_DEFAULT;
        }
    }
    if (et == GO_EFFECT_TYPE_EXPLOSION_2)
    {
        constexpr int nx = 4;
        constexpr int ny = 4;
        m_Dpd[0].texUV = AnimationUV((1.0f - alpha) * (nx * ny), nx, ny);
        m_Dpd[0].texWH = { 1.0f / nx, 1.0f / ny };

        float lm = (float)m_Obj->lifetimeFrame / m_Obj->lifetimeFrameMax;
        if (lm > 0.7f)
        {
            m_Dpd[0].shaderOpt.blendState = DPD_SHADER_OPT_BLEND_STATE_ADDCOLOR;
        }
        else
        {
            m_Dpd[0].shaderOpt.blendState = DPD_SHADER_OPT_BLEND_STATE_DEFAULT;
        }
    }
    if (et == GO_EFFECT_TYPE_EXPLOSION_3)
    {
        constexpr int n = 4;
        m_Dpd[0].texUV = AnimationUV((1.0f - alpha) * (n * n), n, n);
        m_Dpd[0].texWH = { 1.0f / n, 1.0f / n };

        float lm = (float)m_Obj->lifetimeFrame / m_Obj->lifetimeFrameMax;
        if (lm > 0.7f)
        {
            m_Dpd[0].shaderOpt.blendState = DPD_SHADER_OPT_BLEND_STATE_ADDCOLOR;
        }
        else
        {
            m_Dpd[0].shaderOpt.blendState = DPD_SHADER_OPT_BLEND_STATE_DEFAULT;
        }
    }
    if (et == GO_EFFECT_TYPE_EXPLOSION_4)
    {
        constexpr int n = 4;
        m_Dpd[0].texUV = AnimationUV((1.0f - alpha) * (n * n), n, n);
        m_Dpd[0].texWH = { 1.0f / n, 1.0f / n };
    }
    if (et == GO_EFFECT_TYPE_EXPLOSION_SMOKE)
    {
        constexpr int nx = 4;
        constexpr int ny = 3;
        m_Dpd[0].texUV = AnimationUV((1.0f - alpha) * (nx * ny), nx, ny);
        m_Dpd[0].texWH = { 1.0f / nx, 1.0f / ny };
    }

    if (et == GO_EFFECT_TYPE_EXPLOSION_1_GROUND)
    {
        m_Dpd[0].startRot3D.z = m_Obj->graphic.rot3d.z + m_Obj->rot;

        constexpr int n = 5;
        m_Dpd[0].texUV = AnimationUV((1.0f - alpha) * (n * n), n, n);
        m_Dpd[0].texWH = { 1.0f / n, 1.0f / n };

        float lm = (float)m_Obj->lifetimeFrame / m_Obj->lifetimeFrameMax;
        if (lm > 0.6f)
        {
            m_Dpd[0].shaderOpt.blendState = DPD_SHADER_OPT_BLEND_STATE_ADDCOLOR;
        }
        else
        {
            m_Dpd[0].shaderOpt.blendState = DPD_SHADER_OPT_BLEND_STATE_DEFAULT;
        }
    }
    if (et == GO_EFFECT_TYPE_EXPLOSION_1_GROUND2)
    {
        m_Dpd[0].startRot3D.z = m_Obj->graphic.rot3d.z + m_Obj->rot;

        constexpr int n = 5;
        constexpr int sn = 18;
        m_Dpd[0].texUV = AnimationUV(sn + (1.0f - alpha) * (n * n - sn), n, n);
        m_Dpd[0].texWH = { 1.0f / n, 1.0f / n };

    }

    if (et == GO_EFFECT_TYPE_SMOKE)
    {
        m_Dpd[0].startRot3D.x = m_Obj->graphic.rot3d.x;
        m_Dpd[0].startRot3D.y = m_Obj->graphic.rot3d.y;
        m_Dpd[0].startRot3D.z = m_Obj->graphic.rot3d.z;
        m_Dpd[0].texUV = AnimationUV((1.0f - alpha) * 16, 4, 4);
        m_Dpd[0].texWH = { 1.0f / 4, 1.0f / 4 };
        m_Dpd[0].color.w = alpha;
    }
    if (et == GO_EFFECT_TYPE_HAMMER_SHOCKWAVE)
    {
        m_Dpd[0].texUV = AnimationUV((1.0f - alpha) * 16, 4, 4);
        m_Dpd[0].texWH = { 1.0f / 4, 1.0f / 4 };
        //m_Dpd[0].color.w = alpha;
    }
    if (et == GO_EFFECT_TYPE_HAMMER_SHOCKDECAL)
    {
        m_Dpd[0].color.w = alpha;
    }
    if (et == GO_EFFECT_TYPE_SCOREEFF)
    {
        m_Dpd[0].texUV = AnimationUV((1.0f - alpha) * 20, 5, 4);
        m_Dpd[0].texWH = { 1.0f / 5, 1.0f / 4 };
        //m_Dpd[0].color.w = alpha;
    }
    if (et == GO_EFFECT_TYPE_SCOREEFF_REV)
    {
        m_Dpd[0].texUV = AnimationUV((alpha) * 20, 5, 4);
        m_Dpd[0].texWH = { 1.0f / 5, 1.0f / 4 };
        //m_Dpd[0].color.w = alpha;
    }
    if (et == GO_EFFECT_TYPE_HITSTAR_VANISH_REV)
    {
        m_Dpd[0].color.w = alpha * abs(m_Obj->graphic.color.w);
        if (m_Obj->graphic.color.w < 0)
        {
            m_Dpd[0].shaderOpt.blendState = DPD_SHADER_OPT_BLEND_STATE_DEFAULT;
        }
        else
        {
            m_Dpd[0].shaderOpt.blendState = DPD_SHADER_OPT_BLEND_STATE_ADDCOLOR;
        }
        m_Dpd[0].texUV = AnimationUV((alpha) * 16, 4, 4);
        m_Dpd[0].texWH = { 1.0f / 4, 1.0f / 4 };
        //m_Dpd[0].color.w = alpha;
    }


    if (et == GO_EFFECT_TYPE_HAMMER_CHARGE_LOOP)
    {
        m_Dpd[0].color.w = alpha * abs(m_Obj->graphic.color.w);
        if (m_Obj->graphic.color.w < 0)
        {
            m_Dpd[0].shaderOpt.blendState = DPD_SHADER_OPT_BLEND_STATE_DEFAULT;
        }
        else
        {
            m_Dpd[0].shaderOpt.blendState = DPD_SHADER_OPT_BLEND_STATE_ADDCOLOR;
        }
        m_Dpd[0].texUV = AnimationUV((1.0f - alpha) * 16, 4, 4);
        m_Dpd[0].texWH = { 1.0f / 4, 1.0f / 4 };
        //m_Dpd[0].color.w = alpha;
    }
    if (et == GO_EFFECT_TYPE_HAMMER_CHARGE_LOOP_GROUND)
    {
        m_Dpd[0].startRot3D.z = m_Obj->graphic.rot3d.z + m_Obj->rot;

        m_Dpd[0].color.w = alpha * abs(m_Obj->graphic.color.w);
        if (m_Obj->graphic.color.w < 0)
        {
            m_Dpd[0].shaderOpt.blendState = DPD_SHADER_OPT_BLEND_STATE_DEFAULT;
        }
        else
        {
            m_Dpd[0].shaderOpt.blendState = DPD_SHADER_OPT_BLEND_STATE_ADDCOLOR;
        }

        constexpr int n = 4;
        constexpr int sn = 0;
        m_Dpd[0].texUV = AnimationUV(sn + (1.0f - alpha) * (n * n - sn), n, n);
        m_Dpd[0].texWH = { 1.0f / n, 1.0f / n };

    }
    if (et == GO_EFFECT_TYPE_HAMMER_CHARGE_END)
    {
        m_Dpd[0].texUV = AnimationUV((1.0f - alpha) * 16, 4, 4);
        m_Dpd[0].texWH = { 1.0f / 4, 1.0f / 4 };
        //m_Dpd[0].color.w = alpha;
    }
    if (et == GO_EFFECT_TYPE_HAMMER_CHARGE_END_GROUND)
    {
        m_Dpd[0].startRot3D.z = m_Obj->graphic.rot3d.z + m_Obj->rot;

        constexpr int n = 4;
        constexpr int sn = 0;
        m_Dpd[0].texUV = AnimationUV(sn + (1.0f - alpha) * (n * n - sn), n, n);
        m_Dpd[0].texWH = { 1.0f / n, 1.0f / n };

    }
    if (et == GO_EFFECT_TYPE_HAMMER_CHARGE_END_NON_BILLBOARD)
    {
        m_Dpd[0].startRot3D.x = m_Obj->graphic.rot3d.x;
        m_Dpd[0].startRot3D.y = m_Obj->graphic.rot3d.y;
        m_Dpd[0].startRot3D.z = m_Obj->graphic.rot3d.z + m_Obj->rot;

        constexpr int n = 4;
        constexpr int sn = 0;
        m_Dpd[0].texUV = AnimationUV(sn + (1.0f - alpha) * (n * n - sn), n, n);
        m_Dpd[0].texWH = { 1.0f / n, 1.0f / n };
    }

    if (et == GO_EFFECT_TYPE_WARP_IN)
    {
        for (int i = 0; i < m_Dpd_Num; i++)
        {
            m_Dpd[i].size.x = 200.0f * EasingOutCubic(alpha);
            m_Dpd[i].size.y = 150.0f * EasingOutCubic(alpha);
            //m_Dpd[i].color.w = alpha * abs(m_Obj->graphic.color.w);
            m_Dpd[i].texUV = AnimationUV((1.0f - alpha) * 16, 4, 4);
            m_Dpd[i].texWH = { 1.0f / 4, 1.0f / 4 };
            //m_Dpd[0].color.w = alpha;
            m_Dpd[i].color.w = std::min(5.0f * alpha, 1.0f);
        }
        m_Dpd[0].startRot3D.y = +32.0f;
        m_Dpd[1].startRot3D.y = -32.0f;
    }
    if (et == GO_EFFECT_TYPE_WARP_OUT)
    {
        for (int i = 0; i < m_Dpd_Num; i++)
        {
            m_Dpd[i].size.x = 200.0f * EasingOutCubic(1.0f - alpha);
            m_Dpd[i].size.y = 150.0f * EasingOutCubic(1.0f - alpha);
            m_Dpd[i].texUV = AnimationUV((1.0f - alpha) * 16, 4, 4);
            m_Dpd[i].texWH = { 1.0f / 4, 1.0f / 4 };
            //m_Dpd[i].color.w = alpha;
            m_Dpd[i].color.w = std::min(5.0f * alpha, 1.0f);
        }
        m_Dpd[0].startRot3D.y = +32.0f;
        m_Dpd[1].startRot3D.y = -32.0f;
    }

    if (et == GO_EFFECT_TYPE_OVERLAY_BALL)
    {
        //m_Dpd[0].color.w = alpha * abs(m_Obj->graphic.color.w);
        m_Dpd[0].color.w = abs(m_Obj->graphic.color.w);
        m_Dpd[0].color.x = m_Obj->graphic.color.x;
        m_Dpd[0].color.y = m_Obj->graphic.color.y;
        m_Dpd[0].color.z = m_Obj->graphic.color.z;
        if (m_Obj->graphic.color.w < 0)
        {
            m_Dpd[0].shaderOpt.blendState = DPD_SHADER_OPT_BLEND_STATE_DEFAULT;
        }
        else
        {
            m_Dpd[0].shaderOpt.blendState = DPD_SHADER_OPT_BLEND_STATE_ADDCOLOR;
        }
        m_Dpd[0].size.x = (1.0f - alpha);
        m_Dpd[0].size.y = (1.0f - alpha);
        m_Dpd[0].size.z = (1.0f - alpha);
    }
}


void GOE_Effect::Draw()
{
	if (m_Dpd == nullptr || m_DpdOpt == nullptr || m_Obj->frameActive <= 1)
	{
		return;
	}


	for (int i = 0; i < m_Dpd_Num; i++)
	{
		DRAW_POLYGON_DAT dpd = m_Dpd[i];
		dpd.startPos3D.x += m_Obj->pos.x;
		dpd.startPos3D.y += m_Obj->pos.y;
		dpd.startPos3D.z += m_Obj->pos.z;
		dpd.startScale3D.x *= m_Obj->scale.x;
        dpd.startScale3D.y *= m_Obj->scale.y;
        dpd.startScale3D.z *= m_Obj->scale.z;
        //dpd.startScale3D.z *= (m_Obj->scale.x + m_Obj->scale.y) / 2;

		DrawPolygonInCameraEx(dpd, m_DpdOpt[i]);
	}
}


void GOE_Effect::Dead()
{
}


void GOE_Effect::DamageAttacker(GAME_OBJECT_DAMAGE_RESULT dr)
{
}

void GOE_Effect::DamageReciever(GAME_OBJECT_DAMAGE_RESULT dr)
{
}

void GOE_Effect::BlockHit(GAME_OBJECT_POSMOD_RESULT pmr)
{
}


GAME_OBJECT* gO_CreateEffect(GO_EFFECT_TYPE et, GAME_OBJECT* owner)
{
	GAME_OBJECT go = GetGameObjectPrefab(GAME_OBJECT_TYPE_EFFECT);
	go.freeInt = (int)et;
	if (owner != nullptr)
	{
		go.ownerObjectIndex = (int)(owner - GetGameObjects());
	}

	GAME_OBJECT* goCrtd = CreateGameObject(go);
	if (goCrtd != nullptr)
	{
	}

	return goCrtd;
}
GAME_OBJECT* GO_CreateEffect(GO_EFFECT_TYPE et, Float3 pos)
{
	GAME_OBJECT* go = gO_CreateEffect(et, nullptr);
	if (go != nullptr)
	{
		go->pos = pos;
	}

	return go;
}
GAME_OBJECT* GO_CreateEffect(GO_EFFECT_TYPE et, GAME_OBJECT* owner)
{
	GAME_OBJECT* go = gO_CreateEffect(et, owner);
	if (owner == nullptr)
	{ //オーナーが消失していたら非表示にする
		go->graphic.hidden = true;
	}

	return go;
}

GAME_OBJECT* GO_CreateAfterEffect(GAME_OBJECT* owner, int life, Float4 color)
{
	GAME_OBJECT* go = CreateGameObject(GetGameObjectPrefab(GAME_OBJECT_TYPE_EFFECT));
	if (go != nullptr)
	{
		go->zIndex = GAME_OBJECT_ZINDEX_AFTEREFFECT;
		go->graphic = owner->graphic;
		go->graphic.stop = true;
		go->graphic.color = color;
		//go->graphic.gray = true;
		go->graphic.shadow = false;
		//for (int i = 0; i < POLYGON_GRAPH_MODEL_DAT_MAX; i++)
		//{
		//	go->graphic.shaderOpts[i].bloom = false;
		//	go->graphic.shaderOpts[i].outline = false;
		//}
		go->graphic.shaderOpt.bloom = DPD_SHADER_OPT_BLOOM_NONE;
		go->graphic.shaderOpt.outline = DPD_SHADER_OPT_OUTLINE_NONE;
        go->graphic.priority = DRAW_POLYGON_PRIORITY_NML_d3;
        go->graphic.depthBuffIgnore = true;

		go->pos = owner->pos;
		go->scale = owner->scale;

		go->freeInt = (int)GO_EFFECT_TYPE_AFTEREFFECT;
		GameObjectSetLifeTime(go, life);

		if (owner == nullptr)
		{ //オーナーが消失していたら非表示にする
			go->graphic.hidden = true;
		}
	}

	return go;
}

void GO_CreateExplosion(Float3 pos, float sca)
{
    {
        //float sca = 0.8f;
        Float3 efPos = pos;

        {
            Float3 efPos1 = efPos;
            efPos1.y += 96.0f * sca;

            GAME_OBJECT* go = GO_CreateEffect(GO_EFFECT_TYPE_EXPLOSION_1, efPos1);
            if (go != nullptr)
            {
                go->scale = MkF3(
                    16.0f * sca,
                    16.0f * sca,
                    16.0f * sca
                );
                GameObjectSetLifeTime(go, 50);
            }
        }
        {
            Float3 efPos1 = efPos;
            efPos1.y += -20.0f;
            efPos1.z += 96.0f * -1.5f * sca;

            GAME_OBJECT* go = GO_CreateEffect(GO_EFFECT_TYPE_EXPLOSION_2, efPos1);
            if (go != nullptr)
            {
                go->scale = MkF3(
                    20.0f * sca,
                    20.0f * sca,
                    20.0f * sca
                );
                GameObjectSetLifeTime(go, 45);
            }
        }
        {
            Float3 efPos1 = efPos;
            efPos1.y += -40.0f;

            GAME_OBJECT* go = GO_CreateEffect(GO_EFFECT_TYPE_EXPLOSION_3, efPos1);
            if (go != nullptr)
            {
                go->scale = MkF3(
                    26.0f * sca,
                    26.0f * sca,
                    26.0f * sca
                );
                GameObjectSetLifeTime(go, 22);
            }
        }
        {
            Float3 efPos1 = efPos;
            efPos1.y += 10.0f;

            GAME_OBJECT* go = GO_CreateEffect(GO_EFFECT_TYPE_EXPLOSION_4, efPos1);
            if (go != nullptr)
            {
                go->scale = MkF3(
                    27.0f * sca,
                    27.0f * sca,
                    27.0f * sca
                );
                GameObjectSetLifeTime(go, 40);
            }
        }
    }
}