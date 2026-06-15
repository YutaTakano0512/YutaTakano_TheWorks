// ===================================================
// polygon.h ポリゴンの制御
// 
// hara sougo  2024/12/02
// ===================================================

#include "../../game_main.h"
#include "polygon.h"
#include "../Systems/collision.h"

#include "../../Core/_systemModeSwitch.h"


#include "../../Core/DirectX/dx_camera.h"
#include <d3d11.h>
#include <DirectXMath.h>
#include "../../../DirectXTex.h"
using namespace DirectX;
#include "../../Core/DirectX/dx_shader.h"

#include "../Texture/texData.h"
#include "modelData.h"



struct POLYGON_RELEASE_BUFFER {
	bool use;

	DRAW_POLYGON_PRIORITY drawPriority;
    bool depthBuffIgnore;
	bool fixed;

	DPD_VERTEX vertex;
    unsigned int modelNo;
	//DX_MODEL* model;

    //XMFLOAT4X4 matrix;
    XMMATRIX matrix;
	SHADER_VERTEX_SET_MATRIX_EX_DAT svsmdat;
	ID3D11ShaderResourceView* texture;

	float distance;


	DPD_SHADER_OPT shaderOpt;
};

//static void SetVertex(std::vector<Vertex_DX> points);


static ID3D11Buffer* g_VertexBuffer = NULL;			// 線用の頂点情報
static ID3D11Buffer* g_VertexBuffer_Quad = NULL;			// 線用の頂点情報
static ID3D11Buffer* g_VertexBuffer_Cube = NULL;			// 線用の頂点情報

static ID3D11Buffer* g_IndexBuffer_Cube = NULL;			//
//static ID3D11Buffer* g_IndexBuffer = NULL;			//

// 注意！初期化で外部から設定されるもの。Release不要。
static ID3D11Device* g_Device = NULL;
static ID3D11DeviceContext* g_DeviceContext = NULL;


static int g_ShaderIndex_Main = 0;
static int g_ShaderIndex_Bloom = 0;
static int g_ShaderIndex_OutlineBuf2 = 0;
static int g_ShaderIndex_OutlineBuf = 0;
static int g_ShaderIndex_Outline = 0;
static int g_ShaderIndex_Custom_0 = -1;


static std::vector<POLYGON_RELEASE_BUFFER> g_PolygonRelease_buffers = {};

//static constexpr int POLYGON_RELEASE_BUFFER_MAX = (65535);
//static POLYGON_RELEASE_BUFFER g_PolygonRelease_buffers_array[POLYGON_RELEASE_BUFFER_MAX] = {};

static Vertex_DX vertex_cube[NUM_VERTEX_CUBE] = {};

//板ポリゴンの頂点
static constexpr Vertex_DX POLYGON_DEFAULT_SPRITE_VERTEX[NUM_VERTEX_SQUARE] = {
	{ {-0.5f, -0.5f, 0.0f}, {1.0f, 1.0f, 1.0f, 1.0f}, {0.0f, 0.0f}, {-1.0f, -1.0f, 0.5f} },
	{ {+0.5f, -0.5f, 0.0f}, {1.0f, 1.0f, 1.0f, 1.0f}, {1.0f, 0.0f}, {+1.0f, -1.0f, 0.5f} },
	{ {-0.5f, +0.5f, 0.0f}, {1.0f, 1.0f, 1.0f, 1.0f}, {0.0f, 1.0f}, {-1.0f, +1.0f, 0.5f} },
	{ {+0.5f, +0.5f, 0.0f}, {1.0f, 1.0f, 1.0f, 1.0f}, {1.0f, 1.0f}, {+1.0f, +1.0f, 0.5f} },
};
const Vertex_DX* Get_POLYGON_DEFAULT_SPRITE_VERTEX()
{
	return POLYGON_DEFAULT_SPRITE_VERTEX;
}


//
// 頂点を設定
//

//void SetVertex(std::vector<Vertex_DX> points)
void SetVertexBuffer(std::vector<Vertex_DX> points);
void SetVertex(void);
void SetVertex_Quad(void);
void SetVertex_Cube(void);



