// ===================================================
// GOE_Character_Player_KAI.cpp キャラクターオブジェクト
// 
// hara sougo  2025/01/26
// ===================================================
#define NO_CONTROLLER

#include "../../../../Game/Configuration/Param.h"
#include "../../../../game_main.h"
#include "../../../Sprite/sprite.h"
#include "../../../Texture/texture.h"
#include "../../../../Game/Entities/Block/block.h"
#include "../../../Systems/collision.h"
#include "../../../Systems/easing.h"

#include "../../../Managers/gameManager.h"
#include "../../../Managers/scene.h"

#include "../../gameObject.h"
#include "../../gameObjectPrefab.h"

#include "../../../Systems/camera.h"
#include "../../../Systems/ui.h"
#include "../../../Systems/ui_Game.h"

#include "../../../Entities/player.h"


#include "../../../Managers/soundManager.h"

#include "../GOE_Effect.h"
#include "../GOE_Character.h"

#include "GOE_Character_Player_KAI.h"

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
void GOE_Character_Player_KAI::Init()
{
	SuperGOE::Init(); //初期化

    //アイコン
    m_TexIconMain_N = LoadTexData("TEX/ui/in_game_master/icon-assets/kaito_icon");
    m_TexIconMain_Pinch = LoadTexData("TEX/ui/in_game_master/icon-assets/kaito_pinch");
    m_TexIconMain_Out = LoadTexData("TEX/ui/in_game_master/icon-assets/kaito_out");

    m_TexIconSub = LoadTexData("TEX/ui/in_game_master/kaito_icon");
    m_TexIconSkillOn = LoadTexData("TEX/ui/in_game_master/skill_buff_on");
    m_TexIconSkillOff = LoadTexData("TEX/ui/in_game_master/skill_buff_off");

    SetCharaSubGraphic();

    GraphicSet(m_Tex_Main_Idle, 3, 3);
    //m_Obj->graphic.graph.spriteAnim->pos.y += -25.0f;
    //m_Obj->graphic.graph.spriteAnim->pos3D.z += -80.0f;

    m_MoveSpeedMod = MOVE_SPEED_MOD;
    m_MoveSpeedModKako = MOVE_SPEED_MOD_KAKO;

    m_Obj->friction *= MOVE_FRICTION_MOD;


    //かこぽん

    m_KakoAddDis = ONE_BLOCK * 3.5f;

    m_ThemeColor = { 0.0f, 1.0f, 2.0f };

    //m_KakoDat_Main.graphic = KAKOPON_GRAPHIC::Main;
    //m_KakoDat_Main.type = KAKOPON_TYPE::Main;
    //m_KakoDat_Main.frameMax = GOE_Character_Player::KAKOPON_COOLDOWN_FRAME;

    //m_KakoDat_SubTest.graphic = KAKOPON_GRAPHIC::Main;
    //m_KakoDat_SubTest.type = KAKOPON_TYPE::Heal;
    //m_KakoDat_SubTest.frameMax = FRAME_ONE_SECOND * 5;
    m_KakoDatList[0].dat.graphic = KAKOPON_GRAPHIC::Main_Kai;
    m_KakoDatList[0].dat.type = KAKOPON_TYPE::Main;
    m_KakoDatList[0].dat.frameMax = KAKOPON_COOLDOWN_FRAME;

    m_KakoDatList[1].dat.graphic = KAKOPON_GRAPHIC::Buff;
    m_KakoDatList[1].dat.type = KAKOPON_TYPE::BuffAtk;
    m_KakoDatList[1].dat.frameMax = FRAME_ONE_SECOND * 10;

}


