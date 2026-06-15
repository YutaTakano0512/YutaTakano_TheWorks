// ===================================================
// texData.cpp テクスチャのセット
// 
// hara sougo  2024/08/22
// ===================================================
#define IS_TEXDATA
#include "../../game_main.h"
#include "texture.h"
#include "texData.h"

#include "../Sprite/sprite.h"
#include "../Systems/camera.h"


struct TEX_DAT_AUTO {
	TEX_DAT dat;
    int loadNum;

	std::string name;
};


static TEX_DAT texDat[TEX_TYPE_MAX] = {};
static std::vector<TEX_DAT_AUTO> texDatAuto = {};

static TEX_DAT_LOAD_BUFF g_texDatLoadBuff[TEX_DATA_LOAD_BUFF_MAX] = {};

void InitTexData()
{
	//自動読み込み
	std::vector<FILE_AUTO_LOAD_DAT> files = FileScanDirectory(TEX_PATH, "TEX");

	//セット
	for (int i = 0; i < files.size(); i++)
	{
		TEX_DAT_AUTO dat = {};
		dat.name = files[i].name;

        if (dat.name.find("/.") != std::string::npos)
        {
            continue;
        }

        //DebugPrintf("%s\n", files[i].fPath.c_str());
		dat.dat.enable = true;
		strcpy_s(dat.dat.texturePath, sizeof(char) * TEX_DATA_PATH_MAX, files[i].fPath.c_str());

        //dat.dat.textureId = LoadTextureExOpt(dat.dat.texturePath, LOADTEXTURETYPE_MAIN);
        //if (dat.dat.textureId != 0)
        //{
        //    dat.dat.width = GetTextureWidth(dat.dat.textureId);
        //    dat.dat.height = GetTextureHeight(dat.dat.textureId);
        //}

        dat.dat.textureId = i;

        dat.dat.textureId_DX = -1;
        dat.loadNum = 0;

		texDatAuto.push_back(dat);

	}

    texDatAuto.shrink_to_fit();

    {
        std::string texPath[TEX_TYPE_MAX] = {};

        TexDataSet(texPath);

        //セット
        for (int i = 0; i < TEX_TYPE_MAX; i++)
        {
            texDat[i].enable = true;
#ifdef SWITCH_MODE
            strcpy(texDat[i].texturePath, texPath[i].c_str());
#else
            strcpy_s(texDat[i].texturePath, sizeof(char) * TEX_DATA_PATH_MAX, texPath[i].c_str());
#endif
            //texDat[i].textureId_DX =
            //    LoadTextureExOpt(texDat[i].texturePath, LOADTEXTURETYPE_MAIN);
            //texDat[i].textureIdGray =
            //	LoadTextureExOpt(texDat[i].texturePath, LOADTEXTURETYPE_GRAY);
            //texDat[i].textureId = 0;
            for (int j = 0; j < texDatAuto.size(); j++)
            {
                std::string pTmp[2] = {};
                pTmp[0] = texDatAuto[j].dat.texturePath;
                pTmp[1] = texDat[i].texturePath;
                for (int k = 0; k < 2; k++)
                {
                    pTmp[k].erase(remove(pTmp[k].begin(), pTmp[k].end(), ':'), pTmp[k].end());
                    pTmp[k].erase(remove(pTmp[k].begin(), pTmp[k].end(), '/'), pTmp[k].end());
                    pTmp[k].erase(remove(pTmp[k].begin(), pTmp[k].end(), '\\'), pTmp[k].end());
                    pTmp[k].erase(remove(pTmp[k].begin(), pTmp[k].end(), '\n'), pTmp[k].end());
                    pTmp[k].erase(remove(pTmp[k].begin(), pTmp[k].end(), '\r'), pTmp[k].end());
                }
                if (pTmp[0] == pTmp[1])
                {
                    texDat[i].textureId = j;
                    texDat[i].textureId_DX = LoadTexData(texDatAuto[j].name).textureId_DX;
                    texDatAuto[j].loadNum++;

                    break;
                }
            }

            if (texDat[i].textureId_DX != 0)
            {
                texDat[i].width = GetTextureWidth(texDat[i].textureId_DX);
                texDat[i].height = GetTextureHeight(texDat[i].textureId_DX);
            }
        }
    }
}

void UninitTexData()
{
	for (int i = 0; i < TEX_TYPE_MAX; i++)
	{
        UnloadTexData(texDat[i]);
        //if (texDat[i].textureId_DX != -1)
        //{
        //    UnloadTexture(texDat[i].textureId_DX);
        //    //UnloadTexture(texDat[i].textureIdGray);
        //}
	}
	for (int i = 0; i < texDatAuto.size(); i++)
	{
        if (texDatAuto[i].dat.textureId_DX != -1)
        {
            UnloadTexture(texDatAuto[i].dat.textureId_DX);
        }
	}
}

TEX_DAT GetTexData(TEX_TYPE type)
{
	return texDat[type];
}
//TEX_DAT GetTexData(std::string name)
//{
//	TEX_DAT dat = texDatAuto[0].dat;
//	for (int i = 0; i < texDatAuto.size(); i++)
//	{
//		if (texDatAuto[i].name == name)
//		{
//            if (texDatAuto[i].dat.textureId == -1)
//            {
//                texDatAuto[i].dat.textureId =
//                    LoadTextureExOpt(texDatAuto[i].dat.texturePath, LOADTEXTURETYPE_MAIN);
//                if (texDatAuto[i].dat.textureId != 0)
//                {
//                    texDatAuto[i].dat.width = GetTextureWidth(texDatAuto[i].dat.textureId);
//                    texDatAuto[i].dat.height = GetTextureHeight(texDatAuto[i].dat.textureId);
//                }
//            }
//			return texDatAuto[i].dat;
//		}
//	}
//	return dat;
//}