void InitPolygon()
{
	g_PolygonRelease_buffers.reserve(1024);

	// デバイスとデバイスコンテキストの保存
	g_Device = DXGetDevice();
	g_DeviceContext = DXGetDeviceContext();


	g_ShaderIndex_Main			= DX_GetShaderIndex("SHADER/shader");
	g_ShaderIndex_Bloom			= DX_GetShaderIndex("SHADER/shader_bloom");
	g_ShaderIndex_OutlineBuf2	= DX_GetShaderIndex("SHADER/shader_outlineBuf2");
	g_ShaderIndex_OutlineBuf	= DX_GetShaderIndex("SHADER/shader_outlineBuf");
	g_ShaderIndex_Outline		= DX_GetShaderIndex("SHADER/shader_outline");
	g_ShaderIndex_Custom_0		= -1;

	{
		//↓だいじ
		// 頂点バッファ生成
		D3D11_BUFFER_DESC bd = {};
		bd.Usage = D3D11_USAGE_DYNAMIC;
		bd.ByteWidth = sizeof(Vertex_DX) * NUM_VERTEX_MAX;
		//bd.ByteWidth = sizeof(Vertex_DX);
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		g_Device->CreateBuffer(&bd, NULL, &g_VertexBuffer);
	}
	{
		std::vector<Vertex_DX> vert = {};
		for (int i = 0; i < ARRAYSIZE(POLYGON_DEFAULT_SPRITE_VERTEX); i++)
		{
			vert.push_back(POLYGON_DEFAULT_SPRITE_VERTEX[i]);
		}
		SetVertexBuffer(vert);
		SetVertex();
	}

	{
		{
			// 頂点バッファ生成
			D3D11_BUFFER_DESC bd = {};
			bd.Usage = D3D11_USAGE_DEFAULT;
			bd.ByteWidth = sizeof(Vertex_DX) * NUM_VERTEX_SQUARE;
			bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
			bd.CPUAccessFlags = 0;
			//bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

			D3D11_SUBRESOURCE_DATA sd = {};
			sd.pSysMem = POLYGON_DEFAULT_SPRITE_VERTEX;

			g_Device->CreateBuffer(&bd, &sd, &g_VertexBuffer_Quad);
		}
	}
	{
		// 上面
		vertex_cube[0].pos = XMFLOAT3(-0.5f, 0.5f, 0.5f);
		vertex_cube[0].color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		vertex_cube[0].texcoord = XMFLOAT2(0.0f, 0.0f);
		vertex_cube[0].normal = XMFLOAT3(0.0f, 0.5f, 0.0f);

		vertex_cube[1].pos = XMFLOAT3(0.5f, 0.5f, 0.5f);
		vertex_cube[1].color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		vertex_cube[1].texcoord = XMFLOAT2(0.5f, 0.0f);
		vertex_cube[1].normal = XMFLOAT3(0.0f, 0.5f, 0.0f);

		vertex_cube[2].pos = XMFLOAT3(-0.5f, 0.5f, -0.5f);
		vertex_cube[2].color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		vertex_cube[2].texcoord = XMFLOAT2(0.0f, 0.5f / 4.0f);
		vertex_cube[2].normal = XMFLOAT3(0.0f, 0.5f, 0.0f);

		vertex_cube[3].pos = XMFLOAT3(0.5f, 0.5f, -0.5f);
		vertex_cube[3].color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		vertex_cube[3].texcoord = XMFLOAT2(0.5f, 0.5f / 4.0f);
		vertex_cube[3].normal = XMFLOAT3(0.0f, 0.5f, 0.0f);

		vertex_cube[4].pos = XMFLOAT3(-0.5f, -0.5f, 0.5f);
		vertex_cube[4].color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		vertex_cube[4].texcoord = XMFLOAT2(0.0f, 1.0);
		vertex_cube[4].normal = XMFLOAT3(0.0f, -0.5f, 0.0f);

		vertex_cube[5].pos = XMFLOAT3(0.5f, -0.5f, 0.5f);
		vertex_cube[5].color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		vertex_cube[5].texcoord = XMFLOAT2(0.5f, 1.0);
		vertex_cube[5].normal = XMFLOAT3(0.0f, -0.5f, 0.0f);

		vertex_cube[6].pos = XMFLOAT3(-0.5f, -0.5f, -0.5f);
		vertex_cube[6].color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		vertex_cube[6].texcoord = XMFLOAT2(0.0f, 0.0f);
		vertex_cube[6].normal = XMFLOAT3(0.0f, -0.5f, 0.0f);

		vertex_cube[7].pos = XMFLOAT3(0.5f, -0.5f, -0.5f);
		vertex_cube[7].color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		vertex_cube[7].texcoord = XMFLOAT2(0.5f, 0.0f);
		vertex_cube[7].normal = XMFLOAT3(0.0f, -0.5f, 0.0f);


		// 手前面
		vertex_cube[8].pos = XMFLOAT3(-0.5f, 0.5f, -0.5f);
		vertex_cube[8].color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		vertex_cube[8].texcoord = XMFLOAT2(0.0f, 0.0f);
		vertex_cube[8].normal = XMFLOAT3(0.0f, 0.0f, -0.5f);

		vertex_cube[9].pos = XMFLOAT3(0.5f, 0.5f, -0.5f);
		vertex_cube[9].color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		vertex_cube[9].texcoord = XMFLOAT2(0.5f, 0.0f);
		vertex_cube[9].normal = XMFLOAT3(0.0f, 0.0f, -0.5f);

		vertex_cube[10].pos = XMFLOAT3(-0.5f, -0.5f, -0.5f);
		vertex_cube[10].color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		vertex_cube[10].texcoord = XMFLOAT2(0.0f, 0.5f);
		vertex_cube[10].normal = XMFLOAT3(0.0f, 0.0f, -0.5f);

		vertex_cube[11].pos = XMFLOAT3(0.5f, -0.5f, -0.5f);
		vertex_cube[11].color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		vertex_cube[11].texcoord = XMFLOAT2(0.5f, 0.5f);
		vertex_cube[11].normal = XMFLOAT3(0.0f, 0.0f, -0.5f);


		// 奥前面
		vertex_cube[12].pos = XMFLOAT3(-0.5f, 0.5f, 0.5f);
		vertex_cube[12].color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		vertex_cube[12].texcoord = XMFLOAT2(0.0f, 0.0f);
		vertex_cube[12].normal = XMFLOAT3(0.0f, 0.0f, 0.5f);

		vertex_cube[13].pos = XMFLOAT3(0.5f, 0.5f, 0.5f);
		vertex_cube[13].color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		vertex_cube[13].texcoord = XMFLOAT2(0.5f, 0.0f);
		vertex_cube[13].normal = XMFLOAT3(0.0f, 0.0f, 0.5f);

		vertex_cube[14].pos = XMFLOAT3(-0.5f, -0.5f, 0.5f);
		vertex_cube[14].color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		vertex_cube[14].texcoord = XMFLOAT2(0.0f, 0.5f);
		vertex_cube[14].normal = XMFLOAT3(0.0f, 0.0f, 0.5f);

		vertex_cube[15].pos = XMFLOAT3(0.5f, -0.5f, 0.5f);
		vertex_cube[15].color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		vertex_cube[15].texcoord = XMFLOAT2(0.5f, 0.5f);
		vertex_cube[15].normal = XMFLOAT3(0.0f, 0.0f, 0.5f);


		// 右面
		vertex_cube[16].pos = XMFLOAT3(0.5f, 0.5f, -0.5f);
		vertex_cube[16].color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		vertex_cube[16].texcoord = XMFLOAT2(0.0f, 0.0f);
		vertex_cube[16].normal = XMFLOAT3(0.5f, 0.0f, 0.0f);

		vertex_cube[17].pos = XMFLOAT3(0.5f, 0.5f, 0.5f);
		vertex_cube[17].color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		vertex_cube[17].texcoord = XMFLOAT2(0.5f, 0.0f);
		vertex_cube[17].normal = XMFLOAT3(0.5f, 0.0f, 0.0f);

		vertex_cube[18].pos = XMFLOAT3(0.5f, -0.5f, -0.5f);
		vertex_cube[18].color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		vertex_cube[18].texcoord = XMFLOAT2(0.0f, 0.5f);
		vertex_cube[18].normal = XMFLOAT3(0.5f, 0.0f, 0.0f);

		vertex_cube[19].pos = XMFLOAT3(0.5f, -0.5f, 0.5f);
		vertex_cube[19].color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		vertex_cube[19].texcoord = XMFLOAT2(0.5f, 0.5f);
		vertex_cube[19].normal = XMFLOAT3(0.5f, 0.0f, 0.0f);


		//左面
		vertex_cube[20].pos = XMFLOAT3(-0.5f, 0.5f, 0.5f);
		vertex_cube[20].color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		vertex_cube[20].texcoord = XMFLOAT2(0.0f, 0.0f);
		vertex_cube[20].normal = XMFLOAT3(-0.5f, 0.0f, 0.0f);

		vertex_cube[21].pos = XMFLOAT3(-0.5f, 0.5f, -0.5f);
		vertex_cube[21].color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		vertex_cube[21].texcoord = XMFLOAT2(0.5f, 0.0f);
		vertex_cube[21].normal = XMFLOAT3(-0.5f, 0.0f, 0.0f);

		vertex_cube[22].pos = XMFLOAT3(-0.5f, -0.5f, 0.5f);
		vertex_cube[22].color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		vertex_cube[22].texcoord = XMFLOAT2(0.0f, 0.5f);
		vertex_cube[22].normal = XMFLOAT3(-0.5f, 0.0f, 0.0f);

		vertex_cube[23].pos = XMFLOAT3(-0.5f, -0.5f, -0.5f);
		vertex_cube[23].color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		vertex_cube[23].texcoord = XMFLOAT2(0.5f, 0.5f);
		vertex_cube[23].normal = XMFLOAT3(-0.5f, 0.0f, 0.0f);

		//↓だいじ
		{
			// 頂点バッファ生成
			D3D11_BUFFER_DESC bd = {};
			bd.Usage = D3D11_USAGE_DEFAULT;
			bd.ByteWidth = sizeof(Vertex_DX) * NUM_VERTEX_MAX;
			bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
			bd.CPUAccessFlags = 0;
			//bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

			D3D11_SUBRESOURCE_DATA sd = {};
			sd.pSysMem = vertex_cube;

			g_Device->CreateBuffer(&bd, &sd, &g_VertexBuffer_Cube);
		}

		{
			//インデックス設定

			constexpr int indexNum = 36;
			unsigned int index[indexNum] = {
				0,1,2,    1,3,2,
				8,9,10,   9,11,10,
				16,17,18, 17,19,18,
				13,12,14, 13,14,15,
				20,21,22, 21,23,22,
				6,7,4,    7,5,4
			};

			// インデックスバッファ生成
			D3D11_BUFFER_DESC bd = {};
			bd.Usage = D3D11_USAGE_DEFAULT;
			bd.ByteWidth = sizeof(index[0]) * indexNum;
			bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
			bd.CPUAccessFlags = 0;

			D3D11_SUBRESOURCE_DATA sd = {};
			sd.pSysMem = index;

			g_Device->CreateBuffer(&bd, &sd, &g_IndexBuffer_Cube);
		}
	}

	// 頂点バッファへ頂点データの書き込み
	//SetVertex();

	//{
	//	//インデックス設定

	//	constexpr int indexNum = NUM_VERTEX_SQUARE_INDEXED_MAX;

	//	unsigned int index[indexNum] = {
	//		0,1,2,    1,3,2,
	//	};

	//	// インデックスバッファ生成
	//	D3D11_BUFFER_DESC bd = {};
	//	bd.Usage = D3D11_USAGE_DEFAULT;
	//	bd.ByteWidth = sizeof(index[0]) * indexNum;
	//	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	//	bd.CPUAccessFlags = 0;

	//	D3D11_SUBRESOURCE_DATA sd = {};
	//	sd.pSysMem = index;

	//	g_Device->CreateBuffer(&bd, &sd, &g_IndexBuffer);
	//}

	g_PolygonRelease_buffers = {};
	//for (int i = 0; i < POLYGON_RELEASE_BUFFER_MAX; i++)
	//{
	//	g_PolygonRelease_buffers_array[i] = {};
	//}
}


