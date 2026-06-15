#pragma once
#include "../../core/gameObject.h"
#include "../../graphics/spriteRenderer.h"

//=============================================================================
// Ground クラス
// 物理演算対応の床オブジェクト
// Ground1（表層・草あり）+ Ground2（地中・土）の2層構造
//=============================================================================
class Ground : public GameObject
{
public:
    virtual ~Ground() = default;

    void Init()   override;
    void Update(float dt) override;

private:
    // 表層（上に草）
    GameObject*     m_Ground1Obj      = nullptr;
    SpriteRenderer* m_Ground1Renderer = nullptr;

    // 地中（土）
    GameObject*     m_Ground2Obj      = nullptr;
    SpriteRenderer* m_Ground2Renderer = nullptr;

    // ステージ端の見えない壁（左右）
    GameObject*     m_WallLeftObj  = nullptr;
    GameObject*     m_WallRightObj = nullptr;
};
