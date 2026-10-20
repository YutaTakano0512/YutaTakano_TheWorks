// ===================================================
// polygon.h ポリゴンの制御
// 
// hara sougo  2024/12/02
// ===================================================

#ifndef _POLYGON_H_
#define _POLYGON_H_

#include "../Systems/sys_general.h"

#include "../../Core/_systemModeSwitch.h"

#include "../../Core/DirectX/dx.h"
#include "../../Core/DirectX/dx_camera.h"

#include "../../Core/DirectX/dx_model.h"

#include "../Texture/texture.h"

#ifdef SWITCH_MODE
#else
#include <d3d11.h>
#include <DirectXMath.h>
using namespace DirectX;
#endif

//static constexpr int NUM_VERTEX_MAX = (65535);	//必要な頂点の数
//static constexpr int NUM_VERTEX_MAX = (4096);	//必要な頂点の数
static constexpr int NUM_VERTEX_MAX = (16);	//必要な頂点の数
static constexpr int NUM_VERTEX_SQUARE = 4;	//四角の頂点の数
static constexpr int NUM_VERTEX_CUBE = 24;	//四角の頂点の数

static constexpr int POLYGON_DRAW_MAX = 1024;


// ===================================================
// 列挙体定義
// ===================================================

//描画優先度
enum DRAW_POLYGON_PRIORITY : char
{
	DRAW_POLYGON_PRIORITY_DEEPEST, //最背面に描画する

    DRAW_POLYGON_PRIORITY_NML_DEEPEST,
    DRAW_POLYGON_PRIORITY_NML_d3,
    DRAW_POLYGON_PRIORITY_NML_d2,
    DRAW_POLYGON_PRIORITY_NML_d1,
    DRAW_POLYGON_PRIORITY_NORMAL,
    DRAW_POLYGON_PRIORITY_NML_u1,
    DRAW_POLYGON_PRIORITY_NML_u2,
    DRAW_POLYGON_PRIORITY_NML_u3,
    DRAW_POLYGON_PRIORITY_NML_ALWAYS,

	DRAW_POLYGON_PRIORITY_ALWAYS, //最前面に描画する (ポストエフェクトより下)

	DRAW_POLYGON_PRIORITY_FORCE_DEEPEST, //最背面に描画する (ポストエフェクトより上)
	DRAW_POLYGON_PRIORITY_FORCE_NORMAL,
	DRAW_POLYGON_PRIORITY_FORCE_ALWAYS, //最前面に描画する (ポストエフェクトより上)

	DRAW_POLYGON_PRIORITY_MAX,
};

constexpr DRAW_POLYGON_PRIORITY DRAW_POLYGON_PRIORITY_START = DRAW_POLYGON_PRIORITY_DEEPEST;
constexpr DRAW_POLYGON_PRIORITY DRAW_POLYGON_PRIORITY_END = DRAW_POLYGON_PRIORITY_ALWAYS;

constexpr DRAW_POLYGON_PRIORITY DRAW_POLYGON_PRIORITY_FORCE_START = DRAW_POLYGON_PRIORITY_FORCE_DEEPEST;
constexpr DRAW_POLYGON_PRIORITY DRAW_POLYGON_PRIORITY_FORCE_END = DRAW_POLYGON_PRIORITY_FORCE_ALWAYS;

enum DRAW_POLYGON_TYPE : char
{
    DRAW_POLYGON_TYPE_QUAD,
    DRAW_POLYGON_TYPE_CUBE,
    DRAW_POLYGON_TYPE_FREE, //現状機能していません

    DRAW_POLYGON_TYPE_MAX,
};

enum DPD_SHADER_OPT_BLEND_STATE : char
{
    DPD_SHADER_OPT_BLEND_STATE_DEFAULT,
    DPD_SHADER_OPT_BLEND_STATE_ADDCOLOR,

    DPD_SHADER_OPT_BLEND_STATE_MAX,
};


// ===================================================
// 構造体宣言
// ===================================================
struct POLYGON_RELEASE_BUFFER;


struct POLYGON_SHADER_OPT {
	
};