void GOE_Character_Player_KAI::SetCharaSubGraphic()
{
    UnloadTexData(m_Tex_Main_Attack[0]);
    UnloadTexData(m_Tex_Main_Attack[1]);
    UnloadTexData(m_Tex_Main_Attack[2]);
    UnloadTexData(m_Tex_Main_Attack[3]);
    UnloadTexData(m_Tex_Main_Attack[4]);
    UnloadTexData(m_Tex_Main_Attack[5]);
    UnloadTexData(m_Tex_Main_Attack[6]);
    UnloadTexData(m_Tex_Main_Attack[7]);

    UnloadTexData(m_Tex_Main_Walk[0]);
    UnloadTexData(m_Tex_Main_Walk[1]);
    UnloadTexData(m_Tex_Main_Walk[2]);
    UnloadTexData(m_Tex_Main_Walk[3]);
    UnloadTexData(m_Tex_Main_Walk[4]);
    UnloadTexData(m_Tex_Main_Walk[5]);
    UnloadTexData(m_Tex_Main_Walk[6]);
    UnloadTexData(m_Tex_Main_Walk[7]);

    UnloadTexData(m_Tex_Main_Draw[0]);
    UnloadTexData(m_Tex_Main_Draw[1]);
    UnloadTexData(m_Tex_Main_Draw[2]);
    UnloadTexData(m_Tex_Main_Draw[3]);
    UnloadTexData(m_Tex_Main_Draw[4]);
    UnloadTexData(m_Tex_Main_Draw[5]);
    UnloadTexData(m_Tex_Main_Draw[6]);
    UnloadTexData(m_Tex_Main_Draw[7]);

    UnloadTexData(m_Tex_Main_Idle);
    UnloadTexData(m_Tex_Main_Down);

    std::string pstr = m_OnajiCharaNum > 0 ? std::to_string(m_OnajiCharaNum + 1) : "";

    m_Tex_Main_Attack[0] = LoadTexData("TEX/player/kai" + pstr + "/attack/kai" + pstr + "_attack_01");
    m_Tex_Main_Attack[1] = LoadTexData("TEX/player/kai" + pstr + "/attack/kai" + pstr + "_attack_02");
    m_Tex_Main_Attack[2] = LoadTexData("TEX/player/kai" + pstr + "/attack/kai" + pstr + "_attack_03");
    m_Tex_Main_Attack[3] = LoadTexData("TEX/player/kai" + pstr + "/attack/kai" + pstr + "_attack_04");
    m_Tex_Main_Attack[4] = LoadTexData("TEX/player/kai" + pstr + "/attack/kai" + pstr + "_attack_05");
    m_Tex_Main_Attack[5] = LoadTexData("TEX/player/kai" + pstr + "/attack/kai" + pstr + "_attack_06");
    m_Tex_Main_Attack[6] = LoadTexData("TEX/player/kai" + pstr + "/attack/kai" + pstr + "_attack_07");
    m_Tex_Main_Attack[7] = LoadTexData("TEX/player/kai" + pstr + "/attack/kai" + pstr + "_attack_08");

    m_Tex_Main_Walk[0] = LoadTexData("TEX/player/kai" + pstr + "/move/kai" + pstr + "_walk_01");
    m_Tex_Main_Walk[1] = LoadTexData("TEX/player/kai" + pstr + "/move/kai" + pstr + "_walk_02");
    m_Tex_Main_Walk[2] = LoadTexData("TEX/player/kai" + pstr + "/move/kai" + pstr + "_walk_03");
    m_Tex_Main_Walk[3] = LoadTexData("TEX/player/kai" + pstr + "/move/kai" + pstr + "_walk_04");
    m_Tex_Main_Walk[4] = LoadTexData("TEX/player/kai" + pstr + "/move/kai" + pstr + "_walk_05");
    m_Tex_Main_Walk[5] = LoadTexData("TEX/player/kai" + pstr + "/move/kai" + pstr + "_walk_06");
    m_Tex_Main_Walk[6] = LoadTexData("TEX/player/kai" + pstr + "/move/kai" + pstr + "_walk_07");
    m_Tex_Main_Walk[7] = LoadTexData("TEX/player/kai" + pstr + "/move/kai" + pstr + "_walk_08");

    m_Tex_Main_Draw[0] = LoadTexData("TEX/player/kai" + pstr + "/kako/kai" + pstr + "_skill_01");
    m_Tex_Main_Draw[1] = LoadTexData("TEX/player/kai" + pstr + "/kako/kai" + pstr + "_skill_02");
    m_Tex_Main_Draw[2] = LoadTexData("TEX/player/kai" + pstr + "/kako/kai" + pstr + "_skill_03");
    m_Tex_Main_Draw[3] = LoadTexData("TEX/player/kai" + pstr + "/kako/kai" + pstr + "_skill_04");
    m_Tex_Main_Draw[4] = LoadTexData("TEX/player/kai" + pstr + "/kako/kai" + pstr + "_skill_05");
    m_Tex_Main_Draw[5] = LoadTexData("TEX/player/kai" + pstr + "/kako/kai" + pstr + "_skill_06");
    m_Tex_Main_Draw[6] = LoadTexData("TEX/player/kai" + pstr + "/kako/kai" + pstr + "_skill_07");
    m_Tex_Main_Draw[7] = LoadTexData("TEX/player/kai" + pstr + "/kako/kai" + pstr + "_skill_08");

    m_Tex_Main_Idle = LoadTexData("TEX/player/kai" + pstr + "/kai" + pstr + "_idle");

    m_Tex_Main_Down = LoadTexData("TEX/player/kai" + pstr + "/kai" + pstr + "_damage_01");
}

