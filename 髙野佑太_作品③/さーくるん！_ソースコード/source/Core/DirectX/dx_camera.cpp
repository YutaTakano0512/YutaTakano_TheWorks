
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


//グロ－バル変数
static DX_CAMERA_INFO g_CameraInfo[SYS_PLAYER_MAX] = {};


//
// 初期化
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
// 終了処理
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
	if (target.x == pos.x && target.y == pos.y && target.z == pos.z)
	{
		pos.y += 0.01f;
	}
	g_CameraInfo[index].CameraPosRaw = { pos.x, pos.y, pos.z };
	g_CameraInfo[index].CameraTargetRaw = { target.x, target.y, target.z };

	g_CameraInfo[index].CameraUpRaw = { up.x, up.y, up.z };
	//g_CameraInfo[index].CameraScalingRaw = { -scale, scale, scale / 2 };
	g_CameraInfo[index].CameraScalingRaw = { scale, scale, scale };

    //g_CameraInfo[index].CameraPos = XMMatrixPerspectiveFovLH(
    //    fov,
    //    (float)SCREEN_WIDTH / SCREEN_HEIGHT,
    //    0.01f,
    //    4096.0f * 4);

    //XMFLOAT2 vpsize = DXGetWindowSize();
    //XMFLOAT2 vpsize = DXGetViewportSize();
    g_CameraInfo[index].CameraPos = XMMatrixPerspectiveFovLH(
        fov,
        ((float)SCREEN_WIDTH / SCREEN_HEIGHT),
        //vpsize.x / vpsize.y,
        0.1f,
        4096.0f);

    //float scatmp = 0.001f;
    //Float3 scav = NormalizeVector(
    //    MkF3(
    //        g_CameraInfo[index].CameraScalingRaw.x,
    //        g_CameraInfo[index].CameraScalingRaw.y,
    //        g_CameraInfo[index].CameraScalingRaw.z
    //    )
    //);
    //
    //g_CameraInfo[index].CameraPosRaw.x *= scatmp * scav.x;
    //g_CameraInfo[index].CameraPosRaw.y *= scatmp * scav.y;
    //g_CameraInfo[index].CameraPosRaw.z *= scatmp * scav.z;
    //g_CameraInfo[index].CameraTargetRaw.x *= scatmp * scav.x;
    //g_CameraInfo[index].CameraTargetRaw.y *= scatmp * scav.y;
    //g_CameraInfo[index].CameraTargetRaw.z *= scatmp * scav.z;

    g_CameraInfo[index].CameraView = XMMatrixLookAtLH(
        XMLoadFloat3(&g_CameraInfo[index].CameraPosRaw),
        XMLoadFloat3(&g_CameraInfo[index].CameraTargetRaw),
        XMLoadFloat3(&g_CameraInfo[index].CameraUpRaw)
    );
    g_CameraInfo[index].CameraScaling = XMMatrixScaling(
        g_CameraInfo[index].CameraScalingRaw.x,
        g_CameraInfo[index].CameraScalingRaw.y,
        g_CameraInfo[index].CameraScalingRaw.z
    );
    //g_CameraInfo[index].CameraScaling = XMMatrixScaling(
    //    scatmp / abs(g_CameraInfo[index].CameraScalingRaw.x),
    //    scatmp / abs(g_CameraInfo[index].CameraScalingRaw.y),
    //    scatmp / abs(g_CameraInfo[index].CameraScalingRaw.z)
    //);

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

