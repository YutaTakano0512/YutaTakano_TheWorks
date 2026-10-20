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
#include "../../Core/DirectX/dx_shader.h"
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
static constexpr int NUM_VERTEX_UPDATEV = 128;	//四角の頂点の数

static constexpr int POLYGON_DRAW_MAX = 1024;

static constexpr int GLOBAL_OUTLINE_WIDTH_DEFAULT = 3.0f;

// ===================================================
// 列挙体定義
// ===================================================

//描画優先度
enum DRAW_POLYGON_PRIORITY : char
{
	DRAW_POLYGON_PRIORITY_DEEPEST = 0, //最背面に描画する

    DRAW_POLYGON_PRIORITY_NML_DEEPEST,
    DRAW_POLYGON_PRIORITY_NML_d5,
    DRAW_POLYGON_PRIORITY_NML_d4,
    DRAW_POLYGON_PRIORITY_NML_d3,
    DRAW_POLYGON_PRIORITY_NML_d2,
    DRAW_POLYGON_PRIORITY_NML_d1,
    DRAW_POLYGON_PRIORITY_NORMAL,
    DRAW_POLYGON_PRIORITY_NML_u1,
    DRAW_POLYGON_PRIORITY_NML_u2,
    DRAW_POLYGON_PRIORITY_NML_u3,
    DRAW_POLYGON_PRIORITY_NML_u4,
    DRAW_POLYGON_PRIORITY_NML_ALWAYS,

	DRAW_POLYGON_PRIORITY_ALWAYS, //最前面に描画する (ポストエフェクトより下)

	DRAW_POLYGON_PRIORITY_FORCE_DEEPEST, //最背面に描画する (ポストエフェクトより上)
    DRAW_POLYGON_PRIORITY_FORCE_NML_d1,
	DRAW_POLYGON_PRIORITY_FORCE_NORMAL,
    DRAW_POLYGON_PRIORITY_FORCE_NML_u1,
	DRAW_POLYGON_PRIORITY_FORCE_ALWAYS, //最前面に描画する (ポストエフェクトより上)

	DRAW_POLYGON_PRIORITY_MAX,
};

constexpr DRAW_POLYGON_PRIORITY DRAW_POLYGON_PRIORITY_START = DRAW_POLYGON_PRIORITY_DEEPEST;
constexpr DRAW_POLYGON_PRIORITY DRAW_POLYGON_PRIORITY_END = DRAW_POLYGON_PRIORITY_ALWAYS;

constexpr DRAW_POLYGON_PRIORITY DRAW_POLYGON_PRIORITY_FORCE_START = DRAW_POLYGON_PRIORITY_FORCE_DEEPEST;
constexpr DRAW_POLYGON_PRIORITY DRAW_POLYGON_PRIORITY_FORCE_END = DRAW_POLYGON_PRIORITY_FORCE_ALWAYS;

enum DRAW_POLYGON_TYPE : char
{
    DRAW_POLYGON_TYPE_QUAD = 0,
    DRAW_POLYGON_TYPE_CUBE,
    DRAW_POLYGON_TYPE_QUAD_INCETANCE,

    DRAW_POLYGON_TYPE_QUAD_OBJECT_SHADOW_0,

    DRAW_POLYGON_TYPE_FREE, //現状機能していません
    DRAW_POLYGON_TYPE_UPDATE_VERTEX,

    DRAW_POLYGON_TYPE_MAX,
};

enum DPD_SHADER_OPT_BLEND_STATE : char
{
    DPD_SHADER_OPT_BLEND_STATE_DEFAULT = 0,
    DPD_SHADER_OPT_BLEND_STATE_ADDCOLOR,

    DPD_SHADER_OPT_BLEND_STATE_MAX,
};

enum DPD_SHADER_OPT_BLOOM : char
{
    DPD_SHADER_OPT_BLOOM_NONE = 0,
    DPD_SHADER_OPT_BLOOM_NORMAL,

    //DPD_SHADER_OPT_BLOOM_RED,
    //DPD_SHADER_OPT_BLOOM_BLUE,
    //DPD_SHADER_OPT_BLOOM_YELLOW,
    //DPD_SHADER_OPT_BLOOM_GREEN,

    //DPD_SHADER_OPT_BLOOM_PURPLE,
    //DPD_SHADER_OPT_BLOOM_DARK_PURPLE,

    DPD_SHADER_OPT_BLOOM_ALWAYS,
    DPD_SHADER_OPT_BLOOM_MAX,
};
enum DPD_SHADER_OPT_OUTLINE : char
{
    DPD_SHADER_OPT_OUTLINE_NONE = 0,
    DPD_SHADER_OPT_OUTLINE_NORMAL,

    DPD_SHADER_OPT_OUTLINE_WHITE,
    DPD_SHADER_OPT_OUTLINE_RED,
    DPD_SHADER_OPT_OUTLINE_BLUE,
    DPD_SHADER_OPT_OUTLINE_YELLOW,
    DPD_SHADER_OPT_OUTLINE_GREEN,

    DPD_SHADER_OPT_OUTLINE_PURPLE,
    DPD_SHADER_OPT_OUTLINE_DARK_PURPLE,

    DPD_SHADER_OPT_OUTLINE_MAX,
};


//typedef DX_SHADER_TYPE DPD_SHADER_TYPE;

enum DPD_SHADER_TYPE : char {
    DPD_SHADER_TYPE_NONE = 0,
    DPD_SHADER_TYPE_NO_UV_LOOP,

