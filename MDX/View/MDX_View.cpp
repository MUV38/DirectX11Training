#include "MDX_View.h"
#include "../System/MDX_System.h"

namespace MDX{
	View::View() : 
		m_viewPos(0, 0, 10),
		m_viewTarget(0, 0, 0),
		m_viewUp(0, 1, 0),
		m_fovY(DirectX::XM_PI*0.25f),
		m_aspect(1280.0f/720.0f),
		m_nearZ(1.0f),
		m_farZ(1000.0f)
	{
		DirectX::XMMATRIX view = DirectX::XMMatrixLookAtLH(DirectX::XMLoadFloat3(&m_viewPos), DirectX::XMLoadFloat3(&m_viewTarget), DirectX::XMLoadFloat3(&m_viewUp));
		view = DirectX::XMMatrixTranspose(view);
		DirectX::XMStoreFloat4x4(&m_matView, view);
		DirectX::XMMATRIX prj = DirectX::XMMatrixPerspectiveFovLH(m_fovY, m_aspect, m_nearZ, m_farZ);
		prj = DirectX::XMMatrixTranspose(prj);
		DirectX::XMStoreFloat4x4(&m_matPrj, prj);
	}

	View::~View(){
		
	}

	// 適用
	void View::Apply(){
		MDX_GET_RENDER_MANAGER->SetMatView(m_matView);
		MDX_GET_RENDER_MANAGER->SetMatProjection(m_matPrj);
	}

	// セッター
	void View::SetView(const DirectX::XMFLOAT3& pos, const DirectX::XMFLOAT3& target, const DirectX::XMFLOAT3& up){
		m_viewPos = pos;
		m_viewTarget = target;
		m_viewUp = up;
		DirectX::XMMATRIX view = DirectX::XMMatrixLookAtLH(DirectX::XMLoadFloat3(&m_viewPos), DirectX::XMLoadFloat3(&m_viewTarget), DirectX::XMLoadFloat3(&m_viewUp));
		view = DirectX::XMMatrixTranspose(view);
		DirectX::XMStoreFloat4x4(&m_matView, view);
	}
	// ゲッター
	void View::SetProjection(float fovY, float aspect, float nearZ, float farZ){
		m_fovY = fovY;
		m_aspect = aspect;
		m_nearZ = nearZ;
		m_farZ = farZ;
		DirectX::XMMATRIX prj = DirectX::XMMatrixPerspectiveFovLH(m_fovY, m_aspect, m_nearZ, m_farZ);
		prj = DirectX::XMMatrixTranspose(prj);
		DirectX::XMStoreFloat4x4(&m_matPrj, prj);
	}

}