// ===================================================
// FirstFrame
// 最初のフレームのみ実行
// ===================================================
void GOE_Character_Player_KAI::FirstFrame()
{
	SuperGOE::FirstFrame();

	PLAYER* player = GetPlayer(m_Obj);
	if (player != nullptr)
	{
        //
        //↓処理
        //
        
		//return;
	}
    //最初のフレーム(初期化とか)

}

// ===================================================
// Uninit
// オブジェクトが削除されるとき
// ===================================================
void GOE_Character_Player_KAI::Uninit()
{

    UnloadTexData(m_Tex_Main_Attack[0]);
    UnloadTexData(m_Tex_Main_Attack[1]);
    UnloadTexData(m_Tex_Main_Attack[2]);
    UnloadTexData(m_Tex_Main_Attack[3]);
    UnloadTexData(m_Tex_Main_Attack[4]);
    UnloadTexData(m_Tex_Main_Attack[5]);
    UnloadTexData(m_Tex_Main_Attack[6]);
    UnloadTexData(m_Tex_Main_Attack[7]);

    UnloadTexData(m_Tex_Main_Walk[0]);
    UnloadTexData(m_Tex_Main_Walk[1]);
    UnloadTexData(m_Tex_Main_Walk[2]);
    UnloadTexData(m_Tex_Main_Walk[3]);
    UnloadTexData(m_Tex_Main_Walk[4]);
    UnloadTexData(m_Tex_Main_Walk[5]);
    UnloadTexData(m_Tex_Main_Walk[6]);
    UnloadTexData(m_Tex_Main_Walk[7]);

    UnloadTexData(m_Tex_Main_Draw[0]);
    UnloadTexData(m_Tex_Main_Draw[1]);
    UnloadTexData(m_Tex_Main_Draw[2]);
    UnloadTexData(m_Tex_Main_Draw[3]);
    UnloadTexData(m_Tex_Main_Draw[4]);
    UnloadTexData(m_Tex_Main_Draw[5]);
    UnloadTexData(m_Tex_Main_Draw[6]);
    UnloadTexData(m_Tex_Main_Draw[7]);

    UnloadTexData(m_Tex_Main_Idle);
    UnloadTexData(m_Tex_Main_Down);

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
void GOE_Character_Player_KAI::Update()
{
	SuperGOE::Update();


}

// ===================================================
// UpdateActive
// 毎フレーム実行
// ただし、ヒットストップ中などの
// 非アクティブ時は実行されない
// ===================================================
void GOE_Character_Player_KAI::UpdateActive()
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

	{
		//入力処理　適当に

        //m_AxisSpriteIndex = (8 - m_CurrentAxisEx + 1) % 8;
        m_AxisSpriteIndex = (m_CurrentAxisEx - 1) % 8;
        //DebugPrintf("%d\n", m_AxisSpriteIndex);

		if (!m_CharaData->playingMotion.enable)
		{
			//Character::PlayMotionCharacterGameObject(
			//	CharaDat.charaData,
			//	Character::GetCharacterMotionFindByName(Character::GetCharacterMainData(CharaDat.charaData->dataIndex), "IDLE"),
			//	false
			//);
		}

        //攻撃時の残像を生成
        if (m_IsNormalAttack && m_Obj->moveRequ.enable)
        {
            if (m_CharaData->playingMotion.frame == ATTACK_START_FRAME)
            {
                //無敵時間
                if (m_Obj->status.invFrame < ATTACK_TOTAL_FRAME)
                {
                    m_Obj->status.invFrame = ATTACK_TOTAL_FRAME;
                }
            }

            if (m_CharaData->playingMotion.frame >= ATTACK_START_FRAME
                && m_CharaData->playingMotion.frame < ATTACK_TOTAL_FRAME - ATTACK_FTH_FRAME * 0.85//0.78
                && m_CharaData->playingMotion.frame % 4 == 0
            )
            {
                //GAME_OBJECT* go = GO_CreateAfterEffect(m_Obj, 30, MkF4(0.8f, 0.75f, 1.0f, 0.75f));
                GAME_OBJECT* go = GO_CreateAfterEffect(m_Obj, 30, MkF4(0.8f, 1.25f, 1.5f, 0.6f));
            }
        }

        //歩きの煙
        if (m_WalkFrame > 0 &&  m_WalkFrame % (WALK_FRAME / 2) == 0)
        {
            {
                Float3 efPos2 = m_Obj->pos;
                efPos2.z += BLOCK_HIGHT * -0.5f;

                GAME_OBJECT* go = GO_CreateEffect(GO_EFFECT_TYPE_SMOKE, efPos2);
                if (go != nullptr)
                {
                    //go->graphic.rot3d.x = 90.0f;
                    go->graphic.color.w = 1.0f;
                    go->scale = MkF3(
                        0.8f,
                        0.8f,
                        0.8f
                    );
                    GameObjectSetLifeTime(go, 20);
                }
            }
        }
        if (m_IsDrawing)
        {
            if (m_WalkFrame > 0 && m_WalkFrame % 9 == 0)
            {
                GAME_OBJECT* go = GO_CreateAfterEffect(m_Obj, 30, MkF4(1.0f, 1.0f, 1.0f, 0.7f));
            }
        }
        //if (m_WalkFrame > 0 && m_WalkFrame % (WALK_FRAME / 4) == 0)
        //{
        //    SoundEffectPlayRequest(
        //        GetSoundData("SOUND/SE/player/move/Kaito_Walk"),
        //        1.8f, 0.0f);
        //}

		//待機状態なら操作を受け付ける
		{
            bool isNu = !m_CharaData->playingMotion.enable || m_IsIdling || m_IsWalkAnim;
            if (isNu)
			{ //ループアニメーション更新
				std::string animName = "IDLE";

                int* animp = nullptr;
                int maxf = -1;

                if (m_IsDrawing)
                {
                    animName = "DRAW";
                    animp = &m_WalkFrame;
                    maxf = WALK_FRAME;
                    if ((m_CharaData->playingMotion.motion->name) == animName)
                    {
                        m_Obj->graphic.graph.scale.x = 1.0f;
                        m_Obj->graphic.graph.scale.y = 1.0f;
                        m_Obj->graphic.pos3d = m_GraphPosDefault;
                        GraphicSpriteSet(m_Tex_Main_Draw[std::max(m_CurrentAxisEx - 1, 0)], 4, 3, 8);
                    }
                }
                else if (m_IsWalking)
                {
                    animName = "WALK";
                    animp = &m_WalkFrame;
                    maxf = WALK_FRAME;
                    if ((m_CharaData->playingMotion.motion->name) == animName)
                    {
                        m_Obj->graphic.graph.scale.x = 1.0f;
                        m_Obj->graphic.graph.scale.y = 1.0f;
                        m_Obj->graphic.pos3d = m_GraphPosDefault;
                        GraphicSpriteSet(m_Tex_Main_Walk[std::max(m_CurrentAxisEx - 1, 0)], 4, 3, 10);
                    }
                }
                else
                {
                    animp = &m_AxisSpriteIndex;
                    if (m_CharaData->playingMotion.enable 
                        && (m_CharaData->playingMotion.motion->name) == animName)
                    {
                        m_Obj->graphic.graph.scale.x = 1.0f;
                        m_Obj->graphic.graph.scale.y = 1.0f;
                        m_Obj->graphic.pos3d = m_GraphPosDefault;
                        GraphicSpriteSet(m_Tex_Main_Idle, 3, 3, 8);
                    }
                }

				if (!m_CharaData->playingMotion.enable || (m_CharaData->playingMotion.motion->name) != animName)
				{
					Character::PlayMotionCharacterGameObject(
						m_CharaData,
						Character::GetCharacterMotionFindByName(
                            Character::GetCharacterMainData(m_CharaData->dataIndex),
                            animName),
						true,
                        animp,
                        maxf
					);
				}
			}

			if (!m_IsDrawing)
			{ //描いている時ではない

				//通常攻撃
				{
					if (m_AttackCooldown <= 0) //クールタイム中ではない
					{

						//入力をとる
						if (isNu && IsInput_NormalAttack())
						{
                            SetFlagTtrlAttack();

                            CameraShakeRequestAll(
                                CAMERA_SHAKE_MIDIUM_FRAME,
                                CAMERA_SHAKE_MIDIUM_POWER * 1.5f
                            );

							Character::MOTION_COL_DAT motionColDetails =
								Character::GetMotionColDataFindByName(
									Character::MOTION_COL_TYPE_ATK,
									N_ATTACK_TEST_ATK_COL_NAME
								);

							GAME_OBJECT_COLLISION_ATTACK_CHILD attackColTmp = motionColDetails.atkCol;
                            strcpy_s(attackColTmp.tag, GAME_OBJECT_TXT_MAX, N_ATTACK_GENERAL_TAG);

                            //attackColTmp.subjectFamily[GAME_OBJECT_FAMILY_ALL] = true;
                            attackColTmp.subjectFamily[GAME_OBJECT_FAMILY_ENEMY] = true;
                            attackColTmp.subjectFamily[GAME_OBJECT_FAMILY_GIMIK] = true;

							//attackColTmp.size = Float2ToFloat3(m_Obj->size);
       //                     attackColTmp.size.x *= 1.8f;
       //                     attackColTmp.size.y *= 1.8f;
							//attackColTmp.size.z *= 1.0f;
                            attackColTmp.size.x = 1200.0f * m_Obj->scale.x;
                            attackColTmp.size.y = 1200.0f * m_Obj->scale.y;
                            attackColTmp.size.z = 500.0f * m_Obj->scale.z;
							attackColTmp.pos.z = -attackColTmp.size.z / 2;

                            Float3 bvecn = NormalizeVector(m_BeforeVecBuf);
                            attackColTmp.pos.x += 540.0f / 4 * bvecn.x + 0.0f * bvecn.x;
                            attackColTmp.pos.y += 540.0f / 4 * bvecn.y + 0.0f * bvecn.y;
                            
							//攻撃判定を生成
							GameObjectAttackCollisionAdd(m_Obj, attackColTmp,
								ATTACK_START_FRAME + 1,
								ATTACK_FRAME - 1
							);

							//モーション再生
                            GraphicSpriteSet(
                                m_Tex_Main_Attack[
                                    (int)round((CalculateRadToDeg(atan2(bvecn.y, bvecn.x)) - 90.0f + 360.0f) / (360.0f / 8)) % 8
                                ],
                                3, 2);
                            //GraphicSpriteSet(m_Tex_Main_Attack[std::max(m_CurrentAxisEx - 1, 0)], 3, 2);
                            m_Obj->graphic.graph.scale.x = 1.5f;
                            m_Obj->graphic.graph.scale.y = 1.5f;
                            m_Obj->graphic.pos3d = m_GraphPosDefault;
                            m_Obj->graphic.pos3d.y += -60.0f;
                            m_Obj->graphic.pos3d.z += -20.0f;

							Character::PlayMotionCharacterGameObject(
								m_CharaData,
								Character::GetCharacterMotionFindByName(
                                    Character::GetCharacterMainData(m_CharaData->dataIndex),
                                    "ATTACK"),
								false,
								nullptr,
								ATTACK_TOTAL_FRAME
							);

                            //if (m_IsWalking)
                            {
                                SoundEffectPlayRequest(
                                    GetSoundData("SOUND/SE/player/attack/Normal_Attack"),
                                    0.8f, 0.0f, 0.75f);
                                //Float3 dashVec = NormalizeVector(attackColTmp.pos);
                                Float2 dashVec = NormalizeVector(MkF2(attackColTmp.pos.x, attackColTmp.pos.y));
                                GameObjectMoveRequest(m_Obj, ATTACK_TOTAL_FRAME,
                                    MkF3(
                                        m_Obj->pos.x + dashVec.x * ONE_BLOCK * 10.0f,
                                        m_Obj->pos.y + dashVec.y * ONE_BLOCK * 10.0f,
                                        m_Obj->pos.z
                                    ),
                                    EasingInOutSine
                                );

                                //クールタイム設定
                                m_AttackCooldown = ATTACK_COOLDOWN_FRAME;
                            }
                            //else
                            //{
                            //    SoundEffectPlayRequest(
                            //        GetSoundData("SOUND/SE/player/attack/Normal_Attack"),
                            //        0.8f, 0.0f, 1.0f);
                            //    //クールタイム設定
                            //    m_AttackCooldown = (int)(ATTACK_COOLDOWN_FRAME);
                            //}
						}
					}
				}
			}

			//かこポン
			{
                //反応するキー
                bool keyPushMain = isNu && IsInput_KakoMain();
                bool keyPushSubTest = isNu &&
                    m_SkillCooldownFrame <= 0 && IsInput_KakoSub() ;

                //更新
                for (int i = 0; i < KAKOPON_DAT_NUM; i++)
                {
                    //if (i == 1 && GameManagerIsMovieFlag())
                    //{
                    //    continue;
                    //}
                    update_kakoponDat(&m_KakoDatList[i]);
                }
                //update_kakoponDat(&m_KakoDat_Main);
                //update_kakoponDat(&m_KakoDat_SubTest);


                { //プレイヤーの操作による処理

                    //for (int i = 0; i < KAKOPON_DAT_NUM; i++)
                    //{
                    //    update_kakopon_action(keyPushMain, &m_KakoDatList[i].dat, &m_KakoDatList[i].lineDraw);
                    //}
                    //update_kakopon_action(keyPushMain, &m_KakoDat_Main, &m_KakoponLineDraw_Main);
                    //update_kakopon_action(keyPushSubTest, &m_KakoDat_SubTest, &m_KakoponLineDraw_SubTest);
                    update_kakopon_action(keyPushMain, &m_KakoDatList[0].dat, &m_KakoDatList[0].lineDraw);
                    update_kakopon_action(keyPushSubTest, &m_KakoDatList[1].dat, &m_KakoDatList[1].lineDraw);

                    //操作キャンセル処理

                    bool cancel_All = true;
                    for (int i = 0; i < KAKOPON_DAT_NUM; i++)
                    {
                        if (!m_KakoDatList[i].lineDraw.vertexDrawFlag 
                            && !m_KakoDatList[i].dat.isConnected && m_KakoDatList[i].dat.vertexNum > 0
                        )
                        {
                            init_kakoponDat(&m_KakoDatList[i]);
                            //キャンセル時にゲージを戻す
                            m_InkGage = m_InkGage_DrawBefore;
                        }
                        else
                        {
                            cancel_All = false;
                        }
                    }
                    //bool cancel_Main = !m_KakoponLineDraw_Main.vertexDrawFlag && !m_KakoDat_Main.isConnected;
                    //if (cancel_Main)
                    //{
                    //    init_kakoponDat(&m_KakoDat_Main);
                    //}

                    //bool cancel_SubTest = !m_KakoponLineDraw_SubTest.vertexDrawFlag && !m_KakoDat_SubTest.isConnected;
                    //if (cancel_SubTest)
                    //{
                    //    init_kakoponDat(&m_KakoDat_SubTest);
                    //}

                    ////キャンセル時にゲージを戻す
                    //if (cancel_All)
                    //{
                    //    m_InkGage = m_InkGage_DrawBefore;
                    //}
                }

                //囲った時の判定
                if (m_KakoDatList[0].dat.objs.size() > 0
                    && !m_KakoDatList[0].dat.isFirstObjectCheck)
                {
                    m_KakoDatList[0].dat.isFirstObjectCheck = true;

                    //攻撃
                    kakopon_attack_setBuf(&m_KakoDatList[0].dat);
                    kakopon_objects_attack(&m_KakoDatList[0].dat);

                    //CameraZoomRequest(
                    //    player->index,
                    //    0.03f,
                    //    m_KakoDatList[0].dat.connectCentroid,
                    //    EasingInOutSine,
                    //    0.5f
                    //);

                    //SEを入れる
                    //SoundEffectPlayRequest(SOUND_TYPE_SE_3, 4.0f, 0.0f);

                    if (m_KakoDatList[0].dat.IsSuccess)
                    {
                        SoundEffectPlayRequest(
                            GetSoundData("SOUND/SE/player/attack/activation"),
                            3.0f, 0.0f);
                    }

                    //constexpr int stopframe = 60;
                    ////GameStopRequest(stopframe);

                    //CameraZoomResetRequest(
                    //    player->index,
                    //    0.02f,
                    //    EasingInOutSine,
                    //    stopframe
                    //);

                    //g_LineDraw.afterTimerFrame += stopframe;
                }

                //囲った時の判定テスト
                if (m_KakoDatList[1].dat.isConnected)
                    //if (m_KakoDatList[1].dat.objs.size() > 0)
                {
                    if (!m_KakoDatList[1].dat.isFirstObjectCheck)
                    {
                        SetFlagTtrlSkill();

                        m_KakoDatList[1].dat.isFirstObjectCheck = true;
                        SoundEffectPlayRequest(
                            GetSoundData("SOUND/SE/player/attack/ready1"),
                            2.8f, 0.0f, 1.0f);
                    }
                    //SetSkillCooldownFrame(m_KakoDatList[1].dat.frameMax
                    //    + FRAME_ONE_SECOND * 5
                    //);

                    SetSkillCooldownFrame(FRAME_ONE_SECOND * 5);

                    //m_KakoDat_SubTest.isFirstObjectCheck = true;
                    m_KakoDatList[1].dat.IsSuccess = true;

                    //オブジェクトを検証
                    for (int i = 0; i < m_KakoDatList[1].dat.objs.size(); i++)
                    {
                        GAME_OBJECT* ob = GetGameObjectFindByIdentifier(m_KakoDatList[1].dat.objs[i]);

                        if (
                            ob != nullptr
                            && IsBelongsFamilyGameObject(ob, GAME_OBJECT_FAMILY_PLAYER)
                            )
                        {
                            m_BuffInObj[ob] = true;
                        }
                    }
                }

                if (m_BuffInObj.size() > 0)
                {
                    if (m_KakoDatList[1].dat.isConnected)
                    {
                        float buffmod = BUFF_MOD;
                        float buffspdmod = BUFF_SPD_MOD;
                        float buffincmod = BUFF_INC_MOD;
                        if (GetActivePlayerNum() <= 1)
                        {
                            buffmod = BUFF_MOD_SOLO;
                            buffspdmod = BUFF_SPD_MOD_SOLO;
                            buffincmod = BUFF_INC_MOD_SOLO;
                        }

                        //DebugPrintf("%f\n", buffmod);

                        for (const auto& ob : m_BuffInObj)
                        {
                            if (ob.second && IsActiveGameObject(ob.first))
                            {
                                {
                                    GOE_Character_Player* goe =
                                        dynamic_cast<GOE_Character_Player*>(ob.first->events.source);  // ダウンキャスト

                                    if (goe != nullptr)
                                    { //
                                        goe->BuffAttackFlag(buffmod);
                                        goe->BuffSpeedFlag(buffspdmod);
                                        goe->BuffInkCostFlag(buffincmod);
                                    }
                                }
                            }
                        }
                    }
                    else
                    {
                        m_BuffInObj.clear();
                    }
                }
			}
		}

	}

    //SE
    if (m_IsDrawing &&
        m_InkGage > 0 &&
        //m_WalkFrame % 3 == 0
        m_Obj->frameActive % 16 == 0
        )
    {
        //SoundEffectPlayRequest(
        //    GetSoundData("SOUND/SE/player/attack/ready_single"),
        //    3.0f, 0.0f);
        SoundEffectPlayRequest(
            GetSoundData("SOUND/SE/player/attack/ready2"),
            3.0f, 0.0f);
    }

    kakopon_final_update();

    UpdateActiveLate();
}

