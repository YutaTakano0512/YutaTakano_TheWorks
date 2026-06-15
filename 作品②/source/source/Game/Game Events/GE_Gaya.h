// ===================================================
// GE_Gaya.h ガヤの処理
// 
// hara sougo  2025/02/12
// ===================================================
#ifndef _GE_GAYA_H_
#define _GE_GAYA_H_


#include "../Systems/sys_general.h"

#include "../Sprite/spriteGraph.h"

// ===================================================
// マクロ定義
// ===================================================
constexpr auto GAYA_PATH = "rom/gaya";

constexpr int GAYA_TXT_MAX = 64;

// ===================================================
// 構造体宣言
// ===================================================

struct GAYA_REQUEST_RELACE {
    std::wstring oldStr;
    std::wstring newStr;
};
struct GAYA_REQUEST_OPT {
    bool selfOnly;
    SPRITE_GRAPH_DAT graph;
    bool exclusive;

    //std::vector<GAYA_REQUEST_RELACE> repdat;
};

struct GAYA_REQUEST_BUF {
    bool cancelMode;
    char name[GAYA_TXT_MAX];
    GAYA_REQUEST_OPT opt;
};

// ===================================================
// プロトタイプ宣言
// ===================================================
#ifdef IS_GAME_EVENT_MANAGER
void GE_InitGaya(void);
void GE_UninitGaya(void);
void GE_UpdateGaya(void);
void GE_DrawGaya(void);
#endif

std::string GE_GayaStartRequest(std::string name, int delay = 0,
    GAYA_REQUEST_OPT opt = {});
void GE_GayaCancelRequest(std::string id);
void GE_GayaCancelRequestAll();

GAYA_REQUEST_BUF* GE_GetGayaRequestBuf();
void GE_GayaRequestBufRelease();


#endif