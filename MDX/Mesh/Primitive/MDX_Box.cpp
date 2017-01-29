#include "MDX_Box.h"
#include "../../System/MDX_System.h"
#include "../../Utillity/MDX_Utillity.h"
#include "../../Render/MDX_RenderManager.h"
#include "../../Shader/MDX_DefaultVertexLayout.h"

namespace MDX{
	Box::Box(){

	}

	Box::~Box(){
		Release();
	}

	// 箱作成
	bool Box::Create(){
		HRESULT hr = S_OK;
		D3D11_BUFFER_DESC bd;
		D3D11_SUBRESOURCE_DATA initData;

		// 頂点バッファ作成
		DefaultVertexInfo vertices[] = {
			{ Vector3f(-0.5f,  0.5f, -0.5f), Vector3f(0, 0, -1), 0xFFFFFFFF, Vector3f(), Vector3f(), Vector2f(0, 0) },//0
			{ Vector3f(0.5f,  0.5f, -0.5f),  Vector3f(0, 0, -1), 0xFFFFFFFF, Vector3f(), Vector3f(), Vector2f(1, 0) },//1
			{ Vector3f(-0.5f, -0.5f, -0.5f), Vector3f(0, 0, -1), 0xFFFFFFFF, Vector3f(), Vector3f(), Vector2f(0, 1) },//2

			{ Vector3f( 0.5f,  0.5f, -0.5f), Vector3f(0, 0, -1), 0xFFFFFFFF, Vector3f(), Vector3f(), Vector2f(1, 0) },//1
			{ Vector3f(0.5f, -0.5f, -0.5f),  Vector3f(0, 0, -1), 0xFFFFFFFF, Vector3f(), Vector3f(), Vector2f(1, 1) },//3
			{ Vector3f(-0.5f, -0.5f, -0.5f), Vector3f(0, 0, -1), 0xFFFFFFFF, Vector3f(), Vector3f(), Vector2f(0, 1) },//2

			{ Vector3f( 0.5f, 0.5f,   0.5f), Vector3f(0, 0,  1), 0xFFFFFFFF, Vector3f(), Vector3f(), Vector2f(0, 0) },//4
			{ Vector3f(-0.5f, 0.5f,   0.5f), Vector3f(0, 0,  1), 0xFFFFFFFF, Vector3f(), Vector3f(), Vector2f(1, 0) },//5
			{ Vector3f(0.5f, -0.5f,   0.5f), Vector3f(0, 0,  1), 0xFFFFFFFF, Vector3f(), Vector3f(), Vector2f(0, 1) },//6

			{ Vector3f(-0.5f, 0.5f,   0.5f), Vector3f(0, 0, 1), 0xFFFFFFFF, Vector3f(), Vector3f(), Vector2f(1, 0) },//5
			{ Vector3f(-0.5f, -0.5f,  0.5f), Vector3f(0, 0, 1), 0xFFFFFFFF, Vector3f(), Vector3f(), Vector2f(1, 1) },//7
			{ Vector3f(0.5f, -0.5f,   0.5f), Vector3f(0, 0, 1), 0xFFFFFFFF, Vector3f(), Vector3f(), Vector2f(0, 1) },//6

			{ Vector3f( 0.5f,  0.5f, -0.5f), Vector3f(1, 0, 0), 0xFFFFFFFF, Vector3f(), Vector3f(), Vector2f(0, 0) },//1
			{ Vector3f(0.5f, 0.5f,   0.5f),  Vector3f(1, 0, 0), 0xFFFFFFFF, Vector3f(), Vector3f(), Vector2f(1, 0) },//4
			{ Vector3f(0.5f, -0.5f, -0.5f),  Vector3f(1, 0, 0), 0xFFFFFFFF, Vector3f(), Vector3f(), Vector2f(0, 1) },//3

			{ Vector3f( 0.5f, 0.5f,   0.5f), Vector3f(1, 0, 0), 0xFFFFFFFF, Vector3f(), Vector3f(), Vector2f(1, 0) },//4
			{ Vector3f(0.5f, -0.5f,  0.5f),  Vector3f(1, 0, 0), 0xFFFFFFFF, Vector3f(), Vector3f(), Vector2f(1, 1) },//6
			{ Vector3f(0.5f, -0.5f, -0.5f),  Vector3f(1, 0, 0), 0xFFFFFFFF, Vector3f(), Vector3f(), Vector2f(0, 1) },//3
		
			{ Vector3f(-0.5f, 0.5f,   0.5f), Vector3f(-1, 0, 0), 0xFFFFFFFF, Vector3f(), Vector3f(), Vector2f(0, 0) },//5
			{ Vector3f(-0.5f,  0.5f, -0.5f), Vector3f(-1, 0, 0), 0xFFFFFFFF, Vector3f(), Vector3f(), Vector2f(1, 0) },//0
			{ Vector3f(-0.5f, -0.5f,  0.5f), Vector3f(-1, 0, 0), 0xFFFFFFFF, Vector3f(), Vector3f(), Vector2f(0, 1) },//7

			{ Vector3f(-0.5f,  0.5f, -0.5f), Vector3f(-1, 0, 0), 0xFFFFFFFF, Vector3f(), Vector3f(), Vector2f(1, 0) },//0
			{ Vector3f(-0.5f, -0.5f, -0.5f), Vector3f(-1, 0, 0), 0xFFFFFFFF, Vector3f(), Vector3f(), Vector2f(1, 1) },//2
			{ Vector3f(-0.5f, -0.5f,  0.5f), Vector3f(-1, 0, 0), 0xFFFFFFFF, Vector3f(), Vector3f(), Vector2f(0, 1) },//7
		
		
			{ Vector3f(-0.5f, 0.5f,   0.5f), Vector3f(0, 1, 0), 0xFFFFFFFF, Vector3f(), Vector3f(), Vector2f(0, 0) },//5
			{ Vector3f(0.5f, 0.5f,   0.5f),  Vector3f(0, 1, 0), 0xFFFFFFFF, Vector3f(), Vector3f(), Vector2f(1, 0) },//4
			{ Vector3f(-0.5f,  0.5f, -0.5f), Vector3f(0, 1, 0), 0xFFFFFFFF, Vector3f(), Vector3f(), Vector2f(0, 1) },//0

			{ Vector3f( 0.5f, 0.5f,   0.5f), Vector3f(0, 1, 0), 0xFFFFFFFF, Vector3f(), Vector3f(), Vector2f(1, 0) },//4
			{ Vector3f(0.5f,  0.5f, -0.5f),  Vector3f(0, 1, 0), 0xFFFFFFFF, Vector3f(), Vector3f(), Vector2f(1, 1) },//1
			{ Vector3f(-0.5f,  0.5f, -0.5f), Vector3f(0, 1, 0), 0xFFFFFFFF, Vector3f(), Vector3f(), Vector2f(0, 1) },//0

			{ Vector3f( 0.5f, -0.5f,  0.5f), Vector3f(0, -1, 0), 0xFFFFFFFF, Vector3f(), Vector3f(), Vector2f(0, 0) },//6
			{ Vector3f(-0.5f, -0.5f,  0.5f), Vector3f(0, -1, 0), 0xFFFFFFFF, Vector3f(), Vector3f(), Vector2f(1, 0) },//7
			{ Vector3f(0.5f, -0.5f, -0.5f),  Vector3f(0, -1, 0), 0xFFFFFFFF, Vector3f(), Vector3f(), Vector2f(0, 1) },//3

			{ Vector3f(-0.5f, -0.5f,  0.5f), Vector3f(0, -1, 0), 0xFFFFFFFF, Vector3f(), Vector3f(), Vector2f(1, 0) },//7
			{ Vector3f(-0.5f, -0.5f, -0.5f), Vector3f(0, -1, 0), 0xFFFFFFFF, Vector3f(), Vector3f(), Vector2f(1, 1) },//2
			{ Vector3f(0.5f, -0.5f, -0.5f),  Vector3f(0, -1, 0), 0xFFFFFFFF, Vector3f(), Vector3f(), Vector2f(0, 1) },//3

		};
		ZeroMemory(&bd, sizeof(D3D11_BUFFER_DESC));
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.ByteWidth = sizeof(DefaultVertexInfo) * ARRAYSIZE(vertices);
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bd.CPUAccessFlags = 0;
		ZeroMemory(&initData, sizeof(D3D11_SUBRESOURCE_DATA));
		initData.pSysMem = vertices;
		hr = MDX_GET_DEVICE->CreateBuffer(&bd, &initData, &m_vertexBuffer);
		if( FAILED(hr) ){
			return false;
		}

		// インデックスバッファ
		USHORT indices[] = {
			0, 1, 2,
			3, 4, 5,
			6, 7, 8,
			9, 10, 11,
			12, 13, 14,
			15, 16, 17,
			18, 19, 20,
			21, 22, 23,
			24, 25, 26,
			27, 28, 29,
			30, 31, 32,
			33, 34, 35
		};
		ZeroMemory(&bd, sizeof(D3D11_BUFFER_DESC));
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.ByteWidth = sizeof(USHORT) * ARRAYSIZE(indices);
		bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
		bd.CPUAccessFlags = 0;
		ZeroMemory(&initData, sizeof(D3D11_SUBRESOURCE_DATA));
		initData.pSysMem = indices;
		hr = MDX_GET_DEVICE->CreateBuffer(&bd, &initData, &m_indexBuffer);
		if( FAILED(hr) ){
			return false;
		}
		// コンスタントバッファ
		ZeroMemory(&bd, sizeof(D3D11_BUFFER_DESC));
		bd.Usage = D3D11_USAGE_DYNAMIC;
		bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		bd.MiscFlags = 0;
		bd.StructureByteStride = 0;
		bd.ByteWidth = sizeof(CBMatrix);
		hr = MDX_GET_DEVICE->CreateBuffer(&bd, nullptr, &m_cbMatrix);
		if( FAILED(hr) ){
			return false;
		}
		// シェーダー
		m_shader.LoadVSFromCso("VS_Primitive", MDX::Shader::VERTEX_LAYOUT_3D);
		m_shader.LoadPSFromCso("PS_Primitive");


		return true;
	}