void GOE_Character_Player_KAI::UpdateActiveLate()
{
    SuperGOE::UpdateActiveLate();
}


// ===================================================
// Draw
// オブジェクトの描画イベント
// ===================================================
void GOE_Character_Player_KAI::Draw()
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

    //通常攻撃エフェクト
    if (m_IsNormalAttack)
    { 
        {
            {
                float rott = m_AxisSpriteIndex * 360.0f / 8 - 90.0f;

                //調整用
                //if (m_CurrentAxis == AXIS_2D_LEFT) rott += -50.0f;
                //if (m_CurrentAxis == AXIS_2D_RIGHT) rott += 60.0f;
                //if (m_CurrentAxis == AXIS_2D_UP) rott += -100.0f;

                DRAW_SPRITE_DAT dsdat = {};
                dsdat.x = 0.0f;
                dsdat.y = 540.0f * 0.02f;

                dsdat.w = 540.0f * 2.0f;
                dsdat.h = 540.0f * 2.0f * 1.f;

                dsdat.rot = 0.0f;
                //dsdat.color = { 0.2f, 0.2f, 1.0f, 1.0f };
                dsdat.color = { 0.6f, 0.3f, 1.0f, 1.0f * m_MovieVisibleAlpha };

                dsdat.texNo = m_TexId_AttackEffect_s;
                dsdat.loadTexType = LOADTEXTURETYPE_MAIN;

                //dsdat.texUV = { 0.0f, 0.0f };
                //dsdat.texWH = { 1.0f, 1.0f };
                dsdat.texUV = AnimationUV(
                    (int)(
                        m_AnimCount_AttackEffect_s.x * m_AnimCount_AttackEffect_s.y
                        * ((float)m_CharaData->playingMotion.frame / ATTACK_TOTAL_FRAME)
                    ),
                    (int)m_AnimCount_AttackEffect_s.x,
                    (int)m_AnimCount_AttackEffect_s.y
                );
                dsdat.texWH = { 1.0f / m_AnimCount_AttackEffect_s.x, 1.0f / m_AnimCount_AttackEffect_s.y };

                dsdat.startPos3D = m_Obj->pos;
                dsdat.startPos3D.z += -dsdat.h * 0.12f;

                dsdat.startRot = rott;
                dsdat.startScale = { 1.0f, 1.0f };

                dsdat.shaderOpt.blendState = DPD_SHADER_OPT_BLEND_STATE_ADDCOLOR;

                //dsdat.shaderOpt.bloom = true;

                DrawSpriteInCamera(dsdat);
            }
        }
    }


}