void UninitPolygon()
{
	SAFE_RELEASE(g_VertexBuffer);
	SAFE_RELEASE(g_VertexBuffer_Quad);

	
	SAFE_RELEASE(g_IndexBuffer_Cube);
	SAFE_RELEASE(g_VertexBuffer_Cube);
}


//セットアップ
void polygonSetUp_private(DRAW_POLYGON_BUFFER* dsBuffer)
{
	(*dsBuffer) = {};
	dsBuffer->pro = XMMatrixTranslation(0.0f, 0.0f, 0.0f); //一度原点の位置へもっていく

	//dsBuffer->matrixBuf = {};
	//dsBuffer->matrixBuf.reserve(16);

}

void PolygonSetUp(DRAW_POLYGON_BUFFER* dsBuffer, DPD_VERTEX vertex)
{
	polygonSetUp_private(dsBuffer);
	dsBuffer->vertex = vertex;
}
//void PolygonSetUp(DRAW_POLYGON_BUFFER* dsBuffer, DX_MODEL* model)
//{
//	polygonSetUp_private(dsBuffer);
//	dsBuffer->model = model;
//}
void PolygonSetUp(DRAW_POLYGON_BUFFER* dsBuffer, unsigned int modelNo)
{
	polygonSetUp_private(dsBuffer);
	dsBuffer->modelNo = modelNo;
}

//変形
void PolygonShape(DRAW_POLYGON_BUFFER* dsBuffer,
	Float3 pos, Float3 rot, Float3 scale)
{
	//深度設定を保持
	bool deSave = DXDepthStencilStateDepthEnable();

	//DPB_MATRIX_BUF buf = {};

	//buf.scale = XMMatrixScaling(scale.x, scale.y, scale.z); //指定のサイズにする
	dsBuffer->pro *= XMMatrixScaling(scale.x, scale.y, scale.z); //指定のサイズにする

	//dsBuffer->position.x *= scale.x;
	//dsBuffer->position.y *= scale.y;

	if (deSave)
	{
		if (dsBuffer->billboard)
		{
			DX_CAMERA_INFO cameraInfo = DX_GetCameraInfo(dsBuffer->cameraIndex);
			XMMATRIX invView = {};

			XMFLOAT3 pos = cameraInfo.CameraPosRaw;
			pos.x *= -cameraInfo.CameraScalingRaw.x;
			pos.y *= cameraInfo.CameraScalingRaw.y;
			pos.z *= cameraInfo.CameraScalingRaw.z;
			XMFLOAT3 target = cameraInfo.CameraTargetRaw;
			XMFLOAT3 up = cameraInfo.CameraUpRaw;
			up.y *= -1.0f;

			XMMATRIX CameraViewTmp = XMMatrixLookAtLH(XMLoadFloat3(&pos), XMLoadFloat3(&target), XMLoadFloat3(&up));

			invView = XMMatrixInverse(nullptr, CameraViewTmp);
			invView.r[3].m128_f32[0] = 0.0f;
			invView.r[3].m128_f32[1] = 0.0f;
			invView.r[3].m128_f32[2] = 0.0f;

			dsBuffer->pro *= invView;

			dsBuffer->billboard = false; //フラグを折る
		}
		//else
		{
			//buf.rotX = XMMatrixRotationX(XMConvertToRadians(rot.x));
			dsBuffer->pro *= XMMatrixRotationX(XMConvertToRadians(rot.x));
			//{
			//	float rotRad = CalculateDegToRad(rot.x);
			//	float si = sinf(rotRad),
			//		co = cosf(rotRad);

			//	float tmpX, tmpY;
			//	//x = (xcosθ - ysinθ) + posX
			//	//y = (xsinθ + ycosθ) + posY
			//	tmpX = (dsBuffer->position.y * co - dsBuffer->position.z * si);
			//	tmpY = (dsBuffer->position.y * si + dsBuffer->position.z * co);

			//	dsBuffer->position.y += tmpX;
			//	dsBuffer->position.z += tmpY;
			//}
			//buf.rotY = XMMatrixRotationY(XMConvertToRadians(rot.y));
			dsBuffer->pro *= XMMatrixRotationY(XMConvertToRadians(rot.y));

			//{
			//	float rotRad = CalculateDegToRad(rot.y);
			//	float si = sinf(rotRad),
			//		co = cosf(rotRad);

			//	float tmpX, tmpY;
			//	//x = (xcosθ - ysinθ) + posX
			//	//y = (xsinθ + ycosθ) + posY
			//	tmpX = (dsBuffer->position.x * co - dsBuffer->position.z * si);
			//	tmpY = (dsBuffer->position.x * si + dsBuffer->position.z * co);

			//	dsBuffer->position.x += tmpX;
			//	dsBuffer->position.z += tmpY;
			//}
		}
	}
	//buf.rotZ = XMMatrixRotationZ(XMConvertToRadians(rot.z));
	dsBuffer->pro *= XMMatrixRotationZ(XMConvertToRadians(rot.z));
	
	//{
// 
	//	float rotRad = CalculateDegToRad(rot.z);
	//	float si = sinf(rotRad),
	//		co = cosf(rotRad);

	//	float tmpX, tmpY;
	//	//x = (xcosθ - ysinθ) + posX
	//	//y = (xsinθ + ycosθ) + posY
	//	tmpX = (dsBuffer->position.x * co - dsBuffer->position.y * si);
	//	tmpY = (dsBuffer->position.x * si + dsBuffer->position.y * co);

	//	dsBuffer->position.x += tmpX;
	//	dsBuffer->position.y += tmpY;
	//}

	

	//dsBuffer->position.x += pos.x;
	//dsBuffer->position.y += pos.y;

	if (deSave)
	{ //3D
		//buf.pos = XMMatrixTranslation(pos.x, pos.y, pos.z); //本来の位置
		dsBuffer->pro *= XMMatrixTranslation(pos.x, pos.y, pos.z); //本来の位置
		//dsBuffer->position.z += pos.z;

	}
	else
	{ //2D
		//buf.pos = XMMatrixTranslation(pos.x, pos.y, 0.0f); //本来の位置
		dsBuffer->pro *= XMMatrixTranslation(pos.x, pos.y, 0.0f); //本来の位置
		
		//worldViewProjection *= XMMatrixTranslation(
		//	(float)(SCREEN_WIDTH_BASE - SCREEN_WIDTH) / 2,
		//	(float)(SCREEN_HEIGHT_BASE - SCREEN_HEIGHT) / 2,
		//	0.0f);
	}

	//dsBuffer->matrixBuf.push_back(buf);
}

//解放(描画)
void polygonBufferRelease_parts(const POLYGON_RELEASE_BUFFER& v, bool deSave, int priority);

