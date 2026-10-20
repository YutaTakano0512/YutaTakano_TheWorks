// ===================================================
// Stage_Simulation.h ステージ処理
// 
// hara sougo  2025/02/22
// ===================================================
#include "../../game_main.h"
#include "../Sprite/sprite.h"
#include "../Polygon/modelData.h"
#include "../Texture/texture.h"
#include "../Entities/Block/block.h"
#include "../Entities/Block/blockPrefab.h"
#include "../Systems/collision.h"
#include "../Systems/easing.h"

#include "../Managers/scene.h"
#include "../../scenes/SCENE_GAME.h"

#include "../Managers/soundManager.h"
#include "../Managers/gameManager.h"
#include "../Game Map/gameMap.h"

#include "../Game Object/gameObject.h"
#include "../Game Object/gameObjectPrefab.h"

#include "../Managers/bg.h"

#include "../Systems/ui.h"
#include "../Systems/ui_Game.h"

#include "../Entities/player.h"
#include "../Systems/camera.h"

#include "../Entities/character.h"

#include "../Game Events/GE_Gaya.h"

#include "Stage_Simulation.h"

#include "../Game Object/Game Object Events/GOE_Character_Player.h"
#include "../Game Object/Game Object Events/GOE_Character_Enemy/GOE_Character_Enemy_Miresu.h"
#include "../Game Object/Game Object Events/GOE_Character_Enemy/GOE_Character_Enemy_Regatosu.h"
#include "../Game Object/Game Object Events/GOE_Character_Enemy/GOE_Character_Enemy_Boss_Kontoriusu.h"
#include "../Game Object/Game Object Events/GOE_Character_Enemy/GOE_Character_Enemy_Boss_Inperiusu.h"

// ===================================================
// マクロ定義
// ===================================================

// ===================================================
// 構造体宣言
// ===================================================

// ===================================================
// プロトタイプ宣言
// ===================================================

// ===================================================
// グローバル変数
// ===================================================



void Stage_Simulation::Init()
{
    SuperClass::Init(); //必ず呼ぶ

    m_CountFrame = m_CountFrameMax;

    m_Model_Yuka = GetModelDataIndex("MODEL/block/general1");
    m_Model_Ring = GetModelDataIndex("MODEL/beam_01");

    //m_TexId_UI_Stage_Start = LoadTexData("TEX/ui/in_game_master/stage_start");
    m_TexId_UI_Stage_Start_L = LoadTexData("TEX/ui/in_game_master/stage_start_l");
    m_TexId_UI_Stage_Start_R = LoadTexData("TEX/ui/in_game_master/stage_start_r");

    m_TexId_UI_Stage_Name = LoadTexData("TEX/ui/in_game_master/stage_logo");

    m_TexId_Yuka = LoadTexData("TEX/bg/tutorial/base");
    m_ShaderIndex_Floor_Vertex = DX_GetShaderIndex("SHADER/vs_general");
    m_ShaderIndex_Floor_Pixel = DX_GetShaderIndex("SHADER/ps_bg_tutorial_floor");
    m_ShaderIndex_Hikari_Vertex = DX_GetShaderIndex("SHADER/vs_world_tutorial_stage_effect");
    m_ShaderIndex_Hikari_Pixel = DX_GetShaderIndex("SHADER/ps_world_tutorial_stage_effect");
    m_ShaderIndex_Ring_Vertex = DX_GetShaderIndex("SHADER/vs_general");
    m_ShaderIndex_Ring_Pixel  = DX_GetShaderIndex("SHADER/ps_bg_tutorial_ring");

    srand(time(NULL));

    //m_Enemies.clear();
    //m_Enemies.shrink_to_fit();
    for (int i = 0; i < ENEMIES_MAX; i++)
    {
        m_Enemies[i] = {};
    }

    //情報だけ先に保存
    m_UseChara.push_back(Character::Load("ENEMY_MIRESU_ATTACKER"));
    m_UseChara.push_back(Character::Load("ENEMY_MIRESU_SHOOTER"));
    m_UseChara.push_back(Character::Load("ENEMY_REGATOSU"));
    m_UseChara.push_back(Character::Load("ENEMY_BOSS_INPERIUSU"));

    m_UseChara.push_back(Character::Load("ENEMY_MIRESU_ATTACKER_HYPER"));
    m_UseChara.push_back(Character::Load("ENEMY_RUCS"));


    //ステージ用のシェーダ読み込み
    //SetPostEffectShader("SHADER/shader_world_iseki");
    SetPostEffectShaderVS("SHADER/vs_general2");
    SetPostEffectShaderPS("SHADER/ps_world_general");

    //背景の読み込み
    {
        //SetBg(BG_TYPE_TEST_GAME);
        //SetBg(BG_TYPE_TUTORIAL_0);
        SetBg(BG_TYPE_TUTORIAL_1);

        //SetBg(BG_TYPE_ISEKI);

        SetUIScoreVisible_Game(true);
        //SetUITimerVisible_Game(true);
    }

   // m_MapSize = { 90, 45 };

    //for (int i = 0; i < PLAYER_MAX; i++)
    //{
    //    PLAYER* player = GetPlayer(i);
    //    GAME_OBJECT* playerObj = player->obj;
    //    if (playerObj != nullptr)
    //    {
    //        playerObj->pos = {
    //            0.0f,
    //            BLOCK_HIGHT * -15.0f
    //        };
    //    }
    //}


    if (m_SingleBossBattleNum == 0)
    { //ボスのオブジェクトだけ生成しておく
        GAME_OBJECT* co = nullptr;
        co = Character::CreateCharacterGameObject(m_UseChara[3]);
        if (co != nullptr)
        {
            //co->pos = MkF3(1000.0f, -1000.0f, 260.0f);
            //co->pos = MkF3(500.0f, -2500.0f, 260.0f);

            if (m_Difficulty == Difficulty::Hard) //ハード仕様
            {
                co->pos = MkF3(500.0f, -2600.0f, 260.0f);
                ////めっちゃ下に置いとく
                //co->pos.z = 10000.0f;
                //めっちゃ上に置いとく
                co->pos.z = -100000.0f;
                {
                    GOE_Character_Enemy_Boss_Inperiusu* goe =
                        dynamic_cast<GOE_Character_Enemy_Boss_Inperiusu*>(co->events.source);  // ダウンキャスト

                    if (goe != nullptr)
                    { //
                        goe->BecomeHard();
                    }
                }
            }
            else
            {
                co->pos = MkF3(500.0f, -3200.0f, 260.0f);
            }

            m_BossObj = GetIdentifierGameObject(co);
        }
    }

}

