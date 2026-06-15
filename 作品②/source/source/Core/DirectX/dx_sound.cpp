//---------------------------------------------------------------------------
// sound.cpp
// 
// sougo hara  2024/06/05
//---------------------------------------------------------------------------

#include "../../Game/Configuration/configuration.h"
#include "dx.h"
#include <d3d11.h>
#include <DirectXMath.h>
#include "../../../DirectXTex.h"
using namespace DirectX;
#include "Audio.h"
#include "dx_sound.h"


//
// マクロ定義
//

//
// 構造体宣言
//
struct SoundDat {
	int index;
    std::string path;
};

//
// プロトタイプ宣言
//

//
// グローバル変数
//

static SoundDat g_Sounds[1024] = {};
static int g_SoundCount = 0;
//
// テクスチャ初期化
//
void DX_SoundInitialize()
{
	InitAudio();
}

//
// 終了
// 
void DX_SoundFinalize(void)
{
	for (int i = 0; i < g_SoundCount; i++)
	{ //解放
		UnloadAudio(g_Sounds[i].index);
	}
}

void DX_SoundWaitLoad()
{
    WaitLoadAudio();
}

//
// 読み込み
//
int DX_SoundLoad(const char* path, bool isBGM)
{
    { //かぶりチェック	
        for (int i = 0; i < g_SoundCount; i++)
        {
            std::string pTmp[2] = {};
            pTmp[0] = path;
            pTmp[1] = g_Sounds[i].path;
            for (int j = 0; j < 2; j++)
            {
                pTmp[j].erase(remove(pTmp[j].begin(), pTmp[j].end(), '/'), pTmp[j].end());
                pTmp[j].erase(remove(pTmp[j].begin(), pTmp[j].end(), '\\'), pTmp[j].end());
                pTmp[j].erase(remove(pTmp[j].begin(), pTmp[j].end(), '\n'), pTmp[j].end());
                pTmp[j].erase(remove(pTmp[j].begin(), pTmp[j].end(), '\r'), pTmp[j].end());
            }
            if (pTmp[0] == pTmp[1])
            {
                return i;
            }
        }
    }

	int index = LoadAudio(path, isBGM);
	if (index < 0) {
		//読み込めなかった
		MessageBox(DXGetWindowHandle(), "サウンドが読み込めなかった", path, MB_OK);
	}
	else
	{
//#pragma omp critical
        {
            g_Sounds[g_SoundCount].path = path;
            g_Sounds[g_SoundCount].index = index;
        }
	}

	int rNum = g_SoundCount;
	g_SoundCount++;

	return rNum;
}

void DX_SoundSetVolume(int id, float volume)
{
	if (id < 0 || id >= g_SoundCount)
	{
		return;
	}
	SetVolumeAudio(id, volume);
}
void DX_SoundStop(int id)
{
	if (id < 0 || id >= g_SoundCount)
	{
		return;
	}
	StopAudio(id);
}

void DX_SoundPlay(
    int id, float volume, bool loop, float pitch,
    float playBeginSecond,
    float playEndSecond
)
{
	if (id < 0 || id >= g_SoundCount)
	{
		return;
	}
	PlayAudio(id, volume, loop, pitch, playBeginSecond, playEndSecond);
}
