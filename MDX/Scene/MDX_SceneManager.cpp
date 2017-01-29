#include "MDX_SceneManager.h"
#include "MDX_Scene.h"
#include "../Utillity/MDX_Utillity.h"
#include "../Utillity/MDX_UtilDebug.h"
#include <assert.h>

namespace MDX{
	//---------------------------------------------------------------
	// public
	//---------------------------------------------------------------
	SceneManager::SceneManager(){
	}
	SceneManager::~SceneManager(){
		Release();
	}

	// 初期化
	bool SceneManager::Init(){
		return true;
	}

	// 解放
	void SceneManager::Release(){
		SAFE_DELETE(m_scene);
		SAFE_DELETE(m_sceneNext);
	}

	// 更新
	void SceneManager::Update(){
		// シーンの更新
		switch (m_stateUpdate){
		case UPDATE_STATE_INIT:
			if( m_scene->Init() ){
				m_stateUpdate = UPDATE_STATE_UPDATE;
			}
			break;
		case UPDATE_STATE_UPDATE:
			if( m_scene->Update() ){
				m_stateUpdate = UPDATE_STATE_RELEASE;
			}
			break;
		case UPDATE_STATE_RELEASE:
			if( m_scene->Release() ){
				m_stateUpdate = UPDATE_STATE_IDLE;
			}
			break;
		case UPDATE_STATE_IDLE:
			if( m_sceneNext ){
				SAFE_DELETE(m_scene);
				m_scene = m_sceneNext;
				m_sceneNext = nullptr;
				m_stateUpdate = UPDATE_STATE_INIT;
			}
			break;
		default:
			assert(0);
			OUTPUT_CONSOLE_LINE("mdxSceneManager::Updateのシーン更新で不正なState");
			break;
		}

	}

	// 描画
	void SceneManager::Draw(){
		if( m_scene ){
			m_scene->Draw();
		}
	}

	// シーンの切り替え
	void SceneManager::ChangeScene(Scene* sceneNext){
		if( sceneNext ){
			if( m_scene ){
				m_stateUpdate = UPDATE_STATE_RELEASE;
				m_sceneNext = sceneNext;
			}else{
				m_scene = sceneNext;
				m_sceneNext = nullptr;
				m_stateUpdate = UPDATE_STATE_INIT;
			}
		}
	}
}