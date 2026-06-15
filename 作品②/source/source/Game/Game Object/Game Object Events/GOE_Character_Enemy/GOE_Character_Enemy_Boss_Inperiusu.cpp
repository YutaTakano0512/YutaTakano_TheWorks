// ===================================================
// GOE_Character_Enemy_Boss_Inperiusu.cpp キャラクターオブジェクト
// 
// hara sougo  2024/12/10
// ===================================================

//#define NO_CONTROLLER

#include "../../../../Game/Configuration/Param.h"
#include "../../../../game_main.h"
#include "../../../Sprite/sprite.h"
#include "../../../Texture/texture.h"
#include "../../../Polygon/modelData.h"
#include "../../../../Game/Entities/Block/block.h"
#include "../../../Systems/collision.h"
#include "../../../Systems/easing.h"

#include "../../../Managers/bg.h"
#include "../../../Managers/gameManager.h"
#include "../../../Managers/scene.h"
#include "../../../../Scenes/SCENE_GAME.h"

#include "../../gameObject.h"
#include "../../gameObjectPrefab.h"

#include "../../../Systems/camera.h"
#include "../../../Systems/font.h"
#include "../../../Systems/ui.h"
#include "../../../Systems/ui_Game.h"
#include "../../../Game Stage/Stage_Base.h"
#include "../../../Game Stage/Stage_Ruins.h"

#include "../../../Entities/character.h"
#include "../../../Entities/player.h"

#include "../../../Managers/soundManager.h"
#include "../../../Game Events/GE_Gaya.h"

#include "../GOE_Effect.h"
#include "../GOE_Character.h"


#include "GOE_Character_Enemy_Miresu.h"
#include "GOE_Character_Enemy_Regatosu.h"
#include "GOE_Character_Enemy_Satellite.h"
#include "GOE_Character_EnemyProjectile.h"
#include "GOE_Character_Enemy_Boss_Inperiusu.h"

#include "../GOE_Character_Player.h"

#include "../GOE_Item.h"
#include "../GOE_ItemTransporter.h"
#include "../GOE_ObjectDynamic.h"
// ===================================================
// マクロ定義
// ===================================================

// ===================================================
// プロトタイプ宣言
// ===================================================

// ===================================================
// グローバル変数
// ===================================================



void GOE_Character_Enemy_Boss_Inperiusu::BecomeHard()
{
    m_IsHard = true;

    if (m_Tex_Model.enable)
    {
        UnloadTexData(m_Tex_Model);
    }
    m_Tex_Model = LoadTexData("TEX/enemy/boss_inperiusu_tex_2");

    if (m_Tex_Model_Mask.enable)
    {
        UnloadTexData(m_Tex_Model_Mask);
    }
    m_Tex_Model_Mask = LoadTexData("TEX/enemy/boss_inperiusu_tex_2_mask");

    m_shaderType = DPD_SHADER_TYPE_METAL_ENEMY_2;

    //Character::SetStatusData(m_Obj, Character::GetStatusDataFindByName(HARD_STATUS_NAME));

    Character::STATUS_DAT sttdat = Character::GetStatusDataFindByName(HARD_STATUS_NAME);

    m_Obj->status.hpMax = sttdat.status.hpMax;
    m_Obj->status.hp = m_Obj->status.hpMax;
    CalcHpMod(m_Obj, m_RetryCount);

    m_Obj->status.hpMin = m_Obj->status.hpMax * HARD_HP_BORDER;

    m_Obj->status.fullName = sttdat.status.fullName;
    m_Obj->status.nickName = sttdat.status.nickName;

    m_SuperBeamLimitF = FRAME_ONE_SECOND * 25.0f;
}

// ===================================================
// Init
// オブジェクトが生成されたとき
// ===================================================
void GOE_Character_Enemy_Boss_Inperiusu::Init()
{
	SuperGOE::Init();

    m_bgmIn = GetSoundData("SOUND/BGM/ISEKI_BOSS_IN");
    m_bgmInH = GetSoundData("SOUND/BGM/ISEKI_BOSS_OLD_IN");
    m_bgmLoop = GetSoundData("SOUND/BGM/ISEKI_BOSS_LOOP");
    m_bgmLoopH = GetSoundData("SOUND/BGM/ISEKI_BOSS_OLD_LOOP");


    //GraphicSet("TEX/enemy/boss_inperiusu/blade_down", 10, 24);

    //必要なテクスチャを読み込み

    //m_Tex_Main_Idle[0] = LoadTexData("TEX/enemy/boss_inperiusu/idle/in_idle_01");
    //m_Tex_Main_Idle[1] = LoadTexData("TEX/enemy/boss_inperiusu/idle/in_idle_08");
    //m_Tex_Main_Idle[2] = LoadTexData("TEX/enemy/boss_inperiusu/idle/in_idle_07");
    //m_Tex_Main_Idle[3] = LoadTexData("TEX/enemy/boss_inperiusu/idle/in_idle_06");
    //m_Tex_Main_Idle[4] = LoadTexData("TEX/enemy/boss_inperiusu/idle/in_idle_05");
    //m_Tex_Main_Idle[5] = LoadTexData("TEX/enemy/boss_inperiusu/idle/in_idle_04");
    //m_Tex_Main_Idle[6] = LoadTexData("TEX/enemy/boss_inperiusu/idle/in_idle_03");
    //m_Tex_Main_Idle[7] = LoadTexData("TEX/enemy/boss_inperiusu/idle/in_idle_02");

    //m_Tex_Main_Blade[0] = LoadTexData("TEX/enemy/boss_inperiusu/blade/in_blade_01");
    //m_Tex_Main_Blade[1] = LoadTexData("TEX/enemy/boss_inperiusu/blade/in_blade_08");
    //m_Tex_Main_Blade[2] = LoadTexData("TEX/enemy/boss_inperiusu/blade/in_blade_07");
    //m_Tex_Main_Blade[3] = LoadTexData("TEX/enemy/boss_inperiusu/blade/in_blade_06");
    //m_Tex_Main_Blade[4] = LoadTexData("TEX/enemy/boss_inperiusu/blade/in_blade_05");
    //m_Tex_Main_Blade[5] = LoadTexData("TEX/enemy/boss_inperiusu/blade/in_blade_04");
    //m_Tex_Main_Blade[6] = LoadTexData("TEX/enemy/boss_inperiusu/blade/in_blade_03");
    //m_Tex_Main_Blade[7] = LoadTexData("TEX/enemy/boss_inperiusu/blade/in_blade_02");

    //m_Tex_Main_Kidou = LoadTexData("TEX/enemy/boss_inperiusu/in_kidou");
    //m_Tex_Main_Beam1 = LoadTexData("TEX/enemy/boss_inperiusu/in_beam1_01");
    //m_Tex_Main_Beam2 = LoadTexData("TEX/enemy/boss_inperiusu/in_beam2_01");

    m_Tex_Model = LoadTexData("TEX/enemy/boss_inperiusu_tex");
    m_Tex_Model_Mask = LoadTexData("TEX/enemy/boss_inperiusu_tex_mask");
    m_Tex_Model_MaskGray = LoadTexData("TEX/enemy/boss_inperiusu_tex_mask");
    //m_Tex_Model = LoadTexData("TEX/enemy/boss_inperiusu_tex_2");
    //m_shaderType = DPD_SHADER_TYPE_METAL_ENEMY_2;
    m_Tex_Model_Blade = LoadTexData("TEX/effect/tex_blade");

    m_ModelIndex[0] = GetModelDataIndex("MODEL/boss_inperiusu/head");
    m_ModelIndex[1] = GetModelDataIndex("MODEL/boss_inperiusu/body1");
    m_ModelIndex[2] = GetModelDataIndex("MODEL/boss_inperiusu/body2");
    m_ModelIndex[3] = GetModelDataIndex("MODEL/boss_inperiusu/sholder_l");
    m_ModelIndex[4] = GetModelDataIndex("MODEL/boss_inperiusu/blade");
    m_ModelIndex[5] = GetModelDataIndex("MODEL/boss_inperiusu/sholder_r");

    m_Model_Bit = GetModelDataIndex("MODEL/boss_inperiusu/bit");


    //エフェクトとか
    m_Tex_Dis = LoadTexData("TEX/effect/NoiseTile");
    m_Tex_Warp = LoadTexData("TEX/effect/NoiseTile2");
    
    m_Tex_WarningPoint = LoadTexData("TEX/ui/in_game_master/1P_tri");

    m_Model_StanStar = GetModelDataIndex("MODEL/effect/stanstar");

    m_TexId_AttackEffect_s = LoadTexData("TEX/effect/boss_sl_front").textureId;

    m_Tex_BeamCharge = LoadTexData("TEX/effect/beam/tex_atlas_01").textureId;

   /* m_Tex_Beam1 = GetTexData("TEX/effect/beam/legacy/tex_beam_01EX").textureId;
    m_Tex_Beam2 = GetTexData("TEX/effect/beam/legacy/tex_beam_02EX").textureId;
    m_Tex_Beam3 = GetTexData("TEX/effect/beam/legacy/tex_beam_03EX").textureId;*/
    //m_Tex_Beam1 = LoadTexData("TEX/effect/beam/tex_beam_01").textureId;
    //m_Tex_Beam2 = LoadTexData("TEX/effect/beam/tex_beam_02").textureId;
    //m_Tex_Beam3 = LoadTexData("TEX/effect/beam/tex_beam_03").textureId;
    m_Tex_Beam1 = LoadTexData("TEX/effect/beam/sozai_01").textureId;
    m_Tex_Beam2 = LoadTexData("TEX/effect/beam/sozai_02").textureId;
    m_Tex_Beam3 = LoadTexData("TEX/effect/beam/sozai_03").textureId;
    //m_Tex_Beam1 = GetTexData("TEX/ui/in_game/charactor_icon").textureId;
    //m_Tex_Beam2 = GetTexData("TEX/ui/in_game/charactor_icon").textureId;
    //m_Tex_Beam3 = GetTexData("TEX/ui/in_game/charactor_icon").textureId;

    m_Model_Beam = GetModelDataIndex("MODEL/beam_01");
    //m_Model_Beam = GetModelDataIndex("MODEL/beam_02");
    //m_Model_Beam = GetModelDataIndex("MODEL/beamEX");

    //m_Tex_Misile[AXIS_2D_LEFT] = LoadTexData("TEX/enemy/misile/misile_left").textureId;
    //m_Tex_Misile[AXIS_2D_RIGHT] = LoadTexData("TEX/enemy/misile/misile_right").textureId;
    //m_Tex_Misile[AXIS_2D_UP] = LoadTexData("TEX/enemy/misile/misile_back").textureId;
    //m_Tex_Misile[AXIS_2D_DOWN] = LoadTexData("TEX/enemy/misile/misile_front").textureId;

    m_Model_Fire_Misile = GetModelDataIndex("MODEL/Re_misile_fire");
    m_Tex_Misile_Fire_Model = LoadTexData("TEX/enemy/misile/misile_fire2").textureId;
    m_Model_Misile = GetModelDataIndex("MODEL/Re_misile_01");
    m_Tex_Misile_Model = LoadTexData("TEX/enemy/misile/misile_color").textureId;

    m_Tex_Warning[0] = LoadTexData("TEX/ui/in_game/warning_010").textureId;
    m_Tex_Warning[1] = LoadTexData("TEX/ui/in_game/warning_030").textureId;
    m_Tex_Warning[2] = LoadTexData("TEX/ui/in_game/warning_040").textureId;
    

    m_Tex_Barrier_Scroll1 = LoadTexData("TEX/effect/barrier/tex_br_wave").textureId;
    m_Tex_Barrier_Scroll2 = LoadTexData("TEX/effect/barrier/tex_br_wave02").textureId;
    m_Tex_Barrier_Fresnel1 = LoadTexData("TEX/effect/barrier/tex_br_noise").textureId;
    m_Tex_Barrier_Fresnel2 = LoadTexData("TEX/effect/barrier/tex_br_grad02").textureId;
    m_Tex_Barrier_Ground1 = LoadTexData("TEX/effect/barrier/tex_br_grad").textureId;
    m_Tex_Barrier_Ground2 = LoadTexData("TEX/effect/barrier/tex_dedal").textureId;
    m_Tex_Barrier_Bg = LoadTexData("TEX/effect/barrier/tex_br_bg").textureId;
    m_Model_Sphere = GetModelDataIndex("MODEL/effect/FBX_Sphere_03");

    m_Tex_EnergyBall = LoadTexData("TEX/effect/tex_energy_01").textureId;

    m_Tex_PowerBg = LoadTexData("TEX/effect/tex_anim_02").textureId;
    m_Tex_Vignette = LoadTexData("TEX/effect/vignette").textureId;
    m_Tex_ScreenBreak = LoadTexData("TEX/effect/eff_break_01");
    m_Tex_SuperBeamChargeRing = LoadTexData("TEX/effect/eff_anai_ring");
    m_Tex_ScKakoEffect = LoadTexData("TEX/effect/eff_gamen02");

    m_Tex_StartingEff_Eye = LoadTexData("TEX/effect/boss/eff_eye");
    m_Tex_StartingEff_Eye2 = LoadTexData("TEX/effect/boss/eff_eye_02");
    m_Tex_StartingEff_Bk1 = LoadTexData("TEX/effect/boss/tex_inp_bk_01");
    m_Tex_StartingEff_Bk2 = LoadTexData("TEX/effect/boss/tex_inp_bk_02");
    m_Tex_StartingEff_Name = LoadTexData("TEX/effect/boss/tex_inp_name_01");
    m_Tex_StartingEff_NameH = LoadTexData("TEX/effect/boss/tex_inp_name_02");

    m_Tex_cursor_000 = LoadTexData("TEX/ui/in_game_master/cursor/cursor_000");
    m_Tex_cursor_010 = LoadTexData("TEX/ui/in_game_master/cursor/cursor_010");
    m_Tex_cursor_020 = LoadTexData("TEX/ui/in_game_master/cursor/cursor_020");

    

    m_Tex_Shadow = LoadTexData("TEX/effect/shadow");

    //m_Tex_Defeat = LoadTexData("TEX/enemy/boss_inperiusu/boss_dis_01");
    
	//Character::PlayMotionCharacterGameObject(
	//	goDat,
	//	Character::GetCharacterMotionFindByName(Character::GetCharacterMainData(goDat->dataIndex), "IDLE"),
	//	true,
	//	nullptr,
	//	60
	//);

    m_ShaderIndex_CutInName_Vertex = DX_GetShaderIndex("SHADER/vs_general");
    m_ShaderIndex_CutInName_Pixel = DX_GetShaderIndex("SHADER/general/ps_cutin_name");


    //GraphicSet(m_Tex_Main_Kidou, 6, 6);
    //GraphicSpriteSet(m_Tex_Main_Idle[0], 4, 4);
    m_Obj->graphic.pos3d.y += 130.0f;
    m_Obj->graphic.pos3d.z += 200.0f;

    //スプライト非表示
    m_Obj->graphic.hidden = true;


    m_Obj->graphic.shaderOpt.outline = DPD_SHADER_OPT_OUTLINE_NONE;

    m_Obj->collision.atk.enable = false;
    m_Obj->collision.rec.enable = false;
    m_Obj->status.statusType = GAME_OBJECT_STATUS_TYPE_VOID;

    m_Obj->size = MkF2(ONE_BLOCK * 2, ONE_BLOCK * 2);

    for (int i = 0; i < MODEL_NUM; i++)
    {
        m_ModelTransSca[i].x = 1.0f;
        m_ModelTransSca[i].y = 1.0f;
        m_ModelTransSca[i].z = 1.0f;
    }


    //起動前ポーズ
    {
        m_ModelTransPos[0].y = 350.0f;
        m_ModelTransPos[0].z = 75.0f;
        //m_ModelTransPos[0].y = 300.0f;
        //m_ModelTransPos[0].z = 250.0f;
        //m_ModelTransRot[0].x = 45.0f;

        m_ModelTransPos[1].y = 100.0f;
        m_ModelTransRot[1].y = 360.0f * 1;
        m_ModelTransRot[2].y = -360.0f * 2;

        m_ModelTransPos[3].x = 60.0f;
        m_ModelTransPos[3].y = 550.0f;
        m_ModelTransRot[3].x = 360.0f * 1;
        m_ModelTransRot[3].z = -35.0f;

        m_ModelTransPos[5].x = -80.0f;
        m_ModelTransPos[5].y = 550.0f;
        m_ModelTransRot[5].x = 360.0f * 1;
        m_ModelTransRot[5].z = 45.0f;
    }

    {
        GAME_OBJECT_COLLISION_RECIEVE_CHILD* col = m_Obj->collision.rec.collision + 0;
        col->size = MkF3(ONE_BLOCK * 8.0f, ONE_BLOCK * 8.0f, ONE_BLOCK * 20.0f);
    }

    //{
    //    Stage_Base* stg = GetCurrentStageSceneGame();
    //    if (stg != nullptr)
    //    {
    //        m_RetryCount = stg->GetRetryCountCurrentWave();
    //    }
    //}

    CalcHpMod(m_Obj, m_RetryCount);
}

// ===================================================
// FirstFrame
// 最初のフレームのみ実行
// ===================================================
void GOE_Character_Enemy_Boss_Inperiusu::FirstFrame()
{
	SuperGOE::FirstFrame();


}

// ===================================================
// Uninit
// オブジェクトが削除されるとき
// ===================================================
void GOE_Character_Enemy_Boss_Inperiusu::Uninit()
{
    UnloadTexData(m_Tex_cursor_000);
    UnloadTexData(m_Tex_cursor_010);
    UnloadTexData(m_Tex_cursor_020);

    UnloadTexData(m_Tex_Beam1);
    UnloadTexData(m_Tex_Beam2);
    UnloadTexData(m_Tex_Beam3);

    UnloadTexData(m_Model_Beam);

    //UnloadTexData(m_Tex_Misile[AXIS_2D_LEFT]);
    //UnloadTexData(m_Tex_Misile[AXIS_2D_RIGHT]);
    //UnloadTexData(m_Tex_Misile[AXIS_2D_UP]);
    //UnloadTexData(m_Tex_Misile[AXIS_2D_DOWN]);
    UnloadTexData(m_Tex_Misile_Fire_Model);
    UnloadTexData(m_Tex_Misile_Model);

    UnloadTexData(m_Tex_Warning[0]);
    UnloadTexData(m_Tex_Warning[1]);
    UnloadTexData(m_Tex_Warning[2]);

    UnloadTexData(m_Tex_Dis);

 /*   UnloadTexData(m_Tex_Main_Idle[0]);
    UnloadTexData(m_Tex_Main_Idle[1]);
    UnloadTexData(m_Tex_Main_Idle[2]);
    UnloadTexData(m_Tex_Main_Idle[3]);
    UnloadTexData(m_Tex_Main_Idle[4]);
    UnloadTexData(m_Tex_Main_Idle[5]);
    UnloadTexData(m_Tex_Main_Idle[6]);
    UnloadTexData(m_Tex_Main_Idle[7]);

    UnloadTexData(m_Tex_Main_Blade[0]);
    UnloadTexData(m_Tex_Main_Blade[1]);
    UnloadTexData(m_Tex_Main_Blade[2]);
    UnloadTexData(m_Tex_Main_Blade[3]);
    UnloadTexData(m_Tex_Main_Blade[4]);
    UnloadTexData(m_Tex_Main_Blade[5]);
    UnloadTexData(m_Tex_Main_Blade[6]);
    UnloadTexData(m_Tex_Main_Blade[7]);

    UnloadTexData(m_Tex_Main_Kidou);
    UnloadTexData(m_Tex_Main_Beam1);
    UnloadTexData(m_Tex_Main_Beam2);*/
    UnloadTexData(m_Tex_Model);
    UnloadTexData(m_Tex_Model_Blade);
    UnloadTexData(m_Tex_Model_Mask);
    
    UnloadTexData(m_Tex_EnergyBall);

    UnloadTexData(m_Tex_PowerBg);

    UnloadTexData(m_Tex_Vignette);

    UnloadTexData(m_Tex_ScreenBreak);
    UnloadTexData(m_Tex_SuperBeamChargeRing);

    UnloadTexData(m_Tex_StartingEff_Eye);
    UnloadTexData(m_Tex_StartingEff_Eye2);
    UnloadTexData(m_Tex_StartingEff_Bk1);
    UnloadTexData(m_Tex_StartingEff_Bk2);
    UnloadTexData(m_Tex_StartingEff_Name);
    UnloadTexData(m_Tex_StartingEff_NameH);

    UnloadTexData(m_Tex_Shadow);

	SuperGOE::Uninit();
}

// ===================================================
// Update
// 毎フレーム実行
// ===================================================
void GOE_Character_Enemy_Boss_Inperiusu::Update()
{
    update_misile_object();

    for (int i = 0; i < MIRRORGE_STATE_MAX; i++)
    {
        if (m_MirrorgeState[i].enable)
        {
            if (m_MirrorgeState[i].inFrame < MIRRORGE_STATE_INFRAME_MAX)
            {
                m_MirrorgeState[i].inFrame++;
            }
        }
        else
        {
            if (m_MirrorgeState[i].inFrame > 0)
            {
                m_MirrorgeState[i].inFrame--;
            }
        }
    }

	SuperGOE::Update();

    if((!m_StartFlag && !m_StartedFlag) || m_SuperBeamBarrierFlag)
    {
        float indis = (m_SuperBeamBarrierFlag ? ONE_BLOCK * 5.0f: ONE_BLOCK * 1.6f) * ((m_Obj->scale.x + m_Obj->scale.y + m_Obj->scale.z) / 3);
        std::map<int, GAME_OBJECT*> objs = GetGameObjectFindByDistance(m_Obj->pos,
            indis
        );
        for (int i = 0; i < objs.size(); i++)
        {
            if (objs[i] != nullptr 
                && objs[i]->tags.size() > 0
                && objs[i]->tags[0].find("PLAYER_") == 0)
            {
                float dis = 0;
                do {
                    dis = CalculateDistance(objs[i]->pos, m_Obj->pos);
                    Float3 addvel = {};
                    addvel.x = objs[i]->pos.x - m_Obj->pos.x;
                    addvel.y = objs[i]->pos.y - m_Obj->pos.y;
                    //addvel.z = objs[i]->pos.z - m_Obj->pos.z;
                    addvel = NormalizeVector(addvel);

                    objs[i]->pos.x += addvel.x * 1.0f;
                    objs[i]->pos.y += addvel.y * 1.0f;
                    //objs[i]->pos.z += addvel.z;
                } while (dis < indis);

                if (!m_StartedFlag)
                {
                    //GameObjectHitStop(objs[i], 30);

                    GOE_Character_Player* goe =
                        dynamic_cast<GOE_Character_Player*>(objs[i]->events.source);  // ダウンキャスト

                    if (goe != nullptr)
                    { //えふぇくと
                        goe->EffectQuestion();
                    }
                }
            }
        }
    }

    if (!m_StartedFlag)
    {
        //開始フラグがたったら初回イベントを動かす
        if (m_StartFlag)
        {
            update_start();
        }
    }

    if (m_StartEyeEffFrame >= 0)
    {
        m_StartEyeEffFrame--;
        if (m_StartEyeEffFrame < 0)
        {
            m_StartEyeEffFrame = 0;
            m_StartEyeEffFrameMax = 0;
        }
    }
    if (m_FlashScreenMaskFrame > 0)
    {
        m_FlashScreenMaskFrame--;
    }
    if (m_KakoEfScreenMaskFrame > 0)
    {
        m_KakoEfScreenMaskFrame--;
    }


    if (m_IsHard)
    {
        if (m_Obj->status.hp <= m_Obj->status.hpMin
            && !m_HardPhase2Flag
            )
        {
            //m_Obj->hitReaction = {};
            //m_Obj->hitStop = {};
            //m_Obj->moveRequ.enable = false;

            m_Phase2StartFrame = 0;
            m_CurrentState = ActionState::Phase2Start;

            m_HardPhase2Flag = true;

            deadstop_event();
            //return;
        }
    }

    if (m_WarpFrame >= 0)
    {
        if (m_WarpFrame == 0)
        {
            m_Obj->pos = m_WarpPosAfter;
            m_CurrentAxisPrivate = m_WarpAxisAfter;

            {
                float spd = (abs(m_BeforeVecBuf.x) + abs(m_BeforeVecBuf.y)) / 2;
                float rad = CalculateDegToRad(m_CurrentAxisPrivate);
                m_BeforeVecBuf.x = -sin(rad) * spd;
                m_BeforeVecBuf.y = cos(rad) * spd;

                m_BeforeVec = m_BeforeVecBuf;
                m_HeadVecBuf = m_BeforeVec;
                m_HeadVec = m_HeadVecBuf;
            }
        }

        m_WarpFrame++;
        if (m_WarpFrame > m_WarpFrameMax)
        {
            m_WarpFrame = -1;
        }
    }
}

// ===================================================
// UpdateActive
// 毎フレーム実行
// ただし、ヒットストップ中などの
// 非アクティブ時は実行されない
// ===================================================
void GOE_Character_Enemy_Boss_Inperiusu::UpdateActive()
{
    //update_misile_object();

    if (m_WarpFrame >= 0)
    {
        return;
    }

	SuperGOE::UpdateActive();

    float turnspeed = 0.03;

    //開始前はいろいろ実行しない
    if (m_StartedFlag)
    {

        { //向きの設定

            //向きを計算
            constexpr float rotp = 360.0f / 64;
            float rot =
                DegNormalize(
                    CalculateRadToDeg(
                        atan2(m_BeforeVec.y, m_BeforeVec.x))
                    + 180.0f + 90.0f + rotp / 2
                );

            //動いていたら向きに合わせて方向をセット
            if (abs(m_BeforeVec.x) > 0.01f || abs(m_BeforeVec.y) > 0.01f)
            {
                m_CurrentAxisPrivate = (int)(rot / rotp) * rotp;
                //m_CurrentAxisHead = (int)(rot / rotp) * rotp;
            }

            //DebugPrintf("%f  %f\n", rot, (rot / rotp));
        }
        { //向きの設定

            //向きを計算
            constexpr float rotp = 360.0f / 64;
            float rot = 
                DegNormalize(
                    CalculateRadToDeg(
                        atan2(m_HeadVec.y, m_HeadVec.x))
                    + 180.0f + 90.0f + rotp / 2
                    );

            //動いていたら向きに合わせて方向をセット
            if (abs(m_HeadVec.x) > 0.01f || abs(m_HeadVec.y) > 0.01f)
            {
                m_CurrentAxisHead = (int)(rot / rotp) * rotp;
            }

            //DebugPrintf("%f  %f\n", rot, (rot / rotp));
        }

        if (m_Obj->status.deadFlag)
        { //たひ
            if (m_Dead_Frame == 0)
            {
                m_Obj->hitReaction = {};
                m_Obj->hitStop = {};
                m_Obj->moveRequ.enable = false;
                m_Obj->pos.z = 0.0f;

                m_KakoEfScreenMaskFrameMax = 40;
                m_KakoEfScreenMaskFrame = m_KakoEfScreenMaskFrameMax;

                //ボスのHPを削り切ったらリザルトに送る
                //if (HasTagGameObject(m_Obj, "ENEMY_BOSS_INPERIUSU"))
                {
                    //プレイヤー以外を削除する
                    for (int i = 0; i < GAME_OBJECT_MAX; i++)
                    {
                        GAME_OBJECT* ob = GetGameObject(i);
                        if (IsActiveGameObject(ob)
                            && ob != m_Obj
                            && (ob->tags.size() <= 0
                                || !ob->tags[0].find("PLAYER") == 0)
                            )
                        {
                            DeleteGameObject(ob);
                        }
                    }


                    GameManagerResultAddScore(
                        m_IsHard ? 10000 : 5000,
                        RESULT_SCORE_EX_TYPE_CLEAR
                    );

                    if (m_SuperBeamFinish)
                    {
                        SoundEffectStopAllRequest();

                        SoundEffectPlayRequest(
                            GetSoundData("SOUND/SE/player/attack/activation"),
                            2.0f, 0.0f, 0.75f
                        );

                        //SEを再生
                        SoundEffectPlayRequest(
                            GetSoundData("SOUND/SE/enemy/attack/Big_Explosion"),
                            7.8f, 0, 0.7f);
                        SoundEffectPlayRequest(SOUND_TYPE_SE_9, 6.0f, 0, 0.5f);
                        SoundEffectPlayRequest(SOUND_TYPE_SE_9, 4.0f, 30, 0.3f);
                        SoundEffectPlayRequest(SOUND_TYPE_SE_9, 3.0f, 60, 0.9f);
                        SoundEffectPlayRequest(SOUND_TYPE_SE_9, 2.0f, 90, 0.9f);
                        SoundEffectPlayRequest(SOUND_TYPE_SE_9, 1.0f, 120, 0.9f);

                    }
                    else
                    {
                        SoundBGMStopRequest();
                        SoundEffectStopAllRequest();

                        SoundEffectPlayRequest(
                            GetSoundData("SOUND/SE/player/attack/activation"),
                            2.0f, 0.0f, 0.75f
                        );
                        SoundEffectPlayRequest(SOUND_TYPE_SE_9, 6.0f, 0, 0.9f);
                        SoundEffectPlayRequest(SOUND_TYPE_SE_9, 4.0f, 30, 0.9f);
                        SoundEffectPlayRequest(SOUND_TYPE_SE_9, 3.0f, 60, 0.9f);
                        SoundEffectPlayRequest(SOUND_TYPE_SE_9, 2.0f, 90, 0.9f);
                        SoundEffectPlayRequest(SOUND_TYPE_SE_9, 1.0f, 120, 0.9f);

                    }

                    SetUIEventTimer_Game(false);
                    ShowBossbar_Game(GetGameObjectIndex(m_Obj), false, "");

                    m_BeforeVecBuf = NormalizeVector({ -0.2f, 1.0f, 0.0f });
                    m_BeforeVec = m_BeforeVecBuf;

                    m_CurrentState = ActionState::Idle;

                    StartAnim(AnimState::Damage, m_Dead_FrameMax * 2, true);

                    //CameraFlashRequestAll(
                    //    300,
                    //    EasingReverse,
                    //    //MkF4(0.6f, 0.3f, 1.0f, 0.8f),
                    //    MkF4(100.0f, 100.0f, 100.0f, 1.0f),
                    //    //DPD_SHADER_OPT_BLEND_STATE_DEFAULT
                    //    DPD_SHADER_OPT_BLEND_STATE_ADDCOLOR
                    //);
                    //GameStopRequest(60 * 3);

                    //GameManagerSetSceneRequest(SCENE_RESULT, 0, true);
                    //SetSceneFade(SCENE_RESULT);

                    //CameraZoomRequestAll(
                    //    0.04f,
                    //    //Float3ToFloat2(m_Obj->pos),
                    //    m_Obj->pos,
                    //    EasingInOutSine,
                    //    0.45f
                    //);
                    //CameraZoomResetRequestAll(
                    //    0.02f,
                    //    EasingInOutSine,
                    //    300
                    //);

                    SetCameraModeAll(CAMERA_MODE_NORMAL);

                    GE_GayaCancelRequestAll();

                    GE_GayaStartRequest("game_clear_mayu", 40);
                }

                for (int i = 0; i < PLAYER_MAX; i++)
                {
                    PLAYER* pl = GetPlayer(i);
                    if (pl != nullptr)
                    {
                        GAME_OBJECT* plObj = pl->obj;
                        if (plObj != nullptr)
                        {
                            plObj->pos.z = 32767.0f;
                        }
                    }
                }

                Float3 zoomPos = m_Obj->pos;
                zoomPos.z += 32000.0f;

                Float3 zoomPosTa = m_Obj->pos;
                zoomPosTa.z += -14000.0f;

                CameraTargetMoveRequestAll(8,
                    zoomPosTa,
                    EasingInOutSine,
                    CAMERA_MOVE_TYPE_SMOOTH
                );
                CameraMoveRequestAll(8,
                    zoomPos,
                    0.0f,
                    MkF2(1.0f, 1.0f),
                    //0.35f,
                    0.5f,
                    EasingInOutSine,
                    CAMERA_MOVE_TYPE_SMOOTH
                );
                {
                    Stage_Base* stg = GetCurrentStageSceneGame();
                    if (stg != nullptr)
                    {
                        stg->Clear();
                    }
                }
            }
            if (m_Dead_Frame == 8)
            {
                Float3 zoomPos = m_Obj->pos;
                zoomPos.x += 320.0f;
                zoomPos.z += 32000.0f;

                Float3 zoomPosTa = m_Obj->pos;
                zoomPosTa.x += -7500.0f;
                zoomPosTa.z += -17000.0f;

                CameraTargetMoveRequestAll(m_Dead_FrameMax,
                    zoomPosTa,
                    nullptr,
                    CAMERA_MOVE_TYPE_SMOOTH
                );
                CameraMoveRequestAll(m_Dead_FrameMax,
                    zoomPos,
                    0.0f,
                    MkF2(1.0f, 1.0f),
                    //0.35f,
                    0.55f,
                    nullptr,
                    CAMERA_MOVE_TYPE_SMOOTH
                );
            }
            if (m_Dead_Frame == 60)
            {
                CameraFlashRequestAll(
                    10,
                    EasingInOutSine,
                    MkF4(10.0f, 10.0f, 10.0f, 1.0f),
                    DPD_SHADER_OPT_BLEND_STATE_ADDCOLOR
                );
            }

            m_Dead_Frame++;
        }
        else if (m_JetFrame >= 0)
        {
            int firstMovF = m_JetFrameMax;

            int upMovF = (int)(firstMovF * 0.35);
            int downMovF = (int)(firstMovF * 0.4);

            if (m_JetFrame == 0)
            { //初回

                //SEを再生
                SoundEffectPlayRequest(GetSoundData("SOUND/SE/enemy/move/Inperius_Jet_Start"), 2.2f, 5);

                //SEを再生
                SoundEffectPlayRequest(GetSoundData("SOUND/SE/enemy/move/Inperius_Jet"), 
                    2.2f, 0, 1.5f
                );

                GameObjectMoveRequest(m_Obj, upMovF,
                    MkF3(m_Obj->pos.x, m_Obj->pos.y, ONE_BLOCK * -100.0f),
                    EasingInQuart
                );
                //m_BeforeVecBuf = MkF3(0.0f, 1.0f, 0.0f);
                {
                    float spd = (abs(m_BeforeVecBuf.x) + abs(m_BeforeVecBuf.y)) / 2;
                    float rad = CalculateDegToRad(m_JetAxisAfter);
                    m_BeforeVecBuf.x = -sin(rad) * spd;
                    m_BeforeVecBuf.y = cos(rad) * spd;
                }

                if (m_Anim_Frame <= 0)
                {
                    StartAnim(AnimState::BeamBefore, ATTACK_BEAM_BEFORE_FIXED_FRAME + 1, true);
                }
            }
            if (m_JetFrame == ATTACK_BEAM_BEFORE_FIXED_FRAME)
            {
                if (m_Anim_State == AnimState::BeamBefore)
                {
                    StartAnim(AnimState::Beam, upMovF + 1, true);
                }
            }

            if (m_JetFrame < firstMovF / 2
                && m_JetFrame % 6 == 0
                )
            {
                CameraShakeRequestAll(CAMERA_SHAKE_SMALL_FRAME, CAMERA_SHAKE_MIDIUM_POWER);

                for (int i = 0; i < 5; i++)
                {
                    Float3 efPos1 = {};
                    efPos1.x += sin(2.0f * SYS_GENERAL_PI / 5 * i + (float)m_Obj->frame / 7);
                    efPos1.y += cos(2.0f * SYS_GENERAL_PI / 5 * i + (float)m_Obj->frame / 7);

                    Float3 efPos2 = m_Obj->pos;
                    efPos2.x += efPos1.x * 90.0f;
                    efPos2.y += efPos1.y * 90.0f;

                    GAME_OBJECT* go = GO_CreateEffect(GO_EFFECT_TYPE_SMOKE, efPos2);
                    if (go != nullptr)
                    {
                        go->vel = efPos1;
                        go->vel.x *= 45.0f;
                        go->vel.y *= 45.0f;
                        go->vel.z = 12.0f;
                        go->friction = 0.03f;

                        //go->graphic.rot3d.x = 90.0f;
                        go->graphic.color.w = 0.8f;
                        go->scale = MkF3(
                            3.0f,
                            3.0f,
                            3.0f
                        );
                        GameObjectSetLifeTime(go, 40);
                    }
                }
            }
            //if (m_JetFrame == firstMovF * 0.2)
            //{
            //    //SEを再生
            //    SoundEffectPlayRequest(GetSoundData("SOUND/SE/enemy/move/Inperius_Jet"), 6.5f, 0);
            //}


            if (m_JetFrame == firstMovF / 2)
            { //

                //SEを再生
                //SoundEffectPlayRequest(GetSoundData("SOUND/SE/enemy/move/Inperius_Jet"), 3.8f, 10);

                m_BeforeVec = m_BeforeVecBuf;

                m_Obj->pos = MkF3(
                    m_JetPosAfter.x,
                    m_JetPosAfter.y,
                    ONE_BLOCK * -100.0f
                );
                GameObjectMoveRequest(m_Obj, downMovF + FRAME_ONE_SECOND * -0.2f,
                    m_JetPosAfter,
                    EasingInOutQuart
                );
                if (m_Anim_State == AnimState::Beam)
                {
                    StartAnim(AnimState::Beam, downMovF, true);
                }
            }
            //if (m_JetFrame == firstMovF / 2 + FRAME_ONE_SECOND)
            //{ //
            //    event_super_beam_before_movieFlag();
            //}
            if (m_JetFrame >= firstMovF / 2
                && m_JetFrame < firstMovF * 0.8
                && m_JetFrame % 8 == 0
                )
            {
                CameraShakeRequestAll(
                    CAMERA_SHAKE_MIDIUM_FRAME,
                    CAMERA_SHAKE_MIDIUM_POWER
                );

                for (int i = 0; i < 5; i++)
                {
                    Float3 efPos1 = {};
                    efPos1.x += sin(2.0f * SYS_GENERAL_PI / 5 * i + (float)m_Obj->frame / 7);
                    efPos1.y += cos(2.0f * SYS_GENERAL_PI / 5 * i + (float)m_Obj->frame / 7);

                    Float3 efPos2 = m_Obj->pos;
                    efPos2.x += efPos1.x * 90.0f;
                    efPos2.y += efPos1.y * 90.0f;
                    efPos2.z = -50.0f;

                    GAME_OBJECT* go = GO_CreateEffect(GO_EFFECT_TYPE_SMOKE, efPos2);
                    if (go != nullptr)
                    {
                        go->vel = efPos1;
                        go->vel.x *= 110.0f;
                        go->vel.y *= 110.0f;
                        go->vel.z = -72.0f;
                        go->friction = 0.08f;

                        //go->graphic.rot3d.x = 90.0f;
                        go->graphic.color.w = 0.6f;
                        go->scale = MkF3(
                            3.0f,
                            3.0f,
                            3.0f
                        );
                        GameObjectSetLifeTime(go, 32);
                    }
                }
            }
            if (m_JetFrame == firstMovF * 0.8)
            {
                //SEを再生
                SoundEffectPlayRequest(GetSoundData("SOUND/SE/enemy/move/Inperius_Jet_End"), 1.6f, 0);
            }


            m_JetFrame++;
            if (m_JetFrame >= firstMovF)
            {
                m_JetFrame = -1;
            }

            //return;
        }
        else if (m_SimpleMoveFrame >= 0)
        {
            if (m_SimpleMoveFrame == 0)
            { //初回

                GameObjectMoveRequest(m_Obj, m_SimpleMoveFrameMax,
                    m_SimpleMovePosAfter,
                    EasingInOutSine
                );
            }
            {
                m_BeforeVecBuf = NormalizeVector(m_Obj->vel);
            }

            m_SimpleMoveFrame++;
            if (m_SimpleMoveFrame >= m_SimpleMoveFrameMax)
            {
                if (abs(m_SimpleMoveAxisAfter - m_CurrentAxisPrivate) > 1.0f)
                {
                    StartTurn(m_SimpleMoveAxisAfter, 0.05f);
                }
                m_SimpleMoveFrame = -1;
            }

            //return;
        }
        else if (m_TurnSpeed > 0)
        {
            turnspeed = m_TurnSpeed;
            m_BeforeVecBuf = m_TurnVecAfter;

            //if (
            //    abs(m_BeforeVecBuf.x - m_BeforeVec.x) < 0.1f
            //    && abs(m_BeforeVecBuf.y - m_BeforeVec.y) < 0.1f
            //    && abs(m_BeforeVecBuf.z - m_BeforeVec.z) < 0.1f
            //)
            //{
            //    m_BeforeVec = m_BeforeVecBuf;
            //    m_TurnSpeed = 0;
            //}
            if (
                abs(m_BeforeVecBuf.x - m_BeforeVec.x) < 0.5f
                && abs(m_BeforeVecBuf.y - m_BeforeVec.y) < 0.5f
                && abs(m_BeforeVecBuf.z - m_BeforeVec.z) < 0.5f
                )
            {
                m_BeforeVec = m_BeforeVecBuf;
                m_TurnSpeed = 0;
            }
        }
        else
        {

            if (!m_CharaData->playingMotion.enable)
            {
                Character::PlayMotionCharacterGameObject(
                    m_CharaData,
                    Character::GetCharacterMotionFindByName(Character::GetCharacterMainData(m_CharaData->dataIndex), "IDLE"),
                    true,
                    //(int*)(&m_CurrentAxisEx) //スプライトの番号を向きに依存させる
                    nullptr,
                    FRAME_ONE_SECOND * 2
                );
            }

            //if (m_CharaData->playingMotion.motion->name == "IDLE")
            //{
            //    GraphicSpriteSet(m_Tex_Main_Idle[std::max(m_CurrentAxisEx - 1, 0)], 4, 4);
            //}

            //行動パターン
            {
                if (m_TargetObjIdentifier.objCreateFrame > 0)
                { //対象がいるとき

                    GAME_OBJECT* targetObject = GetGameObjectFindByIdentifier(m_TargetObjIdentifier);
                    if (targetObject != nullptr)
                    {
                        m_TargetDistance = CalculateDistance(targetObject->pos, m_Obj->pos);
                        m_TargetVec = NormalizeVector(MkF3(
                            targetObject->pos.x - m_Obj->pos.x,
                            targetObject->pos.y - m_Obj->pos.y,
                            0.0f
                        ));

                        {
                            //対象への向きを入れる
                            m_HeadVecBuf = NormalizeVector(MkF3(
                                targetObject->pos.x - m_Obj->pos.x,
                                targetObject->pos.y - m_Obj->pos.y,
                                //targetObject->pos.z - (m_Obj->pos.z - 120.0f * m_Obj->scale.y)
                                targetObject->pos.z - (m_Obj->pos.z
                                    + (m_ModelTransPos[0].y - 120.0f) * m_Obj->scale.y)

                            ));
                        }

                        update_action_pattern();


                        //タゲ外し
                        if (IsDefeatGameObject(targetObject))
                        {
                            m_TargetObjIdentifier = {};
                            m_TargetFrame = -1;
                        }
                    }

                    //タゲはいってからのカウント
                    m_TargetFrame++;
                }
                else //対象がいないとき
                {
                    //索敵
                    {
                        //マス内のプレイヤーを検索
                        std::vector<PLAYER> players
                            = GetPlayesrFindByDistance(m_Obj->pos, ONE_BLOCK * 1000);

                        auto it = std::find_if(
                            players.begin(),
                            players.end(),
                            [](PLAYER buf) {
                                return IsDefeatGameObject(buf.obj);
                            }
                        );
                        if (it != players.end())
                        {
                            players.erase(it);
                        }


                        //マス以内にプレイヤーがいる
                        if (players.size() > 0)
                        {
                            //ターゲッティング
                            m_TargetObjIdentifier = GetIdentifierGameObject(players[0].obj);
                        }
                    }

                    m_BeforeVec = m_Obj->vel;
                }

                //行動
                switch (m_CurrentState)
                {
                case ActionState::Blade:
                    update_blade();
                    break;
                case ActionState::Beam:
                    update_beam();
                    break;
                case ActionState::Misile:
                    update_misile();
                    break;
                case ActionState::MachineGun:
                    update_machinegun();
                    break;
                case ActionState::MachineGunHand:
                    update_machinegun_hand();
                    break;
                case ActionState::EnergyBallBit:
                    update_energyballbit();
                    break;
                case ActionState::Granade:
                    update_granade();
                    break;
                case ActionState::EnemyCall:
                    update_enemyCall();
                    break;

                case ActionState::SuperBeam:
                    update_super_beam();
                    break;
                case ActionState::Phase2Start:
                    update_phase2Start();
                    break;
                //case ActionState::HealEvent:
                //    update_healevent();
                //    break;
                case ActionState::WaitEvent:
                    update_waitevent();
                    break;
                default:break;
                }
            }


            m_StartedFrameCount++;
        }
    }

    //m_BeforeVec.x += (m_BeforeVec.x - m_BeforeVecBuf.x) * -0.05f;
    //m_BeforeVec.y += (m_BeforeVec.y - m_BeforeVecBuf.y) * -0.05f;
    //m_BeforeVec.z += (m_BeforeVec.z - m_BeforeVecBuf.z) * -0.05f;
    SetBeforeVec(
        &m_BeforeVec,
        &m_BeforeVecBuf,
        turnspeed
    );

    {
        Float3* bufp = &m_BeforeVecBuf;

        float headDeg = DegNormalize(
            DegNormalize(
                CalculateRadToDeg(
                    atan2(m_HeadVecBuf.y, m_HeadVecBuf.x))
                + 180.0f + 90.0f
            ) - m_CurrentAxisPrivate);

        float sikai = 30.0f;
        if (
            ((headDeg >= 0 && headDeg < (90 - sikai))
                || (headDeg > (270 + sikai) && headDeg <= 360))
            && m_TargetDistance <= ONE_BLOCK * 256.0f
        )
        {
            bufp = &m_HeadVecBuf;
        }

        SetBeforeVec(
            &m_HeadVec,
            bufp,
            0.03f
        );
    }

    //穴井亜レーター阻止成功後
    if(m_SuperBeamSuccessWeaknessFlag
        && m_Dead_Frame <= 0
    )
    {
        int indextmp = GetGameObjectIndex(m_Obj);
        if ((m_Obj->frameActive + indextmp * 13) % 4 == 0)
        {
            Float3 postmp = m_Obj->pos;
            postmp.x += sin((float)m_Obj->frame * 13.0f + (0.7f * indextmp)) * ONE_BLOCK * 1.4f * m_Obj->scale.x;
            postmp.y += cos((float)m_Obj->frame * 03.0f + (1.3f * indextmp)) * ONE_BLOCK * 1.4f * m_Obj->scale.y;
            postmp.z += cos((float)m_Obj->frame * 13.0f + (1.3f * indextmp)) * ONE_BLOCK * 1.4f * m_Obj->scale.y + -950.0f;
            //postmp.z += -150.0f * m_Obj->scale.y;
            GAME_OBJECT* go = GO_CreateEffect(GO_EFFECT_TYPE_SPARK, postmp);
            if (go != nullptr)
            {
                go->graphic.color = MkF4(
                    2.0f,
                    2.0f,
                    0.1f,
                    0.6f
                );
                go->rot = -50.0f * m_Obj->frame * indextmp;
                go->scale = MkF3(
                    8.0f,
                    8.0f,
                    8.0f
                );
                GameObjectSetLifeTime(go, 5);
            }
        }
    }


    //第二段階
    if (m_HardPhase2Flag
        && m_Dead_Frame <= 0
        )
    {
        int indextmp = GetGameObjectIndex(m_Obj);
        if ((m_Obj->frameActive + indextmp * 13) % 4 == 0)
        {
            Float3 postmp = m_Obj->pos;
            postmp.x += sin((float)m_Obj->frame * 13.0f + (0.7f * indextmp)) * ONE_BLOCK * 1.4f * m_Obj->scale.x;
            postmp.y += cos((float)m_Obj->frame * 03.0f + (1.3f * indextmp)) * ONE_BLOCK * 1.4f * m_Obj->scale.y;
            postmp.z += cos((float)m_Obj->frame * 13.0f + (1.3f * indextmp)) * ONE_BLOCK * 1.4f * m_Obj->scale.y + -950.0f;
            //postmp.z += -150.0f * m_Obj->scale.y;
            GAME_OBJECT* go = GO_CreateEffect(GO_EFFECT_TYPE_SPARK, postmp);
            if (go != nullptr)
            {
                if (m_Obj->frameActive % 3 == 0)
                {
                    go->graphic.color = MkF4(
                        0.4f,
                        0.0f,
                        0.05f,
                        -0.8f
                    );
                }
                else
                {
                    //go->graphic.color = MkF4(
                    //    1.2f,
                    //    0.6f,
                    //    1.5f,
                    //    0.95f
                    //);
                    go->graphic.color = MkF4(
                        0.8f,
                        0.0f,
                        0.1f,
                        0.8f
                    );
                }
                go->rot = -50.0f * m_Obj->frame * indextmp;
                go->scale = MkF3(
                    8.0f,
                    8.0f,
                    8.0f
                );
                GameObjectSetLifeTime(go, 5);
            }
        }
    }


    if (m_Anim_State == AnimState::SuperBeamBefore)
    {
        if (m_Obj->frameActive % 8 == 0)
        {
            CameraShakeRequestAll(
                CAMERA_SHAKE_MIDIUM_FRAME,
                //CAMERA_SHAKE_EXLARGE_POWER * 1.0f
                CAMERA_SHAKE_MIDIUM_POWER * 1.25f,
                -1,
                true
            );
        }
    }

    {
        if (m_BladeVisibleFlag)
        {
            if (m_BladeVisibleFrame < m_BladeVisibleFrameMax)
            {
                m_BladeVisibleFrame++;
            }
        }
        else
        {
            if (m_BladeVisibleFrame > 0)
            {
                m_BladeVisibleFrame--;
            }
        }
    }

    { //あにむ


        //m_ModelTransSca[4].x = 0.0f;
        //m_ModelTransSca[4].y = 0.0f;
        //m_ModelTransSca[4].z = 0.0f;


        if (m_StartedFlag)
        {
            //if (m_CurrentState == ActionState::Idle)
            {
                //for (int i = 0; i < MODEL_NUM; i++)
                //{
                //    m_ModelTransPos[i] = {};
                //    m_ModelTransRot[i] = {};
                //}

                {
                    float yure1 = sin((float)m_Obj->frameActive * 1.5f);
                    float yure2 = sin((float)m_Obj->frameActive / 100.0f);
                    //for (int i = 0; i < 3; i++)
                    //{
                    //    m_ModelTransPos[3 + i].y = yure1 * 6.0f + yure2 * 120.0f;
                    //}
                    //m_ModelTransPos[1].y = yure1 * 6.0f + yure2 * 60.0f;
                    //m_ModelTransPos[2].y = yure1 * 6.0f + yure2 * 80.0f;
                    for (int i = 1; i < MODEL_NUM; i++)
                    {
                        m_ModelTransRot[i].x = 0.0f;
                    }
                    for (int i = 0; i < 3; i++)
                    {
                        m_ModelTransPos[3 + i].y = yure1 * 6.0f + yure2 * 12.0f;
                    }
                    m_ModelTransPos[1].y = yure1 * 6.0f + yure2 * 20.0f;
                    m_ModelTransPos[2].y = yure1 * 6.0f + yure2 * 30.0f;

                    //頭の向き
                    //m_ModelTransPos[0].y = yure1 * 6.0f + yure2 * 60.0f;
                    m_ModelTransPos[0].y = yure1 * 6.0f + yure2 * 20.0f;
                    m_ModelTransPos[0].x = m_HeadVec.x * 50.0f;
                    //m_ModelTransPos[0].z = (m_HeadVec.y - 1.0f) * -80.0f;
                    //m_ModelTransPos[0].z = (m_HeadVec.y - 1.0f) * -80.0f;
                    m_ModelTransPos[0].z = 0.0f;
                    m_ModelTransRot[0].y = m_CurrentAxisHead - m_CurrentAxisPrivate;
                    m_ModelTransRot[0].x = m_HeadVec.z * 40.0f;
                }

                if(m_SimpleMoveFrame >= 0)
                {
                    //for (int i = 0; i < MODEL_NUM; i++)
                    //{
                    //    m_ModelTransRot[i].x = 10.0f;
                    //}
                    for (int i = 1; i < MODEL_NUM; i++)
                    {
                        m_ModelTransRot[i].x = 10.0f;
                    }
                    m_ModelTransPos[0].z += -10.0f;
                    m_ModelTransPos[0].y += 10.0f;
                    m_ModelTransRot[0].x += 5.0f;
                }
            }
        }


        if (m_Anim_Frame >= 0)
        {
            if(m_Anim_State == AnimState::Blade)
            {
                m_HeadVecBuf = m_BeforeVec;
                m_HeadVec = m_HeadVecBuf;

                //static constexpr int BLADE_BEFORE_FRAME_MAX = 100;
                //static constexpr int BLADE_FRAME_MAX = 90;
                //static constexpr int BLADE_FRAME_ATKF = 20;

                float mod = 1.0f - m_Anim_Mod;
                float mod0 = (std::min((float)(m_Anim_Frame) / ATTACK_BLADE_ATTACKING_FRAME, 1.0f));

                float mod1 = EasingOutSine(1.0f - mod0);
                float mod2 = EasingOutCubic(mod0);

                float mod3 = std::max(std::min((float)((m_Anim_FrameMax - m_Anim_Frame) - ATTACK_BLADE_ATTACKING_FRAME) / (m_Anim_FrameMax - ATTACK_BLADE_ATTACKING_FRAME), 1.0f), 0.0f);
                float mod4 = (EasingOutQuint(mod3));
                //float mod4 = EasingOutBack(EasingOutSine(mod3));
                //float mod4 = EasingOutBack(EasingOutCirc(mod3));
                //float mod4 = 1.0f;

                //Float3 bpos_head = m_ModelTransPos[0];
                //Float3 brot_head = m_ModelTransRot[0];

                for (int i = 0; i < MODEL_NUM; i++)
                {
                    m_ModelTransPos[i] = {};
                    m_ModelTransRot[i] = {};
                }

                if (m_Anim_FrameMax > m_Anim_Frame)
                {
                    //ぼでー
                    {
                        for (int i = 0; i < MODEL_NUM; i++)
                        {
                            m_ModelTransPos[i].x = -40.0f * mod1;
                            m_ModelTransPos[i].z = -40.0f * mod1;
                            m_ModelTransRot[i].x = 10.0f * mod1;
                            m_ModelTransRot[i].y = 95.0f * mod1;
                            m_ModelTransRot[i].z = -5.0f * mod1;
                        }
                        m_ModelTransRot[1].y = 120.0f * mod1;
                        m_ModelTransRot[2].x = 0.0f;

                        for (int i = 0; i < MODEL_NUM; i++)
                        {
                            m_ModelTransPos[i].x += -40.0f * mod2 * mod4;
                            m_ModelTransRot[i].y += -100.0f * mod2 * mod4;
                            m_ModelTransRot[i].z += 5.0f * mod2 * mod4;
                        }
                    }
                    //へっど
                    {
                        m_ModelTransPos[0].x = 30.0f * mod1;
                        m_ModelTransPos[0].y = 80.0f * mod1;
                        m_ModelTransPos[0].z = -40.0f * mod1;
                        m_ModelTransRot[0].x = 30.0f * mod1;
                        m_ModelTransRot[0].y = 70.0f * mod1;
                        m_ModelTransRot[0].z = -5.0f * mod1;

                        m_ModelTransPos[0].x += -45.0f * mod2 * mod4;
                        m_ModelTransPos[0].z += 15.0f * mod2 * mod4;
                        m_ModelTransRot[0].y += -60.0f * mod2 * mod4;
                        m_ModelTransRot[0].z += 5.0f * mod2 * mod4;

                        //m_ModelTransPos[0].x += bpos_head.x * (1.0f - mod4);
                        //m_ModelTransPos[0].y += bpos_head.y * (1.0f - mod4);
                        //m_ModelTransPos[0].z += bpos_head.z * (1.0f - mod4);
                        //m_ModelTransRot[0].x += brot_head.x * (1.0f - mod4);
                        //m_ModelTransRot[0].y += brot_head.y * (1.0f - mod4);
                        //m_ModelTransRot[0].z += brot_head.z * (1.0f - mod4);

                    }

                    //腕
                    {
                        for (int i = 0; i < 2; i++)
                        {
                            m_ModelTransPos[3 + i].x = -120.0f * mod4;
                            m_ModelTransPos[3 + i].y = 100.0f * mod4;
                            m_ModelTransPos[3 + i].z = 300.0f * mod4;
                            m_ModelTransRot[3 + i].x = -80.0f * mod4;
                            m_ModelTransRot[3 + i].y = 130.0f * mod4;
                            m_ModelTransRot[3 + i].z = 6.0f * mod4;
                        }
                        m_ModelTransPos[4].x = -60.0f * mod4;
                        m_ModelTransPos[4].z = -400.0f * mod4;
                        m_ModelTransRot[4].y = 210.0f * mod4;

                        float modtmp = EasingInCirc(mod2) * EasingOutSine(mod4);
                        for (int i = 0; i < 2; i++)
                        {
                            m_ModelTransPos[3 + i].x += 200.0f * modtmp;
                            m_ModelTransPos[3 + i].y += -300.0f * modtmp;
                            m_ModelTransPos[3 + i].z += 300.0f * modtmp;
                            m_ModelTransRot[3 + i].x += 0.0f * modtmp;
                            m_ModelTransRot[3 + i].y += -310.0f * modtmp;
                            m_ModelTransRot[3 + i].z += -12.0f * modtmp;
                        }
                        m_ModelTransPos[4].x += 260.0f * modtmp;
                        m_ModelTransPos[4].y += -100.0f * modtmp;
                        m_ModelTransPos[4].z += 1000.0f * modtmp;
                        m_ModelTransRot[4].y += -90.0f * modtmp;

                        //m_ModelTransSca[5].y = -(0.5f + 1.0f * mod4);
                        //m_Obj->graphic.graph.polygonModel[3].size.x = 500.0f + 1000.0f * mod4;
                    }

                    //if (m_Anim_Frame != m_Anim_FrameMax && m_Anim_Frame < (m_Anim_FrameMax - ATTACK_BLADE_ATTACKING_FRAME))
                    //{
                    //    m_ModelTransSca[4].x = 1.0f;
                    //    m_ModelTransSca[4].y = 1.0f;
                    //    m_ModelTransSca[4].z = 1.0f;
                    //}
                }
            }
            else if (m_Anim_State == AnimState::BladeBefore)
            { //
                m_HeadVecBuf = m_BeforeVec;
                m_HeadVec = m_HeadVecBuf;

                float mod = 1.0f - m_Anim_Mod;

                float kamaeF = ATTACK_BLADE_BEFORE_FIXED_FRAME;
                if (ATTACK_BLADE_BEFORE_FIXED_FRAME > m_Anim_FrameMax)
                {
                    kamaeF = m_Anim_FrameMax * 0.6f;
                }

                float mod1 = EasingInSine(std::min((float)(m_Anim_Frame) / (kamaeF), 1.0f));
                if (mod1 >= 0.95f)
                {
                    mod1 += sin((float)m_Obj->frameActive * 1.0f) * 0.0025f;
                }

                //Float3 bpos_head = m_ModelTransPos[0];
                //Float3 brot_head = m_ModelTransRot[0];

                for (int i = 0; i < MODEL_NUM; i++)
                {
                    m_ModelTransPos[i] = {};
                    m_ModelTransRot[i] = {};
                }

                //ぼでー
                {
                    for (int i = 0; i < MODEL_NUM; i++)
                    {
                        m_ModelTransPos[i].x = -40.0f * mod1;
                        m_ModelTransPos[i].z = -40.0f * mod1;
                        m_ModelTransRot[i].x = 10.0f * mod1;
                        m_ModelTransRot[i].y = 95.0f * mod1;
                        m_ModelTransRot[i].z = -5.0f * mod1;
                    }
                    m_ModelTransRot[1].y = 120.0f * mod1;
                    m_ModelTransRot[2].x = 0.0f;
                }
                //へっど
                {
                    m_ModelTransPos[0].x = 30.0f * mod1;
                    m_ModelTransPos[0].y = 80.0f * mod1;
                    m_ModelTransPos[0].z = -40.0f * mod1;
                    m_ModelTransRot[0].x = 30.0f * mod1;
                    m_ModelTransRot[0].y = 70.0f * mod1;
                    m_ModelTransRot[0].z = -5.0f * mod1;

                    //m_ModelTransPos[0].x += bpos_head.x * (1.0f - mod1);
                    //m_ModelTransPos[0].y += bpos_head.y * (1.0f - mod1);
                    //m_ModelTransPos[0].z += bpos_head.z * (1.0f - mod1);
                    //m_ModelTransRot[0].x += brot_head.x * (1.0f - mod1);
                    //m_ModelTransRot[0].y += brot_head.y * (1.0f - mod1);
                    //m_ModelTransRot[0].z += brot_head.z * (1.0f - mod1);

                }

                //腕
                {
                    for (int i = 0; i < 2; i++)
                    {
                        m_ModelTransPos[3 + i].x = -120.0f * mod1;
                        m_ModelTransPos[3 + i].y = 100.0f * mod1;
                        m_ModelTransPos[3 + i].z = 300.0f * mod1;
                        m_ModelTransRot[3 + i].x = -80.0f * mod1;
                        m_ModelTransRot[3 + i].y = 130.0f * mod1;
                        m_ModelTransRot[3 + i].z = 6.0f * mod1;
                    }
                    m_ModelTransPos[4].x = -60.0f * mod1;
                    m_ModelTransPos[4].z = -400.0f * mod1;
                    m_ModelTransRot[4].y = 210.0f * mod1;
                }
                //m_ModelTransSca[4].y = std::min(mod1, 1.0f);
                //if (m_Anim_Frame > 0)
                //{
                //    m_ModelTransSca[4].x = 1.0f;
                //    m_ModelTransSca[4].y = 1.0f;
                //    m_ModelTransSca[4].z = 1.0f;
                //}
            }
            else if (m_Anim_State == AnimState::BeamBefore)
            { //
                float mod = 1.0f - m_Anim_Mod;

                float kamaeF = ATTACK_BEAM_BEFORE_FIXED_FRAME;
                if (ATTACK_BEAM_BEFORE_FIXED_FRAME > m_Anim_FrameMax)
                {
                    kamaeF = m_Anim_FrameMax * 0.6f;
                }

                float mod1 = EasingInSine(std::min((float)(m_Anim_Frame) / (kamaeF), 1.0f));
                if (mod1 >= 0.95f)
                {
                    mod1 += sin((float)m_Obj->frameActive * 1.0f) * 0.0025f;
                }

                //for (int i = 0; i < MODEL_NUM; i++)
                //{
                //    m_ModelTransPos[i] = {};
                //    m_ModelTransRot[i] = {};
                //}

                //ぼでー
                {
                    for (int i = 0; i < MODEL_NUM; i++)
                    {
                        m_ModelTransPos[i].y = -350.0f * mod1;
                    }
                }
                ////へっど
                //{
                //    m_ModelTransPos[0].x = 30.0f * mod1;
                //    m_ModelTransPos[0].y = 80.0f * mod1;
                //    m_ModelTransPos[0].z = -40.0f * mod1;
                //    m_ModelTransRot[0].x = 30.0f * mod1;
                //    m_ModelTransRot[0].y = 70.0f * mod1;
                //    m_ModelTransRot[0].z = -5.0f * mod1;
                //}
                {
                    m_ModelTransPos[0].y += 80.0f * mod1;
                }

                //腕
                {
                    //m_ModelTransRot[3].z = -40.0f * mod1;
                    //m_ModelTransRot[5].z = 40.0f * mod1;

                    if (mod1 >= 0.95f)
                    {
                        m_ModelTransRot[3].y += std::min(4.0f + (m_ModelTransRot[3].y / 30.0f), 45.0f);
                        m_ModelTransRot[5].y += std::min(4.0f + (m_ModelTransRot[5].y / 30.0f), 45.0f);
                    }
                    //m_ModelTransRot[3].y  = 80.0f;
                    //m_ModelTransRot[5].y  = 80.0f;

                    {
                        float radr = CalculateDegToRad(m_ModelTransRot[3].y + 90.0f);
                        float radl = CalculateDegToRad(m_ModelTransRot[5].y - 90.0f);
                        m_ModelTransPos[3].x = sin(radr) * 100.0f * mod1;
                        m_ModelTransPos[3].z = cos(radr) * 100.0f * mod1;
                        m_ModelTransPos[5].x = sin(radl) * 100.0f * mod1;
                        m_ModelTransPos[5].z = cos(radl) * 100.0f * mod1;

                        //m_ModelTransPos[3].y = (50.0f + abs(cos(radr)) * -600.0f) * mod1;
                        //m_ModelTransPos[5].y = (50.0f + abs(cos(radl)) * -600.0f) * mod1;
                        m_ModelTransPos[3].y += -80.0f * mod1;
                        m_ModelTransPos[5].y += -80.0f * mod1;

                    }
                }
            }
            else if (m_Anim_State == AnimState::Beam)
            { //
                float mod = 1.0f - m_Anim_Mod;

                float kamaeF = ATTACK_BEAM_BEFORE_FIXED_FRAME;
                if (ATTACK_BEAM_BEFORE_FIXED_FRAME > m_Anim_FrameMax)
                {
                    kamaeF = m_Anim_FrameMax * 0.6f;
                }

                float mod1 = EasingInSine(
                    1.0f - std::max(
                        std::min(
                            (float)(m_Anim_Frame - (m_Anim_FrameMax - kamaeF)) / (kamaeF),
                        1.0f),
                    0.0f)
                );
                if (mod1 >= 0.95f)
                {
                    mod1 += sin((float)m_Obj->frameActive * 1.0f) * 0.05f;
                }

                //for (int i = 0; i < MODEL_NUM; i++)
                //{
                //    m_ModelTransPos[i] = {};
                //    m_ModelTransRot[i] = {};
                //}

                //ぼでー
                {
                    for (int i = 0; i < MODEL_NUM; i++)
                    {
                        m_ModelTransPos[i].y = -350.0f * mod1;
                    }
                }
                ////へっど
                //{
                //    m_ModelTransPos[0].x = 30.0f * mod1;
                //    m_ModelTransPos[0].y = 80.0f * mod1;
                //    m_ModelTransPos[0].z = -40.0f * mod1;
                //    m_ModelTransRot[0].x = 30.0f * mod1;
                //    m_ModelTransRot[0].y = 70.0f * mod1;
                //    m_ModelTransRot[0].z = -5.0f * mod1;
                //}
                {
                    m_ModelTransPos[0].y += 80.0f * mod1;
                }

                //腕
                {
                    m_ModelTransRot[3].z = -40.0f * mod1;
                    m_ModelTransRot[5].z = 40.0f * mod1;
                    m_ModelTransRot[3].y = 0.0f;
                    m_ModelTransRot[5].y = 0.0f;

                    m_ModelTransPos[3].x = +230.0f * mod1;
                    m_ModelTransPos[5].x = -230.0f * mod1;
                    m_ModelTransPos[3].y = -90.0f * mod1;
                    m_ModelTransPos[5].y = -90.0f * mod1;
                }
            }

            else if (m_Anim_State == AnimState::MachinegunBefore)
            { //
                m_HeadVecBuf = m_BeforeVec;
                m_HeadVec = m_HeadVecBuf;

                float mod = 1.0f - m_Anim_Mod;

                //float kamaeF = ATTACK_BEAM_BEFORE_FIXED_FRAME;
                //if (ATTACK_BEAM_BEFORE_FIXED_FRAME > m_Anim_FrameMax)
                //{
                //    kamaeF = m_Anim_FrameMax * 0.6f;
                //}
                float kamaeF = m_Anim_FrameMax;

                float mod1 = EasingInSine(std::min((float)(m_Anim_Frame) / (kamaeF), 1.0f));
                if (mod1 >= 0.95f)
                {
                    mod1 += sin((float)m_Obj->frameActive * 1.0f) * 0.0025f;
                }

                for (int i = 0; i < MODEL_NUM; i++)
                {
                    m_ModelTransPos[i] = {};
                    m_ModelTransRot[i] = {};
                }

                //ぼでー
                {
                    for (int i = 0; i < MODEL_NUM; i++)
                    {
                        m_ModelTransRot[i].x = 16.0f * mod1;
                    }
                    m_ModelTransPos[1].y = -60.0f * mod1;
                    m_ModelTransPos[2].y = -70.0f * mod1;
                }

                //腕
                {
                    {
                        m_ModelTransPos[3].x = 110.0f * mod1;
                        m_ModelTransPos[3].y = (-300.0f) * mod1;
                        m_ModelTransPos[3].z = -250.0f * mod1;
                        m_ModelTransPos[5].x = -110.0f * mod1;
                        m_ModelTransPos[5].y = (-300.0f) * mod1;
                        m_ModelTransPos[5].z = -250.0f * mod1;

                        //m_ModelTransRot[3].x = 330.0f * mod1;
                        //m_ModelTransRot[5].x = 330.0f * mod1;
                        m_ModelTransRot[3].x = -420.0f * mod1;
                        m_ModelTransRot[5].x = -420.0f * mod1;
                        //m_ModelTransRot[3].y = -30.0f * mod1;
                        //m_ModelTransRot[5].y = 30.0f * mod1;
                        m_ModelTransRot[3].y = -10.0f * mod1;
                        m_ModelTransRot[5].y = 10.0f * mod1;
                        //m_ModelTransRot[3].z = +28.0f * mod1;
                        //m_ModelTransRot[5].z = -28.0f * mod1;
                    }
                }
            }
            else if (m_Anim_State == AnimState::Machinegun)
            { //
                m_HeadVecBuf = m_BeforeVec;
                m_HeadVec = m_HeadVecBuf;

                float mod = 1.0f - m_Anim_Mod;

                float kamaeF = ATTACK_BEAM_BEFORE_FIXED_FRAME;
                if (ATTACK_BEAM_BEFORE_FIXED_FRAME > m_Anim_FrameMax)
                {
                    kamaeF = m_Anim_FrameMax * 0.6f;
                }

                float mod1 = EasingInSine(
                    1.0f - std::max(
                        std::min(
                            (float)(m_Anim_Frame - (m_Anim_FrameMax - kamaeF)) / (kamaeF),
                            1.0f),
                        0.0f)
                );
                if (mod1 >= 0.95f)
                {
                    mod1 += sin((float)m_Obj->frameActive * 1.0f) * 0.01f;
                }

                for (int i = 0; i < MODEL_NUM; i++)
                {
                    m_ModelTransPos[i] = {};
                    m_ModelTransRot[i] = {};
                }

                //ぼでー
                {
                    for (int i = 0; i < MODEL_NUM; i++)
                    {
                        m_ModelTransRot[i].x = 16.0f * mod1;
                    }
                    m_ModelTransPos[1].y = -60.0f * mod1;
                    m_ModelTransPos[2].y = -70.0f * mod1;
                }

                //腕
                {
                    {
                        m_ModelTransPos[3].x = 110.0f * mod1;
                        m_ModelTransPos[3].y = (-300.0f
                            + sin((float)m_Obj->frameActive * 1.0f) * 1.0f) * mod1;
                        m_ModelTransPos[3].z = -250.0f * mod1;
                        m_ModelTransPos[5].x = -110.0f * mod1;
                        m_ModelTransPos[5].y = (-300.0f
                            + sin((float)m_Obj->frameActive * 1.0f) * 1.0f) * mod1;
                        m_ModelTransPos[5].z = -250.0f * mod1;

                        //m_ModelTransRot[3].x = 330.0f * mod1;
                        //m_ModelTransRot[5].x = 330.0f * mod1;
                        m_ModelTransRot[3].x = -420.0f * mod1;
                        m_ModelTransRot[5].x = -420.0f * mod1;
                        //m_ModelTransRot[3].y = -30.0f * mod1;
                        //m_ModelTransRot[5].y = 30.0f * mod1;
                        m_ModelTransRot[3].y = -10.0f * mod1;
                        m_ModelTransRot[5].y = 10.0f * mod1;
                        //m_ModelTransRot[3].z = +28.0f * mod1;
                        //m_ModelTransRot[5].z = -28.0f * mod1;
                    }
                }
            }


            else if (m_Anim_State == AnimState::MachinegunHandBefore)
            { //
                m_HeadVecBuf = m_BeforeVec;
                m_HeadVec = m_HeadVecBuf;

                float mod = 1.0f - m_Anim_Mod;

                //float kamaeF = ATTACK_BEAM_BEFORE_FIXED_FRAME;
                //if (ATTACK_BEAM_BEFORE_FIXED_FRAME > m_Anim_FrameMax)
                //{
                //    kamaeF = m_Anim_FrameMax * 0.6f;
                //}
                float kamaeF = m_Anim_FrameMax;

                float mod1 = EasingInSine(std::min((float)(m_Anim_Frame) / (kamaeF), 1.0f));
                //if (mod1 >= 0.95f)
                //{
                //    mod1 += sin((float)m_Obj->frameActive * 1.0f) * 0.0025f;
                //}

                for (int i = 0; i < MODEL_NUM; i++)
                {
                    if (i == 0)
                    {
                        continue;
                    }
                    m_ModelTransPos[i] = {};
                    m_ModelTransRot[i] = {};
                }

                ////ぼでー
                //{
                //    for (int i = 0; i < MODEL_NUM; i++)
                //    {
                //        m_ModelTransRot[i].y = -45.0f * mod1;
                //    }
                //    //m_ModelTransPos[0].x = -30.0f * mod1;
                //    //m_ModelTransPos[0].y = 70.0f * mod1;
                //    m_ModelTransRot[0].y += 40.0f * mod1;
                //    //m_ModelTransRot[0].z = 10.0f * mod1;
                //    m_ModelTransPos[1].y = 20.0f * mod1;
                //}

                ////腕
                //{
                //    {
                //        //m_ModelTransPos[5].x = -160.0f * mod1;
                //        //m_ModelTransPos[5].y = 80.0f * mod1;
                //        //m_ModelTransPos[5].z = 50.0f * mod1;

                //        //m_ModelTransRot[5].x = -540.0f * mod1;
                //        //m_ModelTransRot[5].y = -90.0f * mod1;
                //        ////m_ModelTransRot[5].z = -40.0f * mod1;
                //        m_ModelTransPos[5].x = 160.0f * mod1;
                //        m_ModelTransPos[5].y = 480.0f * mod1;
                //        m_ModelTransPos[5].z = -480.0f * mod1;

                //        //m_ModelTransRot[5].x = -540.0f * mod1;
                //        //m_ModelTransRot[5].x = -450.0f * mod1;
                //        m_ModelTransRot[5].x = 270.0f * mod1;
                //        m_ModelTransRot[5].y = -2.0f * mod1;
                //        m_ModelTransRot[5].z = 90.0f * mod1;
                //        //m_ModelTransRot[5].z = -40.0f * mod1;
                //    }
                //}
                //ぼでー
                {
                    for (int i = 0; i < MODEL_NUM; i++)
                    {
                        m_ModelTransRot[i].y = 45.0f * mod1;
                    }
                    //m_ModelTransPos[0].x = -30.0f * mod1;
                    //m_ModelTransPos[0].y = 70.0f * mod1;
                    m_ModelTransPos[0].y += 50.0f * mod1;
                    m_ModelTransPos[0].z += 80.0f * mod1;
                    m_ModelTransRot[0].x += 12.0f * mod1;
                    m_ModelTransRot[0].y += -40.0f * mod1;
                    //m_ModelTransRot[0].z = 10.0f * mod1;
                    m_ModelTransPos[1].y = 20.0f * mod1;
                }

                //腕
                {
                    {
                        m_ModelTransPos[3].x = -160.0f * mod1;
                        m_ModelTransPos[3].y = 480.0f * mod1;
                        m_ModelTransPos[3].z = -420.0f * mod1;

                        m_ModelTransRot[3].x = -450.0f * mod1;
                        m_ModelTransRot[3].y = 1.0f * mod1;
                        m_ModelTransRot[3].z = -90.0f * mod1;
                    }
                }
            }
            else if (m_Anim_State == AnimState::MachinegunHand)
            { //
                m_HeadVecBuf = m_BeforeVec;
                m_HeadVec = m_HeadVecBuf;

                float mod = 1.0f - m_Anim_Mod;

                float kamaeF = ATTACK_BEAM_BEFORE_FIXED_FRAME;
                if (ATTACK_BEAM_BEFORE_FIXED_FRAME > m_Anim_FrameMax)
                {
                    kamaeF = m_Anim_FrameMax * 0.6f;
                }

                float mod1 = EasingInSine(
                    1.0f - std::max(
                        std::min(
                            (float)(m_Anim_Frame - (m_Anim_FrameMax - kamaeF)) / (kamaeF),
                            1.0f),
                        0.0f)
                );
                //if (mod1 >= 0.95f)
                //{
                //    mod1 += sin((float)m_Obj->frameActive * 1.0f) * 0.01f;
                //}

                for (int i = 0; i < MODEL_NUM; i++)
                {
                    if (i == 0)
                    {
                        continue;
                    }
                    m_ModelTransPos[i] = {};
                    m_ModelTransRot[i] = {};
                }

                ////ぼでー
                //{
                //    for (int i = 0; i < MODEL_NUM; i++)
                //    {
                //        m_ModelTransRot[i].y = -45.0f * mod1;
                //    }
                //    //m_ModelTransPos[0].x = -30.0f * mod1;
                //    //m_ModelTransPos[0].y = 70.0f * mod1;
                //    m_ModelTransRot[0].y += 40.0f * mod1;
                //    //m_ModelTransRot[0].z = 10.0f * mod1;
                //    m_ModelTransPos[1].y = 20.0f * mod1;
                //}

                ////腕
                //{
                //    {
                //        //m_ModelTransPos[5].x = -160.0f * mod1;
                //        //m_ModelTransPos[5].y = 80.0f * mod1;
                //        //m_ModelTransPos[5].z = 50.0f * mod1;

                //        //m_ModelTransRot[5].x = -540.0f * mod1;
                //        //m_ModelTransRot[5].y = -90.0f * mod1;
                //        ////m_ModelTransRot[5].z = -40.0f * mod1;
                //        m_ModelTransPos[5].x = 160.0f * mod1;
                //        m_ModelTransPos[5].y = 480.0f * mod1;
                //        m_ModelTransPos[5].z = -480.0f * mod1;

                //        //m_ModelTransRot[5].x = -540.0f * mod1;
                //        //m_ModelTransRot[5].x = -450.0f * mod1;
                //        m_ModelTransRot[5].x = 270.0f * mod1;
                //        m_ModelTransRot[5].y = -2.0f * mod1;
                //        m_ModelTransRot[5].z = 90.0f * mod1;
                //        //m_ModelTransRot[5].z = -40.0f * mod1;
                //    }
                //}
                //ぼでー
                {
                    for (int i = 0; i < MODEL_NUM; i++)
                    {
                        m_ModelTransRot[i].y = 45.0f * mod1;
                    }
                    //m_ModelTransPos[0].x = -30.0f * mod1;
                    //m_ModelTransPos[0].y = 70.0f * mod1;
                    m_ModelTransPos[0].y += 50.0f * mod1;
                    m_ModelTransPos[0].z += 80.0f * mod1;
                    m_ModelTransRot[0].x += 12.0f * mod1;
                    m_ModelTransRot[0].y += -40.0f * mod1;
                    //m_ModelTransRot[0].z = 10.0f * mod1;
                    m_ModelTransPos[1].y = 20.0f * mod1;
                }

                //腕
                {
                    {
                        m_ModelTransPos[3].x = -160.0f * mod1;
                        m_ModelTransPos[3].y = 480.0f * mod1;
                        m_ModelTransPos[3].z = -420.0f * mod1;

                        m_ModelTransRot[3].x = -450.0f * mod1;
                        m_ModelTransRot[3].y = 1.0f * mod1;
                        m_ModelTransRot[3].z = -90.0f * mod1;
                    }
                }
                }


            else if (m_Anim_State == AnimState::GranadeBefore)
            { //
                m_HeadVecBuf = m_BeforeVec;
                m_HeadVec = m_HeadVecBuf;

                float mod = 1.0f - m_Anim_Mod;

                //float kamaeF = ATTACK_BEAM_BEFORE_FIXED_FRAME;
                //if (ATTACK_BEAM_BEFORE_FIXED_FRAME > m_Anim_FrameMax)
                //{
                //    kamaeF = m_Anim_FrameMax * 0.6f;
                //}
                float kamaeF = m_Anim_FrameMax;

                float mod1 = EasingInSine(std::min((float)(m_Anim_Frame) / (kamaeF), 1.0f));
                if (mod1 >= 0.95f)
                {
                    mod1 += sin((float)m_Obj->frameActive * 1.0f) * 0.0025f;
                }

                for (int i = 0; i < MODEL_NUM; i++)
                {
                    m_ModelTransPos[i] = {};
                    m_ModelTransRot[i] = {};
                }

                //ぼでー
                {
                    for (int i = 0; i < MODEL_NUM; i++)
                    {
                        m_ModelTransRot[i].y = -45.0f * mod1;
                    }
                    m_ModelTransPos[0].x = -30.0f * mod1;
                    m_ModelTransPos[0].y = 70.0f * mod1;
                    m_ModelTransRot[0].y = -60.0f * mod1;
                    m_ModelTransRot[0].z = 10.0f * mod1;
                    m_ModelTransPos[1].y = 20.0f * mod1;
                }

                //腕
                {
                    {
                        m_ModelTransPos[5].x = -160.0f * mod1;
                        m_ModelTransPos[5].y = 80.0f * mod1;
                        m_ModelTransPos[5].z = 50.0f * mod1;

                        m_ModelTransRot[5].x = -540.0f * mod1;
                        m_ModelTransRot[5].y = -90.0f * mod1;
                        //m_ModelTransRot[5].z = -40.0f * mod1;
                    }
                }
            }
            else if (m_Anim_State == AnimState::Granade)
            { //
                m_HeadVecBuf = m_BeforeVec;
                m_HeadVec = m_HeadVecBuf;

                float mod = 1.0f - m_Anim_Mod;

                float kamaeF = ATTACK_BEAM_BEFORE_FIXED_FRAME;
                if (ATTACK_BEAM_BEFORE_FIXED_FRAME > m_Anim_FrameMax)
                {
                    kamaeF = m_Anim_FrameMax * 0.6f;
                }

                float mod1 = EasingInSine(
                    1.0f - std::max(
                        std::min(
                            (float)(m_Anim_Frame - (m_Anim_FrameMax - kamaeF)) / (kamaeF),
                            1.0f),
                        0.0f)
                );
                if (mod1 >= 0.95f)
                {
                    mod1 += sin((float)m_Obj->frameActive * 1.0f) * 0.0025f;
                }

                for (int i = 0; i < MODEL_NUM; i++)
                {
                    m_ModelTransPos[i] = {};
                    m_ModelTransRot[i] = {};
                }

                //ぼでー
                {
                    for (int i = 0; i < MODEL_NUM; i++)
                    {
                        m_ModelTransRot[i].y = -45.0f * mod1;
                    }
                    m_ModelTransPos[0].x = -30.0f * mod1;
                    m_ModelTransPos[0].y = 70.0f * mod1;
                    m_ModelTransRot[0].y = -60.0f * mod1;
                    m_ModelTransRot[0].z = 10.0f * mod1;
                    m_ModelTransPos[1].y = 20.0f * mod1;
                }

                //腕
                {
                    {
                        m_ModelTransPos[5].x = -160.0f * mod1;
                        m_ModelTransPos[5].y = 80.0f * mod1;
                        m_ModelTransPos[5].z = 50.0f * mod1;

                        m_ModelTransRot[5].x = -540.0f * mod1;
                        m_ModelTransRot[5].y = -90.0f * mod1;
                        //m_ModelTransRot[5].z = -40.0f * mod1;
                    }
                }
            }

            else if (m_Anim_State == AnimState::GuruguruBefore)
            { //
                float mod = 1.0f - m_Anim_Mod;

                //float kamaeF = ATTACK_BEAM_BEFORE_FIXED_FRAME;
                //if (ATTACK_BEAM_BEFORE_FIXED_FRAME > m_Anim_FrameMax)
                //{
                //    kamaeF = m_Anim_FrameMax * 0.6f;
                //}
                float kamaeF = m_Anim_FrameMax;

                float mod1 = EasingInSine(std::min((float)(m_Anim_Frame) / (kamaeF), 1.0f));
                if (mod1 >= 0.95f)
                {
                    mod1 += sin((float)m_Obj->frameActive * 1.0f) * 0.0025f;
                }

                //for (int i = 0; i < MODEL_NUM; i++)
                //{
                //    m_ModelTransPos[i] = {};
                //    m_ModelTransRot[i] = {};
                //}

                //腕
                {
                    //m_ModelTransRot[3].z = -40.0f * mod1;
                    //m_ModelTransRot[5].z = 40.0f * mod1;

                    if (mod1 >= 0.95f)
                    {
                        m_ModelTransRot[3].y += std::min(4.0f + (m_ModelTransRot[3].y / 30.0f), 45.0f);
                        m_ModelTransRot[5].y += std::min(4.0f + (m_ModelTransRot[5].y / 30.0f), 45.0f);
                    }
                    //m_ModelTransRot[3].y  = 80.0f;
                    //m_ModelTransRot[5].y  = 80.0f;

                    {
                        float radr = CalculateDegToRad(m_ModelTransRot[3].y + 90.0f);
                        float radl = CalculateDegToRad(m_ModelTransRot[5].y - 90.0f);
                        m_ModelTransPos[3].x = sin(radr) * 20.0f * mod1;
                        m_ModelTransPos[3].z = cos(radr) * 20.0f * mod1;
                        m_ModelTransPos[5].x = sin(radl) * 20.0f * mod1;
                        m_ModelTransPos[5].z = cos(radl) * 20.0f * mod1;

                        m_ModelTransPos[3].y = 100.0f * mod1;
                        m_ModelTransPos[5].y = 100.0f * mod1;

                        m_ModelTransRot[3].x = 90.0f * mod1;
                        m_ModelTransRot[5].x = -90.0f * mod1;
                    }
                }
            }
            else if (m_Anim_State == AnimState::Guruguru)
            { //
                float mod = 1.0f - m_Anim_Mod;

                float kamaeF = ATTACK_BEAM_BEFORE_FIXED_FRAME;
                if (ATTACK_BEAM_BEFORE_FIXED_FRAME > m_Anim_FrameMax)
                {
                    kamaeF = m_Anim_FrameMax * 0.6f;
                }

                float mod1 = EasingInSine(
                    1.0f - std::max(
                        std::min(
                            (float)(m_Anim_Frame - (m_Anim_FrameMax - kamaeF)) / (kamaeF),
                            1.0f),
                        0.0f)
                );
                if (mod1 >= 0.95f)
                {
                    mod1 += sin((float)m_Obj->frameActive * 1.0f) * 0.05f;
                }

                //for (int i = 0; i < MODEL_NUM; i++)
                //{
                //    m_ModelTransPos[i] = {};
                //    m_ModelTransRot[i] = {};
                //}

                //腕
                {
                    //m_ModelTransRot[3].z = -40.0f * mod1;
                    //m_ModelTransRot[5].z = 40.0f * mod1;

                    if (mod1 >= 0.95f)
                    {
                        m_ModelTransRot[3].y += std::min(4.0f + (m_ModelTransRot[3].y / 30.0f), 45.0f);
                        m_ModelTransRot[5].y += std::min(4.0f + (m_ModelTransRot[5].y / 30.0f), 45.0f);
                    }
                    else
                    {
                        m_ModelTransRot[3].y *= mod1;
                        m_ModelTransRot[5].y *= mod1;
                    }
                    //m_ModelTransRot[3].y  = 80.0f;
                    //m_ModelTransRot[5].y  = 80.0f;

                    {
                        float radr = CalculateDegToRad(m_ModelTransRot[3].y + 90.0f);
                        float radl = CalculateDegToRad(m_ModelTransRot[5].y - 90.0f);
                        m_ModelTransPos[3].x = sin(radr) * 20.0f * mod1;
                        m_ModelTransPos[3].z = cos(radr) * 20.0f * mod1;
                        m_ModelTransPos[5].x = sin(radl) * 20.0f * mod1;
                        m_ModelTransPos[5].z = cos(radl) * 20.0f * mod1;

                        m_ModelTransPos[3].y = 100.0f * mod1;
                        m_ModelTransPos[5].y = 100.0f * mod1;

                        m_ModelTransRot[3].x = 90.0f * mod1;
                        m_ModelTransRot[5].x = -90.0f * mod1;
                    }
                }
            }

            else if (m_Anim_State == AnimState::Misile)
            { //
                float mod = 1.0f - m_Anim_Mod;
                float mod1 = 1.0f;
                float mod2 = 1.0f;
                {
                    float kamaeF = 20;

                    mod1 = EasingInSine(std::min((float)(m_Anim_Frame) / (kamaeF), 1.0f));
                    //if (mod1 >= 0.95f)
                    //{
                    //    mod1 += sin((float)m_Obj->frameActive * 1.0f) * 0.0025f;
                    //}
                }

                {
                    float kamaeF = 20;
                    if (kamaeF > m_Anim_FrameMax)
                    {
                        kamaeF = m_Anim_FrameMax * 0.6f;
                    }

                    mod2 = EasingInSine(
                        1.0f - std::max(
                            std::min(
                                (float)(m_Anim_Frame - (m_Anim_FrameMax - kamaeF)) / (kamaeF),
                                1.0f),
                            0.0f)
                    );
                    //if (mod1 >= 0.95f)
                    //{
                    //    mod1 += sin((float)m_Obj->frameActive * 1.0f) * 0.05f;
                    //}
                }

                //for (int i = 0; i < MODEL_NUM; i++)
                //{
                //    m_ModelTransPos[i] = {};
                //    m_ModelTransRot[i] = {};
                //}

                //腕
                {
                    {
                        m_ModelTransPos[3].x = 25.0f * mod1 * mod2;
                        m_ModelTransPos[5].x = -25.0f * mod1 * mod2;
                        //m_ModelTransPos[3].y = -10.0f * mod1 * mod2;
                        //m_ModelTransPos[5].y = -10.0f * mod1 * mod2;
                        m_ModelTransPos[3].y = -320.0f * mod1 * mod2;
                        m_ModelTransPos[5].y = -320.0f * mod1 * mod2;

                        m_ModelTransRot[3].x = 540.0f * mod1 * mod2;
                        m_ModelTransRot[5].x = -540.0f * mod1 * mod2;
                        m_ModelTransRot[3].z = 20.0f * mod1 * mod2;
                        m_ModelTransRot[5].z = -30.0f * mod1 * mod2;
                    }
                }
            }

            else if (m_Anim_State == AnimState::SuperBeamBefore)
            { //
                float mod = 1.0f - m_Anim_Mod;

                float kamaeF = ATTACK_BEAM_BEFORE_FIXED_FRAME;
                if (ATTACK_BEAM_BEFORE_FIXED_FRAME > m_Anim_FrameMax)
                {
                    kamaeF = m_Anim_FrameMax * 0.6f;
                }

                float mod1 = EasingInSine(std::min((float)(m_Anim_Frame) / (kamaeF), 1.0f));
                if (mod1 >= 0.95f)
                {
                    mod1 += sin((float)m_Obj->frameActive * 1.0f) * 0.0025f;
                }

                //for (int i = 0; i < MODEL_NUM; i++)
                //{
                //    m_ModelTransPos[i] = {};
                //    m_ModelTransRot[i] = {};
                //}

                //へっど
                {
                    m_ModelTransRot[0] = {};
                    m_ModelTransPos[0] = {};
                    m_ModelTransPos[0].y += 80.0f * mod1;
                }

                //腕
                {
                    //m_ModelTransRot[3].z = -40.0f * mod1;
                    //m_ModelTransRot[5].z = 40.0f * mod1;

                    if (mod1 >= 0.95f)
                    {
                        m_ModelTransRot[3].z += std::min(4.0f + (m_ModelTransRot[3].z / 30.0f), 22.5f);
                        m_ModelTransRot[5].z += std::min(4.0f + (m_ModelTransRot[5].z / 30.0f), 22.5f);
                    }
                    //m_ModelTransRot[3].y  = 80.0f;
                    //m_ModelTransRot[5].y  = 80.0f;

                    {
                        float radr = CalculateDegToRad(m_ModelTransRot[3].z + 90.0f);
                        float radl = CalculateDegToRad(m_ModelTransRot[5].z - 90.0f);
                        m_ModelTransPos[3].x = sin(radr) * -60.0f * mod1;
                        m_ModelTransPos[3].y = cos(radr) * -20.0f * mod1;
                        m_ModelTransPos[5].x = sin(radl) * -60.0f * mod1;
                        m_ModelTransPos[5].y = cos(radl) * -20.0f * mod1;

                        m_ModelTransRot[3].x += 180.0f * mod1;

                        //m_ModelTransPos[3].y = (50.0f + abs(cos(radr)) * -600.0f) * mod1;
                        //m_ModelTransPos[5].y = (50.0f + abs(cos(radl)) * -600.0f) * mod1;
                        m_ModelTransPos[3].y += -180.0f * mod1;
                        m_ModelTransPos[5].y += -180.0f * mod1;
                        m_ModelTransPos[3].z = -300.0f * mod1;
                        m_ModelTransPos[5].z = -400.0f * mod1;

                    }
                }
            }
            else if (m_Anim_State == AnimState::SuperBeam)
            { //
                float mod = 1.0f - m_Anim_Mod;
                float mod1 = 1.0f;
                float mod2 = 1.0f;
                {
                    float kamaeF = 5;

                    mod1 = EasingInSine(std::min((float)(m_Anim_Frame) / (kamaeF), 1.0f));
                    //if (mod1 >= 0.95f)
                    //{
                    //    mod1 += sin((float)m_Obj->frameActive * 1.0f) * 0.0025f;
                    //}
                }

                {
                    float kamaeF = 20;
                    if (kamaeF > m_Anim_FrameMax)
                    {
                        kamaeF = m_Anim_FrameMax * 0.6f;
                    }

                    mod2 = EasingInSine(
                        1.0f - std::max(
                            std::min(
                                (float)(m_Anim_Frame - (m_Anim_FrameMax - kamaeF)) / (kamaeF),
                                1.0f),
                            0.0f)
                    );
                    //if (mod1 >= 0.95f)
                    //{
                    //    mod1 += sin((float)m_Obj->frameActive * 1.0f) * 0.05f;
                    //}
                }

                for (int i = 0; i < MODEL_NUM; i++)
                {
                    m_ModelTransPos[i] = {};
                    m_ModelTransRot[i] = {};
                }

                //へっど
                {
                    m_ModelTransRot[0] = {};
                    m_ModelTransPos[0].y += 80.0f * mod1 * mod2;
                }

                //腕
                {
                    m_ModelTransRot[3].x = -90.0f * mod1 * mod2;
                    m_ModelTransRot[5].x = -90.0f * mod1 * mod2;
                    m_ModelTransRot[3].y = -5.0f * mod1 * mod2;
                    m_ModelTransRot[5].y = +5.0f * mod1 * mod2;

                    m_ModelTransPos[3].x = -80.0f * mod1 * mod2;
                    m_ModelTransPos[5].x = +80.0f * mod1 * mod2;
                    m_ModelTransPos[3].y = -80.0f * mod1 * mod2;
                    m_ModelTransPos[5].y = -80.0f * mod1 * mod2;
                    m_ModelTransPos[3].z = 400.0f * mod1 * mod2;
                    m_ModelTransPos[5].z = 400.0f * mod1 * mod2;
                }
            }
            else if (m_Anim_State == AnimState::Damage)
            { //
                m_HeadVecBuf = m_BeforeVec;
                m_HeadVec = m_HeadVecBuf;

                float mod = 1.0f - m_Anim_Mod;
                float mod1 = 1.0f;
                float mod2 = 1.0f;
                {
                    float kamaeF = 5;

                    mod1 = EasingInSine(std::min((float)(m_Anim_Frame) / (kamaeF), 1.0f));
                    //if (mod1 >= 0.95f)
                    //{
                    //    mod1 += sin((float)m_Obj->frameActive * 1.0f) * 0.0025f;
                    //}
                }

                {
                    float kamaeF = 20;
                    if (kamaeF > m_Anim_FrameMax)
                    {
                        kamaeF = m_Anim_FrameMax * 0.6f;
                    }

                    mod2 = EasingInSine(
                        1.0f - std::max(
                            std::min(
                                (float)(m_Anim_Frame - (m_Anim_FrameMax - kamaeF)) / (kamaeF),
                                1.0f),
                            0.0f)
                    );
                    //if (mod1 >= 0.95f)
                    //{
                    //    mod1 += sin((float)m_Obj->frameActive * 1.0f) * 0.05f;
                    //}
                }

                for (int i = 0; i < MODEL_NUM; i++)
                {
                    m_ModelTransPos[i] = {};
                    m_ModelTransRot[i] = {};
                }

                //へっど
                {
                    m_ModelTransRot[0].x = -35.0f * mod1 * mod2;
                    m_ModelTransRot[0].z = -10.0f * mod1 * mod2;
                    m_ModelTransPos[0].y += -90.0f * mod1 * mod2;
                    m_ModelTransPos[0].z += -30.0f * mod1 * mod2;
                }
                //ぼ
                {
                    m_ModelTransRot[1].x = -12.0f * mod1 * mod2;
                    m_ModelTransRot[1].z = -5.0f * mod1 * mod2;
                    m_ModelTransPos[1].y += -70.0f * mod1 * mod2;

                    m_ModelTransRot[2].x = -10.0f * mod1 * mod2;
                    m_ModelTransRot[2].z = -2.0f * mod1 * mod2;
                    m_ModelTransPos[2].y += -10.0f * mod1 * mod2;
                }

                //腕
                {
                    m_ModelTransRot[3].z = -8.0f * mod1 * mod2;
                    m_ModelTransRot[5].z = +8.0f * mod1 * mod2;

                    m_ModelTransPos[3].x = +40.0f * mod1 * mod2;
                    m_ModelTransPos[5].x = -40.0f * mod1 * mod2;
                    m_ModelTransPos[3].y = -10.0f * mod1 * mod2;
                    m_ModelTransPos[5].y = -10.0f * mod1 * mod2;
                    m_ModelTransPos[3].z = -180.0f * mod1 * mod2;
                    m_ModelTransPos[5].z = -180.0f * mod1 * mod2;
                }
            }
            else if (m_Anim_State == AnimState::Damage2)
            { //
                m_HeadVecBuf = m_BeforeVec;
                m_HeadVec = m_HeadVecBuf;

                float mod = 1.0f - m_Anim_Mod;
                float mod1 = 1.0f;
                float mod2 = 1.0f;
                {
                    float kamaeF = 20;

                    mod1 = EasingInSine(std::min((float)(m_Anim_Frame) / (kamaeF), 1.0f));
                    //if (mod1 >= 0.95f)
                    //{
                    //    mod1 += sin((float)m_Obj->frameActive * 1.0f) * 0.0025f;
                    //}
                }

                {
                    float kamaeF = 20;
                    if (kamaeF > m_Anim_FrameMax)
                    {
                        kamaeF = m_Anim_FrameMax * 0.6f;
                    }

                    mod2 = EasingInSine(
                        1.0f - std::max(
                            std::min(
                                (float)(m_Anim_Frame - (m_Anim_FrameMax - kamaeF)) / (kamaeF),
                                1.0f),
                            0.0f)
                    );
                    //if (mod1 >= 0.95f)
                    //{
                    //    mod1 += sin((float)m_Obj->frameActive * 1.0f) * 0.05f;
                    //}
                }

                for (int i = 0; i < MODEL_NUM; i++)
                {
                    m_ModelTransPos[i] = {};
                    m_ModelTransRot[i] = {};
                }

                //へっど
                {
                    m_ModelTransRot[0].x = 35.0f * mod1 * mod2;
                    m_ModelTransRot[0].z = -5.0f * mod1 * mod2;
                    m_ModelTransPos[0].y += -90.0f * mod1 * mod2;
                    m_ModelTransPos[0].z += 30.0f * mod1 * mod2;
                }
                //ぼ
                {
                    m_ModelTransRot[1].x = 12.0f * mod1 * mod2;
                    m_ModelTransRot[1].z = -4.0f * mod1 * mod2;
                    m_ModelTransPos[1].y += -70.0f * mod1 * mod2;

                    m_ModelTransRot[2].x = 10.0f * mod1 * mod2;
                    m_ModelTransRot[2].z = -1.0f * mod1 * mod2;
                    m_ModelTransPos[2].y += -10.0f * mod1 * mod2;
                }

                //腕
                {
                    m_ModelTransRot[3].z = +4.0f * mod1 * mod2;
                    m_ModelTransRot[5].z = -4.0f * mod1 * mod2;

                    m_ModelTransPos[3].x = +20.0f * mod1 * mod2;
                    m_ModelTransPos[5].x = -20.0f * mod1 * mod2;
                    m_ModelTransPos[3].y = 5.0f * mod1 * mod2;
                    m_ModelTransPos[5].y = 5.0f * mod1 * mod2;
                    m_ModelTransPos[3].z = -180.0f * mod1 * mod2;
                    m_ModelTransPos[5].z = -180.0f * mod1 * mod2;
                }


                for (int i = 0; i < MODEL_NUM; i++)
                {
                    m_ModelTransPos[i].x += cos(-1.4f * m_Obj->frame) * 30.0f;
                    m_ModelTransPos[i].y += sin(-0.9f * m_Obj->frame) * 40.0f;
                }
            }


            else if (m_Anim_State == AnimState::Stating_ArmR_Purge)
            {
                m_ModelTransPos[5].x = -400.0f * m_Anim_Mod;
            }
            else if (m_Anim_State == AnimState::Stating_ArmR_Up)
            {
                float mod = EasingOutExpo(1.0f - m_Anim_Mod);
                m_ModelTransPos[5].x *= mod;
                m_ModelTransPos[5].y *= mod;
                m_ModelTransRot[5].x *= mod;
                m_ModelTransRot[5].z *= mod;
            }
            else if (m_Anim_State == AnimState::Stating_ArmL_Purge)
            {
                m_ModelTransPos[3].x = 400.0f * m_Anim_Mod;
            }
            else if (m_Anim_State == AnimState::Stating_ArmL_Up)
            {
                float mod = EasingOutExpo(1.0f - m_Anim_Mod);
                m_ModelTransPos[3].x *= mod;
                m_ModelTransPos[3].y *= mod;
                m_ModelTransRot[3].x *= mod;
                m_ModelTransRot[3].z *= mod;
            }
            else if (m_Anim_State == AnimState::Stating_Body1_Up)
            {
                float mod = EasingOutExpo(1.0f - m_Anim_Mod);
                float beforey = m_ModelTransPos[1].y;
                m_ModelTransPos[1].y *= mod;
                //m_ModelTransPos[1].z = mod  * -1000.0f * (sin(2.0f * SYS_GENERAL_PI * (m_Anim_Mod)) + 1.0f) / 2;
                //m_ModelTransRot[1].y *= mod;
                //m_ModelTransRot[2].y *= mod;

                m_ModelTransPos[0].y += m_ModelTransPos[1].y - beforey;
            }
            else if (m_Anim_State == AnimState::Stating_Head_Up)
            {
                float mod = EasingOutExpo(1.0f - m_Anim_Mod);
                m_ModelTransPos[0].y *= mod;
                m_ModelTransPos[0].z *= mod;
                m_ModelTransRot[0].x *= mod;
            }
            else if (m_Anim_State == AnimState::Stating_All_Purge)
            {
                {
                    m_ModelTransPos[5].x = -400.0f * m_Anim_Mod;
                }
                {
                    m_ModelTransPos[3].x = 400.0f * m_Anim_Mod;
                }
            }
            else if (m_Anim_State == AnimState::Stating_All_Up)
            {
                {
                    float mod = EasingOutExpo(1.0f - m_Anim_Mod);
                    m_ModelTransPos[5].x *= mod;
                    m_ModelTransPos[5].y *= mod;
                    m_ModelTransRot[5].x *= mod;
                    m_ModelTransRot[5].z *= mod;
                }
                {
                    float mod = EasingOutExpo(1.0f - m_Anim_Mod);
                    m_ModelTransPos[3].x *= mod;
                    m_ModelTransPos[3].y *= mod;
                    m_ModelTransRot[3].x *= mod;
                    m_ModelTransRot[3].z *= mod;
                }
                {
                    float mod = EasingOutExpo(1.0f - m_Anim_Mod);
                    float beforey = m_ModelTransPos[1].y;
                    m_ModelTransPos[1].y *= mod;
                    //m_ModelTransPos[1].z = mod  * -1000.0f * (sin(2.0f * SYS_GENERAL_PI * (m_Anim_Mod)) + 1.0f) / 2;
                    //m_ModelTransRot[1].y *= mod;
                    //m_ModelTransRot[2].y *= mod;

                    m_ModelTransPos[0].y += m_ModelTransPos[1].y - beforey;
                }
            }
        }
    }

    if (m_Anim_Frame >= 0)
    {
        m_Anim_Frame++;
        m_Anim_Mod = (float)m_Anim_Frame / m_Anim_FrameMax;
        //if (m_Anim_Frame >= m_Anim_FrameMax)
        if (m_Anim_Frame > m_Anim_FrameMax)
        {
            if (!m_Anim_NoEndReset)
            {
                for (int i = 0; i < MODEL_NUM; i++)
                {
                    m_ModelTransPos[i] = {};
                    m_ModelTransRot[i] = {};
                    m_ModelTransSca[i].x = 1.0f;
                    m_ModelTransSca[i].y = 1.0f;
                    m_ModelTransSca[i].z = 1.0f;
                }

            }
            m_Anim_Mod = 0.0f;
            m_Anim_Frame = -1;
        }
    }

    {
        auto it = std::find_if(
            m_CallEnemies.begin(),
            m_CallEnemies.end(),
            [](auto buf)
            {
                GAME_OBJECT* targetObject = GetGameObjectFindByIdentifier(buf);
                return !IsActiveGameObject(targetObject);
            }
        );
        if (it != m_CallEnemies.end())
        {
            m_CallEnemies.erase(it);
            m_CallEnemies.shrink_to_fit();
        }
    }


    if (m_BodyTexMaskFrame >= 0)
    {
        m_BodyTexMaskFrame++;
        if (m_BodyTexMaskFrame > m_BodyTexMaskFrameMax)
        {
            m_BodyTexMaskFrame = -1;
        }
    }

    if (m_BodyTexMaskFrame_Init >= 0)
    {
        m_BodyTexMaskFrame_Init++;
        if (m_BodyTexMaskFrame_Init > m_BodyTexMaskFrameMax_Init)
        {
            m_BodyTexMaskFrame_Init = -2;
        }
    }
    if (m_BodyTexColorFrame_Init >= 0)
    {
        m_BodyTexColorFrame_Init++;
        if (m_BodyTexColorFrame_Init > m_BodyTexColorFrameMax_Init)
        {
            m_BodyTexColorFrame_Init = -2;
        }
    }

    if (m_DeadFlag)
    {
        //{
        //    GAME_OBJECT* go = GO_CreateAfterEffect(m_Obj, 30, MkF4(1.0f, 1.0f, 1.0f, 1.0f));
        //    if (go != nullptr)
        //    {
        //        go->graphic.stop = false;
        //        go->graphic.graph.spriteAnim[0].texDat = m_Tex_Defeat;
        //        go->graphic.graph.spriteAnim[0].anim = true;
        //        go->graphic.graph.spriteAnim[0].xcnt = 6;
        //        go->graphic.graph.spriteAnim[0].hcnt = 6;
        //        go->graphic.graph.spriteAnim[0].cntMax = 6 * 6;
        //        go->graphic.graph.spriteAnim[0].size.x = 3240 / 6;
        //        go->graphic.graph.spriteAnim[0].size.y = 3240 / 6;
        //        go->graphic.graph.spriteAnim[0].pos3D = {};
        //        //go->graphic.graph.spriteAnim[0].pos3D.z += -720.0f;
        //    }
        //}
        //DeleteGameObject(m_Obj);
    }
}


void GOE_Character_Enemy_Boss_Inperiusu::drawBody(int i, int flashnum, Float3 shakePos,
    DrawType type
)
{
    for (int ChildIndex = 0; ChildIndex < MIRRORGE_STATE_MAX + 1; ChildIndex++)
    {
        if (ChildIndex > 0 && m_MirrorgeState[ChildIndex - 1].inFrame <= 0)
        {
            continue;
        }

        drawBodyChild(ChildIndex, i, flashnum, shakePos,
             type
        );
    }
}

void GOE_Character_Enemy_Boss_Inperiusu::drawBodyChild(int ChildIndex, int i, int flashnum, Float3 shakePos,
    DrawType type
)
{
    if (i == 4 && m_BladeVisibleFrame <= 0)
    {
        return;
    }

    MIRRORGE_STATE currentMirrorgeState = {};
    if (ChildIndex > 0)
    {
        currentMirrorgeState = m_MirrorgeState[ChildIndex - 1];
    }
    else
    {
        currentMirrorgeState.axis = m_CurrentAxisPrivate;
        currentMirrorgeState.pos = m_Obj->pos;
        currentMirrorgeState.rot = {};
        currentMirrorgeState.sca = MkF3(1.0f, 1.0f, 1.0f);
    }

    DRAW_POLYGON_DAT drawDatTmp = {};
    drawDatTmp.pos = MkF3(
        0.0f + m_ModelTransPos[i].x * MODEL_SCA,
        0.0f + m_ModelTransPos[i].y * MODEL_SCA,
        0.0f + m_ModelTransPos[i].z * MODEL_SCA
    );
    drawDatTmp.size = MkF3(
        m_ModelTransSca[i].x * +9.0f * MODEL_SCA,
        m_ModelTransSca[i].y * -9.0f * MODEL_SCA,
        m_ModelTransSca[i].z * +9.0f * MODEL_SCA
    );

    drawDatTmp.rot = 0.0f;
    //drawDatTmp.rot3D = MkF3(0.0f, 0.0f + (float)GetMainGameFrame() * 2.2f, 0.0f);
    drawDatTmp.rot3D = MkF3(
        m_ModelTransRot[i].x + 0.0f,
        m_ModelTransRot[i].y + 0.0f,
        m_ModelTransRot[i].z + 0.0f
    );


    drawDatTmp.texUV = { 0.0f, 0.0f };
    drawDatTmp.texWH = { 1.0f, 1.0f };

    drawDatTmp.startPos3D = currentMirrorgeState.pos;
    drawDatTmp.startPos3D.z += -800.0f;

    drawDatTmp.startRot3D = MkF3(
        90.0f,
        0.0f,
        0.0f + currentMirrorgeState.axis
    );
    drawDatTmp.startScale = MkF2(1.0f, 1.0f);
    drawDatTmp.startScale3D = MkF3(1.0f, 1.0f, 1.0f);

    drawDatTmp.modelNo = m_ModelIndex[i];

    drawDatTmp.shaderOpt = m_Obj->graphic.shaderOpt;

    drawDatTmp.startPos3D.x += shakePos.x;
    drawDatTmp.startPos3D.y += shakePos.y;
    drawDatTmp.startPos3D.z += shakePos.z;

    DRAW_POLYGON_IN_CAMERA_EX_OPTION opt = {};
    opt.reverse = false;
    opt.fixed = false;
    opt.enableShade = true;
    opt.shaderType = m_shaderType;

    //opt.priority = DRAW_POLYGON_PRIORITY_FORCE_ALWAYS;
    opt.priority = DRAW_POLYGON_PRIORITY_NORMAL;
    //opt.priority = DRAW_POLYGON_PRIORITY_FORCE_NORMAL;

    DPD_SHADER_TYPE cutinShaderType = 
        m_shaderType == DPD_SHADER_TYPE_METAL_ENEMY_2 ?
        DPD_SHADER_TYPE_CUTIN_METAL_ENEMY_2:
        DPD_SHADER_TYPE_CUTIN_METAL_ENEMY
        ;

    if (type == DrawType::Mask)
    {
        if (ChildIndex > 0)
        {
            return;
        }
        if (i == 4)
        {
            return;
        }

        drawDatTmp.texNo = m_Tex_Model_Mask.textureId;
        drawDatTmp.loadTexType = LOADTEXTURETYPE_MAIN;

        float mod1 = 1.0f;
        float mod2 = 1.0f;
        {
            float kamaeF = 20;

            mod1 = EasingInSine(std::min((float)(m_BodyTexMaskFrame) / (kamaeF), 1.0f));
        }

        {
            float kamaeF = 20;
            if (kamaeF > m_BodyTexMaskFrameMax)
            {
                kamaeF = m_BodyTexMaskFrameMax * 0.6f;
            }

            mod2 = EasingInSine(
                1.0f - std::max(
                    std::min(
                        (float)(m_BodyTexMaskFrame - (m_BodyTexMaskFrameMax - kamaeF)) / (kamaeF),
                        1.0f),
                    0.0f)
            );
        }

        Float4 defaultColor = {};
        Float4 attackColor = {};
        if (m_IsHard)
        {
            defaultColor = MkF4(1.0f, 1.0f, 1.0f, 1.0f);
            if (!m_BodyTexMaskInitFlag)
            {
                defaultColor = MkF4(0.01f, 0.01f, 0.01f, 1.0f);
            }
            //attackColor = MkF4(3.0f, 0.5f, 0.5f, 1.0f);
            attackColor = MkF4(1.5f, 0.4f, 0.4f, 1.0f);

            if (m_Anim_Frame >= 0 && m_Anim_State == AnimState::Damage)
            {
                defaultColor = MkF4(
                    0.0f + (m_Anim_Frame % 8 < 4 ? 2.0f: 0.0f),
                    0.0f,
                    0.0f,
                    1.0f
                );

                drawDatTmp.texNo = m_Tex_Model_MaskGray.textureId;
            }
            else  if (m_Anim_Frame >= 0 && m_Anim_State == AnimState::Damage2)
            {
                defaultColor = MkF4(
                    0.0f,
                    0.0f,
                    0.0f,
                    1.0f
                );

                drawDatTmp.texNo = m_Tex_Model_MaskGray.textureId;
            }
            else if (m_HardPhase2Flag)
            {
                //defaultColor = MkF4(
                //    0.7f,
                //    0.0f,
                //    0.1f + 0.4f * (sin((float)m_Obj->frameActive / 12) / 2.0f + 0.5f),
                //    1.0f
                //);
                defaultColor = MkF4(
                    0.7f,
                    0.0f + 0.1f * (sin((float)m_Obj->frameActive / 18) / 2.0f + 0.5f),
                    0.0f,
                    1.0f
                );

                drawDatTmp.texNo = m_Tex_Model_MaskGray.textureId;
                drawDatTmp.shaderOpt.blendState = DPD_SHADER_OPT_BLEND_STATE_ADDCOLOR;
            }
        }
        else
        {
            //drawDatTmp.color = MkF4(0.0f, 1.0f, 0.0f, 1.0f);
            //drawDatTmp.color.x += 2.0f * (mod1 * mod2);
            //drawDatTmp.color.y += -1.0f * (mod1 * mod2);
            //drawDatTmp.color.z += 0.7f * (mod1 * mod2);
            //defaultColor = MkF4(0.1f, 1.6f, 0.1f, 1.0f);
            defaultColor = MkF4(0.0f, 0.9f, 0.0f, 1.0f);
            if (!m_BodyTexMaskInitFlag)
            {
                defaultColor = MkF4(0.01f, 0.01f, 0.01f, 1.0f);
            }
            //attackColor = MkF4(2.0f, 0.0f, 0.4f, 1.0f);
            //attackColor = MkF4(2.0f, 0.0f, 0.9f, 1.0f);
            attackColor = MkF4(0.9f, 0.0f, 0.5f, 1.0f);

            if (m_Anim_Frame >= 0 && m_Anim_State == AnimState::Damage)
            {
                defaultColor = MkF4(
                    0.0f + (m_Anim_Frame % 8 < 4 ? 2.0f : 0.0f),
                    0.0f,
                    0.0f,
                    1.0f
                );

                drawDatTmp.texNo = m_Tex_Model_MaskGray.textureId;
            }
            else  if (m_Anim_Frame >= 0 && m_Anim_State == AnimState::Damage2)
            {
                defaultColor = MkF4(
                    0.0f,
                    0.0f,
                    0.0f,
                    1.0f
                );

                drawDatTmp.texNo = m_Tex_Model_MaskGray.textureId;
            }
            else if(m_SuperBeamSuccessWeaknessFlag)
            {
                Float4 dctmp = MkF4(1.8f, 1.8f, 0.0f, 1.0f);
                Float4 mctmp = MkF4(1.8f, 0.0f, 0.0f, 1.0f);

                float tenmetsumod = (sin((float)m_Obj->frameActive / 10) / 2.0f + 0.5f);

                defaultColor = dctmp;
                defaultColor.x += (mctmp.x - dctmp.x) * tenmetsumod;
                defaultColor.y += (mctmp.y - dctmp.y) * tenmetsumod;
                defaultColor.z += (mctmp.z - dctmp.z) * tenmetsumod;
            }
            else
            {
                if (m_BodyTexMaskFrame_Init >= -1)
                {
                    Float4 mctmp = MkF4(0.2f, 0.3f, 0.2f, 1.0f);

                    float startmod = 1.0f - std::max(
                        (float)m_BodyTexMaskFrame_Init / m_BodyTexMaskFrameMax_Init,
                        0.0f);

                    defaultColor.x += (mctmp.x - defaultColor.x) * startmod;
                    defaultColor.y += (mctmp.y - defaultColor.y) * startmod;
                    defaultColor.z += (mctmp.z - defaultColor.z) * startmod;
                }
                
            }
        }
        drawDatTmp.color = defaultColor;
        drawDatTmp.color.x += (attackColor.x - defaultColor.x) * (mod1 * mod2);
        drawDatTmp.color.y += (attackColor.y - defaultColor.y) * (mod1 * mod2);
        drawDatTmp.color.z += (attackColor.z - defaultColor.z) * (mod1 * mod2);
        //drawDatTmp.color.w *= mod1 * mod2;

        //Float3 svec = NormalizeVector(drawDatTmp.size);
        //drawDatTmp.size.x += svec.x * 1.0f;
        //drawDatTmp.size.y += svec.y * 1.0f;
        //drawDatTmp.size.z += svec.z * 1.0f;

        //drawDatTmp.shaderOpt.outline = DPD_SHADER_OPT_OUTLINE_NONE;
        
        //opt.shaderType = DPD_SHADER_TYPE_NONE;
        //opt.priority = DRAW_POLYGON_PRIORITY_ALWAYS;
        //opt.depthBuffIgnore = true;

        //opt.shaderType = DPD_SHADER_TYPE_NONE;
        //opt.shaderType = DPD_SHADER_TYPE_EFFECT;

        opt.shaderType = DPD_SHADER_TYPE_METAL;
        drawDatTmp.color.x *= 3.0f;
        drawDatTmp.color.y *= 3.0f;
        drawDatTmp.color.z *= 3.0f;

        if (m_StartMotionShaderFlag)
        {
            opt.priority = DRAW_POLYGON_PRIORITY_ALWAYS;
            drawDatTmp.color.x *= 1.5f;
            drawDatTmp.color.y *= 1.5f;
            drawDatTmp.color.z *= 1.5f;

            ///opt.shaderType = cutinShaderType;
        }
    }
    else if (type == DrawType::Dead)
    {
        float mod = ((float)(m_Dead_Frame) / m_Dead_FrameMax);

        //mod = (EasingOutQuad(mod));
        mod = (EasingInQuad(1.0f - mod));

        drawDatTmp.shaderOpt.outline = DPD_SHADER_OPT_OUTLINE_NONE;

        drawDatTmp.loadTexType = LOADTEXTURETYPE_MAIN;

        opt.priority = DRAW_POLYGON_PRIORITY_ALWAYS;

        constexpr float coloutmod = 0.25f;
        float colmod = 0.45f + (0.55f + coloutmod) * mod;
        if (colmod > 1.0f)
        {
            float colmod2 = 1.0f - ((colmod - 1.0f) / coloutmod);
            float addcol = 100.0f * colmod2;


            if (i == 4)
            {
                drawDatTmp.color = MkF4(
                    2.0f + addcol,
                    2.0f + addcol,
                    2.0f + addcol,
                    0.5f);
                drawDatTmp.texNo = 0;
                drawDatTmp.shaderOpt.blendState = DPD_SHADER_OPT_BLEND_STATE_ADDCOLOR;
            }
            else
            {
                drawDatTmp.color = MkF4(
                    1.0f + addcol,
                    1.0f + addcol,
                    1.0f + addcol,
                    1.0f);
                drawDatTmp.texNo = m_Tex_Model.textureId;
            }
        }
        else
        {
            drawDatTmp.color = MkF4(
                colmod,
                colmod,
                colmod,
                2.0f);
            //drawDatTmp.color = MkF4(
            //    1.0f * mod,
            //    1.0f * mod,
            //    1.0f * mod,
            //    2.0f);
            drawDatTmp.texNo = m_Tex_Dis.textureId;
            drawDatTmp.shaderOpt.blendState = DPD_SHADER_OPT_BLEND_STATE_ADDCOLOR;
            drawDatTmp.texWH = {
                1.0f * 1.5f,
                1.0f * 1.5f
            };

            //opt.priority = DRAW_POLYGON_PRIORITY_ALWAYS;

            opt.shaderType = DPD_SHADER_TYPE_EFFECT_DIS;
        }
    }
    else if (ChildIndex <= 0 && (type == DrawType::WarpIn || type == DrawType::WarpOut))
    {
        float mod = ((float)(m_WarpFrame) / m_WarpFrameMax);

        if (type == DrawType::WarpOut)
        {
            //mod = EasingInBack(EasingOutSine(1.0f - mod));
            mod = (EasingInQuad(1.0f - mod));

            drawDatTmp.startPos3D = m_WarpPosBefore;
            drawDatTmp.startPos3D.z += -800.0f;

            drawDatTmp.startRot3D = MkF3(
                90.0f,
                0.0f,
                0.0f + m_WarpAxisBefore
            );
        }
        else
        {
            //mod = EasingInBack(EasingOutSine(mod));
            mod = (EasingOutQuad(mod));
        }

        {
            drawDatTmp.startScale3D.x = 0.2f + 0.8f * EasingInOutBack(mod);
            drawDatTmp.startScale3D.y = 2.5f - 1.5f * EasingInOutBack(mod);
            drawDatTmp.startScale3D.z = 0.2f + 0.8f * EasingInOutBack(mod);
        }

        drawDatTmp.shaderOpt.outline = DPD_SHADER_OPT_OUTLINE_NONE;

        drawDatTmp.loadTexType = LOADTEXTURETYPE_MAIN;

        constexpr float coloutmod = 0.1f;
        float colmod = 0.45f + (0.55f + coloutmod) *mod;
        if (colmod > 1.0f)
        {
            float colmod2 = 1.0f - ((colmod - 1.0f) / coloutmod);
            float addcol = 10.0f * colmod2;


            if (i == 4)
            {
                //drawDatTmp.color = MkF4(
                //    1.75f + addcol,
                //    1.10f + addcol,
                //    2.00f + addcol,
                //    1.0f);
                //drawDatTmp.texUV.x = (float)m_Obj->frameActive * 0.03f;
                //drawDatTmp.texUV.y = (float)m_Obj->frameActive * 0.1f;
                //drawDatTmp.shaderOpt.blendState = DPD_SHADER_OPT_BLEND_STATE_ADDCOLOR;

                //drawDatTmp.texNo = m_Tex_Model_Blade.textureId;
                drawDatTmp.color = MkF4(
                    2.0f + addcol,
                    2.0f + addcol,
                    2.0f + addcol,
                    0.5f);
                drawDatTmp.texNo = 0;
                drawDatTmp.shaderOpt.blendState = DPD_SHADER_OPT_BLEND_STATE_ADDCOLOR;
                opt.shaderType = DPD_SHADER_TYPE_EFFECT;
            }
            else
            {
                drawDatTmp.color = MkF4(
                    1.0f + addcol,
                    1.0f + addcol,
                    1.0f + addcol,
                    1.0f);
                drawDatTmp.texNo = m_Tex_Model.textureId;
            }
        }
        else
        {
            drawDatTmp.color = MkF4(
                colmod,
                colmod,
                colmod,
                2.0f);
            //drawDatTmp.color = MkF4(
            //    1.0f * mod,
            //    1.0f * mod,
            //    1.0f * mod,
            //    2.0f);
            drawDatTmp.texNo = m_Tex_Warp.textureId;
            drawDatTmp.shaderOpt.blendState = DPD_SHADER_OPT_BLEND_STATE_ADDCOLOR;
            drawDatTmp.texWH = {
                1.0f * 2.0f,
                1.0f * 2.0f
            };

            //opt.priority = DRAW_POLYGON_PRIORITY_ALWAYS;

            opt.shaderType = DPD_SHADER_TYPE_EFFECT_WARP;
        }
    }
    else if (type == DrawType::CutInShadow)
    {
        drawDatTmp.loadTexType = LOADTEXTURETYPE_GRAY;

        drawDatTmp.pos.x += 2800.0f;
        drawDatTmp.pos.y += 800.0f;
        drawDatTmp.pos.z += 4000.0f;
        drawDatTmp.size.x *= 3.6f;
        drawDatTmp.size.y *= 3.6f;
        drawDatTmp.size.z *= 3.6f;
        //drawDatTmp.startPos3D.y = -1000.0f;
        //drawDatTmp.startPos3D.z = -200.0f;

        drawDatTmp.color = MkF4(0.75f, 0.0f, 1.0f, 2.0f);

        drawDatTmp.texNo = 0;

        drawDatTmp.shaderOpt.outline = DPD_SHADER_OPT_OUTLINE_NONE;
        drawDatTmp.shaderOpt.bloom = DPD_SHADER_OPT_BLOOM_NONE;

        opt.depthBuffIgnore = true;
        opt.priority = DRAW_POLYGON_PRIORITY_NML_ALWAYS;

        //opt.shaderType = DPD_SHADER_TYPE_CUTIN;
        opt.shaderType = cutinShaderType;
    }
    else if (type == DrawType::Shadow)
    { //影
        drawDatTmp.loadTexType = LOADTEXTURETYPE_GRAY;

        drawDatTmp.startPos3D.z = -15.0f;
        drawDatTmp.startRot3D.x = 90.0f;
        drawDatTmp.startScale3D.y *= 0.001f;

        {
            float mo = 1.0f - std::max(std::min(abs(m_Obj->pos.z) / (ONE_BLOCK * 80.0f), 1.0f), 0.0f);
            drawDatTmp.startScale3D.x *= mo;
            drawDatTmp.startScale3D.z *= mo;
        }

        drawDatTmp.color = MkF4(-1.0f, -1.0f, -1.0f, 1.0f);

        drawDatTmp.texNo = 0;

        drawDatTmp.shaderOpt.outline = DPD_SHADER_OPT_OUTLINE_NONE;
        drawDatTmp.shaderOpt.bloom = DPD_SHADER_OPT_BLOOM_NONE;
    }
    else
    { //デフォ

        if (ChildIndex > 0)
        {
            //if (currentMirrorgeState.inFrame <= 0)
            //{
            //    return;
            //}

            float mod = ((float)(currentMirrorgeState.inFrame) / MIRRORGE_STATE_INFRAME_MAX);

            //mod = EasingInBack(EasingOutSine(mod));
            mod = (EasingOutQuad(mod));

            {
                drawDatTmp.startScale3D.x = 0.2f + 0.8f * EasingInOutBack(mod);
                drawDatTmp.startScale3D.y = 2.5f - 1.5f * EasingInOutBack(mod);
                drawDatTmp.startScale3D.z = 0.2f + 0.8f * EasingInOutBack(mod);
            }

            drawDatTmp.shaderOpt.outline = DPD_SHADER_OPT_OUTLINE_NONE;

            drawDatTmp.loadTexType = LOADTEXTURETYPE_MAIN;

            drawDatTmp.texNo = m_Tex_Warp.textureId;

            constexpr float coloutmod = 0.1f;
            float colmod = 0.45f + (0.55f + coloutmod) * mod;
            if (colmod > 1.0f)
            {
                float colmod2 = 1.0f - ((colmod - 1.0f) / coloutmod);
                float addcol = 10.0f * colmod2;

                drawDatTmp.texUV.x = (float)m_Obj->frameActive * 0.03f;
                drawDatTmp.texUV.y = (float)m_Obj->frameActive * 0.02f;

                if (i == 4)
                {
                    drawDatTmp.color = MkF4(
                        2.0f + addcol,
                        2.0f + addcol,
                        2.0f + addcol,
                        0.5f);
                    drawDatTmp.texNo = 0;
                    drawDatTmp.shaderOpt.blendState = DPD_SHADER_OPT_BLEND_STATE_ADDCOLOR;
                    opt.shaderType = DPD_SHADER_TYPE_EFFECT;
                }
                else
                {
                    drawDatTmp.color = MkF4(
                        1.0f + addcol,
                        1.0f + addcol,
                        1.0f + addcol,
                        1.0f);
                    //drawDatTmp.texNo = m_Tex_Model.textureId;
                }
            }
            else
            {
                if (i == 4)
                {
                    drawDatTmp.color = MkF4(
                        colmod,
                        colmod,
                        colmod,
                        2.0f);
                    drawDatTmp.texNo = 0;
                    opt.shaderType = DPD_SHADER_TYPE_EFFECT;
                }
                else
                {
                    drawDatTmp.color = MkF4(
                        colmod,
                        colmod,
                        colmod,
                        2.0f);
                }
                drawDatTmp.shaderOpt.blendState = DPD_SHADER_OPT_BLEND_STATE_ADDCOLOR;
                //drawDatTmp.texWH = {
                //    1.0f * 2.0f,
                //    1.0f * 2.0f
                //};

                opt.shaderType = DPD_SHADER_TYPE_EFFECT_WARP;

                //opt.priority = DRAW_POLYGON_PRIORITY_ALWAYS
            }
        }
        else
        {
            drawDatTmp.loadTexType = LOADTEXTURETYPE_MAIN;

            if (i == 4)
            {
                float bladeVisibleMod = (float)m_BladeVisibleFrame / m_BladeVisibleFrameMax;
                if (bladeVisibleMod <= 0)
                {
                    return;
                }

                drawDatTmp.shaderOpt.outline = DPD_SHADER_OPT_OUTLINE_NONE;

                constexpr float coloutmod = 0.1f;
                //float colmod = 0.45f + (0.55f + coloutmod) * bladeVisibleMod;
                float colmod = 0.1f + (0.9f + coloutmod) * bladeVisibleMod;
                if (colmod > 1.0f)
                {
                    float colmod2 = 1.0f - ((colmod - 1.0f) / coloutmod);
                    float addcol = 10.0f * colmod2;


                    drawDatTmp.texNo = 0;
                    drawDatTmp.shaderOpt.blendState = DPD_SHADER_OPT_BLEND_STATE_DEFAULT;
                    drawDatTmp.color = MkF4(
                        0.2f,
                        0.0f,
                        0.1f,
                        0.2f);
                    opt.priority = (DRAW_POLYGON_PRIORITY)(opt.priority - 1);
                    opt.depthBuffIgnore = true;
                    opt.shaderType = DPD_SHADER_TYPE_EFFECT;

                    DrawPolygonInCameraEx(drawDatTmp, opt);


                    drawDatTmp.texUV.x = (float)m_Obj->frameActive * 0.02f;
                    drawDatTmp.texUV.y = (float)m_Obj->frameActive * 0.03f;
                    drawDatTmp.texWH.x = 0.7f;
                    drawDatTmp.texWH.y = 0.7f;
                    drawDatTmp.texNo = m_Tex_Model_Blade.textureId;
                    drawDatTmp.shaderOpt.blendState = DPD_SHADER_OPT_BLEND_STATE_ADDCOLOR;
                    //drawDatTmp.color = MkF4(
                    //    1.00f + addcol,
                    //    0.30f + addcol,
                    //    1.30f + addcol,
                    //    1.0f);
                    drawDatTmp.color = MkF4(
                        0.70f + addcol,
                        0.30f + addcol,
                        2.50f + addcol,
                        1.0f);
                    opt.priority = (DRAW_POLYGON_PRIORITY)(opt.priority + 1);
                    opt.depthBuffIgnore = false;
                    opt.shaderType = DPD_SHADER_TYPE_EFFECT;
                }
                else
                {
                    drawDatTmp.color = MkF4(
                        colmod,
                        colmod,
                        colmod,
                        2.0f);
                    drawDatTmp.texNo = m_Tex_Warp.textureId;
                    drawDatTmp.shaderOpt.blendState = DPD_SHADER_OPT_BLEND_STATE_ADDCOLOR;
                    drawDatTmp.texWH = {
                        1.0f * 1.2f,
                        1.0f * 1.2f
                    };

                    opt.shaderType = DPD_SHADER_TYPE_EFFECT_WARP;
                }

            }
            else
            {
                switch (flashnum)
                {
                case 1:
                    drawDatTmp.loadTexType = LOADTEXTURETYPE_GRAY;
                    drawDatTmp.color = MkF4(1.6f, 1.6f, 1.6f, 2.0f);
                    break;
                case 2:
                    drawDatTmp.loadTexType = LOADTEXTURETYPE_GRAY;
                    drawDatTmp.color = MkF4(0.5f, 0.5f, 0.5f, 2.0f);
                    break;
                default:
                    drawDatTmp.color = MkF4(1.0f, 1.0f, 1.0f, 1.0f);
                    break;
                }
                drawDatTmp.texNo = m_Tex_Model.textureId;


                if (m_BodyTexColorFrame_Init >= -1)
                {
                    //Float4 mctmp = MkF4(0.75f, 0.85f, 0.7f, 1.0f);
                    //Float4 mctmp = MkF4(0.65f, 0.8f, 0.6f, 1.0f);
                    Float4 mctmp = MkF4(0.55f, 0.7f, 0.5f, 1.0f);

                    float startmod = 1.0f - std::max(
                        (float)m_BodyTexColorFrame_Init / m_BodyTexColorFrameMax_Init,
                        0.0f);

                    drawDatTmp.color.x += (mctmp.x - drawDatTmp.color.x) * startmod;
                    drawDatTmp.color.y += (mctmp.y - drawDatTmp.color.y) * startmod;
                    drawDatTmp.color.z += (mctmp.z - drawDatTmp.color.z) * startmod;
                }
            }
            if (m_StartMotionShaderFlag)
            {
                opt.priority = DRAW_POLYGON_PRIORITY_ALWAYS;

                opt.shaderType = cutinShaderType;
            }

            //if (true)
            //{
            //    float mod = 1.0f - (float)((m_Obj->frameActive * 8) % 1000) / 1000;

            //    drawDatTmp.color = MkF4(
            //        1.0f * mod,
            //        1.0f * mod,
            //        1.0f * mod,
            //        2.0f);
            //    drawDatTmp.loadTexType = LOADTEXTURETYPE_MAIN;
            //    drawDatTmp.texNo = m_Tex_Dis.textureId;
            //    drawDatTmp.shaderOpt.blendState = DPD_SHADER_OPT_BLEND_STATE_ADDCOLOR;
            //    drawDatTmp.texWH = {
            //        1.0f * 0.6f, 
            //        1.0f * 0.6f 
            //    };

            //    //opt.priority = DRAW_POLYGON_PRIORITY_ALWAYS;

            //    opt.shaderType = DPD_SHADER_TYPE_EFFECT_DIS;
            //}
        }
    }


    DrawPolygonInCameraEx(drawDatTmp, opt);
}

void GOE_Character_Enemy_Boss_Inperiusu::deadstop_event()
{
    //ムービーフラグ
    GameManagerSetMovieFlag(true);
    GameManagerSetMoviePlayerHiddenFlag(true);

    CameraFlashRequestAll(
        20,
        EasingInOutSine,
        MkF4(10.0f, 10.0f, 10.0f, 1.0f),
        DPD_SHADER_OPT_BLEND_STATE_ADDCOLOR
    );

    GameStopRequest(20);

    int waitf = 25;
    GameObjectHitStop(m_Obj, waitf);

    {
        SetCameraModeAll(CAMERA_MODE_NORMAL);

        Float3 zoomPos = m_Obj->pos;
        zoomPos.z += 41000.0f;

        Float3 zoomPosTa = m_Obj->pos;
        zoomPosTa.z += -18000.0f;

        CameraTargetMoveRequestAll(20,
            zoomPosTa,
            EasingInOutSine,
            CAMERA_MOVE_TYPE_SMOOTH
        );
        CameraMoveRequestAll(20,
            zoomPos,
            0.0f,
            MkF2(1.0f, 1.0f),
            0.28f,
            EasingInOutSine,
            CAMERA_MOVE_TYPE_SMOOTH
        );
    }
}

void GOE_Character_Enemy_Boss_Inperiusu::set_blade_visible(bool b)
{
    m_BladeVisibleFlag = b;

    float modtmp = (float)m_BladeVisibleFrame / m_BladeVisibleFrameMax;
    if (b)
    {
        m_BladeVisibleFrameMax = 40;
        if (m_BladeBeforeSpeedMod < 1.0f)
        {
            m_BladeVisibleFrameMax *= m_BladeBeforeSpeedMod;
        }
        m_BladeVisibleFrame = m_BladeVisibleFrameMax * modtmp;
    }
    else
    {
        m_BladeVisibleFrameMax = 80;
        m_BladeVisibleFrame = m_BladeVisibleFrameMax * modtmp;
    }
}

// ===================================================
// Draw
// オブジェクトの描画イベント
// ===================================================
void GOE_Character_Enemy_Boss_Inperiusu::Draw()
{
	SuperGOE::Draw();

    if (m_StartMotionShaderFlag)
    {
        {
            DRAW_POLYGON_DAT dsdat = {};
            dsdat.pos.x = 0.0f;
            dsdat.pos.y = 0.0f;
            dsdat.pos.z = 0.0f;

            dsdat.size.x = SCREEN_WIDTH;
            dsdat.size.y = SCREEN_HEIGHT;
            dsdat.size.z = 1.0f;

            dsdat.rot = 0.0f;

            //dsdat.color = { 0.0f, 0.0f, 1.0f, 1.0 };
            dsdat.color = { 0.1f, 0.1f, 0.2f, 1.0 };
            dsdat.texNo = 0;
            //dsdat.color = { 1.0f, 1.0f, 1.0f, 1.0 };
            dsdat.texNo = m_Tex_PowerBg;

            dsdat.loadTexType = LOADTEXTURETYPE_MAIN;

            dsdat.texUV.x += 0.01 * (float)m_Obj->frame;
            //dsdat.texWH = { 1.0f, 1.0f };
            dsdat.texWH = { 1.0f * ((float)SCREEN_HEIGHT / SCREEN_WIDTH), 1.0f };

            dsdat.startRot = 0.0f;
            dsdat.startScale = MkF2(1.0f, 1.0f);
            dsdat.startScale3D = MkF3(1.0f, 1.0f, 1.0f);

            dsdat.modelNo = 0;

            //dsdat.shaderOpt.bloom = true;
            //dsdat.shaderOpt.blendState = DPD_SHADER_OPT_BLEND_STATE_ADDCOLOR;

            DRAW_POLYGON_IN_CAMERA_EX_OPTION opt = {};
            opt.reverse = false;
            opt.fixed = true;
            opt.enableShade = false;
            opt.billboard = false;
            opt.priority = (DRAW_POLYGON_PRIORITY)(DRAW_POLYGON_PRIORITY_NML_ALWAYS - 1);
            opt.shaderType = DPD_SHADER_TYPE_EFFECT;

            DrawPolygonInCameraEx(dsdat, opt);
        }

        for (int i = 0; i < MODEL_NUM; i++)
        {
            drawBody(i, 0, {}, DrawType::CutInShadow);
        }
    }


    if (m_Dead_Frame > 0)
    {
        {
            DRAW_POLYGON_DAT dsdat = {};
            dsdat.pos.x = 0.0f;
            dsdat.pos.y = 0.0f;
            dsdat.pos.z = 0.0f;

            dsdat.size.x = SCREEN_WIDTH;
            dsdat.size.y = SCREEN_HEIGHT;
            dsdat.size.z = 1.0f;

            dsdat.rot = 0.0f;

            dsdat.color = { 
                0.15f,
                0.15f,
                0.15f, 
                1.0 };
            dsdat.texNo = 0;
            //dsdat.color = { 1.0f, 1.0f, 1.0f, 1.0 };
            //dsdat.texNo = m_Tex_PowerBg;

            dsdat.loadTexType = LOADTEXTURETYPE_MAIN;

            dsdat.texUV.x += 0.02 * (float)m_Obj->frame;
            //dsdat.texWH = { 1.0f, 1.0f };
            dsdat.texWH = { 1.0f * ((float)SCREEN_HEIGHT / SCREEN_WIDTH), 1.0f };

            dsdat.startRot = 0.0f;
            dsdat.startScale = MkF2(1.0f, 1.0f);
            dsdat.startScale3D = MkF3(1.0f, 1.0f, 1.0f);

            dsdat.modelNo = 0;

            //dsdat.shaderOpt.bloom = true;
            //dsdat.shaderOpt.blendState = DPD_SHADER_OPT_BLEND_STATE_ADDCOLOR;

            DRAW_POLYGON_IN_CAMERA_EX_OPTION opt = {};
            opt.reverse = false;
            opt.fixed = true;
            opt.enableShade = false;
            opt.billboard = false;
            opt.priority = (DRAW_POLYGON_PRIORITY)(DRAW_POLYGON_PRIORITY_NML_ALWAYS - 1);

            DrawPolygonInCameraEx(dsdat, opt);
        }
        for (int i = 0; i < MODEL_NUM; i++)
        {
            if (i == 4)
            {
                continue;
            }
            drawBody(i, 0, {}, DrawType::Dead);
        }
    }
    else if (m_WarpFrame >= 0)
    {
        for (int j = 0; j < 2; j++)
        {
            for (int i = 0; i < MODEL_NUM; i++)
            {
                drawBody(i, 0, {}, j == 0 ? DrawType::WarpOut : DrawType::WarpIn);
            }
        }
    }
    else
    {
        Float3 shakePos = {};
        int flashnum = 0;
        if (m_Obj->hitReaction.frame > 0 && m_Obj->hitReaction.frame < m_Obj->hitReaction.frameMax - 1)
        {
            float hrModTmp = (float)m_Obj->hitReaction.frame / m_Obj->hitReaction.frameMax;

            {
                int ftmp = m_Obj->hitReaction.frame % 6;
                if (ftmp < 2)
                {
                    flashnum = 1;
                }
                if (ftmp >= 4)
                {
                    flashnum = 2;
                }
            }

            float shakemod = 0.2f + 0.8f * std::min(std::max((float)m_Obj->status.beforeDamage / 300.0f, 0.0f), 1.0f);
            shakePos.x += sin(1.2f * m_Obj->hitReaction.frame) * hrModTmp * 300.0f * shakemod;
        }


        if (m_StartedFlag && IsInStage())
        {
            DRAW_SPRITE_DAT dsdat = {};
            dsdat.x = m_Obj->pos.x;
            dsdat.y = m_Obj->pos.y;
            dsdat.z = -8.0f;
            //dsdat.z = -0.005f;
            dsdat.w = m_Obj->size.x * m_Obj->scale.x * 0.85f;
            dsdat.h = m_Obj->size.y * m_Obj->scale.y * 0.85f;
            {
                float mo = 1.0f - std::max(std::min(abs(m_Obj->pos.z) / (ONE_BLOCK * 80.0f), 1.0f), 0.0f);
                dsdat.w *= mo;
                dsdat.h *= mo;
            }

            dsdat.rot = 0.0f;
            dsdat.color = { 1.0f, 1.0f, 1.0f, 0.4f };

            dsdat.texNo = m_Tex_Shadow.textureId;
            dsdat.loadTexType = LOADTEXTURETYPE_MAIN;

            dsdat.texUV = { 0.0f, 0.0f };
            dsdat.texWH = { 1.0f, 1.0f };
            dsdat.startPos = { 0.0f, 0.0f };
            dsdat.startRot = 0.0f;
            dsdat.startScale = { 1.0f, 1.0f };

            DrawSpriteInCamera(dsdat, false, false,
                true
            );
        }
        //for (int j = 0; j < 2; j++)
        //{
        //    for (int i = 0; i < MODEL_NUM; i++)
        //    {
        //        drawBody(i, flashnum, shakePos, j == 0 ? DrawType::Shadow: DrawType::Default);
        //    }
        //}
        {
            for (int i = 0; i < MODEL_NUM; i++)
            {
                drawBody(i, flashnum, shakePos, DrawType::Default);
            }
        }

        //if(m_BodyTexMaskFrame >= 0)
        {
            for (int i = 0; i < MODEL_NUM; i++)
            {
                drawBody(i, flashnum, shakePos, DrawType::Mask);
            }
        }
    }


    if (m_StartMotionShaderFlag)
    {

        {
            DRAW_POLYGON_DAT dsdat = {};
            dsdat.pos.x = 0.0f;
            dsdat.pos.y = 0.0f;
            dsdat.pos.z = 0.0f;

            dsdat.size.x = SCREEN_WIDTH * 2.0f;
            dsdat.size.y = SCREEN_HEIGHT * 1.5f;
            dsdat.size.z = 1.0f;

            dsdat.rot = 10.0f;

            //dsdat.color = { 1.0f, 1.0f, 1.0f, 1.0 };
            dsdat.color = { 1.0f, 1.0f, 1.0f, 0.5f };
            //dsdat.texNo = m_Tex_StartingEff_Bk2.textureId;
            dsdat.texNo = m_Tex_StartingEff_Bk1.textureId;

            dsdat.loadTexType = LOADTEXTURETYPE_MAIN;

            dsdat.texUV.x = -0.08f * m_Obj->frame;

            dsdat.texWH = { 1.0f, 1.0f };

            dsdat.startRot = 0.0f;
            dsdat.startScale = MkF2(1.0f, 1.0f);
            dsdat.startScale3D = MkF3(1.0f, 1.0f, 1.0f);

            dsdat.modelNo = 0;

            dsdat.shaderOpt.blendState = DPD_SHADER_OPT_BLEND_STATE_ADDCOLOR;

            DRAW_POLYGON_IN_CAMERA_EX_OPTION opt = {};
            opt.reverse = false;
            opt.fixed = true;
            opt.billboard = false;
            opt.priority = DRAW_POLYGON_PRIORITY_NML_ALWAYS;
            opt.depthBuffIgnore = true;
            //opt.shaderType = DPD_SHADER_TYPE_EFFECT;

            DrawPolygonInCameraEx(dsdat, opt);
        }
        {
            DRAW_POLYGON_DAT dsdat = {};
            dsdat.pos.x = 0.0f;
            //dsdat.pos.y = 0.0f;
            dsdat.pos.y = 160.0f;
            dsdat.pos.z = 0.0f;

            dsdat.size.x = SCREEN_WIDTH * 2.0f;
            //dsdat.size.y = SCREEN_HEIGHT * 1.2f;
            dsdat.size.y = SCREEN_HEIGHT * 0.65f;
            dsdat.size.z = 1.0f;

            dsdat.rot = 10.0f;

            //dsdat.color = { 1.0f, 1.0f, 1.0f, 0.8 };
            //dsdat.color = { 1.0f, 1.0f, 1.0f, 1.0f };
            dsdat.color = { 0.7f, 0.0f, 1.0f, 1.0f };
            //dsdat.texNo = m_Tex_StartingEff_Bk1.textureId;
            dsdat.texNo = m_Tex_StartingEff_Bk2.textureId;

            dsdat.loadTexType = LOADTEXTURETYPE_MAIN;

            dsdat.texUV.x = -0.05f * m_Obj->frame;

            dsdat.texWH = { 1.0f, 1.0f };
            //dsdat.texWH = { 1.0f, 0.3f };

            dsdat.startRot = 0.0f;
            dsdat.startScale = MkF2(1.0f, 1.0f);
            dsdat.startScale3D = MkF3(1.0f, 1.0f, 1.0f);

            dsdat.modelNo = 0;

            dsdat.shaderOpt.blendState = DPD_SHADER_OPT_BLEND_STATE_ADDCOLOR;

            DRAW_POLYGON_IN_CAMERA_EX_OPTION opt = {};
            opt.reverse = false;
            opt.fixed = true;
            opt.billboard = false;
            opt.priority = DRAW_POLYGON_PRIORITY_FORCE_DEEPEST;
            opt.depthBuffIgnore = true;
            opt.shaderType = DPD_SHADER_TYPE_EFFECT;

            DrawPolygonInCameraEx(dsdat, opt);
        }
        {
            DRAW_POLYGON_DAT dsdat = {};
            dsdat.pos.x = -280.0f;
            dsdat.pos.y = 60.0f;
            dsdat.pos.z = 0.0f;
            dsdat.pos.x += cos(-1.2f * m_Obj->frame) * 5.0f;
            dsdat.pos.y += sin(-0.7f * m_Obj->frame) * 5.0f;

            dsdat.size.x = 1300.0f;
            dsdat.size.y = 1300.0f;
            dsdat.size.z = 1.0f;

            dsdat.rot = 12.0f;

            dsdat.color = { 1.0f, 1.0f, 1.0f, 1.0 };
            dsdat.texNo = m_IsHard ?
                m_Tex_StartingEff_NameH.textureId :
                m_Tex_StartingEff_Name.textureId
                ;

            dsdat.loadTexType = LOADTEXTURETYPE_MAIN;

            dsdat.texWH = { 1.0f, 1.0f };

            dsdat.startRot = 0.0f;
            dsdat.startScale = MkF2(1.0f, 1.0f);
            dsdat.startScale3D = MkF3(1.0f, 1.0f, 1.0f);

            dsdat.modelNo = 0;

            DRAW_POLYGON_IN_CAMERA_EX_OPTION opt = {};
            opt.reverse = false;
            opt.fixed = true;
            opt.billboard = false;
            //opt.priority = DRAW_POLYGON_PRIORITY_NML_ALWAYS;
            opt.priority = DRAW_POLYGON_PRIORITY_FORCE_DEEPEST;
            opt.depthBuffIgnore = true;
            //opt.shaderType = DPD_SHADER_TYPE_EFFECT;

            {
                DRAW_POLYGON_DAT dsdat2 = dsdat;
                dsdat2.pos.x += sin(1.2f * m_Obj->frame) * 80.0f;
                dsdat2.pos.y += cos(0.7f * m_Obj->frame) * 80.0f;
                //dsdat2.color = {
                //    1.0f,
                //    0.0f,
                //    0.6f,
                //    0.4f + tan(0.4f * m_Obj->frame) * 0.3f
                //};
                dsdat2.color = {
                    0.7f,
                    0.5f,
                    1.0f,
                    0.4f + tan(0.4f * m_Obj->frame) * 0.3f
                };
                dsdat2.shaderOpt.blendState = DPD_SHADER_OPT_BLEND_STATE_ADDCOLOR;
                DrawPolygonInCameraEx(dsdat2, opt);
            }

            opt.shaderType = DPD_SHADER_TYPE_EX;
            opt.shaderTypeEx.vs = m_ShaderIndex_CutInName_Vertex;
            opt.shaderTypeEx.ps = m_ShaderIndex_CutInName_Pixel;

            DrawPolygonInCameraEx(dsdat, opt);
        }

    }


    if (!m_StartedFlag)
    {
        draw_start();
        //return;
    }
    if (m_StartEyeEffFrameMax > 0)
    {
        float m = (float)m_StartEyeEffFrame / m_StartEyeEffFrameMax;

        {
            DRAW_POLYGON_DAT dsdat = {};
            dsdat.pos.x = 0.0f;
            dsdat.pos.y = 0.0f;
            dsdat.pos.z = 0.0f;

            dsdat.size.x = SCREEN_WIDTH;
            dsdat.size.y = SCREEN_HEIGHT;
            dsdat.size.z = 1.0f;

            dsdat.rot = 0.0f;

            dsdat.color = {
                0.0f,
                0.0f,
                0.0f,
                //std::min(8.0f * EasingInQuart(m), 0.9f)
                std::min(2.0f * m, 0.9f)
            };
            dsdat.texNo = 0;

            dsdat.loadTexType = LOADTEXTURETYPE_MAIN;

            dsdat.texWH = { 1.0f, 1.0f };

            dsdat.startRot = 0.0f;
            dsdat.startScale = MkF2(1.0f, 1.0f);
            dsdat.startScale3D = MkF3(1.0f, 1.0f, 1.0f);

            dsdat.modelNo = 0;

            DRAW_POLYGON_IN_CAMERA_EX_OPTION opt = {};
            opt.reverse = false;
            opt.fixed = true;
            opt.billboard = false;
            //opt.priority = DRAW_POLYGON_PRIORITY_FORCE_DEEPEST;
            opt.priority = DRAW_POLYGON_PRIORITY_NML_ALWAYS;
            opt.depthBuffIgnore = true;
            opt.shaderType = DPD_SHADER_TYPE_EFFECT;

            DrawPolygonInCameraEx(dsdat, opt);
        }
        {
            float m2 = std::min((1.0f - m) / 0.65f, 1.0f);

            DRAW_POLYGON_DAT dsdat = {};
            dsdat.pos.x = 70.0f;
            dsdat.pos.y = 2.0f;
            dsdat.pos.z = 0.0f;

            dsdat.size.x = 2200.0f;
            dsdat.size.y = 2200.0f;
            dsdat.size.z = 1.0f;

            dsdat.rot = 0.0f;

            dsdat.color = { 1.0f, 1.0f, 1.0f, 
                0.85f * (1.0f - m2)
            };
            dsdat.texNo = m_IsHard ?
                m_Tex_StartingEff_Eye2.textureId :
                m_Tex_StartingEff_Eye.textureId;

            dsdat.loadTexType = LOADTEXTURETYPE_MAIN;

            dsdat.texUV = AnimationUV(
                25 * m2,
                5,
                5
            );
            dsdat.texWH = {
                1.0f / 5,
                1.0f / 5
            };

            dsdat.startRot = 0.0f;
            dsdat.startScale = MkF2(1.0f, 1.0f);
            dsdat.startScale3D = MkF3(1.0f, 1.0f, 1.0f);

            dsdat.modelNo = 0;

            //dsdat.shaderOpt.bloom = true;
            dsdat.shaderOpt.blendState = DPD_SHADER_OPT_BLEND_STATE_ADDCOLOR;

            DRAW_POLYGON_IN_CAMERA_EX_OPTION opt = {};
            opt.reverse = false;
            opt.fixed = true;
            opt.enableShade = false;
            opt.billboard = false;
            //opt.priority = DRAW_POLYGON_PRIORITY_NML_u2;
            //opt.priority = DRAW_POLYGON_PRIORITY_FORCE_DEEPEST;
            opt.priority = DRAW_POLYGON_PRIORITY_NML_ALWAYS;
            opt.depthBuffIgnore = true;
            opt.shaderType = DPD_SHADER_TYPE_EFFECT;

            DrawPolygonInCameraEx(dsdat, opt);
        }
        //{
        //    DRAW_POLYGON_DAT dsdat = {};
        //    dsdat.pos.x = 80.0f;
        //    dsdat.pos.y = 10.0f;
        //    dsdat.pos.z = 0.0f;

        //    dsdat.size.x = 3000.0f;
        //    dsdat.size.y = 3000.0f;
        //    dsdat.size.z = 1.0f;

        //    dsdat.rot = 0.0f;

        //    dsdat.color = { 1.0f, 1.0f, 1.0f, 0.85f };
        //    dsdat.texNo = m_IsHard ?
        //        m_Tex_StartingEff_Eye2.textureId :
        //        m_Tex_StartingEff_Eye.textureId;

        //    dsdat.loadTexType = LOADTEXTURETYPE_MAIN;

        //    dsdat.texUV = AnimationUV(
        //        25 * (1.0f - m),
        //        5,
        //        5
        //    );
        //    dsdat.texWH = {
        //        1.0f / 5,
        //        1.0f / 5
        //    };

        //    dsdat.startPos3D = m_Obj->pos;
        //    dsdat.startPos3D.z += -1260.0f + m_ModelTransPos[0].y;

        //    dsdat.startRot = 0.0f;
        //    dsdat.startScale = MkF2(1.0f, 1.0f);
        //    dsdat.startScale3D = MkF3(1.0f, 1.0f, 1.0f);

        //    dsdat.modelNo = 0;

        //    //dsdat.shaderOpt.bloom = true;
        //    dsdat.shaderOpt.blendState = DPD_SHADER_OPT_BLEND_STATE_ADDCOLOR;

        //    DRAW_POLYGON_IN_CAMERA_EX_OPTION opt = {};
        //    opt.reverse = false;
        //    opt.fixed = false;
        //    opt.enableShade = false;
        //    opt.billboard = true;
        //    //opt.priority = DRAW_POLYGON_PRIORITY_NML_u2;
        //    opt.priority = DRAW_POLYGON_PRIORITY_FORCE_DEEPEST;
        //    opt.depthBuffIgnore = true;
        //    opt.shaderType = DPD_SHADER_TYPE_EFFECT;

        //    DrawPolygonInCameraEx(dsdat, opt);
        //}
    }

    if (m_FlashScreenMaskFrame > 0)
    {
        float m = (float)m_FlashScreenMaskFrame / m_FlashScreenMaskFrameMax;

        {
            DRAW_POLYGON_DAT dsdat = {};
            dsdat.pos.x = 0.0f;
            dsdat.pos.y = 0.0f;
            dsdat.pos.z = 0.0f;

            dsdat.size.x = SCREEN_WIDTH;
            dsdat.size.y = SCREEN_HEIGHT;
            dsdat.size.z = 1.0f;

            dsdat.rot = 0.0f;

            dsdat.color = {
                2.0f,
                2.0f,
                2.0f,
                //std::min(1.5f * m, 1.0f)
                std::min(1.5f * m, 1.0f) * 2.0f
            };
            dsdat.texNo = 0;

            dsdat.loadTexType = LOADTEXTURETYPE_GRAY;

            dsdat.texWH = { 1.0f, 1.0f };

            dsdat.startRot = 0.0f;
            dsdat.startScale = MkF2(1.0f, 1.0f);
            dsdat.startScale3D = MkF3(1.0f, 1.0f, 1.0f);

            dsdat.modelNo = 0;

            dsdat.shaderOpt.blendState = DPD_SHADER_OPT_BLEND_STATE_DEFAULT;

            DRAW_POLYGON_IN_CAMERA_EX_OPTION opt = {};
            opt.reverse = false;
            opt.fixed = true;
            opt.billboard = false;
            //opt.priority = DRAW_POLYGON_PRIORITY_FORCE_ALWAYS;
            //opt.priority = DRAW_POLYGON_PRIORITY_FORCE_NORMAL;
            opt.priority = DRAW_POLYGON_PRIORITY_FORCE_DEEPEST;
            opt.depthBuffIgnore = true;

            DrawPolygonInCameraEx(dsdat, opt);
        }
    }

    if (m_Anim_Frame >= 0
        && m_Anim_State == AnimState::Damage2
        && m_SuperBeamSuccessWeaknessFlag
    )
    {
        {
            DRAW_POLYGON_DAT dsdat = {};
            dsdat.pos.x = 0.0f;
            dsdat.pos.y = 0.0f;
            dsdat.pos.z = 0.0f;

            dsdat.size.x = 120.0f;
            dsdat.size.y = 120.0f;
            dsdat.size.z = 120.0f;

            dsdat.rot = 0.0f;
            dsdat.rot3D = { 
                90.0f + sinf((float)m_Obj->frameActive / 12) * 6.0f,
                00.0f + cosf((float)m_Obj->frameActive / 12) * 6.0f,
                0.0f + -4.0f *  m_Obj->frameActive
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
            dsdat.startPos3D.x += m_ModelTransPos[0].x;
            dsdat.startPos3D.y += m_ModelTransPos[0].y + ONE_BLOCK * 3.0f;
            dsdat.startPos3D.z += m_ModelTransPos[0].z + ONE_BLOCK * -18.0f;

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

    //行動
    //switch (m_CurrentState)
    //{
    //case ActionState::Blade:
    //    draw_blade();
    //    break;
    //case ActionState::Beam:
    //    draw_beam();
    //    break;
    //case ActionState::Misile:
    //    draw_misile();
    //    break;
    //default:break;
    //}

    if (m_KakoEfScreenMaskFrame > 0)
    {
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
            dsdat.shaderOpt.blendState = DPD_SHADER_OPT_BLEND_STATE_ADDCOLOR;

            DRAW_POLYGON_IN_CAMERA_EX_OPTION opt = {};
            opt.reverse = false;
            opt.fixed = true;
            opt.depthBuffIgnore = true;
            opt.priority = DRAW_POLYGON_PRIORITY_FORCE_NORMAL;

            //float efmod = std::min((float)m_Dead_FrameMax / 80, 1.0f);
            float efmod = 1.0f- ((float)m_KakoEfScreenMaskFrame / m_KakoEfScreenMaskFrameMax);
            if (efmod > 0)
            {
                dsdat.color = { 1.0f, 1.0f, 1.0f, 1.0f };
                dsdat.texNo = m_Tex_ScKakoEffect.textureId;
                opt.priority = DRAW_POLYGON_PRIORITY_FORCE_NORMAL;
                dsdat.texUV = AnimationUV(
                    16 * efmod,
                    4, 4);
                dsdat.texWH = { 1.0f / 4, 1.0f / 4 };
                dsdat.shaderOpt.blendState = DPD_SHADER_OPT_BLEND_STATE_ADDCOLOR;
                DrawPolygonInCameraEx(dsdat, opt);
            }
        }
    }

    if (m_Obj->status.deadFlag)
    {
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
            dsdat.color = { 20.0f, 20.0f, 20.0f, 1.0f };
            //dsdat.color.w *= std::min((float)m_Dead_Frame / m_Dead_FrameMax, 1.0f);
            dsdat.color.w *= std::max(std::min(
                (float)(m_Dead_Frame - (m_Dead_FrameMax * 0.25))
                / (m_Dead_FrameMax * 0.75f),
                1.0f), 0.0f);

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
            dsdat.shaderOpt.blendState = DPD_SHADER_OPT_BLEND_STATE_ADDCOLOR;

            DRAW_POLYGON_IN_CAMERA_EX_OPTION opt = {};
            opt.reverse = false;
            opt.fixed = true;
            opt.depthBuffIgnore = true;
            opt.priority = DRAW_POLYGON_PRIORITY_FORCE_NORMAL;

            DrawPolygonInCameraEx(dsdat, opt);

        }

        return;
    }

    draw_blade();
    draw_beam();
    draw_misile();
    draw_misile_object();
    draw_machinegun();
    draw_machinegun_hand();
    draw_energyballbit();
    draw_granade();
    draw_enemyCall();

    draw_super_beam();
    draw_phase2Start();
}

// ===================================================
// Dead
// オブジェクトのHPが0になったとき
// ===================================================
void GOE_Character_Enemy_Boss_Inperiusu::Dead()
{
	SuperGOE::Dead();

    deadstop_event();

    m_DeadFlag = true;
}

void GOE_Character_Enemy_Boss_Inperiusu::CalcDamage(
    GAME_OBJECT_DAMAGE_RESULT* output,

    const GAME_OBJECT* obj, const GAME_OBJECT* objAttacker,
    int damage,
    GAME_OBJECT_COLLISION_ATTACK_CHILD colAtk,
    GAME_OBJECT_COLLISION_RECIEVE_CHILD colRec
)
{
    if (std::string(colAtk.tag) == "SUPER_BEAM_EXPLODE")
    {
        return;
    }

    output->damage = CalcRecvDamageMultiPlay(output->damage);

    if (m_SuperBeamSuccessWeaknessFlag)
    {
        output->damage *= SUPER_BEAM_AFTER_WEAKNESS_MOD;
        output->mod *= SUPER_BEAM_AFTER_WEAKNESS_MOD;
    }

    if (m_CurrentState == ActionState::Beam)
    {
        if (m_BeamFrame < FRAME_BEAM_SHOOT_END)
        {
            CalcRecvDamageGuardMod(0.5f, colAtk, colRec, output);
        }
    }
    if (m_CurrentState == ActionState::Misile)
    {
        if (m_MisileFrame >= 1 
            && m_MisileFrame < FRAME_MISILE_AFTER
        )
        {
            CalcRecvDamageGuardMod(0.7f, colAtk, colRec, output);
        }
    }
    if (m_CurrentState == ActionState::Blade)
    {
        if (m_BladeSuperFlag)
        {
            CalcRecvDamageGuardMod(0.3f, colAtk, colRec, output);
        }
    }
    if (m_CurrentState == ActionState::Granade)
    {
        if (m_GranadeFrame >= 2)
        {
            CalcRecvDamageGuardMod(0.7f, colAtk, colRec, output);
        }
    }

    if (m_CurrentState == ActionState::HealEvent)
    {
        if (m_HealEventFrame >= 1)
        {
            CalcRecvDamageGuardMod(0.1f, colAtk, colRec, output);
        }
    }
    if (m_CurrentState == ActionState::WaitEvent)
    {
        CalcRecvDamageGuardMod(0.25f, colAtk, colRec, output);
    }

    if (m_Dead_Frame > 0)
    {
        output->damage = 0;
        output->mod = 0.0f;
    }
}

// ===================================================
// DamageAttacker
// オブジェクトの攻撃判定がヒットしたとき
// ===================================================
void GOE_Character_Enemy_Boss_Inperiusu::DamageAttacker(GAME_OBJECT_DAMAGE_RESULT dr)
{
	SuperGOE::DamageAttacker(dr);

    if (dr.damage > 0 && std::string("BLADE") == dr.colAtk.tag)
    {
        //SEを再生
        SoundEffectPlayRequest(GetSoundData("SOUND/SE/inperiusu/b_attackHit"), 1.3f, 0);

        //GAME_OBJECT* recvObj = GetGameObject(dr.recObjIndex);

        //if (recvObj != nullptr)
        //{
        //    Float2 vec = MkF2(
        //        recvObj->pos.x - m_Obj->pos.x, 
        //        recvObj->pos.y - m_Obj->pos.y
        //    );
        //    vec = NormalizeVector(vec);

        //    //GameObjectMoveRequest(recvObj,
        //    //    36,
        //    //    MkF3(
        //    //        recvObj->pos.x + vec.x * ONE_BLOCK * 18.0f,
        //    //        recvObj->pos.y + vec.y * ONE_BLOCK * 18.0f,
        //    //        recvObj->pos.z
        //    //    ),
        //    //    EasingOutQuad
        //    //);
        //    GameObjectMoveRequest(recvObj,
        //        36,
        //        MkF3(
        //            recvObj->pos.x + vec.x * ONE_BLOCK * dr.colAtk.knockBackPower,
        //            recvObj->pos.y + vec.y * ONE_BLOCK * dr.colAtk.knockBackPower,
        //            recvObj->pos.z
        //        ),
        //        EasingOutQuad
        //    );
        //}
    }
}

// ===================================================
// DamageReciever
// オブジェクトが
// 別のオブジェクトの攻撃判定にヒットしたとき
// ===================================================
void GOE_Character_Enemy_Boss_Inperiusu::DamageReciever(GAME_OBJECT_DAMAGE_RESULT dr)
{
	SuperGOE::DamageReciever(dr);

    if (dr.damage > 0)
    {
        if (m_CurrentState == ActionState::HealEvent
            || m_CurrentState == ActionState::Phase2Start)
        {
            m_HealEventAttackFlag = true;
        }
        //CameraShakeRequestAll(CAMERA_SHAKE_LARGE_FRAME, CAMERA_SHAKE_MIDIUM_FRAME);
    }
}

// ===================================================
// BlockHit
// オブジェクトがブロックに触れたとき
// ===================================================
void GOE_Character_Enemy_Boss_Inperiusu::BlockHit(GAME_OBJECT_POSMOD_RESULT pmr)
{
	SuperGOE::BlockHit(pmr);
}



void GOE_Character_Enemy_Boss_Inperiusu::StartBattle()
{
    m_StartFlag = true;
}

void GOE_Character_Enemy_Boss_Inperiusu::StartTurn(float afterAxis, float speed)
{
    Float2 turnVecAfter = {};
    float rad = CalculateDegToRad(afterAxis);
    turnVecAfter.x = -sin(rad);
    turnVecAfter.y = cos(rad);

    StartTurn(turnVecAfter, speed);
}
void GOE_Character_Enemy_Boss_Inperiusu::StartTurn(Float2 afterVec, float speed)
{
    m_TurnVecAfter = Float2ToFloat3(afterVec);
    m_TurnSpeed = speed;
}

void GOE_Character_Enemy_Boss_Inperiusu::StartMove(
    Float3 afterPos, float afterAxis, double maxF,
    bool distanceOneF
    )
{
    {
        m_SimpleMoveFrame = 0;

        if (distanceOneF)
        {
            float dis = CalculateDistance(afterPos, m_Obj->pos);

            m_SimpleMoveFrameMax = (int)(maxF * (dis / ONE_BLOCK));
        }
        else
        {
            m_SimpleMoveFrameMax = maxF;
        }


        m_SimpleMovePosBefore = m_Obj->pos;
        m_SimpleMoveAxisBefore = m_CurrentAxisPrivate;

        m_SimpleMovePosAfter = afterPos;
        m_SimpleMoveAxisAfter = afterAxis;
    }
}
void GOE_Character_Enemy_Boss_Inperiusu::StartWarp(Float3 afterPos, float afterAxis, int maxF,
    bool safeAreaIgnore)
{
    SoundEffectPlayRequest(GetSoundData("SOUND/SE/enemy/move/warpHigh"), 1.5f, 0);

    {
        m_WarpFrame = 0;
        m_WarpFrameMax = maxF;

        m_WarpPosBefore = m_Obj->pos;
        m_WarpAxisBefore = m_CurrentAxisPrivate;

        m_WarpPosAfter = afterPos;
        m_WarpAxisAfter = afterAxis;

        if (!safeAreaIgnore)
        {
            Stage_Base* stg = GetCurrentStageSceneGame();
            if (stg != nullptr)
            {
                Int2 stgsize = stg->GetStageSize();

                Float2 size = {};
                size.x = BLOCK_WIDTH * stgsize.x * 0.5f;
                size.y = BLOCK_HIGHT * stgsize.y * 0.5f;

                if (m_WarpPosAfter.x < size.x / -2)
                {
                    m_WarpPosAfter.x = size.x / -2;
                }
                if (m_WarpPosAfter.x > size.x / 2)
                {
                    m_WarpPosAfter.x = size.x / 2;
                }

                if (m_WarpPosAfter.y < size.y / -2)
                {
                    m_WarpPosAfter.y = size.y / -2;
                }
                if (m_WarpPosAfter.y > size.y / 2)
                {
                    m_WarpPosAfter.y = size.y / 2;
                }
            }
        }

        {
            Float3 postmp = m_WarpPosBefore;
            postmp.z += ONE_BLOCK * -8.0f * m_Obj->scale.z;

            GAME_OBJECT* go = GO_CreateEffect(
                GO_EFFECT_TYPE_WARP_IN,
                postmp);

            if (go != nullptr)
            {
                go->graphic.color = MkF4(
                    1.0f,
                    1.0f,
                    1.0f,
                    1.0f
                );
                go->scale = MkF3(
                    6.0f * m_Obj->scale.x,
                    6.0f * m_Obj->scale.y,
                    6.0f * m_Obj->scale.z
                );
                GameObjectSetLifeTime(go, 26);
            }
        }

        {
            Float3 postmp = m_WarpPosAfter;
            postmp.z += ONE_BLOCK * -8.0f * m_Obj->scale.z;

            GAME_OBJECT* go = GO_CreateEffect(
                GO_EFFECT_TYPE_WARP_OUT,
                postmp);

            if (go != nullptr)
            {
                go->graphic.color = MkF4(
                    1.0f,
                    1.0f,
                    1.0f,
                    1.0f
                );
                go->scale = MkF3(
                    6.0f * m_Obj->scale.x * 1.25f,
                    6.0f * m_Obj->scale.y * 1.25f,
                    6.0f * m_Obj->scale.z * 1.25f
                );
                GameObjectSetLifeTime(go, 30);
            }
        }
    }
}
void GOE_Character_Enemy_Boss_Inperiusu::StartJet(Float3 afterPos, float afterAxis, int maxF)
{
    //SEを再生
    //SoundEffectPlayRequest(GetSoundData("SOUND/SE/enemy/attack/Beam_Charge"), 0.8f, 10);

    {
        m_JetFrame = 0;
        m_JetFrameMax = maxF;

        m_JetPosBefore = m_Obj->pos;
        m_JetAxisBefore = m_CurrentAxisPrivate;

        m_JetPosAfter = afterPos;
        m_JetAxisAfter = afterAxis;

        Stage_Base* stg = GetCurrentStageSceneGame();
        if (stg != nullptr)
        {
            Int2 stgsize = stg->GetStageSize();

            Float2 size = {};
            size.x = BLOCK_WIDTH * stgsize.x;
            size.y = BLOCK_HIGHT * stgsize.y;

            if (m_JetPosAfter.x < size.x / -2)
            {
                m_JetPosAfter.x = size.x / -2;
            }
            if (m_JetPosAfter.x > size.x / 2)
            {
                m_JetPosAfter.x = size.x / 2;
            }

            if (m_JetPosAfter.y < size.y / -2)
            {
                m_JetPosAfter.y = size.y / -2;
            }
            if (m_JetPosAfter.y > size.y / 2)
            {
                m_JetPosAfter.y = size.y / 2;
            }
        }
    }
}





void GOE_Character_Enemy_Boss_Inperiusu::update_blade()
{

    bool bladeMirrorgeFlag = m_BladeMirrogeFlag 
        && m_BladeWarpNum > 0 
        && m_BladeWaveNum > 0
        ;

    Float3 vec = {};
    GAME_OBJECT* targetObject = GetGameObjectFindByIdentifier(m_TargetObjIdentifier);
    if (targetObject != nullptr)
    {
        float distance = CalculateDistance(targetObject->pos, m_Obj->pos);
        vec = NormalizeVector(MkF3(
            targetObject->pos.x - m_Obj->pos.x,
            targetObject->pos.y - m_Obj->pos.y,
            0.0f
        ));

        //constexpr unsigned int dashOneCycle = 1;
        //constexpr unsigned int dashOneCycle = (60 * 5);
        //unsigned int dashF = m_TargetFrame % dashOneCycle;
        //unsigned int dashF = m_BladeFrame % dashOneCycle;
        int dashF = m_BladeFrame;

        if (dashF == 0)
        {
            if (!m_IsHard)
            {
                Float2 vec2 = NormalizeVector(MkF2(
                    m_Obj->pos.x - targetObject->pos.x,
                    m_Obj->pos.y - targetObject->pos.y
                ));
                if (distance > ONE_BLOCK * 50)
                {
                    StartJet(
                        MkF3(
                            targetObject->pos.x + vec2.x * ONE_BLOCK * 12.0f,
                            targetObject->pos.y + vec2.y * ONE_BLOCK * 12.0f,
                            targetObject->pos.z
                        ),
                        CalculateRadToDeg(atan2(-vec2.y, -vec2.x)) - 90.0f,
                        160//240
                    );
                }
                else if (distance > ONE_BLOCK * 20)
                {
                    StartMove(
                        MkF3(
                            targetObject->pos.x + vec2.x * ONE_BLOCK * 12.0f,
                            targetObject->pos.y + vec2.y * ONE_BLOCK * 12.0f,
                            targetObject->pos.z
                        ),
                        CalculateRadToDeg(atan2(-vec2.y, -vec2.x)) - 90.0f,
                        3.5, true
                    );
                }
            }
        }

        //if (m_TargetFrame > dashOneCycle)
        {
            if (dashF == 1)
            {
                if (!m_BladeNoTurnFlag)
                {
                    StartTurn(Float3ToFloat2(m_TargetVec), 0.05f);
                }
            }
            if (dashF == 2)
            //if (dashF == 1 && distance < ONE_BLOCK * 200.0f)
            //if (dashF == 1 && distance < ONE_BLOCK * 20.0f)
            { //ブレードアニメーション

                //SEを再生
                SoundEffectPlayRequest(GetSoundData("SOUND/SE/inperiusu/b_kamae"), 1.3f, 0);
                if (m_BladeBeforeSpeedMod > 0.4f)
                {
                    //SEを再生
                    SoundEffectPlayRequest(GetSoundData("SOUND/SE/inperiusu/b_charge"), 1.3f, 5);
                }
                
                if (m_BladeSuperFlag/*m_BladeBeforeSpeedMod > 1.5f*/)
                {
                    ////SEを再生
                    //SoundEffectPlayRequest(
                    //    GetSoundData("SOUND/SE/enemy/attack/Beam_Charge"),
                    //    0.3f, 0, 1.2f * (m_BladeBeforeSpeedMod / 1.5f)
                    //);
                    ////SEを再生
                    //SoundEffectPlayRequest(
                    //    GetSoundData("SOUND/SE/enemy/attack/Annihilator_Charge"),
                    //    0.45f, ATTACK_BLADE_BEFORE_FIXED_FRAME - 30,
                    //    1.25f * (m_BladeBeforeSpeedMod / 1.5f)
                    //);
                    //SEを再生
                    SoundEffectPlayRequest(
                        GetSoundData("SOUND/SE/enemy/attack/Beam_Charge"),
                        0.1f, 0, 1.2f * (1.1f / m_BladeBeforeSpeedMod)
                    );
                    //SEを再生
                    SoundEffectPlayRequest(
                        GetSoundData("SOUND/SE/enemy/attack/Annihilator_Charge"),
                        0.45f, ATTACK_BLADE_BEFORE_FIXED_FRAME - 30,
                        1.25f * (2.2f / m_BladeBeforeSpeedMod)
                    );

                    //SetCameraModeAll(CAMERA_MODE_BOSS_CHASE_WIDEMIDDLE);
                    //SetCameraModeAll(CAMERA_MODE_BOSS_CHASE_WIDE);
                    SetCameraModeAll(CAMERA_MODE_BOSS_CHASE_EXWIDE);
                }

                if (bladeMirrorgeFlag)
                {

                    //SetCameraModeAll(CAMERA_MODE_BOSS_CHASE_WIDE);
                    SetCameraModeAll(CAMERA_MODE_BOSS_CHASE_EXWIDE);
                }

                if (m_BladeSuperMirrogeFlag)
                {

                    SetCameraModeAll(CAMERA_MODE_BOSS_CHASE_EXWIDE);
                }

                set_blade_visible(true);
                StartAnim(AnimState::BladeBefore, 
                    ATTACK_BLADE_BEFORE_FRAME * m_BladeBeforeSpeedMod
                );
                StartBodyTexMask(
                    ATTACK_BLADE_BEFORE_FRAME * m_BladeBeforeSpeedMod
                    + ATTACK_BLADE_ATTACK_FRAME);

                //GraphicSpriteSet(m_Tex_Main_Blade[std::max(m_CurrentAxisEx - 1, 0)], 6, 6);
                //Character::PlayMotionCharacterGameObject(
                //    m_CharaData,
                //    Character::GetCharacterMotionFindByName(
                //        Character::GetCharacterMainData(m_CharaData->dataIndex),
                //        "BLADE"),
                //    false,
                //    nullptr,
                //    ATTACK_BLADE_FRAME
                //);
            }
        }

    }

    if (m_Anim_State == AnimState::BladeBefore)
    {
        //if (m_Anim_Frame == ATTACK_BLADE_BEFORE_FIXED_FRAME - 30)
        //{
        //    set_blade_visible(true);
        //}
        if (ATTACK_BLADE_BEFORE_FIXED_FRAME < m_Anim_FrameMax)
        {
            if (m_Anim_Frame >= ATTACK_BLADE_BEFORE_FIXED_FRAME 
                && m_Anim_Frame < m_Anim_FrameMax - 40)
            {
                if (m_BladeSuperFlag/*m_BladeBeforeSpeedMod > 1.5f*/)
                {
                    if (m_Anim_Frame % 10 == 0)
                    {
                        CameraShakeRequestAll(
                            CAMERA_SHAKE_MIDIUM_FRAME,
                            CAMERA_SHAKE_LARGE_POWER * (0.1f + 0.9f * m_Anim_Mod)
                        );
                    }
                }

                if (m_Anim_Frame % 10 == 0)
                {
                    Float3 postmp = m_Obj->pos;
                    postmp.z += ONE_BLOCK * -9.0f;

                    GAME_OBJECT* go = GO_CreateEffect(
                        GO_EFFECT_TYPE_HAMMER_CHARGE_LOOP,
                        postmp);

                    if (go != nullptr)
                    {
                        go->graphic.color = MkF4(
                            1.0f,
                            1.0f,
                            1.0f,
                            0.8f
                        );
                        go->scale = MkF3(
                            16.0f * (1.0f + (1.4f * m_Anim_Mod)),
                            16.0f * (1.0f + (1.4f * m_Anim_Mod)),
                            16.0f * (1.0f + (1.4f * m_Anim_Mod))
                        );
                        GameObjectSetLifeTime(go, 30);
                    }
                }
            }
            if (m_Anim_Frame == m_Anim_FrameMax - 40)
            {
                //f (m_Anim_Frame % 10 == 0)
                {
                    Float3 postmp = m_Obj->pos;
                    postmp.z += ONE_BLOCK * -9.0f;

                    GAME_OBJECT* go = GO_CreateEffect(
                        GO_EFFECT_TYPE_HAMMER_CHARGE_END,
                        postmp);

                    if (go != nullptr)
                    {
                        go->graphic.color = MkF4(
                            1.0f,
                            1.0f,
                            1.0f,
                            0.8f
                        );
                        go->scale = MkF3(
                            24.0f * (1.0f + (1.4f * m_Anim_Mod)),
                            24.0f * (1.0f + (1.4f * m_Anim_Mod)),
                            24.0f * (1.0f + (1.4f * m_Anim_Mod))
                        );
                        GameObjectSetLifeTime(go, 30);
                    }
                }
            }
        }

        int startWarpF = std::max((int)(ATTACK_BLADE_BEFORE_FRAME_STARTPOSE * 1.2
            * m_BladeBeforeSpeedMod - m_BladeWarpNum) - m_BladeWarpHastenFrame, 0);
        int endWarpF = std::max((int)(ATTACK_BLADE_BEFORE_FRAME_STARTPOSE * 1.2
            * m_BladeBeforeSpeedMod) - m_BladeWarpHastenFrame, m_BladeWarpNum);

        if (m_Anim_Frame >= m_Anim_FrameMax - 1)
        {
            if(m_BladeSuperFlag/*m_BladeWaveNum >= 5*/)
            {
                SoundEffectPlayRequest(GetSoundData("SOUND/SE/enemy/attack/Small_Explosion"),
                    3.2f, 0, 0.8f);
                SoundEffectPlayRequest(GetSoundData("SOUND/SE/enemy/attack/Explosion"),
                    0.7f, 0, 0.6f);

                //SEを再生
                SoundEffectPlayRequest(GetSoundData("SOUND/SE/inperiusu/b_attack"),
                    2.4f, 0);

                SoundEffectPlayRequest(
                    GetSoundData("SOUND/SE/enemy/attack/super_beam_miminari"),
                    0.65f, 3, 0.75f);
            }
            else
            {
                //SEを再生
                SoundEffectPlayRequest(GetSoundData("SOUND/SE/inperiusu/b_attack"),
                    2.0f, 0);

            }

            StartAnim(AnimState::Blade, ATTACK_BLADE_ATTACK_FRAME, true);
        }
        else if (
            m_IsHard &&
            m_Anim_Frame < endWarpF
            && m_Anim_Frame >= startWarpF)
        {
            GAME_OBJECT* targetObject = GetGameObjectFindByIdentifier(m_TargetObjIdentifier);
            if (targetObject != nullptr)
            {
                //SEを再生
                //SoundEffectPlayRequest(GetSoundData("SOUND/SE/inperiusu/b_attack"), 1.0f, 0);
                //SEを再生
                //SoundEffectPlayRequest(GetSoundData("SOUND/SE/Test_SE_Attack"), 0.8f, 0);

                float degMainTmp = m_CurrentAxisPrivate - 90.0f;
                {
                    float deg = degMainTmp;
                    float rad = CalculateDegToRad(deg);
                    Float2 vec = NormalizeVector(MkF2(
                        -sin(rad),
                        cos(rad)
                    ));
                    Float2 afterPos = {};
                    if (bladeMirrorgeFlag)
                    {
                        afterPos = MkF2(
                            -ONE_BLOCK * 36.0f * vec.x + targetObject->pos.x,
                            -ONE_BLOCK * 36.0f * vec.y + targetObject->pos.y
                        );
                    }
                    else
                    {
                        afterPos = MkF2(
                            -ONE_BLOCK * 16.0f * vec.x + targetObject->pos.x,
                            -ONE_BLOCK * 16.0f * vec.y + targetObject->pos.y
                        );
                    }

                    StartWarp(Float2ToFloat3(afterPos),
                        CalculateRadToDeg(atan2(
                            targetObject->pos.y - afterPos.y,
                            targetObject->pos.x - afterPos.x
                        ) - 90.0f),
                        25, true);
                }

                if (bladeMirrorgeFlag &&
                    m_Anim_Frame == (endWarpF - 1))
                {
                    //float deg = (float)(((int)(m_CurrentAxisPrivate + 45.0f - 90.0f)) % 180) + 180.0f + 90.0f;
                    float deg = 0.0f;
                    if (degMainTmp < 45.0f || degMainTmp > 315.0f)
                    {
                        deg = degMainTmp + 90.0f;
                    }

                    float rad = CalculateDegToRad(deg);
                    Float2 vec = NormalizeVector(MkF2(
                        -sin(rad),
                        cos(rad)
                    ));
                    Float2 afterPos = MkF2(
                        -ONE_BLOCK * 40.0f * vec.x + targetObject->pos.x,
                        -ONE_BLOCK * 40.0f * vec.y + targetObject->pos.y
                    );

                    MirrorgeStateStartSet(0,
                        CalculateRadToDeg(atan2(
                            targetObject->pos.y - afterPos.y,
                            targetObject->pos.x - afterPos.x
                        ) - 90.0f),
                        Float2ToFloat3(afterPos),
                        {}, MkF3(1.0f, 1.0f, 1.0f)
                    );
                }
            }
        }
    }

    //攻撃エフェクト、判定
    if (m_Anim_State == AnimState::Blade
        && m_Anim_Frame >= ATTACK_BLADE_EFFECT_START_FRAME
        )
        //if ((m_CharaData->playingMotion.motion->name).find("BLADE") == 0
        //    && m_CharaData->playingMotion.frame >= ATTACK_BLADE_EFFECT_START_FRAME
        //    )
    {
        //if (m_Anim_Frame == 0)
        if (m_Anim_Frame == ATTACK_BLADE_EFFECT_START_FRAME)
        //if (m_CharaData->playingMotion.frame == ATTACK_BLADE_EFFECT_START_FRAME)
        {

            Character::MOTION_COL_DAT motionColDetails =
                Character::GetMotionColDataFindByName(
                    Character::MOTION_COL_TYPE_ATK,
                    BLADE_ATK_COL_NAME
                );
            GAME_OBJECT_COLLISION_ATTACK_CHILD attackColTmp = motionColDetails.atkCol;
            attackColTmp.subjectFamily[GAME_OBJECT_FAMILY_PLAYER] = true;
            attackColTmp.subjectFamily[GAME_OBJECT_FAMILY_GIMIK] = true;

            strcpy_s(attackColTmp.tag, ARRAYSIZE(attackColTmp.tag), "BLADE");

            //判定サイズ
            //attackColTmp.size = MkF3(
            //    ONE_BLOCK * 16,
            //    ONE_BLOCK * 16,
            //    ONE_BLOCK * 10
            //);
            attackColTmp.size = MkF3(
                ONE_BLOCK * 20,
                ONE_BLOCK * 20,
                ONE_BLOCK * 10
            );
            attackColTmp.pos.z = -attackColTmp.size.z / 2;

            //attackColTmp.pos.x += m_Obj->size.x / 2 * m_AxisVec.x;
            attackColTmp.pos.x += attackColTmp.size.x / 2 * 1.0f * -sin(CalculateDegToRad(m_CurrentAxisPrivate));
            attackColTmp.pos.y += attackColTmp.size.y / 2 * 1.0f * cos(CalculateDegToRad(m_CurrentAxisPrivate));
            //attackColTmp.pos.y += m_Obj->size.y / 2 * m_AxisVec.y;

            GameObjectMoveRequest(m_Obj, 5,
                MkF3(
                    m_Obj->pos.x + m_BeforeVec.x * ONE_BLOCK * 8.0f,
                    m_Obj->pos.y + m_BeforeVec.y * ONE_BLOCK * 8.0f,
                    m_Obj->pos.z
                ),
                nullptr //EasingInSine
            );

            CameraShakeRequestAll(
                CAMERA_SHAKE_EXLARGE_FRAME,
                CAMERA_SHAKE_EXLARGE_POWER * 2.0f
            );

            //攻撃判定を生成
            GameObjectAttackCollisionAdd(m_Obj, attackColTmp,
                1,
                //ATTACK_BLADE_ATTACK_START_FRAME,
                5
            );

            //int LoopNumTmp = 1;
            ////if (bladeMirrorgeFlag)
            ////{
            ////    LoopNumTmp = MIRRORGE_STATE_MAX + 1;
            ////}
            //int mirroergeEnableNum = 0;
            //for (int i = 0; i < MIRRORGE_STATE_MAX; i++)
            //{
            //    if (m_MirrorgeState[i].enable)
            //    {
            //        mirroergeEnableNum++;
            //    }
            //}
            //if (mirroergeEnableNum > 0)
            //{
            //    LoopNumTmp = MIRRORGE_STATE_MAX + 1;
            //}
            int LoopNumTmp = MIRRORGE_STATE_MAX + 1;

            for(int LoopIndexTmp = 0; LoopIndexTmp < LoopNumTmp; LoopIndexTmp++)
            {
                if (LoopIndexTmp > 0 && !m_MirrorgeState[LoopIndexTmp - 1].enable)
                {
                    continue;
                }

                float axisTmp = m_CurrentAxisPrivate;
                Float3 posBaseTmp = m_Obj->pos;
                if (LoopIndexTmp > 0)
                {
                    axisTmp = m_MirrorgeState[LoopIndexTmp - 1].axis;
                    posBaseTmp = m_MirrorgeState[LoopIndexTmp - 1].pos;
                }

                if (m_IsHard)
                { //ウェーブ

                    float angle = m_BladeWaveAngle;
                    int bulletnum = m_BladeWaveNum;
                    if (LoopIndexTmp > 0)
                    {
                        bulletnum = std::max(bulletnum - 1, 1);
                    }
                    //if (m_HardPhase2Flag)
                    //{
                    //    angle = 140.0f;
                    //    bulletnum = 3;
                    //}

                    for (int i = 0; i < bulletnum; i++)
                    {
                        //float radtmp = CalculateDegToRad(
                        //    m_CurrentAxisPrivate
                        //    + 180.0f + (angle - 180.0f) / 4 - (angle / bulletnum * i)
                        //);

                        //float radtmp = CalculateDegToRad(m_CurrentAxisPrivate + 90.0f);
                        float radtmp = CalculateDegToRad(
                            axisTmp + 90.0f
                            - (angle / 2) + (angle / bulletnum) * (0.5f + (float)i)
                        );

                        Float3 vecWav = MkF3(
                            cos(radtmp),
                            sin(radtmp),
                            0.0f
                        );

                        GOE_Character_EnemyProjectile* goepro =
                            GOE_Character_EnemyProjectile::CreateProjectile(
                                GOE_Character_EnemyProjectile::Type::BladeWave,
                                MkF3(
                                    posBaseTmp.x + attackColTmp.pos.x,
                                    posBaseTmp.y + attackColTmp.pos.y,
                                    posBaseTmp.z + attackColTmp.pos.z
                                ),
                                vecWav
                            );
                        if (goepro != nullptr)
                        {
                            GAME_OBJECT* beamObj = goepro->GetGameObj();
                            if (beamObj != nullptr)
                            {
                                beamObj->scale = m_Obj->scale;
                                {
                                    GAME_OBJECT_COLLISION_ATTACK_CHILD attackColTmp2 = attackColTmp;
                                    attackColTmp2.pos = {};
                                    if (m_BladeSuperFlag/*m_BladeWaveNum >= 5*/)
                                    {
                                        attackColTmp2.invIgnore = true;
                                        goepro->SetNumFlag(1);
                                    }

                                    beamObj->collision.atk.enable = true;
                                    beamObj->collision.atk.ownerId = GetIdentifierGameObject(m_Obj);
                                    beamObj->collision.atk.collision[0] = attackColTmp2;
                                    beamObj->collision.atk.collision[0].enable = true;
                                }
                                GameObjectSetLifeTime(beamObj, FRAME_ONE_SECOND * 5.0f);
                                GameObjectMoveRequest(beamObj, FRAME_ONE_SECOND * 5.0f,
                                    MkF3(
                                        posBaseTmp.x + vecWav.x * ONE_BLOCK * 400.0f,
                                        posBaseTmp.y + vecWav.y * ONE_BLOCK * 400.0f,
                                        posBaseTmp.z + vecWav.z * ONE_BLOCK * 400.0f
                                    ),
                                    nullptr //EasingInSine
                                );
                            }
                        }
                    }
                }
            }
        }

        //エフェクト
        //int indextmp = GetGameObjectIndex(m_Obj);
        //if ((m_Obj->frameActive + indextmp * 13) % 1 == 0)
        //{
        //    Float3 postmp = m_Obj->pos;
        //    postmp.x += sin((float)m_Obj->frame * 13.0f + (0.7f * indextmp)) * ONE_BLOCK * 1.8f * m_Obj->scale.x;
        //    postmp.y += cos((float)m_Obj->frame * +3.0f + (1.3f * indextmp)) * ONE_BLOCK * 4.0f * m_Obj->scale.y + 0.0f;
        //    postmp.z += cos((float)m_Obj->frame * 13.0f + (1.3f * indextmp)) * ONE_BLOCK * 1.8f * m_Obj->scale.y + 0.0f;

        //    postmp.x +=
        //        -(m_Size_AttackEffect_s.x * m_Obj->scale.x * 1.0f / 2)
        //        + (m_Size_AttackEffect_s.x * m_Obj->scale.x * 1.0f)
        //        * (float)(m_CharaData->playingMotion.frame - ATTACK_BLADE_EFFECT_START_FRAME) / (ATTACK_BLADE_FRAME - ATTACK_BLADE_EFFECT_START_FRAME);

        //    postmp.y += m_Obj->size.y * 0.05f * m_Obj->scale.y;
        //    postmp.z += m_Obj->size.y * -0.25f * m_Obj->scale.y;

        //    //postmp.z += -150.0f * m_Obj->scale.y;
        //    GAME_OBJECT* go = GO_CreateEffect(GO_EFFECT_TYPE_SPARK, postmp);
        //    if (go != nullptr)
        //    {
        //        go->graphic.color = MkF4(
        //            0.8f,
        //            0.0f,
        //            1.0f,
        //            1.0f
        //        );
        //        go->rot = -50.0f * m_Obj->frame * indextmp;
        //        go->scale = MkF3(
        //            3.2f,
        //            3.2f
        //        );
        //        GameObjectSetLifeTime(go, 12);
        //    }
        //}
    }

    if (m_Anim_State == AnimState::Blade
        && m_Anim_Frame == ATTACK_BLADE_EFFECT_START_FRAME + ATTACK_BLADE_ATTACKING_FRAME_STARTPOSE
    )
    {
        set_blade_visible(false);

        //if (bladeMirrorgeFlag)
        //{
        //    MirrorgeStateEndSet(0);
        //}

        for (int i = 0; i < MIRRORGE_STATE_MAX; i++)
        {
            if (m_MirrorgeState[i].enable)
            {
                MirrorgeStateEndSet(i);
            }
        }
    }

    m_BladeFrame++;

    if (m_BladeFrame > ATTACK_BLADE_BEFORE_FRAME * m_BladeBeforeSpeedMod 
        + ATTACK_BLADE_ATTACK_FRAME + FRAME_ONE_SECOND * 0.25f
        )
    {
        m_ActionCooldown = m_WazaCooldown;
        m_BladeFrame = -1;
        m_CurrentState = ActionState::Idle;

        SetCameraModeAll(CAMERA_MODE_BOSS_CHASE);

        //if (bladeMirrorgeFlag)
        //{
        //    MirrorgeStateEndSet(0);
        //}
        for (int i = 0; i < MIRRORGE_STATE_MAX; i++)
        {
            if (m_MirrorgeState[i].enable)
            {
                MirrorgeStateEndSet(i);
            }
        }

        {

            //if (!IsInStage())
            if(m_BladeAfterWarpFlag)
            {
                StartWarp({ 0.0f, ONE_BLOCK * -10.0f, 0.0f }, 0.0f, 25, true);
            }
            else
            {
                safe_area_move();
            }
        }
    }
}
void GOE_Character_Enemy_Boss_Inperiusu::draw_blade()
{
    if (m_StartMotionShaderFlag)
    {
        return;
    }

    //通常攻撃エフェクト
    if (m_Anim_State == AnimState::Blade
        && !GameManagerIsMovieFlag()
        && m_Anim_Frame >= ATTACK_BLADE_EFFECT_START_FRAME
        )
    //if (m_Anim_State == AnimState::Blade
    //    && m_Anim_Frame >= 0
    //    && m_BladeFrame >= 0
    //    //&& m_Anim_Frame >= ATTACK_BLADE_EFFECT_START_FRAME
    //    )
        //if (m_Anim_State == AnimState::Blade
        //    && m_Anim_Frame >= ATTACK_BLADE_EFFECT_START_FRAME
        //    )
    //if ((m_CharaData->playingMotion.motion->name).find("BLADE") == 0
    //    && m_CharaData->playingMotion.frame >= ATTACK_BLADE_EFFECT_START_FRAME
    //    )
    {
        {
            {
                //float rott = CalculateRadToDeg(atan2(m_AxisVec.y, m_AxisVec.x)) - 90.0f;
                float rott = 0.0f;

                //調整用
                //if (m_CurrentAxis == AXIS_2D_LEFT) rott += -50.0f;
                //if (m_CurrentAxis == AXIS_2D_RIGHT) rott += 60.0f;
                //if (m_CurrentAxis == AXIS_2D_UP) rott += -100.0f;

                //DRAW_SPRITE_DAT dsdat = {};
                ////dsdat.x = 7200.0f * 0.08f * ((m_Obj->scale.x + m_Obj->scale.y) / 2)
                ////    * sin(CalculateDegToRad(m_CurrentAxisPrivate)) ;
                ////dsdat.y = 7200.0f * 0.08f * ((m_Obj->scale.x + m_Obj->scale.y) / 2)
                ////    * cos(CalculateDegToRad(m_CurrentAxisPrivate));
                //dsdat.y = 120.0f * ((m_Obj->scale.x + m_Obj->scale.y) / 2) ;

                //dsdat.w = m_Size_AttackEffect_s.x * m_Obj->scale.x;
                //dsdat.h = m_Size_AttackEffect_s.y * m_Obj->scale.y;

                //dsdat.rot = 0.0f;
                //dsdat.rot3D.x = 8.0f;
                //dsdat.color = { 1.5f, 0.2f, 1.5f, 1.0f };

                //dsdat.texNo = m_TexId_AttackEffect_s;
                //dsdat.loadTexType = LOADTEXTURETYPE_MAIN;

                ////dsdat.texUV = { 0.0f, 0.0f };
                ////dsdat.texWH = { 1.0f, 1.0f };
                //dsdat.texUV = AnimationUV(
                //    (int)(
                //        m_AnimCount_AttackEffect_s.x * m_AnimCount_AttackEffect_s.y
                //        * std::min((float)(m_Anim_Frame) / (30), 1.0f)
                //        //* std::min((float)(m_Anim_Frame - ATTACK_BLADE_EFFECT_START_FRAME) / (30), 1.0f)
                //        //* ((float)(m_CharaData->playingMotion.frame - ATTACK_BLADE_EFFECT_START_FRAME) / (ATTACK_BLADE_FRAME - ATTACK_BLADE_EFFECT_START_FRAME))
                //        ),
                //    (int)m_AnimCount_AttackEffect_s.x,
                //    (int)m_AnimCount_AttackEffect_s.y
                //);
                //dsdat.texWH = { 1.0f / m_AnimCount_AttackEffect_s.x, 1.0f / m_AnimCount_AttackEffect_s.y };

                //dsdat.startPos3D = m_Obj->pos;
                ////dsdat.startPos3D.z += -dsdat.h * 0.12f - 250.0f;
                //dsdat.startPos3D.z += -750.0f;

                //dsdat.startRot = rott;
                //dsdat.startRot3D.z = m_CurrentAxisPrivate;
                //dsdat.startScale = { 1.0f, 1.0f };

                //dsdat.shaderOpt.bloom = DPD_SHADER_OPT_BLOOM_NORMAL;
                ////dsdat.shaderOpt.blendState = DPD_SHADER_OPT_BLEND_STATE_ADDCOLOR;
                ////if (m_StartMotionShaderFlag)
                ////{
                ////    //dsdat.s
                ////}

                //DrawSpriteInCamera(dsdat);


                DRAW_POLYGON_DAT drawDatBaseTmp = {};

                drawDatBaseTmp.pos.y = 120.0f * ((m_Obj->scale.x + m_Obj->scale.y) / 2);

                drawDatBaseTmp.size.x = m_Size_AttackEffect_s.x * m_Obj->scale.x;
                drawDatBaseTmp.size.y = m_Size_AttackEffect_s.y * m_Obj->scale.y;
                drawDatBaseTmp.size.z = 1.0f;

                drawDatBaseTmp.rot = 0.0f;
                drawDatBaseTmp.rot3D = MkF3(0.0f, 0.0f, 0.0f);
                drawDatBaseTmp.rot3D.x = 5.0f;
                drawDatBaseTmp.color = { 1.5f, 0.2f, 1.5f, 1.0f };

                drawDatBaseTmp.texNo = m_TexId_AttackEffect_s;
                drawDatBaseTmp.loadTexType = LOADTEXTURETYPE_MAIN;

                drawDatBaseTmp.texUV = AnimationUV(
                    (int)(
                        m_AnimCount_AttackEffect_s.x * m_AnimCount_AttackEffect_s.y
                        * std::min((float)(m_Anim_Frame) / (30), 1.0f)
                        //* std::min((float)(m_Anim_Frame - ATTACK_BLADE_EFFECT_START_FRAME) / (30), 1.0f)
                        //* ((float)(m_CharaData->playingMotion.frame - ATTACK_BLADE_EFFECT_START_FRAME) / (ATTACK_BLADE_FRAME - ATTACK_BLADE_EFFECT_START_FRAME))
                        ),
                    (int)m_AnimCount_AttackEffect_s.x,
                    (int)m_AnimCount_AttackEffect_s.y
                );
                drawDatBaseTmp.texWH = { 1.0f / m_AnimCount_AttackEffect_s.x, 1.0f / m_AnimCount_AttackEffect_s.y };

                drawDatBaseTmp.startPos3D = m_Obj->pos;
                drawDatBaseTmp.startPos3D.z += -750.0f;

                drawDatBaseTmp.startRot = rott;
                drawDatBaseTmp.startRot3D.z = m_CurrentAxisPrivate;
                drawDatBaseTmp.startScale = MkF2(1.0f, 1.0f);
                drawDatBaseTmp.startScale3D = MkF3(1.0f, 1.0f, 1.0f);

                drawDatBaseTmp.modelNo = 0;

                //drawDatBaseTmp.shaderOpt.bloom = DPD_SHADER_OPT_BLOOM_NORMAL;
                //drawDatBaseTmp.shaderOpt.blendState = DPD_SHADER_OPT_BLEND_STATE_ADDCOLOR;

                DRAW_POLYGON_IN_CAMERA_EX_OPTION optBaseTmp = {};
                optBaseTmp.reverse = false;
                optBaseTmp.fixed = false;
                optBaseTmp.enableShade = true;
                optBaseTmp.billboard = false;
                optBaseTmp.priority = DRAW_POLYGON_PRIORITY_NORMAL;
                optBaseTmp.shaderType = DPD_SHADER_TYPE_EFFECT;

                DrawPolygonInCameraEx(drawDatBaseTmp, optBaseTmp);
            }
        }
    }

}

void GOE_Character_Enemy_Boss_Inperiusu::beam_object_update()
{
    //Stage_Base* stg = GetCurrentStageSceneGame();

    int index = 0;
    for (auto mbos : m_BeamObjects)
    {
        Float3 mainPos = m_Obj->pos;
        if (index > 0)
        {
            mainPos = m_MirrorgeState[index - 1].pos;
        }

        for (auto v : mbos)
        {
            GAME_OBJECT* ob = GetGameObjectFindByIdentifier(v);
            if (ob != nullptr)
            {
                if (ob->delRequ > 0)
                {
                    continue;
                }

                float dis = CalculateDistance(ob->pos, mainPos);
                float addrot = 90.0f;
                if (m_BeamIsOutLine)
                {
                    addrot += 180.0f;
                }
                //if (m_BeamNum == 3)
                //{
                //    addrot = 90.0f;
                //}
                //else
                //    if (m_BeamNum == 5)
                //    {
                //        addrot = -90.0f;
                //    }
                float rot = CalculateDegToRad(m_BeamRot + ob->freeInt + addrot);
                ob->pos.x = mainPos.x + cos(rot) * dis;
                ob->pos.y = mainPos.y + sin(rot) * dis;

                //if (m_Obj->frameActive % 10 == 0)
                //{
                //    if((int)(dis / (ONE_BLOCK * 2)) % 3 == 0 
                //        && IsInStageGameObject(ob))
                //    {
                //        Float3 posTmp = ob->pos;
                //        posTmp.z = -5.0f;

                //        GAME_OBJECT* go = GO_CreateEffect(
                //            GO_EFFECT_TYPE_EXPLOSION_1_GROUND2,
                //            posTmp
                //        );
                //        if (go != nullptr)
                //        if (go != nullptr)
                //        {
                //            go->rot = 55.0f * ob->frame + m_Obj->frame;
                //            go->scale = MkF3(
                //                30.0f,
                //                30.0f,
                //                30.0f
                //            );
                //            GameObjectSetLifeTime(go, 70);
                //        }
                //    }
                //}


                //if(
                //    (int)(dis / (ONE_BLOCK * 1)) % 3 == 0 && 
                //    m_Obj->frameActive % 8 == 0
                //    )
                //{
                //    if (stg != nullptr)
                //    {
                //        Int2 stgsize = stg->GetStageSize();

                //        Float2 size = {};
                //        size.x = BLOCK_WIDTH * stgsize.x;
                //        size.y = BLOCK_HIGHT * stgsize.y;

                //        if (abs(ob->pos.x) < size.x / 2
                //            && abs(ob->pos.y) < size.y / 2)
                //        {
                //            {
                //                Float3 posTmp = ob->pos;
                //                posTmp.z = -20.0f;

                //                GAME_OBJECT* go = GO_CreateEffect(GO_EFFECT_TYPE_EXPLOSION_1_GROUND, posTmp);
                //                if (go != nullptr)
                //                {
                //                    go->rot = 55.0f * m_Obj->frame;
                //                    go->scale = MkF3(
                //                        15.0f,
                //                        15.0f,
                //                        15.0f
                //                    );
                //                    GameObjectSetLifeTime(go, 50);
                //                }
                //            }
                //        }
                //    }
                //}
            }
        }
        index++;

    }
}

void GOE_Character_Enemy_Boss_Inperiusu::beam_object_delete()
{
    for (auto v : m_BeamObjects)
    {
        for (auto v2 : v)
        {
            GAME_OBJECT* ob = GetGameObjectFindByIdentifier(v2);
            if (ob != nullptr)
            {
                DeleteGameObject(ob);
            }
        }
        v.clear();
    }
    //m_BeamObjects.clear();
}

void GOE_Character_Enemy_Boss_Inperiusu::update_beam()
{
    //bool isInShockWave = m_BeamIsOutLine && m_ActCycleNum > 3;
    bool isInShockWave = m_BeamShockWaveType;

    int mirrorgeCnt = 0;
    {
        for (int i = 0; i < MIRRORGE_STATE_MAX; i++)
        {
            if (m_MirrorgeState[i].enable)
            {
                mirrorgeCnt++;
            }
        }
        isInShockWave &= mirrorgeCnt <= 0;
    }

    if (m_BeamFrame < FRAME_BEAM_SHOOT_END)
    {
        //ヒットストップアーマー
        m_Obj->status.hitStopArmorFrame = 100;
    }

    if (m_BeamFrame == 0)
    {
        m_BeamObjects.resize(1 + mirrorgeCnt);

        if (isInShockWave)
        {
            m_BeamNum = 5;
        }

        m_BeforeVecBuf = m_TargetVec;

        if (!m_BeamNoInitMove)
        {
            Float3 spos = MkF3(
                0.0f,
                0.0f + ONE_BLOCK * -20.0f,
                0.0f);
            if (m_BeamIsOutLine)
            {
                //spos = MkF3(
                //    0.0f,
                //    0.0f + ONE_BLOCK * -10.0f,
                //    0.0f);
                spos = MkF3(
                    0.0f,
                    0.0f + ONE_BLOCK * -16.0f,
                    0.0f);
            }

            if (CalculateDistance(spos, m_Obj->pos) > ONE_BLOCK * 8)
            {
                if (m_IsHard)
                {
                    StartWarp(spos, 0.0f, 25);
                }
                else
                {
                    if (CalculateDistance(spos, m_Obj->pos) > ONE_BLOCK * 20)
                    {
                        StartJet(spos, 0.0f,
                            160//240
                        );
                    }
                    else
                    {
                        StartMove(spos, 0.0f, 3.5, true);
                    }
                }
            }
            else
            {
                StartTurn(0.0f, 0.05f);
            }
        }
    }

    if (m_BeamFrame == 1)
    {
        if (m_IsHard && !m_HardPhase2Flag)
        {
            m_BeamRot = (float)(m_Obj->frameActive % 45);
        }
        else
        {
            m_BeamRot = 0.0f;
        }


        //SEを再生
        SoundEffectPlayRequest(GetSoundData("SOUND/SE/inperiusu/b_kamae"), 1.4f, 0);

        //SEを再生
        SoundEffectPlayRequest(GetSoundData("SOUND/SE/enemy/move/Inperius_Jet_Start"), 
            2.8f, 10);

        ////SEを再生
        //SoundEffectPlayRequest(GetSoundData("SOUND/SE/enemy/attack/Beam_Charge"),
        //    0.35f, 20);
        //SEを再生
        SoundEffectPlayRequest(GetSoundData("SOUND/SE/enemy/attack/Beam_Charge"),
            0.2f, 20);

        //SEを再生
        //SoundEffectPlayRequest(GetSoundData("SOUND/SE/inperiusu/beamChargeStart"),
        // 0.1f, 10);
        //SEを再生
        SoundEffectPlayRequest(GetSoundData("SOUND/SE/inperiusu/beamCharge2"), 
            0.16f, 10, 1.25f);

        //CameraFlashRequestAll(
        //    FRAME_BEAM_SHOOT_START,
        //    EasingReverse,
        //    //MkF4(0.6f, 0.3f, 1.0f, 0.8f),
        //    MkF4(6.0f, 0.5f, 10.0f, 0.3f),
        //    //DPD_SHADER_OPT_BLEND_STATE_DEFAULT
        //    DPD_SHADER_OPT_BLEND_STATE_ADDCOLOR
        //);

        //Float3 zoompos = m_Obj->pos;
        //zoompos.z += -4000.0f;
        //CameraZoomRequestAll(
        //    0.02f,
        //    zoompos,
        //    EasingInOutSine,
        //    0.22f
        //);
        //CameraZoomResetRequestAll(
        //    0.02f,
        //    EasingInOutSine,
        //    FRAME_BEAM_SHOOT_START * 0.5f
        //);
        
        SetCameraModeAll(CAMERA_MODE_BOSS_CHASE_WIDE);
        //SetCameraModeAll(CAMERA_MODE_PLAYER_CHASE);

        StartAnim(AnimState::BeamBefore, FRAME_BEAM_SHOOT_START, true);
        //{ //アニメーション
        //    GraphicSpriteSet(m_Tex_Main_Beam1, 6, 6);
        //    Character::PlayMotionCharacterGameObject(
        //        m_CharaData,
        //        Character::GetCharacterMotionFindByName(
        //            Character::GetCharacterMainData(m_CharaData->dataIndex),
        //            "BEAM_1"),
        //        false,
        //        nullptr,
        //        FRAME_ONE_SECOND * 1
        //    );
        //}
    }
    if (m_BeamFrame >= 1
        && m_BeamFrame < FRAME_BEAM_SHOOT_START
        )
    {
        if (m_BeamFrame % 10 == 0)
        {
            CameraShakeRequestAll(
                CAMERA_SHAKE_MIDIUM_FRAME,
                //CAMERA_SHAKE_LARGE_POWER * 1.0f
                CAMERA_SHAKE_LARGE_POWER * 0.5f
            );
        }
        if (m_BeamFrame % 4 == 0)
        {
            Float3 postmp = m_Obj->pos;
            postmp.z += -1200.0f;

            GAME_OBJECT* go = GO_CreateEffect(GO_EFFECT_TYPE_SPARK, postmp);
            if (go != nullptr)
            {
                //if (obj->frameActive % 4 == 0)
                //{
                //    go->graphic.color = MkF4(
                //        0.2f,
                //        0.2f,
                //        0.1f,
                //        1.0f
                //    );
                //}
                //else
                //go->graphic.rot3d.x = 90.0f;
                go->rot = 90.0f + -52.0f * m_Obj->frame;
                //go->graphic.color = MkF4(
                //    0.1f,
                //    0.0f,
                //    0.3f,
                //    -1.0f
                //);
                go->graphic.color = MkF4(
                    2.0f,
                    0.5f,
                    10.0f,
                    0.75f
                );
                go->scale.x = 32.0f;
                go->scale.y = 32.0f;
                go->scale.z = 32.0f;
                GameObjectSetLifeTime(go, 5);
            }
        }
    }


    if (m_BeamFrame == FRAME_BEAM_SHOOT_START - 20)
    {
        //SoundEffectPlayRequest(GetSoundData("SOUND/SE/enemy/move/warpHigh"), 1.5f, 0);
    }
    if (m_BeamFrame == FRAME_BEAM_SHOOT_START - 18)
    {
        m_BeamBgmVolumeBuf = GetSoundBGMVolume();
        SoundBGMSetVolumeRequest(m_BeamBgmVolumeBuf * 0.4f);

        //SEを再生
        SoundEffectPlayRequest(GetSoundData("SOUND/SE/inperiusu/beamStartEx"),
            2.0f, 0, 0.9f);
        //SEを再生
        SoundEffectPlayRequest(GetSoundData("SOUND/SE/inperiusu/b_attack"),
            0.7f, 5, 0.8f);
    }
    if (m_BeamFrame == FRAME_BEAM_SHOOT_START)
    {
        //SoundEffectPlayRequest(GetSoundData("SOUND/SE/enemy/attack/Small_Explosion"), 5.0f, 0);
        //SoundEffectPlayRequest(GetSoundData("SOUND/SE/enemy/attack/Explosion"), 0.6f, 0);
        ////SoundEffectPlayRequest(GetSoundData("SOUND/SE/enemy/attack/Explosion"), 1.5f, 0);

        ////SEを再生
        //SoundEffectPlayRequest(GetSoundData("SOUND/SE/inperiusu/beamStart"),
        //    0.35f, 0, 2.5f);
        ////SEを再生
        //SoundEffectPlayRequest(GetSoundData("SOUND/SE/inperiusu/beamStart"),
        //    0.08f, 30, 2.2f);
        //SoundEffectPlayRequest(GetSoundData("SOUND/SE/inperiusu/beamStart"),
        //    0.08f, 70, 2.0f);
        ////SEを再生
        //SoundEffectPlayRequest(GetSoundData("SOUND/SE/inperiusu/beamStart"),
        //    0.08f, 110, 1.8f);
        //SEを再生
        //SoundEffectPlayRequest(GetSoundData("SOUND/SE/inperiusu/beamStart2"),
        //    0.4f, 5, 0.7f);
        //SoundEffectPlayRequest(GetSoundData("SOUND/SE/enemy/attack/Small_Explosion"), 
        //    5.0f, 0, 0.9f);
        //SEを再生
        SoundEffectPlayRequest(GetSoundData("SOUND/SE/inperiusu/beamStart2"),
            0.55f, 30, 0.7f);
        //SoundEffectPlayRequest(GetSoundData("SOUND/SE/enemy/attack/Small_Explosion"), 
        //    7.0f, 0, 0.7f);
        

        CameraFlashRequestAll(
            30,
            EasingInOutSine,
            //MkF4(1.0f, 0.5f, 0.9f, 2.0f),
            MkF4(10.0f, 10.0f, 5.0f, 0.6f),
            DPD_SHADER_OPT_BLEND_STATE_ADDCOLOR
        );

        StartAnim(AnimState::Beam, 
            FRAME_BEAM_SHOOT_END - FRAME_BEAM_SHOOT_START, 
            false);

        GAME_OBJECT beamObj = GetGameObjectPrefab(GAME_OBJECT_TYPE_DUMMY);
        {
            //beamObj.pos = m_Obj->pos;

            Character::MOTION_COL_DAT motionColDetails =
                Character::GetMotionColDataFindByName(
                    Character::MOTION_COL_TYPE_ATK,
                    BEAM_ATK_COL_NAME
                );

            GAME_OBJECT_COLLISION_ATTACK_CHILD attackColTmp = motionColDetails.atkCol;
            attackColTmp.subjectFamily[GAME_OBJECT_FAMILY_PLAYER] = true;
            attackColTmp.subjectFamily[GAME_OBJECT_FAMILY_GIMIK] = true;

            attackColTmp.size = MkF3(
                ONE_BLOCK * 8,
                ONE_BLOCK * 8,
                ONE_BLOCK * 8
            );
            attackColTmp.pos.z = -attackColTmp.size.z / 2;
            //attackColTmp.pos.y += attackColTmp.size.y / 2 * 1.0f;

            beamObj.collision.atk.enable = true;
            beamObj.collision.atk.collision[0] = attackColTmp;
            beamObj.collision.atk.collision[0].enable = true;
        }
        
        //{
        //    GAME_OBJECT* beamObjCpy = new GAME_OBJECT;
        //    (*beamObjCpy) = beamObj;
        //    beamObjCpy->collision.atk.collision[0].size.x = ONE_BLOCK * 200.0f;
        //    GAME_OBJECT* beamObjCreated = CreateGameObject(*beamObjCpy);
        //    GameObjectSetLifeTime(beamObjCreated, FRAME_ONE_SECOND * 3);
        //    delete beamObjCpy;
        //}
        //{
        //    GAME_OBJECT* beamObjCpy = new GAME_OBJECT;
        //    (*beamObjCpy) = beamObj;
        //    beamObjCpy->collision.atk.collision[0].size.y = ONE_BLOCK * 200.0f;
        //    GAME_OBJECT* beamObjCreated = CreateGameObject(*beamObjCpy);
        //    GameObjectSetLifeTime(beamObjCreated, FRAME_ONE_SECOND * 3);
        //    delete beamObjCpy;
        //}

        for (int obji = 0; obji < m_BeamObjects.size(); obji++)
        {
            for (int i = 0; i < m_BeamNum; i++)
            {
                //int jCnt = 20;
                int jCnt = 16;
                for (int j = 0; j < jCnt; j++)
                {
                    GAME_OBJECT* beamObjCpy = new GAME_OBJECT;
                    (*beamObjCpy) = beamObj;

                    Float3 mainPos = m_Obj->pos;
                    if (obji > 0)
                    {
                        mainPos = m_MirrorgeState[obji - 1].pos;
                    }
                    beamObjCpy->pos = mainPos;

                    //beamObjCpy->pos.x += (ONE_BLOCK * -150.0f + (ONE_BLOCK * 300.0f / jCnt) * j)
                    //    * (i == 2 ? -1:1) * (i == 0 ? 0 : 1);
                    //beamObjCpy->pos.y += (ONE_BLOCK * -150.0f + (ONE_BLOCK * 300.0f / jCnt) * j)
                    //    * (i == 1 ? 0 : 1);

                    //beamObjCpy->freeInt = (int)CalculateRadToDeg(atan2(
                    //    beamObjCpy->pos.y - m_Obj->pos.y,
                    //    beamObjCpy->pos.x - m_Obj->pos.x
                    //)) - 90.0f;

                    //beamObjCpy->pos = {};
                    if (m_BeamIsOutLine)
                    {
                        beamObjCpy->pos.x += ((ONE_BLOCK * 90.0f - m_BeamOutLineRangeMod) / jCnt) * j;
                        //beamObjCpy->pos.x += -m_BeamOutLineRangeMod;
                    }
                    else
                    {
                        beamObjCpy->pos.x += (ONE_BLOCK * 150.0f / jCnt) * j;
                        beamObjCpy->pos.x += ONE_BLOCK * 20.0f;
                    }

                    beamObjCpy->freeInt = (int)((360.0f / m_BeamNum) * i);

                    GAME_OBJECT* beamObjCreated = CreateGameObject(*beamObjCpy);
                    if (beamObjCreated != nullptr)
                    {
                        m_BeamObjects[obji].push_back(GetIdentifierGameObject(beamObjCreated));
                        //GameObjectSetLifeTime(beamObjCreated, FRAME_ONE_SECOND * 2.5f);
                    }
                    delete beamObjCpy;
                }
            }
        }
    }
    if (m_BeamFrame == FRAME_BEAM_SHOOT_START + 30)
    {
        SoundBGMSetVolumeRequest(m_BeamBgmVolumeBuf);
    }

    beam_object_update();

    if (m_BeamFrame >= FRAME_BEAM_SHOOT_START
        && m_BeamFrame < FRAME_BEAM_SHOOT_END
    )
    {
        //ヒットストップアーマー
        //m_Obj->status.hitStopArmorFrame = 100;

        if (isInShockWave)
        {
            if (m_BeamFrame % 8 == 0)
            {
                float angle = 360.0f;
                int bulletnum = 2;

                for (int obji = 0; obji < m_BeamObjects.size(); obji++)
                {
                    Float3 mainPos = m_Obj->pos;
                    if (obji > 0)
                    {
                        mainPos = m_MirrorgeState[obji - 1].pos;
                    }
                    for (int i = 0; i < bulletnum; i++)
                    {
                        float radtmp = CalculateDegToRad(
                            180.0f + (angle - 180.0f) / 4 - (angle / bulletnum * i)
                            + m_BeamFrame * 2.5f
                        );
                        {
                            Float3 nvec = NormalizeVector(
                                MkF3(
                                    (float)cos(radtmp) * 3.0f,
                                    (float)sin(radtmp) * 3.0f,
                                    0.0f
                                ));

                            GOE_Character_EnemyProjectile* goepro =
                                GOE_Character_EnemyProjectile::CreateProjectile(
                                    GOE_Character_EnemyProjectile::Type::ExplosionShockWave,
                                    MkF3(
                                        mainPos.x + nvec.x * ONE_BLOCK * 5.0f,
                                        mainPos.y + nvec.y * ONE_BLOCK * 5.0f,
                                        mainPos.z + nvec.z * ONE_BLOCK * 5.0f
                                    ),
                                    nvec
                                );
                            if (goepro != nullptr)
                            {
                                goepro->SetColor(MkF4(1.7f, 1.0f, 2.0f, 1.0f));

                                GAME_OBJECT* beamObj = goepro->GetGameObj();
                                if (beamObj != nullptr)
                                {
                                    beamObj->scale.x = 3.5f;
                                    beamObj->scale.y = 3.5f;
                                    beamObj->scale.z = 3.5f;
                                    {
                                        Character::MOTION_COL_DAT motionColDetails =
                                            Character::GetMotionColDataFindByName(
                                                Character::MOTION_COL_TYPE_ATK,
                                                BLADE_ATK_COL_NAME
                                            );

                                        GAME_OBJECT_COLLISION_ATTACK_CHILD attackColTmp = motionColDetails.atkCol;
                                        attackColTmp.subjectFamily[GAME_OBJECT_FAMILY_PLAYER] = true;
                                        attackColTmp.subjectFamily[GAME_OBJECT_FAMILY_GIMIK] = true;

                                        attackColTmp.size = MkF3(
                                            ONE_BLOCK * 3,
                                            ONE_BLOCK * 3,
                                            ONE_BLOCK * 3
                                        );
                                        attackColTmp.pos.z = -attackColTmp.size.z / 2;
                                        attackColTmp.pos.y += attackColTmp.size.y / 2 * 1.0f;

                                        beamObj->collision.atk.enable = true;
                                        beamObj->collision.atk.collision[0] = attackColTmp;
                                        beamObj->collision.atk.collision[0].enable = true;
                                    }
                                    beamObj->blockCollisionIgnore = false;

                                    GameObjectSetLifeTime(beamObj, 80);
                                    GameObjectMoveRequest(beamObj, 80,
                                        /*             GameObjectSetLifeTime(beamObj, 200);
                                                 GameObjectMoveRequest(beamObj, 200,*/
                                        MkF3(
                                            beamObj->pos.x + nvec.x * ONE_BLOCK * 70.0f,
                                            beamObj->pos.y + nvec.y * ONE_BLOCK * 70.0f,
                                            beamObj->pos.z + nvec.z * ONE_BLOCK * 70.0f
                                        ),
                                        nullptr
                                    );
                                }
                            }
                        }
                    }
                }
            }
        }

        if (
            //m_BeamFrame >= FRAME_BEAM_SHOOT_START + 30 &&
            m_BeamFrame % 30 == 0)
        {
            //SEを再生
            SoundEffectPlayRequest(GetSoundData("SOUND/SE/player/attack/Kakopon_Contorius"), 
                m_BeamFrame < FRAME_BEAM_SHOOT_START + 60 ? 0.25f : 0.45f,//0.6f,
                0, 1.1f);
        }
        if (m_BeamFrame % 50 == 0)
        {
            //SEを再生
            SoundEffectPlayRequest(
                GetSoundData("SOUND/SE/enemy/attack/Small_Explosion"),
                3.5f, 0, 0.4);
        }

        if (m_BeamFrame % 8 == 0)
        {
            CameraShakeRequestAll(
                CAMERA_SHAKE_MIDIUM_FRAME,
                //CAMERA_SHAKE_EXLARGE_POWER * 1.0f
                CAMERA_SHAKE_EXLARGE_POWER * 0.7f
            );
        }
        if (m_BeamFrame % 4 == 0)
        {
            Float3 postmp = m_Obj->pos;
            postmp.z += -1200.0f;

            GAME_OBJECT* go = GO_CreateEffect(GO_EFFECT_TYPE_SPARK, postmp);
            if (go != nullptr)
            {
                //if (obj->frameActive % 4 == 0)
                //{
                //    go->graphic.color = MkF4(
                //        0.2f,
                //        0.2f,
                //        0.1f,
                //        1.0f
                //    );
                //}
                //else
                //go->graphic.rot3d.x = 90.0f;
                go->rot = 90.0f + -52.0f * m_Obj->frame;
                //go->graphic.color = MkF4(
                //    0.1f,
                //    0.0f,
                //    0.3f,
                //    -1.0f
                //);
                go->graphic.color = MkF4(
                    2.0f,
                    0.5f,
                    10.0f,
                    0.75f
                );
                go->scale.x = 40.0f;
                go->scale.y = 40.0f;
                go->scale.z = 40.0f;
                GameObjectSetLifeTime(go, 5);
            }
        }
    }

    //if (m_BeamFrame >= FRAME_BEAM_SHOOT_START + 60
    //    && m_BeamFrame < FRAME_BEAM_SHOOT_END - 60)
    //{
    //    //m_BeamRot += 1.5f;
    //    m_BeamRot += 0.5f;
    //}

    if (m_IsHard)
    {
        //if (m_BeamFrame >= FRAME_BEAM_SHOOT_START + 10
        //    && m_BeamFrame < FRAME_BEAM_SHOOT_END - 10)
        {
            //m_BeamRot += 1.5f;
            //m_BeamRot += cos((float)m_BeamFrame / 50) * 1.2f;
            //m_BeamRot += cos(2.0f * SYS_GENERAL_PI
            //    * ((float)(m_BeamFrame - FRAME_BEAM_SHOOT_START)
            //        / (float)(FRAME_BEAM_SHOOT_END - FRAME_BEAM_SHOOT_START))
            //) * 1.0f;

            if (m_HardPhase2Flag)
            {
                //m_BeamRot += sin((float)m_BeamFrame / 72) * 0.65f;

                int actn = m_BeamRotatePattarn;
                switch (actn)
                {
                case 2:
                    if (m_BeamFrame % 60 < 30)
                    {
                        m_BeamRot += 0.85f;
                    }
                    break;
                case 1:
                    m_BeamRot += sin((float)m_BeamFrame / 72) * 0.75f;
                    break;
                default:
                    m_BeamRot += 0.5f;
                    break;
                }
            }
            else
            {
                m_BeamRot += sin((float)m_BeamFrame / 72) * 1.05f;
            }
        }
    }
    else
    {
        //m_BeamRot += 1.5f;
        //m_BeamRot += 0.25f;
        m_BeamRot += 0.36f;
    }

    if (m_BeamFrame == FRAME_BEAM_SHOOT_END)
    { //判定の削除

        beam_object_delete();
        SoundEffectPlayRequest(GetSoundData("SOUND/SE/enemy/move/Inperius_Jet_End"), 
            1.2f, 0);
        //SEを再生
        SoundEffectPlayRequest(GetSoundData("SOUND/SE/inperiusu/beam_end"),
            0.2f, 0, 0.35f);

        SoundBGMSetVolumeRequest(m_BeamBgmVolumeBuf);

        SetCameraModeAll(CAMERA_MODE_BOSS_CHASE);
    }

    bool isCooldownCancel = false;
    if (m_BeamFrame == FRAME_BEAM_SHOOT_END + FRAME_ONE_SECOND)
    {

        for (int i = 0; i < MIRRORGE_STATE_MAX; i++)
        {
            if (m_MirrorgeState[i].enable)
            {
                MirrorgeStateEndSet(i);
            }
        }

        if (m_IsHard)
        {
            GAME_OBJECT* targetObject = GetGameObjectFindByIdentifier(m_TargetObjIdentifier);
            if (targetObject != nullptr)
            {
                if (CalculateDistance(targetObject->pos, m_Obj->pos)
                    < ONE_BLOCK * 20.0f)
                {
                    isCooldownCancel = true;
                }
            }
        }
    }

    //if (m_BeamFrame == FRAME_BEAM_SHOOT_END + FRAME_BEAM_AFTER)
    //{
    //    SoundEffectPlayRequest(GetSoundData("SOUND/SE/enemy/move/Inperius_Jet_End"), 1.2f, 0);
    //}

    m_BeamFrame++;

    if(
        (m_BeamFrame > FRAME_BEAM_SHOOT_END + FRAME_BEAM_AFTER + FRAME_ONE_SECOND * 1)
        || isCooldownCancel
    )
    {
        SetCameraModeAll(CAMERA_MODE_BOSS_CHASE);

        beam_object_delete();

        m_ActionCooldown = m_WazaCooldown;
        m_BeamFrame = -1;
        m_CurrentState = ActionState::Idle;

        if (isCooldownCancel)
        {
            m_BladeFrame = 0;
            m_CurrentState = ActionState::Blade;

            m_BladeWarpNum = 0;
            m_BladeBeforeSpeedMod = 0.4f;
            m_BladeWaveNum = m_HardPhase2Flag ? 1 : 0;
            m_BladeWaveAngle = 0.0f;
            m_BladeNoTurnFlag = false;
            m_BladeAfterWarpFlag = false;
            m_BladeSuperFlag = false;
        }

        if (m_BeamAfterWarpFlag)
        {
            StartWarp({ 0.0f, ONE_BLOCK * -15.0f, 0.0f }, 0.0f, 25, true);
        }
    }
    //m_BeamFrame++;
}
void GOE_Character_Enemy_Boss_Inperiusu::draw_beam()
{
    for (int obji = 0; obji < m_BeamObjects.size(); obji++)
    {
        Float3 mainPos = m_Obj->pos;
        if (obji > 0)
        {
            mainPos = m_MirrorgeState[obji - 1].pos;
        }
        draw_beam_child(mainPos);
    }
}
void GOE_Character_Enemy_Boss_Inperiusu::draw_beam_child(Float3 mainPosition )
{

    //ビーム
    //if(false)

    float beamfmod = ((float)FRAME_BEAM_SHOOT_START / (ANIMCOUNTMAX_BEAM_CHARGE * 0.35f));

    bool isBeamStateActive = 
        m_BeamFrame >= 0
        && m_BeamFrame < FRAME_BEAM_SHOOT_END + FRAME_BEAM_AFTER;
    //if()
    {
        //int beamFrame = (m_BeamFrame / 1) % ANIMCOUNTMAX_BEAM_CHARGE;
        //int beamFrameAnim = (int)(m_BeamFrame / beamfmod) % ANIMCOUNTMAX_BEAM_CHARGE;


        int beamShootStartFrame = FRAME_BEAM_SHOOT_START;
        int beamShootFrame = m_BeamFrame - beamShootStartFrame;
        //int beamShootFrameMax = ANIMCOUNTMAX_BEAM_CHARGE - beamShootStartFrame;
        //int beamShootFrameMax = ANIMCOUNTMAX_BEAM_CHARGE - beamShootStartFrame;

        float beamMotionStartMod = 1.0f;
        float beamMotionStartMod_Bit = 1.0f;
        float beamStartMod = 1.0f;
        float beamEndMod = 1.0f;
        float beamEndMod_Bit = 1.0f;
        {
            int endf = 24;
            int f = m_BeamFrame - 55;
            if (f < endf)
            {
                beamMotionStartMod = EasingInSine(
                    std::max((float)(f) / endf, 0.0f)
                );
            }
        }
        {
            int endf = 50;
            if (m_BeamFrame < endf)
            {
                beamMotionStartMod_Bit = EasingInSine((float)m_BeamFrame / endf);
            }
        }
        {
            int endf = 30;
            if (beamShootFrame < endf)
            {
                beamStartMod = EasingOutElastic((float)beamShootFrame / endf);
            }
        }
        {
            int beamShootEndStartFrame = FRAME_BEAM_SHOOT_END;
            if (m_BeamFrame >= beamShootEndStartFrame)
            {
                int f = m_BeamFrame - beamShootEndStartFrame;
                int fMax = FRAME_BEAM_AFTER;
                beamEndMod = 1.0f - (float)f / fMax;
            }
            //else
            //{
            //    //仮
            //    if (beamFrame % 15 == 0)
            //    {
            //        CameraShakeRequestAll(CAMERA_SHAKE_MIDIUM_FRAME, CAMERA_SHAKE_LARGE_POWER);
            //    }
            //}
        }
        {
            int beamShootEndStartFrame = FRAME_BEAM_SHOOT_END;
            if (m_BeamFrame >= beamShootEndStartFrame)
            {
                int f = m_BeamFrame - beamShootEndStartFrame;
                int fMax = 60;
                beamEndMod_Bit = 1.0f - (float)f / fMax;
            }
            //else
            //{
            //    //仮
            //    if (beamFrame % 15 == 0)
            //    {
            //        CameraShakeRequestAll(CAMERA_SHAKE_MIDIUM_FRAME, CAMERA_SHAKE_LARGE_POWER);
            //    }
            //}
        }

        if (isBeamStateActive)
        {
            if (m_BeamFrame < beamShootStartFrame)
            {
                DRAW_POLYGON_DAT dsdat = {};
                dsdat.pos.x = 0.0f;
                dsdat.pos.y = 0.0f;
                dsdat.pos.z = 0.0f;

                dsdat.size.x = SCREEN_WIDTH;
                dsdat.size.y = SCREEN_HEIGHT;
                dsdat.size.z = 1.0f;

                dsdat.rot = 0.0f;

                //dsdat.color = { 0.5f, 0.0f, 0.3f, 0.25f };
                dsdat.color = {
                    1.0f,
                    1.0f,
                    1.0f,
                    0.8f
                };
                dsdat.color.w *= beamMotionStartMod,

                    dsdat.texNo = m_Tex_Vignette;
                dsdat.loadTexType = LOADTEXTURETYPE_MAIN;

                dsdat.texUV = {
                    0.0f,
                    0.0f
                };
                dsdat.texWH = {
                    1.0f,
                    1.0f
                };

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
                //opt.priority = DRAW_POLYGON_PRIORITY_NML_d1;
                opt.priority = DRAW_POLYGON_PRIORITY_NML_d1;

                DrawPolygonInCameraEx(dsdat, opt);
            }
            {
                DRAW_POLYGON_DAT dsdat = {};
                dsdat.pos.x = 0.0f;
                dsdat.pos.y = 0.0f;
                dsdat.pos.z = 0.0f;

                dsdat.size.x = SCREEN_HEIGHT;
                dsdat.size.y = SCREEN_WIDTH;
                dsdat.size.z = 1.0f;

                dsdat.rot = 90.0f;

                //dsdat.color = { 0.5f, 0.0f, 0.3f, 0.25f };
                dsdat.color = {
                    0.4f,
                    0.0f,
                    0.5f,
                    //0.18f
                    0.5f
                };
                dsdat.color.w *= beamStartMod * beamEndMod,

                    dsdat.texNo = m_Tex_Barrier_Fresnel1;
                dsdat.loadTexType = LOADTEXTURETYPE_MAIN;

                dsdat.texUV = {
                    0.0f + 0.12f * m_Obj->frameActive,
                    0.0f
                };
                dsdat.texWH = {
                    1.2f,
                    1.8f
                };

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

                if (dsdat.color.w > 0)
                {
                    DrawPolygonInCameraEx(dsdat, opt);
                }
            }
        }

        for (int j = 0; j < m_BeamNum; j++)
        //for (int j = 0; j < 8; j++)
        {
            float addOffsetZ = 1000.0f;
            if (m_BeamIsOutLine)
            {
                addOffsetZ += -9500.0f + m_BeamOutLineRangeMod;
            }

            //if(isBeamStateActive
            //    || beamMotionStartMod_Bit < 1.0f
            //    || beamEndMod_Bit < 1.0f
            //)
            {
                bool isvisibletmp = false;
                float blasterAddOffsetY = 0;
                if (m_IsHard)
                {
                    isvisibletmp = isBeamStateActive;
                }
                else
                {
                    constexpr float blasterStartPosY = -4000.0f;
                    blasterAddOffsetY =
                        blasterStartPosY * pow((1.0f - beamMotionStartMod_Bit), 10.0f)
                        + blasterStartPosY * pow((1.0f - beamEndMod_Bit), 10.0f)
                        ;

                    isvisibletmp = isBeamStateActive
                        || blasterAddOffsetY > (blasterStartPosY + 10.0f)
                    ;
                }

                if(isvisibletmp)
                {
                    DRAW_POLYGON_DAT drawDatTmp = {};
                    drawDatTmp.size = MkF3(
                        //200.0f * beamMotionStartMod * beamEndMod,
                        //200.0f * beamMotionStartMod * beamEndMod,
                        //200.0f * beamMotionStartMod * beamEndMod
                        200.0f,
                        -200.0f,
                        200.0f
                    );

                    drawDatTmp.pos.z += -500.0f;
                    //drawDatTmp.pos.z += -500.0f + addOffsetZ;

                    drawDatTmp.rot = j * (360.0f / m_BeamNum) + m_BeamRot + 180.0f;

                    drawDatTmp.rot3D = MkF3(
                        90.0f,//0.0f, 
                        0.0f,//90.0f,
                        0.0f);

                    drawDatTmp.color = MkF4(1.0f, 1.0f, 1.0f, 1.0f);

                    drawDatTmp.texNo = m_Tex_Model.textureId;
                    drawDatTmp.loadTexType = LOADTEXTURETYPE_MAIN;

                    drawDatTmp.texUV = {
                        0.0f,
                        0.0f
                    };
                    drawDatTmp.texWH = {
                        1.0f,
                        1.0f
                    };

                    drawDatTmp.startPos3D = mainPosition;

                    drawDatTmp.startRot3D = MkF3(
                        0.0f,
                        0.0f,
                        0.0f
                    );

                    drawDatTmp.startScale = MkF2(1.0f, 1.0f);
                    drawDatTmp.startScale3D = MkF3(1.0f, 1.0f, 1.0f);

                    drawDatTmp.modelNo = m_Model_Bit;

                    //drawDatTmp.shaderOpt.bloom = DPD_SHADER_OPT_BLOOM_NORMAL;
                    //drawDatTmp.shaderOpt.blendState = DPD_SHADER_OPT_BLEND_STATE_ADDCOLOR;

                    drawDatTmp.shaderOpt.outline = DPD_SHADER_OPT_OUTLINE_NORMAL;
                    drawDatTmp.shaderOpt.outlineLayer = 1;

                    DRAW_POLYGON_IN_CAMERA_EX_OPTION opt = {};
                    opt.shaderType = m_shaderType;
                    opt.reverse = false;
                    opt.fixed = false;
                    opt.enableShade = true;
                    opt.billboard = false;
                    ////opt.priority = DRAW_POLYGON_PRIORITY_NML_u2;
                    //opt.priority = DRAW_POLYGON_PRIORITY_NML_ALWAYS;
                    ////opt.depthBuffIgnore = true;

                    opt.priority = DRAW_POLYGON_PRIORITY_NORMAL;

                    float posAddRotTmp = drawDatTmp.rot - 90.0f;

                    if (m_IsHard)
                    {
                        float mod = -1.0f;

                        if (beamMotionStartMod_Bit < 1.0f)
                        {
                            //mod = EasingInBack(EasingOutSine(mod));
                            mod = (EasingOutQuad(beamMotionStartMod_Bit));
                        }
                        else if (beamEndMod_Bit < 1.0f)
                        {
                            mod = (EasingInQuad(beamEndMod_Bit));
                        }

                        if (mod >= 0.0f)
                        {
                            {
                                drawDatTmp.startScale3D.x = 0.2f + 0.8f * EasingInOutBack(mod);
                                drawDatTmp.startScale3D.y = 0.2f + 0.8f * EasingInOutBack(mod);
                                drawDatTmp.startScale3D.z = 2.5f - 1.5f * EasingInOutBack(mod);
                            }

                            drawDatTmp.shaderOpt.outline = DPD_SHADER_OPT_OUTLINE_NONE;

                            drawDatTmp.loadTexType = LOADTEXTURETYPE_MAIN;

                            constexpr float coloutmod = 0.1f;
                            float colmod = 0.45f + (0.55f + coloutmod) * mod;
                            if (colmod > 1.0f)
                            {
                                float colmod2 = 1.0f - ((colmod - 1.0f) / coloutmod);
                                float addcol = 10.0f * colmod2;


                                drawDatTmp.color = MkF4(
                                    1.0f + addcol,
                                    1.0f + addcol,
                                    1.0f + addcol,
                                    1.0f);
                                drawDatTmp.texNo = m_Tex_Model.textureId;
                            }
                            else
                            {
                                drawDatTmp.color = MkF4(
                                    colmod,
                                    colmod,
                                    colmod,
                                    2.0f);
                                drawDatTmp.texNo = m_Tex_Warp.textureId;
                                drawDatTmp.shaderOpt.blendState = DPD_SHADER_OPT_BLEND_STATE_ADDCOLOR;
                                drawDatTmp.texWH = {
                                    1.0f * 2.0f,
                                    1.0f * 2.0f
                                };

                                opt.shaderType = DPD_SHADER_TYPE_EFFECT_WARP;
                            }
                        }
                    }
                    else
                    {
                        drawDatTmp.startPos3D.z += blasterAddOffsetY;

                        if (beamMotionStartMod_Bit < 1.0f)
                        {
                            drawDatTmp.rot3D.x += 90.0f
                                * EasingInOutCubic(std::min((1.0f - beamMotionStartMod_Bit)
                                    / 0.4f, 1.0f));
                        }
                        if (beamEndMod_Bit < 1.0f)
                        {
                            posAddRotTmp += 90.0f * pow(1.0f - beamEndMod_Bit, 4.0f);
                            drawDatTmp.rot3D.x += 90.0f
                                * EasingInOutCubic(std::min((1.0f - beamEndMod_Bit)
                                    / 0.4f, 1.0f));
                        }
                    }
                    {
                        float rotTmp = CalculateDegToRad(
                            posAddRotTmp
                        );

                        {
                            drawDatTmp.startPos3D.x += cos(rotTmp) * (200.0f + addOffsetZ);
                            drawDatTmp.startPos3D.y += sin(rotTmp) * (200.0f + addOffsetZ);
                        }
                    }

                    DrawPolygonInCameraEx(drawDatTmp, opt);
                }
            }

            if (isBeamStateActive)
            {
                {
                    DRAW_POLYGON_DAT drawDatTmp = {};
                    drawDatTmp.size = MkF3(
                        7500.0f * beamMotionStartMod * beamEndMod,
                        7500.0f * beamMotionStartMod * beamEndMod,
                        1.0f);

                    {
                        //float rotTmp = CalculateDegToRad(
                        //    j * 90.0f + 90.0f + m_BeamRot
                        //);
                        float rotTmp = CalculateDegToRad(
                            j * (360.0f / m_BeamNum) + 90.0f + m_BeamRot
                        );

                        //float rotTmp = CalculateDegToRad(j * 45.0f + 90.0f);
                        //float rotTmp = CalculateDegToRad(m_CurrentAxisEx * 45.0f + 90.0f);
                        drawDatTmp.pos.x += cos(rotTmp) * (900.0f + addOffsetZ);
                        drawDatTmp.pos.y += sin(rotTmp) * (900.0f + addOffsetZ);
                    }
                    //drawDatTmp.pos.y += 120.0f;
                    //drawDatTmp.pos.z += -800.0f;
                    drawDatTmp.pos.z += -500.0f;

                    drawDatTmp.rot = 0.0f;

                    drawDatTmp.rot3D = MkF3(0.0f, 0.0f, 0.0f);

                    drawDatTmp.color = MkF4(1.0f, 1.0f, 1.0f, 0.6f);

                    drawDatTmp.texNo = m_Tex_BeamCharge;
                    drawDatTmp.loadTexType = LOADTEXTURETYPE_MAIN;

                    int beamFrameAnim = (int)(m_BeamFrame / beamfmod);
                    if (beamFrameAnim >= 128)
                    {
                        beamFrameAnim = 128 + beamFrameAnim % ANIMCOUNT_BEAM_CHARGE_X;
                    }
                    drawDatTmp.texUV = AnimationUV(
                        beamFrameAnim,
                        ANIMCOUNT_BEAM_CHARGE_X,
                        ANIMCOUNT_BEAM_CHARGE_Y
                    );
                    drawDatTmp.texWH = {
                        1.0f / ANIMCOUNT_BEAM_CHARGE_X,
                        1.0f / ANIMCOUNT_BEAM_CHARGE_Y
                    };

                    drawDatTmp.startPos3D = mainPosition;

                    //drawDatTmp.startRot = m_CurrentAxisEx * 45.0f;
                    drawDatTmp.startRot3D = MkF3(0.0f, 0.0f, 0.0f);

                    drawDatTmp.startScale = MkF2(1.0f, 1.0f);
                    drawDatTmp.startScale3D = MkF3(1.0f, 1.0f, 1.0f);

                    drawDatTmp.modelNo = 0;

                    drawDatTmp.shaderOpt.bloom = DPD_SHADER_OPT_BLOOM_NORMAL;
                    drawDatTmp.shaderOpt.blendState = DPD_SHADER_OPT_BLEND_STATE_ADDCOLOR;

                    DRAW_POLYGON_IN_CAMERA_EX_OPTION opt = {};
                    opt.reverse = false;
                    opt.fixed = false;
                    opt.enableShade = true;
                    opt.billboard = true;
                    ////opt.priority = DRAW_POLYGON_PRIORITY_NML_u2;
                    //opt.priority = DRAW_POLYGON_PRIORITY_NML_ALWAYS;
                    ////opt.depthBuffIgnore = true;

                    opt.priority = DRAW_POLYGON_PRIORITY_NML_u1;
                    opt.depthBuffIgnore = true;

                    DrawPolygonInCameraEx(drawDatTmp, opt);

                    //{
                    //    DRAW_POLYGON_DAT drawDatTmp2 = drawDatTmp;
                    //    //drawDatTmp2.pos.x *= 0.55f;
                    //    //drawDatTmp2.pos.y *= 0.55f;
                    //    drawDatTmp2.pos.x *= 0.75f;
                    //    drawDatTmp2.pos.y *= 0.75f;
                    //    drawDatTmp2.size = MkF3(
                    //        1800.0f * beamMotionStartMod * beamEndMod,
                    //        1800.0f * beamMotionStartMod * beamEndMod,
                    //        1.0f);
                    //    drawDatTmp2.texNo = m_Tex_EnergyBall;
                    //    drawDatTmp2.texUV = AnimationUV(m_Obj->frameActive, 4, 4);
                    //    drawDatTmp2.texWH = { 1.0f / 4, 1.0f / 4 };
                    //    drawDatTmp2.color = MkF4(1.0f, 1.0f, 1.0f, 1.0f);
                    //    DRAW_POLYGON_IN_CAMERA_EX_OPTION opt2 = opt;
                    //    //opt2.priority = DRAW_POLYGON_PRIORITY_NML_u3;
                    //    opt2.priority = DRAW_POLYGON_PRIORITY_NML_d1;
                    //    //opt2.depthBuffIgnore = false;
                    //    
                    //    DrawPolygonInCameraEx(drawDatTmp2, opt2);
                    //}
                }

                //continue;

                //int beamSplitMax = 1;
                //for (int i = 0; i < beamSplitMax; i++)
                {
                    DRAW_POLYGON_DAT drawDatBaseTmp = {};
                    //drawDatBaseTmp.size = MkF3(50.0f, 50.0f, 1800.0f);
                    //drawDatBaseTmp.size = MkF3(1.0f, 1.0f, 1.0f);
                    //drawDatBaseTmp.size = MkF3(50.0f, 50.0f, 1000.0f);
                    //drawDatBaseTmp.size = MkF3(
                    //    60.0f,
                    //    60.0f,
                    //    4000.0f);
                    drawDatBaseTmp.size = MkF3(
                        80.0f,
                        80.0f,
                        4000.0f
                    );

                    //drawDatBaseTmp.pos.z += -(drawDatBaseTmp.size.z + 0.0f) * i + 1500.0f;
                    drawDatBaseTmp.pos.z += -addOffsetZ;
                    float sizeZScale = 1.0f;
                    if (m_BeamIsOutLine)
                    {
                        float beforez = drawDatBaseTmp.size.z;
                        drawDatBaseTmp.size.z = abs(
                            mainPosition.z - drawDatBaseTmp.pos.z
                        ) / 4.0f + 500.0f;
                        sizeZScale = drawDatBaseTmp.size.z / beforez;
                        //drawDatBaseTmp.pos.z += 2000.0f / sizeZScale;
                        drawDatBaseTmp.pos.z += -500.0f;
                    }
                    drawDatBaseTmp.pos.z += 1000.0f * sizeZScale;

                    drawDatBaseTmp.rot = 0.0f;

                    drawDatBaseTmp.rot3D = MkF3(0.0f, 0.0f, 0.0f);

                    //drawDatTmp.color = MkF4(1.0f, 0.8f, 1.0f, 0.9f);
                    //drawDatBaseTmp.color = MkF4(1.0f * 2, 0.9f * 2, 1.0f * 2, 0.9f);
                    drawDatBaseTmp.color = MkF4(1.0f, 0.9f, 1.0f, 0.8f);
                    //drawDatTmp.color = MkF4(3.0f, 1.0f, 7.0f, 0.9f);

                    //drawDatTmp.texNo = LoadTexture("rom/texture/ui/in_game/charactor_icon.png");
                    drawDatBaseTmp.texNo = 0;
                    drawDatBaseTmp.loadTexType = LOADTEXTURETYPE_MAIN;

                    drawDatBaseTmp.texUV = { 0.0f, 0.0f };
                    drawDatBaseTmp.texWH = { 1.0f, 1.0f };

                    drawDatBaseTmp.startPos3D = mainPosition;
                    drawDatBaseTmp.startPos3D.z += -500.0f;

                    //drawDatBaseTmp.startRot = j * 90.0f + m_BeamRot;
                    drawDatBaseTmp.startRot = j * (360.0f / m_BeamNum) + m_BeamRot;

                    //drawDatBaseTmp.startRot = j * 45.0f;
                    //drawDatBaseTmp.startRot = m_CurrentAxisEx * 45.0f;
                    drawDatBaseTmp.startRot3D = MkF3(90.0f, 0.0f, 0.0f);

                    drawDatBaseTmp.startScale = MkF2(1.0f, 1.0f);
                    drawDatBaseTmp.startScale3D = MkF3(1.0f, 1.0f, 1.0f);

                    drawDatBaseTmp.modelNo = m_Model_Beam;

                    //drawDatBaseTmp.shaderOpt.bloom = DPD_SHADER_OPT_BLOOM_NORMAL;
                    drawDatBaseTmp.shaderOpt.blendState = DPD_SHADER_OPT_BLEND_STATE_ADDCOLOR;

                    DRAW_POLYGON_IN_CAMERA_EX_OPTION optBaseTmp = {};
                    optBaseTmp.reverse = false;
                    optBaseTmp.fixed = false;
                    optBaseTmp.enableShade = true;
                    optBaseTmp.priority = DRAW_POLYGON_PRIORITY_NML_u3;
                    //opt.priority = DRAW_POLYGON_PRIORITY_NORMAL;
                    optBaseTmp.depthBuffIgnore = true;
                    //opt.priority = DRAW_POLYGON_PRIORITY_FORCE_NORMAL;
                    optBaseTmp.shaderType = DPD_SHADER_TYPE_EFFECT;


                    { //予測
                        //int startbeforef = 30;
                        //constexpr int startbeforef = FRAME_BEAM_SHOOT_START - FRAME_BEAM_SHOOT_START_BEFORE;
                        constexpr int startbeforef = FRAME_BEAM_SHOOT_START - 30;
                        constexpr int startbeforefmax = 40;
                        if (m_BeamFrame >= beamShootStartFrame - startbeforef
                            && m_BeamFrame <= beamShootStartFrame)
                        {
                            float startMod = EasingInOutElastic(
                                std::min(
                                    (float)(beamShootFrame + startbeforef) / startbeforefmax, 1.0f)
                            );

                            DRAW_POLYGON_DAT drawDatTmp2 = drawDatBaseTmp;
                            //drawDatTmp2.texNo = m_Tex_Beam1;
                            drawDatTmp2.size.x = 18.0f * startMod * beamMotionStartMod;
                            drawDatTmp2.size.y = 18.0f * startMod * beamMotionStartMod;
                            //drawDatTmp2.size.z = 10.0f * startMod;
                            drawDatTmp2.shaderOpt.blendState = DPD_SHADER_OPT_BLEND_STATE_ADDCOLOR;

                            //drawDatTmp2.color = MkF4(2.0f, 2.0f, 2.0f, 1.0f);
                            //drawDatTmp2.color = MkF4(2.0f, 0.5f, 0.5f, 0.75f);
                            drawDatTmp2.color = MkF4(1.0f, 0.0f, 0.5f, 0.75f);
                            DRAW_POLYGON_IN_CAMERA_EX_OPTION opt2 = optBaseTmp;
                            opt2.priority = DRAW_POLYGON_PRIORITY_NORMAL;
                            DrawPolygonInCameraEx(drawDatTmp2, opt2);
                        }
                    }

                    if (m_BeamFrame >= beamShootStartFrame)
                    {

                        DRAW_POLYGON_DAT drawDatTmp = drawDatBaseTmp;
                        //drawDatTmp.pos.z += -750.0f;
                        //drawDatTmp.pos.z += -750.0f;

                        Float2 scaleAddTmp = MkF2(0.0f, 0.0f);
                        Float2 scaleMulTmp = MkF2(1.0f, 1.0f);
                        {
                            //float addSizeTmp = sin((float)(m_Obj->frameActive) * 0.8f) * 0.05f;
                            float addSizeTmp = sin((float)(m_BeamFrame) * 0.8f) * 2.0f;
                            scaleAddTmp.x += addSizeTmp;
                            scaleAddTmp.y += addSizeTmp;
                            //drawDatTmp.size.z += addSizeTmp;
                        }

                        {
                            if (beamStartMod < 1.0f)
                            {
                                scaleMulTmp.x *= beamStartMod;
                                scaleMulTmp.y *= beamStartMod;
                                //drawDatTmp.size.z *= startMod;
                            }
                        }
                        {
                            if (beamEndMod < 1.0f)
                            {
                                scaleMulTmp.x *= beamEndMod;
                                scaleMulTmp.y *= beamEndMod;
                            }
                            //else
                            //{
                            //    //仮
                            //    if (beamFrame % 15 == 0)
                            //    {
                            //        CameraShakeRequestAll(CAMERA_SHAKE_MIDIUM_FRAME, CAMERA_SHAKE_LARGE_POWER);
                            //    }
                            //}
                        }
                        drawDatTmp.size.x += scaleAddTmp.x;
                        drawDatTmp.size.y += scaleAddTmp.y;
                        drawDatTmp.size.x *= scaleMulTmp.x;
                        drawDatTmp.size.y *= scaleMulTmp.y;

                        //DRAW_POLYGON_DAT drawDatTmp2 = drawDatTmp;
                        //drawDatTmp2.size.x *= 1.3f;
                        //drawDatTmp2.size.y *= 1.3f;
                        ////drawDatTmp2.size.z *= 1.3f;
                        ////drawDatTmp2.color = MkF4(1.0f, 0.3f, 1.0f, 0.6f);
                        //drawDatTmp2.color = MkF4(3.0f, 1.0f, 7.0f, 0.75f);
                        ////drawDatTmp2.color = MkF4(3.0f * 2, 1.0f * 2, 7.0f * 2, 0.75f);
                        //DRAW_POLYGON_IN_CAMERA_EX_OPTION opt2 = optBaseTmp;
                        //opt2.priority = DRAW_POLYGON_PRIORITY_NML_u1;
                        ////opt2.priority = DRAW_POLYGON_PRIORITY_NML_d1;

                        DRAW_POLYGON_DAT drawDatTmp2 = drawDatTmp;
                        drawDatTmp2.size.x *= 3.2f;
                        drawDatTmp2.size.y *= 3.2f;
                        drawDatTmp2.shaderOpt.blendState = DPD_SHADER_OPT_BLEND_STATE_DEFAULT;
                        drawDatTmp2.color = MkF4(0.25f, 0.1f, 0.45f, 0.6f);
                        DRAW_POLYGON_IN_CAMERA_EX_OPTION opt2 = optBaseTmp;
                        opt2.priority = DRAW_POLYGON_PRIORITY_NML_d2;

                        {
                            DRAW_POLYGON_DAT drawDatTmp3 = drawDatTmp;
                            drawDatTmp3.texNo = m_Tex_Beam1;
                            drawDatTmp3.size.x *= 2.4f;
                            drawDatTmp3.size.y *= 2.4f;
                            drawDatTmp3.color = MkF4(2.0f, 0.5f, 1.6f, 0.75f);
                            drawDatTmp3.texUV = {
                                0.0f + 0.01f * (float)(m_BeamFrame / 1),
                                0.0f + 0.05f * (float)(m_BeamFrame / 1)
                            };
                            drawDatTmp3.texWH = {
                                3.0f,
                                5.0f * sizeZScale
                            };
                            drawDatTmp3.shaderOpt.blendState = DPD_SHADER_OPT_BLEND_STATE_DEFAULT;
                            DRAW_POLYGON_IN_CAMERA_EX_OPTION opt3 = optBaseTmp;
                            opt3.priority = DRAW_POLYGON_PRIORITY_NML_u2;
                            DrawPolygonInCameraEx(drawDatTmp3, opt3);
                        }
                        {
                            DRAW_POLYGON_DAT drawDatTmp3 = drawDatTmp;
                            drawDatTmp3.texNo = m_Tex_Beam2;
                            drawDatTmp3.size.x *= 1.7f;
                            drawDatTmp3.size.y *= 1.7f;
                            drawDatTmp3.color = MkF4(3.0f, 1.0f, 7.0f, 0.75f);
                            drawDatTmp3.texUV = {
                                0.0f + 0.06f * (float)(m_BeamFrame / 1),
                                0.0f + 0.05f * (float)(m_BeamFrame / 1)
                            };
                            drawDatTmp3.texWH = {
                                1.0f,
                                3.0f * sizeZScale
                            };
                            drawDatTmp3.shaderOpt.blendState = DPD_SHADER_OPT_BLEND_STATE_DEFAULT;
                            DRAW_POLYGON_IN_CAMERA_EX_OPTION opt3 = optBaseTmp;
                            opt3.priority = DRAW_POLYGON_PRIORITY_NORMAL;
                            DrawPolygonInCameraEx(drawDatTmp3, opt3);
                        }
                        {
                            DRAW_POLYGON_DAT drawDatTmp3 = drawDatTmp;
                            drawDatTmp3.texNo = m_Tex_Beam3;
                            drawDatTmp3.size.x *= 3.5f;
                            drawDatTmp3.size.y *= 3.5f;
                            drawDatTmp3.color = MkF4(0.13f * 2, 0.f, 0.5f * 2, 0.75f);
                            drawDatTmp3.texUV = {
                                0.0f,
                                0.0f + 0.05f * (float)(m_BeamFrame / 1)
                            };
                            drawDatTmp3.texWH = {
                                1.0f,
                                4.5f * sizeZScale
                            };
                            drawDatTmp3.shaderOpt.blendState = DPD_SHADER_OPT_BLEND_STATE_DEFAULT;
                            DRAW_POLYGON_IN_CAMERA_EX_OPTION opt3 = optBaseTmp;
                            opt3.priority = DRAW_POLYGON_PRIORITY_NML_d1;
                            DrawPolygonInCameraEx(drawDatTmp3, opt3);
                        }



                        //DRAW_POLYGON_DAT drawDatBaseTmp2 = drawDatBaseTmp;
                        //{
                        //    float buft = drawDatBaseTmp2.size.y;
                        //    drawDatBaseTmp2.size.y = drawDatBaseTmp2.size.z * 3.0f;
                        //    drawDatBaseTmp2.size.z = buft;
                        //}
                        ////drawDatBaseTmp2.pos = {};
                        //drawDatBaseTmp2.pos.y = drawDatBaseTmp2.size.z / 2.0f;
                        //drawDatBaseTmp2.pos.z = -drawDatBaseTmp2.size.y / 2.0f * 1.25f;
                        //drawDatBaseTmp2.pos.z += 300.0f;
                        //drawDatBaseTmp2.rot3D = MkF3(-90.0f, 0.0f, 0.0f);
                        //{
                        //    float modtmp = 3.0f;
                        //    drawDatBaseTmp2.pos.y += scaleAddTmp.y;
                        //    drawDatBaseTmp2.pos.y *= modtmp * scaleMulTmp.y;

                        //    drawDatBaseTmp2.size.x += scaleAddTmp.x;
                        //    drawDatBaseTmp2.size.z += scaleAddTmp.y;
                        //    drawDatBaseTmp2.size.x *= modtmp * scaleMulTmp.x;
                        //    drawDatBaseTmp2.size.z *= modtmp * scaleMulTmp.y;
                        //}

                        ////drawDatBaseTmp2.size.x *= 1.0f / 2;
                        ////drawDatBaseTmp2.size.z *= 1.0f / 2;

                        //drawDatBaseTmp2.modelNo = 0;

                        ////int cylinderNum = 8;
                        ////for (int k = 0; k < cylinderNum; k++)
                        //{
                        //    DRAW_POLYGON_DAT drawDatBaseTmp2Copy = drawDatBaseTmp2;
                        //    //drawDatBaseTmp2Copy.startRot3D.y += 45.0f * k;

                        //    DRAW_POLYGON_DAT drawDatTmp3 = drawDatBaseTmp2Copy;
                        //    drawDatTmp3.texNo = m_Tex_Beam2;
                        //    //drawDatTmp3.texUV = {
                        //    //    1.0f / cylinderNum * k,
                        //    //    0.0f + (1.0f / beamSplitMax) * i + -0.03f * (float)(m_BeamFrame / 1)
                        //    //};
                        //    //drawDatTmp3.texWH = {
                        //    //    1.0f / cylinderNum ,
                        //    //    2.0f
                        //    //};
                        //    drawDatTmp3.texUV = {
                        //        1.0f,
                        //        0.0f + -0.03f * (float)(m_BeamFrame / 1)
                        //    };
                        //    drawDatTmp3.texWH = {
                        //        1.0f,
                        //        2.0f
                        //    };
                        //    {
                        //        float modtmp = 1.8f;
                        //        drawDatTmp3.pos.y *= modtmp;
                        //        drawDatTmp3.size.x *= modtmp;
                        //        drawDatTmp3.size.z *= modtmp;
                        //    }
                        //    //drawDatTmp3.size.z *= 1.8f;
                        //    //drawDatTmp3.color = MkF4(2.0f, 1.0f, 7.0f, 0.75f);
                        //    drawDatTmp3.color = MkF4(1.6f, 0.5f, 2.0f, 0.75f);
                        //    DRAW_POLYGON_IN_CAMERA_EX_OPTION opt3 = optBaseTmp;
                        //    opt3.priority = DRAW_POLYGON_PRIORITY_NML_u1;


                        //    DRAW_POLYGON_DAT drawDatTmp4 = drawDatBaseTmp2Copy;
                        //    drawDatTmp4.texNo = m_Tex_Beam1;
                        //    //drawDatTmp4.texUV = {
                        //    //    1.0f / cylinderNum * k,
                        //    //    0.0f + (1.0f / beamSplitMax) * i + -0.03f * (float)(m_BeamFrame / 1)
                        //    //};
                        //    //drawDatTmp4.texWH = {
                        //    //    1.0f / cylinderNum,
                        //    //    2.0f
                        //    //};
                        //    drawDatTmp4.texUV = {
                        //        1.0f,
                        //        0.0f + -0.03f * (float)(m_BeamFrame / 1)
                        //    };
                        //    drawDatTmp4.texWH = {
                        //        1.0f,
                        //        2.0f
                        //    };
                        //    {
                        //        float modtmp = 3.3f;
                        //        drawDatTmp4.pos.y *= modtmp;
                        //        drawDatTmp4.size.x *= modtmp;
                        //        drawDatTmp4.size.z *= modtmp;
                        //    }
                        //    //drawDatTmp4.size.z *= 3.3f;
                        //    //drawDatTmp4.color = MkF4(3.0f, 1.0f, 7.0f, 0.5f);
                        //    drawDatTmp4.color = MkF4(2.0f, 0.5f, 1.6f, 0.75f);
                        //    DRAW_POLYGON_IN_CAMERA_EX_OPTION opt4 = optBaseTmp;
                        //    opt4.priority = DRAW_POLYGON_PRIORITY_NORMAL;


                        //    DRAW_POLYGON_DAT drawDatTmp5 = drawDatBaseTmp2Copy;
                        //    drawDatTmp5.texNo = m_Tex_Beam3;
                        //    //drawDatTmp5.texUV = {
                        //    //    1.0f / cylinderNum * k,
                        //    //    0.0f + (1.0f / beamSplitMax) * i + -0.03f * (float)(m_BeamFrame / 1)
                        //    //};
                        //    //drawDatTmp5.texWH = {
                        //    //    1.0f / cylinderNum,
                        //    //    2.0f
                        //    //};
                        //    drawDatTmp5.texUV = {
                        //        1.0f,
                        //        0.0f + -0.03f * (float)(m_BeamFrame / 1)
                        //    };
                        //    drawDatTmp5.texWH = {
                        //        1.0f,
                        //        2.0f
                        //    };
                        //    {
                        //        float modtmp = 4.8f;
                        //        drawDatTmp5.pos.y *= modtmp;
                        //        drawDatTmp5.size.x *= modtmp;
                        //        drawDatTmp5.size.z *= modtmp;
                        //    }
                        //    //drawDatTmp5.size.z *= 4.8f;
                        //    drawDatTmp5.color = MkF4(0.13f * 2, 0.f, 0.5f * 2, 0.75f);
                        //    DRAW_POLYGON_IN_CAMERA_EX_OPTION opt5 = optBaseTmp;
                        //    opt5.priority = DRAW_POLYGON_PRIORITY_NML_d1;


                        //    DrawPolygonInCameraEx(drawDatTmp5, opt5);
                        //    DrawPolygonInCameraEx(drawDatTmp4, opt4);
                        //    DrawPolygonInCameraEx(drawDatTmp3, opt3);
                        //}

                        DrawPolygonInCameraEx(drawDatTmp2, opt2);
                        DrawPolygonInCameraEx(drawDatTmp, optBaseTmp);
                    }
                }
            }
        }
    }
}


void GOE_Character_Enemy_Boss_Inperiusu::update_misile_object()
{
    GAME_OBJECT* targetObject = GetGameObjectFindByIdentifier(m_TargetObjIdentifier);

    //ミサイル個
    if (m_Misile_Objects.size() > 0)
    {
        int index = 0;
        for (const auto& misileObjIde : m_Misile_Objects)
        {
            GAME_OBJECT* misileObj = GetGameObjectFindByIdentifier(
                misileObjIde);
            if (misileObj != nullptr)
            {
                if (misileObj->delRequ > 0)
                {
                    continue;
                }

                //    GameObjectMoveRequest(beamObjCreated, FRAME_MISILE_LIFETIME,
                //        MkF3(
                //            beamObjCreated->pos.x + vecMisileTmp.x * ONE_BLOCK * 160.0f,
                //            beamObjCreated->pos.y + vecMisileTmp.y * ONE_BLOCK * 160.0f,
                //            beamObjCreated->pos.z + vecMisileTmp.z * ONE_BLOCK * 160.0f
                //        ),
                //        EasingInSine
                //    );
                if (misileObj->frameActive % 8 == 0)
                {
                    {
                        Float3 efPos1 = misileObj->pos;
                        efPos1.z += 0.0f;

                        GAME_OBJECT* go = GO_CreateEffect(
                            GO_EFFECT_TYPE_EXPLOSION_SMOKE, efPos1);
                        if (go != nullptr)
                        {
                            go->scale = MkF3(
                                3.4f,
                                3.4f,
                                3.4f
                            );
                            GameObjectSetLifeTime(go, 30);
                        }
                    }
                }

                if (misileObj->freeInt == MISILE_HOMING_FALL_FLAG)
                {
                    if (misileObj->frameActive == 60)
                    {
                        //misileObj->pos.x =
                        //    //-ONE_BLOCK * 100 + ONE_BLOCK * 15 * index;
                        //    -ONE_BLOCK * 100 + (ONE_BLOCK * 200 / m_Misile_Objects.size()) * ((float)index + 0.5f);

                        misileObj->pos.y =
                            -ONE_BLOCK * 60;

                        misileObj->pos.z =
                            -ONE_BLOCK * 60;
                    }
                    if (misileObj->frameActive == 120)
                    {
                        Float3 addpos = {};
                        if (m_ActCycleNum > 2)
                        {
                            addpos = NormalizeVector(targetObject->vel);
                            //addpos = targetObject->vel;
                        }

                        Float3 rpos = MkF3(
                            targetObject->pos.x + addpos.x * ONE_BLOCK * 6.0f,
                            targetObject->pos.y + addpos.y * ONE_BLOCK * 6.0f,
                            0.0f
                        );
                        //Float3 rpos = MkF3(
                        //    targetObject->pos.x + addpos.x * ONE_BLOCK * 0.5f,
                        //    targetObject->pos.y + addpos.y * ONE_BLOCK * 0.5f,
                        //    0.0f
                        //);
                        {
                            Stage_Base* stg = GetCurrentStageSceneGame();
                            if (stg != nullptr)
                            {
                                Int2 stgsize = stg->GetStageSize();

                                Float2 size = {};
                                size.x = BLOCK_WIDTH * stgsize.x * 0.8f;
                                size.y = BLOCK_HIGHT * stgsize.y * 0.8f;


                                if (rpos.y < -size.y / 2.0f)
                                {
                                    rpos.y = -size.y / 2.0f;
                                }
                                if (rpos.y > size.y / 2.0f)
                                {
                                    rpos.y = size.y / 2.0f;
                                }

                                if (rpos.x < -size.x / 2.0f)
                                {
                                    rpos.x = -size.x / 2.0f;
                                }
                                if (rpos.x > size.x / 2.0f)
                                {
                                    rpos.x = size.x / 2.0f;
                                }
                            }
                        }
                        GameObjectMoveRequest(misileObj,
                            60,
                            rpos
                        );

                    }
                    if (misileObj->frameActive == 120 + 60)
                    {
                        float sca = (misileObj->scale.x + misileObj->scale.y + misileObj->scale.z) / 3.0f;

                        //SoundEffectPlayRequest(SOUND_TYPE_GENERAL_CLASH, 0.25f, 0);
                        CameraShakeRequestAll(
                            CAMERA_SHAKE_MIDIUM_FRAME, 
                            CAMERA_SHAKE_LARGE_POWER
                        );

                        //SEを再生
                        SoundEffectPlayRequest(GetSoundData("SOUND/SE/enemy/attack/Big_Explosion"),
                            2.0f, 0, 1.2f);
                        SoundEffectPlayRequest(SOUND_TYPE_SE_9, 2.0f, 0, 1.2f);

                        {
                            Float3 efPos = misileObj->pos;
                            efPos.z += ONE_BLOCK * -2.0f;

                            GO_CreateExplosion(efPos, sca * 0.5f);
                        }
                        {
                            float angle = 360.0f;
                            int bulletnum = 4;

                            for (int i = 0; i < bulletnum; i++)
                            {
                                float radtmp = CalculateDegToRad(
                                    180.0f + (angle - 180.0f) / 4 - (angle / bulletnum * i)
                                    + (((index + 0) / 4) % 2 == 0) * 45.0f
                                );
                                {
                                    Float3 nvec = NormalizeVector(
                                        MkF3(
                                            (float)cos(radtmp) * 3.0f,
                                            (float)sin(radtmp) * 3.0f,
                                            0.0f
                                        ));

                                    GOE_Character_EnemyProjectile* goepro =
                                        GOE_Character_EnemyProjectile::CreateProjectile(
                                            GOE_Character_EnemyProjectile::Type::ExplosionShockWave,
                                            MkF3(
                                                misileObj->pos.x + nvec.x * ONE_BLOCK * 3.0f,
                                                misileObj->pos.y + nvec.y * ONE_BLOCK * 3.0f,
                                                misileObj->pos.z + nvec.z * ONE_BLOCK * 3.0f
                                            ),
                                            nvec
                                        );
                                    if (goepro != nullptr)
                                    {
                                        GAME_OBJECT* beamObj = goepro->GetGameObj();
                                        if (beamObj != nullptr)
                                        {
                                            beamObj->scale.x = 1.25f * misileObj->scale.x;
                                            beamObj->scale.y = 1.25f * misileObj->scale.y;
                                            beamObj->scale.z = 1.25f * misileObj->scale.z;
                                            {
                                                Character::MOTION_COL_DAT motionColDetails =
                                                    Character::GetMotionColDataFindByName(
                                                        Character::MOTION_COL_TYPE_ATK,
                                                        MISILE_ATK_COL_NAME
                                                    );

                                                GAME_OBJECT_COLLISION_ATTACK_CHILD attackColTmp = motionColDetails.atkCol;
                                                attackColTmp.subjectFamily[GAME_OBJECT_FAMILY_PLAYER] = true;
                                                attackColTmp.subjectFamily[GAME_OBJECT_FAMILY_GIMIK] = true;

                                                attackColTmp.size = MkF3(
                                                    ONE_BLOCK * 3,
                                                    ONE_BLOCK * 3,
                                                    ONE_BLOCK * 3
                                                );
                                                attackColTmp.pos.z = -attackColTmp.size.z / 2;
                                                attackColTmp.pos.y += attackColTmp.size.y / 2 * 1.0f;

                                                beamObj->collision.atk.enable = true;
                                                beamObj->collision.atk.collision[0] = attackColTmp;
                                                beamObj->collision.atk.collision[0].enable = true;
                                            }
                                            beamObj->blockCollisionIgnore = false;

                                            GameObjectSetLifeTime(beamObj, 120);
                                            GameObjectMoveRequest(beamObj, 120,
                                                MkF3(
                                                    beamObj->pos.x + nvec.x * ONE_BLOCK * 70.0f,
                                                    beamObj->pos.y + nvec.y * ONE_BLOCK * 70.0f,
                                                    beamObj->pos.z + nvec.z * ONE_BLOCK * 70.0f
                                                ),
                                                nullptr
                                            );
                                        }
                                    }
                                }
                            }
                        }

                        DeleteGameObject(misileObj);
                    }
                }
                else
                {
                    int addStartF = 0;
                    if (misileObj->freeInt == MISILE_OSOI_FLAG)
                    {
                        addStartF = 50;
                    }

                    if (misileObj->frameActive == (60 + addStartF))
                    {
                        misileObj->pos.x =
                            //-ONE_BLOCK * 100 + ONE_BLOCK * 15 * index;
                            -ONE_BLOCK * 100 + (ONE_BLOCK * 200 / m_Misile_Objects.size()) * ((float)index + 0.5f);

                        float addY = 0;
                        if (misileObj->freeInt == MISILE_OSOI_FLAG)
                        {
                            //addY = -ONE_BLOCK * 20;
                            addY = -ONE_BLOCK * 5;
                        }

                        misileObj->pos.y =
                            -ONE_BLOCK * 60 + addY;

                        GameObjectMoveRequest(misileObj,
                            160,
                            MkF3(
                                misileObj->pos.x,
                                misileObj->pos.y + ONE_BLOCK * 30.0f + addY,
                                ONE_BLOCK * -1.0f
                            ),
                            EasingInSine,
                            EasingInExpo,
                            EasingOutExpo
                        );
                    }
                    if (misileObj->frameActive == (160 + addStartF))
                    {
                        if (misileObj->freeInt == MISILE_HOMING_FLAG)
                        {
                            Float3 vecMisileTmp = NormalizeVector(MkF3(
                                targetObject->pos.x - misileObj->pos.x,
                                targetObject->pos.y - misileObj->pos.y,
                                targetObject->pos.z - misileObj->pos.z
                            ));
                            GameObjectMoveRequest(misileObj,
                                240,
                                MkF3(
                                    misileObj->pos.x + vecMisileTmp.x * ONE_BLOCK * 190.0f,
                                    misileObj->pos.y + vecMisileTmp.y * ONE_BLOCK * 190.0f,
                                    misileObj->pos.z
                                ),
                                nullptr
                            );
                        }
                        else if (misileObj->freeInt == MISILE_OSOI_FLAG)
                        {
                            GameObjectMoveRequest(misileObj,
                                //240 * 1.8f,
                                240 * 1.5f,
                                MkF3(
                                    misileObj->pos.x,
                                    misileObj->pos.y + ONE_BLOCK * 190.0f,
                                    misileObj->pos.z
                                ),
                                nullptr
                            );
                            //Float3 vecMisileTmp = NormalizeVector(
                            //    misileObj->vel);
                            //GameObjectMoveRequest(misileObj,
                            //    180,
                            //    MkF3(
                            //        vecMisileTmp.x * ONE_BLOCK * 90.0f,
                            //        vecMisileTmp.y * ONE_BLOCK * 90.0f,
                            //        misileObj->pos.z
                            //    ),
                            //    nullptr
                            //);
                        }
                        else
                        {
                            GameObjectMoveRequest(misileObj,
                                240,
                                MkF3(
                                    misileObj->pos.x,
                                    misileObj->pos.y + ONE_BLOCK * 190.0f,
                                    misileObj->pos.z
                                ),
                                nullptr
                            );
                            //Float3 vecMisileTmp = NormalizeVector(
                            //    misileObj->vel);
                            //GameObjectMoveRequest(misileObj,
                            //    180,
                            //    MkF3(
                            //        vecMisileTmp.x * ONE_BLOCK * 90.0f,
                            //        vecMisileTmp.y * ONE_BLOCK * 90.0f,
                            //        misileObj->pos.z
                            //    ),
                            //    nullptr
                            //);
                        }
                    }
                }

                //if (misileObj->frameActive >= 160)
                //{
                //    if (misileObj->freeInt == MISILE_HOMING_FLAG)
                //    {
                //        //if (misileObj->frameActive < FRAME_ONE_SECOND * 3)
                //        {
                //            if (targetObject != nullptr)
                //            {
                //                Float3 vecMisileTmp = NormalizeVector(MkF3(
                //                    targetObject->pos.x - misileObj->pos.x,
                //                    targetObject->pos.y - misileObj->pos.y,
                //                    targetObject->pos.z - misileObj->pos.z
                //                ));

                //                Float3 vtmp = misileObj->vel;
                //                vtmp.z = 0;

                //                ////vecMisileTmp.x *= 18.0f;
                //                ////vecMisileTmp.y *= 18.0f;
                //                ////vecMisileTmp.z *= 18.0f;
                //                //vecMisileTmp.x *= 0.3f;
                //                //vecMisileTmp.y *= 0.3f;
                //                //vecMisileTmp.z *= 0.3f;

                //                ////misileObj->vel = vecMisileTmp;
                //                //misileObj->vel.x += vecMisileTmp.x;
                //                //misileObj->vel.y += vecMisileTmp.y;
                //                //misileObj->vel.z += vecMisileTmp.z;

                //                SetBeforeVec(
                //                    &vtmp,
                //                    &vecMisileTmp,
                //                    0.07f
                //                );

                //                {
                //                    //vtmp = NormalizeVector(vtmp);

                //                    misileObj->vel.x += vtmp.x * 0.05f;
                //                    misileObj->vel.y += vtmp.y * 0.05f;
                //                    //misileObj->vel.z += vtmp.z * 0.2f;
                //                    //misileObj->pos.z +=
                //                    //    (misileObj->pos.z - targetObject->pos.z)
                //                    //    / 10.0f;
                //                    //misileObj->pos.z = targetObject->pos.z;
                //                }
                //            }

                //            //GameObjectMoveRequest(misileObj, 30,
                //            //    MkF3(
                //            //        misileObj->pos.x + vecMisileTmp.x * ONE_BLOCK * 30.0f,
                //            //        misileObj->pos.y + vecMisileTmp.y * ONE_BLOCK * 30.0f,
                //            //        misileObj->pos.z + vecMisileTmp.z * ONE_BLOCK * 30.0f
                //            //    ),
                //            //    nullptr
                //            //);
                //        }
                //    }
                //}
            }
            index++;
        }
    }
}

void GOE_Character_Enemy_Boss_Inperiusu::draw_misile_object()
{
    if (m_Misile_Objects.size() > 0)
    {
        for (const auto& misileObjIde : m_Misile_Objects)
        {
            GAME_OBJECT* misileObj = GetGameObjectFindByIdentifier(
                misileObjIde);
            if (misileObj != nullptr
                && (misileObj->moveRequ.enable
                    || misileObj->vel.x != 0.0f || misileObj->vel.y != 0.0f))
            {
                Float3 velNom = NormalizeVector(misileObj->vel);

                if (IsInStageGameObject(misileObj))
                {
                    DRAW_SPRITE_DAT dsdat = {};
                    dsdat.x = misileObj->pos.x;
                    dsdat.y = misileObj->pos.y;
                    dsdat.z = -8.0f;
                    //dsdat.z = -0.005f;
                    dsdat.w = 200.0f * misileObj->scale.x;
                    dsdat.h = 200.0f * misileObj->scale.y;

                    {
                        Float3 addposvec = velNom;

                        Float3 addpos = {};
                        addpos.x = dsdat.w / 2.0f * addposvec.x;
                        addpos.y = dsdat.w / 2.0f * addposvec.y;
                        addpos.z = dsdat.w / 2.0f * addposvec.z;

                        dsdat.x += addpos.x;
                        dsdat.y += addpos.y;
                    }

                    {
                        float mo = 1.0f - std::max(std::min(abs(misileObj->pos.z) / (ONE_BLOCK * 80.0f), 1.0f), 0.0f);
                        dsdat.w *= mo;
                        dsdat.h *= mo;
                    }

                    dsdat.rot = 0.0f;
                    dsdat.color = { 1.0f, 1.0f, 1.0f, 0.5f };

                    dsdat.texNo = m_Tex_Shadow.textureId;
                    dsdat.loadTexType = LOADTEXTURETYPE_MAIN;
                    //dsdat.shaderOpt.blendState = DPD_SHADER_OPT_BLEND_STATE_ADDCOLOR;

                    dsdat.texUV = { 0.0f, 0.0f };
                    dsdat.texWH = { 1.0f, 1.0f };
                    dsdat.startPos = { 0.0f, 0.0f };
                    dsdat.startRot = 0.0f;
                    dsdat.startScale = { 1.0f, 1.0f };

                    DrawSpriteInCamera(dsdat, false, false,
                        true
                    );
                }

                {
                    //drawDatTmp.startRot = CalculateRadToDeg(atan2(misileObj->vel.y, misileObj->vel.x)) + 90.0f;

                    //float rottmp = CalculateRadToDeg(atan2(misileObj->vel.y, misileObj->vel.x)) + 180.0f + 45.0f;

                    DRAW_POLYGON_DAT drawDatBaseTmp = {};
                    drawDatBaseTmp.size = MkF3(
                        80.0f * misileObj->scale.x,
                        80.0f * misileObj->scale.y,
                        80.0f * misileObj->scale.z
                    );

                    //drawDatBaseTmp.pos.z = drawDatBaseTmp.size.z / 2;
                    drawDatBaseTmp.pos.y += drawDatBaseTmp.size.y * 3.0f;

                    drawDatBaseTmp.color = MkF4(1.0f, 1.0f, 1.0f, 1.0f);
                    //drawDatBaseTmp.color.x += 8.0f * (1.0f - sin((float)misileObj->frame * 0.7f));
                    //drawDatBaseTmp.color.x += 8.0f * (sin((float)misileObj->frame * 0.7f) / 2.0f + 0.5f);
                    drawDatBaseTmp.color.x += 8.0f * (sin((float)misileObj->frame / 8) / 2.0f + 0.5f);
                    drawDatBaseTmp.color.y = drawDatBaseTmp.color.x;
                    drawDatBaseTmp.color.z = drawDatBaseTmp.color.x;

                    drawDatBaseTmp.texNo = m_Tex_Misile_Model;
                    drawDatBaseTmp.loadTexType = LOADTEXTURETYPE_MAIN;

                    drawDatBaseTmp.texUV = { 0.0f, 0.0f };
                    drawDatBaseTmp.texWH = { 1.0f, 1.0f };

                    drawDatBaseTmp.startPos3D = misileObj->pos;
                    drawDatBaseTmp.startRot3D.x =
                        //90.0f + 45.0f * velNom.z;
                        0.0f + 45.0f * velNom.z;
                    drawDatBaseTmp.startRot3D.z = CalculateRadToDeg(atan2(
                        velNom.y, velNom.x)) - 90.0f;

                    //発射時
                    if (misileObj->frameActive < 60)
                    {
                        drawDatBaseTmp.startRot3D = {
                            -90.0f, 0.0f, 0.0f
                        };
                        //drawDatBaseTmp.rot3D.z = 0.0f;
                    }

                    //drawDatBaseTmp.startPos3D.z += -80.0f;

                    //drawDatBaseTmp.startRot = CalculateRadToDeg(atan2(m_InitVel.y, m_InitVel.x)) - 90.0f;
                    //drawDatBaseTmp.startRot3D = MkF3(90.0f - 45.0f * m_InitVel.z, 0.0f, 0.0f);

                    drawDatBaseTmp.startScale = MkF2(1.0f, 1.0f);
                    drawDatBaseTmp.startScale3D = MkF3(1.0f, 1.0f, 1.0f);

                    drawDatBaseTmp.modelNo = m_Model_Misile;
                    //drawDatBaseTmp.modelNo = m_Model_Beam;

                    drawDatBaseTmp.shaderOpt.bloom = DPD_SHADER_OPT_BLOOM_ALWAYS;
                    drawDatBaseTmp.shaderOpt.outline = DPD_SHADER_OPT_OUTLINE_NORMAL;
                    //drawDatBaseTmp.shaderOpt.outlineLayer = 1;
                    //drawDatBaseTmp.shaderOpt.blendState = DPD_SHADER_OPT_BLEND_STATE_ADDCOLOR;

                    DRAW_POLYGON_IN_CAMERA_EX_OPTION optBaseTmp = {};
                    optBaseTmp.reverse = false;
                    optBaseTmp.fixed = false;
                    optBaseTmp.priority = DRAW_POLYGON_PRIORITY_NORMAL;
                    optBaseTmp.shaderType = DPD_SHADER_TYPE_METAL;

                    {
                        DRAW_POLYGON_DAT drawDatTmp = drawDatBaseTmp;
                        drawDatTmp.texNo = m_Tex_Misile_Fire_Model;
                        drawDatTmp.modelNo = m_Model_Fire_Misile;
                        drawDatTmp.pos.y += drawDatBaseTmp.size.y * -6.0f;
                        drawDatTmp.texUV.y += 0.1f * m_Obj->frameActive;
                        drawDatTmp.shaderOpt.blendState = DPD_SHADER_OPT_BLEND_STATE_ADDCOLOR;
                        drawDatTmp.shaderOpt.outline = DPD_SHADER_OPT_OUTLINE_NONE;
                        DRAW_POLYGON_IN_CAMERA_EX_OPTION optTmp = optBaseTmp;
                        optTmp.shaderType = DPD_SHADER_TYPE_NONE;
                        DrawPolygonInCameraEx(drawDatTmp, optTmp);
                    }
                    DrawPolygonInCameraEx(drawDatBaseTmp, optBaseTmp);

               /*     {
                        DRAW_POLYGON_DAT drawDat2Tmp = drawDatBaseTmp;
                        drawDat2Tmp.size = MkF3(
                            20.0f * misileObj->scale.x,
                            20.0f * misileObj->scale.y,
                            150.0f * misileObj->scale.z
                        );
                        drawDat2Tmp.modelNo = m_Model_Beam;
                        drawDat2Tmp.pos.z = drawDat2Tmp.size.z * -2.5f * misileObj->scale.z;
                        drawDat2Tmp.texNo = m_Tex_Misile_Fire;
                        drawDat2Tmp.texUV = AnimationUV(
                            misileObj->frameActive / 2,
                            4,
                            4
                        );
                        drawDat2Tmp.texWH = {
                            +1.0f / 4,
                            -1.0f / 4
                        };

                        DRAW_POLYGON_IN_CAMERA_EX_OPTION opt2Tmp = optBaseTmp;
                        DrawPolygonInCameraEx(drawDat2Tmp, opt2Tmp);
                    }*/

                    //DRAW_POLYGON_DAT drawDatTmp = {};
                    //drawDatTmp.size = MkF3(800.0f, 800.0f, 1.0f);

                    //drawDatTmp.rot3D = MkF3(0.0f, 0.0f, 0.0f);

                    //drawDatTmp.color = MkF4(1.0f, 1.0f, 1.0f, 0.9f);

                    ////drawDatTmp.texNo = m_Tex_Misile[
                    ////    (int)(rottmp)
                    ////        % 360
                    ////        / 90
                    ////        + 1
                    ////];
                    //drawDatTmp.loadTexType = LOADTEXTURETYPE_MAIN;


                    //drawDatTmp.startPos3D = misileObj->pos;
                    //drawDatTmp.startPos3D.z += -250.0f;

                    //drawDatTmp.startRot3D = MkF3(50.0f, 0.0f, 0.0f);

                    //drawDatTmp.startScale = MkF2(1.0f, 1.0f);
                    //drawDatTmp.startScale3D = MkF3(1.0f, 1.0f, 1.0f);

                    //drawDatTmp.modelNo = 0;

                    //drawDatTmp.shaderOpt.bloom = DPD_SHADER_OPT_BLOOM_NORMAL;

                    //drawDatTmp.rot = (float)((int)(rottmp) % 45) - 45.0f;
                    //drawDatTmp.texNo = m_Tex_Misile_Main;
                    //drawDatTmp.texUV = AnimationUV(
                    //    (int)(rottmp + 180.0f)
                    //    % 360
                    //    / 45,
                    //    m_AnimCount_Misile_Main.x,
                    //    m_AnimCount_Misile_Main.y
                    //);
                    //drawDatTmp.texWH = {
                    //    1.0f / m_AnimCount_Misile_Main.x,
                    //    1.0f / m_AnimCount_Misile_Main.y
                    //};

                    //DRAW_POLYGON_IN_CAMERA_EX_OPTION opt = {};
                    //opt.reverse = false;
                    //opt.fixed = false;
                    //opt.enableShade = true;
                    //opt.billboard = false;
                    //opt.priority = DRAW_POLYGON_PRIORITY_NORMAL;

                    //DRAW_POLYGON_DAT drawDatTmp2 = drawDatTmp;
                    //drawDatTmp2.color = MkF4(-1.0f, -1.0f, -1.0f, 0.5f);
                    //drawDatTmp2.loadTexType = LOADTEXTURETYPE_GRAY;
                    //drawDatTmp2.startPos3D.z = 0.01f;
                    //drawDatTmp2.startRot3D = MkF3(0.0f, 0.0f, 0.0f);
                    //DrawPolygonInCameraEx(drawDatTmp2, opt);

                    //{
                    //    //DRAW_POLYGON_DAT drawDatTmp3 = drawDatTmp;
                    //    //drawDatTmp3.rot = rottmp + 45.0f;
                    //    //drawDatTmp3.texNo = m_Tex_Misile_Fire;
                    //    ////drawDatTmp3.loadTexType = LOADTEXTURETYPE_GRAY;
                    //    //drawDatTmp3.texUV = AnimationUV(
                    //    //    misileObj->frameActive / 2,
                    //    //    m_AnimCount_Misile_Fire.x,
                    //    //    m_AnimCount_Misile_Fire.y
                    //    //);
                    //    //drawDatTmp3.texWH = {
                    //    //    1.0f / m_AnimCount_Misile_Fire.x,
                    //    //    1.0f / m_AnimCount_Misile_Fire.y
                    //    //};
                    //    //drawDatTmp3.size.x *= 0.6f;
                    //    //drawDatTmp3.size.y *= 0.6f;
                    //    //drawDatTmp3.startPos3D.z += 0.01f;
                    //    //drawDatTmp3.startPos3D.y += -0.01f;
                    //    //drawDatTmp3.startPos3D.x +=
                    //    //    cos(CalculateDegToRad(drawDatTmp3.rot - 90.0f)) * drawDatTmp3.size.x / 2;
                    //    //drawDatTmp3.startPos3D.y +=
                    //    //    sin(CalculateDegToRad(drawDatTmp3.rot - 90.0f)) * drawDatTmp3.size.y / 2;
                    //    //DrawPolygonInCameraEx(drawDatTmp3, opt);
                    //    DRAW_POLYGON_DAT drawDatTmp3 = drawDatTmp;
                    //    drawDatTmp3.rot = rottmp + 45.0f;
                    //    drawDatTmp3.texNo = m_Tex_Misile_Fire;
                    //    //drawDatTmp3.loadTexType = LOADTEXTURETYPE_GRAY;
                    //    drawDatTmp3.texUV = AnimationUV(
                    //        misileObj->frameActive / 2,
                    //        m_AnimCount_Misile_Fire.x,
                    //        m_AnimCount_Misile_Fire.y
                    //    );
                    //    drawDatTmp3.texWH = {
                    //        1.0f / m_AnimCount_Misile_Fire.x,
                    //        1.0f / m_AnimCount_Misile_Fire.y
                    //    };
                    //    drawDatTmp3.size.x *= 0.7f;
                    //    drawDatTmp3.size.y *= 0.7f;
                    //    //drawDatTmp3.startPos3D.y += -0.1f;
                    //    //drawDatTmp3.startPos3D.z += 0.1f;
                    //    drawDatTmp3.pos.x +=
                    //        cos(CalculateDegToRad(drawDatTmp3.rot - 90.0f)) 
                    //        * drawDatTmp3.size.x * 0.4f;
                    //    drawDatTmp3.pos.y +=
                    //        sin(CalculateDegToRad(drawDatTmp3.rot - 90.0f)) 
                    //        * drawDatTmp3.size.y * 0.4f;
                    //    drawDatTmp3.pos.z += 10.0f;
                    //    DrawPolygonInCameraEx(drawDatTmp3, opt);
                    //}

                    //DrawPolygonInCameraEx(drawDatTmp, opt);
                }
            }
        }
    }
}

void GOE_Character_Enemy_Boss_Inperiusu::misile_object_delete()
{

    for (const auto& misileObjIde : m_Misile_Objects)
    {
        GAME_OBJECT* misileObj = GetGameObjectFindByIdentifier(
            misileObjIde);
        if (misileObj != nullptr)
        {
            DeleteGameObject(misileObj);
        }
    }

    m_Misile_Objects.clear();
    m_Misile_Objects.shrink_to_fit();
}

void GOE_Character_Enemy_Boss_Inperiusu::update_misile()
{
    m_MisileMeteorFlag = m_HardPhase2Flag && m_ActCycleNum % 2 == 0;

    if (m_MisileFrame < FRAME_MISILE_AFTER)
    {
        //ヒットストップアーマー
        m_Obj->status.hitStopArmorFrame = FRAME_MISILE_AFTER;
    }

    GAME_OBJECT* targetObject = GetGameObjectFindByIdentifier(
        m_TargetObjIdentifier);
    if (targetObject != nullptr)
    {
        //ミサイル
        //constexpr unsigned int misileOneCycle = (250);
        //unsigned int misileF = m_MisileFrame % misileOneCycle;

        if (m_MisileFrame == 0)
        {
            if (m_MisileMeteorFlag
                //&& CalculateDistance(spos, m_Obj->pos) > ONE_BLOCK * 8
            )
            {
                Float3 spos = MkF3(
                    0.0f,
                    0.0f + ONE_BLOCK * -60.0f,
                    0.0f + ONE_BLOCK * -2.0f
                );

                StartWarp(spos, 0.0f, 25, true);
            }
            else
            {
                StartTurn(MkF2(0.0f, 1.0f), 0.05f);
            }
        }

        if (m_MisileFrame == 1)
        {
            //SEを再生
            SoundEffectPlayRequest(GetSoundData("SOUND/SE/inperiusu/b_kamae"), 1.5f, 0);

            //再生
            StartAnim(AnimState::Misile,
                //FRAME_MISILE_AFTER - FRAME_MISILE_START,
                FRAME_MISILE_AFTER - FRAME_MISILE_START + 80,
                true);


            if (m_MisileMeteorFlag)
            {
            }
            else
            {
                //SetCameraModeAll(CAMERA_MODE_BOSS_CHASE_WIDE);
            }
            SetCameraModeAll(CAMERA_MODE_BOSS_CHASE_WIDEMIDDLE);


            misile_object_delete();
        }
        //if (m_MisileFrame == FRAME_MISILE_START + 1)
        //{
        //    StartAnim(AnimState::Misile,
        //        FRAME_MISILE_AFTER,
        //        true);
        //}

        //if (m_MisileFrame == FRAME_MISILE_START - 20)
        //{
        //    //SEを再生
        //    SoundEffectPlayRequest(
        //        GetSoundData("SOUND/SE/enemy/attack/misile_move"),
        //        2.0f, 0, 0.65f);
        //}
        if (m_MisileFrame == FRAME_MISILE_START)
        {
            //SEを再生
            SoundEffectPlayRequest(
                GetSoundData("SOUND/SE/enemy/attack/misile_move"),
                2.5f, 0, 0.8f);
        }

        int misrate = 12;
        if (m_IsHard)
        {
            misrate = 6;
        }
        //int misnum = (FRAME_MISILE_AFTER - FRAME_MISILE_START) / misrate;

        if (m_MisileFrame >= FRAME_MISILE_START 
            && m_MisileFrame < FRAME_MISILE_AFTER
        )
        {
            //ヒットストップアーマー
            //m_Obj->status.hitStopArmorFrame = 200;

            //int misileVal = (m_MisileFrame / misrate) % 4;
            if (m_MisileFrame % misrate == 0)
            {
                Float3 vecMisileTmp = {};
                vecMisileTmp.x = sin(m_Obj->frameActive);
                vecMisileTmp.y = cos(m_Obj->frameActive);

                //SoundEffectPlayRequest(
                //    GetSoundData("SOUND/SE/enemy/attack/granade"), 
                //    1.5f, 0, 1.3f);
                CameraShakeRequestAll(CAMERA_SHAKE_MIDIUM_FRAME, CAMERA_SHAKE_MIDIUM_POWER);

                GAME_OBJECT beamObj = GetGameObjectPrefab(GAME_OBJECT_TYPE_DUMMY);
                beamObj.pos = m_Obj->pos;
                //beamObj.pos.x += vecMisileTmp.x * ONE_BLOCK * 3.0f;
                //beamObj.pos.y += vecMisileTmp.y * ONE_BLOCK * 3.0f;
                //beamObj.pos.z += vecMisileTmp.z * ONE_BLOCK * 3.0f;
                //beamObj.pos.x += m_BeforeVec.x * ONE_BLOCK * -3.0f;
                //beamObj.pos.y += m_BeforeVec.y * ONE_BLOCK * -3.0f;
                //beamObj.pos.z += ONE_BLOCK * -4.0f;
                //beamObj.pos.x += vecMisileTmp.x * ONE_BLOCK * 8.0f;
                //beamObj.pos.y += vecMisileTmp.y * ONE_BLOCK * 8.0f;
                beamObj.pos.x += ONE_BLOCK * 6.0f
                    * ((m_MisileFrame / misrate) % 2 == 0 ? -1 : 1);
                beamObj.pos.z += ONE_BLOCK * -10.0f;
                {
                    Float3 efPos1 = beamObj.pos;
                    efPos1.z += ONE_BLOCK * -1.0f;

                    GAME_OBJECT* go = GO_CreateEffect(GO_EFFECT_TYPE_EXPLOSION_2, efPos1);
                    if (go != nullptr)
                    {
                        go->scale = MkF3(
                            5.0f,
                            5.0f,
                            5.0f
                        );
                        GameObjectSetLifeTime(go, 30);
                    }
                }

                ////SEを再生
                //SoundEffectPlayRequest(
                //    GetSoundData("SOUND/SE/enemy/attack/misile_shoot"),
                //    3.0f, 0, 0.65f);
                
                //if (m_IsHard)
                //{
                //}
                //else
                //{
                //}
                
                ////SEを再生
                //SoundEffectPlayRequest(
                //    GetSoundData("SOUND/SE/enemy/attack/misile_shoot_single" + std::to_string(misileVal)),
                //    1.75f, 0, 0.7f);


                if (m_MisileFrame % (misrate * 2) == 0)
                {
                    //SEを再生
                    SoundEffectPlayRequest(
                        GetSoundData("SOUND/SE/enemy/attack/misile_shoot"),
                        3.2f, 0, 0.65f);

                    if (m_IsHard)
                    {
                        if (m_HardPhase2Flag)
                        {
                            //if (m_ActCycleNum > 2)
                            if (m_MisileMeteorFlag)
                            {
                                beamObj.freeInt = MISILE_HOMING_FALL_FLAG;
                            }
                            else
                            {
                                beamObj.freeInt = MISILE_HOMING_FLAG;
                            }
                        }
                        else
                        {
                            beamObj.freeInt = MISILE_HOMING_FLAG;
                        }
                    }
                }

                {
                    Character::MOTION_COL_DAT motionColDetails =
                        Character::GetMotionColDataFindByName(
                            Character::MOTION_COL_TYPE_ATK,
                            MISILE_ATK_COL_NAME
                        );

                    GAME_OBJECT_COLLISION_ATTACK_CHILD attackColTmp = motionColDetails.atkCol;
                    attackColTmp.subjectFamily[GAME_OBJECT_FAMILY_PLAYER] = true;
                    attackColTmp.subjectFamily[GAME_OBJECT_FAMILY_GIMIK] = true;

                    attackColTmp.size = MkF3(
                        ONE_BLOCK * 3,
                        ONE_BLOCK * 3,
                        ONE_BLOCK * 3
                    );
                    attackColTmp.pos.z = -attackColTmp.size.z / 2;
                    attackColTmp.pos.y += attackColTmp.size.y / 2 * 1.0f;

                    beamObj.collision.atk.enable = true;
                    beamObj.collision.atk.collision[0] = attackColTmp;
                    beamObj.collision.atk.collision[0].enable = true;
                }
                
                int summonNum = 1;
                if (m_HardPhase2Flag && !m_MisileMeteorFlag)
                {
                    //if (beamObj.freeInt == MISILE_HOMING_FLAG)
                    //{
                    //    summonNum = 2;
                    //}
                    summonNum = 2;

                    //beamObj.scale.x *= 1.3f;
                    //beamObj.scale.y *= 1.3f;
                    //beamObj.scale.z *= 1.3f;
                }

                beamObj.scale.x *= 1.3f;
                beamObj.scale.y *= 1.3f;
                beamObj.scale.z *= 1.3f;

                for (int i = 0; i < summonNum; i++)
                {
                    GAME_OBJECT* beamObjCreated = CreateGameObject(beamObj);
                    GameObjectSetLifeTime(beamObjCreated, FRAME_MISILE_LIFETIME);

                    if (m_MisileMeteorFlag)
                    {
                        if ((m_MisileFrame + misrate) % (misrate * 4) == 0)
                        {
                            beamObjCreated->freeInt = MISILE_OSOI_FLAG;
                        }
                    }
                    else
                    {
                        if (i == 1)
                        {
                            beamObjCreated->freeInt = MISILE_OSOI_FLAG;
                        }
                    }

                    //if (beamObj.freeInt != MISILE_HOMING_FLAG)
                    //{
                    //    GameObjectMoveRequest(beamObjCreated, FRAME_MISILE_LIFETIME,
                    //        MkF3(
                    //            beamObjCreated->pos.x + vecMisileTmp.x * ONE_BLOCK * 160.0f,
                    //            beamObjCreated->pos.y + vecMisileTmp.y * ONE_BLOCK * 160.0f,
                    //            beamObjCreated->pos.z + vecMisileTmp.z * ONE_BLOCK * 160.0f
                    //        ),
                    //        EasingInSine
                    //    );
                    //}
                    GameObjectMoveRequest(beamObjCreated, 60,
                        MkF3(
                            beamObjCreated->pos.x,
                            beamObjCreated->pos.y,
                            beamObjCreated->pos.z + ONE_BLOCK * -40.0f
                        ),
                        EasingOutSine
                    );

                    //m_Beam_Objects.push_back(GetIdentifierGameObject(beamObjCreated));
                    //m_Misile_Objects[m_Misile_Object_Count] = GetIdentifierGameObject(beamObjCreated);
                    m_Misile_Objects.push_back(GetIdentifierGameObject(beamObjCreated));
                    //m_Misile_Object_Count++;
                }
            }
        }

        if (m_MisileFrame == FRAME_MISILE_START)
        {
            if (m_MisileMeteorFlag)
            {
                //SEを再生
                SoundEffectPlayRequest(
                    GetSoundData("SOUND/SE/inperiusu/lockon"),
                    0.28f, 0, 0.9f);
            }
        }
    }

    m_MisileFrame++;

    if (m_MisileFrame > FRAME_ONE_SECOND * 5)
    {
        SetCameraModeAll(CAMERA_MODE_BOSS_CHASE);

        if (m_MisileMeteorFlag)
        {
            StartWarp({ 0.0f, ONE_BLOCK * -10.0f, 0.0f }, 0.0f, 25, true);
        }

        m_ActionCooldown = m_WazaCooldown;
        m_MisileFrame = -1;
        m_CurrentState = ActionState::Idle;
    }

}
void GOE_Character_Enemy_Boss_Inperiusu::draw_misile()
{
    //if (m_MisileFrame < 0)
    //{
    //    return;
    //}

    GAME_OBJECT* targetObject = GetGameObjectFindByIdentifier(
        m_TargetObjIdentifier);
    if (targetObject != nullptr)
    {
        if (m_MisileMeteorFlag
            && m_MisileFrame >= FRAME_MISILE_START
            && m_MisileFrame < FRAME_MISILE_AFTER + FRAME_ONE_SECOND * 2)
        {
            float enterf = std::min((float)(m_MisileFrame - FRAME_MISILE_START) / 12, 1.0f);
            for (int i = 0; i < 3; i++)
            {
                DRAW_POLYGON_DAT drawDatTmp = {};
                drawDatTmp.size = MkF3(
                    400.0f * 2.0f * enterf,
                    400.0f * 2.0f * enterf,
                    400.0f * 2.0f * enterf
                );

                if (i == 2)
                {
                    drawDatTmp.rot = 3.0f * m_Obj->frame;
                    drawDatTmp.color = MkF4(1.7f, 1.7f, 0.5f, 0.7f);
                    //drawDatTmp.shaderOpt.blendState = DPD_SHADER_OPT_BLEND_STATE_DEFAULT;
                    drawDatTmp.shaderOpt.blendState = DPD_SHADER_OPT_BLEND_STATE_ADDCOLOR;
                }
                else if (i == 1)
                {
                    drawDatTmp.rot = -5.0f * m_Obj->frame;
                    //drawDatTmp.color = MkF4(0.9f, 0.2f, 0.1f, 0.7f);
                    drawDatTmp.color = MkF4(1.7f, 1.7f, 0.5f, 0.7f);
                    drawDatTmp.shaderOpt.blendState = DPD_SHADER_OPT_BLEND_STATE_ADDCOLOR;
                }
                else
                {
                    drawDatTmp.color = MkF4(1.8f, 1.9f, 2.0f, 0.8f);
                    drawDatTmp.shaderOpt.blendState = DPD_SHADER_OPT_BLEND_STATE_ADDCOLOR;
                }

                drawDatTmp.rot3D = MkF3(0.0f, 0.0f, 0.0f);

                drawDatTmp.texWH = { 1.0f , 1.0f };

                drawDatTmp.texUV = {
                    0.0f,
                    0.0f
                };

                TEX_DAT texno[3] = {
                    m_Tex_cursor_000 ,
                    m_Tex_cursor_010 ,
                    m_Tex_cursor_020 ,
                };
                drawDatTmp.texNo = texno[i].textureId;

                drawDatTmp.loadTexType = LOADTEXTURETYPE_MAIN;

                drawDatTmp.startPos3D = targetObject->pos;
                drawDatTmp.startPos3D.z = ONE_BLOCK * -3.0f;

                drawDatTmp.startRot3D = MkF3(0.0f, 0.0f, 0.0f);

                drawDatTmp.startScale = MkF2(1.0f, 1.0f);
                drawDatTmp.startScale3D = MkF3(1.0f, 1.0f, 1.0f);

                drawDatTmp.modelNo = 0;

                drawDatTmp.shaderOpt.bloom = DPD_SHADER_OPT_BLOOM_ALWAYS;
                //drawDatTmp.shaderOpt.outline = DPD_SHADER_OPT_OUTLINE_RED;

                DRAW_POLYGON_IN_CAMERA_EX_OPTION opt = {};
                opt.reverse = false;
                opt.fixed = false;
                opt.enableShade = true;
                opt.billboard = true;
                opt.priority = DRAW_POLYGON_PRIORITY_NML_u1;
                opt.depthBuffIgnore = true;

                DrawPolygonInCameraEx(drawDatTmp, opt);
            }
        }
    }
}


void GOE_Character_Enemy_Boss_Inperiusu::event_super_beam_before_movieFlag()
{
    //ムービーフラグ
    GameManagerSetMovieFlag(true);

    SetCameraModeAll(CAMERA_MODE_NORMAL);

    {
        Float3 zoomPos = m_SuperBeamPosition;
        //zoomPos.z += 4200.0f;
        zoomPos.z += 20200.0f;

        //SetCameraTargetPosAll(MkF3(zoomPos.x, zoomPos.y + 20000.0f, zoomPos.z + -6000.0f));
        //SetCameraPosAll(zoomPos);
        //SetCameraZoomAll(0.35f);

        CameraMoveRequestAll(FRAME_ONE_SECOND * 0.5,
            zoomPos,
            0.0f,
            MkF2(1.0f, 1.0f),
            //0.35f,
            0.29f,
            EasingInOutSine,
            CAMERA_MOVE_TYPE_SMOOTH
        );
        CameraTargetMoveRequestAll(FRAME_ONE_SECOND * 0.5,
            //MkF3(zoomPos.x, zoomPos.y + 20000.0f, zoomPos.z + -6000.0f),
            MkF3(zoomPos.x, zoomPos.y + 20000.0f, zoomPos.z + -40000.0f),
            EasingInOutSine,
            CAMERA_MOVE_TYPE_SMOOTH
        );

    }

    //CameraFlashRequestAll(25, EasingOutSine, MkF4(-1.0f, -1.0f, -1.0f, 1.0f));

    event_playerPosSet();
}

//穴井亜レ井ター
void GOE_Character_Enemy_Boss_Inperiusu::update_super_beam()
{
    bool isnomovie = m_RetryCount > 0 && m_IsHard;

    int aliveLegaNum = 0;
    for (int i = 0; i < m_SuperBeamEnemies.size(); i++)
    {
        GAME_OBJECT* co = GetGameObjectFindByIdentifier(
            m_SuperBeamEnemies[i]
        );

        if (IsActiveGameObject(co))
        {
            m_SuperBeamEnemiesPos[i] = co->pos;
            aliveLegaNum++;
        }
    }

    if (m_SuperBeamStartFrame >= 0)
    {
        if (m_SuperBeamStartFrame == 0)
        { //初回
            m_SuperBeamEnemiesBuf.clear();
            m_SuperBeamEnemies.clear();
            m_SuperBeamEnemiesPos.clear();
            m_SuperBeamEnemiesFrame.clear();
            m_SuperBeamEnemiesSub.clear();

            m_SuperBeamStartIsNear = CalculateDistance(
                m_Obj->pos, 
                m_SuperBeamPosition
            ) < ONE_BLOCK * 12;
        }

        if (m_IsHard)
        {
            if (m_SuperBeamStartFrame == 0)
            { //初回

                StartWarp(m_SuperBeamPosition, 0.0f, 25);

            }

            if (m_SuperBeamStartFrame == 1)
            {
                if (!isnomovie)
                {
                    event_super_beam_before_movieFlag();
                }

                m_SuperBeamStartFrame = -2;
            }
        }
        else if (m_SuperBeamStartIsNear)
        { //近い場合
            if (m_SuperBeamStartFrame == 0)
            {
                StartMove(m_SuperBeamPosition, 0.0f, 4, true);
            }
            if (m_SuperBeamStartFrame == 1)
            {
                if (!isnomovie)
                {
                    event_super_beam_before_movieFlag();
                }
                m_SuperBeamStartFrame = -2;
            }
            //constexpr int firstMovF = 70;

            //if (m_SuperBeamStartFrame == 0)
            //{ //初回

            //    GameObjectMoveRequest(m_Obj, (int)(firstMovF * 0.8),
            //        m_SuperBeamPosition,
            //        EasingInOutSine,
            //        EasingInOutSine,
            //        EasingInOutSine,
            //        true
            //    );
            //    m_BeforeVecBuf = MkF3(0.0f, 1.0f, 0.0f);
            //}

            //if (m_SuperBeamStartFrame == firstMovF)
            //{
            //    event_super_beam_before_movieFlag();

            //    m_SuperBeamStartFrame = -2;
            //}
        }
        else //遠い場合
        {
            if (m_SuperBeamStartFrame == 0)
            {
                StartJet(m_SuperBeamPosition, 0.0f,
                    160//240
                );
            }
            if (m_SuperBeamStartFrame == 1)
            {
                if (!isnomovie)
                {
                    event_super_beam_before_movieFlag();
                }
                m_SuperBeamStartFrame = -2;
            }
            //constexpr int firstMovF = 240;

            //if (m_SuperBeamStartFrame == 0)
            //{ //初回

            //    //SEを再生
            //    SoundEffectPlayRequest(GetSoundData("SOUND/SE/enemy/attack/Beam_Charge"), 0.8f, 10);

            //    GameObjectMoveRequest(m_Obj, (int)(firstMovF * 0.35),
            //        MkF3(m_Obj->pos.x, m_Obj->pos.y, ONE_BLOCK * -100.0f),
            //        EasingInQuart
            //    );
            //    m_BeforeVecBuf = MkF3(0.0f, 1.0f, 0.0f);
            //}
            //if (m_SuperBeamStartFrame < firstMovF / 2
            //    && m_SuperBeamStartFrame % 4 == 0
            //    )
            //{
            //    CameraShakeRequestAll(CAMERA_SHAKE_SMALL_FRAME, CAMERA_SHAKE_MIDIUM_POWER);

            //    for (int i = 0; i < 5; i++)
            //    {
            //        Float3 efPos1 = {};
            //        efPos1.x += sin(2.0f * SYS_GENERAL_PI / 5 * i + (float)m_Obj->frame / 7);
            //        efPos1.y += cos(2.0f * SYS_GENERAL_PI / 5 * i + (float)m_Obj->frame / 7);

            //        Float3 efPos2 = m_Obj->pos;
            //        efPos2.x += efPos1.x * 90.0f;
            //        efPos2.y += efPos1.y * 90.0f;

            //        GAME_OBJECT* go = GO_CreateEffect(GO_EFFECT_TYPE_SMOKE, efPos2);
            //        if (go != nullptr)
            //        {
            //            go->vel = efPos1;
            //            go->vel.x *= 45.0f;
            //            go->vel.y *= 45.0f;
            //            go->vel.z = 12.0f;
            //            go->friction = 0.03f;

            //            //go->graphic.rot3d.x = 90.0f;
            //            go->graphic.color.w = 0.8f;
            //            go->scale = MkF3(
            //                3.0f,
            //                3.0f,
            //                3.0f
            //            );
            //            GameObjectSetLifeTime(go, 60);
            //        }
            //    }
            //}

            //if (m_SuperBeamStartFrame == firstMovF / 2)
            //{ //

            //    m_Obj->pos = MkF3(
            //        m_SuperBeamPosition.x,
            //        m_SuperBeamPosition.y,
            //        ONE_BLOCK * -100.0f
            //    );
            //    GameObjectMoveRequest(m_Obj, (int)(firstMovF * 0.4),
            //        m_SuperBeamPosition,
            //        EasingInOutQuart
            //    );
            //}
            //if (m_SuperBeamStartFrame == firstMovF / 2 + FRAME_ONE_SECOND)
            //{ //
            //    event_super_beam_before_movieFlag();
            //}
            //if (m_SuperBeamStartFrame >= firstMovF / 2 
            //    && m_SuperBeamStartFrame < firstMovF * 0.8
            //    && m_SuperBeamStartFrame % 6 == 0
            //    )
            //{
            //    CameraShakeRequestAll(CAMERA_SHAKE_MIDIUM_FRAME, CAMERA_SHAKE_SMALL_POWER);

            //    for (int i = 0; i < 5; i++)
            //    {
            //        Float3 efPos1 = {};
            //        efPos1.x += sin(2.0f * SYS_GENERAL_PI / 5 * i + (float)m_Obj->frame / 7);
            //        efPos1.y += cos(2.0f * SYS_GENERAL_PI / 5 * i + (float)m_Obj->frame / 7);

            //        Float3 efPos2 = m_Obj->pos;
            //        efPos2.x += efPos1.x * 90.0f;
            //        efPos2.y += efPos1.y * 90.0f;
            //        efPos2.z = -50.0f;

            //        GAME_OBJECT* go = GO_CreateEffect(GO_EFFECT_TYPE_SMOKE, efPos2);
            //        if (go != nullptr)
            //        {
            //            go->vel = efPos1;
            //            go->vel.x *= 110.0f;
            //            go->vel.y *= 110.0f;
            //            go->vel.z = -72.0f;
            //            go->friction = 0.08f;

            //            //go->graphic.rot3d.x = 90.0f;
            //            go->graphic.color.w = 0.6f;
            //            go->scale = MkF3(
            //                3.0f,
            //                3.0f,
            //                3.0f
            //            );
            //            GameObjectSetLifeTime(go, 50);
            //        }
            //    }
            //}

            //if (m_SuperBeamStartFrame >= firstMovF)
            //{
            //    m_SuperBeamStartFrame = -2;
            //}
        }
        m_SuperBeamStartFrame++;
    }
    else
    {
        m_Obj->status.hitStopArmorFrame = 60;

        if (m_SuperBeamFrame == 0)
        {
            //SEを再生
            SoundEffectPlayRequest(
                GetSoundData("SOUND/SE/enemy/move/Inperius_Jet_Start"),
                2.8f, 0);
            //SEを再生
            SoundEffectPlayRequest(
                GetSoundData("SOUND/SE/enemy/attack/super_beam_start"),
                2.8f, 0);
            
            StartAnim(AnimState::BeamBefore, 160, true);
            //カウントをストップ
            //SetStageCountStopSceneGame(true);

            //プレイヤーの硬直
            //GameManagerSetPlayerCtrlStop(true);
        }

        if (m_SuperBeamFrame == FRAME_ONE_SECOND * 0.5)
        {
            if (!isnomovie)
            {
                Float3 zoomPos = m_Obj->pos;
                zoomPos.z += 100200.0f;

                CameraTargetMoveRequestAll(FRAME_ONE_SECOND * 1.0,
                    MkF3(0.0f, -10000.0f, -16000.0f),
                    EasingInOutSine,
                    CAMERA_MOVE_TYPE_SMOOTH
                );
                CameraMoveRequestAll(FRAME_ONE_SECOND * 1.0,
                    zoomPos,
                    0.0f,
                    MkF2(1.0f, 1.0f),
                    //0.18f,
                    0.1f,
                    EasingInOutSine,
                    CAMERA_MOVE_TYPE_SMOOTH
                );
            }
        }

        if (m_SuperBeamFrame == FRAME_ONE_SECOND * 0.5)
        {
            if (m_SuperBeamFinish)
            {
                Float2 legapos[4] = {
                    {BLOCK_WIDTH * 30.0f, BLOCK_HIGHT * 30.0f},
                    {BLOCK_WIDTH * -30.0f, BLOCK_HIGHT * -30.0f},
                    {BLOCK_WIDTH * 30.0f, BLOCK_HIGHT * -30.0f},
                    {BLOCK_WIDTH * -30.0f, BLOCK_HIGHT * 30.0f},
                };
                for (int i = 0; i < 4; i++)
                {
                    GAME_OBJECT* co = nullptr;
                    {
                        //co = Character::CreateCharacterGameObject("ENEMY_RUCS");
                        co = Character::CreateCharacterGameObject("ENEMY_REGATOSU");
                        if (co != nullptr)
                        {
                            co->pos = MkF3(
                                legapos[i].x,
                                legapos[i].y,
                                0.0f);
                            {
                                GOE_Character_Enemy* goe =
                                    dynamic_cast<GOE_Character_Enemy*>(co->events.source);  // ダウンキャスト

                                if (goe != nullptr)
                                { //
                                    //goe->BecomeApp(90);
                                    goe->BecomeApp(45);
                                    co->freeInt = 90 * i;
                                }
                            }
                        }

                    }

                    if (co != nullptr)
                    {
                        m_SuperBeamEnemiesBuf.push_back(GetIdentifierGameObject(co));
                    }
                }
            }
            else
            {
                Float2 legapos[4] = {
                    {BLOCK_WIDTH * 38.0f, BLOCK_HIGHT * 15.0f},
                    {BLOCK_WIDTH * -52.0f, BLOCK_HIGHT * -16.0f},
                    {BLOCK_WIDTH * 52.0f, BLOCK_HIGHT * -16.0f},
                    {BLOCK_WIDTH * -38.0f, BLOCK_HIGHT * 15.0f},
                };
                for (int i = 0; i < 4; i++)
                {
                    GAME_OBJECT* co = nullptr;
                    {
                        co = Character::CreateCharacterGameObject("ENEMY_REGATOSU");
                        if (co != nullptr)
                        {
                            co->pos = MkF3(
                                legapos[i].x,
                                legapos[i].y,
                                0.0f);
                            {
                                GOE_Character_Enemy_Regatosu* goe =
                                    dynamic_cast<GOE_Character_Enemy_Regatosu*>(co->events.source);  // ダウンキャスト

                                if (goe != nullptr)
                                { //
                                    goe->SetBuffSuper();
                                    goe->SetNoBuff();
                                    goe->SetAnimAttackLoop();
                                    goe->BecomeApp(90);
                                }
                            }
                        }

                        //co = Character::CreateCharacterGameObject("ENEMY_RUCS");
                        //if (co != nullptr)
                        //{
                        //    co->pos = MkF3(
                        //        legapos[i].x,
                        //        legapos[i].y,
                        //        0.0f);
                        //    {
                        //        GOE_Character_Enemy* goe =
                        //            dynamic_cast<GOE_Character_Enemy*>(co->events.source);  // ダウンキャスト

                        //        if (goe != nullptr)
                        //        { //
                        //            goe->BecomeApp(90);
                        //        }
                        //    }
                        //}

                        //co = Character::CreateCharacterGameObject("ENEMY_BOSS_INPERIUSU");
                        //if (co != nullptr)
                        //{
                        //    co->pos = MkF3(
                        //        legapos[i].x,
                        //        legapos[i].y,
                        //        0.0f);
                        //    {
                        //        GOE_Character_Enemy_Boss_Inperiusu* goe =
                        //            dynamic_cast<GOE_Character_Enemy_Boss_Inperiusu*>(co->events.source);  // ダウンキャスト

                        //        if (goe != nullptr)
                        //        { //
                        //            goe->BecomeHard();
                        //            goe->StartBattle();
                        //        }
                        //    }
                        //}

                        //co = Character::CreateCharacterGameObject("ENEMY_BOSS_KONTORIUSU");
                    }

                    if (co != nullptr)
                    {
                        m_SuperBeamEnemiesBuf.push_back(GetIdentifierGameObject(co));
                    }
                }
            }
        }


        if (m_SuperBeamFrame == FRAME_ONE_SECOND * 0.5 + 90)
        {
            //SEを再生
            SoundEffectPlayRequest(
                GetSoundData("SOUND/SE/inperiusu/b_charge"),
                2.0f, 0, 0.75f);

            //SEを再生
            SoundEffectPlayRequest(
                GetSoundData("SOUND/SE/enemy/move/warpHith"),
                2.2f, 0, 1.25f);

            m_SuperBeamEnemies = m_SuperBeamEnemiesBuf;
            for (auto v : m_SuperBeamEnemies)
            {
                GAME_OBJECT* ob = GetGameObjectFindByIdentifier(v);
                if (IsActiveGameObject(ob))
                {
                    m_SuperBeamEnemiesPos.push_back(ob->pos);
                }
                m_SuperBeamEnemiesFrame.push_back(0);
            }
        }

        if (m_SuperBeamFrame == FRAME_ONE_SECOND * 1.3)
        {
            ////SEを再生
            //SoundEffectPlayRequest(GetSoundData("SOUND/SE/enemy/move/warpHith"), 3.8f, 0);
            ////SEを再生
            //SoundEffectPlayRequest(GetSoundData("SOUND/SE/enemy/move/warpLow"), 3.8f, 2);

            StartAnim(AnimState::Beam, 60, true);
        }

        if (m_SuperBeamFrame == FRAME_ONE_SECOND * 3.5)
        {
            if (!isnomovie)
            {
                Float3 zoomPos = m_SuperBeamPosition;
                //zoomPos.z += 4200.0f;
                zoomPos.z += 20200.0f;

                CameraTargetMoveRequestAll(FRAME_ONE_SECOND * 0.5,
                    //MkF3(zoomPos.x, zoomPos.y + 20000.0f, zoomPos.z + -6000.0f),
                    MkF3(zoomPos.x, zoomPos.y + 10000.0f, zoomPos.z + -32000.0f),
                    EasingInOutSine,
                    CAMERA_MOVE_TYPE_SMOOTH
                );
                CameraMoveRequestAll(FRAME_ONE_SECOND * 0.5,
                    zoomPos,
                    0.0f,
                    MkF2(1.0f, 1.0f),
                    //0.35f,
                    0.3f,
                    EasingInOutSine,
                    CAMERA_MOVE_TYPE_SMOOTH
                );
            }
        }

        if (m_SuperBeamFrame == FRAME_ONE_SECOND * 4.0)
        {

            ////SEを再生
            //SoundEffectPlayRequest(
            //    GetSoundData("SOUND/SE/enemy/attack/super_beam_charge"),
            //    0.8f, 20, 1.0f);

            //SEを再生
            SoundEffectPlayRequest(
                GetSoundData("SOUND/SE/enemy/attack/Annihilator_Charge"),
                0.7f, 20, 1.0f); 
            StartAnim(AnimState::SuperBeamBefore, 32767, true);

            //CameraFlashRequestAll(
            //    80,
            //    EasingInOutSine,
            //    //MkF4(0.9f, 0.7f, 0.5f, 0.8f),
            //    MkF4(10.0f, 10.0f, 5.0f, 1.0f),
            //    DPD_SHADER_OPT_BLEND_STATE_ADDCOLOR
            //);
            CameraFlashRequestAll(
                60,
                EasingInOutSine,
                //MkF4(0.9f, 0.7f, 0.5f, 0.8f),
                MkF4(2.0f, 0.1f, 5.0f, 0.5f),
                DPD_SHADER_OPT_BLEND_STATE_ADDCOLOR
            );

            ////SEを再生
            //SoundEffectPlayRequest(GetSoundData("SOUND/SE/enemy/move/warpHith"), 
            //    5.8f, 0, 0.5f);
            //SEを再生
            SoundEffectPlayRequest(
                GetSoundData("SOUND/SE/enemy/move/warpHith"),
                2.8f, 0, 0.9f);
            //SEを再生
            SoundEffectPlayRequest(
                GetSoundData("SOUND/SE/enemy/move/warpLow"),
                2.8f, 2, 0.7f);


            m_Obj->status.guardType = GAME_OBJECT_GUARD_TYPE_INV;

            m_SuperBeamBarrierFlag = true;
        }
        if (m_SuperBeamFrame == FRAME_ONE_SECOND * 4.0 + SUPER_BEAM_BARRIER_IN_FRAME)
        {
            CameraShakeRequestAll(
                CAMERA_SHAKE_EXLARGE_FRAME,
                CAMERA_SHAKE_EXLARGE_POWER * 1
            );
        }
        if (m_SuperBeamFrame == FRAME_ONE_SECOND * 4.0 + 20)
        {
            if (aliveLegaNum >= 1)
            {
                CameraShakeRequestAll(
                    CAMERA_SHAKE_MIDIUM_FRAME,
                    CAMERA_SHAKE_EXLARGE_POWER * 1
                );
                //SEを再生
                SoundEffectPlayRequest(
                    GetSoundData("SOUND/SE/inperiusu/b_attack"),
                    1.0f, 8, 1.9f);
                SoundEffectPlayRequest(SOUND_TYPE_SE_9, 2.5f, 9, 0.9f);

                ShowBossbarBarrier_Game(0, 0);
            }
        }
        if (m_SuperBeamFrame == FRAME_ONE_SECOND * 4.0 + 35)
        {
            if (aliveLegaNum >= 2)
            {
                CameraShakeRequestAll(
                    CAMERA_SHAKE_MIDIUM_FRAME,
                    CAMERA_SHAKE_EXLARGE_POWER * 1
                );
                //SEを再生
                SoundEffectPlayRequest(
                    GetSoundData("SOUND/SE/inperiusu/b_attack"),
                    1.0f, 8, 1.9f);
                SoundEffectPlayRequest(SOUND_TYPE_SE_9, 2.5f, 9, 0.9f);

                ShowBossbarBarrier_Game(1, 0);
            }
        }
        if (m_SuperBeamFrame == FRAME_ONE_SECOND * 4.0 + 50)
        {
            if (aliveLegaNum >= 3)
            {
                CameraShakeRequestAll(
                    CAMERA_SHAKE_MIDIUM_FRAME,
                    CAMERA_SHAKE_EXLARGE_POWER * 1
                );
                //SEを再生
                SoundEffectPlayRequest(
                    GetSoundData("SOUND/SE/inperiusu/b_attack"),
                    1.0f, 8, 1.9f);
                SoundEffectPlayRequest(SOUND_TYPE_SE_9, 2.5f, 9, 0.9f);

                ShowBossbarBarrier_Game(2, 0);
            }
        }
        if (m_SuperBeamFrame == FRAME_ONE_SECOND * 4.0 + 65)
        {
            if (aliveLegaNum >= 4)
            {
                CameraShakeRequestAll(
                    CAMERA_SHAKE_MIDIUM_FRAME,
                    CAMERA_SHAKE_EXLARGE_POWER * 1
                );
                //SEを再生
                SoundEffectPlayRequest(
                    GetSoundData("SOUND/SE/inperiusu/b_attack"),
                    1.0f, 8, 1.9f);
                SoundEffectPlayRequest(SOUND_TYPE_SE_9, 2.5f, 9, 0.9f);

                ShowBossbarBarrier_Game(3, 0);
            }
        }



        if (m_SuperBeamFrame == FRAME_ONE_SECOND * 5.0)
        {
        }

        if (m_SuperBeamFrame == FRAME_ONE_SECOND * 6)
        {
            if(m_IsHard)
            {
                for (int i = 0; i < m_SuperBeamEnemies.size(); i++)
                {
                    GAME_OBJECT* co = GetGameObjectFindByIdentifier(
                        m_SuperBeamEnemies[i]
                    );

                    if (co != nullptr)
                    {
                        (void)rand();
                        //援助ミレス
                        for (int j = 0; j < 3; j++)
                        {
                            GAME_OBJECT* co2 = nullptr;
                            {
                                if (j < 2)
                                {
                                    co2 = Character::CreateCharacterGameObject(
                                        "ENEMY_MIRESU_SHOOTER");
                                }
                                else
                                {
                                    co2 = Character::CreateCharacterGameObject(
                                        "ENEMY_MIRESU_ATTACKER");
                                }
                                if (co2 != nullptr)
                                {
                                    co2->pos = MkF3(
                                        co->pos.x + ((float)(rand() % 1000)
                                            / 1000 - 0.5f) * (BLOCK_WIDTH * 20),
                                        co->pos.y + ((float)(rand() % 1000)
                                            / 1000 - 0.5f) * (BLOCK_HIGHT * 20),
                                        0.0f);
                                    {
                                        GOE_Character_Enemy_Miresu* goe =
                                            dynamic_cast<GOE_Character_Enemy_Miresu*>(co2->events.source);  // ダウンキャスト

                                        if (goe != nullptr)
                                        { //
                                            goe->SetBuffSuper();
                                            goe->BecomeApp(90);
                                        }
                                    }
                                }
                            }

                            if (co2 != nullptr)
                            {
                                m_SuperBeamEnemiesSub.push_back(GetIdentifierGameObject(co2));
                            }
                        }
                    }
                }
            }

            //カウントストップを解除
            //SetStageCountStopSceneGame(false);

            //プレイヤーの硬直を解除
            //GameManagerSetPlayerCtrlStop(false);

            //ムービーフラグ
            GameManagerSetMovieFlag(false);

            SetUIEventTimer_Game(true,
                &m_SuperBeamCountDownFrame,
                m_SuperBeamLimitF,
                2,
                true
            );

            //SetCameraModeAll(CAMERA_MODE_BOSS_CHASE);
            SetCameraModeAll(CAMERA_MODE_BOSSANDZAKO_CHASE);

            if (!m_SuperBeamFinish)
            {
                m_SuperBeamGayaId = GE_GayaStartRequest("iseki_inperiusu_superbeam_start_mayu");
            }

            m_SuperBeamCountDownFrame = 0;
        }

        //カウントダウン&判定
        if (m_SuperBeamCountDownFrame >= 0
            && m_SuperBeamAfterFrame == -1)
        {
            if (m_SuperBeamFinish)
            {
                if (m_SuperBeamEnemies.size() > 0)
                {
                    for (int i = 0; i < m_SuperBeamEnemies.size(); i++)
                    {
                        GAME_OBJECT* enemObj = GetGameObjectFindByIdentifier(m_SuperBeamEnemies[i]);
                        if (IsActiveGameObject(enemObj))
                        {
                            if (enemObj->delRequ > 0)
                            {
                                continue;
                            }

                            float dis = CalculateDistance(enemObj->pos, m_Obj->pos);
                            float rot = CalculateDegToRad(
                                m_SuperBeamCountDownFrame * 1.2f
                                + enemObj->freeInt + 90.0f
                            );
                            enemObj->pos.x = m_Obj->pos.x + cos(rot) * dis;
                            enemObj->pos.y = m_Obj->pos.y + sin(rot) * dis;

                        }
                    }
                }
            }

            if (!m_SuperBeamBreakFlag)
            {
                bool allDefeat = true;
                if (m_SuperBeamEnemies.size() > 0)
                {
                    for (int i = 0; i < m_SuperBeamEnemies.size(); i++)
                    {
                        GAME_OBJECT* enemObj = GetGameObjectFindByIdentifier(m_SuperBeamEnemies[i]);
                        if (IsActiveGameObject(enemObj))
                        {
                            allDefeat = false;
                            break;
                        }
                    }
                }
                //else
                //{
                //    allDefeat = false;
                //}

                //成功
                if (allDefeat)
                {
                    m_SuperBeamBreakFlag = true;
                    m_SuperBeamAfterFrame = 0;
                }
            }

            if (m_SuperBeamCountDownFrame == (m_SuperBeamLimitF - FRAME_ONE_SECOND * 5))
            {
                GE_GayaStartRequest("iseki_inperiusu_superbeam_beforefire_mayu");
            }

            //失敗
            if (m_SuperBeamAfterFrame != 0
                && m_SuperBeamCountDownFrame >= m_SuperBeamLimitF)
            {
                ////SEを再生
                //{
                //    SoundEffectPlayRequest(
                //        GetSoundData("SOUND/SE/enemy/attack/super_beam_miminari"),
                //        1.0f, 0, 0.9f);
                //}
                m_SuperBeamBreakFlag = false;
                m_SuperBeamAfterFrame = 0;
            }

            m_SuperBeamCountDownFrame++;
        }

        //結果
        if (m_SuperBeamAfterFrame >= 0)
        {
            if (m_SuperBeamAfterFrame == 0)
            {
                m_SuperBeamBarrierFlag = false;
                m_Obj->status.guardType = GAME_OBJECT_GUARD_TYPE_NORMAL;

                //SEを再生
                SoundEffectPlayRequest(
                    GetSoundData("SOUND/SE/enemy/move/Inperius_Jet_Start"),
                    2.8f, 0, 1.2f);

                GE_GayaCancelRequest(m_SuperBeamGayaId);

                SetUIEventTimer_Game(false);

                //カウントをストップ
                //SetStageCountStopSceneGame(true);

                //プレイヤーの硬直
                //GameManagerSetPlayerCtrlStop(true);

                //ムービーフラグ
                GameManagerSetMovieFlag(true);
                GameManagerSetMoviePlayerHiddenFlag(true);

                SetCameraModeAll(CAMERA_MODE_NORMAL);

                //CameraMoveRequestAll(FRAME_ONE_SECOND * 0.5,
                //    m_Obj->pos,
                //    0.0f,
                //    MkF2(1.0f, 1.0f),
                //    0.35f,
                //    EasingInOutSine,
                //    CAMERA_MOVE_TYPE_SMOOTH
                //);
                //CameraTargetMoveRequestAll(FRAME_ONE_SECOND * 0.5,
                //    MkF3(m_Obj->pos.x, m_Obj->pos.y, m_Obj->pos.z - 24000.0f),
                //    EasingInOutSine,
                //    CAMERA_MOVE_TYPE_SMOOTH
                //);
                Float3 zoomPos = m_SuperBeamPosition;
                //zoomPos.z += 4200.0f;
                zoomPos.z += 30000.0f;

                CameraTargetMoveRequestAll(FRAME_ONE_SECOND * 0.5,
                    //MkF3(zoomPos.x, zoomPos.y + 20000.0f, zoomPos.z + -6000.0f),
                    MkF3(zoomPos.x, zoomPos.y + 10000.0f, zoomPos.z + -52000.0f),
                    EasingInOutSine,
                    CAMERA_MOVE_TYPE_SMOOTH
                );
                CameraMoveRequestAll(FRAME_ONE_SECOND * 0.5,
                    zoomPos,
                    0.0f,
                    MkF2(1.0f, 1.0f),
                    //0.35f,
                    0.3f,
                    EasingInOutSine,
                    CAMERA_MOVE_TYPE_SMOOTH
                );
            }
            if (m_SuperBeamAfterFrame == 1)
            {
                for (int i = 0; i < m_SuperBeamEnemies.size(); i++)
                {
                    if (IsEnableBossbarBarrier_Game(i)
                        && !IsBreakingBossbarBarrier_Game(i))
                    {
                        ShowBossbarBarrier_Game(i, 1);
                    }
                }
            }

            if (m_SuperBeamBreakFlag)
            {//成功
                if (m_SuperBeamFinish)
                {
                    if (m_SuperBeamAfterFrame == 0)
                    {
                        //m_SuperBeamSuccessFlag = true;

                        //SEを再生
                        SoundEffectPlayRequest(
                            GetSoundData("SOUND/SE/enemy/move/Inperius_Jet"),
                            2.8f, 0, 0.7f);

                        ////BGMを再生する
                        //SoundBGMPlayRequest(m_bgmLoop,
                        //    true,
                        //    2.0f,
                        //    0.0f,

                        //    58.5f
                        //);
                        //SoundBGMStopRequest(0, FRAME_ONE_SECOND * 8.5f);
                        SoundBGMStopRequest(0);
                    }
                    if (m_SuperBeamAfterFrame == FRAME_ONE_SECOND * 1)
                    {
                        m_SuperBeamSuccessFlag = true;
                        m_SuperBeamSuccessWeaknessFlag = true;

                        CameraShakeRequestAll(
                            CAMERA_SHAKE_EXLARGE_FRAME * 2.0,
                            CAMERA_SHAKE_EXLARGE_POWER * 0.3
                        );
                        //SEを再生
                        SoundEffectPlayRequest(GetSoundData("SOUND/SE/player/attack/Kakopon_Contorius"),
                            0.2f, 0, 1.1f);
                        //SEを再生
                        SoundEffectPlayRequest(GetSoundData("SOUND/SE/player/attack/Kakopon_Contorius"),
                            0.5f, 30, 1.3f);
                    }
                    if (m_SuperBeamAfterFrame == FRAME_ONE_SECOND * 2)
                    {
                        m_Obj->status.guardType = GAME_OBJECT_GUARD_TYPE_NORMAL;

                        m_Obj->status.hpMin = 0;
                        m_Obj->status.hp = 0;

                        CameraFlashRequestAll(
                            FRAME_ONE_SECOND * 1.8f,
                            EasingInOutSine,
                            MkF4(2.8f, 2.8f, 0.5f, 1.0f),
                            DPD_SHADER_OPT_BLEND_STATE_ADDCOLOR
                        );
                    }
                }
                else if (m_IsHard)
                {
                    if (m_SuperBeamAfterFrame == 0)
                    {
                        //m_SuperBeamSuccessFlag = true;

                        //SEを再生
                        SoundEffectPlayRequest(
                            GetSoundData("SOUND/SE/enemy/move/Inperius_Jet"),
                            2.8f, 0, 0.7f);

                    }
                    if (m_SuperBeamAfterFrame == FRAME_ONE_SECOND * 1)
                    {
                        m_SuperBeamSuccessFlag = true;

                        CameraShakeRequestAll(
                            CAMERA_SHAKE_EXLARGE_FRAME * 2.0,
                            CAMERA_SHAKE_EXLARGE_POWER * 0.3
                        );
                        //SEを再生
                        SoundEffectPlayRequest(GetSoundData("SOUND/SE/player/attack/Kakopon_Contorius"),
                            0.2f, 0, 1.1f);
                    }
                    if (m_SuperBeamAfterFrame == FRAME_ONE_SECOND * 2)
                    {
                        //m_Obj->status.guardType = GAME_OBJECT_GUARD_TYPE_SHORT;

                        //SEを再生
                        //SoundEffectPlayRequest(
                        //    GetSoundData("SOUND/SE/enemy/attack/Big_Explosion"),
                        //    3.8f, 0, 0.7f);
                        StartAnim(AnimState::Damage, 1, true);

                        //StartAnim(AnimState::Idle, 1, false);
                        //{
                        //    Float3 efPos = m_Obj->pos;
                        //    efPos.y += ONE_BLOCK * 4.0f;
                        //    efPos.z += ONE_BLOCK * -10.0f;
                        //    GO_CreateExplosion(efPos, 0.8f);
                        //}

                        CameraShakeRequestAll(
                            CAMERA_SHAKE_EXLARGE_FRAME * 1.5,
                            CAMERA_SHAKE_EXLARGE_POWER * 1,
                            CAMERA_SHAKE_EXLARGE_POWER * 1
                        );
                        CameraFlashRequestAll(
                            FRAME_ONE_SECOND * 2.5f,
                            EasingInOutSine,
                            MkF4(0.7f, 0.0f, 0.4f, 0.4f),
                            DPD_SHADER_OPT_BLEND_STATE_ADDCOLOR
                        );
                    }
                    if (m_SuperBeamAfterFrame == FRAME_ONE_SECOND * 2)
                    {
                        StartWarp(m_Obj->pos, 0.0f, 30);
                    }

                }
                else
                {
                    if (m_SuperBeamAfterFrame == 0)
                    {
                        //m_SuperBeamSuccessFlag = true;

                        //SEを再生
                        SoundEffectPlayRequest(
                            GetSoundData("SOUND/SE/enemy/move/Inperius_Jet"),
                            2.8f, 0, 0.7f);

                        //BGMを再生する
                        SoundBGMPlayRequest(m_bgmLoop,
                            true,
                            2.0f,
                            0.0f,

                            38.5f
                        );
                    }
                    if (m_SuperBeamAfterFrame == FRAME_ONE_SECOND * 1)
                    {
                        m_SuperBeamSuccessFlag = true;
                        m_SuperBeamSuccessWeaknessFlag = true;

                        CameraShakeRequestAll(
                            CAMERA_SHAKE_EXLARGE_FRAME * 2.0,
                            CAMERA_SHAKE_EXLARGE_POWER * 0.3
                        );
                        //SEを再生
                        SoundEffectPlayRequest(GetSoundData("SOUND/SE/player/attack/Kakopon_Contorius"),
                            0.2f, 0, 1.1f);
                        //SEを再生
                        SoundEffectPlayRequest(GetSoundData("SOUND/SE/player/attack/Kakopon_Contorius"),
                            0.5f, 30, 1.3f);
                    }
                    if (m_SuperBeamAfterFrame == FRAME_ONE_SECOND * 2)
                    {
                        m_Obj->status.guardType = GAME_OBJECT_GUARD_TYPE_NORMAL;

                        //SEを再生
                        SoundEffectPlayRequest(
                            GetSoundData("SOUND/SE/enemy/attack/Big_Explosion"),
                            2.8f, 0, 0.7f);
                        SoundEffectPlayRequest(SOUND_TYPE_SE_9, 3.2f, 0, 0.5f);
                        SoundEffectPlayRequest(SOUND_TYPE_SE_9, 2.8f, 30, 0.3f);

                        StartAnim(AnimState::Damage, 60, true);

                        {
                            Float3 efPos = m_Obj->pos;
                            efPos.y += ONE_BLOCK * 4.0f;
                            efPos.z += ONE_BLOCK * -7.0f;
                            GO_CreateExplosion(efPos, 1.0f);
                        }

                        CameraShakeRequestAll(
                            CAMERA_SHAKE_EXLARGE_FRAME * 1.5,
                            CAMERA_SHAKE_EXLARGE_POWER * 6,
                            CAMERA_SHAKE_EXLARGE_POWER * 0.5
                        );
                        GameSlowRequest(
                            FRAME_ONE_SECOND / 2,
                            4
                        );
                        //CameraFlashRequestAll(
                        //    FRAME_ONE_SECOND * 2.5f,
                        //    EasingInOutSine,
                        //    MkF4(0.7f, 0.7f, 0.4f, 0.4f),
                        //    DPD_SHADER_OPT_BLEND_STATE_ADDCOLOR
                        //);
                        CameraFlashRequestAll(
                            FRAME_ONE_SECOND * 1.8f,
                            EasingInOutSine,
                            MkF4(1.8f, 1.8f, 0.5f, 1.0f),
                            DPD_SHADER_OPT_BLEND_STATE_ADDCOLOR
                        );
                        m_KakoEfScreenMaskFrameMax = 16;
                        m_KakoEfScreenMaskFrame = m_KakoEfScreenMaskFrameMax - 2;
                        //m_SuperBeamScreenBreakFrameMax = 16;
                        //m_SuperBeamScreenBreakFrame = m_SuperBeamScreenBreakFrameMax;

                        if (m_Obj->status.hp > SUPER_BEAM_DAMAGE_MINHP)
                        {
                            //有効なくらい判定を探す
                            GAME_OBJECT_COLLISION_RECIEVE_CHILD colRec = {};
                            for (int j = 0; j < GAME_OBJECT_COLLISION_NUM_MAX; j++)
                            {
                                if (m_Obj->collision.rec.collision[j].enable)
                                {
                                    colRec = m_Obj->collision.rec.collision[j];
                                    break;
                                }
                            }

                            if (colRec.enable)
                            { //判定が有効なら処理する

                                Character::MOTION_COL_DAT motionColDetails =
                                    Character::GetMotionColDataFindByName(
                                        Character::MOTION_COL_TYPE_ATK,
                                        BLADE_ATK_COL_NAME
                                    );
                                motionColDetails.atkCol.power = (int)(m_Obj->status.hpMax * 0.25);

                                strcpy_s(
                                    motionColDetails.atkCol.tag,
                                    ARRAYSIZE(motionColDetails.atkCol.tag),
                                    "SUPER_BEAM_EXPLODE"
                                );

                                GameObjectDamageCollisionHit(m_Obj, m_Obj,
                                    motionColDetails.atkCol,
                                    colRec
                                );
                                GameObjectHitStop(m_Obj, motionColDetails.atkCol.hitStopReciever);


                                if (m_Obj->status.hp < SUPER_BEAM_DAMAGE_MINHP)
                                {
                                    m_Obj->status.hp = SUPER_BEAM_DAMAGE_MINHP;
                                }
                            }
                        }
                    }
                    if (m_SuperBeamAfterFrame == FRAME_ONE_SECOND * 2 + 60)
                    {
                        m_StarnFrame = SUPER_BEAM_AFTER_STARN;
                        StartAnim(AnimState::Damage2, 
                            SUPER_BEAM_AFTER_STARN - SUPER_BEAM_AFTER_STARN_END,
                            true
                        );

                        zako_powerdown();
                    }
                }
                if (m_SuperBeamAfterFrame == FRAME_ONE_SECOND * 3)
                {
                    m_SuperBeamAfterFrame = -2;

                    GE_GayaStartRequest("iseki_inperiusu_superbeam_success_mayu");
                }
            }
            else
            {//失敗
                if (m_SuperBeamAfterFrame == FRAME_ONE_SECOND * 1)
                {
                    //m_Obj->status.guardType = GAME_OBJECT_GUARD_TYPE_SHORT;


                    StartAnim(AnimState::SuperBeam, 120, true);
                }
                if (m_SuperBeamAfterFrame == FRAME_ONE_SECOND * 1 + 10)
                {
                    {
                        Float3 postmp = m_Obj->pos;
                        postmp.y += ONE_BLOCK * 4.0f;
                        postmp.z += ONE_BLOCK * -10.0f;

                        GAME_OBJECT* go = GO_CreateEffect(
                            GO_EFFECT_TYPE_OVERLAY_BALL,
                            postmp);

                        if (go != nullptr)
                        {
                            go->graphic.color = MkF4(
                                2.0f,
                                2.0f,
                                2.0f,
                                1.0f
                            );
                            go->scale = MkF3(
                                4000.0f,
                                4000.0f,
                                4000.0f
                            );
                            GameObjectSetLifeTime(go, 20);
                        }
                    }
                    {
                        m_SuperBeamBgmVolumeBuf = GetSoundBGMVolume();
                        SoundBGMSetVolumeRequest(0.0f);
                    }

                    m_SuperBeamShootBgFrameMax = 40;
                    m_SuperBeamShootBgFrame = m_SuperBeamShootBgFrameMax;

                    GameSlowRequest(10, 5);
                    //CameraFlashRequestAll(
                    //    50,
                    //    EasingInOutSine,
                    //    MkF4(0.3f, 0.0f, 0.0f, 0.2f)
                    //);
                }
                if (m_SuperBeamAfterFrame == FRAME_ONE_SECOND * 1 + 20)
                {
                    //SEを再生
                    SoundEffectPlayRequest(
                        GetSoundData("SOUND/SE/enemy/attack/ExplosionLEx"),
                        3.8f, 0, 1.1f);
                    //SEを再生
                    SoundEffectPlayRequest(
                        GetSoundData("SOUND/SE/enemy/attack/Explosion"),
                        1.4f, 0, 0.7f);
                    //SEを再生
                    SoundEffectPlayRequest(
                        GetSoundData("SOUND/SE/enemy/attack/super_beam_miminari"),
                        0.9f, 3, 0.65f);

                    //CameraFlashRequestAll(
                    //    140,
                    //    EasingOutSine,
                    //    //MkF4(0.9f, 0.7f, 0.5f, 0.8f),
                    //    MkF4(100.0f, 100.0f, 50.0f, 1.0f),
                    //    DPD_SHADER_OPT_BLEND_STATE_ADDCOLOR
                    //);
                    //CameraFlashRequestAll(
                    //    50,
                    //    EasingOutSine,
                    //    //MkF4(0.9f, 0.7f, 0.5f, 0.8f),
                    //    MkF4(100.0f, 100.0f, 50.0f, 1.0f),
                    //    DPD_SHADER_OPT_BLEND_STATE_ADDCOLOR
                    //);
                    m_FlashScreenMaskFrameMax = 50;
                    m_FlashScreenMaskFrame = m_FlashScreenMaskFrameMax;

                    CameraShakeRequestAll(
                        CAMERA_SHAKE_EXLARGE_FRAME * 2,
                        CAMERA_SHAKE_EXLARGE_POWER * 2
                    );

                    //ムービーフラグ
                    GameManagerSetMovieFlag(false);

                    SetCameraModeAll(CAMERA_MODE_BOSS_CHASE);


                    GO_CreateExplosion(m_Obj->pos, 10.0f);

                    {
                        Stage_Base* stg = GetCurrentStageSceneGame();
                        if (stg != nullptr)
                        {
                            Stage_Ruins* stgRuins = dynamic_cast<Stage_Ruins*>(stg);
                            if (stgRuins != nullptr)
                            {
                                stgRuins->StageEventDestroy();
                            }
                        }
                    }
                }

                if (m_SuperBeamAfterFrame == FRAME_ONE_SECOND * 1 + 21)
                {
                    {
                        int summonEnemyNum = 2;
                        for (int i = 0; i < summonEnemyNum; i++)
                        {

                            int summonEnemyNum2 = 3 + 4 * i;
                            for (int j = 0; j < summonEnemyNum2; j++)
                            {
                                float rad = 2.0f * SYS_GENERAL_PI / summonEnemyNum2 * j;

                                Float2 dis = {
                                    (BLOCK_WIDTH * 24) + (BLOCK_WIDTH * 48) * i,
                                    (BLOCK_WIDTH * 16) + (BLOCK_WIDTH * 36) * i,
                                };

                                if (j % 2 == 0)
                                {
                                    dis.x *= 0.75f;
                                    dis.y *= 0.75f;
                                }


                                GAME_OBJECT_COLLIDER collider = {};
                                collider.type = GAME_OBJECT_COLLIDER_TYPE_CAPSULE;

                                collider.capsule.radius = BLOCK_WIDTH
                                    * 4.0f
                                    ;

                                Float3 capVec = {
                                    sin(rad),
                                    cos(rad),
                                    0.0f
                                };
                                collider.capsule.pointA = capVec * BLOCK_WIDTH * +4.0f;
                                collider.capsule.pointB = capVec * BLOCK_WIDTH * -4.0f;
                                collider.capsule.pointA.z = collider.capsule.radius * 0.75f;
                                collider.capsule.pointB.z = collider.capsule.radius * 0.75f;

                                GOE_ObjectDynamic* goe = GOE_ObjectDynamic::CreateObjectDynamic(
                                    GOE_ObjectDynamic::Type::General,
                                    MkF3(
                                        sin(rad) * dis.x,
                                        cos(rad) * dis.y,
                                        BLOCK_WIDTH * -100.0f
                                    ),
                                    collider
                                );
                                goe->SetOwner(GetIdentifierGameObject(m_Obj));
                                goe->StartFall(2 * FRAME_ONE_SECOND);

                                GameObjectHitStop(
                                    goe->GetGameObj(),
                                    25 * i + 20 * j + 40
                                );

                            }
                        }
                    }
                    { //判定
                        std::map<int, GAME_OBJECT*> objs = GetGameObjectFindByDistance(m_Obj->pos, ONE_BLOCK * 4096);
                        for (int i = 0; i < objs.size(); i++)
                        {
                            if (objs[i]->status.statusType != GAME_OBJECT_STATUS_TYPE_ENEMY_BOSS)
                            {
                                //有効なくらい判定を探す
                                GAME_OBJECT_COLLISION_RECIEVE_CHILD colRec = {};
                                for (int j = 0; j < GAME_OBJECT_COLLISION_NUM_MAX; j++)
                                {
                                    if (objs[i]->collision.rec.collision[j].enable)
                                    {
                                        colRec = objs[i]->collision.rec.collision[j];
                                        break;
                                    }
                                }

                                if (colRec.enable)
                                { //判定が有効なら処理する

                                    Character::MOTION_COL_DAT motionColDetails =
                                        Character::GetMotionColDataFindByName(
                                            Character::MOTION_COL_TYPE_ATK,
                                            BLADE_ATK_COL_NAME
                                        );
                                    motionColDetails.atkCol.hitStopAttacker = 0;
                                    motionColDetails.atkCol.invIgnore = true;

                                    if (objs[i]->tags.size() > 0 && objs[i]->tags[0].find("PLAYER") == 0)
                                    {
                                        if (m_SuperBeamFinish)
                                        {
                                            motionColDetails.atkCol.power
                                                = std::max((int)(objs[i]->status.hpMax * 100.0), 1);
                                        }
                                        else if (m_IsHard)
                                        {
                                            motionColDetails.atkCol.power 
                                                = std::max((int)(objs[i]->status.hpMax * 0.8), 1);
                                        }
                                        else
                                        {
                                            motionColDetails.atkCol.power = 
                                                std::max((int)(objs[i]->status.hpMax * 0.5), 1);
                                        }
                                        strcpy_s(
                                            motionColDetails.atkCol.tag,
                                            GAME_OBJECT_TXT_MAX,
                                            "FATAL"
                                        );
                                    }
                                    else
                                    {
                                        motionColDetails.atkCol.power = objs[i]->status.hpMax * 100;
                                    }

                                    GameObjectDamageCollisionHit(objs[i], m_Obj,
                                        motionColDetails.atkCol,
                                        colRec
                                    );

                                }
                            }
                        }
                    }
                }
                if (m_SuperBeamAfterFrame == FRAME_ONE_SECOND * 1 + 35)
                {
                    m_SuperBeamScreenBreakFrameMax = 80;
                    m_SuperBeamScreenBreakFrame = m_SuperBeamScreenBreakFrameMax;
                }
                if (m_SuperBeamAfterFrame == FRAME_ONE_SECOND * 2)
                {
                    SoundBGMSetVolumeRequest(m_SuperBeamBgmVolumeBuf);
                    if (!m_IsHard)
                    {
                        //BGMを再生する
                        SoundBGMPlayRequest(m_bgmLoop,
                            true,
                            2.0f
                        );
                    }

                }
                if (m_SuperBeamAfterFrame == FRAME_ONE_SECOND * 3)
                {
                    m_SuperBeamAfterFrame = -2;

                    GE_GayaStartRequest("iseki_inperiusu_superbeam_failure_mayu");
                }
            }

            m_SuperBeamAfterFrame++;

            //終了
            if (m_SuperBeamAfterFrame < 0)
            {
                //カウントストップを解除
                //SetStageCountStopSceneGame(false);

                //プレイヤーの硬直を解除
                //GameManagerSetPlayerCtrlStop(false);

                //ムービーフラグ
                GameManagerSetMovieFlag(false);

                SetCameraModeAll(CAMERA_MODE_BOSS_CHASE);


                m_ActionCooldown = m_WazaCooldown;
                m_SuperBeamFrame = -1;
                m_SuperBeamStartFrame = -1;
                m_SuperBeamCountDownFrame = -1;
                m_SuperBeamAfterFrame = -1;
                m_CurrentState = ActionState::Idle;


                for (int i = 0; i < m_SuperBeamEnemiesSub.size(); i++)
                {
                    GAME_OBJECT* ob = GetGameObjectFindByIdentifier(m_SuperBeamEnemiesSub[i]);
                    if (IsActiveGameObject(ob) && !IsDefeatGameObject(ob))
                    {
                        DeleteGameObject(ob);
                    }
                }
                m_SuperBeamEnemiesSub.clear();
                m_SuperBeamEnemiesSub.shrink_to_fit();

                for (int i = 0; i < m_SuperBeamEnemies.size(); i++)
                {
                    GAME_OBJECT* ob = GetGameObjectFindByIdentifier(m_SuperBeamEnemies[i]);
                    if (IsActiveGameObject(ob) && !IsDefeatGameObject(ob))
                    {
                        DeleteGameObject(ob);
                    }
                }
                m_SuperBeamEnemies.clear();
                m_SuperBeamEnemies.shrink_to_fit();

                m_SuperBeamEnemiesFrame.clear();
                m_SuperBeamEnemiesFrame.shrink_to_fit();

            }
        }

        m_SuperBeamFrame++;
    }


    bool breakBarrierTmp = false;
    for (int i = 0; i < (m_SuperBeamEnemies.size() - aliveLegaNum); i++)
    {
        int index = m_SuperBeamEnemies.size() - i - 1;
        if (IsEnableBossbarBarrier_Game(index)
            && !IsBreakingBossbarBarrier_Game(index))
        {
            breakBarrierTmp = true;

            ShowBossbarBarrier_Game(index, 2);
        }
    }
    if (breakBarrierTmp)
    {
        CameraShakeRequestAll(
            CAMERA_SHAKE_MIDIUM_FRAME,
            CAMERA_SHAKE_EXLARGE_POWER * 2
        );
        SoundEffectPlayRequest(SOUND_TYPE_SE_9, 3.6f, 0, 0.7f);
    }

    for (int i = 0; i < m_SuperBeamEnemiesFrame.size(); i++)
    {
        if (i >= m_SuperBeamEnemies.size())
        {
            break;
        }

        GAME_OBJECT* co = GetGameObjectFindByIdentifier(
            m_SuperBeamEnemies[i]
        );

        
        if (IsActiveGameObject(co)
            && m_SuperBeamAfterFrame <= 0)
        {
            if (m_SuperBeamEnemiesFrame[i] < SUPER_BEAM_LEGABEAM_IN_FRAME)
            {
                m_SuperBeamEnemiesFrame[i]++;
            }
        }
        else
        {
            if (m_SuperBeamEnemiesFrame[i] > 0)
            {
                //if (m_SuperBeamEnemiesFrame[i] == SUPER_BEAM_LEGABEAM_IN_FRAME)
                //{
                //    CameraShakeRequestAll(
                //        CAMERA_SHAKE_MIDIUM_FRAME,
                //        CAMERA_SHAKE_EXLARGE_POWER * 2
                //    );
                //    ShowBossbarBarrier_Game(aliveLegaNum, 2);
                //}

                m_SuperBeamEnemiesFrame[i]--;
            }
        }
    }

    if (m_SuperBeamBarrierFlag)
    {
        if (m_SuperBeamBarrierFrame < SUPER_BEAM_BARRIER_IN_FRAME)
        {
            m_SuperBeamBarrierFrame++;
        }
    }
    else
    {
        if (m_SuperBeamBarrierFrame > 0)
        {
            m_SuperBeamBarrierFrame--;
        }
    }

    if (m_SuperBeamShootBgFrame > 0)
    {
        m_SuperBeamShootBgFrame--;
    }
    if (m_SuperBeamScreenBreakFrame > 0)
    {
        m_SuperBeamScreenBreakFrame--;
    }
}

void GOE_Character_Enemy_Boss_Inperiusu::draw_super_beam()
{

    if (
        (m_Anim_State == AnimState::SuperBeamBefore
            || (m_CurrentState == ActionState::SuperBeam
                && m_Anim_State == AnimState::SuperBeam)
        )
        && (m_SuperBeamAfterFrame < FRAME_ONE_SECOND * 2)
    )
    {
        {
            float inmod = 1.0f;
            if (m_Anim_State == AnimState::SuperBeamBefore)
            {
                inmod  = std::min((float)m_Anim_Frame / 30, 1.0f);
            }

            DRAW_POLYGON_DAT drawDatTmp = {};
            drawDatTmp.size = MkF3(
                500.0f * inmod,
                500.0f * inmod,
                1.0f);

            drawDatTmp.pos.y += 500.0f;
            drawDatTmp.pos.z += -1000.0f;

            drawDatTmp.rot = 0.0f;

            drawDatTmp.rot3D = MkF3(0.0f, 0.0f, 0.0f);

            drawDatTmp.color = MkF4(1.0f, 1.0f, 1.0f, 0.6f);

            drawDatTmp.texNo = m_Tex_BeamCharge;
            drawDatTmp.loadTexType = LOADTEXTURETYPE_MAIN;

            drawDatTmp.texUV = AnimationUV(
                m_Obj->frameActive,
                ANIMCOUNT_BEAM_CHARGE_X,
                ANIMCOUNT_BEAM_CHARGE_Y
            );
            drawDatTmp.texWH = {
                1.0f / ANIMCOUNT_BEAM_CHARGE_X,
                1.0f / ANIMCOUNT_BEAM_CHARGE_Y
            };

            drawDatTmp.startPos3D = m_Obj->pos;

            //drawDatTmp.startRot = m_CurrentAxisEx * 45.0f;
            drawDatTmp.startRot3D = MkF3(0.0f, 0.0f, 0.0f);

            drawDatTmp.startScale = MkF2(1.0f, 1.0f);
            drawDatTmp.startScale3D = MkF3(1.0f, 1.0f, 1.0f);

            drawDatTmp.modelNo = 0;

            drawDatTmp.shaderOpt.bloom = DPD_SHADER_OPT_BLOOM_NORMAL;
            drawDatTmp.shaderOpt.blendState = DPD_SHADER_OPT_BLEND_STATE_ADDCOLOR;

            DRAW_POLYGON_IN_CAMERA_EX_OPTION opt = {};
            opt.reverse = false;
            opt.fixed = false;
            opt.enableShade = true;
            opt.billboard = true;
            //opt.priority = DRAW_POLYGON_PRIORITY_NML_u2;
            opt.priority = DRAW_POLYGON_PRIORITY_NML_ALWAYS;
            opt.depthBuffIgnore = true;
            opt.shaderType = DPD_SHADER_TYPE_EFFECT;

            //DrawPolygonInCameraEx(drawDatTmp, opt);

            {
                DRAW_POLYGON_DAT drawDatTmp2 = drawDatTmp;
                //drawDatTmp2.pos.x *= 0.55f;
                //drawDatTmp2.pos.y *= 0.55f;
                //drawDatTmp2.pos.x *= 0.75f;
                //drawDatTmp2.pos.y *= 0.75f;
                drawDatTmp2.size = MkF3(
                    1200.0f * inmod,
                    1200.0f * inmod,
                    1.0f);
                if (m_Anim_State == AnimState::SuperBeam)
                {
                    float ebsmod = 1.0f + (0.05f * (m_SuperBeamAfterFrame - FRAME_ONE_SECOND));
                    ebsmod = pow(ebsmod, 3);

                    drawDatTmp2.size.x *= ebsmod;
                    drawDatTmp2.size.y *= ebsmod;
                }
                if (m_SuperBeamSuccessWeaknessFlag
                    && m_SuperBeamAfterFrame >= 0)
                {
                    float ebsmod = 1.0f 
                        + (0.016f * (m_SuperBeamAfterFrame - FRAME_ONE_SECOND))
                        + sin((float)(m_SuperBeamAfterFrame - FRAME_ONE_SECOND) / 4.0f)
                        * 0.16f
                        ;
                    ebsmod = pow(ebsmod, 1.5f);

                    drawDatTmp2.size.x *= ebsmod;
                    drawDatTmp2.size.y *= ebsmod;
                }

                drawDatTmp2.texNo = m_Tex_EnergyBall;
                drawDatTmp2.texUV = AnimationUV(m_Obj->frameActive, 4, 4);
                drawDatTmp2.texWH = { 1.0f / 4, 1.0f / 4 };
                drawDatTmp2.color = MkF4(1.0f, 1.0f, 1.0f, 1.0f);
                DRAW_POLYGON_IN_CAMERA_EX_OPTION opt2 = opt;
                opt2.shaderType = DPD_SHADER_TYPE_EFFECT;
                //opt2.priority = DRAW_POLYGON_PRIORITY_NML_u3;

                DrawPolygonInCameraEx(drawDatTmp2, opt2);
            }
            {
                int ind = 11 * std::min(
                    (float)m_SuperBeamCountDownFrame / m_SuperBeamLimitF,
                    1.0f) - 1;
                if (ind >= 0)
                {
                    DRAW_POLYGON_DAT drawDatTmp2 = drawDatTmp;
                    drawDatTmp2.size = MkF3(
                        1900.0f * inmod,
                        1900.0f * inmod,
                        1.0f);
                    drawDatTmp2.texNo = m_Tex_SuperBeamChargeRing.textureId;

                    drawDatTmp2.texUV = AnimationUV(
                        ind,
                        4, 3);
                    drawDatTmp2.texWH = { 1.0f / 4, 1.0f / 3 };

                    drawDatTmp2.color = MkF4(2.0f, 0.5f, 2.0f, 1.0f);
                    {
                        float alphamod = (sin((float)m_Obj->frame * 0.65f) / 2.0f + 0.5f);
                        drawDatTmp2.color.x *= 18.0f * alphamod;
                        drawDatTmp2.color.y *= 18.0f * alphamod;
                        drawDatTmp2.color.z *= 18.0f * alphamod;
                    }

                    //drawDatTmp2.shaderOpt.blendState = DPD_SHADER_OPT_BLEND_STATE_DEFAULT;

                    DRAW_POLYGON_IN_CAMERA_EX_OPTION opt2 = opt;
                    //opt2.priority = DRAW_POLYGON_PRIORITY_NML_u3;
                    opt2.shaderType = DPD_SHADER_TYPE_EFFECT;

                    DrawPolygonInCameraEx(drawDatTmp2, opt2);
                }
            }

            
        }
    }
    if (m_SuperBeamShootBgFrame > 0 && m_SuperBeamShootBgFrameMax > 0)
    {
        float mod = (float)m_SuperBeamShootBgFrame / m_SuperBeamShootBgFrameMax;
        {
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
                dsdat.color = { 0.0f, 0.0f, 0.0f, 0.8f };
                dsdat.color.w *= mod;

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


                DrawPolygonInCameraEx(dsdat, opt);
            }

        }
    }
    if (m_SuperBeamScreenBreakFrame > 0 && m_SuperBeamScreenBreakFrameMax > 0)
    {
        float mod = (float)m_SuperBeamScreenBreakFrame
            / m_SuperBeamScreenBreakFrameMax;
        {
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
                //dsdat.color = { 1.0f, 1.0f, 1.0f, 1.0f };
                dsdat.color = { 
                    12.0f,
                    12.0f, 
                    12.0f,
                    1.0f };
                //dsdat.color.w *= mod;

                dsdat.texNo = m_Tex_ScreenBreak.textureId;
                dsdat.loadTexType = LOADTEXTURETYPE_MAIN;

                dsdat.texUV = AnimationUV(
                    16 * (1.0f - mod),
                    4,
                    5
                );
                dsdat.texWH = {
                    1.0f / 4,
                    1.0f / 5
                };

                dsdat.startPos = {
                    0.0f,
                    0.0f
                };
                dsdat.startRot = 0;
                dsdat.startScale = { 1.0f, 1.0f };
                dsdat.startScale3D = { 1.0f, 1.0f, 1.0f };

                dsdat.shaderOpt.bloom = DPD_SHADER_OPT_BLOOM_NONE;
                dsdat.shaderOpt.blendState = DPD_SHADER_OPT_BLEND_STATE_ADDCOLOR;

                DRAW_POLYGON_IN_CAMERA_EX_OPTION opt = {};
                opt.reverse = false;
                opt.fixed = true;
                opt.depthBuffIgnore = true;
                opt.priority = DRAW_POLYGON_PRIORITY_FORCE_NORMAL;
                opt.shaderType = DPD_SHADER_TYPE_EFFECT;


                DrawPolygonInCameraEx(dsdat, opt);
            }

        }
    }

    for (int i = 0; i < m_SuperBeamEnemiesFrame.size(); i++)
    {
        if (i >= m_SuperBeamEnemiesPos.size())
        {
            break;
        }

        GAME_OBJECT* co = nullptr;
        if (i < m_SuperBeamEnemies.size())
        {
            co = GetGameObjectFindByIdentifier(
                m_SuperBeamEnemies[i]
            );
        }

        //if (co == nullptr)
        //{
        //    continue;
        //}

        float startMod = EasingInOutSine(
            (float)(m_SuperBeamEnemiesFrame[i])
            / SUPER_BEAM_LEGABEAM_IN_FRAME
        );

        {
            DRAW_POLYGON_DAT drawDatBaseTmp = {};


            drawDatBaseTmp.rot = 0.0f;

            drawDatBaseTmp.rot3D = MkF3(0.0f, 0.0f, 0.0f);

            drawDatBaseTmp.color = MkF4(0.6f, 0.0f, 0.0f, 0.9f);

            drawDatBaseTmp.texNo = 0;
            drawDatBaseTmp.loadTexType = LOADTEXTURETYPE_MAIN;

            drawDatBaseTmp.texUV = { 0.0f, 0.0f };
            drawDatBaseTmp.texWH = { 1.0f, 1.0f };

            drawDatBaseTmp.startPos3D = m_SuperBeamEnemiesPos[i];
            //drawDatBaseTmp.startPos3D.y += -480.0f;
            drawDatBaseTmp.startPos3D.y += -300.0f;
            //drawDatBaseTmp.startPos3D.z += -1100.0f;
            drawDatBaseTmp.startPos3D.z += -300.0f;

            Float3 tarpos = m_Obj->pos;
            tarpos.z += -800.0f + m_ModelTransPos[1].y;

            drawDatBaseTmp.size = MkF3(
                12.0f, 12.0f, 
                CalculateDistance(tarpos,
                    drawDatBaseTmp.startPos3D) * 0.35f
            );
            drawDatBaseTmp.pos.z = drawDatBaseTmp.size.z * 0.4f;

            Float3 rotvec = {
                tarpos.x - drawDatBaseTmp.startPos3D.x,
                tarpos.y - drawDatBaseTmp.startPos3D.y,
                tarpos.z - drawDatBaseTmp.startPos3D.z
            };
            rotvec = NormalizeVector(rotvec);

            drawDatBaseTmp.startRot = CalculateRadToDeg(atan2(
                rotvec.y, rotvec.x
            )) - 90.0f;
            drawDatBaseTmp.startRot3D = MkF3(
                90.0f + 45.0f * rotvec.z,
                0.0f, 0.0f);

            drawDatBaseTmp.startScale = MkF2(1.0f, 1.0f);
            drawDatBaseTmp.startScale3D = MkF3(1.0f, 1.0f, 1.0f);

            drawDatBaseTmp.modelNo = m_Model_Beam;

            drawDatBaseTmp.shaderOpt.bloom = DPD_SHADER_OPT_BLOOM_NORMAL;
            drawDatBaseTmp.shaderOpt.blendState = DPD_SHADER_OPT_BLEND_STATE_ADDCOLOR;

            DRAW_POLYGON_IN_CAMERA_EX_OPTION optBaseTmp = {};
            optBaseTmp.reverse = false;
            optBaseTmp.fixed = false;
            optBaseTmp.enableShade = true;
            //optBaseTmp.priority = DRAW_POLYGON_PRIORITY_NML_u3;
            optBaseTmp.priority = DRAW_POLYGON_PRIORITY_NORMAL;
            //optBaseTmp.depthBuffIgnore = true;
            //opt.priority = DRAW_POLYGON_PRIORITY_FORCE_NORMAL;
            optBaseTmp.shaderType = DPD_SHADER_TYPE_EFFECT;

            //予測エフェクト
            {
                {
                    DRAW_POLYGON_DAT drawDatTmp2 = drawDatBaseTmp;
                    drawDatTmp2.texNo = m_Tex_Beam1;
                    //drawDatTmp2.size.x = 10.0f * startMod;
                    //drawDatTmp2.size.y = 10.0f * startMod;
                    drawDatTmp2.size.x = 64.0f;
                    drawDatTmp2.size.y = 64.0f;

                    drawDatTmp2.color = MkF4(2.0f, 0.2f, 2.2f, 0.75f);
                    if (co != nullptr)
                    {
                        float buf = drawDatTmp2.size.z;
                        drawDatTmp2.size.z *= startMod;
                        buf = buf - drawDatTmp2.size.z;
                        drawDatTmp2.pos.z += buf / -2;
                    }
                    else
                    {
                        drawDatTmp2.color.w *= startMod;
                    }

                    drawDatTmp2.texUV.y += 0.03f * m_Obj->frameActive;

                    drawDatTmp2.shaderOpt.blendState = DPD_SHADER_OPT_BLEND_STATE_ADDCOLOR;
                    DRAW_POLYGON_IN_CAMERA_EX_OPTION opt2 = optBaseTmp;
                    opt2.priority = DRAW_POLYGON_PRIORITY_NORMAL;
                    DrawPolygonInCameraEx(drawDatTmp2, opt2);
                }
            }

        }

        {
            DRAW_POLYGON_DAT drawDatTmp = {};
            drawDatTmp.size = MkF3(
                850.0f * startMod,
                850.0f * startMod,
                1.0f);

            drawDatTmp.rot = 0.0f;

            drawDatTmp.rot3D = MkF3(0.0f, 0.0f, 0.0f);

            //drawDatTmp.color = MkF4(1.0f, 1.0f, 1.0f, 0.6f);

            drawDatTmp.loadTexType = LOADTEXTURETYPE_MAIN;

            drawDatTmp.startPos3D = m_SuperBeamEnemiesPos[i];
            drawDatTmp.startPos3D.y += -300.0f;
            //drawDatTmp.startPos3D.z += -1100.0f;
            drawDatTmp.startPos3D.z += -320.0f;
            drawDatTmp.size.x *= 3.2f;
            drawDatTmp.size.y *= 3.2f;

            drawDatTmp.startRot3D = MkF3(0.0f, 0.0f, 0.0f);

            drawDatTmp.startScale = MkF2(1.0f, 1.0f);
            drawDatTmp.startScale3D = MkF3(1.0f, 1.0f, 1.0f);

            drawDatTmp.modelNo = 0;

            drawDatTmp.shaderOpt.bloom = DPD_SHADER_OPT_BLOOM_NORMAL;
            drawDatTmp.shaderOpt.blendState = DPD_SHADER_OPT_BLEND_STATE_ADDCOLOR;

            DRAW_POLYGON_IN_CAMERA_EX_OPTION opt = {};
            opt.reverse = false;
            opt.fixed = false;
            opt.billboard = true;
            //opt.priority = DRAW_POLYGON_PRIORITY_NML_u1;
            opt.priority = DRAW_POLYGON_PRIORITY_NML_d1;
            //opt.priority = DRAW_POLYGON_PRIORITY_NML_ALWAYS;
            opt.depthBuffIgnore = true;
            opt.shaderType = DPD_SHADER_TYPE_EFFECT;

            {
                DRAW_POLYGON_DAT drawDatTmp2 = drawDatTmp;

                drawDatTmp2.texNo = m_Tex_EnergyBall;
                drawDatTmp2.texUV = AnimationUV(m_Obj->frameActive / 3, 4, 4);
                drawDatTmp2.texWH = { 1.0f / 4, 1.0f / 4 };
                //drawDatTmp2.color = MkF4(1.0f, 1.0f, 1.0f, 1.0f);
                drawDatTmp2.color = MkF4(1.0f, 1.0f, 1.0f, 0.8f);
                DRAW_POLYGON_IN_CAMERA_EX_OPTION opt2 = opt;
                //opt2.priority = DRAW_POLYGON_PRIORITY_NML_u3;

                DrawPolygonInCameraEx(drawDatTmp2, opt2);
            }
        }

        {
            DRAW_POLYGON_DAT drawDatTmp = {};
            drawDatTmp.size = MkF3(
                400.0f,
                400.0f,
                1.0f);

            drawDatTmp.rot = 0.0f;

            drawDatTmp.rot3D = MkF3(0.0f, 0.0f, 0.0f);

            drawDatTmp.color = MkF4(1.0f, 1.0f, 1.0f, 1.0f * startMod);

            drawDatTmp.loadTexType = LOADTEXTURETYPE_MAIN;

            drawDatTmp.startPos3D = m_SuperBeamEnemiesPos[i];
            drawDatTmp.startPos3D.y += -400.0f;
            drawDatTmp.startPos3D.z += -1200.0f 
                + sinf((float)m_Obj->frameActive / 10.0f) * 80.0f;

            drawDatTmp.startRot3D = MkF3(0.0f, 0.0f, 0.0f);

            drawDatTmp.startScale = MkF2(1.0f, 1.0f);
            drawDatTmp.startScale3D = MkF3(1.0f, 1.0f, 1.0f);

            drawDatTmp.modelNo = 0;

            //drawDatTmp.shaderOpt.bloom = DPD_SHADER_OPT_BLOOM_NORMAL;
            drawDatTmp.shaderOpt.blendState = DPD_SHADER_OPT_BLEND_STATE_DEFAULT;

            DRAW_POLYGON_IN_CAMERA_EX_OPTION opt = {};
            opt.reverse = false;
            opt.fixed = false;
            opt.billboard = true;
            opt.priority = DRAW_POLYGON_PRIORITY_NML_ALWAYS;
            opt.depthBuffIgnore = true;
            opt.shaderType = DPD_SHADER_TYPE_EFFECT;

            {
                DRAW_POLYGON_DAT drawDatTmp2 = drawDatTmp;

                drawDatTmp2.texNo = m_Tex_WarningPoint.textureId;
                drawDatTmp2.texUV = { 0.0f, 0.0f };
                drawDatTmp2.texWH = { 1.0f, 1.0f };
                DRAW_POLYGON_IN_CAMERA_EX_OPTION opt2 = opt;
                //opt2.priority = DRAW_POLYGON_PRIORITY_NML_u3;

                DrawPolygonInCameraEx(drawDatTmp2, opt2);
            }
        }
    }

    DRAW_POLYGON_DAT drawDatTmp = {};
    drawDatTmp.size = MkF3(
        ONE_BLOCK * 1.3f,
        ONE_BLOCK * 1.3f,
        ONE_BLOCK * 1.3f
    );
    drawDatTmp.rot = 0.0f;

    drawDatTmp.rot3D = MkF3(0.0f, 0.0f, 0.0f);

    drawDatTmp.texWH = { 1.0f / 1, 1.0f / 1 };

    drawDatTmp.texUV = {
        0.0f,
        0.0f
    };
    drawDatTmp.color = MkF4(0.3f, -0.3f, 0.7f, 1.0f);

    drawDatTmp.texNo = 0;

    drawDatTmp.loadTexType = LOADTEXTURETYPE_MAIN;

    drawDatTmp.startPos3D = m_Obj->pos;
    drawDatTmp.startPos3D.z = ONE_BLOCK * -3.0f;

    drawDatTmp.startRot3D = MkF3(-90.0f, 0.0f, 180.0f);

    drawDatTmp.startScale = MkF2(1.0f, 1.0f);
    drawDatTmp.startScale3D = MkF3(1.0f, 1.0f, 1.0f);

    drawDatTmp.modelNo = m_Model_Sphere;

    drawDatTmp.shaderOpt.bloom = DPD_SHADER_OPT_BLOOM_NONE;
    drawDatTmp.shaderOpt.blendState = DPD_SHADER_OPT_BLEND_STATE_ADDCOLOR;

    DRAW_POLYGON_IN_CAMERA_EX_OPTION opt = {};
    opt.reverse = false;
    opt.fixed = false;
    opt.enableShade = true;
    opt.billboard = false;
    //opt.priority = DRAW_POLYGON_PRIORITY_NORMAL;
    opt.priority = DRAW_POLYGON_PRIORITY_NML_u1;
    //opt.priority = DRAW_POLYGON_PRIORITY_NML_d3;
    //opt.depthBuffIgnore = true;
    opt.shaderType = DPD_SHADER_TYPE_EFFECT;

    //if (m_SuperBeamBarrierFlag)
    if (m_SuperBeamBarrierFrame > 0)
    {

        float barrMod = (float)m_SuperBeamBarrierFrame / SUPER_BEAM_BARRIER_IN_FRAME;

        float barrModSpl = 0.7f;
        float barrMod1 = std::min(
            barrMod / barrModSpl, 
            1.0f);
        float barrMod2 = std::max(
            (barrMod - barrModSpl) / (1.0f - barrModSpl),
            0.0f);

        {
            DRAW_POLYGON_DAT drawDatTmp2 = drawDatTmp;
            drawDatTmp2.size = MkF3(
                ONE_BLOCK * 37.0f,
                ONE_BLOCK * 37.0f,
                ONE_BLOCK * 37.0f
            );
            drawDatTmp2.startPos3D.z += ONE_BLOCK * -4.0f;
            drawDatTmp2.texNo = m_Tex_Barrier_Bg;
            drawDatTmp2.modelNo = 0;
            drawDatTmp2.startRot3D = MkF3(0.0f, 0.0f, 0.0f);
            drawDatTmp2.color = MkF4(0.4f, 0.1f, 0.5f, 
                0.5f * barrMod2
            );

            drawDatTmp2.shaderOpt.blendState = DPD_SHADER_OPT_BLEND_STATE_DEFAULT;

            DRAW_POLYGON_IN_CAMERA_EX_OPTION opt2 = opt;
            opt2.priority = (DRAW_POLYGON_PRIORITY)(DRAW_POLYGON_PRIORITY_NML_DEEPEST + 2);
            opt2.billboard = true;
            opt2.depthBuffIgnore = true;

            if (drawDatTmp2.color.w > 0)
            {
                DrawPolygonInCameraEx(drawDatTmp2, opt2);
            }
        }

        {
            DRAW_POLYGON_DAT drawDatTmp2 = drawDatTmp;
            drawDatTmp2.size = MkF3(
                ONE_BLOCK * 32.0f,
                ONE_BLOCK * 32.0f,
                ONE_BLOCK * 32.0f
            );
            drawDatTmp2.startPos3D.z = -20.0f;
            drawDatTmp2.texNo = m_Tex_Barrier_Ground1;
            drawDatTmp2.modelNo = 0;
            drawDatTmp2.startRot3D = MkF3(0.0f, 0.0f, 0.0f);
            drawDatTmp2.color = MkF4(
                0.3f, -0.4f, 0.4f,
                2.0f * barrMod2
            );
            //drawDatTmp2.shaderOpt.blendState = DPD_SHADER_OPT_BLEND_STATE_DEFAULT;

            DRAW_POLYGON_IN_CAMERA_EX_OPTION opt2 = opt;
            opt2.priority = DRAW_POLYGON_PRIORITY_NML_d1;

            if (drawDatTmp2.color.w > 0)
            {
                DrawPolygonInCameraEx(drawDatTmp2, opt2);
            }


            DRAW_POLYGON_DAT drawDatTmp3 = drawDatTmp2;
            drawDatTmp3.size.x *= 2.0f;
            drawDatTmp3.size.y *= 2.0f;
            drawDatTmp3.size.z *= 2.0f;
            drawDatTmp3.startPos3D.z += -20.0f;
            drawDatTmp3.texNo = m_Tex_Barrier_Ground2;
            DRAW_POLYGON_IN_CAMERA_EX_OPTION opt3 = opt2;
            opt3.priority = DRAW_POLYGON_PRIORITY_NML_d2;

            if (drawDatTmp3.color.w > 0)
            {
                DrawPolygonInCameraEx(drawDatTmp3, opt3);
            }
        }

        if (barrMod <= barrModSpl)
        {
            {
                DRAW_POLYGON_DAT drawDatTmp2 = drawDatTmp;
                drawDatTmp2.texNo = m_Tex_Barrier_Fresnel2;
                drawDatTmp2.color = MkF4(10.0f, 10.0f, 10.0f, 0.9f);
                drawDatTmp2.texUV = {
                    0.0f,
                    0.0f + 0.99f * barrMod1
                };
                drawDatTmp2.texWH = {
                    1.0f,
                    1.0f - 0.99f * barrMod1
                };

                DRAW_POLYGON_IN_CAMERA_EX_OPTION opt2 = opt;
                opt2.priority = DRAW_POLYGON_PRIORITY_NML_u1;

                DrawPolygonInCameraEx(drawDatTmp2, opt2);
            }
        }
        else
        {
            {
                DRAW_POLYGON_DAT drawDatTmp2 = drawDatTmp;
                drawDatTmp2.texNo = m_Tex_Barrier_Fresnel1;
                drawDatTmp2.color = MkF4(
                    +0.3f + 10.0f * (1.0f - barrMod2),
                    -0.3f + 10.0f * (1.0f - barrMod2),
                    +0.7f + 10.0f * (1.0f - barrMod2),
                    2.0f
                );
                DRAW_POLYGON_IN_CAMERA_EX_OPTION opt2 = opt;
                opt2.shaderType = DPD_SHADER_TYPE_EFFECT_FRESNEL;
                opt2.priority = DRAW_POLYGON_PRIORITY_NML_u1;

                DrawPolygonInCameraEx(drawDatTmp2, opt2);
            }
            {
                DRAW_POLYGON_DAT drawDatTmp2 = drawDatTmp;
                drawDatTmp2.size.x *= 1.01f;
                drawDatTmp2.size.y *= 1.01f;
                drawDatTmp2.size.z *= 1.01f;
                drawDatTmp2.texNo = m_Tex_Barrier_Fresnel2;
                drawDatTmp2.color = MkF4(
                    +0.1f + 10.0f * (1.0f - barrMod2),
                    -0.4f + 10.0f * (1.0f - barrMod2),
                    +0.2f + 10.0f * (1.0f - barrMod2),
                    2.0f
                );
                DRAW_POLYGON_IN_CAMERA_EX_OPTION opt2 = opt;
                opt2.shaderType = DPD_SHADER_TYPE_EFFECT_FRESNEL;
                opt2.priority = DRAW_POLYGON_PRIORITY_NML_u4;

                DrawPolygonInCameraEx(drawDatTmp2, opt2);
            }

            {
                DRAW_POLYGON_DAT drawDatTmp2 = drawDatTmp;
                drawDatTmp2.size.x *= 1.02f;
                drawDatTmp2.size.y *= 1.02f;
                drawDatTmp2.size.z *= 1.02f;
                drawDatTmp2.texUV.y = -0.008f * m_Obj->frameActive;
                drawDatTmp2.texNo = m_Tex_Barrier_Scroll1;
                drawDatTmp2.color = MkF4(
                    0.32f + 10.0f * (1.0f - barrMod2), 
                    -0.2f + 10.0f * (1.0f - barrMod2), 
                    +0.4f + 10.0f * (1.0f - barrMod2),
                    2.0f
                );
                //drawDatTmp2.shaderOpt.blendState = DPD_SHADER_OPT_BLEND_STATE_DEFAULT;
                DRAW_POLYGON_IN_CAMERA_EX_OPTION opt2 = opt;
                opt2.priority = DRAW_POLYGON_PRIORITY_NML_u2;

                DrawPolygonInCameraEx(drawDatTmp2, opt2);

                DRAW_POLYGON_DAT drawDatTmp3 = drawDatTmp2;
                drawDatTmp3.size.x *= 1.05f;
                drawDatTmp3.size.y *= 1.05f;
                drawDatTmp3.size.z *= 1.05f;
                drawDatTmp3.texNo = m_Tex_Barrier_Scroll2;
                drawDatTmp3.color = MkF4(
                    0.02f + 10.0f * (1.0f - barrMod2),
                    +0.5f + 10.0f * (1.0f - barrMod2),
                    0.35f + 10.0f * (1.0f - barrMod2),
                    2.0f
                );
                drawDatTmp3.shaderOpt.blendState = DPD_SHADER_OPT_BLEND_STATE_ADDCOLOR;
                DRAW_POLYGON_IN_CAMERA_EX_OPTION opt3 = opt2;
                opt3.priority = DRAW_POLYGON_PRIORITY_NML_u3;

                DrawPolygonInCameraEx(drawDatTmp3, opt3);
            }
        }
    }
}

void GOE_Character_Enemy_Boss_Inperiusu::update_machinegun()
{
    constexpr int shootNum = 8;
    constexpr int shootDelay = 10;

    m_BeforeVecBuf = m_TargetVec;

    Float3 bposBase = MkF3(
        m_Obj->pos.x + m_BeforeVec.x * ONE_BLOCK * 2.5f,
        m_Obj->pos.y + m_BeforeVec.y * ONE_BLOCK * 2.5f,
        m_Obj->pos.z + m_BeforeVec.z * ONE_BLOCK * 2.5f - ONE_BLOCK * 9.0f - ONE_BLOCK * 3.9f
    );

    for (int i = 0; i < 2; i++)
    {
        m_MachineGunShootPos[i] = bposBase;

        //float rad = atan2(bposBase.y, bposBase.x);
        //m_MachineGunShootPos[i].x += sin(rad) * ONE_BLOCK * 1.0f * (i == 1 ? -1.0f : 1.0f);
        //m_MachineGunShootPos[i].y += cos(rad) * ONE_BLOCK * 1.0f * (i == 1 ? -1.0f : 1.0f);
        float rad = atan2(m_BeforeVecBuf.y, m_BeforeVecBuf.x) + CalculateDegToRad(90.0f);
        m_MachineGunShootPos[i].x += sin(rad) * ONE_BLOCK * 1.0f * (i == 1 ? -1.0f: 1.0f);
        m_MachineGunShootPos[i].y += cos(rad) * ONE_BLOCK * 1.0f * (i == 1 ? -1.0f: 1.0f);
    }

    GAME_OBJECT* targetObject = GetGameObjectFindByIdentifier(m_TargetObjIdentifier);
    if (targetObject != nullptr)
    {

        for (int i = 0; i < 2; i++)
        {
            Float3 postmp = {};
            postmp.x = m_Obj->pos.x + m_BeforeVec.x * m_TargetDistance;
            postmp.y = m_Obj->pos.y + m_BeforeVec.y * m_TargetDistance;
            postmp.z = targetObject->pos.z;

            //m_MachineGunShootVec[i] = NormalizeVector(MkF3(
            //    targetObject->pos.x - m_MachineGunShootPos[i].x,
            //    targetObject->pos.y - m_MachineGunShootPos[i].y,
            //    targetObject->pos.z - m_MachineGunShootPos[i].z - 80.0f
            //));
            m_MachineGunShootVec[i] = NormalizeVector(MkF3(
                postmp.x - m_MachineGunShootPos[i].x,
                postmp.y - m_MachineGunShootPos[i].y,
                postmp.z - m_MachineGunShootPos[i].z - 80.0f
            ));
        }
    }
    //m_MachineGunShootVec = m_TargetVec;

    if (m_MachineGunFrame == 1)
    {
        //SEを再生
        SoundEffectPlayRequest(GetSoundData("SOUND/SE/inperiusu/b_kamae"), 1.3f, 0);

        StartAnim(AnimState::MachinegunBefore,
            FRAME_MACHINEGUN_START,
            true);
    }
    if (m_MachineGunFrame == FRAME_MACHINEGUN_START + 1)
    {
        //SEを再生
        SoundEffectPlayRequest(GetSoundData("SOUND/SE/inperiusu/b_charge"), 0.75f, 0);

        StartAnim(AnimState::Machinegun,
            FRAME_MACHINEGUN_SHOOT_END - FRAME_MACHINEGUN_SHOOT_START + FRAME_MACHINEGUN_AFTER,
            true);
    }
    if (m_MachineGunFrame == FRAME_MACHINEGUN_SHOOT_START + 1)
    {
        //SEを再生
        SoundEffectPlayRequest(GetSoundData("SOUND/SE/enemy/attack/LaserRapid"), 1.4f, 0);

    }


    if (m_MachineGunFrame >= FRAME_MACHINEGUN_SHOOT_START + 1)
    {
        if ((m_MachineGunFrame - (FRAME_MACHINEGUN_SHOOT_START + 1)) % shootDelay == 0)
        {
            CameraShakeRequestAll(CAMERA_SHAKE_MIDIUM_FRAME, CAMERA_SHAKE_MIDIUM_POWER);

            int bIndex = ((m_MachineGunFrame / shootDelay) % 2 == 0) ? 1 : 0;
            GOE_Character_EnemyProjectile* goepro =
                GOE_Character_EnemyProjectile::CreateProjectile(
                    GOE_Character_EnemyProjectile::Type::Beam,
                    m_MachineGunShootPos[bIndex],
                    m_MachineGunShootVec[bIndex]
                );
            if (goepro != nullptr)
            {
                GAME_OBJECT* beamObj = goepro->GetGameObj();
                if (beamObj != nullptr)
                {
                    beamObj->scale = MkF3(
                        0.6f,
                        0.6f,
                        1.9f
                    );
                    {
                        Character::MOTION_COL_DAT motionColDetails =
                            Character::GetMotionColDataFindByName(
                                Character::MOTION_COL_TYPE_ATK,
                                MACHINEGUN_ATK_COL_NAME
                            );

                        GAME_OBJECT_COLLISION_ATTACK_CHILD attackColTmp = motionColDetails.atkCol;
                        attackColTmp.subjectFamily[GAME_OBJECT_FAMILY_PLAYER] = true;

                        attackColTmp.size = MkF3(
                            ONE_BLOCK * 3,
                            ONE_BLOCK * 3,
                            ONE_BLOCK * 3
                        );
                        attackColTmp.pos.z = -attackColTmp.size.z / 2;
                        attackColTmp.pos.y += attackColTmp.size.y / 2 * 1.0f;

                        beamObj->collision.atk.enable = true;
                        beamObj->collision.atk.collision[0] = attackColTmp;
                        beamObj->collision.atk.collision[0].enable = true;
                    }
                    beamObj->blockCollisionIgnore = false;
                    GameObjectSetLifeTime(beamObj, 46);
                    GameObjectMoveRequest(beamObj, 46,
                        MkF3(
                            beamObj->pos.x + m_MachineGunShootVec[bIndex].x * ONE_BLOCK * 100.0f,
                            beamObj->pos.y + m_MachineGunShootVec[bIndex].y * ONE_BLOCK * 100.0f,
                            beamObj->pos.z + m_MachineGunShootVec[bIndex].z * ONE_BLOCK * 100.0f
                        ),
                        EasingInSine
                    );
                }
            }
        }
    }


    m_MachineGunFrame++;

    if (m_MachineGunFrame >= FRAME_MACHINEGUN_SHOOT_START + 1 + shootDelay * shootNum)
    {
        //SEを再生
        SoundEffectPlayRequest(GetSoundData("SOUND/SE/enemy/move/Inperius_Jet_End"), 1.2f, 3);

        m_ActionCooldown = m_WazaCooldown;
        m_MachineGunFrame = -1;
        m_CurrentState = ActionState::Idle;
    }
}

void GOE_Character_Enemy_Boss_Inperiusu::draw_machinegun()
{
    //
    // ビーム
    //
    {
        for (int i = 0; i < 2; i++)
        {
            DRAW_POLYGON_DAT drawDatBaseTmp = {};
            drawDatBaseTmp.size = MkF3(12.0f, 12.0f, 1800.0f);

            //drawDatBaseTmp.pos.y += -120.0f - drawDatBaseTmp.size.y / 2;
            //drawDatBaseTmp.pos.z += 30.0f + drawDatBaseTmp.size.z / 2 - (m_Obj->size.x * m_Obj->scale.x + m_Obj->size.y * m_Obj->scale.y) / 4;
            //drawDatBaseTmp.pos.z += 30.0f + drawDatBaseTmp.size.z / 2 - (m_Obj->size.x * m_Obj->scale.x + m_Obj->size.y * m_Obj->scale.y) / 4;

            drawDatBaseTmp.pos.z = drawDatBaseTmp.size.z;

            drawDatBaseTmp.rot = 0.0f;

            drawDatBaseTmp.rot3D = MkF3(0.0f, 0.0f, 0.0f);

            //drawDatTmp.color = MkF4(1.0f, 0.8f, 1.0f, 0.9f);
            drawDatBaseTmp.color = MkF4(1.0f * 2, 0.9f * 2, 1.0f * 2, 0.9f);
            //drawDatTmp.color = MkF4(3.0f, 1.0f, 7.0f, 0.9f);

            //drawDatTmp.texNo = LoadTexture("rom/texture/ui/in_game/charactor_icon.png");
            drawDatBaseTmp.texNo = 0;
            drawDatBaseTmp.loadTexType = LOADTEXTURETYPE_MAIN;

            drawDatBaseTmp.texUV = { 0.0f, 0.0f };
            drawDatBaseTmp.texWH = { 1.0f, 1.0f };

            drawDatBaseTmp.startPos3D = m_MachineGunShootPos[i];
            //drawDatBaseTmp.startPos3D.z += -ONE_BLOCK * 10.0f - ONE_BLOCK * 3.0f;

            drawDatBaseTmp.startRot = CalculateRadToDeg(atan2(
                m_MachineGunShootVec[i].y,
                m_MachineGunShootVec[i].x)) - 90.0f;
            drawDatBaseTmp.startRot3D = MkF3(90.0f + 45.0f * m_MachineGunShootVec[i].z, 0.0f, 0.0f);

            drawDatBaseTmp.startScale = MkF2(1.0f, 1.0f);
            drawDatBaseTmp.startScale3D = MkF3(1.0f, 1.0f, 1.0f);

            drawDatBaseTmp.modelNo = m_Model_Beam;

            drawDatBaseTmp.shaderOpt.bloom = DPD_SHADER_OPT_BLOOM_NORMAL;
            //drawDatBaseTmp.shaderOpt.blendState = DPD_SHADER_OPT_BLEND_STATE_ADDCOLOR;

            DRAW_POLYGON_IN_CAMERA_EX_OPTION optBaseTmp = {};
            optBaseTmp.reverse = false;
            optBaseTmp.fixed = false;
            optBaseTmp.enableShade = true;
            //optBaseTmp.priority = DRAW_POLYGON_PRIORITY_NML_u3;
            //opt.priority = DRAW_POLYGON_PRIORITY_NORMAL;
            //optBaseTmp.depthBuffIgnore = true;
            //opt.priority = DRAW_POLYGON_PRIORITY_FORCE_NORMAL;


            int beamFrameTmp = m_MachineGunFrame - FRAME_MACHINEGUN_SHOOT_START;

            if (m_MachineGunFrame > FRAME_MACHINEGUN_START && m_MachineGunFrame < FRAME_MACHINEGUN_SHOOT_END)
            {
                DRAW_POLYGON_DAT drawDatTmp = {};
                drawDatTmp.size = MkF3(1200.0f, 1200.0f, 1.0f);

                //{
                //    //float rotTmp = CalculateDegToRad(m_CurrentAxisEx * 45.0f + 90.0f);
                //    float rotTmp = CalculateDegToRad(m_CurrentAxisPrivate + 90.0f);
                //    drawDatTmp.pos.x = cos(rotTmp) * ONE_BLOCK * 0.5f;
                //    drawDatTmp.pos.y = sin(rotTmp) * ONE_BLOCK * 0.5f;
                //}
                //drawDatTmp.pos.z = -ONE_BLOCK * 10.0f - ONE_BLOCK * 3.0f;

                {
                    float rotTmp = CalculateDegToRad(m_CurrentAxisPrivate + 180.0f * i);
                    drawDatTmp.pos.x += cos(rotTmp) * ONE_BLOCK * 0.26f;
                    drawDatTmp.pos.y += sin(rotTmp) * ONE_BLOCK * 0.26f;
                }

                drawDatTmp.rot = 0.0f;

                drawDatTmp.rot3D = MkF3(0.0f, 0.0f, 0.0f);

                //if (m_BeamChargeFrame < FRAME_BEAM_SHOOT_START)
                //{
                //    if (m_BeamChargeFrame % 2 == 0)
                //    {
                //        drawDatTmp.color = MkF4(2.0f, 2.0f, 2.0f, 0.9f);
                //    }
                //    else
                //    {
                //        drawDatTmp.color = MkF4(-1.0f, -1.0f, -1.0f, 0.9f);
                //    }
                //}
                //else
                //{
                //    drawDatTmp.color = MkF4(2.0f, 2.0f, 2.0f, 0.9f);

                //}
                drawDatTmp.color = MkF4(2.0f, 2.0f, 2.0f, 0.9f);

                drawDatTmp.texNo = m_Tex_BeamCharge;
                drawDatTmp.loadTexType = LOADTEXTURETYPE_MAIN;

                drawDatTmp.texUV = AnimationUV(
                    m_MachineGunFrame - FRAME_MACHINEGUN_START,
                    ANIMCOUNT_BEAM_CHARGE_X,
                    ANIMCOUNT_BEAM_CHARGE_Y
                );
                drawDatTmp.texWH = {
                    1.0f / ANIMCOUNT_BEAM_CHARGE_X,
                    1.0f / ANIMCOUNT_BEAM_CHARGE_Y
                };

                //drawDatTmp.startPos3D = m_Obj->pos;
                drawDatTmp.startPos3D = m_MachineGunShootPos[i];

                //drawDatTmp.startRot = m_CurrentAxisEx * 45.0f;
                drawDatTmp.startRot3D = MkF3(0.0f, 0.0f, 0.0f);

                drawDatTmp.startScale = MkF2(1.0f, 1.0f);
                drawDatTmp.startScale3D = MkF3(1.0f, 1.0f, 1.0f);

                drawDatTmp.modelNo = 0;
                drawDatTmp.vertex.type = DRAW_POLYGON_TYPE_QUAD;

                drawDatTmp.shaderOpt.bloom = DPD_SHADER_OPT_BLOOM_NORMAL;
                drawDatTmp.shaderOpt.blendState = DPD_SHADER_OPT_BLEND_STATE_ADDCOLOR;

                DRAW_POLYGON_IN_CAMERA_EX_OPTION opt = {};
                opt.reverse = false;
                opt.fixed = false;
                opt.enableShade = false;
                opt.billboard = true;
                //opt.priority = DRAW_POLYGON_PRIORITY_NML_u2;
                opt.priority = DRAW_POLYGON_PRIORITY_NML_u3;
                opt.depthBuffIgnore = true;

                DrawPolygonInCameraEx(drawDatTmp, opt);
            }

            //予測エフェクト
            {
                int startbeforef = 30;
                if (m_MachineGunFrame >= FRAME_MACHINEGUN_SHOOT_START - startbeforef
                    //&& m_MachineGunFrame <= FRAME_BEAM_SHOOT_START
                    )
                {
                    float startMod = EasingInOutElastic((float)(beamFrameTmp + startbeforef) / startbeforef);

                    DRAW_POLYGON_DAT drawDatTmp2 = drawDatBaseTmp;
                    drawDatTmp2.texNo = m_Tex_Beam3;
                    drawDatTmp2.size.x = 10.0f * startMod;
                    drawDatTmp2.size.y = 10.0f * startMod;
                    //drawDatTmp2.pos.z += drawDatTmp2.size.z * -0.3f;
                    drawDatTmp2.pos.z += drawDatTmp2.size.z * -0.6f;

                    drawDatTmp2.color = MkF4(2.0f, 0.2f, 0.2f, 0.75f);
                    drawDatTmp2.shaderOpt.blendState = DPD_SHADER_OPT_BLEND_STATE_ADDCOLOR;
                    //drawDatTmp2.color = MkF4(2.0f, 0.5f, 0.5f, 0.75f);
                    DRAW_POLYGON_IN_CAMERA_EX_OPTION opt2 = optBaseTmp;
                    opt2.priority = DRAW_POLYGON_PRIORITY_NORMAL;
                    DrawPolygonInCameraEx(drawDatTmp2, opt2);
                }
            }
        }

    }
}


void GOE_Character_Enemy_Boss_Inperiusu::update_machinegun_hand()
{
    constexpr int shootNum = 8;
    constexpr int shootDelay = 10;

    m_BeforeVecBuf = m_TargetVec;

    Float3 bposBase = MkF3(
        m_Obj->pos.x + m_BeforeVec.x * ONE_BLOCK * 8.0f,
        m_Obj->pos.y + m_BeforeVec.y * ONE_BLOCK * 8.0f,
        m_Obj->pos.z + m_BeforeVec.z * ONE_BLOCK * 8.0f - ONE_BLOCK * 8.0f
    );
    m_MachineGunHandShootPos[0] = bposBase;

    //for (int i = 0; i < 1; i++)
    //{
    //    m_MachineGunHandShootPos[i] = bposBase;

    //    //float rad = atan2(bposBase.y, bposBase.x);
    //    //m_MachineGunShootPos[i].x += sin(rad) * ONE_BLOCK * 1.0f * (i == 1 ? -1.0f : 1.0f);
    //    //m_MachineGunShootPos[i].y += cos(rad) * ONE_BLOCK * 1.0f * (i == 1 ? -1.0f : 1.0f);
    //    float rad = atan2(m_BeforeVecBuf.y, m_BeforeVecBuf.x) + CalculateDegToRad(90.0f);
    //    m_MachineGunHandShootPos[i].x += sin(rad) * ONE_BLOCK * 1.0f * (i == 1 ? -1.0f : 1.0f);
    //    m_MachineGunHandShootPos[i].y += cos(rad) * ONE_BLOCK * 1.0f * (i == 1 ? -1.0f : 1.0f);
    //}

    GAME_OBJECT* targetObject = GetGameObjectFindByIdentifier(m_TargetObjIdentifier);
    if (targetObject != nullptr)
    {

        for (int i = 0; i < 1; i++)
        {
            Float3 postmp = {};
            postmp.x = m_Obj->pos.x + m_BeforeVec.x * m_TargetDistance;
            postmp.y = m_Obj->pos.y + m_BeforeVec.y * m_TargetDistance;
            postmp.z = targetObject->pos.z;

            //m_MachineGunShootVec[i] = NormalizeVector(MkF3(
            //    targetObject->pos.x - m_MachineGunShootPos[i].x,
            //    targetObject->pos.y - m_MachineGunShootPos[i].y,
            //    targetObject->pos.z - m_MachineGunShootPos[i].z - 80.0f
            //));
            m_MachineGunHandShootVec[i] = NormalizeVector(MkF3(
                postmp.x - m_MachineGunHandShootPos[i].x,
                postmp.y - m_MachineGunHandShootPos[i].y,
                postmp.z - m_MachineGunHandShootPos[i].z - 80.0f
            ));
        }
    }
    //m_MachineGunShootVec = m_TargetVec;

    if (m_MachineGunHandFrame == 1)
    {
        //SEを再生
        SoundEffectPlayRequest(GetSoundData("SOUND/SE/inperiusu/b_kamae"), 1.3f, 0);

        StartAnim(AnimState::MachinegunHandBefore,
            FRAME_MACHINEGUN_START,
            true);
    }
    if (m_MachineGunHandFrame == FRAME_MACHINEGUN_START + 1)
    {
        //SEを再生
        SoundEffectPlayRequest(GetSoundData("SOUND/SE/inperiusu/b_charge"), 0.75f, 0);

        StartAnim(AnimState::MachinegunHand,
            FRAME_MACHINEGUN_SHOOT_END - FRAME_MACHINEGUN_SHOOT_START + FRAME_MACHINEGUN_AFTER,
            true);
    }
    if (m_MachineGunHandFrame == FRAME_MACHINEGUN_SHOOT_START + 1)
    {
        //SEを再生
        SoundEffectPlayRequest(GetSoundData("SOUND/SE/enemy/attack/LaserRapid"), 1.5f, 0);

    }


    if (m_MachineGunHandFrame >= FRAME_MACHINEGUN_SHOOT_START + 1)
    {
        if ((m_MachineGunHandFrame - (FRAME_MACHINEGUN_SHOOT_START + 1)) % shootDelay == 0)
        {
            CameraShakeRequestAll(CAMERA_SHAKE_MIDIUM_FRAME, CAMERA_SHAKE_MIDIUM_POWER);

            //int bIndex = ((m_MachineGunFrame / shootDelay) % 2 == 0) ? 1 : 0;
            int bIndex = 0;
            GOE_Character_EnemyProjectile* goepro =
                GOE_Character_EnemyProjectile::CreateProjectile(
                    GOE_Character_EnemyProjectile::Type::Beam,
                    m_MachineGunHandShootPos[bIndex],
                    m_MachineGunHandShootVec[bIndex]
                );
            if (goepro != nullptr)
            {
                GAME_OBJECT* beamObj = goepro->GetGameObj();
                if (beamObj != nullptr)
                {
                    beamObj->scale = MkF3(
                        1.2f,
                        1.2f,
                        2.0f
                    );
                    {
                        Character::MOTION_COL_DAT motionColDetails =
                            Character::GetMotionColDataFindByName(
                                Character::MOTION_COL_TYPE_ATK,
                                MACHINEGUN_ATK_COL_NAME
                            );

                        GAME_OBJECT_COLLISION_ATTACK_CHILD attackColTmp = motionColDetails.atkCol;
                        attackColTmp.subjectFamily[GAME_OBJECT_FAMILY_PLAYER] = true;

                        attackColTmp.size = MkF3(
                            ONE_BLOCK * 3,
                            ONE_BLOCK * 3,
                            ONE_BLOCK * 3
                        );
                        attackColTmp.pos.z = -attackColTmp.size.z / 2;
                        attackColTmp.pos.y += attackColTmp.size.y / 2 * 1.0f;

                        beamObj->collision.atk.enable = true;
                        beamObj->collision.atk.collision[0] = attackColTmp;
                        beamObj->collision.atk.collision[0].enable = true;
                    }
                    beamObj->blockCollisionIgnore = false;
                    GameObjectSetLifeTime(beamObj, 46);
                    GameObjectMoveRequest(beamObj, 46,
                        MkF3(
                            beamObj->pos.x + m_MachineGunHandShootVec[bIndex].x * ONE_BLOCK * 100.0f,
                            beamObj->pos.y + m_MachineGunHandShootVec[bIndex].y * ONE_BLOCK * 100.0f,
                            beamObj->pos.z + m_MachineGunHandShootVec[bIndex].z * ONE_BLOCK * 100.0f
                        ),
                        EasingInSine
                    );
                }
            }
        }
    }


    m_MachineGunHandFrame++;

    if (m_MachineGunHandFrame >= FRAME_MACHINEGUN_SHOOT_START + 1 + shootDelay * shootNum)
    {
        //SEを再生
        SoundEffectPlayRequest(GetSoundData("SOUND/SE/enemy/move/Inperius_Jet_End"), 1.2f, 3);

        m_ActionCooldown = m_WazaCooldown;
        m_MachineGunHandFrame = -1;
        m_CurrentState = ActionState::Idle;
    }
}

void GOE_Character_Enemy_Boss_Inperiusu::draw_machinegun_hand()
{
    //
    // ビーム
    //
    {
        for (int i = 0; i < 1; i++)
        {
            DRAW_POLYGON_DAT drawDatBaseTmp = {};
            drawDatBaseTmp.size = MkF3(12.0f, 12.0f, 1800.0f);

            //drawDatBaseTmp.pos.y += -120.0f - drawDatBaseTmp.size.y / 2;
            //drawDatBaseTmp.pos.z += 30.0f + drawDatBaseTmp.size.z / 2 - (m_Obj->size.x * m_Obj->scale.x + m_Obj->size.y * m_Obj->scale.y) / 4;
            //drawDatBaseTmp.pos.z += 30.0f + drawDatBaseTmp.size.z / 2 - (m_Obj->size.x * m_Obj->scale.x + m_Obj->size.y * m_Obj->scale.y) / 4;

            drawDatBaseTmp.pos.z = drawDatBaseTmp.size.z;

            drawDatBaseTmp.rot = 0.0f;

            drawDatBaseTmp.rot3D = MkF3(0.0f, 0.0f, 0.0f);

            //drawDatTmp.color = MkF4(1.0f, 0.8f, 1.0f, 0.9f);
            drawDatBaseTmp.color = MkF4(1.0f * 2, 0.9f * 2, 1.0f * 2, 0.9f);
            //drawDatTmp.color = MkF4(3.0f, 1.0f, 7.0f, 0.9f);

            //drawDatTmp.texNo = LoadTexture("rom/texture/ui/in_game/charactor_icon.png");
            drawDatBaseTmp.texNo = 0;
            drawDatBaseTmp.loadTexType = LOADTEXTURETYPE_MAIN;

            drawDatBaseTmp.texUV = { 0.0f, 0.0f };
            drawDatBaseTmp.texWH = { 1.0f, 1.0f };

            drawDatBaseTmp.startPos3D = m_MachineGunHandShootPos[i];
            //drawDatBaseTmp.startPos3D.z += -ONE_BLOCK * 10.0f - ONE_BLOCK * 3.0f;

            drawDatBaseTmp.startRot = CalculateRadToDeg(atan2(
                m_MachineGunHandShootVec[i].y,
                m_MachineGunHandShootVec[i].x)) - 90.0f;
            drawDatBaseTmp.startRot3D = MkF3(90.0f + 45.0f * m_MachineGunHandShootVec[i].z, 0.0f, 0.0f);

            drawDatBaseTmp.startScale = MkF2(1.0f, 1.0f);
            drawDatBaseTmp.startScale3D = MkF3(1.0f, 1.0f, 1.0f);

            drawDatBaseTmp.modelNo = m_Model_Beam;

            drawDatBaseTmp.shaderOpt.bloom = DPD_SHADER_OPT_BLOOM_NORMAL;
            //drawDatBaseTmp.shaderOpt.blendState = DPD_SHADER_OPT_BLEND_STATE_ADDCOLOR;

            DRAW_POLYGON_IN_CAMERA_EX_OPTION optBaseTmp = {};
            optBaseTmp.reverse = false;
            optBaseTmp.fixed = false;
            optBaseTmp.enableShade = true;
            //optBaseTmp.priority = DRAW_POLYGON_PRIORITY_NML_u3;
            //opt.priority = DRAW_POLYGON_PRIORITY_NORMAL;
            //optBaseTmp.depthBuffIgnore = true;
            //opt.priority = DRAW_POLYGON_PRIORITY_FORCE_NORMAL;


            int beamFrameTmp = m_MachineGunHandFrame - FRAME_MACHINEGUN_SHOOT_START;

            if (m_MachineGunHandFrame > FRAME_MACHINEGUN_START 
                && m_MachineGunHandFrame < FRAME_MACHINEGUN_SHOOT_END)
            {
                DRAW_POLYGON_DAT drawDatTmp = {};
                drawDatTmp.size = MkF3(2200.0f, 2200.0f, 1.0f);

                //{
                //    //float rotTmp = CalculateDegToRad(m_CurrentAxisEx * 45.0f + 90.0f);
                //    float rotTmp = CalculateDegToRad(m_CurrentAxisPrivate + 90.0f);
                //    drawDatTmp.pos.x = cos(rotTmp) * ONE_BLOCK * 0.5f;
                //    drawDatTmp.pos.y = sin(rotTmp) * ONE_BLOCK * 0.5f;
                //}
                //drawDatTmp.pos.z = -ONE_BLOCK * 10.0f - ONE_BLOCK * 3.0f;

                {
                    float rotTmp = CalculateDegToRad(m_CurrentAxisPrivate + 180.0f * i);
                    drawDatTmp.pos.x += cos(rotTmp) * ONE_BLOCK * 0.26f;
                    drawDatTmp.pos.y += sin(rotTmp) * ONE_BLOCK * 0.26f;
                }

                drawDatTmp.rot = 0.0f;

                drawDatTmp.rot3D = MkF3(0.0f, 0.0f, 0.0f);

                //if (m_BeamChargeFrame < FRAME_BEAM_SHOOT_START)
                //{
                //    if (m_BeamChargeFrame % 2 == 0)
                //    {
                //        drawDatTmp.color = MkF4(2.0f, 2.0f, 2.0f, 0.9f);
                //    }
                //    else
                //    {
                //        drawDatTmp.color = MkF4(-1.0f, -1.0f, -1.0f, 0.9f);
                //    }
                //}
                //else
                //{
                //    drawDatTmp.color = MkF4(2.0f, 2.0f, 2.0f, 0.9f);

                //}
                drawDatTmp.color = MkF4(2.0f, 2.0f, 2.0f, 0.9f);

                drawDatTmp.texNo = m_Tex_BeamCharge;
                drawDatTmp.loadTexType = LOADTEXTURETYPE_MAIN;

                drawDatTmp.texUV = AnimationUV(
                    m_MachineGunHandFrame - FRAME_MACHINEGUN_START,
                    ANIMCOUNT_BEAM_CHARGE_X,
                    ANIMCOUNT_BEAM_CHARGE_Y
                );
                drawDatTmp.texWH = {
                    1.0f / ANIMCOUNT_BEAM_CHARGE_X,
                    1.0f / ANIMCOUNT_BEAM_CHARGE_Y
                };

                //drawDatTmp.startPos3D = m_Obj->pos;
                drawDatTmp.startPos3D = m_MachineGunHandShootPos[i];

                //drawDatTmp.startRot = m_CurrentAxisEx * 45.0f;
                drawDatTmp.startRot3D = MkF3(0.0f, 0.0f, 0.0f);

                drawDatTmp.startScale = MkF2(1.0f, 1.0f);
                drawDatTmp.startScale3D = MkF3(1.0f, 1.0f, 1.0f);

                drawDatTmp.modelNo = 0;
                drawDatTmp.vertex.type = DRAW_POLYGON_TYPE_QUAD;

                drawDatTmp.shaderOpt.bloom = DPD_SHADER_OPT_BLOOM_NORMAL;
                drawDatTmp.shaderOpt.blendState = DPD_SHADER_OPT_BLEND_STATE_ADDCOLOR;

                DRAW_POLYGON_IN_CAMERA_EX_OPTION opt = {};
                opt.reverse = false;
                opt.fixed = false;
                opt.enableShade = false;
                opt.billboard = true;
                //opt.priority = DRAW_POLYGON_PRIORITY_NML_u2;
                opt.priority = DRAW_POLYGON_PRIORITY_NML_u3;
                opt.depthBuffIgnore = true;

                DrawPolygonInCameraEx(drawDatTmp, opt);
            }

            //予測エフェクト
            {
                int startbeforef = 30;
                if (m_MachineGunHandFrame >= FRAME_MACHINEGUN_SHOOT_START - startbeforef
                    //&& m_MachineGunFrame <= FRAME_BEAM_SHOOT_START
                    )
                {
                    float startMod = EasingInOutElastic((float)(beamFrameTmp + startbeforef) / startbeforef);

                    DRAW_POLYGON_DAT drawDatTmp2 = drawDatBaseTmp;
                    drawDatTmp2.texNo = m_Tex_Beam3;
                    drawDatTmp2.size.x = 10.0f * startMod;
                    drawDatTmp2.size.y = 10.0f * startMod;
                    //drawDatTmp2.pos.z += drawDatTmp2.size.z * -0.3f;
                    drawDatTmp2.pos.z += drawDatTmp2.size.z * -0.6f;

                    drawDatTmp2.color = MkF4(2.0f, 0.2f, 0.2f, 0.75f);
                    drawDatTmp2.shaderOpt.blendState = DPD_SHADER_OPT_BLEND_STATE_ADDCOLOR;
                    //drawDatTmp2.color = MkF4(2.0f, 0.5f, 0.5f, 0.75f);
                    DRAW_POLYGON_IN_CAMERA_EX_OPTION opt2 = optBaseTmp;
                    opt2.priority = DRAW_POLYGON_PRIORITY_NORMAL;
                    DrawPolygonInCameraEx(drawDatTmp2, opt2);
                }
            }
        }

    }
}

void GOE_Character_Enemy_Boss_Inperiusu::update_energyballbit()
{
    //m_BeforeVec = m_TargetVec;

    if (m_EnergyBallBitFrame == 1)
    {
        StartTurn(0.0f, 0.05f);
    }

    if (m_EnergyBallBitFrame == 2)
    {
        //SEを再生
        SoundEffectPlayRequest(GetSoundData("SOUND/SE/enemy/move/Inperius_Jet_Start"), 3.2f, 0);
        StartAnim(AnimState::BeamBefore, 60, true);
    }
    if (m_EnergyBallBitFrame == 60)
    {
        CameraFlashRequestAll(
            40,
            EasingInOutSine,
            //MkF4(0.9f, 0.7f, 0.5f, 0.8f),
            MkF4(10.0f, 0.1f, 10.0f, 0.8f),
            DPD_SHADER_OPT_BLEND_STATE_ADDCOLOR
        );

        //SEを再生
        SoundEffectPlayRequest(GetSoundData("SOUND/SE/enemy/move/warpHith"),2.2f, 0);
        //SEを再生
        SoundEffectPlayRequest(GetSoundData("SOUND/SE/enemy/move/warpLow"), 2.2f, 2);

        StartAnim(AnimState::Beam, 60, true);

        int summonNum = 2;
        for(int i = 0; i < summonNum; i++)
        {
            //GOE_Character_EnemyProjectile* goepro =
            //    GOE_Character_EnemyProjectile::CreateProjectile(
            //        GOE_Character_EnemyProjectile::Type::EnergyBallBit,
            //        MkF3(
            //            m_Obj->pos.x,
            //            m_Obj->pos.y,
            //            m_Obj->pos.z + ONE_BLOCK * -8.0f
            //        ),
            //        {}
            //    );
            //if (goepro != nullptr)
            //{
            //    GAME_OBJECT* beamObj = goepro->GetGameObj();
            //    if (beamObj != nullptr)
            //    {
            //        goepro->SetTarget(m_TargetObjIdentifier);

            //        //goepro->SetNumFlag(m_IsHard && m_HardPhase2Flag ? 1 : 0);
            //        
            //        //beamObj->scale = MkF3(
            //        //    0.5f,
            //        //    0.5f,
            //        //    0.75f
            //        //);
            //        beamObj->friction = 1.0f;
            //        beamObj->blockCollisionIgnore = false;
            //        //GameObjectSetLifeTime(beamObj, FRAME_ONE_SECOND * 5.5f);
            //        GameObjectSetLifeTime(beamObj, FRAME_ONE_SECOND * 20.0f);
            //        GameObjectMoveRequest(beamObj, 40,
            //            MkF3(
            //                m_Obj->pos.x + ONE_BLOCK * 18.0f * (i == 1 ? -1.0f : 1.0),
            //                m_Obj->pos.y,
            //                m_Obj->pos.z + ONE_BLOCK * -10.0f
            //            ),
            //            EasingInSine
            //        );
            //    }
            //}

            GAME_OBJECT* co = Character::CreateCharacterGameObject(GOE_Character_Enemy_Satellite::CHARACTER_NAME);
            if (co != nullptr)
            {
                //co->pos = MkF3(
                //    m_Obj->pos.x,
                //    m_Obj->pos.y,
                //    m_Obj->pos.z + ONE_BLOCK * -8.0f
                //);
                co->pos = MkF3(
                    m_Obj->pos.x + ONE_BLOCK * 18.0f * (i == 1 ? -1.0f : 1.0),
                    m_Obj->pos.y,
                    m_Obj->pos.z + ONE_BLOCK * -10.0f
                );

                {
                    GOE_Character_Enemy_Satellite* goe =
                        dynamic_cast<GOE_Character_Enemy_Satellite*>(co->events.source);  // ダウンキャスト

                    if (goe != nullptr)
                    { //
                        goe->SetInstanceID(i);

                        //goe->SetTarget(m_TargetObjIdentifier);
                        if (m_HardPhase2Flag)
                        {
                            goe->SetBeamMode(true);
                        }

                        co->friction = 1.0f;
                        co->blockCollisionIgnore = false;
                        //GameObjectSetLifeTime(beamObj, FRAME_ONE_SECOND * 5.5f);
                        //GameObjectSetLifeTime(co, FRAME_ONE_SECOND * 20.0f);
                        //GameObjectMoveRequest(co, 40,
                        //    MkF3(
                        //        m_Obj->pos.x + ONE_BLOCK * 18.0f * (i == 1 ? -1.0f : 1.0),
                        //        m_Obj->pos.y,
                        //        m_Obj->pos.z + ONE_BLOCK * -10.0f
                        //    ),
                        //    EasingInSine
                        //);
                    }
                }
            }
        }
    }

    if (m_EnergyBallBitFrame == FRAME_ONE_SECOND * 1.5f)
    {
        //SEを再生
        SoundEffectPlayRequest(GetSoundData("SOUND/SE/enemy/move/Inperius_Jet_End"), 2.2f, 0);
    }


    m_EnergyBallBitFrame++;
    if (
        //m_EnergyBallBitFrame >= FRAME_ONE_SECOND * 7
        m_EnergyBallBitFrame >= FRAME_ONE_SECOND * 2
    )
    {
        m_ActionCooldown = m_WazaCooldown;
        m_EnergyBallBitFrame = -1;
        m_CurrentState = ActionState::Idle;
    }
}

void GOE_Character_Enemy_Boss_Inperiusu::draw_energyballbit()
{
}

void GOE_Character_Enemy_Boss_Inperiusu::update_granade()
{
    bool isBeamCombo = 
        /*m_ActCycleNum > 2*/
        m_ActCycleNum % 2 == 0
        ;

    if (m_GranadeFrame == 0)
    {
        m_GranadeAfterFrame = -1;
        m_Granade_Objects.clear();
    }

    if (m_GranadeFrame == 1)
    {
        StartTurn(Float3ToFloat2(m_TargetVec), 0.05f);
    }

    if (m_GranadeFrame == 2)
    {
        //m_BeforeVecBuf = m_TargetVec;

                //SEを再生
        SoundEffectPlayRequest(GetSoundData("SOUND/SE/inperiusu/b_kamae"), 1.3f, 0);

        StartAnim(AnimState::GranadeBefore,
            FRAME_GRANADE_START,
            true);

        if (m_IsHard)
        {
            SetCameraModeAll(CAMERA_MODE_BOSS_CHASE_WIDE);
        }
    }
    if (m_GranadeFrame == FRAME_GRANADE_START + 2)
    {
        StartAnim(AnimState::Granade,
            FRAME_GRANADE_AFTER,
            true);
    }

    if (m_GranadeFrame == FRAME_GRANADE_START + 10)
    {
        Float3 startpos = MkF3(
            //m_Obj->pos.x + m_BeforeVec.x * -180.0f,
            //m_Obj->pos.y + m_BeforeVec.y * -180.0f,
            //m_Obj->pos.z + m_BeforeVec.z * -180.0f + ONE_BLOCK * -16.0f
            m_Obj->pos.x + m_BeforeVec.x * 500.0f,
            m_Obj->pos.y + m_BeforeVec.y * 500.0f,
            m_Obj->pos.z + m_BeforeVec.z * 500.0f + ONE_BLOCK * -9.0f
        );

        SoundEffectPlayRequest(GetSoundData("SOUND/SE/enemy/attack/Small_Explosion"), 3.0f, 0);
        //SEを再生
        //SoundEffectPlayRequest(GetSoundData("SOUND/SE/inperiusu/b_charge"), 0.5f, 5);
        //SEを再生
        SoundEffectPlayRequest(GetSoundData("SOUND/SE/enemy/move/Inperius_Jet_End"), 1.2f, 35);

        CameraShakeRequestAll(
            CAMERA_SHAKE_LARGE_FRAME,
            CAMERA_SHAKE_EXLARGE_POWER * 0.8f
        );

        {
            Float3 efPos1 = startpos;
            //efPos1.z += ONE_BLOCK * -5.0f;
            efPos1.z += ONE_BLOCK * -2.0f;

            GAME_OBJECT* go = GO_CreateEffect(GO_EFFECT_TYPE_EXPLOSION_2, efPos1);
            if (go != nullptr)
            {
                go->scale = MkF3(
                    10.0f,
                    10.0f,
                    10.0f
                );
                GameObjectSetLifeTime(go, 45);
            }
        }

        {
            //if (m_HardPhase2Flag)
            if(m_GranadeSuperFlag)
            {
                int granadenum = 6;
                //int granadenum = 5;
                for (int i = 0; i < granadenum; i++)
                {
                    //(void)rand();

                    GOE_Character_EnemyProjectile* goepro =
                        GOE_Character_EnemyProjectile::CreateProjectile(
                            GOE_Character_EnemyProjectile::Type::Granade,
                            startpos,
                            {}
                        );
                    if (goepro != nullptr)
                    {
                        GAME_OBJECT* beamObj = goepro->GetGameObj();
                        if (beamObj != nullptr)
                        {
                            bool isFinal = i == (granadenum - 1);
                            //beamObj->scale = MkF3(
                            //    2.0f,
                            //    2.0f,
                            //    2.0f
                            //);
                            beamObj->scale = MkF3(
                                2.5f,
                                2.5f,
                                2.5f
                            );
                            //Float3 hitpos = MkF3(
                            //    0.0f + sin(2.0f * SYS_GENERAL_PI / (granadenum - 1) * i) * 3000.0f,
                            //    ONE_BLOCK * -10.0f + cos(2.0f * SYS_GENERAL_PI / (granadenum - 1) * i) * 1200.0f,
                            //    0.0f
                            //);
                            Float3 hitpos = {};


                            //beamObj->freeInt = 30 * i;
                            beamObj->freeInt = 40 * i;

                            if (isFinal)
                            {
                                //beamObj->scale = MkF3(
                                //    2.5f,
                                //    2.5f,
                                //    2.5f
                                //);
                                //hitpos = MkF3(
                                //    0.0f,
                                //    0.0f,
                                //    0.0f
                                //);
                                //goepro->SetHitPos(hitpos);

                                goepro->SetTarget(m_TargetObjIdentifier);

                                if (isBeamCombo)
                                {
                                    beamObj->scale = MkF3(
                                        2.8f,
                                        2.8f,
                                        2.8f
                                    );
                                    goepro->SetNumFlag(0);
                                }
                                beamObj->freeInt += 80;
                            }
                            else
                            {
                                goepro->SetTarget(m_TargetObjIdentifier);
                            }

                            if (isBeamCombo)
                            {
                                //goepro->SetNumFlag(0);
                                goepro->SetNumFlag(isFinal ? 4 : 3);
                            }
                            else
                            {
                                goepro->SetNumFlag(isFinal ? 2 : 1);
                            }

                            m_Granade_Objects.push_back(GetIdentifierGameObject(beamObj));
                        }
                    }
                }
            }
            else
            {
                int granadenum = 1;
                if (m_IsHard)
                {
                    granadenum = 4;
                }
                //if (m_HardPhase2Flag)
                //{
                //    granadenum = 5;
                //}
                for (int i = 0; i < granadenum; i++)
                {
                    (void)rand();

                    GOE_Character_EnemyProjectile* goepro =
                        GOE_Character_EnemyProjectile::CreateProjectile(
                            GOE_Character_EnemyProjectile::Type::Granade,
                            startpos,
                            {}
                        );
                    if (goepro != nullptr)
                    {
                        GAME_OBJECT* beamObj = goepro->GetGameObj();
                        if (beamObj != nullptr)
                        {

                            beamObj->scale = MkF3(
                                2.0f,
                                2.0f,
                                2.0f
                            );
                            //Float3 hitpos = MkF3(
                            //    m_Obj->pos.x + m_TargetVec.x * 2200.0f,
                            //    m_Obj->pos.y + m_TargetVec.y * 2200.0f,
                            //    m_Obj->pos.z
                            //);

                            //if (i == 1)
                            //{
                            //    hitpos = MkF3(
                            //        m_Obj->pos.x + m_TargetVec.x * 6500.0f,
                            //        m_Obj->pos.y + m_TargetVec.y * 6500.0f,
                            //        m_Obj->pos.z
                            //    );
                            //}
                            //if (i >= 2)
                            //{
                            //    hitpos = MkF3(
                            //        m_Obj->pos.x + ((float)(rand() % 1000) / 1000 - 0.5f) * BLOCK_WIDTH * 50.0f,
                            //        m_Obj->pos.y + ((float)(rand() % 1000) / 1000 - 0.5f) * BLOCK_WIDTH * 50.0f,
                            //        m_Obj->pos.z
                            //    );
                            //}
                            //goepro->SetHitPos(hitpos);
                            goepro->SetTarget(m_TargetObjIdentifier);

                            beamObj->freeInt = 30 * i;


                            //goepro->SetNumFlag(m_IsHard&& m_HardPhase2Flag ? 1 : 0);

                            //goepro->SetTarget(m_TargetObjIdentifier);
                            //beamObj->scale = MkF3(
                            //    0.5f,
                            //    0.5f,
                            //    0.75f
                            //);
                            //beamObj->friction = 1.0f;
                            //GameObjectSetLifeTime(beamObj, FRAME_ONE_SECOND * 5.5f);

                            m_Granade_Objects.push_back(GetIdentifierGameObject(beamObj));
                        }
                    }
                }
            }
        }
    }


    if (m_GranadeFrame == FRAME_GRANADE_START + FRAME_GRANADE_AFTER + 10)
    {
        //if (m_HardPhase2Flag)
        if (m_GranadeSuperFlag)
        {
            if (isBeamCombo)
            {
                //StartWarp({ 0.0f, ONE_BLOCK * -60.0f, ONE_BLOCK * -1.0f },
                //    0.0f, 25, true);
                //{ //多方向レーザー
                //    m_BeamFrame = 0;
                //    m_BeamNum = 5;
                //    m_BeamIsOutLine = false;
                //    m_BeamNoInitMove = true;
                //    m_BeamAfterWarpFlag = true;
                //    m_BeamRotatePattarn = 1;
                //    m_CurrentState = ActionState::Beam;
                //}

                StartTurn(0.0f, 0.05f);
                { //多方向レーザー
                    m_BeamFrame = 0;
                    m_BeamNum = 3;
                    m_BeamIsOutLine = false;
                    m_BeamShockWaveType = false;
                    m_BeamOutLineRangeMod = 0.0f;
                    m_BeamNoInitMove = true;
                    m_BeamAfterWarpFlag = false;
                    m_BeamRotatePattarn = 1;
                    m_CurrentState = ActionState::Beam;
                }
            }
            else
            {
                StartWarp({ 0.0f, ONE_BLOCK * -100.0f, ONE_BLOCK * -10000.0f },
                    0.0f, 25, true);
            }
        }
    }

    m_GranadeFrame++;

    if (m_IsHard)
    {
        //if (m_HardPhase2Flag)
        if (m_GranadeSuperFlag)
        {
            bool superGranadeEnd = false;
            if (m_GranadeFrame >= FRAME_ONE_SECOND * 12)
            {
                superGranadeEnd = true;
            }
            else
            {
                if (m_GranadeAfterFrame >= 0)
                {
                    if (m_GranadeAfterFrame >= FRAME_ONE_SECOND * 2)
                    {
                        superGranadeEnd = true;
                    }

                    m_GranadeAfterFrame++;
                }
                else if (
                    m_GranadeFrame >= 
                        FRAME_GRANADE_START
                        + FRAME_GRANADE_AFTER + FRAME_ONE_SECOND
                )
                {
                    int count = 0;
                    for (const auto& obid : m_Granade_Objects)
                    {
                        GAME_OBJECT* ob = GetGameObjectFindByIdentifier(obid);
                        if (ob != nullptr)
                        {
                            count++;
                        }
                    }
                    if (count <= 0)
                    {
                        m_GranadeAfterFrame = 0;
                    }
                }
            }

            if (superGranadeEnd)
            {
                SetCameraModeAll(CAMERA_MODE_BOSS_CHASE);

                StartWarp({ 0.0f, ONE_BLOCK * -15.0f, 0.0f }, 0.0f, 25, true);

                m_ActionCooldown = m_WazaCooldown;
                m_GranadeFrame = -1;
                m_CurrentState = ActionState::Idle;
            }
        }
        else
        {
            if (m_GranadeFrame >= FRAME_ONE_SECOND * 4)
            {
                SetCameraModeAll(CAMERA_MODE_BOSS_CHASE);

                m_ActionCooldown = m_WazaCooldown;
                m_GranadeFrame = -1;
                m_CurrentState = ActionState::Idle;
            }
        }
    }
    else
    {
        if (m_GranadeFrame >= FRAME_ONE_SECOND * 3)
        {
            m_ActionCooldown = m_WazaCooldown;
            m_GranadeFrame = -1;
            m_CurrentState = ActionState::Idle;
        }
    }
}

void GOE_Character_Enemy_Boss_Inperiusu::draw_granade()
{
}

void GOE_Character_Enemy_Boss_Inperiusu::update_enemyCall()
{
    //m_BeforeVec = m_TargetVec;

    if (m_EnemyCallFrame == 1)
    {
        StartTurn(0.0f, 0.05f);
    }

    if (m_EnemyCallFrame == 2)
    {
        //SEを再生
        SoundEffectPlayRequest(GetSoundData("SOUND/SE/enemy/move/Inperius_Jet_Start"), 2.2f, 0);
        StartAnim(AnimState::BeamBefore, 60, true);
    }
    if (m_EnemyCallFrame == 60)
    {
        CameraFlashRequestAll(
            40,
            EasingInOutSine,
            //MkF4(0.9f, 0.7f, 0.5f, 0.8f),
            MkF4(10.0f, 0.1f, 10.0f, 0.8f),
            DPD_SHADER_OPT_BLEND_STATE_ADDCOLOR
        );

        //SEを再生
        SoundEffectPlayRequest(GetSoundData("SOUND/SE/enemy/move/warpHith"), 2.2f, 0);
        //SEを再生
        SoundEffectPlayRequest(GetSoundData("SOUND/SE/enemy/move/warpLow"), 2.2f, 2);

        StartAnim(AnimState::Beam, 60, true);

        {
            int summonEnemyNum = 8;
            for (int i = 0; i < summonEnemyNum; i++)
            {
                GAME_OBJECT* co = nullptr;

                {
                    if (m_IsHard && i < summonEnemyNum / 2)
                    {
                        co = Character::CreateCharacterGameObject(
                            "ENEMY_MIRESU_SHOOTER");
                    }
                    else
                    {
                        co = Character::CreateCharacterGameObject(
                            "ENEMY_MIRESU_ATTACKER");
                    }
                    if (co != nullptr)
                    {
                        //co->pos = MkF3(
                        //    m_Obj->pos.x + ((float)(rand() % 1000)
                        //        / 1000 - 0.5f) * (BLOCK_WIDTH * 30),
                        //    m_Obj->pos.y + ((float)(rand() % 1000)
                        //        / 1000 - 0.5f) * (BLOCK_HIGHT * 30),
                        //    0.0f);

                        float rad = 2.0f * SYS_GENERAL_PI / summonEnemyNum * i;
                        float dis = (BLOCK_WIDTH * 16);
                        if (i % 2 == 0)
                        {
                            dis *= 0.5f;
                        }

                        co->pos = MkF3(
                            m_Obj->pos.x + sin(rad) * dis,
                            m_Obj->pos.y + cos(rad) * dis,
                            0.0f);
                        {
                            GOE_Character_Enemy_Miresu* goe =
                                dynamic_cast<GOE_Character_Enemy_Miresu*>(co->events.source);  // ダウンキャスト

                            if (goe != nullptr)
                            { //
                                goe->SetBuffSuper();
                                goe->BecomeApp(90);
                            }
                        }
                    }
                }

                if (co != nullptr)
                {
                    m_CallEnemies.push_back(GetIdentifierGameObject(co));
                }
            }
        }
    }
    
    if (m_EnemyCallFrame == FRAME_ONE_SECOND * 1.5f)
    {
        //SEを再生
        SoundEffectPlayRequest(GetSoundData("SOUND/SE/enemy/move/Inperius_Jet_End"), 2.2f, 0);
    }
    

    m_EnemyCallFrame++;
    if (
        //m_EnergyBallBitFrame >= FRAME_ONE_SECOND * 7
        m_EnemyCallFrame >= FRAME_ONE_SECOND * 2
        )
    {
        m_ActionCooldown = m_WazaCooldown;
        m_EnemyCallFrame = -1;
        m_CurrentState = ActionState::Idle;
    }
}

void GOE_Character_Enemy_Boss_Inperiusu::draw_enemyCall()
{
}

void GOE_Character_Enemy_Boss_Inperiusu::update_phase2Start()
{
    m_BeforeVecBuf = NormalizeVector({ 0.0f, 1.0f, 0.0f });
    m_BeforeVec = m_BeforeVecBuf;

    m_HeadVecBuf = NormalizeVector({ 0.0f, 1.0f, 0.5f });
    m_HeadVec = m_HeadVecBuf;

    float bgmstartsec = 38.0f;
    float bgmstartsec2 = 68.0f;
    float bgmendsec2 = 73.0f;
    float bgmstartsec3 = 42.5f;

    if (m_Phase2StartFrame == 0)
    {
        {
            GOE_Character_Player* targetSourcePlayer = nullptr;
            {
                PLAYER* player = GetPlayer(0);
                GAME_OBJECT* playerObj = player->obj;
                if (playerObj != nullptr)
                {
                    m_Phase2StartSourcePlayerObject = GetIdentifierGameObject(playerObj);

                    targetSourcePlayer =
                        dynamic_cast<GOE_Character_Player*>(playerObj->events.source);  // ダウンキャスト
                }
            }

            if (targetSourcePlayer != nullptr
                && !targetSourcePlayer->IsHealIgnore()
                && targetSourcePlayer->GetGameObj()->status.hpMax > targetSourcePlayer->GetGameObj()->status.hp
                )
            {
                m_Phase2StartPlayerDamagedFlag = true;
            }
        }

        m_Obj->hitReaction = {};
        m_Obj->hitStop = {};
        m_Obj->moveRequ.enable = false;


        CameraFlashRequestAll(
            20,
            EasingInOutSine,
            MkF4(10.0f, 10.0f, 10.0f, 0.8f),
            DPD_SHADER_OPT_BLEND_STATE_ADDCOLOR
        );
        m_KakoEfScreenMaskFrameMax = 40;
        m_KakoEfScreenMaskFrame = m_KakoEfScreenMaskFrameMax;

        set_blade_visible(false);

        CameraShakeRequestAll(
            CAMERA_SHAKE_EXLARGE_FRAME,
            CAMERA_SHAKE_EXLARGE_POWER * 3
        );

        {
            m_Obj->status.guardType = GAME_OBJECT_GUARD_TYPE_NORMAL;

            m_SuperBeamBarrierFrame = 0;
            m_SuperBeamBarrierFlag = false;
            for (int i = 0; i < m_SuperBeamEnemies.size(); i++)
            {
                if (IsEnableBossbarBarrier_Game(i)
                    && !IsBreakingBossbarBarrier_Game(i))
                {
                    ShowBossbarBarrier_Game(i, 1);
                }
            }
            for (int i = 0; i < m_SuperBeamEnemiesSub.size(); i++)
            {
                GAME_OBJECT* ob = GetGameObjectFindByIdentifier(m_SuperBeamEnemiesSub[i]);
                if (IsActiveGameObject(ob) && !IsDefeatGameObject(ob))
                {
                    DeleteGameObject(ob);
                }
            }
            m_SuperBeamEnemiesSub.clear();
            m_SuperBeamEnemiesSub.shrink_to_fit();

            for (int i = 0; i < m_SuperBeamEnemies.size(); i++)
            {
                GAME_OBJECT* ob = GetGameObjectFindByIdentifier(m_SuperBeamEnemies[i]);
                if (IsActiveGameObject(ob) && !IsDefeatGameObject(ob))
                {
                    DeleteGameObject(ob);
                }
            }
            m_SuperBeamEnemies.clear();
            m_SuperBeamEnemies.shrink_to_fit();

            m_SuperBeamEnemiesFrame.clear();
            m_SuperBeamEnemiesFrame.shrink_to_fit();
        }
        m_BeamFrame = -1;
        beam_object_delete();
        misile_object_delete();

        {
            //プレイヤー以外を削除する
            for (int i = 0; i < GAME_OBJECT_MAX; i++)
            {
                GAME_OBJECT* ob = GetGameObject(i);
                if (IsActiveGameObject(ob)
                    && ob != m_Obj
                    && (ob->tags.size() <= 0
                        || !ob->tags[0].find("PLAYER") == 0)
                    )
                {
                    DeleteGameObject(ob);
                }
            }

            SoundBGMStopRequest();
            SoundEffectStopAllRequest();

            SetUIEventTimer_Game(false);
            //ShowBossbar_Game(GetGameObjectIndex(m_Obj), false, "");

            SoundEffectPlayRequest(
                GetSoundData("SOUND/SE/player/attack/activation"),
                2.0f, 0.0f, 0.75f
            );
            SoundEffectPlayRequest(SOUND_TYPE_SE_9, 6.0f, 0, 0.9f);
            SoundEffectPlayRequest(SOUND_TYPE_SE_9, 4.0f, 30, 0.9f);
            SoundEffectPlayRequest(SOUND_TYPE_SE_9, 3.0f, 60, 0.9f);
            SoundEffectPlayRequest(SOUND_TYPE_SE_9, 2.0f, 90, 0.9f);
            SoundEffectPlayRequest(SOUND_TYPE_SE_9, 1.0f, 120, 0.9f);

            //ムービーフラグ
            //GameManagerSetMovieFlag(true, true);
            GameManagerSetMoviePlayerHiddenFlag(true);
            GameManagerSetMovieFlag(
                true,
                !m_Phase2StartPlayerDamagedFlag
            );

            SetCameraModeAll(CAMERA_MODE_NORMAL);

            {
                Float3 zoomPos = m_Obj->pos;
                //zoomPos.z += 4200.0f;
                zoomPos.z += 18000.0f;

                CameraMoveRequestAll(FRAME_ONE_SECOND * 0.5,
                    zoomPos,
                    0.0f,
                    MkF2(1.0f, 1.0f),
                    0.3f,
                    EasingInOutSine,
                    CAMERA_MOVE_TYPE_SMOOTH
                );
                CameraTargetMoveRequestAll(FRAME_ONE_SECOND * 0.5,
                    MkF3(zoomPos.x, zoomPos.y + 20000.0f, zoomPos.z + -38000.0f),
                    EasingInOutSine,
                    CAMERA_MOVE_TYPE_SMOOTH
                );
            }

            GE_GayaCancelRequestAll();
        }

        //BGMを再生する
        //SoundBGMPlayRequest(m_bgmLoop,
        //    true,
        //    2.0f,
        //    0.0f,

        //    38.0f
        //    //37.8f
        //);

        SoundBGMPlayRequest(m_bgmLoop,
            false,
            2.0f,
            0.0f,

            bgmstartsec
        );
        SoundBGMPlayRequest(m_bgmLoop,
            true,
            2.0f,
            (int)((bgmstartsec3 - bgmstartsec + 0.2f)* FRAME_ONE_SECOND),

            bgmstartsec2,
            bgmendsec2
        );

        StartAnim(AnimState::Damage, 40, true);
    }


    //int ragtemp = +FRAME_ONE_SECOND * 2.5;
    int ragtemp = +FRAME_ONE_SECOND * 2.2;
    //int ragtemp2 = +FRAME_ONE_SECOND * 0.5;
    int ragtemp2 = +FRAME_ONE_SECOND * 0.3;
    if (m_Phase2StartFrame == 26)
    {
        {
            Float3 zoomPos = m_Obj->pos;
            //zoomPos.z += 4200.0f;
            zoomPos.z += 20000.0f;

            CameraMoveRequestAll(FRAME_ONE_SECOND * 3.0 + ragtemp + ragtemp2,
                zoomPos,
                0.0f,
                MkF2(1.0f, 1.0f),
                0.4f,
                EasingInOutSine,
                CAMERA_MOVE_TYPE_SMOOTH
            );
            CameraTargetMoveRequestAll(FRAME_ONE_SECOND * 3.0 + ragtemp + ragtemp2,
                MkF3(zoomPos.x, zoomPos.y + 20000.0f, zoomPos.z + -40000.0f),
                EasingInOutSine,
                CAMERA_MOVE_TYPE_SMOOTH
            );
        }
    }
    if (m_Phase2StartFrame == 40)
    {
        //StartAnim(AnimState::Damage2, (180 + ragtemp + ragtemp2), true);
        StartAnim(AnimState::Damage2, 32767, true);
    }



    if (m_Phase2StartFrame == ragtemp)
    {
        GE_GayaStartRequest("iseki_inperiusuhard_phase2before_mayu", 0);

        m_HealEventFrame = 0;
    }

    if (m_Phase2StartPlayerDamagedFlag)
    {
        if (m_Phase2StartFrame > ragtemp 
            && m_HealEventFrame >= 0
            && !update_healevent(m_Phase2StartSourcePlayerObject)
        )
        {
            if (m_HealEventFrame == 1)
            {
                //ムービーフラグ
                GameManagerSetMovieFlag(false);
                GameManagerSetMoviePlayerHiddenFlag(false);

                SetCameraModeAll(CAMERA_MODE_PLAYER_CHASE);

                CameraMoveCancelRequestAll();
                CameraTargetMoveCancelRequestAll();
            }
            return;
        }
    }

    if (m_Phase2StartFrame == ragtemp + 1)
    {
        GameManagerSetMovieFlag(true, true);
        GameManagerSetMoviePlayerHiddenFlag(true);

        SetCameraModeAll(CAMERA_MODE_NORMAL);

        {
            Float3 zoomPos = m_Obj->pos;
            //zoomPos.z += 4200.0f;
            zoomPos.z += 20000.0f;

            CameraMoveRequestAll(FRAME_ONE_SECOND * 3.0 + ragtemp2,
                zoomPos,
                0.0f,
                MkF2(1.0f, 1.0f),
                0.4f,
                EasingInOutSine,
                CAMERA_MOVE_TYPE_SMOOTH
            );
            CameraTargetMoveRequestAll(FRAME_ONE_SECOND * 3.0 + ragtemp2,
                MkF3(zoomPos.x, zoomPos.y + 20000.0f, zoomPos.z + -40000.0f),
                EasingInOutSine,
                CAMERA_MOVE_TYPE_SMOOTH
            );
        }
    }


    if (m_Phase2StartFrame > (CAMERA_SHAKE_EXLARGE_FRAME + ragtemp)
        && m_Phase2StartFrame < (180 + ragtemp))
    {
        if (m_Phase2StartFrame % 10 == 0)
        {
            CameraShakeRequestAll(
                CAMERA_SHAKE_MIDIUM_FRAME,
                CAMERA_SHAKE_LARGE_POWER * 0.4f,
                -1,
                true
            );
        }
    }

    if (m_Phase2StartFrame == 150 + ragtemp + ragtemp2 - FRAME_ONE_SECOND * 0.5f)
    {
        //{
        //    Float3 zoomPos2 = m_Obj->pos;
        //    zoomPos2.z += -9000.0f - 4000.0f;
        //    Float3 zoomPos2Ta = m_Obj->pos;
        //    zoomPos2Ta.y += 35000.0f;
        //    zoomPos2Ta.z += -19000.0f - 5000.0f;

        //    CameraMoveRequestAll(30,
        //        zoomPos2,
        //        0.0f,
        //        MkF2(1.0f, 1.0f),
        //        0.6f,
        //        EasingInOutSine,
        //        CAMERA_MOVE_TYPE_SMOOTH
        //    );
        //    CameraTargetMoveRequestAll(30,
        //        zoomPos2Ta,
        //        EasingInOutSine,
        //        CAMERA_MOVE_TYPE_SMOOTH
        //    );
        //}
        {
            Float3 zoomPos = m_Obj->pos;
            //zoomPos.z += 32000.0f;
            zoomPos.z += -1000.0f;

            Float3 zoomPosTa = m_Obj->pos;
            //zoomPosTa.z += -14000.0f;
            zoomPosTa.z += -31000.0f;

            CameraTargetMoveRequestAll(30,
                zoomPosTa,
                EasingInOutSine,
                CAMERA_MOVE_TYPE_SMOOTH
            );
            CameraMoveRequestAll(30,
                zoomPos,
                0.0f,
                MkF2(1.0f, 1.0f),
                //0.35f,
                0.55f,
                EasingInOutSine,
                CAMERA_MOVE_TYPE_SMOOTH
            );
        }

        SoundBGMRequestStopAllRequest();
        SoundBGMPlayRequest(m_bgmLoop,
            true,
            2.0f,
            0.0f,

            bgmstartsec3
        );
    }
    int startingBeforeF = 180 + ragtemp + ragtemp2;
    if (m_Phase2StartFrame == startingBeforeF)
    {
        //m_StartEyeEffFrameMax = 25;
        //m_StartEyeEffFrame = m_StartEyeEffFrameMax;
        //CameraFlashRequestAll(
        //    40,
        //    EasingInOutSine,
        //    MkF4(5.0f, 0.0f, 10.0f, 0.9f),
        //    DPD_SHADER_OPT_BLEND_STATE_ADDCOLOR
        //);

        event_playerPosSet();

        phase2StartEvent(false);


        CameraFlashRequestAll(
            50,
            EasingInOutSine,
            MkF4(0.0f, 0.0f, 0.0f, 0.8f),
            DPD_SHADER_OPT_BLEND_STATE_DEFAULT
        );

        CameraShakeRequestAll(
            CAMERA_SHAKE_EXLARGE_FRAME * 1,
            CAMERA_SHAKE_EXLARGE_POWER
        );

        //SEを再生
        SoundEffectPlayRequest(
            GetSoundData("SOUND/SE/enemy/move/Inperius_Jet_Start"),
            2.6f, 0);
        //SEを再生
        SoundEffectPlayRequest(
            GetSoundData("SOUND/SE/enemy/attack/super_beam_start"),
            2.6f, 0, 1.35f);

        ResetAnim();
        StartAnim(AnimState::BeamBefore, 100, true);

        GE_GayaStartRequest("iseki_inperiusuhard_phase2start_mayu", 0);
    }
    //if (m_Phase2StartFrame == 250 + ragtemp + ragtemp2)
    if (m_Phase2StartFrame == startingBeforeF + 10)
    {
        {
            Float3 zoomPos = m_Obj->pos;
            //zoomPos.z += 32000.0f;
            zoomPos.z += 10000.0f;

            Float3 zoomPosTa = m_Obj->pos;
            //zoomPosTa.z += -14000.0f;
            zoomPosTa.z += -31000.0f;

            CameraTargetMoveRequestAll(FRAME_ONE_SECOND * 0.4,
                zoomPosTa,
                EasingInOutSine,
                CAMERA_MOVE_TYPE_SMOOTH
            );
            CameraMoveRequestAll(FRAME_ONE_SECOND * 0.4,
                zoomPos,
                0.0f,
                MkF2(1.0f, 1.0f),
                //0.35f,
                0.36f,
                EasingInOutSine,
                CAMERA_MOVE_TYPE_SMOOTH
            );
        }
    }

    if (m_Phase2StartFrame == startingBeforeF + FRAME_ONE_SECOND * 0.75)
    {
        Float3 zoomPos = m_Obj->pos;
        zoomPos.z += 100200.0f;

        CameraTargetMoveRequestAll(FRAME_ONE_SECOND * 1.0,
            MkF3(0.0f, -10000.0f, -12000.0f),
            EasingInOutSine,
            CAMERA_MOVE_TYPE_SMOOTH
        );
        CameraMoveRequestAll(FRAME_ONE_SECOND * 1.0,
            zoomPos,
            0.0f,
            MkF2(1.0f, 1.0f),
            //0.18f,
            0.1f,
            EasingInOutSine,
            CAMERA_MOVE_TYPE_SMOOTH
        );
    }

    if (m_Phase2StartFrame == 280 + ragtemp + ragtemp2)
    {
        StartAnim(AnimState::Beam, 60, true);
    }

    int startingF = 300 + ragtemp + ragtemp2;

    if (m_Phase2StartFrame == startingF)
    {
        //ムービーフラグ
        GameManagerSetMovieFlag(false);

        SetCameraModeAll(CAMERA_MODE_BOSS_CHASE);

        //プレイヤーを定位置に移動する
        for (int i = 0; i < PLAYER_MAX; i++)
        {
            PLAYER* player = GetPlayer(i);
            GAME_OBJECT* playerObj = player->obj;
            if (playerObj != nullptr)
            {
                GOE_Character_Player* goe =
                    dynamic_cast<GOE_Character_Player*>(playerObj->events.source);  // ダウンキャスト
                if (goe != nullptr)
                {
                    //hal::dout << "goe ptr=" << goe
                    //    << " real-type=" << typeid(*goe).name() << std::endl;
                    goe->SetKnockBack(
                        40.0f,
                        120,
                        m_Obj
                    );
                    //明示的によんどく
                    goe->SetDownPose();
                }
            }
        }
        //m_Obj->status.hpMin = 0;
        m_Obj->status.hpMin = HARD_HP_MIN;
    }

    m_Phase2StartFrame++;

    if (m_Phase2StartFrame > 340 + ragtemp + ragtemp2)
    {
        m_ActCycle = 0;
        m_ActCycleNum = 0;

        //_ActionCooldown = 150;
        //m_ActionCooldown = 110;
        //m_ActionCooldown = 1;
        m_ActionCooldown = 20;
        m_Phase2StartFrame = -1;
        m_CurrentState = ActionState::Idle;

        StartWarp(
            MkF3(
                0.0f,
                ONE_BLOCK * -30.0f,
                0.0f
            ),
            0.0f,
            25,
            true
        );


        {
            int summonEnemyNum = 2;
            for (int i = 0; i < summonEnemyNum; i++)
            {

                int summonEnemyNum2 = 3 + 2 * i;
                for (int j = 0; j < summonEnemyNum2; j++)
                {
                    float rad = 2.0f * SYS_GENERAL_PI / summonEnemyNum2 * j + SYS_GENERAL_PI;

                    Float2 dis = {
                        (BLOCK_WIDTH * 35) + (BLOCK_WIDTH * 32) * i,
                        (BLOCK_WIDTH * 24) + (BLOCK_WIDTH * 20) * i,
                    };

                    if (j % 2 == 0)
                    {
                        dis.x *= 0.8f;
                        dis.y *= 0.8f;
                    }


                    GAME_OBJECT_COLLIDER collider = {};
                    collider.type = GAME_OBJECT_COLLIDER_TYPE_CAPSULE;

                    collider.capsule.radius = BLOCK_WIDTH
                        * 3.0f
                        ;

                    Float3 capVec = {
                        sin(rad),
                        cos(rad),
                        0.0f
                    };
                    collider.capsule.pointA = capVec * BLOCK_WIDTH * +3.0f;
                    collider.capsule.pointB = capVec * BLOCK_WIDTH * -3.0f;
                    collider.capsule.pointA.z = collider.capsule.radius * 0.75f;
                    collider.capsule.pointB.z = collider.capsule.radius * 0.75f;

                    Float3 setpos = MkF3(
                        sin(rad) * dis.x,
                        cos(rad) * dis.y + BLOCK_HIGHT * -8.0f,
                        BLOCK_WIDTH * -100.0f
                    );

                    if (abs(setpos.x) < BLOCK_HIGHT * 8.0f
                        && (setpos.y) < BLOCK_HIGHT * 1.0f)
                    {
                        continue;
                    }

                    GOE_ObjectDynamic* goe = GOE_ObjectDynamic::CreateObjectDynamic(
                        GOE_ObjectDynamic::Type::General,
                        setpos,
                        collider
                    );
                    goe->SetOwner(GetIdentifierGameObject(m_Obj));
                    goe->StartFall(2 * FRAME_ONE_SECOND);

                    GameObjectHitStop(
                        goe->GetGameObj(),
                        16 * i + 8 * j
                    );

                }
            }
        }
    }

    //ムービースキップ処理 (多分後で変える)
    if (m_Phase2StartFrame < startingBeforeF
        && GameManagerIsMovieSkipTrigger()
        )
    {
        phase2StartEvent(true);

        CameraFlashRequestAll(20, EasingOutSine, MkF4(-1.0f, -1.0f, -1.0f, 1.0f));

        CameraMoveCancelRequestAll();
        CameraTargetMoveCancelRequestAll();
        SetCameraModeAll(CAMERA_MODE_PLAYER_CHASE_INSTANT);

        StopAnim();
        ResetAnim();

        SoundBGMRequestStopAllRequest();
        SoundBGMPlayRequest(m_bgmLoop,
            true,
            2.0f,
            0.0f,

            bgmstartsec3
        );
        m_Phase2StartFrame = startingF;
    }
}
void GOE_Character_Enemy_Boss_Inperiusu::draw_phase2Start()
{
}

void GOE_Character_Enemy_Boss_Inperiusu::phase2StartEvent(bool isfast)
{
    Stage_Base* stg = GetCurrentStageSceneGame();
    if (stg != nullptr)
    {
        Stage_Ruins* stgRuins = dynamic_cast<Stage_Ruins*>(stg);
        if (stgRuins != nullptr)
        {
            stgRuins->StageEventMutation(isfast);
        }
    }

    ShowBossbar_Game(
        GetGameObjectIndex(m_Obj),
        true,
        m_Obj->status.fullName,
        BOSSBAR_TYPE_GAME::Inperius_H,
        0.0f, HARD_HP_BORDER
    );
}


void GOE_Character_Enemy_Boss_Inperiusu::update_action_pattern()
{
    if (m_IsHard)
    {
        update_action_pattern_hard();
    }
    else
    {
        update_action_pattern_normal();
    }
}

void GOE_Character_Enemy_Boss_Inperiusu::update_action_pattern_normal()
{
    int actF = m_ActCycle % 9;


    if (m_StarnFrame > 0)
    {
        if (m_StarnFrame == SUPER_BEAM_AFTER_STARN_END)
        {
            //SEを再生
            SoundEffectPlayRequest(GetSoundData("SOUND/SE/enemy/move/Inperius_Jet_Start"),
                2.0f, 0);
            StartAnim(AnimState::BeamBefore, 80, true);
        }
        if (m_StarnFrame == 60)
        {
            StartAnim(AnimState::Beam, 30, true);


            zako_powerup();

            m_ActionCooldown = 0;
        }

        m_StarnFrame--;

        return;
    }

    if (m_ActionCooldown > 0)
    {

        //if (distance > ONE_BLOCK * 5.0f)
        //{ //接近
        //    m_Obj->vel.x += vec.x * ONE_BLOCK / 12;
        //    m_Obj->vel.y += vec.y * ONE_BLOCK / 12;
        //    m_Obj->vel.z += vec.z * ONE_BLOCK / 12;
        //    //if (privateDat->targetFrame % 40 == 0)
        //    //{
        //    //	GameObjectMoveRequest(obj, 60,
        //    //		MkF3(
        //    //			obj->pos.x + vec.x * OneBlock * 2.0f,
        //    //			obj->pos.y + vec.y * OneBlock * 2.0f,
        //    //			obj->pos.z + vec.z * OneBlock * 2.0f
        //    //		),
        //    //		EasingInOutSine
        //    //	);
        //    //}
        //}

        m_ActionCooldown--;

        //対象への向きを入れる

        //m_BeforeVecBuf = vec;
        return;
    }

    if (m_CurrentState != ActionState::Idle)
    {
        return;
    }

    if (m_StartedFrameCount < FRAME_ONE_SECOND * 4)
    {
        if (m_StartedFrameCount == FRAME_ONE_SECOND * 1.5f)
        {
            StartMove(
                MkF3(
                    m_Obj->pos.x,
                    //m_Obj->pos.y + ONE_BLOCK * 12,
                    -ONE_BLOCK * 8,
                    m_Obj->pos.z
                ),
                0.0f,
                FRAME_ONE_SECOND * 1.5f
            );
        }
        return;
    }

    Float3 targetPos = {};

    GAME_OBJECT* targetObject = GetGameObjectFindByIdentifier(m_TargetObjIdentifier);
    if (targetObject != nullptr)
    {
        targetPos = targetObject->pos;
    }

    bool approach = false;

    if (m_Obj->status.hp <= m_Obj->status.hpMax * SUPER_BEAM_HP_BORDER
        && !m_SuperBeamFirstFlag)
    { //穴井亜レーター

        m_SuperBeamBreakFlag = false;
        m_SuperBeamAfterFrame = -1;
        m_SuperBeamCountDownFrame = -1;
        m_SuperBeamFrame = 0;
        m_SuperBeamStartFrame = 0;
        m_CurrentState = ActionState::SuperBeam;

        m_WazaCooldown = WAZA_COOLDOWN_2;
        m_ActCycle = 6;

        m_SuperBeamFirstFlag = true;
    }
    else if (m_Obj->status.hp <= m_Obj->status.hpMax * 0.1)
    { //穴井亜レーター 2

        StartTurn(0.0f, 0.05f);

        m_ActionCooldown = INT32_MAX;

        //SEを再生
        SoundEffectPlayRequest(GetSoundData("SOUND/SE/inperiusu/b_kamae"), 1.3f, 0);

        //SEを再生
        SoundEffectPlayRequest(
            GetSoundData("SOUND/SE/enemy/attack/Annihilator_Charge"),
            0.8f, 20, 1.0f);

        StartAnim(AnimState::SuperBeamBefore, m_ActionCooldown, true);

        CameraFlashRequestAll(
            60,
            EasingInOutSine,
            //MkF4(0.9f, 0.7f, 0.5f, 0.8f),
            MkF4(2.0f, 0.1f, 5.0f, 0.5f),
            DPD_SHADER_OPT_BLEND_STATE_ADDCOLOR
        );

        SoundEffectPlayRequest(
            GetSoundData("SOUND/SE/enemy/move/warpHith"),
            3.2f, 0, 0.9f);
        //SEを再生
        SoundEffectPlayRequest(
            GetSoundData("SOUND/SE/enemy/move/warpLow"),
            3.2f, 2, 0.7f);

        GE_GayaStartRequest("iseki_inperiusu_final_mayu");
    }

    else if (actF == 0)
    {
        m_ActCycleNum++;

        approach = true;

        //if (m_TargetDistance > ONE_BLOCK * 24
        //    && m_TargetDistance < ONE_BLOCK * 40
        //)
        //{
        //    //でタレントマシンガン
        //    m_MachineGunFrame = 0;
        //    m_CurrentState = ActionState::MachineGun;
        //}
        //else
        //{ //布団ブレード
        //    m_BladeFrame = 0;
        //    m_CurrentState = ActionState::Blade;
        //}
        if (m_TargetDistance > ONE_BLOCK * 24
            && m_TargetDistance <= ONE_BLOCK * 50)
        {
            //でタレントマシンガン
            //m_MachineGunFrame = 0;
            //m_CurrentState = ActionState::MachineGun;
            m_MachineGunHandFrame = 0;
            m_CurrentState = ActionState::MachineGunHand;
        }
        else
        { //布団ブレード
            m_BladeFrame = 0;
            m_CurrentState = ActionState::Blade;
        }
    }
    else if (actF == 2)
    { //グレネード
        m_GranadeFrame = 0;
        m_CurrentState = ActionState::Granade;
    }
    else if (actF == 3)
    {
        if (m_CallEnemies.size() <= 4 && m_ActCycleNum <= 2)
        { //円まーじぇんしーコール
            m_EnemyCallFrame = 0;
            m_CurrentState = ActionState::EnemyCall;
        }
        else
        {//布団ブレード
            m_BladeFrame = 0;
            m_CurrentState = ActionState::Blade;
        }
    }
    else if (actF == 4)
    { //多段ミサイル
        m_MisileFrame = 0;
        m_CurrentState = ActionState::Misile;
    }
    else if (actF == 5)
    {
        m_ActCycleNum++;

        approach = true;

        if (m_TargetDistance > ONE_BLOCK * 24
            && m_TargetDistance <= ONE_BLOCK * 50)
        {
            //でタレントマシンガン
            //m_MachineGunFrame = 0;
            //m_CurrentState = ActionState::MachineGun;
            m_MachineGunHandFrame = 0;
            m_CurrentState = ActionState::MachineGunHand;
        }
        else
        { //布団ブレード
            m_BladeFrame = 0;
            m_CurrentState = ActionState::Blade;
        }
    }
    else if (actF == 6)
    { //多方向レーザー
        m_BeamFrame = 0;
        m_CurrentState = ActionState::Beam;
    }
    else if (actF == 7)
    { //
        m_ActionCooldown = 40;

        Float3 afpos = m_ActionStdPos[m_ActCycleNum % 3];
        if (CalculateDistance(afpos, m_Obj->pos) > ONE_BLOCK * 12)
        {
            float rot = CalculateRadToDeg(atan2(-afpos.y, -afpos.x)) - 90.0f;
            StartJet(afpos, rot, 
                160//240
            );

        }
    }
    else if (actF == 8)
    {
        if (m_CallEnemies.size() <= 4 && m_ActCycleNum <= 2)
        { //円まーじぇんしーコール
            m_EnemyCallFrame = 0;
            m_CurrentState = ActionState::EnemyCall;
        }
        else
        { //布団ブレード
            m_BladeFrame = 0;
            m_CurrentState = ActionState::Blade;
        }
        //{ //オービットレーザー
        //    m_EnergyBallBitFrame = 0;
        //    m_CurrentState = ActionState::EnergyBallBit;
        //}
    }

    //if (actF == 3)
    //{ //仮
    //    Character::MAIN_LOAD* charatmp = Character::Load("ENEMY_MIRESU_SHOOTER");
    //    int enemNum = 3;
    //    for (int i = 0; i < enemNum; i++)
    //    {
    //        GAME_OBJECT* objtmp = Character::CreateCharacterGameObject(charatmp);

    //        if (objtmp != nullptr)
    //        {
    //            objtmp->pos = m_Obj->pos;
    //            objtmp->pos.x += sin(SYS_GENERAL_PI * 2.0f / enemNum * i) * ONE_BLOCK * 8;
    //            objtmp->pos.y += cos(SYS_GENERAL_PI * 2.0f / enemNum * i) * ONE_BLOCK * 8;
    //        }
    //    }
    //    Character::Unload(charatmp);
    //}

    if (approach)
    {
        if (CalculateDistance(targetPos, m_Obj->pos) > ONE_BLOCK * 50)
        {
            Float2 vec = NormalizeVector(MkF2(
                0.0f - targetPos.x,
                0.0f - targetPos.y
            ));
            StartJet(
                MkF3(
                    targetPos.x + vec.x * ONE_BLOCK * 12.0f,
                    targetPos.y + vec.y * ONE_BLOCK * 12.0f,
                    targetPos.z
                ),
                0.0f,
                160//240
            );
        }
    }

    m_ActCycle++;


}


void GOE_Character_Enemy_Boss_Inperiusu::update_action_pattern_hard()
{
    if (m_ActionCooldown > 0)
    {
        m_ActionCooldown--;

        return;
    }

    if (m_CurrentState != ActionState::Idle)
    {
        return;
    }

    if (m_StarnFrame > 0)
    {
        m_StarnFrame--;

        return;
    }

    if (m_HardPhase2Flag)
    {
        update_action_pattern_hard2();
    }
    else
    {
        update_action_pattern_hard1();
    }
}

void GOE_Character_Enemy_Boss_Inperiusu::update_action_pattern_hard1()
{
    int actF = m_ActCycle % 9;


    if (m_StartedFrameCount < FRAME_ONE_SECOND * 3)
    {
        if (m_StartedFrameCount == FRAME_ONE_SECOND * 1.5f)
        {
            StartMove(
                MkF3(
                    m_Obj->pos.x,
                    //m_Obj->pos.y + ONE_BLOCK * 12,
                    -ONE_BLOCK * 8,
                    m_Obj->pos.z
                ),
                0.0f,
                FRAME_ONE_SECOND * 1.5f
            );
        }
        return;
    }

    if (actF == 0)
    { //布団ブレード

        //if (!m_SuperBeamFirstFlag)
        //{ //穴井亜レーター

        //    m_SuperBeamBreakFlag = false;
        //    m_SuperBeamAfterFrame = -1;
        //    m_SuperBeamCountDownFrame = -1;
        //    m_SuperBeamFrame = 0;
        //    m_SuperBeamStartFrame = 0;
        //    m_CurrentState = ActionState::SuperBeam;

        //    m_SuperBeamFirstFlag = true;
        //}
        //else
        { //布団ブレード
            m_ActCycleNum++;

            m_BladeFrame = 0;
            m_CurrentState = ActionState::Blade;

            if (m_ActCycleNum <= 1)
            {
                m_BladeWarpNum = 6;
                m_BladeBeforeSpeedMod = 1.0f;
                m_BladeWaveNum = 1;
            }
            else
            {
                m_BladeWarpNum = m_ActCycleNum % 2 == 0 ? 0 : 1;
                m_BladeBeforeSpeedMod = m_BladeWarpNum <= 0 ? 0.4f : 0.8f;
                m_BladeWaveNum = m_BladeWarpNum <= 0 ? 0 : 1;
            }
        }
    }
    else if (actF == 1)
    {
        if (!m_SuperBeamFirstFlag)
        { //穴井亜レーター

            m_SuperBeamBreakFlag = false;
            m_SuperBeamAfterFrame = -1;
            m_SuperBeamCountDownFrame = -1;
            m_SuperBeamFrame = 0;
            m_SuperBeamStartFrame = 0;
            m_CurrentState = ActionState::SuperBeam;

            m_SuperBeamFirstFlag = true;
        }
        else
        { //オービットレーザー
            m_EnergyBallBitFrame = 0;
            m_CurrentState = ActionState::EnergyBallBit;
        }
    }
    else if (actF == 2)
    {
        //if (!m_SuperBeamFirstFlag)
        //{ //穴井亜レーター

        //    m_SuperBeamBreakFlag = false;
        //    m_SuperBeamAfterFrame = -1;
        //    m_SuperBeamCountDownFrame = -1;
        //    m_SuperBeamFrame = 0;
        //    m_SuperBeamStartFrame = 0;
        //    m_CurrentState = ActionState::SuperBeam;

        //    m_SuperBeamFirstFlag = true;
        //}
        //else
        { //多方向レーザー
            m_BeamFrame = 0;
            m_CurrentState = ActionState::Beam;
        }
    }
    else if (actF == 3)
    { //
        m_ActionCooldown = 40;

        Float3 afpos = m_ActionStdPos[m_ActCycleNum % 3];
        if (CalculateDistance(afpos, m_Obj->pos) > ONE_BLOCK * 8)
        {
            float rot = CalculateRadToDeg(atan2(-afpos.y, -afpos.x)) - 90.0f;
            StartWarp(afpos, rot, 25);
        }
    }
    else if (actF == 4)
    { //グレネード
        m_GranadeFrame = 0;
        m_CurrentState = ActionState::Granade;
    }
    else if (actF == 5)
    { //オービットレーザー
        m_EnergyBallBitFrame = 0;
        m_CurrentState = ActionState::EnergyBallBit;
    }
    else if (actF == 6)
    { //布団ブレード
        { //布団ブレード
            m_BladeFrame = 0;
            m_BladeWarpNum = 4;
            m_BladeBeforeSpeedMod = 0.8f;
            m_BladeWaveNum = 1;
            m_CurrentState = ActionState::Blade;
        }
    }
    else if (actF == 7)
    { //多段ミサイル
        m_MisileFrame = 0;
        m_CurrentState = ActionState::Misile;
    }
    else if (actF == 8)
    {
        if (m_CallEnemies.size() <= 4 && m_ActCycleNum <= 1)
        { //円まーじぇんしーコール
            m_EnemyCallFrame = 0;
            m_CurrentState = ActionState::EnemyCall;
        }
        else
        {
            m_ActionCooldown = 40;

            Float3 afpos = m_ActionStdPos[(m_ActCycleNum + 1) % 3];
            if (CalculateDistance(afpos, m_Obj->pos) > ONE_BLOCK * 8)
            {
                float rot = CalculateRadToDeg(atan2(-afpos.y, -afpos.x)) - 90.0f;
                StartWarp(afpos, rot, 25);
            }
        }
    }

    m_ActCycle++;
}

void GOE_Character_Enemy_Boss_Inperiusu::update_action_pattern_hard2()
{
    int actF = m_ActCycle % 25;

    m_BladeMirrogeFlag = false;
    m_BladeWarpHastenFrame = 0;
    m_BladeSuperMirrogeFlag = false;

    if (m_Obj->status.hp <= m_Obj->status.hpMax * HARD_HP_BORDER * 0.4)
    {
        if (m_Phase2HpPhase < 3)
        {
            m_Phase2HpPhase = 3;

            m_WazaCooldown = FRAME_ONE_SECOND * 0.75f;

            SetPostEffectShaderVariable((float)GetMainGameFrame(), 0);
            SetPostEffectShaderVariable(1.0f, 1);
            SetPostEffectShaderVariable(2.0f, 2);
            SetPostEffectShaderVariable(3.0f, 3);
        }
    }
    else if (m_Obj->status.hp <= m_Obj->status.hpMax * HARD_HP_BORDER * 0.5)
    {
        if (m_Phase2HpPhase < 2)
        {
            m_Phase2HpPhase = 2;
        }
    }
    else if (m_Obj->status.hp <= m_Obj->status.hpMax * HARD_HP_BORDER * 0.75)
    {
        if (m_Phase2HpPhase < 1)
        {
            m_Phase2HpPhase = 1;

            m_WazaCooldown = FRAME_ONE_SECOND * 1.0f;
        }
    }
    else
    {
        if (m_Phase2HpPhase != 0)
        {
            m_Phase2HpPhase = 0;

            m_WazaCooldown = FRAME_ONE_SECOND * 2.0f;
        }
    }

    char hpPhase = m_Phase2HpPhase;

    //if (m_Obj->status.hp <= HARD_HP_MIN)
    //{ //穴井亜レーター
    //    m_SuperBeamLimitF = FRAME_ONE_SECOND * 40.0f;
    //    m_SuperBeamFinish = true;
    //    m_SuperBeamPosition = { 0.0f, ONE_BLOCK * -8.0f, 0.0f };

    //    m_SuperBeamBreakFlag = false;
    //    m_SuperBeamAfterFrame = -1;
    //    m_SuperBeamCountDownFrame = -1;
    //    m_SuperBeamFrame = 0;
    //    m_SuperBeamStartFrame = 0;
    //    m_CurrentState = ActionState::SuperBeam;

    //    m_WazaCooldown = WAZA_COOLDOWN_2;
    //    m_ActCycle = 0;

    //    m_SuperBeamFirstFlag = true;
    //}
    //else
    //if (1)
    //{
    //}
    //else 
        if (actF == 0)
    {
        ////初回は回復イベントを発生させる
        //if (m_ActCycleNum <= 0)
        //{
        //    m_HealEventFrame = 0;
        //    m_CurrentState = ActionState::HealEvent;
        //}

        //初回は待機イベントを発生させる
        if (m_ActCycleNum <= 0)
        {
            m_WaitEventFrame = 0;
            m_WaitEventFrameMax = 260;
            m_CurrentState = ActionState::WaitEvent;

        }
    }
    else 
        if (actF == 1)
    {
        m_ActCycleNum++;

        if(m_Phase2HpPhase == 3)
        { //多方向レーザー
            m_BeamFrame = 0;
            m_BeamNum = 5;
            m_BeamIsOutLine = true;
            m_BeamShockWaveType = true;
            m_BeamOutLineRangeMod = 0.0f;
            m_BeamNoInitMove = false;
            m_BeamAfterWarpFlag = false;
            m_BeamRotatePattarn = 0;
            m_CurrentState = ActionState::Beam;
        }
        else if(m_ActCycleNum > 3)
        {
            {
                MirrorgeStateStartSet(0,
                    -90.0f,
                    {
                        ONE_BLOCK * -50.0f,
                        ONE_BLOCK * -12.0f,
                        0.0f
                    },
                    {},
                    MkF3(1.0f, 1.0f, 1.0f)
                );
            }
            {
                MirrorgeStateStartSet(1,
                    90.0f,
                    {
                        ONE_BLOCK * 50.0f,
                        ONE_BLOCK * -12.0f,
                        0.0f
                    },
                    {},
                    MkF3(1.0f, 1.0f, 1.0f)
                );
            }
            { //多方向レーザー
                m_BeamFrame = 0;
                m_BeamNum = 2;
                m_BeamIsOutLine = true;
                m_BeamShockWaveType = false;
                m_BeamOutLineRangeMod = 0.0f;
                //m_BeamNoInitMove = true;
                m_BeamOutLineRangeMod = 1600.0f;
                m_BeamNoInitMove = false;
                m_BeamAfterWarpFlag = false;
                m_BeamRotatePattarn = 0;
                m_CurrentState = ActionState::Beam;
            }
        }
        else
        { //多方向レーザー
            m_BeamFrame = 0;
            m_BeamNum = 8;
            m_BeamIsOutLine = true;
            //m_BeamOutLineRangeMod = 3000.0f;

            m_BeamShockWaveType = false;
            m_BeamOutLineRangeMod = 0.0f;
            m_BeamNoInitMove = false;
            m_BeamAfterWarpFlag = false;
            //m_BeamRotatePattarn = (m_ActCycleNum - 1) % 3;
            m_BeamRotatePattarn = 0;
            m_CurrentState = ActionState::Beam;
        }
    }
        ////else if (actF == 2)
        ////{ //グレネード
        ////    m_GranadeFrame = 0;
        ////    m_GranadeSuperFlag = true;
        ////    m_CurrentState = ActionState::Granade;
        ////}
        //else if (actF == 2)
        //{
        //    //{
        //    //    StartWarp(
        //    //        {
        //    //            ONE_BLOCK * 45.0f,
        //    //            ONE_BLOCK * -12.0f,
        //    //            0.0f
        //    //        },
        //    //        90, 
        //    //        25,
        //    //        true);
        //    //}
        //    {
        //        MirrorgeStateStartSet(0,
        //            -90.0f,
        //            {
        //                ONE_BLOCK * -50.0f,
        //                ONE_BLOCK * -12.0f,
        //                0.0f
        //            },
        //            {},
        //            MkF3(1.0f, 1.0f, 1.0f)
        //        );
        //    }
        //    {
        //        MirrorgeStateStartSet(1,
        //            90.0f,
        //            {
        //                ONE_BLOCK * 50.0f,
        //                ONE_BLOCK * -12.0f,
        //                0.0f
        //            },
        //            {},
        //            MkF3(1.0f, 1.0f, 1.0f)
        //        );
        //    }
        //    { //多方向レーザー
        //        m_BeamFrame = 0;
        //        m_BeamNum = 2;
        //        m_BeamIsOutLine = true;
        //        //m_BeamNoInitMove = true;
        //        m_BeamOutLineRangeMod = 1600.0f;
        //        m_BeamNoInitMove = false;
        //        m_BeamAfterWarpFlag = false;
        //        //m_BeamRotatePattarn = (m_ActCycleNum - 1) % 3;
        //        m_BeamRotatePattarn = (m_ActCycleNum - 1) % 3;
        //        m_CurrentState = ActionState::Beam;
        //    }
        //    m_ActCycle = 0;
        //    //m_ActCycleNum = 1;
        //}
    else if (actF == 2)
    {
        //if (hpPhase >= 2)
        if (m_ActCycleNum <= 1)
        { //布団ブレード
            m_BladeFrame = 0;
            m_CurrentState = ActionState::Blade;

            Float3 actionStdPos[3] = {
                { ONE_BLOCK * 50.0f, ONE_BLOCK * -28.0f, 0.0f },
                { ONE_BLOCK * -50.0f, ONE_BLOCK * -28.0f, 0.0f },
                { 0.0f, ONE_BLOCK * -60.0f, 0.0f },
            };
            Float3 afpos = actionStdPos[(m_ActCycleNum - 1) % 3];
            float rot = CalculateRadToDeg(atan2(-afpos.y, -afpos.x)) - 90.0f;
            StartWarp(afpos, rot, 25, true);

            m_BladeWarpNum = 0;
            //m_BladeBeforeSpeedMod = 1.8f;
            //m_BladeBeforeSpeedMod = 2.6f;
            m_BladeBeforeSpeedMod = 2.4f;
            m_BladeWaveNum = 7;
            m_BladeWaveAngle = 120.0f;
            m_BladeNoTurnFlag = true;
            m_BladeAfterWarpFlag = true;
            m_BladeSuperFlag = true;

            //if (m_ActCycleNum <= 1)
            //{
            //    m_BladeWarpNum = 3;
            //    m_BladeBeforeSpeedMod = 1.0f;
            //    m_BladeWaveNum = 1;
            //}
            //else
            //{
            //    m_BladeWarpNum = m_ActCycleNum % 2 == 0 ? 0 : 1;
            //    m_BladeBeforeSpeedMod = m_BladeWarpNum <= 0 ? 0.4f : 0.8f;
            //}
        }
        else
        {
            { //布団ブレード
                m_BladeFrame = 0;
                m_CurrentState = ActionState::Blade;

                {
                    //Float3 afpos = {
                    //    0.0f,
                    //    ONE_BLOCK * -50.0f,
                    //    0.0f
                    //};
                    Float3 afpos = {
                        ONE_BLOCK * 75.0f,
                        ONE_BLOCK * -36.0f,
                        0.0f
                    };
                    float rot = 0;
                    StartWarp(afpos, rot, 25, true);
                }

                int bodynum = 5;
                for (int i = 0; i < bodynum; i++)
                {
                    int index = i + 0;
                    Float3 afpos = {
                        ONE_BLOCK * -75.0f,
                        ONE_BLOCK * -36.0f,
                        0.0f
                    };
                    Float3 afpos2 = afpos;
                    afpos2.x += ONE_BLOCK * 140.0f / bodynum * index;
                    afpos2.y *= index % 2 == 0 ? -1.0f : 1.0f;
                    //afpos2.x *= index % 2 == 0 ? -1.0f : 1.0f;
                    //afpos2.y += ONE_BLOCK * 90.0f / bodynum * index;
                    MirrorgeStateStartSet(i,
                        index % 2 == 0 ? 180.0f : 0.0f,
                        //index % 2 == 0 ? 90.0f : 270.0f,
                        afpos2,
                        {},
                        MkF3(1.0f, 1.0f, 1.0f)
                    );
                }

                m_BladeWarpNum = 0;
                m_BladeBeforeSpeedMod = 2.4f;
                //m_BladeBeforeSpeedMod = 1.2f;
                m_BladeWaveNum = 1;
                m_BladeWaveAngle = 0.0f;
                m_BladeNoTurnFlag = true;
                m_BladeAfterWarpFlag = true;
                m_BladeSuperFlag = true;
                m_BladeSuperMirrogeFlag = true;
            }
        }
    }
    else if (actF == 3)
    { //グレネード
        m_GranadeFrame = 0;
        m_GranadeSuperFlag = true;
        m_CurrentState = ActionState::Granade;
    }
    else if (actF == 4)
    { //多段ミサイル
        m_MisileFrame = 0;
        m_CurrentState = ActionState::Misile;
    }
    else if (actF == 5)
    {
        if (m_CallEnemies.size() <= 4 && m_ActCycleNum <= 1)
        { //円まーじぇんしーコール
            m_EnemyCallFrame = 0;
            m_CurrentState = ActionState::EnemyCall;
        }
        else
        {
            m_ActionCooldown = 40;

            Float3 afpos = m_ActionStdPos[(m_ActCycleNum + 1) % 3];
            if (CalculateDistance(afpos, m_Obj->pos) > ONE_BLOCK * 8)
            {
                float rot = CalculateRadToDeg(atan2(-afpos.y, -afpos.x)) - 90.0f;
                StartWarp(afpos, rot, 25);
            }
        }
    }
    else if (actF == 6)
    {
        { //布団ブレード
            m_BladeFrame = 0;
            m_CurrentState = ActionState::Blade;

            if (m_ActCycleNum <= 1)
            {
                m_BladeWarpNum = 3;
                m_BladeBeforeSpeedMod = 1.0f;
                m_BladeWaveNum = 2;
                m_BladeWaveAngle = 140.0f;
                m_BladeNoTurnFlag = false;
                m_BladeAfterWarpFlag = false;
                m_BladeSuperFlag = false;
            }
            else
            {
                m_BladeWarpNum = m_ActCycleNum % 2 == 0 ? 0 : 1;
                m_BladeBeforeSpeedMod = m_BladeWarpNum <= 0 ? 0.4f : 0.8f;
                m_BladeWaveNum = m_BladeWarpNum <= 0 ? 1 : 2;
                m_BladeWaveAngle = 140.0f;
                m_BladeNoTurnFlag = false;
                m_BladeAfterWarpFlag = false;
                m_BladeSuperFlag = false;
            }
        }
    }
    else if (actF == 7)
    {
        { //オービットレーザー
            m_EnergyBallBitFrame = 0;
            m_CurrentState = ActionState::EnergyBallBit;
        }
    }
    else if (actF == 8)
    { //
        if (m_ActCycleNum <= 1)
        {
            m_ActionCooldown = 40;

            Float3 afpos = { 0.0f, ONE_BLOCK * -20.0f, 0.0f };
            if (CalculateDistance(afpos, m_Obj->pos) > ONE_BLOCK * 8)
            {
                float rot = CalculateRadToDeg(atan2(-afpos.y, -afpos.x)) - 90.0f;
                StartWarp(afpos, rot, 25, true);
            }
        }
        else
        {
            m_ActionCooldown = 40;

            Float3 afpos = m_ActionStdPos[(m_ActCycleNum - 1) % 3];
            if (CalculateDistance(afpos, m_Obj->pos) > ONE_BLOCK * 8)
            {
                float rot = CalculateRadToDeg(atan2(-afpos.y, -afpos.x)) - 90.0f;
                StartWarp(afpos, rot, 25);
            }
        }
    }
    else if (actF == 9)
    {
        //初回は待機させる
        if (m_ActCycleNum <= 1)
        {
            //m_ActionCooldown = 360;
            m_WaitEventFrame = 0;
            m_WaitEventFrameMax = 360;
            m_CurrentState = ActionState::WaitEvent;
        }
        else
        { //布団ブレード
            m_BladeFrame = 0;
            m_CurrentState = ActionState::Blade;

            {
                m_BladeWarpNum = 0;
                m_BladeBeforeSpeedMod = 1.2f;
                m_BladeWaveNum = 3;
                m_BladeWaveAngle = 160.0f;
                m_BladeNoTurnFlag = false;
                m_BladeAfterWarpFlag = false;
                m_BladeSuperFlag = false;
            }
        }
    }
    else if (actF == 10)
    { //グレネード
        m_GranadeFrame = 0;
        m_GranadeSuperFlag = false;
        m_CurrentState = ActionState::Granade;
    }
    else if (actF == 11)
    {
        if (m_ActCycleNum <= 1)
        { //布団ブレード
            m_BladeFrame = 0;
            m_CurrentState = ActionState::Blade;

            {
                m_BladeWarpNum = 2;
                m_BladeBeforeSpeedMod = 0.8f;
                m_BladeWaveNum = 1;
                m_BladeWaveAngle = 0.0f;
                m_BladeNoTurnFlag = false;
                m_BladeAfterWarpFlag = false;
                m_BladeSuperFlag = false;
            }
        }
        else
        { //布団ブレード
            m_BladeFrame = 0;
            m_CurrentState = ActionState::Blade;

            {
                m_BladeWarpHastenFrame = ATTACK_BLADE_BEFORE_FIXED_FRAME;
                m_BladeMirrogeFlag = true;

                m_BladeWarpNum = 1;
                m_BladeBeforeSpeedMod = 1.4f;
                m_BladeWaveNum = 1;
                m_BladeWaveAngle = 0.0f;
                m_BladeNoTurnFlag = false;
                m_BladeAfterWarpFlag = false;
                m_BladeSuperFlag = false;
            }
        }
    }

    else if (actF == 12)
    {
        m_ActCycleNum++;

        if (m_Phase2HpPhase == 3)
        { //多方向レーザー
            m_BeamFrame = 0;
            m_BeamNum = 5;
            m_BeamIsOutLine = true;
            m_BeamShockWaveType = true;
            m_BeamOutLineRangeMod = 0.0f;
            m_BeamNoInitMove = false;
            m_BeamAfterWarpFlag = false;
            m_BeamRotatePattarn = 1;
            m_CurrentState = ActionState::Beam;
        }
        else if (m_ActCycleNum > 3)
        {
            {
                MirrorgeStateStartSet(0,
                    -90.0f,
                    {
                        ONE_BLOCK * -50.0f,
                        ONE_BLOCK * -12.0f,
                        0.0f
                    },
                    {},
                    MkF3(1.0f, 1.0f, 1.0f)
                );
            }
            {
                MirrorgeStateStartSet(1,
                    90.0f,
                    {
                        ONE_BLOCK * 50.0f,
                        ONE_BLOCK * -12.0f,
                        0.0f
                    },
                    {},
                    MkF3(1.0f, 1.0f, 1.0f)
                );
            }
            { //多方向レーザー
                m_BeamFrame = 0;
                m_BeamNum = 2;
                m_BeamIsOutLine = true;
                m_BeamShockWaveType = false;
                m_BeamOutLineRangeMod = 0.0f;
                //m_BeamNoInitMove = true;
                m_BeamOutLineRangeMod = 1600.0f;
                m_BeamNoInitMove = false;
                m_BeamAfterWarpFlag = false;
                m_BeamRotatePattarn = 1;
                m_CurrentState = ActionState::Beam;
            }
        }
        else
        { //多方向レーザー
            m_BeamFrame = 0;
            m_BeamNum = 8;
            m_BeamIsOutLine = true;
            m_BeamShockWaveType = false;
            m_BeamOutLineRangeMod = 0.0f;
            m_BeamNoInitMove = false;
            m_BeamAfterWarpFlag = false;
            m_BeamRotatePattarn = 1;
            m_CurrentState = ActionState::Beam;
        }
    }
    else if (actF == 13)
    {
        if(m_ActCycleNum <= 2)
        { //布団ブレード
            m_BladeFrame = 0;
            m_CurrentState = ActionState::Blade;

            Float3 actionStdPos[3] = {
                { ONE_BLOCK * 50.0f, ONE_BLOCK * -28.0f, 0.0f },
                { ONE_BLOCK * -50.0f, ONE_BLOCK * -28.0f, 0.0f },
                { 0.0f, ONE_BLOCK * -60.0f, 0.0f },
            };
            Float3 afpos = actionStdPos[(m_ActCycleNum - 1) % 3];
            float rot = CalculateRadToDeg(atan2(-afpos.y, -afpos.x)) - 90.0f;
            StartWarp(afpos, rot, 25, true);

            m_BladeWarpNum = 0;
            //m_BladeBeforeSpeedMod = 1.8f;
            //m_BladeBeforeSpeedMod = 2.6f;
            m_BladeBeforeSpeedMod = 2.4f;
            m_BladeWaveNum = 7;
            m_BladeWaveAngle = 120.0f;
            m_BladeNoTurnFlag = true;
            m_BladeAfterWarpFlag = true;
            m_BladeSuperFlag = true;

            //if (m_ActCycleNum <= 1)
            //{
            //    m_BladeWarpNum = 3;
            //    m_BladeBeforeSpeedMod = 1.0f;
            //    m_BladeWaveNum = 1;
            //}
            //else
            //{
            //    m_BladeWarpNum = m_ActCycleNum % 2 == 0 ? 0 : 1;
            //    m_BladeBeforeSpeedMod = m_BladeWarpNum <= 0 ? 0.4f : 0.8f;
            //}
        }
        else
        {
            { //布団ブレード
                m_BladeFrame = 0;
                m_CurrentState = ActionState::Blade;

                {
                    {
                        Float3 afpos = {
                            ONE_BLOCK * 35.0f,
                            ONE_BLOCK * -50.0f,
                            0.0f
                        };
                        float rot = 0;
                        StartWarp(afpos, rot, 25, true);
                    }
                    {
                        Float3 afpos = {
                            ONE_BLOCK * -35.0f,
                            ONE_BLOCK * -50.0f,
                            0.0f
                        };
                        float rot = 0;
                        MirrorgeStateStartSet(0,
                            rot,
                            afpos,
                            {},
                            MkF3(1.0f, 1.0f, 1.0f)
                        );
                    }
                    {
                        Float3 afpos = {
                            ONE_BLOCK * 35.0f,
                            ONE_BLOCK * 50.0f,
                            0.0f
                        };
                        float rot = 180.0f;
                        MirrorgeStateStartSet(1,
                            rot,
                            afpos,
                            {},
                            MkF3(1.0f, 1.0f, 1.0f)
                        );
                    }
                    {
                        Float3 afpos = {
                            ONE_BLOCK * -35.0f,
                            ONE_BLOCK * 50.0f,
                            0.0f
                        };
                        float rot = 180.0f;
                        MirrorgeStateStartSet(2,
                            rot,
                            afpos,
                            {},
                            MkF3(1.0f, 1.0f, 1.0f)
                        );
                    }
                    {
                        Float3 afpos = {
                            ONE_BLOCK * -80.0f,
                            ONE_BLOCK * 28.0f,
                            0.0f
                        };
                        float rot = 270.0f;
                        MirrorgeStateStartSet(3,
                            rot,
                            afpos,
                            {},
                            MkF3(1.0f, 1.0f, 1.0f)
                        );
                    }
                    {
                        Float3 afpos = {
                            ONE_BLOCK * -80.0f,
                            ONE_BLOCK * -28.0f,
                            0.0f
                        };
                        float rot = 270.0f;
                        MirrorgeStateStartSet(4,
                            rot,
                            afpos,
                            {},
                            MkF3(1.0f, 1.0f, 1.0f)
                        );
                    }
                    {
                        Float3 afpos = {
                            ONE_BLOCK * 80.0f,
                            ONE_BLOCK * 28.0f,
                            0.0f
                        };
                        float rot = 90.0f;
                        MirrorgeStateStartSet(5,
                            rot,
                            afpos,
                            {},
                            MkF3(1.0f, 1.0f, 1.0f)
                        );
                    }
                    {
                        Float3 afpos = {
                            ONE_BLOCK * 80.0f,
                            ONE_BLOCK * -28.0f,
                            0.0f
                        };
                        float rot = 90.0f;
                        MirrorgeStateStartSet(6,
                            rot,
                            afpos,
                            {},
                            MkF3(1.0f, 1.0f, 1.0f)
                        );
                    }
                }


                m_BladeWarpNum = 0;
                m_BladeBeforeSpeedMod = 2.4f;
                //m_BladeBeforeSpeedMod = 1.2f;
                m_BladeWaveNum = 1;
                m_BladeWaveAngle = 0.0f;
                m_BladeNoTurnFlag = true;
                m_BladeAfterWarpFlag = true;
                m_BladeSuperFlag = true;
                m_BladeSuperMirrogeFlag = true;
            }
        }
    }
    else if (actF == 14)
    { //グレネード
        m_GranadeFrame = 0;
        m_GranadeSuperFlag = true;
        m_CurrentState = ActionState::Granade;
    }
    else if (actF == 15)
    { //多段ミサイル
        m_MisileFrame = 0;
        m_CurrentState = ActionState::Misile;
    }
    else if (actF == 16)
    {
        if (m_CallEnemies.size() <= 4 && m_ActCycleNum <= 2)
        { //円まーじぇんしーコール
            m_EnemyCallFrame = 0;
            m_CurrentState = ActionState::EnemyCall;
        }
        else
        {
            m_ActionCooldown = 40;

            Float3 afpos = m_ActionStdPos[(m_ActCycleNum + 1) % 3];
            if (CalculateDistance(afpos, m_Obj->pos) > ONE_BLOCK * 8)
            {
                float rot = CalculateRadToDeg(atan2(-afpos.y, -afpos.x)) - 90.0f;
                StartWarp(afpos, rot, 25);
            }
        }
    }
    else if (actF == 17)
    {
        if (m_ActCycleNum <= 2)
        { //布団ブレード
            m_BladeFrame = 0;
            m_CurrentState = ActionState::Blade;
            
            {
                m_BladeWarpNum = m_ActCycleNum % 2 == 0 ? 0 : 1;
                m_BladeBeforeSpeedMod = m_BladeWarpNum <= 0 ? 0.4f : 0.8f;
                m_BladeWaveNum = m_BladeWarpNum <= 0 ? 1 : 2;
                m_BladeWaveAngle = 140.0f;
                m_BladeNoTurnFlag = false;
                m_BladeAfterWarpFlag = false;
                m_BladeSuperFlag = false;
            }
        }
        else
        { //布団ブレード
            m_BladeFrame = 0;
            m_CurrentState = ActionState::Blade;

            {
                m_BladeWarpHastenFrame = ATTACK_BLADE_BEFORE_FIXED_FRAME;
                m_BladeMirrogeFlag = true;

                m_BladeWarpNum = 1;
                m_BladeBeforeSpeedMod = 1.2f;
                m_BladeWaveNum = 2;
                m_BladeWaveAngle = 140.0f;
                m_BladeNoTurnFlag = false;
                m_BladeAfterWarpFlag = false;
                m_BladeSuperFlag = false;
            }
        }
    }
    //else if (actF == 18)
    //{
    //    { //オービットレーザー
    //        m_EnergyBallBitFrame = 0;
    //        m_CurrentState = ActionState::EnergyBallBit;
    //    }
    //}

    //else if (actF == 19)
    //{ //多段ミサイル
    // 
    // //忘れずに
    //    m_ActCycleNum++;

    //    m_MisileFrame = 0;
    //    m_CurrentState = ActionState::Misile;
    //}
    //else if (actF == 20)
    //{ //グレネード
    //    m_GranadeFrame = 0;
    //    m_GranadeSuperFlag = true;
    //    m_CurrentState = ActionState::Granade;
    //}
    else if (actF == 21)
    {
        //忘れずに
        m_ActCycleNum++;

        if (m_Phase2HpPhase == 3)
        { //多方向レーザー
            m_BeamFrame = 0;
            m_BeamNum = 5;
            m_BeamIsOutLine = true;
            m_BeamShockWaveType = true;
            m_BeamOutLineRangeMod = 0.0f;
            m_BeamNoInitMove = false;
            m_BeamAfterWarpFlag = false;
            m_BeamRotatePattarn = 2;
            m_CurrentState = ActionState::Beam;
        }
        else if (m_ActCycleNum > 3)
        {
            {
                MirrorgeStateStartSet(0,
                    -90.0f,
                    {
                        ONE_BLOCK * -50.0f,
                        ONE_BLOCK * -12.0f,
                        0.0f
                    },
                    {},
                    MkF3(1.0f, 1.0f, 1.0f)
                );
            }
            {
                MirrorgeStateStartSet(1,
                    90.0f,
                    {
                        ONE_BLOCK * 50.0f,
                        ONE_BLOCK * -12.0f,
                        0.0f
                    },
                    {},
                    MkF3(1.0f, 1.0f, 1.0f)
                );
            }
            { //多方向レーザー
                m_BeamFrame = 0;
                m_BeamNum = 2;
                m_BeamIsOutLine = true;
                m_BeamShockWaveType = false;
                m_BeamOutLineRangeMod = 0.0f;
                //m_BeamNoInitMove = true;
                m_BeamOutLineRangeMod = 1600.0f;
                m_BeamNoInitMove = false;
                m_BeamAfterWarpFlag = false;
                m_BeamRotatePattarn = 2;
                m_CurrentState = ActionState::Beam;
            }
        }
        else
        { //多方向レーザー
            m_BeamFrame = 0;
            m_BeamNum = 8;
            m_BeamIsOutLine = true;
            m_BeamShockWaveType = false;
            m_BeamOutLineRangeMod = 0.0f;
            m_BeamNoInitMove = false;
            m_BeamAfterWarpFlag = false;
            m_BeamRotatePattarn = 2;
            m_CurrentState = ActionState::Beam;
        }
    }
    else if (actF == 22)
    {
        if(m_ActCycleNum <= 3)
        { //布団ブレード
            m_BladeFrame = 0;
            m_CurrentState = ActionState::Blade;

            Float3 actionStdPos[3] = {
                { ONE_BLOCK * 50.0f, ONE_BLOCK * -28.0f, 0.0f },
                { ONE_BLOCK * -50.0f, ONE_BLOCK * -28.0f, 0.0f },
                { 0.0f, ONE_BLOCK * -60.0f, 0.0f },
            };
            Float3 afpos = actionStdPos[(m_ActCycleNum - 1) % 3];
            float rot = CalculateRadToDeg(atan2(-afpos.y, -afpos.x)) - 90.0f;
            StartWarp(afpos, rot, 25, true);

            m_BladeWarpNum = 0;
            //m_BladeBeforeSpeedMod = 1.8f;
            //m_BladeBeforeSpeedMod = 2.6f;
            m_BladeBeforeSpeedMod = 2.4f;
            m_BladeWaveNum = 7;
            m_BladeWaveAngle = 120.0f;
            m_BladeNoTurnFlag = true;
            m_BladeAfterWarpFlag = true;
            m_BladeSuperFlag = true;

            //if (m_ActCycleNum <= 1)
            //{
            //    m_BladeWarpNum = 3;
            //    m_BladeBeforeSpeedMod = 1.0f;
            //    m_BladeWaveNum = 1;
            //}
            //else
            //{
            //    m_BladeWarpNum = m_ActCycleNum % 2 == 0 ? 0 : 1;
            //    m_BladeBeforeSpeedMod = m_BladeWarpNum <= 0 ? 0.4f : 0.8f;
            //}
        }
        else
        {
            { //布団ブレード
                m_BladeFrame = 0;
                m_CurrentState = ActionState::Blade;

                {
                    Float3 afpos = {
                        0.0f,
                        ONE_BLOCK * -70.0f,
                        0.0f
                    };
                    float rot = 0;
                    StartWarp(afpos, rot, 25, true);
                }

                {
                    Float3 afpos = {
                        ONE_BLOCK * 70.0f,
                        ONE_BLOCK * -40.0f,
                        0.0f
                    };
                    float rot = 0;
                    MirrorgeStateStartSet(0,
                        rot,
                        afpos,
                        {},
                        MkF3(1.0f, 1.0f, 1.0f)
                    );
                }
                {
                    Float3 afpos = {
                        ONE_BLOCK * 70.0f,
                        ONE_BLOCK * 40.0f,
                        0.0f
                    };
                    float rot = 90.0f;
                    MirrorgeStateStartSet(1,
                        rot,
                        afpos,
                        {},
                        MkF3(1.0f, 1.0f, 1.0f)
                    );
                }
                {
                    Float3 afpos = {
                        ONE_BLOCK * -70.0f,
                        ONE_BLOCK * 40.0f,
                        0.0f
                    };
                    float rot = 180.0f;
                    MirrorgeStateStartSet(2,
                        rot,
                        afpos,
                        {},
                        MkF3(1.0f, 1.0f, 1.0f)
                    );
                }
                {
                    Float3 afpos = {
                        ONE_BLOCK * -70.0f,
                        ONE_BLOCK * -40.0f,
                        0.0f
                    };
                    float rot = 270.0f;
                    MirrorgeStateStartSet(3,
                        rot,
                        afpos,
                        {},
                        MkF3(1.0f, 1.0f, 1.0f)
                    );
                }

                //int bodynum = 4;
                //for (int i = 0; i < bodynum; i++)
                //{
                //    int index = i + 4;
                //    Float3 afpos = {
                //        sin(2.0f * SYS_GENERAL_PI / bodynum * i) * ONE_BLOCK * 26.0f,
                //        cos(2.0f * SYS_GENERAL_PI / bodynum * i) * ONE_BLOCK * 26.0f,
                //        0.0f
                //    };
                //    MirrorgeStateStartSet(index,
                //        -360.0f / bodynum * i - 0.0f,
                //        afpos,
                //        {},
                //        MkF3(1.0f, 1.0f, 1.0f)
                //    );
                //}

                m_BladeWarpNum = 0;
                m_BladeBeforeSpeedMod = 2.4f;
                //m_BladeBeforeSpeedMod = 1.2f;
                m_BladeWaveNum = 3;
                m_BladeWaveAngle = 25.0f;
                m_BladeNoTurnFlag = true;
                m_BladeAfterWarpFlag = true;
                m_BladeSuperFlag = true;
                m_BladeSuperMirrogeFlag = true;
            }
        }
    }
    else if (actF == 23)
    {
        if (m_CallEnemies.size() <= 4 && m_ActCycleNum <= 3)
        { //円まーじぇんしーコール
            m_EnemyCallFrame = 0;
            m_CurrentState = ActionState::EnemyCall;
        }
        else
        {
            m_ActionCooldown = 40;

            Float3 afpos = m_ActionStdPos[(m_ActCycleNum + 1) % 3];
            if (CalculateDistance(afpos, m_Obj->pos) > ONE_BLOCK * 8)
            {
                float rot = CalculateRadToDeg(atan2(-afpos.y, -afpos.x)) - 90.0f;
                StartWarp(afpos, rot, 25);
            }
        }
    }
    else if (actF == 24)
    {
        { //布団ブレード
            m_BladeFrame = 0;
            m_CurrentState = ActionState::Blade;

            {
                m_BladeWarpHastenFrame = ATTACK_BLADE_BEFORE_FIXED_FRAME;
                m_BladeMirrogeFlag = true;

                m_BladeWarpNum = 1;
                m_BladeBeforeSpeedMod = m_ActCycleNum <= 2 ? 1.4f : 1.2f;
                m_BladeWaveNum = m_ActCycleNum <= 2 ? 1 : 2;
                m_BladeWaveAngle = 140.0f;
                m_BladeNoTurnFlag = false;
                m_BladeAfterWarpFlag = false;
                m_BladeSuperFlag = false;
            }
        }
    }

    m_ActCycle++;
}

void GOE_Character_Enemy_Boss_Inperiusu::update_waitevent()
{
    bool isEnd = false;
    int wazacooldown = 0;

    if (m_WaitEventFrame >= m_WaitEventFrameMax)
    {
        isEnd = true;
    }

    m_WaitEventFrame++;

    if (isEnd)
    {
        m_ActionCooldown = wazacooldown;
        m_WaitEventFrame = -1;
        m_CurrentState = ActionState::Idle;
    }
}

bool GOE_Character_Enemy_Boss_Inperiusu::update_healevent(const GAME_OBJECT_IDENTIFIER& sourcePlayer)
{
    bool isEnd = false;
    //int wazacooldown = 0;

    constexpr int padF = FRAME_ONE_SECOND * 3.0f;

    if (m_HealEventFrame == 0 + padF)
    {
        GOE_Character_Player* targetSourcePlayer = nullptr;
        {
            GAME_OBJECT* playerObj = GetGameObjectFindByIdentifier(sourcePlayer);
            if (playerObj != nullptr)
            {
                targetSourcePlayer =
                    dynamic_cast<GOE_Character_Player*>(playerObj->events.source);  // ダウンキャスト
            }
        }

        if (targetSourcePlayer != nullptr
            //&& !targetSourcePlayer->IsHealIgnore()
            //&& targetSourcePlayer->GetGameObj()->status.hpMax > targetSourcePlayer->GetGameObj()->status.hp
        )
        {
            //int delay = FRAME_ONE_SECOND * 1.0f;
            int delay = 0;

            //std::string gayaname;
            //if (targetSourcePlayer->IsPinch())
            //{
            //    gayaname = "game_give_item_heal_onbosspinch_mayu";
            //}
            //else
            //{
            //    gayaname = "game_give_item_heal_onbossnormal_mayu";
            //}
            std::string gayaname = "iseki_inperiusuhard_phase2before_giveitemheal_mayu";

            std::string gid = GE_GayaStartRequest(
                gayaname,
                delay
            );

            GOE_Item* item = GOE_Item::CreateItem(
                GOE_Item::Type::Heal,
                {
                    96.0f * -200.0f,
                    0.0f,
                    96.0f * -200.0f,
                }
            );
            item->SetGayaID(gid);

            GOE_ItemTransporter* tsptr = GOE_ItemTransporter::Create(
                {
                    96.0f * -200.0f,
                    0.0f,
                    96.0f * -14.0f,
                },
                item->GetGameObj(),
                targetSourcePlayer->GetGameObj()
            );
            tsptr->StartTransport(delay);

            m_HealEventItemObj = GetIdentifierGameObject(item->GetGameObj());
        }
        else
        { 
            //条件に当てはまらなかったらスキップ
            isEnd = true;

            //wazacooldown = 90;
        }
    }
    //if (m_HealEventFrame == 10)
    //{
    //    StartWarp(
    //        MkF3(
    //            0.0f,
    //            ONE_BLOCK * -40.0f,
    //            0.0f
    //        ),
    //        0.0f,
    //        25,
    //        true
    //    );
    //}

    int checkstartf = FRAME_ONE_SECOND * 2.0f + padF;
    if (m_HealEventFrame >= checkstartf)
    {
        if (m_HealEventFrame == checkstartf)
        {
            m_HealEventAttackFlag = false;
        }

        GAME_OBJECT* itemObj = GetGameObjectFindByIdentifier(m_HealEventItemObj);
        
        //アイテムがなくなったら動き出す
        if(!IsActiveGameObject(itemObj))
        {
            isEnd = true;
            //wazacooldown = FRAME_ONE_SECOND * 0.5f;
        }

        //攻撃されたら動き出す
        if (m_HealEventAttackFlag)
        {
            isEnd = true;
            //wazacooldown = FRAME_ONE_SECOND * 0.1f;

            DeleteGameObject(GetGameObjectFindByIdentifier(m_HealEventItemObj));
        }
    }

    m_HealEventFrame++;

    if (isEnd)
    {
        m_HealEventItemObj = {};
        //m_ActionCooldown = wazacooldown;
        m_HealEventFrame = -1;
        //m_CurrentState = ActionState::Idle;
    }

    return isEnd;
}

void GOE_Character_Enemy_Boss_Inperiusu::event_playerPosSet()
{
    int apnum = GetActivePlayerNum();
    for (int i = 0; i < PLAYER_MAX; i++)
    {
        PLAYER* pl = GetPlayer(i);
        if (pl != nullptr)
        {
            GAME_OBJECT* plObj = pl->obj;
            if (plObj != nullptr)
            {
                plObj->pos.x = (1.0f * i - 0.5f * (apnum - 1)) * ONE_BLOCK * 3.0f;
                plObj->pos.y = ONE_BLOCK * 8.0f;
                plObj->pos.z = 0;

                GameObjectMoveRequest(plObj, 4,
                    MkF3(
                        plObj->pos.x,
                        plObj->pos.y - 1.0f, 
                        plObj->pos.z
                    )
                );

                //Float3 postmp = {};
                //postmp.x = (1.0f * i - 0.5f * (apnum - 1)) * ONE_BLOCK * 3.0f;
                //postmp.y = ONE_BLOCK * 8.0f;
                //postmp.z = 0;
                //GameObjectMoveRequest(plObj, FRAME_ONE_SECOND * 2.0f,
                //    MkF3(
                //        postmp.x,
                //        postmp.y - ONE_BLOCK,
                //        postmp.z
                //    ),
                //    EasingInOutSine,
                //    EasingInOutSine,
                //    EasingInOutSine,
                //    true
                //);
            }
        }
    }
}

void GOE_Character_Enemy_Boss_Inperiusu::become_start()
{
    if (m_IsHard)
    {
        SetPostEffectShaderVariable((float)GetMainGameFrame(), 2);
        SetPostEffectShaderVariable(1.0f, 3);
    }
    else
    {
        SetPostEffectShaderVariable((float)GetMainGameFrame(), 0);
        SetPostEffectShaderVariable(1.0f, 1);
    }

    //if (m_StartMotionBeforeShader != -1)
    //{
    //    SetPostEffectShader(m_StartMotionBeforeShader);
    //}
    if (m_StartMotionBeforeShaderVS != -1)
    {
        SetPostEffectShaderVS(m_StartMotionBeforeShaderVS);
    }
    if (m_StartMotionBeforeShaderPS != -1)
    {
        SetPostEffectShaderPS(m_StartMotionBeforeShaderPS);
    }

    ResetPostEffectGlobalOutlineWidth();

    m_BodyTexMaskInitFlag = true;

    m_StartMotionShaderFlag = false;

    m_BodyTexMaskFrame_Init = -2;
    m_BodyTexColorFrame_Init = -2;

    //判定有効化
    m_Obj->collision.atk.enable = true;
    m_Obj->collision.rec.enable = true;
    m_Obj->status.statusType = GAME_OBJECT_STATUS_TYPE_ENEMY_BOSS;
    //m_Obj->graphic.shaderOpt.outline = DPD_SHADER_OPT_OUTLINE_PURPLE;

    m_Obj->graphic.shaderOpt.outline = DPD_SHADER_OPT_OUTLINE_NORMAL;
    m_Obj->graphic.shaderOpt.outlineLayer = 1;

    //m_Obj->status.guardType = GAME_OBJECT_GUARD_TYPE_SHORT;
    m_Obj->status.guardType = GAME_OBJECT_GUARD_TYPE_NORMAL;

    m_Obj->pos.z = 0.0f;


    //ボスのHPを表示する
    if (m_IsHard)
    {
        ShowBossbar_Game(
            GetGameObjectIndex(m_Obj),
            true,
            m_Obj->status.fullName,
            BOSSBAR_TYPE_GAME::Inperius_H,
            HARD_HP_BORDER, 1.0f
        );

    }
    else
    {
        ShowBossbar_Game(
            GetGameObjectIndex(m_Obj),
            true,
            m_Obj->status.fullName,
            BOSSBAR_TYPE_GAME::Inperius
        );

    }

    {
        int enemNum = zako_powerup();
        //int enemLegaNum = 0;

        if (enemNum > 0)
        {
            GE_GayaStartRequest(
                "iseki_inperiusu_zako_powerup_mayu",
                FRAME_ONE_SECOND * 5.0f
            );
        }
        //if (enemLegaNum > 0)
        //{
        //    GE_GayaStartRequest(
        //        "iseki_legatos_explosion",
        //        FRAME_ONE_SECOND * 8.0f
        //    );
        //}
    }
}

void GOE_Character_Enemy_Boss_Inperiusu::update_start()
{
    SOUND_ID alertse = GetSoundData("SOUND/SE/Test_SE_Alart");
    
    SOUND_ID bgminidtmp = m_IsHard ? m_bgmInH : m_bgmIn;
    float bgminLen = m_IsHard ? FRAME_ONE_SECOND * 5.95 : FRAME_ONE_SECOND * 5.55;
    SOUND_ID bgmidtmp = m_IsHard ? m_bgmLoopH : m_bgmLoop;
    //float bgmLen = m_IsHard ? -1 : 39.0f;
    float bgmLen = m_IsHard ? -1 : 43.71f;
    //float bgmLen = -1;

    if (m_RetryCount > 0)
    {
        if (m_StartFrame == 0)
        {
            //m_Obj->pos.z = 0;

            //m_BodyTexMaskFrame_Init = -2;
            //m_BodyTexColorFrame_Init = -2;

            //m_Obj->graphic.shaderOpt.outline = DPD_SHADER_OPT_OUTLINE_NORMAL;
            //m_Obj->graphic.shaderOpt.outlineLayer = 1;
            GE_GayaCancelRequestAll();
            SoundBGMStopRequest();
        }
        if (m_StartFrame == 3)
        {
            SetCameraModeAll(CAMERA_MODE_BOSS_CHASE);
        }

        if (m_IsHard)
        {
            if (m_StartFrame == 0)
            {
                m_BodyTexMaskInitFlag = true;

                m_BodyTexMaskFrame_Init = -2;
                m_BodyTexColorFrame_Init = -2;

                m_Obj->graphic.shaderOpt.outline = DPD_SHADER_OPT_OUTLINE_NORMAL;
                m_Obj->graphic.shaderOpt.outlineLayer = 1;

                for (int i = 0; i < MODEL_NUM; i++)
                {
                    m_ModelTransPos[i].x = 0.0f;
                    m_ModelTransPos[i].y = 0.0f;
                    m_ModelTransPos[i].z = 0.0f;

                    m_ModelTransRot[i].x = 0.0f;
                    m_ModelTransRot[i].y = 0.0f;
                    m_ModelTransRot[i].z = 0.0f;

                    m_ModelTransSca[i].x = 1.0f;
                    m_ModelTransSca[i].y = 1.0f;
                    m_ModelTransSca[i].z = 1.0f;
                }

            }
            //if (m_StartFrame == FRAME_ONE_SECOND * 0.2 + 135)
            if (m_StartFrame == FRAME_ONE_SECOND * 0.3)
            {
                StartWarp(
                    MkF3(
                        m_Obj->pos.x,
                        m_Obj->pos.y,
                        0.0f
                    ),
                    0.0f,
                    25
                );

                m_StartFrame += FRAME_ONE_SECOND * 0.9;
            }
        }
        else
        {
            //if (m_StartFrame == FRAME_ONE_SECOND * 0.2)
            //{
            //    StartJet(
            //        MkF3(
            //            m_Obj->pos.x,
            //            m_Obj->pos.y,
            //            0.0f
            //        ),
            //        0.0f,
            //        160//240
            //    );
            //}
            if (m_StartFrame == 10)
            {
                //SEを再生
                SoundEffectPlayRequest(GetSoundData("SOUND/SE/inperiusu/b_kamae"), 1.3f, 0);
                //SEを再生
                SoundEffectPlayRequest(GetSoundData("SOUND/SE/enemy/move/Inperius_Jet_End"), 
                    1.6f, 0);
                StartAnim(AnimState::Stating_All_Purge, 12, true);
                m_BodyTexMaskFrame_Init = 0;
            }
            if (m_StartFrame == 30)
            {
                //SEを再生
                SoundEffectPlayRequest(GetSoundData("SOUND/SE/inperiusu/b_charge"), 1.4f, 0);
                //SEを再生
                SoundEffectPlayRequest(GetSoundData("SOUND/SE/enemy/move/Inperius_Jet_Start"), 2.0f, 1);

                StartAnim(AnimState::Stating_All_Up, 60, true);

                GameObjectMoveRequest(m_Obj,
                    60,
                    MkF3(m_Obj->pos.x, m_Obj->pos.y, 0.0f),
                    EasingInOutSine,
                    EasingInOutSine,
                    EasingInOutSine,
                    true
                );

                m_BodyTexColorFrame_Init = 0;
            }

            if (m_StartFrame == 90)
            {
                StartAnim(AnimState::Stating_Head_Up, 20, true);
            }

            if (m_StartFrame == 110)
            {
                m_BodyTexMaskInitFlag = true;

                m_Obj->graphic.shaderOpt.outline = DPD_SHADER_OPT_OUTLINE_NORMAL;
                m_Obj->graphic.shaderOpt.outlineLayer = 1;

                //SEを再生
                SoundEffectPlayRequest(GetSoundData("SOUND/SE/inperiusu/b_attack"),
                    1.0f, 0, 0.7f);
                //SEを再生
                //SoundEffectPlayRequest(GetSoundData("SOUND/SE/enemy/move/warpHigh"), 1.8f, 0);

                CameraShakeRequestAll(CAMERA_SHAKE_MIDIUM_FRAME, CAMERA_SHAKE_EXLARGE_POWER);
            }

        }

        if (m_StartFrame == FRAME_ONE_SECOND * 0.2)
        {
            //SEを再生
            SoundBGMIntroPlayRequest(bgminidtmp, 2.0f, 0);

            ////BGMを再生する
            //SoundBGMPlayRequest(m_bgmLoop,
            //    FRAME_ONE_MINUTE + (FRAME_ONE_SECOND * 13),
            //    2.0f,
            //    FRAME_ONE_SECOND * 6.0
            //);

            //BGMを再生する
            SoundBGMPlayRequest(bgmidtmp,
                true,
                2.0f,
                //FRAME_ONE_SECOND * 6.0,
                bgminLen,

                0.0f,
                bgmLen
            );
        }

        constexpr int startPosef = FRAME_ONE_SECOND * 3.2f;
        if (m_StartFrame == startPosef - ATTACK_BLADE_BEFORE_FRAME_STARTPOSE)
        {
            //SEを再生
            SoundEffectPlayRequest(GetSoundData("SOUND/SE/inperiusu/b_kamae"), 1.3f, 0);
            //SEを再生
            //SoundEffectPlayRequest(GetSoundData("SOUND/SE/inperiusu/b_charge"), 1.5f, 5);


            set_blade_visible(true);
            StartAnim(AnimState::BladeBefore, ATTACK_BLADE_BEFORE_FRAME_STARTPOSE, true);

        }
        if (m_StartFrame == startPosef)
        {
            //アウトライン有効化

            //m_Obj->graphic.shaderOpt.outline = true;
            //m_Obj->graphic.shaderOpt.outline = DPD_SHADER_OPT_OUTLINE_PURPLE;

            //SEを再生
            //SoundEffectPlayRequest(GetSoundData("SOUND/SE/inperiusu/b_attackfull"), 1.6f, 0);

            SoundEffectPlayRequest(GetSoundData("SOUND/SE/enemy/attack/Small_Explosion"),
                2.0f, 0, 0.8f);
            //SoundEffectPlayRequest(GetSoundData("SOUND/SE/enemy/attack/Explosion"),
            //    1.0f, 0, 0.6f);
            SoundEffectPlayRequest(GetSoundData("SOUND/SE/inperiusu/b_attack"),
                1.5f, 0);
            SoundEffectPlayRequest(
                GetSoundData("SOUND/SE/enemy/attack/super_beam_miminari"),
                0.45f, 3, 0.75f);

            CameraShakeRequestAll(CAMERA_SHAKE_EXLARGE_FRAME, CAMERA_SHAKE_EXLARGE_POWER);

            StartAnim(AnimState::Blade, ATTACK_BLADE_ATTACK_FRAME, true);

        }

        constexpr int stopstartf = startPosef + ATTACK_BLADE_ATTACKING_FRAME_STARTPOSE;
        if (m_StartFrame == stopstartf)
        {
            SetControllerMainLeftVibration(30, 0);
            SetControllerMainRightVibration(30, 0);

            //GameStopRequest(stopframe);
            //GameObjectHitStop(m_Obj, stopframe);
            //GameObjectHitStop(m_Obj, (int)(FRAME_ONE_SECOND * 0.5));

        }

        if (m_StartFrame == stopstartf + FRAME_ONE_SECOND * 0.5f)
        {
            set_blade_visible(false);
        }

        //開始フレーム
        constexpr int startingF = stopstartf + FRAME_ONE_SECOND / 2;

        if (m_StartFrame == startingF + FRAME_ONE_SECOND / 2)
        {
            m_StartedFlag = true;
        }

        //開始
        if (m_StartFrame == startingF)
        {
            //m_StartMotionBeforeShader = -1;
            m_StartMotionBeforeShaderVS = -1;
            m_StartMotionBeforeShaderPS = -1;
            become_start();
        }
    }
    else
    {
        if (m_StartFrame == 0)
        {
            GE_GayaCancelRequestAll();

            //m_StartMotionBeforeShader = -1;
            m_StartMotionBeforeShaderVS = -1;
            m_StartMotionBeforeShaderPS = -1;
            //カウントをストップ
            //SetStageCountStopSceneGame(true);

            //プレイヤーの硬直
            //GameManagerSetPlayerCtrlStop(true);

            //ムービーフラグ
            GameManagerSetMovieFlag(true, true);

            SetCameraModeAll(CAMERA_MODE_NORMAL);

            SoundBGMStopRequest();
            SoundBGMIntroPlayRequest(alertse, 2.4f, 0);
        }

        if (m_StartFrame == FRAME_ONE_SECOND * 1)
        {
            CameraMoveRequestAll(FRAME_ONE_SECOND * 1,
                Float2ToFloat3(Float3ToFloat2(m_Obj->pos), 0.0f),
                0.0f,
                MkF2(1.0f, 1.0f),
                0.35f,
                EasingInOutSine,
                CAMERA_MOVE_TYPE_SMOOTH
            );
            CameraTargetMoveRequestAll(FRAME_ONE_SECOND * 1,
                Float2ToFloat3(Float3ToFloat2(m_Obj->pos), 0.0f),
                EasingInOutSine,
                CAMERA_MOVE_TYPE_SMOOTH
            );
        }

        if (
            m_StartFrame >= FRAME_ONE_SECOND / 2
            && m_StartFrame < FRAME_ONE_SECOND * 3.85
            )
        {
            if (m_StartFrame % 15 == 0)
            {
                CameraShakeRequestAll(CAMERA_SHAKE_MIDIUM_FRAME, CAMERA_SHAKE_EXLARGE_POWER);
            }
        }
        //タイミング調整
        if (m_StartFrame == FRAME_ONE_SECOND * 3.85)
        {
            m_StartFrame += FRAME_ONE_SECOND * 0.15f;
        }


        //if (m_StartFrame == FRAME_ONE_SECOND * 4)
        //{
        //}
        if (m_StartFrame == FRAME_ONE_SECOND * 4.2)
        {
            GameManagerSetMoviePlayerHiddenFlag(true);

            event_playerPosSet();

            //SEを再生
            SoundBGMIntroPlayRequest(bgminidtmp, 2.0f, 0);

            ////BGMを再生する
            //SoundBGMPlayRequest(m_bgmLoop,
            //    FRAME_ONE_MINUTE + (FRAME_ONE_SECOND * 13),
            //    2.0f,
            //    FRAME_ONE_SECOND * 6.0
            //);
        
            //BGMを再生する
            SoundBGMPlayRequest(bgmidtmp,
                true,
                2.0f,
                //FRAME_ONE_SECOND * 6.0,
                bgminLen,

                0.0f,
                bgmLen
            );

        }

        if (m_IsHard)
        {
            if (m_StartFrame == FRAME_ONE_SECOND * 4)
            {
                m_Obj->graphic.shaderOpt.outline = DPD_SHADER_OPT_OUTLINE_NORMAL;
                m_Obj->graphic.shaderOpt.outlineLayer = 1;

                m_BodyTexMaskFrame_Init = -2;
                m_BodyTexColorFrame_Init = -2;

                Float3 zoomPos = m_Obj->pos;
                zoomPos.z = 10000.0f;
                //zoomPos.x += 350.0f;
                //zoomPos.x += 0.0f;
                //zoomPos.z += 8000.0f;
                //zoomPos.y += -1000.0f;
                //zoomPos.z += -1000.0f;
                //zoomPos.z += -3000.0f;
                zoomPos.z += 500.0f;

                //SetCameraTargetPosAll(MkF3(zoomPos.x, zoomPos.y + 20000.0f, zoomPos.z + -7000.0f));
                //SetCameraTargetPosAll(MkF3(zoomPos.x, zoomPos.y + 5000.0f, zoomPos.z + 3000.0f));
                SetCameraTargetPosAll(MkF3(zoomPos.x, zoomPos.y + 5000.0f, zoomPos.z + -13000.0f));
                SetCameraPosAll(zoomPos);
                //SetCameraZoomAll(0.6f);
                //SetCameraZoomAll(1.5f);
                //SetCameraZoomAll(0.75f);
                SetCameraZoomAll(0.6f);
                CameraFlashRequestAll(16, EasingOutSine, MkF4(-1.0f, -1.0f, -1.0f, 1.0f));

                Float3 zoomPos2 = zoomPos;
                //zoomPos2.z += -7000.0f;
                zoomPos2.z += -6500.0f;
                Float3 zoomPos2Ta = zoomPos;
                zoomPos2Ta.y += 35000.0f;
                //zoomPos2Ta.z += -5500.0f;
                zoomPos2Ta.z += -9500.0f;

                CameraMoveRequestAll(FRAME_ONE_SECOND * 5.6f,
                    zoomPos2,
                    0.0f,
                    MkF2(1.0f, 1.0f),
                    //0.65f,
                    //0.75f,
                    0.6f,
                    EasingInOutSine,
                    CAMERA_MOVE_TYPE_SMOOTH
                );
                CameraTargetMoveRequestAll(FRAME_ONE_SECOND * 5.6f,
                    zoomPos2Ta,
                    EasingInOutSine,
                    CAMERA_MOVE_TYPE_SMOOTH
                );

                for (int i = 0; i < MODEL_NUM; i++)
                {
                    m_ModelTransPos[i].x = 0.0f;
                    m_ModelTransPos[i].y = 0.0f;
                    m_ModelTransPos[i].z = 0.0f;

                    m_ModelTransRot[i].x = 0.0f;
                    m_ModelTransRot[i].y = 0.0f;
                    m_ModelTransRot[i].z = 0.0f;

                    m_ModelTransSca[i].x = 1.0f;
                    m_ModelTransSca[i].y = 1.0f;
                    m_ModelTransSca[i].z = 1.0f;
                }

                //m_StartFrame = FRAME_ONE_SECOND * 9;
            }

            if (m_StartFrame == FRAME_ONE_SECOND * 5.0)
            {
                StartWarp(
                    MkF3(
                        m_Obj->pos.x,
                        m_Obj->pos.y,
                        0.0f
                    ),
                    0.0f,
                    30
                );

            }

            if (m_StartFrame == FRAME_ONE_SECOND * 6 + 85)
            {
                Float3 zoomPos2 = m_Obj->pos;
                zoomPos2.z += -9000.0f;
                Float3 zoomPos2Ta = m_Obj->pos;
                zoomPos2Ta.y += 35000.0f;
                zoomPos2Ta.z += -19000.0f;

                CameraMoveRequestAll(FRAME_ONE_SECOND * 0.3f,
                    zoomPos2,
                    0.0f,
                    MkF2(1.0f, 1.0f),
                    0.9f,
                    EasingInOutSine,
                    CAMERA_MOVE_TYPE_SMOOTH
                );
                CameraTargetMoveRequestAll(FRAME_ONE_SECOND * 0.3f,
                    zoomPos2Ta,
                    EasingInOutSine,
                    CAMERA_MOVE_TYPE_SMOOTH
                );
            }

            if (m_StartFrame == FRAME_ONE_SECOND * 6 + 110)
            {
                //SEを再生
                //SoundEffectPlayRequest(GetSoundData("SOUND/SE/inperiusu/b_attack"), 1.0f, 0);
                //SEを再生
                SoundEffectPlayRequest(GetSoundData("SOUND/SE/enemy/move/warpHigh"), 1.3f, 0);

                CameraShakeRequestAll(CAMERA_SHAKE_MIDIUM_FRAME, CAMERA_SHAKE_EXLARGE_POWER);

                //CameraFlashRequestAll(30,
                //    EasingInOutSine,
                //    MkF4(0.0f, 0.0f, 0.0f, 0.35f),
                //    DPD_SHADER_OPT_BLEND_STATE_DEFAULT
                //);

                //m_StartEyeEffFrameMax = 25;
                m_StartEyeEffFrameMax = 45;
                m_StartEyeEffFrame = m_StartEyeEffFrameMax;

                m_BodyTexMaskInitFlag = true;

                m_StartFrame += FRAME_ONE_SECOND * 3;
            }
        }
        else
        {
            if (m_StartFrame == FRAME_ONE_SECOND * 4 + 1)
            {
                Float3 zoomPos = m_Obj->pos;
                zoomPos.x += -800.0f;
                zoomPos.z += 4200.0f;

                SetCameraTargetPosAll(MkF3(zoomPos.x, zoomPos.y + 20000.0f, zoomPos.z + -7000.0f));
                SetCameraPosAll(zoomPos);
                SetCameraZoomAll(0.9f);
                CameraFlashRequestAll(16, EasingOutSine, MkF4(-1.0f, -1.0f, -1.0f, 1.0f));

                Float3 zoomPos2 = zoomPos;
                zoomPos2.x += 350.0f;
                zoomPos2.z += -1200.0f;
                Float3 zoomPos2Ta = zoomPos;
                zoomPos2Ta.y += 35000.0f;
                zoomPos2Ta.z += -6500.0f;

                CameraMoveRequestAll(FRAME_ONE_SECOND * 2.0f,
                    zoomPos2,
                    0.0f,
                    MkF2(1.0f, 1.0f),
                    0.8f,
                    EasingInOutSine,
                    CAMERA_MOVE_TYPE_SMOOTH
                );
                CameraTargetMoveRequestAll(FRAME_ONE_SECOND * 2.0f,
                    zoomPos2Ta,
                    EasingInOutSine,
                    CAMERA_MOVE_TYPE_SMOOTH
                );
            }
            if (m_StartFrame == FRAME_ONE_SECOND * 4 + 1 + 20)
            {
                //SEを再生
                SoundEffectPlayRequest(GetSoundData("SOUND/SE/inperiusu/b_kamae"), 1.3f, 0);
                //SEを再生
                SoundEffectPlayRequest(GetSoundData("SOUND/SE/enemy/move/Inperius_Jet_End"), 1.4f, 0);
                StartAnim(AnimState::Stating_ArmR_Purge, 12, true);

                Float3 efPos = m_Obj->pos;
                efPos.x += -500.0f;
                efPos.z += -740.0f;

                for (int i = 0; i < 5; i++)
                {
                    Float3 efPos2 = efPos;
                    efPos2.y += sin(2.0f * SYS_GENERAL_PI / 5 * i) * 200.0f;
                    efPos2.z += cos(2.0f * SYS_GENERAL_PI / 5 * i) * 200.0f;

                    GAME_OBJECT* go = GO_CreateEffect(GO_EFFECT_TYPE_SMOKE, efPos2);
                    if (go != nullptr)
                    {
                        go->graphic.rot3d.x = 90.0f;
                        go->graphic.color = MkF4(
                            0.3f,
                            0.3f,
                            0.3f,
                            1.0f
                        );
                        go->scale = MkF3(
                            2.0f,
                            2.0f,
                            2.0f
                        );
                        GameObjectSetLifeTime(go, 30);
                    }
                }
            }
            if (m_StartFrame == FRAME_ONE_SECOND * 4 + 1 + 32)
            {
                CameraShakeRequestAll(CAMERA_SHAKE_MIDIUM_FRAME, CAMERA_SHAKE_EXLARGE_POWER);
            }
            if (m_StartFrame == FRAME_ONE_SECOND * 4 + 1 + 40)
            {
                //SEを再生
                SoundEffectPlayRequest(GetSoundData("SOUND/SE/inperiusu/b_charge"), 1.2f, 0);
                StartAnim(AnimState::Stating_ArmR_Up, 60, true);

                m_BodyTexColorFrame_Init = 0;
            }
            if (m_StartFrame == FRAME_ONE_SECOND * 4 + 1 + 100)
            {
                //SEを再生
                SoundEffectPlayRequest(GetSoundData("SOUND/SE/inperiusu/b_attackfull"), 0.4f, 0);
                SoundEffectPlayRequest(GetSoundData("SOUND/SE/inperiusu/b_attack"), 0.6f, 0);
                CameraShakeRequestAll(
                    CAMERA_SHAKE_LARGE_FRAME * 1.4f,
                    CAMERA_SHAKE_EXLARGE_POWER
                );

                CameraFlashRequestAll(16,
                    EasingOutSine,
                    MkF4(0.5f, 0.0f, 0.8f, 0.6f),
                    DPD_SHADER_OPT_BLEND_STATE_ADDCOLOR
                );
                {
                    Float3 efPos = m_Obj->pos;
                    efPos.x += -500.0f;
                    efPos.z += -1000.0f;

                    GAME_OBJECT* go = GO_CreateEffect(GO_EFFECT_TYPE_SHOCKWAVE_ENEM, efPos);
                    if (go != nullptr)
                    {
                        go->graphic.rot3d.y = 90.0f;
                        go->graphic.rot3d.z = -45.0f;
                        go->graphic.color.w = 1.0f;
                        go->scale = MkF3(
                            18.0f,
                            18.0f,
                            18.0f
                        );
                        //GameObjectSetLifeTime(go, 12);
                        GameObjectSetLifeTime(go, 16);
                    }

                    for (int i = 0; i < 5; i++)
                    {
                        Float3 efPos2 = efPos;
                        efPos2.y += sin(2.0f * SYS_GENERAL_PI / 5 * i) * 200.0f;
                        efPos2.z += cos(2.0f * SYS_GENERAL_PI / 5 * i) * 200.0f;

                        GAME_OBJECT* go = GO_CreateEffect(GO_EFFECT_TYPE_SPARK, efPos2);
                        if (go != nullptr)
                        {
                            go->graphic.color = MkF4(
                                0.4f,
                                0.0f,
                                0.05f,
                                -0.8f
                            );
                            go->vel.y = sin(2.0f * SYS_GENERAL_PI / 5 * i) * 58.0f;
                            go->vel.z = cos(2.0f * SYS_GENERAL_PI / 5 * i) * 58.0f;
                            go->friction = 0.05f;
                            go->graphic.rot3d.x = 90.0f;
                            go->graphic.rot3d.z = 50.0f * i;
                            go->graphic.color.w = 1.0f;
                            go->scale = MkF3(
                                8.0f,
                                8.0f,
                                8.0f
                            );
                            GameObjectSetLifeTime(go, 30);
                        }
                    }
                }
            }

            if (m_StartFrame == FRAME_ONE_SECOND * 6.5)
            {
                Float3 zoomPos = m_Obj->pos;
                zoomPos.x += 800.0f;
                zoomPos.z += 4200.0f;

                SetCameraTargetPosAll(MkF3(zoomPos.x, zoomPos.y + 20000.0f, zoomPos.z + -7000.0f));
                SetCameraPosAll(zoomPos);
                SetCameraZoomAll(0.9f);
                CameraFlashRequestAll(16, EasingOutSine, MkF4(-1.0f, -1.0f, -1.0f, 1.0f));

                Float3 zoomPos2 = zoomPos;
                zoomPos2.x += -350.0f;
                zoomPos2.z += -1200.0f;
                Float3 zoomPos2Ta = zoomPos;
                zoomPos2Ta.y += 35000.0f;
                zoomPos2Ta.z += -6500.0f;

                CameraMoveRequestAll(FRAME_ONE_SECOND * 2.0f,
                    zoomPos2,
                    0.0f,
                    MkF2(1.0f, 1.0f),
                    0.8f,
                    EasingInOutSine,
                    CAMERA_MOVE_TYPE_SMOOTH
                );
                CameraTargetMoveRequestAll(FRAME_ONE_SECOND * 2.0f,
                    zoomPos2Ta,
                    EasingInOutSine,
                    CAMERA_MOVE_TYPE_SMOOTH
                );
            }
            if (m_StartFrame == FRAME_ONE_SECOND * 6.5 + 20)
            {
                //SEを再生
                SoundEffectPlayRequest(GetSoundData("SOUND/SE/inperiusu/b_kamae"), 1.3f, 0);
                //SEを再生
                SoundEffectPlayRequest(GetSoundData("SOUND/SE/enemy/move/Inperius_Jet_End"), 1.4f, 0);
                StartAnim(AnimState::Stating_ArmL_Purge, 12, true);

                Float3 efPos = m_Obj->pos;
                efPos.x += 500.0f;
                efPos.z += -740.0f;

                for (int i = 0; i < 5; i++)
                {
                    Float3 efPos2 = efPos;
                    efPos2.y += sin(2.0f * SYS_GENERAL_PI / 5 * i) * 200.0f;
                    efPos2.z += cos(2.0f * SYS_GENERAL_PI / 5 * i) * 200.0f;

                    GAME_OBJECT* go = GO_CreateEffect(GO_EFFECT_TYPE_SMOKE, efPos2);
                    if (go != nullptr)
                    {
                        go->graphic.rot3d.x = 90.0f;
                        go->graphic.color = MkF4(
                            0.3f,
                            0.3f,
                            0.3f,
                            1.0f
                        );
                        go->scale = MkF3(
                            2.0f,
                            2.0f,
                            2.0f
                        );
                        GameObjectSetLifeTime(go, 30);
                    }
                }
            }
            if (m_StartFrame == FRAME_ONE_SECOND * 6.5 + 32)
            {
                CameraShakeRequestAll(CAMERA_SHAKE_MIDIUM_FRAME, CAMERA_SHAKE_EXLARGE_POWER);
            }
            if (m_StartFrame == FRAME_ONE_SECOND * 6.5 + 40)
            {
                //SEを再生
                SoundEffectPlayRequest(GetSoundData("SOUND/SE/inperiusu/b_charge"), 1.3f, 0);
                StartAnim(AnimState::Stating_ArmL_Up, 60, true);
            }
            if (m_StartFrame == FRAME_ONE_SECOND * 6.5 + 100)
            {
                //SEを再生
                SoundEffectPlayRequest(GetSoundData("SOUND/SE/inperiusu/b_attackfull"), 0.6f, 0);
                SoundEffectPlayRequest(GetSoundData("SOUND/SE/inperiusu/b_attack"), 0.8f, 0);
                CameraShakeRequestAll(
                    CAMERA_SHAKE_LARGE_FRAME * 1.4f,
                    CAMERA_SHAKE_EXLARGE_POWER
                );

                CameraFlashRequestAll(16,
                    EasingOutSine,
                    MkF4(0.5f, 0.0f, 0.8f, 0.6f),
                    DPD_SHADER_OPT_BLEND_STATE_ADDCOLOR
                );
                {
                    Float3 efPos = m_Obj->pos;
                    efPos.x += 500.0f;
                    efPos.z += -1000.0f;

                    GAME_OBJECT* go = GO_CreateEffect(GO_EFFECT_TYPE_SHOCKWAVE_ENEM, efPos);
                    if (go != nullptr)
                    {
                        go->graphic.rot3d.y = 90.0f;
                        go->graphic.rot3d.z = -45.0f;
                        go->graphic.color.w = 1.0f;
                        go->scale = MkF3(
                            18.0f,
                            18.0f,
                            18.0f
                        );
                        //GameObjectSetLifeTime(go, 12);
                        GameObjectSetLifeTime(go, 16);
                    }

                    for (int i = 0; i < 5; i++)
                    {
                        Float3 efPos2 = efPos;
                        efPos2.y += sin(2.0f * SYS_GENERAL_PI / 5 * i) * 200.0f;
                        efPos2.z += cos(2.0f * SYS_GENERAL_PI / 5 * i) * 200.0f;

                        GAME_OBJECT* go = GO_CreateEffect(GO_EFFECT_TYPE_SPARK, efPos2);
                        if (go != nullptr)
                        {
                            go->graphic.color = MkF4(
                                0.4f,
                                0.0f,
                                0.05f,
                                -0.8f
                            );
                            go->vel.y = sin(2.0f * SYS_GENERAL_PI / 5 * i) * 58.0f;
                            go->vel.z = cos(2.0f * SYS_GENERAL_PI / 5 * i) * 58.0f;
                            go->friction = 0.05f;
                            go->graphic.rot3d.x = 90.0f;
                            go->graphic.rot3d.z = 50.0f * i;
                            go->graphic.color.w = 1.0f;
                            go->scale = MkF3(
                                8.0f,
                                8.0f,
                                8.0f
                            );
                            GameObjectSetLifeTime(go, 30);
                        }
                    }
                }
            }

            if (m_StartFrame == FRAME_ONE_SECOND * 9)
            {
                Float3 zoomPos = m_Obj->pos;
                zoomPos.x += 0.0f;
                //zoomPos.z += 8000.0f;
                //zoomPos.y += -1000.0f;
                zoomPos.z += -1000.0f;

                //SetCameraTargetPosAll(MkF3(zoomPos.x, zoomPos.y + 20000.0f, zoomPos.z + -7000.0f));
                SetCameraTargetPosAll(MkF3(zoomPos.x, zoomPos.y + 5000.0f, zoomPos.z + 3000.0f));
                SetCameraPosAll(zoomPos);
                //SetCameraZoomAll(0.6f);
                SetCameraZoomAll(1.5f);
                CameraFlashRequestAll(16, EasingOutSine, MkF4(-1.0f, -1.0f, -1.0f, 1.0f));

                Float3 zoomPos2 = zoomPos;
                zoomPos2.z += -6000.0f;
                Float3 zoomPos2Ta = zoomPos;
                zoomPos2Ta.y += 35000.0f;
                zoomPos2Ta.z += -5500.0f;

                CameraMoveRequestAll(FRAME_ONE_SECOND * 1.0f,
                    zoomPos2,
                    0.0f,
                    MkF2(1.0f, 1.0f),
                    0.65f,
                    EasingInOutSine,
                    CAMERA_MOVE_TYPE_SMOOTH
                );
                CameraTargetMoveRequestAll(FRAME_ONE_SECOND * 1.0f,
                    zoomPos2Ta,
                    EasingInOutSine,
                    CAMERA_MOVE_TYPE_SMOOTH
                );

                GameObjectMoveRequest(m_Obj,
                    FRAME_ONE_SECOND * 2,
                    MkF3(m_Obj->pos.x, m_Obj->pos.y, 0.0f),
                    EasingInOutSine,
                    EasingInOutSine,
                    EasingInOutSine,
                    true
                );
            }
            if (m_StartFrame == FRAME_ONE_SECOND * 9 + 10)
            {
                //SEを再生
                SoundEffectPlayRequest(GetSoundData("SOUND/SE/inperiusu/b_charge"), 1.4f, 0);
                //SEを再生
                SoundEffectPlayRequest(GetSoundData("SOUND/SE/enemy/move/Inperius_Jet_Start"), 2.6f, 1);
                SoundEffectPlayRequest(GetSoundData("SOUND/SE/enemy/attack/Beam_Charge"), 0.15f, 0);

                StartAnim(AnimState::Stating_Body1_Up, 35, true);
                m_BodyTexMaskFrame_Init = 0;
            }
            //if (m_StartFrame == FRAME_ONE_SECOND * 9 + 10)
            //{
            //    //SEを再生
            //    SoundEffectPlayRequest(GetSoundData("SOUND/SE/enemy/attack/Beam_Charge"), 1.0f, 0);
            //}
            if (m_StartFrame == FRAME_ONE_SECOND * 9 + 65)
            {
                Float3 zoomPos2 = m_Obj->pos;
                zoomPos2.z += -9000.0f;
                Float3 zoomPos2Ta = m_Obj->pos;
                zoomPos2Ta.y += 35000.0f;
                zoomPos2Ta.z += -19000.0f;

                CameraMoveRequestAll(FRAME_ONE_SECOND * 0.3f,
                    zoomPos2,
                    0.0f,
                    MkF2(1.0f, 1.0f),
                    0.9f,
                    EasingInOutSine,
                    CAMERA_MOVE_TYPE_SMOOTH
                );
                CameraTargetMoveRequestAll(FRAME_ONE_SECOND * 0.3f,
                    zoomPos2Ta,
                    EasingInOutSine,
                    CAMERA_MOVE_TYPE_SMOOTH
                );
            }
            if (
                m_StartFrame >= FRAME_ONE_SECOND * 9
                && m_StartFrame < FRAME_ONE_SECOND * 9 + 90
                )
            {
                if (m_StartFrame % 15 == 0)
                {
                    CameraShakeRequestAll(CAMERA_SHAKE_MIDIUM_FRAME, CAMERA_SHAKE_MIDIUM_POWER);
                }
                if (m_StartFrame % 8 == 0)
                {
                    int ftmp = (m_StartFrame - FRAME_ONE_SECOND * 9);
                    for (int i = 0; i < 5; i++)
                    {
                        Float3 efPos2 = m_Obj->pos;
                        efPos2.x += sin(2.0f * SYS_GENERAL_PI / 5 * i + 1.7f * ftmp) * (190.0f + 3.0f * ftmp);
                        efPos2.y += cos(2.0f * SYS_GENERAL_PI / 5 * i + 1.7f * ftmp) * (190.0f + 3.0f * ftmp);
                        efPos2.z += -300.0f + ftmp * -10.0f;

                        GAME_OBJECT* go = GO_CreateEffect(GO_EFFECT_TYPE_SPARK, efPos2);
                        if (go != nullptr)
                        {
                            go->graphic.color = MkF4(
                                0.82f,
                                0.05f,
                                0.68f,
                                -0.9f
                            );
                            go->vel.x = sin(2.0f * SYS_GENERAL_PI / 5 * i) * 1.0f;
                            go->vel.y = cos(2.0f * SYS_GENERAL_PI / 5 * i) * 1.0f;
                            go->friction = 0.01f;
                            //go->graphic.rot3d.x = 90.0f;
                            go->graphic.rot3d.z = 50.0f * i;
                            go->graphic.color.w = 1.0f;
                            go->scale = MkF3(
                                5.0f,
                                5.0f,
                                5.0f
                            );
                            GameObjectSetLifeTime(go, 8);
                        }
                    }
                }
            }

            if (m_StartFrame == FRAME_ONE_SECOND * 9 + 90)
            {
                //SoundEffectPlayRequest(GetSoundData("SOUND/SE/inperiusu/b_kamae"), 1.0f, 0);
                StartAnim(AnimState::Stating_Head_Up, 20, true);
            }

            if (m_StartFrame == FRAME_ONE_SECOND * 9 + 110)
            {
                m_Obj->graphic.shaderOpt.outline = DPD_SHADER_OPT_OUTLINE_NORMAL;
                m_Obj->graphic.shaderOpt.outlineLayer = 1;

                //SEを再生
                SoundEffectPlayRequest(GetSoundData("SOUND/SE/inperiusu/b_attack"), 
                    0.8f, 0, 0.7f);
                //SEを再生
                SoundEffectPlayRequest(GetSoundData("SOUND/SE/enemy/move/warpHigh"), 1.3f, 0);

                CameraShakeRequestAll(CAMERA_SHAKE_MIDIUM_FRAME, CAMERA_SHAKE_EXLARGE_POWER);

                //CameraFlashRequestAll(30,
                //    EasingInOutSine,
                //    MkF4(0.0f, 0.0f, 0.0f, 0.35f),
                //    DPD_SHADER_OPT_BLEND_STATE_DEFAULT
                //);

                //m_StartEyeEffFrameMax = 25;
                m_StartEyeEffFrameMax = 45;
                m_StartEyeEffFrame = m_StartEyeEffFrameMax;

                m_BodyTexMaskInitFlag = true;

                {
                    /*           Float3 efPos = m_Obj->pos;
                               efPos.x += 200.0f;
                               efPos.y += 200.0f;
                               efPos.z += -1200.0f;

                               GAME_OBJECT* go = GO_CreateEffect(GO_EFFECT_TYPE_TWINKLE, efPos);
                               if (go != nullptr)
                               {
                                   go->graphic.color.x = 0.1f;
                                   go->graphic.color.y = 1.0f;
                                   go->graphic.color.z = 0.1f;
                                   go->graphic.color.w = 1.0f;
                                   go->size = MkF2(
                                       0.5f,
                                       0.5f
                                   );
                                   go->scale = MkF3(
                                       3.0f,
                                       0.3f,
                                       1.0f
                                   );
                                   GameObjectSetLifeTime(go, 10);
                               }*/
                }

                {
                    Float3 efPos = m_Obj->pos;
                    efPos.y += 100.0f;
                    efPos.z += -1220.0f;

                    for (int i = 0; i < 5; i++)
                    {
                        Float3 efPos2 = efPos;
                        efPos2.x += cos(2.0f * SYS_GENERAL_PI / 5 * i) * 200.0f;
                        efPos2.y += sin(2.0f * SYS_GENERAL_PI / 5 * i) * 200.0f;

                        GAME_OBJECT* go = GO_CreateEffect(GO_EFFECT_TYPE_SMOKE, efPos2);
                        if (go != nullptr)
                        {
                            go->graphic.rot3d.x = 90.0f;
                            go->graphic.color = MkF4(
                                0.3f,
                                0.3f,
                                0.3f,
                                1.0f
                            );
                            go->scale = MkF3(
                                1.2f,
                                1.2f,
                                1.2
                            );
                            GameObjectSetLifeTime(go, 30);
                        }
                    }
                }
            }
        }


        constexpr int startPosef = FRAME_ONE_SECOND * 12;
        if (m_StartFrame == startPosef - ATTACK_BLADE_BEFORE_FRAME_STARTPOSE)
        {
            //SEを再生
            SoundEffectPlayRequest(GetSoundData("SOUND/SE/inperiusu/b_kamae"), 1.4f, 0);
            //SEを再生
            //SoundEffectPlayRequest(GetSoundData("SOUND/SE/inperiusu/b_charge"), 1.5f, 5);


            set_blade_visible(true);
            StartAnim(AnimState::BladeBefore, ATTACK_BLADE_BEFORE_FRAME_STARTPOSE, true);

        
            Float3 zoomPos = m_Obj->pos;
            zoomPos.x += 0.0f;
            zoomPos.y += -1200.0f;
            zoomPos.z += 5000.0f;
            Float3 zoomPosTa = m_Obj->pos;
            zoomPosTa.y += 10.0f;
            zoomPosTa.z += 18000.0f;
            CameraMoveRequestAll(60,
                zoomPos,
                0.0f,
                MkF2(1.0f, 1.0f),
                0.25f,
                EasingInOutSine,
                CAMERA_MOVE_TYPE_SMOOTH
            );
            CameraTargetMoveRequestAll(60,
                zoomPosTa,
                EasingInOutSine,
                CAMERA_MOVE_TYPE_SMOOTH
            );
        }
        //if (m_StartFrame == startPosef - ATTACK_BLADE_BEFORE_FRAME_STARTPOSE
        //    + ATTACK_BLADE_BEFORE_FIXED_FRAME - 30)
        //{
        //    set_blade_visible(true);
        //}
        //if (m_StartFrame == startPosef - ATTACK_BLADE_BEFORE_FRAME_STARTPOSE + 60)
        //{
        //    Float3 zoomPos = m_Obj->pos;
        //    zoomPos.x += 0.0f;
        //    zoomPos.y += -800.0f;
        //    zoomPos.z += 5000.0f;
        //    Float3 zoomPosTa = m_Obj->pos;
        //    zoomPosTa.y += 0.0f;
        //    zoomPosTa.z += 18000.0f;
        //    CameraMoveRequestAll(20,
        //        zoomPos,
        //        0.0f,
        //        MkF2(1.0f, 1.0f),
        //        0.4f,
        //        EasingInOutSine,
        //        CAMERA_MOVE_TYPE_SMOOTH
        //    );
        //    CameraTargetMoveRequestAll(20,
        //        zoomPosTa,
        //        EasingInOutSine,
        //        CAMERA_MOVE_TYPE_SMOOTH
        //    );
        //}

        if (m_StartFrame == startPosef)
        {
            //アウトライン有効化

            //m_Obj->graphic.shaderOpt.outline = true;
            //m_Obj->graphic.shaderOpt.outline = DPD_SHADER_OPT_OUTLINE_PURPLE;

            //SEを再生
            //SoundEffectPlayRequest(GetSoundData("SOUND/SE/inperiusu/b_attackfull"), 1.6f, 0);

            SoundEffectPlayRequest(GetSoundData("SOUND/SE/enemy/attack/Small_Explosion"),
                2.0f, 0, 0.8f);
            //SoundEffectPlayRequest(GetSoundData("SOUND/SE/enemy/attack/Explosion"),
            //    1.0f, 0, 0.6f);
            SoundEffectPlayRequest(GetSoundData("SOUND/SE/inperiusu/b_attack"),
                1.5f, 0);
            SoundEffectPlayRequest(
                GetSoundData("SOUND/SE/enemy/attack/super_beam_miminari"),
                0.45f, 3, 0.75f);

            CameraShakeRequestAll(CAMERA_SHAKE_SMALL_FRAME,
                CAMERA_SHAKE_EXLARGE_POWER);

            StartAnim(AnimState::Blade, ATTACK_BLADE_ATTACK_FRAME, true);

            //GraphicSpriteSet(m_Tex_Main_Blade[0], 6, 6);
            //Character::PlayMotionCharacterGameObject(
            //    m_CharaData,
            //    Character::GetCharacterMotionFindByName(
            //        Character::GetCharacterMainData(m_CharaData->dataIndex),
            //        "BLADE"),
            //    false,
            //    nullptr,
            //    ATTACK_BLADE_FRAME
            //);

            Float3 zoomPos = m_Obj->pos;
            //zoomPos.x += -550.0f;
            zoomPos.x += -450.0f;
           // zoomPos.y += -200.0f;
            zoomPos.y += 300.0f;
            //zoomPos.z += 19600.0f;
            zoomPos.z += 1000.0f;

            Float3 zoomPosTa = m_Obj->pos;
            zoomPosTa.x += -600.0f;
            zoomPosTa.y += -500.0f;
            //zoomPosTa.z += -18000.0f;
            zoomPosTa.z += -19000.0f;

            //CameraZoomRequestAll(
            //    0.03f,
            //    zoomPos,
            //    EasingInOutSine,
            //    0.6f
            //);
            {
                CameraMoveRequestAll(ATTACK_BLADE_ATTACKING_FRAME_STARTPOSE - 1,
                //CameraMoveRequestAll(ATTACK_BLADE_ATTACKING_FRAME,
                    zoomPos,
                    -0.1f,
                    MkF2(1.0f, 1.0f),
                    //0.45f,
                    0.8f,
                    EasingInOutSine,
                    CAMERA_MOVE_TYPE_SMOOTH
                );
                CameraTargetMoveRequestAll(ATTACK_BLADE_ATTACKING_FRAME_STARTPOSE - 1,
                //CameraTargetMoveRequestAll(ATTACK_BLADE_ATTACKING_FRAME,
                    zoomPosTa,
                    EasingInOutSine,
                    CAMERA_MOVE_TYPE_SMOOTH
                );
            }
        }

        constexpr int stopstartf = startPosef + ATTACK_BLADE_ATTACKING_FRAME_STARTPOSE;
        //constexpr int stopstartf = FRAME_ONE_SECOND * 6 + ATTACK_BLADE_EFFECT_START_FRAME + ATTACK_BLADE_ATTACK_START_FRAME;
        //constexpr int stopframe = FRAME_ONE_SECOND * 2;
        constexpr int stopframe = FRAME_ONE_SECOND * 2.25;
        if (m_StartFrame == stopstartf)
        {
            //m_StartMotionBeforeShader = GetPostEffectShader();
            //SetPostEffectShader("SHADER/shader_world_iseki_boss");

            m_StartMotionBeforeShaderPS = GetPostEffectShaderPS();
            SetPostEffectShaderPS("SHADER/ps_world_iseki_boss");
            m_StartMotionBeforeShaderVS = GetPostEffectShaderVS();
            SetPostEffectShaderVS("SHADER/vs_general2");

            SetPostEffectGlobalOutlineWidth(20.0f);

            m_StartMotionShaderFlag = true;

            SetControllerMainLeftVibration(30, 0);
            SetControllerMainRightVibration(30, 0);

            //GameStopRequest(stopframe);
            //GameObjectHitStop(m_Obj, stopframe);
            GameObjectHitStop(m_Obj, stopframe + FRAME_ONE_SECOND * 0.5f);

            //CameraZoomResetRequestAll(
            //    0.02f,
            //    EasingInOutSine,
            //    stopframe
            //);

            {
                CAMERA cInfo = GetCameraInfo(0);
                Float3 pos = cInfo.trans.pos;
                pos.x += -10.0f;

                CameraMoveRequestAll(stopframe,
                    pos,
                    cInfo.trans.rot + -0.01f,
                    MkF2(1.0f, 1.0f),
                    cInfo.trans.zoom + 0.15f,
                    nullptr, //EasingInOutSine,
                    CAMERA_MOVE_TYPE_SMOOTH
                );
            }
        }

        if (m_StartFrame == stopstartf + stopframe)
        //if (m_StartFrame == stopstartf /* + stopframe */ + 1)
        {
            GameManagerSetMoviePlayerHiddenFlag(false);

            //if (m_StartMotionBeforeShader != -1)
            //{
            //    SetPostEffectShader(m_StartMotionBeforeShader);
            //}
            if (m_StartMotionBeforeShaderVS != -1)
            {
                SetPostEffectShaderVS(m_StartMotionBeforeShaderVS);
            }
            if (m_StartMotionBeforeShaderPS != -1)
            {
                SetPostEffectShaderPS(m_StartMotionBeforeShaderPS);
            }

            ResetPostEffectGlobalOutlineWidth();

            m_StartMotionShaderFlag = false;

            {
                //CameraMoveRequestAll(40,
                //    m_Obj->pos,
                //    0.0f,
                //    MkF2(1.0f, 1.0f),
                //    0.25f,
                //    //1.0f,
                //    EasingInSine,
                //    CAMERA_MOVE_TYPE_SMOOTH
                //);
                //CameraTargetMoveRequestAll(40,
                //    m_Obj->pos,
                //    EasingInOutSine,
                //    CAMERA_MOVE_TYPE_SMOOTH
                //);

                Float3 zoomPos = m_Obj->pos;
                zoomPos.x += -450.0f;
                zoomPos.y += 300.0f + 4000.0f;
                zoomPos.z += 1000.0 + 28000.0f - 15000.0f;
                //zoomPos.y += 300.0f + 5000.0f;
                //zoomPos.z += 1000.0 + 28000.0f + 9000.0f;

                Float3 zoomPosTa = m_Obj->pos;
                zoomPosTa.x += -600.0f;
                zoomPosTa.y += -500.0f;
                zoomPosTa.z += -19000.0f + 12000.0f - 15000.0f;
                //zoomPosTa.z += -19000.0f + -2000.0f + 9000.0f;

                {
                    CameraMoveRequestAll(40,
                        zoomPos,
                        0.0f,
                        MkF2(1.0f, 1.0f),
                        0.4f,
                        EasingOutSine,
                        CAMERA_MOVE_TYPE_SMOOTH
                    );
                    CameraTargetMoveRequestAll(40,
                        zoomPosTa,
                        EasingOutSine,
                        CAMERA_MOVE_TYPE_SMOOTH
                    );
                }
            }
        }

        if (m_StartFrame == stopstartf + stopframe + FRAME_ONE_SECOND * 0.5f)
        {
            set_blade_visible(false);
        }

        //開始フレーム
        constexpr int startingF = stopstartf + stopframe + FRAME_ONE_SECOND / 2;

        if (m_StartFrame == startingF + FRAME_ONE_SECOND / 2/* + stopframe */)
        {
            //カウントストップを解除
            //SetStageCountStopSceneGame(false);

            //プレイヤーの硬直を解除
            //GameManagerSetPlayerCtrlStop(false);

            //ムービーフラグ
            GameManagerSetMovieFlag(false);

            SetCameraModeAll(CAMERA_MODE_BOSS_CHASE);
            //SetCameraModeAll(CAMERA_MODE_PLAYER_CHASE);
            m_StartedFlag = true;
        }

        //ムービースキップ処理 (多分後で変える)
        if (m_StartFrame < startingF
            && GameManagerIsMovieSkipTrigger()
        )
        {
            CameraFlashRequestAll(20, EasingOutSine, MkF4(-1.0f, -1.0f, -1.0f, 1.0f));

            SoundEffectStopRequest(alertse, 0);

            if (!IsSoundBGMRequested(bgmidtmp))
            {
                SoundEffectStopRequest(bgminidtmp, 0);
                SoundBGMStopRequest();

                //BGMを再生する
                //SoundBGMPlayRequest(m_bgmLoop, FRAME_ONE_MINUTE + (FRAME_ONE_SECOND * 49), 2.0f);
            
                //BGMを再生する
                SoundBGMPlayRequest(bgmidtmp,
                    true,
                    2.0f,
                    0.0f,

                    0.0f,
                    bgmLen
                );
            }

            CameraMoveCancelRequestAll();
            CameraTargetMoveCancelRequestAll();
            {
                Float3 zoomPos = m_Obj->pos;
                zoomPos.z += -33000.0f;

                SetCameraTargetPosAll(MkF3(zoomPos.x, zoomPos.y - 10.0f, zoomPos.z + 10.0f));
                SetCameraPosAll(zoomPos);
                SetCameraZoomAll(0.6f);
            }
            SetCameraModeAll(CAMERA_MODE_PLAYER_CHASE_INSTANT);

            StopAnim();
            for (int i = 0; i < MODEL_NUM; i++)
            {
                m_ModelTransPos[i].x = 0.0f;
                m_ModelTransPos[i].y = 0.0f;
                m_ModelTransPos[i].z = 0.0f;

                m_ModelTransRot[i].x = 0.0f;
                m_ModelTransRot[i].y = 0.0f;
                m_ModelTransRot[i].z = 0.0f;

                m_ModelTransSca[i].x = 1.0f;
                m_ModelTransSca[i].y = 1.0f;
                m_ModelTransSca[i].z = 1.0f;
            }
            set_blade_visible(false);
            m_Obj->hitStop = {};

            m_StartFrame = startingF;
        }

        //開始
        if (m_StartFrame == startingF)
        {
            become_start();

            SetCameraTargetPosAll(MkF3(0, 0, 0));
            event_playerPosSet();

            if (m_IsHard)
            {
                GE_GayaStartRequest("iseki_inperiusuhard_start_mayu");

            }
            else
            {
                GE_GayaStartRequest("iseki_inperiusu_start_mayu");
            }
        }
    }

    m_StartFrame++;
}

void GOE_Character_Enemy_Boss_Inperiusu::draw_start()
{
    if (m_RetryCount > 0)
    {
    }
    else
    {
        const CAMERA& cinfo = GetCameraInfo(0);
        {
            constexpr int wStartF = FRAME_ONE_SECOND * 1.5;
            constexpr int wEndF = FRAME_ONE_SECOND * 4.5;
            int f = m_StartFrame - wStartF;
            if (
                m_StartFrame >= wStartF
                && m_StartFrame < wEndF
                )
            {
                float mod = std::min(1.0f, f / 30.0f) * std::min(1.0f, 1.0f - (f - (wEndF - wStartF) + 30.0f) / 30.0f);
                for (int i = 0; i < 3; i++)
                {
                    DRAW_POLYGON_DAT dsdat = {};
                    dsdat.pos.x = 0.0f;
                    dsdat.pos.y = 0.0f;
                    dsdat.pos.z = 0.0f;

                    dsdat.size.x = SCREEN_WIDTH;
                    dsdat.size.y = SCREEN_HEIGHT;
                    dsdat.size.z = 1.0f;

                    dsdat.rot = 0.0f;

                    if (i == 0)
                    {
                        dsdat.color = { 1.0f, 1.0f, 1.0f, (0.4f + sin((float)f / 10) * 0.2f) * mod };
                        dsdat.texNo = m_Tex_Warning[0];
                    }
                    if (i == 1)
                    {
                        dsdat.color = { 1.0f, 1.0f, 1.0f, (0.5f + sin((float)f / 10) * 0.1f) * mod };
                        dsdat.texNo = m_Tex_Warning[1];
                    }
                    if (i == 2)
                    {
                        dsdat.texUV.x += f * 0.01f;
                        dsdat.color = { 1.0f, 1.0f, 1.0f, 0.75f * mod };
                        dsdat.texNo = m_Tex_Warning[2];
                    }

                    dsdat.loadTexType = LOADTEXTURETYPE_MAIN;

                    dsdat.texWH = { 1.0f, 1.0f };

                    dsdat.startRot = 0.0f;
                    dsdat.startScale = MkF2(1.0f, 1.0f);
                    dsdat.startScale3D = MkF3(1.0f, 1.0f, 1.0f);

                    dsdat.modelNo = 0;

                    //dsdat.shaderOpt.bloom = true;
                    //dsdat.shaderOpt.blendState = DPD_SHADER_OPT_BLEND_STATE_ADDCOLOR;

                    DRAW_POLYGON_IN_CAMERA_EX_OPTION opt = {};
                    opt.reverse = false;
                    opt.fixed = true;
                    opt.enableShade = false;
                    opt.billboard = false;
                    //opt.priority = DRAW_POLYGON_PRIORITY_NML_u2;
                    opt.priority = DRAW_POLYGON_PRIORITY_NML_ALWAYS;
                    //opt.depthBuffIgnore = true;

                    DrawPolygonInCameraEx(dsdat, opt);
                }
            }
        }

        //if (false && cinfo.flashRequ.frame <= 0)
        //{
        //    //constexpr int txtStartf = FRAME_ONE_SECOND * 6;
        //    //constexpr int txtEndf = FRAME_ONE_SECOND * 7.5;
        //    constexpr int txtStartf = FRAME_ONE_SECOND * 12;
        //    constexpr int txtEndf = FRAME_ONE_SECOND * 15.5;
        //    int f = m_StartFrame - txtStartf;
        //    if (m_StartFrame >= txtStartf
        //        && m_StartFrame < txtEndf)
        //    {
        //        float modIn = EasingInOutBack(std::min(1.0f, f / 30.0f));
        //        float modOut = EasingInOutBack(std::max(0.0f, (f - (txtEndf - txtStartf) + 30.0f) / 30.0f));

        //        float addXIn = (1.0f - modIn) * SCREEN_WIDTH;
        //        float addXOut = modOut * -SCREEN_WIDTH;
        //        DrawFont(m_Obj->status.fullName, FONT_TYPE_KANJI,
        //            MkF2(-600.0f + 20.0f + addXIn + addXOut, 0.0f + 20.0f),
        //            150,
        //            MkF4(0.0f, 0.0f, 0.0f, 0.75f),
        //            FONT_TEXT_ALIGNMENT_CENTER
        //        );
        //        DrawFont(m_Obj->status.fullName, FONT_TYPE_KANJI,
        //            MkF2(-600.0f + addXIn + addXOut, 0.0f),
        //            150,
        //            MkF4(1.0f, 0.0f, 1.0f, 1.0f),
        //            FONT_TEXT_ALIGNMENT_CENTER
        //        );
        //    }
        //}
    }
}

int GOE_Character_Enemy_Boss_Inperiusu::zako_powerup()
{
    int enemNum = 0;
    int enemLegaNum = 0;
    for (int i = 0; i < GAME_OBJECT_MAX; i++)
    {
        GAME_OBJECT* gObj = GetGameObjects() + i;
        if (IsActiveGameObject(gObj))
        {
            if (
                HasTagGameObject(gObj, GOE_Character_Enemy_Miresu::CHARACTER_NAME_ATTACKER)
                || HasTagGameObject(gObj, GOE_Character_Enemy_Miresu::CHARACTER_NAME_SHOOTER)
                )
            {
                GOE_Character_Enemy_Miresu* goeTmp = (GOE_Character_Enemy_Miresu*)(gObj->events.source);
                if (goeTmp != nullptr && !goeTmp->GetIsBuffSuper())
                {
                    goeTmp->SetBuffSuper();
                }
                enemNum++;
            }

            if (HasTagGameObject(gObj, GOE_Character_Enemy_Regatosu::CHARACTER_NAME))
            {
                GOE_Character_Enemy_Regatosu* goeTmp = (GOE_Character_Enemy_Regatosu*)(gObj->events.source);
                if (goeTmp != nullptr && !goeTmp->GetIsBuffSuper())
                {
                    goeTmp->SetBuffSuper();
                }
                enemNum++;
                enemLegaNum++;
            }
        }
    }

    return enemNum;
}

int GOE_Character_Enemy_Boss_Inperiusu::zako_powerdown()
{
    int enemNum = 0;
    //int enemLegaNum = 0;
    for (int i = 0; i < GAME_OBJECT_MAX; i++)
    {
        GAME_OBJECT* gObj = GetGameObjects() + i;
        if (IsActiveGameObject(gObj))
        {
            if (
                HasTagGameObject(gObj, GOE_Character_Enemy_Miresu::CHARACTER_NAME_ATTACKER)
                || HasTagGameObject(gObj, GOE_Character_Enemy_Miresu::CHARACTER_NAME_SHOOTER)
                )
            {
                GOE_Character_Enemy_Miresu* goeTmp = (GOE_Character_Enemy_Miresu*)(gObj->events.source);
                if (goeTmp != nullptr && !goeTmp->GetIsBuffBreak())
                {
                    goeTmp->SetBuffBreak();
                }
                enemNum++;
            }

            //if (HasTagGameObject(gObj, GOE_Character_Enemy_Regatosu::CHARACTER_NAME))
            //{
            //    GOE_Character_Enemy_Regatosu* goeTmp = (GOE_Character_Enemy_Regatosu*)(gObj->events.source);
            //    if (goeTmp != nullptr && !goeTmp->SetBuffBreak())
            //    {
            //        goeTmp->SetBuffBreak();
            //    }
            //    enemNum++;
            //    enemLegaNum++;
            //}
        }
    }

    return enemNum;
}

void GOE_Character_Enemy_Boss_Inperiusu::safe_area_move()
{
    bool isOut = false;
    Float3 afterPos = {};
    {
        Stage_Base* stg = GetCurrentStageSceneGame();
        if (stg != nullptr)
        {
            Int2 stgsize = stg->GetStageSize();

            Float2 size = {};
            size.x = BLOCK_WIDTH * stgsize.x;
            size.y = BLOCK_HIGHT * stgsize.y;


            if (m_Obj->pos.y < size.y * -0.5f / 2.0f)
            {
                isOut = true;
                afterPos.x = m_Obj->pos.x;
                afterPos.y = ONE_BLOCK * -12.0f;
            }
            if (m_Obj->pos.y > size.y * 0.35f / 2.0f)
            {
                isOut = true;
                afterPos.x = m_Obj->pos.x;
                afterPos.y = ONE_BLOCK * -12.0f;
            }

            if (m_Obj->pos.x < size.x * -0.5f / 2.0f)
            {
                isOut = true;
                afterPos.x = size.x * -0.4f / 2.0f;
                afterPos.y = ONE_BLOCK * -12.0f;
            }
            if (m_Obj->pos.x > size.x * 0.5f / 2.0f)
            {
                isOut = true;
                afterPos.x = size.x * 0.4f / 2.0f;
                afterPos.y = ONE_BLOCK * -12.0f;
            }
        }
    }
    if (isOut)
    {
        if (m_IsHard)
        {
            StartWarp(afterPos, 0.0f, 25);
        }
        else
        {
            if (CalculateDistance(afterPos, m_Obj->pos) > ONE_BLOCK * 20)
            {
                StartJet(afterPos, 0.0f,
                    160//240
                );
            }
            else
            {
                StartMove(afterPos, 0.0f, 3, true);
            }
        }
    }
}


//void GOE_Character_Enemy_Boss_Inperiusu::update_start()
//{
//    if (m_StartFrame == 0)
//    {
//        m_StartMotionBeforeShader = -1;
//        //カウントをストップ
//        //SetStageCountStopSceneGame(true);
//
//        //プレイヤーの硬直
//        //GameManagerSetPlayerCtrlStop(true);
//
//        //ムービーフラグ
//        GameManagerSetMovieFlag(true);
//
//        SetCameraModeAll(CAMERA_MODE_NORMAL);
//    }
//
//    if (m_StartFrame == FRAME_ONE_SECOND * 1)
//    {
//        CameraMoveRequestAll(FRAME_ONE_SECOND * 1,
//            m_Obj->pos,
//            0.0f,
//            MkF2(1.0f, 1.0f),
//            0.35f,
//            EasingInOutSine,
//            CAMERA_MOVE_TYPE_SMOOTH
//        );
//        CameraTargetMoveRequestAll(FRAME_ONE_SECOND * 1,
//            m_Obj->pos,
//            EasingInOutSine,
//            CAMERA_MOVE_TYPE_SMOOTH
//        );
//    }
//
//    if (
//        m_StartFrame >= FRAME_ONE_SECOND / 2
//        && m_StartFrame < FRAME_ONE_SECOND * 4
//        )
//    {
//        if (m_StartFrame % 15 == 0)
//        {
//            CameraShakeRequestAll(CAMERA_SHAKE_MIDIUM_FRAME, CAMERA_SHAKE_EXLARGE_POWER);
//        }
//    }
//    if (m_StartFrame == FRAME_ONE_SECOND * 4)
//    {
//        GameObjectMoveRequest(m_Obj,
//            FRAME_ONE_SECOND * 2,
//            MkF3(m_Obj->pos.x, m_Obj->pos.y, 0.0f),
//            EasingInOutSine,
//            EasingInOutSine,
//            EasingInOutSine,
//            true
//        );
//        GraphicSpriteSet(m_Tex_Main_Kidou, 6, 6);
//        Character::PlayMotionCharacterGameObject(
//            m_CharaData,
//            Character::GetCharacterMotionFindByName(
//                Character::GetCharacterMainData(m_CharaData->dataIndex),
//                "KIDOU"),
//            false,
//            nullptr,
//            FRAME_ONE_SECOND * 2
//        );
//    }
//
//    if (m_StartFrame == FRAME_ONE_SECOND * 6)
//    {
//        //アウトライン有効化
//
//        //m_Obj->graphic.shaderOpt.outline = true;
//        m_Obj->graphic.shaderOpt.outline = DPD_SHADER_OPT_OUTLINE_PURPLE;
//
//        GraphicSpriteSet(m_Tex_Main_Blade[0], 6, 6);
//        Character::PlayMotionCharacterGameObject(
//            m_CharaData,
//            Character::GetCharacterMotionFindByName(
//                Character::GetCharacterMainData(m_CharaData->dataIndex),
//                "BLADE"),
//            false,
//            nullptr,
//            ATTACK_BLADE_FRAME
//        );
//
//        Float2 zoomPos = Float3ToFloat2(m_Obj->pos);
//        zoomPos.x += -100.0f;
//        zoomPos.y += -800.0f;
//        //CameraZoomRequestAll(
//        //    0.03f,
//        //    zoomPos,
//        //    EasingInOutSine,
//        //    0.6f
//        //);
//        {
//            CameraMoveRequestAll(40,
//                zoomPos,
//                0.0f,
//                MkF2(1.0f, 1.0f),
//                0.6f,
//                EasingInOutSine,
//                CAMERA_MOVE_TYPE_SMOOTH
//            );
//        }
//    }
//
//    constexpr int stopstartf = FRAME_ONE_SECOND * 6 + 32 + 10;
//    //constexpr int stopstartf = FRAME_ONE_SECOND * 6 + ATTACK_BLADE_EFFECT_START_FRAME + ATTACK_BLADE_ATTACK_START_FRAME;
//    constexpr int stopframe = FRAME_ONE_SECOND * 2;
//    if (m_StartFrame == stopstartf)
//    {
//        m_StartMotionBeforeShader = GetPostEffectShader();
//        SetPostEffectShader("SHADER/shader_world_iseki_boss");
//
//        CameraShakeRequestAll(CAMERA_SHAKE_EXLARGE_FRAME, CAMERA_SHAKE_EXLARGE_POWER);
//
//        GameStopRequest(stopframe);
//        //CameraZoomResetRequestAll(
//        //    0.02f,
//        //    EasingInOutSine,
//        //    stopframe
//        //);
//        {
//            CAMERA cInfo = GetCameraInfo(0);
//            Float3 pos = cInfo.trans.pos;
//            pos.x += -200.0f;
//
//            CameraMoveRequestAll(stopframe,
//                pos,
//                -0.6f,
//                MkF2(1.0f, 1.0f),
//                0.65f,
//                nullptr, //EasingInOutSine,
//                CAMERA_MOVE_TYPE_SMOOTH
//            );
//        }
//    }
//
//    if (m_StartFrame == stopstartf /* + stopframe */ + 1)
//    {
//        {
//            CameraMoveRequestAll(40,
//                Float3ToFloat2(m_Obj->pos),
//                0.0f,
//                MkF2(1.0f, 1.0f),
//                0.35f,
//                //1.0f,
//                EasingInSine,
//                CAMERA_MOVE_TYPE_SMOOTH
//            );
//        }
//    }
//
//    if (m_StartFrame == FRAME_ONE_SECOND * 7.5/* + stopframe */)
//    {
//        //カウントストップを解除
//        //SetStageCountStopSceneGame(false);
//
//        //プレイヤーの硬直を解除
//        //GameManagerSetPlayerCtrlStop(false);
//
//        //ムービーフラグ
//        GameManagerSetMovieFlag(false);
//
//        SetCameraModeAll(CAMERA_MODE_BOSS_CHASE);
//        //SetCameraModeAll(CAMERA_MODE_PLAYER_CHASE);
//        m_StartedFlag = true;
//    }
//
//    //開始
//    int startingF = FRAME_ONE_SECOND * 7;
//    if (m_StartFrame < startingF && GetControllerMainTrigger(CONTROLLER_BUTTON_TYPE_PLUS))
//    {
//        CameraFlashRequestAll(20, EasingOutSine, MkF4(-1.0f, -1.0f, -1.0f, 1.0f));
//        m_StartFrame = startingF;
//    }
//
//    if (m_StartFrame == startingF/* + stopframe */)
//    {
//        if (m_StartMotionBeforeShader != -1)
//        {
//            SetPostEffectShader(m_StartMotionBeforeShader);
//        }
//
//        //判定有効化
//        m_Obj->collision.atk.enable = true;
//        m_Obj->collision.rec.enable = true;
//        m_Obj->status.statusType = GAME_OBJECT_STATUS_TYPE_ENEMY_BOSS;
//        m_Obj->graphic.shaderOpt.outline = DPD_SHADER_OPT_OUTLINE_PURPLE;
//
//        m_Obj->pos.z = 0.0f;
//
//
//        //ボスのHPを表示する
//        if (HasTagGameObject(m_Obj, "ENEMY_BOSS_INPERIUSU"))
//        {
//            ShowBossbar_Game(GetGameObjectIndex(m_Obj), true, m_Obj->status.fullName);
//        }
//
//        //アニメーションを再生する
//        //Character::PlayMotionCharacterGameObject(
//        //	m_CharaData,
//        //	Character::GetCharacterMotionFindByName(Character::GetCharacterMainData(m_CharaData->dataIndex), "IDLE"),
//        //	true,
//        //	(int*)(&m_CurrentAxisEx) //スプライトの番号を向きに依存させる
//        //);
//
//
//        {
//            for (int i = 0; i < GAME_OBJECT_MAX; i++)
//            {
//                GAME_OBJECT* gObj = GetGameObjects() + i;
//                if (IsActiveGameObject(gObj))
//                {
//                    if (
//                        HasTagGameObject(gObj, GOE_Character_Enemy_Miresu::CHARACTER_NAME_ATTACKER)
//                        || HasTagGameObject(gObj, GOE_Character_Enemy_Miresu::CHARACTER_NAME_SHOOTER)
//                        )
//                    {
//                        GOE_Character_Enemy_Miresu* goeTmp = (GOE_Character_Enemy_Miresu*)(gObj->events.source);
//                        if (goeTmp != nullptr && !goeTmp->GetIsBuffSuper())
//                        {
//                            goeTmp->SetBuffSuper();
//                        }
//                    }
//
//                    if (HasTagGameObject(gObj, GOE_Character_Enemy_Regatosu::CHARACTER_NAME))
//                    {
//                        GOE_Character_Enemy_Regatosu* goeTmp = (GOE_Character_Enemy_Regatosu*)(gObj->events.source);
//                        if (goeTmp != nullptr && !goeTmp->GetIsBuffSuper())
//                        {
//                            goeTmp->SetBuffSuper();
//                        }
//                    }
//                }
//            }
//        }
//    }
//
//    m_StartFrame++;
//}
//
//void GOE_Character_Enemy_Boss_Inperiusu::draw_start()
//{
//    const CAMERA& cinfo = GetCameraInfo(0);
//    {
//        constexpr int wStartF = FRAME_ONE_SECOND * 1.5;
//        constexpr int wEndF = FRAME_ONE_SECOND * 5;
//        int f = m_StartFrame - wStartF;
//        if (
//            m_StartFrame >= wStartF
//            && m_StartFrame < wEndF
//            )
//        {
//            float mod = std::min(1.0f, f / 30.0f) * std::min(1.0f, 1.0f - (f - (wEndF - wStartF) + 30.0f) / 30.0f);
//            for (int i = 0; i < 3; i++)
//            {
//                DRAW_POLYGON_DAT dsdat = {};
//                dsdat.pos.x = 0.0f;
//                dsdat.pos.y = 0.0f;
//                dsdat.pos.z = 0.0f;
//
//                dsdat.size.x = SCREEN_WIDTH;
//                dsdat.size.y = SCREEN_HEIGHT;
//                dsdat.size.z = 1.0f;
//
//                dsdat.rot = 0.0f;
//
//                if (i == 0)
//                {
//                    dsdat.color = { 1.0f, 1.0f, 1.0f, (0.4f + sin((float)f / 10) * 0.2f) * mod };
//                    dsdat.texNo = m_Tex_Warning[0];
//                }
//                if (i == 1)
//                {
//                    dsdat.color = { 1.0f, 1.0f, 1.0f, (0.5f + sin((float)f / 10) * 0.1f) * mod };
//                    dsdat.texNo = m_Tex_Warning[1];
//                }
//                if (i == 2)
//                {
//                    dsdat.texUV.x += f * 0.01f;
//                    dsdat.color = { 1.0f, 1.0f, 1.0f, 0.75f * mod };
//                    dsdat.texNo = m_Tex_Warning[2];
//                }
//
//                dsdat.loadTexType = LOADTEXTURETYPE_MAIN;
//
//                dsdat.texWH = { 1.0f, 1.0f };
//
//                dsdat.startRot = 0.0f;
//                dsdat.startScale = MkF2(1.0f, 1.0f);
//                dsdat.startScale3D = MkF3(1.0f, 1.0f, 1.0f);
//
//                dsdat.modelNo = 0;
//
//                //dsdat.shaderOpt.bloom = true;
//                //dsdat.shaderOpt.blendState = DPD_SHADER_OPT_BLEND_STATE_ADDCOLOR;
//
//                DRAW_POLYGON_IN_CAMERA_EX_OPTION opt = {};
//                opt.reverse = false;
//                opt.fixed = true;
//                opt.enableShade = false;
//                opt.billboard = false;
//                //opt.priority = DRAW_POLYGON_PRIORITY_NML_u2;
//                opt.priority = DRAW_POLYGON_PRIORITY_NML_ALWAYS;
//                //opt.depthBuffIgnore = true;
//
//                DrawPolygonInCameraEx(dsdat, opt);
//            }
//        }
//    }
//
//    if (cinfo.flashRequ.frame <= 0)
//    {
//        constexpr int txtStartf = FRAME_ONE_SECOND * 6;
//        constexpr int txtEndf = FRAME_ONE_SECOND * 7.5;
//        int f = m_StartFrame - txtStartf;
//        if (m_StartFrame >= txtStartf
//            && m_StartFrame < txtEndf)
//        {
//            float modIn = EasingInOutBack(std::min(1.0f, f / 30.0f));
//            float modOut = EasingInOutBack(std::max(0.0f, (f - (txtEndf - txtStartf) + 30.0f) / 30.0f));
//
//            float addXIn = (1.0f - modIn) * SCREEN_WIDTH;
//            float addXOut = modOut * -SCREEN_WIDTH;
//            DrawFont(m_Obj->status.fullName, FONT_TYPE_KANJI,
//                MkF2(-600.0f + 20.0f + addXIn + addXOut, 0.0f + 20.0f),
//                150,
//                MkF4(0.0f, 0.0f, 0.0f, 0.75f),
//                FONT_TEXT_ALIGNMENT_CENTER
//            );
//            DrawFont(m_Obj->status.fullName, FONT_TYPE_KANJI,
//                MkF2(-600.0f + addXIn + addXOut, 0.0f),
//                150,
//                MkF4(1.0f, 0.0f, 1.0f, 1.0f),
//                FONT_TEXT_ALIGNMENT_CENTER
//            );
//        }
//    }
//}
