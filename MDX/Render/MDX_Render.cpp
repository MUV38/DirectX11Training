#include "MDX_Render.h"
#include "../System/MDX_System.h"

namespace MDX{
	namespace Render{
		// レンダーターゲットクリア
		void ClearRenderTarget(ID3D11RenderTargetView* render_target, const Vector4f& color){
			float c[4] = {color.x, color.y, color.z, color.w};
			
			MDX_GET_IMMEDIATE_CONTEXT->ClearRenderTargetView(render_target, c);
		}
		// デプスステンシルクリア
		void ClearDepthStencil(ID3D11DepthStencilView* depth_stencil, float depth, UINT8 stencil){
			MDX_GET_IMMEDIATE_CONTEXT->ClearDepthStencilView(depth_stencil, D3D11_CLEAR_DEPTH|D3D11_CLEAR_STENCIL, depth, stencil);
		}

	}
}