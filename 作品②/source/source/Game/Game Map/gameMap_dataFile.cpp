//このファイルはエディタ(sceneEditor.cpp)のプログラムで生成しています
//可読性は考慮していません

#include "../Game Object/gameObject.h"
#include "../Entities/Block/block.h"
#include "gameMap_dataFile.h"


static std::string game_map_data_id_string[GAME_MAP_DATA_ID_MAX] = {
	"TEST_MAP0",
	"TESTMAP2",
	"AAAAAA",
	"STAGE_ISEKI",
	"STAGE_ISEKI_TILE",
	"STAGE_ISEKI_00",
	"STAGE_ISEKI_01",
};

GAME_MAP GameMapDataLoad(GAME_MAP_DATA_ID id)
{
	switch (id)
	{
		case GAME_MAP_DATA_ID_TEST_MAP0:
		{
			GAME_MAP gamemaptmp = {};
			GAME_MAP_BLOCK_DAT* blockdatatmp = (GAME_MAP_BLOCK_DAT*)malloc(sizeof(GAME_MAP_BLOCK_DAT) * 30);
			/*ブロックデータ(とても長いので注意)*/ if(blockdatatmp != nullptr) {
				{GAME_MAP_BLOCK_DAT gmbdtmp = {};gmbdtmp.x = -720;gmbdtmp.y = 240;gmbdtmp.block = (int)BLOCK_TYPE_01;blockdatatmp[0] = gmbdtmp;}
				{GAME_MAP_BLOCK_DAT gmbdtmp = {};gmbdtmp.x = -624;gmbdtmp.y = 240;gmbdtmp.block = (int)BLOCK_TYPE_01;blockdatatmp[1] = gmbdtmp;}
				{GAME_MAP_BLOCK_DAT gmbdtmp = {};gmbdtmp.x = -528;gmbdtmp.y = 240;gmbdtmp.block = (int)BLOCK_TYPE_01;blockdatatmp[2] = gmbdtmp;}
				{GAME_MAP_BLOCK_DAT gmbdtmp = {};gmbdtmp.x = -432;gmbdtmp.y = 240;gmbdtmp.block = (int)BLOCK_TYPE_01;blockdatatmp[3] = gmbdtmp;}
				{GAME_MAP_BLOCK_DAT gmbdtmp = {};gmbdtmp.x = -336;gmbdtmp.y = 240;gmbdtmp.block = (int)BLOCK_TYPE_01;blockdatatmp[4] = gmbdtmp;}
				{GAME_MAP_BLOCK_DAT gmbdtmp = {};gmbdtmp.x = -240;gmbdtmp.y = 240;gmbdtmp.block = (int)BLOCK_TYPE_01;blockdatatmp[5] = gmbdtmp;}
				{GAME_MAP_BLOCK_DAT gmbdtmp = {};gmbdtmp.x = -144;gmbdtmp.y = 240;gmbdtmp.block = (int)BLOCK_TYPE_01;blockdatatmp[6] = gmbdtmp;}
				{GAME_MAP_BLOCK_DAT gmbdtmp = {};gmbdtmp.x = -48;gmbdtmp.y = 240;gmbdtmp.block = (int)BLOCK_TYPE_01;blockdatatmp[7] = gmbdtmp;}
				{GAME_MAP_BLOCK_DAT gmbdtmp = {};gmbdtmp.x = 48;gmbdtmp.y = 240;gmbdtmp.block = (int)BLOCK_TYPE_01;blockdatatmp[8] = gmbdtmp;}
				{GAME_MAP_BLOCK_DAT gmbdtmp = {};gmbdtmp.x = 144;gmbdtmp.y = 240;gmbdtmp.block = (int)BLOCK_TYPE_01;blockdatatmp[9] = gmbdtmp;}
				{GAME_MAP_BLOCK_DAT gmbdtmp = {};gmbdtmp.x = 240;gmbdtmp.y = 240;gmbdtmp.block = (int)BLOCK_TYPE_01;blockdatatmp[10] = gmbdtmp;}
				{GAME_MAP_BLOCK_DAT gmbdtmp = {};gmbdtmp.x = 336;gmbdtmp.y = 240;gmbdtmp.block = (int)BLOCK_TYPE_01;blockdatatmp[11] = gmbdtmp;}
				{GAME_MAP_BLOCK_DAT gmbdtmp = {};gmbdtmp.x = 432;gmbdtmp.y = 240;gmbdtmp.block = (int)BLOCK_TYPE_01;blockdatatmp[12] = gmbdtmp;}
				{GAME_MAP_BLOCK_DAT gmbdtmp = {};gmbdtmp.x = 528;gmbdtmp.y = 240;gmbdtmp.block = (int)BLOCK_TYPE_01;blockdatatmp[13] = gmbdtmp;}
				{GAME_MAP_BLOCK_DAT gmbdtmp = {};gmbdtmp.x = 624;gmbdtmp.y = 240;gmbdtmp.block = (int)BLOCK_TYPE_01;blockdatatmp[14] = gmbdtmp;}
				{GAME_MAP_BLOCK_DAT gmbdtmp = {};gmbdtmp.x = 720;gmbdtmp.y = 240;gmbdtmp.block = (int)BLOCK_TYPE_01;blockdatatmp[15] = gmbdtmp;}
				{GAME_MAP_BLOCK_DAT gmbdtmp = {};gmbdtmp.x = 240;gmbdtmp.y = 144;gmbdtmp.block = (int)BLOCK_TYPE_01;blockdatatmp[16] = gmbdtmp;}
				{GAME_MAP_BLOCK_DAT gmbdtmp = {};gmbdtmp.x = 336;gmbdtmp.y = 144;gmbdtmp.block = (int)BLOCK_TYPE_01;blockdatatmp[17] = gmbdtmp;}
				{GAME_MAP_BLOCK_DAT gmbdtmp = {};gmbdtmp.x = 432;gmbdtmp.y = 144;gmbdtmp.block = (int)BLOCK_TYPE_01;blockdatatmp[18] = gmbdtmp;}
				{GAME_MAP_BLOCK_DAT gmbdtmp = {};gmbdtmp.x = 336;gmbdtmp.y = 48;gmbdtmp.block = (int)BLOCK_TYPE_01;blockdatatmp[19] = gmbdtmp;}
				{GAME_MAP_BLOCK_DAT gmbdtmp = {};gmbdtmp.x = 432;gmbdtmp.y = 48;gmbdtmp.block = (int)BLOCK_TYPE_01;blockdatatmp[20] = gmbdtmp;}
				{GAME_MAP_BLOCK_DAT gmbdtmp = {};gmbdtmp.x = 432;gmbdtmp.y = -48;gmbdtmp.block = (int)BLOCK_TYPE_01;blockdatatmp[21] = gmbdtmp;}
				{GAME_MAP_BLOCK_DAT gmbdtmp = {};gmbdtmp.x = 1392;gmbdtmp.y = 48;gmbdtmp.block = (int)BLOCK_TYPE_07;blockdatatmp[22] = gmbdtmp;}
				{GAME_MAP_BLOCK_DAT gmbdtmp = {};gmbdtmp.x = -816;gmbdtmp.y = 240;gmbdtmp.block = (int)BLOCK_TYPE_01;blockdatatmp[23] = gmbdtmp;}
				{GAME_MAP_BLOCK_DAT gmbdtmp = {};gmbdtmp.x = -912;gmbdtmp.y = 240;gmbdtmp.block = (int)BLOCK_TYPE_01;blockdatatmp[24] = gmbdtmp;}
				{GAME_MAP_BLOCK_DAT gmbdtmp = {};gmbdtmp.x = -1008;gmbdtmp.y = 240;gmbdtmp.block = (int)BLOCK_TYPE_01;blockdatatmp[25] = gmbdtmp;}
				{GAME_MAP_BLOCK_DAT gmbdtmp = {};gmbdtmp.x = -1200;gmbdtmp.y = 240;gmbdtmp.block = (int)BLOCK_TYPE_01;blockdatatmp[26] = gmbdtmp;}
				{GAME_MAP_BLOCK_DAT gmbdtmp = {};gmbdtmp.x = -1104;gmbdtmp.y = 240;gmbdtmp.block = (int)BLOCK_TYPE_01;blockdatatmp[27] = gmbdtmp;}
				{GAME_MAP_BLOCK_DAT gmbdtmp = {};gmbdtmp.x = -1392;gmbdtmp.y = 240;gmbdtmp.block = (int)BLOCK_TYPE_01;blockdatatmp[28] = gmbdtmp;}
				{GAME_MAP_BLOCK_DAT gmbdtmp = {};gmbdtmp.x = -1296;gmbdtmp.y = 240;gmbdtmp.block = (int)BLOCK_TYPE_01;blockdatatmp[29] = gmbdtmp;}}
			GAME_MAP_BLOCK_LIST_DAT gamemapblocktmp = {};
			gamemapblocktmp.count = 30;
			gamemapblocktmp.blocks = blockdatatmp;
			gamemaptmp.blockDat = gamemapblocktmp;
			GAME_MAP_OBJ_DAT* objdatatmp = (GAME_MAP_OBJ_DAT*)malloc(sizeof(GAME_MAP_OBJ_DAT) * 3);
			/*オブジェクトデータ(とても長いので注意)*/ if(objdatatmp != nullptr) {{GAME_MAP_OBJ_DAT gmodtmp = {};gmodtmp.x = -1008;gmodtmp.y = 48;gmodtmp.obj = (int)GAME_OBJECT_TYPE_TEST_2 ;objdatatmp[0] = gmodtmp;}{GAME_MAP_OBJ_DAT gmodtmp = {};gmodtmp.x = -720;gmodtmp.y = 48;gmodtmp.obj = (int)GAME_OBJECT_TYPE_TEST_1 ;objdatatmp[1] = gmodtmp;}{GAME_MAP_OBJ_DAT gmodtmp = {};gmodtmp.x = -432;gmodtmp.y = 48;gmodtmp.obj = (int)GAME_OBJECT_TYPE_TEST ;objdatatmp[2] = gmodtmp;}}
			GAME_MAP_OBJ_LIST_DAT gamemapobjtmp = {};
			gamemapobjtmp.count = 3;
			gamemapobjtmp.objects = objdatatmp;
			gamemaptmp.objDat = gamemapobjtmp;
			return gamemaptmp;
		}
		case GAME_MAP_DATA_ID_TESTMAP2:
		{
			GAME_MAP gamemaptmp = {};
			GAME_MAP_BLOCK_DAT* blockdatatmp = (GAME_MAP_BLOCK_DAT*)malloc(sizeof(GAME_MAP_BLOCK_DAT) * 44);
			/*ブロックデータ(とても長いので注意)*/ if(blockdatatmp != nullptr) {
				{GAME_MAP_BLOCK_DAT gmbdtmp = {};gmbdtmp.x = -1104;gmbdtmp.y = 432;gmbdtmp.block = (int)BLOCK_TYPE_01;blockdatatmp[0] = gmbdtmp;}
				{GAME_MAP_BLOCK_DAT gmbdtmp = {};gmbdtmp.x = -1104;gmbdtmp.y = 336;gmbdtmp.block = (int)BLOCK_TYPE_01;blockdatatmp[1] = gmbdtmp;}
				{GAME_MAP_BLOCK_DAT gmbdtmp = {};gmbdtmp.x = -1104;gmbdtmp.y = 240;gmbdtmp.block = (int)BLOCK_TYPE_01;blockdatatmp[2] = gmbdtmp;}
				{GAME_MAP_BLOCK_DAT gmbdtmp = {};gmbdtmp.x = -1104;gmbdtmp.y = 144;gmbdtmp.block = (int)BLOCK_TYPE_01;blockdatatmp[3] = gmbdtmp;}
				{GAME_MAP_BLOCK_DAT gmbdtmp = {};gmbdtmp.x = -1104;gmbdtmp.y = 48;gmbdtmp.block = (int)BLOCK_TYPE_01;blockdatatmp[4] = gmbdtmp;}
				{GAME_MAP_BLOCK_DAT gmbdtmp = {};gmbdtmp.x = -1104;gmbdtmp.y = -48;gmbdtmp.block = (int)BLOCK_TYPE_01;blockdatatmp[5] = gmbdtmp;}
				{GAME_MAP_BLOCK_DAT gmbdtmp = {};gmbdtmp.x = -1104;gmbdtmp.y = -144;gmbdtmp.block = (int)BLOCK_TYPE_01;blockdatatmp[6] = gmbdtmp;}
				{GAME_MAP_BLOCK_DAT gmbdtmp = {};gmbdtmp.x = -1104;gmbdtmp.y = -240;gmbdtmp.block = (int)BLOCK_TYPE_01;blockdatatmp[7] = gmbdtmp;}
				{GAME_MAP_BLOCK_DAT gmbdtmp = {};gmbdtmp.x = -1104;gmbdtmp.y = -336;gmbdtmp.block = (int)BLOCK_TYPE_01;blockdatatmp[8] = gmbdtmp;}
				{GAME_MAP_BLOCK_DAT gmbdtmp = {};gmbdtmp.x = 1104;gmbdtmp.y = 432;gmbdtmp.block = (int)BLOCK_TYPE_01;blockdatatmp[9] = gmbdtmp;}
				{GAME_MAP_BLOCK_DAT gmbdtmp = {};gmbdtmp.x = 1104;gmbdtmp.y = 336;gmbdtmp.block = (int)BLOCK_TYPE_01;blockdatatmp[10] = gmbdtmp;}
				{GAME_MAP_BLOCK_DAT gmbdtmp = {};gmbdtmp.x = 1104;gmbdtmp.y = 240;gmbdtmp.block = (int)BLOCK_TYPE_01;blockdatatmp[11] = gmbdtmp;}
				{GAME_MAP_BLOCK_DAT gmbdtmp = {};gmbdtmp.x = 1104;gmbdtmp.y = 144;gmbdtmp.block = (int)BLOCK_TYPE_01;blockdatatmp[12] = gmbdtmp;}
				{GAME_MAP_BLOCK_DAT gmbdtmp = {};gmbdtmp.x = 336;gmbdtmp.y = 528;gmbdtmp.block = (int)BLOCK_TYPE_01;blockdatatmp[13] = gmbdtmp;}
				{GAME_MAP_BLOCK_DAT gmbdtmp = {};gmbdtmp.x = 1104;gmbdtmp.y = 48;gmbdtmp.block = (int)BLOCK_TYPE_01;blockdatatmp[14] = gmbdtmp;}
				{GAME_MAP_BLOCK_DAT gmbdtmp = {};gmbdtmp.x = 432;gmbdtmp.y = 528;gmbdtmp.block = (int)BLOCK_TYPE_01;blockdatatmp[15] = gmbdtmp;}
				{GAME_MAP_BLOCK_DAT gmbdtmp = {};gmbdtmp.x = 528;gmbdtmp.y = 528;gmbdtmp.block = (int)BLOCK_TYPE_01;blockdatatmp[16] = gmbdtmp;}
				{GAME_MAP_BLOCK_DAT gmbdtmp = {};gmbdtmp.x = 1104;gmbdtmp.y = -48;gmbdtmp.block = (int)BLOCK_TYPE_01;blockdatatmp[17] = gmbdtmp;}
				{GAME_MAP_BLOCK_DAT gmbdtmp = {};gmbdtmp.x = 1104;gmbdtmp.y = -144;gmbdtmp.block = (int)BLOCK_TYPE_01;blockdatatmp[18] = gmbdtmp;}
				{GAME_MAP_BLOCK_DAT gmbdtmp = {};gmbdtmp.x = 1104;gmbdtmp.y = -240;gmbdtmp.block = (int)BLOCK_TYPE_01;blockdatatmp[19] = gmbdtmp;}
				{GAME_MAP_BLOCK_DAT gmbdtmp = {};gmbdtmp.x = 1104;gmbdtmp.y = -336;gmbdtmp.block = (int)BLOCK_TYPE_01;blockdatatmp[20] = gmbdtmp;}
				{GAME_MAP_BLOCK_DAT gmbdtmp = {};gmbdtmp.x = -1200;gmbdtmp.y = 528;gmbdtmp.block = (int)BLOCK_TYPE_01;blockdatatmp[21] = gmbdtmp;}
				{GAME_MAP_BLOCK_DAT gmbdtmp = {};gmbdtmp.x = -1104;gmbdtmp.y = 528;gmbdtmp.block = (int)BLOCK_TYPE_01;blockdatatmp[22] = gmbdtmp;}
				{GAME_MAP_BLOCK_DAT gmbdtmp = {};gmbdtmp.x = -1008;gmbdtmp.y = 528;gmbdtmp.block = (int)BLOCK_TYPE_01;blockdatatmp[23] = gmbdtmp;}
				{GAME_MAP_BLOCK_DAT gmbdtmp = {};gmbdtmp.x = -816;gmbdtmp.y = 528;gmbdtmp.block = (int)BLOCK_TYPE_01;blockdatatmp[24] = gmbdtmp;}
				{GAME_MAP_BLOCK_DAT gmbdtmp = {};gmbdtmp.x = -720;gmbdtmp.y = 528;gmbdtmp.block = (int)BLOCK_TYPE_01;blockdatatmp[25] = gmbdtmp;}
				{GAME_MAP_BLOCK_DAT gmbdtmp = {};gmbdtmp.x = -624;gmbdtmp.y = 528;gmbdtmp.block = (int)BLOCK_TYPE_01;blockdatatmp[26] = gmbdtmp;}
				{GAME_MAP_BLOCK_DAT gmbdtmp = {};gmbdtmp.x = -528;gmbdtmp.y = 528;gmbdtmp.block = (int)BLOCK_TYPE_01;blockdatatmp[27] = gmbdtmp;}
				{GAME_MAP_BLOCK_DAT gmbdtmp = {};gmbdtmp.x = -432;gmbdtmp.y = 528;gmbdtmp.block = (int)BLOCK_TYPE_01;blockdatatmp[28] = gmbdtmp;}
				{GAME_MAP_BLOCK_DAT gmbdtmp = {};gmbdtmp.x = -336;gmbdtmp.y = 528;gmbdtmp.block = (int)BLOCK_TYPE_01;blockdatatmp[29] = gmbdtmp;}
				{GAME_MAP_BLOCK_DAT gmbdtmp = {};gmbdtmp.x = -912;gmbdtmp.y = 528;gmbdtmp.block = (int)BLOCK_TYPE_01;blockdatatmp[30] = gmbdtmp;}
				{GAME_MAP_BLOCK_DAT gmbdtmp = {};gmbdtmp.x = -240;gmbdtmp.y = 528;gmbdtmp.block = (int)BLOCK_TYPE_01;blockdatatmp[31] = gmbdtmp;}
				{GAME_MAP_BLOCK_DAT gmbdtmp = {};gmbdtmp.x = -144;gmbdtmp.y = 528;gmbdtmp.block = (int)BLOCK_TYPE_01;blockdatatmp[32] = gmbdtmp;}
				{GAME_MAP_BLOCK_DAT gmbdtmp = {};gmbdtmp.x = -48;gmbdtmp.y = 528;gmbdtmp.block = (int)BLOCK_TYPE_01;blockdatatmp[33] = gmbdtmp;}
				{GAME_MAP_BLOCK_DAT gmbdtmp = {};gmbdtmp.x = 48;gmbdtmp.y = 528;gmbdtmp.block = (int)BLOCK_TYPE_01;blockdatatmp[34] = gmbdtmp;}
				{GAME_MAP_BLOCK_DAT gmbdtmp = {};gmbdtmp.x = 144;gmbdtmp.y = 528;gmbdtmp.block = (int)BLOCK_TYPE_01;blockdatatmp[35] = gmbdtmp;}
				{GAME_MAP_BLOCK_DAT gmbdtmp = {};gmbdtmp.x = 240;gmbdtmp.y = 528;gmbdtmp.block = (int)BLOCK_TYPE_01;blockdatatmp[36] = gmbdtmp;}
				{GAME_MAP_BLOCK_DAT gmbdtmp = {};gmbdtmp.x = 624;gmbdtmp.y = 528;gmbdtmp.block = (int)BLOCK_TYPE_01;blockdatatmp[37] = gmbdtmp;}
				{GAME_MAP_BLOCK_DAT gmbdtmp = {};gmbdtmp.x = 720;gmbdtmp.y = 528;gmbdtmp.block = (int)BLOCK_TYPE_01;blockdatatmp[38] = gmbdtmp;}
				{GAME_MAP_BLOCK_DAT gmbdtmp = {};gmbdtmp.x = 816;gmbdtmp.y = 528;gmbdtmp.block = (int)BLOCK_TYPE_01;blockdatatmp[39] = gmbdtmp;}
				{GAME_MAP_BLOCK_DAT gmbdtmp = {};gmbdtmp.x = 912;gmbdtmp.y = 528;gmbdtmp.block = (int)BLOCK_TYPE_01;blockdatatmp[40] = gmbdtmp;}
				{GAME_MAP_BLOCK_DAT gmbdtmp = {};gmbdtmp.x = 1008;gmbdtmp.y = 528;gmbdtmp.block = (int)BLOCK_TYPE_01;blockdatatmp[41] = gmbdtmp;}
				{GAME_MAP_BLOCK_DAT gmbdtmp = {};gmbdtmp.x = 1104;gmbdtmp.y = 528;gmbdtmp.block = (int)BLOCK_TYPE_01;blockdatatmp[42] = gmbdtmp;}
				{GAME_MAP_BLOCK_DAT gmbdtmp = {};gmbdtmp.x = 1200;gmbdtmp.y = 528;gmbdtmp.block = (int)BLOCK_TYPE_01;blockdatatmp[43] = gmbdtmp;}}
			GAME_MAP_BLOCK_LIST_DAT gamemapblocktmp = {};
			gamemapblocktmp.count = 44;
			gamemapblocktmp.blocks = blockdatatmp;
			gamemaptmp.blockDat = gamemapblocktmp;
			GAME_MAP_OBJ_DAT* objdatatmp = (GAME_MAP_OBJ_DAT*)malloc(sizeof(GAME_MAP_OBJ_DAT) * 0);
			/*オブジェクトデータ(とても長いので注意)*/ if(objdatatmp != nullptr) {}
			GAME_MAP_OBJ_LIST_DAT gamemapobjtmp = {};
			gamemapobjtmp.count = 0;
			gamemapobjtmp.objects = objdatatmp;
			gamemaptmp.objDat = gamemapobjtmp;
			return gamemaptmp;
		}
		case GAME_MAP_DATA_ID_AAAAAA:
		{
			GAME_MAP gamemaptmp = {};
			GAME_MAP_BLOCK_DAT* blockdatatmp = (GAME_MAP_BLOCK_DAT*)malloc(sizeof(GAME_MAP_BLOCK_DAT) * 108);
			/*ブロックデータ(とても長いので注意)*/ if(blockdatatmp != nullptr) {
				{GAME_MAP_BLOCK_DAT gmbdtmp = {};gmbdtmp.x = -1776;gmbdtmp.y = 720;gmbdtmp.block = (int)BLOCK_TYPE_GENERAL_0;blockdatatmp[0] = gmbdtmp;}
				{GAME_MAP_BLOCK_DAT gmbdtmp = {};gmbdtmp.x = -1872;gmbdtmp.y = 48;gmbdtmp.block = (int)BLOCK_TYPE_GENERAL_0;blockdatatmp[1] = gmbdtmp;}
				{GAME_MAP_BLOCK_DAT gmbdtmp = {};gmbdtmp.x = -1776;gmbdtmp.y = 48;gmbdtmp.block = (int)BLOCK_TYPE_GENERAL_0;blockdatatmp[2] = gmbdtmp;}
				{GAME_MAP_BLOCK_DAT gmbdtmp = {};gmbdtmp.x = -1680;gmbdtmp.y = 48;gmbdtmp.block = (int)BLOCK_TYPE_GENERAL_0;blockdatatmp[3] = gmbdtmp;}
				{GAME_MAP_BLOCK_DAT gmbdtmp = {};gmbdtmp.x = -1776;gmbdtmp.y = 144;gmbdtmp.block = (int)BLOCK_TYPE_GENERAL_0;blockdatatmp[4] = gmbdtmp;}
				{GAME_MAP_BLOCK_DAT gmbdtmp = {};gmbdtmp.x = -1776;gmbdtmp.y = -48;gmbdtmp.block = (int)BLOCK_TYPE_GENERAL_0;blockdatatmp[5] = gmbdtmp;}
				{GAME_MAP_BLOCK_DAT gmbdtmp = {};gmbdtmp.x = -1872;gmbdtmp.y = -816;gmbdtmp.block = (int)BLOCK_TYPE_GENERAL_0;blockdatatmp[6] = gmbdtmp;}
				{GAME_MAP_BLOCK_DAT gmbdtmp = {};gmbdtmp.x = -1680;gmbdtmp.y = -816;gmbdtmp.block = (int)BLOCK_TYPE_GENERAL_0;blockdatatmp[7] = gmbdtmp;}
				{GAME_MAP_BLOCK_DAT gmbdtmp = {};gmbdtmp.x = -1776;gmbdtmp.y = -912;gmbdtmp.block = (int)BLOCK_TYPE_GENERAL_0;blockdatatmp[8] = gmbdtmp;}
				{GAME_MAP_BLOCK_DAT gmbdtmp = {};gmbdtmp.x = -1776;gmbdtmp.y = -720;gmbdtmp.block = (int)BLOCK_TYPE_GENERAL_0;blockdatatmp[9] = gmbdtmp;}
				{GAME_MAP_BLOCK_DAT gmbdtmp = {};gmbdtmp.x = -2544;gmbdtmp.y = 816;gmbdtmp.block = (int)BLOCK_TYPE_GENERAL_0;blockdatatmp[10] = gmbdtmp;}
				{GAME_MAP_BLOCK_DAT gmbdtmp = {};gmbdtmp.x = -2544;gmbdtmp.y = 720;gmbdtmp.block = (int)BLOCK_TYPE_GENERAL_0;blockdatatmp[11] = gmbdtmp;}
				{GAME_MAP_BLOCK_DAT gmbdtmp = {};gmbdtmp.x = -2448;gmbdtmp.y = 720;gmbdtmp.block = (int)BLOCK_TYPE_GENERAL_0;blockdatatmp[12] = gmbdtmp;}
				{GAME_MAP_BLOCK_DAT gmbdtmp = {};gmbdtmp.x = -2544;gmbdtmp.y = 144;gmbdtmp.block = (int)BLOCK_TYPE_GENERAL_0;blockdatatmp[13] = gmbdtmp;}
				{GAME_MAP_BLOCK_DAT gmbdtmp = {};gmbdtmp.x = -2448;gmbdtmp.y = 144;gmbdtmp.block = (int)BLOCK_TYPE_GENERAL_0;blockdatatmp[14] = gmbdtmp;}
				{GAME_MAP_BLOCK_DAT gmbdtmp = {};gmbdtmp.x = -2544;gmbdtmp.y = 48;gmbdtmp.block = (int)BLOCK_TYPE_GENERAL_0;blockdatatmp[15] = gmbdtmp;}
				{GAME_MAP_BLOCK_DAT gmbdtmp = {};gmbdtmp.x = -2544;gmbdtmp.y = -432;gmbdtmp.block = (int)BLOCK_TYPE_GENERAL_0;blockdatatmp[16] = gmbdtmp;}
				{GAME_MAP_BLOCK_DAT gmbdtmp = {};gmbdtmp.x = -2448;gmbdtmp.y = -432;gmbdtmp.block = (int)BLOCK_TYPE_GENERAL_0;blockdatatmp[17] = gmbdtmp;}
				{GAME_MAP_BLOCK_DAT gmbdtmp = {};gmbdtmp.x = -2448;gmbdtmp.y = -528;gmbdtmp.block = (int)BLOCK_TYPE_GENERAL_0;blockdatatmp[18] = gmbdtmp;}
				{GAME_MAP_BLOCK_DAT gmbdtmp = {};gmbdtmp.x = -2544;gmbdtmp.y = -1104;gmbdtmp.block = (int)BLOCK_TYPE_GENERAL_0;blockdatatmp[19] = gmbdtmp;}
				{GAME_MAP_BLOCK_DAT gmbdtmp = {};gmbdtmp.x = -2448;gmbdtmp.y = -1104;gmbdtmp.block = (int)BLOCK_TYPE_GENERAL_0;blockdatatmp[20] = gmbdtmp;}
				{GAME_MAP_BLOCK_DAT gmbdtmp = {};gmbdtmp.x = -2448;gmbdtmp.y = -1008;gmbdtmp.block = (int)BLOCK_TYPE_GENERAL_0;blockdatatmp[21] = gmbdtmp;}
				{GAME_MAP_BLOCK_DAT gmbdtmp = {};gmbdtmp.x = -3408;gmbdtmp.y = 720;gmbdtmp.block = (int)BLOCK_TYPE_GENERAL_0;blockdatatmp[22] = gmbdtmp;}
				{GAME_MAP_BLOCK_DAT gmbdtmp = {};gmbdtmp.x = -3408;gmbdtmp.y = 624;gmbdtmp.block = (int)BLOCK_TYPE_GENERAL_0;blockdatatmp[23] = gmbdtmp;}
				{GAME_MAP_BLOCK_DAT gmbdtmp = {};gmbdtmp.x = -3408;gmbdtmp.y = 816;gmbdtmp.block = (int)BLOCK_TYPE_GENERAL_0;blockdatatmp[24] = gmbdtmp;}
				{GAME_MAP_BLOCK_DAT gmbdtmp = {};gmbdtmp.x = -3984;gmbdtmp.y = 816;gmbdtmp.block = (int)BLOCK_TYPE_GENERAL_0;blockdatatmp[25] = gmbdtmp;}
				{GAME_MAP_BLOCK_DAT gmbdtmp = {};gmbdtmp.x = -3984;gmbdtmp.y = 720;gmbdtmp.block = (int)BLOCK_TYPE_GENERAL_0;blockdatatmp[26] = gmbdtmp;}
				{GAME_MAP_BLOCK_DAT gmbdtmp = {};gmbdtmp.x = -3984;gmbdtmp.y = 624;gmbdtmp.block = (int)BLOCK_TYPE_GENERAL_0;blockdatatmp[27] = gmbdtmp;}
				{GAME_MAP_BLOCK_DAT gmbdtmp = {};gmbdtmp.x = -720;gmbdtmp.y = 2160;gmbdtmp.block = (int)BLOCK_TYPE_DUMMY;blockdatatmp[28] = gmbdtmp;}
				{GAME_MAP_BLOCK_DAT gmbdtmp = {};gmbdtmp.x = -624;gmbdtmp.y = 2160;gmbdtmp.block = (int)BLOCK_TYPE_DUMMY;blockdatatmp[29] = gmbdtmp;}
				{GAME_MAP_BLOCK_DAT gmbdtmp = {};gmbdtmp.x = -528;gmbdtmp.y = 2160;gmbdtmp.block = (int)BLOCK_TYPE_DUMMY;blockdatatmp[30] = gmbdtmp;}
				{GAME_MAP_BLOCK_DAT gmbdtmp = {};gmbdtmp.x = -432;gmbdtmp.y = 2160;gmbdtmp.block = (int)BLOCK_TYPE_DUMMY;blockdatatmp[31] = gmbdtmp;}
				{GAME_MAP_BLOCK_DAT gmbdtmp = {};gmbdtmp.x = -336;gmbdtmp.y = 2160;gmbdtmp.block = (int)BLOCK_TYPE_DUMMY;blockdatatmp[32] = gmbdtmp;}
				{GAME_MAP_BLOCK_DAT gmbdtmp = {};gmbdtmp.x = 48;gmbdtmp.y = 1776;gmbdtmp.block = (int)BLOCK_TYPE_DUMMY;blockdatatmp[33] = gmbdtmp;}
				{GAME_MAP_BLOCK_DAT gmbdtmp = {};gmbdtmp.x = 144;gmbdtmp.y = 1776;gmbdtmp.block = (int)BLOCK_TYPE_DUMMY;blockdatatmp[34] = gmbdtmp;}
				{GAME_MAP_BLOCK_DAT gmbdtmp = {};gmbdtmp.x = 240;gmbdtmp.y = 1776;gmbdtmp.block = (int)BLOCK_TYPE_DUMMY;blockdatatmp[35] = gmbdtmp;}
				{GAME_MAP_BLOCK_DAT gmbdtmp = {};gmbdtmp.x = 432;gmbdtmp.y = 1776;gmbdtmp.block = (int)BLOCK_TYPE_DUMMY;blockdatatmp[36] = gmbdtmp;}
				{GAME_MAP_BLOCK_DAT gmbdtmp = {};gmbdtmp.x = -720;gmbdtmp.y = 2064;gmbdtmp.block = (int)BLOCK_TYPE_DUMMY;blockdatatmp[37] = gmbdtmp;}
				{GAME_MAP_BLOCK_DAT gmbdtmp = {};gmbdtmp.x = -624;gmbdtmp.y = 2064;gmbdtmp.block = (int)BLOCK_TYPE_DUMMY;blockdatatmp[38] = gmbdtmp;}
				{GAME_MAP_BLOCK_DAT gmbdtmp = {};gmbdtmp.x = -528;gmbdtmp.y = 2064;gmbdtmp.block = (int)BLOCK_TYPE_DUMMY;blockdatatmp[39] = gmbdtmp;}
				{GAME_MAP_BLOCK_DAT gmbdtmp = {};gmbdtmp.x = -432;gmbdtmp.y = 2064;gmbdtmp.block = (int)BLOCK_TYPE_DUMMY;blockdatatmp[40] = gmbdtmp;}
				{GAME_MAP_BLOCK_DAT gmbdtmp = {};gmbdtmp.x = -336;gmbdtmp.y = 2064;gmbdtmp.block = (int)BLOCK_TYPE_DUMMY;blockdatatmp[41] = gmbdtmp;}
				{GAME_MAP_BLOCK_DAT gmbdtmp = {};gmbdtmp.x = -336;gmbdtmp.y = 1968;gmbdtmp.block = (int)BLOCK_TYPE_DUMMY;blockdatatmp[42] = gmbdtmp;}
				{GAME_MAP_BLOCK_DAT gmbdtmp = {};gmbdtmp.x = -432;gmbdtmp.y = 1968;gmbdtmp.block = (int)BLOCK_TYPE_DUMMY;blockdatatmp[43] = gmbdtmp;}
				{GAME_MAP_BLOCK_DAT gmbdtmp = {};gmbdtmp.x = -528;gmbdtmp.y = 1968;gmbdtmp.block = (int)BLOCK_TYPE_DUMMY;blockdatatmp[44] = gmbdtmp;}
				{GAME_MAP_BLOCK_DAT gmbdtmp = {};gmbdtmp.x = -624;gmbdtmp.y = 1968;gmbdtmp.block = (int)BLOCK_TYPE_DUMMY;blockdatatmp[45] = gmbdtmp;}
				{GAME_MAP_BLOCK_DAT gmbdtmp = {};gmbdtmp.x = -720;gmbdtmp.y = 1968;gmbdtmp.block = (int)BLOCK_TYPE_DUMMY;blockdatatmp[46] = gmbdtmp;}
				{GAME_MAP_BLOCK_DAT gmbdtmp = {};gmbdtmp.x = -720;gmbdtmp.y = 1872;gmbdtmp.block = (int)BLOCK_TYPE_DUMMY;blockdatatmp[47] = gmbdtmp;}
				{GAME_MAP_BLOCK_DAT gmbdtmp = {};gmbdtmp.x = -624;gmbdtmp.y = 1872;gmbdtmp.block = (int)BLOCK_TYPE_DUMMY;blockdatatmp[48] = gmbdtmp;}
				{GAME_MAP_BLOCK_DAT gmbdtmp = {};gmbdtmp.x = -528;gmbdtmp.y = 1872;gmbdtmp.block = (int)BLOCK_TYPE_DUMMY;blockdatatmp[49] = gmbdtmp;}
				{GAME_MAP_BLOCK_DAT gmbdtmp = {};gmbdtmp.x = -432;gmbdtmp.y = 1872;gmbdtmp.block = (int)BLOCK_TYPE_DUMMY;blockdatatmp[50] = gmbdtmp;}
				{GAME_MAP_BLOCK_DAT gmbdtmp = {};gmbdtmp.x = -336;gmbdtmp.y = 1872;gmbdtmp.block = (int)BLOCK_TYPE_DUMMY;blockdatatmp[51] = gmbdtmp;}
				{GAME_MAP_BLOCK_DAT gmbdtmp = {};gmbdtmp.x = -720;gmbdtmp.y = 1776;gmbdtmp.block = (int)BLOCK_TYPE_DUMMY;blockdatatmp[52] = gmbdtmp;}
				{GAME_MAP_BLOCK_DAT gmbdtmp = {};gmbdtmp.x = -624;gmbdtmp.y = 1776;gmbdtmp.block = (int)BLOCK_TYPE_DUMMY;blockdatatmp[53] = gmbdtmp;}
				{GAME_MAP_BLOCK_DAT gmbdtmp = {};gmbdtmp.x = -528;gmbdtmp.y = 1776;gmbdtmp.block = (int)BLOCK_TYPE_DUMMY;blockdatatmp[54] = gmbdtmp;}
				{GAME_MAP_BLOCK_DAT gmbdtmp = {};gmbdtmp.x = -432;gmbdtmp.y = 1776;gmbdtmp.block = (int)BLOCK_TYPE_DUMMY;blockdatatmp[55] = gmbdtmp;}
				{GAME_MAP_BLOCK_DAT gmbdtmp = {};gmbdtmp.x = -336;gmbdtmp.y = 1776;gmbdtmp.block = (int)BLOCK_TYPE_DUMMY;blockdatatmp[56] = gmbdtmp;}
				{GAME_MAP_BLOCK_DAT gmbdtmp = {};gmbdtmp.x = 1392;gmbdtmp.y = 1968;gmbdtmp.block = (int)BLOCK_TYPE_DUMMY;blockdatatmp[57] = gmbdtmp;}
				{GAME_MAP_BLOCK_DAT gmbdtmp = {};gmbdtmp.x = 1488;gmbdtmp.y = 1968;gmbdtmp.block = (int)BLOCK_TYPE_DUMMY;blockdatatmp[58] = gmbdtmp;}
				{GAME_MAP_BLOCK_DAT gmbdtmp = {};gmbdtmp.x = 1584;gmbdtmp.y = 1968;gmbdtmp.block = (int)BLOCK_TYPE_DUMMY;blockdatatmp[59] = gmbdtmp;}
				{GAME_MAP_BLOCK_DAT gmbdtmp = {};gmbdtmp.x = 1680;gmbdtmp.y = 1968;gmbdtmp.block = (int)BLOCK_TYPE_DUMMY;blockdatatmp[60] = gmbdtmp;}
				{GAME_MAP_BLOCK_DAT gmbdtmp = {};gmbdtmp.x = 1776;gmbdtmp.y = 1968;gmbdtmp.block = (int)BLOCK_TYPE_DUMMY;blockdatatmp[61] = gmbdtmp;}
				{GAME_MAP_BLOCK_DAT gmbdtmp = {};gmbdtmp.x = 1872;gmbdtmp.y = 1968;gmbdtmp.block = (int)BLOCK_TYPE_DUMMY;blockdatatmp[62] = gmbdtmp;}
				{GAME_MAP_BLOCK_DAT gmbdtmp = {};gmbdtmp.x = 240;gmbdtmp.y = 1968;gmbdtmp.block = (int)BLOCK_TYPE_DUMMY;blockdatatmp[63] = gmbdtmp;}
				{GAME_MAP_BLOCK_DAT gmbdtmp = {};gmbdtmp.x = 144;gmbdtmp.y = 1968;gmbdtmp.block = (int)BLOCK_TYPE_DUMMY;blockdatatmp[64] = gmbdtmp;}
				{GAME_MAP_BLOCK_DAT gmbdtmp = {};gmbdtmp.x = 48;gmbdtmp.y = 1968;gmbdtmp.block = (int)BLOCK_TYPE_DUMMY;blockdatatmp[65] = gmbdtmp;}
				{GAME_MAP_BLOCK_DAT gmbdtmp = {};gmbdtmp.x = 1968;gmbdtmp.y = 1968;gmbdtmp.block = (int)BLOCK_TYPE_DUMMY;blockdatatmp[66] = gmbdtmp;}
				{GAME_MAP_BLOCK_DAT gmbdtmp = {};gmbdtmp.x = 624;gmbdtmp.y = 1968;gmbdtmp.block = (int)BLOCK_TYPE_DUMMY;blockdatatmp[67] = gmbdtmp;}
				{GAME_MAP_BLOCK_DAT gmbdtmp = {};gmbdtmp.x = 528;gmbdtmp.y = 1968;gmbdtmp.block = (int)BLOCK_TYPE_DUMMY;blockdatatmp[68] = gmbdtmp;}
				{GAME_MAP_BLOCK_DAT gmbdtmp = {};gmbdtmp.x = 432;gmbdtmp.y = 1968;gmbdtmp.block = (int)BLOCK_TYPE_DUMMY;blockdatatmp[69] = gmbdtmp;}
				{GAME_MAP_BLOCK_DAT gmbdtmp = {};gmbdtmp.x = 336;gmbdtmp.y = 1968;gmbdtmp.block = (int)BLOCK_TYPE_DUMMY;blockdatatmp[70] = gmbdtmp;}
				{GAME_MAP_BLOCK_DAT gmbdtmp = {};gmbdtmp.x = 48;gmbdtmp.y = 1872;gmbdtmp.block = (int)BLOCK_TYPE_DUMMY;blockdatatmp[71] = gmbdtmp;}
				{GAME_MAP_BLOCK_DAT gmbdtmp = {};gmbdtmp.x = 144;gmbdtmp.y = 1872;gmbdtmp.block = (int)BLOCK_TYPE_DUMMY;blockdatatmp[72] = gmbdtmp;}
				{GAME_MAP_BLOCK_DAT gmbdtmp = {};gmbdtmp.x = 240;gmbdtmp.y = 1872;gmbdtmp.block = (int)BLOCK_TYPE_DUMMY;blockdatatmp[73] = gmbdtmp;}
				{GAME_MAP_BLOCK_DAT gmbdtmp = {};gmbdtmp.x = 336;gmbdtmp.y = 1872;gmbdtmp.block = (int)BLOCK_TYPE_DUMMY;blockdatatmp[74] = gmbdtmp;}
				{GAME_MAP_BLOCK_DAT gmbdtmp = {};gmbdtmp.x = 432;gmbdtmp.y = 1872;gmbdtmp.block = (int)BLOCK_TYPE_DUMMY;blockdatatmp[75] = gmbdtmp;}
				{GAME_MAP_BLOCK_DAT gmbdtmp = {};gmbdtmp.x = 528;gmbdtmp.y = 1872;gmbdtmp.block = (int)BLOCK_TYPE_DUMMY;blockdatatmp[76] = gmbdtmp;}
				{GAME_MAP_BLOCK_DAT gmbdtmp = {};gmbdtmp.x = 624;gmbdtmp.y = 1872;gmbdtmp.block = (int)BLOCK_TYPE_DUMMY;blockdatatmp[77] = gmbdtmp;}
				{GAME_MAP_BLOCK_DAT gmbdtmp = {};gmbdtmp.x = 2160;gmbdtmp.y = 2832;gmbdtmp.block = (int)BLOCK_TYPE_DUMMY;blockdatatmp[78] = gmbdtmp;}
				{GAME_MAP_BLOCK_DAT gmbdtmp = {};gmbdtmp.x = 2256;gmbdtmp.y = 2832;gmbdtmp.block = (int)BLOCK_TYPE_DUMMY;blockdatatmp[79] = gmbdtmp;}
				{GAME_MAP_BLOCK_DAT gmbdtmp = {};gmbdtmp.x = 2352;gmbdtmp.y = 2832;gmbdtmp.block = (int)BLOCK_TYPE_DUMMY;blockdatatmp[80] = gmbdtmp;}
				{GAME_MAP_BLOCK_DAT gmbdtmp = {};gmbdtmp.x = 2448;gmbdtmp.y = 2832;gmbdtmp.block = (int)BLOCK_TYPE_DUMMY;blockdatatmp[81] = gmbdtmp;}
				{GAME_MAP_BLOCK_DAT gmbdtmp = {};gmbdtmp.x = 2544;gmbdtmp.y = 2832;gmbdtmp.block = (int)BLOCK_TYPE_DUMMY;blockdatatmp[82] = gmbdtmp;}
				{GAME_MAP_BLOCK_DAT gmbdtmp = {};gmbdtmp.x = 2640;gmbdtmp.y = 2832;gmbdtmp.block = (int)BLOCK_TYPE_DUMMY;blockdatatmp[83] = gmbdtmp;}
				{GAME_MAP_BLOCK_DAT gmbdtmp = {};gmbdtmp.x = 2736;gmbdtmp.y = 2832;gmbdtmp.block = (int)BLOCK_TYPE_DUMMY;blockdatatmp[84] = gmbdtmp;}
				{GAME_MAP_BLOCK_DAT gmbdtmp = {};gmbdtmp.x = 2832;gmbdtmp.y = 2832;gmbdtmp.block = (int)BLOCK_TYPE_DUMMY;blockdatatmp[85] = gmbdtmp;}
				{GAME_MAP_BLOCK_DAT gmbdtmp = {};gmbdtmp.x = 2928;gmbdtmp.y = 2832;gmbdtmp.block = (int)BLOCK_TYPE_DUMMY;blockdatatmp[86] = gmbdtmp;}
				{GAME_MAP_BLOCK_DAT gmbdtmp = {};gmbdtmp.x = 2064;gmbdtmp.y = 2832;gmbdtmp.block = (int)BLOCK_TYPE_DUMMY;blockdatatmp[87] = gmbdtmp;}
				{GAME_MAP_BLOCK_DAT gmbdtmp = {};gmbdtmp.x = 2736;gmbdtmp.y = 2256;gmbdtmp.block = (int)BLOCK_TYPE_DUMMY;blockdatatmp[88] = gmbdtmp;}
				{GAME_MAP_BLOCK_DAT gmbdtmp = {};gmbdtmp.x = 2640;gmbdtmp.y = 2256;gmbdtmp.block = (int)BLOCK_TYPE_DUMMY;blockdatatmp[89] = gmbdtmp;}
				{GAME_MAP_BLOCK_DAT gmbdtmp = {};gmbdtmp.x = 2544;gmbdtmp.y = 2256;gmbdtmp.block = (int)BLOCK_TYPE_DUMMY;blockdatatmp[90] = gmbdtmp;}
				{GAME_MAP_BLOCK_DAT gmbdtmp = {};gmbdtmp.x = 2544;gmbdtmp.y = 2160;gmbdtmp.block = (int)BLOCK_TYPE_DUMMY;blockdatatmp[91] = gmbdtmp;}
				{GAME_MAP_BLOCK_DAT gmbdtmp = {};gmbdtmp.x = 2640;gmbdtmp.y = 2160;gmbdtmp.block = (int)BLOCK_TYPE_DUMMY;blockdatatmp[92] = gmbdtmp;}
				{GAME_MAP_BLOCK_DAT gmbdtmp = {};gmbdtmp.x = 2736;gmbdtmp.y = 2160;gmbdtmp.block = (int)BLOCK_TYPE_DUMMY;blockdatatmp[93] = gmbdtmp;}
				{GAME_MAP_BLOCK_DAT gmbdtmp = {};gmbdtmp.x = 528;gmbdtmp.y = 1776;gmbdtmp.block = (int)BLOCK_TYPE_DUMMY;blockdatatmp[94] = gmbdtmp;}
				{GAME_MAP_BLOCK_DAT gmbdtmp = {};gmbdtmp.x = 2064;gmbdtmp.y = 1968;gmbdtmp.block = (int)BLOCK_TYPE_DUMMY;blockdatatmp[95] = gmbdtmp;}
				{GAME_MAP_BLOCK_DAT gmbdtmp = {};gmbdtmp.x = 2160;gmbdtmp.y = 1968;gmbdtmp.block = (int)BLOCK_TYPE_DUMMY;blockdatatmp[96] = gmbdtmp;}
				{GAME_MAP_BLOCK_DAT gmbdtmp = {};gmbdtmp.x = 2256;gmbdtmp.y = 1968;gmbdtmp.block = (int)BLOCK_TYPE_DUMMY;blockdatatmp[97] = gmbdtmp;}
				{GAME_MAP_BLOCK_DAT gmbdtmp = {};gmbdtmp.x = 2352;gmbdtmp.y = 1968;gmbdtmp.block = (int)BLOCK_TYPE_DUMMY;blockdatatmp[98] = gmbdtmp;}
				{GAME_MAP_BLOCK_DAT gmbdtmp = {};gmbdtmp.x = 336;gmbdtmp.y = 1776;gmbdtmp.block = (int)BLOCK_TYPE_DUMMY;blockdatatmp[99] = gmbdtmp;}
				{GAME_MAP_BLOCK_DAT gmbdtmp = {};gmbdtmp.x = 624;gmbdtmp.y = 1776;gmbdtmp.block = (int)BLOCK_TYPE_DUMMY;blockdatatmp[100] = gmbdtmp;}
				{GAME_MAP_BLOCK_DAT gmbdtmp = {};gmbdtmp.x = -1200;gmbdtmp.y = 2544;gmbdtmp.block = (int)BLOCK_TYPE_ISEKI_ZANGAI_0;blockdatatmp[101] = gmbdtmp;}
				{GAME_MAP_BLOCK_DAT gmbdtmp = {};gmbdtmp.x = -1968;gmbdtmp.y = 2832;gmbdtmp.block = (int)BLOCK_TYPE_ISEKI_ZANGAI_1;blockdatatmp[102] = gmbdtmp;}
				{GAME_MAP_BLOCK_DAT gmbdtmp = {};gmbdtmp.x = -2832;gmbdtmp.y = 2544;gmbdtmp.block = (int)BLOCK_TYPE_ISEKI_ZANGAI_2;blockdatatmp[103] = gmbdtmp;}
				{GAME_MAP_BLOCK_DAT gmbdtmp = {};gmbdtmp.x = -1968;gmbdtmp.y = 1968;gmbdtmp.block = (int)BLOCK_TYPE_ISEKI_HASHIRA_0;blockdatatmp[104] = gmbdtmp;}
				{GAME_MAP_BLOCK_DAT gmbdtmp = {};gmbdtmp.x = -2640;gmbdtmp.y = 3216;gmbdtmp.block = (int)BLOCK_TYPE_ISEKI_HASHIRA_TAORETA_0;blockdatatmp[105] = gmbdtmp;}
				{GAME_MAP_BLOCK_DAT gmbdtmp = {};gmbdtmp.x = -2832;gmbdtmp.y = 1776;gmbdtmp.block = (int)BLOCK_TYPE_ISEKI_DAIZA_0;blockdatatmp[106] = gmbdtmp;}
				{GAME_MAP_BLOCK_DAT gmbdtmp = {};gmbdtmp.x = -1488;gmbdtmp.y = 3120;gmbdtmp.block = (int)BLOCK_TYPE_ISEKI_KANATSUBAN_0;blockdatatmp[107] = gmbdtmp;}}
			GAME_MAP_BLOCK_LIST_DAT gamemapblocktmp = {};
			gamemapblocktmp.count = 108;
			gamemapblocktmp.blocks = blockdatatmp;
			gamemaptmp.blockDat = gamemapblocktmp;
			GAME_MAP_OBJ_DAT* objdatatmp = (GAME_MAP_OBJ_DAT*)malloc(sizeof(GAME_MAP_OBJ_DAT) * 21);
			/*オブジェクトデータ(とても長いので注意)*/ if(objdatatmp != nullptr) {{GAME_MAP_OBJ_DAT gmodtmp = {};gmodtmp.x = 48;gmodtmp.y = 48;gmodtmp.obj = (int)GAME_OBJECT_TYPE_TEST ;objdatatmp[0] = gmodtmp;}{GAME_MAP_OBJ_DAT gmodtmp = {};gmodtmp.x = -528;gmodtmp.y = -48;gmodtmp.obj = (int)GAME_OBJECT_TYPE_TEST ;objdatatmp[1] = gmodtmp;}{GAME_MAP_OBJ_DAT gmodtmp = {};gmodtmp.x = -432;gmodtmp.y = 336;gmodtmp.obj = (int)GAME_OBJECT_TYPE_TEST ;objdatatmp[2] = gmodtmp;}{GAME_MAP_OBJ_DAT gmodtmp = {};gmodtmp.x = 336;gmodtmp.y = 336;gmodtmp.obj = (int)GAME_OBJECT_TYPE_TEST ;objdatatmp[3] = gmodtmp;}{GAME_MAP_OBJ_DAT gmodtmp = {};gmodtmp.x = 624;gmodtmp.y = -240;gmodtmp.obj = (int)GAME_OBJECT_TYPE_TEST ;objdatatmp[4] = gmodtmp;}{GAME_MAP_OBJ_DAT gmodtmp = {};gmodtmp.x = -912;gmodtmp.y = -528;gmodtmp.obj = (int)GAME_OBJECT_TYPE_TEST ;objdatatmp[5] = gmodtmp;}{GAME_MAP_OBJ_DAT gmodtmp = {};gmodtmp.x = -1008;gmodtmp.y = 528;gmodtmp.obj = (int)GAME_OBJECT_TYPE_TEST ;objdatatmp[6] = gmodtmp;}{GAME_MAP_OBJ_DAT gmodtmp = {};gmodtmp.x = 1008;gmodtmp.y = 528;gmodtmp.obj = (int)GAME_OBJECT_TYPE_TEST ;objdatatmp[7] = gmodtmp;}{GAME_MAP_OBJ_DAT gmodtmp = {};gmodtmp.x = 1104;gmodtmp.y = -432;gmodtmp.obj = (int)GAME_OBJECT_TYPE_TEST ;objdatatmp[8] = gmodtmp;}{GAME_MAP_OBJ_DAT gmodtmp = {};gmodtmp.x = 336;gmodtmp.y = -528;gmodtmp.obj = (int)GAME_OBJECT_TYPE_TEST ;objdatatmp[9] = gmodtmp;}{GAME_MAP_OBJ_DAT gmodtmp = {};gmodtmp.x = -144;gmodtmp.y = -528;gmodtmp.obj = (int)GAME_OBJECT_TYPE_TEST ;objdatatmp[10] = gmodtmp;}{GAME_MAP_OBJ_DAT gmodtmp = {};gmodtmp.x = 48;gmodtmp.y = 528;gmodtmp.obj = (int)GAME_OBJECT_TYPE_TEST ;objdatatmp[11] = gmodtmp;}{GAME_MAP_OBJ_DAT gmodtmp = {};gmodtmp.x = -528;gmodtmp.y = 1200;gmodtmp.obj = (int)GAME_OBJECT_TYPE_PLAYERTEST_DOWN ;objdatatmp[12] = gmodtmp;}{GAME_MAP_OBJ_DAT gmodtmp = {};gmodtmp.x = 336;gmodtmp.y = 1200;gmodtmp.obj = (int)GAME_OBJECT_TYPE_PLAYERTEST_LEFT ;objdatatmp[13] = gmodtmp;}{GAME_MAP_OBJ_DAT gmodtmp = {};gmodtmp.x = 336;gmodtmp.y = 1968;gmodtmp.obj = (int)GAME_OBJECT_TYPE_ISEKI_ZANGAI_0 ;objdatatmp[14] = gmodtmp;}{GAME_MAP_OBJ_DAT gmodtmp = {};gmodtmp.x = 1104;gmodtmp.y = 1968;gmodtmp.obj = (int)GAME_OBJECT_TYPE_ISEKI_ZANGAI_1 ;objdatatmp[15] = gmodtmp;}{GAME_MAP_OBJ_DAT gmodtmp = {};gmodtmp.x = 1872;gmodtmp.y = 1968;gmodtmp.obj = (int)GAME_OBJECT_TYPE_ISEKI_ZANGAI_2 ;objdatatmp[16] = gmodtmp;}{GAME_MAP_OBJ_DAT gmodtmp = {};gmodtmp.x = -528;gmodtmp.y = 1968;gmodtmp.obj = (int)GAME_OBJECT_TYPE_ISEKI_DAIZA_0 ;objdatatmp[17] = gmodtmp;}{GAME_MAP_OBJ_DAT gmodtmp = {};gmodtmp.x = 2640;gmodtmp.y = 1968;gmodtmp.obj = (int)GAME_OBJECT_TYPE_ISEKI_HASHIRA_0 ;objdatatmp[18] = gmodtmp;}{GAME_MAP_OBJ_DAT gmodtmp = {};gmodtmp.x = 2544;gmodtmp.y = 2832;gmodtmp.obj = (int)GAME_OBJECT_TYPE_ISEKI_HASHIRA_TAORETA_0 ;objdatatmp[19] = gmodtmp;}{GAME_MAP_OBJ_DAT gmodtmp = {};gmodtmp.x = 1392;gmodtmp.y = 2832;gmodtmp.obj = (int)GAME_OBJECT_TYPE_ISEKI_KANATSUBAN_0 ;objdatatmp[20] = gmodtmp;}}
			GAME_MAP_OBJ_LIST_DAT gamemapobjtmp = {};
			gamemapobjtmp.count = 21;
			gamemapobjtmp.objects = objdatatmp;
			gamemaptmp.objDat = gamemapobjtmp;
			return gamemaptmp;
		}
		case GAME_MAP_DATA_ID_STAGE_ISEKI:
		{
			GAME_MAP gamemaptmp = {};
			GAME_MAP_BLOCK_DAT* blockdatatmp = (GAME_MAP_BLOCK_DAT*)malloc(sizeof(GAME_MAP_BLOCK_DAT) * 70);
			/*ブロックデータ(とても長いので注意)*/ if(blockdatatmp != nullptr) {
				{GAME_MAP_BLOCK_DAT gmbdtmp = {};gmbdtmp.x = -6288;gmbdtmp.y = 2736;gmbdtmp.block = (int)BLOCK_TYPE_ISEKI_HASHIRA_TAORETA_1;blockdatatmp[0] = gmbdtmp;}
				{GAME_MAP_BLOCK_DAT gmbdtmp = {};gmbdtmp.x = 240;gmbdtmp.y = -3312;gmbdtmp.block = (int)BLOCK_TYPE_ISEKI_HASHIRA_0;blockdatatmp[1] = gmbdtmp;}
				{GAME_MAP_BLOCK_DAT gmbdtmp = {};gmbdtmp.x = -4944;gmbdtmp.y = -2448;gmbdtmp.block = (int)BLOCK_TYPE_ISEKI_ZANGAI_1;blockdatatmp[2] = gmbdtmp;}
				{GAME_MAP_BLOCK_DAT gmbdtmp = {};gmbdtmp.x = -6096;gmbdtmp.y = -2256;gmbdtmp.block = (int)BLOCK_TYPE_ISEKI_HASHIRA_TAORETA_1;blockdatatmp[3] = gmbdtmp;}
				{GAME_MAP_BLOCK_DAT gmbdtmp = {};gmbdtmp.x = -6288;gmbdtmp.y = -48;gmbdtmp.block = (int)BLOCK_TYPE_ISEKI_HASHIRA_0;blockdatatmp[4] = gmbdtmp;}
				{GAME_MAP_BLOCK_DAT gmbdtmp = {};gmbdtmp.x = -4560;gmbdtmp.y = -3312;gmbdtmp.block = (int)BLOCK_TYPE_ISEKI_ZANGAI_2;blockdatatmp[5] = gmbdtmp;}
				{GAME_MAP_BLOCK_DAT gmbdtmp = {};gmbdtmp.x = -3120;gmbdtmp.y = -3408;gmbdtmp.block = (int)BLOCK_TYPE_ISEKI_HASHIRA_0;blockdatatmp[6] = gmbdtmp;}
				{GAME_MAP_BLOCK_DAT gmbdtmp = {};gmbdtmp.x = -1584;gmbdtmp.y = -3408;gmbdtmp.block = (int)BLOCK_TYPE_ISEKI_HASHIRA_TAORETA_0;blockdatatmp[7] = gmbdtmp;}
				{GAME_MAP_BLOCK_DAT gmbdtmp = {};gmbdtmp.x = -3984;gmbdtmp.y = -2928;gmbdtmp.block = (int)BLOCK_TYPE_ISEKI_ZANGAI_1;blockdatatmp[8] = gmbdtmp;}
				{GAME_MAP_BLOCK_DAT gmbdtmp = {};gmbdtmp.x = -5712;gmbdtmp.y = -3216;gmbdtmp.block = (int)BLOCK_TYPE_ISEKI_ZANGAI_1;blockdatatmp[9] = gmbdtmp;}
				{GAME_MAP_BLOCK_DAT gmbdtmp = {};gmbdtmp.x = -3888;gmbdtmp.y = -3600;gmbdtmp.block = (int)BLOCK_TYPE_ISEKI_ZANGAI_1;blockdatatmp[10] = gmbdtmp;}
				{GAME_MAP_BLOCK_DAT gmbdtmp = {};gmbdtmp.x = 1584;gmbdtmp.y = -3408;gmbdtmp.block = (int)BLOCK_TYPE_ISEKI_ZANGAI_0;blockdatatmp[11] = gmbdtmp;}
				{GAME_MAP_BLOCK_DAT gmbdtmp = {};gmbdtmp.x = -624;gmbdtmp.y = -2928;gmbdtmp.block = (int)BLOCK_TYPE_ISEKI_ZANGAI_1;blockdatatmp[12] = gmbdtmp;}
				{GAME_MAP_BLOCK_DAT gmbdtmp = {};gmbdtmp.x = 1104;gmbdtmp.y = -3024;gmbdtmp.block = (int)BLOCK_TYPE_ISEKI_ZANGAI_1;blockdatatmp[13] = gmbdtmp;}
				{GAME_MAP_BLOCK_DAT gmbdtmp = {};gmbdtmp.x = 1584;gmbdtmp.y = -2448;gmbdtmp.block = (int)BLOCK_TYPE_ISEKI_ZANGAI_1;blockdatatmp[14] = gmbdtmp;}
				{GAME_MAP_BLOCK_DAT gmbdtmp = {};gmbdtmp.x = 2352;gmbdtmp.y = -3216;gmbdtmp.block = (int)BLOCK_TYPE_ISEKI_ZANGAI_1;blockdatatmp[15] = gmbdtmp;}
				{GAME_MAP_BLOCK_DAT gmbdtmp = {};gmbdtmp.x = 4560;gmbdtmp.y = -3312;gmbdtmp.block = (int)BLOCK_TYPE_ISEKI_ZANGAI_2;blockdatatmp[16] = gmbdtmp;}
				{GAME_MAP_BLOCK_DAT gmbdtmp = {};gmbdtmp.x = 5808;gmbdtmp.y = -240;gmbdtmp.block = (int)BLOCK_TYPE_ISEKI_HASHIRA_0;blockdatatmp[17] = gmbdtmp;}
				{GAME_MAP_BLOCK_DAT gmbdtmp = {};gmbdtmp.x = 5808;gmbdtmp.y = -3504;gmbdtmp.block = (int)BLOCK_TYPE_ISEKI_HASHIRA_0;blockdatatmp[18] = gmbdtmp;}
				{GAME_MAP_BLOCK_DAT gmbdtmp = {};gmbdtmp.x = 3504;gmbdtmp.y = -3120;gmbdtmp.block = (int)BLOCK_TYPE_ISEKI_ZANGAI_1;blockdatatmp[19] = gmbdtmp;}
				{GAME_MAP_BLOCK_DAT gmbdtmp = {};gmbdtmp.x = 3984;gmbdtmp.y = -2160;gmbdtmp.block = (int)BLOCK_TYPE_ISEKI_ZANGAI_1;blockdatatmp[20] = gmbdtmp;}
				{GAME_MAP_BLOCK_DAT gmbdtmp = {};gmbdtmp.x = 4464;gmbdtmp.y = -2640;gmbdtmp.block = (int)BLOCK_TYPE_ISEKI_ZANGAI_1;blockdatatmp[21] = gmbdtmp;}
				{GAME_MAP_BLOCK_DAT gmbdtmp = {};gmbdtmp.x = 4944;gmbdtmp.y = -1968;gmbdtmp.block = (int)BLOCK_TYPE_ISEKI_ZANGAI_1;blockdatatmp[22] = gmbdtmp;}
				{GAME_MAP_BLOCK_DAT gmbdtmp = {};gmbdtmp.x = 5232;gmbdtmp.y = -2448;gmbdtmp.block = (int)BLOCK_TYPE_ISEKI_ZANGAI_1;blockdatatmp[23] = gmbdtmp;}
				{GAME_MAP_BLOCK_DAT gmbdtmp = {};gmbdtmp.x = 5904;gmbdtmp.y = -1776;gmbdtmp.block = (int)BLOCK_TYPE_ISEKI_ZANGAI_0;blockdatatmp[24] = gmbdtmp;}
				{GAME_MAP_BLOCK_DAT gmbdtmp = {};gmbdtmp.x = 5040;gmbdtmp.y = 1200;gmbdtmp.block = (int)BLOCK_TYPE_ISEKI_ZANGAI_0;blockdatatmp[25] = gmbdtmp;}
				{GAME_MAP_BLOCK_DAT gmbdtmp = {};gmbdtmp.x = 5712;gmbdtmp.y = 2544;gmbdtmp.block = (int)BLOCK_TYPE_ISEKI_HASHIRA_TAORETA_1;blockdatatmp[26] = gmbdtmp;}
				{GAME_MAP_BLOCK_DAT gmbdtmp = {};gmbdtmp.x = 4272;gmbdtmp.y = 3216;gmbdtmp.block = (int)BLOCK_TYPE_ISEKI_ZANGAI_2;blockdatatmp[27] = gmbdtmp;}
				{GAME_MAP_BLOCK_DAT gmbdtmp = {};gmbdtmp.x = 3120;gmbdtmp.y = 2928;gmbdtmp.block = (int)BLOCK_TYPE_ISEKI_ZANGAI_1;blockdatatmp[28] = gmbdtmp;}
				{GAME_MAP_BLOCK_DAT gmbdtmp = {};gmbdtmp.x = 4944;gmbdtmp.y = 2544;gmbdtmp.block = (int)BLOCK_TYPE_ISEKI_ZANGAI_1;blockdatatmp[29] = gmbdtmp;}
				{GAME_MAP_BLOCK_DAT gmbdtmp = {};gmbdtmp.x = 6096;gmbdtmp.y = 1200;gmbdtmp.block = (int)BLOCK_TYPE_ISEKI_ZANGAI_1;blockdatatmp[30] = gmbdtmp;}
				{GAME_MAP_BLOCK_DAT gmbdtmp = {};gmbdtmp.x = 6576;gmbdtmp.y = -144;gmbdtmp.block = (int)BLOCK_TYPE_ISEKI_ZANGAI_1;blockdatatmp[31] = gmbdtmp;}
				{GAME_MAP_BLOCK_DAT gmbdtmp = {};gmbdtmp.x = 6192;gmbdtmp.y = -1104;gmbdtmp.block = (int)BLOCK_TYPE_ISEKI_ZANGAI_1;blockdatatmp[32] = gmbdtmp;}
				{GAME_MAP_BLOCK_DAT gmbdtmp = {};gmbdtmp.x = 4656;gmbdtmp.y = -624;gmbdtmp.block = (int)BLOCK_TYPE_ISEKI_ZANGAI_1;blockdatatmp[33] = gmbdtmp;}
				{GAME_MAP_BLOCK_DAT gmbdtmp = {};gmbdtmp.x = 4944;gmbdtmp.y = 624;gmbdtmp.block = (int)BLOCK_TYPE_ISEKI_ZANGAI_1;blockdatatmp[34] = gmbdtmp;}
				{GAME_MAP_BLOCK_DAT gmbdtmp = {};gmbdtmp.x = 5040;gmbdtmp.y = 624;gmbdtmp.block = (int)BLOCK_TYPE_ISEKI_ZANGAI_1;blockdatatmp[35] = gmbdtmp;}
				{GAME_MAP_BLOCK_DAT gmbdtmp = {};gmbdtmp.x = 3792;gmbdtmp.y = 1872;gmbdtmp.block = (int)BLOCK_TYPE_ISEKI_ZANGAI_1;blockdatatmp[36] = gmbdtmp;}
				{GAME_MAP_BLOCK_DAT gmbdtmp = {};gmbdtmp.x = 6384;gmbdtmp.y = 3312;gmbdtmp.block = (int)BLOCK_TYPE_ISEKI_ZANGAI_1;blockdatatmp[37] = gmbdtmp;}
				{GAME_MAP_BLOCK_DAT gmbdtmp = {};gmbdtmp.x = 6576;gmbdtmp.y = -3216;gmbdtmp.block = (int)BLOCK_TYPE_ISEKI_ZANGAI_1;blockdatatmp[38] = gmbdtmp;}
				{GAME_MAP_BLOCK_DAT gmbdtmp = {};gmbdtmp.x = 2832;gmbdtmp.y = -144;gmbdtmp.block = (int)BLOCK_TYPE_ISEKI_DAIZA_0;blockdatatmp[39] = gmbdtmp;}
				{GAME_MAP_BLOCK_DAT gmbdtmp = {};gmbdtmp.x = -4272;gmbdtmp.y = 2352;gmbdtmp.block = (int)BLOCK_TYPE_ISEKI_ZANGAI_2;blockdatatmp[40] = gmbdtmp;}
				{GAME_MAP_BLOCK_DAT gmbdtmp = {};gmbdtmp.x = -2640;gmbdtmp.y = 240;gmbdtmp.block = (int)BLOCK_TYPE_ISEKI_ZANGAI_0;blockdatatmp[41] = gmbdtmp;}
				{GAME_MAP_BLOCK_DAT gmbdtmp = {};gmbdtmp.x = -1104;gmbdtmp.y = 3696;gmbdtmp.block = (int)BLOCK_TYPE_ISEKI_HASHIRA_0;blockdatatmp[42] = gmbdtmp;}
				{GAME_MAP_BLOCK_DAT gmbdtmp = {};gmbdtmp.x = 720;gmbdtmp.y = 3696;gmbdtmp.block = (int)BLOCK_TYPE_ISEKI_HASHIRA_0;blockdatatmp[43] = gmbdtmp;}
				{GAME_MAP_BLOCK_DAT gmbdtmp = {};gmbdtmp.x = 1584;gmbdtmp.y = -2064;gmbdtmp.block = (int)BLOCK_TYPE_ISEKI_DAIZA_0;blockdatatmp[44] = gmbdtmp;}
				{GAME_MAP_BLOCK_DAT gmbdtmp = {};gmbdtmp.x = -3024;gmbdtmp.y = -1200;gmbdtmp.block = (int)BLOCK_TYPE_ISEKI_KANATSUBAN_0;blockdatatmp[45] = gmbdtmp;}
				{GAME_MAP_BLOCK_DAT gmbdtmp = {};gmbdtmp.x = -1008;gmbdtmp.y = -2064;gmbdtmp.block = (int)BLOCK_TYPE_ISEKI_DAIZA_0;blockdatatmp[46] = gmbdtmp;}
				{GAME_MAP_BLOCK_DAT gmbdtmp = {};gmbdtmp.x = -2064;gmbdtmp.y = -240;gmbdtmp.block = (int)BLOCK_TYPE_ISEKI_ZANGAI_1;blockdatatmp[47] = gmbdtmp;}
				{GAME_MAP_BLOCK_DAT gmbdtmp = {};gmbdtmp.x = -5232;gmbdtmp.y = -336;gmbdtmp.block = (int)BLOCK_TYPE_ISEKI_ZANGAI_1;blockdatatmp[48] = gmbdtmp;}
				{GAME_MAP_BLOCK_DAT gmbdtmp = {};gmbdtmp.x = -5712;gmbdtmp.y = 432;gmbdtmp.block = (int)BLOCK_TYPE_ISEKI_ZANGAI_1;blockdatatmp[49] = gmbdtmp;}
				{GAME_MAP_BLOCK_DAT gmbdtmp = {};gmbdtmp.x = -5328;gmbdtmp.y = 2256;gmbdtmp.block = (int)BLOCK_TYPE_ISEKI_ZANGAI_1;blockdatatmp[50] = gmbdtmp;}
				{GAME_MAP_BLOCK_DAT gmbdtmp = {};gmbdtmp.x = -3312;gmbdtmp.y = 1968;gmbdtmp.block = (int)BLOCK_TYPE_ISEKI_ZANGAI_1;blockdatatmp[51] = gmbdtmp;}
				{GAME_MAP_BLOCK_DAT gmbdtmp = {};gmbdtmp.x = -6096;gmbdtmp.y = 3312;gmbdtmp.block = (int)BLOCK_TYPE_ISEKI_ZANGAI_1;blockdatatmp[52] = gmbdtmp;}
				{GAME_MAP_BLOCK_DAT gmbdtmp = {};gmbdtmp.x = -6000;gmbdtmp.y = 2256;gmbdtmp.block = (int)BLOCK_TYPE_ISEKI_ZANGAI_1;blockdatatmp[53] = gmbdtmp;}
				{GAME_MAP_BLOCK_DAT gmbdtmp = {};gmbdtmp.x = -6672;gmbdtmp.y = 3504;gmbdtmp.block = (int)BLOCK_TYPE_ISEKI_ZANGAI_1;blockdatatmp[54] = gmbdtmp;}
				{GAME_MAP_BLOCK_DAT gmbdtmp = {};gmbdtmp.x = -1776;gmbdtmp.y = 3312;gmbdtmp.block = (int)BLOCK_TYPE_ISEKI_ZANGAI_1;blockdatatmp[55] = gmbdtmp;}
				{GAME_MAP_BLOCK_DAT gmbdtmp = {};gmbdtmp.x = -432;gmbdtmp.y = 3312;gmbdtmp.block = (int)BLOCK_TYPE_ISEKI_ZANGAI_1;blockdatatmp[56] = gmbdtmp;}
				{GAME_MAP_BLOCK_DAT gmbdtmp = {};gmbdtmp.x = -336;gmbdtmp.y = 3312;gmbdtmp.block = (int)BLOCK_TYPE_ISEKI_ZANGAI_1;blockdatatmp[57] = gmbdtmp;}
				{GAME_MAP_BLOCK_DAT gmbdtmp = {};gmbdtmp.x = 144;gmbdtmp.y = 2544;gmbdtmp.block = (int)BLOCK_TYPE_ISEKI_ZANGAI_1;blockdatatmp[58] = gmbdtmp;}
				{GAME_MAP_BLOCK_DAT gmbdtmp = {};gmbdtmp.x = -144;gmbdtmp.y = 1776;gmbdtmp.block = (int)BLOCK_TYPE_ISEKI_ZANGAI_1;blockdatatmp[59] = gmbdtmp;}
				{GAME_MAP_BLOCK_DAT gmbdtmp = {};gmbdtmp.x = 336;gmbdtmp.y = 912;gmbdtmp.block = (int)BLOCK_TYPE_ISEKI_ZANGAI_1;blockdatatmp[60] = gmbdtmp;}
				{GAME_MAP_BLOCK_DAT gmbdtmp = {};gmbdtmp.x = 1200;gmbdtmp.y = 3504;gmbdtmp.block = (int)BLOCK_TYPE_ISEKI_ZANGAI_1;blockdatatmp[61] = gmbdtmp;}
				{GAME_MAP_BLOCK_DAT gmbdtmp = {};gmbdtmp.x = 1392;gmbdtmp.y = 1488;gmbdtmp.block = (int)BLOCK_TYPE_ISEKI_ZANGAI_1;blockdatatmp[62] = gmbdtmp;}
				{GAME_MAP_BLOCK_DAT gmbdtmp = {};gmbdtmp.x = 2640;gmbdtmp.y = -528;gmbdtmp.block = (int)BLOCK_TYPE_ISEKI_ZANGAI_1;blockdatatmp[63] = gmbdtmp;}
				{GAME_MAP_BLOCK_DAT gmbdtmp = {};gmbdtmp.x = -1008;gmbdtmp.y = -1200;gmbdtmp.block = (int)BLOCK_TYPE_ISEKI_ZANGAI_1;blockdatatmp[64] = gmbdtmp;}
				{GAME_MAP_BLOCK_DAT gmbdtmp = {};gmbdtmp.x = -1104;gmbdtmp.y = -1200;gmbdtmp.block = (int)BLOCK_TYPE_ISEKI_ZANGAI_1;blockdatatmp[65] = gmbdtmp;}
				{GAME_MAP_BLOCK_DAT gmbdtmp = {};gmbdtmp.x = -2544;gmbdtmp.y = -2064;gmbdtmp.block = (int)BLOCK_TYPE_ISEKI_ZANGAI_1;blockdatatmp[66] = gmbdtmp;}
				{GAME_MAP_BLOCK_DAT gmbdtmp = {};gmbdtmp.x = -6000;gmbdtmp.y = 1008;gmbdtmp.block = (int)BLOCK_TYPE_ISEKI_ZANGAI_1;blockdatatmp[67] = gmbdtmp;}
				{GAME_MAP_BLOCK_DAT gmbdtmp = {};gmbdtmp.x = 2448;gmbdtmp.y = 3408;gmbdtmp.block = (int)BLOCK_TYPE_ISEKI_ZANGAI_1;blockdatatmp[68] = gmbdtmp;}
				{GAME_MAP_BLOCK_DAT gmbdtmp = {};gmbdtmp.x = -6192;gmbdtmp.y = -1584;gmbdtmp.block = (int)BLOCK_TYPE_ISEKI_ZANGAI_1;blockdatatmp[69] = gmbdtmp;}}
			GAME_MAP_BLOCK_LIST_DAT gamemapblocktmp = {};
			gamemapblocktmp.count = 70;
			gamemapblocktmp.blocks = blockdatatmp;
			gamemaptmp.blockDat = gamemapblocktmp;
			GAME_MAP_OBJ_DAT* objdatatmp = (GAME_MAP_OBJ_DAT*)malloc(sizeof(GAME_MAP_OBJ_DAT) * 0);
			/*オブジェクトデータ(とても長いので注意)*/ if(objdatatmp != nullptr) {}
			GAME_MAP_OBJ_LIST_DAT gamemapobjtmp = {};
			gamemapobjtmp.count = 0;
			gamemapobjtmp.objects = objdatatmp;
			gamemaptmp.objDat = gamemapobjtmp;
			return gamemaptmp;
		}
		case GAME_MAP_DATA_ID_STAGE_ISEKI_TILE:
		{
			GAME_MAP gamemaptmp = {};
			GAME_MAP_BLOCK_DAT* blockdatatmp = (GAME_MAP_BLOCK_DAT*)malloc(sizeof(GAME_MAP_BLOCK_DAT) * 0);
			/*ブロックデータ(とても長いので注意)*/ if(blockdatatmp != nullptr) {}
			GAME_MAP_BLOCK_LIST_DAT gamemapblocktmp = {};
			gamemapblocktmp.count = 0;
			gamemapblocktmp.blocks = blockdatatmp;
			gamemaptmp.blockDat = gamemapblocktmp;
			GAME_MAP_OBJ_DAT* objdatatmp = (GAME_MAP_OBJ_DAT*)malloc(sizeof(GAME_MAP_OBJ_DAT) * 0);
			/*オブジェクトデータ(とても長いので注意)*/ if(objdatatmp != nullptr) {}
			GAME_MAP_OBJ_LIST_DAT gamemapobjtmp = {};
			gamemapobjtmp.count = 0;
			gamemapobjtmp.objects = objdatatmp;
			gamemaptmp.objDat = gamemapobjtmp;
			return gamemaptmp;
		}
		case GAME_MAP_DATA_ID_STAGE_ISEKI_00:
		{
			GAME_MAP gamemaptmp = {};
			GAME_MAP_BLOCK_DAT* blockdatatmp = (GAME_MAP_BLOCK_DAT*)malloc(sizeof(GAME_MAP_BLOCK_DAT) * 97);
			/*ブロックデータ(とても長いので注意)*/ if(blockdatatmp != nullptr) {
				{GAME_MAP_BLOCK_DAT gmbdtmp = {};gmbdtmp.x = 3600;gmbdtmp.y = 624;gmbdtmp.block = (int)BLOCK_TYPE_ISEKI_0_HasiraS2;blockdatatmp[0] = gmbdtmp;}
				{GAME_MAP_BLOCK_DAT gmbdtmp = {};gmbdtmp.x = -4752;gmbdtmp.y = -3408;gmbdtmp.block = (int)BLOCK_TYPE_ISEKI_0_HasiraS1;blockdatatmp[1] = gmbdtmp;}
				{GAME_MAP_BLOCK_DAT gmbdtmp = {};gmbdtmp.x = 6000;gmbdtmp.y = -2160;gmbdtmp.block = (int)BLOCK_TYPE_ISEKI_0_Dark_HasiraYoko1;blockdatatmp[2] = gmbdtmp;}
				{GAME_MAP_BLOCK_DAT gmbdtmp = {};gmbdtmp.x = 4848;gmbdtmp.y = 3312;gmbdtmp.block = (int)BLOCK_TYPE_ISEKI_0_Dark_Taimatsu;blockdatatmp[3] = gmbdtmp;}
				{GAME_MAP_BLOCK_DAT gmbdtmp = {};gmbdtmp.x = 1680;gmbdtmp.y = -3504;gmbdtmp.block = (int)BLOCK_TYPE_ISEKI_0_Dark_HasiraS1;blockdatatmp[4] = gmbdtmp;}
				{GAME_MAP_BLOCK_DAT gmbdtmp = {};gmbdtmp.x = -1488;gmbdtmp.y = -3408;gmbdtmp.block = (int)BLOCK_TYPE_ISEKI_0_Dark_Hasira2;blockdatatmp[5] = gmbdtmp;}
				{GAME_MAP_BLOCK_DAT gmbdtmp = {};gmbdtmp.x = 4560;gmbdtmp.y = -3408;gmbdtmp.block = (int)BLOCK_TYPE_ISEKI_0_HasiraS3;blockdatatmp[6] = gmbdtmp;}
				{GAME_MAP_BLOCK_DAT gmbdtmp = {};gmbdtmp.x = -6192;gmbdtmp.y = -3120;gmbdtmp.block = (int)BLOCK_TYPE_ISEKI_0_Hasira1;blockdatatmp[7] = gmbdtmp;}
				{GAME_MAP_BLOCK_DAT gmbdtmp = {};gmbdtmp.x = -5904;gmbdtmp.y = 2832;gmbdtmp.block = (int)BLOCK_TYPE_ISEKI_0_Hasira2;blockdatatmp[8] = gmbdtmp;}
				{GAME_MAP_BLOCK_DAT gmbdtmp = {};gmbdtmp.x = 1008;gmbdtmp.y = 3696;gmbdtmp.block = (int)BLOCK_TYPE_ISEKI_0_Hasira2;blockdatatmp[9] = gmbdtmp;}
				{GAME_MAP_BLOCK_DAT gmbdtmp = {};gmbdtmp.x = -3120;gmbdtmp.y = -3408;gmbdtmp.block = (int)BLOCK_TYPE_ISEKI_0_Dark_HasiraYoko1;blockdatatmp[10] = gmbdtmp;}
				{GAME_MAP_BLOCK_DAT gmbdtmp = {};gmbdtmp.x = -1008;gmbdtmp.y = 3696;gmbdtmp.block = (int)BLOCK_TYPE_ISEKI_0_Hasira1;blockdatatmp[11] = gmbdtmp;}
				{GAME_MAP_BLOCK_DAT gmbdtmp = {};gmbdtmp.x = 2064;gmbdtmp.y = 3216;gmbdtmp.block = (int)BLOCK_TYPE_ISEKI_0_HasiraS2;blockdatatmp[12] = gmbdtmp;}
				{GAME_MAP_BLOCK_DAT gmbdtmp = {};gmbdtmp.x = -6288;gmbdtmp.y = -1200;gmbdtmp.block = (int)BLOCK_TYPE_ISEKI_0_Hasira1;blockdatatmp[13] = gmbdtmp;}
				{GAME_MAP_BLOCK_DAT gmbdtmp = {};gmbdtmp.x = -48;gmbdtmp.y = -3600;gmbdtmp.block = (int)BLOCK_TYPE_ISEKI_0_Dark_Block;blockdatatmp[14] = gmbdtmp;}
				{GAME_MAP_BLOCK_DAT gmbdtmp = {};gmbdtmp.x = 6000;gmbdtmp.y = -3312;gmbdtmp.block = (int)BLOCK_TYPE_ISEKI_0_Dark_HasiraS2;blockdatatmp[15] = gmbdtmp;}
				{GAME_MAP_BLOCK_DAT gmbdtmp = {};gmbdtmp.x = 6096;gmbdtmp.y = 432;gmbdtmp.block = (int)BLOCK_TYPE_ISEKI_0_Dark_HasiraS3;blockdatatmp[16] = gmbdtmp;}
				{GAME_MAP_BLOCK_DAT gmbdtmp = {};gmbdtmp.x = 3600;gmbdtmp.y = 3216;gmbdtmp.block = (int)BLOCK_TYPE_ISEKI_0_Dark_Block;blockdatatmp[17] = gmbdtmp;}
				{GAME_MAP_BLOCK_DAT gmbdtmp = {};gmbdtmp.x = -5712;gmbdtmp.y = -1968;gmbdtmp.block = (int)BLOCK_TYPE_ISEKI_0_Dark_Taimatsu;blockdatatmp[18] = gmbdtmp;}
				{GAME_MAP_BLOCK_DAT gmbdtmp = {};gmbdtmp.x = -3792;gmbdtmp.y = 3312;gmbdtmp.block = (int)BLOCK_TYPE_ISEKI_0_Dark_Block;blockdatatmp[19] = gmbdtmp;}
				{GAME_MAP_BLOCK_DAT gmbdtmp = {};gmbdtmp.x = 2064;gmbdtmp.y = -1680;gmbdtmp.block = (int)BLOCK_TYPE_ISEKI_0_Daiza;blockdatatmp[20] = gmbdtmp;}
				{GAME_MAP_BLOCK_DAT gmbdtmp = {};gmbdtmp.x = -1488;gmbdtmp.y = -1680;gmbdtmp.block = (int)BLOCK_TYPE_ISEKI_0_Daiza;blockdatatmp[21] = gmbdtmp;}
				{GAME_MAP_BLOCK_DAT gmbdtmp = {};gmbdtmp.x = -2256;gmbdtmp.y = -720;gmbdtmp.block = (int)BLOCK_TYPE_ISEKI_0_Daiza;blockdatatmp[22] = gmbdtmp;}
				{GAME_MAP_BLOCK_DAT gmbdtmp = {};gmbdtmp.x = -2256;gmbdtmp.y = 1584;gmbdtmp.block = (int)BLOCK_TYPE_ISEKI_0_HasiraYoko1;blockdatatmp[23] = gmbdtmp;}
				{GAME_MAP_BLOCK_DAT gmbdtmp = {};gmbdtmp.x = 2544;gmbdtmp.y = -624;gmbdtmp.block = (int)BLOCK_TYPE_ISEKI_0_Daiza;blockdatatmp[24] = gmbdtmp;}
				{GAME_MAP_BLOCK_DAT gmbdtmp = {};gmbdtmp.x = 5904;gmbdtmp.y = -1104;gmbdtmp.block = (int)BLOCK_TYPE_ISEKI_0_Dark_Hasira3;blockdatatmp[25] = gmbdtmp;}
				{GAME_MAP_BLOCK_DAT gmbdtmp = {};gmbdtmp.x = 6384;gmbdtmp.y = 1488;gmbdtmp.block = (int)BLOCK_TYPE_ISEKI_0_Dark_Taimatsu;blockdatatmp[26] = gmbdtmp;}
				{GAME_MAP_BLOCK_DAT gmbdtmp = {};gmbdtmp.x = -2256;gmbdtmp.y = 3504;gmbdtmp.block = (int)BLOCK_TYPE_ISEKI_0_Dark_Taimatsu;blockdatatmp[27] = gmbdtmp;}
				{GAME_MAP_BLOCK_DAT gmbdtmp = {};gmbdtmp.x = 6000;gmbdtmp.y = 2544;gmbdtmp.block = (int)BLOCK_TYPE_ISEKI_0_HasiraS4;blockdatatmp[28] = gmbdtmp;}
				{GAME_MAP_BLOCK_DAT gmbdtmp = {};gmbdtmp.x = -2352;gmbdtmp.y = -3024;gmbdtmp.block = (int)BLOCK_TYPE_ISEKI_0_Taimatsu;blockdatatmp[29] = gmbdtmp;}
				{GAME_MAP_BLOCK_DAT gmbdtmp = {};gmbdtmp.x = -4848;gmbdtmp.y = 3216;gmbdtmp.block = (int)BLOCK_TYPE_ISEKI_0_Dark_Taimatsu;blockdatatmp[30] = gmbdtmp;}
				{GAME_MAP_BLOCK_DAT gmbdtmp = {};gmbdtmp.x = 2928;gmbdtmp.y = -3408;gmbdtmp.block = (int)BLOCK_TYPE_ISEKI_0_Dark_Taimatsu;blockdatatmp[31] = gmbdtmp;}
				{GAME_MAP_BLOCK_DAT gmbdtmp = {};gmbdtmp.x = 8880;gmbdtmp.y = 336;gmbdtmp.block = (int)BLOCK_TYPE_ISEKI_HAIKEI_04;blockdatatmp[32] = gmbdtmp;}
				{GAME_MAP_BLOCK_DAT gmbdtmp = {};gmbdtmp.x = 11472;gmbdtmp.y = -720;gmbdtmp.block = (int)BLOCK_TYPE_ISEKI_HAIKEI_01;blockdatatmp[33] = gmbdtmp;}
				{GAME_MAP_BLOCK_DAT gmbdtmp = {};gmbdtmp.x = -5712;gmbdtmp.y = 8208;gmbdtmp.block = (int)BLOCK_TYPE_ISEKI_HAIKEI_02;blockdatatmp[34] = gmbdtmp;}
				{GAME_MAP_BLOCK_DAT gmbdtmp = {};gmbdtmp.x = 144;gmbdtmp.y = 9552;gmbdtmp.block = (int)BLOCK_TYPE_ISEKI_HAIKEI_02;blockdatatmp[35] = gmbdtmp;}
				{GAME_MAP_BLOCK_DAT gmbdtmp = {};gmbdtmp.x = 5808;gmbdtmp.y = 8016;gmbdtmp.block = (int)BLOCK_TYPE_ISEKI_HAIKEI_02;blockdatatmp[36] = gmbdtmp;}
				{GAME_MAP_BLOCK_DAT gmbdtmp = {};gmbdtmp.x = -11664;gmbdtmp.y = -4176;gmbdtmp.block = (int)BLOCK_TYPE_ISEKI_HAIKEI_01;blockdatatmp[37] = gmbdtmp;}
				{GAME_MAP_BLOCK_DAT gmbdtmp = {};gmbdtmp.x = 8016;gmbdtmp.y = -5520;gmbdtmp.block = (int)BLOCK_TYPE_ISEKI_HAIKEI_01;blockdatatmp[38] = gmbdtmp;}
				{GAME_MAP_BLOCK_DAT gmbdtmp = {};gmbdtmp.x = -1296;gmbdtmp.y = -6864;gmbdtmp.block = (int)BLOCK_TYPE_ISEKI_HAIKEI_03;blockdatatmp[39] = gmbdtmp;}
				{GAME_MAP_BLOCK_DAT gmbdtmp = {};gmbdtmp.x = -7248;gmbdtmp.y = -6096;gmbdtmp.block = (int)BLOCK_TYPE_ISEKI_HAIKEI_04;blockdatatmp[40] = gmbdtmp;}
				{GAME_MAP_BLOCK_DAT gmbdtmp = {};gmbdtmp.x = -8688;gmbdtmp.y = -912;gmbdtmp.block = (int)BLOCK_TYPE_ISEKI_HAIKEI_05;blockdatatmp[41] = gmbdtmp;}
				{GAME_MAP_BLOCK_DAT gmbdtmp = {};gmbdtmp.x = -4656;gmbdtmp.y = -5424;gmbdtmp.block = (int)BLOCK_TYPE_ISEKI_HAIKEI_05;blockdatatmp[42] = gmbdtmp;}
				{GAME_MAP_BLOCK_DAT gmbdtmp = {};gmbdtmp.x = -12048;gmbdtmp.y = 4464;gmbdtmp.block = (int)BLOCK_TYPE_ISEKI_HAIKEI_06;blockdatatmp[43] = gmbdtmp;}
				{GAME_MAP_BLOCK_DAT gmbdtmp = {};gmbdtmp.x = -10512;gmbdtmp.y = -5616;gmbdtmp.block = (int)BLOCK_TYPE_ISEKI_HAIKEI_06;blockdatatmp[44] = gmbdtmp;}
				{GAME_MAP_BLOCK_DAT gmbdtmp = {};gmbdtmp.x = 8496;gmbdtmp.y = 5808;gmbdtmp.block = (int)BLOCK_TYPE_ISEKI_HAIKEI_06;blockdatatmp[45] = gmbdtmp;}
				{GAME_MAP_BLOCK_DAT gmbdtmp = {};gmbdtmp.x = -9072;gmbdtmp.y = -3696;gmbdtmp.block = (int)BLOCK_TYPE_ISEKI_HAIKEI_02;blockdatatmp[46] = gmbdtmp;}
				{GAME_MAP_BLOCK_DAT gmbdtmp = {};gmbdtmp.x = -2448;gmbdtmp.y = -7440;gmbdtmp.block = (int)BLOCK_TYPE_ISEKI_HAIKEI_02;blockdatatmp[47] = gmbdtmp;}
				{GAME_MAP_BLOCK_DAT gmbdtmp = {};gmbdtmp.x = 240;gmbdtmp.y = -6096;gmbdtmp.block = (int)BLOCK_TYPE_ISEKI_HAIKEI_02;blockdatatmp[48] = gmbdtmp;}
				{GAME_MAP_BLOCK_DAT gmbdtmp = {};gmbdtmp.x = 4752;gmbdtmp.y = -7152;gmbdtmp.block = (int)BLOCK_TYPE_ISEKI_HAIKEI_02;blockdatatmp[49] = gmbdtmp;}
				{GAME_MAP_BLOCK_DAT gmbdtmp = {};gmbdtmp.x = 7824;gmbdtmp.y = -7056;gmbdtmp.block = (int)BLOCK_TYPE_ISEKI_HAIKEI_02;blockdatatmp[50] = gmbdtmp;}
				{GAME_MAP_BLOCK_DAT gmbdtmp = {};gmbdtmp.x = -3696;gmbdtmp.y = -7344;gmbdtmp.block = (int)BLOCK_TYPE_ISEKI_HAIKEI_07;blockdatatmp[51] = gmbdtmp;}
				{GAME_MAP_BLOCK_DAT gmbdtmp = {};gmbdtmp.x = 10608;gmbdtmp.y = -2352;gmbdtmp.block = (int)BLOCK_TYPE_ISEKI_HAIKEI_07;blockdatatmp[52] = gmbdtmp;}
				{GAME_MAP_BLOCK_DAT gmbdtmp = {};gmbdtmp.x = 12048;gmbdtmp.y = 4272;gmbdtmp.block = (int)BLOCK_TYPE_ISEKI_HAIKEI_07;blockdatatmp[53] = gmbdtmp;}
				{GAME_MAP_BLOCK_DAT gmbdtmp = {};gmbdtmp.x = 5616;gmbdtmp.y = -8016;gmbdtmp.block = (int)BLOCK_TYPE_ISEKI_HAIKEI_06;blockdatatmp[54] = gmbdtmp;}
				{GAME_MAP_BLOCK_DAT gmbdtmp = {};gmbdtmp.x = -4944;gmbdtmp.y = -11472;gmbdtmp.block = (int)BLOCK_TYPE_ISEKI_HAIKEI_06;blockdatatmp[55] = gmbdtmp;}
				{GAME_MAP_BLOCK_DAT gmbdtmp = {};gmbdtmp.x = 2160;gmbdtmp.y = -11376;gmbdtmp.block = (int)BLOCK_TYPE_ISEKI_HAIKEI_06;blockdatatmp[56] = gmbdtmp;}
				{GAME_MAP_BLOCK_DAT gmbdtmp = {};gmbdtmp.x = -9744;gmbdtmp.y = -12624;gmbdtmp.block = (int)BLOCK_TYPE_ISEKI_HAIKEI_06;blockdatatmp[57] = gmbdtmp;}
				{GAME_MAP_BLOCK_DAT gmbdtmp = {};gmbdtmp.x = -6960;gmbdtmp.y = -12816;gmbdtmp.block = (int)BLOCK_TYPE_ISEKI_HAIKEI_06;blockdatatmp[58] = gmbdtmp;}
				{GAME_MAP_BLOCK_DAT gmbdtmp = {};gmbdtmp.x = 10512;gmbdtmp.y = -11472;gmbdtmp.block = (int)BLOCK_TYPE_ISEKI_HAIKEI_06;blockdatatmp[59] = gmbdtmp;}
				{GAME_MAP_BLOCK_DAT gmbdtmp = {};gmbdtmp.x = -14448;gmbdtmp.y = -11376;gmbdtmp.block = (int)BLOCK_TYPE_ISEKI_HAIKEI_06;blockdatatmp[60] = gmbdtmp;}
				{GAME_MAP_BLOCK_DAT gmbdtmp = {};gmbdtmp.x = -12720;gmbdtmp.y = -13200;gmbdtmp.block = (int)BLOCK_TYPE_ISEKI_HAIKEI_06;blockdatatmp[61] = gmbdtmp;}
				{GAME_MAP_BLOCK_DAT gmbdtmp = {};gmbdtmp.x = -8688;gmbdtmp.y = -12144;gmbdtmp.block = (int)BLOCK_TYPE_ISEKI_HAIKEI_02;blockdatatmp[62] = gmbdtmp;}
				{GAME_MAP_BLOCK_DAT gmbdtmp = {};gmbdtmp.x = -2256;gmbdtmp.y = -12624;gmbdtmp.block = (int)BLOCK_TYPE_ISEKI_HAIKEI_02;blockdatatmp[63] = gmbdtmp;}
				{GAME_MAP_BLOCK_DAT gmbdtmp = {};gmbdtmp.x = 8112;gmbdtmp.y = -8880;gmbdtmp.block = (int)BLOCK_TYPE_ISEKI_HAIKEI_02;blockdatatmp[64] = gmbdtmp;}
				{GAME_MAP_BLOCK_DAT gmbdtmp = {};gmbdtmp.x = 7056;gmbdtmp.y = -9648;gmbdtmp.block = (int)BLOCK_TYPE_ISEKI_HAIKEI_02;blockdatatmp[65] = gmbdtmp;}
				{GAME_MAP_BLOCK_DAT gmbdtmp = {};gmbdtmp.x = -12720;gmbdtmp.y = -6672;gmbdtmp.block = (int)BLOCK_TYPE_ISEKI_HAIKEI_02;blockdatatmp[66] = gmbdtmp;}
				{GAME_MAP_BLOCK_DAT gmbdtmp = {};gmbdtmp.x = -12720;gmbdtmp.y = -11952;gmbdtmp.block = (int)BLOCK_TYPE_ISEKI_HAIKEI_02;blockdatatmp[67] = gmbdtmp;}
				{GAME_MAP_BLOCK_DAT gmbdtmp = {};gmbdtmp.x = 10224;gmbdtmp.y = -9264;gmbdtmp.block = (int)BLOCK_TYPE_ISEKI_HAIKEI_02;blockdatatmp[68] = gmbdtmp;}
				{GAME_MAP_BLOCK_DAT gmbdtmp = {};gmbdtmp.x = 12144;gmbdtmp.y = -10128;gmbdtmp.block = (int)BLOCK_TYPE_ISEKI_HAIKEI_02;blockdatatmp[69] = gmbdtmp;}
				{GAME_MAP_BLOCK_DAT gmbdtmp = {};gmbdtmp.x = 12144;gmbdtmp.y = -11856;gmbdtmp.block = (int)BLOCK_TYPE_ISEKI_HAIKEI_02;blockdatatmp[70] = gmbdtmp;}
				{GAME_MAP_BLOCK_DAT gmbdtmp = {};gmbdtmp.x = -14736;gmbdtmp.y = -6960;gmbdtmp.block = (int)BLOCK_TYPE_ISEKI_HAIKEI_05;blockdatatmp[71] = gmbdtmp;}
				{GAME_MAP_BLOCK_DAT gmbdtmp = {};gmbdtmp.x = -15792;gmbdtmp.y = -9168;gmbdtmp.block = (int)BLOCK_TYPE_ISEKI_HAIKEI_05;blockdatatmp[72] = gmbdtmp;}
				{GAME_MAP_BLOCK_DAT gmbdtmp = {};gmbdtmp.x = 10896;gmbdtmp.y = -7440;gmbdtmp.block = (int)BLOCK_TYPE_ISEKI_HAIKEI_05;blockdatatmp[73] = gmbdtmp;}
				{GAME_MAP_BLOCK_DAT gmbdtmp = {};gmbdtmp.x = 11568;gmbdtmp.y = -6672;gmbdtmp.block = (int)BLOCK_TYPE_ISEKI_HAIKEI_05;blockdatatmp[74] = gmbdtmp;}
				{GAME_MAP_BLOCK_DAT gmbdtmp = {};gmbdtmp.x = -10512;gmbdtmp.y = -10896;gmbdtmp.block = (int)BLOCK_TYPE_ISEKI_HAIKEI_05;blockdatatmp[75] = gmbdtmp;}
				{GAME_MAP_BLOCK_DAT gmbdtmp = {};gmbdtmp.x = 6192;gmbdtmp.y = -12048;gmbdtmp.block = (int)BLOCK_TYPE_ISEKI_HAIKEI_05;blockdatatmp[76] = gmbdtmp;}
				{GAME_MAP_BLOCK_DAT gmbdtmp = {};gmbdtmp.x = 624;gmbdtmp.y = -10032;gmbdtmp.block = (int)BLOCK_TYPE_ISEKI_HAIKEI_05;blockdatatmp[77] = gmbdtmp;}
				{GAME_MAP_BLOCK_DAT gmbdtmp = {};gmbdtmp.x = 1104;gmbdtmp.y = -8592;gmbdtmp.block = (int)BLOCK_TYPE_ISEKI_HAIKEI_05;blockdatatmp[78] = gmbdtmp;}
				{GAME_MAP_BLOCK_DAT gmbdtmp = {};gmbdtmp.x = -7344;gmbdtmp.y = -8688;gmbdtmp.block = (int)BLOCK_TYPE_ISEKI_HAIKEI_05;blockdatatmp[79] = gmbdtmp;}
				{GAME_MAP_BLOCK_DAT gmbdtmp = {};gmbdtmp.x = -9072;gmbdtmp.y = -7344;gmbdtmp.block = (int)BLOCK_TYPE_ISEKI_HAIKEI_01;blockdatatmp[80] = gmbdtmp;}
				{GAME_MAP_BLOCK_DAT gmbdtmp = {};gmbdtmp.x = -2544;gmbdtmp.y = -9456;gmbdtmp.block = (int)BLOCK_TYPE_ISEKI_HAIKEI_03;blockdatatmp[81] = gmbdtmp;}
				{GAME_MAP_BLOCK_DAT gmbdtmp = {};gmbdtmp.x = 4848;gmbdtmp.y = -11280;gmbdtmp.block = (int)BLOCK_TYPE_ISEKI_HAIKEI_03;blockdatatmp[82] = gmbdtmp;}
				{GAME_MAP_BLOCK_DAT gmbdtmp = {};gmbdtmp.x = -8688;gmbdtmp.y = 1968;gmbdtmp.block = (int)BLOCK_TYPE_ISEKI_HAIKEI_05;blockdatatmp[83] = gmbdtmp;}
				{GAME_MAP_BLOCK_DAT gmbdtmp = {};gmbdtmp.x = -9264;gmbdtmp.y = 3216;gmbdtmp.block = (int)BLOCK_TYPE_ISEKI_HAIKEI_05;blockdatatmp[84] = gmbdtmp;}
				{GAME_MAP_BLOCK_DAT gmbdtmp = {};gmbdtmp.x = -9936;gmbdtmp.y = -2160;gmbdtmp.block = (int)BLOCK_TYPE_ISEKI_HAIKEI_05;blockdatatmp[85] = gmbdtmp;}
				{GAME_MAP_BLOCK_DAT gmbdtmp = {};gmbdtmp.x = -9552;gmbdtmp.y = 5328;gmbdtmp.block = (int)BLOCK_TYPE_ISEKI_HAIKEI_02;blockdatatmp[86] = gmbdtmp;}
				{GAME_MAP_BLOCK_DAT gmbdtmp = {};gmbdtmp.x = 2832;gmbdtmp.y = -7920;gmbdtmp.block = (int)BLOCK_TYPE_ISEKI_HAIKEI_07;blockdatatmp[87] = gmbdtmp;}
				{GAME_MAP_BLOCK_DAT gmbdtmp = {};gmbdtmp.x = 3312;gmbdtmp.y = -9168;gmbdtmp.block = (int)BLOCK_TYPE_ISEKI_HAIKEI_05;blockdatatmp[88] = gmbdtmp;}
				{GAME_MAP_BLOCK_DAT gmbdtmp = {};gmbdtmp.x = 10896;gmbdtmp.y = 1104;gmbdtmp.block = (int)BLOCK_TYPE_ISEKI_HAIKEI_02;blockdatatmp[89] = gmbdtmp;}
				{GAME_MAP_BLOCK_DAT gmbdtmp = {};gmbdtmp.x = -9744;gmbdtmp.y = 1680;gmbdtmp.block = (int)BLOCK_TYPE_ISEKI_HAIKEI_02;blockdatatmp[90] = gmbdtmp;}
				{GAME_MAP_BLOCK_DAT gmbdtmp = {};gmbdtmp.x = -12240;gmbdtmp.y = 1488;gmbdtmp.block = (int)BLOCK_TYPE_ISEKI_HAIKEI_02;blockdatatmp[91] = gmbdtmp;}
				{GAME_MAP_BLOCK_DAT gmbdtmp = {};gmbdtmp.x = -12048;gmbdtmp.y = -1872;gmbdtmp.block = (int)BLOCK_TYPE_ISEKI_HAIKEI_05;blockdatatmp[92] = gmbdtmp;}
				{GAME_MAP_BLOCK_DAT gmbdtmp = {};gmbdtmp.x = 9552;gmbdtmp.y = -4464;gmbdtmp.block = (int)BLOCK_TYPE_ISEKI_HAIKEI_07;blockdatatmp[93] = gmbdtmp;}
				{GAME_MAP_BLOCK_DAT gmbdtmp = {};gmbdtmp.x = -10896;gmbdtmp.y = -3600;gmbdtmp.block = (int)BLOCK_TYPE_ISEKI_HAIKEI_07;blockdatatmp[94] = gmbdtmp;}
				{GAME_MAP_BLOCK_DAT gmbdtmp = {};gmbdtmp.x = -10608;gmbdtmp.y = 528;gmbdtmp.block = (int)BLOCK_TYPE_ISEKI_HAIKEI_07;blockdatatmp[95] = gmbdtmp;}
				{GAME_MAP_BLOCK_DAT gmbdtmp = {};gmbdtmp.x = -5904;gmbdtmp.y = 720;gmbdtmp.block = (int)BLOCK_TYPE_ISEKI_0_Dark_HasiraS4;blockdatatmp[96] = gmbdtmp;}}
			GAME_MAP_BLOCK_LIST_DAT gamemapblocktmp = {};
			gamemapblocktmp.count = 97;
			gamemapblocktmp.blocks = blockdatatmp;
			gamemaptmp.blockDat = gamemapblocktmp;
			GAME_MAP_OBJ_DAT* objdatatmp = (GAME_MAP_OBJ_DAT*)malloc(sizeof(GAME_MAP_OBJ_DAT) * 0);
			/*オブジェクトデータ(とても長いので注意)*/ if(objdatatmp != nullptr) {}
			GAME_MAP_OBJ_LIST_DAT gamemapobjtmp = {};
			gamemapobjtmp.count = 0;
			gamemapobjtmp.objects = objdatatmp;
			gamemaptmp.objDat = gamemapobjtmp;
			return gamemaptmp;
		}
		case GAME_MAP_DATA_ID_STAGE_ISEKI_01:
		{
			GAME_MAP gamemaptmp = {};
			GAME_MAP_BLOCK_DAT* blockdatatmp = (GAME_MAP_BLOCK_DAT*)malloc(sizeof(GAME_MAP_BLOCK_DAT) * 92);
			/*ブロックデータ(とても長いので注意)*/ if(blockdatatmp != nullptr) {
				{GAME_MAP_BLOCK_DAT gmbdtmp = {};gmbdtmp.x = -3792;gmbdtmp.y = -4272;gmbdtmp.block = (int)BLOCK_TYPE_ISEKI_0_Dark_HasiraYoko1;blockdatatmp[0] = gmbdtmp;}
				{GAME_MAP_BLOCK_DAT gmbdtmp = {};gmbdtmp.x = 7344;gmbdtmp.y = -2928;gmbdtmp.block = (int)BLOCK_TYPE_ISEKI_0_Dark_HasiraS2;blockdatatmp[1] = gmbdtmp;}
				{GAME_MAP_BLOCK_DAT gmbdtmp = {};gmbdtmp.x = 4080;gmbdtmp.y = 4176;gmbdtmp.block = (int)BLOCK_TYPE_ISEKI_0_Block;blockdatatmp[2] = gmbdtmp;}
				{GAME_MAP_BLOCK_DAT gmbdtmp = {};gmbdtmp.x = 2928;gmbdtmp.y = -4176;gmbdtmp.block = (int)BLOCK_TYPE_ISEKI_0_Taimatsu;blockdatatmp[3] = gmbdtmp;}
				{GAME_MAP_BLOCK_DAT gmbdtmp = {};gmbdtmp.x = 2064;gmbdtmp.y = -4464;gmbdtmp.block = (int)BLOCK_TYPE_ISEKI_0_Dark_HasiraS1;blockdatatmp[4] = gmbdtmp;}
				{GAME_MAP_BLOCK_DAT gmbdtmp = {};gmbdtmp.x = 4560;gmbdtmp.y = -4464;gmbdtmp.block = (int)BLOCK_TYPE_ISEKI_0_HasiraS3;blockdatatmp[5] = gmbdtmp;}
				{GAME_MAP_BLOCK_DAT gmbdtmp = {};gmbdtmp.x = -5040;gmbdtmp.y = -4368;gmbdtmp.block = (int)BLOCK_TYPE_ISEKI_0_HasiraS1;blockdatatmp[6] = gmbdtmp;}
				{GAME_MAP_BLOCK_DAT gmbdtmp = {};gmbdtmp.x = -7248;gmbdtmp.y = 1104;gmbdtmp.block = (int)BLOCK_TYPE_ISEKI_0_Dark_HasiraS4;blockdatatmp[7] = gmbdtmp;}
				{GAME_MAP_BLOCK_DAT gmbdtmp = {};gmbdtmp.x = -1008;gmbdtmp.y = 4176;gmbdtmp.block = (int)BLOCK_TYPE_ISEKI_0_Hasira1;blockdatatmp[8] = gmbdtmp;}
				{GAME_MAP_BLOCK_DAT gmbdtmp = {};gmbdtmp.x = -2544;gmbdtmp.y = -4176;gmbdtmp.block = (int)BLOCK_TYPE_ISEKI_0_Taimatsu;blockdatatmp[9] = gmbdtmp;}
				{GAME_MAP_BLOCK_DAT gmbdtmp = {};gmbdtmp.x = -2256;gmbdtmp.y = 4080;gmbdtmp.block = (int)BLOCK_TYPE_ISEKI_0_Taimatsu;blockdatatmp[10] = gmbdtmp;}
				{GAME_MAP_BLOCK_DAT gmbdtmp = {};gmbdtmp.x = -7536;gmbdtmp.y = -2160;gmbdtmp.block = (int)BLOCK_TYPE_ISEKI_0_Taimatsu;blockdatatmp[11] = gmbdtmp;}
				{GAME_MAP_BLOCK_DAT gmbdtmp = {};gmbdtmp.x = -912;gmbdtmp.y = -4368;gmbdtmp.block = (int)BLOCK_TYPE_ISEKI_0_Dark_Daiza;blockdatatmp[12] = gmbdtmp;}
				{GAME_MAP_BLOCK_DAT gmbdtmp = {};gmbdtmp.x = 7344;gmbdtmp.y = -1008;gmbdtmp.block = (int)BLOCK_TYPE_ISEKI_0_Dark_Hasira3;blockdatatmp[13] = gmbdtmp;}
				{GAME_MAP_BLOCK_DAT gmbdtmp = {};gmbdtmp.x = 7344;gmbdtmp.y = 1680;gmbdtmp.block = (int)BLOCK_TYPE_ISEKI_0_Taimatsu;blockdatatmp[14] = gmbdtmp;}
				{GAME_MAP_BLOCK_DAT gmbdtmp = {};gmbdtmp.x = 2544;gmbdtmp.y = 4176;gmbdtmp.block = (int)BLOCK_TYPE_ISEKI_0_HasiraS2;blockdatatmp[15] = gmbdtmp;}
				{GAME_MAP_BLOCK_DAT gmbdtmp = {};gmbdtmp.x = -7344;gmbdtmp.y = -3792;gmbdtmp.block = (int)BLOCK_TYPE_ISEKI_0_Hasira1;blockdatatmp[16] = gmbdtmp;}
				{GAME_MAP_BLOCK_DAT gmbdtmp = {};gmbdtmp.x = 5232;gmbdtmp.y = 4080;gmbdtmp.block = (int)BLOCK_TYPE_ISEKI_0_Taimatsu;blockdatatmp[17] = gmbdtmp;}
				{GAME_MAP_BLOCK_DAT gmbdtmp = {};gmbdtmp.x = -7344;gmbdtmp.y = -1008;gmbdtmp.block = (int)BLOCK_TYPE_ISEKI_0_Hasira1;blockdatatmp[18] = gmbdtmp;}
				{GAME_MAP_BLOCK_DAT gmbdtmp = {};gmbdtmp.x = -7248;gmbdtmp.y = 3888;gmbdtmp.block = (int)BLOCK_TYPE_ISEKI_0_Hasira2;blockdatatmp[19] = gmbdtmp;}
				{GAME_MAP_BLOCK_DAT gmbdtmp = {};gmbdtmp.x = -5520;gmbdtmp.y = 4080;gmbdtmp.block = (int)BLOCK_TYPE_ISEKI_0_Taimatsu;blockdatatmp[20] = gmbdtmp;}
				{GAME_MAP_BLOCK_DAT gmbdtmp = {};gmbdtmp.x = 1008;gmbdtmp.y = 4176;gmbdtmp.block = (int)BLOCK_TYPE_ISEKI_0_Dark_Hasira2;blockdatatmp[21] = gmbdtmp;}
				{GAME_MAP_BLOCK_DAT gmbdtmp = {};gmbdtmp.x = 6288;gmbdtmp.y = -4272;gmbdtmp.block = (int)BLOCK_TYPE_ISEKI_0_Dark_HasiraYoko1;blockdatatmp[22] = gmbdtmp;}
				{GAME_MAP_BLOCK_DAT gmbdtmp = {};gmbdtmp.x = 7536;gmbdtmp.y = 2928;gmbdtmp.block = (int)BLOCK_TYPE_ISEKI_0_HasiraS4;blockdatatmp[23] = gmbdtmp;}
				{GAME_MAP_BLOCK_DAT gmbdtmp = {};gmbdtmp.x = -4176;gmbdtmp.y = 4272;gmbdtmp.block = (int)BLOCK_TYPE_ISEKI_0_Block;blockdatatmp[24] = gmbdtmp;}
				{GAME_MAP_BLOCK_DAT gmbdtmp = {};gmbdtmp.x = -1584;gmbdtmp.y = -4272;gmbdtmp.block = (int)BLOCK_TYPE_ISEKI_0_Hasira2;blockdatatmp[25] = gmbdtmp;}
				{GAME_MAP_BLOCK_DAT gmbdtmp = {};gmbdtmp.x = 9552;gmbdtmp.y = 432;gmbdtmp.block = (int)BLOCK_TYPE_ISEKI_HAIKEI_04;blockdatatmp[26] = gmbdtmp;}
				{GAME_MAP_BLOCK_DAT gmbdtmp = {};gmbdtmp.x = 11472;gmbdtmp.y = -720;gmbdtmp.block = (int)BLOCK_TYPE_ISEKI_HAIKEI_01;blockdatatmp[27] = gmbdtmp;}
				{GAME_MAP_BLOCK_DAT gmbdtmp = {};gmbdtmp.x = -5712;gmbdtmp.y = 8208;gmbdtmp.block = (int)BLOCK_TYPE_ISEKI_HAIKEI_02;blockdatatmp[28] = gmbdtmp;}
				{GAME_MAP_BLOCK_DAT gmbdtmp = {};gmbdtmp.x = 144;gmbdtmp.y = 9552;gmbdtmp.block = (int)BLOCK_TYPE_ISEKI_HAIKEI_02;blockdatatmp[29] = gmbdtmp;}
				{GAME_MAP_BLOCK_DAT gmbdtmp = {};gmbdtmp.x = 5808;gmbdtmp.y = 8016;gmbdtmp.block = (int)BLOCK_TYPE_ISEKI_HAIKEI_02;blockdatatmp[30] = gmbdtmp;}
				{GAME_MAP_BLOCK_DAT gmbdtmp = {};gmbdtmp.x = -11664;gmbdtmp.y = -4176;gmbdtmp.block = (int)BLOCK_TYPE_ISEKI_HAIKEI_01;blockdatatmp[31] = gmbdtmp;}
				{GAME_MAP_BLOCK_DAT gmbdtmp = {};gmbdtmp.x = 8016;gmbdtmp.y = -5520;gmbdtmp.block = (int)BLOCK_TYPE_ISEKI_HAIKEI_01;blockdatatmp[32] = gmbdtmp;}
				{GAME_MAP_BLOCK_DAT gmbdtmp = {};gmbdtmp.x = -1296;gmbdtmp.y = -6864;gmbdtmp.block = (int)BLOCK_TYPE_ISEKI_HAIKEI_03;blockdatatmp[33] = gmbdtmp;}
				{GAME_MAP_BLOCK_DAT gmbdtmp = {};gmbdtmp.x = -7248;gmbdtmp.y = -6096;gmbdtmp.block = (int)BLOCK_TYPE_ISEKI_HAIKEI_04;blockdatatmp[34] = gmbdtmp;}
				{GAME_MAP_BLOCK_DAT gmbdtmp = {};gmbdtmp.x = -8688;gmbdtmp.y = -912;gmbdtmp.block = (int)BLOCK_TYPE_ISEKI_HAIKEI_05;blockdatatmp[35] = gmbdtmp;}
				{GAME_MAP_BLOCK_DAT gmbdtmp = {};gmbdtmp.x = -4656;gmbdtmp.y = -5424;gmbdtmp.block = (int)BLOCK_TYPE_ISEKI_HAIKEI_05;blockdatatmp[36] = gmbdtmp;}
				{GAME_MAP_BLOCK_DAT gmbdtmp = {};gmbdtmp.x = -12048;gmbdtmp.y = 4464;gmbdtmp.block = (int)BLOCK_TYPE_ISEKI_HAIKEI_06;blockdatatmp[37] = gmbdtmp;}
				{GAME_MAP_BLOCK_DAT gmbdtmp = {};gmbdtmp.x = -10512;gmbdtmp.y = -5616;gmbdtmp.block = (int)BLOCK_TYPE_ISEKI_HAIKEI_06;blockdatatmp[38] = gmbdtmp;}
				{GAME_MAP_BLOCK_DAT gmbdtmp = {};gmbdtmp.x = 8496;gmbdtmp.y = 5808;gmbdtmp.block = (int)BLOCK_TYPE_ISEKI_HAIKEI_06;blockdatatmp[39] = gmbdtmp;}
				{GAME_MAP_BLOCK_DAT gmbdtmp = {};gmbdtmp.x = -9648;gmbdtmp.y = -3408;gmbdtmp.block = (int)BLOCK_TYPE_ISEKI_HAIKEI_02;blockdatatmp[40] = gmbdtmp;}
				{GAME_MAP_BLOCK_DAT gmbdtmp = {};gmbdtmp.x = -2448;gmbdtmp.y = -7440;gmbdtmp.block = (int)BLOCK_TYPE_ISEKI_HAIKEI_02;blockdatatmp[41] = gmbdtmp;}
				{GAME_MAP_BLOCK_DAT gmbdtmp = {};gmbdtmp.x = 240;gmbdtmp.y = -6096;gmbdtmp.block = (int)BLOCK_TYPE_ISEKI_HAIKEI_02;blockdatatmp[42] = gmbdtmp;}
				{GAME_MAP_BLOCK_DAT gmbdtmp = {};gmbdtmp.x = 4752;gmbdtmp.y = -7152;gmbdtmp.block = (int)BLOCK_TYPE_ISEKI_HAIKEI_02;blockdatatmp[43] = gmbdtmp;}
				{GAME_MAP_BLOCK_DAT gmbdtmp = {};gmbdtmp.x = 7824;gmbdtmp.y = -7056;gmbdtmp.block = (int)BLOCK_TYPE_ISEKI_HAIKEI_02;blockdatatmp[44] = gmbdtmp;}
				{GAME_MAP_BLOCK_DAT gmbdtmp = {};gmbdtmp.x = -3696;gmbdtmp.y = -7344;gmbdtmp.block = (int)BLOCK_TYPE_ISEKI_HAIKEI_07;blockdatatmp[45] = gmbdtmp;}
				{GAME_MAP_BLOCK_DAT gmbdtmp = {};gmbdtmp.x = 10608;gmbdtmp.y = -2352;gmbdtmp.block = (int)BLOCK_TYPE_ISEKI_HAIKEI_07;blockdatatmp[46] = gmbdtmp;}
				{GAME_MAP_BLOCK_DAT gmbdtmp = {};gmbdtmp.x = 12048;gmbdtmp.y = 4272;gmbdtmp.block = (int)BLOCK_TYPE_ISEKI_HAIKEI_07;blockdatatmp[47] = gmbdtmp;}
				{GAME_MAP_BLOCK_DAT gmbdtmp = {};gmbdtmp.x = 5616;gmbdtmp.y = -8016;gmbdtmp.block = (int)BLOCK_TYPE_ISEKI_HAIKEI_06;blockdatatmp[48] = gmbdtmp;}
				{GAME_MAP_BLOCK_DAT gmbdtmp = {};gmbdtmp.x = -4944;gmbdtmp.y = -11472;gmbdtmp.block = (int)BLOCK_TYPE_ISEKI_HAIKEI_06;blockdatatmp[49] = gmbdtmp;}
				{GAME_MAP_BLOCK_DAT gmbdtmp = {};gmbdtmp.x = 2160;gmbdtmp.y = -11376;gmbdtmp.block = (int)BLOCK_TYPE_ISEKI_HAIKEI_06;blockdatatmp[50] = gmbdtmp;}
				{GAME_MAP_BLOCK_DAT gmbdtmp = {};gmbdtmp.x = -9744;gmbdtmp.y = -12624;gmbdtmp.block = (int)BLOCK_TYPE_ISEKI_HAIKEI_06;blockdatatmp[51] = gmbdtmp;}
				{GAME_MAP_BLOCK_DAT gmbdtmp = {};gmbdtmp.x = -6960;gmbdtmp.y = -12816;gmbdtmp.block = (int)BLOCK_TYPE_ISEKI_HAIKEI_06;blockdatatmp[52] = gmbdtmp;}
				{GAME_MAP_BLOCK_DAT gmbdtmp = {};gmbdtmp.x = 10512;gmbdtmp.y = -11472;gmbdtmp.block = (int)BLOCK_TYPE_ISEKI_HAIKEI_06;blockdatatmp[53] = gmbdtmp;}
				{GAME_MAP_BLOCK_DAT gmbdtmp = {};gmbdtmp.x = -14448;gmbdtmp.y = -11376;gmbdtmp.block = (int)BLOCK_TYPE_ISEKI_HAIKEI_06;blockdatatmp[54] = gmbdtmp;}
				{GAME_MAP_BLOCK_DAT gmbdtmp = {};gmbdtmp.x = -12720;gmbdtmp.y = -13200;gmbdtmp.block = (int)BLOCK_TYPE_ISEKI_HAIKEI_06;blockdatatmp[55] = gmbdtmp;}
				{GAME_MAP_BLOCK_DAT gmbdtmp = {};gmbdtmp.x = -8688;gmbdtmp.y = -12144;gmbdtmp.block = (int)BLOCK_TYPE_ISEKI_HAIKEI_02;blockdatatmp[56] = gmbdtmp;}
				{GAME_MAP_BLOCK_DAT gmbdtmp = {};gmbdtmp.x = -2256;gmbdtmp.y = -12624;gmbdtmp.block = (int)BLOCK_TYPE_ISEKI_HAIKEI_02;blockdatatmp[57] = gmbdtmp;}
				{GAME_MAP_BLOCK_DAT gmbdtmp = {};gmbdtmp.x = 8112;gmbdtmp.y = -8880;gmbdtmp.block = (int)BLOCK_TYPE_ISEKI_HAIKEI_02;blockdatatmp[58] = gmbdtmp;}
				{GAME_MAP_BLOCK_DAT gmbdtmp = {};gmbdtmp.x = 7056;gmbdtmp.y = -9648;gmbdtmp.block = (int)BLOCK_TYPE_ISEKI_HAIKEI_02;blockdatatmp[59] = gmbdtmp;}
				{GAME_MAP_BLOCK_DAT gmbdtmp = {};gmbdtmp.x = -12720;gmbdtmp.y = -6672;gmbdtmp.block = (int)BLOCK_TYPE_ISEKI_HAIKEI_02;blockdatatmp[60] = gmbdtmp;}
				{GAME_MAP_BLOCK_DAT gmbdtmp = {};gmbdtmp.x = -12720;gmbdtmp.y = -11952;gmbdtmp.block = (int)BLOCK_TYPE_ISEKI_HAIKEI_02;blockdatatmp[61] = gmbdtmp;}
				{GAME_MAP_BLOCK_DAT gmbdtmp = {};gmbdtmp.x = 10224;gmbdtmp.y = -9264;gmbdtmp.block = (int)BLOCK_TYPE_ISEKI_HAIKEI_02;blockdatatmp[62] = gmbdtmp;}
				{GAME_MAP_BLOCK_DAT gmbdtmp = {};gmbdtmp.x = 12144;gmbdtmp.y = -10128;gmbdtmp.block = (int)BLOCK_TYPE_ISEKI_HAIKEI_02;blockdatatmp[63] = gmbdtmp;}
				{GAME_MAP_BLOCK_DAT gmbdtmp = {};gmbdtmp.x = 12144;gmbdtmp.y = -11856;gmbdtmp.block = (int)BLOCK_TYPE_ISEKI_HAIKEI_02;blockdatatmp[64] = gmbdtmp;}
				{GAME_MAP_BLOCK_DAT gmbdtmp = {};gmbdtmp.x = -14736;gmbdtmp.y = -6960;gmbdtmp.block = (int)BLOCK_TYPE_ISEKI_HAIKEI_05;blockdatatmp[65] = gmbdtmp;}
				{GAME_MAP_BLOCK_DAT gmbdtmp = {};gmbdtmp.x = -15792;gmbdtmp.y = -9168;gmbdtmp.block = (int)BLOCK_TYPE_ISEKI_HAIKEI_05;blockdatatmp[66] = gmbdtmp;}
				{GAME_MAP_BLOCK_DAT gmbdtmp = {};gmbdtmp.x = 10896;gmbdtmp.y = -7440;gmbdtmp.block = (int)BLOCK_TYPE_ISEKI_HAIKEI_05;blockdatatmp[67] = gmbdtmp;}
				{GAME_MAP_BLOCK_DAT gmbdtmp = {};gmbdtmp.x = 11568;gmbdtmp.y = -6672;gmbdtmp.block = (int)BLOCK_TYPE_ISEKI_HAIKEI_05;blockdatatmp[68] = gmbdtmp;}
				{GAME_MAP_BLOCK_DAT gmbdtmp = {};gmbdtmp.x = -10512;gmbdtmp.y = -10896;gmbdtmp.block = (int)BLOCK_TYPE_ISEKI_HAIKEI_05;blockdatatmp[69] = gmbdtmp;}
				{GAME_MAP_BLOCK_DAT gmbdtmp = {};gmbdtmp.x = 6192;gmbdtmp.y = -12048;gmbdtmp.block = (int)BLOCK_TYPE_ISEKI_HAIKEI_05;blockdatatmp[70] = gmbdtmp;}
				{GAME_MAP_BLOCK_DAT gmbdtmp = {};gmbdtmp.x = 624;gmbdtmp.y = -10032;gmbdtmp.block = (int)BLOCK_TYPE_ISEKI_HAIKEI_05;blockdatatmp[71] = gmbdtmp;}
				{GAME_MAP_BLOCK_DAT gmbdtmp = {};gmbdtmp.x = 1104;gmbdtmp.y = -8592;gmbdtmp.block = (int)BLOCK_TYPE_ISEKI_HAIKEI_05;blockdatatmp[72] = gmbdtmp;}
				{GAME_MAP_BLOCK_DAT gmbdtmp = {};gmbdtmp.x = -7344;gmbdtmp.y = -8688;gmbdtmp.block = (int)BLOCK_TYPE_ISEKI_HAIKEI_05;blockdatatmp[73] = gmbdtmp;}
				{GAME_MAP_BLOCK_DAT gmbdtmp = {};gmbdtmp.x = -9072;gmbdtmp.y = -7344;gmbdtmp.block = (int)BLOCK_TYPE_ISEKI_HAIKEI_01;blockdatatmp[74] = gmbdtmp;}
				{GAME_MAP_BLOCK_DAT gmbdtmp = {};gmbdtmp.x = -2544;gmbdtmp.y = -9456;gmbdtmp.block = (int)BLOCK_TYPE_ISEKI_HAIKEI_03;blockdatatmp[75] = gmbdtmp;}
				{GAME_MAP_BLOCK_DAT gmbdtmp = {};gmbdtmp.x = 4848;gmbdtmp.y = -11280;gmbdtmp.block = (int)BLOCK_TYPE_ISEKI_HAIKEI_03;blockdatatmp[76] = gmbdtmp;}
				{GAME_MAP_BLOCK_DAT gmbdtmp = {};gmbdtmp.x = -8688;gmbdtmp.y = 1968;gmbdtmp.block = (int)BLOCK_TYPE_ISEKI_HAIKEI_05;blockdatatmp[77] = gmbdtmp;}
				{GAME_MAP_BLOCK_DAT gmbdtmp = {};gmbdtmp.x = -9264;gmbdtmp.y = 3216;gmbdtmp.block = (int)BLOCK_TYPE_ISEKI_HAIKEI_05;blockdatatmp[78] = gmbdtmp;}
				{GAME_MAP_BLOCK_DAT gmbdtmp = {};gmbdtmp.x = -9936;gmbdtmp.y = -2160;gmbdtmp.block = (int)BLOCK_TYPE_ISEKI_HAIKEI_05;blockdatatmp[79] = gmbdtmp;}
				{GAME_MAP_BLOCK_DAT gmbdtmp = {};gmbdtmp.x = -9552;gmbdtmp.y = 5328;gmbdtmp.block = (int)BLOCK_TYPE_ISEKI_HAIKEI_02;blockdatatmp[80] = gmbdtmp;}
				{GAME_MAP_BLOCK_DAT gmbdtmp = {};gmbdtmp.x = 2832;gmbdtmp.y = -7920;gmbdtmp.block = (int)BLOCK_TYPE_ISEKI_HAIKEI_07;blockdatatmp[81] = gmbdtmp;}
				{GAME_MAP_BLOCK_DAT gmbdtmp = {};gmbdtmp.x = 3312;gmbdtmp.y = -9168;gmbdtmp.block = (int)BLOCK_TYPE_ISEKI_HAIKEI_05;blockdatatmp[82] = gmbdtmp;}
				{GAME_MAP_BLOCK_DAT gmbdtmp = {};gmbdtmp.x = 10896;gmbdtmp.y = 1104;gmbdtmp.block = (int)BLOCK_TYPE_ISEKI_HAIKEI_02;blockdatatmp[83] = gmbdtmp;}
				{GAME_MAP_BLOCK_DAT gmbdtmp = {};gmbdtmp.x = -9744;gmbdtmp.y = 1680;gmbdtmp.block = (int)BLOCK_TYPE_ISEKI_HAIKEI_02;blockdatatmp[84] = gmbdtmp;}
				{GAME_MAP_BLOCK_DAT gmbdtmp = {};gmbdtmp.x = -12240;gmbdtmp.y = 1488;gmbdtmp.block = (int)BLOCK_TYPE_ISEKI_HAIKEI_02;blockdatatmp[85] = gmbdtmp;}
				{GAME_MAP_BLOCK_DAT gmbdtmp = {};gmbdtmp.x = -12048;gmbdtmp.y = -1872;gmbdtmp.block = (int)BLOCK_TYPE_ISEKI_HAIKEI_05;blockdatatmp[86] = gmbdtmp;}
				{GAME_MAP_BLOCK_DAT gmbdtmp = {};gmbdtmp.x = 9552;gmbdtmp.y = -4464;gmbdtmp.block = (int)BLOCK_TYPE_ISEKI_HAIKEI_07;blockdatatmp[87] = gmbdtmp;}
				{GAME_MAP_BLOCK_DAT gmbdtmp = {};gmbdtmp.x = -10896;gmbdtmp.y = -3600;gmbdtmp.block = (int)BLOCK_TYPE_ISEKI_HAIKEI_07;blockdatatmp[88] = gmbdtmp;}
				{GAME_MAP_BLOCK_DAT gmbdtmp = {};gmbdtmp.x = -10608;gmbdtmp.y = 528;gmbdtmp.block = (int)BLOCK_TYPE_ISEKI_HAIKEI_07;blockdatatmp[89] = gmbdtmp;}
				{GAME_MAP_BLOCK_DAT gmbdtmp = {};gmbdtmp.x = 7440;gmbdtmp.y = 528;gmbdtmp.block = (int)BLOCK_TYPE_ISEKI_0_Dark_HasiraS3;blockdatatmp[90] = gmbdtmp;}
				{GAME_MAP_BLOCK_DAT gmbdtmp = {};gmbdtmp.x = 1392;gmbdtmp.y = -4368;gmbdtmp.block = (int)BLOCK_TYPE_ISEKI_0_Dark_Daiza;blockdatatmp[91] = gmbdtmp;}}
			GAME_MAP_BLOCK_LIST_DAT gamemapblocktmp = {};
			gamemapblocktmp.count = 92;
			gamemapblocktmp.blocks = blockdatatmp;
			gamemaptmp.blockDat = gamemapblocktmp;
			GAME_MAP_OBJ_DAT* objdatatmp = (GAME_MAP_OBJ_DAT*)malloc(sizeof(GAME_MAP_OBJ_DAT) * 0);
			/*オブジェクトデータ(とても長いので注意)*/ if(objdatatmp != nullptr) {}
			GAME_MAP_OBJ_LIST_DAT gamemapobjtmp = {};
			gamemapobjtmp.count = 0;
			gamemapobjtmp.objects = objdatatmp;
			gamemaptmp.objDat = gamemapobjtmp;
			return gamemaptmp;
		}
	}
	return {};
}

std::string GameMapDataGetIdSring(int id)
{
	return game_map_data_id_string[id];
}

