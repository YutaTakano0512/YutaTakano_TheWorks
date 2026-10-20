// ===================================================
// texData.h テクスチャのセット
// 
// hara sougo  2024/08/22
// ===================================================
#ifndef _TEX_DATA_H_
#define _TEX_DATA_H_

#include "../Systems/sys_general.h"
#include "texture.h"

#include <iostream>

constexpr int TEX_DATA_LOAD_BUFF_MAX = 64;

constexpr auto TEX_PATH = "rom/texture";

// ===================================================
// テクスチャの種類を宣言
// ===================================================
enum TEX_TYPE {
	TEX_TYPE_VOID = 0,

	TEX_TYPE_BLOCK_GENERAL,
	TEX_TYPE_BLOCK_TEST,

	TEX_TYPE_UI_PAUSE_TEXT,
	TEX_TYPE_UI_NUMBER,

	TEX_TYPE_UI_HPBAR_MAIN,
	TEX_TYPE_UI_HPBAR_SUB,
	TEX_TYPE_UI_HPBAR_BG,
	TEX_TYPE_UI_HPBAR_TEXT,
	TEX_TYPE_UI_WHITEBOARD,
	TEX_TYPE_UI_TAG,
	TEX_TYPE_UI_DETAIL0,
	TEX_TYPE_UI_DETAIL1,
	TEX_TYPE_UI_DETAIL2,

	TEX_TYPE_UI_ICON_PLAYER,
	TEX_TYPE_UI_ICON_ENEMY_SMALL,
	TEX_TYPE_UI_ICON_ENEMY_MIDIUM,
	TEX_TYPE_UI_ICON_ENEMY_BOSS,

	TEX_TYPE_BG_TESTTITLE,
	TEX_TYPE_BG_STAGESELECT,
	TEX_TYPE_BG_TESTN,
	TEX_TYPE_BG_TESTRESULT,
	TEX_TYPE_BG_ISEKI_TEST,
	

	TEX_TYPE_EFFECT_FLASH_GENERAL,
	TEX_TYPE_EFFECT_FLASH_RING,
	TEX_TYPE_EFFECT_FLASH_GENERAL_ENEM,
	TEX_TYPE_EFFECT_FLASH_RING_ENEM,
	TEX_TYPE_EFFECT_TWINKLE,
	TEX_TYPE_EFFECT_SPARK,

	//TEX_TYPE_EFFECT_HUDETEST,
	//TEX_TYPE_EFFECT_HUDETEST2,

	TEX_TYPE_EFFECT_INK_TEST_01,

	//TEX_TYPE_EFFECT_KAKOPON_HIT_001,
	TEX_TYPE_EFFECT_KAKOPON_HIT_002,


	TEX_TYPE_PLAYERTEST_DOWN,
	TEX_TYPE_PLAYERTEST_LEFT,

	TEX_TYPE_OBJECT_ISEKI_ZANGAI_0,
	TEX_TYPE_OBJECT_ISEKI_ZANGAI_1,
	TEX_TYPE_OBJECT_ISEKI_ZANGAI_2,
	TEX_TYPE_OBJECT_ISEKI_DAIZA_0,
	TEX_TYPE_OBJECT_ISEKI_HASHIRA_0,
	TEX_TYPE_OBJECT_ISEKI_HASHIRA_TAORETA_0,
	TEX_TYPE_OBJECT_ISEKI_KANATSUBAN_0,

	TEX_TYPE_BLOCK_tile_center,
	TEX_TYPE_BLOCK_tile_down,
	TEX_TYPE_BLOCK_tile_downL,
	TEX_TYPE_BLOCK_tile_downR,
	TEX_TYPE_BLOCK_tile_L,
	TEX_TYPE_BLOCK_tile_R,
	TEX_TYPE_BLOCK_tile_upL,
	TEX_TYPE_BLOCK_tile_upR,
	TEX_TYPE_BLOCK_tile1,
	TEX_TYPE_BLOCK_tile2,
	TEX_TYPE_BLOCK_tile3,
	TEX_TYPE_BLOCK_tile4,

	//キャラクター選択画面
	TEX_TYPE_CHARACTER_KEM,
	TEX_TYPE_CHARACTER_SELECT_ONE,
	TEX_TYPE_CHARACTER_SELECT_TWO,
	TEX_TYPE_BG_CHARACTER_SELECT,


	//キャラクター詳細
	TEX_TYPE_CHARACTER_STATUS_ONE,
	TEX_TYPE_CHARACTER_STATUS_TWO,
	TEX_TYPE_CHARACTER_STATUS_THREE,

