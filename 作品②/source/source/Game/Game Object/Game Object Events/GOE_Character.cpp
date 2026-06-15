// ===================================================
// GOE_Character.cpp キャラクターオブジェクト
// 
// hara sougo  2024/11/11
// ===================================================
#define NO_CONTROLLER

#include "../../../Game/Configuration/Param.h"
#include "../../../game_main.h"
#include "../../Sprite/sprite.h"
#include "../../Polygon/modelData.h"
#include "../../Texture/texture.h"
#include "../../../Game/Entities/Block/block.h"
#include "../../Systems/collision.h"
#include "../../Systems/easing.h"

#include "../../Managers/gameManager.h"

#include "../gameObject.h"
#include "../gameObjectPrefab.h"

#include "../../Systems/camera.h"
#include "../../Systems/ui.h"

#include "../../Entities/player.h"

#include "GOE_Character.h"

// ===================================================
// マクロ定義
// ===================================================

// ===================================================
// プロトタイプ宣言
// ===================================================

// ===================================================
// グローバル変数
// ===================================================


// ===================================================
// Init
// オブジェクトが生成されたとき
// ===================================================
void GOE_Character::Init()
{
    //m_Tex_EfBuff = LoadTexData("TEX/effect/eff_buff_loop_01").textureId;
    //m_Tex_EfBuff2 = LoadTexData("TEX/effect/buff_mires_01").textureId;
    //m_Tex_EfDeBuff = LoadTexData("TEX/effect/eff_debuff_01").textureId;
    //m_Tex_EfHeal = LoadTexData("TEX/effect/eff_heal_01").textureId;

    m_Tex_EfBuff_Flow = LoadTexData("TEX/effect/buff_flow").textureId;
    m_Tex_EfBuff_Flare = LoadTexData("TEX/effect/buff_flare").textureId;
    m_Tex_EfHeal_Flow = LoadTexData("TEX/effect/heal_flow").textureId;
    m_Tex_EfHeal_Flare = LoadTexData("TEX/effect/heal_flare").textureId;
    m_Model_EfBuff = GetModelDataIndex("MODEL/effect/Flare");

	//キャラオブジェクトのデータを取得
	Character::OBJECT_DAT* goDat = Character::GetObjectDataCharacterGameObject(m_Obj);
	if (goDat == nullptr)
	{
		return;
	}
	//
	//↓処理
	//

	m_CharaData = goDat;

    m_CollisionDefault = m_Obj->collision;

	//PLAYER* player = GetPlayer(obj);
	//if (player == nullptr)
	//{
	//	return;
	//}
	////
	////↓処理
	////
	
}

// ===================================================
// FirstFrame
// 最初のフレームのみ実行
// ===================================================
void GOE_Character::FirstFrame()
{
	PLAYER* player = GetPlayer(m_Obj);
	if (player == nullptr)
	{
		return;
	}
	//
	//↓処理
	//

	//最初のフレーム(初期化とか)

}

// ===================================================
// Uninit
// オブジェクトが削除されるとき
// ===================================================
void GOE_Character::Uninit()
{
    //UnloadTexData(m_Tex_EfBuff);
    //UnloadTexData(m_Tex_EfBuff2);
    //UnloadTexData(m_Tex_EfDeBuff);
    //UnloadTexData(m_Tex_EfHeal);
    UnloadTexData(m_Tex_EfBuff_Flow);
    UnloadTexData(m_Tex_EfHeal_Flow);
    UnloadTexData(m_Tex_EfBuff_Flare);
    UnloadTexData(m_Tex_EfHeal_Flare);

	////キャラオブジェクトのデータを取得
	//Character::OBJECT_DAT* goDat = Character::GetObjectDataCharacterGameObject(m_Obj);
	//if (goDat == nullptr)
	//{
	//	return;
	//}
	////
	////↓処理
	////

	////プライベート変数の解放
	//if (goDat->privateVariable != nullptr)
	//{
	//	free(goDat->privateVariable);
	//}

	//テクスチャの解放
	// 
	//if (goDat->texture.textureId != 0)
	//{
	//	UnloadTexture(goDat->texture.textureId);
	//}
	//if (goDat->texture.textureIdGray != 0)
	//{
	//	UnloadTexture(goDat->texture.textureIdGray);
	//}

	//キャラクターデータの解放
	//free(goDat->data);

	//obj->privateVariable は
	//オブジェクト削除時に自動的に解放されるので
	//ここで解放しなくてもいいよ
}

