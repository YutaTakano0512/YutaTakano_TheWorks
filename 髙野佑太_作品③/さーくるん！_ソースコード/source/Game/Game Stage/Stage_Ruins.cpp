// ===================================================
// Stage_Ruins.cpp ステージ処理
// 
// hara sougo  2024/12/13
// ===================================================
#include "../../game_main.h"
#include "../Sprite/sprite.h"
#include "../Texture/texture.h"
#include "../Polygon/modelData.h"
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

#include "Stage_Ruins.h"

#include "../Game Object/Game Object Events/GOE_Character_Player.h"
#include "../Game Object/Game Object Events/GOE_Character_Enemy/GOE_Character_Enemy_Miresu.h"
#include "../Game Object/Game Object Events/GOE_Character_Enemy/GOE_Character_Enemy_Regatosu.h"
#include "../Game Object/Game Object Events/GOE_Character_Enemy/GOE_Character_Enemy_Boss_Kontoriusu.h"
#include "../Game Object/Game Object Events/GOE_Character_Enemy/GOE_Character_Enemy_Boss_Inperiusu.h"

#include "../Game Object/Game Object Events/GOE_Item.h"
#include "../Game Object/Game Object Events/GOE_ItemTransporter.h"
#include "../Game Object/Game Object Events/GOE_Effect.h"

#include "../Game Object/Game Object Events/GOE_ObjectDynamic.h"

#include "../../Shader/commonBindings.h"
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



