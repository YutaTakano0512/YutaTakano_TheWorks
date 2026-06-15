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

#endif