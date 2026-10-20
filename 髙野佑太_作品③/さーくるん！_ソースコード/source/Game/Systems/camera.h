// ===================================================
// camera.h カメラ制御
// 
// hara sougo  2024/07/23
// ===================================================
#ifndef _CAMERA_H_
#define _CAMERA_H_


#include "sys_general.h"
#include "easing.h"
#include "../Sprite/sprite.h"

// ===================================================
// マクロ定義
// ===================================================
#define CAMERA_NUM	(SYS_PLAYER_MAX)
//#define CAMERA_DRAW_BUF_MAX	(512)
#define CAMERA_DRAW_BUF_MAX	(1024)
//#define CAMERA_DRAW_BUF_MAX	(2048)


#define CAMERA_SHAKE_SMALL_FRAME	(15)
#define CAMERA_SHAKE_SMALL_POWER	(4.0f * 3.0f)

#define CAMERA_SHAKE_MIDIUM_FRAME	(25)
#define CAMERA_SHAKE_MIDIUM_POWER	(8.0f * 3.0f)

#define CAMERA_SHAKE_LARGE_FRAME	(35)
#define CAMERA_SHAKE_LARGE_POWER	(16.0f * 3.0f)

#define CAMERA_SHAKE_EXLARGE_FRAME	(60)
#define CAMERA_SHAKE_EXLARGE_POWER	(32.0f * 3.0f)


#define CAMERA_NORMAL_ZOOM			(1.75f)
//#define CAMERA_PLAYER_CHASE_ZOOM	(1.45f)
//#define CAMERA_PLAYER_CHASE_ZOOM	(0.42f)
#define CAMERA_PLAYER_CHASE_ZOOM	(0.38f)
//#define CAMERA_PLAYER_CHASE_ZOOM	(1.2f)
//#define CAMERA_PLAYER_CHASE_ZOOM	(1.0f)

#define CAMERA_PLAYER_CHASE_POS_X	(0.0f)
#define CAMERA_PLAYER_CHASE_POS_Y	(240.0f)
#define CAMERA_PLAYER_CHASE_POS_Z	(-160.0f)

#define CAMERA_PLAYER_CHASE_MOVE_POS_X	(300.0f)
#define CAMERA_PLAYER_CHASE_MOVE_POS_Y	(80.0f)

#define CAMERA_PLAYER_CHASE_ANGLE_POS_X	(50.0f)
#define CAMERA_PLAYER_CHASE_ANGLE_POS_Y	(20.0f)

#define CAMERA_PLAYER_CHASE_MAX_SPEED   (6.0f)

#define CAMERA_3D_MOD   (20.0f)

// ===================================================
// 列挙体定義
// ===================================================

//idou の種類
enum CAMERA_MOVE_TYPE {
	CAMERA_MOVE_TYPE_NORMAL = 0,
	CAMERA_MOVE_TYPE_SMOOTH,
	CAMERA_MOVE_TYPE_FADE,

	CAMERA_MOVE_TYPE_MAX,
};

//idou の種類
enum CAMERA_MODE {
	CAMERA_MODE_NORMAL = 0,
    CAMERA_MODE_PLAYER_CHASE,
    CAMERA_MODE_PLAYER_CHASE_INSTANT,
    CAMERA_MODE_BOSS_CHASE,
    CAMERA_MODE_BOSSANDZAKO_CHASE,
    CAMERA_MODE_BOSS_CHASE_WIDE,
    CAMERA_MODE_BOSS_CHASE_WIDEMIDDLE,
    CAMERA_MODE_BOSS_CHASE_EXWIDE,

	CAMERA_MODE_MAX,
};

// ===================================================
// 構造体宣言
// ===================================================
struct CAMERA_DRAW_BUF { //描画情報バッファ
    bool enable;
    DRAW_POLYGON_BUFFER dpb;
    POLYGON_RELEASE_OPTION pro;
};


struct CAMERA_TRANS {		// カメラの状態構造体
	Float3 pos;
	Float3 target;
	float rot;
	Float2 scale;
	float zoom;
};

struct CAMERA_MOVE_REQUEST {		// 移動リクエスト
	bool enable;

	CAMERA_TRANS startTrans; // 移動前
	CAMERA_TRANS endTrans; // 移動先

