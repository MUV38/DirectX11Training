/**
* @file MDX_RenderTarget.h
* @brief レンダーターゲットクラスの定義
* @author Toru Miyawaki
*/

#ifndef MDX_RENDER_TARGET_H
#define MDX_RENDER_TARGET_H

#include <d3d11.h>
#include "../Math/MDX_Math.h"

/**
* @brief namespace MDX
*/
namespace MDX{
	/**
	* @brief レンダーターゲットクラス
	*/
	class RenderTarget
	{
	public:
		RenderTarget();
		~RenderTarget();

		/**
		* @brief 作成
		* @param [in] width 横幅
		* @param [in] height 縦幅
		* @param [in] format フォーマット
		*/
		bool Create(UINT width, UINT height, DXGI_FORMAT format);

		/**
		* @brief 解放
		*/
		void Release();

		/**
		* @brief レンダーターゲットクリア
		* @param [in] color クリア色
		*/
		void Clear(const Vector4f& color);

		/**
		* @brief ゲッター
		*/
		ID3D11RenderTargetView* GetRenderTargetView() const { return m_rtv; }
		ID3D11ShaderResourceView* GetShaderResourceView() const { return m_srv; }
		UINT GetWidth() const { return m_width; }
		UINT GetHeight() const { return m_height; }

	private:
		/**
		* @brief テクスチャフォーマット取得
		*/
		DXGI_FORMAT GetTextureFormat(DXGI_FORMAT format);

	private:
		UINT m_width = 0;
		UINT m_height = 0;

		ID3D11Texture2D* m_texture = nullptr;
		ID3D11RenderTargetView* m_rtv = nullptr;
		ID3D11ShaderResourceView* m_srv = nullptr;
	};

}

#endif // !MDX_RENDER_TARGET_H