// ===================================================
// Update
// 毎フレーム実行
// ===================================================
void GOE_Character::Update()
{

}

// ===================================================
// UpdateActive
// 毎フレーム実行
// ただし、ヒットストップ中などの
// 非アクティブ時は実行されない
// ===================================================
void GOE_Character::UpdateActive()
{
	if (m_CharaData == nullptr)
	{
		return;
	}
	//
	//↓処理
	//

	{
		{ //向きの設定

			//向きを計算
			constexpr float rotp = 360.0f / (AXIS_2D_EX_MAX - AXIS_2D_EX_D);
            //float rot = CalculateRadToDeg(atan2(m_BeforeVec.y, m_BeforeVec.x)) + 180.0f + 45.0f + rotp / 2;
            float rot = CalculateRadToDeg(atan2(m_BeforeVec.y, m_BeforeVec.x)) + 180.0f + 90.0f + rotp / 2;
			while (rot < 0)
			{
				rot += 360.0f;
			}
			rot = (float)((int)rot % 360);

			//動いていたら向きに合わせて方向をセット
			if (abs(m_BeforeVec.x) > 0.01f || abs(m_BeforeVec.y) > 0.01f)
			{
				m_CurrentAxisEx = (AXIS_2D_EX)(AXIS_2D_EX_D + (rot / rotp));

			}

			//DebugPrintf("%f  %f\n", rot, (rot / rotp));
		}

		//以前のベクトルとして保存
		//goDat->beforeVec = obj->vel;
	}

	//ぬるぽじゃなかったらモーションを再生する
	if (m_CharaData->playingMotion.motion != nullptr
		&& m_CharaData->playingMotion.enable)
	{
		int nowframe = 0;
		if (m_CharaData->playingMotion.frameReference != nullptr)
		{
			//nowframe = (*goDat->playingMotion.frameReference) % goDat->playingMotion.motion->maxFrame;
			nowframe = (int)((*m_CharaData->playingMotion.frameReference)
				* ((double)m_CharaData->playingMotion.motion->maxFrame / m_CharaData->playingMotion.maxFrame))
				% m_CharaData->playingMotion.motion->maxFrame;
		}
		else
		{
			nowframe = (int)(m_CharaData->playingMotion.frame
				* ((double)m_CharaData->playingMotion.motion->maxFrame / m_CharaData->playingMotion.maxFrame));
		}

		//Character::MOTION_FRAME* motionFrame
		//	= &goDat->playingMotion.motion->frame[nowframe];

		//if (motionFrame->use)
		if(nowframe >= 0 && nowframe < m_CharaData->playingMotion.motion->frame.size())
		{
			const Character::MOTION_FRAME* motionFrame
				= &m_CharaData->playingMotion.motion->frame[nowframe];

			float reverseMod = m_CharaData->reverse ? -1.0f : 1.0f;
			//スプライト
			{

				SPRITE_GRAPH_DAT* sgdat = &m_Obj->graphic.graph;
				sgdat->reverse = m_CharaData->reverse;
				//sgdat->outlineWidth = 6.0f;
				//sgdat->alpha = 1.0f;

				//Float2 sizetmp = MkF2(100.0f, 100.0f);
				{ //
					SPRITE_ANIM_DAT* sadat = sgdat->spriteAnim + 0;

					sadat->no = motionFrame->spriteNumber;
				}
			}

			//{
			//	{ //当たり判定
			//		GAME_OBJECT_COLLISION* c = &obj->collision;
			//		//(*c) = {};
			//		//ZeroMemory(c, sizeof(*c));
			//		c->atk = {};
			//		c->rec = {};
			//		c->atk.enable = true;
			//		c->rec.enable = true;

			//		int indexAtk = 0;
			//		int indexRec = 0;
			//		for (int i = 0; i < Character::MOTION_COL_MAX; i++)
			//		{
			//			if(motionFrame->collision[i].use)
			//			{
			//				Float3 posTmp  = MkF3(
			//					motionFrame->collision[i].pos.x * obj->scale.x * reverseMod,
			//					motionFrame->collision[i].pos.y * obj->scale.y,
			//					0.0f);
			//				Float3 sizeTmp = MkF3(
			//					motionFrame->collision[i].size.x * obj->scale.x,
			//					motionFrame->collision[i].size.y * obj->scale.y,
			//					1.0f);

			//				//詳細データを取得
			//				Character::MOTION_COL_DAT motionColDetails = 
			//					Character::GetMotionColDataFindByName(
			//						motionFrame->collision[i].colType,
			//						motionFrame->collision[i].colName
			//					);

			//				if (motionFrame->collision[i].colType == Character::MOTION_COL_TYPE_REC)
			//				{ //くらい判定の設定
			//					if (indexRec >= GAME_OBJECT_COLLISION_NUM_MAX)
			//					{
			//						continue;
			//					}

			//					GAME_OBJECT_COLLISION_RECIEVE_CHILD* cc = c->rec.collision + indexRec;
			//					(*cc) = motionColDetails.recCol;
			//					//cc->enable = true;
			//					cc->pos = posTmp;
			//					cc->size = sizeTmp;
			//					//cc->guardMod = 1.0f;

			//					cc->subjectFamily[GAME_OBJECT_FAMILY_ALL] = true;

			//					indexRec++;
			//				}
			//				else
			//				{ //攻撃、つかみ判定の設定
			//					if (indexAtk >= GAME_OBJECT_COLLISION_NUM_MAX)
			//					{
			//						continue;
			//					}

			//					GAME_OBJECT_COLLISION_ATTACK_CHILD* cc = c->atk.collision + indexAtk;
			//					(*cc) = motionColDetails.atkCol;
			//					//cc->enable = true;
			//					cc->pos = posTmp;
			//					cc->size = sizeTmp;

			//					////攻撃
			//					//if (motionFrame->collision[i].colType == Character::MOTION_COL_TYPE_ATK)
			//					//{
			//					//	//----- 仮、後で改善する
			//					//	cc->power /*			*/ = 40;
			//					//	cc->hitStopAttacker /*	*/ = 8;
			//					//	cc->hitStopReciever/*	*/ = 8;
			//					//	cc->hitCycle /*			*/ = 0;
			//					//	cc->hitStern /*			*/ = 5;
			//					//	cc->guardSternMod /*	*/ = 1.0f;
			//					//	//------
			//					//}

			//					////つかみ
			//					//if (motionFrame->collision[i].colType == Character::MOTION_COL_TYPE_GRAB)
			//					//{
			//					//	cc->isGrab = true; //つかみフラグ

			//					//	//-----
			//					//	cc->power /*			*/ = 0;
			//					//	cc->hitStopAttacker /*	*/ = 0;
			//					//	cc->hitStopReciever/*	*/ = 0;
			//					//	cc->hitCycle /*			*/ = 0;
			//					//	//------
			//					//}

			//					cc->subjectFamily[GAME_OBJECT_FAMILY_ALL] = true;

			//					//cc->subjectFamily[GAME_OBJECT_FAMILY_PLAYER] = true;
			//					cc->groupNumber = motionFrame->collision[i].group;
			//					cc->id = goDat->playingMotion.playKey;

			//					indexAtk++;
			//				}
			//			}
			//		}
			//	}
			//}

            if (m_CharaData->playingMotion.frame >= m_CharaData->playingMotion.maxFrame - 1)
			{
				//ループがオンなら最大フレームを超えていたら
				//戻す
				if (m_CharaData->playingMotion.loop)
				{
					m_CharaData->playingMotion.frame = 0;
				}
				else
				{
					m_CharaData->playingMotion.enable = false;
				}
			}
			else
			{
				m_CharaData->playingMotion.frame++;
			}
		}

	}

    if (m_EfBuff2_Frame >= 0)
    {
        m_EfBuff2_Frame++;
        if (m_EfBuff2_Frame > m_EfBuff2_FrameMax)
        {
            m_EfBuff2_Frame = -1;
        }
    }
}

