// ===================================================
// Stage_Tutorial.h ステージ処理
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

#include "Stage_Tutorial.h"

#include "../Game Object/Game Object Events/GOE_ObjectDynamic.h"
#include "../Game Object/Game Object Events/GOE_Character_Player.h"
#include "../Game Object/Game Object Events/GOE_Character_Enemy/GOE_Character_Enemy_Miresu.h"
#include "../Game Object/Game Object Events/GOE_Character_Enemy/GOE_Character_Enemy_Regatosu.h"
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



void Stage_Tutorial::Init()
{
    SuperClass::Init(); //必ず呼ぶ

    m_Model_Yuka = GetModelDataIndex("MODEL/block/general1");
    m_Model_Ring = GetModelDataIndex("MODEL/beam_01");

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
        SetUIScoreVisible_Game(false);
        SetUITimerVisible_Game(false);
    }

    m_MapSize = { 90, 45 };

    for (int i = 0; i < PLAYER_MAX; i++)
    {
        PLAYER* player = GetPlayer(i);
        GAME_OBJECT* playerObj = player->obj;
        if (playerObj != nullptr)
        {
            playerObj->pos = {
                0.0f,
                BLOCK_HIGHT * -5.0f
            };
        }
    }


    {
        //GAME_OBJECT co = GetGameObjectPrefab(GAME_OBJECT_TYPE_DUMMY);
        //co.pos = {};
        //co.collider.type = GAME_OBJECT_COLLIDER_TYPE_CAPSULE;
        //co.collider.capsule.radius = BLOCK_WIDTH * 3.0f;
        //co.collider.capsule.pointA.x = BLOCK_WIDTH * 3.0f;
        //co.collider.capsule.pointA.y = BLOCK_WIDTH * 3.0f;
        //co.collider.capsule.pointB.x = BLOCK_WIDTH * -3.0f;
        //co.collider.capsule.pointB.y = BLOCK_WIDTH * -3.0f;

        //GAME_OBJECT* coCrd = CreateGameObject(co);
        //if (coCrd != nullptr)
        //{
        //    goe_pp = new GOE_ObjectDynamic(coCrd);

        //    if (goe_pp != nullptr)
        //    { //初期化イベント

        //        coCrd->events.source = goe_pp;
        //        coCrd->events.source->Init();
        //    }
        //}

        {
            GAME_OBJECT_COLLIDER collider = {};
            collider.type = GAME_OBJECT_COLLIDER_TYPE_CAPSULE;
            collider.capsule.radius = BLOCK_WIDTH * 5.0f;
            collider.capsule.pointA.x = BLOCK_WIDTH * 3.0f;
            collider.capsule.pointA.y = BLOCK_WIDTH * -3.0f;
            collider.capsule.pointB.x = BLOCK_WIDTH * -3.0f;
            collider.capsule.pointB.y = BLOCK_WIDTH * 3.0f;
            collider.capsule.pointA.z = collider.capsule.radius * 0.75f;
            collider.capsule.pointB.z = collider.capsule.radius * 0.75f;

            GOE_ObjectDynamic::CreateObjectDynamic(
                GOE_ObjectDynamic::Type::General,
                MkF3(BLOCK_WIDTH * -20.0f, 0.0f, 0.0f),
                collider
            );
        }
        //{
        //    GAME_OBJECT_COLLIDER collider = {};
        //    collider.type = GAME_OBJECT_COLLIDER_TYPE_CAPSULE;
        //    collider.capsule.radius = BLOCK_WIDTH * 3.0f;
        //    collider.capsule.pointA.x = BLOCK_WIDTH * 3.0f;
        //    collider.capsule.pointA.y = BLOCK_WIDTH * 3.0f;
        //    collider.capsule.pointB.x = BLOCK_WIDTH * -3.0f;
        //    collider.capsule.pointB.y = BLOCK_WIDTH * -3.0f;
        //    collider.capsule.pointA.z = collider.capsule.radius * 0.75f;
        //    collider.capsule.pointB.z = collider.capsule.radius * 0.75f;

        //    GOE_ObjectDynamic::CreateObjectDynamic(
        //        GOE_ObjectDynamic::Type::General,
        //        MkF3(BLOCK_WIDTH * 0.0f, 0.0f, 0.0f),
        //        collider
        //    );
        //}
        {
            GAME_OBJECT_COLLIDER collider = {};
            collider.type = GAME_OBJECT_COLLIDER_TYPE_CAPSULE;
            collider.capsule.radius = BLOCK_WIDTH * 2.0f;
            collider.capsule.pointA.x = BLOCK_WIDTH * 3.0f;
            collider.capsule.pointA.y = BLOCK_WIDTH * 5.0f;
            collider.capsule.pointB.x = BLOCK_WIDTH * -3.0f;
            collider.capsule.pointB.y = BLOCK_WIDTH * -4.0f;
            collider.capsule.pointA.z = collider.capsule.radius * 0.75f;
            collider.capsule.pointB.z = collider.capsule.radius * 0.75f;

            GOE_ObjectDynamic::CreateObjectDynamic(
                GOE_ObjectDynamic::Type::General,
                MkF3(BLOCK_WIDTH * 20.0f, 0.0f, 0.0f),
                collider
            );
        }
    }

    //{
    //    //BGMを止める(タイトルからここまで)
    //    SoundBGMStopRequest();

    //    //BGMを再生する
    //    SoundBGMPlayRequest(
    //        GetSoundData("SOUND/BGM/BGM2_DEMO"), 
    //        //(FRAME_ONE_MINUTE * 1) 
    //        + (FRAME_ONE_SECOND * 41),
    //        1.0f);
    //    //SoundBGMPlayRequest(SOUND_TYPE_BGM_2, (60 * 60 * 1) + (60 * 42), 1.0f);
    //}

}