XMMATRIX PolygonReleaseMat(DRAW_POLYGON_BUFFER* dsBuffer, const POLYGON_RELEASE_OPTION& opt)
{
    //深度設定を保持
    bool deSave = DXDepthStencilStateDepthEnable();

    DX_CAMERA_INFO cameraInfo = DX_GetCameraInfo(dsBuffer->cameraIndex);

    XMMATRIX worldViewProjection = dsBuffer->pro; //計算順が大事

    //深度ステート有効だったらカメラの設定を反映する
    if (!opt.fixed && deSave)
    { //3D用

        worldViewProjection *= cameraInfo.CameraScaling;

        worldViewProjection *= cameraInfo.CameraView;

        //if (opt.billboard)
        //{ //ビルボード処理
        //	XMMATRIX Inv = XMMatrixIdentity();

        //	XMFLOAT3 pos = cameraInfo.CameraPosRaw;
        //	XMFLOAT3 target = cameraInfo.CameraTargetRaw;
        //	XMFLOAT3 up = cameraInfo.CameraUpRaw;

        //	XMMATRIX CameraViewTmp = XMMatrixLookAtLH(XMLoadFloat3(&pos), XMLoadFloat3(&target), XMLoadFloat3(&up));
        //	Inv *= XMMatrixInverse(nullptr, CameraViewTmp);
        //	Inv.r[3].m128_f32[0] = 0.0f;
        //	Inv.r[3].m128_f32[1] = 0.0f;
        //	Inv.r[3].m128_f32[2] = 0.0f;
        //	worldViewProjection *= Inv;
        //	/*XMFLOAT3 pos = cameraInfo.CameraPosRaw;
        //	pos.x += dsBuffer->posGameCamera.x * -1;
        //	pos.y += dsBuffer->posGameCamera.y;
        //	pos.z += dsBuffer->posGameCamera.z;
        //	XMFLOAT3 target = cameraInfo.CameraTargetRaw;
        //	target.x += dsBuffer->posGameCamera.x;
        //	target.y += dsBuffer->posGameCamera.y;
        //	target.z += dsBuffer->posGameCamera.z;
        //	XMFLOAT3 up = cameraInfo.CameraUpRaw;

        //	XMMATRIX CameraViewTmp = XMMatrixLookAtLH(XMLoadFloat3(&pos), XMLoadFloat3(&target), XMLoadFloat3(&up));*/

        //	//XMMATRIX invView = {};
        //	////invView = XMMatrixInverse(nullptr, CameraViewTmp);
        //	//invView = XMMatrixInverse(nullptr, cameraInfo.CameraView);

        //	////float rx = atan2(cameraInfo.CameraVector.y, cameraInfo.CameraVector.z);
        //	////float ry = atan2(cameraInfo.CameraVector.y, cameraInfo.CameraVector.x);
        //	////invView *= XMMatrixRotationX(rx);
        //	////invView *= XMMatrixRotationY(ry);

        //	//invView.r[3].m128_f32[0] = 1.0f;
        //	//invView.r[3].m128_f32[1] = 1.0f;
        //	//invView.r[3].m128_f32[2] = 1.0f;

        //	//worldViewProjection *= invView;
        //	//worldViewProjection.r[2].m128_f32[0] = cameraInfo.CameraVector.x;
        //	//worldViewProjection.r[2].m128_f32[1] = cameraInfo.CameraVector.y;
        //	//worldViewProjection.r[2].m128_f32[2] = cameraInfo.CameraVector.z;

        //	//float rx = atan2(cameraInfo.CameraVector.y, cameraInfo.CameraVector.z);
        //	//float ry = atan2(cameraInfo.CameraVector.y, cameraInfo.CameraVector.x);

        //	//worldViewProjection *= XMMatrixRotationX(rx);
        //	//worldViewProjection *= XMMatrixRotationY(ry);
        //}

        worldViewProjection *= cameraInfo.CameraPos;
    }
    else
    { //2D用

        //座標変換
        XMMATRIX projection = XMMatrixOrthographicOffCenterLH(0.0f, SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f, 0.0f, 1.0f);

        worldViewProjection *= XMMatrixTranslation(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f);

        worldViewProjection *= projection;
    }

    return worldViewProjection;
}
void PolygonRelease(DRAW_POLYGON_BUFFER* dsBuffer, POLYGON_RELEASE_OPTION opt,
    bool isCalcMat
)
{
	//深度設定を保持
	bool deSave = DXDepthStencilStateDepthEnable();

	POLYGON_RELEASE_BUFFER srb = {};

	// マトリクス設定
	XMMATRIX worldViewProjection = {};
    XMMATRIX worldViewProjectionRaw = {};

	DX_CAMERA_INFO cameraInfo = DX_GetCameraInfo(dsBuffer->cameraIndex);


	//for (int i = 0; i < dsBuffer->matrixBuf.size(); i++)
	//{
	//	worldViewProjection *= dsBuffer->matrixBuf[i].scale;

	//	worldViewProjection *= dsBuffer->matrixBuf[i].rotX;
	//	worldViewProjection *= dsBuffer->matrixBuf[i].rotY;
	//	worldViewProjection *= dsBuffer->matrixBuf[i].rotZ;

	//	//if (opt.billboard)
	//	//{
	//	//	worldViewProjection *= invView;
	//	//}

	//	worldViewProjection *= dsBuffer->matrixBuf[i].pos;
	//}
	//dsBuffer->matrixBuf.clear();
	//dsBuffer->matrixBuf.shrink_to_fit();
    if (isCalcMat)
    {
        worldViewProjectionRaw = dsBuffer->pro;
    }
    else
    {
        worldViewProjectionRaw = PolygonReleaseMat(dsBuffer, opt);
    }

	worldViewProjection = XMMatrixTranspose(worldViewProjectionRaw);

	//XMFLOAT4X4 matrix;
	//DirectX::XMStoreFloat4x4(&matrix, worldViewProjection);

	// 頂点シェーダの定数バッファに行列を送信
	SHADER_VERTEX_SET_MATRIX_EX_DAT svsmdat = {};
	svsmdat.TexScale = { opt.texWH.x, opt.texWH.y };
	svsmdat.TexUV = { opt.texUV.x, opt.texUV.y };
	svsmdat.TexColor = { opt.color.x, opt.color.y, opt.color.z, opt.color.w };
	if (opt.loadTexType == LOADTEXTURETYPE_GRAY)
	{
		//svsmdat.FixedColor = { 0.5f, 0.5f, 0.5f, 1.0f };
		svsmdat.FixedColor = { 0.5f * opt.color.x, 0.5f * opt.color.y, 0.5f * opt.color.z, 0.5f * opt.color.w };
	}
	else
	{
		svsmdat.FixedColor = { -1.0f, -1.0f, -1.0f, -1.0f };
	}

	svsmdat.EnableShade = opt.enableShade;


	//計算した値を保存
	srb.vertex = dsBuffer->vertex;
    //srb.model = dsBuffer->model;
    srb.modelNo = dsBuffer->modelNo;

	srb.drawPriority = opt.priority;
    srb.depthBuffIgnore = opt.depthBuffIgnore;
	srb.fixed = opt.fixed;
    //srb.matrix = matrix;
    srb.matrix = worldViewProjection;
	srb.svsmdat = svsmdat;

	// テクスチャの設定
    unsigned int texiddx = GetTexId(opt.texNo);
    if (texiddx == -1)
    {
        texiddx = 0;
    }
    ID3D11ShaderResourceView* texture = GetTexture_DX(texiddx);
	srb.texture = texture;

	//描画元から物体までの距離を保存
	//srb.distance = 0.0f;
	//srb.distance += CalculateDistance(
	//	MkF3(
	//		cameraInfo.CameraVector.x * -(4096.0f / cameraInfo.CameraScalingRaw), // cameraInfo.CameraPos.r[3].m128_f32[0]
	//		cameraInfo.CameraVector.y * -(4096.0f / cameraInfo.CameraScalingRaw), // cameraInfo.CameraPos.r[3].m128_f32[1]
	//		cameraInfo.CameraVector.z * -(4096.0f / cameraInfo.CameraScalingRaw)  // cameraInfo.CameraPos.r[3].m128_f32[2]
 //		),
	//	MkF3(
	//		0.0f, //dsBuffer->position.x, // worldViewProjection.r[3].m128_f32[0],
	//		0.0f, //dsBuffer->position.y, // worldViewProjection.r[3].m128_f32[1],
	//		0.0f  //dsBuffer->position.z  // worldViewProjection.r[3].m128_f32[2]
	//	)
	//);
	//srb.distance += CalculateDistance(
	//	MkF3(
	//		cameraInfo.CameraPosRaw.x, // cameraInfo.CameraPos.r[3].m128_f32[0]
	//		cameraInfo.CameraPosRaw.y, // cameraInfo.CameraPos.r[3].m128_f32[1]
	//		cameraInfo.CameraPosRaw.z  // cameraInfo.CameraPos.r[3].m128_f32[2]
 //		),
	//	MkF3(
	//		0.0f, //dsBuffer->position.x, // worldViewProjection.r[3].m128_f32[0],
	//		0.0f, //dsBuffer->position.y, // worldViewProjection.r[3].m128_f32[1],
	//		0.0f  //dsBuffer->position.z  // worldViewProjection.r[3].m128_f32[2]
	//	)
	//);
	
	float cdis = CalculateDistance(Float3(), MkF3(cameraInfo.CameraPosRaw.x, cameraInfo.CameraPosRaw.y, cameraInfo.CameraPosRaw.z));
	srb.distance += CalculateDistance(
		MkF3(
			cameraInfo.CameraVector.x * cdis, // cameraInfo.CameraPos.r[3].m128_f32[0]
			cameraInfo.CameraVector.y * cdis, // cameraInfo.CameraPos.r[3].m128_f32[1]
			cameraInfo.CameraVector.z * cdis  // cameraInfo.CameraPos.r[3].m128_f32[2]
 		),
		MkF3(
			worldViewProjection.r[3].m128_f32[0] , //dsBuffer->position.x, // worldViewProjection.r[3].m128_f32[0],
			worldViewProjection.r[3].m128_f32[1] , //dsBuffer->position.y, // worldViewProjection.r[3].m128_f32[1],
			worldViewProjection.r[3].m128_f32[2]   //dsBuffer->position.z  // worldViewProjection.r[3].m128_f32[2]
		)
	);
		//srb.distance += CalculateDistance(
	//	MkF3(
	//		-cameraInfo.CameraVector.x * 100.0f, // cameraInfo.CameraPos.r[3].m128_f32[0]
	//		-cameraInfo.CameraVector.y * 100.0f, // cameraInfo.CameraPos.r[3].m128_f32[1]
	//		-cameraInfo.CameraVector.z * 100.0f  // cameraInfo.CameraPos.r[3].m128_f32[2]
 //		),
	//	MkF3(
	//		worldViewProjection.r[3].m128_f32[0] * cameraInfo.CameraVector.x  * cameraInfo.CameraScalingRaw.x , //dsBuffer->position.x, // worldViewProjection.r[3].m128_f32[0],
	//		worldViewProjection.r[3].m128_f32[1] * cameraInfo.CameraVector.y  * cameraInfo.CameraScalingRaw.y , //dsBuffer->position.y, // worldViewProjection.r[3].m128_f32[1],
	//		worldViewProjection.r[3].m128_f32[2] * cameraInfo.CameraVector.z  * cameraInfo.CameraScalingRaw.z   //dsBuffer->position.z  // worldViewProjection.r[3].m128_f32[2]
	//	)
	//);

	//srb.distance = CalculateDistance(
	//	//MkF3(
	//	//	worldViewProjection.r[0].m128_f32[3], //dsBuffer->position.x, // worldViewProjection.r[3].m128_f32[0],
	//	//	-worldViewProjection.r[1].m128_f32[3], //dsBuffer->position.y, // worldViewProjection.r[3].m128_f32[1],
	//	//	worldViewProjection.r[2].m128_f32[3]  //dsBuffer->position.z  // worldViewProjection.r[3].m128_f32[2]
	//	//),
	//	MkF3(
	//		dsBuffer->posPivot.x, //dsBuffer->position.x, // worldViewProjection.r[3].m128_f32[0],
	//		dsBuffer->posPivot.y, //dsBuffer->position.y, // worldViewProjection.r[3].m128_f32[1],
	//		dsBuffer->posPivot.z  //dsBuffer->position.z  // worldViewProjection.r[3].m128_f32[2]
	//	),
	//	MkF3(
	//		(dsBuffer->posGameCamera.x + cameraInfo.CameraVector.x * cdis), // cameraInfo.CameraPos.r[3].m128_f32[0]
	//		(dsBuffer->posGameCamera.y + cameraInfo.CameraVector.y * cdis), // cameraInfo.CameraPos.r[3].m128_f32[1]
	//		(dsBuffer->posGameCamera.z + cameraInfo.CameraVector.z * cdis)  // cameraInfo.CameraPos.r[3].m128_f32[2]
	//	)
	//);

	//if (!opt.fixed && opt.priority == DRAW_POLYGON_PRIORITY_NORMAL)
	//{
	//	//DebugPrintf("%d  %f\n", GetMainGameFrame(), srb.distance);
	//	DebugPrintf("%d  %f\n %f %f %f\n %f %f %f \n\n", GetMainGameFrame(),
	//		srb.distance,
	//		dsBuffer->posPivot.x,
	//		dsBuffer->posPivot.y,
	//		dsBuffer->posPivot.z,

	//		dsBuffer->posGameCamera.x,
	//		dsBuffer->posGameCamera.y,
	//		dsBuffer->posGameCamera.z
	//	);
		//DebugPrintf("%d  %f\n", GetMainGameFrame(), srb.distance);
		//DebugPrintf("%d   %f %f %f\n %f %f %f %f  \n %f %f %f %f  \n %f %f %f %f  \n %f %f %f %f  \n", GetMainGameFrame(),
		//	worldViewProjection.r[0].m128_f32[3],
		//	worldViewProjection.r[1].m128_f32[3],
		//	worldViewProjection.r[2].m128_f32[3],

		//	worldViewProjection.r[0].m128_f32[0],
		//	worldViewProjection.r[0].m128_f32[1],
		//	worldViewProjection.r[0].m128_f32[2],
		//	worldViewProjection.r[0].m128_f32[3],

		//	worldViewProjection.r[1].m128_f32[0],
		//	worldViewProjection.r[1].m128_f32[1],
		//	worldViewProjection.r[1].m128_f32[2],
		//	worldViewProjection.r[1].m128_f32[3],

		//	worldViewProjection.r[2].m128_f32[0],
		//	worldViewProjection.r[2].m128_f32[1],
		//	worldViewProjection.r[2].m128_f32[2],
		//	worldViewProjection.r[2].m128_f32[3],

		//	worldViewProjection.r[3].m128_f32[0],
		//	worldViewProjection.r[3].m128_f32[1],
		//	worldViewProjection.r[3].m128_f32[2],
		//	worldViewProjection.r[3].m128_f32[3]
		//	);
	//}												
		

	srb.shaderOpt = opt.shaderOpt;

	//if (dsBuffer->gameCameraDistance != nullptr)
	//{ //ゲーム側のカメラの距離があったら加算
	//	srb.distance += (*dsBuffer->gameCameraDistance);
	//}

	{
		XMMATRIX checkMat = worldViewProjectionRaw;

		//checkMat = dsBuffer->pro; //計算順が大事

		//checkMat *= cameraInfo.CameraScaling;
		//checkMat *= cameraInfo.CameraView;
		//checkMat *= cameraInfo.CameraPos;

		//画面外は処理しない
		if (
			checkMat.r[3].m128_f32[0] < -SCREEN_WIDTH / 2 || checkMat.r[3].m128_f32[0] > SCREEN_WIDTH / 2
			|| checkMat.r[3].m128_f32[1] < -SCREEN_HEIGHT || checkMat.r[3].m128_f32[1] > SCREEN_HEIGHT / 2)
		{
		}
		else
		{
			//描画情報を保存
			g_PolygonRelease_buffers.push_back(srb);

			//{
			//	//シェーダ
			//	DX_SetShader_Main();
			//	SetBlendState_Main();

			//	//オフスクに書き込む
			//	//SetRenderTargetView_OffScreen(); //レンダーターゲットビュー
			//	//DX_SetSampler_OffScreen();

			//	polygonBufferRelease_parts(srb, deSave, 0);

			//	//if (srb.shaderOpt.outline)
			//	//{
			//	//	if (deSave)
			//	//	{ //有効だったら一時的に無効化する
			//	//		DXSetDepthStencilStateDepth(false);
			//	//	}

			//	//	//シェーダ
			//	//	DX_SetShader_OutlineBuf();
			//	//	SetRenderTargetView_OutlineBuf(); //レンダーターゲットビュー

			//	//	polygonBufferRelease_parts(srb, deSave, 0);

			//	//	if (deSave)
			//	//	{ //深度ステートをもとに戻す
			//	//		DXSetDepthStencilStateDepth(deSave);
			//	//	}
			//	//}
			//}
			
			
			//for (int i = 0; i < POLYGON_RELEASE_BUFFER_MAX; i++)
			//{
			//	if (!g_PolygonRelease_buffers_array[i].use)
			//	{
			//		g_PolygonRelease_buffers_array[i] = srb;
			//		g_PolygonRelease_buffers_array[i].use = true;
			//		break;
			//	}
			//}
		}
	}


}


