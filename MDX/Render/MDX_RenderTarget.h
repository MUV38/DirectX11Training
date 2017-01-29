/**
* @file MDX_RenderTarget.h
* @brief �����_�[�^�[�Q�b�g�N���X�̒�`
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
	* @brief �����_�[�^�[�Q�b�g�N���X
	*/
	class RenderTarget
	{
	public:
		RenderTarget();
		~RenderTarget();

		/**
		* @brief �쐬
		* @param [in] width ����
		* @param [in] height �c��
		* @param [in] format �t�H�[�}�b�g
		*/
		bool Create(UINT width, UINT height, DXGI_FORMAT format);

		/**
		* @brief ���
		*/
		void Release();

		/**
		* @brief �����_�[�^�[�Q�b�g�N���A
		* @param [in] color �N���A�F
		*/
		void Clear(const Vector4f& color);

		/**
		* @brief �Q�b�^�[
		*/
		ID3D11RenderTargetView* GetRenderTargetView() const { return m_rtv; }
		ID3D11ShaderResourceView* GetShaderResourceView() const { return m_srv; }
		UINT GetWidth() const { return m_width; }
		UINT GetHeight() const { return m_height; }

	private:
		/**
		* @brief �e�N�X�`���t�H�[�}�b�g�擾
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