// ===================================================
// Draw
// オブジェクトの描画イベント
// ===================================================
void GOE_Character::Draw()
{
	//GAME_OBJECT* obj = GetGameObject(index);

	////キャラオブジェクトのデータを取得
	//Character::OBJECT_DAT* goDat = Character::GetObjectDataCharacterGameObject(obj);
	//if (goDat == nullptr)
	//{
	//	return;
	//}
	////
	////↓処理
	////

	//{
	//	DRAW_SPRITE_DAT dsdat = {};
	//	dsdat.x = obj->pos.x;
	//	dsdat.y = obj->pos.y;
	//	dsdat.w = obj->size.x;
	//	dsdat.h = obj->size.y;
	//	dsdat.rot = 0.0f;
	//	dsdat.color = { 1.0f, 1.0f, 1.0f, 0.75f };

	//	dsdat.texNo = goDat->texture.textureId;
	//	dsdat.loadTexType = LOADTEXTURETYPE_MAIN;

	//	dsdat.texUV = { 0.0f, 0.0f };
	//	dsdat.texWH = { 1.0f, 1.0f };

	//	dsdat.startPos = { 0.0f, 0.0f };
	//	dsdat.startRot = 0.0f;
	//	dsdat.startScale = { 1.0f, 1.0f };

	//	DrawSpriteInCamera(dsdat);
	//}

    if (m_EfBuff2_Frame >= 0)
    {
        //int f = (m_AnimCount_Buff.x * m_AnimCount_Buff.y)
        //    * ((float)m_EfBuff2_Frame / m_EfBuff2_FrameMax)
        //    ;
        //DrawCharaEff(CharaEffType::Buff2, m_EfBuff2_Size, m_EfBuff2_Pos, f);
        int f = m_EfBuff2_Frame;
        DrawCharaEff(CharaEffType::Buff, m_EfBuff2_Size, m_EfBuff2_Pos, f);
    }
}

