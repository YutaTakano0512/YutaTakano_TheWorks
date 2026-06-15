#pragma once
#include "../../core/gameObject.h"
#include "../../camera/camera2D.h"

//=============================================================================
// MainCamera クラス
// プレイヤーに追従するカメラオブジェクト
//=============================================================================
class MainCamera : public GameObject
{
private:
    Camera2D* m_Camera = nullptr;
    bool    m_IsFixed = false;
    Vector2 m_FixedPosition = { 0.0f, 0.0f };

    // カメラクランプ用
    bool  m_ClampEnabled = false;
    float m_StageMinX    = 0.0f;
    float m_StageMaxX    = 0.0f;
    float m_StageMinY    = 0.0f;
    float m_StageMaxY    = 0.0f;

public:
    virtual ~MainCamera() = default;

    void Init() override;
    void Update(float dt) override;

    Camera2D* GetCamera() const { return m_Camera; }

    // ボス戦時にカメラを指定座標に固定する
    void SetFixed(bool fixed, Vector2 pos = { 0.0f, 0.0f })
    {
        m_IsFixed = fixed;
        if (fixed) m_FixedPosition = pos;
    }
    bool IsFixed() const { return m_IsFixed; }

    // カメラがステージ端で止まるようにクランプ範囲を設定する
    // minX/maxX/minY/maxY : ワールド座標でのステージ端
    void SetStageBounds(float minX, float maxX, float minY, float maxY)
    {
        m_StageMinX    = minX;
        m_StageMaxX    = maxX;
        m_StageMinY    = minY;
        m_StageMaxY    = maxY;
        m_ClampEnabled = true;
    }
    void DisableClamp() { m_ClampEnabled = false; }
};
