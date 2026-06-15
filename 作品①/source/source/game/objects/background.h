#pragma once
#include "../../core/gameObject.h"
#include "../../graphics/spriteRenderer.h"

//=============================================================================
// Background クラス
// パララックス（視差）スクロール付き背景
//=============================================================================

#define BG_LAYER_COUNT 7

class Background : public GameObject
{
public:
    void Init()           override;
    void Update(float dt) override;
    void Draw()           override;

    // カメラのワールド座標を毎フレーム渡す
    void SetCameraPosition(float cameraX, float cameraY);

private:
    SpriteRenderer* m_Sprite[BG_LAYER_COUNT];        // 各レイヤーのスプライト
    float           m_ParallaxRateX[BG_LAYER_COUNT];  // X視差率
    float           m_ParallaxRateY[BG_LAYER_COUNT];  // Y視差率（微量）
    float           m_UVOffsetX[BG_LAYER_COUNT];      // 現在のX UVオフセット
    float           m_UVOffsetY[BG_LAYER_COUNT];      // 現在のY UVオフセット
};
