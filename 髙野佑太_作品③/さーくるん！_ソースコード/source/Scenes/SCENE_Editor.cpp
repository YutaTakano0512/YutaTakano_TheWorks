// ===================================================
// SCENE_Editor.h エディタシーン制御
// 
// 制作段階でマップの作製とかに使います
// ゲームプレイ中には使用しません
// 
// hara sougo  2024/08/11
// ===================================================
#include "../game_main.h"

#include "SCENE_Editor.h"

#include "../Core/Input/controller_main.h"
#include "../Game/Managers/bg.h"
#include "../Game/Managers/scene.h"
#include "../Game/Sprite/sprite.h"

#include "../Game/Game Object/gameObject.h"
#include "../Game/Game Object/gameObjectPrefab.h"
#include "../Game/Entities/Block/block.h"
#include "../Game/Entities/Block/blockPrefab.h"
#include "../Game/Game Map/gameMap.h"
#include "../Game/Systems/camera.h"
#include "../Game/Systems/collision.h"


#ifdef SWITCH_MODE
#else

#include "../Core/debugPrintf.h"
#include "../main.h"
#include "../Core/DirectX/dx.h"
#include "../Core/Input/keyboard.h"
#include "../Core/Input/mouse.h"

#include "../list.h"

#include <fstream>
#include <sstream>
#include <utility>
#include <array>

//いらんかった
//#define VAR_STR(var) #var   //引数にした変数を変数名を示す文字列リテラルとして返すマクロ関数

//列挙体の名前を文字列として取得するのに使うやつ
//これはコピペなので細かいことはわかりません
template <size_t N>
struct StaticString
{
	StaticString(const char* c)
		: StaticString(c, std::make_index_sequence<N>())
	{}

	std::string str() const { return std::string(mStr); }

private:
	template <size_t ...Indices>
	StaticString(const char* c, std::index_sequence<Indices...>)
		: mStr{ c[Indices]..., '\0' }
	{}

	char mStr[N + 1];
};

constexpr size_t FindChar(const char* str, char f)
{
	size_t res = 0;
	while (*str != f && *str != '\0') ++res, ++str;
	return res;
}

template <class M, M m>
constexpr auto EnumName()
{
#ifdef _MSC_VER
	constexpr char name[] = { __FUNCSIG__ };
	constexpr size_t s = FindChar(name, ',') + 1;
	constexpr size_t e = FindChar(name, '>');
#else
	constexpr const char* name_ = __PRETTY_FUNCTION__;
	constexpr const char* name = name_ + FindChar(name_, '=') + 1;
	constexpr size_t s = FindChar(name, '=') + 2;
	constexpr size_t e = FindChar(name, ']');
#endif
	constexpr size_t N = e - s;
	return StaticString<N>(name + s);
}
template <class Enum, class Indices>
struct EnumNames;
template <class Enum, std::underlying_type_t<Enum> ...Indices>
struct EnumNames<Enum, std::integer_sequence<std::underlying_type_t<Enum>, Indices...>>
{
	static const std::array<std::string, sizeof...(Indices)> msNames;
};
template <class Enum, std::underlying_type_t<Enum> ...Indices>
const std::array<std::string, sizeof...(Indices)>
EnumNames<Enum, std::integer_sequence<std::underlying_type_t<Enum>, Indices...>>::msNames
= { EnumName<Enum, (Enum)(Indices)>().str()... };

template <class Enum, std::underlying_type_t<Enum> Max = 128>
std::string GetEnumName(Enum e)
{
	using UType = std::underlying_type_t<Enum>;
	static const auto& names = EnumNames<Enum, std::make_integer_sequence<UType, Max>>::msNames;
	return names[(UType)e];
}

//マクロ定義
#define GAME_MAP_NAME_MAX (256)

#define S_EDIT_CAMERA_MODE_SPEED (30.0f)
#define S_EDIT_CAMERA_ZOOM_SPEED (0.3)

//列挙体定義
enum MAP_EDIT_SELECT_SET_MODE {
	MAP_EDIT_SELECT_SET_MODE_BLOCK = 0,
	MAP_EDIT_SELECT_SET_MODE_OBJ,
};
//構造体宣言

struct GAME_MAP_EDIT_OBJ_DAT {
	bool use;
	GAME_MAP_OBJ_DAT obj;
};
struct GAME_MAP_EDIT_BLOCK_DAT {
	bool use;
	GAME_MAP_BLOCK_DAT block;
};
struct GAME_MAP_EDIT_U {
	GAME_MAP_EDIT_OBJ_DAT objDat[GAME_OBJECT_MAX];
	GAME_MAP_EDIT_BLOCK_DAT blockDat[BLOCK_MAX];
};

struct GAME_MAP_EDIT_DAT {
	char name[GAME_MAP_NAME_MAX];
	GAME_MAP_EDIT_U map;
};

//プロトタイプ宣言
void editdatasave(void);
void editorMapSelectComboUpdate(void);


//グローバル変数
static unsigned int isForegroundWindowFrame = 0;
static EditorWindowHWND g_ewHwnd = {};

static GAME_OBJECT_TYPE nowSelectObj = GAME_OBJECT_TYPE_VOID;
static BLOCK_TYPE nowSelectBlock = BLOCK_TYPE_VOID;

