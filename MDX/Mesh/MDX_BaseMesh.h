#ifndef _MDX_BASE_MESH_H
#define	_MDX_BASE_MESH_H

#include "../Render/MDX_BaseActor.h"

/**
* @brief namespace MDX
*/
namespace MDX
{
	/**
	* @brief ���b�V�����N���X
	*/
	class BaseMesh : public BaseActor
	{
	public:
		BaseMesh(){}
		virtual ~BaseMesh(){}

		/**
		* @brief �t�@�C���ǂݍ���
		* @param [in] filename �t�@�C����
		* @return �ǂݍ��ݐ������H
		*/
		virtual bool Load(const char* filename) = 0;

		/**
		* @brief ���
		*/
		virtual void Release() = 0;

		/**
		* @brief �X�V
		*/
		virtual void Update() = 0;

		/**
		* @brief �`��
		*/
		virtual void Draw() = 0;

	private:

	};
}

#endif // !_MDX_BASE_MESH_H
