#include "texture.h"
#include "renderer.h"
#include "../../DirectXTex.h"
#include <wrl/client.h>

#ifdef _DEBUG
#pragma comment(lib, "DirectXTex_Debug.lib")
#else
#pragma comment(lib, "DirectXTex_Release.lib")
#endif

using namespace DirectX;
using Microsoft::WRL::ComPtr;

//=============================================================================
// 静的メンバ定義
//=============================================================================
std::unordered_map<std::string, Texture::Entry> Texture::m_Cache;

//=============================================================================
// Load
//=============================================================================
ID3D11ShaderResourceView* Texture::Load(const char* filePath)
{
    // キャッシュ検索
    auto it = m_Cache.find(filePath);
    if (it != m_Cache.end())
        return it->second.srv;

    // ファイル名をワイド文字列に変換
    int len = MultiByteToWideChar(CP_UTF8, 0, filePath, -1, nullptr, 0);
    std::wstring wPath(len, L'\0');
    MultiByteToWideChar(CP_UTF8, 0, filePath, -1, &wPath[0], len);

    // DirectXTex でテクスチャロード
    ScratchImage image;
    TexMetadata  meta;

    HRESULT hr = E_FAIL;

    // 拡張子判定
    std::string path(filePath);
    std::string ext = path.substr(path.find_last_of('.') + 1);
    for (auto& c : ext) c = static_cast<char>(tolower(c));

    if (ext == "dds")
        hr = LoadFromDDSFile(wPath.c_str(), DDS_FLAGS_NONE, &meta, image);
    else if (ext == "tga")
        hr = LoadFromTGAFile(wPath.c_str(), TGA_FLAGS_NONE, &meta, image);
    else
        hr = LoadFromWICFile(wPath.c_str(), WIC_FLAGS_NONE, &meta, image);

    if (FAILED(hr))
    {
        // ロード失敗: 1x1 白テクスチャを返す
        ID3D11Device* dev = Renderer::GetDevice();
        D3D11_TEXTURE2D_DESC td = {};
        td.Width = td.Height = 1;
        td.MipLevels = td.ArraySize = 1;
        td.Format            = DXGI_FORMAT_R8G8B8A8_UNORM;
        td.SampleDesc.Count  = 1;
        td.Usage             = D3D11_USAGE_IMMUTABLE;
        td.BindFlags         = D3D11_BIND_SHADER_RESOURCE;

        UINT32 white = 0xFFFFFFFF;
        D3D11_SUBRESOURCE_DATA sd = {};
        sd.pSysMem     = &white;
        sd.SysMemPitch = sizeof(UINT32);

        ComPtr<ID3D11Texture2D> tex;
        dev->CreateTexture2D(&td, &sd, &tex);

        Entry entry;
        entry.width = entry.height = 1;
        dev->CreateShaderResourceView(tex.Get(), nullptr, &entry.srv);
        m_Cache[filePath] = entry;
        return entry.srv;
    }

    // SRV 生成
    ID3D11ShaderResourceView* srv = nullptr;
    hr = CreateShaderResourceView(
        Renderer::GetDevice(), image.GetImages(), image.GetImageCount(), meta, &srv);

    if (FAILED(hr)) return nullptr;

    Entry entry;
    entry.srv    = srv;
    entry.width  = static_cast<UINT>(meta.width);
    entry.height = static_cast<UINT>(meta.height);
    m_Cache[filePath] = entry;

    return srv;
}

//=============================================================================
// Unload
//=============================================================================
void Texture::Unload(const char* filePath)
{
    auto it = m_Cache.find(filePath);
    if (it != m_Cache.end())
    {
        if (it->second.srv) it->second.srv->Release();
        m_Cache.erase(it);
    }
}

void Texture::UnloadAll()
{
    for (auto& pair : m_Cache)
        if (pair.second.srv) pair.second.srv->Release();
    m_Cache.clear();
}

bool Texture::GetSize(const char* filePath, UINT& outWidth, UINT& outHeight)
{
    auto it = m_Cache.find(filePath);
    if (it != m_Cache.end())
    {
        outWidth  = it->second.width;
        outHeight = it->second.height;
        return true;
    }
    return false;
}
