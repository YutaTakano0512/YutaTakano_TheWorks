// ===================================================
// GOE_Character_Player.cpp キャラクターオブジェクト
// 
// hara sougo  2024/11/25
// ===================================================
#define NO_CONTROLLER

#include "../../../Game/Configuration/Param.h"
#include "../../../game_main.h"
#include "../../Sprite/sprite.h"
#include "../../Texture/texture.h"
#include "../../Polygon/modelData.h"
#include "../../../Game/Entities/Block/block.h"
#include "../../Systems/collision.h"
#include "../../Systems/easing.h"

#include "../../Managers/gameManager.h"
#include "../../Managers/scene.h"
#include "../../../Scenes/SCENE_Game.h"
#include "../../Game Stage/Stage_Base.h"

#include "../gameObject.h"
#include "../gameObjectPrefab.h"

#include "../../Systems/camera.h"
#include "../../Systems/ui.h"
#include "../../Systems/ui_Game.h"

#include "../../Entities/player.h"


#include "../../Managers/soundManager.h"

#include "../../Game Events/GE_Gaya.h"

#include "GOE_Effect.h"
#include "GOE_Character.h"

#include "GOE_Character_Player.h"
#include "GOE_ObjectDynamic.h"

// ===================================================
// マクロ定義
// ===================================================

//#define	PLAYER_MOVE_AXCEL					(9.0f)	//加速度
//#define	PLAYER_MOVE_SPEED_MAX				(25.0f)	//最高速
//#define	PLAYER_MOVE_AXCEL					(18.0f)	//加速度
#define	PLAYER_MOVE_AXCEL					(22.0f)	//加速度
#define	PLAYER_MOVE_SPEED_MAX				(50.0f)	//最高速

#define	PLAYER_JUMP_FRAME_MIN				(4)	//ジャンプ時間最小
#define	PLAYER_JUMP_FRAME_MAX				(25)	//ジャンプ時間最大

//#define	PLAYER_JUMP_FORCE					(34.0f)	//ジャンプ力
#define	PLAYER_JUMP_FORCE					(28.0f)	//ジャンプ力
//#define	PLAYER_JUMP_FRICTION				(1.2f)	//ジャンプ摩擦力
#define	PLAYER_JUMP_FRICTION				(0.8f)	//ジャンプ摩擦力

//#define	PLAYER_GRAVITY						(0.6f)	//重力加速度
//#define	PLAYER_GRAVITY						(0.42f)	//重力加速度
//#define	PLAYER_GRAVITY						(0.25)	//重力加速度

// ===================================================
// プロトタイプ宣言
// ===================================================

// ===================================================
// グローバル変数
// ===================================================

//Float2 GOE_Character_Player::GetKakoponVertexNum()
//{
//	int n = 0;
//	for (int i = 0; i < KAKOPON_VERTEX_MAX; i++)
//	{
//		if (g_LineDraw.p_vertex[i].enable)
//		{
//			n++;
//		}
//	}
//	return { (float)n, P_VERTEX_MAX };
//}

// ===================================================
// Init
// オブジェクトが生成されたとき
// ===================================================
void GOE_Character_Player::Init()
{
	SuperGOE::Init(); //初期化

    //必要なテクスチャを読み込み
    m_TexId_AttackEffect_s = LoadTexData("TEX/effect/tex_sl_back").textureId;
    m_TexId_kakoponEffect_Aura = LoadTexData("TEX/effect/kakopon/tex_aura_01").textureId;
    m_TexId_kakoponEffect_Add = LoadTexData("TEX/effect/kakopon/tex_add_01").textureId;
    ///m_TexId_kakoponEffect_Trail = LoadTexData("TEX/effect/kakopon/tex_trail_01").textureId;

    //m_TexId_kakoponEffect_Trail = LoadTexData("TEX/effect/kakopon/ink_tex").textureId;
    m_TexId_kakoponEffect_Trail_Kai[0] = LoadTexData("TEX/effect/kakopon/ink/KaiInk_01").textureId;
    m_TexId_kakoponEffect_Trail_Kai[1] = LoadTexData("TEX/effect/kakopon/ink/KaiInk_02").textureId;
    m_TexId_kakoponEffect_Trail_Kai[2] = LoadTexData("TEX/effect/kakopon/ink/KaiInk_03").textureId;
    m_TexId_kakoponEffect_Trail_Kai[3] = LoadTexData("TEX/effect/kakopon/ink/KaiInk_04").textureId;

    m_TexId_kakoponEffect_Trail_Neo[0] = LoadTexData("TEX/effect/kakopon/ink/NeoInk_01").textureId;
    m_TexId_kakoponEffect_Trail_Neo[1] = LoadTexData("TEX/effect/kakopon/ink/NeoInk_02").textureId;
    m_TexId_kakoponEffect_Trail_Neo[2] = LoadTexData("TEX/effect/kakopon/ink/NeoInk_03").textureId;
    m_TexId_kakoponEffect_Trail_Neo[3] = LoadTexData("TEX/effect/kakopon/ink/NeoInk_04").textureId;

    m_TexId_kakoponEffect_Trail_Kn[0] = LoadTexData("TEX/effect/kakopon/ink/KonInk_01").textureId;
    m_TexId_kakoponEffect_Trail_Kn[1] = LoadTexData("TEX/effect/kakopon/ink/KonInk_02").textureId;
    m_TexId_kakoponEffect_Trail_Kn[2] = LoadTexData("TEX/effect/kakopon/ink/KonInk_03").textureId;
    m_TexId_kakoponEffect_Trail_Kn[3] = LoadTexData("TEX/effect/kakopon/ink/KonInk_04").textureId;
    m_TexId_kakoponEffect_Add_Kn = LoadTexData("TEX/effect/kakopon/tex_add_01").textureId;


    m_TexId_kakoponEffect_Ground = LoadTexData("TEX/effect/barrier/tex_br_noise").textureId;
    //m_TexId_kakoponEffect_Ground = LoadTexData("TEX/effect/kakopon/kp_cos").textureId;

    //m_TexId_kakoponEffect_General[0] = LoadTexData("TEX/effect/kakopon/general/kp_tex_01").textureId;
    //m_TexId_kakoponEffect_General[1] = LoadTexData("TEX/effect/kakopon/general/kp_tex_02").textureId;
    //m_TexId_kakoponEffect_General[2] = LoadTexData("TEX/effect/kakopon/general/kp_tile_01").textureId;
    //m_TexId_kakoponEffect_General[3] = LoadTexData("TEX/effect/kakopon/general/kp_tile_02").textureId;
    //m_TexId_kakoponEffect_General[4] = LoadTexData("TEX/effect/kakopon/general/kp_tile_03").textureId;
    //m_TexId_kakoponEffect_General[0] = LoadTexData("TEX/effect/kakopon/general/kp_flare").textureId;
    //m_TexId_kakoponEffect_General[1] = LoadTexData("TEX/effect/kakopon/general/kp_star_02").textureId;
    m_TexId_kakoponEffect_General[0] = LoadTexData("TEX/effect/kakopon/general/kp_blue").textureId;
    m_TexId_kakoponEffect_General[1] = LoadTexData("TEX/effect/kakopon/general/test20").textureId;
    m_TexId_kakoponEffect_General2[0] = LoadTexData("TEX/effect/kakopon/general/kp_pink").textureId;
    m_TexId_kakoponEffect_General2[1] = LoadTexData("TEX/effect/kakopon/general/test20").textureId;
    m_TexId_kakoponEffect_Kn[0] = LoadTexData("TEX/effect/barrier/tex_br_noise").textureId;
    m_TexId_kakoponEffect_Kn[1] = LoadTexData("TEX/effect/barrier/tex_br_wave").textureId;

    m_TexId_kakoponEffect_Heal[0] = LoadTexData("TEX/effect/kakopon/heal/tex_tile_heal_01").textureId;
    m_TexId_kakoponEffect_Heal[1] = LoadTexData("TEX/effect/kakopon/heal/tex_tile_heal_02").textureId;
    m_TexId_kakoponEffect_Buff[0] = LoadTexData("TEX/effect/kakopon/buff/tex_tile_buff_01").textureId;
    m_TexId_kakoponEffect_Buff[1] = LoadTexData("TEX/effect/kakopon/buff/tex_tile_buff_02").textureId;
    m_TexId_kakoponEffect_DeBuff[0] = LoadTexData("TEX/effect/kakopon/debuff/tex_tile_debuff_01").textureId;
    m_TexId_kakoponEffect_DeBuff[1] = LoadTexData("TEX/effect/kakopon/debuff/tex_tile_debuff_02").textureId;


    m_Tex_DamageEffect = LoadTexData("TEX/effect/eff_damage");
    m_Tex_ScKakoEffect = LoadTexData("TEX/effect/eff_gamen02");

    m_Model_StanStar = GetModelDataIndex("MODEL/effect/stanstar");

    //m_TexId_kakoponEffect_Add_Heal = LoadTexData("TEX/effect/kakopon/tex_tile_heal").textureId;
    //m_TexId_kakoponEffect_Add_Buff = LoadTexData("TEX/effect/kakopon/tex_tile_buff").textureId;
    //m_TexId_kakoponEffect_Add_DeBuff = LoadTexData("TEX/effect/kakopon/tex_tile_debuff").textureId;

	//ここでプレイヤーデータの取得はできないので
	// やりたい場合は FirstFrame で
	
	//PLAYER* player = GetPlayer(obj);
	//if (player == nullptr)
	//{
	//	return;
	//}
	////
	////↓処理
	////

    m_Obj->family[GAME_OBJECT_FAMILY_PLAYER] = true;

    m_Obj->size.x = ONE_BLOCK * 4;
    m_Obj->size.y = ONE_BLOCK * 3;
    {
        GAME_OBJECT_COLLISION_RECIEVE_CHILD* col = m_Obj->collision.rec.collision + 0;
        col->size = MkF3(ONE_BLOCK * 1.0f, ONE_BLOCK * 1.0f, ONE_BLOCK * 3.0f);
    }

    m_InkGageMax = KAKOPON_INK_GAGE_MAX;
    m_InkGage = m_InkGageMax;
}

// ===================================================
// FirstFrame
// 最初のフレームのみ実行
// ===================================================
void GOE_Character_Player::FirstFrame()
{
	SuperGOE::FirstFrame();

	PLAYER* player = GetPlayer(m_Obj);
	if (player != nullptr)
	{
        //
        //↓処理
        //

        { //HP表示
            //ShowHpbar(player->index, true);
            ShowHpbar_Game(player->index, true);
        }

	}
    //最初のフレーム(初期化とか)

    //m_BeforeVecBuf = { 0.0f, 1.0f, 0.0f };

	//初期アニメーションを再生
	Character::PlayMotionCharacterGameObject(
		m_CharaData,
		Character::GetCharacterMotionFindByName(Character::GetCharacterMainData(m_CharaData->dataIndex), "IDLE"),
		false,
        nullptr,
        0
	);

}
void GOE_Character_Player::HpBarReset()
{
    PLAYER* player = GetPlayer(m_Obj);
    if (player != nullptr)
    {
        //
        //↓処理
        //

        { //HP表示
            //ShowHpbar(player->index, true);
            ShowHpbar_Game(player->index, true);
        }

    }
}

// ===================================================
// Uninit
// オブジェクトが削除されるとき
// ===================================================
void GOE_Character_Player::Uninit()
{
	//
	//↓処理
	//
    UnloadTexData(m_TexId_AttackEffect_s);
    UnloadTexData(m_TexId_kakoponEffect_Aura);
    UnloadTexData(m_TexId_kakoponEffect_Add);
    //UnloadTexData(m_TexId_kakoponEffect_Trail);
    UnloadTexData(m_TexId_kakoponEffect_Trail_Kai[0]);
    UnloadTexData(m_TexId_kakoponEffect_Trail_Kai[1]);
    UnloadTexData(m_TexId_kakoponEffect_Trail_Kai[2]);
    UnloadTexData(m_TexId_kakoponEffect_Trail_Kai[3]);

    UnloadTexData(m_TexId_kakoponEffect_Trail_Neo[0]);
    UnloadTexData(m_TexId_kakoponEffect_Trail_Neo[1]);
    UnloadTexData(m_TexId_kakoponEffect_Trail_Neo[2]);
    UnloadTexData(m_TexId_kakoponEffect_Trail_Neo[3]);

    UnloadTexData(m_TexId_kakoponEffect_Trail_Kn[0]);
    UnloadTexData(m_TexId_kakoponEffect_Trail_Kn[1]);
    UnloadTexData(m_TexId_kakoponEffect_Trail_Kn[2]);
    UnloadTexData(m_TexId_kakoponEffect_Trail_Kn[3]);
    UnloadTexData(m_TexId_kakoponEffect_Add_Kn);
    

    //UnloadTexData(m_TexId_kakoponEffect_Add_Heal);
    //UnloadTexData(m_TexId_kakoponEffect_Add_Buff);
    //UnloadTexData(m_TexId_kakoponEffect_Add_DeBuff);

    UnloadTexData(m_TexId_kakoponEffect_General[0]);
    UnloadTexData(m_TexId_kakoponEffect_General[1]);
    UnloadTexData(m_TexId_kakoponEffect_General2[0]);
    UnloadTexData(m_TexId_kakoponEffect_General2[1]);
    UnloadTexData(m_TexId_kakoponEffect_Ground);

    UnloadTexData(m_TexId_kakoponEffect_Heal[0]);
    UnloadTexData(m_TexId_kakoponEffect_Heal[1]);
    UnloadTexData(m_TexId_kakoponEffect_Buff[0]);
    UnloadTexData(m_TexId_kakoponEffect_Buff[1]);
    UnloadTexData(m_TexId_kakoponEffect_DeBuff[0]);
    UnloadTexData(m_TexId_kakoponEffect_DeBuff[1]);

    UnloadTexData(m_TexIconMain_N);
    UnloadTexData(m_TexIconMain_Pinch);
    UnloadTexData(m_TexIconMain_Out);
    UnloadTexData(m_TexIconSub);
    UnloadTexData(m_TexIconSkillOn);
    UnloadTexData(m_TexIconSkillOff);

    UnloadTexData(m_Tex_DamageEffect);
    UnloadTexData(m_Tex_ScKakoEffect);
    
	//終了処理なのでこいつは最後に
	SuperGOE::Uninit(); //終了処理

	//CharaDat.charaData->privateVariable は
	// GOE_UninitCharacterObject の関数内で解放されるので
	//ここで解放しなくてもいいよ
}

