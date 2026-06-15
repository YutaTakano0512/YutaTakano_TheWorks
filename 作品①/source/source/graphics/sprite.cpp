#include "sprite.h"
#include "texture.h"
#include <cmath>

//=============================================================================
// Sprite 実装
//=============================================================================

Sprite::Sprite()  {}
Sprite::~Sprite() { Uninit(); }

void Sprite::Init()
{
    // 動的頂点バッファ (4頂点TriangleStrip)
    D3D11_BUFFER_DESC bd = {};
    bd.ByteWidth      = sizeof(VERTEX_2D) * 4;
    bd.Usage          = D3D11_USAGE_DYNAMIC;
    bd.BindFlags      = D3D11_BIND_VERTEX_BUFFER;
    bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    Renderer::GetDevice()->CreateBuffer(&bd, nullptr, &m_VertexBuffer);

    // シェーダーロード
    Renderer::CreateVertexShader(&m_VS, &m_Layout, "shader/sprite_VS.cso");
    Renderer::CreatePixelShader (&m_PS,             "shader/sprite_PS.cso");
}

void Sprite::Uninit()
{
    auto SafeRelease = [](auto*& p) { if (p) { p->Release(); p = nullptr; } };
    SafeRelease(m_VertexBuffer);
    SafeRelease(m_VS);
    SafeRelease(m_PS);
    SafeRelease(m_Layout);
}

void Sprite::LoadCustomPixelShader(const char* path)
{
    if (m_PS)
    {
        m_PS->Release();
        m_PS = nullptr;
    }
    Renderer::CreatePixelShader(&m_PS, path);
}

//=============================================================================
// 頂点バッファ更新
//=============================================================================
void Sprite::UpdateVertices(
    const Vector2& size,
    const Vector2& uvMin,
    const Vector2& uvMax,
    const DirectX::XMFLOAT4& color,
    bool flipX, bool flipY)
{
    float u0 = flipX ? uvMax.x : uvMin.x;
    float u1 = flipX ? uvMin.x : uvMax.x;
    float v0 = flipY ? uvMax.y : uvMin.y;
    float v1 = flipY ? uvMin.y : uvMax.y;

    float hw = size.x * 0.5f;
    float hh = size.y * 0.5f;

    // 中心基点の4頂点 (後でWorldMatrixで変換)
    VERTEX_2D verts[4] = {
        { {-hw, -hh, 0.0f}, color, {u0, v0} }, // 左上
        { { hw, -hh, 0.0f}, color, {u1, v0} }, // 右上
        { {-hw,  hh, 0.0f}, color, {u0, v1} }, // 左下
        { { hw,  hh, 0.0f}, color, {u1, v1} }, // 右下
    };

    D3D11_MAPPED_SUBRESOURCE mapped;
    auto* ctx = Renderer::GetDeviceContext();
    if (SUCCEEDED(ctx->Map(m_VertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped)))
    {
        memcpy(mapped.pData, verts, sizeof(verts));
        ctx->Unmap(m_VertexBuffer, 0);
    }
}

//=============================================================================
// Draw
//=============================================================================
void Sprite::Draw(
    ID3D11ShaderResourceView* srv,
    const Vector2& position,
    const Vector2& size,
    const DirectX::XMFLOAT4& color,
    const Vector2& uvMin,
    const Vector2& uvMax,
    float rotation,
    const Vector2& pivot,
    bool flipX, bool flipY)
{
    // pivot オフセット: pivot(0.5,0.5) なら中心基点、(0,0) なら左上基点
    float ox = (pivot.x - 0.5f) * size.x;
    float oy = (pivot.y - 0.5f) * size.y;

    // ワールド行列: 中心→pivot補正→回転→移動
    DirectX::XMMATRIX world =
        DirectX::XMMatrixTranslation(-ox, -oy, 0.0f) *
        DirectX::XMMatrixRotationZ(rotation) *
        DirectX::XMMatrixTranslation(position.x, position.y, 0.0f);

    DrawWithMatrix(srv, world, size, color, uvMin, uvMax, flipX, flipY);
}

void Sprite::DrawWithMatrix(
    ID3D11ShaderResourceView* srv,
    const DirectX::XMMATRIX& world,
    const Vector2& size,
    const DirectX::XMFLOAT4& color,
    const Vector2& uvMin,
    const Vector2& uvMax,
    bool flipX, bool flipY)
{
    UpdateVertices(size, uvMin, uvMax, color, flipX, flipY);

    auto* ctx = Renderer::GetDeviceContext();

    Renderer::SetWorldMatrix(world);
    Renderer::SetMaterialColor(color, srv != nullptr);

    // シェーダーセット
    ctx->VSSetShader(m_VS, nullptr, 0);
    ctx->PSSetShader(m_PS, nullptr, 0);
    ctx->IASetInputLayout(m_Layout);

    // テクスチャセット
    ctx->PSSetShaderResources(0, 1, &srv);

    // 頂点バッファセット
    UINT stride = sizeof(VERTEX_2D);
    UINT offset = 0;
    ctx->IASetVertexBuffers(0, 1, &m_VertexBuffer, &stride, &offset);
    ctx->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

    ctx->Draw(4, 0);
}

void Sprite::SetPointSampling(bool enable)
{
    // 実装はRendererのサンプラーバインドで行う
    // ここでは何もしない (SpriteRenderer側でサンプラーを切り替える)
}
