// ===================================================
// ui_Game.cpp ui制御
// 
// hara sougo  2024/12/05
// ===================================================
#include "../Configuration/configuration.h"
#include "../../game_main.h"
#include "easing.h"
#include "../Texture/texData.h"
#include "../Sprite/sprite.h"
#include "../Sprite/spriteNumber.h"
#include "ui_Game.h"

#include "collision.h"

#include "../Entities/Block/block.h"
#include "../Game Object/gameObject.h"
#include "../Game Object/Game Object Events/GOE_Character_Player.h"
#include "../Entities/player.h"

#include "font.h"
#include "fade.h"
#include "camera.h"

#include "../Managers/gameManager.h"
#include "../Managers/scene.h"
#include "../../Scenes/SCENE_Game.h"

//#include "../../Core/DirectX/dx.h"
//#include "../../Core/DirectX/dx_camera.h"
//#include <directxcollision.h>
//#include "gameManager.h"

// ===================================================
// マクロ定義
// ===================================================
#define UI_RATE_MAX							(10000)

#define GAMEFRAME_ENTER_FRAME				(30)

//HPゲージ
//#define HPBAR_CHANGE_SPEED					(30)
#define HPBAR_CHANGE_SPEED					(50)
#define HPBAR_CHANGE_FRAME_MAX				(30)
#define HPBAR_CHANGE_FLASH_FRAME			(20)
#define HPBAR_CHANGE_FLASH_CYCLE_FRAME		(6)
#define HPBAR_WAIT_FRAME					(50)
#define HPBAR_ENTER_SPEED					(130)
#define HPBAR_ENTER_FRAME_MAX				(20)
#define HPBAR_SHAKE_POWER_X					(8.0f)
#define HPBAR_SHAKE_POWER_Y					(10.0f)

#define HPBAR_PLAYER_OVERLAP_RATE_SPEED		(900)
#define HPBARBG_PLAYER_OVERLAP_ALPHA		(0.25f)
#define HPBAR_PLAYER_OVERLAP_ALPHA			(0.6f)

//↓ぼすのHPげーじ
#define BOSSBAR_OFFSET_X					(SCREEN_WIDTH * -0.016f)
#define BOSSBAR_OFFSET_Y					(SCREEN_HEIGHT * -0.42f)

//#define BOSSBAR_SIZE_X						(1100)
#define BOSSBAR_SIZE_X						(1000)
#define BOSSBAR_SIZE_Y						(BOSSBAR_SIZE_X)

//#define BOSSBARBG_SIZE_X					(BOSSBAR_SIZE_X * 1.055f)
//#define BOSSBARBG_SIZE_Y					(BOSSBAR_SIZE_Y * 1.055f)
#define BOSSBARBG_SIZE_X					(BOSSBAR_SIZE_X * 1.05f)
#define BOSSBARBG_SIZE_Y					(BOSSBAR_SIZE_Y * 1.05f)

#define BOSSBAR_PAD_X						((BOSSBARBG_SIZE_X - BOSSBAR_SIZE_X) * 0.045f)
#define BOSSBAR_PAD_Y						((BOSSBARBG_SIZE_Y - BOSSBAR_SIZE_Y) * 0.34f)

#define BOSSBAR_SHAKE_X						(6.0f)
#define BOSSBAR_SHAKE_Y						(12.0f)

#define BOSSBAR_BARRIER_NUM					(4)
#define BOSSBAR_BARRIER_IN_F				(16)
#define BOSSBAR_BARRIER_OUT_F				(20)
#define BOSSBAR_BARRIER_BREAK_F				(25)

//ぷれいやーのHPゲージ
#define HPBAR_SCALE							(0.6f * 0.9f)
#define HPBAR_OFFSET_X						(SCREEN_WIDTH * -0.298f)
#define HPBAR_OFFSET_Y						(SCREEN_HEIGHT * 0.39f)
//#define HPBAR_PAD_X							(31)
#define HPBAR_PAD_X							(0)
#define HPBAR_PAD_Y							(-5)
//#define HPBAR_SIZE_X						(477)
//#define HPBAR_SIZE_Y						(300)
#define HPBAR_SIZE_X						(500)
#define HPBAR_SIZE_Y						(HPBAR_SIZE_X)
//#define HPBARBG_SIZE_X						(800)
#define HPBARBG_SIZE_X						(HPBAR_SIZE_X * 1.03f)
#define HPBARBG_SIZE_Y						(HPBAR_SIZE_Y * 1.03f)
#define HPBAR_SHAKE_X						(5.0f)
#define HPBAR_SHAKE_Y						(6.0f)


//インク
#define INKBAR_SCALE						(0.75f * 0.9f)
#define INKBAR_OFFSET_X						(SCREEN_WIDTH * -0.275f)
#define INKBAR_OFFSET_Y						(SCREEN_HEIGHT * 0.43f)

#define INKBAR_PAD_X						(0)
#define INKBAR_PAD_Y						(-4)

#define INKBAR_SIZE_X						(500 * 1.075f)
#define INKBAR_SIZE_Y						(INKBAR_SIZE_X)

#define INKBARBG_SIZE_X						(INKBAR_SIZE_X * 1.01f)
#define INKBARBG_SIZE_Y						(INKBAR_SIZE_Y * 1.01f)

//すこあ
//#define SCORE_OFFSET_X						(SCREEN_WIDTH * -0.32f)
//#define SCORE_OFFSET_Y						(SCREEN_HEIGHT * -0.41f)
//#define SCORE_OFFSET_X						(SCREEN_WIDTH * 0.45f)
//#define SCORE_OFFSET_Y						(SCREEN_HEIGHT * -0.2f)
//#define SCORE_OFFSET_X						(SCREEN_WIDTH * -0.38)
#define SCORE_OFFSET_X						(SCREEN_WIDTH * 0.38)
//#define SCORE_OFFSET_Y						(SCREEN_HEIGHT * -0.41f)
//#define SCORE_OFFSET_Y						(SCREEN_HEIGHT * -0.415f)
#define SCORE_OFFSET_Y						(SCREEN_HEIGHT * -0.4f)
#define SCORE_SIZE_X						(50)
#define SCORE_SIZE_Y						(50)
#define SCORE_ADD_DAT_MAX					(32)

//ムービー帯
#define MOVIE_FLAG_FRAME_MAX				(60)
// ===================================================
// 構造体宣言
// ===================================================
struct UI_BAR_DAT {
    bool beforeEnable;

	int barMain;
	int barSub;
	int barBuf;	//HPゲージの差分用変数
	int barBuf2;	//HPゲージの差分用変数
	short barChangeFrame;
	short barEnterFrame; //HPゲージ出現時用の変数
	short barStopFrame; //HPゲージが制止するフレーム
	int barShakeFrame;

	bool isPlayerOverlap;
	int playerOverlapRate;

	Float2 size;
	Float2 pos;
};

struct UI_BOSSBAR_BARRIER_DAT {
    bool enable;

    int inF;
    int outF;
    int breakF;
};
struct UI_BOSSBAR_DAT {
    bool enable;
    GAME_OBJECT* obj;
    //BOSSBAR_NAME name;
    std::string name;
    BOSSBAR_TYPE_GAME type;

    UI_BAR_DAT bar;

    float min;
    float max;

    UI_BOSSBAR_BARRIER_DAT barrier[BOSSBAR_BARRIER_NUM];
};
struct UI_HPBAR_DAT {
	bool enable;

	UI_BAR_DAT bar;
	bool danger;

    PLAYER* player;
    GOE_Character_Player* playerGOE;
};

struct UI_INK_BAR_DAT {
	bool enable;

	UI_BAR_DAT bar;
};


struct UI_SCORE_ADD_DAT {
	bool enable;

	int score;

	int frame;
	int frameMax;
	int index;
};
struct UI_SCORE_DAT {
    bool enable;

    int score;
    UI_SCORE_ADD_DAT scoreAddDat[SCORE_ADD_DAT_MAX];

    int addFrame;
    int addFrameMax;
};

struct UI_EVENTTIMER_DAT {
    bool enable;

    int digit;
    bool rev;

    const int* frame;
    int frameMax;


    int enterFrame;
    int countEffectFrame;
};

struct UI_DAT {		// UI構造体
	bool gameFrameView;
	short gameFrameEnterFrame;

	UI_HPBAR_DAT hpbar[PLAYER_MAX];
	UI_INK_BAR_DAT inkbar[PLAYER_MAX];

	UI_BOSSBAR_DAT bossbar;
	UI_SCORE_DAT score;

    UI_EVENTTIMER_DAT eventTimer;

    short movieFlagFrame;

    bool scoreVisible;
    bool timerVisible;
};

// ===================================================
// プロトタイプ宣言
// ===================================================
static void barUpdate_Game(UI_BAR_DAT* bar, int current);
static void update_player_gui(unsigned int gframe, int i, int cIndex, Float4 colorAlphaSetTmp, float movieFlagAlpha,
    bool isSub, int subindex
);

// ===================================================
// グローバル変数
// ===================================================
static UI_DAT UIDat = {};

static TEX_DAT g_tex_icon_player = {};
static TEX_DAT g_tex_icon_enemy_small = {};
static TEX_DAT g_tex_icon_enemy_midium = {};
static TEX_DAT g_tex_icon_enemy_boss = {};

static unsigned int g_tex_bossbar_main = {};
static unsigned int g_tex_bossbar_sub = {};

static unsigned int g_tex_bossbar_bg_Inperius = {};
static unsigned int g_tex_bossbar_bg_Inperius_H = {};
static unsigned int g_tex_bossbar_bg_Contrius = {};
static unsigned int g_tex_bossbar_bg_Contrius_H = {};

static unsigned int g_tex_bossbar_barrier = {};
static unsigned int g_tex_bossbar_barrier_sprite = {};


static unsigned int g_tex_Charactor_Z = {};

static unsigned int g_tex_hpbar_main = {};
static unsigned int g_tex_hpbar_sub = {};
static unsigned int g_tex_hpbar_bg = {};
static unsigned int g_tex_hpbarS_main = {};
static unsigned int g_tex_hpbarS_sub = {};
static unsigned int g_tex_hpbarS_bg = {};

static unsigned int g_tex_ink_par = {};
static unsigned int g_tex_ink_main = {};
static unsigned int g_tex_ink_bg = {};

static unsigned int g_tex_head_ink_bg = {};
static unsigned int g_tex_head_ink_main = {};
static unsigned int g_tex_head_ink_main2 = {};
static unsigned int g_tex_head_ink_cross = {};
static unsigned int g_tex_head_ink_text_pinch = {};
static unsigned int g_tex_head_ink_text_danger = {};

static unsigned int g_tex_chara_icon_hp = {};
static unsigned int g_tex_chara_icon_ink = {};

constexpr int CHARA_ICON_NUM = 4;
static unsigned int g_tex_chara_icon_num[CHARA_ICON_NUM] = {};
static unsigned int g_tex_chara_icon_num_tri[CHARA_ICON_NUM] = {};

static unsigned int g_tex_btn_x = {};
static unsigned int g_tex_btn_y = {};
static unsigned int g_tex_timer_on = {};
static unsigned int g_tex_timer_off = {};

static unsigned int g_score_up_sprite = {};
static unsigned int g_score_up_sprite_w = {};
static unsigned int g_score_bg = {};
static unsigned int g_score_eff = {};

static unsigned int g_tex_btn_startskip = {};

static unsigned int g_tex_Meter_bk_enter = {};
static unsigned int g_tex_Meter_bk = {};
static unsigned int g_tex_Meter_eff = {};
static unsigned int g_tex_Meter_time_gray = {};
static unsigned int g_tex_Meter_UI = {};

static int g_ShaderIndex_UIBgGeneral_Vertex = {};
static int g_ShaderIndex_UIBgGeneral_Pixel = {};
static int g_ShaderIndex_GageGeneral_Pixel = {};
static int g_ShaderIndex_GageGradient_Vertex = {};
static int g_ShaderIndex_GageGradient_Pixel = {};
static int g_ShaderIndex_HpbarGradient_Pixel = {};
static int g_ShaderIndex_HpbarSubGradient_Pixel = {};
static int g_ShaderIndex_BossbarGradient_Pixel = {};
static int g_ShaderIndex_BossbarSubGradient_Pixel = {};
static int g_ShaderIndex_FadeClock_Pixel = {};
static int g_ShaderIndex_EventTimerEnterNumber_Pixel = {};


static SPRITE_NUMBER_DRAW_EX g_number_template = { };

static float g_movieFlagAlpha = 1.0f;

#if defined(_DEBUG) || defined(DEBUG)
static bool g_uiinfoView = true;
#endif

