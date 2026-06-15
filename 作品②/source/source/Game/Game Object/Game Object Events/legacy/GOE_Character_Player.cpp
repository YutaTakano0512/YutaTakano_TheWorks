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
#include "../../../Game/Entities/Block/block.h"
#include "../../Systems/collision.h"
#include "../../Systems/easing.h"

#include "../../Managers/gameManager.h"
#include "../../Managers/scene.h"

#include "../gameObject.h"
#include "../gameObjectPrefab.h"

#include "../../Systems/camera.h"
#include "../../Systems/ui.h"
#include "../../Systems/ui_Game.h"

#include "../../Entities/player.h"


#include "../../Managers/soundManager.h"

#include "GOE_General.h"
#include "GOE_Character.h"

#include "GOE_Character_Player.h"

// ===================================================
// マクロ定義
// ===================================================

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

    GraphicSet("TEX/player/character_test", 6, 8);

    //必要なテクスチャを読み込み
    m_TexId_AttackEffect_s = LoadTexData("TEX/effect/tex_sl_back").textureId;
    m_TexId_kakoponEffect_Aura = LoadTexData("TEX/effect/kakopon/tex_aura_01").textureId;
    m_TexId_kakoponEffect_Add = LoadTexData("TEX/effect/kakopon/tex_add_01").textureId;
    m_TexId_kakoponEffect_Trail = LoadTexData("TEX/effect/kakopon/tex_trail_01").textureId;

    m_TexId_kakoponEffect_Add_Heal = LoadTexData("TEX/effect/kakopon/tex_tile_heal").textureId;

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

		//return;
	}
    //最初のフレーム(初期化とか)

	//初期アニメーションを再生
	Character::PlayMotionCharacterGameObject(
		m_CharaData,
		Character::GetCharacterMotionFindByName(Character::GetCharacterMainData(m_CharaData->dataIndex), "IDLE"),
		true
	);

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
    UnloadTexData(m_TexId_kakoponEffect_Trail);


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
    m_IsWalkAnim = (m_CharaData->playingMotion.motion->name).find("WALK") == 0;
    m_IsNormalAttack = (m_CharaData->playingMotion.motion->name).find("ATTACK") == 0;

    m_IsWalking = m_WalkFrame > 0;