// ===================================================
// Update
// 毎フレーム実行
// ===================================================
void GOE_Character_Player::Update()
{
	SuperGOE::Update();

    PLAYER* player = GetPlayer(m_Obj);
    if (player != nullptr)
    {
        //
        //↓処理
        //
        
        //アウトラインの色
        //if (GetActivePlayerNum() <= 1)
        //{ //一人の時は主張の強いアウトラインにはしない
        //    m_Obj->graphic.shaderOpt.outline = DPD_SHADER_OPT_OUTLINE_NORMAL;
        //}
        //else
        //{
        //    m_Obj->graphic.shaderOpt.outline = (DPD_SHADER_OPT_OUTLINE)(DPD_SHADER_OPT_OUTLINE_RED + player->index);
        //}

        m_Obj->graphic.shaderOpt.outline = DPD_SHADER_OPT_OUTLINE_NORMAL;

        //return;
    }

    //描いている最中か
    //m_IsDrawing = m_KakoponLineDraw_Main.isLineDrawing || m_KakoponLineDraw_SubTest.isLineDrawing;
    m_IsDrawing = false;
    for (int i = 0; i < KAKOPON_DAT_NUM; i++)
    {
        if (m_KakoDatList[i].lineDraw.isLineDrawing)
        {
            m_IsDrawing = true;
            break;
        }
    }
    //m_IsDrawing = m_KakoponLineDraw.isLineDrawing;

    m_IsIdling = (m_CharaData->playingMotion.motion->name).find("IDLE") == 0;
    m_IsWalkAnim = 
        (m_CharaData->playingMotion.motion->name).find("WALK") == 0
        || (m_CharaData->playingMotion.motion->name).find("DRAW") == 0;
    m_IsNormalAttack = (m_CharaData->playingMotion.motion->name).find("ATTACK") == 0;

    m_IsWalking = m_WalkFrame > 0;

#if defined(_DEBUG) || defined(DEBUG)

    //デバッグ エネミー全ダメージ
    if (GetAsyncKeyState(VK_SHIFT))
    {
        for (int i = 0; i < GAME_OBJECT_MAX; i++)
        {
            GAME_OBJECT* ob = GetGameObject(i);
            if (IsActiveGameObject(ob)
                && ob->status.statusType != GAME_OBJECT_STATUS_TYPE_VOID
                && ob->type == GAME_OBJECT_TYPE_CHARACTER
                && (IsBelongsFamilyGameObject(ob, GAME_OBJECT_FAMILY_ENEMY)
                    || IsBelongsFamilyGameObject(ob, GAME_OBJECT_FAMILY_ENEMY_ATTACK)
                   )
            )
            {
                if (Keyboard_IsKeyTrigger(KK_L))
                {
                    ob->status.hp += -100;
                }
                else if(Keyboard_IsKeyTrigger(KK_K))
                {
                    if (ob->status.statusType == GAME_OBJECT_STATUS_TYPE_ENEMY_BOSS)
                    {
                        ob->status.hp = ob->status.hpMin;
                    }
                    else
                    {
                        DeleteGameObject(ob);
                    }
                }
            }
        }
        if (Keyboard_IsKeyTrigger(KK_O))
        {
            m_Obj->status.hp += -100;
        }
    }
#endif
}

// ===================================================
// UpdateActive
// 毎フレーム実行
// ただし、ヒットストップ中などの
// 非アクティブ時は実行されない
// ===================================================
void GOE_Character_Player::UpdateActive()
{
	SuperGOE::UpdateActive();

	PLAYER* player = GetPlayer(m_Obj);
	if (player == nullptr)
	{
		return;
	}
	//
	//↓処理
	//

    m_MoveInputVectorBefore = m_MoveInputVectorCurrent;
    m_MoveInputVectorCurrent = {};
    {
        Float2 ls = Character::GetCtrlStickLeft(m_CharaData);
        if (ls.x != 0 || ls.y != 0)
        { //スティック移動
            //obj->vel.x += PLAYER_MOVE_AXCEL * ls.x;
            m_MoveInputVectorCurrent = MkF3(ls.x, -ls.y, 0.0f);

            //if (ls.x < 0)
            //{
            //	CharaDat.charaData->reverse = true;
            //}
            //else
            //{
            //	CharaDat.charaData->reverse = false;
            //}

            //if (m_Obj->frameActive % 60 == 0)
            //{
            //	SoundEffectPlayRequest(SOUND_TYPE_SE_1, 2.5f, 0.0f);
            //}
        }
        else
        { //十字移動

            Float2 moveVelJuuji = {};

            if (Character::GetCtrlPress(m_CharaData, CONTROLLER_BUTTON_TYPE_LEFT))
            {
                //if (m_Obj->frameActive % 60 == 0)
                //{
                //	SoundEffectPlayRequest(SOUND_TYPE_SE_1, 3.5f, 0.0f);
                //}

                //CharaDat.charaData->reverse = true;
                moveVelJuuji.x = -1.0f;
            }
            if (Character::GetCtrlPress(m_CharaData, CONTROLLER_BUTTON_TYPE_RIGHT))
            {
                //if (m_Obj->frameActive % 60 == 0)
                //{
                //	SoundEffectPlayRequest(SOUND_TYPE_SE_1, 3.5f, 0.0f);
                //}

                //CharaDat.charaData->reverse = false;
                moveVelJuuji.x = 1.0f;
            }
            if (Character::GetCtrlPress(m_CharaData, CONTROLLER_BUTTON_TYPE_UP))
            {
                //if (m_Obj->frameActive % 60 == 0)
                //{
                //	SoundEffectPlayRequest(SOUND_TYPE_SE_1, 3.5f, 0.0f);
                //}

                moveVelJuuji.y = -1.0f;
            }
            if (Character::GetCtrlPress(m_CharaData, CONTROLLER_BUTTON_TYPE_DOWN))
            {
                //if (m_Obj->frameActive % 60 == 0)
                //{
                //	SoundEffectPlayRequest(SOUND_TYPE_SE_1, 3.5f, 0.0f);
                //}

                moveVelJuuji.y = 1.0f;
            }

            m_MoveInputVectorCurrent = MkF3(moveVelJuuji.x, moveVelJuuji.y, 0.0f);
        }

        m_MoveInputVectorCurrent = NormalizeVector(m_MoveInputVectorCurrent);
    }

    m_DamageSternAnimInLoopFlag = false;
    {
        if (m_Obj->status.deadFlag)
        {
            //やられモーションのコマ数
            m_DefeatFrameAnim = std::min(m_DefeatFrame, m_DefeatFrameAnimMax - 1);
        }
        else if (m_DamageSternFrame >= 0)
        {
            if (m_DamageSternFrame == 0)
            {
                m_DamageSternAnimInLoopFlag = true;
            }
            constexpr int sternupf = 18;
            //int spl = (m_DamageSternFrameMax - m_DamageSternFrameMax * 0.7)
            //    / m_DefeatFrameAnimMax;
            int spl = sternupf / m_DefeatFrameAnimMax;
            if (m_DamageSternFrame > m_DamageSternFrameMax - sternupf + spl * 2)
            {
                if (m_DefeatFrameAnim > 1)
                {
                    if (m_DamageSternFrame % std::max(spl, 1) == 0)
                    {
                        m_DefeatFrameAnim--;
                    }
                }
            }
            else
            {
                //やられモーションのコマ数
                m_DefeatFrameAnim = std::min(m_DamageSternFrame, m_DefeatFrameAnimMax - 1);
            }

            //煙
            if (m_Obj->moveRequ.enable && m_DamageSternFrame % 5 == 0)
            {
                {
                    Float3 efPos2 = m_Obj->pos;
                    efPos2.z += BLOCK_HIGHT * -1.5f;

                    GAME_OBJECT* go = GO_CreateEffect(GO_EFFECT_TYPE_SMOKE, efPos2);
                    if (go != nullptr)
                    {
                        //go->graphic.rot3d.x = 90.0f;
                        go->graphic.color.w = 1.0f;
                        go->scale = MkF3(
                            1.8f,
                            1.8f,
                            1.8f
                        );
                        GameObjectSetLifeTime(go, 20);
                    }
                }
            }

            if (m_DamageSternEffectFlag)
            {
                //レバガチャ
                if (
                    CalculateDistance({}, m_MoveInputVectorCurrent) > 0.4f
                    && (
                        m_MoveInputVectorCurrent.x != m_MoveInputVectorBefore.x
                        || m_MoveInputVectorCurrent.y != m_MoveInputVectorBefore.y
                        || m_MoveInputVectorCurrent.z != m_MoveInputVectorBefore.z
                    )
                )
                {
                    m_DamageSternFrame += (int)std::max(CalculateDistance(
                        m_MoveInputVectorCurrent,
                        m_MoveInputVectorBefore
                    ) * 10.0f, 1.0f);
                }
            }

            m_DamageSternFrame++;
            if (m_DamageSternFrame > m_DamageSternFrameMax)
            {
                m_DamageSternFrame = -1;
            }
        }
        else
        {
            if (m_DefeatFrameAnim > 0)
            {
                m_DefeatFrameAnim--;
                if (m_DefeatFrameAnim <= 0)
                {
                    m_CharaData->playingMotion.enable = false;
                }
            }
        }
    }

    if (m_Obj->status.deadFlag)
    {
        m_Obj->status.invFrame = 2;

        m_DefeatFrame++;

        if (!m_GameoverDecision)
        {
            if (
                m_DefeatFrame > (FRAME_ONE_SECOND * 4
                    + FRAME_ONE_SECOND * 4 * m_DeadNum)
                )
            { //復活処理
                BecomeRevive();
            }
        }

        return;
    }
    else
    {
        if (m_DefeatFrame > 0)
        {
            m_Obj->status.invFrame = DAMAGE_INV_FRAME;

            m_DefeatFrame = 0;
        }
    }

    Float3 moveVel = {};

	{
		//入力処理　適当に

		{
            bool isNu = !m_CharaData->playingMotion.enable || m_IsIdling || m_IsWalkAnim;

			//移動
			{
                if (m_MoveRequWalfFrame > 0)
                {
                   moveVel.x =  m_Obj->vel.x;
                   moveVel.y =  m_Obj->vel.y;

                   m_MoveRequWalfFrame--;
                }
                else
                {
                    //待機状態なら操作を受け付ける
                    if (!IsDefeat() //やられていない
                        && isNu)
                    {
                        moveVel = m_MoveInputVectorCurrent;
                        moveVel.x *= PLAYER_MOVE_AXCEL * m_MoveSpeedMod;
                        moveVel.y *= PLAYER_MOVE_AXCEL * m_MoveSpeedMod;
                        if (m_IsDrawing)
                        {
                            moveVel.x *= m_MoveSpeedModKako;
                            moveVel.y *= m_MoveSpeedModKako;
                        }

                        {
                            float buffspd = GetBuffSpeedMod(); //バフ効果
                            moveVel.x *= buffspd;
                            moveVel.y *= buffspd;
                        }


                        m_Obj->vel.x += moveVel.x;
                        m_Obj->vel.y += moveVel.y;
                    }
                }

			}

		}

	}

    if (moveVel.x != 0 || moveVel.y != 0)
    {
        //m_BeforeVec = NormalizeVector(moveVel);
        m_BeforeVecBuf = NormalizeVector(moveVel);

        m_WalkFrame++;

        if (m_WalkFrame > 10)
        {
            SetFlagTtrlMove();
        }
    }
    else
    {
        m_WalkFrame = 0;
    }

    //強制移動中は向きも強制的にセット
    if (m_Obj->moveRequ.frame > 0)
    {
        m_BeforeVecBuf = NormalizeVector(m_Obj->vel);
    }
    SetBeforeVec();

    if (m_HealingFrame > 0)
    {
        m_HealingFrame--;
    }
    if (m_BuffAttackFrame > 0)
    {
        m_BuffAttackFrame--;
    }
    if (m_BuffSpeedFrame > 0)
    {
        m_BuffSpeedFrame--;
    }

    if (m_CameraMoveActionFlag > 0)
    {
        m_CameraMoveActionFlag--;
    }
	//if (GetAsyncKeyState('P'))
	//{
	//	obj->pos.z -= 8.0f;
	//}

	//if (GetAsyncKeyState('L'))
	//{
	//	obj->pos.z += 8.0f;
	//}

    if (!IsPinch())
    {
        if (m_PinchFlag)
        {
            m_PinchFlag = false;
        }
    }

    if (m_DamageFrame >= 0)
    {
        m_DamageFrame++;
    }

    if (m_EffectQuestionCooldown > 0)
    {
        m_EffectQuestionCooldown--;
    }
}

void GOE_Character_Player::UpdateActiveLate()
{
    become_stern_motion();
}