void Stage_Ruins::Init()
{
    SuperClass::Init(); //必ず呼ぶ

    srand(time(NULL));

    //m_CurrentWave = 4; //テスト
    //m_CurrentWave = 3; //テスト
    //m_CurrentWave = 2; //テスト

    m_flag_set_mutation = false;
    m_flag_set_destroyStage = false;
    m_MutationFrame = -1;
    m_DestroyStageFrame = -1;

    if (m_Difficulty == Difficulty::Hard)
    {
        m_CountFrameMax = CountFrameMax_H + 1;
        m_SpeedClearFrame = SpeedClearFrameMax_H;

        m_WaveEnemiesAndObjects[0] = LoadStageData("rom\\stagedata\\iseki\\StageData_Iseki_HRD_Wave_00.stgwav");
        m_WaveEnemiesAndObjects[1] = LoadStageData("rom\\stagedata\\iseki\\StageData_Iseki_HRD_Wave_01.stgwav");
        m_WaveEnemiesAndObjects[2] = LoadStageData("rom\\stagedata\\iseki\\StageData_Iseki_HRD_Wave_02.stgwav");
    }
    else
    {
        m_CountFrameMax = CountFrameMax + 1;
        m_SpeedClearFrame = SpeedClearFrameMax;

        m_WaveEnemiesAndObjects[0] = LoadStageData("rom\\stagedata\\iseki\\StageData_Iseki_NML_Wave_00.stgwav");
        m_WaveEnemiesAndObjects[1] = LoadStageData("rom\\stagedata\\iseki\\StageData_Iseki_NML_Wave_01.stgwav");
        m_WaveEnemiesAndObjects[2] = LoadStageData("rom\\stagedata\\iseki\\StageData_Iseki_NML_Wave_02.stgwav");
        m_WaveEnemiesAndObjects[3] = LoadStageData("rom\\stagedata\\iseki\\StageData_Iseki_NML_Wave_03.stgwav");
    }
    m_CountFrame = m_CountFrameMax;

    m_TexId_MapMichi = LoadTexData("TEX/bg/iseki/isekimap_mithi13");
    m_TexId_MapTile = LoadTexData("TEX/bg/iseki/isekimap13");
    m_TexId_MapWall = LoadTexData("TEX/bg/iseki/isekimap_yoko");
    m_TexId_Mapkoubou1 = LoadTexData("TEX/bg/iseki/koubou1");
    m_TexId_Mapkoubou2 = LoadTexData("TEX/bg/iseki/koubou2");
    m_TexId_Mapnaraku = LoadTexData("TEX/effect/NoiseTile");

    m_TexId_MapHikari = LoadTexData("TEX/effect/spotlight_0");
    m_TexId_MapSmoke = LoadTexData("TEX/effect/stagesmoke");

    m_tex_iseki_mut[0] = LoadTexData("TEX/effect/boss/tex_inp_bk_01");
    m_tex_iseki_mut[1] = LoadTexData("TEX/effect/boss/tex_inp_bk_02");
    m_tex_iseki_mut[2] = LoadTexData("TEX/effect/barrier/tex_br_noise");
    m_tex_iseki_mut[3] = LoadTexData("TEX/effect/barrier/tex_br_wave");
    m_tex_iseki_mut[4] = LoadTexData("TEX/effect/barrier/tex_br_wave02");

    //m_TexId_UI_Stage_Start = LoadTexData("TEX/ui/in_game_master/stage_start");
    m_TexId_UI_Stage_Start_L = LoadTexData("TEX/ui/in_game_master/stage_start_l");
    m_TexId_UI_Stage_Start_R = LoadTexData("TEX/ui/in_game_master/stage_start_r");

    m_TexId_UI_Stage_Name = LoadTexData("TEX/ui/in_game_master/stage_logo");

    m_TexId_MutBg0 = LoadTexData("TEX/effect/boss/tex_inp_bk_01");
    m_Model_Ring = GetModelDataIndex("MODEL/beam_01");
    m_Model_Ball = GetModelDataIndex("MODEL/ball");

    m_ShaderIndex_Ring_Vertex = DX_GetShaderIndex("SHADER/vs_general");
    m_ShaderIndex_Ring_Pixel = DX_GetShaderIndex("SHADER/ps_bg_tutorial_ring");

    m_ShaderIndex_Hikari_Vertex = DX_GetShaderIndex("SHADER/vs_world_lightningbug");
    m_ShaderIndex_Hikari_Pixel = DX_GetShaderIndex("SHADER/ps_world_lightningbug");
    m_ShaderIndex_Fog_Vertex = DX_GetShaderIndex("SHADER/vs_world_fog");
    m_ShaderIndex_Fog_Pixel = DX_GetShaderIndex("SHADER/ps_world_fog");


    GE_GayaCancelRequestAll();

    ResetCamera();
    ResetPlayers();

    SetPostEffectShaderVariable({});
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
    m_UseChara.push_back(Character::Load("ENEMY_RUCS"));
    m_UseChara.push_back(Character::Load("ENEMY_BOSS_INPERIUSU"));

    m_UseChara.push_back(Character::Load("ENEMY_MIRESU_ATTACKER_HYPER"));
    //m_UseChara.push_back(Character::Load("ENEMY_SATELLITE"));

    //マップの読み込み
    m_cachedMapBlocks.clear();
    GameMapLoad(GAME_MAP_DATA_ID_STAGE_ISEKI_01, &m_cachedMapBlocks);

    { //ボスのオブジェクトだけ生成しておく
        GAME_OBJECT* co = nullptr;
        co = Character::CreateCharacterGameObject(m_UseChara[4]);
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
    SetPostEffectShaderPS("SHADER/ps_world_iseki");

    //背景の読み込み
    {
        //SetBg(BG_TYPE_TEST_GAME);
        //SetBg(BG_TYPE_GENERAL_0);

        SetBg(BG_TYPE_ISEKI);

        SetUIScoreVisible_Game(true);
        SetUITimerVisible_Game(true);
    }

    {
        //BGMを止める(タイトルからここまで)
        SoundBGMStopRequest();

        //BGMを再生する
        SoundBGMPlayRequest(
            GetSoundData("SOUND/BGM/ISEKI_LONG"), 
            //(FRAME_ONE_MINUTE * 1) 
            + (FRAME_ONE_SECOND * 41),
            1.0f);
        //SoundBGMPlayRequest(SOUND_TYPE_BGM_2, (60 * 60 * 1) + (60 * 42), 1.0f);
    }
}

void Stage_Ruins::Uninit()
{
    //シェーダを消す
    //SetPostEffectShader("");
    SetPostEffectShaderVS("");
    SetPostEffectShaderPS("");

    //既存のブロックとオブジェクトとBGMを消す
    SoundBGMStopRequest();

    UnloadTexData(m_tex_iseki_mut[0]);
    UnloadTexData(m_tex_iseki_mut[1]);
    UnloadTexData(m_tex_iseki_mut[2]);
    UnloadTexData(m_tex_iseki_mut[3]);
    UnloadTexData(m_tex_iseki_mut[4]);

    UnloadTexData(m_TexId_Mapkoubou1);
    UnloadTexData(m_TexId_Mapkoubou2);
    UnloadTexData(m_TexId_MapHikari);
    
    UnloadTexData(m_TexId_MapTile);
    UnloadTexData(m_TexId_MapWall);
    UnloadTexData(m_TexId_MapMichi);

    //UnloadTexData(m_TexId_UI_Stage_Start);
    UnloadTexData(m_TexId_UI_Stage_Start_L);
    UnloadTexData(m_TexId_UI_Stage_Start_R);
    UnloadTexData(m_TexId_UI_Stage_Name);
    

    SuperClass::Uninit(); //必ず呼ぶ
}


void Stage_Ruins::Update()
{
    SuperClass::Update();

    update_mutation();
    update_destroyStage();

#if defined(_DEBUG) || defined(DEBUG)
    if (Keyboard_IsKeyTrigger(KK_B))
    {//
        { //ボスのオブジェクトだけ生成しておく
            GAME_OBJECT* co = nullptr;
            co = Character::CreateCharacterGameObject(m_UseChara[4]);
            if (co != nullptr)
            {
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
            }
            if (co != nullptr)
            {
                GOE_Character_Enemy_Boss_Inperiusu* goe =
                    dynamic_cast<GOE_Character_Enemy_Boss_Inperiusu*>(co->events.source);  // ダウンキャスト

                if (goe != nullptr)
                { //開始命令
                    goe->SetRetryCount(1);
                    goe->StartBattle();
                }

                m_EnemyBoss = GetIdentifierGameObject(co);

                //m_Enemies.push_back(GetIdentifierGameObject(co));
                //EnemyRegister(GetIdentifierGameObject(co));
            }
        }
    }


    if (Keyboard_IsKeyTrigger(KK_F))
    {
        for (int i = 0; i < 3; i++)
        {
            GAME_OBJECT_COLLIDER collider = {};
            collider.type = GAME_OBJECT_COLLIDER_TYPE_CAPSULE;

            collider.capsule.radius = BLOCK_WIDTH 
                * (2.0f + 3.0f * (float)(rand() % 1000) / 1000.0f)
                ;

            Float3 capVec = NormalizeVector(
                MkF3(
                    ((float)(rand() % 1000) / 1000.0f) * 2.0f - 1.0f,
                    ((float)(rand() % 1000) / 1000.0f) * 2.0f - 1.0f,
                    0.0f
                )
            );
            collider.capsule.pointA = capVec * BLOCK_WIDTH * +4.0f;
            collider.capsule.pointB = capVec * BLOCK_WIDTH * -4.0f;
            collider.capsule.pointA.z = collider.capsule.radius * 0.75f;
            collider.capsule.pointB.z = collider.capsule.radius * 0.75f;

            GOE_ObjectDynamic* goe = GOE_ObjectDynamic::CreateObjectDynamic(
                GOE_ObjectDynamic::Type::General,
                MkF3(
                    BLOCK_WIDTH * (-m_MapSize.x / 2 + m_MapSize.x * (float)(rand() % 1000) / 1000.0f),
                    BLOCK_WIDTH * (-m_MapSize.y / 2 + m_MapSize.y * (float)(rand() % 1000) / 1000.0f),
                    BLOCK_WIDTH * -100.0f
                ),
                collider
            );
            goe->StartFall();

            GameObjectHitStop(goe->GetGameObj(), 20 * i);
        }
    }
#endif


    //ゲームオーバー処理
    update_gameover();

    update_start();

    //終わっていたら更新をしない
    if (m_CountFrame <= 0 || IsGameover())
    {
        return;
    }

    int currentFrame = CountFrameMax - m_CountFrame;

    if(m_WaveCountWait == -1)
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

static float KoboFrame(const Float3& pos)
{
    return (float)GetMainGameFrame()
        + sin(pos.x / 123.0f) * 75.0f
        + cos(pos.y / 211.0f) * 75.0f
        + tan(pos.z / 377.0f) * 75.0f
        ;
}
static float KoboAlpha(const CAMERA& camera, int i, const Float3& pos)
{
    //DX_CAMERA_INFO dxcamera = DX_GetCameraInfo(i);

    Float3 pos1 = camera.trans.pos;
    pos1.x += camera.camera3D.pos.x / -camera.camera3D.scale / camera.trans.zoom;
    pos1.y += camera.camera3D.pos.y / -camera.camera3D.scale / camera.trans.zoom;
    pos1.z += camera.camera3D.pos.z / camera.camera3D.scale / camera.trans.zoom;
    Float3 pos2 = pos;
    //pos1.z = 0;
    //pos2.z = 0;

    //float a = std::max(std::min(CalculateDistance(pos1, pos2) / 1200.0f, 1.0f), 0.0f);
    float a = std::max(std::min((pos1.y - pos2.y) / 3200.0f, 1.0f), 0.0f);
    //if (i == 0)
    //{
    //    DebugPrintf("%f\n", pos1.x);
    //    DebugPrintf("%f\n", pos1.y);
    //    DebugPrintf("%f\n", pos1.z);
    //}
    return a;
}

static void KoboDraw(int i, DRAW_POLYGON_DAT& ddt, DRAW_POLYGON_IN_CAMERA_EX_OPTION& opt)
{
    CAMERA camera = GetCameraInfo(i);

    float f = KoboFrame(ddt.startPos3D);

    //ddt.color.w += sin(f / 110.0f) * 0.2f;
    //ddt.color.w += sin(f / 900.0f
    //    * (1.0f + tan(ddt.startPos3D.x + ddt.startPos3D.y) * 0.13f)
    //    + (ddt.startPos3D.x + ddt.startPos3D.y) * SYS_GENERAL_PI / 2)
    //    * 0.4f;
    ddt.color.w = 0.22f;
    ddt.color.w += sin(f / 900.0f
        * (1.0f + tan(ddt.startPos3D.x + ddt.startPos3D.y) * 0.13f)
        + (ddt.startPos3D.x + ddt.startPos3D.y) * SYS_GENERAL_PI / 2)
        * 0.2f;
    ddt.color.w = std::min(std::max(ddt.color.w, 0.0f), 1.0f);
    ddt.color.w *= KoboAlpha(camera, i, ddt.startPos3D);

    if (ddt.color.w > 0)
    {
        ddt.size.x += sin(f / 120.0f) * 600.0f + 300.0f;
        ddt.size.y += cos(f / 190.0f) * 600.0f + 300.0f;
        ddt.startPos3D.x += sin(f / 150.0f) * 600.0f;
        ddt.startPos3D.y += cos(f / 140.0f) * 600.0f;
        ddt.startPos3D.z += sin(f / 180.0f) * 50.0f;

        //ddt.startPos3D.z += -600.0f * (0.3f / camera.trans.zoom);
        ddt.startPos3D.z += -800.0f * (0.3f / camera.trans.zoom);
        ddt.startPos3D.z += -800.0f
            * std::min((camera.trans.target.z - camera.trans.pos.z) / -100000.0f, 0.0f)
            + 0.0f
            //+ camera.trans.pos.z / 5000.0f
            ;

        DrawPolygonInCameraSingle(i, ddt, opt);
    }
}

void Stage_Ruins::Draw()
{
    SuperClass::Draw();

    draw_start();

    
    float gamef = (float)GetMainGameFrame();

    Float4 shaderVariable = GetPostEffectShaderVariable();
    float day1Mod = std::min(std::max((gamef - shaderVariable.x) / (60.0f * ISEKI_DAY_CHANGE_TIME), 0.0f), 1.0f) * shaderVariable.y;
    float day2Mod = std::min(std::max((gamef - shaderVariable.z) / (60.0f * ISEKI_DAY_CHANGE_TIME), 0.0f), 1.0f) * shaderVariable.w;

    float dayMutationMod = shaderVariable.w <= 0.0f ? 0.0f : (pow(std::min(std::max(
        (gamef - shaderVariable.x) / (60.0f * shaderVariable.w),
        0.0f), 1.0f) * shaderVariable.y, shaderVariable.z));

    //DebugPrintf("%f\n", dayMutationMod);

    bool isMutation = GetBg().type == BG_TYPE_ISEKI_MUTATION;

    Int2 mapsizetmp = m_MapSize;
    mapsizetmp.x *= 1.2f;
    mapsizetmp.y *= 1.2f;

    //if (!isMutation)
    
    if(isMutation)
    {
        DRAW_POLYGON_DAT drawDatTmp = {};
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
        drawDatTmp.texWH = { 20.0f, 1.0f };
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


        {
            DRAW_POLYGON_DAT drawDatCpyTmp = drawDatTmp;
            drawDatCpyTmp.pos = {};
            drawDatCpyTmp.size = MkF3(
                +SCREEN_WIDTH * 2,
                +SCREEN_HEIGHT * 2,
                +1.0f);
            drawDatCpyTmp.color = MkF4(
                1.0f,
                0.0f,
                0.0f,
                0.3f + 0.2f * sin(gamef / 30)
            );
            drawDatCpyTmp.color.w *= dayMutationMod;

            drawDatCpyTmp.startPos = {};
            drawDatCpyTmp.texUV = { 0.0f, 0.0f };
            drawDatCpyTmp.texWH = { 1.0f, 1.0f };
            drawDatCpyTmp.texNo = 0;
            drawDatCpyTmp.shaderOpt.blendState = DPD_SHADER_OPT_BLEND_STATE_ADDCOLOR;
            DRAW_POLYGON_IN_CAMERA_EX_OPTION optCpyTmp = opt;
            optCpyTmp.fixed = true;
            DrawPolygonInCameraEx(drawDatCpyTmp, optCpyTmp);
        }
        {
            DRAW_POLYGON_DAT drawDatCpyTmp = drawDatTmp;
            drawDatCpyTmp.pos = {};
            drawDatCpyTmp.size = MkF3(
                +SCREEN_WIDTH * 2,
                +SCREEN_HEIGHT * 2,
                +1.0f);
            drawDatCpyTmp.color = MkF4(
                1.0f,
                0.0f,
                1.0f,
                0.4f + 0.2f * sin(gamef / 20)
            );
            drawDatCpyTmp.color.w *= dayMutationMod;

            drawDatCpyTmp.startPos = {};
            drawDatCpyTmp.texUV = {
                0.0f,
                0.0f + gamef * 0.008f,
            };
            drawDatCpyTmp.texWH = {
                2.0f,
                2.0f
            };
            drawDatCpyTmp.texNo = m_tex_iseki_mut[3].textureId;
            drawDatCpyTmp.shaderOpt.blendState = DPD_SHADER_OPT_BLEND_STATE_ADDCOLOR;
            DRAW_POLYGON_IN_CAMERA_EX_OPTION optCpyTmp = opt;
            optCpyTmp.fixed = true;
            DrawPolygonInCameraEx(drawDatCpyTmp, optCpyTmp);
        }
    }

    {

        DRAW_POLYGON_DAT drawDatTmp = {};
        drawDatTmp.size = MkF3(
            BLOCK_WIDTH * 240.0f,
            BLOCK_HIGHT * 240.0f,
            BLOCK_HIGHT * 240.0f
        );


        drawDatTmp.rot = 0.0f;

        drawDatTmp.rot3D = MkF3(0.0f, 0.0f, 0.0f);

        drawDatTmp.texNo = m_TexId_Mapnaraku.textureId;
        drawDatTmp.loadTexType = LOADTEXTURETYPE_MAIN;

        drawDatTmp.texWH = {
            10.0f,
            10.0f
        };
        drawDatTmp.startScale = MkF2(1.0f, 1.0f);
        drawDatTmp.startScale3D = MkF3(1.0f, 1.0f, 1.0f);

        drawDatTmp.modelNo = m_Model_Ball;

        DRAW_POLYGON_IN_CAMERA_EX_OPTION opt = {};
        opt.reverse = false;
        opt.fixed = false;
        opt.enableShade = false;
        opt.priority = DRAW_POLYGON_PRIORITY_DEEPEST;

        opt.priority = (DRAW_POLYGON_PRIORITY)(DRAW_POLYGON_PRIORITY_DEEPEST + 1);
        opt.depthBuffIgnore = true;

        float f = gamef;
        if (isMutation)
        {
        }
        else
        {
        }

        drawDatTmp.texUV = {
            -0.003f * f,
            -0.002f * f
        };

        for (int i = 0; i < 3; i++)
        {
            drawDatTmp.color = MkF4(0.02f, 0.03f, 0.1f, 0.1f);
            drawDatTmp.color.x += 0.3f - 0.1f * i;
            drawDatTmp.color.y += 0.3f - 0.1f * i;
            drawDatTmp.color.z += 0.3f - 0.1f * i;
            drawDatTmp.startScale3D = MkF3(
                1.0f + 0.4f * i,
                1.0f + 0.4f * i, 
                1.0f + 0.4f * i
            );
            DrawPolygonInCameraEx(drawDatTmp, opt);
        }
    }
    {
        
        DRAW_POLYGON_DAT drawDatTmp = {};
        drawDatTmp.pos.z = BLOCK_HIGHT * 35.0f;
        drawDatTmp.size = MkF3(
            BLOCK_WIDTH * 1000.0f,
            BLOCK_HIGHT * 1000.0f,
            1.0f);


        drawDatTmp.rot = 0.0f;

        drawDatTmp.rot3D = MkF3(0.0f, 0.0f, 0.0f);

        drawDatTmp.texNo = m_TexId_Mapnaraku.textureId;
        drawDatTmp.loadTexType = LOADTEXTURETYPE_MAIN;

        drawDatTmp.texWH = {
            10.0f,
            10.0f 
        };
        drawDatTmp.startScale = MkF2(1.0f, 1.0f);
        drawDatTmp.startScale3D = MkF3(1.0f, 1.0f, 1.0f);

        drawDatTmp.modelNo = 0;

        //drawDatTmp.shaderOpt.bloom = DPD_SHADER_OPT_BLOOM_NORMAL;

        DRAW_POLYGON_IN_CAMERA_EX_OPTION opt = {};
        opt.reverse = false;
        opt.fixed = false;
        opt.enableShade = false;
        //opt.priority = DRAW_POLYGON_PRIORITY_NML_DEEPEST;
        opt.priority = DRAW_POLYGON_PRIORITY_DEEPEST;

        float f = (float)GetMainGameFrame();
        if (isMutation)
        {
            //drawDatTmp.color = MkF4(0.4f, 0.06f, 0.2f, 0.8f);

            //drawDatTmp.texUV = {
            //    -0.003f * f,
            //    -0.002f * f
            //};

            drawDatTmp.color = MkF4(0.1, 0.1, 0.1, 0.9f);

            Float4 colortmp = MkF4(0.3f, 0.06f, 0.1f, 0.9f);
            drawDatTmp.color.x += (colortmp.x - drawDatTmp.color.x) * dayMutationMod;
            drawDatTmp.color.y += (colortmp.y - drawDatTmp.color.y) * dayMutationMod;
            drawDatTmp.color.z += (colortmp.z - drawDatTmp.color.z) * dayMutationMod;

            drawDatTmp.texUV = {
                -0.003f * f,
                -0.002f * f
            };
            //opt.priority = (DRAW_POLYGON_PRIORITY)(DRAW_POLYGON_PRIORITY_DEEPEST + 1);
            //opt.depthBuffIgnore = true;
        }
        else
        {
            drawDatTmp.color = MkF4(0.03f, 0.06f, 0.08f, 1.0f);

            drawDatTmp.texUV = {
                -0.0006f * f,
                -0.0004f * f
            };
            opt.depthBuffIgnore = false;
        }
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

        drawDatTmp.color = MkF4(1.0f, 1.0f, 1.0f, 1.0f);

        //drawDatTmp.color = MkF4(3.5f, 3.5f, 3.5f, 1.0f);
        //drawDatTmp.color = { 0.6f, 0.6f, 0.75f, 1.0f };

        drawDatTmp.texNo = m_TexId_MapTile.textureId;
        drawDatTmp.loadTexType = LOADTEXTURETYPE_MAIN;

        drawDatTmp.texUV = { 0.0f, 0.0f };
        drawDatTmp.texWH = { 1.0f, 1.0f };
        drawDatTmp.startScale = MkF2(1.0f, 1.0f);
        drawDatTmp.startScale3D = MkF3(1.0f, 1.0f, 1.0f);

        drawDatTmp.modelNo = 0;

        //drawDatTmp.shaderOpt.bloom = DPD_SHADER_OPT_BLOOM_NORMAL;

        DRAW_POLYGON_IN_CAMERA_EX_OPTION opt = {};
        opt.reverse = false;
        opt.fixed = false;
        opt.enableShade = false;
        //opt.priority = DRAW_POLYGON_PRIORITY_NML_DEEPEST;
        opt.priority = DRAW_POLYGON_PRIORITY_NML_d5;
        opt.depthBuffIgnore = false;

        if (isMutation)
        {
        }
        else
        {
        }

        if (m_DestroyStageFrame >= 0)
        {
            drawDatTmp.color.x += -0.2f;
            drawDatTmp.color.y += -0.3f;
            drawDatTmp.color.z += -0.3f;
        }
        DrawPolygonInCameraEx(drawDatTmp, opt);

        {
            DRAW_POLYGON_DAT drawDatTmp2 = drawDatTmp;
            drawDatTmp2.shaderOpt.bloom = DPD_SHADER_OPT_BLOOM_NONE;
            drawDatTmp2.size = MkF3(
                m_TexId_MapMichi.width * (drawDatTmp.size.x / m_TexId_MapTile.width),
                m_TexId_MapMichi.height * (drawDatTmp.size.y / m_TexId_MapTile.height),
                1.0f);
            drawDatTmp2.texNo = m_TexId_MapMichi.textureId;
            drawDatTmp2.startPos3D.x = 0.0f;
            drawDatTmp2.startPos3D.y = drawDatTmp.size.y / 2;
            drawDatTmp2.startPos3D.y += drawDatTmp2.size.y / 2 + 10.0f;
            drawDatTmp2.color.x *= 0.75f;
            drawDatTmp2.color.y *= 0.8f;
            drawDatTmp2.color.z *= 0.7f;

            DrawPolygonInCameraEx(drawDatTmp2, opt);
        }

        DRAW_POLYGON_DAT drawDatTmp2 = drawDatTmp;
        drawDatTmp2.shaderOpt.bloom = DPD_SHADER_OPT_BLOOM_NONE;
        drawDatTmp2.texNo = m_TexId_MapWall.textureId;
        drawDatTmp2.size = MkF3(
            m_TexId_MapWall.width * (drawDatTmp.size.x / m_TexId_MapTile.width),
            m_TexId_MapWall.height * (drawDatTmp.size.y / m_TexId_MapTile.height),
            1.0f);
        //drawDatTmp2.rot = 180.0f;
        drawDatTmp2.startPos3D.x = 0.0f;
        drawDatTmp2.startPos3D.y = drawDatTmp.size.y / 2;
        drawDatTmp2.startPos3D.z = abs(drawDatTmp2.size.y) / 2;
        drawDatTmp2.startRot3D.x = 90.0f;
        DrawPolygonInCameraEx(drawDatTmp2, opt);

        DRAW_POLYGON_DAT drawDatTmp3 = drawDatTmp;
        drawDatTmp3.shaderOpt.bloom = DPD_SHADER_OPT_BLOOM_NONE;
        drawDatTmp3.texNo = m_TexId_MapWall.textureId;
        drawDatTmp3.size = MkF3(
            drawDatTmp.size.y,
            drawDatTmp2.size.y,
            1.0f);
        drawDatTmp3.texWH.x = drawDatTmp3.size.x / drawDatTmp.size.x;
        drawDatTmp3.rot = -90.0f;
        drawDatTmp3.startPos3D.x = drawDatTmp.size.x / 2;
        drawDatTmp3.startPos3D.y = 0.0f;
        drawDatTmp3.startPos3D.z = abs(drawDatTmp3.size.y) / 2;
        drawDatTmp3.startRot3D.y = -90.0f;
        DrawPolygonInCameraEx(drawDatTmp3, opt);

        DRAW_POLYGON_DAT drawDatTmp4 = drawDatTmp3;
        drawDatTmp4.startPos3D.x = -drawDatTmp.size.x / 2;
        DrawPolygonInCameraEx(drawDatTmp4, opt);
        
    }


    for (int i = 0; i < CAMERA_NUM; i++)
    {
        { //
            DRAW_POLYGON_DAT drawDatTmp = {};
            //drawDatTmp.size = MkF3(
            //    //BLOCK_WIDTH * 955.0f / 20,
            //    //BLOCK_HIGHT * 432.0f / 20,
            //    //-BLOCK_WIDTH * 1309.0f / 100,
            //    //-BLOCK_HIGHT * 1081.0f / 100,
            //    -BLOCK_WIDTH * 1309.0f / 60,
            //    -BLOCK_HIGHT * 1081.0f / 60,
            //    1.0f);
            drawDatTmp.texNo = m_TexId_Mapkoubou1.textureId;
            drawDatTmp.size = MkF3(
                //BLOCK_WIDTH * 955.0f / 20,
                //BLOCK_HIGHT * 432.0f / 20,
                //-BLOCK_WIDTH * 1309.0f / 100,
                //-BLOCK_HIGHT * 1081.0f / 100,
                //BLOCK_WIDTH * 0.05f * m_TexId_Mapkoubou1.width,
                //BLOCK_HIGHT * 0.05f * m_TexId_Mapkoubou1.height,
                BLOCK_WIDTH * 0.065f * m_TexId_Mapkoubou1.width,
                BLOCK_HIGHT * 0.065f * m_TexId_Mapkoubou1.height,
                1.0f);


            drawDatTmp.rot = 0.0f;

            drawDatTmp.rot3D = MkF3(80.0f, 0.0f, 0.0f);

            //drawDatTmp.color = MkF4(1.0f, 1.0f, 1.0f, 0.3f);
            //drawDatTmp.color = { 0.6f, 0.6f, 0.75f, 1.0f };
            drawDatTmp.color = MkF4(1.0f, 1.0f, 0.7f, 1.0f);

            if (day1Mod > 0.0f)
            {
                Float4 colortmp = MkF4(2.0f, 0.8f, 0.7f, 1.0f);
                drawDatTmp.color.x += (colortmp.x - drawDatTmp.color.x) * day1Mod;
                drawDatTmp.color.y += (colortmp.y - drawDatTmp.color.y) * day1Mod;
                drawDatTmp.color.z += (colortmp.z - drawDatTmp.color.z) * day1Mod;
            }
            if (day2Mod > 0.0f)
            {
                Float4 colortmp = MkF4(0.6f, 0.9f, 1.2f, 1.0f);
                drawDatTmp.color.x += (colortmp.x - drawDatTmp.color.x) * day2Mod;
                drawDatTmp.color.y += (colortmp.y - drawDatTmp.color.y) * day2Mod;
                drawDatTmp.color.z += (colortmp.z - drawDatTmp.color.z) * day2Mod;
            }

            if (isMutation)
            {
                drawDatTmp.color = MkF4(0.8f, 0.01f, 0.01f, 1.0f);

                Float4 colortmp = MkF4(1.8f, 0.1f, 0.5f, 1.0f);
                drawDatTmp.color.x += (colortmp.x - drawDatTmp.color.x) * dayMutationMod;
                drawDatTmp.color.y += (colortmp.y - drawDatTmp.color.y) * dayMutationMod;
                drawDatTmp.color.z += (colortmp.z - drawDatTmp.color.z) * dayMutationMod;
            }
            drawDatTmp.loadTexType = LOADTEXTURETYPE_MAIN;

            drawDatTmp.texUV = { 0.0f, 0.0f };
            drawDatTmp.texWH = { 1.0f, 1.0f };
            drawDatTmp.startScale = MkF2(1.0f, 1.0f);
            drawDatTmp.startScale3D = MkF3(1.0f, 1.0f, 1.0f);

            drawDatTmp.modelNo = 0;

            drawDatTmp.shaderOpt.bloom = DPD_SHADER_OPT_BLOOM_NORMAL;
            drawDatTmp.shaderOpt.blendState = DPD_SHADER_OPT_BLEND_STATE_ADDCOLOR;

            DRAW_POLYGON_IN_CAMERA_EX_OPTION opt = {};
            opt.reverse = false;
            opt.fixed = false;
            opt.enableShade = false;
            //opt.priority = DRAW_POLYGON_PRIORITY_NML_ALWAYS;
            opt.priority = (DRAW_POLYGON_PRIORITY)(DRAW_POLYGON_PRIORITY_NML_ALWAYS - 2);
            //opt.priority = DRAW_POLYGON_PRIORITY_NML_u1;
            //opt.depthBuffIgnore = false;
            opt.depthBuffIgnore = true;

            //drawDatTmp.startPos3D.z = -2000.0f;
            //drawDatTmp.startPos3D.z = -1800.0f;
            
            //drawDatTmp.startPos3D.z = -2200.0f - 500.0f;
            drawDatTmp.startPos3D.z = -1100.0f;

            {
                DRAW_POLYGON_DAT ddt = drawDatTmp;
                ddt.startPos3D.x = BLOCK_WIDTH * mapsizetmp.x / 2 * -0.7f;
                ddt.startPos3D.y = BLOCK_HIGHT * mapsizetmp.y / 2 * -0.76f;
                KoboDraw(i, ddt, opt);
            }
            {
                DRAW_POLYGON_DAT ddt = drawDatTmp;
                ddt.startPos3D.x = BLOCK_WIDTH * mapsizetmp.x / 2 * 0.8f;
                ddt.startPos3D.y = BLOCK_HIGHT * mapsizetmp.y / 2 * -0.9f;
                KoboDraw(i, ddt, opt);
            }
            {
                DRAW_POLYGON_DAT ddt = drawDatTmp;
                ddt.startPos3D.x = BLOCK_WIDTH * mapsizetmp.x / 2 * 0.1f;
                ddt.startPos3D.y = BLOCK_HIGHT * mapsizetmp.y / 2 * -0.2f;
                drawDatTmp.texNo = m_TexId_Mapkoubou2.textureId;
                drawDatTmp.size = MkF3(
                    BLOCK_WIDTH * 0.065f * m_TexId_Mapkoubou2.width,
                    BLOCK_HIGHT * 0.065f * m_TexId_Mapkoubou2.height,
                    1.0f);
                KoboDraw(i, ddt, opt);
            }
            {
                DRAW_POLYGON_DAT ddt = drawDatTmp;
                ddt.startPos3D.x = BLOCK_WIDTH * mapsizetmp.x / 2 * 0.25f;
                ddt.startPos3D.y = BLOCK_HIGHT * mapsizetmp.y / 2 * -0.35f;
                KoboDraw(i, ddt, opt);
            }
            {
                DRAW_POLYGON_DAT ddt = drawDatTmp;
                ddt.startPos3D.x = BLOCK_WIDTH * mapsizetmp.x / 2 * -0.8f;
                ddt.startPos3D.y = BLOCK_HIGHT * mapsizetmp.y / 2 * 0.6f;
                KoboDraw(i, ddt, opt);
            }
            {
                DRAW_POLYGON_DAT ddt = drawDatTmp;
                ddt.startPos3D.x = BLOCK_WIDTH * mapsizetmp.x / 2 * 0.1f;
                ddt.startPos3D.y = BLOCK_HIGHT * mapsizetmp.y / 2 * 0.7f;
                drawDatTmp.texNo = m_TexId_Mapkoubou2.textureId;
                drawDatTmp.size = MkF3(
                    BLOCK_WIDTH * 0.065f * m_TexId_Mapkoubou2.width,
                    BLOCK_HIGHT * 0.065f * m_TexId_Mapkoubou2.height,
                    1.0f);
                KoboDraw(i, ddt, opt);
            }
        }
    }


    { //エフェクト

        { //霧っぽいもの
            DRAW_POLYGON_DAT drawDatTmp = {};
            drawDatTmp.pos.z = 0.0f;
            //drawDatTmp.size = MkF3(
            //    BLOCK_WIDTH * 400.0f,
            //    BLOCK_HIGHT * 160.0f,
            //    1.0f
            //);
            drawDatTmp.size = MkF3(
                BLOCK_WIDTH * 240.0f,
                BLOCK_HIGHT * 120.0f,
                1.0f
            );


            drawDatTmp.rot = 0.0f;

            drawDatTmp.rot3D = MkF3(90.0f, 0.0f, 0.0f);

            drawDatTmp.color = MkF4(0.26, 0.3, 0.32, 0.86f);

            drawDatTmp.texNo = m_TexId_MapSmoke.textureId;
            //drawDatTmp.texNo = 0;
            drawDatTmp.loadTexType = LOADTEXTURETYPE_MAIN;

            drawDatTmp.texUV = { 0.0f, 0.0f };
            drawDatTmp.texWH = { 1.0f, 1.0f };
            drawDatTmp.startScale = MkF2(1.0f, 1.0f);
            drawDatTmp.startScale3D = MkF3(1.0f, 1.0f, 1.0f);

            drawDatTmp.modelNo = 0;

            drawDatTmp.vertex.type = DRAW_POLYGON_TYPE_QUAD_INCETANCE;
            drawDatTmp.vertex.instanceNum = 400;

            //drawDatTmp.shaderOpt.blendState = DPD_SHADER_OPT_BLEND_STATE_ADDCOLOR;

            DRAW_POLYGON_IN_CAMERA_EX_OPTION opt = {};
            opt.reverse = false;
            opt.fixed = false;
            //opt.billboard = true;
            opt.enableShade = false;
            //opt.priority = DRAW_POLYGON_PRIORITY_NORMAL;
            opt.depthBuffIgnore = true;
            opt.priority = DRAW_POLYGON_PRIORITY_NML_DEEPEST;

            opt.shaderType = DPD_SHADER_TYPE_EX;
            opt.shaderTypeEx.vs = m_ShaderIndex_Fog_Vertex;
            opt.shaderTypeEx.ps = m_ShaderIndex_Fog_Pixel;

            if (isMutation)
            {
                //drawDatTmp.color = MkF4(0.5, 0.1, 0.3, 0.99f);
                drawDatTmp.color = MkF4(0.3, 0.1, 0.6, 0.99f);
                drawDatTmp.shaderOpt.blendState = DPD_SHADER_OPT_BLEND_STATE_ADDCOLOR;
                //drawDatTmp.pos.z = BLOCK_HIGHT * -10.0f;
                //drawDatTmp.color = MkF4(0.0, 0.0, 0.0, 0.99f);
                //drawDatTmp.size.x *= 1.2f;
                //drawDatTmp.size.y *= 1.2f;
                //drawDatTmp.startScale.x *= 1.2f;
                //drawDatTmp.startScale.y *= 1.2f;
                //drawDatTmp.vertex.instanceNum = 100;
            }

            DrawPolygonInCameraEx(drawDatTmp, opt);
        }

        { //ホタル？
            DRAW_POLYGON_DAT drawDatTmp = {};
            drawDatTmp.pos.z = 0.0f;
            drawDatTmp.size = MkF3(
                BLOCK_WIDTH * 6.0f,
                BLOCK_HIGHT * 6.0f,
                1.0f
            );


            drawDatTmp.rot = 0.0f;

            drawDatTmp.rot3D = MkF3(90.0f, 0.0f, 0.0f);

            //drawDatTmp.color = MkF4(1.0, 0.9, 0.5, 0.7f);
            drawDatTmp.color = MkF4(0.7, 1.0, 0.5, 0.7f);

            //drawDatTmp.texNo = m_TexId_MapHikari.textureId;
            drawDatTmp.texNo = 0;
            drawDatTmp.loadTexType = LOADTEXTURETYPE_MAIN;

            drawDatTmp.texUV = { 0.0f, 0.0f };
            drawDatTmp.texWH = { 1.0f, 1.0f };
            drawDatTmp.startScale = MkF2(1.0f, 1.0f);
            drawDatTmp.startScale3D = MkF3(1.0f, 1.0f, 1.0f);

            drawDatTmp.modelNo = 0;

            drawDatTmp.vertex.type = DRAW_POLYGON_TYPE_QUAD_INCETANCE;
            drawDatTmp.vertex.instanceNum = 800;

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

            if (isMutation)
            {
                //drawDatTmp.size.x *= 1.5f;
                //drawDatTmp.size.y *= 1.5f;
                //drawDatTmp.color = MkF4(1.0, 0.1, 0.6, 1.0f);

                drawDatTmp.color = MkF4(0.8, 0.1, 0.9, 0.9f);

                Float4 colortmp = MkF4(0.7, 0.1, 1.5, 0.9f);
                drawDatTmp.color.x += (colortmp.x - drawDatTmp.color.x) * dayMutationMod;
                drawDatTmp.color.y += (colortmp.y - drawDatTmp.color.y) * dayMutationMod;
                drawDatTmp.color.z += (colortmp.z - drawDatTmp.color.z) * dayMutationMod;
            }
            else
            {
                if (m_DestroyStageFrame >= 0)
                {
                    drawDatTmp.color = MkF4(0.9, 0.2, 0.1, 0.9f);
                }
            }

            DrawPolygonInCameraEx(drawDatTmp, opt);

        }


        if (isMutation)
        { //背景リング
            DRAW_POLYGON_DAT drawDatTmp = {};
            drawDatTmp.pos.z = BLOCK_HIGHT * 1500.0f;
            drawDatTmp.size = MkF3(
                BLOCK_WIDTH * 90.0f,
                BLOCK_HIGHT * 80.0f,
                BLOCK_WIDTH * 2000.0f
            );


            drawDatTmp.rot = 0.0f;

            drawDatTmp.rot3D = MkF3(0.0f, 0.0f, 0.0f);

            //drawDatTmp.color = MkF4(1.2f, 0.1f, 1.0f, 1.0f);
            drawDatTmp.color = MkF4(1.0f, 0.1f, 0.7f, 1.0f);

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
            //opt.priority = DRAW_POLYGON_PRIORITY_DEEPEST;
            opt.priority = (DRAW_POLYGON_PRIORITY)(DRAW_POLYGON_PRIORITY_DEEPEST + 1);
            opt.depthBuffIgnore = true;

            opt.shaderType = DPD_SHADER_TYPE_EX;
            opt.shaderTypeEx.vs = m_ShaderIndex_Ring_Vertex;
            opt.shaderTypeEx.ps = m_ShaderIndex_Ring_Pixel;

            for (int i = 0; i < 4; i++)
            {
                DRAW_POLYGON_DAT drawDatCpyTmp = drawDatTmp;
                DRAW_POLYGON_IN_CAMERA_EX_OPTION optCpy = opt;

                drawDatCpyTmp.size.x += BLOCK_WIDTH * 20.0f * i;
                drawDatCpyTmp.size.y += BLOCK_HIGHT * 20.0f * i;

                if (i == 0)
                {
                    optCpy.shaderType = {};
                    drawDatCpyTmp.color = MkF4(0.5f, 0.0f, 1.0f, 0.9f);
                    drawDatCpyTmp.size = MkF3(
                        BLOCK_WIDTH * 70.0f,
                        BLOCK_HIGHT * 60.0f,
                        BLOCK_WIDTH * 2000.0f
                    );
                    drawDatCpyTmp.pos.z = BLOCK_HIGHT * 1500.0f;

                    drawDatCpyTmp.texNo = m_TexId_MutBg0.textureId;
                    drawDatCpyTmp.texUV = {
                        0.0f + (float)GetMainGameFrame() * 0.02f,
                        0.0f,
                    };
                    drawDatCpyTmp.texWH = { 4.0f, 30.0f };
                    drawDatCpyTmp.shaderOpt.blendState = DPD_SHADER_OPT_BLEND_STATE_ADDCOLOR;
                }
                else
                {
                    if (i == 2)
                    {
                        drawDatCpyTmp.size.x *= -1.0f;
                        drawDatCpyTmp.size.y *= -1.0f;
                        drawDatCpyTmp.color = MkF4(0.5f, 0.1f, 1.0f, 1.0f);
                    }
                    else if (i == 3)
                    {
                        drawDatCpyTmp.color = MkF4(0.2f, 0.1f, 0.9f, 1.0f);
                    }
                    drawDatCpyTmp.texWH = { 1.0f, 30.0f };
                }

                drawDatCpyTmp.color.w *= (0.2f + 0.8f * dayMutationMod);

                //drawDatTmp.color = MkF4(0.3, 0.1, 0.3, 0.7f);

                //Float4 colortmp = MkF4(0.7, 0.1, 1.5, 0.9f);
                //drawDatTmp.color.x += (colortmp.x - drawDatTmp.color.x) * dayMutationMod;
                //drawDatTmp.color.y += (colortmp.y - drawDatTmp.color.y) * dayMutationMod;
                //drawDatTmp.color.z += (colortmp.z - drawDatTmp.color.z) * dayMutationMod;
                DrawPolygonInCameraEx(drawDatCpyTmp, optCpy);
            }
        }
    }


    //float mutationTransStartMod = (shaderVariable.w - (30.0f / 60.0f)) / shaderVariable.w;
    //if (isMutation && dayMutationMod >= mutationTransStartMod && dayMutationMod < 1.0f)
    //{
    //    float modtmp = (dayMutationMod - mutationTransStartMod)
    //        / (1.0f - mutationTransStartMod)
    //        ;
    //    modtmp = (sin(SYS_GENERAL_PI / 2.0f * modtmp) + 1.0f) / 2.0f;

    //    DRAW_POLYGON_DAT drawDatTmp = {};
    //    drawDatTmp.size = MkF3(
    //        +SCREEN_WIDTH,
    //        +SCREEN_HEIGHT,
    //        +1.0f);


    //    drawDatTmp.rot = 0.0f;

    //    drawDatTmp.rot3D = MkF3(0.0f, 0.0f, 0.0f);

    //    drawDatTmp.loadTexType = LOADTEXTURETYPE_MAIN;

    //    drawDatTmp.texUV = { 0.0f, 0.0f };
    //    drawDatTmp.texWH = { 0.0f, 1.0f };
    //    drawDatTmp.startScale = MkF2(1.0f, 1.0f);
    //    drawDatTmp.startScale3D = MkF3(1.0f, 1.0f, 1.0f);

    //    drawDatTmp.shaderOpt.outline = DPD_SHADER_OPT_OUTLINE_NONE;
    //    drawDatTmp.shaderOpt.bloom = DPD_SHADER_OPT_BLOOM_NONE;

    //    DRAW_POLYGON_IN_CAMERA_EX_OPTION opt = {};
    //    opt.reverse = false;
    //    opt.fixed = false;
    //    opt.enableShade = false;
    //    opt.priority = DRAW_POLYGON_PRIORITY_NML_d3;
    //    opt.depthBuffIgnore = true;

    //    drawDatTmp.modelNo = 0;


    //    {
    //        DRAW_POLYGON_DAT drawDatCpyTmp = drawDatTmp;
    //        drawDatCpyTmp.pos = {};
    //        drawDatCpyTmp.size = MkF3(
    //            +SCREEN_WIDTH * 2,
    //            +SCREEN_HEIGHT * 2,
    //            +1.0f);

    //        drawDatCpyTmp.color = MkF4(
    //            3.0f,
    //            3.0f,
    //            3.0f,
    //            1.0
    //        );
    //        drawDatCpyTmp.color.x *= modtmp;
    //        drawDatCpyTmp.color.y *= modtmp;
    //        drawDatCpyTmp.color.z *= modtmp;
    //        drawDatCpyTmp.color.w *= modtmp;

    //        drawDatCpyTmp.startPos = {};
    //        drawDatCpyTmp.texUV = { 0.0f, 0.0f };
    //        drawDatCpyTmp.texWH = { 1.0f, 1.0f };
    //        drawDatCpyTmp.texNo = 0;
    //        drawDatCpyTmp.shaderOpt.blendState = DPD_SHADER_OPT_BLEND_STATE_ADDCOLOR;
    //        DRAW_POLYGON_IN_CAMERA_EX_OPTION optCpyTmp = opt;
    //        optCpyTmp.fixed = true;

    //        DrawPolygonInCameraEx(drawDatCpyTmp, optCpyTmp);
    //    }
    //}

}

void Stage_Ruins::Retry()
{
    m_flag_set_mutation = false;
    m_flag_set_destroyStage = false;
    m_MutationFrame = -1;
    m_DestroyStageFrame = -1;

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

    for (auto& b : m_cachedMapBlocks)
    {
        if (b == nullptr) continue;
        DeleteBlock(b);
    }
    m_cachedMapBlocks.clear();
    GameMapLoad(GAME_MAP_DATA_ID_STAGE_ISEKI_01, &m_cachedMapBlocks);

    //現存する敵キャラ
    for (int i = 0; i < ENEMIES_MAX;i++)
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

    SetPostEffectShaderVariable({});

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

    { //ボスのオブジェクトだけ生成しておく
        GAME_OBJECT* co = nullptr;
        co = Character::CreateCharacterGameObject(m_UseChara[4]);
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
    SetPostEffectShaderPS("SHADER/ps_world_iseki");

    //背景の読み込み
    {
        //SetBg(BG_TYPE_TEST_GAME);
        //SetBg(BG_TYPE_GENERAL_0);

        SetBg(BG_TYPE_ISEKI);

        SetUIScoreVisible_Game(true);
        SetUITimerVisible_Game(true);
    }

    {
        //BGMを止める(タイトルからここまで)
        SoundBGMStopRequest();

        bool isBoss =
            (m_CurrentWave == 3 && m_Difficulty == Difficulty::Hard)
            || m_CurrentWave == 4
        ;

        if (!isBoss)
        {
            //BGMを再生する
            SoundBGMPlayRequest(
                GetSoundData("SOUND/BGM/ISEKI_LONG"),
                //(FRAME_ONE_MINUTE * 1) 
                +(FRAME_ONE_SECOND * 41),
                1.0f);
        }
        //SoundBGMPlayRequest(SOUND_TYPE_BGM_2, (60 * 60 * 1) + (60 * 42), 1.0f);
    }
}

void Stage_Ruins::Clear()
{
    int retryCount = 0;
    {
        Stage_Base* stg = GetCurrentStageSceneGame();
        if (stg != nullptr)
        {
            retryCount = stg->GetRetryCount();

            if (retryCount <= 0)
            {
                //int count = stg->GetCountFrame();
                //int countMax = stg->GetCountFrameMax();

                //GameManagerResultAddScore(
                //    4000 * std::min(std::max((float)count / (FRAME_ONE_MINUTE * 3), 0.0f), 1.0f),
                //    RESULT_SCORE_EX_TYPE_SPEED_CLEAR
                //);
                //GameManagerResultAddScore(
                //    9999 * std::min(std::max((float)count / countMax, 0.0f), 1.0f),
                //    RESULT_SCORE_EX_TYPE_SPEED_CLEAR
                //);
                GameManagerResultAddScore(
                    9999 * pow(1.0f - std::min(std::max(
                        (float)stg->GetClearTimeFrame() / stg->GetSpeedClearTimeFrame(),
                        0.0f), 1.0f),
                        2.0f),
                    RESULT_SCORE_EX_TYPE_SPEED_CLEAR
                );
            }

            //タイマーストップ
            stg->SetCountStop(true);

            GameManagerSetResultClearTime(stg->GetClearTimeFrame());
        }
    }
    {
        //int totalHp = 0;
        //int totalMaxHp = 0;
        //for (int i = 0; i < PLAYER_MAX; i++)
        //{
        //    PLAYER* player = GetPlayer(i);
        //    GAME_OBJECT* playerObj = player->obj;
        //    if (playerObj != nullptr)
        //    {
        //        totalHp += playerObj->status.hp;
        //        totalMaxHp += playerObj->status.hpMax;
        //    }
        //}
        //GameManagerResultAddScore(
        //    5000 * std::min(std::max((float)totalHp / totalMaxHp, 0.0f), 1.0f),
        //    RESULT_SCORE_EX_TYPE_DAMAGE_LESS
        //);

        if (retryCount <= 0)
        {
            GameManagerResultAddScore(
                5000
                * (1.0f - std::min(std::max(
                    (float)GetTotalPlayerDamage() / (1000 * GetActivePlayerNum()),
                    0.0f), 1.0f)),
                RESULT_SCORE_EX_TYPE_DAMAGE_LESS
            );
        }
    }

    { //ランク決め
        int total_score = GameManagerGetResult().score;
        if (30000 <= total_score)
        {//SS
            GameManagerSetResultRank(4);
        }
        else if (25000 <= total_score)
        {//S
            GameManagerSetResultRank(3);
        }
        else if (15000 <= total_score)
        {//A
            GameManagerSetResultRank(2);
        }
        else if (10000 <= total_score)
        {//B
            GameManagerSetResultRank(1);
        }
        else
        {//C 
            GameManagerSetResultRank(0);
        }
    }

    //GameManagerSetSceneRequest(SCENE_RESULT, m_Dead_FrameMax, true);
    GameManagerSetSceneRequest(SCENE_RESULT, 360, true);
}

void Stage_Ruins::GiveUp()
{
    GameManagerSetSceneRequest(SCENE_RESULT, 0, false);
}


void Stage_Ruins::StageEventMutation(bool isFast)
{
    if (isFast)
    {
        set_mutation();
        set_destroyStage();

        m_MutationFrame = (int)(FRAME_ONE_SECOND * 0.75 + FRAME_ONE_SECOND) + 1;
        m_DestroyStageFrame = (int)(FRAME_ONE_SECOND * 0.25) + 1;
    }
    else
    {
        m_flag_set_mutation = true;

        m_MutationFrame = 0;
    }
}

void Stage_Ruins::StageEventDestroy()
{
    if (m_DestroyStageFrame < 0)
    {
        m_flag_set_destroyStage = true;

        m_DestroyStageFrame = 0;
    }
}

void Stage_Ruins::StageDynamicBlockClear()
{
    for (const auto& b : m_cachedMapDynamicBlocks)
    {
        GAME_OBJECT* ob = GetGameObjectFindByIdentifier(b);
        if (!IsActiveGameObject(ob))
        {
            continue;
        }

        GOE_ObjectDynamic* goe =
            dynamic_cast<GOE_ObjectDynamic*>(ob->events.source);  // ダウンキャスト

        if (goe != nullptr)
        {
            goe->Clear();
        }
    }
    m_cachedMapDynamicBlocks.clear();
}


void Stage_Ruins::update_start()
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
                                    BLOCK_HIGHT* m_MapSize.y / 2 - BLOCK_HIGHT * 10.0f
                                    + addpos.y,
                                    playerObj->pos.z
                                ),
                                EasingOutSine
                            );
                        }
                    }
                }
            }

            //if (m_StartFrame == 0 + FRAME_ONE_SECOND * 0.5f)
            //{
            //    m_StartSignFrame = 0;
            //}

            if (m_StartFrame == 0 + FRAME_ONE_SECOND * 0.5f + STARTING_FRAME)
            {
                //GameManagerSetMovieFlag(false);

                SetCountStop(false);
            }

        }
        else
        {
            constexpr int startingF = FRAME_ONE_SECOND * 8;

            if (m_StartFrame == 0)
            {
                GameManagerSetMovieFlag(true, true);
                UIMovieFlagInstant_Game();

                SetCameraModeAll(CAMERA_MODE_NORMAL);

                m_StartStageNameFrame = 0;
            }
            if (m_StartFrame == FRAME_ONE_SECOND * 0)
            {
                Float3 zoomPos = {};
                zoomPos.x += -BLOCK_WIDTH * m_MapSize.x / 2 * 0.3f;
                zoomPos.z += -1000.0f;

                SetCameraTargetPosAll(MkF3(
                    zoomPos.x - 4000.0f,
                    zoomPos.y + 5000.0f,
                    zoomPos.z + 3000.0f
                ));
                SetCameraPosAll(zoomPos);
                SetCameraZoomAll(0.35f);
                CameraFlashRequestAll(16, EasingOutSine, MkF4(-1.0f, -1.0f, -1.0f, 1.0f));

                Float3 zoomPos2 = {};
                zoomPos2.x += BLOCK_WIDTH * m_MapSize.x / 2 * 0.3f;
                zoomPos2.z += -1000.0f;
                Float3 zoomPos2Ta = zoomPos2;
                zoomPos2Ta.x += 4000.0f;
                //zoomPos2Ta.y += 35000.0f;
                //zoomPos2Ta.z += -5500.0f;

                CameraMoveRequestAll(FRAME_ONE_SECOND * 5.0f,
                    zoomPos2,
                    0.0f,
                    MkF2(1.0f, 1.0f),
                    0.35f,
                    EasingInOutSine,
                    CAMERA_MOVE_TYPE_SMOOTH
                );
                CameraTargetMoveRequestAll(FRAME_ONE_SECOND * 5.0f,
                    zoomPos2Ta,
                    EasingInOutSine,
                    CAMERA_MOVE_TYPE_SMOOTH
                );
            }
            if (m_StartFrame == FRAME_ONE_SECOND * 5)
            {
                Float3 zoomPos = {};
                zoomPos.x += 0.0f;
                zoomPos.y += BLOCK_HIGHT * m_MapSize.y / 2 * 0.3f;
                zoomPos.z += -6000.0f;

                {
                    GAME_OBJECT* co = GetGameObjectFindByIdentifier(m_BossObj);
                    if (co != nullptr)
                    {
                        zoomPos.x = co->pos.x;
                    }
                }

                SetCameraTargetPosAll(MkF3(zoomPos.x, zoomPos.y + 5000.0f, zoomPos.z + 3000.0f));
                SetCameraPosAll(zoomPos);
                SetCameraZoomAll(0.3f);
                CameraFlashRequestAll(16, EasingOutSine, MkF4(-1.0f, -1.0f, -1.0f, 1.0f));

                Float3 zoomPos2 = {};
                zoomPos2.x = zoomPos.x;
                zoomPos2.y += -BLOCK_HIGHT * m_MapSize.y / 2 * 0.6f;
                Float3 zoomPos2Ta = zoomPos2;
                zoomPos2Ta.y += 35000.0f;
                zoomPos2Ta.z += -5500.0f;

                CameraMoveRequestAll(FRAME_ONE_SECOND * 5.0f,
                    zoomPos2,
                    0.0f,
                    MkF2(1.0f, 1.0f),
                    0.4f,
                    EasingOutSine,
                    CAMERA_MOVE_TYPE_SMOOTH
                );
                CameraTargetMoveRequestAll(FRAME_ONE_SECOND * 5.0f,
                    zoomPos2Ta,
                    EasingOutSine,
                    CAMERA_MOVE_TYPE_SMOOTH
                );
            }

            //ムービースキップ処理 (多分後で変える)
            if (m_StartFrame < startingF
                && GameManagerIsMovieSkipTrigger()
                )
            {
                GameManagerSetMovieFlag(true, false);
                m_StartStageNameFrame = -1;
                m_StartFrame = startingF;
            }

            if (m_StartFrame == startingF)
            {
                CameraFlashRequestAll(20, EasingOutSine, MkF4(-1.0f, -1.0f, -1.0f, 1.0f));

                CameraMoveCancelRequestAll();
                CameraTargetMoveCancelRequestAll();

                //{
                //    Float3 zoomPos = {};
                //    zoomPos.x = 0.0f;
                //    //zoomPos.y = BLOCK_HIGHT * m_MapSize.y / 2;
                //    zoomPos.z = 12000.0f;

                //    SetCameraTargetPosAll(MkF3(
                //        zoomPos.x,
                //        zoomPos.y - 10.0f,
                //        -21000.0f
                //    ));
                //    SetCameraPosAll(zoomPos);
                //    SetCameraZoomAll(0.2f);
                //}

                SetCameraModeAll(CAMERA_MODE_PLAYER_CHASE_INSTANT);
            }
            if (m_StartFrame == startingF + 1)
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

            if (m_StartFrame == startingF + FRAME_ONE_SECOND * 0.5f)
            {
                m_StartSignFrame = 0;
            }

            if (m_StartFrame == startingF + FRAME_ONE_SECOND * 0.5f + STARTING_FRAME)
            {
                GameManagerSetMovieFlag(false);

                SetCameraModeAll(CAMERA_MODE_PLAYER_CHASE);

                SetCountStop(false);

                GE_GayaStartRequest("game_start_mayu");
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

void Stage_Ruins::draw_start()
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

        for(int i = 0; i < 2; i++)
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

void Stage_Ruins::update_gameover()
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

            SoundEffectStopAllRequest();

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
            GE_GayaStartRequest("game_over_mayu");
        }

        if (m_CountFrame <= 0)
        { //タイムアップ
            gameoverflag = true;

            SoundEffectStopAllRequest();

            //SoundEffectPlayRequest(SOUND_TYPE_SE_9, 10.0f, 0.0f);

            //GameManagerSetMovieFlag(true);

            //CameraFlashRequestAll(
            //    180,
            //    EasingReverse,
            //    MkF4(0.0f, 0.0f, 1.0f, 0.9f),
            //    DPD_SHADER_OPT_BLEND_STATE_ADDCOLOR
            //);
            m_GameoverFadeColor = MkF4(0.0f, 0.0f, 1.0f, 0.9f);

            GameStopRequest(120);

            GE_GayaCancelRequestAll();
            GE_GayaStartRequest("game_timeup_mayu");
        }

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

bool Stage_Ruins::update_wave()
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

void Stage_Ruins::updating_wave()
{
    if (m_CurrentWave == 0)
    {
        StageDynamicBlockClear();
        create_map_objects(m_WaveEnemiesAndObjects[0]);
    }

    if (m_CurrentWave == 1)
    {
        StageDynamicBlockClear();

        GE_GayaStartRequest("game_wave_first_mayu");

        {
            {
                int ucn = (rand() % 1000 < 200) ? 1 : 0;
                GAME_OBJECT* co = Character::CreateCharacterGameObject(m_UseChara[ucn]);
                if (co != nullptr)
                {
                    co->pos = MkF3(
                        ((float)(rand() % 1000) / 1000 - 0.5f) * (m_MapSize.x * BLOCK_WIDTH - BLOCK_WIDTH * 50),
                        ((float)(rand() % 1000) / 1000 - 0.5f) * (m_MapSize.y * BLOCK_HIGHT - BLOCK_HIGHT * 50),
                        0.0f);
                    {
                        GOE_Character_Enemy_Miresu* goe =
                            dynamic_cast<GOE_Character_Enemy_Miresu*>(co->events.source);  // ダウンキャスト

                        if (goe != nullptr)
                        { //
                            if (ucn == 1)
                            {
                                goe->SetType(GOE_Character_Enemy_Miresu::Type::Rapid);
                            }
                            else
                            {
                                goe->SetType(
                                    (GOE_Character_Enemy_Miresu::Type)
                                    (
                                        (int)GOE_Character_Enemy_Miresu::Type::Hard
                                        + (rand() % 4)
                                        )
                                );
                            }
                            goe->BecomeApp();
                        }
                    }
                    //m_Enemies.push_back(GetIdentifierGameObject(co));
                    EnemyRegister(GetIdentifierGameObject(co));
                }
            }


            create_map_objects(m_WaveEnemiesAndObjects[1]);
        }

    }

    if (m_CurrentWave == 2)
    {
        StageDynamicBlockClear();

        if (GetActivePlayerNum() > 1)
        {
            GE_GayaStartRequest("game_wave_middle_multi_mayu");
        }
        else
        {
            GE_GayaStartRequest("game_wave_middle_solo_mayu");
        }

        create_map_objects(m_WaveEnemiesAndObjects[2]);

    }

    if (m_CurrentWave == 3)
    {
        StageDynamicBlockClear();

        if (m_Difficulty == Difficulty::Hard)
        {
            {
                GAME_OBJECT* co = nullptr;
                co = Character::CreateCharacterGameObject("ENEMY_BOSS_KONTORIUSU");
                if (co != nullptr)
                {
                    //co->pos = MkF3(500.0f, -800.0f,
                    //    0.0f - 32767.0f
                    //);
                    //co->pos = MkF3(500.0f, -1500.0f,
                    //    0.0f - 32767.0f
                    //);
                    co->pos = MkF3(0.0f, -1500.0f,
                        0.0f - 32767.0f
                    );

                    {
                        GOE_Character_Enemy_Boss_Kontoriusu* goe =
                            dynamic_cast<GOE_Character_Enemy_Boss_Kontoriusu*>(co->events.source);  // ダウンキャスト

                        if (goe != nullptr)
                        { //
                            goe->BecomeHard();
                        }
                    }

                    //m_BossObj = GetIdentifierGameObject(co);
                    EnemyRegister(GetIdentifierGameObject(co));
                    m_EnemyBoss = GetIdentifierGameObject(co);
                }
            }
        }
        else
        {
            if (GetActivePlayerNum() > 1)
            {
                GE_GayaStartRequest("game_wave_final_multi_mayu");
            }
            else
            {
                GE_GayaStartRequest("game_wave_final_solo_mayu");
            }

            {
                GOE_Character_Player* sourcePlayer = nullptr;
                {
                    PLAYER* player = GetPlayer(0);
                    GAME_OBJECT* playerObj = player->obj;
                    if (playerObj != nullptr)
                    {
                        sourcePlayer =
                            dynamic_cast<GOE_Character_Player*>(playerObj->events.source);  // ダウンキャスト
                    }
                }

                if (sourcePlayer != nullptr && !sourcePlayer->IsHealIgnore())
                {
                    int delay = FRAME_ONE_SECOND * 3.0f;

                    std::string gayaname ;
                    if (sourcePlayer->IsPinch())
                    {
                        gayaname = "game_give_item_heal_pinch_mayu";
                    }
                    else
                    {
                        gayaname = "game_give_item_heal_normal_mayu";
                    }

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
                        sourcePlayer->GetGameObj()
                    );
                    tsptr->StartTransport(delay);
                }
            }

            create_map_objects(m_WaveEnemiesAndObjects[3]);
        }
    }

    if (m_CurrentWave == 4)
    {
        StageDynamicBlockClear();

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


    }

}

