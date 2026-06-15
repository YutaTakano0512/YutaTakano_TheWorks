#include "ground.h"
#include "../../core/scene.h"
#include "../../core/manager.h"
#include "../../graphics/spriteRenderer.h"
#include "../../physics/boxCollider2D.h"
#include "../../physics/rigidbody2D.h"

//=============================================================================
// 地面の設定定数
//=============================================================================
// 画面に表示する横幅の倍数（カメラ移動に対して余裕をもって覆う）
static constexpr float GROUND_SCREEN_W     = 1280.0f;       // スクリーン幅
static constexpr float GROUND_TILE_W       = GROUND_SCREEN_W * 3.0f; // タイル総幅（画面5枚分）

// 1タイルあたりの表示サイズ (px)
// テクスチャが32pxでもこの値のサイズで1コマとして繰り返される
static constexpr float GROUND1_TILE_SIZE   = 64.0f;  // 32pxテクスチャを2倍スケールで表示
static constexpr float GROUND2_TILE_SIZE   = 64.0f;  // 同上

// Ground1（表層）の描画高さ
static constexpr float GROUND1_H           = GROUND1_TILE_SIZE;
// Ground2（地中）の高さ = 画面下部を埋める
static constexpr float GROUND2_H           = 200.0f;

// 地面のY座標（ゲーム座標系: Y下がプラス）
// プレイヤーのY=-100付近で着地できるよう調整
static constexpr float GROUND_Y            = 100.0f;

//=============================================================================
// 初期化
//=============================================================================

void Ground::Init()
{
    GameObject::Init();
    Scene* scene = Manager::GetScene();

    SetName("Ground");
    SetTag("Ground");

    transform.position = { 0.0f, GROUND_Y };

    if (!scene) return;

	// 地表の設定
    m_Ground1Obj = scene->AddGameObject<GameObject>(Layer::FIELD);
    m_Ground1Obj->transform.position = { 0.0f, GROUND_Y };
    m_Ground1Obj->AddComponent<BoxCollider2D>()->size = { GROUND_TILE_W, GROUND1_H };
    m_Ground1Obj->AddComponent<Rigidbody2D>()->bodyType = Rigidbody2D::BodyType::Static;
    m_Ground1Renderer = m_Ground1Obj->AddComponent<SpriteRenderer>();
    m_Ground1Renderer->SetTexture("asset/texture/grounds/ground1.png");
    m_Ground1Renderer->size     = { GROUND_TILE_W, GROUND1_H };
    m_Ground1Renderer->tiling   = true;
    m_Ground1Renderer->tileSize = { GROUND1_TILE_SIZE, GROUND1_TILE_SIZE };
    m_Ground1Renderer->pixelArt = true;

	// 地中の設定
    m_Ground2Obj = scene->AddGameObject<GameObject>(Layer::FIELD);
    m_Ground2Obj->transform.position = { 0.0f, GROUND_Y + GROUND1_H / 2.0f + GROUND2_H / 2.0f };
    m_Ground2Renderer = m_Ground2Obj->AddComponent<SpriteRenderer>();
    m_Ground2Renderer->SetTexture("asset/texture/grounds/ground2.png");
    m_Ground2Renderer->size     = { GROUND_TILE_W, GROUND2_H };
    m_Ground2Renderer->tiling   = true;
    m_Ground2Renderer->tileSize = { GROUND2_TILE_SIZE, GROUND2_TILE_SIZE };
    m_Ground2Renderer->pixelArt = true;

    // 見えない壁の設定
    static constexpr float WALL_W = 64.0f;
    static constexpr float WALL_H = 2000.0f;

    const float stageHalfW = GROUND_TILE_W * 0.5f;
    const float wallCenterY = GROUND_Y;

    m_WallLeftObj = scene->AddGameObject<GameObject>(Layer::FIELD);
    m_WallLeftObj->SetName("WallLeft");
    m_WallLeftObj->SetTag("Wall");
    m_WallLeftObj->transform.position = { -stageHalfW - WALL_W * 0.5f, wallCenterY };
    m_WallLeftObj->AddComponent<BoxCollider2D>()->size = { WALL_W, WALL_H };
    m_WallLeftObj->AddComponent<Rigidbody2D>()->bodyType = Rigidbody2D::BodyType::Static;

    m_WallRightObj = scene->AddGameObject<GameObject>(Layer::FIELD);
    m_WallRightObj->SetName("WallRight");
    m_WallRightObj->SetTag("Wall");
    m_WallRightObj->transform.position = { stageHalfW + WALL_W * 0.5f, wallCenterY };
    m_WallRightObj->AddComponent<BoxCollider2D>()->size = { WALL_W, WALL_H };
    m_WallRightObj->AddComponent<Rigidbody2D>()->bodyType = Rigidbody2D::BodyType::Static;
}

//=============================================================================
// 更新
//=============================================================================

void Ground::Update(float dt)
{
    GameObject::Update(dt);
}
