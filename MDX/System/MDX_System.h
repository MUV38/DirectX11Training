/**
* @file MDX_System.h
* @brief MDXのシステム管理関連を定義したファイル
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
	* @brief MDXのシステム管理クラス
	*/
	class System : public Singleton<System>{
	public:
		System();
		~System();

		/**
		* @brief 初期化
		* @param [in] hWnd ウィンドウハンドル
		* @return 初期化成功か？
		*/
		bool Init(HWND hWnd);

		/**
		* @brief 解放
		* @return 解放終了か？
		*/
		bool Release();

		/**
		* @brief 更新
		*/
		void Update();

		/**
		* @brief 描画
		*/
		void Draw();

		/**
		* @brief DXシステムの取得
		* @return DXシステムのポインタ
		*/
		DXSystem* GetDXSystem() const { return m_DXSystem; }

		/**
		* @brief シーンマネージャーの取得
		* @return シーンマネージャーのポインタ
		*/
		SceneManager* GetSceneManager() const { return m_sceneManager; }

		/**
		* @brief レンダーマネージャーの取得
		* @return レンダーマネージャーのポインタ
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

// 短縮用define
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