void Stage_Ruins::update_current_wave()
{
}

void Stage_Ruins::update_mutation()
{
    if (m_MutationFrame >= 0)
    {
        Float3 bossPosTmp = {};
        {
            GAME_OBJECT* co = GetGameObjectFindByIdentifier(m_EnemyBoss);
            if (co != nullptr)
            {
                bossPosTmp = co->pos;
                bossPosTmp.z += BLOCK_HIGHT * -1.0f;
            }
        }


        if (m_MutationFrame == 0)
        {
            //SEを再生
            SoundEffectPlayRequest(GetSoundData("SOUND/SE/etc/gareki"),
                3.0f, 0, 0.6f);
        }

        if (m_MutationFrame == (int)(FRAME_ONE_SECOND * 0.75 + FRAME_ONE_SECOND) - 20)
        {
            //SEを再生
            SoundEffectPlayRequest(GetSoundData("SOUND/SE/etc/zusyan"),
                2.0f, 0, 0.7f);

            CameraShakeRequestAll(
                CAMERA_SHAKE_EXLARGE_FRAME * 1.0,
                CAMERA_SHAKE_EXLARGE_POWER * 2.0
            );

            {
                Float3 postmp = bossPosTmp;

                GAME_OBJECT* go = GO_CreateEffect(
                    GO_EFFECT_TYPE_OVERLAY_BALL,
                    postmp);

                if (go != nullptr)
                {
                    go->graphic.color = MkF4(
                        2.0f,
                        0.1f,
                        0.9f,
                        1.0f
                    );
                    go->scale = MkF3(
                        5000.0f,
                        5000.0f,
                        5000.0f
                    );
                    GameObjectSetLifeTime(go, 20);
                }
            }
        }

        if (m_MutationFrame == (int)(FRAME_ONE_SECOND * 0.75 + FRAME_ONE_SECOND))
        //if (m_MutationFrame == 60)
        {
            m_flag_set_mutation = false;
            set_mutation();

            //CameraFlashRequestAll(
            //    40,
            //    EasingInOutSine,
            //    MkF4(0.0f, 0.0f, 0.0f, 2.0f),
            //    DPD_SHADER_OPT_BLEND_STATE_DEFAULT
            //);
            CameraFlashRequestAll(
                90,
                EasingInOutSine,
                MkF4(2.0f, 2.0f, 2.0f, 2.0f),
                DPD_SHADER_OPT_BLEND_STATE_ADDCOLOR
            );

            CameraShakeRequestAll(
                CAMERA_SHAKE_EXLARGE_FRAME * 1.2,
                CAMERA_SHAKE_EXLARGE_POWER * 7.0,
                CAMERA_SHAKE_EXLARGE_POWER * 5.8
            );

            GO_CreateExplosion(bossPosTmp, 10.0f);

            StageEventDestroy();
        }


        m_MutationFrame++;
    }
}