static int nowSelectMapIndex = 0;
static LIST editMapList = nullptr;
static GAME_MAP_EDIT_DAT* nowSelectMapP = nullptr;

static MAP_EDIT_SELECT_SET_MODE nowSetModeSelect = MAP_EDIT_SELECT_SET_MODE_BLOCK;

static bool isViewSizeCheck = false;

static int modMouseMidScroll = 0;
#endif



// ===================================================
// タイトルシーンの初期化
// ===================================================
void InitializeSceneEditor(void)
{

#ifdef SWITCH_MODE
#else

	g_ewHwnd = EditorGetWindowHWND();

	//新たにリストを作成
	editMapList = NewList();

	//
	// エディタウィンドウのGUI設定
	//
	SendMessageA(g_ewHwnd.mapSelectList, CB_RESETCONTENT, 0, 0);
	for (int i = 0; i < GAME_MAP_DATA_ID_MAX; i++)
	{ //まず読み込む
		GAME_MAP_EDIT_DAT* gmed = (GAME_MAP_EDIT_DAT*)malloc(sizeof(GAME_MAP_EDIT_DAT));
		if (gmed != nullptr)
		{
			std::string nametmp = GameMapDataGetIdSring(i);
			strcpy_s(gmed->name, GAME_MAP_NAME_MAX,  nametmp.c_str());
			//strcpy(gmed->name, nametmp.c_str());
			
			//gmed->map = GameMapDataLoad((GAME_MAP_DATA_ID)i);
			//gmed->map = {};
			ZeroMemory(&gmed->map, sizeof(gmed->map));

			//マップを読み込み、エディタ用のデータにコンバート
			GAME_MAP maptmp = GameMapDataLoad((GAME_MAP_DATA_ID)i);
			for (int j = 0; j < maptmp.blockDat.count; j++)
			{
				gmed->map.blockDat[j].use = true;
				gmed->map.blockDat[j].block = maptmp.blockDat.blocks[j];
			}
			for (int j = 0; j < maptmp.objDat.count; j++)
			{
				gmed->map.objDat[j].use = true;
				gmed->map.objDat[j].obj = maptmp.objDat.objects[j];
			}
			//マップの解放
			GameMapFree(&maptmp);

			SendMessageA(g_ewHwnd.mapSelectList, CB_ADDSTRING, 0, (LPARAM)gmed->name);
			ListAdd(editMapList, gmed);
			if (i == 0)
			{
				nowSelectMapP = gmed;
			}
		}
	}
	SendMessageA(g_ewHwnd.mapSelectList, CB_SETCURSEL, 0, 0);

	SendMessageA(g_ewHwnd.blockSelectList, CB_RESETCONTENT, 0, 0);
	for (int i = 0; i < BLOCK_TYPE_MAX; i++)
	{
		SendMessageA(g_ewHwnd.blockSelectList, CB_ADDSTRING, 0, (LPARAM)GetEnumName((BLOCK_TYPE)i).c_str());
	}
	SendMessageA(g_ewHwnd.blockSelectList, CB_SETCURSEL, 0, 0);

	SendMessageA(g_ewHwnd.objSelectList, CB_RESETCONTENT, 0, 0);
	for (int i = 0; i < GAME_OBJECT_TYPE_MAX; i++)
	{
		SendMessageA(g_ewHwnd.objSelectList, CB_ADDSTRING, 0, (LPARAM)GetEnumName((GAME_OBJECT_TYPE)i).c_str());
	}
	SendMessageA(g_ewHwnd.objSelectList, CB_SETCURSEL, 0, 0);

	EditorWindowShow(true);
	editorMapSelectComboUpdate();
#endif

	SetCameraMode(0, CAMERA_MODE_NORMAL);

	SetBg(BG_TYPE_TEST_GAME);

	//既存のブロックとオブジェクトを消す
	DeleteBlockAll();
	DeleteGameObjectAll();
}

