#pragma once

#undef min
#undef max

#include <unordered_map>

#include "assimp/cimport.h"
#include "assimp/scene.h"
#include "assimp/postprocess.h"
#include "assimp/matrix4x4.h"
#pragma comment (lib, "assimp-vc143-mt.lib")

#include "dx_shader.h"

//struct DX_MODEL;


struct DX_MODEL_ANIM {
    int frame;
    XMMATRIX mat;
    SHADER_VERTEX_SET_MATRIX_EX_DAT svsmdat;
};
struct DX_MODEL
{
	const aiScene* AiScene = nullptr;

	ID3D11Buffer** VertexBuffer = nullptr;
	ID3D11Buffer** IndexBuffer = nullptr;

	std::unordered_map<std::string, ID3D11ShaderResourceView*> Texture;

	//~DX_MODEL()
	//{
	//	DX_ModelRelease(this);
	//}

};

void DX_ModelInitialize();
void DX_ModelFinalize();

DX_MODEL* DX_ModelLoad(const char* FileName, bool isBr);
void DX_ModelRelease(DX_MODEL* model);


void DX_ModelDraw(DX_MODEL* model, DX_MODEL_ANIM animdat, ID3D11ShaderResourceView* exSRV = nullptr);
