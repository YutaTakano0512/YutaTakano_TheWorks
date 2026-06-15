// ===================================================
// soundData.h サウンドのセット
// 
// hara sougo  2024/08/22
// ===================================================
#ifndef _SOUND_DATA_H_
#define _SOUND_DATA_H_

#include "../Systems/sys_general.h"
#include "../Systems/sound.h"

#ifdef SWITCH_MODE
#include "../sound_switch.h"
typedef nn::atk::SoundArchive::ItemId SOUNDLIST;
#else
typedef std::string SOUNDLIST;
#endif

#include <iostream>


constexpr auto SOUND_PATH = "rom\\sound";

// ===================================================
// サウンドの種類を宣言
// ===================================================
enum SOUND_TYPE {
	SOUND_TYPE_TEST = 0,

	//SOUND_TYPE_BGM_1,
	//SOUND_TYPE_BGM_2,
	SOUND_TYPE_BGM_3,
    SOUND_TYPE_BGM_4,
    SOUND_TYPE_BGM_5,

	//SOUND_TYPE_SE_1,
	SOUND_TYPE_SE_2,
	//SOUND_TYPE_SE_3,
	SOUND_TYPE_SE_4,
	//SOUND_TYPE_SE_5,
	//SOUND_TYPE_SE_6,
	//SOUND_TYPE_SE_7,
	//SOUND_TYPE_SE_8,
	SOUND_TYPE_SE_9,
    //SOUND_TYPE_SE_10,

	//美澄は気づくかな
	//SOUND_TYPE_SE_EX,

    SOUND_TYPE_SE_UI_SELECT,
    SOUND_TYPE_SE_UI_OK,
    SOUND_TYPE_SE_UI_CANCEL,

    SOUND_TYPE_SE_UI_SELECT_SCENE_ENTER,
    SOUND_TYPE_SE_UI_SELECT_SCENE_ENTER2,
    SOUND_TYPE_SE_UI_SELECT_SCENE_EXIT,
    SOUND_TYPE_SE_UI_SELECT_SCENE_STAMP,

    SOUND_TYPE_SE_UI_,

	SOUND_TYPE_MAX,
};

struct SOUNDLISTPRESET {
    SOUNDLIST sound;
    float volume;
    float pitch;
};

//インライン関数
inline void SoundDataSet(SOUNDLISTPRESET* s)
{
#ifdef SWITCH_MODE //SWITCH

#else //DX

	//SE
    s[SOUND_TYPE_TEST]/*						*/ = { "SE/_none", 1.0f, 1.0f };

	//s[SOUND_TYPE_SE_1]/*						*/ = { "SE/Test_SE_Walk", 1.0f, 1.0f };
	s[SOUND_TYPE_SE_2]/*						*/ = { "SE/Test_SE_Draw", 1.0f, 1.0f };
	//s[SOUND_TYPE_SE_3]/*						*/ = { "SE/Test_SE_Attack", 1.0f, 1.0f };
	s[SOUND_TYPE_SE_4]/*						*/ = { "SE/Test_SE_Damage", 1.0f, 1.0f };
    //s[SOUND_TYPE_SE_5]/*						*/ = { "SE/Test_SE_Select", 1.0f, 1.0f };
    //s[SOUND_TYPE_SE_6]/*						*/ = { "SE/Test_SE_Deside", 1.0f, 1.0f };
    //s[SOUND_TYPE_SE_7]/*						*/ = { "SE/Test_SE_Cancel", 1.0f, 1.0f };
    //s[SOUND_TYPE_SE_8]/*						*/ = { "SE/Test_SE_Detail", 1.0f, 1.0f };
	s[SOUND_TYPE_SE_9]/*						*/ = { "SE/Test_SE_FinalAttack", 1.0f, 1.0f };
    //s[SOUND_TYPE_SE_10]/*						*/ = { "SE/Test_SE_FinalAttack", 1.0f, 1.0f };

	//s[SOUND_TYPE_SE_EX]/*						*/ = { "SE/Test_SE_Extra", 1.0f, 1.0f };

    s[SOUND_TYPE_SE_UI_SELECT]/*				*/ = { "SE/ui/general/select", 0.25f / 2.0f, 1.0f };
    s[SOUND_TYPE_SE_UI_OK]/*					*/ = { "SE/ui/general/ok", 0.24f / 2.0f, 1.0f };
    s[SOUND_TYPE_SE_UI_CANCEL]/*				*/ = { "SE/ui/general/cancel", 0.8f / 2.0f, 1.0f };

    s[SOUND_TYPE_SE_UI_SELECT_SCENE_ENTER]/*				*/ = { "SE/ui/selectscene/paper2", 0.7f, 0.9f };
    s[SOUND_TYPE_SE_UI_SELECT_SCENE_ENTER2]/*				*/ = { "SE/ui/selectscene/paper3", 0.7f, 0.9f };
    s[SOUND_TYPE_SE_UI_SELECT_SCENE_EXIT]/*					*/ = { "SE/ui/selectscene/paper1", 0.7f, 0.9f };
    s[SOUND_TYPE_SE_UI_SELECT_SCENE_STAMP]/*				*/ = { "SE/ui/selectscene/stamp",  0.9f, 1.0f };

	//BGM
	//s[SOUND_TYPE_BGM_1]/*				       	*/ = { "BGM/Test_BGM_Title", 1.0f, 1.0f };
	//s[SOUND_TYPE_BGM_2]/*				       	*/ = { "BGM/Test_BGM_Game", 1.0f, 1.0f };
	// s[SOUND_TYPE_BGM_3]/*				    */ = { "sound/BGM/Test_BGM_Clear", 1.0f, 1.0f };
    s[SOUND_TYPE_BGM_3]/*				       	*/ = { "BGM/Result_BGM_loop", 1.0f, 1.0f };
    s[SOUND_TYPE_BGM_4]/*				       	*/ = { "BGM/Result_BGM_Fanfare", 1.0f, 1.0f };
    s[SOUND_TYPE_BGM_5]/*                       */ = { "BGM/BGM_Gameover", 1.0f, 1.0f };

#endif
}

// ===================================================
// プロトタイプ宣言
// ===================================================

size_t InitSoundDataFiles();
void InitSoundData();
void UninitSoundData();

SOUND_ID GetSoundData(SOUND_TYPE type);
float GetSoundDataPreset_Volume(SOUND_TYPE type);
float GetSoundDataPreset_Pitch(SOUND_TYPE type);

SOUND_ID GetSoundData(std::string name);

std::vector<SOUND_ID> GetSoundIdAll();

#endif