void Stage_Simulation::Uninit()
{
    //シェーダを消す
    //SetPostEffectShader("");
    SetPostEffectShaderVS("");
    SetPostEffectShaderPS("");

    //既存のブロックとオブジェクトとBGMを消す
    //SoundBGMStopRequest();

    //UnloadTexData(m_TexId_Mapkoubou1);

    SuperClass::Uninit(); //必ず呼ぶ
}


void Stage_Simulation::Update()
{
    SuperClass::Update();

    if (m_DelayedClearFrame > 0)
    {
        m_DelayedClearFrame--;
        if (m_DelayedClearFrame <= 0)
        {
            clear_stage();
        }
    }

    //ゲームオーバー処理
    update_gameover();

    update_start();

    //終わっていたら更新をしない
    if (
        //m_CountFrame <= 0 ||
        IsGameover())
    {
        return;
    }

    //int currentFrame = CountFrameMax - m_CountFrame;

    if (m_WaveCountWait == -1)
    {
        if (update_wave()) return;
    }

    //ウェーブ進行の待ち時間
    if (m_WaveCountWait > 0)
    {
        m_WaveCountWait--;
        return;
    }

    //ウェーブが進行したら進
    if (m_CurrentWave == m_BeforeWave)
    {
        update_current_wave();
        m_CurrentWaveFrame++;
        return;
    }

    updating_wave();

    m_BeforeWave = m_CurrentWave;
    m_WaveCountWait = -1;
    m_CurrentWaveFrame = 0;
}


