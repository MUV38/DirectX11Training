/**
* @file MDX_ConstantBuffer.h
* @brief コンスタントバッファクラスの定義
* @author Toru Miyawaki
*/

#ifndef MDX_CONSTANT_BUFFER_H
#define MDX_CONSTANT_BUFFER_H

#include <d3d11.h>

/**
* @brief namespace MDX
*/
namespace MDX {
	/**
	* @brief コンスタントバッファクラス
	*/
	class ConstantBuffer
	{
	public:
		ConstantBuffer();
		~ConstantBuffer();

		/**
		* @brief 作成
		* @param [in] byteWidth バイトサイズ
		* @return 作成成功かどうか
		*/
		bool Create(UINT byteWidth);

		/**
		* @brief 解放
		*/
		void Release();

		/**
		* @brief マップ
		* @return マップしたリソース
		*/
		D3D11_MAPPED_SUBRESOURCE Map();

		/**
		* @brief アンマップ
		*/
		void Unmap();

	public:
		/**
		* @brief 頂点シェーダーに設定
		*/
		void SetVS(UINT slot);

		/**
		* @brief ピクセルシェーダーに設定
		*/
		void SetPS(UINT slot);

	private:
		ID3D11Buffer* m_buffer;
	};

}

#endif // !MDX_CONSTANT_BUFFER_H
