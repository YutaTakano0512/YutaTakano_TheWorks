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

    TEX_TYPE_UI_ICON_PLAYER,
    TEX_TYPE_UI_ICON_ENEMY_SMALL,
    TEX_TYPE_UI_ICON_ENEMY_MIDIUM,
    TEX_TYPE_UI_ICON_ENEMY_BOSS,

	//TEX_TYPE_BG_TESTTITLE,
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

    //ステージセレクト画面
    TEX_TYPE_EFFECT_MAP,
    TEX_TYPE_UI_PIN,
    // TEX_TYPE_BG_STAGESELECT,
    // TEX_TYPE_UI_TAG,
    // TEX_TYPE_UI_DETAIL2,
    TEX_TYPE_BG_OBJECT_COLOR,
    TEX_TYPE_BG_OBJECT_LINE,
    TEX_TYPE_BG_MAP,
    TEX_TYPE_UI_NOSTAGE,
	//TEX_TYPE_BLOCK_tile_center,
	//TEX_TYPE_BLOCK_tile_down,
	//TEX_TYPE_BLOCK_tile_downL,
	//TEX_TYPE_BLOCK_tile_downR,
	//TEX_TYPE_BLOCK_tile_L,
	//TEX_TYPE_BLOCK_tile_R,
	//TEX_TYPE_BLOCK_tile_upL,
	//TEX_TYPE_BLOCK_tile_upR,
	//TEX_TYPE_BLOCK_tile1,
	//TEX_TYPE_BLOCK_tile2,
	//TEX_TYPE_BLOCK_tile3,
	//TEX_TYPE_BLOCK_tile4,

	//キャラクター詳細
	TEX_TYPE_CHARACTER_STATUS_TWO,
	TEX_TYPE_CHARACTER_STATUS_THREE,

	//リザルト画面
	
	/*TEX_TYPE_BG_TESTRESULT_CLER,
	TEX_TYPE_BG_TESTRESULT_GAMEOVER,
    
	TEX_TYPE_UI_RESULT_CLEAR,
	TEX_TYPE_UI_TITLE_TEXT,
	TEX_TYPE_UI_REPLAY_TEXT,
	TEX_TYPE_UI_SCORE_TEXT,
	TEX_TYPE_UI_GAMEOVER_TEXT,*/
    TEX_TYPE_BG_RESULT,
    TEX_TYPE_UI_RESULT_TITLE_TEXT,
    TEX_TYPE_UI_RESULT_TITLE_TEXT_ON,
    TEX_TYPE_UI_RESULT_RANKING_TEXT,
    TEX_TYPE_UI_RESULT_RANKING_TEXT_ON,
    TEX_TYPE_UI_RESULT_REPLAY_TEXTON,
    TEX_TYPE_UI_RESULT_PAPER,
    TEX_TYPE_UI_RESULT_CLERSTAMP,
    TEX_TYPE_UI_RESULT_FAILURESTAMP,
    TEX_TYPE_UI_RESULT_SCORE_RANK_SS,
    TEX_TYPE_UI_RESULT_SCORE_RANK_S,
    TEX_TYPE_UI_RESULT_SCORE_RANK_A,
    TEX_TYPE_UI_RESULT_SCORE_RANK_B,
    TEX_TYPE_UI_RESULT_SCORE_RANK_C,
    TEX_TYPE_UI_RESULT_RANKING_PAPER,
    TEX_TYPE_UI_RESULT_RANKING_RESULT_TEXT,
    TEX_TYPE_UI_RESULT_RESULT_TEXT_ON,
    TEX_TYPE_UI_RESULT_SELECT_PAPER,
    TEX_TYPE_UI_RESULT_NORMAL,
    TEX_TYPE_UI_RESULT_HARD,


//<<<<<<< Updated upstream
//=======
    
    //キャラクター選択画面
    TEX_TYPE_CHARACTER_KEM,
    TEX_TYPE_CHARACTER_SELECT_ONE,
    TEX_TYPE_CHARACTER_SELECT_TWO,
    TEX_TYPE_BG_CHARACTER_SELECT,
