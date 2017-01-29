#include "MDX_DXSystem.h"
#include "../Utillity/MDX_Utillity.h"
#include "../Utillity/MDX_UtilDebug.h"

#include <imgui/imgui.h>
#include <imgui/directx11/imgui_impl_dx11.h>

#include <iostream>

namespace MDX{
	//--------------------------------------------------------------------
	// public
	//--------------------------------------------------------------------
	DXSystem::DXSystem(){
	}

	DXSystem::~DXSystem(){
		Release();
	}

	// 初期化
	bool DXSystem::Init(HWND hWnd){
		if( FAILED(InitDevice(hWnd)) ){
			return false;
		}
		return true;
	}

	// 解放
	void DXSystem::Release(){
		// imgui開放
		ImGui_ImplDX11_Shutdown();

		SAFE_RELEASE(m_backBufferRTV);
		SAFE_RELEASE(m_backBufferDSV);
		SAFE_RELEASE(m_swapChain);
		SAFE_RELEASE(m_device);
		SAFE_RELEASE(m_immediateContext);
	}
	
	// 更新
	void DXSystem::Update() {
		// imgui更新
		ImGui_ImplDX11_NewFrame();
	}
	
	//--------------------------------------------------------------------
	// private
	//--------------------------------------------------------------------

	// デバイス初期化
	HRESULT DXSystem::InitDevice(HWND hWnd){
		HRESULT hr = S_OK;

		m_hWnd = hWnd;

		// ウィンドウサイズ取得
		RECT rc;
		GetClientRect(hWnd, &rc);
		m_windowWidth = rc.right - rc.left;
		m_windowHeight = rc.bottom - rc.top;

		UINT createDeviceFlags = 0;
	#if defined(_DEBUG) || defined(DEBUG)
		createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
	#endif

		// ドライバーフラグ
		D3D_DRIVER_TYPE driverTypes[] = {
			D3D_DRIVER_TYPE_HARDWARE,
			D3D_DRIVER_TYPE_WARP,
			D3D_DRIVER_TYPE_REFERENCE
		};
		UINT numDriverTypes = ARRAYSIZE(driverTypes);

		// 機能レベル
		D3D_FEATURE_LEVEL featureLevels[] = {
			D3D_FEATURE_LEVEL_11_0,
			D3D_FEATURE_LEVEL_11_1
		};
		UINT numFeatureLevels = ARRAYSIZE(featureLevels);

		// スワップチェイン設定
		DXGI_SWAP_CHAIN_DESC sd;
		ZeroMemory(&sd, sizeof(DXGI_SWAP_CHAIN_DESC));
		sd.BufferCount = 2;
		sd.BufferDesc.Width = m_windowWidth;
		sd.BufferDesc.Height = m_windowHeight;
		sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
		sd.BufferDesc.RefreshRate.Numerator = 60;
		sd.BufferDesc.RefreshRate.Denominator = 1;
		sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		sd.OutputWindow = hWnd;
		sd.SampleDesc.Count = 1;
		sd.SampleDesc.Quality = 0;
		sd.Windowed = true;

		// デバイスとスワップチェイン作成
		for( UINT i=0 ; i<numDriverTypes ; i++){
			D3D_DRIVER_TYPE driverType = driverTypes[i];
			hr = D3D11CreateDeviceAndSwapChain(
					nullptr,
					driverType,
					nullptr,
					createDeviceFlags,
					featureLevels,
					numFeatureLevels,
					D3D11_SDK_VERSION,
					&sd,
					&m_swapChain,
					&m_device,
					&m_featureLevel,
					&m_immediateContext);
			if( SUCCEEDED(hr) ){
				break;
			}
		}
		if( FAILED(hr) ){
			OUTPUT_CONSOLE_LINE("Failed... Create Device&SwapChain");
			return hr;
		}

		// レンダーターゲットビュー作成
		if( FAILED(CreateBackBufferRTV()) ){
			return E_FAIL;
		}

		// デプスステンシルビュー
		if( FAILED(CreateBackBufferDSV()) ){
			return E_FAIL;
		}

		// 出力マネージャーにレンダーターゲットビュー設定
		m_immediateContext->OMSetRenderTargets(1, &m_backBufferRTV, m_backBufferDSV);

		// ビューポート
		D3D11_VIEWPORT vp;
		vp.Width = static_cast<float>(m_windowWidth);
		vp.Height = static_cast<float>(m_windowHeight);
		vp.MinDepth = 0.0f;
		vp.MaxDepth = 1.0f;
		vp.TopLeftX = 0;
		vp.TopLeftY = 0;
		m_immediateContext->RSSetViewports(1, &vp);

		// imgui初期化
		ImGui_ImplDX11_Init(hWnd, m_device, m_immediateContext);

		return hr;
	}

