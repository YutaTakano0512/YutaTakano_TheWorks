#include "shaderManager.h"
#include "renderer.h"
#include <fstream>

//=============================================================================
// 静的メンバ定義
//=============================================================================
std::unordered_map<std::string, ShaderManager::ShaderSet> ShaderManager::m_Cache;
ID3D11VertexShader* ShaderManager::m_FullscreenVS    = nullptr;
ID3D11InputLayout*  ShaderManager::s_FullscreenLayout= nullptr;

//=============================================================================
// ヘルパー
//=============================================================================
static std::vector<char> LoadCSOBytes(const char* path)
{
    std::ifstream ifs(path, std::ios::binary | std::ios::ate);
    if (!ifs) return {};
    auto size = ifs.tellg();
    ifs.seekg(0);
    std::vector<char> buf(static_cast<size_t>(size));
    ifs.read(buf.data(), size);
    return buf;
}

//=============================================================================
// Load (VS + PS)
//=============================================================================
bool ShaderManager::Load(const std::string& key, const char* vsPath, const char* psPath)
{
    if (m_Cache.count(key)) return true;  // 既にロード済み

    ShaderSet set;
    Renderer::CreateVertexShader(&set.vs, &set.layout, vsPath);
    Renderer::CreatePixelShader (&set.ps, psPath);

    if (!set.vs || !set.ps) return false;

    m_Cache[key] = set;
    return true;
}

//=============================================================================
// LoadPS (フルスクリーンVS + カスタムPS)
//=============================================================================
bool ShaderManager::LoadPS(const std::string& key, const char* psPath)
{
    if (m_Cache.count(key)) return true;

    if (!m_FullscreenVS)
    {
        if (!InitFullscreenVS()) return false;
    }

    ShaderSet set;
    set.vs     = m_FullscreenVS;
    set.layout = s_FullscreenLayout;
    Renderer::CreatePixelShader(&set.ps, psPath);

    if (!set.ps) return false;

    m_Cache[key] = set;
    return true;
}

//=============================================================================
// Get
//=============================================================================
const ShaderManager::ShaderSet* ShaderManager::Get(const std::string& key)
{
    auto it = m_Cache.find(key);
    return (it != m_Cache.end()) ? &it->second : nullptr;
}

//=============================================================================
// Apply
//=============================================================================
bool ShaderManager::Apply(const std::string& key)
{
    auto* set = Get(key);
    if (!set) return false;

    auto* ctx = Renderer::GetDeviceContext();
    ctx->VSSetShader(set->vs,     nullptr, 0);
    ctx->PSSetShader(set->ps,     nullptr, 0);
    ctx->IASetInputLayout(set->layout);
    return true;
}

//=============================================================================
// UnloadAll
//=============================================================================
void ShaderManager::UnloadAll()
{
    for (auto& pair : m_Cache)
    {
        auto& s = pair.second;
        // フルスクリーンVS は共有されているので個別に解放しない
        if (s.vs != m_FullscreenVS && s.vs) s.vs->Release();
        if (s.ps) s.ps->Release();
        if (s.layout != s_FullscreenLayout && s.layout) s.layout->Release();
    }
    m_Cache.clear();

    if (m_FullscreenVS)    { m_FullscreenVS->Release();    m_FullscreenVS    = nullptr; }
    if (s_FullscreenLayout){ s_FullscreenLayout->Release(); s_FullscreenLayout= nullptr; }
}

//=============================================================================
// フルスクリーンVS 初期化 (UV付き inputLayout不要)
//=============================================================================
bool ShaderManager::InitFullscreenVS()
{
    auto code = LoadCSOBytes("shader/fullscreen_VS.cso");
    if (code.empty()) return false;

    auto* dev = Renderer::GetDevice();
    dev->CreateVertexShader(code.data(), code.size(), nullptr, &m_FullscreenVS);
    s_FullscreenLayout = nullptr; // フルスクリーンVSは頂点バッファ不要

    return m_FullscreenVS != nullptr;
}
