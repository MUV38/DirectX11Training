/**
* @file MDX_SceneManager.h
* @brief シーンマネージャーの定義
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
	* @brief シーン管理クラス
	*/
	class SceneManager{
	public:
		SceneManager();
		~SceneManager();

		/**
		* @brief 初期化
		* @return 初期化成功か
		*/
		bool Init();
		/**
		* @brief 解放
		*/
		void Release();
		/**
		* @brief 更新
		*/
		void Update();
		/**
		* @brief 描画
		*/
		void Draw();
		/**
		* @brief シーンの切り替え
		*/

		void ChangeScene(Scene* sceneNext);

	private:
		/**
		* @brief SceneManagerの更新の状態
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