/**
* @file MDX_Box.h
* @brief ���N���X
* @author Toru Miyawaki
*/

#ifndef MDX_BOX_H
#define MDX_BOX_H

#include <d3d11.h>
#include <DirectXMath.h>
#include "../../Shader/MDX_Shader.h"

/**
* @brief namespace MDX
*/
namespace MDX{
	/**
	* @brief ���N���X
	*/
	class Box{
	public:
		Box();
		~Box();
		
		/**
		* @brief ���쐬
		* @return �쐬������?
		*/
		bool Create();

		/**
		* @brief ���
		*/
		void Release();

		/**
		* @brief �`��
		*/
		void Draw();
		/**
		* @biref �Q�b�^�[
		*/
		DirectX::XMFLOAT3 GetPos() const { return m_pos; }
		DirectX::XMFLOAT3 GetAngle() const { return m_angle; }
		DirectX::XMFLOAT3 GetScale() const { return m_scale; }
		/**
		* @brief �Z�b�^�[
		*/
		void SetPos(DirectX::XMFLOAT3& pos){ m_pos = pos; }
		void SetAngle(DirectX::XMFLOAT3& angle){ m_angle = angle; }
		void SetScale(DirectX::XMFLOAT3& scale){ m_scale = scale; }

	private:
		/**
		* @brief �ϊ��s��R���X�^���g�o�b�t�@
		*/
		struct CBMatrix{
			DirectX::XMFLOAT4X4 matWorld;
			DirectX::XMFLOAT4X4 matView;
			DirectX::XMFLOAT4X4 matProjection;
		};

	private:
		ID3D11Buffer* m_vertexBuffer = nullptr;
		ID3D11Buffer* m_indexBuffer = nullptr;
		ID3D11Buffer* m_cbMatrix = nullptr;

		DirectX::XMFLOAT3 m_pos = DirectX::XMFLOAT3(0, 0, 0);
		DirectX::XMFLOAT3 m_angle = DirectX::XMFLOAT3(0, 0, 0);
		DirectX::XMFLOAT3 m_scale = DirectX::XMFLOAT3(1, 1, 1);

		Shader m_shader;
	};
}

#endif // !MDX_BOX_H