void Stage_Ruins::update_destroyStage()
{
    if (m_DestroyStageFrame >= 0)
    {
        bool isMutation = GetBg().type == BG_TYPE_ISEKI_MUTATION;

        if (m_DestroyStageFrame == 0)
        {
            if (!isMutation)
            {
                SetPostEffectShaderVS("SHADER/vs_general2");
                SetPostEffectShaderPS("SHADER/ps_world_iseki_destroystage");
            }

            //SEを再生
            SoundEffectPlayRequest(SOUND_TYPE_SE_9,
                5.0f, 0.0f, 0.3f);
            //SEを再生
            SoundEffectPlayRequest(
                GetSoundData("SOUND/SE/enemy/attack/Big_Explosion"),
                6.5f, 0, 0.6f);

            //SEを再生
            SoundEffectPlayRequest(GetSoundData("SOUND/SE/etc/kuzureru"),
                0.7f, 10, 0.2f);

            m_flag_set_destroyStage = false;
            set_destroyStage();
        }


        if (m_DestroyStageFrame >= (int)(FRAME_ONE_SECOND * 0.25))
        {
            for (auto& b : m_cachedMapBlocks)
            {
                if (b == nullptr) continue;
                if (b->isUse)
                {
                    if (b->isBackground)
                    {
                        float seed = sin(cos(b->pos.x * 12.9898 + b->pos.y * 78.233) * 43758.5453) * 8472.5453;

                        float speedmod = std::max((int)m_DestroyStageFrame / (FRAME_ONE_SECOND * 50.0f), 1.0f);
                        //speedmod = EasingInQuad(speedmod);
                        speedmod = pow(speedmod, 10.0f);
                        speedmod *= 0.2f;

                        //if((m_DestroyStageFrame + (int)seed) % 40 == 0)
                        //{
                        //    Float3 postmp2 = Float2ToFloat3(
                        //        b->pos,
                        //        b->posZ + BLOCK_HIGHT * -8.0f
                        //    );
                        //    postmp2.x += sin(seed * m_DestroyStageFrame) * BLOCK_WIDTH * 3.0f;
                        //    postmp2.y += cos(seed * m_DestroyStageFrame) * BLOCK_HIGHT * 3.0f;

                        //    GAME_OBJECT* go = GO_CreateEffect(
                        //        //GO_EFFECT_TYPE_EXPLOSION_SMOKE,
                        //        GO_EFFECT_TYPE_EXPLOSION_1,
                        //        postmp2);

                        //    if (go != nullptr)
                        //    {
                        //        go->graphic.color = MkF4(
                        //            1.0f,
                        //            1.0f,
                        //            1.0f,
                        //            1.0f
                        //        );
                        //        go->scale = MkF3(
                        //            (b->sprite.size.x + b->sprite.size.y) / 2.0f / 300.0f,
                        //            (b->sprite.size.x + b->sprite.size.y) / 2.0f / 300.0f,
                        //            (b->sprite.size.x + b->sprite.size.y) / 2.0f / 300.0f
                        //        );
                        //        GameObjectSetLifeTime(go, 40 + 20 * sin(seed));
                        //    }
                        //}

                        if (b->type == BLOCK_TYPE_ISEKI_HAIKEI_07)
                        {
                            b->posZ += (96.0f + 12.0f * sin(seed + 3.2f)) * speedmod;
                        }
                        else if (b->type == BLOCK_TYPE_ISEKI_HAIKEI_04)
                        {
                            b->posZ += (10.0f + 3.0f * cos(seed + 17.2f)) * speedmod;
                            if (b->sprite.rot3D.y < 90.0f && b->sprite.rot3D.y > -90.0f)
                            {
                                b->sprite.rot3D.y += ((6.0f + 0.1f * sin(seed))
                                    * (cos(seed + 17.2f) > 0 ? 1.0f : -1.0f)) * speedmod;
                            }
                        }
                        else
                        {
                            b->posZ += (10.0f + 3.0f * cos(seed + 17.2f)) * speedmod;
                            b->sprite.rot3D.y += ((0.08f + 0.06f * sin(seed))
                                * (cos(seed + 17.2f) > 0 ? 1.0f : -1.0f)) * speedmod;
                        }

                        if (b->posZ > BLOCK_HIGHT * 100.0f)
                        {
                            DeleteBlock(b);
                            b = nullptr;
                        }
                    }
                }
            }
        }


        m_DestroyStageFrame++;
    }
}

