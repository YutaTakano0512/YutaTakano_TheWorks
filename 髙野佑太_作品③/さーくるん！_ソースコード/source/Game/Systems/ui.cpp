// ===================================================
// ui.cpp ui制御
// 
// hara sougo  2024/08/21
// ===================================================
#include "../Configuration/configuration.h"
#include "../../game_main.h"
#include "easing.h"
#include "../Texture/texData.h"
#include "../Sprite/sprite.h"
#include "ui.h"

#include "collision.h"

#include "../Entities/Block/block.h"
#include "../Game Object/gameObject.h"
#include "../Entities/player.h"

#include "fade.h"
#include "camera.h"
#include "../Managers/scene.h"
//#include "gameManager.h"

// ===================================================
// マクロ定義
// ===================================================
#define UI_RATE_MAX							(10000)

#define GAMEFRAME_ENTER_FRAME				(30)

//#define HPBAR_CHANGE_SPEED					(30) //ゲージ減る速度
#define HPBAR_CHANGE_SPEED					(100) //ゲージ減る速度

#define HPBAR_CHANGE_FRAME_MAX				(30)
#define HPBAR_CHANGE_FLASH_FRAME			(20)
#define HPBAR_CHANGE_FLASH_CYCLE_FRAME		(6)
#define HPBAR_WAIT_FRAME					(50)
#define HPBAR_ENTER_SPEED					(80)
#define HPBAR_ENTER_FRAME_MAX				(20)
#define HPBAR_SHAKE_POWER_X					(8.0f)
#define HPBAR_SHAKE_POWER_Y					(10.0f)

#define HPBAR_PLAYER_OVERLAP_RATE_SPEED		(900)
#define HPBARBG_PLAYER_OVERLAP_ALPHA		(0.25f)
#define HPBAR_PLAYER_OVERLAP_ALPHA			(0.6f)

#define BOSSBAR_OFFSET_X					(SCREEN_WIDTH * 0.22f)
#define BOSSBAR_OFFSET_Y					(SCREEN_HEIGHT * -0.42f)
#define BOSSBAR_PAD_X						(-19)
#define BOSSBAR_SIZE_X						(814)
#define BOSSBAR_SIZE_Y						(215)
#define BOSSBARBG_SIZE_X					(1028)
#define BOSSBARBG_SIZE_Y					(BOSSBAR_SIZE_Y)
#define BOSSBAR_SHAKE_X						(6.0f)
#define BOSSBAR_SHAKE_Y						(12.0f)

#define HPBAR_SCALE							(0.9f)
#define HPBAR_P_PAD_KARI					(SCREEN_WIDTH / 2)
//#define HPBAR_OFFSET_X						(SCREEN_WIDTH * -0.4f)
//#define HPBAR_OFFSET_Y						(SCREEN_HEIGHT * 0.45f)
#define HPBAR_OFFSET_X						(SCREEN_WIDTH * -0.25f)
#define HPBAR_OFFSET_Y						(SCREEN_HEIGHT * -0.4f)
#define HPBAR_PAD_X							(31)
#define HPBAR_SIZE_X						(477)
#define HPBAR_SIZE_Y						(300)
#define HPBARBG_SIZE_X						(800)
#define HPBARBG_SIZE_Y						(HPBAR_SIZE_Y)
#define HPBAR_SHAKE_X						(5.0f)
#define HPBAR_SHAKE_Y						(6.0f)
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
	BOSSBAR_NAME name;

	UI_BAR_DAT bar;
};
struct UI_HPBAR_DAT {
	bool enable;

	UI_BAR_DAT bar;
	bool danger;
};
struct UI_DAT {		// UI構造体
	bool gameFrameView;
	short gameFrameEnterFrame;

	UI_HPBAR_DAT hpbar[CONTROLLER_DEVICE_MAX];
	UI_BOSSBAR_DAT bossbar;
};

// ===================================================
// プロトタイプ宣言
// ===================================================
void barUpdate(UI_BAR_DAT* bar, int current);

// ===================================================
// グローバル変数
// ===================================================
static UI_DAT UIDat = {};

static TEX_DAT g_tex_bossbar_main = {};
static TEX_DAT g_tex_bossbar_sub = {};
static TEX_DAT g_tex_bossbar_bg = {};
static TEX_DAT g_tex_bossbar_name_troller01 = {};
static TEX_DAT g_tex_bossbar_name_troller06 = {};
static TEX_DAT g_tex_bossbar_name_troller17 = {};

static TEX_DAT g_tex_hpbar_main = {};
static TEX_DAT g_tex_hpbar_sub = {};
static TEX_DAT g_tex_hpbar_bg = {};
static TEX_DAT g_tex_hpbar_text = {};

static TEX_DAT g_tex_gameframe = {};


