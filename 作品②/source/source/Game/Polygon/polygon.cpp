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

#include "../Texture/texData.h"
#include "modelData.h"
#include <future>



struct POLYGON_RELEASE_BUFFER {
	bool use;

	DRAW_POLYGON_PRIORITY drawPriority;
    bool depthBuffIgnore;
    bool fixed;
    bool cameraFix;

	DPD_VERTEX vertex;
    unsigned int modelNo;
	//DX_MODEL* model;

    //XMFLOAT4X4 matrix;
    XMMATRIX matrix;
	SHADER_VERTEX_SET_MATRIX_EX_DAT svsmdat;
	ID3D11ShaderResourceView* texture;
    bool isTexNone;

	float distance;

    DPD_SHADER_TYPE shaderType;
    DPD_SHADER_TYPE_EX_OPT shaderTypeEx;
	DPD_SHADER_OPT shaderOpt;
};

//static void SetVertex(std::vector<Vertex_DX> points);


static ID3D11Buffer* g_VertexBuffer = NULL;			// 線用の頂点情報
static ID3D11Buffer* g_VertexBuffer_Quad = NULL;			// 線用の頂点情報
static ID3D11Buffer* g_VertexBuffer_Quad_Up = NULL;			// 線用の頂点情報
static ID3D11Buffer* g_VertexBuffer_Quad_Object_Shadow_0 = NULL;			// 線用の頂点情報
static ID3D11Buffer* g_VertexBuffer_Cube = NULL;			// 線用の頂点情報

static ID3D11Buffer* g_IndexBuffer_Cube = NULL;			//
//static ID3D11Buffer* g_IndexBuffer = NULL;			//

// 注意！初期化で外部から設定されるもの。Release不要。
static ID3D11Device* g_Device = NULL;
static ID3D11DeviceContext* g_DeviceContext = NULL;

//static ID3D11Texture2D* g_GenralTexture = nullptr;
//static ID3D11ShaderResourceView* g_GenralSRV = nullptr;

struct shaderSetStruct {
    int vs;
    int ps;
    int gs;
};
//static shaderSetStruct g_ShaderIndex_Main = {};
static shaderSetStruct g_ShaderIndex_Main[DPD_SHADER_TYPE_MAX] = {};
static shaderSetStruct g_ShaderIndex_Bloom = {};
//static shaderSetStruct g_ShaderIndex_OutlineBuf2 = {};
static shaderSetStruct g_ShaderIndex_OutlineBuf = {};
static shaderSetStruct g_ShaderIndex_Outline = {};
static shaderSetStruct g_ShaderIndex_Custom_0 = { -1,-1 };
static Float4 g_ShaderFreeVariable_Custom_0 = {};


static int g_PolygonRelease_buffers_InOutLine = 0;
static int g_PolygonRelease_buffers_InBloom = 0;
static std::vector<POLYGON_RELEASE_BUFFER> g_PolygonRelease_buffers = {};
static std::vector<POLYGON_SRV_BUF> g_PolygonSRVDrawBuf = {};
static const POLYGON_RELEASE_BUFFER* g_PlyRelease_buffers_Buf[DRAW_POLYGON_PRIORITY_MAX][POLYGON_DRAW_MAX] = {};

static ID3D11Buffer* g_updateVertex_structuredBuffer = nullptr;

static float g_globalOutlineWidth = GLOBAL_OUTLINE_WIDTH_DEFAULT;

//static constexpr int POLYGON_RELEASE_BUFFER_MAX = (65535);
//static POLYGON_RELEASE_BUFFER g_PolygonRelease_buffers_array[POLYGON_RELEASE_BUFFER_MAX] = {};

//板ポリゴンの頂点
static constexpr Vertex_DX POLYGON_DEFAULT_SPRITE_VERTEX[NUM_VERTEX_SQUARE] = {
    { {-0.5f, -0.5f, 0.0f}, {1.0f, 1.0f, 1.0f, 1.0f}, {0.0f, 0.0f}, {-1.0f, -1.0f, 0.5f} },
    { {+0.5f, -0.5f, 0.0f}, {1.0f, 1.0f, 1.0f, 1.0f}, {1.0f, 0.0f}, {+1.0f, -1.0f, 0.5f} },
    { {-0.5f, +0.5f, 0.0f}, {1.0f, 1.0f, 1.0f, 1.0f}, {0.0f, 1.0f}, {-1.0f, +1.0f, 0.5f} },
    { {+0.5f, +0.5f, 0.0f}, {1.0f, 1.0f, 1.0f, 1.0f}, {1.0f, 1.0f}, {+1.0f, +1.0f, 0.5f} },
};
//板ポリゴンの頂点Up
static constexpr Vertex_DX POLYGON_DEFAULT_SPRITE_UP_VERTEX[NUM_VERTEX_SQUARE] = {
    { {-0.5f, 0.0f, -0.5f, }, {1.0f, 1.0f, 1.0f, 1.0f}, {0.0f, 0.0f}, {-1.0f,  0.5f, -1.0f,} },
    { {+0.5f, 0.0f, -0.5f, }, {1.0f, 1.0f, 1.0f, 1.0f}, {1.0f, 0.0f}, {+1.0f,  0.5f, -1.0f,} },
    { {-0.5f, 0.0f, +0.5f, }, {1.0f, 1.0f, 1.0f, 1.0f}, {0.0f, 1.0f}, {-1.0f,  0.5f, +1.0f,} },
    { {+0.5f, 0.0f, +0.5f, }, {1.0f, 1.0f, 1.0f, 1.0f}, {1.0f, 1.0f}, {+1.0f,  0.5f, +1.0f,} },
};
//const Vertex_DX* Get_POLYGON_DEFAULT_SPRITE_VERTEX()
//{
//	return POLYGON_DEFAULT_SPRITE_VERTEX;
//}

//板ポリゴンの頂点 カゲ
static constexpr Vertex_DX POLYGON_SPRITE_VERTEX_OBJECT_SHADOW[NUM_VERTEX_SQUARE] = {
    { {-0.7f, -0.15f, 0.0f}, {1.0f, 1.0f, 1.0f, 1.0f}, {0.0f, 0.0f}, {-1.0f, -1.0f, 0.5f} },
    { {+0.1f, -0.3f, 0.0f}, {1.0f, 1.0f, 1.0f, 1.0f}, {1.0f, 0.0f}, {+1.0f, -1.0f, 0.5f} },
    { {-0.5f, +0.15f, 0.0f}, {1.0f, 1.0f, 1.0f, 1.0f}, {0.0f, 1.0f}, {-1.0f, +1.0f, 0.5f} },
    { {+0.5f, +0.15f, 0.0f}, {1.0f, 1.0f, 1.0f, 1.0f}, {1.0f, 1.0f}, {+1.0f, +1.0f, 0.5f} },
};

void SetVertex_Ex(DRAW_POLYGON_TYPE type)
{
    ID3D11Buffer** buf = 0;

    if (type == DRAW_POLYGON_TYPE_FREE)
    {
        //         std::vector<Vertex_DX> vertexTmp(v.vertex.vertex);
                 //SetVertexBuffer();
                 //SetVertex();
        buf = &g_VertexBuffer_Quad;
    }

    if (type == DRAW_POLYGON_TYPE_QUAD)
    {
        buf = &g_VertexBuffer_Quad;
    }
    if (type == DRAW_POLYGON_TYPE_QUAD_INCETANCE)
    {
        //buf = &g_VertexBuffer_Quad_Up;
        buf = &g_VertexBuffer_Quad;
    }
    if (type == DRAW_POLYGON_TYPE_QUAD_OBJECT_SHADOW_0)
    {
        buf = &g_VertexBuffer_Quad_Object_Shadow_0;
    }


    // 頂点バッファ設定
    UINT stride = sizeof(Vertex_DX);
    UINT offset = 0;
    g_DeviceContext->IASetVertexBuffers(0, 1, buf, &stride, &offset);

    // プリミティブトポロジ設定
    g_DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

}



//static constexpr XMFLOAT4 BLOOM_COLORS[DPD_SHADER_OPT_BLOOM_MAX] = {
//    { 0.0f, 0.0f, 0.0f, 0.0f },
//    { 0.3f, 0.3f, 0.3f, 1.0f },
//
//    { 1.0f, 0.1f, 0.1f, 1.0f },
//    { 0.2f, 0.2f, 1.0f, 1.0f },
//    { 0.9f, 0.8f, 0.0f, 1.0f },
//    { 0.1f, 0.9f, 0.1f, 1.0f },
//
//    { 0.5f, 0.1f, 0.7f, 1.0f },
//    { 0.19f, 0.05f, 0.19f, 1.0f },
//};

//static constexpr XMFLOAT4 OUTLINE_COLORS[DPD_SHADER_OPT_OUTLINE_MAX] = {
//    { 0.0f, 0.0f, 0.0f, 0.0f },
//    { 0.8f, 0.8f, 0.9f, 1.0f },
//    //{ 0.5f, 0.5f, 0.6f, 1.0f },
//
//    { 1.0f, 1.0f, 1.0f, 1.0f },
//    { 1.0f, 0.1f, 0.1f, 1.0f },
//    { 0.2f, 0.2f, 1.0f, 1.0f },
//    { 0.9f, 0.8f, 0.0f, 1.0f },
//    { 0.1f, 0.9f, 0.1f, 1.0f },
//
//    { 0.5f, 0.1f, 0.7f, 1.0f },
//    { 0.19f, 0.05f, 0.19f, 1.0f },
//    //{ 0.08f, 0.0f, 0.09f, 1.0f },
//};



//
// 頂点を設定
//

//void SetVertex(std::vector<Vertex_DX> points)
void SetVertexBuffer(std::vector<Vertex_DX> points);
void SetVertex(void);
void SetVertex_Quad(void);
void SetVertex_Cube(void);


static shaderSetStruct g_ShaderIndex_LightingBug = {};
static ID3D11Buffer* g_VertexBuffer_LightingBug = nullptr;