// ===================================================
// Draw
// オブジェクトの描画イベント
// ===================================================
void GOE_Character_Player::Draw()
{
	SuperGOE::Draw();

    PLAYER* player = GetPlayer(m_Obj);
    if (player == nullptr)
    {
        return;
    }
    //
    //↓処理
    //

    if (GameManagerIsMovieFlag() && GameManagerIsMoviePlayerHiddenFlag())
    {
        if (m_movieVisibleFrame > 0)
        {
            m_movieVisibleFrame--;
        }
    }
    else
    {
        if (m_movieVisibleFrame < movieVisibleFrameMax)
        {
            m_movieVisibleFrame++;
        }
    }
    m_MovieVisibleAlpha = (float)m_movieVisibleFrame / movieVisibleFrameMax;

    //影
    m_Obj->graphic.shadowColor.w = 0.4f * m_MovieVisibleAlpha;

    if (m_Obj->status.invFrame > 1)
    {
        //float amod = (sin((float)m_Obj->frameActive / 2) + 1.0f) / 2 * -0.3f;
        //m_Obj->graphic.color = MkF4(
        //    1.0f,
        //    1.0f,
        //    1.0f,
        //    1.0f + amod
        //);
        float amod = 1.0f;
        if ((m_Obj->status.invFrame) % 4 < 2)
        {
            amod = 0.7f;
        }
        m_Obj->graphic.color = MkF4(
            1.0f,
            1.0f,
            1.0f,
            1.0f * amod * m_MovieVisibleAlpha
        );
    }
    else if (m_HealingFrame > 0)
    {
        float gbmod = (sin((float)m_Obj->frameActive / 8) + 1.0f) / 2 * -0.75f;
        m_Obj->graphic.color = MkF4(
            1.0f + gbmod,
            1.0f,
            1.0f + gbmod,
            1.0f * m_MovieVisibleAlpha
        );
        DrawCharaEff(CharaEffType::Heal,
            MkF3(
                ONE_BLOCK * 9.0f,
                ONE_BLOCK * 9.0f,
                ONE_BLOCK * 9.0f
            ),
            m_BuffEfPos);
    }
    else if (m_BuffAttackFrame > 0)
    {
        float gbmod = (sin((float)m_Obj->frameActive / 8) + 1.0f) / 2 * -0.75f;
        m_Obj->graphic.color = MkF4(
            1.0f,
            1.0f,
            1.0f + gbmod,
            1.0f * m_MovieVisibleAlpha
        );
        DrawCharaEff(CharaEffType::Buff,
            MkF3(
                ONE_BLOCK * 9.0f,
                ONE_BLOCK * 9.0f,
                ONE_BLOCK * 9.0f
            ),
            m_BuffEfPos);
    }
    else if (IsPinch())
    {
        float gbmod = (sin((float)m_Obj->frameActive / 2) + 1.0f) / 2 * -0.75f;
        m_Obj->graphic.color = MkF4(
            1.0f - gbmod,
            1.0f + gbmod,
            1.0f + gbmod,
            1.0f * m_MovieVisibleAlpha
        );
    }
    else
    {
        m_Obj->graphic.color = MkF4(
            1.0f,
            1.0f,
            1.0f,
            1.0f * m_MovieVisibleAlpha
        );
    }

    for (int i = 0; i < KAKOPON_DAT_NUM; i++)
    {
        draw_kakoponDat(&m_KakoDatList[i]);
    }
    //draw_kakoponDat(&m_KakoDat_Main);
    //draw_kakoponDat(&m_KakoDat_SubTest);

    //かこポンエフェクト
	if (m_IsDrawing)
	{

		//if (m_KakoponLineDraw.isVertexConnected)
		//{
		//	//DRAW_SPRITE_DAT dsdat = {};
		//	//dsdat.x = 0.0f;
		//	//dsdat.y = 600.0f;
		//	//dsdat.z = -1200.0f;

		//	////dsdat.w = dist + 20.0f;
		//	////dsdat.h = 56.0f;
		//	//dsdat.w = 1800.0f;
		//	//dsdat.h = 1800.0f;

		//	//dsdat.rot3D = { 60.0f, 0.0f, 0.0f };
		//	//dsdat.color = { 1.0f, 1.0f, 1.0f, 1.0f };

		//	////dsdat.texNo = 0;
		//	//dsdat.texNo = GetTexData(TEX_TYPE_EFFECT_KAKOPON_HIT_001).textureId;
		//	//dsdat.loadTexType = LOADTEXTURETYPE_MAIN;

		//	////dsdat.texUV = { 0.0f, 0.0f };
		//	////dsdat.texWH = { 1.0f, 1.0f };
		//	//dsdat.texUV = AnimationUV(CHARACTER_PLAYER_KAKOPON_COOLDOWN_FRAME - g_LineDraw.afterTimerFrame, 8, 8);
		//	//dsdat.texWH = { 1.0f / 8, 1.0f / 8 };

		//	//dsdat.startPos3D = { g_LineDraw.connectCentroid.x, g_LineDraw.connectCentroid.y, 0.0f };
		//	//dsdat.startRot3D = { 0.0f, 0.0f, 0.0f };
		//	//dsdat.startScale = { 1.0f, 1.0f };

		//	//DrawSpriteInCamera(dsdat);
		//}
	}

    {
        if(m_DamageFrame >= 0)
        {
            //float alphamod = 1.0f - (cos(2.0f * SYS_GENERAL_PI
            //    * std::min((float)m_DamageFrame / 30, 1.0f)) / 2.0f + 0.5f);
            float alphamod = EasingOutQuint(1.0f - (float)m_DamageFrame / 30);

            if (alphamod > 0)
            {
                DRAW_POLYGON_DAT dsdat = {};
                dsdat.pos.x = 0.0f;
                dsdat.pos.y = 0.0f;
                dsdat.pos.z = 0.0f;

                dsdat.size.x = SCREEN_WIDTH;
                dsdat.size.y = SCREEN_HEIGHT;
                dsdat.size.z = 1.0f;

                dsdat.rot = 0.0f;

                //dsdat.color = { 1.0f, 1.0f, 0.8f, 0.4f };
                dsdat.color = { 2.0f, 0.0f, 0.0f, 0.6f * m_MovieVisibleAlpha };
                dsdat.color.w *= alphamod;

                dsdat.texNo = m_Tex_DamageEffect.textureId;
                dsdat.loadTexType = LOADTEXTURETYPE_MAIN;

                dsdat.texUV = {
                    0.0f,
                    0.0f
                };
                dsdat.texWH = { 1.0f, 1.0f };

                dsdat.startPos = {
                    0.0f,
                    0.0f
                };
                dsdat.startRot = 0;
                dsdat.startScale = { 1.0f, 1.0f };
                dsdat.startScale3D = { 1.0f, 1.0f, 1.0f };

                dsdat.shaderOpt.bloom = DPD_SHADER_OPT_BLOOM_NONE;
                //dsdat.shaderOpt.blendState = DPD_SHADER_OPT_BLEND_STATE_ADDCOLOR;

                DRAW_POLYGON_IN_CAMERA_EX_OPTION opt = {};
                opt.reverse = false;
                opt.fixed = true;
                opt.depthBuffIgnore = true;
                opt.priority = DRAW_POLYGON_PRIORITY_NML_ALWAYS;

                DrawPolygonInCameraSingle(player->index, dsdat, opt);
            }
        }
    }

    {
        if (m_DamageSternFrame >= 0 && m_DamageSternEffectFlag)
        {
            {
                DRAW_POLYGON_DAT dsdat = {};
                dsdat.pos.x = 0.0f;
                dsdat.pos.y = 0.0f;
                dsdat.pos.z = 0.0f;

                dsdat.size.x = 40.0f;
                dsdat.size.y = 40.0f;
                dsdat.size.z = 40.0f;

                dsdat.rot = 0.0f;
                dsdat.rot3D = {
                    90.0f + sinf((float)m_Obj->frameActive / 12) * 6.0f,
                    00.0f + cosf((float)m_Obj->frameActive / 12) * 6.0f,
                    0.0f + -4.0f * m_Obj->frameActive
                };

                dsdat.color = { 1.0f, 1.0f, 1.0f, 1.0f };

                dsdat.texNo = 0;
                dsdat.modelNo = m_Model_StanStar;
                dsdat.loadTexType = LOADTEXTURETYPE_MAIN;

                dsdat.texUV = {
                    0.0f,
                    0.0f
                };
                dsdat.texWH = { 1.0f, 1.0f };

                dsdat.startPos3D = m_Obj->pos;
                dsdat.startPos3D.x += 0.0f;
                dsdat.startPos3D.y += 0.0f + ONE_BLOCK * 0.0f;
                dsdat.startPos3D.z += 0.0f + ONE_BLOCK * -5.0f;

                dsdat.startRot = 0;
                dsdat.startScale = { 1.0f, 1.0f };
                dsdat.startScale3D = { 1.0f, 1.0f, 1.0f };

                dsdat.shaderOpt.bloom = DPD_SHADER_OPT_BLOOM_NONE;
                dsdat.shaderOpt.blendState = DPD_SHADER_OPT_BLEND_STATE_ADDCOLOR;

                DRAW_POLYGON_IN_CAMERA_EX_OPTION opt = {};
                opt.reverse = false;
                opt.fixed = false;
                opt.depthBuffIgnore = false;
                opt.priority = DRAW_POLYGON_PRIORITY_NML_u1;
                opt.shaderType = DPD_SHADER_TYPE_EFFECT;

                DrawPolygonInCameraEx(dsdat, opt);
            }
        }
    }
}

// ===================================================
// Dead
// オブジェクトのHPが0になったとき
// ===================================================
void GOE_Character_Player::Dead()
{
	SuperGOE::Dead();

    PLAYER* player = GetPlayer(m_Obj);
    if (player == nullptr)
    {
        return;
    }
    //
    //↓処理
    //

    m_DeadNum++;

    m_Obj->vel = {};
    m_Obj->hitStop = {};

    if (GetActivePlayerNum() > 1)
    {
        int defnum = GetActivePlayerNum() - GetAlivePlayerNum();
        if (defnum == 1)
        {
            GE_GayaStartRequest("game_multi_ally_defeat_1_mayu");
        }
        if (defnum == 2)
        {
            GE_GayaStartRequest("game_multi_ally_defeat_2_mayu");
        }
        if (defnum == 3)
        {
            GE_GayaStartRequest("game_multi_ally_defeat_3_mayu");
        }
        //GE_GayaStartRequest("game_multi_ally_damage_mayu");
    }

 /*   if (GetAlivePlayerNum() <= 1)
    {
        CameraFlashRequestAll(
            180,
            EasingReverse,
            MkF4(1.0f, 0.0f, 0.0f, 0.9f),
            DPD_SHADER_OPT_BLEND_STATE_ADDCOLOR
        );
    }*/
}

// ===================================================
// DamageAttacker
// オブジェクトの攻撃判定がヒットしたとき
// ===================================================
void GOE_Character_Player::DamageAttacker(GAME_OBJECT_DAMAGE_RESULT dr)
{
	SuperGOE::DamageAttacker(dr);

	//DebugPrintf("ateta\n");
	//if (dr.colAtk.isGrab)
		//GameObjectMoveRequest(recOb,
		//	20,
		//	MkF3(recOb->pos.x, recOb->pos.y - 600.0f, recOb->pos.z),
		//	EasingInOutSine);

    if(dr.damage > 0)
	{
		GAME_OBJECT* recOb = GetGameObject(dr.recObjIndex);

        if(dr.colAtk.attribute == GAME_OBJECT_COLLISION_ATTACK_ATTRIBUTE_PLAYER_NORMAL_ATTACK)
        {
            if (std::string(N_ATTACK_GENERAL_TAG) == dr.colAtk.tag)
            {
                //インクゲージの回復
                if (m_InkGage < m_InkGageMax)
                {
                    m_InkGage += KAKOPON_INK_GAGE_HEAL_ATTACK;
                    m_InkGage = std::min(m_InkGage, m_InkGageMax);
                }
            }

            //float arDis = CalculateDistance(m_Obj->pos, recOb->pos);
            Float3 atkColPosTmp = MkF3(
                m_Obj->pos.x + dr.colAtk.pos.x,
                m_Obj->pos.y + dr.colAtk.pos.y,
                m_Obj->pos.z + dr.colAtk.pos.z
            );
            Float3 arVec = NormalizeVector(CalculatePosToVector(recOb->pos, atkColPosTmp));
            Float3 efPos = recOb->pos;
            efPos.x += arVec.x * recOb->size.x / 2;
            efPos.y += arVec.y * recOb->size.y / 2;
            efPos.z += arVec.z * recOb->size.y / 2;
            //efPos.z += -m_Obj->size.y / 2;
            efPos.z += -m_Obj->size.y / 4;

            GAME_OBJECT* go = GO_CreateEffect(GO_EFFECT_TYPE_N_ATTACK_HIT, efPos);
            if (go != nullptr)
            {
                go->scale = MkF3(
                    2.2f,
                    2.2f,
                    2.2f
                );
                GameObjectSetLifeTime(go, 20);
            }
        }
	}
}

// ===================================================
// DamageReciever
// オブジェクトが
// 別のオブジェクトの攻撃判定にヒットしたとき
// ===================================================
void GOE_Character_Player::DamageReciever(GAME_OBJECT_DAMAGE_RESULT dr)
{
	SuperGOE::DamageReciever(dr);

	if (dr.damage > 0)
	{
        PLAYER* player = GetPlayer(m_Obj);
        if (player != nullptr)
        {
            SetControllerMainLeftVibration(30, player->index);
            SetControllerMainRightVibration(30, player->index);

            TotalPlayerDamageAdd(dr.damage);
        }

        if (IsDefeat())
        {
            m_Obj->hitStop = {};
            if (player != nullptr)
            {
                if (GetAlivePlayerNum() >= 2)
                {
                    //CameraFlashRequest(
                    //    player->index,
                    //    80,
                    //    EasingInOutSine,
                    //    MkF4(1.0f, 0.0f, 0.0f, 0.5f),
                    //    DPD_SHADER_OPT_BLEND_STATE_ADDCOLOR
                    //);

                    CameraShakeRequest(
                        player->index,
                        CAMERA_SHAKE_EXLARGE_FRAME,
                        CAMERA_SHAKE_EXLARGE_POWER * 2.5f,
                        -1,
                        true
                    );
                }
            }

            SoundEffectPlayRequest(SOUND_TYPE_SE_4, 4.0f, 0.0f);
        }
        else
        {
            GAME_OBJECT* atkrObj = GetGameObject(dr.atkObjIndex);

            if (player != nullptr)
            {
                //CameraFlashRequest(
                //    player->index,
                //    30,
                //    EasingInOutSine,
                //    MkF4(1.0f, 0.0f, 0.0f, 0.3f),
                //    DPD_SHADER_OPT_BLEND_STATE_ADDCOLOR
                //);

                CameraShakeRequest(
                    player->index,
                    CAMERA_SHAKE_LARGE_FRAME,
                    CAMERA_SHAKE_EXLARGE_POWER * 1.5f,
                    -1,
                    true
                );
            }

            SoundEffectPlayRequest(SOUND_TYPE_SE_4, 4.0f, 0.0f);

            m_Obj->status.invFrame = GOE_Character_Player::DAMAGE_INV_FRAME;

            SetKnockBack(
                dr.colAtk.knockBackPower,
                dr.colAtk.hitStern,
                atkrObj
            );

            if (GetActivePlayerNum() <= 1)
            {
                if (IsPinch() && std::string("FATAL") != dr.colAtk.tag)
                {
                    if (!m_PinchFlag)
                    {
                        m_PinchFlag = true;

                        GE_GayaStartRequest("game_pinch_solo_mayu");
                    }
                }
            }
        }

        m_DamageFrame = 0;
	}
}

// ===================================================
// BlockHit
// オブジェクトがブロックに触れたとき
// ===================================================
void GOE_Character_Player::BlockHit(GAME_OBJECT_POSMOD_RESULT pmr)
{
	SuperGOE::BlockHit(pmr);

    //if (!m_Obj->moveRequ.enable)
    //{
    //    //float refpow = pow(m_Obj->friction + 1.0f, 3);
    //    //m_Obj->vel.x += -pmr.beforeVel.x * 0.7f * refpow;
    //    //m_Obj->vel.y += -pmr.beforeVel.y * 0.7f * refpow;
    //    m_Obj->vel.x += -pmr.beforeVel.x * 0.9f;
    //    m_Obj->vel.y += -pmr.beforeVel.y * 0.9f;

    //    float power = (abs(m_Obj->vel.x) + abs(m_Obj->vel.y)) / 2;
    //    if (power > 16.0f)
    //    {
    //        CameraShakeRequestAll(
    //            CAMERA_SHAKE_SMALL_FRAME,
    //            CAMERA_SHAKE_LARGE_POWER * (1.0f + power * 0.018f)
    //        );
    //        {
    //            Float3 efPos2 = GetGameObjectColRecAbsolutePos(m_Obj);
    //            efPos2.x += m_BeforeVec.x * BLOCK_WIDTH * 0.1f;
    //            efPos2.y += m_BeforeVec.y * BLOCK_WIDTH * 0.1f;
    //            efPos2.z += BLOCK_HIGHT * -0.5f;

    //            GAME_OBJECT* go = GO_CreateEffect(GO_EFFECT_TYPE_SMOKE, efPos2);
    //            if (go != nullptr)
    //            {
    //                //go->graphic.rot3d.x = 90.0f;
    //                go->graphic.color.w = 1.0f;
    //                go->scale = MkF3(
    //                    1.8f,
    //                    1.8f,
    //                    1.8f
    //                );
    //                GameObjectSetLifeTime(go, 25);
    //            }
    //        }
    //    }
    //}

    //if(m_IsDrawing)
    {
        GAME_OBJECT* other = GetGameObject(pmr.objIndex);

        float power = (abs(m_Obj->vel.x) + abs(m_Obj->vel.y)) / 2;
        power *= 0.3f;
        power = std::max(power, 16.0f);

        if (!m_Obj->moveRequ.enable)
        {
            Float3 knockbackvec = pmr.beforeVel;
            knockbackvec = NormalizeVector(knockbackvec);
            GameObjectMoveRequest(m_Obj,
                std::min((int)power + 1, 40),
                MkF3(
                    m_Obj->pos.x + -knockbackvec.x * ONE_BLOCK * power / 4,
                    m_Obj->pos.y + -knockbackvec.y * ONE_BLOCK * power / 4,
                    m_Obj->pos.z
                ),
                EasingOutQuad
            );

            CameraShakeRequestAll(
                CAMERA_SHAKE_SMALL_FRAME,
                CAMERA_SHAKE_LARGE_POWER * (1.0f + power * 0.018f)
            );
            {
                Float3 efPos2 = GetGameObjectColRecAbsolutePos(m_Obj);
                efPos2.x += m_BeforeVec.x * BLOCK_WIDTH * 0.1f;
                efPos2.y += m_BeforeVec.y * BLOCK_WIDTH * 0.1f;
                efPos2.z += BLOCK_HIGHT * -0.5f;

                GAME_OBJECT* go = GO_CreateEffect(GO_EFFECT_TYPE_SMOKE, efPos2);
                if (go != nullptr)
                {
                    //go->graphic.rot3d.x = 90.0f;
                    go->graphic.color.w = 1.0f;
                    go->scale = MkF3(
                        1.8f,
                        1.8f,
                        1.8f
                    );
                    GameObjectSetLifeTime(go, 25);
                }
            }
        }

        if (IsActiveGameObject(other))
        {
            GOE_ObjectDynamic* goe =
                dynamic_cast<GOE_ObjectDynamic*>(other->events.source);  // ダウンキャスト

            if (goe != nullptr)
            { //
                goe->PlayerHit(this);
            }
        }
    }
    //DebugPrintf("hit\n");
}