void polygonBufferRelease_parts(const POLYGON_RELEASE_BUFFER& v, bool deSave, int priority)
{
	if (v.fixed && deSave)
	{ //有効だったら一時的に無効化する
		DXSetDepthStencilStateDepth(false);
	}


	// テクスチャの設定
	if (v.texture != nullptr)
	{
		g_DeviceContext->PSSetShaderResources(0, 1, &v.texture);
	}

    if (v.shaderOpt.blendState)
    {
        SetBlendState_AddColor();
    }
    else
    {
        SetBlendState_Main();
    }

    XMFLOAT4X4 matrix4x4;
    DirectX::XMStoreFloat4x4(&matrix4x4, v.matrix);

    //マトリックス設定
    XMFLOAT4X4 matrix = matrix4x4;
    SHADER_VERTEX_SET_MATRIX_EX_DAT svsmdat = v.svsmdat;

    //送信
    DX_ShaderVertexSetMatrixEx(&matrix, svsmdat);

    //if (v.model != nullptr)
    //{
    //    DX_ModelDraw(v.model);
    //}
    if (v.modelNo != 0)
    {
        DX_MODEL_ANIM modelAnimDat = {};
        //modelAnimDat.frame = (int)GetMainGameFrame();
        //modelAnimDat.svsmdat = v.svsmdat;
        //modelAnimDat.mat = v.matrix;

        DX_ModelDraw(GetModelData(v.modelNo), modelAnimDat);
        //DX_ModelDraw(v.model);
    }
	else
	{
		//SetVertex(v.vertex);
		if (v.vertex.type == DRAW_POLYGON_TYPE_FREE)
		{
   //         std::vector<Vertex_DX> vertexTmp(v.vertex.vertex);
			//SetVertexBuffer();
			//SetVertex();

            SetVertex_Quad();

            // ポリゴン描画
            g_DeviceContext->Draw(NUM_VERTEX_SQUARE, 0);
		}
        if (v.vertex.type == DRAW_POLYGON_TYPE_QUAD)
        {
            SetVertex_Quad();

            // ポリゴン描画
            g_DeviceContext->Draw(NUM_VERTEX_SQUARE, 0);
        }
        if (v.vertex.type == DRAW_POLYGON_TYPE_CUBE)
        {
            SetVertex_Cube();

            g_DeviceContext->IASetIndexBuffer(g_IndexBuffer_Cube, DXGI_FORMAT_R32_UINT, 0);

            // ポリゴン描画
            g_DeviceContext->DrawIndexed(NUM_VERTEX_CUBE, 0, 0);
        }
	}

	////アウトライン
	//if (v.outline)
	//{
	//	if (v.model != nullptr)
	//	{
	//		DX_ModelDraw(v.model);
	//	}
	//	else
	//	{
	//		SetVertex(v.vertex);
	//		// ポリゴン描画
	//		g_DeviceContext->Draw(NUM_VERTEX_SQUARE, 0);
	//	}
	//}



	if (v.fixed && deSave)
	{ //深度ステートをもとに戻す
		DXSetDepthStencilStateDepth(deSave);
	}
}