//>>>>>>> Stashed changes

    // キャラクター選択画面
    TEX_TYPE_CHARACTER_ON,//キャラ1
    TEX_TYPE_CHARACTER_OFF,//キャラ1OFF

    TEX_TYPE_CHARACTER_KAITO_ON,//カイト君ON
    TEX_TYPE_CHARACTER_KAITO_OFF,//カイト君OFF
    TEX_TYPE_CHARACTER_NEO_ON,//ねおON
    TEX_TYPE_CHARACTER_NEO_OFF,//ねお君OFF
    TEX_TYPE_CHARACTER_KONTO_ON,//コントリウス君ON
    TEX_TYPE_CHARACTER_KONTO_OFF,//コントリウス君OFF
    TEX_TYPE_CHARACTER_KAITO_PAGE,//カイト履歴書
    TEX_TYPE_CHARACTER_NEO_PAGE,//ネオ履歴書
    TEX_TYPE_CHARACTER_KONTO_PAGE,//コントリウス履歴書
    TEX_TYPE_CHARACTER_KAITO_BUKI,//カイト武器
    TEX_TYPE_CHARACTER_NEO_BUKI,//ネオ武器
    TEX_TYPE_CHARACTER_KONTO_BUKI,//コントリウス武器
    TEX_TYPE_CHARACTER_KAKKO,//キャラ選択枠
    TEX_TYPE_CHARACTER_SELECT_UP,//キャラ矢印上
    TEX_TYPE_CHARACTER_SELECT_DOWN,//キャラ矢印下

    TEX_TYPE_CHARACTER_BOOK,//本

    TEX_TYPE_CHARACTER_TITLE,//出撃
    TEX_TYPE_CHARACTER_ILLUST,//立ち絵イラスト
    TEX_TYPE_CHARACTER_TEXT,//テキスト
    TEX_TYPE_CHARACTER_LIGHT,//ライト
    TEX_TYPE_CHARACTER_DESK,//机
    TEX_TYPE_CHARACTER_OBJECT_LINE,//オブジェクトLINE
    TEX_TYPE_CHARACTER_OBJECT_COLOR,//COLOR
    

    //キャラクターステータスBOX
    TEX_TYPE_CHARACTER_HP_ON,
    TEX_TYPE_CHARACTER_HP_OFF,
    TEX_TYPE_CHARACTER_ATK_ON,
    TEX_TYPE_CHARACTER_ATK_OFF,
    TEX_TYPE_CHARACTER_AGL_ON,
    TEX_TYPE_CHARACTER_AGL_OFF,

    //タイトル素材の追加
    //TEX_TYPE_TITLE_TITLETEXT,
    TEX_TYPE_TITLE_GAMESTARTTEXT_ON,
    TEX_TYPE_TITLE_MULTITEXT_ON,
        TEX_TYPE_TITLE_BOSSBATTLETEXT_ON,
        TEX_TYPE_TITLE_SOUNDROOMTEXT_ON,
    TEX_TYPE_TITLE_EXITTEXT_ON,
    TEX_TYPE_TITLE_GAMESTARTTEXT_OFF,
    TEX_TYPE_TITLE_MULTITEXT_OFF,
        TEX_TYPE_TITLE_BOSSBATTLETEXT_OFF,
    TEX_TYPE_TITLE_SOUNDROOMTEXT_OFF,
    TEX_TYPE_TITLE_EXITTEXT_OFF,
    TEX_TYPE_TITLE_NORMAL_ON,
    TEX_TYPE_TITLE_NORMAL_OFF,
    TEX_TYPE_TITLE_NORMAL_TEXT,
    TEX_TYPE_TITLE_HARD_ON,
    TEX_TYPE_TITLE_HARD_OFF,
    TEX_TYPE_TITLE_HARD_TEXT,

    TEX_TYPE_TITLE_MULTI_TEXT,
    TEX_TYPE_TITLE_EXIT_TEXT,

    TEX_TYPE_TITLE_BUTTON_A,
    TEX_TYPE_TITLE_BUTTON_B,

    TEX_TYPE_UI_RETIRE,

    //TEX_TYPE_TITLE_MULTI_TITLETEXT,
    //TEX_TYPE_TITLE_CONFIRM_TITLETEXT,
    TEX_TYPE_TITLE_CONFIRM_YESTEXT_ON,
    TEX_TYPE_TITLE_CONFIRM_YESTEXT_OFF,
    TEX_TYPE_TITLE_CONFIRM_NOTEXT_ON,
    TEX_TYPE_TITLE_CONFIRM_NOTEXT_OFF,
    TEX_TYPE_TITLE_CONFIRM_DIFFICULTY,
    TEX_TYPE_TITLE_CONFIRM_CHECK_Z,

    //タイトル素材追加
    TEX_TYPE_TITLE_COMSEPTART,//タイトルイラスト
    TEX_TYPE_TITLE_LOGO,//タイトルロゴ
    TEX_TYPE_TITLE_LOGO_Z,//タイトルロゴback
    TEX_TYPE_TITLE_LOGO_Y,//タイトルロゴback
    TEX_TYPE_TITLE_BUTTON_Z,//タイトルボタン
    TEX_TYPE_TITLE_BUTTON_Z2,//タイトルボタン2
    TEX_TYPE_TITLE_PRESS_ANY_BUTTON,

    TEX_TYPE_TITLE_UI_PANEL_Z,
    
 
    //汎用素材キャラ
    //TEX_TYPE_BUTTON_A,//ボタンA
    //TEX_TYPE_BUTTON_B,//ボタンB
    //TEX_TYPE_BUTTON_A_OFF,//ボタンAオフ
    //TEX_TYPE_BUTTON_B_OFF,//ボタンBオフ
    //TEX_TYPE_BUTTON_A_ON,//ボタンAオン
    //TEX_TYPE_BUTTON_B_ON,//ボタンBオン
    //TEX_TYPE_COFFEE_ON,//コーヒーON
    //TEX_TYPE_COFFEE_OFF,//コーヒーOFF
    //TEX_TYPE_HANKO_ON,//ハンコON
    //TEX_TYPE_HANKO_OFF,//ハンコOFF
    TEX_TYPE_SMOKE,//コーヒーの煙
    TEX_TYPE_BG_DESK,//デスク
    TEX_TYPE_UI_APPROVAL,//承認
    TEX_TYPE_UI_DESK_OBJECT,//オブジェクトカラー,ライン合体版
    TEX_TYPE_UI_BACK,//全体デスク
    TEX_TYPE_UI_HANKO,//本ハンコ
    TEX_TYPE_UI_COFFEE,//本コーヒー
    TEX_TYPE_UI_STUMP,//スタンプ
    TEX_TYPE_UI_MAP,

    //ステージ選択画面素材
    TEX_TYPE_UI_DETAIL1,//ステージ詳細1
    TEX_TYPE_UI_DETAIL2,//ステージ詳細2
    TEX_TYPE_UI_DETAIL3,//ステージ詳細3
    TEX_TYPE_UI_DETAIL4,//ステージ詳細4
    TEX_TYPE_UI_NORMAL_ON,//ノーマルON
    TEX_TYPE_UI_NORMAL_OFF,//ノーマルOFF
    TEX_TYPE_UI_HARD_ON,//ハードON
    TEX_TYPE_UI_HARD_OFF,//ハードOFF
    TEX_TYPE_UI_BUTTON_A_ON,//ボタンAON
    TEX_TYPE_UI_BUTTON_A_OFF,//ボタンAOFF
    TEX_TYPE_UI_BUTTON_B_ON,//ボタンBON
    TEX_TYPE_UI_BUTTON_B_OFF,//ボタンBOFF
    TEX_TYPE_UI_CURSOR,


    TEX_TYPE_MAX,
};