void Stage_Simulation::Draw()
{
    SuperClass::Draw();

    Int2 mapsizetmp = m_MapSize;

    { //背景リング
        DRAW_POLYGON_DAT drawDatTmp = {};
        drawDatTmp.pos.z = BLOCK_HIGHT * 20.0f;
        drawDatTmp.size = MkF3(
            BLOCK_WIDTH * 70.0f,
            BLOCK_HIGHT * 60.0f,
            BLOCK_WIDTH * 30.0f
        );


        drawDatTmp.rot = 0.0f;

        drawDatTmp.rot3D = MkF3(0.0f, 0.0f, 0.0f);

        drawDatTmp.color = MkF4(0.0f, 1.0f, 1.2f, 1.0f);

        drawDatTmp.texNo = 0;
        drawDatTmp.loadTexType = LOADTEXTURETYPE_MAIN;

        drawDatTmp.texUV = { 0.0f, 0.0f };
        drawDatTmp.texWH = { 1.0f, 1.0f };
        drawDatTmp.startScale = MkF2(1.0f, 1.0f);
        drawDatTmp.startScale3D = MkF3(1.0f, 1.0f, 1.0f);

        drawDatTmp.modelNo = m_Model_Ring;

        DRAW_POLYGON_IN_CAMERA_EX_OPTION opt = {};
        opt.reverse = false;
        opt.fixed = false;
        opt.enableShade = false;
        opt.priority = DRAW_POLYGON_PRIORITY_DEEPEST;
        opt.depthBuffIgnore = false;

        opt.shaderType = DPD_SHADER_TYPE_EX;
        opt.shaderTypeEx.vs = m_ShaderIndex_Ring_Vertex;
        opt.shaderTypeEx.ps = m_ShaderIndex_Ring_Pixel;

        DrawPolygonInCameraEx(drawDatTmp, opt);
    }
    { //床
        DRAW_POLYGON_DAT drawDatTmp = {};
        drawDatTmp.pos.z = 0.1f;
        drawDatTmp.size = MkF3(
            BLOCK_WIDTH * mapsizetmp.x,
            BLOCK_HIGHT * mapsizetmp.y,
            1.0f);


        drawDatTmp.rot = 0.0f;

        drawDatTmp.rot3D = MkF3(0.0f, 0.0f, 0.0f);

        drawDatTmp.color = MkF4(0.0f, 1.0f, 0.7f, 1.0f);

        drawDatTmp.texNo = m_TexId_Yuka.textureId;
        drawDatTmp.loadTexType = LOADTEXTURETYPE_MAIN;

        drawDatTmp.texUV = { 0.0f, 0.0f };
        drawDatTmp.texWH = { 1.0f, 1.0f };
        drawDatTmp.startScale = MkF2(1.0f, 1.0f);
        drawDatTmp.startScale3D = MkF3(1.0f, 1.0f, 1.0f);

        drawDatTmp.modelNo = 0;

        DRAW_POLYGON_IN_CAMERA_EX_OPTION opt = {};
        opt.reverse = false;
        opt.fixed = false;
        opt.enableShade = false;
        opt.priority = DRAW_POLYGON_PRIORITY_NML_d5;
        opt.depthBuffIgnore = false;

        opt.shaderType = DPD_SHADER_TYPE_EX;
        opt.shaderTypeEx.vs = m_ShaderIndex_Floor_Vertex;
        opt.shaderTypeEx.ps = m_ShaderIndex_Floor_Pixel;

        DrawPolygonInCameraEx(drawDatTmp, opt);
    }
    //{
    //    for(int i = 0; i < 2; i++)
    //    { //床
    //        DRAW_POLYGON_DAT drawDatTmp = {};
    //        drawDatTmp.size = MkF3(
    //            //BLOCK_WIDTH * m_MapSize.x,
    //            //BLOCK_HIGHT * m_MapSize.y,
    //            500.0f,
    //            1000.0f,
    //            500.0f);

    //        drawDatTmp.pos.z = 0.1f + drawDatTmp.size.z * 2.0f;
    //        //drawDatTmp.pos.x = drawDatTmp.size.x * -1.0f + drawDatTmp.size.x * 2 * i;
    //        drawDatTmp.pos.x = drawDatTmp.size.x * 0.16f + 
    //            drawDatTmp.size.x * -4.65f + drawDatTmp.size.x * 9.3 * i;


    //        drawDatTmp.rot = 0.0f;

    //        drawDatTmp.rot3D = MkF3(90.0f, 0.0f, 0.0f);

    //        drawDatTmp.color = MkF4(1.0f, 1.0f, 1.0f, 1.0f);
    //        //drawDatTmp.color = MkF4(3.5f, 3.5f, 3.5f, 1.0f);
    //        //drawDatTmp.color = { 0.6f, 0.6f, 0.75f, 1.0f };

    //        drawDatTmp.texNo = 0;
    //        drawDatTmp.loadTexType = LOADTEXTURETYPE_MAIN;

    //        drawDatTmp.texUV = { 0.0f, 0.0f };
    //        drawDatTmp.texWH = { 1.0f, 1.0f };
    //        drawDatTmp.startScale = MkF2(1.0f, 1.0f);
    //        drawDatTmp.startScale3D = MkF3(1.0f, 1.0f, 1.0f);

    //        drawDatTmp.modelNo = m_Model_Yuka;

    //        //drawDatTmp.shaderOpt.bloom = DPD_SHADER_OPT_BLOOM_NORMAL;

    //        DRAW_POLYGON_IN_CAMERA_EX_OPTION opt = {};
    //        opt.reverse = false;
    //        opt.fixed = false;
    //        opt.enableShade = false;
    //        opt.priority = DRAW_POLYGON_PRIORITY_NML_DEEPEST;
    //        opt.depthBuffIgnore = false;

    //        DrawPolygonInCameraEx(drawDatTmp, opt);
    //    }
    //}


    { //ホタル？
        DRAW_POLYGON_DAT drawDatTmp = {};
        drawDatTmp.pos.z = 0.0f;
        drawDatTmp.size = MkF3(
            BLOCK_WIDTH * 16.0f,
            BLOCK_HIGHT * 16.0f,
            1.0f
        );


        drawDatTmp.rot = 0.0f;

        drawDatTmp.rot3D = MkF3(90.0f, 0.0f, 0.0f);

        //drawDatTmp.color = MkF4(1.0, 0.9, 0.5, 0.7f);
        drawDatTmp.color = MkF4(0.2, 1.0, 0.7, 0.5f);

        //drawDatTmp.texNo = m_TexId_MapHikari.textureId;
        drawDatTmp.texNo = 0;
        drawDatTmp.loadTexType = LOADTEXTURETYPE_MAIN;

        drawDatTmp.texUV = { 0.0f, 0.0f };
        drawDatTmp.texWH = { 1.0f, 1.0f };
        drawDatTmp.startScale = MkF2(1.0f, 1.0f);
        drawDatTmp.startScale3D = MkF3(1.0f, 1.0f, 1.0f);

        drawDatTmp.modelNo = 0;

        drawDatTmp.vertex.type = DRAW_POLYGON_TYPE_QUAD_INCETANCE;
        drawDatTmp.vertex.instanceNum = 200;

        drawDatTmp.shaderOpt.blendState = DPD_SHADER_OPT_BLEND_STATE_ADDCOLOR;

        DRAW_POLYGON_IN_CAMERA_EX_OPTION opt = {};
        opt.reverse = false;
        opt.fixed = false;
        //opt.billboard = true;
        opt.enableShade = false;
        //opt.priority = DRAW_POLYGON_PRIORITY_NML_ALWAYS;
        //opt.depthBuffIgnore = false;
        opt.priority = DRAW_POLYGON_PRIORITY_NORMAL;
        opt.depthBuffIgnore = true;

        opt.shaderType = DPD_SHADER_TYPE_EX;
        opt.shaderTypeEx.vs = m_ShaderIndex_Hikari_Vertex;
        opt.shaderTypeEx.ps = m_ShaderIndex_Hikari_Pixel;

        DrawPolygonInCameraEx(drawDatTmp, opt);

    }
}