void polygonBufferRelease_post(
	ID3D11ShaderResourceView* refRes,
	XMMATRIX worldViewProjection,
	SHADER_VERTEX_SET_MATRIX_EX_DAT svsmdat,
	bool isFirst
)
{
	XMFLOAT4X4 matrix;
	DirectX::XMStoreFloat4x4(&matrix, worldViewProjection);

	// 頂点シェーダの定数バッファに行列を送信
	if (isFirst)
	{
		svsmdat.NoAlpha = true;
	}

	//送信
	DX_ShaderVertexSetMatrixEx(&matrix, svsmdat);

	g_DeviceContext->PSSetShaderResources(0, 1, &refRes);


    SetBlendState_Main();
	// ポリゴン描画

	g_DeviceContext->Draw(NUM_VERTEX_SQUARE, 0);
}

void polygonBufferRelease_MainScreen()
{
	//深度設定を保持
	bool deSave = DXDepthStencilStateDepthEnable();

	{
		ID3D11ShaderResourceView* refRes = GetSRV_OffScreen();

		if (refRes != nullptr)
		{
			if (deSave)
			{ //有効だったら一時的に無効化する
				DXSetDepthStencilStateDepth(false);
			}

			//std::vector<Vertex_DX> vertex = {};
			//for (int i = 0; i < NUM_VERTEX_SQUARE; i++)
			//{
			//	vertex.push_back(POLYGON_DEFAULT_SPRITE_VERTEX[i]);
			//}
			//SetVertex(vertex);
			SetVertex_Quad();


			//マトリックス設定
			XMMATRIX worldViewProjection = {};
			{
				XMMATRIX pos = XMMatrixTranslation(0.0f, 0.0f, 0.0f);
				XMMATRIX rot = XMMatrixRotationRollPitchYaw(0.0f, 0.0f, 0.0f);
				XMMATRIX scale = XMMatrixScaling(
					SCREEN_WIDTH,
					SCREEN_HEIGHT,
					1.0f
				);

				worldViewProjection = scale * rot * pos;
			}
			{
				//座標変換
				XMMATRIX projection = XMMatrixOrthographicOffCenterLH(0.0f, SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f, 0.0f, 1.0f);

				worldViewProjection *= XMMatrixTranslation(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f);

				worldViewProjection *= projection;
			}

			worldViewProjection = XMMatrixTranspose(worldViewProjection);

			SHADER_VERTEX_SET_MATRIX_EX_DAT svsmdat = {};
			svsmdat.TexScale = { 1.0f, 1.0f };
			svsmdat.TexUV = { 0.0f, 0.0f };
			svsmdat.TexColor = { 1.0f, 1.0f, 1.0f, 1.0f };
			svsmdat.FixedColor = { -1.0f, -1.0f, -1.0f, -1.0f };

			svsmdat.EnableShade = true;

			svsmdat.ScreenSize = { SCREEN_WIDTH, SCREEN_HEIGHT };


			if (g_ShaderIndex_Custom_0 > 0)
			{
				DX_SetShader(g_ShaderIndex_Custom_0);
			}
			else
			{
				DX_SetShader(g_ShaderIndex_Main);
			}
			SetBlendState_Main();

			//メインスクリーンに書き込む
			SetRenderTargetView_Main();
			DX_SetSampler_Main();

			//ベース描画{
			//ID3D11ShaderResourceView* refRes_OutlineBuf = GetSRV_OutlineBuf();
			polygonBufferRelease_post(
				refRes,
				worldViewProjection,
				svsmdat,
				true
			);

			//ポストエフェクト
			//if (true)
			//{
			//	SetBlendState_AddColor();

			//	XMMATRIX wvp0 = worldViewProjection;
			//	SHADER_VERTEX_SET_MATRIX_EX_DAT svsmdatTmp0 = svsmdat;
			//	svsmdatTmp0.TexColor = { 1.0f + 0.1f, 0.0f, 0.0f, 1.0f / 3 };

			//	XMMATRIX wvp1 = worldViewProjection;
			//	SHADER_VERTEX_SET_MATRIX_EX_DAT svsmdatTmp1 = svsmdat;
			//	svsmdatTmp1.TexColor = { 0.0f, 1.0f, 0.0f, 1.0f / 3 };

			//	XMMATRIX wvp2 = worldViewProjection;
			//	SHADER_VERTEX_SET_MATRIX_EX_DAT svsmdatTmp2 = svsmdat;
			//	svsmdatTmp2.TexColor = { 0.0f, 0.0f, 1.0f - 0.1f, 1.0f / 3 };

			//	polygonBufferRelease_post(
			//		refRes,
			//		wvp0,
			//		svsmdatTmp0,
			//		false
			//	);
			//	polygonBufferRelease_post(
			//		refRes,
			//		wvp1,
			//		svsmdatTmp1,
			//		false
			//	);
			//	polygonBufferRelease_post(
			//		refRes,
			//		wvp2,
			//		svsmdatTmp2,
			//		false
			//	);

			//	SetBlendState_Main();
			//}

			//ぶるーむ

			//if (false)
			{
				ID3D11ShaderResourceView* refRes_BloomBuf = GetSRV_BloomBuf();
				if (refRes_BloomBuf != nullptr)
				{
					SetBlendState_AddColor();
					DX_SetShader(g_ShaderIndex_Bloom);

					polygonBufferRelease_post(
						refRes_BloomBuf,
						worldViewProjection,
						svsmdat,
						false
					);
					//polygonBufferRelease_post(
					//	refRes,
					//	worldViewProjection,
					//	svsmdat,
					//	true
					//);

					SetBlendState_Main();
					DX_SetShader(g_ShaderIndex_Main);
				}
			}

			//アウトライン

			//if (false)
			{
				ID3D11ShaderResourceView* refRes_OutlineBuf = GetSRV_OutlineBuf();
				if (refRes_OutlineBuf != nullptr)
				{
					DX_SetShader(g_ShaderIndex_Outline);

					polygonBufferRelease_post(
						refRes_OutlineBuf,
						worldViewProjection,
						svsmdat,
						false
					);

					DX_SetShader(g_ShaderIndex_Main);
				}
			}

			//for (int i = 0; i < 1; i++)
			//{
			//}

			if (deSave)
			{ //深度ステートをもとに戻す
				DXSetDepthStencilStateDepth(deSave);
			}
		}
	}
}