	//Float2 oldPos;		// 1F前の移動先の座標
	int frame;
	int frameMax;

	//イージング関数を指定する
	std::function<float(float)> easing;

	CAMERA_MOVE_TYPE moveType;
};

struct CAMERA_FLASH_REQUEST {		// フラッシュリクエスト
	int frame;
	int frameMax;

	//Float4 color;
    POLYGON_RELEASE_OPTION opt;

	//イージング関数を指定する
	std::function<float(float)> easing;

};

struct CAMERA_GAME_OBJECT_ZOOM_REQUEST {	// ズームリクエスト
	bool isUse;

	int objIndex;
	unsigned int objCreateFrame;

	Float3 pos;

	float zoom;
	float speed;

	float zoomScale;

	//イージング関数を指定する
	std::function<float(float)> easing;
};
struct CAMERA_GAME_OBJECT_ZOOM_DELAY_REQUEST {
	int frame;
	int frameMax;

	CAMERA_GAME_OBJECT_ZOOM_REQUEST dat;
};

struct CAMERA_MODE_SET_REQUEST {		// カメラモード変更リクエスト
	int frame;
	int frameMax;

	CAMERA_MODE mode;
};
struct CAMERA_REVERSE_REQUEST {	// 反転リクエスト
	bool isUse;

	float mod;
	float speed;

	//イージング関数を指定する
	std::function<float(float)> easing;
};

struct CAMERA_PLAYER_CHASE {	// プレイヤーをおう
	//Float2 angle;
    float velSubRequ;
    float velSubRequSpd;
    //float velBuf;
    float vel;
    float playerBeforeSpd;
    //float playerBeforeVecSpd;
    Float3 playerBeforeVec;

    Float3 cameraPosPlayerAxisAdd;

    Float3 cameraTargetModAdd;
    Float3 cameraPosModAdd;

    bool cameraPosBufModdingFlag;
    Float3 cameraPosBuf;
    bool cameraTargetBufModdingFlag;
    Float3 cameraTargetBuf;

    Float3 cameraPosProvision;
    Float3 cameraTargetProvision;
    bool cameraZoomProvision;
};


struct CAMERA_3D {		// カメラ3D構造体
	float fov;
	Float3 pos;
	Float3 target;
	Float3 up;
	float scale;

};

struct CAMERA_DRAW_DAT {
	Float2 shakePos;

	Float2 zoomAddPos;
	float zoomAddScale;

	float revScaleY;
};

struct CAMERA {		// カメラ構造体
	CAMERA_TRANS trans;

    float shakePower;
    float shakePowerY;
	int shakeFrame;
	int shakeFrameMax;

	CAMERA_MOVE_REQUEST moveRequ;

	CAMERA_FLASH_REQUEST flashRequ;
	CAMERA_MODE_SET_REQUEST modeSetRequ;

	CAMERA_REVERSE_REQUEST revRequ;
	CAMERA_GAME_OBJECT_ZOOM_DELAY_REQUEST goZoomDelayRequ;
	CAMERA_GAME_OBJECT_ZOOM_REQUEST goZoomRequ;
	CAMERA_PLAYER_CHASE playerChase;
	CAMERA_MODE mode;
    Int2 modeChangeLerpFrame;

	CAMERA_3D camera3D; //3D用

	Float3* targetRef;
	CAMERA_MOVE_REQUEST moveRequTarget;

	CAMERA_DRAW_DAT cameraDrawDat; //描画用変数
};



struct DRAW_POLYGON_IN_CAMERA_EX_OPTION {
	bool reverse;
	bool fixed;
	bool cameraFix;

    bool enableShade; //機能なし
    DPD_SHADER_TYPE shaderType;
    DPD_SHADER_TYPE_EX_OPT shaderTypeEx;

	bool billboard;

    DRAW_POLYGON_PRIORITY priority;
    bool depthBuffIgnore;
};
struct DRAW_POLYGON_IN_CAMERA_EX_MAT_EX {
    //DRAW_POLYGON_BUFFER dpb;
    //POLYGON_RELEASE_OPTION ropt;
    //XMMATRIX mat;
    bool isFixed;
    Float2 offset;
};


