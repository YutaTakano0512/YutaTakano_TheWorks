#pragma once
#include "../core/main.h"
#include <string>
#include <unordered_map>
#include <memory>

//=============================================================================
// Texture クラス
// DirectXTexを使ってファイルからテクスチャをロード・キャッシュ管理
//=============================================================================
class Texture
{
public:
    // テクスチャを読み込む (キャッシュあり)
    // 同じパスなら同じSRVを返す
    static ID3D11ShaderResourceView* Load(const char* filePath);

    // キャッシュを全解放
    static void UnloadAll();

    // 指定テクスチャだけ解放
    static void Unload(const char* filePath);

    // テクスチャの幅・高さ取得
    static bool GetSize(const char* filePath, UINT& outWidth, UINT& outHeight);

private:
    struct Entry
    {
        ID3D11ShaderResourceView* srv = nullptr;
        UINT width  = 0;
        UINT height = 0;
    };

    static std::unordered_map<std::string, Entry> m_Cache;
};