void GOE_Character_Player::CalcAttackerDamage(
    GAME_OBJECT_DAMAGE_RESULT* output,

    const GAME_OBJECT* obj, const GAME_OBJECT* objAttacker,
    int damage,
    GAME_OBJECT_COLLISION_ATTACK_CHILD colAtk,
    GAME_OBJECT_COLLISION_RECIEVE_CHILD colRec
)
{
    float atkBufMod = GetBuffAttackMod();
    if (atkBufMod != 1.0f)
    {
        output->damage *= atkBufMod;
    }
};

Float2 GOE_Character_Player::GetKakoponVertexNum()
{
    int n = 0;
    for (int i = 0; i < KAKOPON_VERTEX_MAX; i++)
    {
    	if (m_KakoDatList[0].dat.vertex[i].enableFrame > 0)
    	{
    		n++;
    	}
    }
    return { (float)n, KAKOPON_VERTEX_MAX };
}

Int2 GOE_Character_Player::GetKakoponInkGage()
{
    return { m_InkGage, m_InkGageMax };
}

void GOE_Character_Player::WalkRequest(
    int frame, 
    Float3 pos,
    std::function<float(float)> easingX, 
    std::function<float(float)> easingY,
    std::function<float(float)> easingZ,
    bool isBlockCollisionIgnore
)
{
    m_MoveRequWalfFrame = frame; 
    GameObjectMoveRequest(m_Obj,
        frame,
        pos,
        easingX,
        easingY,
        easingZ,
        isBlockCollisionIgnore
    );
}

void GOE_Character_Player::BecomeRevive()
{
    m_Obj->status.hp = m_Obj->status.hpMax * 0.25f;
    m_Obj->status.deadFlag = false;
    //m_DefeatFrame = 0;
}

bool GOE_Character_Player::IsDefeat() const
{
    bool isGameOver = false;
    {
        Stage_Base* stg = GetCurrentStageSceneGame();
        if (stg != nullptr)
        {
            isGameOver = stg->IsGameover();
        }
    }
    return IsDefeatGameObject(m_Obj) || isGameOver;
}

void GOE_Character_Player::HealingFlag()
{
    m_HealingFrame = FRAME_ONE_SECOND;
}

void GOE_Character_Player::BuffAttackFlag(float mod)
{
    m_BuffAttackFrame = FRAME_ONE_SECOND;
    m_BuffAttackMod = mod;
}
void GOE_Character_Player::BuffSpeedFlag(float mod)
{
    m_BuffSpeedFrame = FRAME_ONE_SECOND;
    m_BuffSpeedMod = mod;
}
void GOE_Character_Player::BuffInkCostFlag(float mod)
{
    m_BuffInkCostFrame = FRAME_ONE_SECOND;
    m_BuffInkCostMod = mod;
}

float GOE_Character_Player::GetBuffAttackMod() const
{
    if (m_BuffAttackFrame > 0)
    {
        return m_BuffAttackMod;
    }
    return 1.0f;
}
float GOE_Character_Player::GetBuffSpeedMod() const
{
    if (m_BuffSpeedFrame > 0)
    {
        return m_BuffSpeedMod;
    }
    return 1.0f;
}
float GOE_Character_Player::GetBuffInkCostMod() const
{
    if (m_BuffInkCostFrame > 0)
    {
        return m_BuffInkCostMod;
    }
    return 1.0f;
}

void GOE_Character_Player::SetCameraMoveActionFlag(int n)
{
    m_CameraMoveActionFlag = n;
}

int GOE_Character_Player::GetCameraMoveActionFlag() const
{
    return m_CameraMoveActionFlag;
}

//
// kakopon
//

void GOE_Character_Player::kakopon_attack_setBuf(KAKOPON_DATA* dat)
{
    KAKOPON_DATA_ATKBUF atkBuf = {};
    atkBuf.level = ((m_InkGage_DrawBefore - m_InkGage) / KAKOPON_INK_GAGE_ONE);

    {
        Character::MOTION_COL_DAT motionColDetails =
            Character::GetMotionColDataFindByName(
                Character::MOTION_COL_TYPE_ATK,
                m_KakoDamName
            );

        GAME_OBJECT_COLLISION_ATTACK_CHILD atkColtmp = motionColDetails.atkCol;

        int baseDam = atkColtmp.power;
        //atkCol.power += 200 * ((dat.vertexNum * KAKOPON_INK_GAGE_COST) / KAKOPON_INK_GAGE_ONE);
        atkColtmp.power += (baseDam * m_KakoDamAddMod) * atkBuf.level;
        //DebugPrintf("%d  %d  %d\n", m_InkGage_DrawBefore, m_InkGage, KAKOPON_INK_GAGE_ONE);

        //atkColtmp.power *= GetBuffAttackMod(); //バフ効果

        calc_kakopon_damage(&atkColtmp);

        atkBuf.col = atkColtmp;
    }


    dat->atkBuf = atkBuf;
}

bool GOE_Character_Player::is_kakopon_objects_attack_target(GAME_OBJECT* ob)
{
    return
        ob != nullptr
        && IsBelongsFamilyGameObject(ob, GAME_OBJECT_FAMILY_ENEMY);
}
void GOE_Character_Player::kakopon_objects_attack(KAKOPON_DATA* dat)
{
    GAME_OBJECT_COLLISION_ATTACK_CHILD atkCol = dat->atkBuf.col;
    //DebugPrintf("%d\n", atkCol.power);

    std::vector<KAKOPON_ATTACK_OBJECT_ST> attackObj = {};

    //オブジェクトを検証
    for (int i = 0; i < dat->objs.size(); i++)
    {
        GAME_OBJECT* ob = GetGameObjectFindByIdentifier(dat->objs[i]);

        if (is_kakopon_objects_attack_target(ob))
        {
            
            {
                if (ob->collision.rec.enable)
                { //くらい判定が有効なら処理する

                    //有効なくらい判定を探す
                    GAME_OBJECT_COLLISION_RECIEVE_CHILD colRec = {};
                    for (int j = 0; j < GAME_OBJECT_COLLISION_NUM_MAX; j++)
                    {
                        if (ob->collision.rec.collision[j].enable)
                        {
                            colRec = ob->collision.rec.collision[j];
                            break;
                        }
                    }

                    if (colRec.enable)
                    { //判定が有効なら処理する

                        KAKOPON_ATTACK_OBJECT_ST stru = {};
                        stru.obj = ob;
                        stru.colRev = colRec;
                        stru.dist = CalculateDistance(
                            dat->connectCentroid,
                            Float3ToFloat2(ob->pos)
                        );
                        attackObj.push_back(stru);
                    }
                }
            }

        }
    }

    if (attackObj.size() > 0)
    {
        std::sort(attackObj.begin(),
            attackObj.end(),
            [](const KAKOPON_ATTACK_OBJECT_ST& a, const KAKOPON_ATTACK_OBJECT_ST& b)
            {
                return a.dist < b.dist;
            }
        );

        int attackObjCount = 0;
        //オブジェクトを検証
        for (int i = 0; i < attackObj.size(); i++)
        {
            GAME_OBJECT* ob = attackObj[i].obj;

            if (IsActiveGameObject(ob))
            {
                GameObjectDamageCollisionHit(ob, m_Obj,
                    atkCol,
                    attackObj[i].colRev
                );

                if (ob->hitStop.frame > 0)
                {
                    int addStopf = std::max(std::min(60 / (int)attackObj.size(),
                        20), 6)
                        * attackObjCount
                        ;
                    //int addStopf = 5 * attackObjCount;
                    ob->hitStop.frameMax += addStopf;
                    ob->hitStop.frame += addStopf;
                }

                attackObjCount++;
            }
        }

        if (attackObjCount > 0)
        {
            int invf = FRAME_ONE_SECOND / 2;
            if (m_Obj->status.invFrame < invf)
            {
                m_Obj->status.invFrame = invf;
            }

            PLAYER* player = GetPlayer(m_Obj);
            if (player != nullptr)
            {
                SetFlagTtrlKako();

                dat->IsSuccess = true;
                CameraShakeRequest(player->index,
                    CAMERA_SHAKE_LARGE_FRAME,
                    CAMERA_SHAKE_EXLARGE_POWER * 2.0f
                );

                //GameManagerResultAddScore(
                //    ((int)(1.0f / 5 * attackCount) * 100),
                //    RESULT_SCORE_EX_TYPE_MANY_ENEMY_DEFEAT
                //);
                //if (attackCount >= 10
                //    && !m_EnemyManyDefeatFlag)
                //{
                //    m_EnemyManyDefeatFlag = true;

                //    GE_GayaStartRequest(
                //        "game_defeat_large_number_of_enemies",
                //        FRAME_ONE_SECOND * 2.0f
                //    );
                //}
            }
        }
    }
}

//かこポンデータ初期化
void GOE_Character_Player::init_kakoponDat(KAKOPON_SET* set)
{
    KAKOPON_DATA* dat = &set->dat;

    dat->frame = 0;
    dat->frameDraw = 0;

    dat->isFirstObjectCheck = false;
    dat->isConnected = false;
    dat->IsSuccess = false;

    dat->vertexNum = 0;
    for (int i = 0; i < KAKOPON_VERTEX_MAX; i++)
    {
        //if (dat->vertex[i].enableFrame > 0)
        //{
        //    dat->vertex[i].enableFrame = 0;
        //}
        dat->vertex[i].enableFrame = 0;
        dat->vertex[i].isEndLine = false;
    }

    dat->objs.clear();
    dat->objs.shrink_to_fit();
}

