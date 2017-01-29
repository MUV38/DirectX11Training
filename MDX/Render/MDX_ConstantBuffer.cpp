/**
* @file MDX_ConstantBuffer.cpp
* @brief �R���X�^���g�o�b�t�@�N���X�̒�`
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

	// �쐬
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
		assert(SUCCEEDED(hr) && "�R���X�^���g�o�b�t�@�쐬���s");

		return (hr == S_OK);
	}

	// ���
	void ConstantBuffer::Release()
	{
		SAFE_RELEASE(m_buffer);
	}

	// �}�b�v
	D3D11_MAPPED_SUBRESOURCE ConstantBuffer::Map()
	{
		HRESULT hr = S_OK;

		D3D11_MAPPED_SUBRESOURCE resource;
		hr = MDX_GET_IMMEDIATE_CONTEXT->Map(m_buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &resource);
		assert(SUCCEEDED(hr) && "�R���X�^���g�o�b�t�@�}�b�v���s");

		return resource;
	}

	// �A���}�b�v
	void ConstantBuffer::Unmap()
	{
		MDX_GET_IMMEDIATE_CONTEXT->Unmap(m_buffer, 0);
	}

	// ���_�V�F�[�_�[�ɐݒ�
	void ConstantBuffer::SetVS(UINT slot)
	{
		MDX_GET_IMMEDIATE_CONTEXT->VSSetConstantBuffers(slot, 1, &m_buffer);
	}

	// �s�N�Z���V�F�[�_�[�ɐݒ�
	void ConstantBuffer::SetPS(UINT slot)
	{
		MDX_GET_IMMEDIATE_CONTEXT->PSSetConstantBuffers(slot, 1, &m_buffer);
	}
}