void Stage_Simulation::Retry()
{
    m_RetryCount_CurrentWave++;
    m_RetryCount++;

    GameStopResetRequest();

    //プレイヤー以外を削除する
    for (int i = 0; i < GAME_OBJECT_MAX; i++)
    {
        GAME_OBJECT* ob = GetGameObject(i);
        if (IsActiveGameObject(ob)
            && (ob->tags.size() <= 0
                || !ob->tags[0].find("PLAYER") == 0)
            )
        {
            DeleteGameObject(ob);
        }
    }

    //for (auto& b : m_cachedMapBlocks)
    //{
    //    if (b == nullptr) continue;
    //    DeleteBlock(b);
    //}
    //m_cachedMapBlocks.clear();
    //GameMapLoad(GAME_MAP_DATA_ID_STAGE_ISEKI_01, &m_cachedMapBlocks);

    //現存する敵キャラ
    for (int i = 0; i < ENEMIES_MAX; i++)
    {
        m_Enemies[i] = {};
    }

    m_EnemyBoss = {};

    //m_CurrentWave = 0;
    m_BeforeWave = -1;

    //m_WaveCountWait = 0;
    m_WaveCountWait = FRAME_ONE_SECOND * 1.0f;

    m_CountStop = true;
    m_CountFrame = m_CountFrameMax;
    // m_CountUpFrame = 0;

    m_GameoverConfirmFlag = false;
    m_GameoverFlag = false;
    m_GameoverFrame = 0;

    m_StartFrame = 0;

    m_StartStageNameFrame = -1;
    m_StartSignFrame = -1;

    //スコアをリセット
    //GameManagerResetResult();
    GameManagerResetResultScore();
    GameManagerResetResultAddScoreBuf();
    GameManagerResetResultAddScoreEvent();

    GE_GayaCancelRequestAll();

    //ムービーフラグ
    GameManagerSetMovieFlag(false);

    UIReset();
    SetUIEventTimer_Game(false);
    ShowBossbar_Game(0, false, "");

    ResetCamera();
    ResetPlayers();

    ////ハードの場合プレイヤーの耐久を下げる
    //if (m_Diff == Difficulty::Hard)
    //{
    //    for (int i = 0; i < PLAYER_MAX; i++)
    //    {
    //        PLAYER* player = GetPlayer(i);
    //        GAME_OBJECT* playerObj = player->obj;
    //        if (playerObj != nullptr)
    //        {
    //            playerObj->status.hpMax *= 0.6;
    //            playerObj->status.hp = playerObj->status.hpMax;
    //        }
    //    }
    //}

    if (m_SingleBossBattleNum == 0)
    { //ボスのオブジェクトだけ生成しておく
        GAME_OBJECT* co = nullptr;
        co = Character::CreateCharacterGameObject(m_UseChara[3]);
        if (co != nullptr)
        {
            //co->pos = MkF3(1000.0f, -1000.0f, 260.0f);
            //co->pos = MkF3(500.0f, -2500.0f, 260.0f);

            if (m_Difficulty == Difficulty::Hard) //ハード仕様
            {
                co->pos = MkF3(500.0f, -2600.0f, 260.0f);
                ////めっちゃ下に置いとく
                //co->pos.z = 10000.0f;
                //めっちゃ上に置いとく
                co->pos.z = -100000.0f;
                {
                    GOE_Character_Enemy_Boss_Inperiusu* goe =
                        dynamic_cast<GOE_Character_Enemy_Boss_Inperiusu*>(co->events.source);  // ダウンキャスト

                    if (goe != nullptr)
                    { //
                        goe->BecomeHard();
                    }
                }
            }
            else
            {
                co->pos = MkF3(500.0f, -3200.0f, 260.0f);
            }

            m_BossObj = GetIdentifierGameObject(co);
        }
    }

    //ステージ用のシェーダ読み込み
    //SetPostEffectShader("SHADER/shader_world_iseki");
    SetPostEffectShaderVS("SHADER/vs_general2");
    SetPostEffectShaderPS("SHADER/ps_world_general");

    //背景の読み込み
    {
        //SetBg(BG_TYPE_TEST_GAME);
        //SetBg(BG_TYPE_TUTORIAL_0);
        SetBg(BG_TYPE_TUTORIAL_1);

        //SetBg(BG_TYPE_ISEKI);

        SetUIScoreVisible_Game(true);
        //SetUITimerVisible_Game(true);
    }

    {
        //BGMを止める(タイトルからここまで)
        SoundBGMStopRequest();

        //bool isBoss =
        //    (m_CurrentWave == 3 && m_Difficulty == Difficulty::Hard)
        //    || m_CurrentWave == 4
        //    ;

        //if (!isBoss)
        //{
        //    //BGMを再生する
        //    SoundBGMPlayRequest(
        //        GetSoundData("SOUND/BGM/ISEKI_LONG"),
        //        //(FRAME_ONE_MINUTE * 1) 
        //        +(FRAME_ONE_SECOND * 41),
        //        1.0f);
        //}
        //SoundBGMPlayRequest(SOUND_TYPE_BGM_2, (60 * 60 * 1) + (60 * 42), 1.0f);
    }
}

