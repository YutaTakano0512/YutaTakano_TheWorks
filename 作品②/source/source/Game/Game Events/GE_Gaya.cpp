// ===================================================
// GE_Gaya.cpp ガヤの処理
// 
// hara sougo  2025/02/12
// ===================================================
#include "../../game_main.h"
#include "../Sprite/sprite.h"
#include "../Texture/texture.h"
#include "../Entities/Block/block.h"
#include "../Systems/collision.h"
#include "../Systems/easing.h"

#include "../Managers/gameManager.h"
#include "../Managers/soundManager.h"

#include "../Game Object/gameObject.h"
#include "../Game Object/gameObjectPrefab.h"

#include "../Systems/font.h"
#include "../Systems/enum.h"
#include "../Systems/ui.h"
#include "../Systems/ui_Game.h"

#include "../Entities/player.h"
#include "../Systems/camera.h"

#include "../Entities/character.h"

#include "GE_Gaya.h"

// ===================================================
// マクロ定義
// ===================================================
static constexpr auto GAYA_TYPE_PLAYER = "PLAYER";

static constexpr auto GAYA_SOUND_TYPE_NONE = "NONE";

//static constexpr int PLAYING_GAYA_IN_FRAME = 30;
//static constexpr int PLAYING_GAYA_OUT_FRAME = 30;
static constexpr int PLAYING_GAYA_IN_FRAME = 16;
static constexpr int PLAYING_GAYA_OUT_FRAME = 16;

// ===================================================
// 構造体宣言
// ===================================================
struct GAYA_DAT {
    std::string name;

    std::string type;

    std::string icon;
    int frameMax;
    int delay;

    std::wstring txt;
    int hCnt;

    int priority;
    std::string sound;

    std::vector<DX_DrawString_SetSize> sizeEx;
    std::vector<DX_DrawString_SetDrawingEffect> drawingEffect;
};

struct GAYA_PLAY_DAT {
    bool enable;

    std::string id;

    const GAYA_DAT* dat;
    std::wstring txt;

    int frame;
    int frameMax;

    Float2 pos;

    GAYA_REQUEST_OPT opt;
};

struct GAYA_RQU_BUF {
    bool enable;

    std::string id;

    const GAYA_DAT* dat;
    std::wstring txt;

    int delay;

    GAYA_REQUEST_OPT opt;
};

// ===================================================
// プロトタイプ宣言
// ===================================================
void gayaDatInit(void);
void gayaStartRequest(GAYA_RQU_BUF gayabuf);
void gayaStartRequAddBuf(GAYA_RQU_BUF* gayarequbuf);
void gayaStartRequBufUpdate();

// ===================================================
// グローバル変数
// ===================================================
static std::vector<GAYA_DAT> gayaDat = {};

static constexpr int GAYA_PLAY_MAX = 3;
//static GAYA_PLAY_DAT g_PlayingGaya = {};
static GAYA_PLAY_DAT g_PlayingGaya[GAYA_PLAY_MAX] = {};

static GAYA_REQUEST_BUF g_GayaRequestBuf = {};

static TEX_DAT g_Tex_GayZ = {};
static TEX_DAT g_Tex_GayaZgeneral = {};
static TEX_DAT g_Tex_GayaGraphBg = {};
static std::map<std::string, TEX_DAT> g_Tex_Icon = {};

static constexpr int GAYA_REQU_MAX = 16;
static GAYA_RQU_BUF g_gayaRequ[GAYA_REQU_MAX] = {};