	// 解放
	void Box::Release(){
		SAFE_RELEASE(m_vertexBuffer);
		SAFE_RELEASE(m_indexBuffer);
		SAFE_RELEASE(m_cbMatrix);
		m_shader.Release();
	}

	// 描画
	void Box::Draw(){
		assert(m_vertexBuffer && m_indexBuffer && m_cbMatrix);

		m_shader.SetShader();

		HRESULT hr = S_OK;

		// 変換行列作成
		DirectX::XMMATRIX mat = DirectX::XMMatrixIdentity();
		mat *= DirectX::XMMatrixScaling(m_scale.x, m_scale.y, m_scale.z);
		mat *= DirectX::XMMatrixRotationX(m_angle.x);
		mat *= DirectX::XMMatrixRotationY(m_angle.y);
		mat *= DirectX::XMMatrixRotationZ(m_angle.z);
		mat *= DirectX::XMMatrixTranslation(m_pos.x, m_pos.y, m_pos.z);
		mat = DirectX::XMMatrixTranspose(mat);
		CBMatrix cbm;
		DirectX::XMStoreFloat4x4(&cbm.matWorld, mat);
		cbm.matView = MDX_GET_RENDER_MANAGER->GetMatView();
		cbm.matProjection = MDX_GET_RENDER_MANAGER->GetMatProjection();

		// コンスタントバッファに書き込み
		D3D11_MAPPED_SUBRESOURCE mappedResource;
		hr = MDX_GET_IMMEDIATE_CONTEXT->Map(m_cbMatrix, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
		CopyMemory(mappedResource.pData, reinterpret_cast<void*>(&cbm), sizeof(CBMatrix));
		MDX_GET_IMMEDIATE_CONTEXT->Unmap(m_cbMatrix, 0);
		assert(hr == S_OK);

		// 描画
		UINT stride = sizeof(DefaultVertexInfo);
		UINT offset = 0;
		MDX_GET_IMMEDIATE_CONTEXT->IASetVertexBuffers(0, 1, &m_vertexBuffer, &stride, &offset);
		MDX_GET_IMMEDIATE_CONTEXT->IASetIndexBuffer(m_indexBuffer, DXGI_FORMAT_R16_UINT, 0);
		MDX_GET_IMMEDIATE_CONTEXT->VSSetConstantBuffers(0, 1, &m_cbMatrix);
		MDX_GET_IMMEDIATE_CONTEXT->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		MDX_GET_IMMEDIATE_CONTEXT->DrawIndexed(3*2*6, 0, 0);
	}
}