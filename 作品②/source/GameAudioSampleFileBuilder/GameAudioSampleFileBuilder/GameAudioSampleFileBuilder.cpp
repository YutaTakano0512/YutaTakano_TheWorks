// GameAudioSampleFileBuilder.cpp : このファイルには 'main' 関数が含まれています。プログラム実行の開始と終了がそこで行われます。
//

#include <iostream>
#include "Audio.h"

#include <fstream>

struct FILE_AUTO_LOAD_DAT {
    std::string lPath;
    std::string fPath;

    std::string name;
};

std::vector<FILE_AUTO_LOAD_DAT> FileScanDirectory(const std::string& dir_name, const std::string& f_name)
{
    HANDLE fHandle;
    WIN32_FIND_DATA win32fd;
    std::vector<FILE_AUTO_LOAD_DAT> file_names;

    std::string search_name = dir_name + "\\*";

    fHandle = FindFirstFile(search_name.c_str(), &win32fd);

    if (fHandle == INVALID_HANDLE_VALUE)
    {
        // "'GetLastError() == 3' is 'file not found'"
        return file_names;
    }

    do {
        if (win32fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
        {//ディレクトリである

            //そのディレクトリそのものを表す場合は処理しない
            if (win32fd.cFileName[0] == '.') continue;

            std::string fullpath = dir_name + "\\" + win32fd.cFileName;

            std::string f_nameTmp = f_name + "/" + win32fd.cFileName;

            //再帰的にファイルを検索
            std::vector<FILE_AUTO_LOAD_DAT> files = FileScanDirectory(fullpath, f_nameTmp);

            file_names.insert(file_names.end(), files.begin(), files.end());
        }
        else
        {//ファイルである
            FILE_AUTO_LOAD_DAT dat = {};
            dat.lPath = win32fd.cFileName;
            dat.fPath = dir_name + "\\" + win32fd.cFileName;

            char* cpytxtContext = nullptr;
            char* token = strtok_s(win32fd.cFileName, ".", &cpytxtContext);

            dat.name = f_name + "/" + token;

            file_names.emplace_back(dat);
        }
    } while (FindNextFile(fHandle, &win32fd));

    FindClose(fHandle);

    return file_names;
}

static std::vector<FILE_AUTO_LOAD_DAT> g_SoundDatFiles = {};

int main()
{
    HRESULT hr = CoInitializeEx(NULL, COINIT_MULTITHREADED);
    if (FAILED(hr)) {
        printf("CoInitializeEx failed: 0x%X\n", hr);
    }
    std::cout << "Start\n";
    InitAudio();

    g_SoundDatFiles = FileScanDirectory("BGM", "SOUND");
    for (int i = 0; i < g_SoundDatFiles.size(); i++)
    {
        std::cout << g_SoundDatFiles[i].fPath + " Start\n";

        int id = LoadAudio(g_SoundDatFiles[i].fPath.c_str(), true);

        std::vector<float> dat = GetAudioDat(id);

        {

            std::string fnametmp = g_SoundDatFiles[i].lPath;
            int numCharsToRemove = 3; // 末尾を削除
            if (numCharsToRemove <= fnametmp.length()) {
                fnametmp.erase(fnametmp.length() - numCharsToRemove);
            }


            // ファイル名の指定
            std::string filename = "soundsample/" + fnametmp + "sp";

            // テキストファイルを開く (書き込みモード)
            std::ofstream file(filename);
            //if (!file.is_open()) {

            //    // テキストファイルを開く
            //    std::ifstream file2(filename);
            //    if (!file2.is_open()) {
            //        return;
            //    }

            //    // ファイルを閉じる
            //    file2.close();

            //}

            // データをファイルに書き込む
            for (const auto& value : dat)
            {
                file << value << std::endl;
            }

            // ファイルを閉じる
            file.close();
        }

        UnloadAudio(id);
        std::cout << g_SoundDatFiles[i].fPath + " End\n";
    }


    UninitAudio();
    std::cout << "完了\n";
    std::cin.get(); // ユーザーがキーを押すまで待機

    CoUninitialize();
}

// プログラムの実行: Ctrl + F5 または [デバッグ] > [デバッグなしで開始] メニュー
// プログラムのデバッグ: F5 または [デバッグ] > [デバッグの開始] メニュー

// 作業を開始するためのヒント: 
//    1. ソリューション エクスプローラー ウィンドウを使用してファイルを追加/管理します 
//   2. チーム エクスプローラー ウィンドウを使用してソース管理に接続します
//   3. 出力ウィンドウを使用して、ビルド出力とその他のメッセージを表示します
//   4. エラー一覧ウィンドウを使用してエラーを表示します
//   5. [プロジェクト] > [新しい項目の追加] と移動して新しいコード ファイルを作成するか、[プロジェクト] > [既存の項目の追加] と移動して既存のコード ファイルをプロジェクトに追加します
//   6. 後ほどこのプロジェクトを再び開く場合、[ファイル] > [開く] > [プロジェクト] と移動して .sln ファイルを選択します
