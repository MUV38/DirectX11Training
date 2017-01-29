/**
* @file MDX_System.h
* @brief MDX�̃V�X�e���Ǘ��֘A���`�����t�@�C��
* @author Toru Miyawaki
*/

#ifndef MDX_SYSTEM_H
#define MDX_SYSTEM_H

#include <Windows.h>
#include "../Utillity/MDX_Singleton.h"
#include "../DXSystem/MDX_DXSystem.h"
#include "../Render/MDX_RenderManager.h"

/**
* @brief namespace MDX
*/
namespace MDX{
	class SceneManager;
	class DebugConsoleWindow;

	/**
	* @brief MDX�̃V�X�e���Ǘ��N���X
	*/
	class System : public Singleton<System>{
	public:
		System();
		~System();

		/**
		* @brief ������
		* @param [in] hWnd �E�B���h�E�n���h��
		* @return �������������H
		*/
		bool Init(HWND hWnd);

		/**
		* @brief ���
		* @return ����I�����H
		*/
		bool Release();

		/**
		* @brief �X�V
		*/
		void Update();

		/**
		* @brief �`��
		*/
		void Draw();

		/**
		* @brief DX�V�X�e���̎擾
		* @return DX�V�X�e���̃|�C���^
		*/
		DXSystem* GetDXSystem() const { return m_DXSystem; }

		/**
		* @brief �V�[���}�l�[�W���[�̎擾
		* @return �V�[���}�l�[�W���[�̃|�C���^
		*/
		SceneManager* GetSceneManager() const { return m_sceneManager; }

		/**
		* @brief �����_�[�}�l�[�W���[�̎擾
		* @return �����_�[�}�l�[�W���[�̃|�C���^
		*/
		RenderManager* GetRenderManager() const { return m_renderManager; }
	private:
		HWND m_hWnd;

		DXSystem* m_DXSystem = nullptr;
		SceneManager* m_sceneManager = nullptr;
		RenderManager* m_renderManager = nullptr;

#if defined(_DEBUG) || defined(DEBUG)
		DebugConsoleWindow* m_consoleWindow = nullptr;
#endif
	};
}

// �Z�k�pdefine
#define MDX_GET_SYSTEM (MDX::System::GetInstance())
#define MDX_GET_SCENE_MANAGER (MDX_GET_SYSTEM.GetSceneManager())
#define MDX_GET_RENDER_MANAGER (MDX_GET_SYSTEM.GetRenderManager())
#define MDX_GET_DX_SYSTEM (MDX::System::GetInstance().GetDXSystem())
#define MDX_GET_DEVICE (MDX_GET_DX_SYSTEM->GetDevice())
#define MDX_GET_SWAP_CHAIN (MDX_GET_DX_SYSTEM->GetSwapChain())
#define MDX_GET_IMMEDIATE_CONTEXT (MDX_GET_DX_SYSTEM->GetImmediateContext())
#define MDX_GET_BACK_BUFFER_RTV (MDX_GET_DX_SYSTEM->GetBackBufferRTV())
#define MDX_GET_BACK_BUFFER_DSV (MDX_GET_DX_SYSTEM->GetBackBufferDSV())


#endif //! MDX_SYSTEM_H