//インライン関数
inline void TexDataSet(std::string* t)
{
	t[TEX_TYPE_VOID]/*									*/ = "rom:/texture/gray.tga";

	t[TEX_TYPE_BLOCK_GENERAL]/*							*/ = "rom:/texture/block/general.tga";
	t[TEX_TYPE_BLOCK_TEST]/*							*/ = "rom:/texture/block/test_block.tga";

	//t[TEX_TYPE_BG_TESTTITLE]/*							*/ = "rom:/texture/bg/title.png";
	t[TEX_TYPE_BG_TESTN]/*								*/ = "rom:/texture/bg/test_n.tga";
	t[TEX_TYPE_BG_TESTRESULT]/*							*/ = "rom:/texture/bg/test_result.tga";
	t[TEX_TYPE_BG_ISEKI_TEST]/*							*/ = "rom:/texture/bg/iseki_test.png";

	t[TEX_TYPE_UI_HPBAR_MAIN]/*							*/ = "rom:/texture/ui/hpbar_main.tga";
	t[TEX_TYPE_UI_HPBAR_SUB]/*							*/ = "rom:/texture/ui/hpbar_sub.tga";
	t[TEX_TYPE_UI_HPBAR_BG]/*							*/ = "rom:/texture/ui/hpbar_bg.tga";
	t[TEX_TYPE_UI_HPBAR_TEXT]/*							*/ = "rom:/texture/ui/hpbar_text.tga";

	t[TEX_TYPE_UI_PAUSE_TEXT]/*							*/ = "rom:/texture/ui/pause_text.tga";
	t[TEX_TYPE_UI_NUMBER]/*								    */ = "rom:/texture/ui/number.tga";
    //t[TEX_TYPE_UI_WHITEBOARD]/*							*/ = "rom:/texture/ui/whiteboard.png";//削除
	//t[TEX_TYPE_UI_TAG]/*							                    */ = "rom:/texture/ui/tag.png";//削除
	//t[TEX_TYPE_UI_DETAIL0]/*							             */ = "rom:/texture/ui/In_Detail_UI.png";//削除
    //t[TEX_TYPE_UI_DETAIL2]/*							             */ = "rom:/texture/ui/In_Detail_UI3.png";//削除
    //t[TEX_TYPE_UI_DETAIL2]/*							             */ = "rom:/texture/ui/In_Detail_UI2.png";//削除

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

	//t[TEX_TYPE_BLOCK_tile_center]/*							*/ = "rom:/texture/block/iseki/tile_center.png";
	//t[TEX_TYPE_BLOCK_tile_down]/*							*/ = "rom:/texture/block/iseki/tile_down.png";
	//t[TEX_TYPE_BLOCK_tile_downL]/*							*/ = "rom:/texture/block/iseki/tile_downL.png";
	//t[TEX_TYPE_BLOCK_tile_downR]/*							*/ = "rom:/texture/block/iseki/tile_downR.png";
	//t[TEX_TYPE_BLOCK_tile_L]/*								*/ = "rom:/texture/block/iseki/tile_L.png";
	//t[TEX_TYPE_BLOCK_tile_R]/*								*/ = "rom:/texture/block/iseki/tile_R.png";
	//t[TEX_TYPE_BLOCK_tile_upL]/*							*/ = "rom:/texture/block/iseki/tile_upL.png";
	//t[TEX_TYPE_BLOCK_tile_upR]/*							*/ = "rom:/texture/block/iseki/tile_upR.png";
	//t[TEX_TYPE_BLOCK_tile1]/*								*/ = "rom:/texture/block/iseki/tile1.png";
	//t[TEX_TYPE_BLOCK_tile2]/*								*/ = "rom:/texture/block/iseki/tile2.png";
	//t[TEX_TYPE_BLOCK_tile3]/*								*/ = "rom:/texture/block/iseki/tile3.png";
	//t[TEX_TYPE_BLOCK_tile4]/*								*/ = "rom:/texture/block/iseki/tile4.png";

    // ステージ選択
    t[TEX_TYPE_BG_STAGESELECT]/*							                       */ = "rom:/texture/bg/StageSelectBG.png";
    t[TEX_TYPE_EFFECT_MAP]/*							                       */ = "rom:/texture/effect/ui_map.png";
    t[TEX_TYPE_UI_PIN]/*							                       */ = "rom:/texture/ui/stageselect/pin.png";
    t[TEX_TYPE_UI_CURSOR]/*							                       */ = "rom:/texture/ui/stageselect/cursor.png";
    t[TEX_TYPE_BG_MAP]/*							                       */ = "rom:/texture/bg/bg_MAP.png";
    t[TEX_TYPE_BG_DESK]/*                                            */ = "rom:/texture/bg/bg_desk.png";//////
    t[TEX_TYPE_UI_DETAIL1]/*							             */ = "rom:/texture/ui/stageselect/page1_text.png";
    t[TEX_TYPE_UI_DETAIL2]/*							             */ = "rom:/texture/ui/stageselect/page2_text.png";
    t[TEX_TYPE_UI_DETAIL3]/*							             */ = "rom:/texture/ui/stageselect/page3_text.png";
    t[TEX_TYPE_UI_DETAIL4]/*							             */ = "rom:/texture/ui/stageselect/page4_text.png";
    t[TEX_TYPE_UI_NORMAL_ON]/*							             */ = "rom:/texture/ui/stageselect/Normal_ON.png";
    t[TEX_TYPE_UI_NORMAL_OFF]/*							             */ = "rom:/texture/ui/stageselect/Normal_OFF.png";
    t[TEX_TYPE_UI_HARD_ON]/*							             */ = "rom:/texture/ui/stageselect/Hard_ON.png";
    t[TEX_TYPE_UI_HARD_OFF]/*							             */ = "rom:/texture/ui/stageselect/Hard_OFF.png";
    t[TEX_TYPE_UI_NOSTAGE]/*							             */ = "rom:/texture/ui/stageselect/NoStage.png";
    


    //t[TEX_TYPE_CHARACTER_ON]/*							*/ = "rom:/texture/ui/character_select/charactor1_ON.png";//キャラ1
    //t[TEX_TYPE_CHARACTER_OFF]/*							*/ = "rom:/texture/ui/character_select/charactor1_OFF.png";//キャラ1OFF

    //本素材キャラクター
    t[TEX_TYPE_CHARACTER_KAITO_ON]/*							*/ = "rom:/texture/ui/character_select/icon/icon_kaito_0001_kaito_on.png";//カイトオン
    t[TEX_TYPE_CHARACTER_KAITO_OFF]/*							*/ = "rom:/texture/ui/character_select/icon/icon_kaito_0000_kaito_off.png";//カイトオフ
    t[TEX_TYPE_CHARACTER_NEO_ON]/*							*/ = "rom:/texture/ui/character_select/icon/icon_neo_on.png";//ネオオン
    t[TEX_TYPE_CHARACTER_NEO_OFF]/*							*/ = "rom:/texture/ui/character_select/icon/icon_neo_off.png";//ネオオフ
    t[TEX_TYPE_CHARACTER_KONTO_ON]/*							*/ = "rom:/texture/ui/character_select/icon/icon_contrius_on.png";//コントリウスオン
    t[TEX_TYPE_CHARACTER_KONTO_OFF]/*							*/ = "rom:/texture/ui/character_select/icon/icon_contrius_off.png";//コントリウスオフ
    t[TEX_TYPE_CHARACTER_TITLE]/*							*/ = "rom:/texture/ui/character_select/member.png";//本出撃
    t[TEX_TYPE_CHARACTER_KAITO_PAGE]/*							*/ = "rom:/texture/ui/character_select/kaito_page-assets/kaito_page1.png";//カイト履歴書
    t[TEX_TYPE_CHARACTER_NEO_PAGE]/*							*/ = "rom:/texture/ui/character_select/neo_page-assets/neo_page1.png";//ネオ履歴書
    t[TEX_TYPE_CHARACTER_KONTO_PAGE]/*							*/ = "rom:/texture/ui/character_select/contrius_page-assets/contrius_page1.png";//コントリウス履歴書
    t[TEX_TYPE_CHARACTER_KAITO_BUKI]/*							*/ = "rom:/texture/ui/character_select/kaito_page-assets/buki_kaito.png";//カイト武器
    t[TEX_TYPE_CHARACTER_NEO_BUKI]/*							*/ = "rom:/texture/ui/character_select/neo_page-assets/neo_buki.png";//ネオ武器
    t[TEX_TYPE_CHARACTER_KONTO_BUKI]/*							*/ = "rom:/texture/ui/character_select/contrius_page-assets/contrius_buki.png";//コントリウス武器
    t[TEX_TYPE_CHARACTER_KAKKO]/*							*/ = "rom:/texture/ui/character_select/select_kakko.png";//キャラ選択枠
    t[TEX_TYPE_CHARACTER_SELECT_UP]/*							*/ = "rom:/texture/ui/character_select/select_up.png";//キャラ矢印上
    t[TEX_TYPE_CHARACTER_SELECT_DOWN]/*							*/ = "rom:/texture/ui/character_select/select_down.png";//キャラ矢印下
    t[TEX_TYPE_CHARACTER_LIGHT]/*							*/ = "rom:/texture/ui/a/light.png";//ライト

  //  t[TEX_TYPE_CHARACTER_BOOK]/*							*/ = "rom:/texture/ui/character_select/Book.png";//本

    //t[TEX_TYPE_CHARACTER_ILLUST]/*							*/ = "rom:/texture/ui/character_select/charactor_illust.png";//立ち絵イラスト
   // t[TEX_TYPE_CHARACTER_TEXT]/*							*/ = "rom:/texture/ui/character_select/text.png";//テキスト
  
    //t[TEX_TYPE_CHARACTER_DESK]/*							*/ = "rom:/texture/ui/character_select/desk.png";//デスク
   // t[TEX_TYPE_CHARACTER_DESK]/*							*/ = "rom:/texture/ui/a/desk.png";//デスク
   // t[TEX_TYPE_CHARACTER_OBJECT_LINE]/*							*/ = "rom:/texture/ui/sozai/object_line.png";//オブジェクトLINE
  //  t[TEX_TYPE_CHARACTER_OBJECT_COLOR]/*							*/ = "rom:/texture/ui/sozai/object_color.png";//オブジェクトカラー
    //t[TEX_TYPE_UI_APPROVAL]/*							*/ = "rom:/texture/ui/a/approval.png";//承認ハンコ

    //t[TEX_TYPE_CHARACTER_HP_ON]/*							*/ = "rom:/texture/ui/character_select/Box_HP_ON.png";//HP_ON
    //t[TEX_TYPE_CHARACTER_HP_OFF]/*							*/ = "rom:/texture/ui/character_select/Box_HP_OFF.png";//HP_OFF
    //t[TEX_TYPE_CHARACTER_ATK_ON]/*							*/ = "rom:/texture/ui/character_select/Box_ATK_ON.png";//ATK_ON
   // t[TEX_TYPE_CHARACTER_ATK_OFF]/*							*/ = "rom:/texture/ui/character_select/Box_ATK_OFF.png";//ATK_OFF
   // t[TEX_TYPE_CHARACTER_AGL_ON]/*							*/ = "rom:/texture/ui/character_select/Box_AGL_ON.png";//AGL_ON
   // t[TEX_TYPE_CHARACTER_AGL_OFF]/*							*/ = "rom:/texture/ui/character_select/Box_AGL_OFF.png";//AGL_OFF

	//t[TEX_TYPE_CHARACTER_STATUS_TWO]/*							*/ = "rom:/texture/bg/status02.png";
	//t[TEX_TYPE_CHARACTER_STATUS_THREE]/*							*/ = "rom:/texture/bg/status03.png";
   
	//リザルト画面
	//t[TEX_TYPE_UI_RESULT_CLEAR]/*								*/ = "rom:/texture/ui/result/result_clear.tga";
	//t[TEX_TYPE_BG_TESTRESULT_CLER]/*							*/ = "rom:/texture/bg/White.png";
	//t[TEX_TYPE_BG_TESTRESULT_GAMEOVER]/*						*/ = "rom:/texture/bg/Black.png";
	//t[TEX_TYPE_UI_GAMEOVER_TEXT]/*							*/ = "rom:/texture/ui/result/GameOver.png";
	//t[TEX_TYPE_UI_SCORE_TEXT]/*								*/ = "rom:/texture/ui/result/ScoreText.png";
	//t[TEX_TYPE_UI_TITLE_TEXT]/*						　　	*/ = "rom:/texture/ui/result/TitleText.png";
	//t[TEX_TYPE_UI_REPLAY_TEXT]/*								*/ = "rom:/texture/ui/result/ReplayText.png";
	t[TEX_TYPE_UI_RESULT_TITLE_TEXT]/*						　	*/ = "rom:/texture/ui/result/Result_title_text.png";//タイトルテキスト
    t[TEX_TYPE_UI_RESULT_TITLE_TEXT_ON]                            = "rom:/texture/ui/result/Result_title_text_on.png";//オンタイトルテキスト
	t[TEX_TYPE_UI_RESULT_RANKING_TEXT]                             = "rom:/texture/ui/result/Result_ranking_text.png";//ランキングテキスト
    t[TEX_TYPE_UI_RESULT_RANKING_TEXT_ON]                          = "rom:/texture/ui/result/Result_ranking_text_on.png";//オンランキングテキスト
    t[TEX_TYPE_UI_RESULT_RANKING_RESULT_TEXT]                      ="rom:/texture/ui/result/Result_result_text.png";//リザルトテキスト
    t[TEX_TYPE_UI_RESULT_RESULT_TEXT_ON]                           = "rom:/texture/ui/result/Result_result_text_on.png"; //オンリザルトテキスト

    t[TEX_TYPE_BG_RESULT]/*									    */ = "rom:/texture/bg/haikei.png";
    
    t[TEX_TYPE_UI_RESULT_RANKING_PAPER] =/*　　　　　　　　　　　*/ "rom:/texture/ui/result/Result_ranking_paper.png";//ランキングの紙
    
    t[TEX_TYPE_UI_RESULT_PAPER] =/*                             */ "rom:/texture/ui/result/Result_paper.png";//報告書

    t[TEX_TYPE_UI_RESULT_CLERSTAMP] =/*                         */ "rom:/texture/ui/result/stamp.png";//スタンプクリア
    t[TEX_TYPE_UI_RESULT_FAILURESTAMP] =/*                         */ "rom:/texture/ui/result/stamp2.png";//スタンプ失敗
    
    t[TEX_TYPE_UI_RESULT_SCORE_RANK_SS] =/*                     */ "rom:/texture/ui/result/ss.png";//ssランクスタンプ
    t[TEX_TYPE_UI_RESULT_SCORE_RANK_S] =/*                     */ "rom:/texture/ui/result/S.png";//Sランクスタンプ
    t[TEX_TYPE_UI_RESULT_SCORE_RANK_A] =/*                     */ "rom:/texture/ui/result/A.png";//Aランクスタンプ
    t[TEX_TYPE_UI_RESULT_SCORE_RANK_B] =/*                     */ "rom:/texture/ui/result/B.png";//Bランクスタンプ
    t[TEX_TYPE_UI_RESULT_SCORE_RANK_C] =/*                     */ "rom:/texture/ui/result/C.png";//Cランクスタンプ
    t[TEX_TYPE_UI_RESULT_RANKING_PAPER]=/*                       */"rom:/texture/ui/result/Result_ranking_paper.png"; //ランキング時の紙文字
    t[TEX_TYPE_UI_RESULT_SELECT_PAPER]                           ="rom:/texture/ui/result/select_page.png"; //セレクトペーパー
    t[TEX_TYPE_UI_RESULT_NORMAL] = "rom:/texture/ui/result/nomal.png"; //ノーマルペーパー
    t[TEX_TYPE_UI_RESULT_HARD] = "rom:/texture/ui/result/hard.png"; //ハードペーパー
    //タイトル素材
	t[TEX_TYPE_TITLE_GAMESTARTTEXT_ON]								   = "rom:/texture/ui/title/start_on.png";
    t[TEX_TYPE_TITLE_MULTITEXT_ON] = "rom:/texture/ui/title/multi_on.png";
    t[TEX_TYPE_TITLE_BOSSBATTLETEXT_ON] = "rom:/texture/ui/title/bossbattle_on.png";
    t[TEX_TYPE_TITLE_SOUNDROOMTEXT_ON] = "rom:/texture/ui/title/sound_on.png";
	t[TEX_TYPE_TITLE_EXITTEXT_ON]									                      = "rom:/texture/ui/title/end_on.png";
    t[TEX_TYPE_TITLE_GAMESTARTTEXT_OFF]                                 = "rom:/texture/ui/title/start_off.png";
    t[TEX_TYPE_TITLE_MULTITEXT_OFF] = "rom:/texture/ui/title/multi_off.png";
    t[TEX_TYPE_TITLE_BOSSBATTLETEXT_OFF] = "rom:/texture/ui/title/bossbattle_off.png";
    t[TEX_TYPE_TITLE_SOUNDROOMTEXT_OFF] = "rom:/texture/ui/title/sound_off.png";
    t[TEX_TYPE_TITLE_EXITTEXT_OFF] = "rom:/texture/ui/title/end_off.png";

    t[TEX_TYPE_TITLE_HARD_ON] = "rom:/texture/ui/title/hard_on.png";
    t[TEX_TYPE_TITLE_HARD_OFF] = "rom:/texture/ui/title/hard_off.png";
    t[TEX_TYPE_TITLE_HARD_TEXT] = "rom:/texture/ui/title/text_hard.png";
    t[TEX_TYPE_TITLE_NORMAL_ON] = "rom:/texture/ui/title/normal_on.png";
    t[TEX_TYPE_TITLE_NORMAL_OFF] = "rom:/texture/ui/title/normal_off.png";
    t[TEX_TYPE_TITLE_NORMAL_TEXT] = "rom:/texture/ui/title/text_normal.png";
    t[TEX_TYPE_TITLE_MULTI_TEXT] = "rom:/texture/ui/title/text_check_multi.png";
    t[TEX_TYPE_TITLE_EXIT_TEXT] = "rom:/texture/ui/title/text_check_end.png";

    t[TEX_TYPE_TITLE_COMSEPTART]                                 ="rom:/texture/ui/title/comseptart.png";//タイトルイラスト
    t[TEX_TYPE_TITLE_LOGO]                                 ="rom:/texture/ui/title/Title2_Logo.png";//タイトルロゴ
    t[TEX_TYPE_TITLE_LOGO_Z]                                 ="rom:/texture/ui/title/Title2_Logo_Z.png";//タイトルロゴ
    t[TEX_TYPE_TITLE_LOGO_Y]                                 ="rom:/texture/ui/title/Title2_Logo_Y.png";//タイトルロゴ
    t[TEX_TYPE_TITLE_BUTTON_Z]                                 ="rom:/texture/ui/title/Button_Z.png";//タイトルBUTTON
    t[TEX_TYPE_TITLE_BUTTON_Z2]                                 ="rom:/texture/ui/title/difficulty_cursor.png";//タイトルBUTTON

    t[TEX_TYPE_TITLE_BUTTON_A]                                 ="rom:/texture/ui/title/A_Button_0ff.png";//タイトルBUTTON
    t[TEX_TYPE_TITLE_BUTTON_B]                                 ="rom:/texture/ui/title/B_button_off.png";//タイトルBUTTON

	t[TEX_TYPE_UI_RETIRE]									       = "rom:/texture/ui/Pause_Retire_Test.png";

    //t[TEX_TYPE_TITLE_MULTI_TITLETEXT]                            = "rom:/texture/ui/titile/text_check_multi.png";
    //t[TEX_TYPE_TITLE_CONFIRM_TITLETEXT]                            = "rom:/texture/ui/titile/text_check_end.png";
    t[TEX_TYPE_TITLE_CONFIRM_YESTEXT_ON]                              = "rom:/texture/ui/title/yes_on.png";
    t[TEX_TYPE_TITLE_CONFIRM_YESTEXT_OFF]                              = "rom:/texture/ui/title/yes_off.png";
    t[TEX_TYPE_TITLE_CONFIRM_NOTEXT_ON]							   = "rom:/texture/ui/title/no_on.png";
    t[TEX_TYPE_TITLE_CONFIRM_NOTEXT_OFF]							   = "rom:/texture/ui/title/no_off.png";
    t[TEX_TYPE_TITLE_CONFIRM_DIFFICULTY] = "rom:/texture/ui/title/difficulty_Z2.png";
    t[TEX_TYPE_TITLE_CONFIRM_CHECK_Z]                    = "rom:/texture/ui/title/check_Z.png";
    t[TEX_TYPE_TITLE_PRESS_ANY_BUTTON]                    = "rom:/texture/ui/title/any_button.png";

    t[TEX_TYPE_TITLE_UI_PANEL_Z] = "rom:/texture/ui/title/difficulty_Z.png";
    //汎用素材（キャラ選択に使います）
    //t[TEX_TYPE_BUTTON_A] = "rom:/texture/ui/a/ButtonA.png";//ボタンＡ
    //t[TEX_TYPE_BUTTON_B] = "rom:/texture/ui/a/ButtonB.png";//ボタンＢ
    //t[TEX_TYPE_BUTTON_A_OFF] = "rom:/texture/ui/a/A_Button_0ff.png";//ボタンＡ
    //t[TEX_TYPE_BUTTON_B_OFF] = "rom:/texture/ui/a/B_button_off.png";//ボタンＢ
    //t[TEX_TYPE_BUTTON_A_ON] = "rom:/texture/ui/a/A_Button_on.png";//ボタンＡ
    //t[TEX_TYPE_BUTTON_B_ON] = "rom:/texture/ui/a/B_button_on.png";//ボタンＢ
    //t[TEX_TYPE_COFFEE_ON] = "rom:/texture/ui/a/Coffee_ON.png";//コーヒーON
    //t[TEX_TYPE_COFFEE_OFF] = "rom:/texture/ui/a/Coffee_OFF.png";//コーヒーOFF
    //t[TEX_TYPE_HANKO_ON] = "rom:/texture/ui/a/Hanko_ON.png";//ハンコＯＮ
    //t[TEX_TYPE_HANKO_OFF] = "rom:/texture/ui/a/Hanko_OFF.png";//ハンコOFF
   // t[TEX_TYPE_SMOKE] = "rom:/texture/ui/a/coffee_smoke60..png";//コーヒーの煙
    t[TEX_TYPE_UI_DESK_OBJECT] = "rom:/texture/ui/a/desk_object.png";//デスクのオブジェクト
    t[TEX_TYPE_UI_BACK] = "rom:/texture/ui/a/desk2.png";    //全体デスク
    //t[TEX_TYPE_UI_MAP] = "rom:/texture/ui/a/map.png";    //マップ
    t[TEX_TYPE_UI_MAP] = "rom:/texture/ui/a/map.png";    //マップ
  //  t[TEX_TYPE_UI_HANKO] = "rom:/texture/ui/a/stump.png";//本ハンコ
  //  t[TEX_TYPE_UI_COFFEE] = "rom:/texture/ui/a/coffee.png";//本こーひーだ

    t[TEX_TYPE_UI_BUTTON_A_ON]/*							             */ = "rom:/texture/ui/a/A_Button_on.png";//スタンプ＋AボタンのON
    t[TEX_TYPE_UI_BUTTON_A_OFF]/*							             */ = "rom:/texture/ui/a/A_Button_0ff.png";//スタンプ＋AボタンのOFF
    t[TEX_TYPE_UI_BUTTON_B_ON]/*							             */ = "rom:/texture/ui/a/B_button_on.png";//コップ＋BボタンのON
    t[TEX_TYPE_UI_BUTTON_B_OFF]/*							             */ = "rom:/texture/ui/a/B_button_off.png";// コップ＋BボタンのOFF
    t[TEX_TYPE_UI_STUMP]/*							             */ = "rom:/texture/ui/a/stump.png";// スタンプ



   
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

size_t InitTexDataFiles();
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


std::vector<unsigned int> GetTexIdAll();

#endif