void GE_InitGaya(void)
{
    g_Tex_GayZ = LoadTexData("TEX/ui/in_game_master/Gaya_Z");
    g_Tex_GayaZgeneral = LoadTexData("TEX/ui/in_game_master/Gaya_Z_general");
    g_Tex_GayaGraphBg = LoadTexData("TEX/ui/in_game_master/Time_on_old");

    //アイコン
    g_Tex_Icon["CA"] = LoadTexData("TEX/ui/in_game_master/G_characyor");
    g_Tex_Icon["MAYU"] = LoadTexData("TEX/ui/in_game_master/icon-assets/touno_icon");
    g_Tex_Icon["MAYU_SML"] = LoadTexData("TEX/ui/in_game_master/icon-assets/touno_smile");

    g_Tex_Icon["KAI"] = LoadTexData("TEX/ui/in_game_master/icon-assets/kaito_icon");
    g_Tex_Icon["NEO"] = LoadTexData("TEX/ui/in_game_master/icon-assets/neo_icon");
    g_Tex_Icon["KN"] = LoadTexData("TEX/ui/in_game_master/icon-assets/kontorius_icon");

    gayaDatInit();

    //int a;
}

void GE_UninitGaya(void)
{
    for (auto v : g_Tex_Icon)
    {
        UnloadTexData(v.second);
    }
    UnloadTexData(g_Tex_GayaZgeneral);
    UnloadTexData(g_Tex_GayZ);
    UnloadTexData(g_Tex_GayaGraphBg);
}

void GE_UpdateGaya(void)
{
    if (!IsGamePause() 
        //&& !IsGameStop()
        )
    {
        gayaStartRequBufUpdate();

        {
            int musengayanumMax = 0;
            for (int i = 0; i < GAYA_PLAY_MAX; i++)
            {
                if (g_PlayingGaya[i].enable)
                {
                    if (g_PlayingGaya[i].dat->type != GAYA_TYPE_PLAYER)
                    {
                        musengayanumMax++;
                    }
                }
            }
            int musengayanum = 0;
            for (int i = 0; i < GAYA_PLAY_MAX; i++)
            {
                if (g_PlayingGaya[i].enable)
                {
                    if (g_PlayingGaya[i].opt.graph.alpha > 0)
                    {
                        g_PlayingGaya[i].opt.graph.spriteAnim[0].no =
                            g_PlayingGaya[i].frame / 8
                            % g_PlayingGaya[i].opt.graph.spriteAnim[0].cntMax;
                    }
                    if (g_PlayingGaya[i].dat->type != GAYA_TYPE_PLAYER)
                    {
                        float y = -150.0f * (musengayanumMax - musengayanum - 1);

                        g_PlayingGaya[i].pos.y += (y - g_PlayingGaya[i].pos.y) / 12.0f;

                        musengayanum++;
                    }

                    g_PlayingGaya[i].frame++;
                    if (g_PlayingGaya[i].frame > g_PlayingGaya[i].frameMax)
                    {
                        g_PlayingGaya[i].enable = false;
                    }
                }
            }
        }
    }

    //if (Keyboard_IsKeyTrigger(KK_G))
    //{
    //    GE_GayaStartRequest("test");
    //}
}