void InitUI()
{
	UIDat = {};
	//g_tex_bossbar_main = GetTexData(TEX_TYPE_UI_BOSSBAR_MAIN);
	//g_tex_bossbar_sub = GetTexData(TEX_TYPE_UI_BOSSBAR_SUB);
	//g_tex_bossbar_bg = GetTexData(TEX_TYPE_UI_BOSSBAR_BG);
	//g_tex_bossbar_name_troller01 = GetTexData(TEX_TYPE_UI_BOSSBAR_NAME_TROLLER01);
	//g_tex_bossbar_name_troller06 = GetTexData(TEX_TYPE_UI_BOSSBAR_NAME_TROLLER06);
	//g_tex_bossbar_name_troller17 = GetTexData(TEX_TYPE_UI_BOSSBAR_NAME_TROLLER17);

	g_tex_hpbar_main = GetTexData(TEX_TYPE_UI_HPBAR_MAIN);
	g_tex_hpbar_sub = GetTexData(TEX_TYPE_UI_HPBAR_SUB);
	g_tex_hpbar_bg = GetTexData(TEX_TYPE_UI_HPBAR_BG);
	g_tex_hpbar_text = GetTexData(TEX_TYPE_UI_HPBAR_TEXT);

	//g_tex_gameframe = GetTexData(TEX_TYPE_UI_GAME_FRAME);
}

void UninitUI()
{
}

void UpdateUI()
{
	if (UIDat.gameFrameEnterFrame > 0)
	{
		UIDat.gameFrameEnterFrame--;
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

			barUpdate(&bb->bar, current);
		}
	}

	//プレイヤーのHP
	for (int i = 0; i < PLAYER_MAX; i++)
	{
		if (UIDat.hpbar[i].enable)
		{
			PLAYER* player = GetPlayer(i);

			if (player->obj != nullptr)
			{
				UI_HPBAR_DAT* hb = UIDat.hpbar + i;

				hb->bar.pos = MkF2(HPBAR_OFFSET_X, HPBAR_OFFSET_Y);
				hb->bar.size = MkF2(HPBAR_SIZE_X * HPBAR_SCALE, HPBAR_SIZE_Y * HPBAR_SCALE);

				int current = (int)(((double)player->obj->status.hp / player->obj->status.hpMax) * UI_RATE_MAX);

				//hb->danger = (player->stateType == PLAYER_STATE_TYPE_DANGER);

				barUpdate(&hb->bar, current);
				//DebugPrintf("%d   %d   %d\n", hb->barMain, hb->barSub, hb->barBuf);
			}
		}
	}


	////プレイヤーのHP
	//{
	//	int indexPlayer = 0;
	//	for (int i = 0; i < GAME_OBJECT_MAX; i++)
	//	{

	//		if (UIDat.hpbar[indexPlayer].enable)
	//		{
	//			GAME_OBJECT* objc = GetGameObject(i);

	//			if (objc != nullptr
	//				&& objc->type == GAME_OBJECT_TYPE_CHARACTER)
	//			{
	//				UI_HPBAR_DAT* hb = UIDat.hpbar + indexPlayer;

	//				hb->bar.pos = MkF2(HPBAR_OFFSET_X, HPBAR_OFFSET_Y);
	//				hb->bar.size = MkF2(HPBAR_SIZE_X * HPBAR_SCALE, HPBAR_SIZE_Y * HPBAR_SCALE);

	//				int current = (int)(((double)objc->status.hp / objc->status.hpMax) * UI_RATE_MAX);

	//				barUpdate(&hb->bar, current);

	//				indexPlayer++;
	//			}
	//		}
	//	}
	//}
}