	//リザルト画面
	
	TEX_TYPE_BG_TESTRESULT_CLER,
	TEX_TYPE_BG_TESTRESULT_GAMEOVER,
	TEX_TYPE_UI_RESULT_CLEAR,
	TEX_TYPE_UI_TITLE_TEXT,
	TEX_TYPE_UI_REPLAY_TEXT,
	TEX_TYPE_UI_SCORE_TEXT,
	TEX_TYPE_UI_GAMEOVER_TEXT,

	TEX_TYPE_TITLE_TITLETEXT,
	TEX_TYPE_TITLE_GAMESTARTTEXT,
	TEX_TYPE_TITLE_RANKINGTEXT,
	TEX_TYPE_TITLE_EXITTEXT,

    TEX_TYPE_UI_RETIRE,

    TEX_TYPE_TITLE_CONFIRM_TITLETEXT,
    TEX_TYPE_TITLE_CONFIRM_YESTEXT,
    TEX_TYPE_TITLE_CONFIRM_NOTEXT,

	TEX_TYPE_MAX,
};

//インライン関数
inline void TexDataSet(std::string* t)
{
	t[TEX_TYPE_VOID]/*									*/ = "rom:/texture/gray.tga";

	t[TEX_TYPE_BLOCK_GENERAL]/*							*/ = "rom:/texture/block/general.tga";
	t[TEX_TYPE_BLOCK_TEST]/*							*/ = "rom:/texture/block/test_block.tga";

	t[TEX_TYPE_BG_TESTTITLE]/*							*/ = "rom:/texture/bg/title.png";
	t[TEX_TYPE_BG_STAGESELECT]/*                        */ = "rom:/texture/bg/stage_select.tga";
	t[TEX_TYPE_BG_TESTN]/*								*/ = "rom:/texture/bg/test_n.tga";
	t[TEX_TYPE_BG_TESTRESULT]/*							*/ = "rom:/texture/bg/test_result.tga";
	t[TEX_TYPE_BG_ISEKI_TEST]/*							*/ = "rom:/texture/bg/iseki_test.png";

	t[TEX_TYPE_UI_HPBAR_MAIN]/*							*/ = "rom:/texture/ui/hpbar_main.tga";
	t[TEX_TYPE_UI_HPBAR_SUB]/*							*/ = "rom:/texture/ui/hpbar_sub.tga";
	t[TEX_TYPE_UI_HPBAR_BG]/*							*/ = "rom:/texture/ui/hpbar_bg.tga";
	t[TEX_TYPE_UI_HPBAR_TEXT]/*							*/ = "rom:/texture/ui/hpbar_text.tga";

	t[TEX_TYPE_UI_PAUSE_TEXT]/*							*/ = "rom:/texture/ui/pause_text.tga";
	t[TEX_TYPE_UI_NUMBER]/*								    */ = "rom:/texture/ui/number.tga";
	t[TEX_TYPE_UI_WHITEBOARD]/*							*/ = "rom:/texture/ui/whiteboard.png";
	t[TEX_TYPE_UI_TAG]/*							                    */ = "rom:/texture/ui/tag.png";
	t[TEX_TYPE_UI_DETAIL0]/*							             */ = "rom:/texture/ui/In_Detail_UI.png";
	t[TEX_TYPE_UI_DETAIL1]/*							             */ = "rom:/texture/ui/In_Detail_UI2.png";
	t[TEX_TYPE_UI_DETAIL2]/*							             */ = "rom:/texture/ui/In_Detail_UI3.png";

	t[TEX_TYPE_UI_ICON_PLAYER]/*						*/ = "rom:/texture/ui/icon_player.png";
	t[TEX_TYPE_UI_ICON_ENEMY_SMALL]/*					*/ = "rom:/texture/ui/icon_enemy_small.png";
	t[TEX_TYPE_UI_ICON_ENEMY_MIDIUM]/*					*/ = "rom:/texture/ui/icon_enemy_midium.png";
	t[TEX_TYPE_UI_ICON_ENEMY_BOSS]/*					*/ = "rom:/texture/ui/icon_enemy_boss.png";

	t[TEX_TYPE_EFFECT_FLASH_GENERAL]/*					*/ = "rom:/texture/effect/flashGeneral.tga";
	t[TEX_TYPE_EFFECT_FLASH_RING]/*						*/ = "rom:/texture/effect/flashRing.tga";
	t[TEX_TYPE_EFFECT_FLASH_GENERAL_ENEM]/*				*/ = "rom:/texture/effect/flashGeneral_enem.tga";
	t[TEX_TYPE_EFFECT_FLASH_RING_ENEM]/*				*/ = "rom:/texture/effect/flashRing_enem.tga";
	t[TEX_TYPE_EFFECT_TWINKLE]/*						*/ = "rom:/texture/effect/twinkle.tga";
    //t[TEX_TYPE_EFFECT_SPARK]/*							*/ = "rom:/texture/effect/spark.tga";
    t[TEX_TYPE_EFFECT_SPARK]/*							*/ = "rom:/texture/effect/spark.png";

	
	//t[TEX_TYPE_EFFECT_HUDETEST]/*							*/ = "rom:/texture/effect/hudetest.png";
	//t[TEX_TYPE_EFFECT_HUDETEST2]/*							*/ = "rom:/texture/effect/hudetest2.png";

	t[TEX_TYPE_EFFECT_INK_TEST_01]/*						*/ = "rom:/texture/effect/tex_ink_01.png";

	//t[TEX_TYPE_EFFECT_KAKOPON_HIT_001]/*					*/ = "rom:/texture/effect/kakopon_hit_001.png";
	t[TEX_TYPE_EFFECT_KAKOPON_HIT_002]/*					*/ = "rom:/texture/effect/tex_anim_02.png";

	t[TEX_TYPE_PLAYERTEST_DOWN]/*							*/ = "rom:/texture/player/character_3dtest_down.png";
	t[TEX_TYPE_PLAYERTEST_LEFT]/*							*/ = "rom:/texture/player/character_3dtest_left.png";

	t[TEX_TYPE_OBJECT_ISEKI_ZANGAI_0]/*						*/ = "rom:/texture/object/iseki/zangai_0.png";
	t[TEX_TYPE_OBJECT_ISEKI_ZANGAI_1]/*						*/ = "rom:/texture/object/iseki/zangai_1.png";
	t[TEX_TYPE_OBJECT_ISEKI_ZANGAI_2]/*						*/ = "rom:/texture/object/iseki/zangai_2.png";
	t[TEX_TYPE_OBJECT_ISEKI_DAIZA_0]/*						*/ = "rom:/texture/object/iseki/daiza_0.png";
	t[TEX_TYPE_OBJECT_ISEKI_HASHIRA_0]/*					*/ = "rom:/texture/object/iseki/hashira_0.png";
	t[TEX_TYPE_OBJECT_ISEKI_HASHIRA_TAORETA_0]/*			*/ = "rom:/texture/object/iseki/hashira_taoreta_0.png";
	t[TEX_TYPE_OBJECT_ISEKI_KANATSUBAN_0]/*					*/ = "rom:/texture/object/iseki/kanatsuban_0.png";

	t[TEX_TYPE_BLOCK_tile_center]/*							*/ = "rom:/texture/block/iseki/tile_center.png";
	t[TEX_TYPE_BLOCK_tile_down]/*							*/ = "rom:/texture/block/iseki/tile_down.png";
	t[TEX_TYPE_BLOCK_tile_downL]/*							*/ = "rom:/texture/block/iseki/tile_downL.png";
	t[TEX_TYPE_BLOCK_tile_downR]/*							*/ = "rom:/texture/block/iseki/tile_downR.png";
	t[TEX_TYPE_BLOCK_tile_L]/*								*/ = "rom:/texture/block/iseki/tile_L.png";
	t[TEX_TYPE_BLOCK_tile_R]/*								*/ = "rom:/texture/block/iseki/tile_R.png";
	t[TEX_TYPE_BLOCK_tile_upL]/*							*/ = "rom:/texture/block/iseki/tile_upL.png";
	t[TEX_TYPE_BLOCK_tile_upR]/*							*/ = "rom:/texture/block/iseki/tile_upR.png";
	t[TEX_TYPE_BLOCK_tile1]/*								*/ = "rom:/texture/block/iseki/tile1.png";
	t[TEX_TYPE_BLOCK_tile2]/*								*/ = "rom:/texture/block/iseki/tile2.png";
	t[TEX_TYPE_BLOCK_tile3]/*								*/ = "rom:/texture/block/iseki/tile3.png";
	t[TEX_TYPE_BLOCK_tile4]/*								*/ = "rom:/texture/block/iseki/tile4.png";

	t[TEX_TYPE_CHARACTER_KEM]/*							*/ = "rom:/texture/player/KEM.png";
	t[TEX_TYPE_CHARACTER_SELECT_ONE]/*							*/ = "rom:/texture/player/Character1.png";
	t[TEX_TYPE_CHARACTER_SELECT_TWO]/*							*/ = "rom:/texture/player/Character2.png";
	t[TEX_TYPE_BG_CHARACTER_SELECT]/*							*/ = "rom:/texture/bg/CharaSelect.tga";

	t[TEX_TYPE_CHARACTER_STATUS_ONE]/*							*/ = "rom:/texture/bg/status01.png";
	t[TEX_TYPE_CHARACTER_STATUS_TWO]/*							*/ = "rom:/texture/bg/status02.png";
	t[TEX_TYPE_CHARACTER_STATUS_THREE]/*							*/ = "rom:/texture/bg/status03.png";

	
	t[TEX_TYPE_UI_RESULT_CLEAR]/*								*/ = "rom:/texture/ui/result_clear.tga";
	t[TEX_TYPE_BG_TESTRESULT_CLER]/*							*/ = "rom:/texture/bg/White.png";
	t[TEX_TYPE_BG_TESTRESULT_GAMEOVER]/*						*/ = "rom:/texture/bg/Black.png";
	t[TEX_TYPE_UI_GAMEOVER_TEXT]/*								*/ = "rom:/texture/ui/GameOver.png";
	t[TEX_TYPE_UI_SCORE_TEXT]/*									*/ = "rom:/texture/ui/ScoreText.png";
	t[TEX_TYPE_UI_TITLE_TEXT]/*						　　		*/ = "rom:/texture/ui/TitleText.png";
	t[TEX_TYPE_UI_REPLAY_TEXT]/*								*/ = "rom:/texture/ui/ReplayText.png";
	t[TEX_TYPE_UI_GAMEOVER_TEXT]/*								*/ = "rom:/texture/ui/GameOver.png";
	t[TEX_TYPE_UI_SCORE_TEXT]/*									*/ = "rom:/texture/ui/ScoreText.png";

	t[TEX_TYPE_TITLE_TITLETEXT]									   = "rom:/texture/ui/Title_TitleName_text.png";
	t[TEX_TYPE_TITLE_GAMESTARTTEXT]								   = "rom:/texture/ui/Title_GameStart_text.png";
	t[TEX_TYPE_TITLE_RANKINGTEXT]								   = "rom:/texture/ui/Title_Ranking_text.png";
	t[TEX_TYPE_TITLE_EXITTEXT]									   = "rom:/texture/ui/Title_Exit_text.png";

	t[TEX_TYPE_UI_RETIRE]									       = "rom:/texture/ui/Pause_Retire_Test.png";

    t[TEX_TYPE_TITLE_CONFIRM_TITLETEXT]                            = "rom:/texture/ui/Syuuryou_text.png";
    t[TEX_TYPE_TITLE_CONFIRM_YESTEXT]                              = "rom:/texture/ui/Hai_text.png";
    t[TEX_TYPE_TITLE_CONFIRM_NOTEXT]							   = "rom:/texture/ui/Iie_text.png";
}


