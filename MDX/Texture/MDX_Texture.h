/**
* @file MDX_Texture.h
* @brief テクスチャクラス
* @author Toru Miyawaki
*/

#ifndef MDX_TEXTURE_H
#define	MDX_TEXTURE_H

#include <d3d11.h>
#include <string>

/**
* @brief namespace MDX
*/
namespace MDX{
	/**
	* @brief テクスチャクラス
	*/
	class Texture{
	public:
		/**
		* @brief 読み込み情報
		*/
		class LoadInfo{
		public:
			std::string filename;
			size_t maxSize;
			D3D11_USAGE usage;
			unsigned int bindFlags;
			unsigned int cpuAccessFlags;
			unsigned int miscFlags; 
			bool forceSRGB;

			LoadInfo(){
				Init();
			}

			/**
			* @brief 初期化
			*/
			void Init(){
				filename = "";
				maxSize = 0;
				usage = D3D11_USAGE_DEFAULT;
				bindFlags = D3D11_BIND_SHADER_RESOURCE;
				cpuAccessFlags = 0;
				miscFlags = 0;
				forceSRGB = false;
			}
		};

	public:
		Texture();
		~Texture();

		/**
		* @brief テクスチャ読み込み
		* @param [in] info 読み込み情報
		* @return 読み込み成功か？
		*/
		bool Load(const LoadInfo& info);

		/**
		* @brief 解放
		*/
		void Release();

		/**
		* @brief ピクセルシェーダーにシェーダーリソース設定
		* @param [in] slot スロット番号
		*/
		void PSSetShaderResource(UINT slot);

		/**
		* @brief ゲッター
		*/
		ID3D11ShaderResourceView* GetShaderResourceView() const { return m_srv; }
		UINT GetWidth() const { return m_width; }
		UINT GetHeight() const { return m_height; }


	private:
		/**
		* @brief WICテクスチャ読み込み
		*/
		bool LoadWICTexture(const LoadInfo& info);

		/**
		* @brief DDSテクスチャ読み込み
		*/
		bool LoadDDSTexture(const LoadInfo& info);

		/**
		* @brief テクスチャのサイズ取得
		*/
		void GetTextureSize(UINT* width, UINT* height);

	private:
		ID3D11Resource* m_resource;
		ID3D11ShaderResourceView* m_srv;

		UINT m_width;
		UINT m_height;
	};
}

#endif // !MDX_TEXTURE_H