void Stage_Simulation::Clear()
{
    m_DelayedClearFrame = 100;
}
void Stage_Simulation::GiveUp()
{
    //SetSceneFade(SCENE_CHARACTERSELECT, FADE_TYPE_STAR_WIPE);
    SetSceneFade(SCENE_TITLE, FADE_TYPE_STAR_WIPE);
}


void Stage_Simulation::update_start()
{
    if (m_StartFrame >= 0)
    {
        if (m_RetryCount > 0)
        { //二回目以降
            if (m_StartFrame == 0)
            {
                //GameManagerSetMovieFlag(true);

                //CameraFlashRequestAll(16, EasingOutSine, MkF4(-1.0f, -1.0f, -1.0f, 1.0f));

                Float3 zoomPos2 = {};
                zoomPos2.y += BLOCK_HIGHT * m_MapSize.y / 2 * 0.3f;
                Float3 zoomPos2Ta = zoomPos2;
                zoomPos2Ta.y += 35000.0f;
                zoomPos2Ta.z += -5500.0f;

                SetCameraTargetPosAll(zoomPos2Ta);
                SetCameraPosAll(zoomPos2);
                SetCameraZoomAll(0.4f);

                CameraFlashRequestAll(20, EasingOutSine, MkF4(-1.0f, -1.0f, -1.0f, 1.0f));

                CameraMoveCancelRequestAll();
                CameraTargetMoveCancelRequestAll();

                //SetCameraModeAll(CAMERA_MODE_PLAYER_CHASE_INSTANT);
                SetCameraModeAll(CAMERA_MODE_PLAYER_CHASE);

            }
            if (m_StartFrame == 0 + 1)
            {
                int apnum = GetActivePlayerNum();
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
                            Float2 addpos = {};
                            if (player->index >= 1)
                            {
                                addpos.x = (1.0f * (i - 1) - 0.5f * (apnum - 2))
                                    * BLOCK_WIDTH * 4.0f;
                                addpos.y = BLOCK_HIGHT * 4.0f;
                            }
                            playerObj->pos = MkF3(
                                addpos.x,
                                BLOCK_HIGHT * m_MapSize.y / 2 - BLOCK_HIGHT * 9.0f
                                + addpos.y,
                                0.0f
                            );

                            //移動
                            goe->WalkRequest(
                                5,
                                MkF3(
                                    playerObj->pos.x,
                                    BLOCK_HIGHT * m_MapSize.y / 2 - BLOCK_HIGHT * 10.0f
                                    + addpos.y,
                                    playerObj->pos.z
                                ),
                                EasingOutSine
                            );
                        }
                    }
                }
            }
        }
        else
        {
            if (m_StartFrame == 0)
            {
                GameManagerSetMovieFlag(true);
                UIMovieFlagInstant_Game();

                SetCameraModeAll(CAMERA_MODE_NORMAL);

                int apnum = GetActivePlayerNum();
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
                            Float2 addpos = {};
                            if (player->index >= 1)
                            {
                                addpos.x = (1.0f * (i - 1) - 0.5f * (apnum - 2))
                                    * BLOCK_WIDTH * 4.0f;
                                addpos.y = BLOCK_HIGHT * 4.0f;
                            }
                            playerObj->pos = MkF3(
                                addpos.x,
                                BLOCK_HIGHT * m_MapSize.y / 2 + BLOCK_HIGHT * 16.0f,
                                0.0f
                            );

                            //移動
                            goe->WalkRequest(
                                STARTING_FRAME,
                                MkF3(
                                    playerObj->pos.x,
                                    BLOCK_HIGHT * m_MapSize.y / 2 - BLOCK_HIGHT * 9.0f
                                    + addpos.y,
                                    playerObj->pos.z
                                ),
                                EasingOutSine
                            );
                        }
                    }
                }
            }

        }

        m_StartFrame++;
    }

    if (m_StartStageNameFrame >= 0)
    {
        m_StartStageNameFrame++;
        if (m_StartStageNameFrame > STARTING_STAGE_NAME_FRAME)
        {
            m_StartStageNameFrame = -1;
        }
    }
    if (m_StartSignFrame >= 0)
    {
        m_StartSignFrame++;
        if (m_StartSignFrame > STARTING_FRAME)
        {
            m_StartSignFrame = -1;
        }
    }
}