// ===================================================
// Dead
// オブジェクトのHPが0になったとき
// ===================================================
void GOE_Character::Dead()
{
}

// ===================================================
// DamageAttacker
// オブジェクトの攻撃判定がヒットしたとき
// ===================================================
void GOE_Character::DamageAttacker(GAME_OBJECT_DAMAGE_RESULT dr)
{
	//DebugPrintf("ateta\n");
	//if (dr.colAtk.isGrab)
	//{
	//	//GAME_OBJECT* recOb = GetGameObject(dr.recObjIndex);
	//	//GameObjectMoveRequest(recOb,
	//	//	20,
	//	//	MkF3(recOb->pos.x, recOb->pos.y - 600.0f, recOb->pos.z),
	//	//	EasingInOutSine);
	//}
}

// ===================================================
// DamageReciever
// オブジェクトが
// 別のオブジェクトの攻撃判定にヒットしたとき
// ===================================================
void GOE_Character::DamageReciever(GAME_OBJECT_DAMAGE_RESULT dr)
{
	//キャラオブジェクトのデータを取得
	//Character::OBJECT_DAT* goDat = Character::GetObjectDataCharacterGameObject(obj);
	//if (goDat == nullptr)
	//{
	//	return;
	//}
	//
	//↓処理
	//

	//DebugPrintf("kuratta\n");

	//仮　くらい硬直
	if (dr.damage > 0)
	{
		//Character::PlayMotionCharacterGameObject(
		//	goDat,
		//	Character::GetCharacterMotionFindByName(goDat->data, "DAMAGE"),
		//	false,
		//	nullptr,
		//	dr.colAtk.hitStern
		//);
	}
}

// ===================================================
// BlockHit
// オブジェクトがブロックに触れたとき
// ===================================================
void GOE_Character::BlockHit(GAME_OBJECT_POSMOD_RESULT pmr)
{
}

