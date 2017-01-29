/**
* @file MDX_View.h
* @brief ビュークラスの定義
* @author Toru Miyawaki
*/

#ifndef MDX_VIEW_H
#define MDX_VIEW_H

#include <DirectXMath.h>

/**
* @brief namespace MDX
*/
namespace MDX{
	class View{
	public:
		View();
		~View();

		/**
		* @brief ビュー情報適用
		*/
		void Apply();

		/**
		* @brief ゲッター
		*/
		DirectX::XMFLOAT4X4 GetMatView() const { return m_matView; }
		DirectX::XMFLOAT4X4 GetMatPrj() const { return m_matPrj; }

		DirectX::XMFLOAT3 GetViewPos() const { return m_viewPos; }
		DirectX::XMFLOAT3 GetViewTarget() const { return m_viewTarget; }
		DirectX::XMFLOAT3 GetViewUp() const { return m_viewUp; }

		float GetFovY() const { return m_fovY; }
		float GetAspect() const { return m_aspect; }
		float GetNearZ() const { return m_nearZ; }
		float GetFarZ() const { return m_farZ; }

		/**
		* @brief セッター
		*/
		void SetView(const DirectX::XMFLOAT3& pos, const DirectX::XMFLOAT3& target, const DirectX::XMFLOAT3& up=DirectX::XMFLOAT3(0, 1, 0));
		void SetProjection(float fovY, float aspect, float nearZ, float farZ);

	private:
		DirectX::XMFLOAT4X4 m_matView;
		DirectX::XMFLOAT4X4 m_matPrj;

		DirectX::XMFLOAT3 m_viewPos;
		DirectX::XMFLOAT3 m_viewTarget;
		DirectX::XMFLOAT3 m_viewUp;

		float m_fovY;
		float m_aspect;
		float m_nearZ;
		float m_farZ;
	};
}

#endif // !MDX_VIEW_H
