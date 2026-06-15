#pragma once
#include "../core/main.h"
#include <string>
#include <unordered_map>

//=============================================================================
// ShaderManager クラス
// シェーダーのロード・キャッシュを一元管理する。
// Sprite以外の特殊シェーダー (エフェクト・ポストプロセス) で使用。
//=============================================================================
class ShaderManager
{
public:
    struct ShaderSet
    {
        ID3D11VertexShader* vs     = nullptr;
        ID3D11PixelShader*  ps     = nullptr;
        ID3D11InputLayout*  layout = nullptr;
    };

    // VSとPSのペアをロード (csoファイルパス)
    // 同じキーでGetすれば同じシェーダーを返す
    static bool Load(
        const std::string& key,
        const char* vsPath,
        const char* psPath);

    // PS のみロード (フルスクリーンクワッドVSと組み合わせる場合)
    static bool LoadPS(const std::string& key, const char* psPath);

    // シェーダーセットを取得
    static const ShaderSet* Get(const std::string& key);

    // 指定キーのシェーダーをパイプラインにセット
    static bool Apply(const std::string& key);

    // 全解放
    static void UnloadAll();

private:
    static std::unordered_map<std::string, ShaderSet> m_Cache;

    // フルスクリーンクワッド用 VS (ポストプロセス共通)
    static ID3D11VertexShader* m_FullscreenVS;
    static ID3D11InputLayout*  s_FullscreenLayout;

    static bool InitFullscreenVS();
};
