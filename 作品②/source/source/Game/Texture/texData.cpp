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

    //1. OpenMPの有効化
    //　Visual Studioでは、OpenMPを利用するためにプロジェクト設定を変更する必要がある。
    //　- プロジェクトのプロパティを開く:-メニューからプロジェクト > プロパティを選択。
    //　- OpenMPを有効化:-「構成プロパティ > C / C++ > 言語」セクションで、OpenMP サポートを有効に設定。
    //　- 設定を保存してプロジェクトを再ビルドします。
#include <omp.h>



struct TEX_DAT_AUTO {
	TEX_DAT dat;
    int loadNum;

	std::string name;
};


static TEX_DAT texDat[TEX_TYPE_MAX] = {};
static std::vector<TEX_DAT_AUTO> texDatAuto = {};

static TEX_DAT_LOAD_BUFF g_texDatLoadBuff[TEX_DATA_LOAD_BUFF_MAX] = {};

//自動読み込み
std::vector<FILE_AUTO_LOAD_DAT> g_TexDatFiles = {};

#if defined(_DEBUG) || defined(DEBUG)
#include <iostream>
#include <fstream>

static std::vector<int> g_loadedTex = {};
#endif

static void texdataadd(int i, TEX_DAT_AUTO& dat)
{
    //dat.dat.textureId = LoadTextureExOpt(dat.dat.texturePath, LOADTEXTURETYPE_MAIN);
    //if (dat.dat.textureId != 0)
    //{
    //    dat.dat.width = GetTextureWidth(dat.dat.textureId);
    //    dat.dat.height = GetTextureHeight(dat.dat.textureId);
    //}

    dat.dat.textureId = texDatAuto.size();

    //dat.dat.textureId_DX = -1;
    //dat.loadNum = 0;

    //---
    dat.dat.textureId_DX =
        LoadTextureExOpt(dat.dat.texturePath, LOADTEXTURETYPE_MAIN);
    //if (dat.dat.textureId_DX != 0)
    //{
    //    dat.dat.width = GetTextureWidth(dat.dat.textureId_DX);
    //    dat.dat.height = GetTextureHeight(dat.dat.textureId_DX);
    //}
    dat.loadNum = 1;
    //---

//#pragma omp critical
    {
        // 安全なアクセス
        texDatAuto.push_back(dat);
        //texDatAuto[i] = dat;
    }
}

size_t InitTexDataFiles()
{
    g_TexDatFiles = FileScanDirectory(TEX_PATH, "TEX");

    return g_TexDatFiles.size();
}
void InitTexData()
{
	//自動読み込み
	//std::vector<FILE_AUTO_LOAD_DAT> files = FileScanDirectory(TEX_PATH, "TEX");


    {
        TEX_DAT_AUTO dat = {};
        dat.name = "TEX/gray";
        dat.dat.enable = true;
        strcpy_s(dat.dat.texturePath, sizeof(char) * TEX_DATA_PATH_MAX, "rom/texture/gray.tga");

        texdataadd(0, dat);
    }

    //texDatAuto.resize(files.size() + 1);
    //for (int i = 0; i < files.size() + 1; i++)
    //{
    //    texDatAuto.push_back({});
    //}

	//セット
// OpenMP並列処理
//#pragma omp parallel for
	for (int i = 0; i < g_TexDatFiles.size(); i++)
	{
		TEX_DAT_AUTO dat = {};
		dat.name = g_TexDatFiles[i].name;

        //if (dat.name.find("/.") != std::string::npos)
        //{
        //    continue;
        //}
        
        //DebugPrintf("%s\n", files[i].fPath.c_str());
        dat.dat.enable = true;
        strcpy_s(dat.dat.texturePath, sizeof(char)* TEX_DATA_PATH_MAX, g_TexDatFiles[i].fPath.c_str());


        texdataadd(i + 1, dat);
	}
    
    //待機
    WaitLoadTexture();

    //ロードが終わったらサイズをキャッシュ
    for (auto& dat : texDatAuto)
    {
        if (dat.dat.textureId_DX != 0)
        {
            dat.dat.width = GetTextureWidth(dat.dat.textureId_DX);
            dat.dat.height = GetTextureHeight(dat.dat.textureId_DX);
        }
    }

    texDatAuto.shrink_to_fit();


#if defined(_DEBUG) || defined(DEBUG)
    //g_loadedTex = new int[texDatAuto.size() + 1];
    for (int i = 0; i < texDatAuto.size(); i++)
    {
        g_loadedTex.push_back(0);
    }
    
#endif

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

#if defined(_DEBUG) || defined(DEBUG)
    for (int i = 0; i < texDatAuto.size(); i++)
    {
        g_loadedTex[i] = 0;
    }
#endif

    g_TexDatFiles.clear();
    g_TexDatFiles.shrink_to_fit();
}

