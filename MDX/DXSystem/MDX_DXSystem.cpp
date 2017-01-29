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

	// ������
	bool DXSystem::Init(HWND hWnd){
		if( FAILED(InitDevice(hWnd)) ){
			return false;
		}
		return true;
	}

	// ���
	void DXSystem::Release(){
		// imgui�J��
		ImGui_ImplDX11_Shutdown();

		SAFE_RELEASE(m_backBufferRTV);
		SAFE_RELEASE(m_backBufferDSV);
		SAFE_RELEASE(m_swapChain);
		SAFE_RELEASE(m_device);
		SAFE_RELEASE(m_immediateContext);
	}
	
	// �X�V
	void DXSystem::Update() {
		// imgui�X�V
		ImGui_ImplDX11_NewFrame();
	}
	
	//--------------------------------------------------------------------
	// private
	//--------------------------------------------------------------------

	// �f�o�C�X������
	HRESULT DXSystem::InitDevice(HWND hWnd){
		HRESULT hr = S_OK;

		m_hWnd = hWnd;

		// �E�B���h�E�T�C�Y�擾
		RECT rc;
		GetClientRect(hWnd, &rc);
		m_windowWidth = rc.right - rc.left;
		m_windowHeight = rc.bottom - rc.top;

		UINT createDeviceFlags = 0;
	#if defined(_DEBUG) || defined(DEBUG)
		createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
	#endif

		// �h���C�o�[�t���O
		D3D_DRIVER_TYPE driverTypes[] = {
			D3D_DRIVER_TYPE_HARDWARE,
			D3D_DRIVER_TYPE_WARP,
			D3D_DRIVER_TYPE_REFERENCE
		};
		UINT numDriverTypes = ARRAYSIZE(driverTypes);

		// �@�\���x��
		D3D_FEATURE_LEVEL featureLevels[] = {
			D3D_FEATURE_LEVEL_11_0,
			D3D_FEATURE_LEVEL_11_1
		};
		UINT numFeatureLevels = ARRAYSIZE(featureLevels);

		// �X���b�v�`�F�C���ݒ�
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

		// �f�o�C�X�ƃX���b�v�`�F�C���쐬
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

		// �����_�[�^�[�Q�b�g�r���[�쐬
		if( FAILED(CreateBackBufferRTV()) ){
			return E_FAIL;
		}

		// �f�v�X�X�e���V���r���[
		if( FAILED(CreateBackBufferDSV()) ){
			return E_FAIL;
		}

		// �o�̓}�l�[�W���[�Ƀ����_�[�^�[�Q�b�g�r���[�ݒ�
		m_immediateContext->OMSetRenderTargets(1, &m_backBufferRTV, m_backBufferDSV);

		// �r���[�|�[�g
		D3D11_VIEWPORT vp;
		vp.Width = static_cast<float>(m_windowWidth);
		vp.Height = static_cast<float>(m_windowHeight);
		vp.MinDepth = 0.0f;
		vp.MaxDepth = 1.0f;
		vp.TopLeftX = 0;
		vp.TopLeftY = 0;
		m_immediateContext->RSSetViewports(1, &vp);

		// imgui������
		ImGui_ImplDX11_Init(hWnd, m_device, m_immediateContext);

		return hr;
	}

	// �o�b�N�o�b�t�@�̃����_�[�^�[�Q�b�g�r���[�쐬
	HRESULT DXSystem::CreateBackBufferRTV(){
		HRESULT hr;

		// �o�b�N�o�b�t�@�擾
		ID3D11Texture2D* backBuffer = nullptr;
		hr = m_swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&backBuffer);
		if( FAILED(hr) ){
			OUTPUT_CONSOLE_LINE("Failed... Create BackBuffer");
			return E_FAIL;
		}

		// �����_�[�^�[�Q�b�g�r���[�쐬
		hr = m_device->CreateRenderTargetView(backBuffer, nullptr, &m_backBufferRTV);
		backBuffer->Release();
		backBuffer = nullptr;
		if( FAILED(hr) ){
			OUTPUT_CONSOLE_LINE("Failed... Create RenderTargetView");
		}

		return hr;
	}

	// �o�b�N�o�b�t�@�̃f�v�X�X�e���V���r���[�쐬
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

	// �E�B���h�E�T�C�Y�ύX
	void DXSystem::ResizeWindow(){
		// �N������WM_SIZE�C�x���g���Ă΂��̂ł��̑΍�
		if( !m_swapChain ){
			return;
		}

		// imgui�̊J��
		ImGui_ImplDX11_InvalidateDeviceObjects();

		// �E�B���h�E�T�C�Y�擾
		RECT rc;
		GetClientRect(m_hWnd, &rc);
		m_windowWidth = rc.right - rc.left;
		m_windowHeight = rc.bottom - rc.top;
		if( m_windowWidth == 0 && m_windowHeight == 0 ){
			return;
		}
		// ������o�b�t�@���폜
		m_immediateContext->OMSetRenderTargets(0, nullptr, nullptr);
		SAFE_RELEASE(m_backBufferRTV);
		SAFE_RELEASE(m_backBufferDSV);
		// �o�b�t�@���č쐬
		HRESULT hr = S_OK;
		hr = m_swapChain->ResizeBuffers(2, m_windowWidth, m_windowHeight, DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH);
		hr = CreateBackBufferRTV();
		hr = CreateBackBufferDSV();
		// �o�̓}�l�[�W���[�Ƀ����_�[�^�[�Q�b�g�r���[�ݒ�
		m_immediateContext->OMSetRenderTargets(1, &m_backBufferRTV, m_backBufferDSV);
		// �r���[�|�[�g
		D3D11_VIEWPORT vp;
		vp.Width = static_cast<float>(m_windowWidth);
		vp.Height = static_cast<float>(m_windowHeight);
		vp.MinDepth = 0.0f;
		vp.MaxDepth = 1.0f;
		vp.TopLeftX = 0;
		vp.TopLeftY = 0;
		m_immediateContext->RSSetViewports(1, &vp);

		// imgui�̍ď�����
		ImGui_ImplDX11_CreateDeviceObjects();
	}
}

