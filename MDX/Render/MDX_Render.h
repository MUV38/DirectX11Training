#ifndef RENDER_H
#define RENDER_H

#include <d3d11.h>
#include "../Math/MDX_Math.h"

namespace MDX{
	namespace Render{
		/**
		* @brief レンダーターゲットクリア
		* @param [in] render_target レンダーターゲット
		* @param [in] color クリア色
		*/
		extern void ClearRenderTarget(ID3D11RenderTargetView* render_target, const Vector4f& color);

		/**
		* @brief デプスステンシルクリア
		* @param [in] depth_stencil デプスステンシル
		* @param [in] depth 深度値
		* @param [in] stencil ステンシル
		*/
		extern void ClearDepthStencil(ID3D11DepthStencilView* depth_stencil, FLOAT depth, UINT8 stencil);
	}
}

#endif // !RENDER_H
