/**
* @file MDX_SceneManager.h
* @brief �V�[���}�l�[�W���[�̒�`
* @author Toru Miyawaki
*/

#ifndef MDX_SCENE_MANAGER_H
#define MDX_SCENE_MANAGER_H

/**
* @brief namespace MDX
*/
namespace MDX{
	class Scene;

	/**
	* @brief �V�[���Ǘ��N���X
	*/
	class SceneManager{
	public:
		SceneManager();
		~SceneManager();

		/**
		* @brief ������
		* @return ������������
		*/
		bool Init();
		/**
		* @brief ���
		*/
		void Release();
		/**
		* @brief �X�V
		*/
		void Update();
		/**
		* @brief �`��
		*/
		void Draw();
		/**
		* @brief �V�[���̐؂�ւ�
		*/

		void ChangeScene(Scene* sceneNext);

	private:
		/**
		* @brief SceneManager�̍X�V�̏��
		*/
		enum UpdateStateType{
			UPDATE_STATE_INIT,
			UPDATE_STATE_UPDATE,
			UPDATE_STATE_RELEASE,
			UPDATE_STATE_IDLE,
			UPDATE_STATE_MAX
		};
	private:
		Scene* m_scene = nullptr;
		Scene* m_sceneNext = nullptr;
		UpdateStateType m_stateUpdate = UPDATE_STATE_IDLE;
	};

}

#endif //! MDX_SCENE_MANAGER_H