//かこポンデータ更新
void GOE_Character_Player::update_kakoponDat(KAKOPON_SET* set)
{
    KAKOPON_DATA* dat = &set->dat;

    //つながっていたら
    if (dat->isConnected)
    {
        //if (!m_KakoponLineDraw.isVertexObjectCheck)
        {
            dat->objs.clear();
            //オブジェクトを検証
            for (int i = 0; i < GAME_OBJECT_MAX; i++)
            {
                GAME_OBJECT* ob = GetGameObject(i);
                if (IsActiveGameObject(ob))
                {
                    //bool beforeVectorInit = false;
                    //float beforeVector = 0;
                    //int beforeIndex = -1;

                    int countR = 0;
                    int countD = 0;
                    for (int j = 0; j < KAKOPON_VERTEX_MAX - 1; j++)
                    {
                        KAKOPON_VERTEX* lineStartVertexTmp = dat->vertex + j;
                        KAKOPON_VERTEX* lineEndVertexTmp = dat->vertex + j + 1;
                        if (lineStartVertexTmp->enableFrame > 0 && lineEndVertexTmp->enableFrame > 0)
                        {
                            //float rott = CalculateRadToDeg(atan2(lineEndVertexTmp->pos.y - lineStartVertexTmp->pos.y,
                            //	lineEndVertexTmp->pos.x - lineStartVertexTmp->pos.x));

                /*			if ((!beforeVectorInit || abs(beforeVector - rott) > 20.0f)
                                && (beforeIndex == -1 || abs(beforeVector - j) > 2)
                                && (rott > 8.0f || rott < -8.0f)
                                && CheckLineCollider(MkF2(ob->pos.x, ob->pos.y), MkF2(ob->pos.x + SCREEN_WIDTH, ob->pos.y),
                                lineStartVertexTmp->pos, lineEndVertexTmp->pos)
                                )*/
                                //Float2 nmv = NormalizeVector(MkF2(lineEndVertexTmp->pos.x - lineStartVertexTmp->pos.x,
                                //	lineEndVertexTmp->pos.y - lineStartVertexTmp->pos.y));


                            //誤判定防止のため
                            // Rayを横に伸ばしている場合は
                            // 頂点とのy座標の差が多少ある場合のみ判定するようにする
                            if (
                                (abs(lineStartVertexTmp->pos.y - ob->pos.y) > 0.001f)
                                && CheckLineCollider(MkF2(ob->pos.x, ob->pos.y), MkF2(ob->pos.x + SCREEN_WIDTH * 8, ob->pos.y),
                                    lineStartVertexTmp->pos, lineEndVertexTmp->pos)
                                )
                            {
                                //beforeVectorInit = true;
                                //beforeVector = rott;

                                //beforeIndex = j;

                                countR++;
                            }
                            // Rayを縦に伸ばしている場合は
                            // 頂点とのx座標の差が多少ある場合のみ判定するようにする
                            if (
                                (abs(lineStartVertexTmp->pos.x - ob->pos.x) > 0.001f)
                                && CheckLineCollider(MkF2(ob->pos.x, ob->pos.y), MkF2(ob->pos.x, ob->pos.y + SCREEN_WIDTH * 8),
                                    lineStartVertexTmp->pos, lineEndVertexTmp->pos)
                                )
                            {
                                countD++;
                            }
                        }
                    }

                    //Rayと線の交差数が奇数の場合は中に入ってるとする
                    if (countR != 0 && countR % 2 != 0
                        && countD != 0 && countD % 2 != 0)
                    {
                        //追加
                        dat->objs.push_back(GetIdentifierGameObject(ob));
                        //areaObjects.push_back(GetIdentifierGameObject(ob));
                    }
                }
            }

            //m_KakoponLineDraw.isVertexObjectCheck = true;
        }

        dat->frame++;
    }
    else
    { //つながっていない場合

        //bool startLineEnable = false;
        //Float2 startLineStart = {};
        //Float2 startLineEnd = {};

        //最後の線
        bool endLineEnable = false;
        int endLineIndex = 0;
        KAKOPON_VERTEX* endLineStartVertex = nullptr;
        KAKOPON_VERTEX* endLineEndVertex = nullptr;

        //for (int i = 0; i < P_VERTEX_MAX - 1; i++)
        //{
        //	if (p_vertex[i].enable && p_vertex[i + 1].enable)
        //	{
        //		startLineEnable = true;
        //		startLineStart = p_vertex[i].pos;
        //		startLineEnd = p_vertex[i + 1].pos;
        //		break;
        //	}
        //}

        //頂点数をカウント
        int maxLineVertex = 0;
        for (int i = 0; i < KAKOPON_VERTEX_MAX; i++)
        {
            if (dat->vertex[i].enableFrame > 0)
            {
                maxLineVertex++;
            }
        }

        //最後の線を探す
        for (int i = 0; i < KAKOPON_VERTEX_MAX; i++)
        {
            dat->vertex[i].isEndLine = false;
        }
        for (int i = 0; i < KAKOPON_VERTEX_MAX - 1; i++)
        {
            if (dat->vertex[i].enableFrame > 0
                && dat->vertex[i + 1].enableFrame > 0)
            {
                endLineEnable = true;
                endLineIndex = i;
                endLineStartVertex = dat->vertex + i;
                endLineEndVertex = dat->vertex + i + 1;
            }
        }
        if (endLineStartVertex != nullptr)
        {
            endLineStartVertex->isEndLine = true;
        }

        //かこったかを判定
        if (endLineEnable && maxLineVertex >= 3)
        {
            for (int i = 0; i < KAKOPON_VERTEX_MAX - 1; i++)
            {
                KAKOPON_VERTEX* lineStartVertexTmp = dat->vertex + i;
                KAKOPON_VERTEX* lineEndVertexTmp = dat->vertex + i + 1;

                if (lineStartVertexTmp->enableFrame > 0 && lineEndVertexTmp->enableFrame > 0)
                {
                    //{
                    //    int num = 0;
                    //    for (int j = i; j < KAKOPON_VERTEX_MAX; j++)
                    //    {
                    //        if (dat->vertex[j].enable)
                    //        {
                    //            num++;
                    //        }
                    //    }
                    //    //Float2* areaPoints = (Float2*)malloc(sizeof(Float2) * num);

                    //    Float2 areaPoints[KAKOPON_VERTEX_MAX] = {};
                    //    for (int j = 0; j < num; j++)
                    //    {
                    //        if (dat->vertex[i + j].enable)
                    //        {
                    //            areaPoints[j] = dat->vertex[i + j].pos;
                    //        }
                    //    }
                    //    float polygonArea = CalculatePolygonArea(areaPoints, num);
                    //    //free(areaPoints);

                    //    //DebugPrintf("%f\n", polygonArea);

                    //    if (polygonArea < 50.0f)
                    //    {
                    //        continue;
                    //    }
                    //}


                    float endLineRot = CalculateRadToDeg(atan2(endLineEndVertex->pos.y - endLineStartVertex->pos.y,
                        endLineEndVertex->pos.x - endLineStartVertex->pos.x));
                    float lineRot = CalculateRadToDeg(atan2(lineEndVertexTmp->pos.y - lineStartVertexTmp->pos.y,
                        lineEndVertexTmp->pos.x - lineStartVertexTmp->pos.x));

                    Float2 endLineNv = NormalizeVector(MkF2(endLineEndVertex->pos.x - endLineStartVertex->pos.x,
                        endLineEndVertex->pos.y - endLineStartVertex->pos.y));
                    Float2 lineNv = NormalizeVector(MkF2(lineEndVertexTmp->pos.x - lineStartVertexTmp->pos.x,
                        lineEndVertexTmp->pos.y - lineStartVertexTmp->pos.y));

                    //線の交点を計算
                    CalculateIntersectionResult inteTmp = CalculateIntersection(
                        lineStartVertexTmp->pos, lineEndVertexTmp->pos,
                        endLineStartVertex->pos, endLineEndVertex->pos);

                    //constexpr float connectSize = 100.0f;
                    //constexpr float connectSize = ONE_BLOCK * 3.0f;
                    //float connectSize = ONE_BLOCK * 3.0f;
                    //if (m_InkGage < KAKOPON_INK_GAGE_ONE)
                    //{
                    //    connectSize *= 3.0f;
                    //}
                    float connectSize = ONE_BLOCK * 5.0f;
                    if (m_InkGage < KAKOPON_INK_GAGE_ONE)
                    {
                        connectSize *= 2.0f;
                    }

                    //交差しているかを判定

                    //if (//abs(endLineRot - lineRot) > 0.01f &&
                    //	(
                    //		(
                    //			abs(endLineIndex - i) >= 8
                    //			&& CheckLineCollider(
                    //				lineStartVertexTmp->pos, lineEndVertexTmp->pos,
                    //				endLineStartVertex->pos, endLineEndVertex->pos)
                    //		)
                    //		|| (
                    //			abs(endLineIndex - i) >= 8
                    //			&& CheckLineBoxCollider(
                    //				lineStartVertexTmp->pos, lineEndVertexTmp->pos,
                    //				MkF2(
                    //					endLineEndVertex->pos.x + lineNv.x * connectSize / 2,
                    //					endLineEndVertex->pos.y + lineNv.y * connectSize / 2
                    //				),
                    //				MkF2(
                    //					connectSize,
                    //					connectSize
                    //				)
                    //			)
                    //		)
                    //	)
                    //)
                    if ((
                        (
                            abs(endLineRot - lineRot) > 0.1f
                            && abs(endLineIndex - i) >= 3
                            && CheckLineCollider(
                                lineStartVertexTmp->pos, lineEndVertexTmp->pos,
                                endLineStartVertex->pos, endLineEndVertex->pos)
                            )
                        || (
                            abs(endLineIndex - i) >= 3
                            && CalculateDistance(
                                CalculateCentroid(lineStartVertexTmp->pos, lineEndVertexTmp->pos),
                                endLineEndVertex->pos
                            ) < connectSize

                            //CheckLineBoxCollider(
                            //    endLineStartVertex->pos, 
                            //    endLineEndVertex->pos,
                            //    CalculateCentroid(lineStartVertexTmp->pos, lineEndVertexTmp->pos),
                            //    Float2 size
                            //)
                        )
                        //|| (
                        //    abs(endLineRot - lineRot) <= 0.1f
                        //    && abs(endLineIndex - i) >= 3
                        //    && CalculateDistance(
                        //        CalculateCentroid(lineStartVertexTmp->pos, lineEndVertexTmp->pos),
                        //        endLineEndVertex->pos
                        //    ) < connectSize
                        //    /*				&& CheckLineBoxCollider(
                        //                        lineStartVertexTmp->pos, lineEndVertexTmp->pos,
                        //                        MkF2(
                        //                            endLineEndVertex->pos.x,
                        //                            endLineEndVertex->pos.y
                        //                        ),
                        //                        MkF2(
                        //                            connectSize,
                        //                            connectSize
                        //                        )
                        //                    )
                        //                    && (
                        //                        CalculateDistanceBetweenParallelLines(endLineNv, lineNv) < connectSize2
                        //                        || CalculateDistanceBetweenParallelLines(lineNv, endLineNv) < connectSize2
                        //                    )*/
                        //    )
                        )
                    )
                    {
                        //つなげる頂点の生成
                        Float2 connectPoint = {};
                        if (inteTmp.enable)
                        { //交点が計算できていた場合
                            connectPoint = inteTmp.pos;
                        }
                        else
                        {
                            //交点が計算できていない場合
                            //線を伸ばして再度計算する
                            Float2 lpostmp1 = lineEndVertexTmp->pos;
                            Float2 lpostmp2 = endLineEndVertex->pos;
                            {
                                Float2 vectmp1 = {};
                                vectmp1.x = lineEndVertexTmp->pos.x - lineStartVertexTmp->pos.x;
                                vectmp1.y = lineEndVertexTmp->pos.y - lineStartVertexTmp->pos.y;
                                vectmp1 = NormalizeVector(vectmp1);
                                Float3 vectmp2 = {};
                                vectmp2.x = endLineEndVertex->pos.x - endLineStartVertex->pos.x;
                                vectmp2.y = endLineEndVertex->pos.y - endLineStartVertex->pos.y;
                                vectmp2 = NormalizeVector(vectmp2);

                                lpostmp1.x += vectmp1.x * connectSize;
                                lpostmp1.y += vectmp1.y * connectSize;
                                lpostmp2.x += vectmp2.x * connectSize;
                                lpostmp2.y += vectmp2.y * connectSize;
                            }

                            //線の交点を計算
                            CalculateIntersectionResult inteTmp2 = CalculateIntersection(
                                lineStartVertexTmp->pos, lpostmp1,
                                endLineStartVertex->pos, lpostmp2);

                            if (inteTmp.enable)
                            { //交点が計算できていた場合
                                connectPoint = inteTmp2.pos;
                            }
                            else
                            {
                                //交差しているのに交点が計算できていない、
                                //つまり線が平行の場合
                                connectPoint = lineStartVertexTmp->pos;
                            }
                        }

                        lineStartVertexTmp->pos = connectPoint;
                        endLineEndVertex->pos = connectPoint;

                        //切れ端を削除
                        for (int j = 0; j < i; j++)
                        {
                            //dat->vertex[j].enableFrame = -1;
                            dat->vertex[j].enableFrame = 0;
                        }

                        {
                            int useVertexCount = 0;
                            for (int j = 0; j < KAKOPON_VERTEX_MAX; j++)
                            {
                                if (dat->vertex[j].enableFrame > 0)
                                {
                                    useVertexCount++;
                                }
                            }
                            dat->vertexNum = useVertexCount;

                            Float2* points = (Float2*)malloc(sizeof(Float2) * useVertexCount);

                            int index = 0;
                            for (int j = 0; j < KAKOPON_VERTEX_MAX; j++)
                            {
                                if (dat->vertex[j].enableFrame > 0)
                                {
                                    points[index] = dat->vertex[j].pos;
                                    index++;
                                }
                            }

                            Float2 point = CalculateCentroid(points, useVertexCount);

                            //中心座標セット
                            dat->connectCentroid = point;

                            free(points);
                        }

                        {
                            Float2 areaPoints[KAKOPON_VERTEX_MAX] = {};
                            int indexNum = 0;
                            for (int j = i; j < KAKOPON_VERTEX_MAX; j++)
                            {
                                if (dat->vertex[j].enableFrame > 0)
                                {
                                    areaPoints[indexNum] = dat->vertex[j].pos;
                                    indexNum++;
                                }
                            }

                            //面積
                            dat->connectAreaSize = CalculatePolygonArea(areaPoints, indexNum);

                            //DebugPrintf("%f\n", dat->connectAreaSize);
                        }

                        //囲ったフラグ
                        dat->isConnected = true;

                        //g_LineDraw.afterTimerFrame = 25;

                        //フレーム初期化
                        dat->frame = 0;

                        break;
                    }
                }
            }
        }
    }

    //フレーム管理
    if (dat->frameMax > 0)
    {
        if (dat->frame >= dat->frameMax)
        {
            if (!set->lineDraw.isLineDrawing)
            {
                init_kakoponDat(set);
            }
        }
    }
}

Float2 GOE_Character_Player::kakoponCalSetVertexPos()
{
    Float2 nPos = Float3ToFloat2(m_Obj->pos);

    if (m_KakoAddDis > 0)
    {
        Float2 vectmp = NormalizeVector(Float3ToFloat2(m_BeforeVec));
        nPos.x += vectmp.x * m_KakoAddDis;
        nPos.y += vectmp.y * m_KakoAddDis;
    }

    return nPos;
}
void GOE_Character_Player::update_kakopon_action(bool keyPush, KAKOPON_DATA* dat, KAKOPON_LINEDRAW* linedraw)
{
    //指定のキー入力中のみフレームカウント
    if (keyPush)
    {
        linedraw->vertexDrawInputFlag++;
    }
    else
    {
        if (linedraw->vertexDrawInputFlag > 0)
        {
            linedraw->vertexDrawInputFlagBefore = linedraw->vertexDrawInputFlag;
        }
        linedraw->vertexDrawInputFlag = 0;
    }

    //入力を始めたらフラグを立てる
    if (linedraw->vertexDrawInputFlag == 1)
    {
        linedraw->vertexDrawCancelF = 0;
        linedraw->vertexDrawFlag = true;
    }
    //入力を辞めたらフラグを折る
    if (linedraw->vertexDrawInputFlag == 0)
    {
        linedraw->vertexDrawCancelF++;
        if (GameManagerIsMovieFlag()
            || GameManagerIsPlayerCtrlStop()

            || linedraw->vertexDrawInputFlagBefore < 30
            || linedraw->vertexDrawCancelF > 20)
        {
            //linedraw->vertexDrawFlag = false;
            *linedraw = {};
            return;
        }
    }



    //線を描くのをキャンセル
    if (linedraw->afterTimerFrame > 0)
    { //後隙があったら消費する
        linedraw->afterTimerFrame--;
        if (linedraw->afterTimerFrame <= 0)
        {
            //linedraw->vertexDrawFlag = false;
            *linedraw = {};
            return;
        }
    }
    else if (!linedraw->vertexDrawFlag)
    { //フラグが折れていたらキャンセル処理

        ////ZeroMemory(&g_LineDraw, sizeof(g_LineDraw));

        //linedraw->vertexDrawFlag = false;

        ////linedraw->isVertexObjectCheck = false;

        //linedraw->afterTimerFrame = 0;

        ////dat->isConnected = false;

        ////for (int i = 0; i < KAKOPON_VERTEX_MAX; i++)
        ////{
        ////    if (dat->vertex[i].enable)
        ////    {
        ////        dat->vertex[i].enable = false;
        ////    }
        ////}
        *linedraw = {};
        return;
    }

    //フラグがたっていたら頂点を設置していく
    if (linedraw->vertexDrawFlag)
    {
        m_KakoFirstPos = Float2ToFloat3(dat->vertex[0].pos);

        //Float2 nPos = GetControllerMainTouchScreenPosition();
        //PLAYER* pl = GetPlayer(0); 
        //PLAYER* pl = player;
        //GAME_OBJECT* plObj = pl->obj;
        Float2 nPos = kakoponCalSetVertexPos();

        //つながっていなかったら頂点を追加する
        if (!dat->isConnected
            && m_InkGage > 0
        )
        {
            if (linedraw->vertexAddFlag)
            {
                //頂点追加フラグがたっていたら
                //指定の距離進んだかをチェック
                if (CalculateDistance(nPos, linedraw->vertexBeforePos) > KAKOPON_ONE_LINE_DISTANE)
                {
                    linedraw->vertexAddFlag = false; //頂点追加フラグを折る
                }
            }
            else
            {
                linedraw->vertexAddFlag = true; //頂点追加フラグを立てる
                linedraw->vertexBeforePos = nPos; //座標を保存

                for (int i = 0; i < KAKOPON_VERTEX_MAX; i++)
                {
                    if (!dat->vertex[i].enableFrame > 0)
                    {
                        dat->vertex[i].enableFrame = 1;
                        dat->vertexNum++;

                        Float2 setPos = nPos;

                        //インクを消費
                        m_InkGage += -KAKOPON_INK_GAGE_COST
                            * m_KakoInkCostMod
                            * GetBuffInkCostMod()
                            ;

                        //SEを入れる
                        //if (dat->vertexNum % 2 == 0)
                        //{
                        //    SoundEffectPlayRequest(
                        //        GetSoundData("SOUND/SE/player/attack/2ready"),
                        //        3.0f, 0.0f);
                        //}

                        //頂点の設置位置を補正
                        //setPos.y += plObj->size.y / 2 - 180.0f;
                        //setPos.y += Character::DEFAULT_SIZE / 2 - 180.0f;
                        //setPos.y += Character::DEFAULT_SIZE / 5;

                        dat->vertex[i].pos = setPos;

                        break;
                    }
                }
            }


            //{ //敵にヒットストップを
            //	if (GetMainGameFrame() % 3 != 0)
            //	{
            //		for (int i = 0; i < GAME_OBJECT_MAX; i++)
            //		{
            //			GAME_OBJECT* ob = GetGameObject(i);
            //			if (IsActiveGameObject(ob)
            //				&& IsBelongsFamilyGameObject(ob, GAME_OBJECT_FAMILY_ENEMY))
            //			{
            //				GameObjectHitStop(ob, 1);
            //			}
            //		}
            //	}
            //}

        }
    }

    //後隙設定　仮かもしれない
    
    //linedraw->afterTimerFrame = GOE_Character_Player::KAKOPON_COOLDOWN_FRAME;
    if (linedraw->afterTimerFrame <= 0 && dat->isConnected)
    {
        linedraw->afterTimerFrame = dat->frameMax;
    }

    //std::vector<GAME_OBJECT_IDENTIFIER> areaObjects = {};


    //描いているかのフラグをセットする
    linedraw->isLineDrawing = linedraw->vertexDrawFlag && !dat->isConnected;
    //if (
    //    (linedraw->vertexDrawFlag || dat->isConnected)
    //    )
    //{
    //    linedraw->isLineDrawing = true;
    //}
    //else
    //{
    //    linedraw->isLineDrawing = false;
    //}
}

