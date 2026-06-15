#pragma once
#include "../core/main.h"
#include "renderer.h"
#include "../math/vector2.h"

//=============================================================================
// Sprite クラス
// 2Dスプライト一枚分の描画を担当。
// 動的頂点バッファを持ち、毎フレーム位置・UV・色を設定して描画。
//=============================================================================
class Sprite
{
public:
    Sprite();
    ~Sprite();

    // 初期化 (シェーダーロード)
    void Init();
    void Uninit();

    // カスタムピクセルシェーダーのロード
    void LoadCustomPixelShader(const char* path);

    //-------------------------------------------------------------------------
    // 描画 (毎フレーム呼び出す)
    //-------------------------------------------------------------------------
    // position : 画面座標(左上原点ピクセル)。デフォルトは中心基点。
    // size     : 描画サイズ (ピクセル)
    // color    : RGBA乗算色 (1,1,1,1) = そのまま
    // uvMin/Max: UV座標 (0〜1, デフォルトはテクスチャ全体)
    // rotation : Z軸回転 (ラジアン)
    // pivot    : 基点オフセット (0,0=左上  0.5,0.5=中心  1,1=右下)
    // flipX/Y  : 左右・上下反転
    void Draw(
        ID3D11ShaderResourceView* srv,
        const Vector2& position,
        const Vector2& size,
        const DirectX::XMFLOAT4& color   = { 1,1,1,1 },
        const Vector2& uvMin    = { 0,0 },
        const Vector2& uvMax    = { 1,1 },
        float rotation          = 0.0f,
        const Vector2& pivot    = { 0.5f, 0.5f },
        bool flipX              = false,
        bool flipY              = false);

    // ワールド行列を外部指定して描画 (SpriteRenderer用)
    void DrawWithMatrix(
        ID3D11ShaderResourceView* srv,
        const DirectX::XMMATRIX& world,
        const Vector2& size,
        const DirectX::XMFLOAT4& color,
        const Vector2& uvMin,
        const Vector2& uvMax,
        bool flipX = false,
        bool flipY = false);

    // ポイントサンプラーに切り替え (ドット絵向け)
    static void SetPointSampling(bool enable);

private:
    ID3D11Buffer*       m_VertexBuffer = nullptr;
    ID3D11VertexShader* m_VS          = nullptr;
    ID3D11PixelShader*  m_PS          = nullptr;
    ID3D11InputLayout*  m_Layout      = nullptr;

    void UpdateVertices(
        const Vector2& size,
        const Vector2& uvMin,
        const Vector2& uvMax,
        const DirectX::XMFLOAT4& color,
        bool flipX, bool flipY);
};