// ===================================================
// シーンの更新
// ===================================================
void UpdateSceneEditor(void)
{
	// シーン遷移
	if (GetControllerMainTrigger(CONTROLLER_BUTTON_TYPE_PLUS))
	{
		SetSceneFade(SCENE_TITLE);
	}


#ifdef SWITCH_MODE
#else

	{ //データの書き込み
		static bool savekeypush = false;
		if (GetAsyncKeyState(VK_CONTROL) && GetAsyncKeyState(VK_SHIFT)
			&& !savekeypush)
		{
			editdatasave();

			MessageBoxA(NULL, "セーブしました", " ", MB_OK);
			savekeypush = true;
		}
		else
		{
			savekeypush = false;
		}
	}

	{ //クリック
		constexpr int clickokframe = 5;
		static MouseState_tag l_GameMouseState = {};
		Mouse_GetState(&l_GameMouseState);
		//DebugPrintf("%d", l_GameMouseState.scrollWheelValue);

		CAMERA cinfo = GetCameraInfo(0);

		{
			SetCameraZoom(0, (float)(1.0f + (float)(l_GameMouseState.scrollWheelValue - modMouseMidScroll) / 1000 * S_EDIT_CAMERA_ZOOM_SPEED));
			{
				static bool crekeypush = false;
				if (l_GameMouseState.middleButton
					&& !crekeypush)
				{
					if (isForegroundWindowFrame > clickokframe)
					{
						modMouseMidScroll = l_GameMouseState.scrollWheelValue;
					}

					crekeypush = true;
				}
				else
				{
					crekeypush = false;
				}
			}

			{
				//Float2 ls = GetControllerMainLeftStick();
				Float2 ls = {};
				if (GetControllerMainPress(CONTROLLER_BUTTON_TYPE_LEFT, 0))
				{
					ls.x += -1.0f;
				}
				if (GetControllerMainPress(CONTROLLER_BUTTON_TYPE_RIGHT, 0))
				{
					ls.x += 1.0f;
				}
				if (GetControllerMainPress(CONTROLLER_BUTTON_TYPE_UP, 0))
				{
					ls.y += -1.0f;
				}
				if (GetControllerMainPress(CONTROLLER_BUTTON_TYPE_DOWN, 0))
				{
					ls.y += 1.0f;
				}
				SetCameraPos(0,
                    MkF3(cinfo.trans.pos.x + ls.x * S_EDIT_CAMERA_MODE_SPEED, 
                        cinfo.trans.pos.y + ls.y * S_EDIT_CAMERA_MODE_SPEED,
                        0)
                );
			}

			/*{
				Float2 rs = GetControllerMainRightStick();
				SetCameraRot(cinfo.trans.rot + rs.x * 1.0f);
			}*/
		}

		{
			//static bool setkeypush = false;
			static int setkeypush2 = 0;
			if (l_GameMouseState.leftButton	)
			{
				if (isForegroundWindowFrame > clickokframe)
				{
					if(nowSelectMapP != nullptr)
					{
						if (nowSetModeSelect == MAP_EDIT_SELECT_SET_MODE_BLOCK && nowSelectBlock > 0)
						{
							for (int i = 0; i < BLOCK_MAX; i++)
							{
								GAME_MAP_EDIT_BLOCK_DAT* bed = nowSelectMapP->map.blockDat + i;
								if (bed->use)
								{
									Float2 tpos = CameraPosToWorldPos(0, GetControllerMainTouchScreenPosition());
									BLOCK b = GetBlockPrefab((BLOCK_TYPE)bed->block.block);
									if (CheckBoxCollider(tpos, MkF2((float)bed->block.x, (float)bed->block.y), MkF2(1.0f, 1.0f), b.size))
									{
										bed->use = false;
										break;
									}
								}
							}

							for (int i = 0; i < BLOCK_MAX; i++)
							{
								GAME_MAP_EDIT_BLOCK_DAT* bed = nowSelectMapP->map.blockDat + i;
								if (!bed->use)
								{
									bed->use = true;
									{
										Float2 tpos = CameraPosToWorldPos(0, GetControllerMainTouchScreenPosition());

										GAME_MAP_BLOCK_DAT* bd = &bed->block;
										bd->block = (int)nowSelectBlock;
										//bd->x = tpos.x;
										//bd->y = tpos.y;
										bd->x = (int)((float)(ceil(tpos.x / BLOCK_WIDTH) * BLOCK_WIDTH) - (BLOCK_WIDTH / 2));
										bd->y = (int)((float)(ceil(tpos.y / BLOCK_HIGHT) * BLOCK_HIGHT) - (BLOCK_HIGHT / 2));
									}
									break;
								}
							}
						}
						else if (setkeypush2 == 1 && nowSetModeSelect == MAP_EDIT_SELECT_SET_MODE_OBJ && nowSelectObj > 0)
						{
							for (int i = 0; i < GAME_OBJECT_MAX; i++)
							{
								GAME_MAP_EDIT_OBJ_DAT* oed = nowSelectMapP->map.objDat + i;
								if (!oed->use)
								{
									oed->use = true;
									{
										Float2 tpos = CameraPosToWorldPos(0, GetControllerMainTouchScreenPosition());

										GAME_MAP_OBJ_DAT* od = &oed->obj;
										od->obj = (int)nowSelectObj;
										//od->x = tpos.x;
										//od->y = tpos.y;
										od->x = (int)((float)(ceil(tpos.x / BLOCK_WIDTH) * BLOCK_WIDTH) - (BLOCK_WIDTH / 2));
										od->y = (int)((float)(ceil(tpos.y / BLOCK_HIGHT) * BLOCK_HIGHT) - (BLOCK_HIGHT / 2));
									}
									break;
								}
							}
						}
					}
					//DebugPrintf("LR");
					//MessageBoxA(NULL, "mc", " ", MB_OK);

				}

				setkeypush2++;
			}
			else
			{
				setkeypush2 = 0;
			}
		}
		{
			//static bool delkeypush = false;
			static int delkeypush2 = 0;
			if (l_GameMouseState.rightButton)
			{
				if (isForegroundWindowFrame > clickokframe)
				{
					if (nowSelectMapP != nullptr)
					{
						if (nowSetModeSelect == MAP_EDIT_SELECT_SET_MODE_BLOCK)
						{ //ブロック
							for (int i = 0; i < BLOCK_MAX; i++)
							{
								GAME_MAP_EDIT_BLOCK_DAT* bed = nowSelectMapP->map.blockDat + i;
								if (bed->use)
								{
									Float2 tpos = CameraPosToWorldPos(0, GetControllerMainTouchScreenPosition());
									BLOCK b = GetBlockPrefab((BLOCK_TYPE)bed->block.block);
									if (CheckBoxCollider(tpos, MkF2((float)bed->block.x, (float)bed->block.y), MkF2(1.0f, 1.0f), b.size))
									{
										bed->use = false;
										break;
									}
								}
							}
						}
						else if (delkeypush2 == 1 && nowSetModeSelect == MAP_EDIT_SELECT_SET_MODE_OBJ)
						{ //オブジェクト
							for (int i = 0; i < GAME_OBJECT_MAX; i++)
							{
								GAME_MAP_EDIT_OBJ_DAT* oed = nowSelectMapP->map.objDat + i;
								if (oed->use)
								{
									Float2 tpos = CameraPosToWorldPos(0, GetControllerMainTouchScreenPosition());
									GAME_OBJECT o = GetGameObjectPrefab((GAME_OBJECT_TYPE)oed->obj.obj);
									if (CheckBoxCollider(tpos, MkF2((float)oed->obj.x, (float)oed->obj.y), MkF2(1.0f, 1.0f), o.size))
									{
										oed->use = false;
										break;
									}
								}
							}
						}
					}
				}

				delkeypush2++;
			}
			else
			{
				delkeypush2 = 0;
			}
		}
	}

	//ウィンドウが前面にあるかをカウント
	if (GetMainHWND() == GetForegroundWindow())
	{
		isForegroundWindowFrame++;
	}
	else
	{
		isForegroundWindowFrame = 0;
	}

#endif
}