void Stage_Ruins::set_mutation()
{
    if (m_flag_set_mutation)
    {
        return;
    }
    m_flag_set_mutation = true;


    SetBg(BG_TYPE_ISEKI_MUTATION);

    //ステージ用のシェーダ読み込み
    //SetPostEffectShader("SHADER/shader_world_iseki_mutation");
    SetPostEffectShaderVS("SHADER/vs_general2");
    SetPostEffectShaderPS("SHADER/ps_world_iseki_mutation");
    SetPostEffectShaderVariable((float)GetMainGameFrame(), 0);
    SetPostEffectShaderVariable(1.0f, 1);
    SetPostEffectShaderVariable(1.0f, 2);
    SetPostEffectShaderVariable(-1.0f, 3);
}
void Stage_Ruins::set_destroyStage()
{
    if (m_flag_set_destroyStage)
    {
        return;
    }
    m_flag_set_destroyStage = true;

    for (auto& b : m_cachedMapBlocks)
    {
        if (b == nullptr) continue;
        if (b->isUse)
        {
            float seed = sin(cos(b->pos.x * 12.9898 + b->pos.y * 78.233) * 43758.5453) * 8472.5453;

            if (b->isBackground)
            {
                //GO_CreateExplosion(Float2ToFloat3(b->pos), 2.0f);
            }
            else
            {
                //GO_CreateExplosion(Float2ToFloat3(b->pos),
                //    (b->sprite.size.x + b->sprite.size.y) / 2.0f / 1000.0f
                //);

                Float2 postmp = b->pos;
                Float2 sizetmp = b->sprite.size;


                {
                    Float3 postmp2 = Float2ToFloat3(
                        postmp,
                        BLOCK_HIGHT * -(5.0f + 2.0f * sin(seed))
                    );

                    if (postmp2.y < 0.0f)
                    {

                        GAME_OBJECT* go = GO_CreateEffect(
                            GO_EFFECT_TYPE_EXPLOSION_1,
                            postmp2);

                        if (go != nullptr)
                        {
                            go->graphic.color = MkF4(
                                1.0f,
                                1.0f,
                                1.0f,
                                1.0f
                            );
                            go->scale = MkF3(
                                (b->sprite.size.x + b->sprite.size.y) / 2.0f / 40.0f,
                                (b->sprite.size.x + b->sprite.size.y) / 2.0f / 40.0f,
                                (b->sprite.size.x + b->sprite.size.y) / 2.0f / 40.0f
                            );
                            GameObjectSetLifeTime(go, 120 + 40 * sin(seed));
                        }
                    }
                }

                //if (
                //       b->type == BLOCK_TYPE_ISEKI_0_Hasira1
                //    || b->type == BLOCK_TYPE_ISEKI_0_Dark_Hasira1
                //)
                //{
                //    *b = GetBlockPrefab(BLOCK_TYPE_ISEKI_0_HasiraS1);
                //    b->pos = postmp;
                //}
                //else if (
                //    b->type == BLOCK_TYPE_ISEKI_0_Hasira2
                //    || b->type == BLOCK_TYPE_ISEKI_0_Dark_Hasira2
                //)
                //{
                //    *b = GetBlockPrefab(BLOCK_TYPE_ISEKI_0_HasiraS2);
                //    b->pos = postmp;
                //}
                //else if (
                //    b->type == BLOCK_TYPE_ISEKI_0_Hasira3
                //    || b->type == BLOCK_TYPE_ISEKI_0_Dark_Hasira3
                //    )
                //{
                //    *b = GetBlockPrefab(BLOCK_TYPE_ISEKI_0_HasiraS3);
                //    b->pos = postmp;
                //}
                if (
                    b->type == BLOCK_TYPE_ISEKI_0_Hasira1
                    || b->type == BLOCK_TYPE_ISEKI_0_Dark_Hasira1
                    || b->type == BLOCK_TYPE_ISEKI_0_Hasira2
                    || b->type == BLOCK_TYPE_ISEKI_0_Dark_Hasira2
                    || b->type == BLOCK_TYPE_ISEKI_0_Hasira3
                    || b->type == BLOCK_TYPE_ISEKI_0_Dark_Hasira3
                    )
                {
                    *b = GetBlockPrefab((BLOCK_TYPE)(BLOCK_TYPE_ISEKI_0_Dark_HasiraS1
                        + (int)(abs(seed * 334.0f)) % 4));
                    b->pos = postmp;
                }
                else if (
                    b->type == BLOCK_TYPE_ISEKI_0_Taimatsu
                    || b->type == BLOCK_TYPE_ISEKI_0_Dark_Taimatsu
                    )
                {
                    //仮
                    DeleteBlock(b);
                    b = nullptr;
                }
                else if (
                    b->type == BLOCK_TYPE_ISEKI_0_Block
                    || b->type == BLOCK_TYPE_ISEKI_0_Dark_Block
                    )
                {
                    DeleteBlock(b);
                    b = nullptr;
                }
                else
                {
                    *b = GetBlockPrefab(BLOCK_TYPE_ISEKI_0_Dark_Block);
                    b->pos = postmp;
                    b->sprite.size = sizetmp;
                }

                if (b != nullptr)
                {
                    b->sprite.color.x += -0.2f;
                    b->sprite.color.y += -0.3f;
                    b->sprite.color.z += -0.3f;
                }
                //DeleteBlock(b);
                //b = nullptr;
            }
        }
    }
}

