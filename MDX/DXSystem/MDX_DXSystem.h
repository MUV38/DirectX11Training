/**
* @file MDX_System.h
* @brief DirectXの中核を管理する場所
* @author Toru Miyawaki
*/

#ifndef MDX_DXSYSTEM_H
#define MDX_DXSYSTEM_H

#include <d3d11.h>
#include <Windows.h>
#include <DirectXMath.h>
#include "../Math/MDX_Math.h"
#include "../Render/MDX_RenderTarget.h"

/**
* @brief namespace MDX
*/
namespace MDX{
	/**
	* @brief DirectXの中核を管理するクラス
	*/
	class DXSystem{
	public:
		static const int MRT_MAX = 8;

	public:
		DXSystem();
		~DXSystem();

		/**
		* @brief 初期化
		* @param [in] hWnd ウィンドウハンドル
		* @return 初期化成功か？
		*/
		bool Init(HWND hWnd);

		/**
		* @brief 解放
		*/
		void Release();

		/**
		* @brief 更新
		*/
		void Update();

	public:
		/**
		* @brief デバイス取得
		* @return デバイスのポインター
		*/
		ID3D11Device* GetDevice() const { return m_device; }
		/**
		* @brief スワップチェイン取得
		* @return スワップチェインのポインター
		*/
		IDXGISwapChain* GetSwapChain() const { return m_swapChain; }
		/**
		* @brief デバイスコンテキスト取得
		* @return デバイスコンテキストのポインター
		*/
		ID3D11DeviceContext* GetImmediateContext() const { return m_immediateContext; }

		/**
		* @brief バックバッファのRenderTargetView取得
		* @retunr バックバッファのRenderTargetView
		*/
		ID3D11RenderTargetView* GetBackBufferRTV() const { return m_backBufferRTV; }
		
		/**
		* @brief バックバッファのDepthStencilView取得
		* @retunr バックバッファのDepthStencilView
		*/
		ID3D11DepthStencilView* GetBackBufferDSV() const { return m_backBufferDSV; }

		/**
		* @brief ウィンドウサイズ取得
		* @retun ウィンドウサイズ
		*/
		DirectX::XMUINT2 GetWindowSize() const { return DirectX::XMUINT2(m_windowWidth, m_windowHeight); }

		/**
		* @brief ウィンドウサイズ変更
		*/
		void ResizeWindow();
	private:
		/**
		* @brief デバイス初期化
		* @param [in] hWnd ウィンドウハンドル
		* @return 初期化成功か？
		*/
		HRESULT InitDevice(HWND hWnd);

		/**
		* @brief バックバッファのレンダーターゲットビュー作成
		*/
		HRESULT CreateBackBufferRTV();

		/**
		* @brief バックバッファのデプスステンシルビュー作成
		*/
		HRESULT CreateBackBufferDSV();

	private:

		IDXGISwapChain* m_swapChain = nullptr;
		ID3D11Device* m_device = nullptr;
		ID3D11DeviceContext* m_immediateContext = nullptr;
		D3D_FEATURE_LEVEL m_featureLevel = D3D_FEATURE_LEVEL_11_0;

		ID3D11RenderTargetView* m_backBufferRTV = nullptr;
		ID3D11DepthStencilView* m_backBufferDSV = nullptr;

		HWND m_hWnd = 0;

		UINT m_windowWidth = 0;
		UINT m_windowHeight = 0;
	};
}

#endif // MDX_DXSYSTEM_H