/**
* @file MDX_Scene.h
* @brief シーンの定義
* @author Toru Miyawaki
*/

#ifndef MDX_SCENE_H
#define MDX_SCENE_H

/**
* @brief namespace MDX
*/
namespace MDX{
	/**
	* @brief シーン基底クラス
	* このクラスを継承するクラスはシーンとして登録する事が出来るようになる
	*/
	class Scene{
	public:
		virtual ~Scene(){}

		/**
		* @brief 初期化
		* @return 初期化終了か
		*/
		virtual bool Init() = 0;
		/**
		* @brief 解放
		* @return 解放終了か
		*/
		virtual bool Release() = 0;
		/**
		* @brief 更新
		* @return 更新終了か
		*/
		virtual bool Update() = 0;
		/**
		* @brief 描画
		*/
		virtual void Draw() = 0;
	private:
	};

}

#endif //! MDX_SCENE_H