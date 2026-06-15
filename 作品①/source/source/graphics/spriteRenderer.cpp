#include "spriteRenderer.h"
#include "texture.h"
#include "../core/gameObject.h"

//=============================================================================
// SpriteRenderer 実装
//=============================================================================

void SpriteRenderer::Init()
{
    m_Sprite.Init();

    if (!texturePath.empty())
        SetTexture(texturePath.c_str());
}

void SpriteRenderer::Uninit()
{
    m_Sprite.Uninit();
    m_SRV = nullptr; // Textureクラスがキャッシュ管理するので Release しない
}

void SpriteRenderer::Draw()
{
    if (!enabled) return;

    // ブレンドモードセット
    Renderer::SetBlendMode(blendMode);

    // サイズ: 0 なら自動 (テクスチャサイズ)
    Vector2 drawSize = size;
    if (drawSize.x <= 0.0f || drawSize.y <= 0.0f)
    {
        UINT w = 0, h = 0;
        if (Texture::GetSize(texturePath.c_str(), w, h))
        {
            drawSize.x = static_cast<float>(w);
            drawSize.y = static_cast<float>(h);
        }
        else
        {
            drawSize = { 64.0f, 64.0f };
        }
    }

    // GameObjectのTransformからワールド行列を取得
    DirectX::XMMATRIX world = m_GameObject->transform.GetWorldMatrix();

    // ピボットオフセットを計算 (フリップ時はピボット位置も反転させる)
    float px = flipX ? (1.0f - pivot.x) : pivot.x;
    float py = flipY ? (1.0f - pivot.y) : pivot.y;
    float ox = (px - 0.5f) * drawSize.x;
    float oy = (py - 0.5f) * drawSize.y;

    if (ox != 0.0f || oy != 0.0f)
    {
        world = DirectX::XMMatrixTranslation(-ox, -oy, 0.0f) * world;
    }

    if (!customShaderPath.empty())
    {
        Renderer::SetParameter(shaderParameter);
    }

    // タイリング時のUV計算
    // tileSize が指定されている場合はその値を1タイルの表示サイズとして使用する
    // (例: 32pxテクスチャに tileSize={64,64} → 64px刻みでタイル)
    // tileSize が {0,0} の場合はテクスチャの実寸をそのまま使用 (従来動作)
    Vector2 tiledUvMin = uvMin;
    Vector2 tiledUvMax = uvMax;
    if (tiling)
    {
        UINT texW = 0, texH = 0;
        if (Texture::GetSize(texturePath.c_str(), texW, texH) && texW > 0 && texH > 0)
        {
            // 1タイルの基準サイズ: tileSize が有効ならそれを、なければテクスチャ実寸
            float baseW = (tileSize.x > 0.0f) ? tileSize.x : static_cast<float>(texW);
            float baseH = (tileSize.y > 0.0f) ? tileSize.y : static_cast<float>(texH);

            float scaleU = drawSize.x / baseW;
            float scaleV = drawSize.y / baseH;
            tiledUvMin.x = uvMin.x * scaleU;
            tiledUvMin.y = uvMin.y * scaleV;
            tiledUvMax.x = uvMax.x * scaleU;
            tiledUvMax.y = uvMax.y * scaleV;
        }
    }

    Renderer::SetSamplerState(pixelArt);
    m_Sprite.DrawWithMatrix(m_SRV, world, drawSize, color, tiledUvMin, tiledUvMax, flipX, flipY);
    Renderer::SetSamplerState(false); // 元に戻す

    // ブレンドモードを通常に戻す
    Renderer::SetBlendMode(BlendMode::NORMAL);
}

void SpriteRenderer::SetTexture(const char* path)
{
    texturePath = path;
    m_SRV = Texture::Load(path);
}

void SpriteRenderer::SetTextureRegion(int col, int row, int colCount, int rowCount)
{
    float cw = 1.0f / static_cast<float>(colCount);
    float ch = 1.0f / static_cast<float>(rowCount);
    uvMin = { col * cw, row * ch };
    uvMax = { (col + 1) * cw, (row + 1) * ch };
}

void SpriteRenderer::FitToTexture()
{
    UINT w = 0, h = 0;
    if (Texture::GetSize(texturePath.c_str(), w, h))
    {
        size.x = static_cast<float>(w);
        size.y = static_cast<float>(h);
    }
}

void SpriteRenderer::SetCustomShader(const char* path)
{
    customShaderPath = path;
    m_Sprite.LoadCustomPixelShader(path);
}
