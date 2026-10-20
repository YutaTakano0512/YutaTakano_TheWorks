// ===================================================
// texture.cpp テクスチャの制御
// 
// hara sougo  2024/11/08
// ===================================================

#define IS_TEXDATA

#include "../../Core/_systemModeSwitch.h"

#include "../../game_main.h"
#include "texture.h"


#ifdef SWITCH_MODE
#else

#include "../../Core/DirectX/dx.h"

#include <d3d11.h>
#include <DirectXMath.h>
#include "../../../DirectXTex.h"
using namespace DirectX;
#include <future>


#define TEXTURE_MAX (2048)

//構造体
struct TextureDat_DX {
    bool use;
	ID3D11ShaderResourceView* texture;
	std::string path;
	int width;
	int hight;

    std::future<void> loadtask;
};

//グローバル変数
static TextureDat_DX g_Textures[TEXTURE_MAX] = {};

// 注意！初期化で外部から設定されるもの。Release不要。
static ID3D11Device* g_Device = NULL;

//constexpr int LOADINGNUMMAX = 4;
static int g_loadingNumMax = 1;
static int g_loadingNum = 0;

#endif


void InitTexture(void)
{
#ifdef SWITCH_MODE
#else
    g_loadingNumMax = std::thread::hardware_concurrency();
	g_Device = DXGetDevice();
	LoadTexture("rom:/texture/gray.tga", true);
#endif

}

void UninitTexture(void)
{
#ifdef SWITCH_MODE
#else

	for (int i = 0; i < TEXTURE_MAX; i++)
	{ //テクスチャの解放
		if (g_Textures[i].texture != nullptr)
		{
			SAFE_RELEASE(g_Textures[i].texture);
		}
	}

#endif
}

void loadTextureTask(TextureDat_DX* texDatp)
{
    //現在ロード中の数が最大数を超えていたら待つ
    while (g_loadingNum >= g_loadingNumMax)
    {
        Sleep(1);
    }

    g_loadingNum++;

    FileLoadAddBefore(1, FILELOADADDBEFORE_TXT_TEX, FILELOADADD_TYPE_NORMAL);
    // 非同期タスクで画像をロード
    //auto task = std::async(std::launch::async, [&]() {
    //    {
    //    }
    //});

    // タスクの完了を待機
    //task.wait();
//#pragma omp parallel
    {
 /*       size_t FileNameSize = strlen(texDatp->path.c_str()) + 1;

        wchar_t* wcFileName = new wchar_t[FileNameSize];

        size_t convertedChars = 0;
        mbstowcs_s(&convertedChars, wcFileName, FileNameSize, texDatp->path.c_str(), _TRUNCATE);*/

        std::wstring wcFileName = StringToWString(texDatp->path);

        ID3D11ShaderResourceView* texture_temp = NULL;
        {

            //char ms[100];
            //size_t msNum = 0;
            ////wcstombs(ms, (const wchar_t*)wcFileName, 100);
            //wcstombs_s(&msNum, ms, 100, (const wchar_t*)wcFileName.c_str(), 100);
            //char* extension = strstr(ms, ".");

            char ms[256];
            strcpy_s(ms, 256, texDatp->path.c_str());
            char* extension = strstr(ms, ".");

            if (extension != NULL)
            {

                size_t sw = 0;
                size_t sh = 0;
                TexMetadata metadata;

                {
                    if (strcmp(extension, ".tga") == 0 || strcmp(extension, ".TGA") == 0)
                    { //tga
                        GetMetadataFromTGAFile((const wchar_t*)wcFileName.c_str(), metadata);

                        std::unique_ptr<ScratchImage> image(new ScratchImage);
                        HRESULT hr = LoadFromTGAFile((const wchar_t*)wcFileName.c_str(), &metadata, *image);
                        if (FAILED(hr))
                        {
                        }
                        hr = CreateShaderResourceView(g_Device, image->GetImages(), image->GetImageCount(), metadata, &texture_temp);
                        if (FAILED(hr))
                        {
                        }

                        sw = image->GetImages()[0].width;
                        sh = image->GetImages()[0].height;
                    }
                    else
                    { //png
                        ScratchImage image;
                        //ScratchImage imageRe;
                        //LoadFromWICFile((const wchar_t*)wcFileName, WIC_FLAGS_NONE, &metadata, image);
                        LoadFromWICFile(
                            (const wchar_t*)wcFileName.c_str(), 
                            WIC_FLAGS_FILTER_LINEAR, 
                            &metadata, image);
                        //metadata.width *= 0.5f;
                        //metadata.height *= 0.5f;

                        //圧縮処理
                        //if (false) //処理がおっせえのでいったん無効化
                        ////if (metadata.width >= 4000 && metadata.height >= 4000)
                        //{
                        //    // 縮小後のサイズを設定
                        //    size_t newWidth = metadata.width / 2;
                        //    size_t newHeight = metadata.height / 2;

                        //    // 画像を縮小する
                        //    Resize(image.GetImages(), image.GetImageCount(), metadata,
                        //        newWidth, newHeight, TEX_FILTER_DEFAULT, imageRe);

                        //    CreateShaderResourceView(g_Device, imageRe.GetImages(), imageRe.GetImageCount(),
                        //        imageRe.GetMetadata(), &texture_temp);

                        //}

                        //if (false) //処理がおっせえのでいったん無効化
                        ////if (metadata.width >= 1000 && metadata.height >= 1000)
                        //{
                        //    Convert(image.GetImages(), image.GetImageCount(), metadata, 
                        //        DXGI_FORMAT_R8G8B8A8_UNORM_SRGB,
                        //        TEX_FILTER_DEFAULT,
                        //        0.0f, imageRe);

                        //    CreateShaderResourceView(g_Device, imageRe.GetImages(), imageRe.GetImageCount(),
                        //        imageRe.GetMetadata(), &texture_temp);

                        //}
                        //else
                        {
                            // ミップマップのレベルを制限
                            metadata.mipLevels = 1;

                            CreateShaderResourceView(g_Device,
                                image.GetImages(), image.GetImageCount(),
                                metadata, &texture_temp);

                            // ミップマップを生成
                            //DXGetDeviceContext()->GenerateMips(texture_temp);
                        }

                        if (texture_temp)
                        {
                            sw = image.GetImages()[0].width;
                            sh = image.GetImages()[0].height;
                        }
                    }
                }

                if (!texture_temp) {
                    //テクスチャが読み込めなかった
                    MessageBoxW(NULL, L"テクスチャが読み込めなかった", wcFileName.c_str(), MB_OK);
                }
                else
                {
                    texDatp->width = (int)sw;
                    texDatp->hight = (int)sh;
                    texDatp->texture = texture_temp;
                    //for (int i = 0; i < TEXTURE_MAX; i++)
                    //{
                    //    if (g_Textures[i].texture == nullptr)
                    //    {
                    //        //g_Textures[i].width = (int)metadata.width;
                    //        //g_Textures[i].hight = (int)metadata.height;
                    //        g_Textures[i].width = (int)sw;
                    //        g_Textures[i].hight = (int)sh;
                    //        g_Textures[i].texture = texture_temp;
                    //        g_Textures[i].path = fpTmp;
                    //        texId = i;

                    //        /*{
                    //            char c[256] = {};
                    //            sprintf(c, "%d", i);
                    //            MessageBox(NULL, "テクスチャ", c, MB_OK);
                    //        }*/
                    //        break;
                    //    }
                    //}

                    FileLoadAdd(1, FILELOADADD_TXT_TEX, FILELOADADD_TYPE_NORMAL);
                }
            }
        }

        //delete[] wcFileName;
    }
    g_loadingNum--;
}