// ===================================================
// シーンの描画
// ===================================================
void DrawSceneEditor(void)
{
#ifdef SWITCH_MODE
#else
	{
		CAMERA cinfo = GetCameraInfo(0);

		Float2 cPos = {};

		cPos.x = cinfo.trans.pos.x;
		cPos.y = cinfo.trans.pos.y;

		float zoomAnti = (1.0f / cinfo.trans.zoom);
		float widthMax = SCREEN_WIDTH * zoomAnti;
		float hightMax = SCREEN_HEIGHT * zoomAnti;

		for (int i = 0; i < (widthMax / BLOCK_WIDTH); i++)
		{
			Float2 cPosX = cPos;
			cPosX.x += (-widthMax / 2 + BLOCK_WIDTH * i);
			cPosX.x = (float)((int)(ceil(cPosX.x / BLOCK_WIDTH) * BLOCK_WIDTH));
			cPosX.y = (float)((int)(ceil(cPosX.y / BLOCK_HIGHT) * BLOCK_HIGHT));

			{
				DRAW_SPRITE_DAT dsdat = {};
				dsdat.x = cPosX.x;
				dsdat.y = cPosX.y;
				dsdat.w = 8;
				dsdat.h = hightMax;
				dsdat.rot = 0.0f;
				dsdat.color = { 1.0f, 1.0f, 1.0f, 0.5f };

				dsdat.texNo = 0;
				dsdat.loadTexType = LOADTEXTURETYPE_MAIN;

				dsdat.texUV = { 0.0f, 0.0f };
				dsdat.texWH = { 1.0f, 1.0f };
				dsdat.startPos = { 0.0f, 0.0f };
				dsdat.startRot = 0.0f;
				dsdat.startScale = { 1.0f, 1.0f };

				DrawSpriteInCamera(dsdat);
			}
		}
		for (int i = 0; i < (hightMax / BLOCK_HIGHT); i++)
		{
			Float2 cPosY = cPos;
			cPosY.y += (-hightMax / 2 + BLOCK_HIGHT * i);
			cPosY.x = (float)((int)(ceil(cPosY.x / BLOCK_WIDTH) * BLOCK_WIDTH));
			cPosY.y = (float)((int)(ceil(cPosY.y / BLOCK_HIGHT) * BLOCK_HIGHT));

			{
				DRAW_SPRITE_DAT dsdat = {};
				dsdat.x = cPosY.x;
				dsdat.y = cPosY.y;
				dsdat.w = widthMax;
				dsdat.h = 8;
				dsdat.rot = 0.0f;
				dsdat.color = { 1.0f, 1.0f, 1.0f, 0.5f };

				dsdat.texNo = 0;
				dsdat.loadTexType = LOADTEXTURETYPE_MAIN;

				dsdat.texUV = { 0.0f, 0.0f };
				dsdat.texWH = { 1.0f, 1.0f };
				dsdat.startPos = { 0.0f, 0.0f };
				dsdat.startRot = 0.0f;
				dsdat.startScale = { 1.0f, 1.0f };

				DrawSpriteInCamera(dsdat);
			}
		}
	}

	if (nowSelectMapP != nullptr)
	{
		for (int i = 0; i < BLOCK_MAX; i++)
		{
			GAME_MAP_EDIT_BLOCK_DAT* bed = nowSelectMapP->map.blockDat + i;
			if (bed->use)
			{
				GAME_MAP_BLOCK_DAT* bd = &bed->block;
				if (bd->block >= BLOCK_TYPE_VOID && bd->block < BLOCK_TYPE_MAX)
				{
					BLOCK b = GetBlockPrefab((BLOCK_TYPE)bd->block);
					b.pos = MkF2((float)bd->x, (float)bd->y);

					DrawBlockData(&b);
				}
			}
		}

		for (int i = 0; i < GAME_OBJECT_MAX; i++)
		{
			GAME_MAP_EDIT_OBJ_DAT* oed = nowSelectMapP->map.objDat + i;
			if (oed->use)
			{
				GAME_MAP_OBJ_DAT* od = &oed->obj;
				if (od->obj >= GAME_OBJECT_TYPE_VOID && od->obj < GAME_OBJECT_TYPE_MAX)
				{
					GAME_OBJECT o = GetGameObjectPrefab((GAME_OBJECT_TYPE)od->obj);
					o.pos = MkF3((float)od->x, (float)od->y, 0.0f);

					DrawGameObjectData(&o);
				}
			}
		}

		for (int i = 0; i < BLOCK_MAX; i++)
		{
			GAME_MAP_EDIT_BLOCK_DAT* bed = nowSelectMapP->map.blockDat + i;
			if (bed->use)
			{
				GAME_MAP_BLOCK_DAT* bd = &bed->block;
				if (bd->block >= BLOCK_TYPE_VOID && bd->block < BLOCK_TYPE_MAX)
				{
					BLOCK b = GetBlockPrefab((BLOCK_TYPE)bd->block);
					b.pos = MkF2((float)bd->x, (float)bd->y);

					if (isViewSizeCheck && GetMainGameFrame() % 30 >= 3)
					{
						DRAW_SPRITE_DAT dsdat = {};
						dsdat.x = (float)bd->x;
						dsdat.y = (float)bd->y;
						dsdat.w = b.size.x;
						dsdat.h = b.size.y;
						dsdat.rot = 0.0f;
						dsdat.color = { 0.0f, 0.0f, 1.0f, 0.5f };

						dsdat.texNo = 0;
						dsdat.loadTexType = LOADTEXTURETYPE_MAIN;

						dsdat.texUV = { 0.0f, 0.0f };
						dsdat.texWH = { 1.0f, 1.0f };
						dsdat.startPos = { 0.0f, 0.0f };
						dsdat.startRot = 0.0f;
						dsdat.startScale = { 1.0f, 1.0f };

						DrawSpriteInCamera(dsdat);
					}
				}
			}
		}

		for (int i = 0; i < GAME_OBJECT_MAX; i++)
		{
			GAME_MAP_EDIT_OBJ_DAT* oed = nowSelectMapP->map.objDat + i;
			if (oed->use)
			{
				GAME_MAP_OBJ_DAT* od = &oed->obj;
				if (od->obj >= GAME_OBJECT_TYPE_VOID && od->obj < GAME_OBJECT_TYPE_MAX)
				{
					GAME_OBJECT o = GetGameObjectPrefab((GAME_OBJECT_TYPE)od->obj);
					o.pos = MkF3((float)od->x, (float)od->y, 0.0f);

					if (isViewSizeCheck && GetMainGameFrame() % 30 >= 3)
					{
						DRAW_SPRITE_DAT dsdat = {};
						dsdat.x = (float)od->x;
						dsdat.y = (float)od->y;
						dsdat.w = o.size.x;
						dsdat.h = o.size.y;
						dsdat.rot = 0.0f;
						dsdat.color = { 1.0f, 1.0f, 0.0f, 0.5f };

						dsdat.texNo = 0;
						dsdat.loadTexType = LOADTEXTURETYPE_MAIN;

						dsdat.texUV = { 0.0f, 0.0f };
						dsdat.texWH = { 1.0f, 1.0f };
						dsdat.startPos = { 0.0f, 0.0f };
						dsdat.startRot = 0.0f;
						dsdat.startScale = { 1.0f, 1.0f };

						DrawSpriteInCamera(dsdat);
					}

				}
			}
		}
	}

	{
		constexpr int MAP_X = 144;
		constexpr int MAP_Y = 81;
		for (int i = 0; i < MAP_X + 2; i++)
		{
			for (int j = 0; j < 2; j++)
			{
				BLOCK b = GetBlockPrefab((BLOCK_TYPE)(BLOCK_TYPE_01 + i % 10));
				b.pos = MkF2(
					-BLOCK_WIDTH * MAP_X / 2 + BLOCK_WIDTH * i - BLOCK_WIDTH / 2,
					-BLOCK_HIGHT * (MAP_Y / 2 + 1) * (j == 1 ? -1 : 1) - BLOCK_HIGHT / 2
				);
				DrawBlockData(&b);
			}
		}
		for (int i = 0; i < MAP_Y; i++)
		{
			for (int j = 0; j < 2; j++)
			{
				BLOCK b = GetBlockPrefab((BLOCK_TYPE)(BLOCK_TYPE_01 + i % 10));
				b.pos = MkF2(
					-BLOCK_WIDTH * ((float)MAP_X / 2 + 0.5f) * (j == 1 ? -1 : 1) 
					- BLOCK_WIDTH / 2 + BLOCK_WIDTH * 1.0f - BLOCK_WIDTH / 2,

					-BLOCK_HIGHT * MAP_Y / 2 + BLOCK_HIGHT * i
				);
				DrawBlockData(&b);
			}
		}
	}

	/*DRAW_SPRITE_DAT dsdat = {};
	dsdat.x = 0.0f;
	dsdat.y = -SCREEN_HEIGHT * 0.02 / 2;
	dsdat.w = SCREEN_WIDTH;
	dsdat.h = SCREEN_HEIGHT * 0.98;
	dsdat.rot = 0.0f;
	dsdat.color = { 0.0f, 0.0f, 1.0f, 1.0f };

	dsdat.texNo = 0;
	dsdat.loadTexType = LOADTEXTURETYPE_GRAY;

	dsdat.texUV = { 0.0f, 0.0f };
	dsdat.texWH = { 1.0f, 1.0f };
	dsdat.startPos = { 0.0f, 0.0f };
	dsdat.startRot = 0.0f;
	dsdat.startScale = { 1.0f, 1.0f };

	DrawSpriteInCamera(dsdat);*/
#endif
}