void InitPolygon()
{
	g_PolygonRelease_buffers.reserve(1024);

	// デバイスとデバイスコンテキストの保存
	g_Device = DXGetDevice();
	g_DeviceContext = DXGetDeviceContext();


	//g_ShaderIndex_Main			= DX_GetShaderIndex("SHADER/shader");
	//g_ShaderIndex_Bloom			= DX_GetShaderIndex("SHADER/shader_bloom");
	//g_ShaderIndex_OutlineBuf2	= DX_GetShaderIndex("SHADER/shader_outlineBuf2");
	//g_ShaderIndex_OutlineBuf	= DX_GetShaderIndex("SHADER/shader_outlineBuf");
	//g_ShaderIndex_Outline		= DX_GetShaderIndex("SHADER/shader_outline");
	//g_ShaderIndex_Custom_0		= -1;

    //g_ShaderIndex_Main.vs = DX_GetShaderIndex("SHADER/vs_general");
    //g_ShaderIndex_Main.ps = DX_GetShaderIndex("SHADER/ps_general");

    for (int i = 0; i < DPD_SHADER_TYPE_MAX; i++)
    {
        g_ShaderIndex_Main[i].vs = DX_GetShaderIndex("SHADER/vs_general");
    }
    g_ShaderIndex_Main[DPD_SHADER_TYPE_NONE].ps
        = DX_GetShaderIndex("SHADER/general/ps_default");
    g_ShaderIndex_Main[DPD_SHADER_TYPE_NO_UV_LOOP].ps
        = DX_GetShaderIndex("SHADER/general/ps_no_uv_loop");
    g_ShaderIndex_Main[DPD_SHADER_TYPE_METAL].ps 
        = DX_GetShaderIndex("SHADER/general/ps_metal");
    g_ShaderIndex_Main[DPD_SHADER_TYPE_METAL_ENEMY].ps 
        = DX_GetShaderIndex("SHADER/general/ps_metal_enemy");
    g_ShaderIndex_Main[DPD_SHADER_TYPE_METAL_ENEMY_2].ps 
        = DX_GetShaderIndex("SHADER/general/ps_metal_enemy_2");
    g_ShaderIndex_Main[DPD_SHADER_TYPE_CUTIN].ps
        = DX_GetShaderIndex("SHADER/general/ps_cutin");
    g_ShaderIndex_Main[DPD_SHADER_TYPE_CUTIN_METAL_ENEMY].ps
        = DX_GetShaderIndex("SHADER/general/ps_cutin_metal_enemy");
    g_ShaderIndex_Main[DPD_SHADER_TYPE_CUTIN_METAL_ENEMY_2].ps
        = DX_GetShaderIndex("SHADER/general/ps_cutin_metal_enemy_2");
    g_ShaderIndex_Main[DPD_SHADER_TYPE_EFFECT_DIS].ps 
        = DX_GetShaderIndex("SHADER/general/ps_dis");
    g_ShaderIndex_Main[DPD_SHADER_TYPE_EFFECT_WARP].ps 
        = DX_GetShaderIndex("SHADER/general/ps_warp");
    g_ShaderIndex_Main[DPD_SHADER_TYPE_EFFECT_FRESNEL].ps
        = DX_GetShaderIndex("SHADER/general/ps_fresnel");
    g_ShaderIndex_Main[DPD_SHADER_TYPE_ALPHA_DISSOLVE].ps 
        = DX_GetShaderIndex("SHADER/general/ps_dissolve");
    g_ShaderIndex_Main[DPD_SHADER_TYPE_METAL_ENEMY_DARK].ps
        = DX_GetShaderIndex("SHADER/general/ps_metal_enemy_dark");
    g_ShaderIndex_Main[DPD_SHADER_TYPE_EFFECT].ps
        = DX_GetShaderIndex("SHADER/general/ps_effect");
    g_ShaderIndex_Main[DPD_SHADER_TYPE_BLUR].ps
        = DX_GetShaderIndex("SHADER/general/ps_blur");
    g_ShaderIndex_Main[DPD_SHADER_TYPE_UI_BG_GENERAL].ps
        = DX_GetShaderIndex("SHADER/general/ps_ui_bg_general");
    
    g_ShaderIndex_Bloom.vs = DX_GetShaderIndex("SHADER/vs_general2");
    g_ShaderIndex_Bloom.ps = DX_GetShaderIndex("SHADER/ps_bloom");

    //g_ShaderIndex_OutlineBuf2.vs = DX_GetShaderIndex("SHADER/vs_general2");
    //g_ShaderIndex_OutlineBuf2.ps = DX_GetShaderIndex("SHADER/ps_outlineBuf2");

    g_ShaderIndex_OutlineBuf.vs = DX_GetShaderIndex("SHADER/vs_general2");
    g_ShaderIndex_OutlineBuf.ps = DX_GetShaderIndex("SHADER/ps_outlineBuf");

    g_ShaderIndex_Outline.vs = DX_GetShaderIndex("SHADER/vs_general2");
    g_ShaderIndex_Outline.ps = DX_GetShaderIndex("SHADER/ps_outline");

    g_ShaderIndex_Custom_0.vs = -1;
    g_ShaderIndex_Custom_0.ps = -1;
    g_ShaderFreeVariable_Custom_0 = {};


    //
    {
        g_ShaderIndex_LightingBug.vs = DX_GetShaderIndex("SHADER/vs_world_lightningbug");
        g_ShaderIndex_LightingBug.ps = DX_GetShaderIndex("SHADER/ps_world_lightningbug");
        g_ShaderIndex_LightingBug.gs = DX_GetShaderIndex("SHADER/gs_point_sprite");

        constexpr int vnum = 10000;
        Vertex_DX* vertextmp = new Vertex_DX[vnum];
        for (int i = 0; i < vnum; i++)
        {
            vertextmp[i] = {};
            vertextmp[i].pos.z = 1.0f * i;
            vertextmp[i].color = {1.0f, 1.0f, 1.0f, 1.0f};
        }

        //↓だいじ
        // 頂点バッファ生成
        D3D11_BUFFER_DESC bd = {};
        bd.Usage = D3D11_USAGE_DEFAULT;
        bd.ByteWidth = sizeof(Vertex_DX) * vnum;
        bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
        bd.CPUAccessFlags = 0;
        D3D11_SUBRESOURCE_DATA sd = {};
        sd.pSysMem = vertextmp;

        g_Device->CreateBuffer(&bd, &sd, &g_VertexBuffer_LightingBug);

        delete[] vertextmp;
    }

    //{// 白色のテクスチャを作成
    //    D3D11_TEXTURE2D_DESC texDesc = {};
    //    texDesc.Width = 4;
    //    texDesc.Height = 4;
    //    texDesc.MipLevels = 1;
    //    texDesc.ArraySize = 1;
    //    texDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    //    texDesc.SampleDesc.Count = 1;
    //    texDesc.Usage = D3D11_USAGE_DEFAULT;
    //    texDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;

    //    D3D11_SUBRESOURCE_DATA initData = {};
    //    UINT whitePixel[4 * 4] = { 0xFFFFFFFF }; // 白色
    //    initData.pSysMem = whitePixel;
    //    initData.SysMemPitch = 4 * sizeof(UINT);

    //    g_Device->CreateTexture2D(&texDesc, &initData, &g_GenralTexture);

    //    // シェーダーリソースビューを作成
    //    if (g_GenralTexture != nullptr)
    //    {
    //        g_Device->CreateShaderResourceView(g_GenralTexture, nullptr, &g_GenralSRV);
    //    }
    //}

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
        constexpr int vnum = ARRAYSIZE(POLYGON_SPRITE_VERTEX_OBJECT_SHADOW);
        Vertex_DX* vertextmp = new Vertex_DX[vnum];
        for (int i = 0; i < vnum; i++)
        {
            vertextmp[i] = POLYGON_SPRITE_VERTEX_OBJECT_SHADOW[i];
        }
        
        //↓だいじ
        // 頂点バッファ生成
        D3D11_BUFFER_DESC bd = {};
        bd.Usage = D3D11_USAGE_DEFAULT;
        bd.ByteWidth = sizeof(Vertex_DX) * vnum;
        bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
        bd.CPUAccessFlags = 0;

        D3D11_SUBRESOURCE_DATA sd = {};
        sd.pSysMem = vertextmp;


        g_Device->CreateBuffer(&bd, &sd, &g_VertexBuffer_Quad_Object_Shadow_0);

        delete[] vertextmp;
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
        {
            // 頂点バッファ生成
            D3D11_BUFFER_DESC bd = {};
            bd.Usage = D3D11_USAGE_DEFAULT;
            bd.ByteWidth = sizeof(Vertex_DX) * NUM_VERTEX_SQUARE;
            bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
            bd.CPUAccessFlags = 0;
            //bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

            D3D11_SUBRESOURCE_DATA sd = {};
            sd.pSysMem = POLYGON_DEFAULT_SPRITE_UP_VERTEX;

            g_Device->CreateBuffer(&bd, &sd, &g_VertexBuffer_Quad_Up);
        }
    }
	{
        //static Vertex_DX vertex_cube[NUM_VERTEX_CUBE] = {};
        Vertex_DX* vertex_cube = new Vertex_DX[NUM_VERTEX_CUBE];

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

        delete[] vertex_cube;
	}

    {
        D3D11_BUFFER_DESC bufferDesc = {};
        bufferDesc.ByteWidth = sizeof(Vertex_DX) * NUM_VERTEX_UPDATEV;
        bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
        bufferDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
        bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

        g_Device->CreateBuffer(&bufferDesc, nullptr,
            &g_updateVertex_structuredBuffer);

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
    SAFE_RELEASE(g_VertexBuffer_Quad_Up);
    SAFE_RELEASE(g_VertexBuffer_Quad_Object_Shadow_0);

	
	SAFE_RELEASE(g_IndexBuffer_Cube);
	SAFE_RELEASE(g_VertexBuffer_Cube);

    //SAFE_RELEASE(g_GenralSRV);
    //SAFE_RELEASE(g_GenralTexture);
    
    SAFE_RELEASE(g_updateVertex_structuredBuffer);

    SAFE_RELEASE(g_VertexBuffer_LightingBug);
}