void polygonBufferRelease_LayerRelease(
	const POLYGON_RELEASE_BUFFER* v, bool deSave, int i,
	bool force
)
{
	//drawCount++;

	//シェーダ
	DX_SetShader(g_ShaderIndex_Main);
	SetBlendState_Main();

    if (v->depthBuffIgnore && deSave)
    { //有効だったら一時的に無効化する
        DXSetDepthStencilStateDepth(false);
    }

	if (force)
	{
		//メインに書き込む
		SetRenderTargetView_Main(); //レンダーターゲットビュー
		DX_SetSampler_Main();

		polygonBufferRelease_parts(*v, deSave, i);
	}
	else
	{
		//オフスクに書き込む
		SetRenderTargetView_OffScreen(); //レンダーターゲットビュー
		DX_SetSampler_OffScreen();

		polygonBufferRelease_parts(*v, deSave, i);

		{ //エフェクト系の処理
			if (!v->depthBuffIgnore && deSave)
			{ //有効だったら一時的に無効化する
				DXSetDepthStencilStateDepth(false);
			}

			if (v->shaderOpt.bloom)
			{
				//シェーダ
				DX_SetShader(g_ShaderIndex_Main);
				SetRenderTargetView_BloomBuf(); //レンダーターゲットビュー
				//SetRenderTargetView_OffScreen(); //レンダーターゲットビュー

				polygonBufferRelease_parts(*v, deSave, i);
			}
			if (v->shaderOpt.outline)
			{
				//シェーダ
				DX_SetShader(g_ShaderIndex_OutlineBuf);
				//if (v->shaderOpt.outline)
				//{
				//	DX_SetShader_OutlineBuf();
				//}
				//else
				//{
				//	DX_SetShader_OutlineBuf2();
				//}

				SetRenderTargetView_OutlineBuf(); //レンダーターゲットビュー
				//SetRenderTargetView_OffScreen(); //レンダーターゲットビュー

				polygonBufferRelease_parts(*v, deSave, i);
			}

			if (!v->depthBuffIgnore && deSave)
			{ //深度ステートをもとに戻す
				DXSetDepthStencilStateDepth(deSave);
			}
		}
	}

    if (v->depthBuffIgnore && deSave)
    { //深度ステートをもとに戻す
    	DXSetDepthStencilStateDepth(deSave);
    }
}

static const POLYGON_RELEASE_BUFFER* g_PlyRelease_buffers_Buf[DRAW_POLYGON_PRIORITY_MAX][POLYGON_DRAW_MAX] = {};
void PolygonBufferRelease()
{
	//深度設定を保持
	bool deSave = DXDepthStencilStateDepthEnable();

    //DebugPrintf("%d\n", g_PolygonRelease_buffers.capacity() * sizeof(POLYGON_RELEASE_BUFFER));

	{
		//描画優先度ごとにグループ分けする

        int plyRelease_buffers_Size[DRAW_POLYGON_PRIORITY_MAX] = {};

        //std::map<int, const POLYGON_RELEASE_BUFFER*> g_PlyRelease_buffers_Buf[DRAW_POLYGON_PRIORITY_MAX] = {};
        //std::vector<const POLYGON_RELEASE_BUFFER*> g_PlyRelease_buffers_Buf[DRAW_POLYGON_PRIORITY_MAX] = {};

		for (int i = 0; i < DRAW_POLYGON_PRIORITY_MAX; i++)
		{
            plyRelease_buffers_Size[i] = 0;
			for (const POLYGON_RELEASE_BUFFER& v : g_PolygonRelease_buffers)
			{
				if (i == v.drawPriority)
				{
                    //g_PlyRelease_buffers_Buf[i].push_back(&v);
                    g_PlyRelease_buffers_Buf[i][plyRelease_buffers_Size[i]] = &v;
                    plyRelease_buffers_Size[i]++;
				}
			}
			//for (int j = 0; j < POLYGON_RELEASE_BUFFER_MAX; j++)
			//{
			//	if (i == g_PolygonRelease_buffers_array[j].drawPriority)
			//	{
			//		g_PlyRelease_buffers_Buf[i].push_back(&g_PolygonRelease_buffers_array[j]);
			//	}
			//}
			
		}

		if (deSave)
		{ // 3D用

			//fixedは無視する
			//深度順に並び替え
			for (int i = 0; i < DRAW_POLYGON_PRIORITY_MAX; i++)
			{
                //std::sort(g_PlyRelease_buffers_Buf[i].begin(),
                //    g_PlyRelease_buffers_Buf[i].end(),
                    std::sort(g_PlyRelease_buffers_Buf[i],
                        g_PlyRelease_buffers_Buf[i] + plyRelease_buffers_Size[i],
					[](const POLYGON_RELEASE_BUFFER* a, const POLYGON_RELEASE_BUFFER* b) {
						if (a == nullptr || b == nullptr)
						{
							return false;
						}
						//return (a->fixed || b->fixed) ? a->fixed < b->fixed : (a->distance) < (b->distance);
						return (a->fixed || b->fixed) ? a->fixed > b->fixed : (a->distance) > (b->distance);
					}
				);
			}
		}
		else
		{ // 2D用
		}

		//通常レイヤーをリリースする
		for (int i = DRAW_POLYGON_PRIORITY_START; i < DRAW_POLYGON_PRIORITY_END + 1; i++)
		{
			//for (const POLYGON_RELEASE_BUFFER* v : g_PlyRelease_buffers_Buf[i])
			//{
			//	polygonBufferRelease_LayerRelease(v, deSave, i, false);
			//}
            for (int j = 0; j < plyRelease_buffers_Size[i]; j++)
            {
                polygonBufferRelease_LayerRelease(g_PlyRelease_buffers_Buf[i][j], deSave, i, false);
            }
		}

		//メインスクリーンに描画
		polygonBufferRelease_MainScreen();

		//最前面レイヤーをリリースする
		for (int i = DRAW_POLYGON_PRIORITY_FORCE_START; i < DRAW_POLYGON_PRIORITY_FORCE_END + 1; i++)
		{
            //for (const POLYGON_RELEASE_BUFFER* v : g_PlyRelease_buffers_Buf[i])
            //{
            //    polygonBufferRelease_LayerRelease(v, deSave, i, true);
            //}
            for (int j = 0; j < plyRelease_buffers_Size[i]; j++)
            {
                polygonBufferRelease_LayerRelease(g_PlyRelease_buffers_Buf[i][j], deSave, i, true);
            }
		}
	}

	//バッファをクリア
    
	//for (int i = 0; i < g_PolygonRelease_buffers.size(); i++)
	//{
	//	g_PolygonRelease_buffers[i].vertex.vertex.clear();
	//	g_PolygonRelease_buffers[i].vertex.vertex.shrink_to_fit();
	//}

	g_PolygonRelease_buffers.clear();
	g_PolygonRelease_buffers.shrink_to_fit();
	//for (int i = 0; i < POLYGON_RELEASE_BUFFER_MAX; i++)
	//{
	//	g_PolygonRelease_buffers_array[i] = {};
	//}

}




//
// 頂点を設定
//

