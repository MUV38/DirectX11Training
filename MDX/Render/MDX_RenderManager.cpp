#include "MDX_RenderManager.h"
#include "../Utillity/MDX_Utillity.h"
#include "../System/MDX_System.h"
#include "MDX_BaseActor.h"
#include "../2D/MDX_Sprite.h"
#include "MDX_Render.h"

namespace MDX{
	RenderManager::RenderManager()
	{
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
		m_shader->LoadVSFromCso("VS_Primitive", MDX::Shader::VERTEX_LAYOUT_3D);
		m_shader->LoadPSFromCso("PS_Primitive");

		// ダウンスケール4x4
		m_shaderDownScale4x4.reset(new Shader);
		m_shaderDownScale4x4->LoadVSFromCso("VS_2D", MDX::Shader::VERTEX_LAYOUT_2D);
		m_shaderDownScale4x4->LoadPSFromCso("PS_DownScale4x4");

		// 輝度計算初回
		m_shaderSampleLumInitial.reset(new Shader);
		m_shaderSampleLumInitial->LoadVSFromCso("VS_2D", MDX::Shader::VERTEX_LAYOUT_2D);
		m_shaderSampleLumInitial->LoadPSFromCso("PS_SampleLumInitial");

		// 輝度計算中間
		m_shaderSampleLumInterative.reset(new Shader);
		m_shaderSampleLumInterative->LoadVSFromCso("VS_2D", MDX::Shader::VERTEX_LAYOUT_2D);
		m_shaderSampleLumInterative->LoadPSFromCso("PS_SampleLumInterative");

		// 輝度計算最終
		m_shaderSampleLumFinal.reset(new Shader);
		m_shaderSampleLumFinal->LoadVSFromCso("VS_2D", MDX::Shader::VERTEX_LAYOUT_2D);
		m_shaderSampleLumFinal->LoadPSFromCso("PS_SampleLumFinal");

		// ポストエフェクトシェーダー
		m_shaderPostEffect.reset(new Shader);
		m_shaderPostEffect->LoadVSFromCso("VS_PostEffect", MDX::Shader::VERTEX_LAYOUT_2D);
		m_shaderPostEffect->LoadPSFromCso("PS_PostEffect");

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

		// ポストエフェクト用コンスタントバッファ
		{
			// ダウンスケール4x4
			{
				m_cbDownScale4x4.reset(new ConstantBuffer());
				m_cbDownScale4x4->Create(sizeof(CBDownScale4x4));
			}
			// 輝度計算初回
			{
				m_cbSampleLumInitial.reset(new ConstantBuffer());
				m_cbSampleLumInitial->Create(sizeof(CBSampleLumInitial));
			}
		}

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

		// ブルームテクスチャ
		for (int i = 0; i<NUM_BLOOM_TEXTURE; i++)
		{
			m_bloomTexture[i].Create(window_size.x / 8.0f, window_size.y / 8.0f, DXGI_FORMAT_R16G16B16A16_FLOAT);
		}

		// 順応輝度テクスチャ
		for (int i = 0; i < 2; i++) {
			m_adaptionLumTexture[i].Create(1, 1, DXGI_FORMAT_R32_FLOAT);
		}
		m_adaptedLumCur = &m_adaptionLumTexture[0];
		m_adaptedLumLast = &m_adaptionLumTexture[1];

		return true;
	}

