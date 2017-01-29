/**
* @file MDX_ConstantBuffer.cpp
* @brief コンスタントバッファクラスの定義
* @author Toru Miyawaki
*/

#include "MDX_ConstantBuffer.h"
#include "../System/MDX_System.h"
#include "../Utillity/MDX_Utillity.h"

namespace MDX
{
	ConstantBuffer::ConstantBuffer()
		: m_buffer(nullptr)
	{
	}

	ConstantBuffer::~ConstantBuffer()
	{
		Release();
	}

	// 作成
	bool ConstantBuffer::Create(UINT byteWidth)
	{
		HRESULT hr = S_OK;

		D3D11_BUFFER_DESC bd;
		ZeroMemory(&bd, sizeof(D3D11_BUFFER_DESC));
		bd.Usage				= D3D11_USAGE_DYNAMIC;
		bd.BindFlags			= D3D11_BIND_CONSTANT_BUFFER;
		bd.CPUAccessFlags		= D3D11_CPU_ACCESS_WRITE;
		bd.MiscFlags			= 0;
		bd.StructureByteStride	= 0;
		bd.ByteWidth			= byteWidth;

		hr = MDX_GET_DEVICE->CreateBuffer(&bd, nullptr, &m_buffer);
		assert(SUCCEEDED(hr) && "コンスタントバッファ作成失敗");

		return (hr == S_OK);
	}

	// 解放
	void ConstantBuffer::Release()
	{
		SAFE_RELEASE(m_buffer);
	}

	// マップ
	D3D11_MAPPED_SUBRESOURCE ConstantBuffer::Map()
	{
		HRESULT hr = S_OK;

		D3D11_MAPPED_SUBRESOURCE resource;
		hr = MDX_GET_IMMEDIATE_CONTEXT->Map(m_buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &resource);
		assert(SUCCEEDED(hr) && "コンスタントバッファマップ失敗");

		return resource;
	}

	// アンマップ
	void ConstantBuffer::Unmap()
	{
		MDX_GET_IMMEDIATE_CONTEXT->Unmap(m_buffer, 0);
	}

	// 頂点シェーダーに設定
	void ConstantBuffer::SetVS(UINT slot)
	{
		MDX_GET_IMMEDIATE_CONTEXT->VSSetConstantBuffers(slot, 1, &m_buffer);
	}

	// ピクセルシェーダーに設定
	void ConstantBuffer::SetPS(UINT slot)
	{
		MDX_GET_IMMEDIATE_CONTEXT->PSSetConstantBuffers(slot, 1, &m_buffer);
	}
}