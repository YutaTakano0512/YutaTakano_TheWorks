#pragma once
#include "../core/component.h"
#include "../core/main.h"
#include "renderer.h"
#include "sprite.h"
#include <string>

//=============================================================================
// SpriteRenderer コンポーネント
// GameObjectに付与してスプライト描画を行う。
//=============================================================================
class SpriteRenderer : public Component
{
public:
    // テクスチャパス (Init後に変更可)
    std::string texturePath;

    // 描画サイズ (ピクセル)。0,0 のときテクスチャサイズを使用
    Vector2 size   = { 0.0f, 0.0f };

    // ピボット (0,0=左上 0.5,0.5=中心 1,1=右下)。描画の基点（フリップ時も維持されます）
    Vector2 pivot  = { 0.5f, 0.5f };

    // 色乗算 (RGBA)
    DirectX::XMFLOAT4 color = { 1.0f, 1.0f, 1.0f, 1.0f };

    // UV範囲 (スプライトシートのセル切り出しに使う)
    Vector2 uvMin  = { 0.0f, 0.0f };
    Vector2 uvMax  = { 1.0f, 1.0f };

    // 左右・上下反転
    bool flipX = false;
    bool flipY = false;

    // ブレンドモード
    BlendMode blendMode = BlendMode::NORMAL;

    // ドット絵モード (点サンプリング)
    bool pixelArt = false;

    // タイリング (true のとき描画サイズ ÷ テクスチャサイズ でUVを自動スケールしループ描画)
    bool tiling = false;

    // タイリング時の1タイル表示サイズ (px)。{0,0} のときテクスチャの実寸を使用
    // 例: テクスチャが32pxでも tileSize={64,64} にすれば 64px刻みでタイルされる
    Vector2 tileSize = { 0.0f, 0.0f };

    // カスタムシェーダー設定
    std::string customShaderPath;
    DirectX::XMFLOAT4 shaderParameter = { 0.0f, 0.0f, 0.0f, 0.0f };
    void SetCustomShader(const char* path);

    //-------------------------------------------------------------------------
    // ライフサイクル
    //-------------------------------------------------------------------------
    void Init()   override;
    void Uninit() override;
    void Draw()   override;

    //-------------------------------------------------------------------------
    // テクスチャ操作
    //-------------------------------------------------------------------------
    void SetTexture(const char* path);
    void SetTextureRegion(int col, int row, int colCount, int rowCount);

    ID3D11ShaderResourceView* GetSRV() const { return m_SRV; }

    // 描画サイズをテクスチャサイズに合わせる
    void FitToTexture();

private:
    ID3D11ShaderResourceView* m_SRV = nullptr;
    Sprite m_Sprite;
};