	// 解放
	void RenderManager::Release(){
		m_shader.reset();
		m_shaderDownScale4x4.reset();
		m_shaderSampleLumInitial.reset();
		m_shaderSampleLumInterative.reset();
		m_shaderSampleLumFinal.reset();
		m_shaderPostEffect.reset();

		m_defaultIrradianceTex.reset();
		m_defaultRadianceTex.reset();
		SAFE_RELEASE(m_cbMatrix);
		SAFE_RELEASE(m_cbEnvironment);
		SAFE_RELEASE(m_cbPbr);

		m_cbDownScale4x4->Release();
		m_cbSampleLumInitial->Release();

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

		for (int i = 0; i < 2; i++) {
			m_adaptionLumTexture[i].Release();
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

		DirectX::XMUINT2 window_size = MDX_GET_DX_SYSTEM->GetWindowSize();

		// HDRフォーマットにシーン描画
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

		// HDRシーンをダウンスケール
		Scene_DownScale();

		// 輝度計算
		CalculateLuminance();

		// 順応輝度計算
		CalculateAdaptionLuminance();

		// ファイナルショットを描画
		{
			ID3D11RenderTargetView* rtv = MDX_GET_BACK_BUFFER_RTV;
			MDX_GET_IMMEDIATE_CONTEXT->OMSetRenderTargets(1, &rtv, MDX_GET_BACK_BUFFER_DSV);
			
			MDX::Render::ClearRenderTarget(MDX_GET_BACK_BUFFER_RTV, Vector4f(0.0f, 0.0f, 0.0f, 1.0f));
			MDX::Render::ClearDepthStencil(MDX_GET_BACK_BUFFER_DSV, 1.0f, 0);

			Sprite::Render2D(0, 0, window_size.x, window_size.y, m_shaderPostEffect.get(), m_renderTarget[RT_HDR].GetShaderResourceView(), window_size.x, window_size.y);
			//Sprite::Render2D(0, 0, window_size.x, window_size.y, m_shaderPostEffect.get(), m_renderTarget[RT_DOWN_SCALED_HDR].GetShaderResourceView(), m_renderTarget[RT_DOWN_SCALED_HDR].GetWidth(), m_renderTarget[RT_DOWN_SCALED_HDR].GetHeight());
			//Sprite::Render2D(0, 0, window_size.x, window_size.y, m_shaderPostEffect.get(), m_toneMapTexture[0].GetShaderResourceView(), m_toneMapTexture[0].GetWidth(), m_toneMapTexture[0].GetHeight());
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

	// ダウンスケール4x4のサンプルオフセット取得
	void RenderManager::GetDownScale4x4SampleOffset(UINT width, UINT height, Vector2f* sampleOffset)
	{
		float tu = 1.0f / static_cast<float>(width);
		float tv = 1.0f / static_cast<float>(height);

		int index = 0;
		for (int x = 0; x < 4; x++) {
			for (int y = 0; y < 4; y++) {
				sampleOffset[index].x = (x - 1.5f) * tu;
				sampleOffset[index].y = (y - 1.5f) * tv;

				index++;
			}
		}
	}

	// シーンをダウンスケール
	void RenderManager::Scene_DownScale()
	{
		DirectX::XMUINT2 window_size = MDX_GET_DX_SYSTEM->GetWindowSize();

		// コンスタントバッファ更新
		{
			CBDownScale4x4 cb;
			GetDownScale4x4SampleOffset(window_size.x, window_size.y, cb.sampleOffset);
			auto resource = m_cbDownScale4x4->Map();
			CopyMemory(resource.pData, reinterpret_cast<void*>(&cb), sizeof(CBDownScale4x4));
			m_cbDownScale4x4->Unmap();

			m_cbDownScale4x4->SetPS(0);
		}
		// レンダーターゲット設定
		RenderTarget* rt = &m_renderTarget[RT_DOWN_SCALED_HDR];
		auto rtv = rt->GetRenderTargetView();
		MDX_GET_IMMEDIATE_CONTEXT->OMSetRenderTargets(1, &rtv, MDX_GET_BACK_BUFFER_DSV);

		// 画面クリア
		m_renderTarget[RT_DOWN_SCALED_HDR].Clear(Vector4f(0, 0, 0, 1));
		MDX::Render::ClearDepthStencil(MDX_GET_BACK_BUFFER_DSV, 1.0f, 0);

		// 描画
		Sprite::Render2D(0, 0, rt->GetWidth(), rt->GetHeight(), m_shaderDownScale4x4.get(), m_renderTarget[RT_HDR].GetShaderResourceView(), rt->GetWidth(), rt->GetHeight());
	}

	// 輝度計算
	void RenderManager::CalculateLuminance()
	{
		int curTexIndex = NUM_TONEMAP_TEXTURE - 1;

		// 初回輝度計算
		{
			RenderTarget* rt = &m_toneMapTexture[curTexIndex];

			// 初回起動計算のコンスタントバッファ更新
			{
				CBSampleLumInitial cb;
				float size = static_cast<float>(1 << (2 * curTexIndex));
				float tu = 1.0f / (3.0f * static_cast<float>(rt->GetWidth()));
				float tv = 1.0f / (3.0f * static_cast<float>(rt->GetHeight()));

				int index = 0;
				for (int x = -1; x <= 1; x++){
					for (int y = -1; y <= 1; y++){
						cb.sampleOffset[index].x = x * tu;
						cb.sampleOffset[index].y = y * tv;

						index++;
					}
				}
				auto resource = m_cbSampleLumInitial->Map();
				CopyMemory(resource.pData, reinterpret_cast<void*>(&cb), sizeof(cb));
				m_cbSampleLumInitial->Unmap();

				m_cbSampleLumInitial->SetPS(0);
			}

			// レンダーターゲット設定
			auto rtv = rt->GetRenderTargetView();
			MDX_GET_IMMEDIATE_CONTEXT->OMSetRenderTargets(1, &rtv, MDX_GET_BACK_BUFFER_DSV);

			// 画面クリア
			m_toneMapTexture[curTexIndex].Clear(Vector4f(0, 0, 0, 1));
			MDX::Render::ClearDepthStencil(MDX_GET_BACK_BUFFER_DSV, 1.0f, 0);

			// 描画
			Sprite::Render2D(0, 0, rt->GetWidth(), rt->GetHeight(), m_shaderSampleLumInitial.get(), m_renderTarget[RT_DOWN_SCALED_HDR].GetShaderResourceView(), m_renderTarget[RT_DOWN_SCALED_HDR].GetWidth(), m_renderTarget[RT_DOWN_SCALED_HDR].GetHeight());
			
			curTexIndex--;
		}
		
		// 輝度計算（1×1までダウサンプル）
		while (curTexIndex > 0){
			RenderTarget* rt = &m_toneMapTexture[curTexIndex];

			// コンスタントバッファ更新
			{
				CBDownScale4x4 cb;
				GetDownScale4x4SampleOffset(m_toneMapTexture[curTexIndex + 1].GetWidth(), m_toneMapTexture[curTexIndex + 1].GetHeight(), cb.sampleOffset);
				auto resource = m_cbDownScale4x4->Map();
				CopyMemory(resource.pData, reinterpret_cast<void*>(&cb), sizeof(CBDownScale4x4));
				m_cbDownScale4x4->Unmap();

				m_cbDownScale4x4->SetPS(0);
			}

			// レンダーターゲット設定
			auto rtv = rt->GetRenderTargetView();
			MDX_GET_IMMEDIATE_CONTEXT->OMSetRenderTargets(1, &rtv, MDX_GET_BACK_BUFFER_DSV);

			// 画面クリア
			m_toneMapTexture[curTexIndex].Clear(Vector4f(0, 0, 0, 1));
			MDX::Render::ClearDepthStencil(MDX_GET_BACK_BUFFER_DSV, 1.0f, 0);

			// 描画
			Sprite::Render2D(0, 0, rt->GetWidth(), rt->GetHeight(), m_shaderSampleLumInterative.get(), m_toneMapTexture[curTexIndex+1].GetShaderResourceView(), m_toneMapTexture[curTexIndex+1].GetWidth(), m_toneMapTexture[curTexIndex+1].GetHeight());

			curTexIndex--;
		}

		// 1×１の輝度計算
		{
			RenderTarget* rt = &m_toneMapTexture[curTexIndex];

			// コンスタントバッファ更新
			{
				CBDownScale4x4 cb;
				GetDownScale4x4SampleOffset(m_toneMapTexture[1].GetWidth(), m_toneMapTexture[1].GetHeight(), cb.sampleOffset);
				auto resource = m_cbDownScale4x4->Map();
				CopyMemory(resource.pData, reinterpret_cast<void*>(&cb), sizeof(CBDownScale4x4));
				m_cbDownScale4x4->Unmap();

				m_cbDownScale4x4->SetPS(0);
			}

			// レンダーターゲット設定
			auto rtv = rt->GetRenderTargetView();
			MDX_GET_IMMEDIATE_CONTEXT->OMSetRenderTargets(1, &rtv, MDX_GET_BACK_BUFFER_DSV);

			// 画面クリア
			m_toneMapTexture[curTexIndex].Clear(Vector4f(0, 0, 0, 1));
			MDX::Render::ClearDepthStencil(MDX_GET_BACK_BUFFER_DSV, 1.0f, 0);

			// 描画
			Sprite::Render2D(0, 0, rt->GetWidth(), rt->GetHeight(), m_shaderSampleLumFinal.get(), m_toneMapTexture[curTexIndex + 1].GetShaderResourceView(), m_toneMapTexture[1].GetWidth(), m_toneMapTexture[1].GetHeight());
		}
	}

	// 順応輝度計算
	void RenderManager::CalculateAdaptionLuminance()
	{
		// テクスチャ入れ替え
		auto tmp = m_adaptedLumCur;
		m_adaptedLumCur = m_adaptedLumLast;
		m_adaptedLumLast = tmp;


	}
}