void GOE_Character::GraphicSet(std::string texName, int splitX, int splitY, int cntmax)
{
    TEX_DAT texDat = LoadTexData(texName);
    GraphicSet(texDat, splitX, splitY, cntmax);
}

void GOE_Character::GraphicSet(const TEX_DAT& texDat, int splitX, int splitY, int cntmax)
{
    if (!texDat.enable)
    {
        return;
    }

    DPD_SHADER_OPT graShadeOpt = m_Obj->graphic.shaderOpt;
    Float3 graPos = m_Obj->graphic.graph.spriteAnim[0].pos3D;

    GAME_OBJECT_GRAPHIC goGra = {};
    InitGameObjectGraphic(&goGra);
    InitSpriteGraph(&goGra.graph);

    goGra.shadow = true;

    goGra.color = MkF4(1.0f, 1.0f, 1.0f, 1.0f);

    goGra.shaderOpt = graShadeOpt;
    //goGra.shaderOpt.bloom = DPD_SHADER_OPT_BLOOM_NORMAL;
    //goGra.shaderOpt.outline = DPD_SHADER_OPT_OUTLINE_NORMAL;
    ////goGra.shaderOpt.outline = DPD_SHADER_OPT_OUTLINE_WHITE;

    {

        SPRITE_GRAPH_DAT* sgdat = &goGra.graph;
        sgdat->outlineWidth = 20.0f;
        //sgdat->alpha = 1.0f;
        ;
        int imgW = texDat.width;
        int imgH = texDat.height;

        int imgXCnt = splitX;
        int imgYCnt = splitY;
        if (imgXCnt <= 0)
        {
            imgXCnt = 1;
        }
        if (imgYCnt <= 0)
        {
            imgYCnt = 1;
        }

        int img1FW = imgW / imgXCnt;
        int img1FH = imgH / imgYCnt;

        float mX = 1.0f;
        float mY = 1.0f;

        if (img1FW > img1FH)
        {
            mY = ((float)img1FH) / ((float)img1FW);
        }
        else
        {
            mX = ((float)img1FW) / ((float)img1FH);
        }

        Float2 sizetmp = MkF2(
            Character::DEFAULT_SIZE * mX,
            Character::DEFAULT_SIZE * mY
        );

        { //スプライトをセット
            SPRITE_ANIM_DAT* sadat = sgdat->spriteAnim + 0;
            sadat->isUse = true;
            sadat->texDat = texDat;
            //sadat->color = MkF4(1.0f, 1.0f, 1.0f, 1.0f);

            sadat->pos3D = graPos;
            sadat->rot = 0.0f;
            sadat->size = MkF2(sizetmp.x, sizetmp.y);

            sadat->no = 0;
            sadat->xcnt = splitX;
            sadat->hcnt = splitY;
            sadat->cntMax = (cntmax < 0 ? (splitX * splitY) : cntmax);
        }

        //goGra.rot3d.x = 50.0f;
        goGra.rot3d.x = 65.0f;
        goGra.pos3d.y = -sizetmp.y / 4;
        goGra.pos3d.z = -sizetmp.y / 2;
    }
    m_Obj->graphic = goGra;

    m_GraphPosDefault = m_Obj->graphic.pos3d;
}


void GOE_Character::GraphicSpriteSet(const TEX_DAT& texDat, int splitX, int splitY, int cntmax)
{
    if (!texDat.enable)
    {
        return;
    }

    {
        SPRITE_GRAPH_DAT* sgdat = &m_Obj->graphic.graph;
        { //スプライトをセット
            SPRITE_ANIM_DAT* sadat = sgdat->spriteAnim + 0;
            sadat->isUse = true;
            sadat->texDat = texDat;
            sadat->xcnt = splitX;
            sadat->hcnt = splitY;
            sadat->cntMax = (cntmax < 0 ? (splitX * splitY) : cntmax);
        }
    }
}