// ===================================================
// マクロ定義
// ===================================================
#define TEX_DATA_PATH_MAX (256)
// ===================================================
// 構造体宣言
// ===================================================
struct TEX_DAT {
	bool enable;
	char texturePath[TEX_DATA_PATH_MAX];

	unsigned int textureId;

	//unsigned int textureIdGray;

    //DirectXで読み込んだテクスチャのID
    //これは基本使わないで
    unsigned int textureId_DX;

	int width;
	int height;
};

struct TEX_DAT_LOAD_BUFF {
    bool enable;
    bool unload;
    unsigned int textureId;
};

// ===================================================
// プロトタイプ宣言
// ===================================================

void InitTexData();
void UninitTexData();

//文字列での検索と比べると軽量
TEX_DAT GetTexData(TEX_TYPE type);

//文字列での検索はやや重いです。
//Updateで呼ぶと重大なパフォーマンス低下を引き起こすので
//これはInitで呼ぶようにしてください。
//TEX_DAT GetTexData(std::string name);

//テクスチャデータのロード
//使い終わったデータは UnloadTexData で解放してください
//これは絶対にUpdateの中で呼ばないでください
TEX_DAT LoadTexData(std::string name);
TEX_DAT LoadTexData(unsigned int index);

//テクスチャデータの解放
void UnloadTexData(unsigned int id);
void UnloadTexData(std::string name);
void UnloadTexData(TEX_DAT texdat);


unsigned int GetTexId(unsigned int index);
std::vector<unsigned int> GetTexIdAll();


void TexDataBuffClear();
TEX_DAT_LOAD_BUFF* GetTexDataBuff();

#endif