// スクリーン座標をワールド座標に変換
XMVECTOR* CalcScreenToWorld(
	XMVECTOR* pout,
	int Sx,  // スクリーンX座標
	int Sy,  // スクリーンY座標
	float fZ,  // 射影空間でのZ値（0～1）
	int Screen_w,
	int Screen_h,
	XMMATRIX* Scale,
	XMMATRIX* View,
	XMMATRIX* Prj
)
{
	// 各行列の逆行列を算出
	XMMATRIX InvView, InvPrj, InvScale,
		VP, InvViewport;
	InvScale = XMMatrixInverse(NULL, *Scale);
	InvView = XMMatrixInverse(NULL, *View);
	InvPrj = XMMatrixInverse(NULL, *Prj);
	VP = XMMatrixIdentity();

	XMFLOAT4X4 mtx;
	DirectX::XMStoreFloat4x4(&mtx, VP);
	mtx._11 = Screen_w / 2.0f; mtx._22 = -Screen_h / 2.0f;
	mtx._41 = Screen_w / 2.0f; mtx._42 = Screen_h / 2.0f;
	VP = XMLoadFloat4x4(&mtx);

	InvViewport = XMMatrixInverse(NULL, VP);

	// 逆変換
	XMMATRIX tmp = InvViewport * InvPrj * InvView ;
	//XMMATRIX tmp = InvViewport * InvPrj * InvView * InvScale;
	XMFLOAT3 poutf3 = XMFLOAT3(Sx, Sy, fZ);
	*pout = XMVector3TransformCoord(XMLoadFloat3(&poutf3), tmp);

	return pout;
}

DX_SCREEN_TO_VEC_RESULT DX_CalcScreenPosToVec(
	int Sx,
	int Sy,
	int Screen_w,
	int Screen_h,
	XMMATRIX* Scale,
	XMMATRIX* View,
	XMMATRIX* Prj
)
{
	DX_SCREEN_TO_VEC_RESULT r;

	XMVECTOR nearpos;
	XMVECTOR farpos;
	XMVECTOR ray;
	CalcScreenToWorld(&nearpos, Sx, Sy, 0.0f, Screen_w, Screen_h, Scale, View, Prj);
	CalcScreenToWorld(&farpos, Sx, Sy, 1.0f, Screen_w, Screen_h, Scale, View, Prj);
	ray = farpos - nearpos;
	ray = XMVector3Normalize(ray);

	XMStoreFloat3(&r.nearpos, nearpos);
	XMStoreFloat3(&r.farpos, farpos);
	XMStoreFloat3(&r.ray, ray);

	return r;
}

// XZ平面とスクリーン座標の交点算出関数
XMVECTOR* DX_CalcScreenToXZ(
	XMVECTOR* pout,
	int Sx,
	int Sy,
	int Screen_w,
	int Screen_h,
	XMMATRIX* Scale,
	XMMATRIX* View,
	XMMATRIX* Prj
)
{
	//XMFLOAT3 rayFl3 = DX_CalcScreenPosToVec(
	//	Sx,
	//	Sy,
	//	Screen_w,
	//	Screen_h,
	//	Scale,
	//	View,
	//	Prj
	//);
	XMVECTOR nearpos;
	XMVECTOR farpos;
	XMVECTOR ray;
	CalcScreenToWorld(&nearpos, Sx, Sy, 0.0f, Screen_w, Screen_h, Scale, View, Prj);
	CalcScreenToWorld(&farpos, Sx, Sy, 1.0f, Screen_w, Screen_h, Scale, View, Prj);
	ray = farpos - nearpos;
	ray = XMVector3Normalize(ray);

	XMFLOAT3 rayFl3;
	XMStoreFloat3(&rayFl3, ray);


	// 床との交差が起きている場合は交点を
	// 起きていない場合は遠くの壁との交点を出力
	if (rayFl3.y > 0) 
	{
		// 床交点
		XMFLOAT3 Lrayf3 = XMFLOAT3(0.0f, -1.0f, 0.0f);
		//XMFLOAT3 Lrayf3 = XMFLOAT3(0.0f, 1.0f, 0.0f);
		float Lray ;
		XMStoreFloat(&Lray, XMVector3Dot(ray, XMLoadFloat3(&Lrayf3)));

		float LP0 ;
		XMStoreFloat(&LP0, XMVector3Dot((-nearpos), XMLoadFloat3(&Lrayf3)));

		*pout = nearpos + (LP0 / Lray) * ray;
	}
	else
	{
		*pout = farpos;
	}

	return pout;
}