void InitUI_Game()
{
	UIDat = {};

	g_tex_icon_player = GetTexData(TEX_TYPE_UI_ICON_PLAYER);
	g_tex_icon_enemy_small = GetTexData(TEX_TYPE_UI_ICON_ENEMY_SMALL);
	g_tex_icon_enemy_midium = GetTexData(TEX_TYPE_UI_ICON_ENEMY_MIDIUM);
	g_tex_icon_enemy_boss = GetTexData(TEX_TYPE_UI_ICON_ENEMY_BOSS);

    g_score_up_sprite = LoadTexData("TEX/ui/in_game_master/score_up_sprite").textureId;
    g_score_up_sprite_w = LoadTexData("TEX/ui/in_game_master/score_sprite").textureId;

	g_tex_bossbar_main	= LoadTexData("TEX/ui/in_game_master/Boss_HP_red").textureId;
	g_tex_bossbar_sub	= LoadTexData("TEX/ui/in_game_master/Boss_HP_yellow").textureId;

    g_tex_bossbar_bg_Inperius = LoadTexData("TEX/ui/in_game_master/Boss").textureId;
    g_tex_bossbar_bg_Inperius_H = LoadTexData("TEX/ui/in_game_master/Boss2").textureId;
    g_tex_bossbar_bg_Contrius = LoadTexData("TEX/ui/in_game_master/Boss3").textureId;
    g_tex_bossbar_bg_Contrius_H = LoadTexData("TEX/ui/in_game_master/Boss4").textureId;

    g_tex_bossbar_barrier = LoadTexData("TEX/ui/in_game_master/barrier").textureId;
    g_tex_bossbar_barrier_sprite = LoadTexData("TEX/ui/in_game_master/barrier_sprite").textureId;


	g_tex_Charactor_Z = LoadTexData("TEX/ui/in_game_master/Charactor_Z").textureId;

	g_tex_hpbar_main	= LoadTexData("TEX/ui/in_game_master/HP_m_bar_on").textureId;
	g_tex_hpbar_sub		= LoadTexData("TEX/ui/in_game_master/HP_m_bar_on").textureId;
	g_tex_hpbar_bg      = LoadTexData("TEX/ui/in_game_master/HP_m_bar_off").textureId;

    g_tex_hpbarS_main = LoadTexData("TEX/ui/in_game_master/HP_s_bar_on").textureId;
    g_tex_hpbarS_sub = LoadTexData("TEX/ui/in_game_master/HP_s_bar_on").textureId;
    g_tex_hpbarS_bg = LoadTexData("TEX/ui/in_game_master/HP_s_bar_off").textureId;

	g_tex_ink_par = LoadTexData("TEX/ui/in_game_master/ink_bar_partition").textureId;
    g_tex_ink_main = LoadTexData("TEX/ui/in_game_master/Ink_bar_on").textureId;
    g_tex_ink_bg = LoadTexData("TEX/ui/in_game_master/Ink_bar_off").textureId;
    g_tex_head_ink_bg = LoadTexData("TEX/ui/in_game_master/head_Ink_bar_off").textureId;
    g_tex_head_ink_main = LoadTexData("TEX/ui/in_game_master/head_Ink_bar_on").textureId;
    g_tex_head_ink_main2 = LoadTexData("TEX/ui/in_game_master/head_Ink_bar_on_2").textureId;
    g_tex_head_ink_cross = LoadTexData("TEX/ui/in_game_master/head_Ink_bar_cross").textureId;
    g_tex_head_ink_text_pinch = LoadTexData("TEX/ui/in_game_master/head_Ink_bar_text_pinch").textureId;
    g_tex_head_ink_text_danger = LoadTexData("TEX/ui/in_game_master/head_Ink_bar_text_danger").textureId;

    g_tex_chara_icon_hp = LoadTexData("TEX/ui/in_game_master/HP_icon").textureId;
    g_tex_chara_icon_ink = LoadTexData("TEX/ui/in_game_master/INK_icon").textureId;

    g_tex_chara_icon_num[0] = LoadTexData("TEX/ui/in_game_master/1P_icon").textureId;
    g_tex_chara_icon_num[1] = LoadTexData("TEX/ui/in_game_master/2P").textureId;
    g_tex_chara_icon_num[2] = LoadTexData("TEX/ui/in_game_master/3P").textureId;
    g_tex_chara_icon_num[3] = LoadTexData("TEX/ui/in_game_master/4P").textureId;

    g_tex_chara_icon_num_tri[0] = LoadTexData("TEX/ui/in_game_master/1P_tri").textureId;
    g_tex_chara_icon_num_tri[1] = LoadTexData("TEX/ui/in_game_master/2P_tri").textureId;
    g_tex_chara_icon_num_tri[2] = LoadTexData("TEX/ui/in_game_master/3P_tri").textureId;
    g_tex_chara_icon_num_tri[3] = LoadTexData("TEX/ui/in_game_master/4P_tri").textureId;

    g_tex_btn_x = LoadTexData("TEX/ui/in_game_master/X_Button").textureId;
    g_tex_btn_y = LoadTexData("TEX/ui/in_game_master/Y_Button").textureId;

    g_tex_timer_on = LoadTexData("TEX/ui/in_game_master/Time_on").textureId;
    g_tex_timer_off = LoadTexData("TEX/ui/in_game_master/Time_off").textureId;
    g_score_bg = LoadTexData("TEX/ui/in_game_master/Score_icon_Z").textureId;
    g_score_eff = LoadTexData("TEX/effect/eff_score").textureId;

    g_tex_btn_startskip = LoadTexData("TEX/ui/in_game_master/movieskip").textureId;

    g_tex_Meter_bk_enter = LoadTexData("TEX/ui/in_game_master/countdown/Meter_bk_enter").textureId;
    //g_tex_Meter_bk_enter = LoadTexData("TEX/ui/in_game_master/Time_off").textureId;
    g_tex_Meter_bk = LoadTexData("TEX/ui/in_game_master/countdown/Meter_bk").textureId;
    g_tex_Meter_eff = LoadTexData("TEX/ui/in_game_master/countdown/Meter_eff").textureId;
    g_tex_Meter_time_gray = LoadTexData("TEX/ui/in_game_master/countdown/Meter_time_gray").textureId;
    g_tex_Meter_UI = LoadTexData("TEX/ui/in_game_master/countdown/Meter_UI").textureId;

    g_ShaderIndex_UIBgGeneral_Vertex = DX_GetShaderIndex("SHADER/vs_general");
    g_ShaderIndex_GageGeneral_Pixel = DX_GetShaderIndex("SHADER/general/ps_default");
    g_ShaderIndex_GageGradient_Vertex = DX_GetShaderIndex("SHADER/vs_general");
    g_ShaderIndex_GageGradient_Pixel = DX_GetShaderIndex("SHADER/general/ps_gage_gradient");
    g_ShaderIndex_HpbarGradient_Pixel = DX_GetShaderIndex("SHADER/general/ps_hpbar_gradient");
    g_ShaderIndex_HpbarSubGradient_Pixel = DX_GetShaderIndex("SHADER/general/ps_hpbar_sub_gradient");
    g_ShaderIndex_BossbarGradient_Pixel = DX_GetShaderIndex("SHADER/general/ps_bossbar_gradient");
    g_ShaderIndex_BossbarSubGradient_Pixel = DX_GetShaderIndex("SHADER/general/ps_bossbar_sub_gradient");
    g_ShaderIndex_UIBgGeneral_Pixel = DX_GetShaderIndex("SHADER/general/ps_ui_bg_general");
    g_ShaderIndex_FadeClock_Pixel = DX_GetShaderIndex("SHADER/general/ps_fade_clock");
    g_ShaderIndex_EventTimerEnterNumber_Pixel = DX_GetShaderIndex("SHADER/general/ps_cutin_name");
    

    g_number_template.value = 0;
    g_number_template.texNo = g_score_up_sprite;
    g_number_template.texCntX = 11;
    g_number_template.texCntY = 1;
    //g_number_template.texNo = LoadTexData("TEX/ui/number").textureId;
    //g_number_template.texCntX = 5;
    //g_number_template.texCntY = 5;
    
    g_number_template.size = MkF2(80.0f, 80.0f);
    g_number_template.digit = -1;
    g_number_template.gap = -25;
    g_number_template.plus_minus = false;
    g_number_template.color = MkF4(1.0f, 1.0f, 1.0f, 1.0f);
    g_number_template.align = SPRITE_NUMBER_DRAW_EX_ALIGN::Center;
}

void UninitUI_Game()
{
    UnloadTexData(g_score_eff);
    UnloadTexData(g_score_bg);
    UnloadTexData(g_tex_timer_on);
    UnloadTexData(g_tex_timer_off);
    UnloadTexData(g_score_up_sprite);
    UnloadTexData(g_score_up_sprite_w);
    
	UnloadTexData(g_tex_bossbar_main);
	UnloadTexData(g_tex_bossbar_sub);
	//UnloadTexData(g_tex_bossbar_bg);
    UnloadTexData(g_tex_bossbar_bg_Inperius);
    UnloadTexData(g_tex_bossbar_bg_Inperius_H);
    UnloadTexData(g_tex_bossbar_bg_Contrius);
    UnloadTexData(g_tex_bossbar_bg_Contrius_H);

    UnloadTexData(g_tex_bossbar_barrier);
    UnloadTexData(g_tex_bossbar_barrier_sprite);


	UnloadTexData(g_tex_Charactor_Z);

	UnloadTexData(g_tex_hpbar_main);
	UnloadTexData(g_tex_hpbar_sub);
	UnloadTexData(g_tex_hpbar_bg);
    UnloadTexData(g_tex_hpbarS_main);
    UnloadTexData(g_tex_hpbarS_sub);
    UnloadTexData(g_tex_hpbarS_bg);
    UnloadTexData(g_tex_ink_par);
    UnloadTexData(g_tex_ink_main);
    UnloadTexData(g_tex_ink_bg);
    UnloadTexData(g_tex_head_ink_bg);
    UnloadTexData(g_tex_head_ink_main);
    UnloadTexData(g_tex_head_ink_main2);
    UnloadTexData(g_tex_head_ink_cross);
    UnloadTexData(g_tex_head_ink_text_pinch);
    UnloadTexData(g_tex_head_ink_text_danger);
    

    UnloadTexData(g_tex_chara_icon_hp);
    UnloadTexData(g_tex_chara_icon_ink);

    for (int i = 0; i < CHARA_ICON_NUM; i++)
    {
        UnloadTexData(g_tex_chara_icon_num[i]);
        UnloadTexData(g_tex_chara_icon_num_tri[i]);
    }

    UnloadTexData(g_tex_btn_startskip);

    UnloadTexData(g_tex_Meter_bk_enter);
    UnloadTexData(g_tex_Meter_bk);
    UnloadTexData(g_tex_Meter_eff);
    UnloadTexData(g_tex_Meter_time_gray);
    UnloadTexData(g_tex_Meter_UI);
}

void UpdateUI_Game()
{

#if defined(_DEBUG) || defined(DEBUG)

    {
        static bool kp = false;
        if (Keyboard_IsKeyTrigger(KK_U))
        {
            if (!kp)
            {
                g_uiinfoView = !g_uiinfoView;
                kp = true;
            }
        }
        else
        {
            kp = false;
        }
    }

    if (!g_uiinfoView)
    {
        return;
    }

#endif


	if (UIDat.gameFrameEnterFrame > 0)
	{
		UIDat.gameFrameEnterFrame--;
	}

    if (GameManagerIsMovieFlag())
    {
        if (UIDat.movieFlagFrame < MOVIE_FLAG_FRAME_MAX)
        {
            UIDat.movieFlagFrame++;
        }
    }
    else
    {
        if (UIDat.movieFlagFrame > 0)
        {
            UIDat.movieFlagFrame--;
        }
    }

	//ボスのHP
	if (UIDat.bossbar.enable)
	{
		UI_BOSSBAR_DAT* bb = &UIDat.bossbar;
		if (bb->obj != nullptr)
		{
			bb->bar.pos = MkF2(BOSSBAR_OFFSET_X, BOSSBAR_OFFSET_Y);
			bb->bar.size = MkF2(BOSSBAR_SIZE_X, BOSSBAR_SIZE_Y);

            //double curMod = ((double)bb->obj->status.hp / bb->obj->status.hpMax);
            double curMod = (
                (double)(bb->obj->status.hp - (bb->obj->status.hpMax * bb->min))
                / (bb->obj->status.hpMax * (bb->max - bb->min))
            );
            curMod = std::max(std::min(curMod, 1.0), 0.0);

			int current = (int)(curMod * UI_RATE_MAX);

			barUpdate_Game(&bb->bar, current);

            for (int i = 0; i < BOSSBAR_BARRIER_NUM; i++)
            {
                if (bb->barrier[i].enable)
                {
                    if (bb->barrier[i].inF >= 0 
                        && bb->barrier[i].inF < BOSSBAR_BARRIER_IN_F)
                    {
                        bb->barrier[i].inF++;
                    }
                    if (bb->barrier[i].outF >= 0
                        && bb->barrier[i].outF < BOSSBAR_BARRIER_OUT_F)
                    {
                        bb->barrier[i].outF++;
                        if (bb->barrier[i].outF >= BOSSBAR_BARRIER_OUT_F)
                        {
                            bb->barrier[i].enable = false;
                        }
                    }
                    if (bb->barrier[i].breakF >= 0
                        && bb->barrier[i].breakF < BOSSBAR_BARRIER_BREAK_F)
                    {
                        bb->barrier[i].breakF++;
                        if (bb->barrier[i].breakF >= BOSSBAR_BARRIER_BREAK_F)
                        {
                            bb->barrier[i].enable = false;
                        }
                    }
                }
            }
		}
	}

	for (int i = 0; i < PLAYER_MAX; i++)
	{
		if (UIDat.hpbar[i].enable)
		{
			PLAYER* player = GetPlayer(i);

			if (player->obj != nullptr)
			{
                UIDat.hpbar[i].player = player;
                if (UIDat.hpbar[i].player->obj == nullptr)
                {
                    continue;
                }
                UIDat.hpbar[i].playerGOE = ((GOE_Character_Player*)player->obj->events.source);
                if (UIDat.hpbar[i].playerGOE == nullptr)
                {
                    continue;
                }

				//プレイヤーのHP
				{
					UI_HPBAR_DAT* hb = UIDat.hpbar + i;

					hb->bar.pos = MkF2(HPBAR_OFFSET_X, HPBAR_OFFSET_Y);
					hb->bar.size = MkF2(HPBAR_SIZE_X * HPBAR_SCALE, HPBAR_SIZE_Y * HPBAR_SCALE);

					int current = (int)(((double)player->obj->status.hp / player->obj->status.hpMax) * UI_RATE_MAX);

					barUpdate_Game(&hb->bar, current);
					//DebugPrintf("%d   %d   %d\n", hb->barMain, hb->barSub, hb->barBuf);
				}

				//プレイヤーのインク
				{
					UI_INK_BAR_DAT* ib = UIDat.inkbar + i;

					ib->bar.pos = MkF2(INKBAR_OFFSET_X, INKBAR_OFFSET_Y);
					ib->bar.size = MkF2(INKBAR_SIZE_X * INKBAR_SCALE, INKBAR_SIZE_Y * INKBAR_SCALE);

                    //Float2 ink = GOE_Character_Player::GetKakoponVertexNum();
                    Int2 ink = UIDat.hpbar[i].playerGOE->GetKakoponInkGage();
					int current = (int)((/*1.0 - */(double)ink.x / std::max(ink.y, 1)) * UI_RATE_MAX);

					barUpdate_Game(&ib->bar, current);
                    if (ib->bar.barMain > ib->bar.barSub)
                    {
                        ib->bar.barSub = ib->bar.barMain;
                    }
					//DebugPrintf("%d   %d   %d\n", hb->barMain, hb->barSub, hb->barBuf);
				}
			}
		}
	}


    //スコア
    //if (UIDat.score.enable)
    {
        GAME_MANAGER_SAVE_RESULT gmsr = GameManagerGetResult();
        int addscore = GameManagerGetResultAddScoreBuf();

        UIDat.score.score = gmsr.score;

        if (addscore != 0)
        {
            int index = 0;
            for (int i = 0; i < SCORE_ADD_DAT_MAX; i++)
            {
                if (!UIDat.score.scoreAddDat[i].enable)
                {
                    UIDat.score.scoreAddDat[i].enable = true;
                    UIDat.score.scoreAddDat[i].score = addscore;
                    //UIDat.score.scoreAddDat[i].frameMax = 60;
                    UIDat.score.scoreAddDat[i].frameMax = 160;
                    UIDat.score.scoreAddDat[i].frame = UIDat.score.scoreAddDat[i].frameMax;
                    UIDat.score.scoreAddDat[i].index = index;

                    UIDat.score.addFrameMax = 25;
                    //UIDat.score.addFrameMax = 40;
                    UIDat.score.addFrame = UIDat.score.addFrameMax;

                    break;
                }
                else
                {
                    index++;
                }
            }
        }

        for (int i = 0; i < SCORE_ADD_DAT_MAX; i++)
        {
            if (UIDat.score.scoreAddDat[i].enable)
            {
                if (UIDat.score.scoreAddDat[i].frame <= 0)
                {
                    UIDat.score.scoreAddDat[i].enable = false;
                }
                else
                {

                    UIDat.score.scoreAddDat[i].frame--;
                }
            }
        }

        if (UIDat.score.addFrame > 0)
        {
            UIDat.score.addFrame--;
        }

        GameManagerResetResultAddScoreBuf();
    }


    if (UIDat.eventTimer.enable)
    {
        int value = *UIDat.eventTimer.frame;
        if (UIDat.eventTimer.rev)
        {
            value = UIDat.eventTimer.frameMax - value;
        }
        if (
            (value == UIDat.eventTimer.frameMax)
            || (value % (FRAME_ONE_SECOND * 10) == 0)
            || (value <= (FRAME_ONE_SECOND * 5)
                && value % (FRAME_ONE_SECOND) == 0)
        )
        {
            UIDat.eventTimer.countEffectFrame = 0;
        }

        UIDat.eventTimer.enterFrame++;
        UIDat.eventTimer.countEffectFrame++;
    }
    else
    {
        if (UIDat.eventTimer.enterFrame > 0)
        {
            UIDat.eventTimer.enterFrame = 0;
        }
    }
}