void GE_DrawGaya(void)
{
    if (!IsVisibleUI_Game())
    {
        return;
    }

    for (int i = 0; i < GAYA_PLAY_MAX; i++)
    {
        if (g_PlayingGaya[i].enable)
        {
            float fInmod = std::max(std::min(
                (float)g_PlayingGaya[i].frame / PLAYING_GAYA_IN_FRAME,
                1.0f), 0.0f);
            float fOutmod = std::max(std::min(
                (float)(g_PlayingGaya[i].frame
                    - (g_PlayingGaya[i].frameMax - PLAYING_GAYA_OUT_FRAME))
                / PLAYING_GAYA_OUT_FRAME,
                1.0f), 0.0f);

            float ftxtmod = std::max(std::min(
                (float)(g_PlayingGaya[i].frame - PLAYING_GAYA_IN_FRAME)
                / g_PlayingGaya[i].dat->delay,
                1.0f), 0.0f);

            float fmod = (float)g_PlayingGaya[i].frame 
                / g_PlayingGaya[i].frameMax;

            //std::wstring txt = g_PlayingGaya.dat->txt;
            std::wstring txt = g_PlayingGaya[i].txt;
            std::fill(
                txt.begin() + (txt.size() * ftxtmod),
                txt.end(),
                ' '
            );

            unsigned int texGayaZ = g_Tex_GayZ.textureId;
            Float2 texScale = { 1.0f, 1.0f };
            Float2 posGraphBg;
            Float2 posGraph;
            Float2 posGayaBg;
            Float2 posGayaIcon;
            Float2 posFont;

            if (g_PlayingGaya[i].dat->type == "PLAYER")
            {
                float fadex =
                    +SCREEN_WIDTH * -0.5f * EasingInBack(1.0f - fInmod)
                    + SCREEN_WIDTH * -0.5f * EasingInBack(fOutmod);

                posGraphBg = { g_PlayingGaya[i].pos.x + SCREEN_WIDTH * 0.1f + fadex, g_PlayingGaya[i].pos.y + SCREEN_HEIGHT * 0.4f };
                posGraph   = { g_PlayingGaya[i].pos.x + SCREEN_WIDTH * 0.1f + fadex, g_PlayingGaya[i].pos.y + SCREEN_HEIGHT * 0.325f };
                posGayaBg  = { g_PlayingGaya[i].pos.x + SCREEN_WIDTH * -0.17f + fadex, g_PlayingGaya[i].pos.y + SCREEN_HEIGHT * 0.4f };
                posGayaIcon = { g_PlayingGaya[i].pos.x + SCREEN_WIDTH * -0.45f + fadex,
                    g_PlayingGaya[i].pos.y + SCREEN_HEIGHT * 0.4f };
                posFont = MkF2(
                    g_PlayingGaya[i].pos.x + SCREEN_WIDTH * -0.315f + -65 + fadex,
                    g_PlayingGaya[i].pos.y + SCREEN_HEIGHT * 0.395f
                );
                texGayaZ = g_Tex_GayaZgeneral.textureId;
                texScale = { -1.0f, 1.0f };
            }
            else
            {
                float fadex =
                    +SCREEN_WIDTH * 0.5f * EasingInBack(1.0f - fInmod)
                    + SCREEN_WIDTH * 0.5f * EasingInBack(fOutmod);

                //posGraphBg = { g_PlayingGaya[i].pos.x + SCREEN_WIDTH * -0.1f + fadex, g_PlayingGaya[i].pos.y + SCREEN_HEIGHT * 0.4f };
                //posGraph = { g_PlayingGaya[i].pos.x + SCREEN_WIDTH * -0.1f + fadex, g_PlayingGaya[i].pos.y + SCREEN_HEIGHT * 0.325f };
                posGraphBg = { 
                    0.0f,
                    SCREEN_HEIGHT * -0.38f + -fadex
                };
                posGraph = { 
                    posGraphBg.x,
                    posGraphBg.y + SCREEN_HEIGHT * -0.075f
                };

                posGayaBg = { g_PlayingGaya[i].pos.x + SCREEN_WIDTH * 0.17f + fadex, g_PlayingGaya[i].pos.y + SCREEN_HEIGHT * 0.4f };
                posGayaIcon = { g_PlayingGaya[i].pos.x + SCREEN_WIDTH * 0.45f + fadex, 
                    g_PlayingGaya[i].pos.y + SCREEN_HEIGHT * 0.4f };
                posFont = MkF2(
                    g_PlayingGaya[i].pos.x + -65 + fadex,
                    g_PlayingGaya[i].pos.y + SCREEN_HEIGHT * 0.395f
                );
            }

            if(g_PlayingGaya[i].opt.graph.alpha > 0)
            {
                DrawSpriteInCameraSingle(
                    0,
                    //表示座標 (四角形の真ん中)
                    posGraphBg.x,
                    posGraphBg.y,

                    //サイズ width hight
                    200.0f,
                    200.0f,

                    //角度
                    0.0f,

                    //色
                    MkF4(1.0f, 1.0f, 1.0f, 1.0f),

                    g_Tex_GayaGraphBg.textureId,  //画像(テクスチャ)の番号
                    MkF2(0.0f, 0.0f),
                    MkF2(1.0f, 1.0f),
                    DRAW_POLYGON_PRIORITY_FORCE_ALWAYS
                );

                SPRITE_GRAPH_DAT gd = g_PlayingGaya[i].opt.graph;

                DRAW_SPRITE_GRAPH_OPTION opt = {};
                opt.fixed = true;
                opt.startPos = {
                    posGraph.x,
                    posGraph.y
                };
                opt.priority = DRAW_POLYGON_PRIORITY_FORCE_ALWAYS;
                opt.startScale = {
                    0.35f, 
                    0.35f
                };
                opt.depthBuffIgnore = true;
                opt.colorMul = {1.0f, 1.0f, 1.0f, 1.0f };

                DrawSpriteGraphEx(
                    &gd,
                    opt
                );
            }
            
            {
                DrawSpriteInCameraSingle(
                    0,
                    //表示座標 (四角形の真ん中)
                    posGayaBg.x,
                    posGayaBg.y,

                    //サイズ width hight
                    900.0f * texScale.x,
                    900.0f * texScale.y,

                    //角度
                    0.0f,

                    //色
                    MkF4(1.0f, 1.0f, 1.0f, 1.0f),

                    texGayaZ,  //画像(テクスチャ)の番号
                    MkF2(0.0f, 0.0f),
                    MkF2(1.0f, 1.0f),
                    DRAW_POLYGON_PRIORITY_FORCE_ALWAYS
                );

                for (auto v : g_Tex_Icon)
                {
                    if (v.first == g_PlayingGaya[i].dat->icon)
                    {
                        DrawSpriteInCameraSingle(
                            0,
                            //表示座標 (四角形の真ん中)
                            posGayaIcon.x,
                            posGayaIcon.y,

                            //サイズ width hight
                            160.0f * ((float)g_Tex_Icon[g_PlayingGaya[i].dat->icon].width
                                / g_Tex_Icon[g_PlayingGaya[i].dat->icon].height),
                            160.0f,

                            //角度
                            0.0f,

                            //色
                            MkF4(1.0f, 1.0f, 1.0f, 1.0f),

                            v.second.textureId,  //画像(テクスチャ)の番号
                            //g_Tex_Icon[g_PlayingGaya.dat->icon].textureId,  //画像(テクスチャ)の番号
                            MkF2(0.0f, 0.0f),
                            MkF2(1.0f, 1.0f),
                            DRAW_POLYGON_PRIORITY_FORCE_ALWAYS
                        );
                        break;
                    }
                }
            }

            {
                DRAW_FONT_DAT dat = {};
                dat.txt = txt;
                dat.fonttype = FONT_TYPE_STICK;
                dat.pos = MkF2(
                    posFont.x,
                    posFont.y
                );
                dat.fontSize = 40;
                dat.posShadow = MkF2(
                    (float)dat.fontSize / 13.0f,
                    (float)dat.fontSize / 13.0f
                );
                dat.color = FONT_TEXT_COLOR_WHITE;
                dat.colorShadow = FONT_TEXT_COLOR_BLACK;
                dat.textAlign = FONT_TEXT_ALIGNMENT_LEFT;
                //dat.maxWidth = dat.fontSize * 20.0f;
                //dat.maxHight = dat.fontSize * 1.0f;
                dat.LineSpacingWidth = dat.fontSize;
                dat.LineSpacingHight = dat.fontSize;

                dat.pos.y += dat.fontSize * (float)g_PlayingGaya[i].dat->hCnt / -2;

                dat.sizeEx = g_PlayingGaya[i].dat->sizeEx;
                dat.drawingEffect = g_PlayingGaya[i].dat->drawingEffect;

                DrawFont(dat);
            }
        }
    }
}