void UninitTexData()
{
#if defined(_DEBUG) || defined(DEBUG)
    {
        // ファイル名の指定
        std::string filename = "noloadtex.txt";

        // テキストファイルを開く (書き込みモード)
        std::ofstream file(filename);
        if (!file.is_open()) {
            return;
        }

        // データをファイルに書き込む
        for (int i = 0; i < g_loadedTex.size(); i++)
        {
            //if (g_loadedTex[i] <= 0)
            //{
            //    file << texDatAuto[i].name << std::endl;
            //}
            //file << texDatAuto[i].name << std::endl;
            if (g_loadedTex[i] <= 1)
            {
                //file << texDatAuto[i].name << std::endl;
                file << texDatAuto[i].dat.texturePath << std::endl;
            }
        }

        // ファイルを閉じる
        file.close();
    }
    //g_loadedTex = g_loadedTex;
    //for (int i = 0; i < texDatAuto.size(); i++)
    //{
    //    if (g_loadedTex[i] <= 0)
    //    {
    //        DebugPrintf(" %s\n", texDatAuto[i].dat.texturePath);
    //    }
    //}
    //delete[] g_loadedTex;
#endif

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
#if defined(_DEBUG) || defined(DEBUG)
    for (int i = 0; i < texDatAuto.size(); i++)
    {
        if (texDatAuto[i].dat.textureId_DX == texDat[type].textureId_DX)
        {
            g_loadedTex[i]++;
            break;
        }
    }
#endif
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
    //if (texDatAuto[index].dat.textureId_DX == -1)
    //{
    //    texDatAuto[index].dat.textureId_DX =
    //        LoadTextureExOpt(texDatAuto[index].dat.texturePath, LOADTEXTURETYPE_MAIN);
    //    if (texDatAuto[index].dat.textureId_DX != 0)
    //    {
    //        texDatAuto[index].dat.width = GetTextureWidth(texDatAuto[index].dat.textureId_DX);
    //        texDatAuto[index].dat.height = GetTextureHeight(texDatAuto[index].dat.textureId_DX);
    //    }
    //}

    //texDatAuto[index].loadNum++;

    //for (int i = 0; i < TEX_DATA_LOAD_BUFF_MAX; i++)
    //{
    //    if (g_texDatLoadBuff[i].enable == false)
    //    {
    //        TEX_DAT_LOAD_BUFF buff = {};
    //        buff.enable = true;
    //        buff.unload = false;
    //        buff.textureId = index;
    //        g_texDatLoadBuff[i] = buff;

    //        break;
    //    }
    //}

    //return texDatAuto[index].dat;

#if defined(_DEBUG) || defined(DEBUG)
    g_loadedTex[index]++;
#endif
    return texDatAuto[index].dat;
}