void Stage_Simulation::draw_start()
{


    if (m_StartStageNameFrame >= 0)
    {
        float modIn = std::max(std::min((float)m_StartStageNameFrame / 30, 1.0f), 0.0f);
        float modOut = std::max(std::min((float)(m_StartStageNameFrame - (STARTING_STAGE_NAME_FRAME - 30)) / 30, 1.0f), 0.0f);
        //float mod = std::max(std::min((float)m_StartSignFrame / STARTING_FRAME, 1.0f), 0.0f);

        {
            DRAW_POLYGON_DAT dsdat = {};
            dsdat.pos.x = 0.0f + SCREEN_WIDTH * 2
                - SCREEN_WIDTH * 2 * EasingOutExpo(modIn)
                - SCREEN_WIDTH * 2 * EasingInExpo(modOut)
                ;

            /*       dsdat.pos.x = -20.0f + SCREEN_WIDTH * 2
                    - SCREEN_WIDTH * 2
                    * (1.0f - EasingInOutExpo(sin(2.0f * SYS_GENERAL_PI * (1.0f - mod)) / 2.0f + 0.5f));*/

                    //dsdat.pos.x = 250.0f + SCREEN_WIDTH * 2
                    //    - SCREEN_WIDTH * 2 * EasingOutExpo(modIn)
                    //    + SCREEN_WIDTH * 2 * EasingInExpo(modOut)
                    //    ;

            dsdat.pos.y = 0.0f;
            dsdat.pos.z = 0.0f;

            dsdat.size.x = (float)(m_TexId_UI_Stage_Name.width);
            dsdat.size.y = (float)(m_TexId_UI_Stage_Name.height);
            dsdat.size.z = 1.0f;

            dsdat.rot = 0.0f;

            dsdat.color = {
                1.0f,
                1.0f,
                1.0f,
                0.9f
            };
            dsdat.color.w *= EasingOutExpo(modIn);
            //dsdat.texNo = m_TexId_UI_Stage_Start.textureId;

            dsdat.texNo = m_TexId_UI_Stage_Name.textureId;

            dsdat.loadTexType = LOADTEXTURETYPE_MAIN;

            dsdat.texWH = { 1.0f, 1.0f };

            dsdat.startRot = 0.0f;
            dsdat.startScale = MkF2(1.0f, 1.0f);
            dsdat.startScale3D = MkF3(1.0f, 1.0f, 1.0f);

            dsdat.modelNo = 0;

            DRAW_POLYGON_IN_CAMERA_EX_OPTION opt = {};
            opt.reverse = false;
            opt.fixed = true;
            opt.enableShade = false;
            opt.billboard = false;
            opt.priority = DRAW_POLYGON_PRIORITY_FORCE_NORMAL;
            opt.shaderType = DPD_SHADER_TYPE_UI_BG_GENERAL;

            DrawPolygonInCameraEx(dsdat, opt);
        }
    }
    if (m_StartSignFrame >= 0)
    {
        float modIn = std::max(std::min((float)m_StartSignFrame / 30, 1.0f), 0.0f);
        float modOut = std::max(std::min((float)(m_StartSignFrame - (STARTING_FRAME - 30)) / 30, 1.0f), 0.0f);
        //float mod = std::max(std::min((float)m_StartSignFrame / STARTING_FRAME, 1.0f), 0.0f);

        for (int i = 0; i < 2; i++)
        {
            DRAW_POLYGON_DAT dsdat = {};
            //dsdat.pos.x = 0.0f + SCREEN_WIDTH * 2
            //    - SCREEN_WIDTH * 2 * EasingOutExpo(modIn)
            //    - SCREEN_WIDTH * 2 * EasingInExpo(modOut)
            //    ;

     /*       dsdat.pos.x = -20.0f + SCREEN_WIDTH * 2
                - SCREEN_WIDTH * 2
                * (1.0f - EasingInOutExpo(sin(2.0f * SYS_GENERAL_PI * (1.0f - mod)) / 2.0f + 0.5f));*/

                //dsdat.pos.x = 250.0f + SCREEN_WIDTH * 2
                //    - SCREEN_WIDTH * 2 * EasingOutExpo(modIn)
                //    + SCREEN_WIDTH * 2 * EasingInExpo(modOut)
                //    ;

            dsdat.pos.y = 0.0f + SCREEN_HEIGHT * -0.28f;
            dsdat.pos.z = 0.0f;

            dsdat.size.x = (float)(768 * 1.6) * (1.0f + 3.0f * EasingInExpo(1.0f - modIn));
            dsdat.size.y = (float)(768 * 1.6) * (1.0f + 3.0f * EasingInExpo(1.0f - modIn));
            dsdat.size.z = 1.0f;

            dsdat.pos.x = 0.0f //+ SCREEN_WIDTH * 2
                //+ dsdat.size.x / 4 * EasingInExpo(1.0f - modIn)
                + SCREEN_WIDTH * 2 * EasingInExpo(modOut)
                ;
            dsdat.pos.x *= (i == 0 ? -1 : 1);

            dsdat.rot = 0.0f;

            dsdat.color = {
                1.0f,
                1.0f,
                1.0f,
                //0.75f
                0.85f
            };
            dsdat.color.w *= EasingOutExpo(modIn);
            //dsdat.texNo = m_TexId_UI_Stage_Start.textureId;

            switch (i)
            {
            case 1:
                dsdat.texNo = m_TexId_UI_Stage_Start_R.textureId;
                break;
            default:
                dsdat.texNo = m_TexId_UI_Stage_Start_L.textureId;
                break;
            }

            dsdat.loadTexType = LOADTEXTURETYPE_MAIN;

            dsdat.texWH = { 1.0f, 1.0f };

            dsdat.startRot = 0.0f;
            dsdat.startScale = MkF2(1.0f, 1.0f);
            dsdat.startScale3D = MkF3(1.0f, 1.0f, 1.0f);

            dsdat.modelNo = 0;

            DRAW_POLYGON_IN_CAMERA_EX_OPTION opt = {};
            opt.reverse = false;
            opt.fixed = true;
            opt.enableShade = false;
            opt.billboard = false;
            opt.priority = DRAW_POLYGON_PRIORITY_FORCE_NORMAL;
            //opt.shaderType = DPD_SHADER_TYPE_UI_BG_GENERAL;

            DrawPolygonInCameraEx(dsdat, opt);
        }
    }
}

