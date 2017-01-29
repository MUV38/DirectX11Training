#include "MDX_System.h"
#include "../Scene/MDX_SceneManager.h"
#include "../Utillity/MDX_UtilDebug.h"
#include "../../SourceFiles/Scene/SceneMain.h"
#include "../Render/MDX_BaseActor.h"
#include "../RenderState/MDX_RenderState.h"
#include "../Render/MDX_Render.h"
#include "../2D/MDX_Sprite.h"

#include <imgui/imgui.h>

namespace MDX{
	System::System(){
		
	}

	System::~System(){
		Release();
	}

	// 初期化
	bool System::Init(HWND hWnd){
		// デバッグ用コンソール開く
#if defined(_DEBUG) || defined(DEBUG)
		m_consoleWindow = new DebugConsoleWindow();
		m_consoleWindow->Open();
#endif

		// DirectXのシステム起動
		m_DXSystem = new DXSystem();
		if( !m_DXSystem->Init(hWnd) ){
			return false;
		}

		// レンダーステート
		if( !RenderState::Initialize() ){
			return false;
		}

		// スプライト
		if( !Sprite::Initialize() ){
			return false;
		}

		// シーンマネージャー
		m_sceneManager = new SceneManager();
		if( !m_sceneManager->Init() ){
			return false;
		}
		m_sceneManager->ChangeScene(new SceneMain());

		// レンダーマネージャー
		m_renderManager = new RenderManager();
		if( !m_renderManager->Init() ){
			return false;
		}


		// 常駐データ初期化
		BaseActor::InitResidentData();

		return true;
	}

	// 解放
	bool System::Release(){
		if( m_DXSystem ){
			m_DXSystem->Release();
			delete m_DXSystem;
			m_DXSystem = nullptr;
		}
		// レンダーステート
		RenderState::Release();

		// スプライト
		Sprite::Release();

		if( m_sceneManager ){
			m_sceneManager->Release();
			delete m_sceneManager;
			m_sceneManager = nullptr;
		}
		if( m_renderManager ){
			delete m_renderManager;
			m_renderManager = nullptr;
		}
		BaseActor::ReleaseResidentData();


#if defined(_DEBUG) || defined(DEBUG)
		if( m_consoleWindow ){
			m_consoleWindow->Close();
			delete m_consoleWindow;
			m_consoleWindow = nullptr;
		}
#endif
		return true;
	}

	// 更新
	void System::Update(){
		m_sceneManager->Update();
		m_renderManager->Update();
		RenderState::Update();
	}

	// 描画
	void System::Draw(){
		m_renderManager->Draw();
		m_sceneManager->Draw();

		ImGui::Render();

		MDX_GET_SWAP_CHAIN->Present(0, 0);
	}
}