void gayaStartRequest(GAYA_RQU_BUF gayabuf)
{
    if (gayabuf.dat == nullptr)
    {
        return;
    }

    for (int i = 0; i < GAYA_PLAY_MAX; i++)
    {
        //if (!g_PlayingGaya[i].enable ||
        //    g_PlayingGaya[i].dat == nullptr ||
        //    g_PlayingGaya[i].dat->priority >= gayabuf.dat->priority
        //    )
        if (!g_PlayingGaya[i].enable ||
            g_PlayingGaya[i].dat == nullptr ||
            (g_PlayingGaya[i].dat->priority >= gayabuf.dat->priority
                && i == GAYA_PLAY_MAX - 1)
        )
        {
            if (gayabuf.dat->sound != GAYA_SOUND_TYPE_NONE)
            {
                //SoundEffectPlayRequest(GetSoundData("SOUND/SE/etc/Gaya_Sound"), 0.55f, 10);
                SoundEffectPlayRequest(GetSoundData("SOUND/SE/etc/Gaya_Sound"), 0.35f, 10);
            }

            if (!gayabuf.opt.selfOnly)
            {
                strcpy_s(g_GayaRequestBuf.name, GAYA_TXT_MAX, gayabuf.dat->name.c_str());
                g_GayaRequestBuf.opt = gayabuf.opt;
                g_GayaRequestBuf.cancelMode = false;
            }

            g_PlayingGaya[i] = {};
            g_PlayingGaya[i].enable = true;
            g_PlayingGaya[i].dat = gayabuf.dat;
            g_PlayingGaya[i].txt = gayabuf.txt;
            g_PlayingGaya[i].frameMax = g_PlayingGaya[i].dat->frameMax;
            g_PlayingGaya[i].id = gayabuf.id;
            g_PlayingGaya[i].opt = gayabuf.opt;
            break;
        }
    }
}

