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
	// csoファイルのデータを読み込むクラス
	class CsoLoader{
	public:
		CsoLoader();
		~CsoLoader();

		// csoファイルのデータを読み込む
		bool Load(const char* filename);
		
		// 解放
		void Release();
		
		// データ取得
		const char* GetData() const { return m_data; }

		// サイズ取得
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

	// csoファイルのデータを読み込む
	bool CsoLoader::Load(const char* filename){
		std::ifstream ifs;

		ifs.open(filename, std::ios::binary);
		// 読み込み失敗チェック
		if( !ifs ){
			OUTPUT_CONSOLE_LINE("failed...OpenCso:" << filename);
			return false;
		}
		// ファイルサイズを取得する
		ifs.seekg(0, std::ios::end);
		m_size = static_cast<int>(ifs.tellg());
		ifs.seekg(0, std::ios::beg);
		// メモリを確保し、読み込み
		m_data = new char[m_size];
		ifs.read(m_data, m_size);

		ifs.close();

		return true;
	}

	// 解放
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
	
	// VertexShaderのcsoファイル読み込み
	bool Shader::LoadVSFromCso(const char* filename, VertexLayoutType vevertexLayoutTypertex){
		// パスの合成
		std::string fullPath = ROOT_PAHT_SHADER;
		fullPath += filename;
		fullPath += ".cso";
		// csoファイルのデータ読み込み
		CsoLoader loader;
		if( !loader.Load(fullPath.c_str()) ){
			OUTPUT_CONSOLE_LINE("Filed... LoadCso:" << fullPath.c_str());
			return false;
		}
		// 頂点シェーダー作成
		HRESULT hr = MDX_GET_DEVICE->CreateVertexShader(loader.GetData(), loader.GetSize(), nullptr, &m_vertexShader);
		// 読み込み失敗
		if( FAILED(hr) ){
			OUTPUT_CONSOLE_LINE("Filed... CreateVertexShader:" << fullPath.c_str());
			return false;
		}
		// 頂点レイアウト作成
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
		// 読み込み失敗
		if( FAILED(hr) ){
			OUTPUT_CONSOLE_LINE("Filed... CreateInputLayout:" << fullPath.c_str());
			return false;
		}

		loader.Release();


		return true;
	}
	// PixelShaderのcsoファイル読み込み
	bool Shader::LoadPSFromCso(const char* filename){
		// パスの合成
		std::string fullPath = ROOT_PAHT_SHADER;
		fullPath += filename;
		fullPath += ".cso";
		// csoファイルのデータ読み込み
		CsoLoader loader;
		if( !loader.Load(fullPath.c_str()) ){
			OUTPUT_CONSOLE_LINE("Filed... LoadCso:" << fullPath.c_str());
			return false;
		}
		// ピクセルシェーダー作成
		HRESULT hr = MDX_GET_DEVICE->CreatePixelShader(loader.GetData(), loader.GetSize(), nullptr, &m_pixelShader);
		// 読み込み失敗
		if( FAILED(hr) ){
			OUTPUT_CONSOLE_LINE("Filed... CreatePixelShader:" << fullPath.c_str());
			return false;
		}

		loader.Release();

		return true;
	}
	// 解放
	void Shader::Release(){
		SAFE_RELEASE(m_vertexShader);
		SAFE_RELEASE(m_pixelShader);
		SAFE_RELEASE(m_inputLayout);
	}

	// シェーダーを設定
	void Shader::SetShader(){
		MDX_GET_IMMEDIATE_CONTEXT->IASetInputLayout(m_inputLayout);
		MDX_GET_IMMEDIATE_CONTEXT->VSSetShader(m_vertexShader, nullptr, 0);
		MDX_GET_IMMEDIATE_CONTEXT->PSSetShader(m_pixelShader, nullptr, 0);
	}
	//--------------------------------------------------------------------
	// private
	//--------------------------------------------------------------------
}