// ===================================================
// タイトルシーンの終了処理
// ===================================================
void FinalizeSceneEditor(void)
{
	//既存のブロックとオブジェクトを消す
	DeleteBlockAll();
	DeleteGameObjectAll();

    DXSetDepthStencilStateDepth(true);

#ifdef SWITCH_MODE
#else
	EditorWindowShow(false);

	//for (int i = 0; i < ListGetSize(editMapList); i++)
	//{
	//	GAME_MAP_EDIT_DAT* gmed = (GAME_MAP_EDIT_DAT*)ListGetValue(editMapList, i);
	//	//free(gmed->name);
	//	GameMapFree(&gmed->map);
	//}
	ListClear(editMapList, true);
	free(editMapList);
#endif
}



#ifdef SWITCH_MODE
#else

//
// エディタのイベント (外部から呼び出されます)
//
void OnWindowEventEditor(unsigned short cas, unsigned short cod)
{
	switch (cas)
	{
	case EditorWindowHMENU_addMapAddBtn: //マップの追加
		if (cod == BN_CLICKED) {
			char mapnametmp[GAME_MAP_NAME_MAX] = {};
			int txtnumtmp = GetWindowText(g_ewHwnd.addMapTextBox, mapnametmp, GAME_MAP_NAME_MAX);
			//int txtnumtmp = SendMessageA(g_ewHwnd.addMapTextBox, EM_GETLINE, 0, (LPARAM)(&mapnametmp));
			if (txtnumtmp > 0)
			{
				{
					GAME_MAP_EDIT_DAT* gmed = (GAME_MAP_EDIT_DAT*)malloc(sizeof(GAME_MAP_EDIT_DAT));
					if (gmed != nullptr)
					{
						
						//strcpy(gmed->name, mapnametmp);
						strcpy_s(gmed->name, GAME_MAP_NAME_MAX, mapnametmp);

						//gmed->map = {};
						ZeroMemory(&gmed->map, sizeof(gmed->map));

						/*gmed->map.blockDat.blocks = (GAME_MAP_BLOCK_DAT*)malloc(sizeof(GAME_MAP_BLOCK_DAT));
						gmed->map.blockDat.count = 0;
						gmed->map.objDat.objects = (GAME_MAP_OBJ_DAT*)malloc(sizeof(GAME_MAP_OBJ_LIST_DAT));
						gmed->map.objDat.count = 0;*/

						SendMessageA(g_ewHwnd.mapSelectList, CB_ADDSTRING, 0, (LPARAM)gmed->name);
						ListAdd(editMapList, gmed);
					}
				}
				editorMapSelectComboUpdate();

				MessageBoxA(NULL, "マップを追加しました", " ", MB_OK);
			}
		}
		break;
	case EditorWindowHMENU_mapSelectList: //マップの選択
		if (cod == LBN_SELCHANGE) {
			int cursel = (int)SendMessageA(g_ewHwnd.mapSelectList, CB_GETCURSEL, 0, 0);
			if (cursel >= 0)
			{
				nowSelectMapIndex = cursel;
				nowSelectMapP = (GAME_MAP_EDIT_DAT*)ListGetValue(editMapList, nowSelectMapIndex);
				DebugPrintf("%d %p", nowSelectMapIndex, nowSelectMapP);
			}
		}
		break;
	case EditorWindowHMENU_blockSelectList: //ブロックの選択
		if (cod == LBN_SELCHANGE) {
			int cursel = (int)SendMessageA(g_ewHwnd.blockSelectList, CB_GETCURSEL, 0, 0);
			if (cursel > 0)
			{
				nowSelectBlock = (BLOCK_TYPE)cursel;
			}
		}
		break;
	case EditorWindowHMENU_objSelectList: //オブジェクトの選択
		if (cod == LBN_SELCHANGE) {
			int cursel = (int)SendMessageA(g_ewHwnd.objSelectList, CB_GETCURSEL, 0, 0);
			if (cursel > 0)
			{
				nowSelectObj = (GAME_OBJECT_TYPE)cursel;
			}
		}
		break;

	//配置モードの選択
	case EditorWindowHMENU_setModeSelectBlockBtn:
		if (cod == BN_CLICKED) {
			nowSetModeSelect = MAP_EDIT_SELECT_SET_MODE_BLOCK;
		}
		break;
	case EditorWindowHMENU_setModeSelectObjBtn:
		if (cod == BN_CLICKED) {
			nowSetModeSelect = MAP_EDIT_SELECT_SET_MODE_OBJ;
		}
		break;


	case EditorWindowHMENU_viewSizeCheckBox: //オブジェクトの選択
		if (cod == BN_CLICKED) {
			isViewSizeCheck = SendMessage(g_ewHwnd.viewSizeCheckBox, BM_GETCHECK, 0, 0) == BST_CHECKED;
		}
		break;
	default:break;
	}
}
//
// エディタのマップセレクトの更新
//
void editorMapSelectComboUpdate(void)
{
	int cursel = (int)SendMessageA(g_ewHwnd.mapSelectList, CB_GETCURSEL, 0, 0);

	SendMessageA(g_ewHwnd.mapSelectList, CB_RESETCONTENT, 0, 0);
	for (int i = 0; i < ListGetSize(editMapList); i++)
	{
		GAME_MAP_EDIT_DAT* gmed = (GAME_MAP_EDIT_DAT*)ListGetValue(editMapList, i);
		SendMessageA(g_ewHwnd.mapSelectList, CB_ADDSTRING, 0, (LPARAM)gmed->name);
	}

	{
		int r = (int)SendMessageA(g_ewHwnd.mapSelectList, CB_SETCURSEL, cursel, 0);
		if (r == CB_ERR)
		{
			SendMessageA(g_ewHwnd.mapSelectList, CB_SETCURSEL, 0, 0);
		}
	}
}