struct DPD_SHADER_OPT {
	bool outline;
	bool bloom;
    DPD_SHADER_OPT_BLEND_STATE blendState;
};

struct DPD_VERTEX {
	DRAW_POLYGON_TYPE type;
    //std::vector<Vertex_DX> vertex; //頂点データ
	//Vertex_DX vertex[NUM_VERTEX_MAX]; //頂点データ
};
struct DRAW_POLYGON_DAT {		// 描画データ
	DPD_VERTEX vertex; //頂点データ
    unsigned int modelNo;
	//DX_MODEL* model; //モデルデータ

	Float3 pos;
	Float3 size;

	float rot;
	Float3 rot3D;

	Float4 color;

	unsigned int texNo;
	LOADTEXTURETYPE loadTexType;

	Float2 texUV;
	Float2 texWH;

	Float2 startPos;
	Float3 startPos3D;

	float startRot;
	Float3 startRot3D;

	Float2 startScale;
	Float3 startScale3D;

	DPD_SHADER_OPT shaderOpt;
};

//struct DPB_MATRIX_BUF {
//	XMMATRIX scale;
//	XMMATRIX rotX;
//	XMMATRIX rotY;
//	XMMATRIX rotZ;
//	XMMATRIX pos;
//};
struct DRAW_POLYGON_BUFFER {
	DPD_VERTEX vertex;
    unsigned int modelNo;
	//DX_MODEL* model;

	bool billboard;

    char cameraIndex;
	//Float3 posPivot;
	//Float3 posGameCamera;

    //DX_CAMERA_INFO DXCameraInfo;
	XMMATRIX pro;
	//std::vector<DPB_MATRIX_BUF> matrixBuf;
};

struct POLYGON_RELEASE_OPTION {
	Float4 color;
	unsigned int texNo;
	Float2 texUV;
	Float2 texWH;
	LOADTEXTURETYPE loadTexType;
	bool fixed;
	DRAW_POLYGON_PRIORITY priority;
    bool depthBuffIgnore;

	bool enableShade;

	//bool billboard;

	DPD_SHADER_OPT shaderOpt;
};


// ===================================================
// プロトタイプ宣言
// ===================================================

#ifdef IS_MAIN
//メイン以外からは呼び出せないようにしておく
//メインから呼び出すのを忘れずに
void InitPolygon();
void UninitPolygon();
//保存した書き込み情報を出力
void PolygonBufferRelease(void);

void PolygonTextureDrawAll(std::vector<unsigned int> idList);
#endif
//テクスチャをキャッシュする
//初めてのテクスチャ描画は重くなるため
//これをあらかじめ読んでおくことでかくつきを回避する
void PolygonTextureCache(unsigned int id);

//描画の処理を小分けにしたもの

//ポリゴン初期化(頂点設定モード)
void PolygonSetUp(DRAW_POLYGON_BUFFER* dsBuffer, DPD_VERTEX vertex);

//ポリゴン初期化(モデルモード)
//void PolygonSetUp(DRAW_POLYGON_BUFFER* dsBuffer, DX_MODEL* model);
void PolygonSetUp(DRAW_POLYGON_BUFFER* dsBuffer, unsigned int modelNo);

//ポリゴン変形
void PolygonShape(DRAW_POLYGON_BUFFER* dsBuffer,
	Float3 pos, Float3 rot, Float3 scale);

//ポリゴン描画前のマトリクス計算
XMMATRIX PolygonReleaseMat(DRAW_POLYGON_BUFFER* dsBuffer, const POLYGON_RELEASE_OPTION& opt);
//ポリゴン描画
void PolygonRelease(DRAW_POLYGON_BUFFER* dsBuffer, POLYGON_RELEASE_OPTION opt,
    bool isCalcMat = false
);


const Vertex_DX* Get_POLYGON_DEFAULT_SPRITE_VERTEX();

//ポストエフェクトの設定
void SetPostEffectShader(std::string name);
void SetPostEffectShader(int index);
int GetPostEffectShader();


#endif //_POLYGON_H_