//void SetVertex(std::vector<Vertex_DX> points)
void SetVertexBuffer(std::vector<Vertex_DX> points)
{
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	ZeroMemory(&mappedResource, sizeof(D3D11_MAPPED_SUBRESOURCE));
	Vertex_DX* vertices = new Vertex_DX[points.size() + 1];
	for (int i = 0; i < points.size(); i++)
	{
		vertices[i] = points[i];
	}

	//  Disable GPU access to the vertex buffer data.
	g_DeviceContext->Map(g_VertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	//  Update the vertex buffer here.
	//memcpy(mappedResource.pData, vertices, sizeof(vertices));
	memcpy(mappedResource.pData, vertices, sizeof(Vertex_DX) * points.size());
	//  Reenable GPU access to the vertex buffer data.
	g_DeviceContext->Unmap(g_VertexBuffer, 0);

	delete[] vertices;

	//D3D11_MAPPED_SUBRESOURCE msr;
	//g_DeviceContext->Map(g_VertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);

	//Vertex_DX* v = (Vertex_DX*)msr.pData;
	//for (int i = 0; i < size; i++)
	//{
	//	v[i] = points[i];
	//}
	////memcpy_s(msr.pData, sizeof(Vertex_DX) * size, points, msr.RowPitch);

	//////頂点を設定
	//////Vertex_DX* buf = (Vertex_DX*)malloc(sizeof(Vertex_DX) * points.size());
	//////if (buf != nullptr)
	//////{
	//////	for (int i = 0; i < points.size(); i++)
	//////	{
	//////		buf[i] = points[i];
	//////	}
	//////	memcpy_s(msr.pData, sizeof(Vertex_DX) * points.size(), buf, msr.RowPitch); // 頂点コピー msr.RowPitchで全体を書き換えないとまれに落ちる
	//////}

	//g_DeviceContext->Unmap(g_VertexBuffer, 0);

	////インデックスバッファ
	//{
	//	g_DeviceContext->IASetIndexBuffer(g_IndexBuffer, DXGI_FORMAT_R32_UINT, 0);
	//}
}
void SetVertex(void)
{
	//D3D11_MAPPED_SUBRESOURCE mappedResource;
	//ZeroMemory(&mappedResource, sizeof(D3D11_MAPPED_SUBRESOURCE));
	//Vertex_DX vertices[] =
	//{
	//	{ {-0.5f, -0.5f + (float)(cos((float)GetMainGameFrame() / 1000) / 2), 0.0f}, {1.0f, 1.0f, 1.0f, 1.0f}, {0.0f, 0.0f}, {-1.0f, -1.0f, 0.1f} },
	//	{ {+0.5f, -0.5f, 0.0f}, {1.0f, 1.0f, 1.0f, 1.0f}, {1.0f, 0.0f}, {+1.0f, -1.0f, 0.1f} },
	//	{ {-0.5f, +0.5f, 0.0f}, {1.0f, 1.0f, 1.0f, 1.0f}, {0.0f, 1.0f}, {-1.0f, +1.0f, 0.1f} },
	//	{ {+0.2f, +0.3f, 0.0f}, {1.0f, 1.0f, 1.0f, 1.0f}, {1.0f, 1.0f}, {+1.0f, +1.0f, 0.1f} },
	//	{ {+0.5f, +0.5f, 0.0f}, {1.0f, 1.0f, 1.0f, 1.0f}, {1.0f, 1.0f}, {+1.0f, +1.0f, 0.1f} },
	//};
	////  Disable GPU access to the vertex buffer data.
	//g_DeviceContext->Map(g_VertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	////  Update the vertex buffer here.
	//memcpy(mappedResource.pData, vertices, sizeof(vertices));
	////  Reenable GPU access to the vertex buffer data.
	//g_DeviceContext->Unmap(g_VertexBuffer, 0);


	//D3D11_MAPPED_SUBRESOURCE msr;
	//g_DeviceContext->Map(g_VertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);
	//Vertex_DX vertices[] =
	//{
	//	{ {-0.5f, -0.5f + (float)(cos((float)GetMainGameFrame() / 1000) / 2), 0.0f}, {1.0f, 1.0f, 1.0f, 1.0f}, {0.0f, 0.0f}, {-1.0f, -1.0f, 0.1f} },
	//	{ {+0.5f, -0.5f, 0.0f}, {1.0f, 1.0f, 1.0f, 1.0f}, {1.0f, 0.0f}, {+1.0f, -1.0f, 0.1f} },
	//	{ {-0.5f, +0.5f, 0.0f}, {1.0f, 1.0f, 1.0f, 1.0f}, {0.0f, 1.0f}, {-1.0f, +1.0f, 0.1f} },
	//	{ {+0.5f, +0.5f, 0.0f}, {1.0f, 1.0f, 1.0f, 1.0f}, {1.0f, 1.0f}, {+1.0f, +1.0f, 0.1f} },
	//};
	//memcpy(msr.pData, vertices, sizeof(vertices));
	//g_DeviceContext->Unmap(g_VertexBuffer, 0);

	// 頂点バッファ設定
	UINT stride = sizeof(Vertex_DX);
	UINT offset = 0;
	g_DeviceContext->IASetVertexBuffers(0, 1, &g_VertexBuffer, &stride, &offset);

	// プリミティブトポロジ設定
	g_DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

}
void SetVertex_Quad(void)
{
    // 頂点バッファ設定
    UINT stride = sizeof(Vertex_DX);
    UINT offset = 0;
    g_DeviceContext->IASetVertexBuffers(0, 1, &g_VertexBuffer_Quad, &stride, &offset);

    // プリミティブトポロジ設定
    g_DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

}
void SetVertex_Cube(void)
{
    // 頂点バッファ設定
    UINT stride = sizeof(Vertex_DX);
    UINT offset = 0;
    g_DeviceContext->IASetVertexBuffers(0, 1, &g_VertexBuffer_Cube, &stride, &offset);

    // プリミティブトポロジ設定
    g_DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

}


void SetPostEffectShader(std::string name)
{
	if (name.size() <= 0)
	{
		g_ShaderIndex_Custom_0 = -1;
	}
	else
	{
		g_ShaderIndex_Custom_0 = DX_GetShaderIndex(name);
	}
}
void SetPostEffectShader(int index)
{
    g_ShaderIndex_Custom_0 = index;
}

int GetPostEffectShader()
{
    return g_ShaderIndex_Custom_0;
}


void PolygonTextureCache(unsigned int id)
{
    bool deSave = DXDepthStencilStateDepthEnable();

    ID3D11ShaderResourceView* refRes = GetTexture_DX(id);

    if (refRes != nullptr)
    {
        //if (GetTextureWidth(id) * GetTextureHeight(id) < 64000000)
        //{
        //    continue;
        //}

        if (deSave)
        { //有効だったら一時的に無効化する
            DXSetDepthStencilStateDepth(false);
        }

        SetVertex_Quad();

        //マトリックス設定
        XMMATRIX worldViewProjection = {};
        {
            XMMATRIX pos = XMMatrixTranslation(0.0f, 0.0f, 0.0f);
            XMMATRIX rot = XMMatrixRotationRollPitchYaw(0.0f, 0.0f, 0.0f);
            XMMATRIX scale = XMMatrixScaling(
                SCREEN_WIDTH,
                SCREEN_HEIGHT,
                1.0f
            );

            worldViewProjection = scale * rot * pos;
        }
        {
            //座標変換
            XMMATRIX projection = XMMatrixOrthographicOffCenterLH(0.0f, SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f, 0.0f, 1.0f);

            worldViewProjection *= XMMatrixTranslation(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f);

            worldViewProjection *= projection;
        }

        worldViewProjection = XMMatrixTranspose(worldViewProjection);

        SHADER_VERTEX_SET_MATRIX_EX_DAT svsmdat = {};
        svsmdat.TexScale = { 1.0f, 1.0f };
        svsmdat.TexUV = { 0.0f, 0.0f };
        svsmdat.TexColor = { 1.0f, 1.0f, 1.0f, 1.0f };
        svsmdat.FixedColor = { -1.0f, -1.0f, -1.0f, -1.0f };

        svsmdat.EnableShade = true;

        svsmdat.ScreenSize = { SCREEN_WIDTH, SCREEN_HEIGHT };

        DX_SetShader(g_ShaderIndex_Main);
        SetBlendState_Main();

        //メインスクリーンに書き込む
        SetRenderTargetView_Main();
        DX_SetSampler_Main();

        polygonBufferRelease_post(
            refRes,
            worldViewProjection,
            svsmdat,
            true
        );

        if (deSave)
        { //深度ステートをもとに戻す
            DXSetDepthStencilStateDepth(deSave);
        }
    }
}

void PolygonTextureDrawAll(std::vector<unsigned int> idList)
{
    for (unsigned int id : idList)
    {
        PolygonTextureCache(id);
    }
}