//
// エディットデータの書き込み
//
void editdatasave(void)
{
	{ //マップデータの生成
		std::ofstream writ_hf;
		std::ofstream writ_cf;
		std::string hf_name = std::string(GAME_DAT_PATH) + std::string("Game\\Game Map\\") + std::string(GAME_MAP_DAT_H_PATH);
		std::string cf_name = std::string(GAME_DAT_PATH) + std::string("Game\\Game Map\\") + std::string(GAME_MAP_DAT_C_PATH);

		writ_hf.open(hf_name, std::ios::out);
		writ_cf.open(cf_name, std::ios::out);

		//コメント
		std::string text_war = std::string("")
			+ "//このファイルはエディタ(sceneEditor.cpp)のプログラムで生成しています\n"
			+ "//可読性は考慮していません\n";

		//コード内容を書き込むところ
		std::string hf_text = "";
		std::string cf_text = "";


		//ヘッダファイル頭の部分
		writ_hf << text_war << "\n"
			<< "#ifndef _GAME_MAP_DAT_H_" << "\n"
			<< "#define _GAME_MAP_DAT_H_ " << "\n"
			<< "#include \"gameMapGeneral.h\" " << "\n"
			;

		//ソースファイル頭の部分
		writ_cf << text_war << "\n"
			<< "#include \"../Game Object/gameObject.h\"" << "\n"
			<< "#include \"../Entities/Block/block.h\"" << "\n"
			<< "#include \"" << std::string(GAME_MAP_DAT_H_PATH) << "\"\n"
			<< "\n"
			;

		{
			//マップの列挙体 頭の部分
			std::string map_enum_text = "enum GAME_MAP_DATA_ID {\n";
			std::string map_enum_string_text = std::string("")
				+ "static std::string game_map_data_id_string[GAME_MAP_DATA_ID_MAX] = {\n";

			//マップをロードする関数 頭の部分
			std::string map_loadcode_text = std::string("")
				+ "GAME_MAP GameMapDataLoad(GAME_MAP_DATA_ID id)\n"
				+ "{\n"
				+ "\tswitch (id)\n"
				+ "\t{\n";

			//マップを生成するためのコードを生成
			for (int i = 0; i < ListGetSize(editMapList); i++)
			{
				GAME_MAP_EDIT_DAT* gmed = (GAME_MAP_EDIT_DAT*)ListGetValue(editMapList, i);

				std::string idtmp = "";
				{ //列挙体の生成
					std::ostringstream oss;
					oss << gmed->name;
					/*oss
						<< "TESTMAP" << i
						;*/

					idtmp = "GAME_MAP_DATA_ID_" + oss.str();

					map_enum_text += "\t" + idtmp;
					if (i == 0)
					{
						map_enum_text += " = 0";
					}
					map_enum_text += ",\n";

					map_enum_string_text += "\t\"" + oss.str() + "\",\n";
				}

				{ //マップデータのコード生成
					std::ostringstream oss;

					std::ostringstream oss2;
					oss2 << "\t\t{\n";
					oss2 << "\t\t\t" << "GAME_MAP gamemaptmp = {};" << "\n";
					{
						std::ostringstream oss3tmp;
						std::ostringstream oss4tmp;

						int useNum = 0;
						for (int i = 0; i < BLOCK_MAX; i++)
						{
							GAME_MAP_EDIT_BLOCK_DAT* betmp = gmed->map.blockDat + i;
							if (betmp->use)
							{
								GAME_MAP_BLOCK_DAT* btmp = &betmp->block;
								oss4tmp
									<< "\n\t\t\t\t{"
									<< "GAME_MAP_BLOCK_DAT gmbdtmp = {};"
									<< "gmbdtmp.x = " << btmp->x << ";"
									<< "gmbdtmp.y = " << btmp->y << ";"
									<< "gmbdtmp.block = (int)" << GetEnumName((BLOCK_TYPE)btmp->block) << ";"
									<< "blockdatatmp[" << useNum << "] = gmbdtmp;"
									<< "}"
									;
								useNum++;
							}

							//BLOCK* btmp = GetBlocks() + i;

							/*if (btmp->isUse)
							{
							oss4tmp
								<< "\n\t\t\t\t{"
								<< "GAME_MAP_BLOCK_DAT gmbdtmp = {};"
								<< "gmbdtmp.x = " << btmp->pos.x << ";"
								<< "gmbdtmp.y = " << btmp->pos.y << ";"
								<< "gmbdtmp.block = (int)" << GetEnumName(btmp->type) << ";"
								<< "blockdatatmp[" << useNum << "] = gmbdtmp;"
								<< "}"
								;
							useNum++;
							}*/
						}

						oss3tmp
							<< "\t\t\t" << "GAME_MAP_BLOCK_DAT* blockdatatmp = (GAME_MAP_BLOCK_DAT*)malloc(sizeof(GAME_MAP_BLOCK_DAT) * " << useNum << ");" << "\n"
							<< "\t\t\t/*ブロックデータ(とても長いので注意)*/ if(blockdatatmp != nullptr) {" << oss4tmp.str() << "}\n"
							<< "\t\t\t" << "GAME_MAP_BLOCK_LIST_DAT gamemapblocktmp = {};" << "\n"
							<< "\t\t\t" << "gamemapblocktmp.count = " << useNum << ";" << "\n"
							<< "\t\t\t" << "gamemapblocktmp.blocks = blockdatatmp;" << "\n"
							<< "\t\t\t" << "gamemaptmp.blockDat = gamemapblocktmp;" << "\n"
							;
						oss2 << oss3tmp.str();
					}
					{
						std::ostringstream oss3tmp;
						std::ostringstream oss4tmp;

						int useNum = 0;
						for (int i = 0; i < GAME_OBJECT_MAX; i++)
						{
							GAME_MAP_EDIT_OBJ_DAT* goetmp = gmed->map.objDat + i;
							if (goetmp->use)
							{
								GAME_MAP_OBJ_DAT* gotmp = &goetmp->obj;
								oss4tmp
									<< "{"
									<< "GAME_MAP_OBJ_DAT gmodtmp = {};"
									<< "gmodtmp.x = " << gotmp->x << ";"
									<< "gmodtmp.y = " << gotmp->y << ";"
									<< "gmodtmp.obj = (int)" << GetEnumName((GAME_OBJECT_TYPE)gotmp->obj) << " ;"
									<< "objdatatmp[" << useNum << "] = gmodtmp;"
									<< "}"
									;
								useNum++;
							}

							/*GAME_OBJECT* gotmp = GetGameObjects() + i;

							if (gotmp->use)
							{
								oss4tmp
									<< "{"
									<< "GAME_MAP_OBJ_DAT gmodtmp = {};"
									<< "gmodtmp.x = " << gotmp->pos.x << ";"
									<< "gmodtmp.y = " << gotmp->pos.y << ";"
									<< "gmodtmp.obj = (int)" << GetEnumName(gotmp->type) << " ;"
									<< "objdatatmp[" << useNum << "] = gmodtmp;"
									<< "}"
									;
								useNum++;
							}*/
						}

						oss3tmp
							<< "\t\t\t" << "GAME_MAP_OBJ_DAT* objdatatmp = (GAME_MAP_OBJ_DAT*)malloc(sizeof(GAME_MAP_OBJ_DAT) * " << useNum << ");" << "\n"
							<< "\t\t\t/*オブジェクトデータ(とても長いので注意)*/ if(objdatatmp != nullptr) {" << oss4tmp.str() << "}\n"
							<< "\t\t\t" << "GAME_MAP_OBJ_LIST_DAT gamemapobjtmp = {};" << "\n"
							<< "\t\t\t" << "gamemapobjtmp.count = " << useNum << ";" << "\n"
							<< "\t\t\t" << "gamemapobjtmp.objects = objdatatmp;" << "\n"
							<< "\t\t\t" << "gamemaptmp.objDat = gamemapobjtmp;" << "\n"
							;
						oss2 << oss3tmp.str();
					}

					oss << "\t\tcase " << idtmp << ":\n"
						<< oss2.str()
						<< "\t\t\treturn gamemaptmp;\n"
						<< "\t\t}\n";
					;

					map_loadcode_text += oss.str();
				}
			}

			map_enum_text += "\tGAME_MAP_DATA_ID_MAX,\n";
			
			//かっことじ
			map_enum_text += "};\n";
			map_enum_string_text += "};\n\n";
			map_loadcode_text += "\t}\n\treturn {};\n}\n";

			//ついか
			hf_text += map_enum_text;

			cf_text += map_enum_string_text;
			cf_text += map_loadcode_text;
		}

		writ_hf << "\n"
			<< hf_text << "\n"
			//<< "void GameMapDataInit(void);\n"
			<< "GAME_MAP GameMapDataLoad(GAME_MAP_DATA_ID id);\n"
			<< "std::string GameMapDataGetIdSring(int id);\n"
			<< "#endif\n"
			<< std::endl;

		writ_cf << "\n"
			<< cf_text << "\n"
			<< "std::string GameMapDataGetIdSring(int id)\n"
			<< "{\n"
			<< "\treturn game_map_data_id_string[id];\n"
			<< "}\n"
			<< std::endl;

		writ_hf.close();
		writ_cf.close();
	}
}

#endif