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

//#include "gameManager.h"

// ===================================================
// マクロ定義
// ===================================================
#define UI_RATE_MAX							(10000)

#define GAMEFRAME_ENTER_FRAME				(30)

//HPゲージ
#define HPBAR_CHANGE_SPEED					(30)
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
#define BOSSBAR_OFFSET_X					(SCREEN_WIDTH * 0.18f)
#define BOSSBAR_OFFSET_Y					(SCREEN_HEIGHT * -0.44f)

#define BOSSBAR_PAD_X						(0)

#define BOSSBAR_SIZE_X						(900)
#define BOSSBAR_SIZE_Y						(BOSSBAR_SIZE_X)

#define BOSSBARBG_SIZE_X					(BOSSBAR_SIZE_X * 1.01f)
#define BOSSBARBG_SIZE_Y					(BOSSBAR_SIZE_Y * 1.01f)

#define BOSSBAR_SHAKE_X						(6.0f)
#define BOSSBAR_SHAKE_Y						(12.0f)

//ぷれいやーのHPゲージ
#define HPBAR_SCALE							(0.7f)
#define HPBAR_OFFSET_X						(SCREEN_WIDTH * -0.32f)
#define HPBAR_OFFSET_Y						(SCREEN_HEIGHT * 0.48f)
//#define HPBAR_PAD_X							(31)
#define HPBAR_PAD_X							(0)
//#define HPBAR_SIZE_X						(477)
//#define HPBAR_SIZE_Y						(300)
#define HPBAR_SIZE_X						(500)
#define HPBAR_SIZE_Y						(HPBAR_SIZE_X)
//#define HPBARBG_SIZE_X						(800)
#define HPBARBG_SIZE_X						(HPBAR_SIZE_X * 1.01f)
#define HPBARBG_SIZE_Y						(HPBAR_SIZE_Y * 1.01f)
#define HPBAR_SHAKE_X						(5.0f)
#define HPBAR_SHAKE_Y						(6.0f)


//インク
#define INKBAR_SCALE						(0.7f)
#define INKBAR_OFFSET_X						(SCREEN_WIDTH * -0.31f)
#define INKBAR_OFFSET_Y						(SCREEN_HEIGHT * 0.44f)

#define INKBAR_PAD_X						(0)

#define INKBAR_SIZE_X						(500 * 1.075f)
#define INKBAR_SIZE_Y						(HPBAR_SIZE_X)

#define INKBARBG_SIZE_X						(HPBAR_SIZE_X * 1.08f)
#define INKBARBG_SIZE_Y						(HPBAR_SIZE_Y * 1.08f)
#define INKBARBG2_SIZE_X						(HPBAR_SIZE_X * 1.48f)
#define INKBARBG2_SIZE_Y						(HPBAR_SIZE_Y * 1.48f)

#define INKBARBG2_PAD_X						(80)
#define INKBARBG2_PAD_Y						(1)

//すこあ
#define SCORE_OFFSET_X						(SCREEN_WIDTH * -0.32f)
#define SCORE_OFFSET_Y						(SCREEN_HEIGHT * -0.41f)
#define SCORE_SIZE_X						(60)
#define SCORE_SIZE_Y						(60)
#define SCORE_ADD_DAT_MAX					(16)

//ムービー帯
#define MOVIE_FLAG_FRAME_MAX				(60)
// ===================================================
// 構造体宣言
// ===================================================
struct UI_BAR_DAT {
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
struct UI_BOSSBAR_DAT {
	bool enable;
	GAME_OBJECT* obj;
	//BOSSBAR_NAME name;
	std::string name;

	UI_BAR_DAT bar;
};
struct UI_HPBAR_DAT {
	bool enable;

	UI_BAR_DAT bar;
	bool danger;
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

struct UI_DAT {		// UI構造体
	bool gameFrameView;
	short gameFrameEnterFrame;

	UI_HPBAR_DAT hpbar[PLAYER_MAX];
	UI_INK_BAR_DAT inkbar[PLAYER_MAX];

	UI_BOSSBAR_DAT bossbar;
	UI_SCORE_DAT score;