TEX_DAT LoadTexData(std::string name)
{
    //TEX_DAT dat = texDatAuto[0].dat;
    //for (unsigned int i = 0; i < texDatAuto.size(); i++)
    //{
    //    std::string pTmp[2] = {};
    //    pTmp[0] = texDatAuto[i].dat.texturePath;
    //    pTmp[1] = texDat[i].texturePath;
    //    for (int j = 0; j < 2; j++)
    //    {
    //        pTmp[j].erase(remove(pTmp[j].begin(), pTmp[j].end(), ':'), pTmp[j].end());
    //        pTmp[j].erase(remove(pTmp[j].begin(), pTmp[j].end(), '/'), pTmp[j].end());
    //        pTmp[j].erase(remove(pTmp[j].begin(), pTmp[j].end(), '\\'), pTmp[j].end());
    //        pTmp[j].erase(remove(pTmp[j].begin(), pTmp[j].end(), '\n'), pTmp[j].end());
    //        pTmp[j].erase(remove(pTmp[j].begin(), pTmp[j].end(), '\r'), pTmp[j].end());
    //    }

    //    if (texDatAuto[i].name == name
    //       // || pTmp[0] == pTmp[1]
    //        )
    //    {
    //        return LoadTexData(i);
    //    }
    //}
    //return dat;
	TEX_DAT dat = texDatAuto[0].dat;
	for (int i = 0; i < texDatAuto.size(); i++)
	{
		if (texDatAuto[i].name == name)
		{
#if defined(_DEBUG) || defined(DEBUG)
            g_loadedTex[i]++;
#endif
			return texDatAuto[i].dat;
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
    //for (int i = 0; i < TEX_DATA_LOAD_BUFF_MAX; i++)
    //{
    //    if (g_texDatLoadBuff[i].enable == false)
    //    {
    //        TEX_DAT_LOAD_BUFF buff = {};
    //        buff.enable = true;
    //        buff.unload = true;
    //        buff.textureId = tda->dat.textureId;
    //        g_texDatLoadBuff[i] = buff;

    //        break;
    //    }
    //}

    //if (tda->dat.textureId_DX != -1)
    //{
    //    if (tda->loadNum > 0)
    //    {
    //        tda->loadNum--;
    //    }
    //    else
    //    {
    //        UnloadTexture(tda->dat.textureId_DX);
    //    }
    //}
}

void UnloadTexData(unsigned int id)
{
    //unloadTexDataParts(&texDatAuto[id]);
}
void UnloadTexData(std::string name)
{
    //TEX_DAT dat = texDatAuto[0].dat;
    //for (int i = 0; i < texDatAuto.size(); i++)
    //{
    //    if (texDatAuto[i].name == name)
    //    {
    //        unloadTexDataParts(&texDatAuto[i]);
    //        return;
    //    }
    //}
}
void UnloadTexData(TEX_DAT texdat)
{
    //TEX_DAT dat = texDatAuto[0].dat;
    //for (int i = 0; i < texDatAuto.size(); i++)
    //{
    //    if (std::string(texDatAuto[i].dat.texturePath) == texdat.texturePath)
    //    {
    //        unloadTexDataParts(&texDatAuto[i]);
    //        return;
    //    }
    //}
}


unsigned int GetTexId(unsigned int index)
{
    return texDatAuto[index].dat.textureId_DX;
}

std::vector<unsigned int> GetTexIdAll()
{
    //std::vector<unsigned int> r = {};
    //for (int i = 0; i < texDatAuto.size(); i++)
    //{
    //    TEX_DAT texd = LoadTexData(texDatAuto[i].name);
    //    //TEX_DAT texd = GetTexData(texDatAuto[i].name);

    //    r.push_back(texd.textureId_DX);

    //    UnloadTexData(texd);
    //}
    //return r;
    std::vector<unsigned int> r = {};
    for (int i = 0; i < texDatAuto.size(); i++)
    {
        TEX_DAT texd = LoadTexData(texDatAuto[i].name);
        //TEX_DAT texd = GetTexData(texDatAuto[i].name);

        r.push_back(texd.textureId_DX);
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