void gayaStartRequAddBuf(GAYA_RQU_BUF* gayarequbuf)
{
    for (int i = 0; i < GAYA_REQU_MAX; i++)
    {
        if (!g_gayaRequ[i].enable)
        {
            g_gayaRequ[i] = *gayarequbuf;
            break;
        }
    }
}
void gayaStartRequBufUpdate()
{
    for (int i = 0; i < GAYA_REQU_MAX; i++)
    {
        if (g_gayaRequ[i].enable)
        {
            g_gayaRequ[i].delay--;
            if (g_gayaRequ[i].delay <= 0)
            {
                g_gayaRequ[i].enable = false;
                gayaStartRequest(g_gayaRequ[i]);
            }
        }
    }
}

std::string  GE_GayaStartRequest(std::string name, int delay,
    GAYA_REQUEST_OPT opt
)
{
    std::string idr = "";

    bool exclusiveEnd = false;
    if(opt.exclusive)
    {
        for (int i = 0; i < GAYA_PLAY_MAX; i++)
        {
            if (g_PlayingGaya[i].enable && g_PlayingGaya[i].dat
                && g_PlayingGaya[i].dat->name == name
            )
            {
                exclusiveEnd = true;
                break;
            }
        }
        if (!exclusiveEnd)
        {
            for (int i = 0; i < GAYA_REQU_MAX; i++)
            {
                if (g_gayaRequ[i].enable && g_gayaRequ[i].dat
                    && g_gayaRequ[i].dat->name == name
                )
                {
                    exclusiveEnd = true;
                    break;
                }
            }
        }
    }
    if (exclusiveEnd)
    {
        return idr;
    }

    for (int i = 0; i < gayaDat.size(); i++)
    {
        if (gayaDat[i].name == name)
        {
            GAYA_RQU_BUF gbuf = {};
            gbuf.enable = true;
            gbuf.delay = delay;
            gbuf.dat = &gayaDat[i];

            idr = std::to_string(i) + "-" + std::to_string(GetMainGameFrame());
            gbuf.id = idr;
            gbuf.opt = opt;

            //if(opt.repdat.size() > 0)
            //{
            //    std::wstring strtmp = gbuf.dat->txt;

            //    for (const auto& v : opt.repdat)
            //    {
            //        size_t pos = 0;
            //        while ((pos = strtmp.find(v.oldStr, pos)) != std::wstring::npos)
            //        {
            //            strtmp.replace(pos, v.oldStr.size(), v.newStr);
            //            pos += v.newStr.length();
            //            break;
            //        }
            //    }
            //    gbuf.txt = gbuf.dat->txt;
            //}
            //else
            {
                gbuf.txt = gbuf.dat->txt;
            }

            gayaStartRequAddBuf(&gbuf);
            break;
        }
    }
    return  idr;
}

