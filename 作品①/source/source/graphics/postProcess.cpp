#include "postProcess.h"
#include "renderer.h"
#include <algorithm>

//=============================================================================
// PostProcess 実装
//=============================================================================

PostProcess::PostProcess()  {}
PostProcess::~PostProcess() { Uninit(); }

bool PostProcess::Init()
{
    if (!m_SceneRT.Init())     return false;
    if (!m_PingPong[0].Init()) return false;
    if (!m_PingPong[1].Init()) return false;

    // シェーダーをロード
    struct ShaderInfo { PostEffectType type; const char* path; };
    ShaderInfo shaders[] = {
        { PostEffectType::BLUR,        "shader/postBlur_PS.cso"      },
        { PostEffectType::BLOOM,       "shader/postBloom_PS.cso"     },
        { PostEffectType::VIGNETTE,    "shader/postVignette_PS.cso"  },
        { PostEffectType::CRT,         "shader/postCRT_PS.cso"       },
        { PostEffectType::COLOR_GRADE, "shader/postColorGrade_PS.cso"},
        { PostEffectType::WAVE,        "shader/postWave_PS.cso"      },
        { PostEffectType::LETTERBOX,   "shader/postLetterbox_PS.cso" },
        { PostEffectType::FADE, "shader/postFade_PS.cso"},
        { PostEffectType::IRIS_OUT, "shader/postIrisOut_PS.cso"}
    };
    for (auto& info : shaders)
        ShaderManager::LoadPS(GetShaderKey(info.type), info.path);

    // 最終出力用シェーダー (スプライトPS兼用)
    ShaderManager::LoadPS("postFinal", "shader/sprite_PS.cso");

    m_Initialized = true;
    return true;
}

void PostProcess::Uninit()
{
    m_SceneRT.Uninit();
    m_PingPong[0].Uninit();
    m_PingPong[1].Uninit();
    m_Initialized = false;
}

//=============================================================================
// Begin : ゲーム描画をオフスクリーンへ
//=============================================================================
void PostProcess::Begin()
{
    if (!enabled || !m_Initialized)
    {
        // ポストプロセス無効時はバックバッファへ直接描画
        RenderTarget::End();
        return;
    }

    // 背景テクスチャの暗部が青みがかって見えないよう純粋な黒でクリア
    const float cc[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
    m_SceneRT.Begin(cc);
}

//=============================================================================
// End : エフェクト適用 → 画面出力
//=============================================================================
void PostProcess::End()
{
    if (!enabled || !m_Initialized || m_Effects.empty())
    {
        // そのままバックバッファへ転送
        RenderTarget::End();
        if (m_Initialized)
        {
            DrawFullscreen(m_SceneRT.GetSRV(), "postFinal",
                           {1,0,0,0}, {0,0,0,0});
        }
        return;
    }

    // ピンポンバッファでエフェクトチェーン
    m_PingPongIdx = 0;
    ID3D11ShaderResourceView* src = m_SceneRT.GetSRV();

    for (size_t i = 0; i < m_Effects.size(); ++i)
    {
        const auto& fx = m_Effects[i];
        bool isLast = (i == m_Effects.size() - 1);

        if (isLast)
        {
            // 最後はバックバッファへ
            RenderTarget::End();
        }
        else
        {
            // 中間はピンポンバッファへ
            m_PingPong[m_PingPongIdx].Begin();
        }

        DrawFullscreen(src, GetShaderKey(fx.type), fx.param0, fx.param1);

        if (!isLast)
        {
            src = m_PingPong[m_PingPongIdx].GetSRV();
            m_PingPongIdx ^= 1;
        }
    }
}

//=============================================================================
// エフェクト管理
//=============================================================================
void PostProcess::AddEffect(PostEffectType type, DirectX::XMFLOAT4 p0, DirectX::XMFLOAT4 p1)
{
    m_Effects.push_back({ type, p0, p1 });
}

void PostProcess::ClearEffects()
{
    m_Effects.clear();
}

void PostProcess::UpdateEffectParam(PostEffectType type, DirectX::XMFLOAT4 param0)
{
    for (auto& fx : m_Effects)
    {
        if (fx.type == type)
        {
            fx.param0 = param0;
            return;
        }
    }
}

void PostProcess::RemoveEffect(PostEffectType type)
{
    m_Effects.erase(
        std::remove_if(m_Effects.begin(), m_Effects.end(),
            [type](const PostEffectDesc& fx){ return fx.type == type; }),
        m_Effects.end());
}

//=============================================================================
// フルスクリーンクワッド描画
//=============================================================================
void PostProcess::DrawFullscreen(
    ID3D11ShaderResourceView* srv,
    const char* shaderKey,
    const DirectX::XMFLOAT4& p0,
    const DirectX::XMFLOAT4& p1)
{
    auto* ctx = Renderer::GetDeviceContext();

    ShaderManager::Apply(shaderKey);

    Renderer::SetWorldViewProjection2D();
    Renderer::SetParameter(p0);
    // フルスクリーン描画時は常にテクスチャ有効＆白にする（直前の描画ステートを引き継いで真っ青になるのを防ぐ）
    Renderer::SetMaterialColor({ 1.0f, 1.0f, 1.0f, 1.0f }, true);

    ctx->PSSetShaderResources(0, 1, &srv);
    ctx->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
    ctx->IASetVertexBuffers(0, 0, nullptr, nullptr, nullptr);
    ctx->IASetInputLayout(nullptr);
    ctx->Draw(4, 0);  // フルスクリーンVSが頂点バッファ不要で4頂点生成
}

//=============================================================================
// シェーダーキー
//=============================================================================
const char* PostProcess::GetShaderKey(PostEffectType type)
{
    switch (type)
    {
    case PostEffectType::BLUR:        return "postBlur";
    case PostEffectType::BLOOM:       return "postBloom";
    case PostEffectType::VIGNETTE:    return "postVignette";
    case PostEffectType::CRT:         return "postCRT";
    case PostEffectType::COLOR_GRADE: return "postColorGrade";
    case PostEffectType::WAVE:        return "postWave";
    case PostEffectType::LETTERBOX:   return "postLetterbox";
    case PostEffectType::FADE:      return "postFade";
    case PostEffectType::IRIS_OUT:    return "postIrisOut";
    }
    return "postFinal";
}

const char* PostProcess::GetShaderPath(PostEffectType type)
{
    switch (type)
    {
    case PostEffectType::BLUR:        return "shader/postBlur_PS.cso";
    case PostEffectType::BLOOM:       return "shader/postBloom_PS.cso";
    case PostEffectType::VIGNETTE:    return "shader/postVignette_PS.cso";
    case PostEffectType::CRT:         return "shader/postCRT_PS.cso";
    case PostEffectType::COLOR_GRADE: return "shader/postColorGrade_PS.cso";
    case PostEffectType::WAVE:        return "shader/postWave_PS.cso";
    case PostEffectType::LETTERBOX:   return "shader/postLetterbox_PS.cso";
    case PostEffectType::FADE:      return "shader/postFade_PS.cso";
    case PostEffectType::IRIS_OUT:    return "shader/postIrisOut_PS.cso";
    }
    return "shader/sprite_PS.cso";
}