// ===================================================
// Dead
// オブジェクトのHPが0になったとき
// ===================================================
void GOE_Character_Player_KAI::Dead()
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

    SetDownPoseStart();
    SetDownPose();
}

// ===================================================
// DamageAttacker
// オブジェクトの攻撃判定がヒットしたとき
// ===================================================
void GOE_Character_Player_KAI::DamageAttacker(GAME_OBJECT_DAMAGE_RESULT dr)
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
        if (std::string(dr.colAtk.tag) == N_ATTACK_GENERAL_TAG)
        {
            //SEを再生
            SoundEffectPlayRequest(
                GetSoundData("SOUND/SE/player/attack/atkhit_mid"),
                0.6f);
        }
	}
}

// ===================================================
// DamageReciever
// オブジェクトが
// 別のオブジェクトの攻撃判定にヒットしたとき
// ===================================================
void GOE_Character_Player_KAI::DamageReciever(GAME_OBJECT_DAMAGE_RESULT dr)
{
	SuperGOE::DamageReciever(dr);

	//if (dr.damage > 0 || dr.colAtk.isGrab)
	//{

	//}
}

// ===================================================
// BlockHit
// オブジェクトがブロックに触れたとき
// ===================================================
void GOE_Character_Player_KAI::BlockHit(GAME_OBJECT_POSMOD_RESULT pmr)
{
	SuperGOE::BlockHit(pmr);
}

void GOE_Character_Player_KAI::SetDownPoseStart()
{
    m_DefeatFrameAnimMax = 6;
}

void GOE_Character_Player_KAI::SetDownPose()
{
    //モーション再生
    GraphicSpriteSet(
        m_Tex_Main_Down,
        3, 2);
    //m_Obj->graphic.graph.scale.x = 1.5f;
    //m_Obj->graphic.graph.scale.y = 1.5f;
    //m_Obj->graphic.pos3d = m_GraphPosDefault;
    //m_Obj->graphic.pos3d.y += -60.0f;
    //m_Obj->graphic.pos3d.z += -20.0f;
    m_Obj->graphic.pos3d = m_GraphPosDefault;
    m_Obj->graphic.pos3d.y += 180.0f;
    m_Obj->graphic.pos3d.z += -10.0f;
    m_Obj->graphic.graph.scale.x = 1.0f;
    m_Obj->graphic.graph.scale.y = 1.0f;

    Character::PlayMotionCharacterGameObject(
        m_CharaData,
        Character::GetCharacterMotionFindByName(
            Character::GetCharacterMainData(m_CharaData->dataIndex),
            "DOWN"),
        true,
        &m_DefeatFrameAnim,
        -1
    );
}