void GOE_Character::DrawCharaEff(CharaEffType type, Float3 size, Float3 pos, int frame)
{
    int frametmp = frame < 0 ? m_Obj->frameActive : frame;

    for (int i = 0; i < 2; i++)
    {
        DRAW_POLYGON_DAT drawDatTmp = {};
        drawDatTmp.size = MkF3(
            size.x / 6,
            size.y / 3,
            size.z / 6
        );
        //drawDatTmp.pos.y += m_Obj->size.y / 2;
        //drawDatTmp.pos.z += 20.0f;
        //drawDatTmp.pos.y += drawDatTmp.size.y / -2;
        //drawDatTmp.pos.y = drawDatTmp.size.y * -0.4f;
        drawDatTmp.pos.y = drawDatTmp.size.y * 1.2f;

        drawDatTmp.rot = 0.0f;

        drawDatTmp.color = MkF4(1.0f, 1.0f, 1.0f, 0.9f * m_MovieVisibleAlpha);


        drawDatTmp.texWH = {
            1.0f,
            1.0f
        };

        drawDatTmp.loadTexType = LOADTEXTURETYPE_MAIN;

        drawDatTmp.startPos3D = m_Obj->pos;

        drawDatTmp.startRot3D = MkF3(0.0, 0.0f, 0.0f);
        drawDatTmp.startRot3D.x = -90.0f;

        drawDatTmp.startScale = MkF2(1.0f, 1.0f);
        drawDatTmp.startScale3D = MkF3(1.0f, 1.0f, 1.0f);

        drawDatTmp.modelNo = m_Model_EfBuff;

        //drawDatTmp.shaderOpt.bloom = DPD_SHADER_OPT_BLOOM_NORMAL;
        drawDatTmp.shaderOpt.bloom = DPD_SHADER_OPT_BLOOM_NONE;

        DRAW_POLYGON_IN_CAMERA_EX_OPTION opt = {};
        opt.reverse = false;
        opt.fixed = false;
        opt.enableShade = true;
        opt.billboard = false;
        //opt.priority = DRAW_POLYGON_PRIORITY_NORMAL;
        //opt.priority = DRAW_POLYGON_PRIORITY_NML_u1;
        opt.priority = DRAW_POLYGON_PRIORITY_NML_d1;
        opt.depthBuffIgnore = true;


        if (i == 1)
        {
            drawDatTmp.size.x *= 1.1f;
            drawDatTmp.size.z *= 1.1f;
            drawDatTmp.texUV.x = 0.005f * frametmp;
            //drawDatTmp.texWH.y = 1.0f/ 2;
            //opt.priority = DRAW_POLYGON_PRIORITY_NML_d1;
            opt.priority = DRAW_POLYGON_PRIORITY_NORMAL;
            opt.depthBuffIgnore = false;
        }

        if (type == CharaEffType::Heal)
        {
            if (i == 1)
            {
                drawDatTmp.color = MkF4(0.5f, 2.0f, 0.5f, 1.0f);
                //drawDatTmp.texUV;
                drawDatTmp.texNo = m_Tex_EfHeal_Flow;
            }
            else
            {
                drawDatTmp.color = MkF4(0.9f, 2.0f, 0.9f, 1.0f);
                //drawDatTmp.texUV;
                drawDatTmp.texNo = m_Tex_EfHeal_Flare;
            }
        }
        else
        {
            if (i == 1)
            {
                drawDatTmp.color = MkF4(1.0f, 0.8f, 0.1f, 0.9f);
                drawDatTmp.texNo = m_Tex_EfBuff_Flow;
            }
            else
            {
                drawDatTmp.color = MkF4(1.0f, 0.4f, 0.0f, 1.0f);
                //drawDatTmp.texUV;
                drawDatTmp.texNo = m_Tex_EfBuff_Flare;
            }
        }

        drawDatTmp.pos.x += pos.x;
        drawDatTmp.pos.y += pos.y;
        drawDatTmp.pos.z += pos.z;

        {
            //DRAW_POLYGON_DAT drawDatTmp = {};
            //drawDatTmp.size = MkF3(size.x, size.y, 1.0f);
            ////drawDatTmp.pos.y += m_Obj->size.y / 2;
            ////drawDatTmp.pos.z += 20.0f;
            ////drawDatTmp.pos.y += drawDatTmp.size.y / -2;
            //drawDatTmp.pos.y = drawDatTmp.size.y * -0.4f;

            //drawDatTmp.rot = 0.0f;

            //drawDatTmp.rot3D = MkF3(0.0f, 0.0f, 0.0f);

            //drawDatTmp.color = MkF4(1.0f, 1.0f, 1.0f, 0.9f);


            //drawDatTmp.texWH = {
            //    1.0f / m_AnimCount_Buff.x,
            //    1.0f / m_AnimCount_Buff.y
            //};

            //drawDatTmp.loadTexType = LOADTEXTURETYPE_MAIN;

            //drawDatTmp.startPos3D = m_Obj->pos;
            ////drawDatTmp.startPos3D.x += m_Obj->graphic.pos3d.x;
            ////drawDatTmp.startPos3D.y += m_Obj->graphic.pos3d.y - 2.0f;
            ////drawDatTmp.startPos3D.z += m_Obj->graphic.pos3d.z + 2.0f;
            //drawDatTmp.startPos3D.y += -2.0f;
            //drawDatTmp.startPos3D.z += +2.0f;

            //drawDatTmp.startRot3D = MkF3(m_Obj->graphic.rot3d.x, 0.0f, 0.0f);

            //drawDatTmp.startScale = MkF2(1.0f, 1.0f);
            //drawDatTmp.startScale3D = MkF3(1.0f, 1.0f, 1.0f);

            //drawDatTmp.modelNo = 0;

            ////drawDatTmp.shaderOpt.bloom = DPD_SHADER_OPT_BLOOM_NORMAL;
            //drawDatTmp.shaderOpt.bloom = DPD_SHADER_OPT_BLOOM_NONE;

            //DRAW_POLYGON_IN_CAMERA_EX_OPTION opt = {};
            //opt.reverse = false;
            //opt.fixed = false;
            //opt.enableShade = true;
            //opt.billboard = false;
            ////opt.priority = DRAW_POLYGON_PRIORITY_NORMAL;
            //opt.priority = DRAW_POLYGON_PRIORITY_NML_u1;
            //opt.depthBuffIgnore = true;


            //if (type == CharaEffType::Heal)
            //{
            //    drawDatTmp.texUV = AnimationUV(
            //        1 + (int)(frametmp) % (m_AnimCount_Buff.x * m_AnimCount_Buff.y - 2),
            //        m_AnimCount_Buff.x,
            //        m_AnimCount_Buff.y
            //    );
            //    drawDatTmp.texNo = m_Tex_EfHeal;
            //}
            //else if (type == CharaEffType::DeBuff)
            //{
            //    drawDatTmp.texUV = AnimationUV(
            //        2 + (int)(frametmp) % (m_AnimCount_Buff.x * m_AnimCount_Buff.y - 2),
            //        m_AnimCount_Buff.x,
            //        m_AnimCount_Buff.y
            //    );
            //    drawDatTmp.texNo = m_Tex_EfDeBuff;
            //}
            //else if (type == CharaEffType::Buff2)
            //{
            //    drawDatTmp.pos.y = drawDatTmp.size.y * -0.3f;
            //    drawDatTmp.texUV = AnimationUV(
            //        frametmp,
            //        m_AnimCount_Buff.x,
            //        m_AnimCount_Buff.y
            //    );
            //    drawDatTmp.texNo = m_Tex_EfBuff2;
            //}
            //else
            //{
            //    drawDatTmp.texUV = AnimationUV(
            //        frametmp,
            //        m_AnimCount_Buff.x,
            //        m_AnimCount_Buff.y
            //    );
            //    drawDatTmp.texNo = m_Tex_EfBuff;
            //}

            //drawDatTmp.pos.x += pos.x;
            //drawDatTmp.pos.y += pos.y;
            //drawDatTmp.pos.z += pos.z;
        }


        DrawPolygonInCameraEx(drawDatTmp, opt);
    }
}