// ===================================================
// プロトタイプ宣言
// ===================================================
void InitCamera(void);
void ResetCamera(void);

void UpdateCamera(void);

void UninitCamera(void);
void DrawCamera(void);
void DrawCameraBufferRelease(void);
void DrawCameraBufferDrawOnly();
void DrawCameraBufferReleaseOnly();
void CameraAddDrawBuf(int index, CAMERA_DRAW_BUF buf);
//const std::vector<CAMERA_DRAW_BUF>& CameraGetDrawBuf(int index);
CAMERA_DRAW_BUF* CameraGetDrawBuf(int index);


void DrawSpriteInCameraSingle(
    int index,
    float x, float y, float w, float h,
    float rot,
    Float4 color,
    unsigned int texNo,
    Float2 texUV, Float2 texWH,
    DRAW_POLYGON_PRIORITY priority = DRAW_POLYGON_PRIORITY_FORCE_NORMAL,
    DPD_SHADER_TYPE shadertype = DPD_SHADER_TYPE_NONE,
    DPD_SHADER_TYPE_EX_OPT shadertypeExOpt = {}
);
void DrawSpriteInCamera(
    float x, float y, float w, float h,
    float rot,
    Float4 color,
    unsigned int texNo,
    Float2 texUV, Float2 texWH,
    DRAW_POLYGON_PRIORITY priority = DRAW_POLYGON_PRIORITY_FORCE_NORMAL,
    DPD_SHADER_TYPE shadertype = DPD_SHADER_TYPE_NONE,
    DPD_SHADER_TYPE_EX_OPT shadertypeExOpt = {}
);
void DrawSpriteInCamera(DRAW_SPRITE_DAT drawDat,
	bool reverse = false,
	bool fixed = false,
	bool enableShade = false
);
void DrawPolygonInCamera(DRAW_POLYGON_DAT drawDat,
	bool reverse = false,
	bool fixed = false,
	bool enableShade = false
);
void DrawPolygonInCameraEx(DRAW_POLYGON_DAT drawDat,
	DRAW_POLYGON_IN_CAMERA_EX_OPTION opt
);

XMMATRIX DrawPolygonInCameraCalMatrix(
    DRAW_POLYGON_BUFFER& dpb,
    POLYGON_RELEASE_OPTION& ropt,

    int index,
    DRAW_POLYGON_DAT drawDat,
    DRAW_POLYGON_IN_CAMERA_EX_OPTION opt
);
CAMERA_DRAW_BUF DrawPolygonInCameraSingle(int index,
    DRAW_POLYGON_DAT drawDat,
    DRAW_POLYGON_IN_CAMERA_EX_OPTION opt,
    DRAW_POLYGON_IN_CAMERA_EX_MAT_EX* polMatref =nullptr
);

Float2 CameraPosToWorldPos(int index, Float2 cp);
Float2 WorldPosToCameraPos(int index, Float3 wp);

Float2 GetCameraSize(int index);
Float3 GetCameraPos(int index);

Float3 GetCameraPCPPos(int index);
Float3 GetCameraPCPTarget(int index);
float GetCameraPCPZoom(int index);


void CameraShakeRequestAll(int frame, float power,
    float powerY = -1.0f, 
    bool isPriorityHighPower = false
);
void CameraShakeRequest(int index, int frame, float power, 
    float powerY = -1.0f,
    bool isPriorityHighPower = false
);

void CameraMoveRequestAll(
    int frame,
    Float2 pos,
    float rot,
    Float2 scale,
    float zoom,
    std::function<float(float)> easing = nullptr,
    CAMERA_MOVE_TYPE moveType = CAMERA_MOVE_TYPE_SMOOTH
);
void CameraMoveRequestAll(
	int frame,
	Float3 pos,
	float rot,
	Float2 scale,
	float zoom,
	std::function<float(float)> easing = nullptr,
	CAMERA_MOVE_TYPE moveType = CAMERA_MOVE_TYPE_SMOOTH
);
void CameraMoveRequest(
    int index,
    int frame,
    Float3 pos,
    float rot,
    Float2 scale,
    float zoom,
    std::function<float(float)> easing = nullptr,
    CAMERA_MOVE_TYPE moveType = CAMERA_MOVE_TYPE_SMOOTH
);

