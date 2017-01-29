#include "MDX_RenderManager.h"
#include "../Utillity/MDX_Utillity.h"
#include "../System/MDX_System.h"
#include "MDX_BaseActor.h"
#include "../2D/MDX_Sprite.h"
#include "MDX_Render.h"

namespace MDX{
	RenderManager::RenderManager(){
	}

	RenderManager::~RenderManager(){
		Release();
	}

	// 初期化
	bool RenderManager::Init(){
		bool isSucceeded = true;
		HRESULT hr = S_OK;

		// シェーダー
		m_shader.reset(new Shader);
		isSucceeded = m_shader->LoadVSFromCso("VS_Primitive", MDX::Shader::VERTEX_LAYOUT_3D);
		assert(isSucceeded && "MDX::BaseActor頂点シェーダー初期化失敗");
		isSucceeded = m_shader->LoadPSFromCso("PS_Primitive");
		assert(isSucceeded && "MDX::BaseActorピクセルシェーダー初期化失敗");

		// ポストエフェクトシェーダー
		m_shaderPostEffect.reset(new Shader);
		isSucceeded = m_shaderPostEffect->LoadVSFromCso("VS_PostEffect", MDX::Shader::VERTEX_LAYOUT_2D);
		assert(isSucceeded && "MDX::BaseActor頂点シェーダー初期化失敗");
		isSucceeded = m_shaderPostEffect->LoadPSFromCso("PS_PostEffect");
		assert(isSucceeded && "MDX::BaseActorピクセルシェーダー初期化失敗");

		// イラディアンステクスチャ
		m_defaultIrradianceTex.reset(new Texture());
		{
			MDX::Texture::LoadInfo loadInfo;
			{
				loadInfo.filename = "DATA\\IBL\\default_irradiance.dds";
				loadInfo.forceSRGB = true;
			}
			m_defaultIrradianceTex->Load(loadInfo);
		}

		// ラディアンステクスチャ
		m_defaultRadianceTex.reset(new Texture());
		{
			MDX::Texture::LoadInfo loadInfo;
			{
				loadInfo.filename = "DATA\\IBL\\default_radiance.dds";
				loadInfo.forceSRGB = true;
			}
			m_defaultRadianceTex->Load(loadInfo);
		}

		// 変換行列用コンスタントバッファ
		D3D11_BUFFER_DESC bd;
		ZeroMemory(&bd, sizeof(D3D11_BUFFER_DESC));
		bd.Usage = D3D11_USAGE_DYNAMIC;
		bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		bd.MiscFlags = 0;
		bd.StructureByteStride = 0;
		bd.ByteWidth = sizeof(CBMatrix);
		hr = MDX_GET_DEVICE->CreateBuffer(&bd, nullptr, &m_cbMatrix);
		assert(SUCCEEDED(hr) && "MDX::BaseActorコンスタントバッファ作成失敗");

		//  シェーダー環境関連変数
		ZeroMemory(&bd, sizeof(D3D11_BUFFER_DESC));
		bd.Usage = D3D11_USAGE_DYNAMIC;
		bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		bd.MiscFlags = 0;
		bd.StructureByteStride = 0;
		bd.ByteWidth = sizeof(CBEnvironment);
		hr = MDX_GET_DEVICE->CreateBuffer(&bd, nullptr, &m_cbEnvironment);
		assert(SUCCEEDED(hr) && "MDX::BaseActorコンスタントバッファ作成失敗");

		// 物理ベースレンダリング用コンスタントバッファ
		ZeroMemory(&bd, sizeof(D3D11_BUFFER_DESC));
		bd.Usage = D3D11_USAGE_DYNAMIC;
		bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		bd.MiscFlags = 0;
		bd.StructureByteStride = 0;
		bd.ByteWidth = sizeof(CBPbr);
		hr = MDX_GET_DEVICE->CreateBuffer(&bd, nullptr, &m_cbPbr);
		assert(SUCCEEDED(hr) && "MDX::BaseActorコンスタントバッファ作成失敗");

		DirectX::XMUINT2 window_size = MDX_GET_DX_SYSTEM->GetWindowSize();
		// レンダーターゲット
		{
			
			// HDRシーン
			m_renderTarget[RT_HDR].Create(window_size.x, window_size.y, DXGI_FORMAT_R16G16B16A16_FLOAT);
			// ダウンスケールHDRシーン
			m_renderTarget[RT_DOWN_SCALED_HDR].Create(window_size.x/4.0f, window_size.y/4.0f, DXGI_FORMAT_R16G16B16A16_FLOAT);
			// ブライトパスフィルタ
			m_renderTarget[RT_BRIGHT_PASS_FILTER].Create(window_size.x / 4.0f, window_size.y / 4.0f, DXGI_FORMAT_R16G16B16A16_FLOAT);
			// ブルーム
			m_renderTarget[RT_BLOOM].Create(window_size.x / 8.0f, window_size.y / 8.0f, DXGI_FORMAT_R16G16B16A16_FLOAT);
			// ブルーム(ファイナル)
			m_renderTarget[RT_BLOOM_FINAL].Create(window_size.x / 8.0f, window_size.y / 8.0f, DXGI_FORMAT_R16G16B16A16_FLOAT);
			// 輝度
			m_renderTarget[RT_LUMINANCE].Create(1, 1, DXGI_FORMAT_R32_FLOAT);
		}

		// トーンマップテクスチャ
		for(int i=0 ; i<NUM_TONEMAP_TEXTURE ; i++)
		{
			int size = 1 << (2*i);
			m_toneMapTexture[i].Create(size, size, DXGI_FORMAT_R32_FLOAT);
		}

		// ブルーム
		for (int i = 0; i<NUM_BLOOM_TEXTURE; i++)
		{
			m_bloomTexture[i].Create(window_size.x / 8.0f, window_size.y / 8.0f, DXGI_FORMAT_R16G16B16A16_FLOAT);
		}

		return true;
	}