void DrawUI_Game()
{

#if defined(_DEBUG) || defined(DEBUG)
    if (!g_uiinfoView)
    {
        return;
    }
#endif

	unsigned int gframe = GetMainGameFrame();
	Float4 colorDefaultTmp = MkF4(1.0f, 1.0f, 1.0f, 1.0f);

    g_movieFlagAlpha = 1.0f;
    {
        float smod = std::max(
            std::min(
                (float)UIDat.movieFlagFrame / MOVIE_FLAG_FRAME_MAX, 
                1.0f),
            0.0f);
        g_movieFlagAlpha = 1.0f - smod;

        if (smod > 0)
        {
            for (int i = 0; i < 2; i++)
            {
                float revmod = i == 1 ? -1.0f : 1.0f;
                float sizex = SCREEN_HEIGHT / 10;
                {
                    DrawSpriteInCamera(
                        //表示座標 (四角形の真ん中)
                        SCREEN_WIDTH * (1.0f - smod) * revmod,
                        (SCREEN_HEIGHT / 2 - sizex / 2) * revmod,

                        //サイズ width hight
                        SCREEN_WIDTH,
                        sizex,

                        //角度
                        0.0f,

                        //色
                        MkF4(0.0f, 0.0f, 0.0f, 0.5f),

                        0,  //画像(テクスチャ)の番号
                        MkF2(0.0f, 0.0f),
                        MkF2(1.0f, 1.0f),
                        DRAW_POLYGON_PRIORITY_FORCE_DEEPEST
                    );
                }
            }

            {
                float startmodtmp = 
                    (float)GameManagerGetMovieSkipFlagStartFrame() 
                    / MOVIE_SKIP_FLAG_START_FRAME;
                float pushmodtmp =
                    (float)GameManagerGetMovieSkipFlagPushFrame()
                    / MOVIE_SKIP_FLAG_PUSH_FRAME;

                int skipfragf = GameManagerGetMovieSkipFlagFrame();

                Float2 postmp = {
                        SCREEN_WIDTH / 2 - 210.0f,
                        SCREEN_HEIGHT / 2 - 50.0f,
                };
                Float2 sizetmp = {
                        400.0f,
                        400.0f,
                };

                if (skipfragf > 0)
                {
                    float skipfragfAlphaTmp = std::min(
                        (float)(skipfragf)
                        / MOVIE_SKIP_FLAG_START_FRAME,
                        1.0f);
                    DrawSpriteInCamera(
                        //表示座標 (四角形の真ん中)
                        postmp.x,
                        postmp.y,

                        //サイズ width hight
                        sizetmp.x,
                        sizetmp.y,

                        //角度
                        0.0f,

                        //色
                        MkF4(1.0f, 1.0f, 1.0f,
                            smod * (1.0f - startmodtmp) 
                            * skipfragfAlphaTmp
                        ),

                        g_tex_btn_startskip,  //画像(テクスチャ)の番号
                        MkF2(0.0f, 0.0f),
                        MkF2(1.0f, 1.0f),
                        //DRAW_POLYGON_PRIORITY_FORCE_DEEPEST,
                        DRAW_POLYGON_PRIORITY_FORCE_ALWAYS
                    );
                }

                if (pushmodtmp > 0)
                {
                    float smtmp = EasingInOutBack(1.0f - pushmodtmp);
                    DrawSpriteInCamera(
                        //表示座標 (四角形の真ん中)
                        postmp.x,
                        postmp.y,

                        //サイズ width hight
                        sizetmp.x * (1.0f + 0.3f * smtmp),
                        sizetmp.y * (1.0f + 0.3f * smtmp),

                        //角度
                        0.0f,

                        //色
                        MkF4(1.0f, 1.0f, 1.0f, pushmodtmp),

                        g_tex_btn_startskip,  //画像(テクスチャ)の番号
                        MkF2(0.0f, 0.0f),
                        MkF2(1.0f, 1.0f),
                        //DRAW_POLYGON_PRIORITY_FORCE_DEEPEST
                        DRAW_POLYGON_PRIORITY_FORCE_ALWAYS
                    );
                }
            }
        }
    }
    float movieFlagAlpha = g_movieFlagAlpha;

	//ボスのHP
	if (UIDat.bossbar.enable)
	{
		UI_BOSSBAR_DAT* bb = &UIDat.bossbar;
		if (bb->obj != nullptr)
		{
			Float2 pos = bb->bar.pos;
			Float2 size = bb->bar.size;
            bool isRev = (bb->bar.barMain > bb->bar.barSub);
			//bool isRev = (bb->bar.barBuf > bb->bar.barSub);
			float barmod = 1.0f;
			float barentermod = 1.0f;
			if (bb->bar.barEnterFrame >= 0)
			{
				float mtmp = (float)bb->bar.barEnterFrame / HPBAR_ENTER_FRAME_MAX;
				if (mtmp > 1.0f)
				{
					mtmp = 1.0f;
				}
				barentermod = EasingOutBack(mtmp);
			}

			double shakemodtmp = (double)bb->bar.barChangeFrame / HPBAR_CHANGE_FRAME_MAX;
			double shakextmp = sin(HPBAR_SHAKE_POWER_X * bb->bar.barShakeFrame) * shakemodtmp * BOSSBAR_SHAKE_X;
			double shakeytmp = -cos(HPBAR_SHAKE_POWER_Y * bb->bar.barShakeFrame) * shakemodtmp * BOSSBAR_SHAKE_Y;

			Float2 pos2 = MkF2(pos.x + (float)shakextmp, pos.y + (float)shakeytmp);

			for (int i = 0; i < 3; i++)
			{
				bool hidden = false;
				unsigned int tex = 0;
				float txpadx = 0.0005f;
				Float4 colorTmp = MkF4(1.0f, 1.0f, 1.0f, 1.0f);
				Float2 posTmp = pos;
				Float2 sizeTmp = size;
                DPD_SHADER_TYPE_EX_OPT opt = {};
                opt.vs = g_ShaderIndex_UIBgGeneral_Vertex;
                opt.ps = g_ShaderIndex_GageGeneral_Pixel;

				colorTmp.w = HPBAR_PLAYER_OVERLAP_ALPHA
					+ (1.0f - HPBAR_PLAYER_OVERLAP_ALPHA)
					* (1.0f - (float)bb->bar.playerOverlapRate / UI_RATE_MAX);

				switch (i)
				{
				case 0:
                    if (bb->type == BOSSBAR_TYPE_GAME::Inperius)
                    {
                        tex = g_tex_bossbar_bg_Inperius;
                        sizeTmp = MkF2(BOSSBARBG_SIZE_X, BOSSBARBG_SIZE_Y);
                    }
                    else
                    {
                        if (bb->type == BOSSBAR_TYPE_GAME::Inperius_H)
                        {
                            tex = g_tex_bossbar_bg_Inperius_H;
                        }
                        if (bb->type == BOSSBAR_TYPE_GAME::Contrius)
                        {
                            tex = g_tex_bossbar_bg_Contrius;
                        }
                        if (bb->type == BOSSBAR_TYPE_GAME::Contrius_H)
                        {
                            tex = g_tex_bossbar_bg_Contrius_H;
                        }
                        sizeTmp = MkF2(
                            BOSSBARBG_SIZE_X,
                            BOSSBARBG_SIZE_Y * (99.0f / 1177.0f)
                        );
                    }

					colorTmp.w = HPBARBG_PLAYER_OVERLAP_ALPHA
						+ (1.0f - HPBARBG_PLAYER_OVERLAP_ALPHA)
						* (1.0f - (float)bb->bar.playerOverlapRate / UI_RATE_MAX);

                    opt.vs = g_ShaderIndex_UIBgGeneral_Vertex;
                    opt.ps = g_ShaderIndex_UIBgGeneral_Pixel;
					break;
				case 1:
					barmod = (float)(isRev ? bb->bar.barMain : bb->bar.barSub) / UI_RATE_MAX;
					tex = g_tex_bossbar_sub;
                    posTmp.x += BOSSBAR_PAD_X + 1.0f;
                    posTmp.y += BOSSBAR_PAD_Y + 1.5f;

                    //シェーダーの設定
                    opt.vs = g_ShaderIndex_GageGradient_Vertex;
                    opt.ps = g_ShaderIndex_BossbarSubGradient_Pixel;
					break;
				case 2:
					tex = g_tex_bossbar_main;
					barmod = (float)(isRev ? bb->bar.barSub : bb->bar.barMain) / UI_RATE_MAX;
					if (bb->bar.barChangeFrame > (HPBAR_CHANGE_FRAME_MAX - HPBAR_CHANGE_FLASH_FRAME)
						&& bb->bar.barChangeFrame <= HPBAR_CHANGE_FRAME_MAX
						&& bb->bar.barChangeFrame % HPBAR_CHANGE_FLASH_CYCLE_FRAME < HPBAR_CHANGE_FLASH_CYCLE_FRAME / 2)
					{
						colorTmp = MkF4(100.0f, 100.0f, 100.0f, 1.0f);
					}
                    posTmp.x += BOSSBAR_PAD_X + 1.0f;
                    posTmp.y += BOSSBAR_PAD_Y + 1.5f;

                    //シェーダーの設定
                    opt.vs = g_ShaderIndex_GageGradient_Vertex;
                    opt.ps = g_ShaderIndex_BossbarGradient_Pixel;
					break;
				default:
					break;
				}

				if (hidden)
				{
					continue;
				}

                barmod = std::max(barmod, barmod <= 0.0f ? 0.0f: 0.01f);

                DrawSpriteInCamera(
					//表示座標 (四角形の真ん中)
					posTmp.x - sizeTmp.x / 2 * (1.0f - barmod) + (float)shakextmp,
					posTmp.y + (float)shakeytmp,

					//サイズ width hight
					sizeTmp.x * barmod * (bb->bar.beforeEnable ? 1.0f : barentermod),
					sizeTmp.y,

					//角度
					0.0f,

					//色
					colorTmp,

					tex,  //画像(テクスチャ)の番号
					MkF2(0.0f + txpadx, 0.0f),
					MkF2(barmod * (1.0f - txpadx), 1.0f),
                    DRAW_POLYGON_PRIORITY_FORCE_NORMAL,
                    DPD_SHADER_TYPE_EX,
                    opt
				);
			}

            for (int i = 0; i < BOSSBAR_BARRIER_NUM; i++)
            {
                if (bb->barrier[i].enable)
                {
                    Float4 colorTmp = MkF4(1.0f, 1.0f, 1.0f, 1.0f);
                    unsigned int tex = g_tex_bossbar_barrier;
                    Float2 uv = MkF2(0.0f, 0.0f);
                    Float2 wh = MkF2(1.0f, 1.0f);
                    Float2 sca = MkF2(1.0f, 1.0f);
                    Float2 siz = MkF2(
                        742.0f * 0.88f,
                        408.0f * 0.88f
                    );

                    if (bb->barrier[i].inF >= 0)
                    {
                        float m = ((float)bb->barrier[i].inF / BOSSBAR_BARRIER_IN_F);

                        //sca.x *= 1.0f + (1.5f * EasingInOutBack(1.0f - m));
                        //sca.y *= 1.0f + (1.5f * EasingInOutBack(1.0f - m));
                        sca.x *= 1.0f + (2.5f * EasingInOutBack(1.0f - m));
                        sca.y *= 1.0f + (2.5f * EasingInOutBack(1.0f - m));
                        colorTmp.w *= m;
                    }
                    if (bb->barrier[i].outF >= 0)
                    {
                        float m = ((float)bb->barrier[i].outF / BOSSBAR_BARRIER_OUT_F);

                        sca.x *= 1.0f + EasingInOutSine(0.5f * m);
                        sca.y *= 1.0f + EasingInOutSine(0.5f * m);
                        colorTmp.w *= (1.0f - m);
                    }
                    if (bb->barrier[i].breakF >= 0)
                    {
                        float m = ((float)bb->barrier[i].breakF / BOSSBAR_BARRIER_BREAK_F);
                        tex = g_tex_bossbar_barrier_sprite;
                        uv = AnimationUV(
                            8 * m,
                            4,
                            2
                        );
                        wh = MkF2(1.0f / 4, 1.0f / 2);
                        sca.x *= 1.0f + EasingInOutSine(0.5f * m);
                        sca.y *= 1.0f + EasingInOutSine(0.5f * m);
                    }

                    DrawSpriteInCamera(
                        //表示座標 (四角形の真ん中)
                        -52.0f + pos2.x + siz.x / 4 + size.x / -2 + (size.x / BOSSBAR_BARRIER_NUM) * i,
                        +20.0f + pos2.y,

                        //サイズ width hight
                        siz.x * sca.x,
                        siz.y * sca.y,

                        //角度
                        0.0f,

                        //色
                        colorTmp,

                        tex,  //画像(テクスチャ)の番号
                        uv,
                        wh
                    );
                }
            }
		}
	}

    //if (UIDat.movieFlagFrame > 0)
    //{
    //    return;
    //}

    
    Float4 colorAlphaSetTmp = MkF4(1.0f, 1.0f, 1.0f, movieFlagAlpha);

	//プレイヤーのhud
	for (int i = 0; i < PLAYER_MAX; i++)
	{
		if (UIDat.hpbar[i].enable)
		{
            {
                int index = 0;
                for (int k = 0; k < PLAYER_MAX; k++)
                {
                    if (i != k && UIDat.hpbar[k].enable)
                    {
                        update_player_gui(gframe, k, i, colorAlphaSetTmp, movieFlagAlpha,
                            true, index
                        );
                        index++;
                    }
                }
            }
            update_player_gui(gframe, i, i, colorAlphaSetTmp, movieFlagAlpha,
                false, 0
            );
		}
	}


	//タイマー
    if(UIDat.timerVisible)
	{
        Int2 timerValue = GetStageCountFrameSceneGame();
        {
            DPD_SHADER_TYPE_EX_OPT opt = {};
            opt.vs = g_ShaderIndex_UIBgGeneral_Vertex;
            opt.ps = g_ShaderIndex_UIBgGeneral_Pixel;
            DrawSpriteInCamera(
                //表示座標 (四角形の真ん中)
                -SCREEN_WIDTH / 2 + 150.0f,
                -SCREEN_HEIGHT / 2 + 110.0f,

                //サイズ width hight
                200.0f,
                200.0f * (137.0f / 199.0f),

                //角度
                0.0f,

                //色
                colorAlphaSetTmp,

                g_tex_timer_on,  //画像(テクスチャ)の番号
                MkF2(0.0f, 0.0f),
                MkF2(1.0f, 1.0f),
                DRAW_POLYGON_PRIORITY_FORCE_NORMAL,
                DPD_SHADER_TYPE_EX,
                opt
            );
        }
        {
            DPD_SHADER_TYPE_EX_OPT opt = {};
            opt.vs = g_ShaderIndex_UIBgGeneral_Vertex;
            opt.ps = g_ShaderIndex_FadeClock_Pixel;
            opt.free.x = (float)timerValue.x / timerValue.y;
            
            DrawSpriteInCamera(
                //表示座標 (四角形の真ん中)
                -SCREEN_WIDTH / 2 + 150.0f,
                -SCREEN_HEIGHT / 2 + 110.0f,

                //サイズ width hight
                200.0f,
                200.0f * (137.0f / 199.0f),

                //角度
                0.0f,

                //色
                colorAlphaSetTmp,

                g_tex_timer_off,  //画像(テクスチャ)の番号
                MkF2(0.0f, 0.0f),
                MkF2(1.0f, 1.0f),
                DRAW_POLYGON_PRIORITY_FORCE_NORMAL,
                DPD_SHADER_TYPE_EX,
                opt
            );
        }


        SPRITE_NUMBER_DRAW_EX sndex = g_number_template;
        sndex.texNo = g_score_up_sprite_w;
        sndex.texCntX = 10;
        sndex.value = (int)timerValue.x / 60;
        if (sndex.value <= 0)
        {
            sndex.digit = 1;
        }
        //sndex.value = 88;
        sndex.color = MkF4(1.0f, 1.0f, 1.0f, movieFlagAlpha);
        sndex.pos = MkF2(
            -SCREEN_WIDTH / 2 + 150.0f,
            -SCREEN_HEIGHT / 2 + 110.0f
        );
        //sndex.size = MkF2(
        //    65.0f,
        //    65.0f
        //);
        //sndex.gap += 5.0f;
        sndex.size = MkF2(
            56.0f,
            56.0f
        );
        sndex.gap += 4.0f;

		SpriteNumberDrawEx(sndex);
	}

    //スコア
    //if (UIDat.score.enable)
    if (UIDat.scoreVisible)
    {
        float addsm = 0.0f;
        if (UIDat.score.addFrame > 0 && UIDat.score.addFrameMax > 0)
        {
            addsm = (float)UIDat.score.addFrame / UIDat.score.addFrameMax;
        }

        float scorebaseScale = 1.0f + (-0.25 * (1.0f - addsm));
        Float2 scorebasePos = {};
        scorebasePos.x += 26.0f * (1.0f - addsm);
        scorebasePos.y += -12.0f * (1.0f - addsm);

        float addsAlpha = EasingOutBack(addsm);
        //float addsaddy = (SCORE_SIZE_Y
        //    + (float)sin((float)GetMainGameFrame() * 0.7f) * SCORE_SIZE_Y * -0.08f)
        //    * EasingOutBack(addsm);
        //float addsadds = (0.0f
        //    + (float)sin((float)GetMainGameFrame() * 0.9f) * SCORE_SIZE_Y * 0.28f)
        //    * EasingOutBack(addsm);
        float addsadds = (0.0f
            + (float)sin((float)GetMainGameFrame() * 0.4f) * SCORE_SIZE_Y * 0.1f)
            * EasingOutBack(addsm);

        {
            DPD_SHADER_TYPE_EX_OPT opt = {};
            opt.vs = g_ShaderIndex_UIBgGeneral_Vertex;
            opt.ps = g_ShaderIndex_UIBgGeneral_Pixel;
            DrawSpriteInCamera(
                //表示座標 (四角形の真ん中)
                SCORE_OFFSET_X + scorebasePos.x,
                SCORE_OFFSET_Y + scorebasePos.y,

                //サイズ width hight
                SCORE_SIZE_X * 8 * scorebaseScale,
                SCORE_SIZE_Y * 8 * scorebaseScale * (79.0f / 305.0f),

                //角度
                0.0f,

                //色
                MkF4(1.0f, 1.0f, 1.0f, 1.0f * movieFlagAlpha),

                g_score_bg,  //画像(テクスチャ)の番号
                MkF2(0.0f, 0.0f),
                MkF2(1.0f, 1.0f),
                DRAW_POLYGON_PRIORITY_FORCE_NORMAL,
                DPD_SHADER_TYPE_EX,
                opt
            );
        }

        SPRITE_NUMBER_DRAW_EX sndex = g_number_template;
        sndex.value = UIDat.score.score;
        sndex.color = MkF4(1.0f, 1.0f, 1.0f, movieFlagAlpha);
        sndex.pos = MkF2(
            SCORE_OFFSET_X + scorebasePos.x,
            SCORE_OFFSET_Y + scorebasePos.y + 15.0f * scorebaseScale
        );
        sndex.size = MkF2(
            (SCORE_SIZE_X + addsadds) * scorebaseScale, 
            (SCORE_SIZE_Y + addsadds) * scorebaseScale
        );
        sndex.digit = 5;
        sndex.gap = 0;

        {
            SPRITE_NUMBER_DRAW_EX sndex2 = sndex;
            sndex2.texNo = g_score_up_sprite_w;
            sndex2.texCntX = 10;
            SpriteNumberDrawEx(sndex2);
        }

        {
            int index = 0;
            for (int i = 0; i < SCORE_ADD_DAT_MAX; i++)
            {
                if (UIDat.score.scoreAddDat[i].enable)
                {
                    float m = (float)UIDat.score.scoreAddDat[i].frame / UIDat.score.scoreAddDat[i].frameMax;

                    int value = UIDat.score.scoreAddDat[i].score;

                    //float a = EasingInOutSine(m) * movieFlagAlpha;
                    float a = EasingInOutSine(m);

                    Float4 color = MkF4(1.0f, 1.0f, 1.0f, a);
                    //Float4 color = MkF4(2.0f, 2.0f, -0.2f, a);
                    //if (value < 0)
                    //{
                    //    color = MkF4(-1.0f, 1.0f, 2.0f, a);
                    //}

                    for (int j = 0; j < 1; j++)
                    {
                        SPRITE_NUMBER_DRAW_EX sndex2 = sndex;

                        Float2 adp = MkF2(0.0f, 0.0f);
                        //Float4 ct = color;
                        //if (j == 0)
                        //{
                        //    adp = MkF2(SCORE_SIZE_X * 0.08f, SCORE_SIZE_Y * 0.08f);
                        //    ct = MkF4(-1.0f, -1.0f, -1.0f, a * 0.75f);
                        //}

                        sndex2.color = color;

                        sndex2.value = value;
                        sndex2.pos = MkF2(
                            adp.x + SCORE_OFFSET_X + SCORE_SIZE_X * 2.5f,
                            adp.y + SCORE_OFFSET_Y + SCORE_SIZE_Y * 1.2f
                            + SCORE_SIZE_Y * 0.5f * UIDat.score.scoreAddDat[i].index
                            + (SCORE_SIZE_Y * 0.6f
                                - SCORE_SIZE_Y * 0.6f * EasingOutElastic(1.0f - m))
                        );
                        sndex2.size = MkF2(
                            SCORE_SIZE_X * 1.1f, 
                            SCORE_SIZE_Y * 1.1f
                        );
                        sndex2.plus_minus = true;
                        sndex2.gap = -8.0f;
                        sndex2.digit = -1;

                        SpriteNumberDrawEx(sndex2);

                        {
                            //float m2 = std::min((1.0f - m) / , 1.0f);
                            float m2 = std::max(std::min(
                                (float)(UIDat.score.scoreAddDat[i].frameMax - UIDat.score.scoreAddDat[i].frame)
                                / 60,
                                1.0f), 0.0f);
                            {
                                DRAW_POLYGON_DAT dsdat = {};
                                dsdat.pos.x = sndex2.pos.x;
                                dsdat.pos.y = sndex2.pos.y;
                                dsdat.pos.z = 0.0f;

                                dsdat.size.x = sndex2.size.x * 10.0f;
                                dsdat.size.y = sndex2.size.y * 10.0f;
                                dsdat.size.z = 1.0f;

                                dsdat.rot = 0.0f;

                                dsdat.color = color;
                                dsdat.texNo = g_score_eff;

                                dsdat.loadTexType = LOADTEXTURETYPE_MAIN;

                                dsdat.texUV = AnimationUV(
                                    19 * m2,
                                    5,
                                    4
                                );

                                dsdat.texWH = MkF2(1.0f / 5, 1.0f / 4);

                                dsdat.startRot = 0.0f;
                                dsdat.startScale = MkF2(1.0f, 1.0f);
                                dsdat.startScale3D = MkF3(1.0f, 1.0f, 1.0f);

                                dsdat.modelNo = 0;

                                dsdat.shaderOpt.blendState = DPD_SHADER_OPT_BLEND_STATE_ADDCOLOR;

                                DRAW_POLYGON_IN_CAMERA_EX_OPTION opt = {};
                                opt.reverse = false;
                                opt.fixed = true;
                                opt.billboard = false;
                                opt.priority = DRAW_POLYGON_PRIORITY_FORCE_NORMAL;
                                opt.depthBuffIgnore = true;

                                DrawPolygonInCameraEx(dsdat, opt);
                            }


                        }

                    }

                    index++;
                }
            }
        }
    }

    //イベントタイマー
    if (UIDat.eventTimer.enable)
    {
        //g_tex_Meter_bk
        //g_tex_Meter_eff
        //g_tex_Meter_time_01
        //g_tex_Meter_time_02
        //g_tex_Meter_UI
        int value = *UIDat.eventTimer.frame;
        if (UIDat.eventTimer.rev)
        {
            value = UIDat.eventTimer.frameMax - value;
        }

        float enterMod1 = std::min(
            (float)UIDat.eventTimer.enterFrame / 10,
            1.0f);
        float enterMod2 = std::max(std::min(
            (float)(UIDat.eventTimer.enterFrame - 50) / 60,
            1.0f), 0.0f);

        float enterMod2Ease1 = std::max(EasingInOutBack(enterMod2), 0.0f);
        float enterMod2Ease2 = std::max(EasingInOutBack(1.0f - enterMod2), 0.0f);
        float enterMod2PosY = 200.0f * enterMod2Ease2;
        //float enterMod2Scale = 1.0f + 2.0f * enterMod2Ease2;
        float enterMod2Scale = 1.0f + 1.75f * enterMod2Ease2;

        float countEffectMod1 = std::min((float)UIDat.eventTimer.countEffectFrame / 20, 1.0f);
        float countEffectMod2 = std::min((float)UIDat.eventTimer.countEffectFrame / 40, 1.0f);

        float valuerate = (float)value / UIDat.eventTimer.frameMax;
        //bool isPinch = (double)(1.0 - 28.0 / 39.0f) >= (double)valuerate;
        bool isPinch = value <= (FRAME_ONE_SECOND * 10);
        bool isDanger = value <= (FRAME_ONE_SECOND * 5);

        Float4 colortmp = {};
        if (isDanger)
        {
            colortmp = MkF4(2.0f, 0.25f, 0.25f, 1.0f);
        }
        else if (isPinch)
        {
            colortmp = MkF4(2.0f, 2.0f, 0.25f, 1.0f);
        }
        else
        {
            colortmp = MkF4(1.8f, 1.8f, 1.8f, 1.0f);
        }
        {
            //Float4 color2tmp = MkF4(2.0f, 0.2f, 2.0f, 1.0f);
            Float4 color2tmp = MkF4(2.0f, 0.25f, 1.5f, 1.0f);
            colortmp.x += (color2tmp.x - colortmp.x) * enterMod2Ease2;
            colortmp.y += (color2tmp.y - colortmp.y) * enterMod2Ease2;
            colortmp.z += (color2tmp.z - colortmp.z) * enterMod2Ease2;
        }

        if (isPinch || isDanger)
        {
            DRAW_POLYGON_DAT dsdat = {};
            dsdat.pos.x = 0.0f;
            dsdat.pos.y = 0.0f;
            dsdat.pos.z = 0.0f;

            dsdat.size.x = SCREEN_WIDTH;
            dsdat.size.y = SCREEN_HEIGHT;
            dsdat.size.z = 1.0f;

            dsdat.rot = 0.0f;

            dsdat.color = colortmp;

            dsdat.texNo = g_tex_Meter_eff;
            dsdat.loadTexType = LOADTEXTURETYPE_MAIN;

            dsdat.texUV = {
                0.0f,
                0.0f
            };
            dsdat.texWH = { 1.0f, 1.0f };

            dsdat.startPos = {
                0.0f,
                0.0f
            };
            dsdat.startRot = 0;
            dsdat.startScale = { 1.0f, 1.0f };
            dsdat.startScale3D = { 1.0f, 1.0f, 1.0f };

            dsdat.shaderOpt.bloom = DPD_SHADER_OPT_BLOOM_NONE;
            //dsdat.shaderOpt.blendState = DPD_SHADER_OPT_BLEND_STATE_ADDCOLOR;

            DRAW_POLYGON_IN_CAMERA_EX_OPTION opt = {};
            opt.reverse = false;
            opt.fixed = true;
            opt.depthBuffIgnore = true;
            opt.priority = DRAW_POLYGON_PRIORITY_NML_ALWAYS;

            DrawPolygonInCameraEx(dsdat, opt);
        }

        Float2 pos = MkF2(
            0.0f,
            BOSSBAR_OFFSET_Y + 90.0f + enterMod2PosY
        );

        {
            float alphatmp = 1.0f * enterMod1
                //* (1.0f - 0.6f * (1.0f - enterMod2))
                * enterMod2Ease1;
            if (alphatmp > 0)
            {
                {
                    DPD_SHADER_TYPE_EX_OPT opt = {};
                    opt.vs = g_ShaderIndex_UIBgGeneral_Vertex;
                    opt.ps = g_ShaderIndex_UIBgGeneral_Pixel;
                    DrawSpriteInCamera(
                        //表示座標 (四角形の真ん中)
                        pos.x,
                        pos.y,

                        //サイズ width hight
                        570.0f * 1.16f * enterMod2Scale,
                        130.0f * 1.16f * enterMod2Scale,

                        //角度
                        0.0f,

                        //色
                        //MkF4(1.0f, 1.0f, 1.0f, 1.0f * movieFlagAlpha),
                        MkF4(0.2f, 0.2f, 0.2f,
                            //std::min(std::max(alphatmp, 0.0f), 1.0f) * 0.8f
                            std::min(std::max(alphatmp, 0.0f), 1.0f) * 0.75f
                        ),

                        g_tex_Meter_bk,  //画像(テクスチャ)の番号
                        MkF2(0.0f, 0.0f),
                        MkF2(1.0f, 1.0f),
                        DRAW_POLYGON_PRIORITY_FORCE_NORMAL
                        //,
                        //DPD_SHADER_TYPE_EX,
                        //opt
                    );
                }
                if (countEffectMod2 < 1.0f)
                {
                    DrawSpriteInCamera(
                        //表示座標 (四角形の真ん中)
                        pos.x,
                        pos.y,

                        //サイズ width hight
                        570.0f * 1.16f * enterMod2Scale * (1.0f + 0.6f * countEffectMod2),
                        130.0f * 1.16f * enterMod2Scale * (1.0f + 0.6f * countEffectMod2),

                        //角度
                        0.0f,

                        //色
                        //MkF4(1.0f, 1.0f, 1.0f, 1.0f * movieFlagAlpha),
                        MkF4(2.5f, 2.0f, 2.5f,
                            std::min(std::max(alphatmp, 0.0f), 1.0f)* (1.0f - countEffectMod2)
                        ),

                        g_tex_Meter_bk,  //画像(テクスチャ)の番号
                        MkF2(0.0f, 0.0f),
                        MkF2(1.0f, 1.0f)
                    );
                }
            }
        }

        {
            float alphatmp =
                1.0f * enterMod1
                * enterMod2Ease2;
            if (alphatmp > 0)
            {
                DPD_SHADER_TYPE_EX_OPT opt = {};
                opt.vs = g_ShaderIndex_UIBgGeneral_Vertex;
                opt.ps = g_ShaderIndex_UIBgGeneral_Pixel;
                DrawSpriteInCamera(
                    //表示座標 (四角形の真ん中)
                    pos.x,
                    pos.y,

                    //サイズ width hight
                    //224.0f * 2.5f * enterMod2Scale,
                    //+77.0f * 2.5f * enterMod2Scale,
                    216.0f * 1.4f * enterMod2Scale,
                    154.0f * 1.4f * enterMod2Scale,

                    //角度
                    0.0f,

                    //色
                    //MkF4(1.0f, 1.0f, 1.0f, 1.0f * movieFlagAlpha),
                    MkF4(1.0f, 1.0f, 1.0f,
                        //std::min(std::max(alphatmp, 0.0f), 1.0f) * 0.75f
                        std::min(std::max(alphatmp, 0.0f), 1.0f) * 0.35f
                    ),
                    //MkF4(0.0f, 0.0f, 0.0f,
                    //    alphatmp
                    //),

                    g_tex_Meter_bk_enter,  //画像(テクスチャ)の番号
                    MkF2(0.0f, 0.0f),
                    MkF2(1.0f, 1.0f),
                    DRAW_POLYGON_PRIORITY_FORCE_NORMAL
                    //,
                    //DPD_SHADER_TYPE_EX,
                    //opt
                );
            }
        }
        //{
        //    SPRITE_NUMBER_DRAW_EX sndex = g_number_template;
        //    sndex.texNo = g_tex_Meter_time_01;
        //    sndex.texCntX = 11;
        //    sndex.value = (int)GetStageCountFrameSceneGame();
        //    sndex.digit = 3;
        //    sndex.color = MkF4(1.0f, 1.0f, 1.0f, movieFlagAlpha);
        //    sndex.pos = MkF2(
        //        pos.x + 160.0f,
        //        pos.y
        //    );
        //    sndex.size = MkF2(
        //        60.0f,
        //        60.0f
        //    );
        //    sndex.gap += -5.0f;

        //    SpriteNumberTimerDraw(sndex);
        //}

        {
            float numsca = 1.0f;
            if (countEffectMod2 < 1.0f)
            {
                numsca *= (1.0f + 0.9f * EasingInBack(1.0f - countEffectMod2));
            }

            SPRITE_NUMBER_DRAW_EX sndex = g_number_template;
            //sndex.texNo = isPinch ? g_tex_Meter_time_02 : g_tex_Meter_time_01;
            sndex.texNo = g_tex_Meter_time_gray;
            sndex.texCntX = 11;
            sndex.value = value;
            sndex.digit = UIDat.eventTimer.digit;
            //sndex.color = MkF4(1.0f, 1.0f, 1.0f, movieFlagAlpha);
            sndex.color = colortmp;
            sndex.color.w *= enterMod1;
            
            sndex.size = MkF2(
                60.0f * enterMod2Scale * numsca,
                60.0f * enterMod2Scale * numsca
            );
            sndex.pos = MkF2(
                pos.x
                + ((sndex.size.x - 0.0f) * (sndex.digit * 2 + (sndex.digit - 1))) / 4 
                + 80.0f * enterMod2Ease1
                //+ 160.0f 
                //+ (-(sndex.size.x - 60.0f) * (sndex.digit * 2 + (sndex.digit - 1))) / 4 
                //* (1.0f - enterMod2)
                ,
                pos.y
            );
            sndex.gap += -6.0f;
            sndex.gap *= enterMod2Scale * numsca;

            SpriteNumberTimerDraw(sndex);
            {
                float shadowalpha = enterMod2Ease2;
                if (shadowalpha > 0)
                {
                    //{
                    //    int shadownum = 8;
                    //    for (int i = 0; i < shadownum; i++)
                    //    {
                    //        SPRITE_NUMBER_DRAW_EX sndex2 = sndex;
                    //        sndex2.color = MkF4(1.75f, 1.75f, 1.75f, 1.0f);
                    //        sndex2.color.w *= enterMod1 * shadowalpha;
                    //        sndex2.pos.x += sin(2.0f * SYS_GENERAL_PI / shadownum * i) * sndex.size.x / 12.0f;
                    //        sndex2.pos.y += cos(2.0f * SYS_GENERAL_PI / shadownum * i) * sndex.size.y / 12.0f;
                    //        SpriteNumberTimerDraw(sndex2);
                    //    }
                    //}
                    {
                        int shadownum = 8;
                        for (int i = 0; i < shadownum; i++)
                        {
                            SPRITE_NUMBER_DRAW_EX sndex2 = sndex;
                            sndex2.shadertype = DPD_SHADER_TYPE_EX;
                            sndex2.shadertypeExOpt.vs = g_ShaderIndex_UIBgGeneral_Vertex;
                            sndex2.shadertypeExOpt.ps = g_ShaderIndex_EventTimerEnterNumber_Pixel;
                            //sndex2.color = MkF4(2.0f, 2.0f, 2.0f, 1.0f);
                            sndex2.color = MkF4(0.0f, 0.0f, 0.0f, 1.0f);
                            sndex2.color.w *= enterMod1 * shadowalpha;
                            sndex2.pos.x += sin(2.0f * SYS_GENERAL_PI / shadownum * i) * sndex.size.x / 24.0f;
                            sndex2.pos.y += cos(2.0f * SYS_GENERAL_PI / shadownum * i) * sndex.size.y / 24.0f;
                            SpriteNumberTimerDraw(sndex2);
                        }
                    }
                    {
                        SPRITE_NUMBER_DRAW_EX sndex2 = sndex;
                        //sndex2.shadertype = DPD_SHADER_TYPE_EX;
                        //sndex2.shadertypeExOpt.vs = g_ShaderIndex_UIBgGeneral_Vertex;
                        //sndex2.shadertypeExOpt.ps = g_ShaderIndex_EventTimerEnterNumber_Pixel;
                        //sndex2.color = colortmp;
                        //sndex2.color = MkF4(2.0f, 2.0f, 2.0f, 1.0f);
                        sndex2.color = MkF4(1.75f, 0.05f, 0.15f, 1.0f);
                        sndex2.color.w *= enterMod1 * shadowalpha;
                        SpriteNumberTimerDraw(sndex2);
                    }

                }
            }


            //{
            //    float alphatmp =
            //        1.0f * enterMod1
            //        * enterMod2Ease2;
            //    if (alphatmp > 0)
            //    {
            //        SPRITE_NUMBER_DRAW_EX sndex2 = sndex;
            //        sndex.shadertype = DPD_SHADER_TYPE_EX;
            //        sndex.shadertypeExOpt.vs = g_ShaderIndex_UIBgGeneral_Vertex;
            //        sndex.shadertypeExOpt.ps = g_ShaderIndex_EventTimerEnterNumber_Pixel;
            //        sndex.color.a *= alphatmp;
            //        SpriteNumberTimerDraw(sndex2);
            //    }
        }
        {
            DrawSpriteInCamera(
                //表示座標 (四角形の真ん中)
                pos.x + -160.0f + -50.0f * enterMod2Ease2,
                pos.y,

                //サイズ width hight
                100.0f * enterMod2Scale,
                100.0f * enterMod2Scale,

                //角度
                0.0f,

                //色
                //MkF4(1.0f, 1.0f, 1.0f, 1.0f * movieFlagAlpha),
                MkF4(colortmp.x, colortmp.y, colortmp.z, 
                    colortmp.w * enterMod1 * enterMod2Ease1),

                g_tex_Meter_UI,  //画像(テクスチャ)の番号
                AnimationUV(
                    (int)(39 * (1.0f - valuerate)) % 39,
                    6,7),
                MkF2(1.0f / 6, 1.0f / 7)
            );
        }
    }
}