void CameraMoveRequestAll(
	int frame,
	Float3 pos,
	std::function<float(float)> easing = nullptr,
	CAMERA_MOVE_TYPE moveType = CAMERA_MOVE_TYPE_SMOOTH
);
void CameraMoveRequest(
	int index,
	int frame,
	Float3 pos,
	std::function<float(float)> easing = nullptr,
	CAMERA_MOVE_TYPE moveType = CAMERA_MOVE_TYPE_SMOOTH
);
void CameraMoveCancelRequestAll();
void CameraMoveCancelRequest(int index);

void CameraTargetMoveRequestAll(
	int frame,
	Float3 pos,
	std::function<float(float)> easing = nullptr,
	CAMERA_MOVE_TYPE moveType = CAMERA_MOVE_TYPE_SMOOTH
);
void CameraTargetMoveRequest(
	int index,
	int frame,
	Float3 pos,
	std::function<float(float)> easing = nullptr,
	CAMERA_MOVE_TYPE moveType = CAMERA_MOVE_TYPE_SMOOTH
);
void CameraTargetMoveCancelRequestAll();
void CameraTargetMoveCancelRequest(int index);

void CameraTargetPositionRegister(int index, Float3* pos);

void CameraFlashRequestAll(
    int frame,
    std::function<float(float)> easing = nullptr,
    Float4 color = {10.0f, 10.0f, 10.0f, 1.0f},
    DPD_SHADER_OPT_BLEND_STATE colorBlendState = DPD_SHADER_OPT_BLEND_STATE_DEFAULT
);
void CameraFlashRequest(
    int index,
    int frame,
    std::function<float(float)> easing = nullptr,
    Float4 color = {10.0f, 10.0f, 10.0f, 1.0f},
    DPD_SHADER_OPT_BLEND_STATE colorBlendState= DPD_SHADER_OPT_BLEND_STATE_DEFAULT
);
void CameraZoomRequestAll(
    float speed,
    int objIndex,
    std::function<float(float)> easing = nullptr,
    float zoomScale = 2.0f
);
void CameraZoomRequest(
    int index,
    float speed,
    int objIndex,
    std::function<float(float)> easing = nullptr,
    float zoomScale = 2.0f
);
void CameraZoomRequestAll(
    float speed,
    Float3 pos,
    std::function<float(float)> easing = nullptr,
    float zoomScale = 2.0f
);
void CameraZoomRequest(
    int index,
    float speed,
    Float3 pos,
    std::function<float(float)> easing = nullptr,
    float zoomScale = 2.0f
);
void CameraZoomRequestAll(
    float speed,
    Float2 pos,
    std::function<float(float)> easing = nullptr,
    float zoomScale = 2.0f
);
void CameraZoomRequest(
    int index,
    float speed,
    Float2 pos,
    std::function<float(float)> easing = nullptr,
    float zoomScale = 2.0f
);
void CameraZoomResetRequestAll(
    float speed = 0,
    std::function<float(float)> easing = nullptr, int delay = 0
);
void CameraZoomResetRequest(
    int index,
    float speed = 0,
    std::function<float(float)> easing = nullptr, int delay = 0
);

void CameraRevRequestAll(
    float speed,
    std::function<float(float)> easing = nullptr
);
void CameraRevRequest(
    int index,
    float speed,
    std::function<float(float)> easing = nullptr
);
void CameraRevResetRequestAll(
    float speed = 0,
    std::function<float(float)> easing = nullptr
);
void CameraRevResetRequest(
    int index,
    float speed = 0,
    std::function<float(float)> easing = nullptr
);
bool IsCameraRev(int index);

CAMERA GetCameraInfo(int index);
void SetCameraInfo(int index, CAMERA info);

void SetCameraZoomAll(float n);
void SetCameraZoom(int index, float n);
void SetCameraRotAll(float n);
void SetCameraRot(int index, float n);
void SetCameraPosAll(Float3 pos);
void SetCameraPos(int index, Float3 pos);
void SetCameraTargetPosAll(Float3 pos);
void SetCameraTargetPos(int index, Float3 pos);

void SetCameraModeAll(CAMERA_MODE mode, int delay = 0);
void SetCameraMode(int index, CAMERA_MODE mode, int delay = 0);

#endif