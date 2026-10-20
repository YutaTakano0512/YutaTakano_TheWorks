// ===================================================
// Stage_Base.h ステージ処理
// 
// hara sougo  2024/12/13
// ===================================================
#include "../../game_main.h"
#include "../Sprite/sprite.h"
#include "../Texture/texture.h"
#include "../Entities/Block/block.h"
#include "../Entities/Block/blockPrefab.h"
#include "../Systems/collision.h"
#include "../Systems/easing.h"

#include "../Managers/soundManager.h"
#include "../Managers/gameManager.h"
#include "../Game Map/gameMap.h"

#include "../Game Object/gameObject.h"
#include "../Game Object/gameObjectPrefab.h"

#include "../Managers/bg.h"

#include "../Systems/ui.h"

#include "../Entities/player.h"
#include "../Systems/camera.h"
#include "../Game Object/Game Object Events/GOE_Character_Player.h"

#include "Stage_Base.h"

#include <fstream>
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



void Stage_Base::Init()
{
    //変数初期化
    m_CurrentWave = 0;
    m_BeforeWave = -1;

    m_CountFrame = 0;


    //マップに壁を生成
    //{
    //    int mspTmpX = MAP_X;
    //    int mspTmpY = MAP_Y;
    //    for (int i = 0; i < mspTmpX + 2; i++)
    //    {
    //        for (int j = 0; j < 2; j++)
    //        {
    //            //BLOCK b = GetBlockPrefab((BLOCK_TYPE)(BLOCK_TYPE_01 + i % 10));
    //            BLOCK b = GetBlockPrefab(BLOCK_TYPE_DUMMY);
    //            b.pos = MkF2(
    //                -BLOCK_WIDTH * mspTmpX / 2 + BLOCK_WIDTH * i,
    //                -BLOCK_HIGHT * (mspTmpY / 2 + 1) * (j == 1 ? -1 : 1) - BLOCK_HIGHT / 2
    //            );
    //            SetBlock(b);
    //        }
    //    }
    //    for (int i = 0; i < mspTmpY; i++)
    //    {
    //        for (int j = 0; j < 2; j++)
    //        {
    //            //BLOCK b = GetBlockPrefab((BLOCK_TYPE)(BLOCK_TYPE_01 + i % 10));
    //            BLOCK b = GetBlockPrefab(BLOCK_TYPE_DUMMY);
    //            b.pos = MkF2(
    //                -BLOCK_WIDTH * ((float)mspTmpX / 2 + 0.5f) * (j == 1 ? -1 : 1) - BLOCK_WIDTH / 2 + BLOCK_WIDTH * 1.0f,
    //                -BLOCK_HIGHT * mspTmpY / 2 + BLOCK_HIGHT * i
    //            );
    //            SetBlock(b);
    //        }
    //    }
    //}

}

void Stage_Base::Uninit()
{
    for (Character::MAIN_LOAD* p : m_UseChara)
    {
        Character::Unload(p);
    }
    m_UseChara.clear();
    m_UseChara.shrink_to_fit();

    DeleteBlockAll();
}


void Stage_Base::Update()
{
    if (m_GameoverFlag)
    {
        return;
    }

    if (!m_CountStop && !GameManagerIsMovieFlag())
    {
        if (m_CountFrame > 0)
        {
            m_CountFrame--;
        }
        m_CountUpFrame++;
    }

    //if (!GameManagerIsMovieFlag())
    {
        for (int i = 0; i < PLAYER_MAX; i++)
        {
            PLAYER* pl = GetPlayer(i);
            if (pl != nullptr)
            {
                GAME_OBJECT* plOb = pl->obj;
                if (plOb != nullptr)
                {
                    bool isEventWalking = false;

                    GOE_Character_Player* goe =
                        dynamic_cast<GOE_Character_Player*>(plOb->events.source);  // ダウンキャスト
                    if (goe != nullptr)
                    {
                        isEventWalking = goe->IsEventWalking();
                    }

                    if (!isEventWalking)
                    {
                        float xs = (m_MapSize.x * BLOCK_WIDTH - plOb->size.x) / 2;
                        float ys = (m_MapSize.y * BLOCK_HIGHT - plOb->size.y) / 2;

                        if (plOb->pos.x > xs)
                        {
                            plOb->pos.x = xs;
                        }
                        if (plOb->pos.x < -xs)
                        {
                            plOb->pos.x = -xs;
                        }

                        if (plOb->pos.y > ys)
                        {
                            plOb->pos.y = ys;
                        }
                        if (plOb->pos.y < -ys)
                        {
                            plOb->pos.y = -ys;
                        }
                    }
                }
            }
        }
    }
}

void Stage_Base::Draw()
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

        dsdat.color = m_GameoverFadeColor;
        dsdat.color.w *= (float)m_GameoverFrame / GAME_OVER_AFTER_FRAME,

        dsdat.texNo = 0;
        dsdat.loadTexType = LOADTEXTURETYPE_MAIN;

        dsdat.texUV = {
            0.0f,
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
        opt.priority = DRAW_POLYGON_PRIORITY_FORCE_NORMAL;

        DrawPolygonInCameraEx(dsdat, opt);
    }
}

GAME_OBJECT* Stage_Base::GetBossObj()
{
    GAME_OBJECT* co = GetGameObjectFindByIdentifier(m_EnemyBoss);
    
    return co;
}



Stage_Base::StageObjList Stage_Base::LoadStageData(const std::string& stagePath)
{
    StageObjList objects{};

    std::ifstream ifs(stagePath);
    if (!ifs.is_open()) return objects;

    constexpr float invScale = BLOCK_WIDTH;
    std::string line;
    while (std::getline(ifs, line))
    {
        if (line.empty()) continue;

        // 1行をコンマ区切りで全部トークン化
        std::istringstream iss(line);
        std::vector<std::string> tokens;
        std::string tok;
        while (std::getline(iss, tok, ','))
        {
            tokens.push_back(tok);
        }

        // 最低３トークン（t, x1, y1）は必須
        if (tokens.size() < 3) continue;

        StageObj o;
        o.type = std::stoi(tokens[0]);
        // 始点
        o.pt.x = std::stof(tokens[1]) * invScale;
        o.pt.y = std::stof(tokens[2]) * invScale;
        o.pt.z = 0.0f;

        // Line ならトークン数が6以上か確認して終点を読み込む
        if (tokens.size() >= 6)
        {
            o.pt2.x = std::stof(tokens[3]) * invScale;
            o.pt2.y = std::stof(tokens[4]) * invScale;
            o.pt2.z = 0.0f;
            o.r = std::stof(tokens[5]) * invScale;
        }

        objects.push_back(o);
    }

    return objects;
}