bool ShowHpbar_Game(int index, bool show)
{
	UIDat.hpbar[index] = {};
	UIDat.hpbar[index].enable = show;
    UIDat.hpbar[index].bar.barSub = -2;
    UIDat.hpbar[index].bar.barEnterFrame = -1;

	return show;
}

bool ShowBossbar_Game(
    int index, bool show, std::string name,
    BOSSBAR_TYPE_GAME type,
    float min, float max
)
//bool ShowBossbar(int index, bool show, BOSSBAR_NAME name)
{
	GAME_OBJECT* obj = GetGameObject(index);

    bool beforeEnable = UIDat.bossbar.enable;

	UIDat.bossbar = {};
	UIDat.bossbar.enable = show;
	UIDat.bossbar.obj = obj;
    UIDat.bossbar.name = name;
    UIDat.bossbar.type = type;
    UIDat.bossbar.min = min;
    UIDat.bossbar.max = max;

    UIDat.bossbar.bar.beforeEnable = beforeEnable;

    for (int i = 0; i < BOSSBAR_BARRIER_NUM; i++)
    {
        UIDat.bossbar.barrier[i].enable = false;
        UIDat.bossbar.barrier[i].inF = -1;
        UIDat.bossbar.barrier[i].outF = -1;
        UIDat.bossbar.barrier[i].breakF = -1;
    }

	return show;
}

