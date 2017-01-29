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

	// ������
	bool RenderManager::Init(){
		bool isSucceeded = true;
		HRESULT hr = S_OK;

		// �V�F�[�_�[
		m_shader.reset(new Shader);
		isSucceeded = m_shader->LoadVSFromCso("VS_Primitive", MDX::Shader::VERTEX_LAYOUT_3D);
		assert(isSucceeded && "MDX::BaseActor���_�V�F�[�_�[���������s");
		isSucceeded = m_shader->LoadPSFromCso("PS_Primitive");
		assert(isSucceeded && "MDX::BaseActor�s�N�Z���V�F�[�_�[���������s");

		// �|�X�g�G�t�F�N�g�V�F�[�_�[
		m_shaderPostEffect.reset(new Shader);
		isSucceeded = m_shaderPostEffect->LoadVSFromCso("VS_PostEffect", MDX::Shader::VERTEX_LAYOUT_2D);
		assert(isSucceeded && "MDX::BaseActor���_�V�F�[�_�[���������s");
		isSucceeded = m_shaderPostEffect->LoadPSFromCso("PS_PostEffect");
		assert(isSucceeded && "MDX::BaseActor�s�N�Z���V�F�[�_�[���������s");

		// �C���f�B�A���X�e�N�X�`��
		m_defaultIrradianceTex.reset(new Texture());
		{
			MDX::Texture::LoadInfo loadInfo;
			{
				loadInfo.filename = "DATA\\IBL\\default_irradiance.dds";
				loadInfo.forceSRGB = true;
			}
			m_defaultIrradianceTex->Load(loadInfo);
		}

		// ���f�B�A���X�e�N�X�`��
		m_defaultRadianceTex.reset(new Texture());
		{
			MDX::Texture::LoadInfo loadInfo;
			{
				loadInfo.filename = "DATA\\IBL\\default_radiance.dds";
				loadInfo.forceSRGB = true;
			}
			m_defaultRadianceTex->Load(loadInfo);
		}

		// �ϊ��s��p�R���X�^���g�o�b�t�@
		D3D11_BUFFER_DESC bd;
		ZeroMemory(&bd, sizeof(D3D11_BUFFER_DESC));
		bd.Usage = D3D11_USAGE_DYNAMIC;
		bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		bd.MiscFlags = 0;
		bd.StructureByteStride = 0;
		bd.ByteWidth = sizeof(CBMatrix);
		hr = MDX_GET_DEVICE->CreateBuffer(&bd, nullptr, &m_cbMatrix);
		assert(SUCCEEDED(hr) && "MDX::BaseActor�R���X�^���g�o�b�t�@�쐬���s");

		//  �V�F�[�_�[���֘A�ϐ�
		ZeroMemory(&bd, sizeof(D3D11_BUFFER_DESC));
		bd.Usage = D3D11_USAGE_DYNAMIC;
		bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		bd.MiscFlags = 0;
		bd.StructureByteStride = 0;
		bd.ByteWidth = sizeof(CBEnvironment);
		hr = MDX_GET_DEVICE->CreateBuffer(&bd, nullptr, &m_cbEnvironment);
		assert(SUCCEEDED(hr) && "MDX::BaseActor�R���X�^���g�o�b�t�@�쐬���s");

		// �����x�[�X�����_�����O�p�R���X�^���g�o�b�t�@
		ZeroMemory(&bd, sizeof(D3D11_BUFFER_DESC));
		bd.Usage = D3D11_USAGE_DYNAMIC;
		bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		bd.MiscFlags = 0;
		bd.StructureByteStride = 0;
		bd.ByteWidth = sizeof(CBPbr);
		hr = MDX_GET_DEVICE->CreateBuffer(&bd, nullptr, &m_cbPbr);
		assert(SUCCEEDED(hr) && "MDX::BaseActor�R���X�^���g�o�b�t�@�쐬���s");

		DirectX::XMUINT2 window_size = MDX_GET_DX_SYSTEM->GetWindowSize();
		// �����_�[�^�[�Q�b�g
		{
			
			// HDR�V�[��
			m_renderTarget[RT_HDR].Create(window_size.x, window_size.y, DXGI_FORMAT_R16G16B16A16_FLOAT);
			// �_�E���X�P�[��HDR�V�[��
			m_renderTarget[RT_DOWN_SCALED_HDR].Create(window_size.x/4.0f, window_size.y/4.0f, DXGI_FORMAT_R16G16B16A16_FLOAT);
			// �u���C�g�p�X�t�B���^
			m_renderTarget[RT_BRIGHT_PASS_FILTER].Create(window_size.x / 4.0f, window_size.y / 4.0f, DXGI_FORMAT_R16G16B16A16_FLOAT);
			// �u���[��
			m_renderTarget[RT_BLOOM].Create(window_size.x / 8.0f, window_size.y / 8.0f, DXGI_FORMAT_R16G16B16A16_FLOAT);
			// �u���[��(�t�@�C�i��)
			m_renderTarget[RT_BLOOM_FINAL].Create(window_size.x / 8.0f, window_size.y / 8.0f, DXGI_FORMAT_R16G16B16A16_FLOAT);
			// �P�x
			m_renderTarget[RT_LUMINANCE].Create(1, 1, DXGI_FORMAT_R32_FLOAT);
		}

		// �g�[���}�b�v�e�N�X�`��
		for(int i=0 ; i<NUM_TONEMAP_TEXTURE ; i++)
		{
			int size = 1 << (2*i);
			m_toneMapTexture[i].Create(size, size, DXGI_FORMAT_R32_FLOAT);
		}

		// �u���[��
		for (int i = 0; i<NUM_BLOOM_TEXTURE; i++)
		{
			m_bloomTexture[i].Create(window_size.x / 8.0f, window_size.y / 8.0f, DXGI_FORMAT_R16G16B16A16_FLOAT);
		}

		return true;
	}

	// ���
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

	// �X�V
	void RenderManager::Update(){
		for( uint32_t i=0 ;i<m_actor.size() ; i++ ){
			m_actor[i]->Update();
		}
	}

	// �`��
	void RenderManager::Draw(){
		m_defaultIrradianceTex->PSSetShaderResource(1);
		m_defaultRadianceTex->PSSetShaderResource(2);

		// HDR�t�H�[�}�b�g�ɕ`��
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
		// �t�@�C�i���V���b�g��`��
		{
			ID3D11RenderTargetView* rtv = MDX_GET_BACK_BUFFER_RTV;
			MDX_GET_IMMEDIATE_CONTEXT->OMSetRenderTargets(1, &rtv, MDX_GET_BACK_BUFFER_DSV);
			
			MDX::Render::ClearRenderTarget(MDX_GET_BACK_BUFFER_RTV, Vector4f(0.0f, 0.0f, 0.0f, 1.0f));
			MDX::Render::ClearDepthStencil(MDX_GET_BACK_BUFFER_DSV, 1.0f, 0);

			DirectX::XMUINT2 window_size = MDX_GET_DX_SYSTEM->GetWindowSize();
			Sprite::Render2D(0, 0, window_size.x, window_size.y, m_shaderPostEffect.get(), m_renderTarget[RT_HDR].GetShaderResourceView(), window_size.x, window_size.y);
		}
	}

	// �A�N�^�[�̒ǉ�
	void RenderManager::AddActor(IActor* actor){
		assert(actor);

		m_actor.push_back(actor);
	}

	// �ϊ��s��R���X�^���g�o�b�t�@�X�V
	void RenderManager::UpdateCBMatrix(const Matrix4X4& world){
		// �ϊ��s��쐬
		CBMatrix cbm;
		cbm.matWorld = world;
		cbm.matView = m_matrix.matView;
		cbm.matProjection = m_matrix.matProjection;

		// �ϊ��s��R���X�^���g�o�b�t�@�ɏ�������
		HRESULT hr = S_OK;
		D3D11_MAPPED_SUBRESOURCE mappedResource;
		hr = MDX_GET_IMMEDIATE_CONTEXT->Map(m_cbMatrix, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
		CopyMemory(mappedResource.pData, reinterpret_cast<void*>(&cbm), sizeof(CBMatrix));
		MDX_GET_IMMEDIATE_CONTEXT->Unmap(m_cbMatrix, 0);
		assert(hr == S_OK && "RenderManager::UpdateCBMatrix�R���X�^���g�o�b�t�@�������ݎ��s");
	
		MDX_GET_IMMEDIATE_CONTEXT->VSSetConstantBuffers(0, 1, &m_cbMatrix);
		MDX_GET_IMMEDIATE_CONTEXT->PSSetConstantBuffers(0, 1, &m_cbMatrix);
	}

	// ���p�R���X�^���g�o�b�t�@�X�V
	void RenderManager::UpdateCBEnvironmment(CBEnvironment* environment){
		// �����x�[�X�����_�����O�R���X�^���g�o�b�t�@�ɏ�������
		HRESULT hr = S_OK;
		D3D11_MAPPED_SUBRESOURCE mappedResource;
		hr = MDX_GET_IMMEDIATE_CONTEXT->Map(m_cbEnvironment, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
		CopyMemory(mappedResource.pData, reinterpret_cast<void*>(environment), sizeof(CBEnvironment));
		MDX_GET_IMMEDIATE_CONTEXT->Unmap(m_cbEnvironment, 0);
		assert(hr == S_OK && "RenderManager::UpdateCBEnvironmment�R���X�^���g�o�b�t�@�������ݎ��s");

		MDX_GET_IMMEDIATE_CONTEXT->VSSetConstantBuffers(1, 1, &m_cbEnvironment);
		MDX_GET_IMMEDIATE_CONTEXT->PSSetConstantBuffers(1, 1, &m_cbEnvironment);
	}


	// PBR�p�R���X�^���g�o�b�t�@�X�V
	void RenderManager::UpdateCBPbr(CBPbr* pbr){
		// �����x�[�X�����_�����O�R���X�^���g�o�b�t�@�ɏ�������
		HRESULT hr = S_OK;
		D3D11_MAPPED_SUBRESOURCE mappedResource;
		hr = MDX_GET_IMMEDIATE_CONTEXT->Map(m_cbPbr, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
		CopyMemory(mappedResource.pData, reinterpret_cast<void*>(pbr), sizeof(CBPbr));
		MDX_GET_IMMEDIATE_CONTEXT->Unmap(m_cbPbr, 0);
		assert(hr == S_OK && "RenderManager::UpdateCBPbr�R���X�^���g�o�b�t�@�������ݎ��s");

		MDX_GET_IMMEDIATE_CONTEXT->VSSetConstantBuffers(2, 1, &m_cbMatrix);
		MDX_GET_IMMEDIATE_CONTEXT->PSSetConstantBuffers(2, 1, &m_cbPbr);
	}

	// �A�N�^�[�̕`��
	void RenderManager::DrawActor(IActor* actor){
		actor->UpdateCBMatrix();
		actor->UpdateCBPbr();
		actor->Draw();
	}
}