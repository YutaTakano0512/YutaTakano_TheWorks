#include "../../game_main.h"

#include "../../Core/_systemModeSwitch.h"

#include "../../Core/DirectX/dx.h"


void InitSystem()
{
}




void UninitSystem()
{
}


void SystemClearB()
{
	DXClearBackBuffer();
}

void SystemSwapBuffers()
{
    DXSwapBuffer();
}