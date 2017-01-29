#include "MDX_RenderTarget.h"
#include "../Utillity/MDX_Utillity.h"
#include "../System/MDX_System.h"

namespace MDX{
	RenderTarget::RenderTarget(){
	}

	RenderTarget::~RenderTarget(){
		Release();
	}

	// 作成
	bool RenderTarget::Create(UINT width, UINT height, DXGI_FORMAT format){
		assert(width > 0 && height > 0 && "MDX::RenderTargetのサイズが無効値");

		m_width = width;
		m_height = height;

		HRESULT hr = S_OK;

		// テクスチャ作成
		D3D11_TEXTURE2D_DESC texDesc;
		memset(&texDesc, 0, sizeof(texDesc));
		texDesc.Usage              = D3D11_USAGE_DEFAULT;
		texDesc.Format             = GetTextureFormat(format);
		texDesc.BindFlags          = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
		texDesc.Width              = width;
		texDesc.Height             = height;
		texDesc.CPUAccessFlags     = 0;
		texDesc.MipLevels          = 1;
		texDesc.ArraySize          = 1;
		texDesc.SampleDesc.Count   = 1;
		texDesc.SampleDesc.Quality = 0;
		hr = MDX_GET_DEVICE->CreateTexture2D(&texDesc, nullptr, &m_texture);
		assert(SUCCEEDED(hr) && "MDX::RenderTargetのテクスチャ作成失敗");

		// レンダーターゲットビュー作成
		D3D11_RENDER_TARGET_VIEW_DESC rtvDesc;
		memset(&rtvDesc, 0, sizeof(rtvDesc));
		rtvDesc.Format             = format;
		rtvDesc.ViewDimension      = D3D11_RTV_DIMENSION_TEXTURE2D;
		hr = MDX_GET_DEVICE->CreateRenderTargetView(m_texture, &rtvDesc, &m_rtv);
		assert(SUCCEEDED(hr) && "MDX::RenderTargetのレンダーターゲットビュー作成失敗");

		// シェーダーリソースビュー作成
		D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
		memset(&srvDesc, 0, sizeof(srvDesc));
		srvDesc.Format              = format;
		srvDesc.ViewDimension       = D3D11_SRV_DIMENSION_TEXTURE2D;
		srvDesc.Texture2D.MipLevels = 1;
		hr = MDX_GET_DEVICE->CreateShaderResourceView(m_texture, &srvDesc, &m_srv);
		assert(SUCCEEDED(hr) && "MDX::RenderTargetのシェーダーリソースビュー作成失敗");

		return true;
	}

	// 解放
	void RenderTarget::Release(){
		SAFE_RELEASE(m_texture);
		SAFE_RELEASE(m_rtv);
		SAFE_RELEASE(m_srv);
	}

	// テクスチャフォーマット取得
	DXGI_FORMAT RenderTarget::GetTextureFormat(DXGI_FORMAT format){
		DXGI_FORMAT f = format;

		switch (f){
		case DXGI_FORMAT_R8G8B8A8_UNORM:
		case DXGI_FORMAT_R8G8B8A8_UNORM_SRGB:
			f = DXGI_FORMAT_R8G8B8A8_TYPELESS;
			break;
		case DXGI_FORMAT_B8G8R8A8_UNORM:
		case DXGI_FORMAT_B8G8R8A8_UNORM_SRGB:
			f = DXGI_FORMAT_B8G8R8A8_TYPELESS;
			break;
		case DXGI_FORMAT_R16G16B16A16_UNORM:
			f = DXGI_FORMAT_R16G16B16A16_TYPELESS;
			break;
		case DXGI_FORMAT_R16G16B16A16_FLOAT:
			f = DXGI_FORMAT_R16G16B16A16_TYPELESS;
			break;
		case DXGI_FORMAT_R32_FLOAT:
			f = DXGI_FORMAT_R32_TYPELESS;
			break;
		default:
			assert(false && "MDX::RenderTarget、無効なテクスチャフォーマット");
			break;
		}
		return f;
	}

	// レンダーターゲットクリア
	void RenderTarget::Clear(const Vector4f& color){
		assert(m_rtv && "MDX::RenderTargetの画面クリア失敗");

		float c[4] = {color.x, color.y, color.z, color.w};
		MDX_GET_IMMEDIATE_CONTEXT->ClearRenderTargetView(m_rtv, c);
	}
}