void Stage_Simulation::update_gameover()
{
    int activep = 0;
    for (int i = 0; i < PLAYER_MAX; i++)
    {
        PLAYER* player = GetPlayer(i);
        GAME_OBJECT* playerObj = player->obj;
        if (playerObj != nullptr)
        {

            if (!IsDefeatGameObject(playerObj))
            {
                activep++;
            }
        }
    }

    if (!IsGameover())
    {
        bool gameoverflag = false;
        if (activep <= 0)
        { //全滅
            gameoverflag = true;

            SetPlayerGameoverDecision();

            SoundEffectPlayRequest(SOUND_TYPE_SE_9, 5.0f, 0.0f);

            //GameManagerSetMovieFlag(true);

            //CameraFlashRequestAll(
            //    180,
            //    EasingReverse,
            //    MkF4(1.0f, 0.0f, 0.0f, 0.9f),
            //    DPD_SHADER_OPT_BLEND_STATE_ADDCOLOR
            //);

            CameraShakeRequestAll(
                CAMERA_SHAKE_EXLARGE_FRAME * 2.0f,
                CAMERA_SHAKE_EXLARGE_POWER * 2.0f
            );
            m_GameoverFadeColor = MkF4(1.0f, 0.0f, 0.0f, 0.9f);

            GameStopRequest(120);

            GE_GayaCancelRequestAll();
            //GE_GayaStartRequest("game_over_mayu");
        }

        //if (m_CountFrame <= 0)
        //{ //タイムアップ
        //    gameoverflag = true;

        //    //SoundEffectPlayRequest(SOUND_TYPE_SE_9, 10.0f, 0.0f);

        //    //GameManagerSetMovieFlag(true);

        //    //CameraFlashRequestAll(
        //    //    180,
        //    //    EasingReverse,
        //    //    MkF4(0.0f, 0.0f, 1.0f, 0.9f),
        //    //    DPD_SHADER_OPT_BLEND_STATE_ADDCOLOR
        //    //);
        //    m_GameoverFadeColor = MkF4(0.0f, 0.0f, 1.0f, 0.9f);

        //    GameStopRequest(120);

        //    GE_GayaCancelRequestAll();
        //    //GE_GayaStartRequest("game_timeup_mayu");
        //}

        if (gameoverflag)
        {
            SoundBGMStopRequest();

            //GamePause(true);

            SetGameover();

            //GameStopRequest(120);
            //SetSceneFade(SCENE_RESULT);

            //CameraZoomRequest(
            //    player->index,
            //    0.04f,
            //    Float3ToFloat2(m_Obj->pos),
            //    EasingInOutSine,
            //    0.65f
            //);
            //CameraZoomResetRequest(
            //    player->index,
            //    0.02f,
            //    EasingInOutSine,
            //    120
            //);
        }
    }

    else if (!IsGameStop())
    {
        if (m_GameoverFrame == 0)
        {
            m_GameoverConfirmFlag = true;
            //GameManagerSetSceneRequest(SCENE_RESULT, GAME_OVER_AFTER_FRAME, false);

            //for (int i = 0; i < GAME_OBJECT_MAX; i++)
            //{
            //    GAME_OBJECT* obj = GetGameObject(i);
            //    if (obj != nullptr
            //        && GetPlayer(obj) == nullptr)
            //    {
            //        GameObjectHitStop(obj, FRAME_ONE_MINUTE);
            //    }
            //}
        }

        m_GameoverFrame++;
    }
}