void Stage_Tutorial::Uninit()
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


void Stage_Tutorial::Update()
{
    SuperClass::Update();

    if (m_WaveCountWait == -1)
    {
        int activeEnemNum = 0;

        for (int i = 0; i < ENEMIES_MAX; i++)
        {
            GAME_OBJECT* enemObj = GetGameObjectFindByIdentifier(m_Enemies[i]);
            if (IsActiveGameObject(enemObj))
            {
                activeEnemNum++;
            }
        }

        if (activeEnemNum < 1)
        { //ウェーブを進める

            m_WaveCountWait = 1;
            m_CurrentWave++;

            for (int i = 0; i < ENEMIES_MAX; i++)
            {
                GAME_OBJECT* enemObj = GetGameObjectFindByIdentifier(m_Enemies[i]);
                if (!IsActiveGameObject(enemObj))
                {
                    m_Enemies[i] = {};
                }
            }
            return;
        }
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
        return;
    }

    {

        //GE_GayaStartRequest("iseki_wave1_mayu");

        {
            for (int i = 0; i < 3; i++)
            {
                (void)rand();
                GAME_OBJECT* co = nullptr;
                {
                    co = Character::CreateCharacterGameObject(m_UseChara[0]);

                    if (co != nullptr)
                    {
                        co->pos = MkF3(
                            ((float)(rand() % 1000) / 1000 - 0.5f) * (m_MapSize.x * BLOCK_WIDTH - BLOCK_WIDTH * 10),
                            ((float)(rand() % 1000) / 1000 - 0.5f) * (m_MapSize.y * BLOCK_HIGHT - BLOCK_HIGHT * 10),
                            0.0f);
                    }
                }

                if (co != nullptr)
                {
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
        }

    }


    m_BeforeWave = m_CurrentWave;
    m_WaveCountWait = -1;
}


void Stage_Tutorial::Draw()
{
    SuperClass::Draw();

    Int2 mapsizetmp = m_MapSize;

    { //背景リング
        DRAW_POLYGON_DAT drawDatTmp = {};
        drawDatTmp.pos.z = BLOCK_HIGHT * 20.0f;
        drawDatTmp.size = MkF3(
            BLOCK_WIDTH * 30.0f,
            BLOCK_HIGHT * 30.0f,
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