	// 解放
	void RenderManager::Release(){
		m_shader.reset();
		m_shaderPostEffect.reset();
		m_defaultIrradianceTex.reset();
		m_defaultRadianceTex.reset();
		SAFE_RELEASE(m_cbMatrix);
		SAFE_RELEASE(m_cbEnvironment);
		SAFE_RELEASE(m_cbPbr);
		for( uint32_t i=0 ;i<m_actor.size() ; i++ ){
			m_actor[i]->Release();
			SAFE_DELETE(m_actor[i]);
		}
		std::vector<IActor*>().swap(m_actor);
		for( uint32_t i=0 ; i<ARRAYSIZE(m_renderTarget) ; i++ ){
			m_renderTarget[i].Release();
		}

		for (int i = 0; i<NUM_TONEMAP_TEXTURE; i++)
		{
			m_toneMapTexture[i].Release();
		}

		for (int i = 0; i<NUM_BLOOM_TEXTURE; i++)
		{
			m_bloomTexture[i].Release();
		}

	}

	// 更新
	void RenderManager::Update(){
		for( uint32_t i=0 ;i<m_actor.size() ; i++ ){
			m_actor[i]->Update();
		}
	}

	// 描画
	void RenderManager::Draw(){
		m_defaultIrradianceTex->PSSetShaderResource(1);
		m_defaultRadianceTex->PSSetShaderResource(2);

		// HDRフォーマットに描画
		{
			ID3D11RenderTargetView* rtv = m_renderTarget[RT_HDR].GetRenderTargetView();
			MDX_GET_IMMEDIATE_CONTEXT->OMSetRenderTargets(1, &rtv, MDX_GET_BACK_BUFFER_DSV);
			
			m_renderTarget[RT_HDR].Clear(Vector4f(0, 0.5f, 0.5f, 1));
			MDX::Render::ClearDepthStencil(MDX_GET_BACK_BUFFER_DSV, 1.0f, 0);

			//m_shader->SetShader();

			for( uint32_t i=0 ;i<m_actor.size() ; i++ ){
				DrawActor(m_actor[i]);
			}
		}
		// ファイナルショットを描画
		{
			ID3D11RenderTargetView* rtv = MDX_GET_BACK_BUFFER_RTV;
			MDX_GET_IMMEDIATE_CONTEXT->OMSetRenderTargets(1, &rtv, MDX_GET_BACK_BUFFER_DSV);
			
			MDX::Render::ClearRenderTarget(MDX_GET_BACK_BUFFER_RTV, Vector4f(0.0f, 0.0f, 0.0f, 1.0f));
			MDX::Render::ClearDepthStencil(MDX_GET_BACK_BUFFER_DSV, 1.0f, 0);

			DirectX::XMUINT2 window_size = MDX_GET_DX_SYSTEM->GetWindowSize();
			Sprite::Render2D(0, 0, window_size.x, window_size.y, m_shaderPostEffect.get(), m_renderTarget[RT_HDR].GetShaderResourceView(), window_size.x, window_size.y);
		}
	}