void ShowBossbarBarrier_Game(int index, int flag)
{
    if (index < 0 || index >= BOSSBAR_BARRIER_NUM)
    {
        return;
    }

    switch (flag)
    {
    case 0:
        UIDat.bossbar.barrier[index].enable = true;
        UIDat.bossbar.barrier[index].inF = 0;
        break;
    case 1:
        if (UIDat.bossbar.barrier[index].enable
            && UIDat.bossbar.barrier[index].outF < 0)
        {
            UIDat.bossbar.barrier[index].outF = 0;
        }
        break;
    case 2:
        if (UIDat.bossbar.barrier[index].enable
            && UIDat.bossbar.barrier[index].breakF < 0)
        {
            UIDat.bossbar.barrier[index].breakF = 0;
        }
        break;
    default:
        break;
    }
}
bool IsEnableBossbarBarrier_Game(int index)
{
    if (index < 0 || index >= BOSSBAR_BARRIER_NUM)
    {
        return false;
    }

    return UIDat.bossbar.barrier[index].enable;
}
bool IsBreakingBossbarBarrier_Game(int index)
{
    if (index < 0 || index >= BOSSBAR_BARRIER_NUM)
    {
        return false;
    }

    return UIDat.bossbar.barrier[index].breakF >= 0;
}

int GetBossbarObjIndex_Game()
{
    if (UIDat.bossbar.obj == nullptr)
    {
        return -1;
    }
    return GetGameObjectIndex(UIDat.bossbar.obj);
}

void barUpdate_Game(UI_BAR_DAT* bar, int current)
{
	if (bar->barEnterFrame >= 0)
	{
		if (bar->barEnterFrame > HPBAR_ENTER_FRAME_MAX)
		{
			if (current > bar->barMain)
			{
				bar->barMain += HPBAR_ENTER_SPEED;
				if (current < bar->barMain)
				{
					bar->barMain = current;
				}
				bar->barSub = bar->barMain;
				bar->barBuf = bar->barMain;
				bar->barBuf2 = bar->barMain;
			}
		}

		bar->barEnterFrame++;
		if (current <= bar->barMain)
		{
			bar->barEnterFrame = -1;
		}
	}
	else
	{
		bar->barMain = current;

		if (bar->barStopFrame > 0)
		{
			bar->barStopFrame--;
			if (bar->barStopFrame <= 0)
			{
				bar->barBuf = current;
			}
		}

        if (bar->barSub == -2)
        {
            bar->barSub = bar->barMain;
            bar->barBuf = bar->barMain;
            bar->barBuf2 = bar->barMain;
        }

		if (bar->barBuf2 != current)
		{
			if (bar->barBuf2 > current)
			{
				bar->barChangeFrame = HPBAR_CHANGE_FRAME_MAX;
			}
			bar->barStopFrame = HPBAR_WAIT_FRAME;
			bar->barBuf2 = current;
		}

		if (bar->barBuf < bar->barSub)
		{
			bar->barSub -= HPBAR_CHANGE_SPEED;
			if (bar->barBuf > bar->barSub)
			{
				bar->barSub = bar->barBuf;
			}
		}
		else if (bar->barBuf > bar->barSub)
		{
			bar->barSub += HPBAR_CHANGE_SPEED;
			if (bar->barBuf < bar->barSub)
			{
				bar->barSub = bar->barBuf;
			}
		}
		bar->barShakeFrame++;
	}
	if (bar->barChangeFrame > 0)
	{
		bar->barChangeFrame--;
	}



	/*bar->isPlayerOverlap = false;
	for (int i = 0; i < PLAYER_MAX; i++)
	{
		PLAYER* player = GetPlayer(i);

		if (player->obj != nullptr)
		{
			if (CheckBoxCollider(
				MkF2(player->obj->pos.x, player->obj->pos.y),
				bar->pos,
				MkF2(player->obj->size.x, player->obj->size.y),
				bar->size )
			)
			{
				bar->isPlayerOverlap = true;
			}
			break;
		}
	}

	if (bar->isPlayerOverlap)
	{
		if (bar->playerOverlapRate < UI_RATE_MAX)
		{
			bar->playerOverlapRate += HPBAR_PLAYER_OVERLAP_RATE_SPEED;
			if (bar->playerOverlapRate > UI_RATE_MAX)
			{
				bar->playerOverlapRate = UI_RATE_MAX;
			}
		}
	}
	else
	{
		if (bar->playerOverlapRate > 0)
		{
			bar->playerOverlapRate -= HPBAR_PLAYER_OVERLAP_RATE_SPEED;
			if (bar->playerOverlapRate < 0)
			{
				bar->playerOverlapRate = 0;
			}
		}
	}*/

}

