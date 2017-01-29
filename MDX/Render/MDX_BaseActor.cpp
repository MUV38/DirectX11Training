#include "MDX_BaseActor.h"
#include "../System/MDX_System.h"
#include "../Utillity/MDX_Utillity.h"

namespace MDX{
	// �ÓI�����o�ϐ�
	ID3D11SamplerState* BaseActor::m_sampler = nullptr;

	BaseActor::BaseActor() {
		m_OnBeginDraw = nullptr;
		m_OnBeginDrawArg = nullptr;

		m_pbr.baseColor = Vector4f(1, 1, 1, 1);
		m_pbr.roughness = 0.5f;
		m_pbr.metallic = 0.0f;
	}

	BaseActor::~BaseActor(){
		Release();
	}

	/**
	* @brief �풓�f�[�^������
	* �A�v���P�[�V�������������Ɉ�񂾂��Ăяo��
	*/
	bool BaseActor::InitResidentData(){

		D3D11_SAMPLER_DESC samplerDesc;
		samplerDesc.Filter = D3D11_FILTER_ANISOTROPIC;         // �T���v�����O���Ɏg�p����t�B���^�B�����ł͈ٕ����t�B���^�[���g�p����B
		samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;     // 0 �` 1 �͈̔͊O�ɂ��� u �e�N�X�`���[���W�̕`����@
		samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;     // 0 �` 1 �͈̔͊O�ɂ��� v �e�N�X�`���[���W
		samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;     // 0 �` 1 �͈̔͊O�ɂ��� w �e�N�X�`���[���W
		samplerDesc.MipLODBias = 0;                            // �v�Z���ꂽ�~�b�v�}�b�v ���x������̃o�C�A�X
		samplerDesc.MaxAnisotropy = 16;                        // �T���v�����O�Ɉٕ�����Ԃ��g�p���Ă���ꍇ�̌��E�l�B�L���Ȓl�� 1 �` 16 �B
		samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;  // ��r�I�v�V�����B
		// ���E�F
		for(int i=0 ; i<4 ; i++){
			samplerDesc.BorderColor[0] = 0.0f;
		}
		samplerDesc.MinLOD = 0;                                // �A�N�Z�X�\�ȃ~�b�v�}�b�v�̉����l
		samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;                // �A�N�Z�X�\�ȃ~�b�v�}�b�v�̏���l
		MDX_GET_DEVICE->CreateSamplerState(&samplerDesc, &m_sampler);

		return true;
	}

	/**
	* @brief �풓�f�[�^���
	* �A�v���P�[�V����������Ɉ�񂾂��Ăяo��
	*/
	void BaseActor::ReleaseResidentData(){
		SAFE_RELEASE(m_sampler);
	}

	// ���
	void BaseActor::Release(){
	}

	// �X�V
	void BaseActor::Update(){
	}

	// �`��
	void BaseActor::Draw(){
		// �`��J�n�R�[���o�b�N
		if (m_OnBeginDraw) {
			m_OnBeginDraw(m_OnBeginDrawArg);
		}

		// TODO: ���̏����͕ʂ̃N���X�Ɉڂ�
		if (m_shader) {
			m_shader->SetShader();
		}else {
			MDX_GET_RENDER_MANAGER->GetDefaultShader()->SetShader();
		}
	}

	// �ϊ��s��R���X�^���g�o�b�t�@�X�V
	void BaseActor::UpdateCBMatrix()
	{
		// ���[���h�ϊ��s��
		DirectX::XMMATRIX mat = DirectX::XMMatrixIdentity();
		mat *= DirectX::XMMatrixScaling(m_scale.x, m_scale.y, m_scale.z);
		mat *= DirectX::XMMatrixRotationX(m_angle.x);
		mat *= DirectX::XMMatrixRotationY(m_angle.y);
		mat *= DirectX::XMMatrixRotationZ(m_angle.z);
		mat *= DirectX::XMMatrixTranslation(m_pos.x, m_pos.y, m_pos.z);
		mat = DirectX::XMMatrixTranspose(mat);
		DirectX::XMFLOAT4X4 matWorld;
		DirectX::XMStoreFloat4x4(&matWorld, mat);

		// �����_�[�}�l�[�W���[��
		MDX_GET_RENDER_MANAGER->UpdateCBMatrix(matWorld);
	}

	// PBR�p�R���X�^���g�o�b�t�@�X�V
	void BaseActor::UpdateCBPbr(){
		// �����_�[�}�l�[�W���[��
		MDX_GET_RENDER_MANAGER->UpdateCBPbr(&m_pbr);
	}

	//-------------------------------------------------------------------------
	// PBR�p�����[�^�[
	//-------------------------------------------------------------------------
	void BaseActor::SetRoughness(float roughness) {
		m_pbr.roughness = MDX::Clamp<float>(0, 1, roughness);
	}
	void BaseActor::SetMetallic(float metallic) {
		m_pbr.metallic = MDX::Clamp<float>(0, 1, metallic);
	}
}