void GE_GayaCancelRequest(std::string id)
{
    if (id.size() <= 0)
    {
        return;
    }

    strcpy_s(g_GayaRequestBuf.name, GAYA_TXT_MAX, id.c_str());
    g_GayaRequestBuf.cancelMode = true;

    for (int i = 0; i < GAYA_REQU_MAX; i++)
    {
        if (g_gayaRequ[i].enable)
        {
            if (g_gayaRequ[i].id == id)
            {
                g_gayaRequ[i].enable = false;
                break;
            }
        }
    }
    for (int i = 0; i < GAYA_PLAY_MAX; i++)
    {
        if (g_PlayingGaya[i].enable)
        {
            if (g_PlayingGaya[i].id == id)
            {
                g_PlayingGaya[i].frame = g_PlayingGaya[i].frameMax - PLAYING_GAYA_OUT_FRAME;
                break;
            }
        }
    }
}
void GE_GayaCancelRequestAll()
{
    g_GayaRequestBuf.name[0] = 0;
    g_GayaRequestBuf.cancelMode = true;

    for (int i = 0; i < GAYA_REQU_MAX; i++)
    {
        if (g_gayaRequ[i].enable)
        {
            g_gayaRequ[i].enable = false;
        }
    }
    for (int i = 0; i < GAYA_PLAY_MAX; i++)
    {
        if (g_PlayingGaya[i].enable)
        {
            g_PlayingGaya[i].frame = g_PlayingGaya[i].frameMax - PLAYING_GAYA_OUT_FRAME;
        }
    }
}

GAYA_REQUEST_BUF* GE_GetGayaRequestBuf()
{
    return &g_GayaRequestBuf;
}

void GE_GayaRequestBufRelease()
{
    g_GayaRequestBuf = {};
}