void DrawUI()
{
	unsigned int gframe = GetMainGameFrame();
	Float4 colorDefaultTmp = MkF4(1.0f, 1.0f, 1.0f, 1.0f);

	if(UIDat.gameFrameView)
	{
		Float4 coltmp = MkF4(1.0f, 1.0f, 1.0f, 1.0f);
		if (UIDat.gameFrameEnterFrame > 0)
		{
			coltmp.w = (1.0f - ((float)UIDat.gameFrameEnterFrame / GAMEFRAME_ENTER_FRAME));
		}

		DrawSpriteQuadColorRotateTexUv(
			//表示座標 (四角形の真ん中)
			-1.0f,
			-1.0f,

			//サイズ width hight
			SCREEN_WIDTH + 2.0f,
			SCREEN_HEIGHT + 2.0f,

			//角度
			0.0f,

			//色
			coltmp,

			g_tex_gameframe.textureId,  //画像(テクスチャ)の番号
			MkF2(0.0f, 0.0f),
			MkF2(1.0f, 1.0f),
			DRAW_POLYGON_PRIORITY_ALWAYS
		);
	}



	//ボスのHP
	if (UIDat.bossbar.enable)
	{
		UI_BOSSBAR_DAT* bb = &UIDat.bossbar;
		if (bb->obj != nullptr)
		{
			for (int i = 0; i < 4; i++)
			{
				unsigned int tex = 0;
				float txpadx = 0.0005f;
				Float4 colorTmp = MkF4(1.0f, 1.0f, 1.0f, 1.0f);
				Float2 pos = bb->bar.pos;
				Float2 size = bb->bar.size;

				bool isRev = (bb->bar.barBuf > bb->bar.barSub);
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

				colorTmp.w = HPBAR_PLAYER_OVERLAP_ALPHA
					+ (1.0f - HPBAR_PLAYER_OVERLAP_ALPHA)
					* (1.0f - (float)bb->bar.playerOverlapRate / UI_RATE_MAX);

				switch (i)
				{
				case 0:
					tex = g_tex_bossbar_bg.textureId;
					size = MkF2(BOSSBARBG_SIZE_X, BOSSBARBG_SIZE_Y);

					colorTmp.w = HPBARBG_PLAYER_OVERLAP_ALPHA
						+ (1.0f - HPBARBG_PLAYER_OVERLAP_ALPHA)
						* (1.0f - (float)bb->bar.playerOverlapRate / UI_RATE_MAX);

					break;
				case 1:
					barmod = (float)(isRev ? bb->bar.barMain : bb->bar.barSub) / UI_RATE_MAX;
					tex = g_tex_bossbar_sub.textureId;
					pos.x += BOSSBAR_PAD_X;
					break;
				case 2:
					barmod = (float)(isRev ? bb->bar.barSub : bb->bar.barMain) / UI_RATE_MAX;
					if (bb->bar.barChangeFrame > (HPBAR_CHANGE_FRAME_MAX - HPBAR_CHANGE_FLASH_FRAME)
						&& bb->bar.barChangeFrame <= HPBAR_CHANGE_FRAME_MAX
						&& bb->bar.barChangeFrame % HPBAR_CHANGE_FLASH_CYCLE_FRAME < HPBAR_CHANGE_FLASH_CYCLE_FRAME / 2)
					{
						//tex = g_tex_bossbar_main.textureIdGray;
						tex = g_tex_bossbar_main.textureId;
						colorTmp = MkF4(100.0f, 100.0f, 100.0f, 1.0f);
					}
					else
					{
						tex = g_tex_bossbar_main.textureId;
					}
					pos.x += BOSSBAR_PAD_X;
					break;
				case 3:
				{
					//switch (bb->name)
					//{
					//default:
					//	break;
					//}
					break;
				}

					size = MkF2(BOSSBARBG_SIZE_X, BOSSBARBG_SIZE_Y);

					/*colorTmp.w = HPBARBG_PLAYER_OVERLAP_ALPHA
						+ (1.0f - HPBARBG_PLAYER_OVERLAP_ALPHA)
						* (1.0f - (float)bb->bar.playerOverlapRate / UI_RATE_MAX);*/

					break;
				default:
					break;
				}

				DrawSpriteQuadColorRotateTexUv(
					//表示座標 (四角形の真ん中)
					pos.x - size.x / 2 * (1.0f - barmod) + (float)shakextmp,
					pos.y + (float)shakeytmp,

					//サイズ width hight
					size.x * barmod * barentermod,
					size.y,

					//角度
					0.0f,

					//色
					colorTmp,

					tex,  //画像(テクスチャ)の番号
					MkF2(0.0f + txpadx, 0.0f),
					MkF2(barmod * (1.0f - txpadx), 1.0f),
					DRAW_POLYGON_PRIORITY_ALWAYS
				);
			}
		}
	}

	//プレイヤーのHP
	for (int i = 0; i < PLAYER_MAX; i++)
	{
		if (UIDat.hpbar[i].enable)
		{
			UI_HPBAR_DAT* hb = UIDat.hpbar + i;

			for (int j = 0; j < 3 /* 4 */; j++)
			{
				unsigned int tex = 0;
				Float4 colorTmp = MkF4(1.0f, 1.0f, 1.0f, 1.0f);
				Float2 pos = hb->bar.pos;
				Float2 size = hb->bar.size;

				float reverse = (float)(i % 2 == 0 ? -1 : 1);

				bool isRev = (hb->bar.barBuf > hb->bar.barSub);
				float barmod = 1.0f;

				double shakemodtmp = (double)hb->bar.barChangeFrame / HPBAR_CHANGE_FRAME_MAX;
				double shakextmp = sin(HPBAR_SHAKE_POWER_X * hb->bar.barShakeFrame) * shakemodtmp * HPBAR_SHAKE_X;
				double shakeytmp = -cos(HPBAR_SHAKE_POWER_Y * hb->bar.barShakeFrame) * shakemodtmp * HPBAR_SHAKE_Y;

				colorTmp.w = HPBAR_PLAYER_OVERLAP_ALPHA
					+ (1.0f - HPBAR_PLAYER_OVERLAP_ALPHA)
					* (1.0f - (float)hb->bar.playerOverlapRate / UI_RATE_MAX);

				switch (j)
				{
				case 0:
					tex = g_tex_hpbar_bg.textureId;
					size = MkF2(HPBARBG_SIZE_X * HPBAR_SCALE, HPBARBG_SIZE_Y * HPBAR_SCALE);

					colorTmp.w = HPBARBG_PLAYER_OVERLAP_ALPHA
						+ (1.0f - HPBARBG_PLAYER_OVERLAP_ALPHA)
						* (1.0f - (float)hb->bar.playerOverlapRate / UI_RATE_MAX);

					break;
				case 1:
					barmod = (float)(isRev ? hb->bar.barMain: hb->bar.barSub) / UI_RATE_MAX;
					tex = g_tex_hpbar_sub.textureId;
					pos.x += HPBAR_PAD_X * HPBAR_SCALE * reverse;
					break;
				case 2:
					barmod = (float)(isRev ? hb->bar.barSub: hb->bar.barMain) / UI_RATE_MAX;

					if (hb->danger)
					{
						if (gframe % 9 < 3)
						{
							//tex = g_tex_hpbar_main.textureIdGray;
							tex = g_tex_hpbar_main.textureId;
							colorTmp = MkF4(10.0f, 0.5f, 0.5f, 1.0f);
						}
					}

					if (hb->bar.barChangeFrame > (HPBAR_CHANGE_FRAME_MAX - HPBAR_CHANGE_FLASH_FRAME)
						&& hb->bar.barChangeFrame <= HPBAR_CHANGE_FRAME_MAX
						&& hb->bar.barChangeFrame % HPBAR_CHANGE_FLASH_CYCLE_FRAME < HPBAR_CHANGE_FLASH_CYCLE_FRAME / 2)
					{
						//tex = g_tex_hpbar_main.textureIdGray;
						tex = g_tex_hpbar_main.textureId;
						colorTmp = MkF4(10.0f, 10.0f, 10.0f, 1.0f);
					}
					else
					{
						tex = g_tex_hpbar_main.textureId;
					}
					pos.x += HPBAR_PAD_X * HPBAR_SCALE * reverse;
					break;
				case 3:
					tex = g_tex_hpbar_text.textureId;
					size = MkF2(HPBARBG_SIZE_X * HPBAR_SCALE, HPBARBG_SIZE_Y * HPBAR_SCALE);

					/*colorTmp.w = HPBARBG_PLAYER_OVERLAP_ALPHA
						+ (1.0f - HPBARBG_PLAYER_OVERLAP_ALPHA)
						* (1.0f - (float)hb->bar.playerOverlapRate / UI_RATE_MAX);*/

					break;
					
				default:
					break;
				}

				DrawSpriteQuadColorRotateTexUv(
					//表示座標 (四角形の真ん中)
					pos.x + ((float)shakextmp) - (size.x / 2 * (1.0f - barmod)) * reverse
					+ HPBAR_P_PAD_KARI * i,
					//+(SCREEN_WIDTH / (SCREEN_WIDTH / (HPBAR_SIZE_X * HPBAR_SCALE))) * i,
					pos.y + ((float)shakeytmp),

					//サイズ width hight
					size.x * barmod * reverse,
					size.y,

					//角度
					0.0f,

					//色
					colorTmp,

					tex,  //画像(テクスチャ)の番号
					MkF2(0.0f, 0.0f),
					MkF2(barmod, 1.0f),
					DRAW_POLYGON_PRIORITY_ALWAYS
				);
			}
		}
	}
}


bool ShowHpbar(int index, bool show)
{
	UIDat.hpbar[index] = {};
	UIDat.hpbar[index].enable = show;

	return show;
}

bool ShowBossbar(int index, bool show, BOSSBAR_NAME name)
{
	GAME_OBJECT* obj = GetGameObject(index);
	UIDat.bossbar = {};
	UIDat.bossbar.enable = show;
	UIDat.bossbar.obj = obj;
	UIDat.bossbar.name = name;

	return show;
}


bool ShowGameFrame(bool show)
{
	UIDat.gameFrameView = show;
	if (show)
	{
		UIDat.gameFrameEnterFrame = GAMEFRAME_ENTER_FRAME;
	}

	return UIDat.gameFrameView;
}



void barUpdate(UI_BAR_DAT* bar, int current)
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



void UIReset()
{
	short gfef = UIDat.gameFrameEnterFrame;
	UIDat = {};
	UIDat.gameFrameEnterFrame = gfef;
}