void GOE_Character::StartCharaEffBuff(int frame, Float3 size, Float3 pos)
{
    m_EfBuff2_Frame = 0;
    m_EfBuff2_FrameMax = frame;
    m_EfBuff2_Pos = pos;
    m_EfBuff2_Size = size;
}

void GOE_Character::SetBeforeVec()
{
    SetBeforeVec(&m_BeforeVec, &m_BeforeVecBuf, 0.055f);
}
void GOE_Character::SetBeforeVec(
    Float3* bbec,
    Float3* bbecBuf,
    float speed
)
{
    float bufdeg = CalculateRadToDeg(atan2(bbecBuf->x, bbecBuf->y));
    float deg = CalculateRadToDeg(atan2(bbec->x, bbec->y));

    //if (((bufdeg >= -90 && bufdeg < 0) && (deg >= 0 && deg <= 90)))
    //{
    //    bufdeg += 360.0f;
    //}
    //else if ((deg >= -90 && deg < 0) && (bufdeg >= 0 && bufdeg <= 90))
    //{
    //    deg += 360.0f;
    //}

    float deg2 = DegNormalize(deg);

    float bufdeg2 = DegNormalize(bufdeg);

    float subdeg = DegNormalize(bufdeg2 - deg2);

    //{
    //    float subdeg2 = 360.0f - abs(subdeg);
    //    if (subdeg > subdeg2)
    //    {
    //        subdeg = subdeg2;
    //    }
    //}
    ////{
    ////    float subdeg2 = 360.0f - subdeg;
    ////    while (subdeg2 < 0.0f)
    ////    {
    ////        subdeg2 += 360.0f;
    ////    }
    ////    while (subdeg2 > 360.0f)
    ////    {
    ////        subdeg2 += -360.0f;
    ////    }

    ////    if (subdeg > subdeg2)
    ////    {
    ////        subdeg = subdeg2;
    ////    }
    ////}

    ////if (deg2 >= 180.0f && subdeg <= 180.0f)
    ////if (((deg2 >= 180.0f && deg2 < 360.0f) || deg2 >= 270.0f) && subdeg <= 180.0f)
    ////else if ((bufdeg2 - deg2) >= 0 && deg2 < 180.0f)
    ////{
    ////    deg2 += +360.0f;
    ////}

    //float subdeg2 = abs(bufdeg2 - deg2);
    //float sub = subdeg2 * -0.1f;
    //if (subdeg < 180)
    //{
    //    sub *= -1.0f;
    //}
    ////float nrad = deg + sub * 0.1f;
    //float nrad = deg + sub;

    ////DebugPrintf("%f\n", bufdeg);

    //m_BeforeVec.x = sin(CalculateDegToRad(nrad));
    //m_BeforeVec.y = cos(CalculateDegToRad(nrad));

    ////m_BeforeVec.x += (m_BeforeVec.x - m_BeforeVecBuf.x) * -0.055f;
    ////m_BeforeVec.y += (m_BeforeVec.y - m_BeforeVecBuf.y) * -0.055f;
    //m_BeforeVec.z += (m_BeforeVec.z - m_BeforeVecBuf.z) * -0.055f;
    //DebugPrintf("%f\n", subdeg);
    if ((subdeg) >= 170.0f && (subdeg) <= 190.0f)
    {
        float d = bufdeg - 90.0f; 
        bbec->x += sin(CalculateDegToRad(d));
        bbec->y += cos(CalculateDegToRad(d));
    }
    bbec->x += (bbec->x - bbecBuf->x) * -speed;
    bbec->y += (bbec->y - bbecBuf->y) * -speed;
    bbec->z += (bbec->z - bbecBuf->z) * -speed;

    //Float3 forward = m_BeforeVecBuf;
    //Quaternion rot = Quaternion::LookRotation(forward);

    //rot = Quaternion::slerp(Quaternion::LookRotation(m_BeforeVec), rot, 0.1);
    //m_BeforeVec = Quaternion::rotateVector(rot, { 0, 0, 1 });
}