void Stage_Ruins::create_map_objects(const StageObjList& objList)
{
    bool isFirst = m_CurrentWave == 0;

    int index = 0;
    int indexBlock = 0;
    for (const auto& stgObj : objList)
    {
        if (stgObj.type == 4)
        {
            Float2 pointPos = CalculateCentroid(
                Float3ToFloat2(stgObj.pt),
                Float3ToFloat2(stgObj.pt2)
            );

            GAME_OBJECT_COLLIDER collider = {};
            collider.type = GAME_OBJECT_COLLIDER_TYPE_CAPSULE;

            collider.capsule.radius = stgObj.r;

            collider.capsule.pointA = MkF3(
                stgObj.pt.x - pointPos.x,
                stgObj.pt.y - pointPos.y,
                0.0f
            );
            collider.capsule.pointB = MkF3(
                stgObj.pt2.x - pointPos.x,
                stgObj.pt2.y - pointPos.y,
                0.0f
            );
            collider.capsule.pointA.z = collider.capsule.radius * 0.75f;
            collider.capsule.pointB.z = collider.capsule.radius * 0.75f;

            GOE_ObjectDynamic* goe = GOE_ObjectDynamic::CreateObjectDynamic(
                GOE_ObjectDynamic::Type::General,
                MkF3(
                    pointPos.x,
                    pointPos.y,
                    isFirst ? 0.0f : BLOCK_WIDTH * -100.0f
                ),
                collider
            );
            //goe->SetOwner(GetIdentifierGameObject(m_Obj));

            if (!isFirst)
            {
                goe->StartFall(2 * FRAME_ONE_SECOND);

                GameObjectHitStop(
                    goe->GetGameObj(),
                    25 * indexBlock + 30
                );
            }

            m_cachedMapDynamicBlocks.push_back(GetIdentifierGameObject(goe->GetGameObj()));

            indexBlock++;
        }
        else
        {
            GAME_OBJECT* co = Character::CreateCharacterGameObject(m_UseChara[stgObj.type]);

            if (co != nullptr)
            {
                co->pos = MkF3(
                    stgObj.pt.x,
                    stgObj.pt.y,
                    0.0f);

                if(!isFirst)
                {
                    GOE_Character_Enemy* goe =
                        dynamic_cast<GOE_Character_Enemy*>(co->events.source);  // ダウンキャスト

                    if (goe != nullptr)
                    { //
                        goe->BecomeApp();
                    }
                }

                EnemyRegister(GetIdentifierGameObject(co));
            }
        }

        index++;
    }
}
