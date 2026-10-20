#include <windows.h>
#include <iostream>

int APIENTRY WinMain(_In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPSTR lpCmdLine,
    _In_ int nCmdShow)
{
    SHELLEXECUTEINFO sei = { sizeof(sei) };
    //sei.lpVerb = "runas"; // 管理者権限で実行
    sei.lpFile = "rom\\app\\main.exe";
    sei.nShow = SW_SHOWNORMAL;

    if (ShellExecuteEx(&sei))
    {
        //MessageBox(NULL, "アプリが正常に起動しました！", "成功", MB_OK);
    }
    else {
        //MessageBox(NULL, "起動に失敗しました。", "エラー", MB_OK);
    }


    return 0;
}