    short movieFlagFrame;
};

// ===================================================
// プロトタイプ宣言
// ===================================================
static void barUpdate_Game(UI_BAR_DAT* bar, int current);

// ===================================================
// グローバル変数
// ===================================================
static UI_DAT UIDat = {};

static TEX_DAT g_tex_icon_player = {};
static TEX_DAT g_tex_icon_enemy_small = {};
static TEX_DAT g_tex_icon_enemy_midium = {};
static TEX_DAT g_tex_icon_enemy_boss = {};

static unsigned int g_tex_bossbar_hi = {};
static unsigned int g_tex_bossbar_main = {};
static unsigned int g_tex_bossbar_sub = {};
static unsigned int g_tex_bossbar_bg = {};
static unsigned int g_tex_bossbar_icon = {};

static unsigned int g_tex_hpbar_hi = {};
static unsigned int g_tex_hpbar_main = {};
static unsigned int g_tex_hpbar_sub = {};
static unsigned int g_tex_hpbar_bg = {};

static unsigned int g_tex_ph_charactor_icon = {};
static unsigned int g_tex_ph_MAP = {};

static unsigned int g_tex_hude_base = {};
static unsigned int g_tex_ink_base = {};
static unsigned int g_tex_ink_color = {};
static unsigned int g_tex_ink_hightlight_a = {};
static unsigned int g_tex_ink_separate = {};

static unsigned int g_tex_timer = {};


void InitUI_Game()
{
	UIDat = {};

	g_tex_icon_player = GetTexData(TEX_TYPE_UI_ICON_PLAYER);
	g_tex_icon_enemy_small = GetTexData(TEX_TYPE_UI_ICON_ENEMY_SMALL);
	g_tex_icon_enemy_midium = GetTexData(TEX_TYPE_UI_ICON_ENEMY_MIDIUM);
	g_tex_icon_enemy_boss = GetTexData(TEX_TYPE_UI_ICON_ENEMY_BOSS);


	g_tex_bossbar_hi	= LoadTexData("TEX/ui/in_game/HPboss_hightlight_a").textureId;
	g_tex_bossbar_main	= LoadTexData("TEX/ui/in_game/HPboss_red").textureId;
	g_tex_bossbar_sub	= LoadTexData("TEX/ui/in_game/HPboss_yellow").textureId;
	g_tex_bossbar_bg	= LoadTexData("TEX/ui/in_game/HPboss_base").textureId;
	g_tex_bossbar_icon	= LoadTexData("TEX/ui/in_game/boss_icon").textureId;

	g_tex_hpbar_hi		= LoadTexData("TEX/ui/in_game/HPboss_hightlight_a").textureId;
	g_tex_hpbar_main	= LoadTexData("TEX/ui/in_game/HPboss_red").textureId;
	g_tex_hpbar_sub		= LoadTexData("TEX/ui/in_game/HPboss_yellow").textureId;
	g_tex_hpbar_bg = LoadTexData("TEX/ui/in_game/HPboss_base").textureId;

	g_tex_ph_charactor_icon = LoadTexData("TEX/ui/in_game/charactor_icon").textureId;
	g_tex_ph_MAP = LoadTexData("TEX/ui/in_game/MAP").textureId;

	g_tex_hude_base			= LoadTexData("TEX/ui/in_game/hude_base").textureId;
	g_tex_ink_base			= LoadTexData("TEX/ui/in_game/ink_base").textureId;
	g_tex_ink_color			= LoadTexData("TEX/ui/in_game/ink_color").textureId;
	g_tex_ink_hightlight_a  = LoadTexData("TEX/ui/in_game/ink_hightlight_a").textureId;
	g_tex_ink_separate      = LoadTexData("TEX/ui/in_game/ink_separate").textureId;

	g_tex_timer = LoadTexData("TEX/ui/in_game/timer").textureId;

}

void UninitUI_Game()
{
	UnloadTexData(g_tex_timer);

	UnloadTexData(g_tex_hude_base);
	UnloadTexData(g_tex_ink_base);
	UnloadTexData(g_tex_ink_color);
	UnloadTexData(g_tex_ink_hightlight_a);
	UnloadTexData(g_tex_ink_separate);

	UnloadTexData(g_tex_ph_MAP);
	UnloadTexData(g_tex_ph_charactor_icon);

	UnloadTexData(g_tex_bossbar_hi);
	UnloadTexData(g_tex_bossbar_main);
	UnloadTexData(g_tex_bossbar_sub);
	UnloadTexData(g_tex_bossbar_bg);
	UnloadTexData(g_tex_bossbar_icon);

	UnloadTexData(g_tex_hpbar_hi);
	UnloadTexData(g_tex_hpbar_main);
	UnloadTexData(g_tex_hpbar_sub);
	UnloadTexData(g_tex_hpbar_bg);
}

void UpdateUI_Game()
{
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

			int current = (int)(((double)bb->obj->status.hp / bb->obj->status.hpMax) * UI_RATE_MAX);

			barUpdate_Game(&bb->bar, current);
		}
	}