//セットアップ
void polygonSetUp_private(DRAW_POLYGON_BUFFER* dsBuffer)
{
	(*dsBuffer) = {};
    dsBuffer->pro = XMMatrixTranslation(0.0f, 0.0f, 0.0f); //一度原点の位置へもっていく
    dsBuffer->proObj = XMMatrixTranslation(0.0f, 0.0f, 0.0f); //一度原点の位置へもっていく

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
	Float3 pos, Float3 rot, Float3 scale,
    bool isObj
)
{
	//深度設定を保持
	bool deSave = DXDepthStencilStateDepthEnable();

	//DPB_MATRIX_BUF buf = {};

	//buf.scale = XMMatrixScaling(scale.x, scale.y, scale.z); //指定のサイズにする
	XMMATRIX scaleMat = XMMatrixScaling(scale.x, scale.y, scale.z); //指定のサイズにする
    dsBuffer->pro *= scaleMat;
    if(isObj) dsBuffer->proObj *= scaleMat;

	//dsBuffer->position.x *= scale.x;
	//dsBuffer->position.y *= scale.y;

	if (deSave)
	{
		//else
		{
			//buf.rotX = XMMatrixRotationX(XMConvertToRadians(rot.x));
            XMMATRIX rotxMat = XMMatrixRotationX(XMConvertToRadians(rot.x));
            dsBuffer->pro *= rotxMat;
            if (isObj) dsBuffer->proObj *= rotxMat;
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
            XMMATRIX rotyMat = XMMatrixRotationY(XMConvertToRadians(rot.y));
            dsBuffer->pro *= rotyMat;
            if (isObj) dsBuffer->proObj *= rotyMat;

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
    XMMATRIX rotzMat = XMMatrixRotationZ(XMConvertToRadians(rot.z));
    dsBuffer->pro *= rotzMat;
    if (isObj) dsBuffer->proObj *= rotzMat;

    if (dsBuffer->billboard)
    {
        DX_CAMERA_INFO cameraInfo = DX_GetCameraInfo(dsBuffer->cameraIndex);
        //XMMATRIX invView = {};

        //XMFLOAT3 pos = cameraInfo.CameraPosRaw;
        //pos.x *= -cameraInfo.CameraScalingRaw.x;
        //pos.y *= cameraInfo.CameraScalingRaw.y;
        //pos.z *= cameraInfo.CameraScalingRaw.z;
        //XMFLOAT3 target = cameraInfo.CameraTargetRaw;
        //XMFLOAT3 up = cameraInfo.CameraUpRaw;
        //up.y *= -1.0f;
//         //up = { 0.0f, -1.0f, 0.0f };

         //XMMATRIX CameraViewTmp = XMMatrixLookAtLH(XMLoadFloat3(&pos), XMLoadFloat3(&target), XMLoadFloat3(&up));

         //invView = XMMatrixInverse(nullptr, CameraViewTmp);
         //invView.r[3].m128_f32[0] = 0.0f;
         //invView.r[3].m128_f32[1] = 0.0f;
         //invView.r[3].m128_f32[2] = 0.0f;

         //Float3 vectmp = MkF3(cameraInfo.CameraScalingRaw.x,
         //    cameraInfo.CameraScalingRaw.y, cameraInfo.CameraScalingRaw.z);
         //vectmp = NormalizeVector(vectmp);

        XMFLOAT3 pos = cameraInfo.CameraPosRaw;
        //pos.x *= -1.0f;
        pos.y *= -1.0f;
        //pos.z *= -1.0f;
        XMFLOAT3 target = cameraInfo.CameraTargetRaw;
        XMFLOAT3 up = cameraInfo.CameraUpRaw;
        up = { 0.0f, 0.0f, 1.0f };

        XMMATRIX CameraViewTmp = XMMatrixLookAtLH(XMLoadFloat3(&pos), XMLoadFloat3(&target), XMLoadFloat3(&up));

        // ビューの逆行列
        XMMATRIX invView;
        invView = XMMatrixInverse(nullptr, CameraViewTmp); // 逆行列
        //invView = XMMatrixInverse(nullptr, cameraInfo.CameraView); // 逆行列
        invView.r[3].m128_f32[0] = 0.0f;
        invView.r[3].m128_f32[1] = 0.0f;
        invView.r[3].m128_f32[2] = 0.0f;


        //dsBuffer->pro *= XMMatrixScaling(vectmp.x, vectmp.y, vectmp.z);
        dsBuffer->pro *= invView;
        if (isObj) dsBuffer->proObj *= invView;

        dsBuffer->billboard = false; //フラグを折る
    }
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
        XMMATRIX posMat = XMMatrixTranslation(pos.x, pos.y, pos.z); //本来の位置
        dsBuffer->pro *= posMat;
        if (isObj) dsBuffer->proObj *= posMat;
		//dsBuffer->position.z += pos.z;

	}
	else
	{ //2D
		//buf.pos = XMMatrixTranslation(pos.x, pos.y, 0.0f); //本来の位置
        XMMATRIX posMat = XMMatrixTranslation(pos.x, pos.y, 0.0f); //本来の位置
        dsBuffer->pro *= posMat;
        if (isObj) dsBuffer->proObj *= posMat;
		
		//worldViewProjection *= XMMatrixTranslation(
		//	(float)(SCREEN_WIDTH_BASE - SCREEN_WIDTH) / 2,
		//	(float)(SCREEN_HEIGHT_BASE - SCREEN_HEIGHT) / 2,
		//	0.0f);
	}

	//dsBuffer->matrixBuf.push_back(buf);
}

//解放(描画)
void polygonBufferRelease_parts(const POLYGON_RELEASE_BUFFER& v, bool deSave, int priority);

XMMATRIX PolygonReleaseMat(
    DRAW_POLYGON_BUFFER* dsBuffer, 
    const POLYGON_RELEASE_OPTION& opt
)
{
    //深度設定を保持
    bool deSave = DXDepthStencilStateDepthEnable();

    DX_CAMERA_INFO cameraInfo = DX_GetCameraInfo(dsBuffer->cameraIndex);

    XMMATRIX worldViewProjection = dsBuffer->pro; //計算順が大事

    //深度ステート有効だったらカメラの設定を反映する
    if (deSave && !opt.fixed)
    { //3D用

        if (opt.cameraFix)
        {
            //// 2) オブジェクトのワールド座標（移動成分）を取得
            ////    → ワールドに原点(0,0,0)を変換する
            ////XMVECTOR objPos = XMVector3TransformCoord(
            ////    XMVectorZero(), 
            ////    worldViewProjection
            ////); //*XMMatrixRotationX(XMConvertToRadians(-35.0f))
            //XMVECTOR objPos = worldViewProjection.r[3];

            //// 3) カメラのベース位置／注視点をオフセット付きで決定
            ////    cameraInfo.CameraPosRaw は「キャラ中心から見たカメラ相対位置」など
            //XMVECTOR baseEye = XMLoadFloat3(&cameraInfo.CameraPosRaw);
            //XMVECTOR baseTarget = XMLoadFloat3(&cameraInfo.CameraTargetRaw);
            //XMVECTOR upVec = XMLoadFloat3(&cameraInfo.CameraUpRaw);

            ////    ワールド空間上の最終的なカメラ位置・注視点
            //XMVECTOR eye = objPos + baseEye;
            //XMVECTOR target = objPos + baseTarget;

            //// 4) ビュー行列を作成
            //XMMATRIX view = XMMatrixLookAtLH(eye, target, upVec);

            //// 5) 必要ならカメラスケール（ズーム）も掛ける
            ////    cameraInfo.CameraScaling がスケーリング行列なら
            //XMMATRIX viewScaled = view * cameraInfo.CameraScaling;

            //// 6) プロジェクション行列（透視 or 正射影）
            //XMMATRIX proj = cameraInfo.CameraPos;  // 変数名に注意：実態は Projection

            //// 7) 最終合成： World → View(Scaled) → Projection
            //worldViewProjection *= viewScaled * proj;

            // ① まずビュー行列からカメラのワールド行列を得る
            //    view = XMMatrixLookAtLH(eye, target, up);

// 1) オブジェクトのワールド行列を取り出す
//    dsBuffer->pro には「scale×rotation×translation(オブジェクト)」だけが入っている想定
            XMMATRIX world = worldViewProjection;

            // 2) ワールド行列の平行移動成分（オブジェクト位置）を抽出
            XMVECTOR objPos = world.r[3];  // { x, y, z, w }
            //XMVECTOR objPos = {};  // { x, y, z, w }
            //XMVECTOR objPos = XMVector3TransformCoord(
            //    XMVectorZero(), 
            //    worldViewProjection
            //); //*XMMatrixRotationX(XMConvertToRadians(-35.0f))

            // 3) カメラの相対オフセットをロード
            XMVECTOR baseEye = XMLoadFloat3(&cameraInfo.CameraPosRaw);    // カメラ → キャラ中心 の相対位置
            XMVECTOR baseTarget = XMLoadFloat3(&cameraInfo.CameraTargetRaw); // カメラ目標 → キャラ中心 の相対オフセット
            XMVECTOR upVec = XMLoadFloat3(&cameraInfo.CameraUpRaw);

            // 4) 最終的なカメラのワールド座標／注視点を計算
            //    キャラ位置 + オフセット で常にキャラを追いかける
            XMVECTOR eye = objPos + baseEye;
            XMVECTOR target = objPos + baseTarget;

            // 5) ビュー行列を作成
            XMMATRIX view = XMMatrixLookAtLH(eye, target, upVec);

            //// 2) View×Proj を合成して逆行列を取る
            //XMMATRIX vp = view * cameraInfo.CameraPos;
            //XMMATRIX invViewProj = XMMatrixInverse(nullptr, vp);

            //// 3) pos の右側に invViewProj を掛けると world 行列になる
            //XMMATRIX world2 = world * invViewProj;


            //    （もしズーム等でカメラスケールをかけたいならここで）
            //    カメラスケール行列は「view の後ろ」に掛けるとズーム扱いになります
            XMMATRIX viewScaled = view * cameraInfo.CameraScaling;

            XMFLOAT3 objposs = {};
            XMStoreFloat3(&objposs, dsBuffer->proObj.r[3]);
            //hal::dout << "x: " << objposs.x << std::endl;
            //hal::dout << "y: " << objposs.y << std::endl;
            worldViewProjection *= XMMatrixTranslation(
                objposs.x * +1.0f,
                objposs.y * -1.0f,
                objposs.z * +1.0f
            );
            //worldViewProjection *= world2;
            //worldViewProjection = world * cameraInfo.CameraScaling;

            worldViewProjection *= viewScaled;

            worldViewProjection *= cameraInfo.CameraPos;


        }
        else
        {
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

    //
    svsmdat.World = worldViewProjectionRaw;
    svsmdat.View = cameraInfo.CameraView;
    svsmdat.Projection = cameraInfo.CameraPos;
    //

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

    //svsmdat.EnableShade = opt.enableShade;
    //svsmdat.ShaderType = opt.shaderType; //ShaderType廃止
    svsmdat.CameraPos = cameraInfo.CameraPosRaw;
    svsmdat.ScreenSize = { SCREEN_WIDTH, SCREEN_HEIGHT };
    svsmdat.GameFrame = GetMainGameFrame();

	//計算した値を保存
	srb.vertex = dsBuffer->vertex;
    //srb.model = dsBuffer->model;
    srb.modelNo = dsBuffer->modelNo;

	srb.drawPriority = opt.priority;
    srb.depthBuffIgnore = opt.depthBuffIgnore;
    srb.fixed = opt.fixed;
    srb.cameraFix = opt.cameraFix;
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

    if (opt.texNo == 0)
    {
        srb.isTexNone = true;
    }
    else
    {
        srb.isTexNone = false;
    }


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
	
	//float cdis = CalculateDistance(Float3(), MkF3(cameraInfo.CameraPosRaw.x, cameraInfo.CameraPosRaw.y, cameraInfo.CameraPosRaw.z));
	//srb.distance += CalculateDistance(
	//	MkF3(
	//		cameraInfo.CameraVector.x * cdis, // cameraInfo.CameraPos.r[3].m128_f32[0]
	//		cameraInfo.CameraVector.y * cdis, // cameraInfo.CameraPos.r[3].m128_f32[1]
	//		cameraInfo.CameraVector.z * cdis  // cameraInfo.CameraPos.r[3].m128_f32[2]
 //		),
	//	MkF3(
	//		worldViewProjection.r[3].m128_f32[0] , //dsBuffer->position.x, // worldViewProjection.r[3].m128_f32[0],
	//		worldViewProjection.r[3].m128_f32[1] , //dsBuffer->position.y, // worldViewProjection.r[3].m128_f32[1],
	//		worldViewProjection.r[3].m128_f32[2]   //dsBuffer->position.z  // worldViewProjection.r[3].m128_f32[2]
	//	)
	//);
    srb.distance = worldViewProjection.r[3].m128_f32[2];

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
		

    srb.shaderType = opt.shaderType;
    srb.shaderTypeEx = opt.shaderTypeEx;
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

		////画面外は処理しない
		//if (
		//	checkMat.r[3].m128_f32[0] < -SCREEN_WIDTH / 2 || checkMat.r[3].m128_f32[0] > SCREEN_WIDTH / 2
		//	|| checkMat.r[3].m128_f32[1] < -SCREEN_HEIGHT || checkMat.r[3].m128_f32[1] > SCREEN_HEIGHT / 2)
		//{
		//}
		//else
		{
            //if (srb.shaderOpt.outline != DPD_SHADER_OPT_OUTLINE_NONE)
            //{
            //    g_PolygonRelease_buffers_InOutLine++;
            //}
            //if (srb.shaderOpt.bloom != DPD_SHADER_OPT_BLOOM_NONE)
            //{
            //    g_PolygonRelease_buffers_InBloom++;
            //}

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

//static std::vector<DPD_VERTEX_FRAGMENT> polygonTriangulatePolygon(
//    const std::vector<DPD_VERTEX_FRAGMENT>& polygon)
//{
//    std::vector<DPD_VERTEX_FRAGMENT> triangles;
//    std::vector<DPD_VERTEX_FRAGMENT> tempPolygon = polygon;
//
//    while (tempPolygon.size() > 2)
//    {
//        for (size_t i = 0; i < tempPolygon.size(); i++)
//        {
//            size_t prev = (i == 0) ? tempPolygon.size() - 1 : i - 1;
//            size_t next = (i + 1) % tempPolygon.size();
//
//            // 三角形を追加
//            triangles.push_back(tempPolygon[prev]);
//            triangles.push_back(tempPolygon[i]);
//            triangles.push_back(tempPolygon[next]);
//
//            // 耳を削除
//            tempPolygon.erase(tempPolygon.begin() + i);
//            break;
//        }
//    }
//
//    return triangles;
//}

static Float2 polygonComputeUV(
    Float3 vertex, Float3 minBounds, Float3 maxBounds
) 
{
    Float2 uv;
    uv.x = (vertex.x - minBounds.x) / (maxBounds.x - minBounds.x);
    uv.y = (vertex.y - minBounds.y) / (maxBounds.y - minBounds.y);
    return uv;
}

// **三角形内に他の頂点が含まれるか確認**
static bool polygonIsEarValid(
    const std::vector<DPD_VERTEX_FRAGMENT>& polygon,
    size_t prev, size_t curr, size_t next
)
{
    Float3 v0 = polygon[prev].pos;
    Float3 v1 = polygon[curr].pos;
    Float3 v2 = polygon[next].pos;

    for (size_t i = 0; i < polygon.size(); i++)
    {
        if (i != prev && i != curr && i != next) 
        {
            Float3 p = polygon[i].pos;
            // ここで「頂点 p が三角形 (v0, v1, v2) 内にあるか」判定
            if (CheckPointTriangleCollider(p, v0, v1, v2))
            {
                return false;
            }
        }
    }
    return true;
}


// **3. 三角形分割（Ear Clipping法）**
static std::vector<DPD_VERTEX_FRAGMENT> 
    polygonTriangulatePolygonWithUV(
        const std::vector<DPD_VERTEX_FRAGMENT>& polygon
    ) 
{
    std::vector<DPD_VERTEX_FRAGMENT> triangles;
    Float3 minBounds = { 
        std::numeric_limits<float>::max(),
        std::numeric_limits<float>::max(),
        std::numeric_limits<float>::max() };
    Float3 maxBounds = {
        std::numeric_limits<float>::lowest(), 
        std::numeric_limits<float>::lowest(),
        std::numeric_limits<float>::lowest() };

    // 頂点の範囲を取得
    for (const auto& p : polygon) {
        minBounds.x = std::min(minBounds.x, p.pos.x);
        minBounds.y = std::min(minBounds.y, p.pos.y);
        maxBounds.x = std::max(maxBounds.x, p.pos.x);
        maxBounds.y = std::max(maxBounds.y, p.pos.y);
    }

    // 三角形化（Ear Clipping法）
    std::vector<DPD_VERTEX_FRAGMENT> tempPolygon = polygon;
    //while (tempPolygon.size() > 2)
    for (size_t j = 0; j < 16; j++)
    {
        if (tempPolygon.size() <= 2)
        {
            break;
        }
        for (size_t i = 0; i < tempPolygon.size(); i++)
        {
            size_t prev = (i == 0) ? tempPolygon.size() - 1 : i - 1;
            size_t next = (i + 1) % tempPolygon.size();

            if (polygonIsEarValid(tempPolygon, prev, i, next))
            {
                // 三角形を追加（UV適用）
                DPD_VERTEX_FRAGMENT v0 = {
                    tempPolygon[prev].pos,
                    {1.0f, 1.0f, 1.0f, 1.0f},
                    polygonComputeUV(tempPolygon[prev].pos, minBounds, maxBounds),
                    {0.0f, 0.0f, 1.0f} };
                DPD_VERTEX_FRAGMENT v1 = {
                    tempPolygon[i].pos,
                    {1.0f, 1.0f, 1.0f, 1.0f},
                    polygonComputeUV(tempPolygon[i].pos, minBounds, maxBounds),
                    {0.0f, 0.0f, 1.0f} };
                DPD_VERTEX_FRAGMENT v2 = {
                    tempPolygon[next].pos,
                    {1.0f, 1.0f, 1.0f, 1.0f},
                    polygonComputeUV(tempPolygon[next].pos, minBounds, maxBounds),
                    {0.0f, 0.0f, 1.0f} };

                triangles.push_back(v0);
                triangles.push_back(v1);
                triangles.push_back(v2);

                // 耳を削除
                tempPolygon.erase(tempPolygon.begin() + i);
                break;
            }
        }
    }

    return triangles;
}

//// **1. Y座標でソート**
//static std::vector<DPD_VERTEX_FRAGMENT> polygonSortByY(
//    std::vector<DPD_VERTEX_FRAGMENT> polygon
//) {
//    std::sort(polygon.begin(), polygon.end(), [](const DPD_VERTEX_FRAGMENT& a, const DPD_VERTEX_FRAGMENT& b) {
//        return a.pos.y < b.pos.y;
//        });
//    return polygon;
//}
//
//// **2. モンジュール法による三角形化**
//static std::vector<DPD_VERTEX_FRAGMENT> polygonMonotoneTriangulation(
//    const std::vector<DPD_VERTEX_FRAGMENT>& polygon
//) {
//    std::vector<DPD_VERTEX_FRAGMENT> triangles;
//    Float3 minBounds = { 
//        std::numeric_limits<float>::max(),
//        std::numeric_limits<float>::max(),
//        std::numeric_limits<float>::max() };
//    Float3 maxBounds = {
//        std::numeric_limits<float>::lowest(), 
//        std::numeric_limits<float>::lowest(),
//        std::numeric_limits<float>::lowest() };
//    std::vector<DPD_VERTEX_FRAGMENT> sortedPolygon = polygonSortByY(polygon);
//
//    while (sortedPolygon.size() > 2) {
//        for (size_t i = 1; i < sortedPolygon.size() - 1; i++) {
//            //// 三角形を追加
//            //triangles.push_back(sortedPolygon[0]);
//            //triangles.push_back(sortedPolygon[i]);
//            //triangles.push_back(sortedPolygon[i + 1]);
//
//            // 三角形を追加（UV適用）
//            DPD_VERTEX_FRAGMENT v0 = {
//                sortedPolygon[0].pos,
//                {1.0f, 1.0f, 1.0f, 1.0f},
//                polygonComputeUV(sortedPolygon[0].pos, minBounds, maxBounds),
//                {0.0f, 0.0f, 1.0f} };
//            DPD_VERTEX_FRAGMENT v1 = {
//                sortedPolygon[i].pos,
//                {1.0f, 1.0f, 1.0f, 1.0f},
//                polygonComputeUV(sortedPolygon[i].pos, minBounds, maxBounds),
//                {0.0f, 0.0f, 1.0f} };
//            DPD_VERTEX_FRAGMENT v2 = {
//                sortedPolygon[i + 1].pos,
//                {1.0f, 1.0f, 1.0f, 1.0f},
//                polygonComputeUV(sortedPolygon[i + 1].pos, minBounds, maxBounds),
//                {0.0f, 0.0f, 1.0f} };
//
//            triangles.push_back(v0);
//            triangles.push_back(v1);
//            triangles.push_back(v2);
//
//            // 頂点を削除
//            sortedPolygon.erase(sortedPolygon.begin() + i);
//            break;
//        }
//    }
//
//    return triangles;
//}


// 三角形の外接円を計算
//static bool IsPointInsideCircumcircle(Float3 p, FloatTriangle t) 
//{
//    float ax = t.v0.x - p.x;
//    float ay = t.v0.y - p.y;
//    float bx = t.v1.x - p.x;
//    float by = t.v1.y - p.y;
//    float cx = t.v2.x - p.x;
//    float cy = t.v2.y - p.y;
//
//    float det = (ax * (by * cy - cy * by)) 
//        - (ay * (bx * cy - cx * by))
//        + (bx * (cy * ay - by * cx));
//    return det > 0;
//}
//static bool IsNearlyEqual(float a, float b, float epsilon = 1e-6f) {
//    return std::fabs(a - b) < epsilon;
//}
//static std::vector<FloatTriangle> RemoveDuplicateTriangles(
//    const std::vector<FloatTriangle>& triangles)
//{
//    std::vector<FloatTriangle> uniqueTriangles;
//    for (const auto& tri : triangles) 
//    {
//        bool isDuplicate = false;
//        for (const auto& ut : uniqueTriangles)
//        {
//            if (
//                (      IsNearlyEqual(tri.v0.x, ut.v0.x) && IsNearlyEqual(tri.v0.x, ut.v0.x) && IsNearlyEqual(tri.v0.x, ut.v0.x)
//                    && IsNearlyEqual(tri.v0.y, ut.v0.y) && IsNearlyEqual(tri.v0.y, ut.v0.y) && IsNearlyEqual(tri.v0.y, ut.v0.y)
//                    && IsNearlyEqual(tri.v0.z, ut.v0.z) && IsNearlyEqual(tri.v0.z, ut.v0.z) && IsNearlyEqual(tri.v0.z, ut.v0.z)) ||
//                (      IsNearlyEqual(tri.v1.x, ut.v1.x) && IsNearlyEqual(tri.v1.x, ut.v1.x) && IsNearlyEqual(tri.v1.x, ut.v1.x)
//                    && IsNearlyEqual(tri.v1.y, ut.v1.y) && IsNearlyEqual(tri.v1.y, ut.v1.y) && IsNearlyEqual(tri.v1.y, ut.v1.y)
//                    && IsNearlyEqual(tri.v1.z, ut.v1.z) && IsNearlyEqual(tri.v1.z, ut.v1.z) && IsNearlyEqual(tri.v1.z, ut.v1.z)) ||
//                (      IsNearlyEqual(tri.v2.x, ut.v2.x) && IsNearlyEqual(tri.v2.x, ut.v2.x) && IsNearlyEqual(tri.v2.x, ut.v2.x)
//                    && IsNearlyEqual(tri.v2.y, ut.v2.y) && IsNearlyEqual(tri.v2.y, ut.v2.y) && IsNearlyEqual(tri.v2.y, ut.v2.y)
//                    && IsNearlyEqual(tri.v2.z, ut.v2.z) && IsNearlyEqual(tri.v2.z, ut.v2.z) && IsNearlyEqual(tri.v2.z, ut.v2.z))
//                ) {
//                isDuplicate = true;
//                break;
//            }
//        }
//        if (!isDuplicate) {
//            uniqueTriangles.push_back(tri);
//        }
//    }
//    return uniqueTriangles;
//}
//// Delaunay 三角形分割
//static std::vector<FloatTriangle> DelaunayTriangulation(
//    const std::vector<DPD_VERTEX_FRAGMENT>& points
//)
//{
//    std::vector<FloatTriangle> triangles;
//
//    float maxbX = 0;
//    float minbX = 0;
//    float maxbY = 0;
//    float minbY = 0;
//    {
//        for (int i = 0; i < points.size(); i++)
//        {
//            if (minbX > points[i].pos.x)
//            {
//                minbX = points[i].pos.x;
//            }
//            if (maxbX < points[i].pos.x)
//            {
//                maxbX = points[i].pos.x;
//            }
//            if (minbY > points[i].pos.y)
//            {
//                minbY = points[i].pos.y;
//            }
//            if (maxbY < points[i].pos.y)
//            {
//                maxbY = points[i].pos.y;
//            }
//        }
//    }
//    float bsize = ((maxbX - minbX) + (maxbY - minbY)) / 2.0f * 4.0f;
//
//    // 初期三角形を作成（仮の大きな三角形）
//    FloatTriangle superTriangle = {
//        {-bsize, -bsize}, {bsize, -bsize}, {0, bsize}
//    };
//    triangles.push_back(superTriangle);
//
//    for (const auto& p : points)
//    {
//        std::vector<FloatTriangle> badTriangles;
//        for (const auto& t : triangles) 
//        {
//            if (IsPointInsideCircumcircle(p.pos, t))
//            {
//                badTriangles.push_back(t);
//            }
//        }
//
//        // 新しい三角形を追加
//        for (const auto& bt : badTriangles)
//        {
//            triangles.push_back({ bt.v0, bt.v1, p.pos });
//            triangles.push_back({ bt.v1, bt.v2, p.pos });
//            triangles.push_back({ bt.v2, bt.v0, p.pos });
//        }
//    }
//
//    return triangles;
//}

//#include <iostream>
//#include <vector>
//#include <tuple>
//#include <cmath>


////// 円に含まれるか判定（ドロネー条件）
////static bool inCircle(Float3 p, Float3 a, Float3 b, Float3 c) {
////    double ax = a.x - p.x, ay = a.y - p.y;
////    double bx = b.x - p.x, by = b.y - p.y;
////    double cx = c.x - p.x, cy = c.y - p.y;
////
////    double det = (ax * (by * cy - cy * by) - bx * (ay * cy - cy * ay) + cx * (ay * by - by * ay));
////
////    return det > 0; // 簡易判定（本来は厳密な外接円計算が必要）
////}
//// 外接円の判定（行列式を使った方法）
//static bool inCircle(Float3 p, Float3 a, Float3 b, Float3 c) {
//    //double det =
//    //    (a.x * (b.y * c.z + b.z * p.y + c.y * p.z - b.z * c.y - p.y * c.z - p.z * b.y)) -
//    //    (a.y * (b.x * c.z + b.z * p.x + c.x * p.z - b.z * c.x - p.x * c.z - p.z * b.x)) +
//    //    (a.z * (b.x * c.y + b.y * p.x + c.x * p.y - b.y * c.x - p.x * c.y - p.y * b.x));
//
//    //return det > 0;
//
//    Float3 pos = CalculateCentroid({a, b, c});
//    float dis = CalculateDistance(pos, a);
//    return CheckCircleCollider(
//        Float3ToFloat2(p),
//        Float3ToFloat2(pos),
//        0.0f,
//        dis
//    );
//}
//
//
//static std::vector<FloatTriangle> bowyerWatson(
//    std::vector<DPD_VERTEX_FRAGMENT>& points) 
//{
//    std::vector<std::tuple<Float3, Float3, Float3>> triangles;
//
//    float maxbX = 0;
//    float minbX = 0;
//    float maxbY = 0;
//    float minbY = 0;
//    {
//        for (int i = 0; i < points.size(); i++)
//        {
//            if (minbX > points[i].pos.x)
//            {
//                minbX = points[i].pos.x;
//            }
//            if (maxbX < points[i].pos.x)
//            {
//                maxbX = points[i].pos.x;
//            }
//            if (minbY > points[i].pos.y)
//            {
//                minbY = points[i].pos.y;
//            }
//            if (maxbY < points[i].pos.y)
//            {
//                maxbY = points[i].pos.y;
//            }
//        }
//    }
//    float bsize = ((maxbX - minbX) + (maxbY - minbY)) / 2.0f * 4.0f;
//
//    // 初期三角形を作成（仮の大きな三角形）
//    triangles.push_back({
//        {-bsize, -bsize}, {bsize, -bsize}, {0, bsize}
//    });
//
//    // 各点を追加して三角形分割を更新
//    for (const auto& p : points) 
//    {
//        std::vector<std::tuple<Float3, Float3, Float3>> newTriangles;
//        for (const auto& tri : triangles)
//        {
//            Float3 a, b, c;
//            std::tie(a, b, c) = tri;
//
//            if (inCircle(p.pos, a, b, c))
//            {
//                newTriangles.push_back({ a, p.pos, b });
//                newTriangles.push_back({ b, p.pos, c });
//                newTriangles.push_back({ c, p.pos, a });
//                //triangles.push_back({ a, p.pos, b });
//                //triangles.push_back({ b, p.pos, c });
//                //triangles.push_back({ c, p.pos, a });
//            }
//            else 
//            {
//                newTriangles.push_back(tri);
//                //triangles.push_back(tri);
//            }
//        }
//        triangles = newTriangles;
//    }
//
//    // 結果出力
//    std::vector<FloatTriangle> trianglesR;
//    {
//        int i = 0;
//        for (const auto& tri : triangles)
//        {
//            if (i > 0)
//            {
//                Float3 a, b, c;
//                std::tie(a, b, c) = tri;
//                trianglesR.push_back({ a, b, c });
//            }
//
//            i++;
//        }
//    }
//    return trianglesR;
//}



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

        DX_ModelDraw(GetModelData(v.modelNo), modelAnimDat, v.isTexNone ? nullptr : v.texture);
        //DX_ModelDraw(v.model);
    }
	else
	{
		//SetVertex(v.vertex);

        //if (v.vertex.type == DRAW_POLYGON_TYPE_UPDATE_VERTEX)
        //{
        //    //std::vector<Float3> vertexDataPosTmp = {};
        //    //for (int i = 0; i < v.vertex.vertexNum; i++)
        //    //{
        //    //    vertexDataPosTmp.push_back(v.vertex.vertex[i]);
        //    //}
        //    //vertexDataPosTmp = TriangulatePolygon(vertexDataPosTmp);

        //    std::vector<DPD_VERTEX_FRAGMENT> vertexDataPosTmp
        //        = v.vertex.vertex;

        //    std::sort(vertexDataPosTmp.begin(), vertexDataPosTmp.end(),
        //        // 比較関数: 左上順（x昇順、xが同じなら y昇順）
        //        [](const DPD_VERTEX_FRAGMENT& a, const DPD_VERTEX_FRAGMENT& b) {
        //            if (a.pos.x != b.pos.x) return a.pos.x < b.pos.x;  // x座標で昇順
        //            return a.pos.y < b.pos.y;                  // xが同じなら y座標で昇順
        //        }
        //    );


        //    //while (vertexDataPosTmp.size() % 3 != 0)
        //    //{
        //    //    vertexDataPosTmp.push_back(
        //    //        vertexDataPosTmp[vertexDataPosTmp.size() - 1]
        //    //    );
        //    //}
        //    vertexDataPosTmp
        //        //= polygonMonotoneTriangulation(v.vertex.vertex);
        //        = polygonTriangulatePolygonWithUV(vertexDataPosTmp);
        //    //= MonotoneTriangulation(vertexDataPosTmp);

        //    //Float3 minBounds = { 
        //    //    std::numeric_limits<float>::max(),
        //    //    std::numeric_limits<float>::max(),
        //    //    std::numeric_limits<float>::max() };
        //    //Float3 maxBounds = {
        //    //    std::numeric_limits<float>::lowest(), 
        //    //    std::numeric_limits<float>::lowest(),
        //    //    std::numeric_limits<float>::lowest() };

        //    //std::vector<FloatTriangle> triangles = bowyerWatson(
        //    //    vertexDataPosTmp
        //    //);
        //    //triangles = RemoveDuplicateTriangles(triangles);
        //    //if (triangles.size() <= 1)
        //    //{
        //    //    triangles = triangles;
        //    //}

        //    //vertexDataPosTmp.clear();
        //    //for (const auto& tri : triangles)
        //    //{
        //    //    DPD_VERTEX_FRAGMENT v0 = {
        //    //        tri.v0,
        //    //        {1.0f, 1.0f, 1.0f, 1.0f},
        //    //        polygonComputeUV(tri.v0, minBounds, maxBounds),
        //    //        {0.0f, 0.0f, 1.0f} };
        //    //    DPD_VERTEX_FRAGMENT v1 = {
        //    //        tri.v1,
        //    //        {1.0f, 1.0f, 1.0f, 1.0f},
        //    //        polygonComputeUV(tri.v1, minBounds, maxBounds),
        //    //        {0.0f, 0.0f, 1.0f} };
        //    //    DPD_VERTEX_FRAGMENT v2 = {
        //    //        tri.v2,
        //    //        {1.0f, 1.0f, 1.0f, 1.0f},
        //    //        polygonComputeUV(tri.v2, minBounds, maxBounds),
        //    //        {0.0f, 0.0f, 1.0f} };

        //    //    vertexDataPosTmp.push_back(v0);
        //    //    vertexDataPosTmp.push_back(v1);
        //    //    vertexDataPosTmp.push_back(v2);
        //    //}

        //    //{
        //    //    minBounds = minBounds;
        //    //}
        //    
        //    //std::vector<Vertex_DX> vertexDataTmp = {};
        //    //for (int i = 0; i < vertexDataPosTmp.size(); i++)
        //    //{
        //    //    Vertex_DX vdtmp = {};
        //    //    //vdtmp.pos.x = vertexDataPosTmp[i].x;
        //    //    //vdtmp.pos.y = vertexDataPosTmp[i].y;
        //    //    //vdtmp.pos.z = vertexDataPosTmp[i].z;
        //    //    //vdtmp.color = {1.0f, 1.0f, 1.0f, 1.0f};
        //    //    //vdtmp.texcoord = { 0.0f, 0.0f };
        //    //    //vdtmp.normal = { 0.0f, 1.0f, 0.0f };
        //    //    vdtmp.pos.x = vertexDataPosTmp[i].pos.x;
        //    //    vdtmp.pos.y = vertexDataPosTmp[i].pos.y;
        //    //    vdtmp.pos.z = vertexDataPosTmp[i].pos.z;
        //    //    vdtmp.color.x = vertexDataPosTmp[i].color.x;
        //    //    vdtmp.color.y = vertexDataPosTmp[i].color.y;
        //    //    vdtmp.color.z = vertexDataPosTmp[i].color.z;
        //    //    vdtmp.color.w = vertexDataPosTmp[i].color.w;
        //    //    vdtmp.texcoord.x = vertexDataPosTmp[i].texcoord.x;
        //    //    vdtmp.texcoord.y = vertexDataPosTmp[i].texcoord.y;
        //    //    vdtmp.normal.x = vertexDataPosTmp[i].normal.x;
        //    //    vdtmp.normal.y = vertexDataPosTmp[i].normal.y;
        //    //    vdtmp.normal.z = vertexDataPosTmp[i].normal.z;

        //    //    vertexDataTmp.push_back(vdtmp);
        //    //}

        //    D3D11_MAPPED_SUBRESOURCE mappedResource;
        //    g_DeviceContext->Map(g_updateVertex_structuredBuffer,
        //        0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
        //    //memcpy(mappedResource.pData,
        //    //    vertexDataTmp.data(),
        //    //    sizeof(Vertex_DX) * vertexDataTmp.size());
        //    memcpy(mappedResource.pData,
        //        vertexDataPosTmp.data(),
        //        sizeof(DPD_VERTEX_FRAGMENT) 
        //        * vertexDataPosTmp.size()
        //    );
        //    g_DeviceContext->Unmap(g_updateVertex_structuredBuffer, 0);

        //    // 頂点バッファ設定
        //    //UINT stride = sizeof(Vertex_DX);
        //    UINT stride = sizeof(DPD_VERTEX_FRAGMENT);
        //    UINT offset = 0;
        //    g_DeviceContext->IASetVertexBuffers(0, 1, 
        //        &g_updateVertex_structuredBuffer, &stride, &offset);

        //    // プリミティブトポロジ設定
        //    //g_DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
        //    g_DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

        //    // ポリゴン描画
        //    //g_DeviceContext->Draw(vertexDataTmp.size(), 0);
        //    g_DeviceContext->Draw(vertexDataPosTmp.size(), 0);
        //}
        //else 
            if (v.vertex.type == DRAW_POLYGON_TYPE_CUBE)
        {
            SetVertex_Cube();

            g_DeviceContext->IASetIndexBuffer(g_IndexBuffer_Cube, DXGI_FORMAT_R32_UINT, 0);

            // ポリゴン描画
            g_DeviceContext->DrawIndexed(NUM_VERTEX_CUBE, 0, 0);
        }
        else if(v.vertex.type == DRAW_POLYGON_TYPE_QUAD_INCETANCE)
        {
            SetVertex_Ex(v.vertex.type);

            // ポリゴン描画
            g_DeviceContext->DrawInstanced(
                NUM_VERTEX_SQUARE, 
                v.vertex.instanceNum,
                0, 0);
        }
        else
        {
            SetVertex_Ex(v.vertex.type);

            // ポリゴン描画
            g_DeviceContext->Draw(NUM_VERTEX_SQUARE, 0);
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
                //Float2 WSize = { SCREEN_WIDTH , SCREEN_HEIGHT };
                //{
                //    XMFLOAT2 vpsize = DXGetWindowSize();
                //    WSize.x = vpsize.x;
                //    WSize.y = vpsize.y;
                //}

				//座標変換
				XMMATRIX projection = XMMatrixOrthographicOffCenterLH(
                    0.0f,
                    SCREEN_WIDTH,
                    SCREEN_HEIGHT,
                    0.0f, 0.0f, 1.0f
                );

				worldViewProjection *= XMMatrixTranslation(
                    SCREEN_WIDTH / 2,
                    SCREEN_HEIGHT / 2,
                    0.0f);

				worldViewProjection *= projection;
			}

			worldViewProjection = XMMatrixTranspose(worldViewProjection);

			SHADER_VERTEX_SET_MATRIX_EX_DAT svsmdat = {};
			svsmdat.TexScale = { 1.0f, 1.0f };

			svsmdat.TexUV = { 0.0f, 0.0f };
			svsmdat.TexColor = { 1.0f, 1.0f, 1.0f, 1.0f };
			svsmdat.FixedColor = { -1.0f, -1.0f, -1.0f, -1.0f };

            //svsmdat.EnableShade = true;
            //svsmdat.ShaderType = DPD_SHADER_TYPE_NONE;

            svsmdat.ScreenSize = { SCREEN_WIDTH, SCREEN_HEIGHT };
            svsmdat.GameFrame = GetMainGameFrame();
            svsmdat.CameraPos = DX_GetCameraInfo(0).CameraPosRaw;

            svsmdat.Free = {
                g_ShaderFreeVariable_Custom_0.x,
                g_ShaderFreeVariable_Custom_0.y,
                g_ShaderFreeVariable_Custom_0.z,
                g_ShaderFreeVariable_Custom_0.w,
            };

			//if (g_ShaderIndex_Custom_0 > 0)
			//{
   //             DX_SetShader(g_ShaderIndex_Custom_0);
			//}
			//else
			//{
			//	DX_SetShader(g_ShaderIndex_Main);
			//}
            if (g_ShaderIndex_Custom_0.vs > 0)
            {
                DX_SetShaderVS(g_ShaderIndex_Custom_0.vs);
            }
            else
            {
                //DX_SetShaderVS(g_ShaderIndex_Main.vs);
                DX_SetShaderVS(g_ShaderIndex_Main[DPD_SHADER_TYPE_NONE].vs);
            }

            if (g_ShaderIndex_Custom_0.ps > 0)
            {
                DX_SetShaderPS(g_ShaderIndex_Custom_0.ps);
            }
            else
            {
                //DX_SetShaderPS(g_ShaderIndex_Main.ps);
                DX_SetShaderPS(g_ShaderIndex_Main[DPD_SHADER_TYPE_NONE].ps);
            }
            //DX_SetShaderVS(g_ShaderIndex_Main[DPD_SHADER_TYPE_NONE].vs);
            //DX_SetShaderPS(g_ShaderIndex_Main[DPD_SHADER_TYPE_NONE].ps);


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
            if(g_PolygonRelease_buffers_InBloom > 0)
			{
				ID3D11ShaderResourceView* refRes_BloomBuf = GetSRV_BloomBuf();
				if (refRes_BloomBuf != nullptr)
				{
                    //DX_SetSampler_OffScreen();
					SetBlendState_AddColor();
                    //DX_SetShader(g_ShaderIndex_Bloom);
                    DX_SetShaderVS(g_ShaderIndex_Bloom.vs);
                    DX_SetShaderPS(g_ShaderIndex_Bloom.ps);

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
                    //DX_SetSampler_Main();

					SetBlendState_Main();
					//DX_SetShader(g_ShaderIndex_Main);
                    DX_SetShaderVS(g_ShaderIndex_Main[DPD_SHADER_TYPE_NONE].vs);
                    DX_SetShaderPS(g_ShaderIndex_Main[DPD_SHADER_TYPE_NONE].ps);
				}
			}


            //{
            //    {
            //        SetBlendState_AddColor();
            //        DX_SetShader(g_ShaderIndex_Main);

            //        {
            //            SHADER_VERTEX_SET_MATRIX_EX_DAT svsmdat2 = svsmdat;
            //            svsmdat2.Transition.x = -0.01f;
            //            svsmdat2.TexColor = { 2.0f, 2.0f / 3, 2.0f / 3, 1.0f / 3 };
            //            polygonBufferRelease_post(
            //                refRes,
            //                worldViewProjection,
            //                svsmdat2,
            //                false
            //            );
            //        }
            //        {
            //            SHADER_VERTEX_SET_MATRIX_EX_DAT svsmdat2 = svsmdat;
            //            svsmdat2.TexColor = { 2.0f / 3, 2.0f, 2.0f / 3, 1.0f / 3 };
            //            polygonBufferRelease_post(
            //                refRes,
            //                worldViewProjection,
            //                svsmdat2,
            //                false
            //            );
            //        }
            //        {
            //            SHADER_VERTEX_SET_MATRIX_EX_DAT svsmdat2 = svsmdat;
            //            svsmdat2.Transition.x = 0.01f;
            //            svsmdat2.TexColor = { 2.0f / 3, 2.0f / 3, 2.0f, 1.0f / 3 };
            //            polygonBufferRelease_post(
            //                refRes,
            //                worldViewProjection,
            //                svsmdat2,
            //                false
            //            );
            //        }

            //        SetBlendState_Main();
            //    }
            //}

			//アウトライン

			//if (false)
            if(g_PolygonRelease_buffers_InOutLine > 0)
			{
                DX_SetSampler_OffScreen();
                //DX_SetShader(g_ShaderIndex_Outline);
                DX_SetShaderVS(g_ShaderIndex_Outline.vs);
                DX_SetShaderPS(g_ShaderIndex_Outline.ps);

                for (int i = 0; i < DX_OUTLINE_BUF_NUM; i++)
                {
                    ID3D11ShaderResourceView* refRes_OutlineBuf 
                        = GetSRV_OutlineBuf(i);
                    if (refRes_OutlineBuf != nullptr)
                    {
                        SHADER_VERTEX_SET_MATRIX_EX_DAT svsmdat2 = svsmdat;
                        svsmdat2.Free.x = g_globalOutlineWidth;
                        polygonBufferRelease_post(
                            refRes_OutlineBuf,
                            worldViewProjection,
                            svsmdat2,
                            false
                        );
                    }
                }
                DX_SetSampler_Main();

                //DX_SetShader(g_ShaderIndex_Main);
                DX_SetShaderVS(g_ShaderIndex_Main[DPD_SHADER_TYPE_NONE].vs);
                DX_SetShaderPS(g_ShaderIndex_Main[DPD_SHADER_TYPE_NONE].ps);
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

void polygonBufferRelease_SetOutlineColor(
    const POLYGON_RELEASE_BUFFER* v, 
    POLYGON_RELEASE_BUFFER& prb
)
{
    static constexpr XMFLOAT4 OUTLINE_COLORS[DPD_SHADER_OPT_OUTLINE_MAX] = {
        { 0.0f, 0.0f, 0.0f, 0.0f },
        { 0.8f, 0.8f, 0.9f, 1.0f },
        //{ 0.5f, 0.5f, 0.6f, 1.0f },

        { 1.0f, 1.0f, 1.0f, 1.0f },
        { 1.0f, 0.3f, 0.3f, 1.0f },
        { 0.2f, 0.3f, 1.0f, 1.0f },
        { 1.0f, 0.9f, 0.5f, 1.0f },
        { 0.6f, 1.0f, 0.6f, 1.0f },

        { 0.3f, 0.2f, 0.4f, 1.0f },
        { 0.09f, 0.02f, 0.09f, 1.0f },
        //{ 0.08f, 0.0f, 0.09f, 1.0f },
    };

    //prb2->svsmdat.FixedColor = OUTLINE_COLORS[v->shaderOpt.outline];
    if (v->shaderOpt.outline == DPD_SHADER_OPT_OUTLINE_NORMAL)
    {
        prb.svsmdat.FixedColor = { -1.0f, -1.0f, -1.0f, -1.0f };
    }
    else
    {
        prb.svsmdat.FixedColor = OUTLINE_COLORS[v->shaderOpt.outline];
    }

    //prb.svsmdat.Free.x = v->shaderOpt.outlineWidth;
}

void polygonBufferRelease_LayerRelease(
	const POLYGON_RELEASE_BUFFER* v, bool deSave, int i,
	bool force
)
{
	//drawCount++;

    POLYGON_RELEASE_BUFFER prb2 = *v;

	//シェーダ
	//DX_SetShader(g_ShaderIndex_Main);
    if (v->shaderType == DPD_SHADER_TYPE_EX)
    {
        DX_SetShaderVS(v->shaderTypeEx.vs);
        DX_SetShaderPS(v->shaderTypeEx.ps);
        prb2.svsmdat.Free = {
            v->shaderTypeEx.free.x,
            v->shaderTypeEx.free.y,
            v->shaderTypeEx.free.z,
            v->shaderTypeEx.free.w,
        };
    }
    else
    {
        DX_SetShaderVS(g_ShaderIndex_Main[v->shaderType].vs);
        DX_SetShaderPS(g_ShaderIndex_Main[v->shaderType].ps);
    }
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

		polygonBufferRelease_parts(prb2, deSave, i);
	}
	else
	{
		//オフスクに書き込む
		SetRenderTargetView_OffScreen(); //レンダーターゲットビュー
		//DX_SetSampler_OffScreen();
        DX_SetSampler_Main();

		polygonBufferRelease_parts(prb2, deSave, i);

		{ //エフェクト系の処理
			if (!v->depthBuffIgnore && deSave)
			{ //有効だったら一時的に無効化する
				DXSetDepthStencilStateDepth(false);
			}

			if (v->shaderOpt.bloom)
			{
                g_PolygonRelease_buffers_InBloom++;

				//シェーダ
				//DX_SetShader(g_ShaderIndex_Main);
                DX_SetShaderVS(g_ShaderIndex_Main[v->shaderType].vs);
                DX_SetShaderPS(g_ShaderIndex_Main[v->shaderType].ps);
				SetRenderTargetView_BloomBuf(); //レンダーターゲットビュー
				//SetRenderTargetView_OffScreen(); //レンダーターゲットビュー

                //POLYGON_RELEASE_BUFFER prb2 = *v;

                //{
                //    prb2.svsmdat.FixedColor = BLOOM_COLORS[v->shaderOpt.bloom];
                //}

                //POLYGON_RELEASE_BUFFER* prb2 = new POLYGON_RELEASE_BUFFER;
                //*prb2 = *v;

                if(v->shaderOpt.bloom == DPD_SHADER_OPT_BLOOM_ALWAYS)
                {
                    //prb2->svsmdat.FixedColor = { 2.0f, 2.0f, 2.0f, 1.0f };
                    prb2.svsmdat.FixedColor = { 2.0f, 2.0f, 2.0f, 1.0f };
                }

                //polygonBufferRelease_parts(*prb2, deSave, i);
                polygonBufferRelease_parts(prb2, deSave, i);
                //polygonBufferRelease_parts(*v, deSave, i);

                //delete prb2;
			}
			if (v->shaderOpt.outline)
			{
                g_PolygonRelease_buffers_InOutLine++;

				//シェーダ
				//DX_SetShader(g_ShaderIndex_OutlineBuf);
                DX_SetShaderVS(g_ShaderIndex_OutlineBuf.vs);
                DX_SetShaderPS(g_ShaderIndex_OutlineBuf.ps);
				//if (v->shaderOpt.outline)
				//{
				//	DX_SetShader_OutlineBuf();
				//}
				//else
				//{
				//	DX_SetShader_OutlineBuf2();
				//}

				SetRenderTargetView_OutlineBuf(v->shaderOpt.outlineLayer); //レンダーターゲットビュー
				//SetRenderTargetView_OffScreen(); //レンダーターゲットビュー

                //POLYGON_RELEASE_BUFFER* prb2 = new POLYGON_RELEASE_BUFFER;
                //*prb2 = *v;
                polygonBufferRelease_SetOutlineColor(
                    v,
                    prb2
                );

                //polygonBufferRelease_parts(*prb2, deSave, i);
                polygonBufferRelease_parts(prb2, deSave, i);

                //delete prb2;
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

void polygonBufferReleaseTask()
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
                if (plyRelease_buffers_Size[i] <= 0)
                {
                    continue;
                }
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
            if (plyRelease_buffers_Size[i] > 0)
            {
                for (int j = 0; j < plyRelease_buffers_Size[i]; j++)
                {
                    polygonBufferRelease_LayerRelease(g_PlyRelease_buffers_Buf[i][j], deSave, i, false);
                }
            }
            if (g_PolygonSRVDrawBuf.size() > 0)
            {
                for (auto v : g_PolygonSRVDrawBuf)
                {
                    if (v.priority == i)
                    {
                        //PolygonSRVDraw(v.srv,
                        //    v.posIn,
                        //    v.rotIn,
                        //    v.scaleIn,
                        //    v.shaderIndex
                        //);
                        PolygonSRVDraw(v.srv,
                            v.posIn,
                            v.rotIn,
                            v.scaleIn,
                            v.shaderIndexVS,
                            v.shaderIndexPS
                        );
                    }
                }
            }
		}

		//メインスクリーンに描画
		polygonBufferRelease_MainScreen();

        //ワールドエフェクトを描画
        //polygonDraw_WorldEffect_LightingBug();

		//最前面レイヤーをリリースする
		for (int i = DRAW_POLYGON_PRIORITY_FORCE_START; i < DRAW_POLYGON_PRIORITY_FORCE_END + 1; i++)
		{
            //for (const POLYGON_RELEASE_BUFFER* v : g_PlyRelease_buffers_Buf[i])
            //{
            //    polygonBufferRelease_LayerRelease(v, deSave, i, true);
            //}
            if (plyRelease_buffers_Size[i] > 0)
            {
                for (int j = 0; j < plyRelease_buffers_Size[i]; j++)
                {
                    polygonBufferRelease_LayerRelease(g_PlyRelease_buffers_Buf[i][j], deSave, i, true);
                }
            }
            if (g_PolygonSRVDrawBuf.size() > 0)
            {
                for (auto v : g_PolygonSRVDrawBuf)
                {
                    if (v.priority == i)
                    {
                        //PolygonSRVDraw(v.srv,
                        //    v.posIn,
                        //    v.rotIn,
                        //    v.scaleIn,
                        //    v.shaderIndex
                        //);
                        PolygonSRVDraw(v.srv,
                            v.posIn,
                            v.rotIn,
                            v.scaleIn,
                            v.shaderIndexVS,
                            v.shaderIndexPS
                        );
                    }
                }
            }
		}
	}

	//バッファをクリア
    
	//for (int i = 0; i < g_PolygonRelease_buffers.size(); i++)
	//{
	//	g_PolygonRelease_buffers[i].vertex.vertex.clear();
	//	g_PolygonRelease_buffers[i].vertex.vertex.shrink_to_fit();
	//}

    g_PolygonRelease_buffers_InOutLine = 0;
    g_PolygonRelease_buffers_InBloom = 0;

	g_PolygonRelease_buffers.clear();
    //g_PolygonRelease_buffers.shrink_to_fit();

    g_PolygonSRVDrawBuf.clear();
    //g_PolygonSRVDrawBuf.shrink_to_fit();
    //PolygonBufferReleaseOnly();

	//for (int i = 0; i < POLYGON_RELEASE_BUFFER_MAX; i++)
	//{
	//	g_PolygonRelease_buffers_array[i] = {};
	//}

}
void PolygonBufferReleaseOnly(void)
{
    g_PolygonRelease_buffers_InOutLine = 0;
    g_PolygonRelease_buffers_InBloom = 0;

    g_PolygonRelease_buffers.clear();
    //g_PolygonRelease_buffers.shrink_to_fit();

    
    g_PolygonSRVDrawBuf.clear();
    //g_PolygonSRVDrawBuf.shrink_to_fit();
}
void PolygonBufferRelease()
{
    //g_Textures[i].loadtask = 
    //std::async(
    //    std::launch::async,
    //    polygonBufferReleaseTask
    //);

    polygonBufferReleaseTask();
}

void PolygonMainBufRelease()
{

    //最終出力
    {
        ID3D11ShaderResourceView* srv
            = GetSRV_MainDummyBuf();
        if (srv != nullptr)
        {
            bool deSave = DXDepthStencilStateDepthEnable();

            if (deSave)
            { //有効だったら一時的に無効化する
                DXSetDepthStencilStateDepth(false);
            }

            SetVertex_Quad();

            //マトリックス設定
            XMMATRIX worldViewProjection = {};

            SHADER_VERTEX_SET_MATRIX_EX_DAT svsmdat = {};
            svsmdat.TexScale = { 1.0f, 1.0f };
            svsmdat.TexUV = { 0.0f, 0.0f };
            svsmdat.TexColor = { 1.0f, 1.0f, 1.0f, 1.0f };
            svsmdat.FixedColor = { -1.0f, -1.0f, -1.0f, -1.0f };

            //svsmdat.EnableShade = true;
            //svsmdat.ShaderType = DPD_SHADER_TYPE_NONE;

            svsmdat.ScreenSize = { SCREEN_WIDTH, SCREEN_HEIGHT };
            svsmdat.GameFrame = GetMainGameFrame();

            {
                {
                    XMMATRIX pos = XMMatrixTranslation(0.0f, 0.0f, 0.0f);
                    XMMATRIX rot = XMMatrixRotationRollPitchYaw(0.0f, 0.0f, 0.0f);

                    Float2 scaleraw = {
                        +SCREEN_WIDTH,
                        SCREEN_HEIGHT,
                    };
                    {
                        XMFLOAT2 vpsize = DXGetWindowSize();

                        float aspectRatioWindow = (float)vpsize.x / vpsize.y; // ウィンドウアスペクト比
                        float aspectRatioBase = (float)SCREEN_WIDTH / SCREEN_HEIGHT; // 固定アスペクト比

                        if (aspectRatioWindow > aspectRatioBase)
                        {
                            scaleraw.x = +SCREEN_WIDTH * 1.0f;
                            scaleraw.y = SCREEN_HEIGHT * (((float)SCREEN_HEIGHT / SCREEN_WIDTH) / ((float)vpsize.y / vpsize.x));
                        }
                        else
                        {
                            scaleraw.x = +SCREEN_WIDTH * (aspectRatioBase / aspectRatioWindow);
                            scaleraw.y = SCREEN_HEIGHT * 1.0f;
                        }
                    }

                    XMMATRIX scale = XMMatrixScaling(
                        scaleraw.x,
                        scaleraw.y,
                        1.0f
                    );

                    worldViewProjection = scale * rot * pos;
                }

                {
                    //座標変換
                    XMMATRIX projection = XMMatrixOrthographicOffCenterLH(
                        0.0f,
                        +SCREEN_WIDTH,
                        SCREEN_HEIGHT,
                        0.0f, 0.0f, 1.0f);

                    worldViewProjection *= XMMatrixTranslation(
                        +SCREEN_WIDTH / 2,
                        SCREEN_HEIGHT / 2,
                        0.0f);

                    worldViewProjection *= projection;
                }

                worldViewProjection = XMMatrixTranspose(worldViewProjection);

                SHADER_VERTEX_SET_MATRIX_EX_DAT svsmdat2 = svsmdat;
                svsmdat2.TexColor = { 1.0f, 1.0f, 1.0f, 0.45f };
                //DX_SetShader(shaderIndex);
                DX_SetShaderVS(g_ShaderIndex_Main[DPD_SHADER_TYPE_BLUR].vs);
                DX_SetShaderPS(g_ShaderIndex_Main[DPD_SHADER_TYPE_BLUR].ps);
                SetBlendState_Main();
                //SetBlendState_AddColor();

                //メインスクリーンに書き込む
                SetRenderTargetView_Final();
                //DX_SetSampler_Main();
                DX_SetSampler_Final();

                polygonBufferRelease_post(
                    srv,
                    worldViewProjection,
                    svsmdat2,
                    false
                );
            }

            {
                {
                    XMMATRIX pos = XMMatrixTranslation(0.0f, 0.0f, 0.0f);
                    XMMATRIX rot = XMMatrixRotationRollPitchYaw(0.0f, 0.0f, 0.0f);

                    Float2 scaleraw = {
                        +SCREEN_WIDTH,
                        SCREEN_HEIGHT,
                    };
                    {
                        XMFLOAT2 vpsize = DXGetWindowSize();

                        float aspectRatioWindow = (float)vpsize.x / vpsize.y; // ウィンドウアスペクト比
                        float aspectRatioBase = (float)SCREEN_WIDTH / SCREEN_HEIGHT; // 固定アスペクト比

                        if (aspectRatioWindow > aspectRatioBase)
                        {
                            scaleraw.x = +SCREEN_WIDTH * (aspectRatioBase / aspectRatioWindow);
                            scaleraw.y = SCREEN_HEIGHT * 1.0f;
                        }
                        else
                        {
                            scaleraw.x = +SCREEN_WIDTH * 1.0f;
                            scaleraw.y = SCREEN_HEIGHT * (((float)SCREEN_HEIGHT / SCREEN_WIDTH) / ((float)vpsize.y / vpsize.x));
                        }
                    }

                    XMMATRIX scale = XMMatrixScaling(
                        scaleraw.x,
                        scaleraw.y,
                        1.0f
                    );

                    worldViewProjection = scale * rot * pos;
                }

                {
                    //座標変換
                    XMMATRIX projection = XMMatrixOrthographicOffCenterLH(
                        0.0f,
                        +SCREEN_WIDTH,
                        SCREEN_HEIGHT,
                        0.0f, 0.0f, 1.0f);

                    worldViewProjection *= XMMatrixTranslation(
                        +SCREEN_WIDTH / 2,
                        SCREEN_HEIGHT / 2,
                        0.0f);

                    worldViewProjection *= projection;
                }

                worldViewProjection = XMMatrixTranspose(worldViewProjection);

                //DX_SetShader(shaderIndex);
                DX_SetShaderVS(g_ShaderIndex_Main[DPD_SHADER_TYPE_NONE].vs);
                DX_SetShaderPS(g_ShaderIndex_Main[DPD_SHADER_TYPE_NONE].ps);
                SetBlendState_Main();
                //SetBlendState_AddColor();

                //メインスクリーンに書き込む
                SetRenderTargetView_Final();
                //DX_SetSampler_Main();
                DX_SetSampler_Final();

                polygonBufferRelease_post(
                    srv,
                    worldViewProjection,
                    svsmdat,
                    true
                );
            }

            SetRenderTargetView_Main();
            DX_SetSampler_Main();

            if (deSave)
            { //深度ステートをもとに戻す
                DXSetDepthStencilStateDepth(deSave);
            }
        }
    }
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

void PolygonSRVDraw(POLYGON_SRV_BUF buf)
{
    g_PolygonSRVDrawBuf.push_back(buf);
}


void SetPostEffectShaderVariable(float value, int index)
{
    if (index < 0 || index >= ARRAYSIZE(g_ShaderFreeVariable_Custom_0.v)) return;
    g_ShaderFreeVariable_Custom_0.v[index] = value;
}
void SetPostEffectShaderVariable(Float4 value)
{
    g_ShaderFreeVariable_Custom_0 = value;
}
Float4  GetPostEffectShaderVariable()
{
    return g_ShaderFreeVariable_Custom_0;
}


//
//void SetPostEffectShader(std::string name)
//{
//	if (name.size() <= 0)
//	{
//		g_ShaderIndex_Custom_0 = -1;
//	}
//	else
//	{
//		g_ShaderIndex_Custom_0 = DX_GetShaderIndex(name);
//	}
//}
//void SetPostEffectShader(int index)
//{
//    g_ShaderIndex_Custom_0 = index;
//}
//
//int GetPostEffectShader()
//{
//    return g_ShaderIndex_Custom_0;
//}
void SetPostEffectShaderPS(std::string name)
{
    if (name.size() <= 0)
    {
        g_ShaderIndex_Custom_0.ps = -1;
    }
    else
    {
        g_ShaderIndex_Custom_0.ps = DX_GetShaderIndex(name);
    }
}
void SetPostEffectShaderPS(int index)
{
    g_ShaderIndex_Custom_0.ps = index;
}

int GetPostEffectShaderPS()
{
    return g_ShaderIndex_Custom_0.ps;
}
void SetPostEffectShaderVS(std::string name)
{
	if (name.size() <= 0)
	{
		g_ShaderIndex_Custom_0.vs = -1;
	}
	else
	{
		g_ShaderIndex_Custom_0.vs = DX_GetShaderIndex(name);
	}
}
void SetPostEffectShaderVS(int index)
{
    g_ShaderIndex_Custom_0.vs = index;
}

int GetPostEffectShaderVS()
{
    return g_ShaderIndex_Custom_0.vs;
}

float GetPostEffectGlobalOutlineWidth()
{
    return g_globalOutlineWidth;
}

void SetPostEffectGlobalOutlineWidth(float width)
{
    g_globalOutlineWidth = width;
}
void ResetPostEffectGlobalOutlineWidth()
{
    g_globalOutlineWidth = GLOBAL_OUTLINE_WIDTH_DEFAULT;
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

        //svsmdat.EnableShade = true;
        //svsmdat.ShaderType = DPD_SHADER_TYPE_NONE;

        svsmdat.ScreenSize = { SCREEN_WIDTH, SCREEN_HEIGHT };
        svsmdat.GameFrame = GetMainGameFrame();

        //DX_SetShader(g_ShaderIndex_Main);
        SetBlendState_Main();

        //メインスクリーンに書き込む
        SetRenderTargetView_Main();
        DX_SetSampler_Main();
        //DX_SetSampler_OffScreen();

        for (int i = 0; i < DPD_SHADER_TYPE_MAX; i++)
        {
            DX_SetShaderVS(g_ShaderIndex_Main[i].vs);
            DX_SetShaderPS(g_ShaderIndex_Main[i].ps);

            polygonBufferRelease_post(
                refRes,
                worldViewProjection,
                svsmdat,
                true
            );
        }

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
        FileLoadAddBefore(1, FILELOADADDBEFORE_TXT_CACHE, FILELOADADD_TYPE_NORMAL);
        PolygonTextureCache(id);
        //FileLoadAddBefore(0, FILELOADADDBEFORE_TXT_CACHE, FILELOADADD_TYPE_FINAL);

        FileLoadAdd(1, FILELOADADD_TXT_CACHE, FILELOADADD_TYPE_NORMAL);
    }
}

//void PolygonDrawLoadProgress(size_t cur, size_t max)
//{
//    //ID3D11ShaderResourceView* refRes = g_GenralSRV;
//
//    //if (refRes == nullptr) return;
//
//    bool deSave = DXDepthStencilStateDepthEnable();
//
//    if (deSave)
//    { //有効だったら一時的に無効化する
//        DXSetDepthStencilStateDepth(false);
//    }
//
//    SetVertex_Quad();
//
//    //マトリックス設定
//    XMMATRIX worldViewProjection = {};
//
//    SHADER_VERTEX_SET_MATRIX_EX_DAT svsmdat = {};
//    svsmdat.TexScale = { 1.0f, 1.0f };
//    svsmdat.TexUV = { 0.0f, 0.0f };
//    svsmdat.TexColor = { 1.0f, 1.0f, 1.0f, 1.0f };
//    svsmdat.FixedColor = { 1.0f, 1.0f, 1.0f, 1.0f };
//
//    //svsmdat.EnableShade = true;
//    //svsmdat.ShaderType = DPD_SHADER_TYPE_NONE;
//
//    svsmdat.ScreenSize = { SCREEN_WIDTH, SCREEN_HEIGHT };
//    svsmdat.GameFrame = GetMainGameFrame();
//    svsmdat.NoAlpha = true;
//
//    {
//        {
//            XMMATRIX rot = XMMatrixRotationRollPitchYaw(0.0f, 0.0f, 0.0f);
//
//            Float2 scaleraw = {
//                +SCREEN_WIDTH,
//                SCREEN_HEIGHT,
//            };
//            {
//                XMFLOAT2 vpsize = DXGetWindowSize();
//
//                float aspectRatioWindow = (float)vpsize.x / vpsize.y; // ウィンドウアスペクト比
//                float aspectRatioBase = (float)SCREEN_WIDTH / SCREEN_HEIGHT; // 固定アスペクト比
//
//                if (aspectRatioWindow > aspectRatioBase)
//                {
//                    scaleraw.x = +SCREEN_WIDTH * (aspectRatioBase / aspectRatioWindow);
//                    scaleraw.y = SCREEN_HEIGHT * 1.0f;
//                }
//                else
//                {
//                    scaleraw.x = +SCREEN_WIDTH * 1.0f;
//                    scaleraw.y = SCREEN_HEIGHT * (((float)SCREEN_HEIGHT / SCREEN_WIDTH) / ((float)vpsize.y / vpsize.x));
//                }
//            }
//
//            float progressMod = (float)cur / max;
//
//            XMMATRIX scale = XMMatrixScaling(
//                scaleraw.x * 0.8f * progressMod,
//                scaleraw.y * 0.05f,
//                1.0f
//            );
//            XMMATRIX pos = XMMatrixTranslation(
//                scaleraw.x * 0.8f / -2.0f * (1.0f - progressMod),
//                0.0f, 0.0f);
//
//            worldViewProjection = scale * rot * pos;
//        }
//
//        {
//            //座標変換
//            XMMATRIX projection = XMMatrixOrthographicOffCenterLH(
//                0.0f,
//                +SCREEN_WIDTH,
//                SCREEN_HEIGHT,
//                0.0f, 0.0f, 1.0f);
//
//            worldViewProjection *= XMMatrixTranslation(
//                +SCREEN_WIDTH / 2,
//                SCREEN_HEIGHT / 2,
//                0.0f);
//
//            worldViewProjection *= projection;
//        }
//
//        worldViewProjection = XMMatrixTranspose(worldViewProjection);
//
//        //DX_SetShader(shaderIndex);
//        DX_SetShaderVS(g_ShaderIndex_Main[DPD_SHADER_TYPE_NONE].vs);
//        DX_SetShaderPS(g_ShaderIndex_Main[DPD_SHADER_TYPE_NONE].ps);
//        SetBlendState_Main();
//
//        //メインスクリーンに書き込む
//        SetRenderTargetView_Final();
//        DX_SetSampler_Main();
//
//
//        //polygonBufferRelease_post(
//        //    refRes,
//        //    worldViewProjection,
//        //    svsmdat,
//        //    true
//        //);
//
//        XMFLOAT4X4 matrix;
//        DirectX::XMStoreFloat4x4(&matrix, worldViewProjection);
//
//        // 頂点シェーダの定数バッファに行列を送信
//
//        //送信
//        DX_ShaderVertexSetMatrixEx(&matrix, svsmdat);
//
//        //g_DeviceContext->PSSetShaderResources(0, 1, &refRes);
//        //g_DeviceContext->PSSetShaderResources(0, 1, nullptr);
//
//
//        SetBlendState_Main();
//        // ポリゴン描画
//
//        g_DeviceContext->Draw(NUM_VERTEX_SQUARE, 0);
//    }
//
//    SetRenderTargetView_Main();
//
//    if (deSave)
//    { //深度ステートをもとに戻す
//        DXSetDepthStencilStateDepth(deSave);
//    }
//}


//void PolygonSRVDraw(ID3D11ShaderResourceView* srv,
//    Float3 posIn,
//    Float3 rotIn,
//    Float3 scaleIn,
//    int shaderIndex
//)
void PolygonSRVDraw(ID3D11ShaderResourceView* srv,
    Float3 posIn,
    Float3 rotIn,
    Float3 scaleIn,
    int shaderIndexVS,
    int shaderIndexPS
)
{
    if (srv != nullptr)
    {
        bool deSave = DXDepthStencilStateDepthEnable();

        if (deSave)
        { //有効だったら一時的に無効化する
            DXSetDepthStencilStateDepth(false);
        }

        SetVertex_Quad();

        //マトリックス設定
        XMMATRIX worldViewProjection = {};

        {
            XMMATRIX pos = XMMatrixTranslation(posIn.x, posIn.y, posIn.z);
            XMMATRIX rot = XMMatrixRotationRollPitchYaw(rotIn.x, rotIn.y, rotIn.z);
            XMMATRIX scale = XMMatrixScaling(
                scaleIn.x,
                scaleIn.y,
                scaleIn.z
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

        //svsmdat.EnableShade = true;
        //svsmdat.ShaderType = DPD_SHADER_TYPE_NONE;

        svsmdat.ScreenSize = { SCREEN_WIDTH, SCREEN_HEIGHT };
        svsmdat.GameFrame = GetMainGameFrame();

        //DX_SetShader(shaderIndex);
        DX_SetShaderVS(shaderIndexVS);
        DX_SetShaderPS(shaderIndexPS);
        SetBlendState_Main();

        //メインスクリーンに書き込む
        SetRenderTargetView_Main();
        DX_SetSampler_Main();

        polygonBufferRelease_post(
            srv,
            worldViewProjection,
            svsmdat,
            false
        );

        SetBlendState_Main();
        //DX_SetShader(g_ShaderIndex_Main);
        DX_SetShaderVS(g_ShaderIndex_Main[DPD_SHADER_TYPE_NONE].vs);
        DX_SetShaderPS(g_ShaderIndex_Main[DPD_SHADER_TYPE_NONE].ps);

        if (deSave)
        { //深度ステートをもとに戻す
            DXSetDepthStencilStateDepth(deSave);
        }
    }
}

//void polygonDraw_WorldEffect_LightingBug()
//{
//
//    //深度設定を保持
//    bool deSave = DXDepthStencilStateDepthEnable();
//
//    DXSetDepthStencilStateDepth(false);
//
//    // 頂点バッファ設定
//    UINT stride = sizeof(Vertex_DX);
//    UINT offset = 0;
//    g_DeviceContext->IASetVertexBuffers(0, 1, &g_VertexBuffer_LightingBug, &stride, &offset);
//
//    //SetVertex_Quad();
//
//    // プリミティブトポロジ設定
//    //g_DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
//    g_DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);
//
//    //DX_SetShaderVS(g_ShaderIndex_Main[DPD_SHADER_TYPE_NONE].vs);
//    //DX_SetShaderPS(g_ShaderIndex_Main[DPD_SHADER_TYPE_NONE].ps);
//    DX_SetShaderVS(g_ShaderIndex_LightingBug.vs);
//    DX_SetShaderPS(g_ShaderIndex_LightingBug.ps);
//    DX_SetShaderGS(g_ShaderIndex_LightingBug.gs);
//
//
//    DX_CAMERA_INFO cameraInfo = DX_GetCameraInfo(0);
//
//    //マトリックス設定
//    XMMATRIX worldViewProjectionRaw = {};
//    XMMATRIX worldViewProjection = {};
//
//    {
//        XMMATRIX pos = XMMatrixTranslation(0.0f, 0.0f, 0.0f);
//        XMMATRIX rot = XMMatrixRotationRollPitchYaw(0.0f, 0.0f, 0.0f);
//        XMMATRIX scale = XMMatrixScaling(
//            1.0f,
//            1.0f,
//            1.0f
//        );
//
//        worldViewProjectionRaw = scale * rot * pos;
//    }
//
//    worldViewProjectionRaw *= cameraInfo.CameraScaling;
//
//    worldViewProjectionRaw *= cameraInfo.CameraView;
//
//    worldViewProjectionRaw *= cameraInfo.CameraPos;
//
//    worldViewProjection = XMMatrixTranspose(worldViewProjectionRaw);
//
//    SHADER_VERTEX_SET_MATRIX_EX_DAT svsmdat = {};
//    svsmdat.TexScale = { 1.0f, 1.0f };
//    svsmdat.TexUV = { 0.0f, 0.0f };
//    svsmdat.TexColor = { 1.0f, 1.0f, 1.0f, 1.0f };
//    svsmdat.FixedColor = { -1.0f, -1.0f, -1.0f, -1.0f };
//
//    svsmdat.ScreenSize = { SCREEN_WIDTH, SCREEN_HEIGHT };
//    svsmdat.GameFrame = GetMainGameFrame();
//
//    //
//    svsmdat.World = worldViewProjectionRaw;
//    svsmdat.View = cameraInfo.CameraView;
//    svsmdat.Projection = cameraInfo.CameraPos;
//    //
//
//    svsmdat.CameraPos = cameraInfo.CameraPosRaw;
//
//    svsmdat.Free.x = 10.0f;
//    svsmdat.Free.y = 10.0f;
//
//
//    XMFLOAT4X4 matrix;
//    DirectX::XMStoreFloat4x4(&matrix, worldViewProjection);
//
//    //送信
//    DX_ShaderVertexSetMatrixEx(&matrix, svsmdat);
//
//    ID3D11ShaderResourceView* refRes = GetTexture_DX(0);
//    g_DeviceContext->PSSetShaderResources(0, 1, &refRes);
//    //g_DeviceContext->PSSetShaderResources(0, 0, nullptr);
//
//
//    //メインスクリーンに書き込む
//    SetRenderTargetView_Main();
//    DX_SetSampler_Main();
//
//    SetBlendState_Main();
//    // ポリゴン描画
//
//    g_DeviceContext->Draw(10000, 0);
//
//    DX_SetShaderGS(-1);
//
//    if (deSave)
//    { //深度ステートをもとに戻す
//        DXSetDepthStencilStateDepth(deSave);
//    }
//}