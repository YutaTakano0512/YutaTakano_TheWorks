// ===================================================
// modelData.cpp モデルのセット
// 
// hara sougo  2024/12/17
// ===================================================
#include "../../game_main.h"
#include "polygon.h"
#include "modelData.h"


struct MODEL_DAT_AUTO {
    DX_MODEL* model;

	std::string name;
};

static std::vector<MODEL_DAT_AUTO> modelDatAuto = {};

static std::vector<FILE_AUTO_LOAD_DAT> g_ModelDatFiles = {};

size_t InitModelDataFiles()
{
    g_ModelDatFiles = FileScanDirectory(MODEL_PATH, "MODEL");
    return g_ModelDatFiles.size();
}

void InitModelData()
{

	//自動読み込み
	//std::vector<FILE_AUTO_LOAD_DAT> files = FileScanDirectory(MODEL_PATH, "MODEL");

	//セット
	for (int i = 0; i < g_ModelDatFiles.size(); i++)
	{
        FileLoadAddBefore(1, FILELOADADDBEFORE_TXT_MODEL, FILELOADADD_TYPE_NORMAL);

        MODEL_DAT_AUTO dat = {};
		dat.name = g_ModelDatFiles[i].name;
        dat.model = DX_ModelLoad(g_ModelDatFiles[i].fPath.c_str(), false);;
        modelDatAuto.push_back(dat);

        FileLoadAdd(1, FILELOADADD_TXT_MODEL, FILELOADADD_TYPE_NORMAL);
	}

    //files = files;

    g_ModelDatFiles.clear();
    g_ModelDatFiles.shrink_to_fit();
}

void UninitModelData()
{
	for (int i = 0; i < modelDatAuto.size(); i++)
	{
        DX_ModelRelease(modelDatAuto[i].model);
	}
}

unsigned int GetModelDataIndex(std::string name)
{
	for (int i = 0; i < modelDatAuto.size(); i++)
	{
		if (modelDatAuto[i].name == name)
		{
			return i + 1;
		}
	}
	return 0;
}

DX_MODEL* GetModelData(unsigned int index)
{
    DX_MODEL* dat = nullptr;
    if (index >= 1 && index < modelDatAuto.size() + 1)
    {
        dat = modelDatAuto[index - 1].model;
    }
    return dat;
}
