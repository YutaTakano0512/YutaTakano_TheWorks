#pragma once
#include "../core/main.h"
#include "renderTarget.h"
#include "shaderManager.h"
#include <vector>
#include <string>

//=============================================================================
// PostEffectType 列挙
//=============================================================================
enum class PostEffectType
{
    BLUR,        // ガウスブラー
    BLOOM,       // ブルーム (発光)
    VIGNETTE,    // ビネット
    CRT,         // CRTエフェクト (スキャンライン)
    COLOR_GRADE, // カラーグレーディング
    WAVE,        // 衝撃波エフェクト
    LETTERBOX,   // 映画風黒帯エフェクト
    FADE,
    IRIS_OUT,    // アイリスアウト
};

//=============================================================================
// PostEffect 設定
//=============================================================================
struct PostEffectDesc
{
    PostEffectType type;
    DirectX::XMFLOAT4 param0 = { 1.0f, 0.0f, 0.0f, 0.0f };
    DirectX::XMFLOAT4 param1 = { 0.0f, 0.0f, 0.0f, 0.0f };
};

//=============================================================================
// PostProcess クラス
// ゲーム描画 → オフスクリーンRT → 各エフェクト適用 → 画面出力
//
// 使い方:
//   postProcess.Begin();     // ゲーム描画をRTにリダイレクト
//   ... scene.Draw() ...
//   postProcess.End();       // エフェクト適用して画面に出力
//=============================================================================
class PostProcess
{
public:
    PostProcess();
    ~PostProcess();

    bool Init();
    void Uninit();

    // ゲーム描画をオフスクリーンにリダイレクト開始
    void Begin();

    // エフェクト適用 → 画面出力
    void End();

    // エフェクト追加
    void AddEffect(PostEffectType type,
                   DirectX::XMFLOAT4 param0 = {1,0,0,0},
                   DirectX::XMFLOAT4 param1 = {0,0,0,0});

    // 既存エフェクトのパラメータを動的に更新する
    void UpdateEffectParam(PostEffectType type, DirectX::XMFLOAT4 param0);

    // 特定エフェクトを削除する
    void RemoveEffect(PostEffectType type);

    void ClearEffects();

    // ポストプロセスON/OFF
    bool enabled = true;

private:
    RenderTarget m_SceneRT;       // ゲーム描画先
    RenderTarget m_PingPong[2];   // ピンポンバッファ
    int          m_PingPongIdx = 0;

    std::vector<PostEffectDesc> m_Effects;

    // フルスクリーンクワッド描画
    void DrawFullscreen(ID3D11ShaderResourceView* srv, const char* shaderKey,
                        const DirectX::XMFLOAT4& p0, const DirectX::XMFLOAT4& p1);

    // シェーダーキーを取得
    static const char* GetShaderKey(PostEffectType type);
    static const char* GetShaderPath(PostEffectType type);

    bool m_Initialized = false;
};
