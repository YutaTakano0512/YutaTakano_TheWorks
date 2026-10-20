

#undef min
#undef max

#include <d3d11.h>
#include <DirectXMath.h>
#include "../../../DirectXTex.h"
using namespace DirectX;

#include "../../Game/Systems/sys_general.h"

#include "dx.h"
#include "dx_model.h"




void DX_ModelInitialize()
{
    //SetDllDirectory("rom\\lib"); // DLLの検索パスを変更
    //HMODULE hModule = LoadLibrary("assimp-vc143-mt.dll"); // DLLをロード

    //if (hModule) {
    //    MessageBox(NULL, "DLLロード成功", "成功", MB_OK);
    //    FreeLibrary(hModule);
    //}
    //else {
    //    MessageBox(NULL, "DLLロード失敗", "エラー", MB_OK);
    //}
}

void DX_ModelFinalize()
{
}

DX_MODEL* DX_ModelLoad(const char* FileName, bool isBr)
{
	DX_MODEL* model = new DX_MODEL;


	const std::string modelPath( FileName );

    int flag = 0;
    flag |= aiProcessPreset_TargetRealtime_MaxQuality;
    flag |= aiProcess_ConvertToLeftHanded;
    flag |= aiProcess_Triangulate;
    //flag |= aiProcess_SortByPType;
    //flag |= aiProcess_GenNormals; 
    //flag |= aiProcess_OptimizeMeshes;
    
    //flag |= aiProcess_CalcTangentSpace;
    //flag |= aiProcess_Triangulate;
    //flag |= aiProcess_GenSmoothNormals;
    //flag |= aiProcess_PreTransformVertices;
    //flag |= aiProcess_RemoveRedundantMaterials;
    //flag |= aiProcess_FlipUVs;// モデル表示の際、モデルに応じてUV反転する

	model->AiScene = aiImportFile(FileName, flag);
	assert(model->AiScene);

	model->VertexBuffer = new ID3D11Buffer*[model->AiScene->mNumMeshes];
	model->IndexBuffer = new ID3D11Buffer*[model->AiScene->mNumMeshes];


	for (unsigned int m = 0; m < model->AiScene->mNumMeshes; m++)
	{
		aiMesh* mesh = model->AiScene->mMeshes[m];

		// 頂点バッファ生成
		{
			Vertex_DX* vertex = new Vertex_DX[mesh->mNumVertices];

			for (unsigned int v = 0; v < mesh->mNumVertices; v++)
			{
				if (isBr)
				{
					vertex[v].pos = XMFLOAT3(mesh->mVertices[v].x, -mesh->mVertices[v].z, mesh->mVertices[v].y);
					vertex[v].texcoord = XMFLOAT2(mesh->mTextureCoords[0][v].x, mesh->mTextureCoords[0][v].y);
					vertex[v].color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
					vertex[v].normal = XMFLOAT3(mesh->mNormals[v].x, -mesh->mNormals[v].z, mesh->mNormals[v].y);
				}
				else
				{
					vertex[v].pos = XMFLOAT3(mesh->mVertices[v].x, mesh->mVertices[v].y, mesh->mVertices[v].z);
					vertex[v].texcoord = XMFLOAT2(mesh->mTextureCoords[0][v].x, mesh->mTextureCoords[0][v].y);
					vertex[v].color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
					//vertex[v].normal = XMFLOAT3(mesh->mNormals[v].x, mesh->mNormals[v].y, mesh->mNormals[v].z);
					vertex[v].normal = XMFLOAT3(mesh->mNormals[v].x, -mesh->mNormals[v].z, mesh->mNormals[v].y);
				}
			}

			D3D11_BUFFER_DESC bd;
			ZeroMemory(&bd, sizeof(bd));
			bd.Usage = D3D11_USAGE_DYNAMIC;
			bd.ByteWidth = sizeof(Vertex_DX) * mesh->mNumVertices;
			bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
			bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

			D3D11_SUBRESOURCE_DATA sd;
			ZeroMemory(&sd, sizeof(sd));
			sd.pSysMem = vertex;

			DXGetDevice()->CreateBuffer(&bd, &sd, &model->VertexBuffer[m]);

			delete[] vertex;

		}


		// インデックスバッファ生成
		{
			unsigned int* index = new unsigned int[mesh->mNumFaces * 3];

			for (unsigned int f = 0; f < mesh->mNumFaces; f++)
			{
				const aiFace* face = &mesh->mFaces[f];

				//assert(face->mNumIndices == 3);
                if (face->mNumIndices < 3)
                {
                    continue;
                }

				index[f * 3 + 0] = face->mIndices[0];
				index[f * 3 + 1] = face->mIndices[1];
				index[f * 3 + 2] = face->mIndices[2];
			}

			D3D11_BUFFER_DESC bd;
			ZeroMemory(&bd, sizeof(bd));
			bd.Usage = D3D11_USAGE_DEFAULT;
			bd.ByteWidth = sizeof(unsigned int) * mesh->mNumFaces * 3;
			bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
			bd.CPUAccessFlags = 0;

			D3D11_SUBRESOURCE_DATA sd;
			ZeroMemory(&sd, sizeof(sd));
			sd.pSysMem = index;

			DXGetDevice()->CreateBuffer(&bd, &sd, &model->IndexBuffer[m]);

			delete[] index;
		}

	}



	//テクスチャ読み込み
	for(unsigned int i = 0; i < model->AiScene->mNumTextures; i++)
	{
		aiTexture* aitexture = model->AiScene->mTextures[i];

		ID3D11ShaderResourceView* texture;
		TexMetadata metadata;
		ScratchImage image;
		LoadFromWICMemory(aitexture->pcData, aitexture->mWidth, WIC_FLAGS_NONE, &metadata, image);
		CreateShaderResourceView(DXGetDevice(), image.GetImages(), image.GetImageCount(), metadata, &texture);
		assert(texture);

		model->Texture[aitexture->mFilename.data] = texture;
	}



	return model;
}




