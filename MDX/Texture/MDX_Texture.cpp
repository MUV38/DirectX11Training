#include "MDX_Texture.h"
#include "../Utillity/MDX_Utillity.h"
#include "../System/MDX_System.h"
#include <WICTextureLoader.h>
#include <DDSTextureLoader.h>

namespace MDX{
	Texture::Texture() : 
		m_resource(nullptr),
		m_srv(nullptr)
	{
	
	}

	Texture::~Texture(){
		Release();
	}

	// �e�N�X�`���ǂݍ���
	bool Texture::Load(const LoadInfo& info){
		// �g���q�œǂݍ��ݐ��ς���
		std::string extension = GetExtension(info.filename.c_str());
		if(extension == "dds"){
			return LoadDDSTexture(info);
		}else if(extension == "bmp" ||
				 extension == "png" ||
				 extension == "jpg" ||
				 extension == "tif"){
			return LoadWICTexture(info);
		}else{
			return false;
		}
	}

	// ���
	void Texture::Release(){
		SAFE_RELEASE(m_resource);
		SAFE_RELEASE(m_srv);
	}

	// �s�N�Z���V�F�[�_�[�ɃV�F�[�_�[���\�[�X�ݒ�
	void Texture::PSSetShaderResource(UINT slot){
		MDX_GET_IMMEDIATE_CONTEXT->PSSetShaderResources(slot, 1, &m_srv);
	}

	// WIC�e�N�X�`��
	bool Texture::LoadWICTexture(const LoadInfo& info){
		HRESULT hr = S_OK;
		wchar_t filename[1024];
		size_t numChar = 0;

		mbstowcs_s(&numChar, filename, info.filename.c_str(), info.filename.size());
		hr = DirectX::CreateWICTextureFromFileEx(MDX_GET_DEVICE, filename, info.maxSize, info.usage, info.bindFlags, info.cpuAccessFlags, info.miscFlags, info.forceSRGB, &m_resource, &m_srv);
		
		// �e�N�X�`�����擾
		GetTextureSize(&m_width, &m_height);

		return SUCCEEDED(hr);
	}

	// DDS�e�N�X�`��
	bool Texture::LoadDDSTexture(const LoadInfo& info){
		HRESULT hr = S_OK;
		wchar_t filename[1024];
		size_t numChar = 0;
		
		mbstowcs_s(&numChar, filename, info.filename.c_str(), info.filename.size());
		hr = DirectX::CreateDDSTextureFromFileEx(MDX_GET_DEVICE, filename, info.maxSize, info.usage, info.bindFlags, info.cpuAccessFlags, info.miscFlags, info.forceSRGB, &m_resource, &m_srv);
		
		// �e�N�X�`�����擾
		GetTextureSize(&m_width, &m_height);
		
		return SUCCEEDED(hr);
	}

	// �e�N�X�`���̃T�C�Y�擾
	void Texture::GetTextureSize(UINT* width, UINT* height){
		if( m_resource == nullptr ){
			*width = *height = 0;
			return;
		}

		D3D11_RESOURCE_DIMENSION type;
		m_resource->GetType(&type);

		ID3D11Texture2D* tmp;
		m_resource->QueryInterface(__uuidof(ID3D11Texture2D), reinterpret_cast<void **>(&tmp));
		if( tmp ){
			D3D11_TEXTURE2D_DESC desc;
			tmp->GetDesc(&desc);
			*width = desc.Width;
			*height = desc.Height;

			tmp->Release();
		}else{
			*width = *height = 0;
		}
	}
}