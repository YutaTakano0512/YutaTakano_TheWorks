
//---------------------------------------------------------------------------
// dx_camera.cpp
// 
// sougo hara  2024/11/23
//---------------------------------------------------------------------------

#include "../../Game/Configuration/configuration.h"
#include "../../Game/Systems/sys_general.h"
#include "../../main.h"

#include "../../Core/DirectX/dx_shader.h"

#include "dx.h"
#include "dx_camera.h"
#include "../../game_main.h"


//ƒOƒ|ƒoƒ‹•Ï”
static DX_CAMERA_INFO g_CameraInfo[SYS_PLAYER_MAX] = {};


//
// ‰Šú‰»
//
void DX_CameraInitialize(void)
{
    for (int i = 0; i < SYS_PLAYER_MAX; i++)
    {
        DX_SetCameraInfo(
            i,
            1.0f,
            { 0.0f, 0.0f, -1.0f },
            { 0.0f, 0.0f, 0.0f },
            { 0.0f, -1.0f, 0.0f },
            0.01f
        );
    }
}

//
// I—¹ˆ—
//
void DX_CameraFinalize(void)
{
}


DX_CAMERA_INFO DX_GetCameraInfo(int index)
{
	return g_CameraInfo[index];
}


void DX_SetCameraInfo(int index, DX_CAMERA_INFO info)
{
    g_CameraInfo[index] = info;
}

void DX_SetCameraInfo(int index,
    float fov, XMFLOAT3 pos, XMFLOAT3 target, XMFLOAT3 up, float scale)
{
	g_CameraInfo[index].CameraPosRaw = { pos.x, pos.y, pos.z };
	g_CameraInfo[index].CameraTargetRaw = { target.x, target.y, target.z };
	g_CameraInfo[index].CameraUpRaw = { up.x, up.y, up.z };
	//g_CameraInfo[index].CameraScalingRaw = { -scale, scale, scale / 2 };
	g_CameraInfo[index].CameraScalingRaw = { -scale, scale, scale };

	g_CameraInfo[index].CameraPos = XMMatrixPerspectiveFovLH(fov, (float)SCREEN_WIDTH / SCREEN_HEIGHT, 1.0f, 1000.0f);

	g_CameraInfo[index].CameraView = XMMatrixLookAtLH(XMLoadFloat3(&pos), XMLoadFloat3(&target), XMLoadFloat3(&up));

	g_CameraInfo[index].CameraScaling = XMMatrixScaling(
		g_CameraInfo[index].CameraScalingRaw.x,
		g_CameraInfo[index].CameraScalingRaw.y,
		g_CameraInfo[index].CameraScalingRaw.z
	);

	Float3 nv = NormalizeVector(MkF3(pos.x - target.x, pos.y - target.y, pos.z - target.z));
	g_CameraInfo[index].CameraVector = { nv.x, nv.y, nv.z };

	//DebugPrintf("nv x: %f y: %f z: %f\n",
	//	pos.x - target.x,
	//	pos.y - target.y,
	//	pos.z - target.z
	//);
	//DebugPrintf("nvr x: %f y: %f z: %f\n",
	//	g_CameraInfo.CameraVector.x,
	//	g_CameraInfo.CameraVector.y,
	//	g_CameraInfo.CameraVector.z
	//);
}