/**
* @file SceneMain.h
* @brief メインシーンの定義
* @author Toru Miyawaki
*/

#ifndef SCENE_MAIN_H
#define SCENE_MAIN_H

#include "../../MDX/Scene/MDX_Scene.h"

class SceneMain : public MDX::Scene
{
public:
	SceneMain();
	~SceneMain();

	/**
	* @brief 初期化
	*/
	bool Init() override;

	/**
	* @brief 解放
	*/
	bool Release() override;

	/**
	* @brief 更新
	*/
	bool Update() override;

	/**
	* @brief 描画
	*/
	void Draw() override;
private:

};

#endif // !SCENE_MAIN_H