void DX_ModelRelease(DX_MODEL* model)
{
	for (unsigned int m = 0; m < model->AiScene->mNumMeshes; m++)
	{
		model->VertexBuffer[m]->Release();
		model->IndexBuffer[m]->Release();
	}

	delete[] model->VertexBuffer;
	delete[] model->IndexBuffer;


	for (std::pair<const std::string, ID3D11ShaderResourceView*> pair : model->Texture)
	{
		pair.second->Release();
	}


	aiReleaseImport(model->AiScene);


	delete model;
}

void DX_ModelDraw(DX_MODEL* model, DX_MODEL_ANIM animdat, ID3D11ShaderResourceView* exSRV)
{
	if (model == nullptr)
	{
		return;
	}

	DXGetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	for (unsigned int m = 0; m < model->AiScene->mNumMeshes; m++)
	{
		aiMesh* mesh = model->AiScene->mMeshes[m];

        if (exSRV != nullptr)
        {
            DXGetDeviceContext()->PSSetShaderResources(0, 1, &exSRV);
        }
        else
        {
            aiString texture = {};
            aiMaterial* aimaterial = model->AiScene->mMaterials[mesh->mMaterialIndex];
            aimaterial->GetTexture(aiTextureType_DIFFUSE, 0, &texture);

            if (texture != aiString(""))
            {
                DXGetDeviceContext()->PSSetShaderResources(0, 1, &model->Texture[texture.data]);
            }
        }

        //XMMATRIX mattmp = {};

        //if (model->AiScene->mAnimations != nullptr)
        //{
        //    aiAnimation* anim = model->AiScene->mAnimations[0];

        //    if (anim->mChannels != nullptr)
        //    {
        //        aiNodeAnim* animNode = anim->mChannels[0];

        //        {
        //            int posF = animdat.frame % animNode->mNumPositionKeys;
        //            int rotF = animdat.frame % animNode->mNumRotationKeys;
        //            int scaF = animdat.frame % animNode->mNumScalingKeys;

        //            XMMATRIX pos = XMMatrixTranslation(
        //                animNode->mPositionKeys[posF].mValue.x,
        //                animNode->mPositionKeys[posF].mValue.y,
        //                animNode->mPositionKeys[posF].mValue.z
        //            );

        //            XMFLOAT4 rotf4 = {
        //                animNode->mRotationKeys[rotF].mValue.x,
        //                animNode->mRotationKeys[rotF].mValue.y,
        //                animNode->mRotationKeys[rotF].mValue.z,
        //                animNode->mRotationKeys[rotF].mValue.w
        //            };
        //            XMMATRIX rot = XMMatrixRotationQuaternion(XMLoadFloat4(&rotf4));

        //            XMMATRIX scale = XMMatrixScaling(
        //                animNode->mScalingKeys[scaF].mValue.x,
        //                animNode->mScalingKeys[scaF].mValue.y,
        //                animNode->mScalingKeys[scaF].mValue.z
        //            );

        //            mattmp *= scale;
        //            mattmp *= rot;
        //            mattmp *= pos;
        //        }
        //    }
        //}
        //mattmp *= animdat.mat;

        //XMFLOAT4X4 matrix4x4;
        //DirectX::XMStoreFloat4x4(&matrix4x4, mattmp);

        ////送信
        //DX_ShaderVertexSetMatrixEx(&matrix4x4, animdat.svsmdat);


		UINT stride = sizeof(Vertex_DX);
		UINT offset = 0;
		DXGetDeviceContext()->IASetVertexBuffers(0, 1, &model->VertexBuffer[m], &stride, &offset);

		DXGetDeviceContext()->IASetIndexBuffer(model->IndexBuffer[m], DXGI_FORMAT_R32_UINT, 0);

		DXGetDeviceContext()->DrawIndexed(mesh->mNumFaces * 3, 0, 0);
	}
}





