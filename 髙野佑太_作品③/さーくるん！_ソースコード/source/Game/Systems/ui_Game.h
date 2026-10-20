// ===================================================
// ui_Game.h ui制御
// 
// hara sougo  2024/12/05
// ===================================================

#ifndef _UI_GAME_H_
#define _UI_GAME_H_


// ===================================================
// 列挙体宣言
// ===================================================
enum class BOSSBAR_TYPE_GAME {
    Inperius = 0,
    Inperius_H,
    Contrius,
    Contrius_H,
};
// ===================================================
// 構造体宣言
// ===================================================

// ===================================================
// プロトタイプ宣言
// ===================================================

void InitUI_Game();
void UninitUI_Game();
void UpdateUI_Game();
void DrawUI_Game();

bool ShowHpbar_Game(int index, bool);
bool ShowBossbar_Game(int index, bool, std::string,
    BOSSBAR_TYPE_GAME = BOSSBAR_TYPE_GAME::Inperius,
    float min = 0.0f, float max = 1.0f
);


//flag  0 in  1 in  2 break
void ShowBossbarBarrier_Game(int index, int flag);
bool IsEnableBossbarBarrier_Game(int index);
bool IsBreakingBossbarBarrier_Game(int index);

int GetBossbarObjIndex_Game();

void UIReset_Game();

void UIMovieFlagInstant_Game();
bool IsVisibleUI_Game();

void SetUIScoreVisible_Game(bool);
void SetUITimerVisible_Game(bool);

void SetUIEventTimer_Game(bool, const int* =nullptr, int =0, int =3, bool = false);

float GetUIMovieFlagAlpha_Game();

#endif