bool Stage_Simulation::update_wave()
{
    int activeEnemCurrentHitStopNum = 0;
    int activeEnemNum = 0;

    //if (m_Enemies.size() > 0)
    //{
    //    for (int i = 0; i < m_Enemies.size(); i++)
    //    {
    //        GAME_OBJECT* enemObj = GetGameObjectFindByIdentifier(m_Enemies[i]);
    //        if (IsActiveGameObject(enemObj))
    //        {
    //            activeEnemNum++;
    //            //if (enemObj->hitStop.frame > 0)
    //            //{
    //            //    activeEnemCurrentHitStopNum++;
    //            //}
    //        }
    //    }
    //}
    //else
    //{
    //    activeEnemNum = -1;
    //}
    for (int i = 0; i < ENEMIES_MAX; i++)
    {
        GAME_OBJECT* enemObj = GetGameObjectFindByIdentifier(m_Enemies[i]);
        if (IsActiveGameObject(enemObj))
        {
            activeEnemNum++;
            if (enemObj->hitStop.frame > 0)
            {
                activeEnemCurrentHitStopNum++;
            }
        }
    }

    if (!IsActiveGameObject(GetGameObjectFindByIdentifier(m_EnemyBoss))
        && activeEnemNum < LimitEnemyNum
        && activeEnemCurrentHitStopNum <= 0
        )
    { //ウェーブを進める

        m_RetryCount_CurrentWave = 0;
        m_WaveCountWait = 25;
        m_CurrentWave++;

        for (int i = 0; i < ENEMIES_MAX; i++)
        {
            GAME_OBJECT* enemObj = GetGameObjectFindByIdentifier(m_Enemies[i]);
            if (!IsActiveGameObject(enemObj))
            {
                m_Enemies[i] = {};
            }
        }
        //m_Enemies.clear();
        //auto removeIt = std::remove_if(
        //    m_Enemies.begin(),
        //    m_Enemies.end(),
        //    [&](const GAME_OBJECT_IDENTIFIER ene) 
        //    {
        //        GAME_OBJECT* enemObj = GetGameObjectFindByIdentifier(ene);
        //        if (IsActiveGameObject(enemObj))
        //        {
        //            return false;
        //        }
        //        return true;
        //    }
        //);
        //m_Enemies.erase(removeIt, m_Enemies.end());

        //m_Enemies.shrink_to_fit();

        return true;
    }
    //else
    //{
    //    //時間経過でウェーブを進める
    //    if (
    //        (currentFrame == FRAME_ONE_SECOND * 30 && m_CurrentWave == 0)
    //        || (currentFrame == FRAME_ONE_SECOND * 30 * 2 && m_CurrentWave == 1)
    //        || (m_CountFrame == FRAME_ONE_MINUTE && m_CurrentWave == 2)
    //    )
    //    {
    //        m_CurrentWave++;
    //    }
    //}

    return false;
}

void Stage_Simulation::updating_wave()
{
    if (m_CurrentWave == 0)
    {
        if(m_SingleBossBattleNum == 0)
        {
            GAME_OBJECT* co = GetGameObjectFindByIdentifier(m_BossObj);
            if (co != nullptr)
            {
                GOE_Character_Enemy_Boss_Inperiusu* goe =
                    dynamic_cast<GOE_Character_Enemy_Boss_Inperiusu*>(co->events.source);  // ダウンキャスト

                if (goe != nullptr)
                { //開始命令
                    goe->SetRetryCount(GetRetryCountCurrentWave());
                    goe->StartBattle();
                }

                //m_Enemies.push_back(GetIdentifierGameObject(co));
                EnemyRegister(GetIdentifierGameObject(co));

                m_EnemyBoss = m_BossObj;
            }
        }

        if (m_SingleBossBattleNum == 1)
        {
            {
                GAME_OBJECT* co = nullptr;
                co = Character::CreateCharacterGameObject("ENEMY_BOSS_KONTORIUSU");
                if (co != nullptr)
                {
                    if (m_Difficulty == Difficulty::Hard)
                    {
                        co->pos = MkF3(0.0f, -1500.0f,
                            0.0f - 32767.0f
                        );

                        GOE_Character_Enemy_Boss_Kontoriusu* goe =
                            dynamic_cast<GOE_Character_Enemy_Boss_Kontoriusu*>(co->events.source);  // ダウンキャスト

                        if (goe != nullptr)
                        { //
                            goe->BecomeHard();
                        }
                    }
                    else
                    {
                        co->pos = MkF3(0.0f, -1500.0f,
                            0.0f - 5000.0f
                        );

                    }

                    //m_BossObj = GetIdentifierGameObject(co);
                    EnemyRegister(GetIdentifierGameObject(co));
                    m_EnemyBoss = GetIdentifierGameObject(co);
                }
            }
        }

    }


    if (m_CurrentWave == 1)
    {
        clear_stage();
    }
}

void Stage_Simulation::update_current_wave()
{
}

void Stage_Simulation::clear_stage()
{
    if (m_IsCleared)
    {
        return;
    }

    m_IsCleared = true;

    //SetSceneFade(SCENE_CHARACTERSELECT, FADE_TYPE_STAR_WIPE);
    SetSceneFade(SCENE_TITLE, FADE_TYPE_STAR_WIPE);
}
