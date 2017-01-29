#include "MDX_Shader.h"
#include "../System/MDX_System.h"
#include "../Utillity/MDX_UtilDebug.h"
#include "../Utillity/MDX_Utillity.h"
#include "MDX_DefaultVertexLayout.h"

#include <iostream>
#include <fstream>
#include <cassert>
#include <string>

namespace MDX{
	//**********************************************************************
	// CsoLoader
	//**********************************************************************
	// cso�t�@�C���̃f�[�^��ǂݍ��ރN���X
	class CsoLoader{
	public:
		CsoLoader();
		~CsoLoader();

		// cso�t�@�C���̃f�[�^��ǂݍ���
		bool Load(const char* filename);
		
		// ���
		void Release();
		
		// �f�[�^�擾
		const char* GetData() const { return m_data; }

		// �T�C�Y�擾
		int GetSize() const { return m_size; }
	private:
		char* m_data = nullptr;
		int m_size = 0;
	};

	CsoLoader::CsoLoader(){
	
	}
	CsoLoader::~CsoLoader(){
		Release();
	}

	// cso�t�@�C���̃f�[�^��ǂݍ���
	bool CsoLoader::Load(const char* filename){
		std::ifstream ifs;

		ifs.open(filename, std::ios::binary);
		// �ǂݍ��ݎ��s�`�F�b�N
		if( !ifs ){
			OUTPUT_CONSOLE_LINE("failed...OpenCso:" << filename);
			return false;
		}
		// �t�@�C���T�C�Y���擾����
		ifs.seekg(0, std::ios::end);
		m_size = static_cast<int>(ifs.tellg());
		ifs.seekg(0, std::ios::beg);
		// ���������m�ۂ��A�ǂݍ���
		m_data = new char[m_size];
		ifs.read(m_data, m_size);

		ifs.close();

		return true;
	}

	// ���
	void CsoLoader::Release(){
		SAFE_DELETE(m_data);
	}

	//**********************************************************************
	// mdxShader
	//**********************************************************************
	//--------------------------------------------------------------------
	// public
	//--------------------------------------------------------------------
	Shader::Shader(){
	}
	Shader::~Shader(){
		Release();
	}
	
	// VertexShader��cso�t�@�C���ǂݍ���
	bool Shader::LoadVSFromCso(const char* filename, VertexLayoutType vevertexLayoutTypertex){
		// �p�X�̍���
		std::string fullPath = ROOT_PAHT_SHADER;
		fullPath += filename;
		fullPath += ".cso";
		// cso�t�@�C���̃f�[�^�ǂݍ���
		CsoLoader loader;
		if( !loader.Load(fullPath.c_str()) ){
			OUTPUT_CONSOLE_LINE("Filed... LoadCso:" << fullPath.c_str());
			return false;
		}
		// ���_�V�F�[�_�[�쐬
		HRESULT hr = MDX_GET_DEVICE->CreateVertexShader(loader.GetData(), loader.GetSize(), nullptr, &m_vertexShader);
		// �ǂݍ��ݎ��s
		if( FAILED(hr) ){
			OUTPUT_CONSOLE_LINE("Filed... CreateVertexShader:" << fullPath.c_str());
			return false;
		}
		// ���_���C�A�E�g�쐬
		const D3D11_INPUT_ELEMENT_DESC* inputDesc = nullptr;
		int numElements = 0;
		switch (vevertexLayoutTypertex){
		case VERTEX_LAYOUT_3D:
			inputDesc = g_defaultVertexLayout3D;
			numElements = ARRAYSIZE(g_defaultVertexLayout3D);
			break;
		case VERTEX_LAYOUT_2D:
			inputDesc = g_defaultVertexLayout2D;
			numElements = ARRAYSIZE(g_defaultVertexLayout2D);
			break;
		default:
			OUTPUT_CONSOLE_LINE("Error VertexLayoutType");
			assert(0);
			break;
		}
		hr = MDX_GET_DEVICE->CreateInputLayout(inputDesc, numElements, loader.GetData(), loader.GetSize(), &m_inputLayout);
		// �ǂݍ��ݎ��s
		if( FAILED(hr) ){
			OUTPUT_CONSOLE_LINE("Filed... CreateInputLayout:" << fullPath.c_str());
			return false;
		}

		loader.Release();


		return true;
	}
	// PixelShader��cso�t�@�C���ǂݍ���
	bool Shader::LoadPSFromCso(const char* filename){
		// �p�X�̍���
		std::string fullPath = ROOT_PAHT_SHADER;
		fullPath += filename;
		fullPath += ".cso";
		// cso�t�@�C���̃f�[�^�ǂݍ���
		CsoLoader loader;
		if( !loader.Load(fullPath.c_str()) ){
			OUTPUT_CONSOLE_LINE("Filed... LoadCso:" << fullPath.c_str());
			return false;
		}
		// �s�N�Z���V�F�[�_�[�쐬
		HRESULT hr = MDX_GET_DEVICE->CreatePixelShader(loader.GetData(), loader.GetSize(), nullptr, &m_pixelShader);
		// �ǂݍ��ݎ��s
		if( FAILED(hr) ){
			OUTPUT_CONSOLE_LINE("Filed... CreatePixelShader:" << fullPath.c_str());
			return false;
		}

		loader.Release();

		return true;
	}
	// ���
	void Shader::Release(){
		SAFE_RELEASE(m_vertexShader);
		SAFE_RELEASE(m_pixelShader);
		SAFE_RELEASE(m_inputLayout);
	}

	// �V�F�[�_�[��ݒ�
	void Shader::SetShader(){
		MDX_GET_IMMEDIATE_CONTEXT->IASetInputLayout(m_inputLayout);
		MDX_GET_IMMEDIATE_CONTEXT->VSSetShader(m_vertexShader, nullptr, 0);
		MDX_GET_IMMEDIATE_CONTEXT->PSSetShader(m_pixelShader, nullptr, 0);
	}
	//--------------------------------------------------------------------
	// private
	//--------------------------------------------------------------------
}
