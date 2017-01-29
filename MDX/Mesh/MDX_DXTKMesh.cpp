#include "MDX_DXTKMesh.h"
#include "../Utillity/MDX_Utillity.h"
#include "../System/MDX_System.h"


namespace MDX
{
	DXTKMesh::DXTKMesh()
	{

	}

	DXTKMesh::~DXTKMesh()
	{
		Release();
	}

	// ファイル読み込み
	bool DXTKMesh::Load(const char* filename)
	{
		std::string extension = GetExtension(filename);
		if (extension != "sdkmesh") {
			return false;
		}

		// ファイル名をwchar_t*に
		wchar_t wcFilename[256];
		size_t size = 0;
		mbstowcs_s(&size, wcFilename, 256, filename, strlen(filename));

		// エフェクト作成
		m_effectFactory.reset(new DirectX::EffectFactory(MDX_GET_DEVICE));

		// モデル作成
		m_model = DirectX::Model::CreateFromSDKMESH(MDX_GET_DEVICE, wcFilename, *m_effectFactory.get(), true);
			
		return true;
	}

	// 解放
	void DXTKMesh::Release()
	{
		BaseActor::Release();
	}

	// 更新
	void DXTKMesh::Update()
	{
		BaseActor::Update();
	}

	// 描画
	void DXTKMesh::Draw()
	{
		BaseActor::Draw();

		auto context = MDX_GET_IMMEDIATE_CONTEXT;
		for (uint32_t i = 0; i < m_model->meshes.size(); i++) {
			auto mesh = m_model->meshes[i];
			for (uint32_t j = 0; j < mesh->meshParts.size(); j++) {
				context->IASetPrimitiveTopology(mesh->meshParts[j]->primitiveType);

				UINT stride = mesh->meshParts[j]->vertexStride;
				UINT offset = mesh->meshParts[j]->vertexOffset;
				context->IASetVertexBuffers(0, 1, mesh->meshParts[j]->vertexBuffer.GetAddressOf(), &stride, &offset);
				context->IASetIndexBuffer(mesh->meshParts[j]->indexBuffer.Get(), mesh->meshParts[j]->indexFormat, 0);
				
				uint32_t indexCount = mesh->meshParts[j]->indexCount;
				uint32_t startIndex = mesh->meshParts[j]->startIndex;
				context->DrawIndexed(indexCount, startIndex, 0);
			}
		}
	}
}