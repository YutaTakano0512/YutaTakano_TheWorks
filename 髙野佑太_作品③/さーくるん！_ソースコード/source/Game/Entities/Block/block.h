// ===================================================
// block.h ブロック
// 
// hara sougo  2024/07/22
// ===================================================
#ifndef _BLOCK_H_
#define _BLOCK_H_

//#include "system.h"

#include "../../Systems/sys_general.h"

#include "../../Sprite/spriteAnim.h"
#include "../../Systems/easing.h"

//#define BLOCK_MAX	(4096)
#define BLOCK_MAX	(1024)

#define BLOCK_WIDTH	(96.0f)
#define BLOCK_HIGHT	(96.0f)

enum BLOCK_TYPE {
	BLOCK_TYPE_VOID = 0,
	BLOCK_TYPE_01,
	BLOCK_TYPE_02,
	BLOCK_TYPE_03,
	BLOCK_TYPE_04,
	BLOCK_TYPE_05,
	BLOCK_TYPE_06,
	BLOCK_TYPE_07,
	BLOCK_TYPE_08,
	BLOCK_TYPE_09,
	BLOCK_TYPE_10,

	BLOCK_TYPE_GENERAL_0,
	BLOCK_TYPE_GENERAL_1,

	BLOCK_TYPE_DUMMY,

	//↓ステージオブジェクト

	BLOCK_TYPE_ISEKI_ZANGAI_0,
	BLOCK_TYPE_ISEKI_ZANGAI_1,
	BLOCK_TYPE_ISEKI_ZANGAI_2,
	BLOCK_TYPE_ISEKI_DAIZA_0,
	BLOCK_TYPE_ISEKI_HASHIRA_0,
	BLOCK_TYPE_ISEKI_HASHIRA_TAORETA_0,
	BLOCK_TYPE_ISEKI_HASHIRA_TAORETA_1,
	BLOCK_TYPE_ISEKI_KANATSUBAN_0,


	BLOCK_TYPE_ISEKI_OBJECT_01,
	BLOCK_TYPE_ISEKI_OBJECT_02,
	BLOCK_TYPE_ISEKI_OBJECT_03,
	BLOCK_TYPE_ISEKI_OBJECT_04,
	BLOCK_TYPE_ISEKI_OBJECT_05,
	BLOCK_TYPE_ISEKI_OBJECT_06,
	BLOCK_TYPE_ISEKI_OBJECT_07,
	BLOCK_TYPE_ISEKI_OBJECT_08,
	BLOCK_TYPE_ISEKI_OBJECT_09,
	BLOCK_TYPE_ISEKI_OBJECT_10,
	BLOCK_TYPE_ISEKI_OBJECT_Trap,

    BLOCK_TYPE_ISEKI_0_Block,
    BLOCK_TYPE_ISEKI_0_Daiza,
    BLOCK_TYPE_ISEKI_0_Hasira1,
    BLOCK_TYPE_ISEKI_0_Hasira2,
    BLOCK_TYPE_ISEKI_0_Hasira3,
    BLOCK_TYPE_ISEKI_0_HasiraS1,
    BLOCK_TYPE_ISEKI_0_HasiraS2,
    BLOCK_TYPE_ISEKI_0_HasiraS3,
    BLOCK_TYPE_ISEKI_0_HasiraS4,
    BLOCK_TYPE_ISEKI_0_HasiraYoko1,
    BLOCK_TYPE_ISEKI_0_HasiraYoko2,
    BLOCK_TYPE_ISEKI_0_Kanatuban,
    BLOCK_TYPE_ISEKI_0_Taimatsu,

    BLOCK_TYPE_ISEKI_0_Dark_Block,
    BLOCK_TYPE_ISEKI_0_Dark_Daiza,
    BLOCK_TYPE_ISEKI_0_Dark_Hasira1,
    BLOCK_TYPE_ISEKI_0_Dark_Hasira2,
    BLOCK_TYPE_ISEKI_0_Dark_Hasira3,
    BLOCK_TYPE_ISEKI_0_Dark_HasiraS1,
    BLOCK_TYPE_ISEKI_0_Dark_HasiraS2,
    BLOCK_TYPE_ISEKI_0_Dark_HasiraS3,
    BLOCK_TYPE_ISEKI_0_Dark_HasiraS4,
    BLOCK_TYPE_ISEKI_0_Dark_HasiraYoko1,
    BLOCK_TYPE_ISEKI_0_Dark_HasiraYoko2,
    BLOCK_TYPE_ISEKI_0_Dark_Kanatuban,
    BLOCK_TYPE_ISEKI_0_Dark_Taimatsu,

    BLOCK_TYPE_ISEKI_HAIKEI_01,
    BLOCK_TYPE_ISEKI_HAIKEI_02,
    BLOCK_TYPE_ISEKI_HAIKEI_03,
    BLOCK_TYPE_ISEKI_HAIKEI_04,
    BLOCK_TYPE_ISEKI_HAIKEI_05,
    BLOCK_TYPE_ISEKI_HAIKEI_06,
    BLOCK_TYPE_ISEKI_HAIKEI_07,


	//BLOCK_TYPE_tile_center,
	//BLOCK_TYPE_tile_down,
	//BLOCK_TYPE_tile_downL,
	//BLOCK_TYPE_tile_downR,
	//BLOCK_TYPE_tile_L,
	//BLOCK_TYPE_tile_R,
	//BLOCK_TYPE_tile_upL,
	//BLOCK_TYPE_tile_upR,
	//BLOCK_TYPE_tile1,
	//BLOCK_TYPE_tile2,
	//BLOCK_TYPE_tile3,
	//BLOCK_TYPE_tile4,


	BLOCK_TYPE_MAX,
};

struct BLOCK {
	bool			isUse;
	BLOCK_TYPE		type;

	Float2			pos;
    float posZ;
	Float2			size;
    float sizeZ;

	bool			noCollision;
	bool alwaysUpdate;

	SPRITE_ANIM_DAT sprite;
	DX_MODEL* model;

    bool isBackground;
};


void InitBlock(BLOCK* block);
void InitBlock();

void UninitBlock();
void UpdateBlock();

void DrawBlock();
void DrawBlockData(BLOCK* block);
BLOCK* GetBlocks();
BLOCK* SetBlock(BLOCK dat);

bool DeleteBlock(BLOCK* block);
bool DeleteBlock(int index);
bool DeleteBlockAll();

bool IsCameraInBlock(BLOCK* block);
bool IsCameraInBlock(int cameraIndex, BLOCK* block);

#endif