#if defined(_DEBUG) || defined(DEBUG)
    //デバッグ エネミー全ダメージ
    if (GetAsyncKeyState(VK_SHIFT)
        && Keyboard_IsKeyTrigger(KK_K))
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
                ob->status.hp = 0;
            }
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

	{
		//入力処理　適当に

		//待機
		if (!m_CharaData->playingMotion.enable)
		{
		}

        //方向名
		std::string axisAnimName[AXIS_2D_MAX] = {};
		{
			axisAnimName[AXIS_2D_N] = "";
			axisAnimName[AXIS_2D_LEFT] = "LEFT";
			axisAnimName[AXIS_2D_RIGHT] = "RIGHT";
			axisAnimName[AXIS_2D_UP] = "UP";
			axisAnimName[AXIS_2D_DOWN] = "DOWN";
		}

		{ //方向処理
			//m_CurrentAxis = AXIS_2D_N;
			m_CurrentAxis = AXIS_2D_DOWN;
            m_AxisVec = MkF2(0.0f, 1.0f);

			if (m_BeforeVec.x < 0 && abs(m_BeforeVec.y) < 1.0f)
			{
				m_CurrentAxis = AXIS_2D_LEFT;
                m_AxisVec = MkF2( -1.0f, 0.0f );
			}
			if (m_BeforeVec.x > 0 && abs(m_BeforeVec.y) < 1.0f)
			{
				m_CurrentAxis = AXIS_2D_RIGHT;
                m_AxisVec = MkF2(1.0f, 0.0f);
			}
			if (m_BeforeVec.y < 0 && abs(m_BeforeVec.x) < 1.0f)
			{
				m_CurrentAxis = AXIS_2D_UP;
                m_AxisVec = MkF2(0.0f, -1.0f);
			}
			//if (m_BeforeVec.y > 0 && abs(m_BeforeVec.x) < 1.0f)
			//{
			//	m_CurrentAxis = AXIS_2D_DOWN;
			//}

		}

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
            if (m_CharaData->playingMotion.frame >= ATTACK_START_FRAME
                && m_CharaData->playingMotion.frame < ATTACK_TOTAL_FRAME - ATTACK_FTH_FRAME * 0.78
                && m_CharaData->playingMotion.frame % 2 == 0
            )
            {
                GAME_OBJECT* go = GO_CreateAfterEffect(m_Obj, 30, MkF4(0.8f, 0.75f, 1.0f, 0.75f));
            }
        }

		//待機状態なら操作を受け付ける
		if (!m_CharaData->playingMotion.enable || m_IsIdling || m_IsWalkAnim)
		{
			{ //歩きの更新
				std::string animName = "IDLE";

				if (axisAnimName[m_CurrentAxis].size() > 0)
				{
					animName = "WALK_" + axisAnimName[m_CurrentAxis];
				}

				if (m_IsDrawing)
				{
					animName += "_DRAW";
				}

				if ((m_CharaData->playingMotion.motion->name) != animName)
				{
					Character::PlayMotionCharacterGameObject(
						m_CharaData,
						Character::GetCharacterMotionFindByName(Character::GetCharacterMainData(m_CharaData->dataIndex), animName),
						true,
						&m_WalkFrame,
						GOE_Character_Player::WALK_FRAME
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
						if (Character::GetCtrlTrigger(m_CharaData, CONTROLLER_BUTTON_TYPE_A))
						{
							Character::MOTION_COL_DAT motionColDetails =
								Character::GetMotionColDataFindByName(
									Character::MOTION_COL_TYPE_ATK,
									GOE_Character_Player::N_ATTACK_TEST_ATK_COL_NAME
								);

							GAME_OBJECT_COLLISION_ATTACK_CHILD attackColTmp = motionColDetails.atkCol;

                            //attackColTmp.subjectFamily[GAME_OBJECT_FAMILY_ALL] = true;
                            attackColTmp.subjectFamily[GAME_OBJECT_FAMILY_ENEMY] = true;

							attackColTmp.size = Float2ToFloat3(m_Obj->size);
							attackColTmp.size.z = (float)sqrt(pow(attackColTmp.size.x, 2) + pow(attackColTmp.size.y, 2));
							attackColTmp.pos.z = -attackColTmp.size.z / 2;

                            attackColTmp.pos.x += m_Obj->size.x / 2 * m_AxisVec.x;
                            attackColTmp.pos.y += m_Obj->size.y / 2 * m_AxisVec.y;
                            
							//攻撃判定を生成
							GameObjectAttackCollisionAdd(m_Obj, attackColTmp,
								GOE_Character_Player::ATTACK_START_FRAME,
								GOE_Character_Player::ATTACK_FRAME
							);

							//モーション再生
							Character::PlayMotionCharacterGameObject(
								m_CharaData,
								Character::GetCharacterMotionFindByName(Character::GetCharacterMainData(m_CharaData->dataIndex), "ATTACK_" + axisAnimName[m_CurrentAxis]),
								false,
								nullptr,
								GOE_Character_Player::ATTACK_TOTAL_FRAME
							);

                            if (m_IsWalking)
                            {
                                //無敵時間
                                m_Obj->status.invFrame = GOE_Character_Player::ATTACK_TOTAL_FRAME;

                                Float3 dashVec = NormalizeVector(attackColTmp.pos);
                                GameObjectMoveRequest(m_Obj, GOE_Character_Player::ATTACK_TOTAL_FRAME,
                                    MkF3(
                                        m_Obj->pos.x + dashVec.x * ONE_BLOCK * 16.0f,
                                        m_Obj->pos.y + dashVec.y * ONE_BLOCK * 16.0f,
                                        m_Obj->pos.z
                                    ),
                                    EasingInOutBack
                                );

                                //クールタイム設定
                                m_AttackCooldown = GOE_Character_Player::ATTACK_COOLDOWN_FRAME;
                            }
                            else
                            {
                                //クールタイム設定
                                m_AttackCooldown = (int)(GOE_Character_Player::ATTACK_COOLDOWN_FRAME * 0.4f);
                            }
						}
					}
				}
			}

			//移動
			{
				Float3 moveVel = {};

				Float2 ls = Character::GetCtrlStickLeft(m_CharaData);
				if (ls.x != 0 || ls.y != 0)
				{ //スティック移動
					//obj->vel.x += PLAYER_MOVE_AXCEL * ls.x;
					moveVel = MkF3(ls.x, -ls.y, 0.0f);

					//if (ls.x < 0)
					//{
					//	CharaDat.charaData->reverse = true;
					//}
					//else
					//{
					//	CharaDat.charaData->reverse = false;
					//}

					if (m_Obj->frameActive % 60 == 0)
					{
						SoundEffectPlayRequest(SOUND_TYPE_SE_1, 2.5f, 0.0f);
					}
				}
				else
				{ //十字移動

					Float2 moveVelJuuji = {};

					if (Character::GetCtrlPress(m_CharaData, CONTROLLER_BUTTON_TYPE_LEFT))
					{
						if (m_Obj->frameActive % 60 == 0)
						{
							SoundEffectPlayRequest(SOUND_TYPE_SE_1, 3.5f, 0.0f);
						}

						//CharaDat.charaData->reverse = true;
						moveVelJuuji.x = -1.0f;
					}
					if (Character::GetCtrlPress(m_CharaData, CONTROLLER_BUTTON_TYPE_RIGHT))
					{
						if (m_Obj->frameActive % 60 == 0)
						{
							SoundEffectPlayRequest(SOUND_TYPE_SE_1, 3.5f, 0.0f);
						}

						//CharaDat.charaData->reverse = false;
						moveVelJuuji.x = 1.0f;
					}
					if (Character::GetCtrlPress(m_CharaData, CONTROLLER_BUTTON_TYPE_UP))
					{
						if (m_Obj->frameActive % 60 == 0)
						{
							SoundEffectPlayRequest(SOUND_TYPE_SE_1, 3.5f, 0.0f);
						}

						moveVelJuuji.y = -1.0f;
					}
					if (Character::GetCtrlPress(m_CharaData, CONTROLLER_BUTTON_TYPE_DOWN))
					{
						if (m_Obj->frameActive % 60 == 0)
						{
							SoundEffectPlayRequest(SOUND_TYPE_SE_1, 3.5f, 0.0f);
						}

						moveVelJuuji.y = 1.0f;
					}

					moveVel = MkF3(moveVelJuuji.x, moveVelJuuji.y, 0.0f);
				}
				moveVel = NormalizeVector(moveVel);
				moveVel.x *= PLAYER_MOVE_AXCEL;
				moveVel.y *= PLAYER_MOVE_AXCEL;
                if (m_IsDrawing)
                {
                    moveVel.x *= KAKOPON_WALK_SPEED_MOD;
                    moveVel.y *= KAKOPON_WALK_SPEED_MOD;
                }

				m_Obj->vel.x += moveVel.x;
				m_Obj->vel.y += moveVel.y;

				if (moveVel.x != 0 || moveVel.y != 0)
				{
					m_BeforeVec = NormalizeVector(moveVel);

					m_WalkFrame++;
				}
				else
				{
					m_WalkFrame = 0;
				}

			}

			//かこポン
			{
                //反応するキー
                bool keyPushMain =
                    Character::GetCtrlPress(m_CharaData, CONTROLLER_BUTTON_TYPE_R)
                    || Character::GetCtrlPress(m_CharaData, CONTROLLER_BUTTON_TYPE_B)
                    ;
                bool keyPushSubTest = !keyPushMain && Character::GetCtrlPress(m_CharaData, CONTROLLER_BUTTON_TYPE_Y)
                    ;

                //適当
                //m_KakoDat_Main.graphic = KAKOPON_GRAPHIC::Main;
                //m_KakoDat_Main.type = KAKOPON_TYPE::Main;
                //m_KakoDat_Main.frameMax = GOE_Character_Player::KAKOPON_COOLDOWN_FRAME;

                //m_KakoDat_SubTest.graphic = KAKOPON_GRAPHIC::Main;
                //m_KakoDat_SubTest.type = KAKOPON_TYPE::Heal;
                //m_KakoDat_SubTest.frameMax = FRAME_ONE_SECOND * 5;
                m_KakoDatList[0].dat.graphic = KAKOPON_GRAPHIC::Main;
                m_KakoDatList[0].dat.type = KAKOPON_TYPE::Main;
                m_KakoDatList[0].dat.frameMax = GOE_Character_Player::KAKOPON_COOLDOWN_FRAME;

                m_KakoDatList[1].dat.graphic = KAKOPON_GRAPHIC::Main;
                m_KakoDatList[1].dat.type = KAKOPON_TYPE::Heal;
                m_KakoDatList[1].dat.frameMax = FRAME_ONE_SECOND * 5;

                //更新
                for (int i = 0; i < KAKOPON_DAT_NUM; i++)
                {
                    update_kakoponDat(&m_KakoDatList[i].dat);
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
                            init_kakoponDat(&m_KakoDatList[i].dat);
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
                if (m_KakoDatList[0].dat.objs.size() > 0 && !m_KakoDatList[0].dat.isFirstObjectCheck)
                {
                    m_KakoDatList[0].dat.isFirstObjectCheck = true;

                    //攻撃
                    kakopon_objects_attack(m_KakoDatList[0].dat);

                    //CameraZoomRequest(
                    //    player->index,
                    //    0.03f,
                    //    m_KakoDatList[0].dat.connectCentroid,
                    //    EasingInOutSine,
                    //    0.5f
                    //);

                    //SEを入れる
                    SoundEffectPlayRequest(SOUND_TYPE_SE_3, 4.0f, 0.0f);

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
                if (m_KakoDatList[1].dat.objs.size() > 0 && !m_KakoDatList[1].dat.isFirstObjectCheck)
                {
                    //m_KakoDat_SubTest.isFirstObjectCheck = true;

                    //オブジェクトを検証
                    for (int i = 0; i < m_KakoDatList[1].dat.objs.size(); i++)
                    {
                        GAME_OBJECT* ob = GetGameObjectFindByIdentifier(m_KakoDatList[1].dat.objs[i]);

                        if (
                            ob != nullptr
                            && IsBelongsFamilyGameObject(ob, GAME_OBJECT_FAMILY_PLAYER)
                            )
                        {
                            if (ob->frame % FRAME_ONE_SECOND == 0)
                            {
                                ob->status.hp += (int)(ob->status.hpMax * 0.1);
                                if (ob->status.hp > ob->status.hpMax)
                                {
                                    ob->status.hp = ob->status.hpMax;
                                }
                            }
                            //ob->status.hp += 1;
                        }
                    }
                }

                
			}
		}

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
			if (m_AttackCooldown > 0)
			{
				m_AttackCooldown--;
			}
		}
	}

	//if (GetAsyncKeyState('P'))
	//{
	//	obj->pos.z -= 8.0f;
	//}

	//if (GetAsyncKeyState('L'))
	//{
	//	obj->pos.z += 8.0f;
	//}

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

    {
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
                1.0f * amod
            );
        }
        else
        {
            m_Obj->graphic.color = MkF4(
                1.0f,
                1.0f,
                1.0f,
                1.0f
            );
        }
    }


    //通常攻撃エフェクト
    if (m_IsNormalAttack)
    { 
        {
            {
                float rott = CalculateRadToDeg(atan2(m_AxisVec.y, m_AxisVec.x)) - 90.0f;

                //調整用
                //if (m_CurrentAxis == AXIS_2D_LEFT) rott += -50.0f;
                //if (m_CurrentAxis == AXIS_2D_RIGHT) rott += 60.0f;
                //if (m_CurrentAxis == AXIS_2D_UP) rott += -100.0f;

                DRAW_SPRITE_DAT dsdat = {};
                dsdat.x = 0.0f;
                dsdat.y = m_Obj->size.y * 0.02f;

                dsdat.w = m_Obj->size.x * 2.4f;
                dsdat.h = m_Obj->size.y * 2.4f * 1.f;

                dsdat.rot = 0.0f;
                dsdat.color = { 1.0f, 1.0f, 0.0f, 1.0f };

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

                dsdat.shaderOpt.bloom = true;

                DrawSpriteInCamera(dsdat);
            }
        }
    }


    for (int i = 0; i < KAKOPON_DAT_NUM; i++)
    {
        draw_kakoponDat(&m_KakoDatList[i].dat);
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

    //テスト
    if (player->index == 0)
    {
        SoundEffectPlayRequest(SOUND_TYPE_SE_9, 10.0f, 0.0f);

        GameStopRequest(60);
        GameManagerSetSceneRequest(SCENE_RESULT, 0, false);
        //SetSceneFade(SCENE_RESULT);

        CameraZoomRequest(
            player->index,
            0.04f,
            Float3ToFloat2(m_Obj->pos),
            EasingInOutSine,
            0.65f
        );
        CameraZoomResetRequest(
            player->index,
            0.02f,
            EasingInOutSine,
            60
        );
    }
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
            //インクゲージの回復
            if (m_InkGage < m_InkGageMax)
            {
                m_InkGage += KAKOPON_INK_GAGE_HEAL_ATTACK;
                m_InkGage = std::min(m_InkGage, m_InkGageMax);
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
            efPos.z += -m_Obj->size.y / 2;

            GAME_OBJECT* go = GO_CreateEffect(GO_EFFECT_TYPE_N_ATTACK_HIT, efPos);
            if (go != nullptr)
            {
                go->scale = MkF2(
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

	if (dr.damage > 0 || dr.colAtk.isGrab)
	{
		SoundEffectPlayRequest(SOUND_TYPE_SE_4, 4.0f, 0.0f);

        m_Obj->status.invFrame = GOE_Character_Player::DAMAGE_INV_FRAME;
	}
}

// ===================================================
// BlockHit
// オブジェクトがブロックに触れたとき
// ===================================================
void GOE_Character_Player::BlockHit(GAME_OBJECT_POSMOD_RESULT pmr)
{
	SuperGOE::BlockHit(pmr);
}


Float2 GOE_Character_Player::GetKakoponVertexNum()
{
    int n = 0;
    for (int i = 0; i < KAKOPON_VERTEX_MAX; i++)
    {
    	if (m_KakoDatList[0].dat.vertex[i].enable)
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

//
// kakopon
//

void GOE_Character_Player::kakopon_objects_attack(KAKOPON_DATA dat)
{
    Character::MOTION_COL_DAT motionColDetails =
        Character::GetMotionColDataFindByName(
            Character::MOTION_COL_TYPE_ATK,
            GOE_Character_Player::KAKOPON_GENERAL_ATK_COL_NAME
        );

    GAME_OBJECT_COLLISION_ATTACK_CHILD atkCol = motionColDetails.atkCol;
    //atkCol.power += 200 * ((dat.vertexNum * KAKOPON_INK_GAGE_COST) / KAKOPON_INK_GAGE_ONE);
    atkCol.power += 300 * ((m_InkGage_DrawBefore - m_InkGage) / KAKOPON_INK_GAGE_ONE);

    //オブジェクトを検証
    for (int i = 0; i < dat.objs.size(); i++)
    {
        GAME_OBJECT* ob = GetGameObjectFindByIdentifier(dat.objs[i]);

        if (
            ob!= nullptr 
            && IsBelongsFamilyGameObject(ob, GAME_OBJECT_FAMILY_ENEMY)
        )
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

                        float cDist = CalculateDistance(
                            dat.connectCentroid,
                            Float3ToFloat2(ob->pos)
                        );
                        //motionColDatInit();

                        GameObjectDamageCollisionHit(ob, m_Obj,
                            atkCol,
                            colRec
                        );

                        int addStopf = (int)(0.02f * cDist);
                        ob->hitStop.frameMax += addStopf;
                        ob->hitStop.frame += addStopf;
                    }
                }
            }

        }
    }
}

//かこポンデータ初期化
void GOE_Character_Player::init_kakoponDat(KAKOPON_DATA* dat)
{
    dat->frame = 0;

    dat->isFirstObjectCheck = false;
    dat->isConnected = false;

    dat->vertexNum = 0;
    for (int i = 0; i < KAKOPON_VERTEX_MAX; i++)
    {
        if (dat->vertex[i].enable)
        {
            dat->vertex[i].enable = false;
        }
    }

    dat->objs.clear();
    dat->objs.shrink_to_fit();
}

//かこポンデータ更新
void GOE_Character_Player::update_kakoponDat(KAKOPON_DATA* dat)
{
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
                        if (lineStartVertexTmp->enable && lineEndVertexTmp->enable)
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
            if (dat->vertex[i].enable)
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
            if (dat->vertex[i].enable
                && dat->vertex[i + 1].enable)
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

                if (lineStartVertexTmp->enable && lineEndVertexTmp->enable)
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

                    constexpr float connectSize = 100.0f;

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
                            abs(endLineRot - lineRot) <= 0.1f
                            && abs(endLineIndex - i) >= 3
                            && CalculateDistance(
                                CalculateCentroid(lineStartVertexTmp->pos, lineEndVertexTmp->pos),
                                endLineEndVertex->pos
                            ) < connectSize
                            /*				&& CheckLineBoxCollider(
                                                lineStartVertexTmp->pos, lineEndVertexTmp->pos,
                                                MkF2(
                                                    endLineEndVertex->pos.x,
                                                    endLineEndVertex->pos.y
                                                ),
                                                MkF2(
                                                    connectSize,
                                                    connectSize
                                                )
                                            )
                                            && (
                                                CalculateDistanceBetweenParallelLines(endLineNv, lineNv) < connectSize2
                                                || CalculateDistanceBetweenParallelLines(lineNv, endLineNv) < connectSize2
                                            )*/
                            )
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
                            //交差しているのに交点が計算できていない、
                            //つまり線が平行の場合
                            connectPoint = lineStartVertexTmp->pos;
                        }

                        lineStartVertexTmp->pos = connectPoint;
                        endLineEndVertex->pos = connectPoint;

                        //切れ端を削除
                        for (int j = 0; j < i; j++)
                        {
                            dat->vertex[j].enable = false;
                        }

                        {
                            int useVertexCount = 0;
                            for (int j = 0; j < KAKOPON_VERTEX_MAX; j++)
                            {
                                if (dat->vertex[j].enable)
                                {
                                    useVertexCount++;
                                }
                            }
                            dat->vertexNum = useVertexCount;

                            Float2* points = (Float2*)malloc(sizeof(Float2) * useVertexCount);

                            int index = 0;
                            for (int j = 0; j < KAKOPON_VERTEX_MAX; j++)
                            {
                                if (dat->vertex[j].enable)
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
                                if (dat->vertex[j].enable)
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
            init_kakoponDat(dat);
        }
    }
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
        linedraw->vertexDrawInputFlag = 0;
    }

    //入力を始めたらフラグを立てる
    if (linedraw->vertexDrawInputFlag == 1)
    {
        linedraw->vertexDrawFlag = true;
    }
    //入力を辞めたらフラグを折る
    if (linedraw->vertexDrawInputFlag == 0)
    {
        linedraw->vertexDrawFlag = false;
    }



    //線を描くのをキャンセル
    if (linedraw->afterTimerFrame > 0)
    { //後隙があったら消費する
        linedraw->afterTimerFrame--;
        if (linedraw->afterTimerFrame <= 0)
        {
            linedraw->vertexDrawFlag = false;
        }
    }
    else if (!linedraw->vertexDrawFlag)
    { //フラグが折れていたらキャンセル処理

        //ZeroMemory(&g_LineDraw, sizeof(g_LineDraw));

        linedraw->vertexDrawFlag = false;

        //linedraw->isVertexObjectCheck = false;

        linedraw->afterTimerFrame = 0;

        //dat->isConnected = false;

        //for (int i = 0; i < KAKOPON_VERTEX_MAX; i++)
        //{
        //    if (dat->vertex[i].enable)
        //    {
        //        dat->vertex[i].enable = false;
        //    }
        //}
    }

    //フラグがたっていたら頂点を設置していく
    if (linedraw->vertexDrawFlag)
    {
        //Float2 nPos = GetControllerMainTouchScreenPosition();
        //PLAYER* pl = GetPlayer(0); 
        //PLAYER* pl = player;
        //GAME_OBJECT* plObj = pl->obj;
        Float2 nPos = Float3ToFloat2(m_Obj->pos);

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
                    if (!dat->vertex[i].enable)
                    {
                        dat->vertex[i].enable = true;
                        dat->vertexNum++;

                        Float2 setPos = nPos;

                        //インクを消費
                        m_InkGage += -KAKOPON_INK_GAGE_COST;

                        //SEを入れる
                        if (dat->vertexNum % 4 == 0)
                        {
                            SoundEffectPlayRequest(SOUND_TYPE_SE_2, 3.0f, 0.0f);
                        }

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

void GOE_Character_Player::draw_kakoponDat(KAKOPON_DATA* dat)
{
    float beforeSizeX_effect0 = 0.0f;
    float beforeSizeX_effect1 = 0.0f;

    int frame = dat->frame;
    int revFrame = (dat->frameMax - dat->frame);

    for (int i = 0; i < KAKOPON_VERTEX_MAX; i++)
    {
        if (dat->vertex[i].enable)
        {

            //頂点の描画
            //{
            //	DRAW_SPRITE_DAT dsdat = {};
            //	dsdat.x = g_LineDraw.p_vertex[i].pos.x;
            //	dsdat.y = g_LineDraw.p_vertex[i].pos.y;
            //	dsdat.w = 6.0f;
            //	dsdat.h = 6.0f;
            //	dsdat.rot = 0.0f;
            //	dsdat.color = { 2.0f, 2.0f, 2.0f, 1.0f };
            //	if (g_LineDraw.p_vertex_connected)
            //	{
            //		dsdat.color = { 2.0f, 2.0f, 0.0f, 1.0f };
            //	}

            //	dsdat.texNo = 0;
            //	dsdat.loadTexType = LOADTEXTURETYPE_MAIN;

            //	dsdat.texUV = { 0.0f, 0.0f };
            //	dsdat.texWH = { 1.0f, 1.0f };
            //	dsdat.startPos = { 0.0f, 0.0f };
            //	dsdat.startRot = 0.0f;
            //	dsdat.startScale = { 1.0f, 1.0f };
            //	DrawSpriteInCamera(dsdat);
            //}

            if (i < KAKOPON_VERTEX_MAX - 1)
            {
                Float4 lineColor = { 2.0f, 2.0f, 2.0f, 2.0f };
                if (dat->vertex[i + 1].enable)
                {
                    float dist = CalculateDistance(
                        dat->vertex[i].pos,
                        dat->vertex[i + 1].pos
                    );
                    float rott = CalculateRadToDeg(atan2(
                        dat->vertex[i + 1].pos.y - dat->vertex[i].pos.y,
                        dat->vertex[i + 1].pos.x - dat->vertex[i].pos.x
                    ));


                    //DRAW_SPRITE_DAT dsdat = {};
                    //dsdat.x = dist / 2;
                    //dsdat.y = 0.0f;

                    ////dsdat.w = dist;
                    ////dsdat.h = 28.0f;
                    //// 
                    ////dsdat.w = 180.0f;
                    //dsdat.w = dist * 18.0f / 2;
                    //dsdat.h = 180.0f / 2;

                    //dsdat.rot = 0.0f;
                    ////dsdat.rot = +(float)(i * 0.1198831f * 1445);
                    //dsdat.color = { 2.0f, 2.0f, 2.0f, 1.0f };
                    //if (g_LineDraw.p_vertex_connected)
                    //{
                    //	dsdat.color = { 2.0f, 2.0f, 0.0f, 1.0f };
                    //}
                    ////if (p_vertex[i].isEndLine)
                    ////{
                    ////	dsdat.color = { 0.0f, 0.0f, 2.0f, 1.0f };
                    ////}

                    //dsdat.texNo = GetTexData(TEX_TYPE_EFFECT_HUDETEST).textureId;
                    //dsdat.loadTexType = LOADTEXTURETYPE_GRAY;

                    //dsdat.texUV = { 0.0f, 0.0f };
                    //dsdat.texWH = { 1.0f, 1.0f };
                    //dsdat.startPos = { g_LineDraw.p_vertex[i].pos.x, g_LineDraw.p_vertex[i].pos.y };
                    //dsdat.startRot = rott;
                    //dsdat.startScale = { 1.0f, 1.0f };

                    float addX = 0.0f;
                    float addW = 60.0f;
                    if (i + 1 >= KAKOPON_VERTEX_MAX - 1
                        || !dat->vertex[i + 2].enable)
                    { //線が終端である
                        addX = -dist / 4;
                        addW = -dist / 2;
                    }

                    float widthTmp = dist;
                    float hightTmp = 500.0f;

                    DRAW_SPRITE_DAT dsdat = {};
                    dsdat.x = dist / 2 + addX;
                    dsdat.y = 0.0f;
                    dsdat.z = -10.0f;

                    //dsdat.w = dist + 20.0f;
                    //dsdat.h = 56.0f;

                    /*dsdat.w = dist + 50.0f;
                    dsdat.h = 200.0f;*/

                    dsdat.w = widthTmp + addW;
                    dsdat.h = hightTmp;

                    dsdat.rot = 0.0f;

                    if (dat->type == KAKOPON_TYPE::Heal)
                    {
                        lineColor = { 0.0f, 2.0f, 0.0f, 2.0f };
                        if (dat->isConnected)
                        {
                            lineColor = { 1.0f, 2.0f, 1.0f, 2.0f };
                            lineColor.w *= std::min((float)revFrame / 30, 1.0f);
                        }
                    }
                    else
                    {
                        lineColor = { 2.0f, 2.0f, 2.0f, 2.0f };
                        if (dat->isConnected)
                        {
                            lineColor = { 2.0f, 2.0f, 1.2f, 2.0f };
                            lineColor.w *= std::min((float)revFrame / 30, 1.0f);
                        }
                    }

                    dsdat.color = lineColor;
                    //if (p_vertex[i].isEndLine)
                    //{
                    //	dsdat.color = { 0.0f, 0.0f, 2.0f, 1.0f };
                    //}

                    dsdat.texNo = m_TexId_kakoponEffect_Trail;
                    //dsdat.texNo = 0;
                    dsdat.loadTexType = LOADTEXTURETYPE_GRAY;

                    //dsdat.texUV = { 0.0f, 0.0f };
                    //dsdat.texWH = { 1.0f, 1.0f };
                    dsdat.texUV = {
                        0.0f + ((widthTmp) / dsdat.h) * i,
                        0.0f };
                    dsdat.texWH = { ((dsdat.w) / dsdat.h), 1.0f };

                    dsdat.startPos = { dat->vertex[i].pos.x, dat->vertex[i].pos.y };
                    dsdat.startRot = rott;
                    dsdat.startScale = { 1.0f, 1.0f };

                    dsdat.shaderOpt.bloom = true;

                    DrawSpriteInCamera(dsdat);
                }

                //if (dat->isConnected || m_KakoponLineDraw.afterTimerFrame > 0)
                if (dat->isConnected)
                {
                    float alphaTmp = 1.0f * ((float)revFrame / dat->frameMax);
                    KAKOPON_VERTEX* lineStartVertexTmp = dat->vertex + i;
                    KAKOPON_VERTEX* lineEndVertexTmp = dat->vertex + i + 1;

                    if (lineStartVertexTmp->enable && lineEndVertexTmp->enable)
                    {
                        Float2 postmp = CalculateCentroid(lineStartVertexTmp->pos, lineEndVertexTmp->pos);

                        {
                            DRAW_SPRITE_DAT dsdat = {};
                            dsdat.x = 0.0f;
                            dsdat.y = 0.0f;

                            //dsdat.w = dist + 20.0f;
                            //dsdat.h = 56.0f;
                            dsdat.w = CalculateDistance(lineStartVertexTmp->pos, lineEndVertexTmp->pos);
                            dsdat.h = 2200.0f;

                            dsdat.rot3D.x = 90.0f;
                            dsdat.color = { 1.0f, 1.0f, 1.0f,
                                //2.0f * ((float)revFrame / GOE_Character_Player::KAKOPON_COOLDOWN_FRAME)
                                alphaTmp
                            };


                            if (dat->type == KAKOPON_TYPE::Heal)
                            {
                                dsdat.color.x = 1.0f;
                                dsdat.color.y = 1.0f;
                                dsdat.color.z = 1.0f;
                                dsdat.texNo = m_TexId_kakoponEffect_Add_Heal;
                            }
                            else
                            {
                                dsdat.color.x = lineColor.x;
                                dsdat.color.y = lineColor.y;
                                dsdat.color.z = lineColor.z;

                                dsdat.texNo = m_TexId_kakoponEffect_Add;
                            }


                            //dsdat.texNo = 0;
                            dsdat.loadTexType = LOADTEXTURETYPE_MAIN;

                            //dsdat.texUV = { 0.0f, 0.0f };
                            //dsdat.texWH = { 1.0f, 1.0f };
                            dsdat.texUV = {
                                beforeSizeX_effect0,
                                0.0f
                            };
                            dsdat.texWH = {
                                dsdat.w / dsdat.h,
                                1.0f
                            };
                            beforeSizeX_effect0 += dsdat.texWH.x;

                            dsdat.startPos3D = { postmp.x, postmp.y, -1100.0f };
                            dsdat.startRot3D = { 0.0f, 0.0f, 0.0f };
                            dsdat.startRot3D.z = CalculateRadToDeg(
                                atan2(
                                    lineEndVertexTmp->pos.y - lineStartVertexTmp->pos.y,
                                    lineEndVertexTmp->pos.x - lineStartVertexTmp->pos.x
                                )
                            );
                            dsdat.startScale = {
                                1.0f,
                                1.0f
                            };

                            dsdat.shaderOpt.bloom = true;

                            DrawSpriteInCamera(dsdat);
                        }
                        {
                            DRAW_SPRITE_DAT dsdat = {};
                            dsdat.x = 0.0f;
                            dsdat.y = 0.0f;
                            dsdat.z = 10.0f;

                            //dsdat.w = dist + 20.0f;
                            //dsdat.h = 56.0f;
                            dsdat.w = CalculateDistance(lineStartVertexTmp->pos, lineEndVertexTmp->pos);
                            dsdat.h = 500.0f;

                            //dsdat.rot3D.x = 90.0f;
                            dsdat.rot3D.z = CalculateRadToDeg(
                                atan2(
                                    lineEndVertexTmp->pos.y - lineStartVertexTmp->pos.y,
                                    lineEndVertexTmp->pos.x - lineStartVertexTmp->pos.x
                                )
                            );

                            dsdat.color = { 1.0f, 1.0f, 1.0f,
                                //2.0f * ((float)revFrame / GOE_Character_Player::KAKOPON_COOLDOWN_FRAME)
                                alphaTmp
                            };
                            dsdat.color.x = lineColor.x;
                            dsdat.color.y = lineColor.y;
                            dsdat.color.z = lineColor.z;

                            dsdat.texNo = m_TexId_kakoponEffect_Aura;
                            

                            //dsdat.texNo = 0;
                            dsdat.loadTexType = LOADTEXTURETYPE_MAIN;

                            //dsdat.texUV = { 0.0f, 0.0f };
                            //dsdat.texWH = { 1.0f, 1.0f };
                            dsdat.texUV = {
                                beforeSizeX_effect1 + (0.02f * (float)revFrame),
                                0.0f + (-0.02f * (float)revFrame)

                            };
                            dsdat.texWH = {
                                //dsdat.w / dsdat.h,
                                dsdat.w / 1600,
                                1.0f
                            };
                            beforeSizeX_effect1 += dsdat.texWH.x;

                            dsdat.startPos3D = { postmp.x, postmp.y, -250.0f };
                            dsdat.startRot3D = { 90.0f, 0.0f, 0.0f };
                            /*	dsdat.startRot3D.z = CalculateRadToDeg(
                                    atan2(
                                        lineEndVertexTmp->pos.y - lineStartVertexTmp->pos.y,
                                        lineEndVertexTmp->pos.x - lineStartVertexTmp->pos.x
                                    )
                                );*/
                            dsdat.startScale = {
                                1.0f,
                                1.0f
                            };

                            dsdat.shaderOpt.bloom = true;

                            DrawSpriteInCamera(dsdat,
                                false,
                                false,
                                false
                            );
                        }
                    }
                }
            }
        }
    }
}