unsigned int LoadTexture(const char* FileName, bool force)
{
#ifdef SWITCH_MODE
#else
        //ファイルパス
        std::string fnTmp = std::string(FileName);
        if (fnTmp[3] == ':')
        {
            fnTmp.replace(3, 1, "");
        }
        std::string fpTmp = /*"data/" +*/ fnTmp;

        if (!force)
        { //かぶりチェック	
            for (int i = 0; i < TEXTURE_MAX; i++)
            {
                if (g_Textures[i].use)
                {
                    std::string pTmp[2] = {};
                    pTmp[0] = fpTmp;
                    pTmp[1] = g_Textures[i].path;
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
        }

        unsigned int texId = 0;

        for (int i = 0; i < TEXTURE_MAX; i++)
        {
            if (!g_Textures[i].use)
            {
                g_Textures[i].use = true;
                g_Textures[i].path = fpTmp;
                g_Textures[i].loadtask = std::async(
                    std::launch::async,
                    loadTextureTask,
                    g_Textures + i
                );
                //loadTextureTask(g_Textures + i);
                texId = i;

                break;
            }
        }

//#pragma omp parallel
        return texId;
#endif
}


unsigned int LoadTextureExOpt(const char* FileName, LOADTEXTURETYPE opt)
{
#ifdef SWITCH_MODE
#else

return LoadTexture(FileName, false);

#endif
}
TEXTURE_DAT LoadTextureEx(const char* FileName)
{
	TEXTURE_DAT rtd = {};
	rtd.main = LoadTextureExOpt(FileName, LOADTEXTURETYPE_MAIN);
	rtd.gray = LoadTextureExOpt(FileName, LOADTEXTURETYPE_GRAY);

	return rtd;
}



void UnloadTexture(unsigned int Texture)
{
#ifdef SWITCH_MODE
#else
	SAFE_RELEASE(g_Textures[Texture].texture);
	g_Textures[Texture].texture = nullptr;
#endif
}





void SetTexture(unsigned int Texture)
{
#ifdef SWITCH_MODE
#endif
}

void WaitLoadTexture()
{   
    // タスクの完了を待つ
    //bool isFinish = true;
    //do {
    //    for (int i = 0; i < TEXTURE_MAX; i++)
    //    {
    //        if (g_Textures[i].loadtask.valid())
    //        {
    //            g_Textures[i].loadtask.get();
    //            isFinish = false;
    //            break;
    //        }
    //    }
    //} while (isFinish);
    
    for (int i = 0; i < TEXTURE_MAX; i++)
    {
        if (g_Textures[i].loadtask.valid())
        {
            g_Textures[i].loadtask.get();
        }
    }
}


#ifdef SWITCH_MODE
#else

ID3D11ShaderResourceView* GetTexture_DX(unsigned int Texture)
{
	return g_Textures[Texture].texture;
}
int GetTextureWidth(unsigned int Texture)
{
	return g_Textures[Texture].width;
}
int GetTextureHeight(unsigned int Texture)
{
	return g_Textures[Texture].hight;
}

#endif