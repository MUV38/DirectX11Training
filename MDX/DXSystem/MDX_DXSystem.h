/**
* @file MDX_System.h
* @brief DirectX�̒��j���Ǘ�����ꏊ
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
	* @brief DirectX�̒��j���Ǘ�����N���X
	*/
	class DXSystem{
	public:
		static const int MRT_MAX = 8;

	public:
		DXSystem();
		~DXSystem();

		/**
		* @brief ������
		* @param [in] hWnd �E�B���h�E�n���h��
		* @return �������������H
		*/
		bool Init(HWND hWnd);

		/**
		* @brief ���
		*/
		void Release();

		/**
		* @brief �X�V
		*/
		void Update();

	public:
		/**
		* @brief �f�o�C�X�擾
		* @return �f�o�C�X�̃|�C���^�[
		*/
		ID3D11Device* GetDevice() const { return m_device; }
		/**
		* @brief �X���b�v�`�F�C���擾
		* @return �X���b�v�`�F�C���̃|�C���^�[
		*/
		IDXGISwapChain* GetSwapChain() const { return m_swapChain; }
		/**
		* @brief �f�o�C�X�R���e�L�X�g�擾
		* @return �f�o�C�X�R���e�L�X�g�̃|�C���^�[
		*/
		ID3D11DeviceContext* GetImmediateContext() const { return m_immediateContext; }

		/**
		* @brief �o�b�N�o�b�t�@��RenderTargetView�擾
		* @retunr �o�b�N�o�b�t�@��RenderTargetView
		*/
		ID3D11RenderTargetView* GetBackBufferRTV() const { return m_backBufferRTV; }
		
		/**
		* @brief �o�b�N�o�b�t�@��DepthStencilView�擾
		* @retunr �o�b�N�o�b�t�@��DepthStencilView
		*/
		ID3D11DepthStencilView* GetBackBufferDSV() const { return m_backBufferDSV; }

		/**
		* @brief �E�B���h�E�T�C�Y�擾
		* @retun �E�B���h�E�T�C�Y
		*/
		DirectX::XMUINT2 GetWindowSize() const { return DirectX::XMUINT2(m_windowWidth, m_windowHeight); }

		/**
		* @brief �E�B���h�E�T�C�Y�ύX
		*/
		void ResizeWindow();
	private:
		/**
		* @brief �f�o�C�X������
		* @param [in] hWnd �E�B���h�E�n���h��
		* @return �������������H
		*/
		HRESULT InitDevice(HWND hWnd);

		/**
		* @brief �o�b�N�o�b�t�@�̃����_�[�^�[�Q�b�g�r���[�쐬
		*/
		HRESULT CreateBackBufferRTV();

		/**
		* @brief �o�b�N�o�b�t�@�̃f�v�X�X�e���V���r���[�쐬
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