void GOE_Character_Player::kakopon_final_update()
{
    { //クールダウン系
        if (m_AttackCooldown > 0)
        {
            m_AttackCooldown--;
        }
    }

    //if (GameManagerIsMovieFlag())
    //{
    //    return;
    //}

    if (!m_IsDrawing)
    { //インクゲージの回復
        if (m_InkGage < m_InkGageMax)
        {
            m_InkGage += KAKOPON_INK_GAGE_HEAL;
            m_InkGage = std::min(m_InkGage, m_InkGageMax);
        }
        m_InkGage_DrawBefore = m_InkGage;
    }

    { //クールダウン系
        if (m_SkillCooldownFrame > 0)
        {
            m_SkillCooldownFrame--;
        }
    }

    //if (GameManagerIsMovieFlag())
    //{
    //    //更新
    //    for (int i = 0; i < KAKOPON_DAT_NUM; i++)
    //    {
    //        init_kakoponDat(&m_KakoDatList[i].dat);
    //    }
    //}
}

void GOE_Character_Player::become_stern_motion()
{
    if (m_DamageSternAnimInLoopFlag)
    {
        SetDownPose();
    }
}

void GOE_Character_Player::draw_kakoponDat(KAKOPON_SET* set)
{
    KAKOPON_DATA* dat = &set->dat;

    bool IsMainKako = dat->graphic == KAKOPON_GRAPHIC::Main
        || dat->graphic == KAKOPON_GRAPHIC::Main_Kai
        || dat->graphic == KAKOPON_GRAPHIC::Main_Neo
        || dat->graphic == KAKOPON_GRAPHIC::Main_Ca
        || dat->graphic == KAKOPON_GRAPHIC::Main_Kn
        ;

    float beforeSizeX_effect0 = 0.0f;
    float beforeSizeX_effect1 = 0.0f;

    int frame = dat->frame;
    int revFrame = (dat->frameMax - dat->frame);

    if (dat->graphic == KAKOPON_GRAPHIC::Main_Kn)
    {
        revFrame *= 0.5f;
    }

    float inMod = std::min(((float)dat->frameDraw) / 10.0f, 1.0f);
    float outMod = std::min((float)revFrame / 10, 1.0f);

    //m_TexId_kakoponEffect_Ground

    PLAYER* player = GetPlayer(m_Obj);

    if (dat->isConnected)
    {

        if (IsMainKako && dat->IsSuccess)
        {
            //PLAYER* player = GetPlayer(m_Obj);
            if (player != nullptr)
            {
                DRAW_POLYGON_DAT dsdat = {};
                dsdat.pos.x = 0.0f;
                dsdat.pos.y = 0.0f;
                dsdat.pos.z = 0.0f;

                dsdat.size.x = SCREEN_WIDTH;
                dsdat.size.y = SCREEN_HEIGHT;
                dsdat.size.z = 1.0f;

                dsdat.rot = 0.0f;

                //dsdat.color = { 1.0f, 1.0f, 0.8f, 0.4f };
                dsdat.color = { 0.0f, 0.0f, 0.0f, 0.4f * m_MovieVisibleAlpha };
                dsdat.color.w *= inMod * outMod;

                dsdat.texNo = 0;
                dsdat.loadTexType = LOADTEXTURETYPE_MAIN;

                dsdat.texUV = {
                    0.0f,
                    0.0f 
                };
                dsdat.texWH = { 1.0f, 1.0f };

                dsdat.startPos = { 
                    0.0f,
                    0.0f
                };
                dsdat.startRot = 0;
                dsdat.startScale = { 1.0f, 1.0f };
                dsdat.startScale3D = { 1.0f, 1.0f, 1.0f };

                dsdat.shaderOpt.bloom = DPD_SHADER_OPT_BLOOM_NONE;
                //dsdat.shaderOpt.blendState = DPD_SHADER_OPT_BLEND_STATE_ADDCOLOR;

                DRAW_POLYGON_IN_CAMERA_EX_OPTION opt = {};
                opt.reverse = false;
                opt.fixed = true;
                opt.depthBuffIgnore = true;
                opt.priority = DRAW_POLYGON_PRIORITY_NML_d1;


                DrawPolygonInCameraSingle(player->index, dsdat, opt);

                if(
                    dat->objs.size() > 10
                    || dat->vertexNum > (KAKOPON_INK_GAGE_MAX / KAKOPON_INK_GAGE_COST) / 2
                )
                {
                    dsdat.color = { 1.0f, 1.0f, 1.0f, 1.0f * m_MovieVisibleAlpha };
                    dsdat.color.w *= inMod * outMod;
                    dsdat.texNo = m_Tex_ScKakoEffect.textureId;
                    opt.priority = DRAW_POLYGON_PRIORITY_NML_ALWAYS;
                    dsdat.texUV = AnimationUV(
                        16 * std::min((float)dat->frameDraw / 40, 1.0f),
                        4, 4);
                    dsdat.texWH = { 1.0f / 4, 1.0f / 4 };
                    dsdat.shaderOpt.blendState = DPD_SHADER_OPT_BLEND_STATE_ADDCOLOR;
                    DrawPolygonInCameraSingle(player->index, dsdat, opt);
                }
            }
            
        }


        //{
        //    DRAW_POLYGON_DAT dsdat = {};
        //    dsdat.pos = Float2ToFloat3(dat->connectCentroid);
        //    //dsdat.pos.x = 0;
        //    //dsdat.pos.y = 0;
        //    //for (int i = 0; i < KAKOPON_VERTEX_MAX; i++)
        //    //{
        //    //    if (dat->vertex[i].enableFrame > 0)
        //    //    {
        //    //        if (dsdat.pos.x > dat->vertex[i].pos.x)
        //    //        {
        //    //            dsdat.pos.x = dat->vertex[i].pos.x;
        //    //        }
        //    //        if (dsdat.pos.y > dat->vertex[i].pos.y)
        //    //        {
        //    //            dsdat.pos.y = dat->vertex[i].pos.y;
        //    //        }
        //    //    }
        //    //}
        //    //dsdat.pos.x *= -1;
        //    //dsdat.pos.y *= -1;
        //    //dsdat.pos.x += 100.0f;
        //    dsdat.pos.y += 100.0f;

        //    //dsdat.size.x = 1000.0f;
        //    //dsdat.size.y = 1000.0f;
        //    dsdat.size.x = 1.0f;
        //    dsdat.size.y = 1.0f;
        //    dsdat.size.z = 1.0f;

        //    dsdat.rot = 0.0f;

        //    //dsdat.color = { 1.0f, 1.0f, 0.8f, 0.4f };
        //    //dsdat.color = { 0.0f, 0.0f, 0.0f, 0.4f };
        //    //dsdat.color.w *= inMod * outMod;

        //    //dsdat.color = { 2.0f, 2.0f, 2.0f, 1.0f };
        //    //dsdat.color = { 0.0f, 0.0f, 0.0f, 1.0f };
        //    //dsdat.color = { 2.0f, 2.0f, 2.0f, 0.4f };
        //    //dsdat.color = { 1.0f, 1.0f, 1.0f, 0.9f };

        //    dsdat.color = { 0.3f, 0.0f, 0.6f, 1.0f };

        //    //みため
        //    if (dat->graphic == KAKOPON_GRAPHIC::DeBuff)
        //    {
        //        dsdat.shaderOpt.blendState = DPD_SHADER_OPT_BLEND_STATE_ADDCOLOR;
        //        dsdat.color = { 0.0f, 0.1f, 1.8f, 1.0f };
        //    }
        //    else if (dat->graphic == KAKOPON_GRAPHIC::Buff)
        //    {
        //        dsdat.shaderOpt.blendState = DPD_SHADER_OPT_BLEND_STATE_ADDCOLOR;
        //        dsdat.color = { 1.8f, 0.6f, 0.0f, 1.0f };
        //    }
        //    else if (dat->graphic == KAKOPON_GRAPHIC::Heal)
        //    {
        //        dsdat.shaderOpt.blendState = DPD_SHADER_OPT_BLEND_STATE_ADDCOLOR;
        //        dsdat.color = { 0.0f, 1.8f, 0.0f, 1.0f };
        //    }
        //    dsdat.color.w *= 0.8f;
        //    dsdat.color.w *= inMod * outMod;

        //    dsdat.texNo = m_TexId_kakoponEffect_Ground;
        //    //dsdat.texNo = m_TexId_kakoponEffect_General[1];
        //    dsdat.loadTexType = LOADTEXTURETYPE_MAIN;

        //    dsdat.texUV = {
        //        0.0f + -0.01f * (float)m_Obj->frame,
        //        0.0f
        //    };
        //    dsdat.texWH = { 1.0f, 1.0f };

        //    dsdat.startPos = {
        //        0.0f,
        //        0.0f
        //    };
        //    dsdat.startRot = 0;
        //    dsdat.startScale = { 1.0f, 1.0f };
        //    dsdat.startScale3D = { 1.0f, 1.0f, 1.0f };

        //    dsdat.shaderOpt.bloom = DPD_SHADER_OPT_BLOOM_NONE;
        //    //dsdat.shaderOpt.blendState = DPD_SHADER_OPT_BLEND_STATE_ADDCOLOR;

        //    dsdat.vertex.type = DRAW_POLYGON_TYPE_UPDATE_VERTEX;
        //    for (int i = 0; i < KAKOPON_VERTEX_MAX; i++)
        //    {
        //        if (dat->vertex[i].enableFrame > 0)
        //        {
        //            //Float3 postmp = {
        //            //    dat->vertex[i].pos.x - m_Obj->pos.x,
        //            //    dat->vertex[i].pos.y - m_Obj->pos.y,
        //            //    0.0f - m_Obj->pos.z,
        //            //};
        //            //Float3 postmp = {
        //            //    dat->vertex[i].pos.x,
        //            //    dat->vertex[i].pos.y,
        //            //    0.0f,
        //            //};
        //            //dsdat.vertex.vertex[dsdat.vertex.vertexNum]
        //            //    = postmp;
        //            //dsdat.vertex.vertexNum++;

        //            DPD_VERTEX_FRAGMENT v0 = {
        //                //Float2ToFloat3(dat->vertex[i].pos),
        //                {
        //                    dat->vertex[i].pos.x - dsdat.pos.x,
        //                    dat->vertex[i].pos.y - dsdat.pos.y,
        //                    0.0f - dsdat.pos.z,
        //                },
        //                {1.0f, 1.0f, 1.0f, 1.0f},
        //                {0.0f, 0.0f},
        //                {0.0f, 0.0f, 1.0f} 
        //            };
        //            dsdat.vertex.vertex.push_back(v0);
        //        }
        //    }

        //    DRAW_POLYGON_IN_CAMERA_EX_OPTION opt = {};
        //    opt.reverse = false;
        //    //opt.fixed = true;
        //    //opt.depthBuffIgnore = true;
        //    //opt.priority = DRAW_POLYGON_PRIORITY_NML_d1;
        //    opt.depthBuffIgnore = true;
        //    opt.priority = DRAW_POLYGON_PRIORITY_NML_d2;


        //    DrawPolygonInCameraSingle(player->index, dsdat, opt);

        //}
    }

    for (int i = 0; i < KAKOPON_VERTEX_MAX; i++)
    {
        if (dat->vertex[i].enableFrame > 0)
        {
            if (i < KAKOPON_VERTEX_MAX - 1)
            {
                Float4 lineColor = { 2.0f, 2.0f, 2.0f, 1.0f };
                if (dat->vertex[i + 1].enableFrame > 0)
                {
                    //線が終端である
                    bool isLineEnd = i + 1 >= KAKOPON_VERTEX_MAX - 1
                        || !(dat->vertex[i + 2].enableFrame > 0);

                    Float2 endpostmp = dat->vertex[i + 1].pos;
                    //if (isLineEnd && !dat->isConnected && m_InkGage > 0)
                    //{
                    //    //endpostmp = kakoponCalSetVertexPos();
                    //    endpostmp = Float3ToFloat2(m_Obj->pos);
                    //}

                    float dist = CalculateDistance(
                        dat->vertex[i].pos,
                        endpostmp
                    );

                    float rott = CalculateRadToDeg(atan2(
                        endpostmp.y - dat->vertex[i].pos.y,
                        endpostmp.x - dat->vertex[i].pos.x
                    ));

                    float addX = 0.0f;
                    //float addW = 60.0f;
                    float addW = 28.0f;
                    //if (isLineEnd)
                    //{ //線が終端である
                    //    addX = -dist / 4;
                    //    addW = -dist / 2;
                    //}

                    float widthTmp = dist;
                    float hightTmp = 500.0f;

                    DRAW_POLYGON_DAT dsdat = {};
                    dsdat.pos.x = dist / 2 + addX;
                    dsdat.pos.y = 0.0f;
                    //dsdat.z = -10.0f;
                    dsdat.pos.z = -20.0f;

                    //dsdat.w = dist + 20.0f;
                    //dsdat.h = 56.0f;

                    /*dsdat.w = dist + 50.0f;
                    dsdat.h = 200.0f;*/

                    dsdat.size.x = widthTmp + addW;
                    dsdat.size.y = hightTmp;
                    dsdat.size.z = 1.0f;

                    dsdat.rot = 0.0f;

                    //dsdat.texNo = m_TexId_kakoponEffect_Trail;

                    //dsdat.texNo = 0;
                    //dsdat.loadTexType = LOADTEXTURETYPE_GRAY;
                    dsdat.loadTexType = LOADTEXTURETYPE_MAIN;

                    dsdat.startPos = { dat->vertex[i].pos.x, dat->vertex[i].pos.y };
                    dsdat.startRot = rott;
                    dsdat.startScale = { 1.0f, 1.0f };
                    dsdat.startScale3D = { 1.0f, 1.0f, 1.0f };

                    //dsdat.texUV = { 0.0f, 0.0f };
                    //dsdat.texWH = { 1.0f, 1.0f };

                    dsdat.texUV = { 0.0f, 0.0f };
                    //dsdat.texWH = { //適当な数値で乱数っぽく
                    //    1.0f * (sin((float)i * 0.7f + dsdat.startPos.x * 7.12f + dsdat.startPos.y * 0.2f + dsdat.startRot * 1.12f) < 0 ? 1.0f : -1.0f),
                    //    1.0f * (cos((float)i * 0.3f + dsdat.startPos.x * 0.92f + dsdat.startPos.y * 2.3f + dsdat.startRot * 0.19f) < 0 ? 1.0f : -1.0f)
                    //};

                    //dsdat.texUV = {
                    //    0.0f + 1.0f / 2 * i ,
                    //    0.0f 
                    //};
                    dsdat.texWH = { 
                        1.0f,
                        1.0f
                    };

                    //dsdat.texUV = {
                    //    0.0f + ((widthTmp) / dsdat.size.y) * i,
                    //    0.0f };
                    //dsdat.texWH = { ((dsdat.size.x) / dsdat.size.y), 1.0f };

                    DRAW_POLYGON_IN_CAMERA_EX_OPTION opt = {};
                    opt.reverse = false;
                    opt.fixed = false;
                    opt.depthBuffIgnore = true;
                    opt.priority = DRAW_POLYGON_PRIORITY_NML_d1;

                    //適当な数値で乱数っぽく
                    int trailIndexRand = (int)(
                        abs(sin((float)i * 0.7f + dsdat.startPos.x * 7.12f + dsdat.startPos.y * 0.2f + dsdat.startRot * 1.12f)
                            * 375.0f)
                        + abs(cos((float)i * 0.3f + dsdat.startPos.x * 0.92f + dsdat.startPos.y * 2.3f + dsdat.startRot * 0.19f)
                            * 247.0f)
                        );

                    dsdat.texNo = m_TexId_kakoponEffect_Trail_Kai[trailIndexRand % 4];

                    //みため
                    if (dat->graphic == KAKOPON_GRAPHIC::DeBuff)
                    {
                        dsdat.color = { 1.4f, 1.6f, 2.0f, 2.0f * m_MovieVisibleAlpha };
                        if (dat->isConnected)
                        {
                            dsdat.color = { 1.6f, 1.7f, 2.0f, 2.0f * m_MovieVisibleAlpha };
                            dsdat.color.w *= outMod;
                        }
                        dsdat.shaderOpt.bloom = DPD_SHADER_OPT_BLOOM_ALWAYS;
                    }
                    else if (dat->graphic == KAKOPON_GRAPHIC::Buff)
                    {
                        dsdat.color = { 2.0f, 0.7f, 0.3f, 2.0f * m_MovieVisibleAlpha };
                        if (dat->isConnected)
                        {
                            dsdat.color = { 2.0f, 1.0f, 0.5f, 2.0f * m_MovieVisibleAlpha };
                            dsdat.color.w *= outMod;
                        }
                        dsdat.shaderOpt.bloom = DPD_SHADER_OPT_BLOOM_ALWAYS;
                    }
                    else if (dat->graphic == KAKOPON_GRAPHIC::Heal)
                    {
                        dsdat.color = { 1.2f, 2.0f, 1.2f, 2.0f * m_MovieVisibleAlpha };
                        if (dat->isConnected)
                        {
                            dsdat.color = { 1.4f, 2.0f, 1.4f, 2.0f * m_MovieVisibleAlpha };
                            dsdat.color.w *= outMod;
                        }
                        dsdat.shaderOpt.bloom = DPD_SHADER_OPT_BLOOM_ALWAYS;
                    }

                    else if (dat->graphic == KAKOPON_GRAPHIC::Main_Kai)
                    {
                        dsdat.color = { 1.0f, 1.6f, 2.0f, 2.0f * m_MovieVisibleAlpha };
                    }

                    else if (dat->graphic == KAKOPON_GRAPHIC::Main_Neo)
                    {
                        dsdat.texNo = m_TexId_kakoponEffect_Trail_Neo[trailIndexRand % 4];
                        dsdat.color = { 2.0f, 1.1f, 1.3f, 2.0f * m_MovieVisibleAlpha };
                    }

                    else if (dat->graphic == KAKOPON_GRAPHIC::Main_Ca)
                    {
                        dsdat.color = { 2.0f, 1.7f, 1.1f, 2.0f * m_MovieVisibleAlpha };
                    }

                    else if (dat->graphic == KAKOPON_GRAPHIC::Main_Kn)
                    {
                        dsdat.texNo = m_TexId_kakoponEffect_Trail_Kn[trailIndexRand % 4];
                        //dsdat.color = { 2.0f, 0.8f, 0.0f, 2.0f };
                        //dsdat.color = { 0.2f, 0.02f, 0.0f, 0.65f };
                        dsdat.color = { 0.8f, 0.4f, 0.1f, 0.9f * m_MovieVisibleAlpha };
                    }

                    else if (dat->graphic == KAKOPON_GRAPHIC::Main)
                    {
                        dsdat.color = { 2.0f, 2.0f, 2.0f, 2.0f * m_MovieVisibleAlpha };
                    }

                    else
                    {
                        dsdat.color = { 2.0f, 2.0f, 2.0f, 2.0f * m_MovieVisibleAlpha };
                        if (dat->isConnected)
                        {
                            dsdat.color = { 2.0f, 2.0f, 1.2f, 2.0f * m_MovieVisibleAlpha };
                            dsdat.color.w *= outMod;
                        }
                    }

                    //メイン用
                    if (IsMainKako)
                    {
                        if (dat->isConnected)
                        {
                            dsdat.color = { 2.0f, 2.0f, 1.2f, 2.0f * m_MovieVisibleAlpha };
                            dsdat.color.w *= outMod;
                        }

                        //if (player != nullptr)
                        //{
                        //    //ブルームの色
                        //    dsdat.shaderOpt.bloom = (DPD_SHADER_OPT_BLOOM)(DPD_SHADER_OPT_BLOOM_RED + player->index);
                        //}
                        //else
                        {
                            //dsdat.shaderOpt.bloom = DPD_SHADER_OPT_BLOOM_NORMAL;
                            dsdat.shaderOpt.bloom = DPD_SHADER_OPT_BLOOM_ALWAYS;
                        }

                        //if (player != nullptr)
                        //{
                        //    dsdat.shaderOpt.outline = (DPD_SHADER_OPT_OUTLINE)(DPD_SHADER_OPT_OUTLINE_RED + player->index);
                        //}
                    }

                    for (int pInd = 0; pInd < PLAYER_MAX; pInd++)
                    {
                        DRAW_POLYGON_DAT dsdat2 = dsdat;

                        if (player != nullptr)
                        {
                            if (player->index == pInd && !dat->isConnected)
                            {
                                //float adc = sin((float)m_Obj->frameActive / 8) / 2.0f + 0.5f;
                                float adc = (sin((float)m_Obj->frameActive / 8) / 2.0f + 0.5f)
                                    * 1.5f - 0.5f;
                                dsdat2.color.x += adc;
                                dsdat2.color.y += adc;
                                dsdat2.color.z += adc;
                            }
                        }

                        DrawPolygonInCameraSingle(pInd, dsdat2, opt);
                    }
                }

                //if (dat->isConnected || m_KakoponLineDraw.afterTimerFrame > 0)
                {
                    KAKOPON_VERTEX* lineStartVertexTmp = dat->vertex + i;
                    KAKOPON_VERTEX* lineEndVertexTmp = dat->vertex + i + 1;

                    if (lineStartVertexTmp->enableFrame > 0 && lineEndVertexTmp->enableFrame > 0)
                    {
                        Float2 postmp = CalculateCentroid(lineStartVertexTmp->pos, lineEndVertexTmp->pos);

                        float rottmp = CalculateRadToDeg(
                            atan2(
                                lineEndVertexTmp->pos.y - lineStartVertexTmp->pos.y,
                                lineEndVertexTmp->pos.x - lineStartVertexTmp->pos.x
                            )
                        );

                        float sizeX = CalculateDistance(lineStartVertexTmp->pos, lineEndVertexTmp->pos);
                        float sizeY = 2200.0f * 1.2f * inMod;

                        if (dat->graphic == KAKOPON_GRAPHIC::DeBuff
                            || dat->graphic == KAKOPON_GRAPHIC::Buff
                            || dat->graphic == KAKOPON_GRAPHIC::Heal
                        )
                        {
                            sizeY = 1000.0f * inMod;
                        }

                        if (dat->graphic == KAKOPON_GRAPHIC::Main_Kn)
                        {
                            sizeY = 2000.0f;
                        }

                        float sizeX_effect0_tmp = sizeX / std::max(sizeY, 0.0001f);

                        if (dat->graphic == KAKOPON_GRAPHIC::Main_Kn)
                        {
                            int inFOri = dat->frameDraw - 30;
                            if (dat->isConnected && inFOri >= 0)
                            {
                                float inModOri = std::max(std::min(
                                    ((float)inFOri) / 10.0f,
                                    1.0f), 0.0f);
                                float outModOri = std::min((float)revFrame / 5, 1.0f);
                                float alphaTmp = 1.0f * outModOri * inModOri;

                                sizeY = 6000.0f;
                                sizeX_effect0_tmp = sizeX * 4.0f
                                    / std::max(sizeY, 0.0001f);

                                for (int j = 0; j < 3; j++)
                                {
                                    if (j == 0)
                                    { //いったｎ
                                        continue;
                                    }

                                    DRAW_POLYGON_DAT dsdat = {};
                                    dsdat.size.x = sizeX;
                                    dsdat.size.y = sizeY;

                                    if (j == 0)
                                    {
                                        dsdat.color = { 0.0f, 0.0f, 0.0f,
                                            alphaTmp * 0.9f
                                        };

                                        dsdat.texNo = 0;

                                        dsdat.texWH = {
                                            1.0f,
                                            1.0f
                                        };
                                    }
                                    else
                                    {
                                        dsdat.color = { 2.0f, 0.3f, 0.1f,
                                            alphaTmp
                                        };

                                        dsdat.texNo = m_TexId_kakoponEffect_Kn[j - 1];

                                        dsdat.texUV = {
                                            beforeSizeX_effect0,
                                            0.0f
                                        };
                                        //if (j == 1)
                                        //{
                                        //    dsdat.texUV.y = 0.06f * (float)m_Obj->frame;
                                        //}
                                        dsdat.texUV.y = 0.028f * (float)m_Obj->frame;

                                        dsdat.texWH = {
                                            sizeX_effect0_tmp,
                                            1.0f * 1.5f
                                        };
                                        //if (j == 1)
                                        //{
                                        //    dsdat.rot = 90.0f;
                                        //    {
                                        //        float t = dsdat.texUV.x;
                                        //        dsdat.texUV.x = dsdat.texUV.y;
                                        //        dsdat.texUV.y = t;
                                        //    }
                                        //    {
                                        //        float t = dsdat.texUV.x;
                                        //        dsdat.texWH.x = dsdat.texWH.y;
                                        //        dsdat.texWH.y = t;
                                        //    }
                                        //}
                                        dsdat.shaderOpt.blendState = DPD_SHADER_OPT_BLEND_STATE_ADDCOLOR;
                                    }

                                    draw_kakoponDat_Aura(
                                        dsdat,
                                        Float2ToFloat3(postmp),
                                        rottmp,
                                        j,
                                        dat
                                    );
                                }
                            }
                            else
                            {
                                {
                                    DRAW_POLYGON_DAT dsdat = {};
                                    dsdat.size.x = sizeX;
                                    dsdat.size.y = sizeY;

                                    dsdat.color = {
                                        1.0f, 0.4f, 0.0f,
                                        1.0f
                                    };

                                    dsdat.texNo = m_TexId_kakoponEffect_Add_Kn;

                                    dsdat.texUV = {
                                        beforeSizeX_effect0 + 0.003f * (float)m_Obj->frame,
                                        0.0f
                                    };

                                    dsdat.texWH = {
                                        sizeX_effect0_tmp,
                                        1.0f
                                    };
                                    dsdat.shaderOpt.blendState = DPD_SHADER_OPT_BLEND_STATE_ADDCOLOR;

                                    draw_kakoponDat_Aura(
                                        dsdat,
                                        Float2ToFloat3(postmp),
                                        rottmp,
                                        0,
                                        dat
                                    );
                                }
                            }
                        }
                        else if (dat->isConnected && dat->IsSuccess)
                        {
                            //float alphaTmp = 1.0f * ((float)revFrame / dat->frameMax);
                            float alphaTmp = 1.0f * outMod;

                            //みため
                            if (dat->graphic == KAKOPON_GRAPHIC::DeBuff)
                            {
                                for (int j = 0; j < 3; j++)
                                {
                                    if (j == 0)
                                    { //いったｎ
                                        continue;
                                    }

                                    DRAW_POLYGON_DAT dsdat = {};
                                    dsdat.size.x = sizeX;
                                    dsdat.size.y = sizeY;

                                    if (j == 0)
                                    {
                                        //dsdat.size.z = 0;
                                        dsdat.color = { 1.0f, 1.0f, 1.0f,
                                            alphaTmp * 0.25f
                                        };

                                        dsdat.texNo = 0;

                                        dsdat.texWH = {
                                            1.0f,
                                            1.0f
                                        };
                                    }
                                    else
                                    {
                                        dsdat.color = { 1.0f, 1.0f, 1.0f,
                                            alphaTmp
                                        };

                                        dsdat.texNo = m_TexId_kakoponEffect_DeBuff[j - 1];

                                        dsdat.texUV = {
                                            beforeSizeX_effect0,
                                            0.0f
                                        };
                                        if (j == 2)
                                        {
                                            dsdat.texUV.y = -0.01f * (float)m_Obj->frame;
                                        }

                                        dsdat.texWH = {
                                            sizeX_effect0_tmp,
                                            1.0f
                                        };
                                    }
                                    dsdat.shaderOpt.blendState = DPD_SHADER_OPT_BLEND_STATE_ADDCOLOR;

                                    draw_kakoponDat_Aura(
                                        dsdat,
                                        Float2ToFloat3(postmp),
                                        rottmp,
                                        j - 1,
                                        dat
                                    );
                                }
                                //{
                                //    DRAW_POLYGON_DAT dsdat = {};
                                //    dsdat.size.x = sizeX;
                                //    dsdat.size.y = sizeY;

                                //    dsdat.color = { 1.0f, 1.0f, 1.0f,
                                //        alphaTmp * 2.0f
                                //    };

                                //    dsdat.texNo = m_TexId_kakoponEffect_Add_DeBuff;

                                //    dsdat.texUV = {
                                //        beforeSizeX_effect0 + 0.01f * (float)m_Obj->frame,
                                //        0.0f
                                //    };

                                //    dsdat.texWH = {
                                //        sizeX_effect0_tmp,
                                //        1.0f
                                //    };

                                //    draw_kakoponDat_Aura(
                                //        dsdat,
                                //        Float2ToFloat3(postmp),
                                //        rottmp
                                //    );
                                //}
                            }
                            else if (dat->graphic == KAKOPON_GRAPHIC::Buff)
                            {
                                for (int j = 0; j < 3; j++)
                                {
                                    if (j == 0)
                                    { //いったｎ
                                        continue;
                                    }

                                    DRAW_POLYGON_DAT dsdat = {};
                                    dsdat.size.x = sizeX;
                                    dsdat.size.y = sizeY;

                                    if (j == 0)
                                    {
                                        //dsdat.size.z = 0;
                                        dsdat.color = { 1.0f, 1.0f, 1.0f,
                                            alphaTmp * 0.25f
                                        };

                                        dsdat.texNo = 0;

                                        dsdat.texWH = {
                                            1.0f,
                                            1.0f
                                        };
                                    }
                                    else
                                    {
                                        dsdat.color = { 1.0f, 1.0f, 1.0f,
                                            alphaTmp
                                        };

                                        dsdat.texNo = m_TexId_kakoponEffect_Buff[j - 1];

                                        dsdat.texUV = {
                                            beforeSizeX_effect0,
                                            0.0f
                                        };
                                        if (j == 2)
                                        {
                                            dsdat.texUV.y = 0.01f * (float)m_Obj->frame;
                                        }

                                        dsdat.texWH = {
                                            sizeX_effect0_tmp,
                                            1.0f
                                        };
                                    }
                                    dsdat.shaderOpt.blendState = DPD_SHADER_OPT_BLEND_STATE_ADDCOLOR;

                                    draw_kakoponDat_Aura(
                                        dsdat,
                                        Float2ToFloat3(postmp),
                                        rottmp,
                                        j - 1,
                                        dat
                                    );
                                }
                                //{
                                //    DRAW_POLYGON_DAT dsdat = {};
                                //    dsdat.size.x = sizeX;
                                //    dsdat.size.y = sizeY;

                                //    dsdat.color = { 1.0f, 1.0f, 1.0f,
                                //        alphaTmp * 2.0f
                                //    };

                                //    dsdat.texNo = m_TexId_kakoponEffect_Add_Buff;

                                //    dsdat.texUV = {
                                //        beforeSizeX_effect0 + 0.01f * (float)m_Obj->frame,
                                //        0.0f
                                //    };

                                //    dsdat.texWH = {
                                //        sizeX_effect0_tmp,
                                //        1.0f
                                //    };

                                //    draw_kakoponDat_Aura(
                                //        dsdat,
                                //        Float2ToFloat3(postmp),
                                //        rottmp
                                //    );
                                //}
                            }
                            else if (dat->graphic == KAKOPON_GRAPHIC::Heal)
                            {
                                for (int j = 0; j < 3; j++)
                                {
                                    if (j == 0)
                                    { //いったｎ
                                        continue;
                                    }

                                    DRAW_POLYGON_DAT dsdat = {};
                                    dsdat.size.x = sizeX;
                                    dsdat.size.y = sizeY;

                                    if (j == 0)
                                    {
                                        //dsdat.size.z = 0;
                                        dsdat.color = { 1.0f, 1.0f, 1.0f,
                                            alphaTmp * 0.25f
                                        };

                                        dsdat.texNo = 0;

                                        dsdat.texWH = {
                                            1.0f,
                                            1.0f
                                        };
                                    }
                                    else
                                    {
                                        dsdat.color = { 1.0f, 1.0f, 1.0f,
                                            alphaTmp
                                        };

                                        dsdat.texNo = m_TexId_kakoponEffect_Heal[j - 1];

                                        dsdat.texUV = {
                                            beforeSizeX_effect0,
                                            0.0f
                                        };
                                        if (j == 2)
                                        {
                                            dsdat.texUV.y = 0.01f * (float)m_Obj->frame;
                                        }

                                        dsdat.texWH = {
                                            sizeX_effect0_tmp,
                                            1.0f
                                        };
                                    }
                                    dsdat.shaderOpt.blendState = DPD_SHADER_OPT_BLEND_STATE_ADDCOLOR;

                                    draw_kakoponDat_Aura(
                                        dsdat,
                                        Float2ToFloat3(postmp),
                                        rottmp,
                                        j - 1,
                                        dat
                                    );
                                }
                                //{
                                //    DRAW_POLYGON_DAT dsdat = {};
                                //    dsdat.size.x = sizeX;
                                //    dsdat.size.y = sizeY;

                                //    dsdat.color = { 1.0f, 1.0f, 1.0f,
                                //        alphaTmp * 2.0f
                                //    };

                                //    dsdat.texNo = m_TexId_kakoponEffect_Add_Heal;

                                //    dsdat.texUV = {
                                //        beforeSizeX_effect0 + 0.01f * (float)m_Obj->frame,
                                //        0.0f
                                //    };

                                //    dsdat.texWH = {
                                //        sizeX_effect0_tmp,
                                //        1.0f
                                //    };

                                //    draw_kakoponDat_Aura(
                                //        dsdat,
                                //        Float2ToFloat3(postmp),
                                //        rottmp
                                //    );
                                //}
                            }
                            else
                            {
                                for (int j = 0; j < 3; j++)
                                {
                                    if (j == 0)
                                    { //いったｎ
                                        continue;
                                    }

                                    DRAW_POLYGON_DAT dsdat = {};
                                    dsdat.size.x = sizeX;
                                    dsdat.size.y = sizeY;

                                    if (j == 0)
                                    {
                                        //dsdat.size.z = 0;
                                        dsdat.color = { 1.0f, 1.0f, 1.0f,
                                            alphaTmp * 0.25f
                                        };

                                        dsdat.texNo = 0;

                                        dsdat.texWH = {
                                            1.0f,
                                            1.0f
                                        };
                                    }
                                    else
                                    {
                                        dsdat.color = { 1.0f, 1.0f, 1.0f,
                                            alphaTmp
                                        };

                                        if (dat->graphic == KAKOPON_GRAPHIC::Main_Neo)
                                        {
                                            dsdat.texNo = m_TexId_kakoponEffect_General2[j - 1];
                                        }
                                        else
                                        {
                                            dsdat.texNo = m_TexId_kakoponEffect_General[j - 1];
                                        }

                                        dsdat.texUV = {
                                            beforeSizeX_effect0,
                                            0.0f
                                        };
                                        if (j == 2)
                                        {
                                            dsdat.texUV.y = 0.01f * (float)m_Obj->frame;
                                        }

                                        dsdat.texWH = {
                                            sizeX_effect0_tmp,
                                            1.0f
                                        };
                                    }
                                    dsdat.shaderOpt.blendState = DPD_SHADER_OPT_BLEND_STATE_ADDCOLOR;

                                    draw_kakoponDat_Aura(
                                        dsdat,
                                        Float2ToFloat3(postmp),
                                        rottmp,
                                        j - 1,
                                        dat
                                    );
                                }
                            }
                        }

                        beforeSizeX_effect0 += sizeX_effect0_tmp;

                    }
                }
            }
        }
    }

    if (dat->isConnected)
    {
        dat->frameDraw++;
    }
}