void update_player_gui(
    unsigned int gframe, int i, int cIndex,
    Float4 colorAlphaSetTmp, float movieFlagAlpha,
    bool isSub, int subindex
)
{
    float playerIconMod = 0;

    TEX_DAT playerIcon = {};
    float playerIconXScaleMod = 1.0f;
    float playerIconYScaleMod = 1.0f;
    Float4 playerIconColor = colorAlphaSetTmp;

    unsigned int playerIconSub = 0;
    unsigned int playerIconNum = 0;
    unsigned int playerIconSkillOn = 0;
    unsigned int playerIconSkillOff = 0;

    int playerIndex = 0;
    GOE_Character_Player* playerGOE = UIDat.hpbar[i].playerGOE;
    {
        if (UIDat.hpbar[i].player == nullptr)
        {
            return;
        }
        if (UIDat.hpbar[i].player->obj == nullptr)
        {
            return;
        }
        if (playerGOE == nullptr)
        {
            return;
        }

        playerIndex = UIDat.hpbar[i].player->index;
        playerIconNum = g_tex_chara_icon_num[playerIndex];

        {
            if (playerGOE->IsDefeat())
            {
                playerIcon = playerGOE->GetTex_IconMain_Out();
            }
            else
            {
                if (playerGOE->IsPinch())
                {
                    {
                        float m = sin((float)UIDat.hpbar[i].player->obj->frameActive * 0.7f)
                            / 2.0f + 0.5f;
                        playerIconColor.x += 0.35f * m;
                        playerIconColor.y += -0.35f * m;
                        playerIconColor.z += -0.35f * m;
                    }
                    playerIcon = playerGOE->GetTex_IconMain_Pinch();
                }
                else
                {
                    playerIcon = playerGOE->GetTex_IconMain_N();
                }

                {
                    constexpr int dfIn = 20;
                    constexpr int dffade = 25;

                    int df = playerGOE->GetDamageFrame();
                    if (df >= 0 && df <= dfIn + dffade)
                    {
                        int damintf = std::min(df, dfIn);
                        int damoutf = std::max(df - dfIn, 0);

                        if (damoutf <= dffade / 2)
                        {
                            playerIcon = playerGOE->GetTex_IconMain_Pinch();
                        }
                        if (df <= dfIn)
                        {
                            float m = 1.0f - (float)damintf / dfIn;
                            playerIconColor.x += +0.85f * EasingInSine(1.0f - m);
                            playerIconColor.y += -0.35f * EasingInSine(m);
                            playerIconColor.z += -0.35f * EasingInSine(m);

                            float sm = 1.0f - EasingOutSine(std::min(
                                (float)df / (float)std::max(dfIn / 4, 1),
                                1.0f));
                            playerIconXScaleMod *= (1.0f + 0.5f * (sm));
                            playerIconYScaleMod *= (1.0f + 0.5f * (sm));
                        }

                        playerIconXScaleMod *= (cos(2.0f * SYS_GENERAL_PI
                            * (float)damoutf / dffade) / 2.0f + 0.5f);

                    }
                }
            }

            playerIconMod = playerGOE->GetTex_IconMain_Mod();
        }


        playerIconSub = playerGOE->GetTex_IconSub().textureId;
        playerIconSkillOn = playerGOE->GetTex_IconSkillOn().textureId;
        playerIconSkillOff = playerGOE->GetTex_IconSkillOff().textureId;
    }

    Float2 subPos = {};
    
    Float2 subPosIcon = {};
    Float2 subPosBar = {};
    Float2 subPosBar_Ink = {};

    Float2 subScale = {};
    subScale.x = 1.0f;
    subScale.y = 1.0f;


    float inkbarMod = (float)(UIDat.inkbar[i].bar.barMain) / UI_RATE_MAX;
    constexpr float inkbarPinchMod = 0.4f;
    bool isInkbarPinch = 
        inkbarMod <= inkbarPinchMod
        && gframe % 8 < 4;

    float infoscale = 1.0f;
    Float2 infoAddPos = {};
    float infoscaleSub = 0.0f;

    if (isSub)
    {
        subPosBar.x += -150.0f;
        subPosBar.y += -80.0f * subindex - 120.0f;
        subPosBar_Ink.y += 20.0f;

        subPosIcon.x += -20.0f;
        subPosIcon.y += -80.0f * subindex - 120.0f;

        subScale.x *= 0.8f;
        subScale.y *= 0.8f;

        //アイコん
        {
            Float4 colrtmp = colorAlphaSetTmp;
            if (playerIndex == 0)
            {
                colrtmp.x = 248.0f;
                colrtmp.y = 47.0f;
                colrtmp.z = 94.0f;
            }
            if (playerIndex == 1)
            {
                colrtmp.x = 72.0f;
                colrtmp.y = 0.0f;
                colrtmp.z = 205.0f;
            }
            if (playerIndex == 2)
            {
                colrtmp.x = 255.0f;
                colrtmp.y = 140.0f;
                colrtmp.z = 4.0f;
            }
            if (playerIndex == 3)
            {
                colrtmp.x = 0.0f;
                colrtmp.y = 168.0f;
                colrtmp.z = 24.0f;
            }
            colrtmp.x /= 255.0f;
            colrtmp.y /= 255.0f;
            colrtmp.z /= 255.0f;
            //colrtmp.x *= 2.0f;
            //colrtmp.y *= 2.0f;
            //colrtmp.z *= 2.0f;
            colrtmp.x += 0.9f;
            colrtmp.y += 0.9f;
            colrtmp.z += 0.9f;
            
            //DrawSpriteInCameraSingle(
            //    cIndex,
            //    //表示座標 (四角形の真ん中)
            //    -SCREEN_WIDTH / 2 + 120.0f + subPosIcon.x,
            //    SCREEN_HEIGHT / 2 - 130.0f + subPosIcon.y,

            //    //サイズ width hight
            //    60.0f * subScale.x,
            //    60.0f * subScale.y,

            //    //角度
            //    0.0f,

            //    //色
            //    colrtmp,

            //    playerIconSub,  //画像(テクスチャ)の番号
            //    MkF2(0.0f, 0.0f),
            //    MkF2(1.0f, 1.0f)
            //);
            {
                DRAW_POLYGON_DAT drawDatTmp = {};
                drawDatTmp.pos = MkF3(
                    -SCREEN_WIDTH / 2 + 120.0f + subPosIcon.x,
                    SCREEN_HEIGHT / 2 - 130.0f + subPosIcon.y,
                    0);
                drawDatTmp.size = MkF3(
                    60.0f * subScale.x, 
                    60.0f * subScale.y, 
                    1.0f);

                drawDatTmp.rot = 0.0f;

                drawDatTmp.color = colrtmp;

                drawDatTmp.texNo = playerIconSub;
                drawDatTmp.loadTexType = LOADTEXTURETYPE_MAIN;

                drawDatTmp.texUV = MkF2(0.0f, 0.0f);
                drawDatTmp.texWH = MkF2(1.0f, 1.0f);
                drawDatTmp.startScale = MkF2(1.0f, 1.0f);
                drawDatTmp.startScale3D = MkF3(1.0f, 1.0f, 1.0f);

                drawDatTmp.vertex.type = DRAW_POLYGON_TYPE_QUAD;

                //drawDatTmp.shaderOpt.blendState = DPD_SHADER_OPT_BLEND_STATE_ADDCOLOR;

                DRAW_POLYGON_IN_CAMERA_EX_OPTION opt = {};
                opt.reverse = false;
                opt.fixed = true;
                opt.enableShade = false;
                opt.priority = DRAW_POLYGON_PRIORITY_FORCE_NORMAL;

                DrawPolygonInCameraSingle(cIndex, drawDatTmp, opt);
            }
            DrawSpriteInCameraSingle(
                cIndex,
                //表示座標 (四角形の真ん中)
                -SCREEN_WIDTH / 2 + 80.0f + subPosIcon.x,
                SCREEN_HEIGHT / 2 - 130.0f + subPosIcon.y,

                //サイズ width hight
                60.0f * subScale.x,
                60.0f * subScale.y,

                //角度
                0.0f,

                //色
                colorAlphaSetTmp,

                playerIconNum,  //画像(テクスチャ)の番号
                MkF2(0.0f, 0.0f),
                MkF2(1.0f, 1.0f)
            );
        }
    }
    else
    {
        int skF = playerGOE->GetSkillCooldownFrame();
        int skFmax = playerGOE->GetSkillCooldownFrameMax();
        float skFmod = 1.0f - (float)skF / skFmax;

        //プレイヤー表記

        {
            CAMERA cinfo = GetCameraInfo(cIndex);
            DX_CAMERA_INFO dxcinfo = DX_GetCameraInfo(cIndex);

            for (int j = 0; j < PLAYER_MAX; j++)
            {
                if (UIDat.hpbar[j].player == nullptr)
                {
                    continue;
                }
                if (UIDat.hpbar[j].player->obj == nullptr)
                {
                    continue;
                }

                Float3 abPos = GetGameObjectColRecAbsolutePos(UIDat.hpbar[j].player->obj);
                
                {
                    DRAW_POLYGON_DAT dsdat = {};
                    //dsdat.pos.z = -800.0f;

                    float dismod = 1.0f + 0.25f * std::max(1.0f - std::max(
                        cinfo.trans.pos.y - UIDat.hpbar[j].player->obj->pos.y, 0.0f)
                        / 1500.0f, 0.0f) * dxcinfo.CameraVector.y;

                    dsdat.size.z = 1.0f;

                    dsdat.rot = 0.0f;

                    //dsdat.color = { 1.0f, 1.0f, 0.8f, 0.4f };
                    dsdat.color = colorAlphaSetTmp;

                    dsdat.texNo = 0;
                    dsdat.loadTexType = LOADTEXTURETYPE_MAIN;

                    dsdat.texUV = {
                        0.0f,
                        0.0f
                    };
                    dsdat.texWH = { 1.0f, 1.0f };

                    dsdat.startPos = {
                        0.0f,
                        0.0f
                    };
                    dsdat.startPos3D = UIDat.hpbar[j].player->obj->pos;

                    dsdat.startRot = 0;
                    dsdat.startScale = { 1.0f, 1.0f };
                    dsdat.startScale3D = { 1.0f, 1.0f, 1.0f };

                    dsdat.shaderOpt.bloom = DPD_SHADER_OPT_BLOOM_NONE;
                    //dsdat.shaderOpt.blendState = DPD_SHADER_OPT_BLEND_STATE_ADDCOLOR;

                    DRAW_POLYGON_IN_CAMERA_EX_OPTION opt = {};
                    opt.reverse = false;
                    opt.fixed = false;
                    opt.billboard = true;
                    opt.depthBuffIgnore = true;
                    opt.priority = DRAW_POLYGON_PRIORITY_FORCE_DEEPEST;

                    DRAW_POLYGON_IN_CAMERA_EX_MAT_EX matex = {};


                    if (playerGOE->IsDrawing())
                    {
                        DRAW_POLYGON_IN_CAMERA_EX_OPTION opt2 = opt;

                        bool isEmpty = UIDat.inkbar->bar.barMain <= 0;
                        float emptyMod = 1.0f;
                        if (isEmpty)
                        {
                            emptyMod = 1.0f + 0.5f * (sin((float)gframe / 10) / 2.0f + 0.5f);
                        }

                        float barmod = inkbarMod;
                        //barmod = 0.02f + 0.96f * barmod;

                        dsdat.size.x = 1.25f * 140.0f / cinfo.trans.zoom * dismod * emptyMod;
                        dsdat.size.y = 1.25f * 20.0f / cinfo.trans.zoom * dismod * emptyMod;
                        matex.offset.y = 
                            (130.0f + (abPos.y - UIDat.hpbar[j].player->obj->pos.y) * 100.0f)
                            * cinfo.trans.zoom * dismod;

                        //dsdat.color.x = 0.25f;
                        //dsdat.color.y = 0.25f;
                        //dsdat.color.z = 0.25f;
                        dsdat.texNo = g_tex_head_ink_bg;
                        dsdat.color.x = 1.0f * emptyMod;
                        dsdat.color.y = 1.0f;
                        dsdat.color.z = 1.0f;
                        dsdat.color.w = 0.85f;
                        //dsdat.startPos.x = dsdat.size.x * ((barmod) / 2);
                        //dsdat.startScale.x = (1.0f - barmod);
                        //dsdat.texUV.x = barmod;
                        //dsdat.texWH.x = (1.0f - barmod);
                        dsdat.startPos.x = 0.0f;
                        dsdat.startScale.x = 1.0f;
                        dsdat.texUV.x = 0.0f;
                        dsdat.texWH.x = 1.0f;
                        DrawPolygonInCameraSingle(cIndex, dsdat, opt2
                            , &matex
                        );

                        opt2.shaderType = DPD_SHADER_TYPE_EX;
                        opt2.shaderTypeEx.vs = g_ShaderIndex_GageGradient_Vertex;
                        opt2.shaderTypeEx.ps = g_ShaderIndex_GageGradient_Pixel;
                        //dsdat.color.x = 2.0f;
                        //dsdat.color.y = 2.0f;
                        //dsdat.color.z = 2.0f;
                        dsdat.texNo = g_tex_head_ink_main2;
                        dsdat.color.x = 1.0f;
                        dsdat.color.y = 1.0f;
                        dsdat.color.z = 1.0f;
                        dsdat.color.w = 0.85f;
                        //dsdat.startPos.x = dsdat.size.x * (-(1.0f - barmod) / 2);
                        //dsdat.startScale.x = barmod;
                        //dsdat.texUV.x = 0.0f;
                        //dsdat.texWH.x = barmod;

                        dsdat.color = playerGOE->GetThemeColor();
                        if (isInkbarPinch)
                        {
                            dsdat.color = {2.0f, 0.0f, 0.0f, 1.0f};
                        }
                        dsdat.color.w = 1.0f;
                        dsdat.startPos.x = 0.0f;
                        dsdat.startScale.x = 1.0f;
                        dsdat.size.x = 1.25f * 133.0f / cinfo.trans.zoom * dismod * emptyMod;
                        dsdat.size.y = 1.25f * 13.0f / cinfo.trans.zoom * dismod * emptyMod;
                        
                        dsdat.texUV.x = 0.5f * (1.0f - barmod);
                        dsdat.texWH.x = 0.5f;
                        DrawPolygonInCameraSingle(cIndex, dsdat, opt2
                            , &matex
                        );

                        if (isEmpty)
                        {
                            dsdat.texNo = g_tex_head_ink_cross;
                            dsdat.color.x = 1.0f;
                            dsdat.color.y = 1.0f;
                            dsdat.color.z = 1.0f;
                            dsdat.color.w = 1.0f;
                            dsdat.startPos.x = 0.0f;
                            dsdat.startScale.x = 1.0f;
                            dsdat.size.y = dsdat.size.x;

                            dsdat.texUV.x = 0.0;
                            dsdat.texWH.x = 1.0f;
                            DrawPolygonInCameraSingle(cIndex, dsdat, opt
                                , &matex
                            );

                            //matex.offset.y =
                            //    (180.0f + (abPos.y - UIDat.hpbar[j].player->obj->pos.y) * 100.0f)
                            //    * cinfo.trans.zoom * dismod;
                            //dsdat.texNo = g_tex_head_ink_text_danger;
                            //dsdat.color.x = 1.0f;
                            //dsdat.color.y = 1.0f;
                            //dsdat.color.z = 1.0f;
                            //dsdat.color.w = 1.0f;
                            //dsdat.startPos.x = 0.0f;
                            //dsdat.startScale.x = 1.0f;
                            //dsdat.size.x *= 2.4f;
                            //dsdat.size.y = dsdat.size.x;

                            //dsdat.texUV.x = 0.0;
                            //dsdat.texWH.x = 1.0f;
                            //DrawPolygonInCameraSingle(cIndex, dsdat, opt
                            //    , &matex
                            //);
                        }
                        
                    }
                    else
                        if (playerGOE->IsInput_KakoSub() && skFmod < 1.0f)
                        {
                            dsdat.size.x = 140.0f / cinfo.trans.zoom * dismod;
                            dsdat.size.y = 140.0f / cinfo.trans.zoom * dismod;
                            matex.offset.y =
                                (140.0f + (abPos.y - UIDat.hpbar[j].player->obj->pos.y) * 100.0f)
                                * cinfo.trans.zoom * dismod;

                            dsdat.color.x = 1.8f;
                            dsdat.color.y = 1.8f;
                            dsdat.color.z = 1.8f;
                            dsdat.color.w *= 0.5f;

                            dsdat.texNo = g_tex_Meter_UI;  //画像(テクスチャ)の番号
                            dsdat.texUV = AnimationUV(
                                (int)(39 * (skFmod)) % 39,
                                6, 7);
                            dsdat.texWH = MkF2(1.0f / 6, 1.0f / 7);

                            DrawPolygonInCameraSingle(cIndex, dsdat, opt
                                , &matex
                            );

                            //dsdat.color.x = 2.0f;
                            //dsdat.color.y = 2.0f;
                            //dsdat.color.z = 2.0f;
                            ////matex.offset.y += dsdat.size.y * ((1.0f - skFmod) / 2) 
                            ////    * cinfo.trans.zoom * dismod;
                            //dsdat.startScale.y *= skFmod;

                            //DrawPolygonInCameraSingle(cIndex, dsdat, opt
                            //    , &matex
                            //);
                        }
                }

                if (GetActivePlayerNum() >= 2)
                {
                    DRAW_POLYGON_DAT dsdat = {};
                    //dsdat.pos.z = -800.0f;

                    //float dismod = 1.0f;
                    //float dismod = CalculateDistance(
                    //    Float3ToFloat2(cinfo.trans.target),
                    //    Float3ToFloat2(cinfo.trans.pos))
                    //    / (20000);
                    //float dismod = CalculateDistance(cinfo.trans.target, cinfo.trans.pos)
                    //    / (20000);
                    float dismod = 1.0f + 0.25f * std::max(1.0f - std::max(
                        cinfo.trans.pos.y - UIDat.hpbar[j].player->obj->pos.y, 0.0f)
                        / 1500.0f, 0.0f) * dxcinfo.CameraVector.y;

                    dsdat.size.x = 75.0f / cinfo.trans.zoom * dismod;
                    dsdat.size.y = 75.0f / cinfo.trans.zoom * dismod;
                    dsdat.size.z = 1.0f;

                    dsdat.rot = 0.0f;

                    //dsdat.color = { 1.0f, 1.0f, 0.8f, 0.4f };
                    dsdat.color = colorAlphaSetTmp;

                    dsdat.texNo = g_tex_chara_icon_num[j];
                    dsdat.loadTexType = LOADTEXTURETYPE_MAIN;

                    dsdat.texUV = {
                        0.0f,
                        0.0f
                    };
                    dsdat.texWH = { 1.0f, 1.0f };

                    dsdat.startPos = {
                        0.0f,
                        0.0f
                    };
                    dsdat.startPos3D = UIDat.hpbar[j].player->obj->pos;

                    dsdat.startRot = 0;
                    dsdat.startScale = { 1.0f, 1.0f };
                    dsdat.startScale3D = { 1.0f, 1.0f, 1.0f };

                    dsdat.shaderOpt.bloom = DPD_SHADER_OPT_BLOOM_NONE;
                    //dsdat.shaderOpt.blendState = DPD_SHADER_OPT_BLEND_STATE_ADDCOLOR;

                    DRAW_POLYGON_IN_CAMERA_EX_OPTION opt = {};
                    opt.reverse = false;
                    opt.fixed = false;
                    opt.billboard = true;
                    opt.depthBuffIgnore = true;
                    opt.priority = DRAW_POLYGON_PRIORITY_FORCE_DEEPEST;
                    
                    DRAW_POLYGON_IN_CAMERA_EX_MAT_EX matex = {};
                    matex.offset.y = 120.0f * cinfo.trans.zoom * dismod;

                    DRAW_POLYGON_DAT dsdat2 = dsdat;
                    dsdat2.texNo = g_tex_chara_icon_num_tri[j];
                    dsdat2.size.x *= 0.75f;
                    dsdat2.size.y *= 0.75f;
                    DRAW_POLYGON_IN_CAMERA_EX_OPTION opt2 = opt;
                    DRAW_POLYGON_IN_CAMERA_EX_MAT_EX matex2 = matex;
                    matex2.offset.y += -25.0f * cinfo.trans.zoom * dismod;

                    //{
                    //    DRAW_POLYGON_BUFFER dpb = {};
                    //    POLYGON_RELEASE_OPTION ropt = {};

                    //    XMMATRIX polMat = DrawPolygonInCameraCalMatrix(
                    //        dpb,
                    //        ropt,

                    //        cIndex,
                    //        dsdat,
                    //        opt
                    //    );

                    //    //float outdis = 80.0f;

                    //    Float3 imgpos =
                    //        MkF3(
                    //            polMat.r[3].m128_f32[0] + matex.offset.x,
                    //            polMat.r[3].m128_f32[1] + matex.offset.y,
                    //            polMat.r[3].m128_f32[2]
                    //        );
                    //    Float2 imgcpos = WorldPosToCameraPos(cIndex, imgpos);
                    //    //if (
                    //    //    CalculateDistance({}, imgpos
                    //    //    ) > outdis
                    //    //)
                    //    //{
                    //    //    Float2 vectmp = {
                    //    //        imgpos.x,
                    //    //        imgpos.y
                    //    //    };
                    //    //    vectmp = NormalizeVector(vectmp);

                    //    //    matex.isFixed = true;
                    //    //    matex.offset.x = vectmp.x * outdis;
                    //    //    matex.offset.y = vectmp.y * outdis;
                    //    //}
                    //    float outdisX = 70.0f;
                    //    float outdisY = 60.0f;
                    //    Float2 vectmp = {
                    //        imgcpos.x,
                    //        imgcpos.y
                    //    };
                    //    vectmp = NormalizeVector(vectmp);
                    //    if (abs(imgcpos.x) > outdisX
                    //        || abs(imgcpos.y) > outdisY)
                    //    {
                    //        matex.isFixed = true;
                    //        matex.offset.x = vectmp.x * outdisX;
                    //        matex.offset.y = vectmp.y * outdisY;

                    //        matex2.isFixed = true;
                    //        matex2.offset.x = vectmp.x * outdisX * 1.2f;
                    //        matex2.offset.y = vectmp.y * outdisY * 1.2f;
                    //        dsdat2.rot3D.y = CalculateRadToDeg(atan2(vectmp.y, vectmp.x)) - 90.0f;
                    //    }
                    //}

                    //Float3 imgpos =
                    //    MkF3(
                    //        UIDat.hpbar[j].player->obj->pos.x,
                    //        UIDat.hpbar[j].player->obj->pos.y,
                    //        UIDat.hpbar[j].player->obj->pos.z
                    //    );

                    //Float2 imgcpos = WorldPosToCameraPos(cIndex, imgpos);

                    //if (cIndex == 0 && j == 1)
                    //{
                    //    DebugPrintf("%f %f\n",
                    //        imgcpos.x,
                    //        imgcpos.y
                    //    );
                    //}
                    //
                    //{
                    //    DrawSpriteInCameraSingle(
                    //        cIndex,
                    //        //表示座標 (四角形の真ん中)
                    //        imgcpos.x ,
                    //        imgcpos.y ,
                    //        //サイズ width hight
                    //        100.0f,
                    //        100.0f,
                    //        //角度
                    //        0.0f,
                    //        //色
                    //        colorAlphaSetTmp,
                    //        g_tex_chara_icon_num[j],  //画像(テクスチャ)の番号
                    //        MkF2(0.0f, 0.0f),
                    //        MkF2(1.0f, 1.0f)
                    //    );
                    //}

                    //if (cIndex == 0 && j == 1)
                    //{
                    //    DebugPrintf("%f %f %f\n",
                    //        polMat.r[3].m128_f32[0],
                    //        polMat.r[3].m128_f32[1],
                    //        polMat.r[3].m128_f32[2]
                    //    );
                    //}

                    //if (
                    //    polMat.r[3].m128_f32[0] < -SCREEN_WIDTH / 2 || polMat.r[3].m128_f32[0] > SCREEN_WIDTH / 2
                    //    || polMat.r[3].m128_f32[1] < -SCREEN_HEIGHT / 2 || polMat.r[3].m128_f32[1] > SCREEN_HEIGHT / 2)
                    //{
                    //}

                    //{
                    //    Float3 pos = dsdat.startPos3D;

                    //    float poslimX = +SCREEN_WIDTH * 0.75f / cinfo.trans.zoom * dismod;
                    //    float poslimY = SCREEN_HEIGHT * 0.75f / cinfo.trans.zoom * dismod;

                    //    float possubX = pos.x - cinfo.trans.pos.x;
                    //    float possubY = pos.y - cinfo.trans.pos.y;
                    //    bool outflag = false;
                    //    if (possubX > poslimX / 2)
                    //    {
                    //        outflag = true;
                    //        dsdat.startPos3D.x = cinfo.trans.pos.x + poslimX / 2;
                    //    }
                    //    if (possubX < -poslimX / 2)
                    //    {
                    //        outflag = true;
                    //        dsdat.startPos3D.x = cinfo.trans.pos.x - poslimX / 2;
                    //    }
                    //    if (possubY > poslimY / 2)
                    //    {
                    //        dsdat.startPos3D.y = cinfo.trans.pos.y + poslimY / 2;
                    //    }
                    //    //if (possubY < -poslimY / 2)
                    //    //{
                    //    //    dsdat.startPos3D.y = cinfo.trans.pos.y - poslimY / 2;
                    //    //}
                    //    if (outflag)
                    //    {
                    //        dsdat2.startPos3D = dsdat.startPos3D;
                    //        //dsdat2.rot3D.x = 90.0f;
                    //        dsdat2.rot = CalculateRadToDeg(atan2(
                    //            dsdat2.startPos3D.y,
                    //            dsdat2.startPos3D.x
                    //        )) - 90.0f;
                    //        opt2.billboard = false;
                    //    }
                    //}

                    //if (cIndex == 0 && j == 1)
                    //{
                    //    DX_CAMERA_INFO dxcinfo = DX_GetCameraInfo(cIndex);
                    //    //DebugPrintf("%f %f %f\n",
                    //    //    cdb.dpb.pro.r[3].m128_f32[0],
                    //    //    cdb.dpb.pro.r[3].m128_f32[1],
                    //    //    cdb.dpb.pro.r[3].m128_f32[2]
                    //    //);
                    //    //
                    //}

                    //{
                    //    DX_CAMERA_INFO dxcinfo = DX_GetCameraInfo(cIndex);
                    //    //DebugPrintf("%f %f %f\n",
                    //    //    cdb.dpb.pro.r[3].m128_f32[0],
                    //    //    cdb.dpb.pro.r[3].m128_f32[1],
                    //    //    cdb.dpb.pro.r[3].m128_f32[2]
                    //    //);
                    //    //
                    //    // スプライトの位置
                    //    XMFLOAT3 spritePosition = {
                    //        UIDat.hpbar[j].player->obj->pos.x / CAMERA_3D_MOD / 100,
                    //        UIDat.hpbar[j].player->obj->pos.y / CAMERA_3D_MOD / 100,
                    //        UIDat.hpbar[j].player->obj->pos.z / CAMERA_3D_MOD / 100
                    //    };
                    //    XMVECTOR spritePosition2 = XMLoadFloat3(&spritePosition);

                    //    // カメラの視野範囲を計算
                    //    XMMATRIX viewMatrix = dxcinfo.CameraView;
                    //    XMMATRIX projectionMatrix = dxcinfo.CameraPos;
                    //    DirectX::BoundingFrustum frustum;
                    //    DirectX::BoundingFrustum::CreateFromMatrix(frustum, projectionMatrix);

                    //    // スプライトが視野範囲内にあるか確認
                    //    DirectX::BoundingBox spriteBox;
                    //    DirectX::BoundingBox::CreateFromPoints(
                    //        spriteBox, spritePosition2, spritePosition2);

                    //    bool isVisible = frustum.Intersects(spriteBox);

                    //    if (isVisible) 
                    //    {
                    //    }
                    //    else 
                    //    {
                    //        dsdat2.startPos3D = {};
                    //        dsdat.startPos3D = {};
                    //    }
                    //}

                    // スプライトはカメラに映っている

                    DrawPolygonInCameraSingle(cIndex, dsdat2, opt2
                        , &matex2
                    );
                    //CAMERA_DRAW_BUF cdb =
                    DrawPolygonInCameraSingle(cIndex, dsdat, opt
                        , &matex
                    );

                    //DRAW_POLYGON_DAT dsdat = {};
                    //dsdat.startPos3D = UIDat.hpbar[j].player->obj->pos;
                    //DRAW_POLYGON_IN_CAMERA_EX_OPTION opt = {};
                    //opt.reverse = false;
                    //opt.fixed = false;
                    //opt.billboard = true;
                    //opt.depthBuffIgnore = true;
                    //opt.priority = DRAW_POLYGON_PRIORITY_FORCE_DEEPEST;

                    //DRAW_POLYGON_BUFFER dpb = {};
                    //POLYGON_RELEASE_OPTION ropt = {};
                    //XMMATRIX polMat = DrawPolygonInCameraCalMatrix(
                    //    dpb,
                    //    ropt,

                    //    cIndex,
                    //    dsdat,
                    //    opt
                    //);
                    //Float2 cp = WorldPosToCameraPos(
                    //    cIndex,
                    //    MkF3(
                    //        polMat.r[3].m128_f32[0],
                    //        polMat.r[3].m128_f32[1],
                    //        polMat.r[3].m128_f32[2]
                    //    )
                    //);
                    //Float2 cp = WorldPosToCameraPos(
                    //    cIndex,
                    //    UIDat.hpbar[j].player->obj->pos
                    //);
                    //cp.x *= -1;
                    //cp.x += -SCREEN_WIDTH / 4;
                   //cp.y += -SCREEN_HEIGHT / 2;
                    //cp.y *= -1;
                    //cp.y += -SCREEN_HEIGHT / 2
                    //    + -300.0f * cinfo.trans.zoom;
                    //cp.y += -SCREEN_HEIGHT;
                    //    DebugPrintf("%f %f\n",
                    //        cp.x,
                    //        cp.y
                    //    );

                    //{
                    //    DrawSpriteInCameraSingle(
                    //        cIndex,
                    //        //表示座標 (四角形の真ん中)
                    //        cp.x,
                    //        cp.y,
                    //        //サイズ width hight
                    //        100.0f,
                    //        100.0f,
                    //        //角度
                    //        0.0f,
                    //        //色
                    //        colorAlphaSetTmp,
                    //        g_tex_chara_icon_num[j],  //画像(テクスチャ)の番号
                    //        MkF2(0.0f, 0.0f),
                    //        MkF2(1.0f, 1.0f)
                    //    );
                    //}
                }
            }
        }

        //infoscale = 0.8f * 1.25f;
        infoscale = 0.8f * 1.45f;
        infoscaleSub = infoscale - 0.8f;
        infoAddPos.x = -15.0f;
        infoAddPos.y = +20.0f;
        //
        {
            DPD_SHADER_TYPE_EX_OPT opt = {};
            opt.vs = g_ShaderIndex_UIBgGeneral_Vertex;
            opt.ps = g_ShaderIndex_UIBgGeneral_Pixel;
            DrawSpriteInCameraSingle(
                cIndex,
                //表示座標 (四角形の真ん中)
                infoAddPos.x + -SCREEN_WIDTH / 2 + 400.0f - (750.0f / 2 * (1.0f - infoscale) * 1.0f),
                infoAddPos.y + SCREEN_HEIGHT / 2 - 100.0f + (750.0f / 2 * (1.0f - infoscale) * 0.25f),

                //サイズ width hight
                750.0f * infoscale,
                750.0f * infoscale,

                //角度
                0.0f,

                //色
                colorAlphaSetTmp,

                g_tex_Charactor_Z,  //画像(テクスチャ)の番号
                MkF2(0.0f, 0.0f),
                MkF2(1.0f, 1.0f),
                DRAW_POLYGON_PRIORITY_FORCE_NORMAL,
                DPD_SHADER_TYPE_EX,
                opt
            );
        }

        {
            {
                Float2 sizetmp = MkF2(
                    120.0f,
                    120.0f
                );
                Float2 postmp = MkF2(
                    -SCREEN_WIDTH / 2 + 695.0f 
                    - (sizetmp.x / 2 * (1.0f - infoscale) * 11.3f),
                    SCREEN_HEIGHT / 2 - 100.0f
                    + (sizetmp.y / 2 * (1.0f - infoscale) * 1.5f)
                );
                sizetmp.x *= infoscale;
                sizetmp.y *= infoscale;

                DrawSpriteInCameraSingle(
                    cIndex,
                    //表示座標 (四角形の真ん中)
                    infoAddPos.x + postmp.x,
                    infoAddPos.y + postmp.y,

                    //サイズ width hight
                    sizetmp.x,
                    sizetmp.y,

                    //角度
                    0.0f,

                    //色
                    colorAlphaSetTmp,

                    skF > 0 ?
                    playerIconSkillOff : playerIconSkillOn,  //画像(テクスチャ)の番号

                    MkF2(0.0f, 0.0f),
                    MkF2(1.0f, 1.0f)
                );

                if (skF > 0)
                {
                    float mod = skFmod;

                    Float4 colorTmp = colorAlphaSetTmp;
                    colorTmp.w *= 0.7f;

                    DrawSpriteInCameraSingle(
                        cIndex,
                        //表示座標 (四角形の真ん中)
                        infoAddPos.x + postmp.x,
                        infoAddPos.y + postmp.y + sizetmp.y / 2 * (1.0f - mod),

                        //サイズ width hight
                        sizetmp.x,
                        sizetmp.y * mod,

                        //角度
                        0.0f,

                        //色
                        colorTmp,

                        playerIconSkillOn,  //画像(テクスチャ)の番号

                        MkF2(0.0f, 1.0f - mod),
                        MkF2(1.0f, 1.0f * mod)
                    );
                }
            }
            {
                DrawSpriteInCameraSingle(
                    cIndex,
                    //表示座標 (四角形の真ん中)
                    infoAddPos.x + -SCREEN_WIDTH / 2 + 735.0f - (50.0f / 2 * (1.0f - infoscale) * 27.0f),
                    infoAddPos.y + SCREEN_HEIGHT / 2 - 150.0f + (50.0f / 2 * (1.0f - infoscale) * 7.0f),

                    //サイズ width hight
                    50.0f * infoscale,
                    50.0f * infoscale,

                    //角度
                    0.0f,

                    //色
                    colorAlphaSetTmp,

                    g_tex_btn_x,  //画像(テクスチャ)の番号

                    MkF2(0.0f, 0.0f),
                    MkF2(1.0f, 1.0f)
                );
            }
        }

        {
            //アイコん
            {
                DrawSpriteInCameraSingle(
                    cIndex,
                    //表示座標 (四角形の真ん中)
                    infoAddPos.x + -SCREEN_WIDTH / 2 + 120.0f + subPosIcon.x 
                    - (150.0f / 2 * (1.0f - infoscale) * 1.0f) + -5.0f,
                    infoAddPos.y + SCREEN_HEIGHT / 2 - 120.0f + subPosIcon.y
                    + (150.0f / 2 * (1.0f - infoscale) * 2.0f) + 8.0f,

                    //サイズ width hight
                    140.0f * subScale.x * infoscale * playerIconMod
                     * ((float)playerIcon.width / playerIcon.height) 
                     * playerIconXScaleMod * 0.95f,
                    140.0f * subScale.y * infoscale * playerIconMod
                    * playerIconYScaleMod * 0.95f,

                    //角度
                    0.0f,

                    //色
                    playerIconColor,

                    playerIcon.textureId,  //画像(テクスチャ)の番号
                    MkF2(0.0f, 0.0f),
                    MkF2(1.0f, 1.0f)
                );
            }
            //np
            //{
            //    DrawSpriteInCameraSingle(
            //        cIndex,
            //        //表示座標 (四角形の真ん中)
            //        //-SCREEN_WIDTH / 2 + 120.0f + subPosIcon.x 
            //        //- (80.0f / 2 * (1.0f - infoscale) * 1.0f),
            //        //SCREEN_HEIGHT / 2 - +80.0f + subPosIcon.y
            //        //+ (80.0f / 2 * (1.0f - infoscale) * 1.0f) ,
            //        -SCREEN_WIDTH / 2 + 70.0f + subPosIcon.x
            //        - (80.0f / 2 * (1.0f - infoscale) * 1.0f) - 5.0f,
            //        SCREEN_HEIGHT / 2 - +70.0f + subPosIcon.y
            //        + (80.0f / 2 * (1.0f - infoscale) * 1.0f) + 5.0f,

            //        //サイズ width hight
            //        //80.0f * subScale.x * infoscale,
            //        //80.0f * subScale.y * infoscale,
            //        70.0f * subScale.x * infoscale * 0.85f,
            //        70.0f * subScale.y * infoscale * 0.85f,

            //        //角度
            //        0.0f,

            //        //色
            //        colorAlphaSetTmp,

            //        playerIconNum,  //画像(テクスチャ)の番号
            //        MkF2(0.0f, 0.0f),
            //        MkF2(1.0f, 1.0f)
            //    );
            //}

        }
        {
            {
                DrawSpriteInCameraSingle(
                    cIndex,
                    //表示座標 (四角形の真ん中)
                    infoAddPos.x + -SCREEN_WIDTH / 2 + 220.0f - (40.0f / 2 * (1.0f - infoscale) * 7.0f) + 16.0f * infoscaleSub,
                    infoAddPos.y + SCREEN_HEIGHT / 2 - 115.0f + (40.0f / 2 * (1.0f - infoscale) * 3.0f) - 16.0f *infoscaleSub,

                    //サイズ width hight
                    40.0f * infoscale,
                    40.0f * infoscale,

                    //角度
                    0.0f,

                    //色
                    colorAlphaSetTmp,

                    g_tex_chara_icon_hp,  //画像(テクスチャ)の番号
                    MkF2(0.0f, 0.0f),
                    MkF2(1.0f, 1.0f)
                );
            }
            {
                DrawSpriteInCameraSingle(
                    cIndex,
                    //表示座標 (四角形の真ん中)
                    infoAddPos.x + -SCREEN_WIDTH / 2 + 220.0f - (40.0f / 2 * (1.0f - infoscale) * 7.0f) + 16.0f * infoscaleSub,
                    infoAddPos.y + SCREEN_HEIGHT / 2 - 075.0f + (40.0f / 2 * (1.0f - infoscale) * 3.0f) + 16.0f * infoscaleSub,

                    //サイズ width hight
                    40.0f * infoscale,
                    40.0f * infoscale,

                    //角度
                    0.0f,

                    //色
                    colorAlphaSetTmp,

                    g_tex_chara_icon_ink,  //画像(テクスチャ)の番号
                    MkF2(0.0f, 0.0f),
                    MkF2(1.0f, 1.0f)
                );
            }
        }
    }
    //プレイヤーのHP
    {
        UI_HPBAR_DAT* hb = UIDat.hpbar + i;

        for (int j = 0; j < 3; j++)
        {
            unsigned int tex = 0;

            Float4 colorTmp = MkF4(1.0f, 1.0f, 1.0f, 1.0f);
            //Float4 colorTmp = colorAlphaSetTmp;

            Float2 pos = {};
            Float2 size = hb->bar.size;
            DPD_SHADER_TYPE_EX_OPT opt = {};
            opt.vs = g_ShaderIndex_UIBgGeneral_Vertex;
            opt.ps = g_ShaderIndex_GageGeneral_Pixel;

            bool isRev = (hb->bar.barMain > hb->bar.barSub);
            //bool isRev = (hb->bar.barBuf > hb->bar.barSub);
            float barmod = 1.0f;

            double shakemodtmp = (double)hb->bar.barChangeFrame / HPBAR_CHANGE_FRAME_MAX;
            double shakextmp = sin(HPBAR_SHAKE_POWER_X * hb->bar.barShakeFrame) * shakemodtmp * HPBAR_SHAKE_X;
            double shakeytmp = -cos(HPBAR_SHAKE_POWER_Y * hb->bar.barShakeFrame) * shakemodtmp * HPBAR_SHAKE_Y;

            colorTmp.w = HPBAR_PLAYER_OVERLAP_ALPHA
                + (1.0f - HPBAR_PLAYER_OVERLAP_ALPHA)
                * (1.0f - (float)hb->bar.playerOverlapRate / UI_RATE_MAX);
            colorTmp.w *= movieFlagAlpha;

            switch (j)
            {
            case 0:
                tex = g_tex_hpbar_bg;
                size = MkF2(HPBARBG_SIZE_X * HPBAR_SCALE, HPBARBG_SIZE_Y * HPBAR_SCALE);

                colorTmp.w = HPBARBG_PLAYER_OVERLAP_ALPHA
                    + (1.0f - HPBARBG_PLAYER_OVERLAP_ALPHA)
                    * (1.0f - (float)hb->bar.playerOverlapRate / UI_RATE_MAX);
                colorTmp.w *= movieFlagAlpha;

                break;
            case 1:
                barmod = (float)(isRev ? hb->bar.barMain : hb->bar.barSub) / UI_RATE_MAX;

                if (isRev)
                {
                    colorTmp = MkF4(0.5f, 2.0f, 0.5f, 1.0f);
                }
                else
                {
                    colorTmp = MkF4(0.7f, 0.7f, 0.7f, 1.0f);
                }
                colorTmp.w *= movieFlagAlpha;

                tex = isSub ? g_tex_hpbarS_sub: g_tex_hpbar_sub;
                pos.x += HPBAR_PAD_X * HPBAR_SCALE;
                pos.y += HPBAR_PAD_Y * HPBAR_SCALE;
                //シェーダーの設定
                opt.vs = g_ShaderIndex_GageGradient_Vertex;
                opt.ps = g_ShaderIndex_HpbarSubGradient_Pixel;
                break;
            case 2:
                barmod = (float)(isRev ? hb->bar.barSub : hb->bar.barMain) / UI_RATE_MAX;

                tex = isSub ? g_tex_hpbarS_main: g_tex_hpbar_main;
                if (hb->danger)
                {
                    if (gframe % 9 < 3)
                    {
                        //colorTmp = MkF4(10.0f, 0.5f, 0.5f, 1.0f);
                        colorTmp = MkF4(0.05f, 0.05f, 0.05f, 1.0f);
                        colorTmp.w *= movieFlagAlpha;
                    }
                }

                if (hb->bar.barChangeFrame > (HPBAR_CHANGE_FRAME_MAX - HPBAR_CHANGE_FLASH_FRAME)
                    && hb->bar.barChangeFrame <= HPBAR_CHANGE_FRAME_MAX
                    && hb->bar.barChangeFrame % HPBAR_CHANGE_FLASH_CYCLE_FRAME < HPBAR_CHANGE_FLASH_CYCLE_FRAME / 2)
                {
                    colorTmp = MkF4(10.0f, 10.0f, 10.0f, 1.0f);
                    colorTmp.w *= movieFlagAlpha;
                }
                pos.x += HPBAR_PAD_X * HPBAR_SCALE + -1.0f;
                pos.y += HPBAR_PAD_Y * HPBAR_SCALE + 1.0f;
                //シェーダーの設定
                opt.vs = g_ShaderIndex_GageGradient_Vertex;
                opt.ps = g_ShaderIndex_HpbarGradient_Pixel;
                break;

            default:
                break;
            }

            barmod = std::max(barmod, barmod <= 0.0f ? 0.0f : 0.05f);

            DrawSpriteInCameraSingle(
                cIndex,
                //表示座標 (四角形の真ん中)
                infoAddPos.x + hb->bar.pos.x
                + (pos.x + ((float)shakextmp) - size.x / 2 * (1.0f - barmod) * infoscale) * subScale.x /*+ (SCREEN_WIDTH / (SCREEN_WIDTH / HPBAR_SIZE_X)) * i*/
                + subPosBar.x - (size.x / 2 * (1.0f - infoscale) * 1.0f 
                    + 180.0f * (1.0f - infoscale)) + 16.0f * infoscaleSub,
                infoAddPos.y + hb->bar.pos.y + (pos.y + ((float)shakeytmp)) * subScale.y
                + subPosBar.y + (size.y / 2 * (1.0f - infoscale) * 0.5f) - 16.0f * infoscaleSub,

                //サイズ width hight
                size.x * barmod * subScale.x * infoscale,
                size.y * subScale.y* infoscale,

                //角度
                0.0f,

                //色
                colorTmp,

                tex,  //画像(テクスチャ)の番号
                MkF2(0.0f, 0.0f),
                MkF2(barmod, 1.0f),
                DRAW_POLYGON_PRIORITY_FORCE_NORMAL,
                DPD_SHADER_TYPE_EX,
                opt
            );
        }
    }

    //プレイヤーのインクゲージ
    if (!isSub)
    {
        UI_INK_BAR_DAT* ib = UIDat.inkbar + i;

        int indBefmax = 2;
        int indmax = indBefmax + (GOE_Character_Player::KAKOPON_INK_GAGE_MEMORY_NUM / 2);
        for (int j = 0; j < indmax; j++)
        {
            unsigned int tex = 0;
            Float4 colorTmp = MkF4(1.0f, 1.0f, 1.0f, 1.0f);
            Float2 pos = {};
            //Float2 pos = ib->bar.pos;
            Float2 size = ib->bar.size;

            bool isRev = (ib->bar.barMain > ib->bar.barSub);
            //bool isRev = (ib->bar.barBuf > ib->bar.barSub);
            float barmod = 1.0f;

            double shakemodtmp = (double)ib->bar.barChangeFrame / HPBAR_CHANGE_FRAME_MAX;
            double shakextmp = sin(HPBAR_SHAKE_POWER_X * ib->bar.barShakeFrame) * shakemodtmp * HPBAR_SHAKE_X;
            double shakeytmp = -cos(HPBAR_SHAKE_POWER_Y * ib->bar.barShakeFrame) * shakemodtmp * HPBAR_SHAKE_Y;

            colorTmp.w = HPBAR_PLAYER_OVERLAP_ALPHA
                + (1.0f - HPBAR_PLAYER_OVERLAP_ALPHA)
                * (1.0f - (float)ib->bar.playerOverlapRate / UI_RATE_MAX);
            colorTmp.w *= movieFlagAlpha;

            switch (j)
            {
            case 0:
                tex = g_tex_ink_bg;
                size = MkF2(INKBARBG_SIZE_X * INKBAR_SCALE, INKBARBG_SIZE_Y * INKBAR_SCALE);

                colorTmp.w = HPBAR_PLAYER_OVERLAP_ALPHA
                    + (1.0f - HPBAR_PLAYER_OVERLAP_ALPHA)
                    * (1.0f - (float)ib->bar.playerOverlapRate / UI_RATE_MAX);
                colorTmp.w *= movieFlagAlpha;
                break;
            case 1:
                barmod = (float)(ib->bar.barMain) / UI_RATE_MAX;

                tex = g_tex_ink_main;
                size = MkF2(INKBAR_SIZE_X * INKBAR_SCALE, INKBAR_SIZE_Y * INKBAR_SCALE);

                //colorTmp = MkF4(0.0f, 0.0f, 0.0f, 1.0f);
                colorTmp.w = HPBAR_PLAYER_OVERLAP_ALPHA
                    + (1.0f - HPBAR_PLAYER_OVERLAP_ALPHA)
                    * (1.0f - (float)ib->bar.playerOverlapRate / UI_RATE_MAX);
                if (isInkbarPinch)
                {
                    colorTmp = { 2.0f, 0.0f, 0.0f, 1.0f };
                }
                colorTmp.w *= movieFlagAlpha;

                pos.x += INKBAR_PAD_X * INKBAR_SCALE + -1.0f;
                pos.y += INKBAR_PAD_Y * INKBAR_SCALE + 1.0f;
                break;
            default:
                break;
            }

            if (j >= indBefmax && j < indmax)
            {
                barmod = 1.0f;
                colorTmp = MkF4(1.0f, 1.0f, 1.0f, 1.0f);
                colorTmp.w *= movieFlagAlpha;
                //pos.y += INKBAR_SIZE_Y * -0.006f;
                tex = g_tex_ink_par;

                //size.x /= 3;
                pos.x += -(size.x / 3.0f) * infoscale;
                size.x /= GOE_Character_Player::KAKOPON_INK_GAGE_MEMORY_NUM;
                size.x += 2;

                //size.y = size.x;
                size.y /= 3;
                //size.y *= infoscale;
                //pos.x += -size.x + size.x * (j - indBefmax);
                pos.x += (size.x * (j - indBefmax) * 2 - (indmax - 1 == j ? size.x : 0)) * infoscale;

                //pos.x += INKBAR_PAD_X * INKBAR_SCALE;
                //pos.y += INKBAR_PAD_Y * INKBAR_SCALE;
                //pos.x += -(size.x / 2 * (1.0f - infoscale) * 1.0
                //    + 180.0f * (1.0f - infoscale));
                pos.x += -(150.0f * (1.0f - infoscale));
                pos.y += +(size.y / 2 * (1.0f - infoscale) * 1.0f);
            }


            barmod = std::max(barmod, barmod <= 0.0f ? 0.0f : 0.05f);

            DrawSpriteInCameraSingle(
                cIndex,
                //表示座標 (四角形の真ん中)
                infoAddPos.x + ib->bar.pos.x + (pos.x + ((float)shakextmp)
                    - size.x / 2 * (1.0f - barmod) * infoscale) * subScale.x
                /*+ (SCREEN_WIDTH / (SCREEN_WIDTH / INKBAR_SIZE_X)) * i*/ 
                + subPosBar.x + subPosBar_Ink.x
                - (size.x / 2 * (1.0f - infoscale) * 1.0f
                    + 180.0f * (1.0f - infoscale)) + 16.0f * infoscaleSub,
                infoAddPos.y + ib->bar.pos.y + (pos.y + ((float)shakeytmp))* subScale.y
                + subPosBar.y + subPosBar_Ink.y
                + (size.y / 2 * (1.0f - infoscale) * 0.5f) + 16.0f * infoscaleSub,

                //サイズ width hight
                size.x * barmod * subScale.x * infoscale,
                size.y * subScale.y * infoscale,

                //角度
                0.0f,

                //色
                colorTmp,

                tex,  //画像(テクスチャ)の番号
                MkF2(0.0f, 0.0f),
                MkF2(barmod, 1.0f)
            );
        }

        if (ib->bar.barMain <= 0)
        {
            DrawSpriteInCameraSingle(
                cIndex,
                //表示座標 (四角形の真ん中)
                infoAddPos.x + ib->bar.pos.x + -68.0f + 322.0f * infoscaleSub,
                infoAddPos.y + ib->bar.pos.y + 12.0f + -64.0f * infoscaleSub,

                //サイズ width hight
                INKBARBG_SIZE_X * INKBAR_SCALE * 0.4f * subScale.x * infoscale,
                INKBARBG_SIZE_Y * INKBAR_SCALE * 0.4f * subScale.y * infoscale,

                //角度
                0.0f,

                //色
                MkF4(
                    colorAlphaSetTmp.x,
                    colorAlphaSetTmp.y,
                    colorAlphaSetTmp.z,
                    colorAlphaSetTmp.w
                ),

                g_tex_head_ink_cross,  //画像(テクスチャ)の番号
                MkF2(0.0f, 0.0f),
                MkF2(1.0f, 1.0f)
            );
        }
    }
}


void UIReset_Game()
{
	short gfef = UIDat.gameFrameEnterFrame;
	UIDat = {};
	UIDat.gameFrameEnterFrame = gfef;
}

void UIMovieFlagInstant_Game()
{
    UIDat.movieFlagFrame = MOVIE_FLAG_FRAME_MAX;
}

bool IsVisibleUI_Game()
{
#if defined(_DEBUG) || defined(DEBUG)
    return g_uiinfoView;
#else
    return true;
#endif
}

void SetUIScoreVisible_Game(bool b)
{
    UIDat.scoreVisible = b;
}

void SetUITimerVisible_Game(bool b)
{
    UIDat.timerVisible = b;
}

void SetUIEventTimer_Game(bool b, const int* reff, int fmax, int digit, bool rev)
{
    UIDat.eventTimer.enable = b;

    UIDat.eventTimer.frame = reff;
    UIDat.eventTimer.frameMax = fmax;

    UIDat.eventTimer.digit = digit;
    UIDat.eventTimer.rev = rev;
}

float GetUIMovieFlagAlpha_Game()
{
    return g_movieFlagAlpha;
}