    DPD_SHADER_TYPE_METAL,
    DPD_SHADER_TYPE_METAL_ENEMY,
    DPD_SHADER_TYPE_METAL_ENEMY_2,
    DPD_SHADER_TYPE_METAL_ENEMY_DARK,

    DPD_SHADER_TYPE_CUTIN,
    DPD_SHADER_TYPE_CUTIN_METAL_ENEMY,
    DPD_SHADER_TYPE_CUTIN_METAL_ENEMY_2,
    DPD_SHADER_TYPE_BLUR,

    DPD_SHADER_TYPE_EFFECT_DIS,
    DPD_SHADER_TYPE_EFFECT_WARP,
    DPD_SHADER_TYPE_EFFECT_FRESNEL,

    DPD_SHADER_TYPE_ALPHA_DISSOLVE,

    DPD_SHADER_TYPE_EFFECT,

    DPD_SHADER_TYPE_UI_BG_GENERAL,

    DPD_SHADER_TYPE_EX,

    DPD_SHADER_TYPE_MAX
};

// ===================================================
// 構造体宣言
// ===================================================
struct POLYGON_RELEASE_BUFFER;


struct POLYGON_SHADER_OPT {
	
};

struct DPD_SHADER_TYPE_EX_OPT {
    int vs;
    int ps;
    Float4 free;
};

struct DPD_SHADER_OPT {
    char outlineLayer;
    DPD_SHADER_OPT_OUTLINE outline;

    DPD_SHADER_OPT_BLOOM bloom;
    DPD_SHADER_OPT_BLEND_STATE blendState;
};

struct DPD_VERTEX_FRAGMENT { // 頂点構造体
    Float3 pos;	// 座標
    Float4 color;	// 頂点カラー
    Float2 texcoord; // テクスチャ座標(UV)
    Float3 normal;	// 法線
};

struct DPD_VERTEX {
	DRAW_POLYGON_TYPE type;
 //   //std::vector<Vertex_DX> vertex; //頂点データ
	////Vertex_DX vertex[NUM_VERTEX_MAX]; //頂点データ
 //   //Vertex_DX vertex[NUM_VERTEX_UPDATEV]; //頂点データ
 //   Float3 vertex[NUM_VERTEX_UPDATEV]; //頂点データ
 //   short vertexNum;
 
    //std::vector<DPD_VERTEX_FRAGMENT> vertex; //頂点データ
    //DPD_VERTEX() { 
    //    type = DRAW_POLYGON_TYPE_QUAD;
    //    vertex = {}; 
    //}
    unsigned int instanceNum;
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
    XMMATRIX proObj;
	//std::vector<DPB_MATRIX_BUF> matrixBuf;
};

struct POLYGON_RELEASE_OPTION {
	Float4 color;
	unsigned int texNo;
	Float2 texUV;
	Float2 texWH;
	LOADTEXTURETYPE loadTexType;
	bool fixed;
    bool cameraFix;
	DRAW_POLYGON_PRIORITY priority;
    bool depthBuffIgnore;

	//bool enableShade;
    DPD_SHADER_TYPE shaderType;
    DPD_SHADER_TYPE_EX_OPT shaderTypeEx;

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
void PolygonBufferReleaseOnly(void);
void PolygonMainBufRelease(void);

void PolygonTextureDrawAll(std::vector<unsigned int> idList);
//void PolygonDrawLoadProgress(size_t cur, size_t max);
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
    Float3 pos, Float3 rot, Float3 scale,
    bool isObj = false
);

//ポリゴン描画前のマトリクス計算
XMMATRIX PolygonReleaseMat(DRAW_POLYGON_BUFFER* dsBuffer, const POLYGON_RELEASE_OPTION& opt);
//ポリゴン描画
void PolygonRelease(DRAW_POLYGON_BUFFER* dsBuffer, POLYGON_RELEASE_OPTION opt,
    bool isCalcMat = false
);

//シェーダーリソースビューの描画
//void PolygonSRVDraw(ID3D11ShaderResourceView* srv,
//    Float3 posIn,
//    Float3 rotIn,
//    Float3 scaleIn,
//    int shaderIndex
//);
void PolygonSRVDraw(ID3D11ShaderResourceView* srv,
    Float3 posIn,
    Float3 rotIn,
    Float3 scaleIn,
    int shaderIndexVS,
    int shaderIndexPS
);

struct POLYGON_SRV_BUF {
    ID3D11ShaderResourceView* srv;
    Float3 posIn;
    Float3 rotIn;
    Float3 scaleIn;
    //int shaderIndex;
    int shaderIndexVS;
    int shaderIndexPS;
    DRAW_POLYGON_PRIORITY priority;
};
//シェーダーリソースビューの描画
void PolygonSRVDraw(
    POLYGON_SRV_BUF buf
);


//const Vertex_DX* Get_POLYGON_DEFAULT_SPRITE_VERTEX();

//ポストエフェクトの設定
//void SetPostEffectShader(std::string name);
//void SetPostEffectShader(int index);
//int GetPostEffectShader();
void SetPostEffectShaderPS(std::string name);
void SetPostEffectShaderPS(int index);
int  GetPostEffectShaderPS();
void SetPostEffectShaderVS(std::string name);
void SetPostEffectShaderVS(int index);
int  GetPostEffectShaderVS();

void SetPostEffectShaderVariable(float value, int index);
void SetPostEffectShaderVariable(Float4 value);
Float4  GetPostEffectShaderVariable();

float GetPostEffectGlobalOutlineWidth();
void SetPostEffectGlobalOutlineWidth(float width);
void ResetPostEffectGlobalOutlineWidth();

#endif //_POLYGON_H_