	// バックバッファのレンダーターゲットビュー作成
	HRESULT DXSystem::CreateBackBufferRTV(){
		HRESULT hr;

		// バックバッファ取得
		ID3D11Texture2D* backBuffer = nullptr;
		hr = m_swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&backBuffer);
		if( FAILED(hr) ){
			OUTPUT_CONSOLE_LINE("Failed... Create BackBuffer");
			return E_FAIL;
		}

		// レンダーターゲットビュー作成
		hr = m_device->CreateRenderTargetView(backBuffer, nullptr, &m_backBufferRTV);
		backBuffer->Release();
		backBuffer = nullptr;
		if( FAILED(hr) ){
			OUTPUT_CONSOLE_LINE("Failed... Create RenderTargetView");
		}

		return hr;
	}

	// バックバッファのデプスステンシルビュー作成
	HRESULT DXSystem::CreateBackBufferDSV(){
		D3D11_TEXTURE2D_DESC depthStencilDesc;
		depthStencilDesc.Width = m_windowWidth;
		depthStencilDesc.Height = m_windowHeight;
		depthStencilDesc.MipLevels = 1;
		depthStencilDesc.ArraySize = 1;
		depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		depthStencilDesc.SampleDesc.Count = 1;
		depthStencilDesc.SampleDesc.Quality = 0;
		depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
		depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		depthStencilDesc.CPUAccessFlags = 0;
		depthStencilDesc.MiscFlags = 0;
		ID3D11Texture2D* depthStencil = nullptr;
		m_device->CreateTexture2D(&depthStencilDesc, nullptr, &depthStencil);
		D3D11_DEPTH_STENCIL_VIEW_DESC dsvd;
		ZeroMemory(&dsvd, sizeof(dsvd));
		dsvd.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		dsvd.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		dsvd.Texture2D.MipSlice = 0;

		HRESULT hr = S_OK;
		hr = m_device->CreateDepthStencilView(depthStencil, &dsvd, &m_backBufferDSV);
		depthStencil->Release();

		if( FAILED(hr) ){
			OUTPUT_CONSOLE_LINE("Failed... Create DepthStencilView");
		}

		return hr;
	}

	// ウィンドウサイズ変更
	void DXSystem::ResizeWindow(){
		// 起動時にWM_SIZEイベントが呼ばれるのでその対策
		if( !m_swapChain ){
			return;
		}

		// imguiの開放
		ImGui_ImplDX11_InvalidateDeviceObjects();

		// ウィンドウサイズ取得
		RECT rc;
		GetClientRect(m_hWnd, &rc);
		m_windowWidth = rc.right - rc.left;
		m_windowHeight = rc.bottom - rc.top;
		if( m_windowWidth == 0 && m_windowHeight == 0 ){
			return;
		}
		// 今あるバッファを削除
		m_immediateContext->OMSetRenderTargets(0, nullptr, nullptr);
		SAFE_RELEASE(m_backBufferRTV);
		SAFE_RELEASE(m_backBufferDSV);
		// バッファを再作成
		HRESULT hr = S_OK;
		hr = m_swapChain->ResizeBuffers(2, m_windowWidth, m_windowHeight, DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH);
		hr = CreateBackBufferRTV();
		hr = CreateBackBufferDSV();
		// 出力マネージャーにレンダーターゲットビュー設定
		m_immediateContext->OMSetRenderTargets(1, &m_backBufferRTV, m_backBufferDSV);
		// ビューポート
		D3D11_VIEWPORT vp;
		vp.Width = static_cast<float>(m_windowWidth);
		vp.Height = static_cast<float>(m_windowHeight);
		vp.MinDepth = 0.0f;
		vp.MaxDepth = 1.0f;
		vp.TopLeftX = 0;
		vp.TopLeftY = 0;
		m_immediateContext->RSSetViewports(1, &vp);

		// imguiの再初期化
		ImGui_ImplDX11_CreateDeviceObjects();
	}
}

