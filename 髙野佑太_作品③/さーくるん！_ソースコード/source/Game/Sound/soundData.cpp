// ===================================================
// soundData.cpp サウンドのセット
// 
// hara sougo  2024/08/22
// ===================================================
#include "../../game_main.h"
#include "../Systems/sound.h"
#include "soundData.h"


    //1. OpenMPの有効化
    //　Visual Studioでは、OpenMPを利用するためにプロジェクト設定を変更する必要がある。
    //　- プロジェクトのプロパティを開く:-メニューからプロジェクト > プロパティを選択。
    //　- OpenMPを有効化:-「構成プロパティ > C / C++ > 言語」セクションで、OpenMP サポートを有効に設定。
    //　- 設定を保存してプロジェクトを再ビルドします。
#include <omp.h>

struct SOUND_DAT_PRESET {
    SOUND_ID id;

    float volume;
    float pitch;
};
struct SOUND_DAT_AUTO {
    SOUND_ID id;

    std::string name;
};

static SOUND_DAT_PRESET soundDat[SOUND_TYPE_MAX] = {};
static std::vector<SOUND_DAT_AUTO> soundDatAuto = {};

static std::vector<FILE_AUTO_LOAD_DAT> g_SoundDatFiles = {};
size_t InitSoundDataFiles()
{
    g_SoundDatFiles = FileScanDirectory(SOUND_PATH, "SOUND");

    return g_SoundDatFiles.size();
}

void InitSoundData()
{
    SOUNDLISTPRESET sd[SOUND_TYPE_MAX] = {};

	SoundDataSet(sd);

	//セット

    //自動読み込み
    //std::vector<FILE_AUTO_LOAD_DAT> files = FileScanDirectory(SOUND_PATH, "SOUND");

    //セット

    //soundDatAuto.resize(files.size());
    //for (int i = 0; i < files.size(); i++)
    //{
    //    soundDatAuto.push_back({});
    //}

// OpenMP並列処理
//#pragma omp parallel for
    for (int i = 0; i < g_SoundDatFiles.size(); i++)
    {
        if (GetExtension(g_SoundDatFiles[i].fPath) != ".wav")
        {
            continue;
        }

        SOUND_DAT_AUTO dat = {};
        dat.name = g_SoundDatFiles[i].name;
        if (dat.name.find("/.") != std::string::npos)
        {
            continue;
        }

        //DebugPrintf("%s\n", files[i].fPath.c_str());
        dat.id = DX_SoundLoad(
            g_SoundDatFiles[i].fPath.c_str(), 
            (dat.name.find("SOUND/BGM") != std::string::npos)
        );


//#pragma omp critical
        {
            // 安全なアクセス
            soundDatAuto.push_back(dat);
            //soundDatAuto[i] = dat;
        }
    }

    //DX_SoundWaitLoad();

    // OpenMP並列処理
    //#pragma omp parallel for
    for (int i = 0; i < SOUND_TYPE_MAX; i++)
    {
#ifdef SWITCH_MODE
        soundDat[i] = sd[i];
#else

        FileLoadAddBefore(1, FILELOADADDBEFORE_TXT_SOUND, FILELOADADD_TYPE_NORMAL);
        ////#pragma omp critical
        //{
        //    // 安全なアクセス
        //    soundDat[i] = DX_SoundLoad(("rom/" + sd[i] + ".wav").c_str());
        //}
        soundDat[i].id = GetSoundData("SOUND/" + sd[i].sound);
        soundDat[i].volume = sd[i].volume;
        soundDat[i].pitch = sd[i].pitch;

        FileLoadAdd(1, FILELOADADD_TXT_SOUND, FILELOADADD_TYPE_NORMAL);
#endif
    }

    g_SoundDatFiles.clear();
    g_SoundDatFiles.shrink_to_fit();
}

void UninitSoundData()
{
	for (int i = 0; i < SOUND_TYPE_MAX; i++)
	{
	}
}

SOUND_ID GetSoundData(SOUND_TYPE type)
{
    return soundDat[type].id;
}
float GetSoundDataPreset_Volume(SOUND_TYPE type)
{
    return soundDat[type].volume;
}

float GetSoundDataPreset_Pitch(SOUND_TYPE type)
{
    return soundDat[type].pitch;
}

SOUND_ID GetSoundData(std::string name)
{
    SOUND_ID dat = -1;
    for (int i = 0; i < soundDatAuto.size(); i++)
    {
        if (soundDatAuto[i].name == name)
        {
            return soundDatAuto[i].id;
        }
    }
    return dat;
}

std::vector<SOUND_ID> GetSoundIdAll()
{
    std::vector<SOUND_ID> dat = {};
    for (int i = 0; i < soundDatAuto.size(); i++)
    {
        dat.push_back(soundDatAuto[i].id);
    }
    return dat;
}