	for (int i = 0; i < PLAYER_MAX; i++)
	{
		if (UIDat.hpbar[i].enable)
		{
			PLAYER* player = GetPlayer(i);

			if (player->obj != nullptr)
			{
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
                    Float2 ink = ((GOE_Character_Player*)player->obj->events.source)->GetKakoponVertexNum();
					int current = (int)((1.0 - (double)ink.x / ink.y) * UI_RATE_MAX);

					barUpdate_Game(&ib->bar, current);
					//DebugPrintf("%d   %d   %d\n", hb->barMain, hb->barSub, hb->barBuf);
				}
			}
		}
	}

}

void DrawUI_Game()
{
	unsigned int gframe = GetMainGameFrame();
	Float4 colorDefaultTmp = MkF4(1.0f, 1.0f, 1.0f, 1.0f);

    float movieFlagAlpha = 1.0f;
    {
        float smod = std::max(
            std::min(
                (float)UIDat.movieFlagFrame / MOVIE_FLAG_FRAME_MAX, 
                1.0f),
            0.0f);
        movieFlagAlpha = 1.0f - smod;

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
    }

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

            DrawSpriteInCamera(
				//表示座標 (四角形の真ん中)
				SCREEN_WIDTH / 2 - 100.0f,
				pos2.y + 25.0f,

				//サイズ width hight
				100.0f,
				100.0f,

				//角度
				0.0f,

				//色
				colorDefaultTmp,

				g_tex_bossbar_icon,  //画像(テクスチャ)の番号
				MkF2(0.0f, 0.0f),
				MkF2(1.0f, 1.0f)
			);
			for (int i = 0; i < 4; i++)
			{
				bool hidden = false;
				unsigned int tex = 0;
				float txpadx = 0.0005f;
				Float4 colorTmp = MkF4(1.0f, 1.0f, 1.0f, 1.0f);
				Float2 posTmp = pos;
				Float2 sizeTmp = size;

				colorTmp.w = HPBAR_PLAYER_OVERLAP_ALPHA
					+ (1.0f - HPBAR_PLAYER_OVERLAP_ALPHA)
					* (1.0f - (float)bb->bar.playerOverlapRate / UI_RATE_MAX);

				switch (i)
				{
				case 0:
					tex = g_tex_bossbar_bg;
					sizeTmp = MkF2(BOSSBARBG_SIZE_X, BOSSBARBG_SIZE_Y);

					colorTmp.w = HPBARBG_PLAYER_OVERLAP_ALPHA
						+ (1.0f - HPBARBG_PLAYER_OVERLAP_ALPHA)
						* (1.0f - (float)bb->bar.playerOverlapRate / UI_RATE_MAX);

					break;
				case 1:
					barmod = (float)(isRev ? bb->bar.barMain : bb->bar.barSub) / UI_RATE_MAX;
					tex = g_tex_bossbar_sub;
					posTmp.x += BOSSBAR_PAD_X;
					break;
				case 2:
				case 3:
					tex = g_tex_bossbar_main;
					barmod = (float)(isRev ? bb->bar.barSub : bb->bar.barMain) / UI_RATE_MAX;
					if (bb->bar.barChangeFrame > (HPBAR_CHANGE_FRAME_MAX - HPBAR_CHANGE_FLASH_FRAME)
						&& bb->bar.barChangeFrame <= HPBAR_CHANGE_FRAME_MAX
						&& bb->bar.barChangeFrame % HPBAR_CHANGE_FLASH_CYCLE_FRAME < HPBAR_CHANGE_FLASH_CYCLE_FRAME / 2)
					{
						colorTmp = MkF4(100.0f, 100.0f, 100.0f, 1.0f);
					}

					if (i == 3)
					{
						colorTmp = MkF4(1.0f, 1.0f, 1.0f, 1.0f);
						tex = g_tex_bossbar_hi;
						posTmp.y += BOSSBAR_SIZE_Y * -0.006f;
					}
					posTmp.x += BOSSBAR_PAD_X;
					break;
				default:
					break;
				}

				if (hidden)
				{
					continue;
				}

                DrawSpriteInCamera(
					//表示座標 (四角形の真ん中)
					posTmp.x - sizeTmp.x / 2 * (1.0f - barmod) + (float)shakextmp,
					posTmp.y + (float)shakeytmp,

					//サイズ width hight
					sizeTmp.x * barmod * barentermod,
					sizeTmp.y,

					//角度
					0.0f,

					//色
					colorTmp,

					tex,  //画像(テクスチャ)の番号
					MkF2(0.0f + txpadx, 0.0f),
					MkF2(barmod * (1.0f - txpadx), 1.0f)
				);
			}

			for (int i = 0; i < 1; i++)
			{
				Float2 posTmp = MkF2(pos2.x + size.x / 2 - 150.0f, pos2.y + 4.0f );
				Float4 colorTmp = MkF4(0.9f, 0.8f, 1.0f, 1.0f);
			/*	if (i == 0)
				{
					posTmp.x += -8.0f;
					posTmp.y += 8.0f;
					colorTmp = MkF4(0.1f, 0.1f, 0.1f, 0.75f);
				}*/
				{
					DRAW_FONT_DAT dat = {};
					dat.txt = StringToWString(bb->name);
					dat.fonttype = FONT_TYPE_DEFAULT;
					dat.pos = posTmp;
					dat.fontSize = 30;
					dat.color = colorTmp;
					dat.textAlign = FONT_TEXT_ALIGNMENT_RIGHT;
					dat.shadow = true;
					dat.shadowColor = MkF4(0.1f, 0.1f, 0.1f, 0.75f);
					dat.shadowPos = MkF2(-8.0f, -8.0f);

					DrawFont(dat);
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
				//アイコん
				{
                    DrawSpriteInCamera(
						//表示座標 (四角形の真ん中)
						-SCREEN_WIDTH / 2 + 80.0f,
						SCREEN_HEIGHT / 2 - 80.0f,

						//サイズ width hight
						120.0f,
						120.0f,

						//角度
						0.0f,

						//色
                        colorAlphaSetTmp,

						g_tex_ph_charactor_icon,  //画像(テクスチャ)の番号
						MkF2(0.0f, 0.0f),
						MkF2(1.0f, 1.0f)
					);
				}

			}

			//プレイヤーのHP
			{
				UI_HPBAR_DAT* hb = UIDat.hpbar + i;

				for (int j = 0; j < 4; j++)
				{
					unsigned int tex = 0;

                    Float4 colorTmp = MkF4(1.0f, 1.0f, 1.0f, 1.0f);
                    //Float4 colorTmp = colorAlphaSetTmp;
                    
					Float2 pos = hb->bar.pos;
					Float2 size = hb->bar.size;

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
                            colorTmp = MkF4(0.0f, 2.0f, 1.0f, 1.0f);
                            colorTmp.w *= movieFlagAlpha;
                        }
						tex = g_tex_hpbar_sub;
						pos.x += HPBAR_PAD_X * HPBAR_SCALE;
						break;
					case 2:
					case 3:
						barmod = (float)(isRev ? hb->bar.barSub : hb->bar.barMain) / UI_RATE_MAX;

						tex = g_tex_hpbar_main;
						if (hb->danger)
						{
							if (gframe % 9 < 3)
							{
								tex = g_tex_hpbar_main;
								colorTmp = MkF4(10.0f, 0.5f, 0.5f, 1.0f);
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
						if (i == 3)
						{
							colorTmp = MkF4(1.0f, 1.0f, 1.0f, 1.0f);
                            colorTmp.w *= movieFlagAlpha;
							pos.y += HPBAR_SIZE_Y * -0.006f;
							tex = g_tex_hpbar_hi;
						}
						pos.x += HPBAR_PAD_X * HPBAR_SCALE;
						break;

					default:
						break;
					}

                    DrawSpriteInCamera(
						//表示座標 (四角形の真ん中)
						pos.x + ((float)shakextmp) - size.x / 2 * (1.0f - barmod) + (SCREEN_WIDTH / (SCREEN_WIDTH / HPBAR_SIZE_X)) * i,
						pos.y + ((float)shakeytmp),

						//サイズ width hight
						size.x * barmod,
						size.y,

						//角度
						0.0f,

						//色
						colorTmp,

						tex,  //画像(テクスチャ)の番号
						MkF2(0.0f, 0.0f),
						MkF2(barmod, 1.0f)
					);
				}
			}

			//プレイヤーのインクゲージ
			{
				UI_INK_BAR_DAT* ib = UIDat.inkbar + i;

				for (int j = 0; j < 6; j++)
				{
					unsigned int tex = 0;
					Float4 colorTmp = MkF4(1.0f, 1.0f, 1.0f, 1.0f);
					Float2 pos = ib->bar.pos;
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
						tex = g_tex_hude_base;
						pos.x += INKBARBG2_PAD_X * INKBAR_SCALE;
						pos.y += INKBARBG2_PAD_Y * INKBAR_SCALE;
						size = MkF2(INKBARBG2_SIZE_X * INKBAR_SCALE, INKBARBG2_SIZE_Y * INKBAR_SCALE);

						colorTmp.w = HPBAR_PLAYER_OVERLAP_ALPHA
							+ (1.0f - HPBAR_PLAYER_OVERLAP_ALPHA)
							* (1.0f - (float)ib->bar.playerOverlapRate / UI_RATE_MAX);
                        colorTmp.w *= movieFlagAlpha;

						break;
					case 1:
						tex = g_tex_ink_base;
						size = MkF2(INKBARBG_SIZE_X * INKBAR_SCALE, INKBARBG_SIZE_Y * INKBAR_SCALE);

						colorTmp.w = HPBAR_PLAYER_OVERLAP_ALPHA
							+ (1.0f - HPBAR_PLAYER_OVERLAP_ALPHA)
							* (1.0f - (float)ib->bar.playerOverlapRate / UI_RATE_MAX);
                        colorTmp.w *= movieFlagAlpha;

						break;
					case 2:
						colorTmp = MkF4(0.6f, 0.0f, 0.0f, 1.0f);
                        colorTmp.w *= movieFlagAlpha;

						barmod = (float)(isRev ? ib->bar.barMain : ib->bar.barSub) / UI_RATE_MAX;
						tex = g_tex_ink_color;
						pos.x += INKBAR_PAD_X * INKBAR_SCALE;
						break;
					case 3:
					case 4:
					case 5:
						barmod = (float)(isRev ? ib->bar.barSub : ib->bar.barMain) / UI_RATE_MAX;

						tex = g_tex_ink_color;

						if (ib->bar.barChangeFrame > (HPBAR_CHANGE_FRAME_MAX - HPBAR_CHANGE_FLASH_FRAME)
							&& ib->bar.barChangeFrame <= HPBAR_CHANGE_FRAME_MAX
							&& ib->bar.barChangeFrame % HPBAR_CHANGE_FLASH_CYCLE_FRAME < HPBAR_CHANGE_FLASH_CYCLE_FRAME / 2)
						{
							colorTmp = MkF4(10.0f, 10.0f, 10.0f, 1.0f);
                            colorTmp.w *= movieFlagAlpha;
						}

						if (i == 4)
						{
							colorTmp = MkF4(1.0f, 1.0f, 1.0f, 1.0f);
                            colorTmp.w *= movieFlagAlpha;
							pos.y += INKBAR_SIZE_Y * -0.006f;
							tex = g_tex_ink_hightlight_a;
						}
						if (i == 5)
						{
							colorTmp = MkF4(1.0f, 1.0f, 1.0f, 1.0f);
                            colorTmp.w *= movieFlagAlpha;
							//pos.y += INKBAR_SIZE_Y * -0.006f;
							tex = g_tex_ink_separate;
						}

						pos.x += INKBAR_PAD_X * INKBAR_SCALE;
						break;

					default:
						break;
					}

                    DrawSpriteInCamera(
						//表示座標 (四角形の真ん中)
						pos.x + ((float)shakextmp) - size.x / 2 * (1.0f - barmod) + (SCREEN_WIDTH / (SCREEN_WIDTH / INKBAR_SIZE_X)) * i,
						pos.y + ((float)shakeytmp),

						//サイズ width hight
						size.x * barmod,
						size.y,

						//角度
						0.0f,

						//色
						colorTmp,

						tex,  //画像(テクスチャ)の番号
						MkF2(0.0f, 0.0f),
						MkF2(barmod, 1.0f)
					);
				}
			}
		}
	}


	//タイマー
	{
        DrawSpriteInCamera(
			//表示座標 (四角形の真ん中)
			-SCREEN_WIDTH / 2 + 150.0f,
			-SCREEN_HEIGHT / 2 + 120.0f,

			//サイズ width hight
			240.0f,
			240.0f,

			//角度
			0.0f,

			//色
			colorAlphaSetTmp,

			g_tex_timer,  //画像(テクスチャ)の番号
			MkF2(0.0f, 0.0f),
			MkF2(1.0f, 1.0f)
		);


		Float4 colorTmp = MkF4(0.2f, 0.2f, 0.0f, movieFlagAlpha);
		SpriteNumberDraw(
			(int)GetStageCountFrameSceneGame() / 60,
			MkF2(
				/*	0.0f,
					0.0f*/
				-SCREEN_WIDTH / 2 + 170.0f,
				-SCREEN_HEIGHT / 2 + 150.0f
			),
			MkF2(
				80.0f,
				80.0f
			),
			-1,
			colorTmp,
			-30
		);
	}

	//マップ
	{
		constexpr int MAP_X = 144;
		constexpr int MAP_Y = 81;

		constexpr float posX = (float)SCREEN_WIDTH * -0.38f;
		constexpr float posY = (float)SCREEN_HEIGHT * -0.15f;
		constexpr float scale = 0.03f;

        DRAW_POLYGON_PRIORITY dppTmp = DRAW_POLYGON_PRIORITY_FORCE_NORMAL;
        //DRAW_POLYGON_PRIORITY dppTmp = DRAW_POLYGON_PRIORITY_ALWAYS;
		{
			Float4 color = MkF4(1.0f, 1.0f, 1.0f, 0.25f * movieFlagAlpha);
			Float4 color2 = MkF4(1.0f, 1.0f, 1.0f, 1.0f * movieFlagAlpha);
            DrawSpriteInCamera(
				//表示座標 (四角形の真ん中)
				posX,
				posY,

				//サイズ width hight
				(float)(MAP_X* BLOCK_WIDTH* scale * 1.075f),
				(float)(MAP_X* BLOCK_WIDTH* scale * 1.075f),

				//角度
				0.0f,

				//色
				color2,

				g_tex_ph_MAP,  //画像(テクスチャ)の番号
				MkF2(0.0f, 0.0f),
				MkF2(1.0f, 1.0f),
                dppTmp
			);
            DrawSpriteInCamera(
				//表示座標 (四角形の真ん中)
				posX,
				posY,

				//サイズ width hight
				(float)(MAP_X* BLOCK_WIDTH* scale),
				(float)(MAP_Y* BLOCK_HIGHT* scale),

				//角度
				0.0f,

				//色
				color,

				0,  //画像(テクスチャ)の番号
				MkF2(0.0f, 0.0f),
				MkF2(1.0f, 1.0f),
                dppTmp
			);
		}

		for (int i = 0; i < BLOCK_MAX; i++)
		{
			BLOCK* b = GetBlocks() + i;
			if (b->isUse && !b->noCollision && b->sprite.isUse)
			{
				float posXtmp = posX + b->pos.x * scale;
				float posYtmp = posY + b->pos.y * scale;
				float scaletmp = 1.0f;

				{
					Float4 color = MkF4(1.0f, 1.0f, 1.0f, 0.5f);
                    color.w *= movieFlagAlpha;
                    DrawSpriteInCamera(
						//表示座標 (四角形の真ん中)
						posXtmp,
						posYtmp,

						//サイズ width hight
						(float)(b->size.x * scale),
						(float)(b->size.y * scale),

						//角度
						0.0f,

						//色
						color,

						0,  //画像(テクスチャ)の番号
						MkF2(0.0f, 0.0f),
						MkF2(1.0f, 1.0f),
                        dppTmp
					);
				}
			}
		}

		for (int i = 0; i < GAME_OBJECT_MAX; i++)
		{
			GAME_OBJECT* ob = GetGameObject(i);
			if (IsActiveGameObject(ob)
				&& ob->status.statusType != GAME_OBJECT_STATUS_TYPE_VOID
				&& ob->type == GAME_OBJECT_TYPE_CHARACTER)
			{
				float posXtmp = posX + ob->pos.x * scale;
				float posYtmp = posY + ob->pos.y * scale;
				float scaletmp = 0.0f;

				unsigned int textmp = 0;
				switch (ob->status.statusType)
				{
				case GAME_OBJECT_STATUS_TYPE_PLAYER:
					scaletmp = 0.3f;
					textmp = g_tex_icon_player.textureId;
					break;

				case GAME_OBJECT_STATUS_TYPE_ENEMY_SMALL:
					scaletmp = 0.3f;
					textmp = g_tex_icon_enemy_small.textureId;
					break;

				case GAME_OBJECT_STATUS_TYPE_ENEMY_MIDIUM:
					scaletmp = 0.4f;
					textmp = g_tex_icon_enemy_midium.textureId;
					break;

				case GAME_OBJECT_STATUS_TYPE_ENEMY_BOSS:
					scaletmp = 0.6f;
					textmp = g_tex_icon_enemy_boss.textureId;
					break;

				default:break;
				}
				{
					Float4 color = MkF4(1.0f, 1.0f, 1.0f, 0.5f);
                    color.w *= movieFlagAlpha;
                    DrawSpriteInCamera(
						//表示座標 (四角形の真ん中)
						posXtmp,
						posYtmp,

						//サイズ width hight
						(float)(BLOCK_WIDTH * scaletmp),
						(float)(BLOCK_HIGHT * scaletmp),

						//角度
						0.0f,

						//色
						color,

						textmp,  //画像(テクスチャ)の番号
						MkF2(0.0f, 0.0f),
						MkF2(1.0f, 1.0f),
                        dppTmp
					);
				}
			}
		}
	}
}


bool ShowHpbar_Game(int index, bool show)
{
	UIDat.hpbar[index] = {};
	UIDat.hpbar[index].enable = show;

	return show;
}

bool ShowBossbar_Game(int index, bool show, std::string name)
//bool ShowBossbar(int index, bool show, BOSSBAR_NAME name)
{
	GAME_OBJECT* obj = GetGameObject(index);
	UIDat.bossbar = {};
	UIDat.bossbar.enable = show;
	UIDat.bossbar.obj = obj;
	UIDat.bossbar.name = name;

	return show;
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


void UIReset_Game()
{
	short gfef = UIDat.gameFrameEnterFrame;
	UIDat = {};
	UIDat.gameFrameEnterFrame = gfef;
}