void gayaDatInit(void)
{
    gayaDat.clear();

    std::string folderPath = GAYA_PATH;

    {
        std::string folderPathChild = folderPath;

        std::vector<std::string> fileNames;

        GetFileNames(folderPathChild, fileNames);

        //指定のフォルダ内を調べる
        for (const auto& fname : fileNames)
        {
            // ファイルを開く
            FILE* fp = nullptr;

            std::string pathstr = folderPathChild + std::string("/") + fname;

            fopen_s(&fp, pathstr.c_str(), "rb");

            if (fp != nullptr)
            {
                GAYA_DAT newGayaDat = {};
                std::wstring gayaText = L"";

                {
                    char cpytxt[GAYA_TXT_MAX] = {};
                    char* cpytxtContext = nullptr;
                    strcpy_s(cpytxt, GAYA_TXT_MAX, fname.c_str());

                    char* token = strtok_s(cpytxt, ".", &cpytxtContext);
                    if (token != NULL)
                    {
                        //strcpy_s(newGayaDat.name, GAYA_TXT_MAX, cpytxt);
                        newGayaDat.name = cpytxt;
                    }
                }

                bool starttxt = false;

                char buf[GAYA_TXT_MAX] = {};
                while (fgets(buf, sizeof(buf), fp))
                {
                    std::string buf2 = std::string(buf);
                    buf2.erase(remove(buf2.begin(), buf2.end(), ' '), buf2.end());

                    char cpytxt[GAYA_TXT_MAX] = {};
                    char* cpytxtContext = nullptr;
                    strcpy_s(cpytxt, GAYA_TXT_MAX, buf2.c_str());

                    char* token = strtok_s(cpytxt, "//", &cpytxtContext);
                    if (token != NULL && std::string(token).size() > 0)
                    {
                        //これ以降はテキストの読み込みとする
                        if (NormalizeString(cpytxt) == "<text>")
                        {
                            starttxt = true;
                            continue;
                        }

                        if (starttxt)
                        {
                            if (gayaText.size() > 0)
                            {
                                gayaText += L"\n";
                            }
                            newGayaDat.hCnt++;
                            //gayaText += NormalizeString(cpytxt);
                            gayaText += StringToWString(cpytxt);
                        }
                        else
                        {
                            std::string loadTestBuf = std::string(token);
                            token = strtok_s(cpytxt, ":", &cpytxtContext);
                            if (token != NULL && std::string(token).size() > 0)
                            {
                                std::string typeBuf = std::string(token);
                                if (cpytxtContext != NULL && std::string(cpytxtContext).size() > 0)
                                {
                                    std::string valueBuf = std::string(cpytxtContext);

                                    if (typeBuf == "icon")
                                    {
                                        newGayaDat.icon = NormalizeString(valueBuf);
                                    }
                                    if (typeBuf == "frameMax")
                                    {
                                        newGayaDat.frameMax = atoi(valueBuf.c_str());
                                    }
                                    if (typeBuf == "delay")
                                    {
                                        newGayaDat.delay = atoi(valueBuf.c_str());
                                    }
                                    if (typeBuf == "priority")
                                    {
                                        newGayaDat.priority = atoi(valueBuf.c_str());
                                    }
                                    if (typeBuf == "type")
                                    {
                                        newGayaDat.type = NormalizeString(valueBuf);
                                    }
                                    if (typeBuf == "sound")
                                    {
                                        newGayaDat.sound = NormalizeString(valueBuf);
                                    }
                                }
                            }
                        }
                    }
                }


                std::wstring ssL[FONT_TEXT_COLOR_MAX] = {
                    L"<white>",
                    L"<black>",
                    L"<red>",
                    L"<orange>",
                    L"<yellow>",
                    L"<green>",
                    L"<lightblue>",
                    L"<blue>",
                    L"<purple>",
                };
                std::wstring esL[FONT_TEXT_COLOR_MAX] = {
                    L"<whiteE>",
                    L"<blackE>",
                    L"<redE>",
                    L"<orangeE>",
                    L"<yellowE>",
                    L"<greenE>",
                    L"<lightblueE>",
                    L"<blueE>",
                    L"<purpleE>",
                };

                for(int i = 0; i < FONT_TEXT_COLOR_MAX; i ++)
                {
                    //std::wstring ss = L"<red>";
                    //std::wstring es = L"<redE>";
                    std::wstring ss = ssL[i];
                    std::wstring es = esL[i];

                    size_t spos = -1;
                    while(1) 
                    {
                        spos = gayaText.find(ss);

                        if (spos == std::wstring::npos)
                        {
                            break;
                        }
                        else
                        {
                            gayaText.erase(spos, ss.size());

                            size_t epos = -1;
                            while (1)
                            {
                                epos = gayaText.find(es);

                                if (epos == std::wstring::npos)
                                {
                                    break;
                                }
                                else
                                {
                                    gayaText.erase(epos, es.size());

                                    {
                                        DX_DrawString_SetDrawingEffect ef = {};
                                        ef.start = spos;
                                        ef.length = ((epos)-ef.start);
                                        ef.drawingEffect
                                            = GetFontColorBrush((FONT_TEXT_COLOR)i);
                                        newGayaDat.drawingEffect.push_back(ef);
                                    }

                                    break;
                                }
                            }
                        }
                    }
                }

                //newGayaDat.txt = StringToWString(gayaText);
                newGayaDat.txt = gayaText;

                gayaDat.push_back(newGayaDat);

                // ファイルを閉じる
                fclose(fp);
            }
        }
    }
}