TEX_DAT LoadTexData(unsigned int index)
{
    if (texDatAuto[index].dat.textureId_DX == -1)
    {
        texDatAuto[index].dat.textureId_DX =
            LoadTextureExOpt(texDatAuto[index].dat.texturePath, LOADTEXTURETYPE_MAIN);
        if (texDatAuto[index].dat.textureId_DX != 0)
        {
            texDatAuto[index].dat.width = GetTextureWidth(texDatAuto[index].dat.textureId_DX);
            texDatAuto[index].dat.height = GetTextureHeight(texDatAuto[index].dat.textureId_DX);
        }
    }

    texDatAuto[index].loadNum++;

    for (int i = 0; i < TEX_DATA_LOAD_BUFF_MAX; i++)
    {
        if (g_texDatLoadBuff[i].enable == false)
        {
            TEX_DAT_LOAD_BUFF buff = {};
            buff.enable = true;
            buff.unload = false;
            buff.textureId = index;
            g_texDatLoadBuff[i] = buff;

            break;
        }
    }

    return texDatAuto[index].dat;
}

TEX_DAT LoadTexData(std::string name)
{
    TEX_DAT dat = texDatAuto[0].dat;
    for (unsigned int i = 0; i < texDatAuto.size(); i++)
    {
        std::string pTmp[2] = {};
        pTmp[0] = texDatAuto[i].dat.texturePath;
        pTmp[1] = texDat[i].texturePath;
        for (int j = 0; j < 2; j++)
        {
            pTmp[j].erase(remove(pTmp[j].begin(), pTmp[j].end(), ':'), pTmp[j].end());
            pTmp[j].erase(remove(pTmp[j].begin(), pTmp[j].end(), '/'), pTmp[j].end());
            pTmp[j].erase(remove(pTmp[j].begin(), pTmp[j].end(), '\\'), pTmp[j].end());
            pTmp[j].erase(remove(pTmp[j].begin(), pTmp[j].end(), '\n'), pTmp[j].end());
            pTmp[j].erase(remove(pTmp[j].begin(), pTmp[j].end(), '\r'), pTmp[j].end());
        }

        if (texDatAuto[i].name == name
           // || pTmp[0] == pTmp[1]
            )
        {
            return LoadTexData(i);
        }
    }
    return dat;
}

//void UnloadTexData(TEX_DAT texdat)
//{
//    if (texdat.textureId != -1)
//    {
//        UnloadTexture(texdat.textureId);
//    }
//}
//void UnloadTexData(std::string name)
//{
//    TEX_DAT dat = texDatAuto[0].dat;
//    for (int i = 0; i < texDatAuto.size(); i++)
//    {
//        if (texDatAuto[i].name == name)
//        {
//            UnloadTexData(texDatAuto[i].dat);
//            return;
//        }
//    }
//}


void unloadTexDataParts(TEX_DAT_AUTO* tda)
{
    for (int i = 0; i < TEX_DATA_LOAD_BUFF_MAX; i++)
    {
        if (g_texDatLoadBuff[i].enable == false)
        {
            TEX_DAT_LOAD_BUFF buff = {};
            buff.enable = true;
            buff.unload = true;
            buff.textureId = tda->dat.textureId;
            g_texDatLoadBuff[i] = buff;

            break;
        }
    }

    if (tda->dat.textureId_DX != -1)
    {
        if (tda->loadNum > 0)
        {
            tda->loadNum--;
        }
        else
        {
            UnloadTexture(tda->dat.textureId_DX);
        }
    }
}

void UnloadTexData(unsigned int id)
{
    unloadTexDataParts(&texDatAuto[id]);
}
void UnloadTexData(std::string name)
{
    TEX_DAT dat = texDatAuto[0].dat;
    for (int i = 0; i < texDatAuto.size(); i++)
    {
        if (texDatAuto[i].name == name)
        {
            unloadTexDataParts(&texDatAuto[i]);
            return;
        }
    }
}
void UnloadTexData(TEX_DAT texdat)
{
    TEX_DAT dat = texDatAuto[0].dat;
    for (int i = 0; i < texDatAuto.size(); i++)
    {
        if (std::string(texDatAuto[i].dat.texturePath) == texdat.texturePath)
        {
            unloadTexDataParts(&texDatAuto[i]);
            return;
        }
    }
}


unsigned int GetTexId(unsigned int index)
{
    return texDatAuto[index].dat.textureId_DX;
}

std::vector<unsigned int> GetTexIdAll()
{
    std::vector<unsigned int> r = {};
    for (int i = 0; i < texDatAuto.size(); i++)
    {
        TEX_DAT texd = LoadTexData(texDatAuto[i].name);
        //TEX_DAT texd = GetTexData(texDatAuto[i].name);

        r.push_back(texd.textureId_DX);

        UnloadTexData(texd);
    }
    return r;
}

void TexDataBuffClear()
{
    for (int i = 0; i < TEX_DATA_LOAD_BUFF_MAX;i++)
    {
        g_texDatLoadBuff[i].enable = false;
    }
}

TEX_DAT_LOAD_BUFF* GetTexDataBuff()
{
    return g_texDatLoadBuff;
}