	// アクターの追加
	void RenderManager::AddActor(IActor* actor){
		assert(actor);

		m_actor.push_back(actor);
	}

	// 変換行列コンスタントバッファ更新
	void RenderManager::UpdateCBMatrix(const Matrix4X4& world){
		// 変換行列作成
		CBMatrix cbm;
		cbm.matWorld = world;
		cbm.matView = m_matrix.matView;
		cbm.matProjection = m_matrix.matProjection;

		// 変換行列コンスタントバッファに書き込み
		HRESULT hr = S_OK;
		D3D11_MAPPED_SUBRESOURCE mappedResource;
		hr = MDX_GET_IMMEDIATE_CONTEXT->Map(m_cbMatrix, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
		CopyMemory(mappedResource.pData, reinterpret_cast<void*>(&cbm), sizeof(CBMatrix));
		MDX_GET_IMMEDIATE_CONTEXT->Unmap(m_cbMatrix, 0);
		assert(hr == S_OK && "RenderManager::UpdateCBMatrixコンスタントバッファ書き込み失敗");
	
		MDX_GET_IMMEDIATE_CONTEXT->VSSetConstantBuffers(0, 1, &m_cbMatrix);
		MDX_GET_IMMEDIATE_CONTEXT->PSSetConstantBuffers(0, 1, &m_cbMatrix);
	}

	// 環境用コンスタントバッファ更新
	void RenderManager::UpdateCBEnvironmment(CBEnvironment* environment){
		// 物理ベースレンダリングコンスタントバッファに書き込み
		HRESULT hr = S_OK;
		D3D11_MAPPED_SUBRESOURCE mappedResource;
		hr = MDX_GET_IMMEDIATE_CONTEXT->Map(m_cbEnvironment, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
		CopyMemory(mappedResource.pData, reinterpret_cast<void*>(environment), sizeof(CBEnvironment));
		MDX_GET_IMMEDIATE_CONTEXT->Unmap(m_cbEnvironment, 0);
		assert(hr == S_OK && "RenderManager::UpdateCBEnvironmmentコンスタントバッファ書き込み失敗");

		MDX_GET_IMMEDIATE_CONTEXT->VSSetConstantBuffers(1, 1, &m_cbEnvironment);
		MDX_GET_IMMEDIATE_CONTEXT->PSSetConstantBuffers(1, 1, &m_cbEnvironment);
	}


	// PBR用コンスタントバッファ更新
	void RenderManager::UpdateCBPbr(CBPbr* pbr){
		// 物理ベースレンダリングコンスタントバッファに書き込み
		HRESULT hr = S_OK;
		D3D11_MAPPED_SUBRESOURCE mappedResource;
		hr = MDX_GET_IMMEDIATE_CONTEXT->Map(m_cbPbr, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
		CopyMemory(mappedResource.pData, reinterpret_cast<void*>(pbr), sizeof(CBPbr));
		MDX_GET_IMMEDIATE_CONTEXT->Unmap(m_cbPbr, 0);
		assert(hr == S_OK && "RenderManager::UpdateCBPbrコンスタントバッファ書き込み失敗");

		MDX_GET_IMMEDIATE_CONTEXT->VSSetConstantBuffers(2, 1, &m_cbMatrix);
		MDX_GET_IMMEDIATE_CONTEXT->PSSetConstantBuffers(2, 1, &m_cbPbr);
	}

	// アクターの描画
	void RenderManager::DrawActor(IActor* actor){
		actor->UpdateCBMatrix();
		actor->UpdateCBPbr();
		actor->Draw();
	}
}