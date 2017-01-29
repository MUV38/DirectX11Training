/**
* @file SceneMain.h
* @brief ���C���V�[���̒�`
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
	* @brief ������
	*/
	bool Init() override;

	/**
	* @brief ���
	*/
	bool Release() override;

	/**
	* @brief �X�V
	*/
	bool Update() override;

	/**
	* @brief �`��
	*/
	void Draw() override;
private:

};

#endif // !SCENE_MAIN_H
