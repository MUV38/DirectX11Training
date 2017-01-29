#ifndef _MDX_DXTKMESH_H_
#define	_MDX_DXTKMESH_H_

#include <memory>

#include <Model.h>
#include <Effects.h>

#include "MDX_BaseMesh.h"

/**
* @brief namespace MDX
*/
namespace MDX
{
	/**
	* @brief DirectXTK���b�V���N���X
	*/
	class DXTKMesh : public BaseMesh
	{
	public:
		DXTKMesh();
		~DXTKMesh();

		/**
		* @brief �t�@�C���ǂݍ���
		* @param [in] device	�f�o�C�X
		* @param [in] filename	�t�@�C����
		* @return �ǂݍ��ݐ������H
		*/
		bool Load(const char* filename) override;

		/**
		* @brief ���
		*/
		void Release() override;

		/**
		* @brief �X�V
		*/
		void Update() override;

		/**
		* @brief �`��
		*/
		void Draw() override;

	private:

	private:
		std::shared_ptr<DirectX::Model>			m_model;
		std::unique_ptr<DirectX::EffectFactory>	m_effectFactory;
	};
}

#endif // !_MDX_DXTKMESH_H_