void GOE_Character_Player::draw_kakoponDat_Aura(
    DRAW_POLYGON_DAT& dsdat,
    Float3 postmp,
    float rottmp,
    int index,
    KAKOPON_DATA* dat
)
{
    PLAYER* player = GetPlayer(m_Obj);

    if(player != nullptr)
    {
        //DRAW_POLYGON_DAT dsdat = {};
        //dsdat.pos.x = 0.0f;
        //dsdat.pos.y = 0.0f;

        //dsdat.w = dist + 20.0f;
        //dsdat.h = 56.0f;

        //dsdat.size.x = sizeX;
        //dsdat.size.y = sizeY;

        dsdat.size.z = 1.0f;

        dsdat.rot3D.x = 90.0f;
        //dsdat.color = { 1.0f, 1.0f, 1.0f,
        //    //2.0f * ((float)revFrame / GOE_Character_Player::KAKOPON_COOLDOWN_FRAME)
        //    alphaTmp
        //};

        //dsdat.texNo = m_TexId_kakoponEffect_General[i];


        //dsdat.texNo = 0;
        dsdat.loadTexType = LOADTEXTURETYPE_MAIN;

        //dsdat.texUV = { 0.0f, 0.0f };
        //dsdat.texWH = { 1.0f, 1.0f };
        dsdat.startPos3D = { postmp.x, postmp.y, dsdat.size.y / -2 };
        dsdat.startRot3D = { 0.0f, 0.0f, 0.0f };
        dsdat.startRot3D.z = rottmp;
        dsdat.startScale = {
            1.0f,
            1.0f
        };
        dsdat.startScale3D = {
            1.0f,
            1.0f,
            1.0f
        };

        dsdat.shaderOpt.bloom = DPD_SHADER_OPT_BLOOM_NONE;

        //dsdat.color.w *= 0.95f;
        dsdat.color.w *= (0.1f + 0.9f * std::min((float)dat->vertexNum / 20.0f, 3.0f));
        dsdat.color.w *= 1.75f * m_MovieVisibleAlpha;

        DRAW_POLYGON_IN_CAMERA_EX_OPTION opt = {};
        opt.reverse = false;
        opt.fixed = false;
        //opt.priority = (DRAW_POLYGON_PRIORITY)(DRAW_POLYGON_PRIORITY_NML_d5 + i);
        //opt.priority = DRAW_POLYGON_PRIORITY_NORMAL;
        //opt.shaderType

        opt.depthBuffIgnore = true;
        opt.priority = DRAW_POLYGON_PRIORITY_NML_d1;

        //opt.priority = (DRAW_POLYGON_PRIORITY)(DRAW_POLYGON_PRIORITY_NML_u3 + index);

        DrawPolygonInCameraSingle(player->index, dsdat, opt);
    }
}

