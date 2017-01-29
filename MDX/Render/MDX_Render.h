#ifndef RENDER_H
#define RENDER_H

#include <d3d11.h>
#include "../Math/MDX_Math.h"

namespace MDX{
	namespace Render{
		/**
		* @brief �����_�[�^�[�Q�b�g�N���A
		* @param [in] render_target �����_�[�^�[�Q�b�g
		* @param [in] color �N���A�F
		*/
		extern void ClearRenderTarget(ID3D11RenderTargetView* render_target, const Vector4f& color);

		/**
		* @brief �f�v�X�X�e���V���N���A
		* @param [in] depth_stencil �f�v�X�X�e���V��
		* @param [in] depth �[�x�l
		* @param [in] stencil �X�e���V��
		*/
		extern void ClearDepthStencil(ID3D11DepthStencilView* depth_stencil, FLOAT depth, UINT8 stencil);
	}
}

#endif // !RENDER_H
