//---------------------------------------------------------------------------
// dx_camera.h
// 
// sougo hara  2024/11/23
//---------------------------------------------------------------------------
#ifndef _DX_CAMERA_H_
#define _DX_CAMERA_H_

#include <d3d11.h>
#include <DirectXMath.h>
#include "../../../DirectXTex.h"
using namespace DirectX;

//
// 構造体宣言
//
struct DX_CAMERA_INFO {

	XMMATRIX CameraPos;
	XMMATRIX CameraView;
	XMMATRIX CameraScaling;

	XMFLOAT3 CameraPosRaw;
	XMFLOAT3 CameraTargetRaw;
	XMFLOAT3 CameraUpRaw;
	XMFLOAT3 CameraScalingRaw;

	XMFLOAT3 CameraVector;
};

//
// プロトタイプ宣言
//
void DX_CameraInitialize(void);
void DX_CameraFinalize(void);

DX_CAMERA_INFO DX_GetCameraInfo(int index);

void DX_SetCameraInfo(int index, DX_CAMERA_INFO info);
void DX_SetCameraInfo(int index,
    float fov, XMFLOAT3 pos, XMFLOAT3 target, XMFLOAT3 up, float scale);

struct DX_SCREEN_TO_VEC_RESULT {
	XMFLOAT3 nearpos;
	XMFLOAT3 farpos;
	XMFLOAT3 ray;
};
DX_SCREEN_TO_VEC_RESULT DX_CalcScreenPosToVec(
	int Sx,
	int Sy,
	int Screen_w,
	int Screen_h,
	XMMATRIX* Scale,
	XMMATRIX* View,
	XMMATRIX* Prj
);
XMVECTOR* DX_CalcScreenToXZ(
	XMVECTOR* pout,
	int Sx,
	int Sy,
	int Screen_w,
	int Screen_h,
	XMMATRIX* Scale,
	XMMATRIX* View,
	XMMATRIX* Prj
);

#endif