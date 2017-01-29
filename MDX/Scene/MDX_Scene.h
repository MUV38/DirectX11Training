/**
* @file MDX_Scene.h
* @brief �V�[���̒�`
* @author Toru Miyawaki
*/

#ifndef MDX_SCENE_H
#define MDX_SCENE_H

/**
* @brief namespace MDX
*/
namespace MDX{
	/**
	* @brief �V�[�����N���X
	* ���̃N���X���p������N���X�̓V�[���Ƃ��ēo�^���鎖���o����悤�ɂȂ�
	*/
	class Scene{
	public:
		virtual ~Scene(){}

		/**
		* @brief ������
		* @return �������I����
		*/
		virtual bool Init() = 0;
		/**
		* @brief ���
		* @return ����I����
		*/
		virtual bool Release() = 0;
		/**
		* @brief �X�V
		* @return �X�V�I����
		*/
		virtual bool Update() = 0;
		/**
		* @brief �`��
		*/
		virtual void Draw() = 0;
	private:
	};

}

#endif //! MDX_SCENE_H