bool GOE_Character_Player::IsInput_NormalAttack() const
{
    return !IsDefeat() && Character::GetCtrlTrigger(
        m_CharaData, CONTROLLER_BUTTON_TYPE_A);
}
bool GOE_Character_Player::IsInput_NormalAttackPress() const
{
    return !IsDefeat() && Character::GetCtrlPress(
        m_CharaData, CONTROLLER_BUTTON_TYPE_A);
}
bool GOE_Character_Player::IsInput_KakoMain() const
{
    bool keyPushMain =
        Character::GetCtrlPress(m_CharaData, CONTROLLER_BUTTON_TYPE_R)
        || Character::GetCtrlPress(m_CharaData, CONTROLLER_BUTTON_TYPE_B)
        ;
    return !IsDefeat() && keyPushMain;
}
bool GOE_Character_Player::IsInput_KakoSub() const
{
    bool keyPushSubTest = !IsInput_KakoMain() && Character::GetCtrlPress(m_CharaData, CONTROLLER_BUTTON_TYPE_Y)
        ;
    return !IsDefeat() && keyPushSubTest;
}

bool GOE_Character_Player::IsPinch() const
{
    return m_Obj->status.hp <= m_Obj->status.hpMax * 0.4f;
}

void GOE_Character_Player::SetStern(int frame, bool enableEffect)
{
    m_DamageSternFrameMax = frame;
    m_DamageSternFrame = 0;
    m_DamageSternEffectFlag = enableEffect;

    //SetDownPose();
    SetDownPoseStart();
}

void GOE_Character_Player::SetKnockBack(float power, int sternf, GAME_OBJECT* atkrObj)
{
    if (sternf > 0)
    {
        SetStern(sternf);
        m_Obj->status.invFrame += sternf;
    }

    if (power > 0)
    {
        if (atkrObj != nullptr)
        {
            Float2 vec = MkF2(
                atkrObj->pos.x - m_Obj->pos.x,
                atkrObj->pos.y - m_Obj->pos.y
            );
            vec = NormalizeVector(vec);

            //GameObjectMoveRequest(recvObj,
            //    36,
            //    MkF3(
            //        recvObj->pos.x + vec.x * ONE_BLOCK * 18.0f,
            //        recvObj->pos.y + vec.y * ONE_BLOCK * 18.0f,
            //        recvObj->pos.z
            //    ),
            //    EasingOutQuad
            //);
            GameObjectMoveRequest(m_Obj,
                36,
                MkF3(
                    m_Obj->pos.x + -vec.x * ONE_BLOCK * power,
                    m_Obj->pos.y + -vec.y * ONE_BLOCK * power,
                    m_Obj->pos.z
                ),
                EasingOutQuad
            );
        }
    }

}

void GOE_Character_Player::EffectQuestion()
{
    if(m_EffectQuestionCooldown <= 0)
    { //えふぇくと

        Float3 postmp = m_Obj->pos;
        postmp.y += -200.0f * m_Obj->scale.y;
        postmp.z += -750.0f * m_Obj->scale.y;
        GAME_OBJECT* go = GO_CreateEffect(GO_EFFECT_TYPE_QUESTION, postmp);
        if (go != nullptr)
        {
            go->scale = MkF3(
                1.0f,
                1.0f,
                1.0f
            );
            GameObjectSetLifeTime(go, 50);
        }

        m_EffectQuestionCooldown = 60;
    }
}

void GOE_Character_Player::InitResetAction()
{

    ResetAction();

    m_CharaData->playingMotion.enable = false;

    m_DamageFrame = -1;
    m_DefeatFrame = 0;
    m_DefeatFrameAnim = 0;
    m_DefeatFrameAnimMax = 0;
    m_DamageSternFrame = -1;
    m_DamageSternFrameMax = 0;
    m_DeadNum = 0;
    m_GameoverDecision = false;
}

void GOE_Character_Player::ResetAction()
{
    m_SkillCooldownFrame = 0;

    for (int i = 0; i < KAKOPON_DAT_NUM; i++)
    {
        init_kakoponDat(&m_KakoDatList[i]);
    }
}
