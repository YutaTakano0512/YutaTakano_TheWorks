// ===================================================
// movie.cpp ui制御
// 
// hara sougo  2025/02/19
// ===================================================
#include "../Configuration/configuration.h"
#include "../../game_main.h"
#include "easing.h"
#include "../Texture/texData.h"
#include "../Sprite/sprite.h"
#include "ui.h"

#include "../../Core/DirectX/dx_video_texture.h"

#include "collision.h"

#include "../Entities/Block/block.h"
#include "../Game Object/gameObject.h"
#include "../Entities/player.h"

#include "fade.h"
#include "camera.h"
#include "../Managers/scene.h"
//#include "gameManager.h"
#include "movie.h"

// ===================================================
// マクロ定義
// ===================================================

// ===================================================
// 構造体宣言
// ===================================================

// ===================================================
// プロトタイプ宣言
// ===================================================

// ===================================================
// グローバル変数
// ===================================================

static bool g_video_loop = false;
static bool g_video_buffer_loop = false;
static VideoTexture* g_video = nullptr;
static VideoTexture* g_video_buffer = nullptr;

static VideoClock g_vclock;
//static int g_video_shader = 0;
static int g_video_shader_vs = 0;
static int g_video_shader_ps = 0;
//static std::function<void(void)> g_afterFunc = nullptr;


void InitMovie()
{
    VideoTexture::createAPI();

    //g_video_shader = DX_GetShaderIndex("SHADER/video");
    g_video_shader_vs = DX_GetShaderIndex("SHADER/vs_general2");
    g_video_shader_ps = DX_GetShaderIndex("SHADER/ps_video");

}

void UninitMovie()
{
    if (g_video != nullptr)
    {
        g_video->destroy();
        delete g_video;
        g_video = nullptr;
    }
    if (g_video_buffer != nullptr)
    {
        g_video_buffer->destroy();
        delete g_video_buffer;
        g_video_buffer = nullptr;
    }
    VideoTexture::destroyAPI();
}

void UpdateMovie()
{
    if (g_video != nullptr)
    {
        if (!g_video->enable())
        {
            return;
        }

        {
            //float elapsed = g_vclock.elapsed();
            //g_video.update(elapsed);

            //static float t = 0;
            //t += 1.0f / 60.0f / 4;

            //bool isfinish = g_video->update(t);
            bool isfinish = g_video->update(1.0f / 60.0f);

            if (isfinish)
            {
                if (g_video_buffer != nullptr)
                {
                    if (g_video != nullptr)
                    {
                        g_video->destroy();
                        delete g_video;

                        g_video = g_video_buffer;
                        g_video_buffer = nullptr;
                    }
                }
                else //if (!g_video_loop)
                {
                    UnloadMovie();
                }

                //if (g_afterFunc != nullptr)
                //{
                //    g_afterFunc();
                //    g_afterFunc = nullptr;
                //}
            }
        }
    }
}

void DrawMovie()
{
    if (g_video != nullptr)
    {
        if (!g_video->enable())
        {
            return;
        }

        if (!g_video->hasFinished())
        {
            POLYGON_SRV_BUF buf = {};
            buf.srv = g_video->getTexture()->shader_resource_view;
            //buf.posIn = MkF3(
            //    +SCREEN_WIDTH * 0.3f,
            //    SCREEN_HEIGHT * 0.3f,
            //    0.0f);
            buf.posIn = MkF3(
                0.0f,
                0.0f,
                0.0f);
            buf.rotIn = {};
            //buf.scaleIn =
            //    MkF3(
            //        +SCREEN_WIDTH * 0.4f,
            //        SCREEN_HEIGHT * 0.4f,
            //        1.0f);
            buf.scaleIn =
                MkF3(
                    +SCREEN_WIDTH,
                    SCREEN_HEIGHT,
                    1.0f);
            //buf.shaderIndex = g_video_shader;
            buf.shaderIndexVS = g_video_shader_vs;
            buf.shaderIndexPS = g_video_shader_ps;
            buf.priority = DRAW_POLYGON_PRIORITY_FORCE_DEEPEST;
            PolygonSRVDraw(buf);
            //Get->UpdateSubresource(cb, 0, nullptr, this, 0, 0);
        }
    }
}



bool LoadMovieSingle(
    std::string path/*,
    std::function<void(void)> afterFunc*/
)
{
    //g_afterFunc = afterFunc;
    return LoadMovie(
        path, false
    );
}

bool LoadMovie(
    std::string path, bool loop,
    std::string afterMovPath, bool afterMovLoop
)
{
    if (afterMovPath.size() > 0)
    {
        g_video_buffer = new VideoTexture;
        g_video_buffer_loop = afterMovLoop;
        bool r = g_video_buffer->create(afterMovPath.c_str(), g_video_buffer_loop);
        if (r)
        {
            {
                POLYGON_SRV_BUF buf = {};
                buf.srv = g_video_buffer->getTexture()->shader_resource_view;
                buf.posIn = MkF3(
                    SCREEN_WIDTH,
                    SCREEN_HEIGHT,
                    0.0f);
                buf.rotIn = {};
                buf.scaleIn =
                    MkF3(
                        +SCREEN_WIDTH,
                        SCREEN_HEIGHT,
                        1.0f);
                buf.shaderIndexVS = g_video_shader_vs;
                buf.shaderIndexPS = g_video_shader_ps;
                buf.priority = DRAW_POLYGON_PRIORITY_FORCE_DEEPEST;

                //PolygonSRVDraw(buf);
                PolygonSRVDraw(buf.srv,
                    buf.posIn,
                    buf.rotIn,
                    buf.scaleIn,
                    buf.shaderIndexVS,
                    buf.shaderIndexPS
                );
            }
        }
    }
    g_video = new VideoTexture;
    g_video_loop = loop;

    bool r = !g_video->create(path.c_str(), g_video_loop);
    if (r)
    {
        {
            POLYGON_SRV_BUF buf = {};
            buf.srv = g_video->getTexture()->shader_resource_view;
            buf.posIn = MkF3(
                SCREEN_WIDTH,
                SCREEN_HEIGHT,
                0.0f);
            buf.rotIn = {};
            buf.scaleIn =
                MkF3(
                    +SCREEN_WIDTH,
                    SCREEN_HEIGHT,
                    1.0f);
            buf.shaderIndexVS = g_video_shader_vs;
            buf.shaderIndexPS = g_video_shader_ps;
            buf.priority = DRAW_POLYGON_PRIORITY_FORCE_DEEPEST;

            //PolygonSRVDraw(buf);
            PolygonSRVDraw(buf.srv,
                buf.posIn,
                buf.rotIn,
                buf.scaleIn,
                buf.shaderIndexVS,
                buf.shaderIndexPS
            );
        }
    }

    return r;
}

void UnloadMovie()
{
    if (g_video != nullptr)
    {
        g_video->destroy();
        delete g_video;
        g_video = nullptr;
    }
    if (g_video_buffer != nullptr)
    {
        g_video_buffer->destroy();
        delete g_video_buffer;
        g_video_buffer = nullptr;
    }

    //g_afterFunc = nullptr;
}