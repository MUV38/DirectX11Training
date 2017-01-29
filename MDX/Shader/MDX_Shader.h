/**
* @file MDX_Shader.h
* @brief シェーダー関連の定義
* @author Toru Miyawaki
*/

#ifndef MDX_SHADER_H
#define MDX_SHADER_H

#include <d3d11.h>

/**シェーダーを読み込むルートパスの定数*/
#define ROOT_PAHT_SHADER "DATA/Shader/";

/**
* @brief namespace MDX
*/
namespace MDX{
	/**
	* @brief シェーダークラス
	*/
	class Shader{
	public:
		/**
		* @brief 頂点レイアウトの種類
		*/
		enum VertexLayoutType{
			VERTEX_LAYOUT_3D,
			VERTEX_LAYOUT_2D,
			VERTEX_LAYOUT_TYPE_MAX
		};
	public:
		Shader();
		~Shader();

		/**
		* @brief VertexShaderのcsoファイル読み込み
		* @param [in] filename ファイル名
		* @param [in] vertexLayoutType 頂点レイアウトの種類
		* @return ファイルが読み込めたか
		* "DATA/Shader"がデフォルトパスなので注意
		*/
		bool LoadVSFromCso(const char* filename, VertexLayoutType vevertexLayoutTypertex);
		
		/**
		* @brief PixelShaderのcsoファイル読み込み
		* @param [in] filename ファイル名
		* @return ファイルが読み込めたか
		* "DATA/Shader"がデフォルトパスなので注意
		*/
		bool LoadPSFromCso(const char* filename);

		/**
		* @brief 解放
		*/
		void Release();

		/**
		* @brief シェーダーを設定
		*/
		void SetShader();
	private:

		ID3D11VertexShader* m_vertexShader = nullptr;
		ID3D11PixelShader* m_pixelShader = nullptr;
		ID3D11InputLayout* m_inputLayout = nullptr;
	};
}

#endif // !MDX_SHADER_H
