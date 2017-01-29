#ifndef MDX_RENDER_STATE_H
#define MDX_RENDER_STATE_H

#include <d3d11.h>
#include "../DXSystem/MDX_DXSystem.h"

namespace MDX
{

	//*********************************************************************************************
	//		@name	�T���v���[�X�e�[�g�p�萔
	//*********************************************************************************************
	enum SamplerStateType
	{
		//	�ٕ����t�B���^�����O
		FILTER_ANISOTROPIC_ADDRESS_WRAP,
		FILTER_ANISOTROPIC_ADDRESS_CLAMP,
		//	���`�t�B���^�����O
		FILTER_LINEAR_ADDRESS_WRAP,
		FILTER_LINEAR_ADDRESS_CLAMP,
		//	�ŋߓ_�T���v�����O
		FILTER_POINT_ADDRESS_WRAP,
		FILTER_POINT_ADDRESS_CLAMP,

		SAMPLER_STATE_MAX
	};

	//*********************************************************************************************
	//		@name	�u�����h�X�e�[�g�p�萔
	//*********************************************************************************************
	enum BlendStateType
	{
		ALPHA,	//	���`����
		ADD,		//	���Z����
		SUB,			// ���Z����
		MUL,		//	��Z����
		NONE,		//	�u�����h�Ȃ�
		BLEND_STATE_MAX
	};

	//*********************************************************************************************
	//		@name	�`���Ԑݒ�N���X
	//*********************************************************************************************
	class RenderState
	{
	public:
		RenderState(){}
		~RenderState(){}

		//---------------------------------------------------------------------------------------------------------------------------
		//		@brief ���
		//---------------------------------------------------------------------------------------------------------------------------
		static void	Release();

		//---------------------------------------------------------------------------------------------------------------------------
		//		@brief ������
		//		@note �V�X�e���̏��������ɂP�񂾂��Ăяo��
		//---------------------------------------------------------------------------------------------------------------------------
		static bool	Initialize();

		//---------------------------------------------------------------------------------------------------------------------------
		//		@brief �X�V
		//		@note mdxSystem::Update�Ŗ��t���[���Ă�
		//---------------------------------------------------------------------------------------------------------------------------
		static void	Update();

		//*********************************************************************************************
		//		@name	�f�v�X�X�e���V���X�e�[�g�֘A
		//*********************************************************************************************
		//---------------------------------------------------------------------------------------------------------------------------
		//		@brief �f�v�X�X�e���V���X�e�[�g�̃p�����[�^�ݒ�
		//		@param[in] depthStencilDesc �f�v�X�X�e���V���X�e�[�g�̃p�����[�^
		//---------------------------------------------------------------------------------------------------------------------------
		static void	SetDepthStencilDesc(D3D11_DEPTH_STENCIL_DESC depthStencilDesc);
		//---------------------------------------------------------------------------------------------------------------------------
		//		@brief ���X�^���C�U�[�X�e�[�g�p�����[�^�擾
		//		@return ���X�^���C�U�[�X�e�[�g�̃p�����[�^
		//---------------------------------------------------------------------------------------------------------------------------
		static D3D11_DEPTH_STENCIL_DESC	GetDepthStencilDesc() { return m_depthStencilDesc; }

		//*********************************************************************************************
		//		@name	���X�^���C�U�[�X�e�[�g�֘A
		//*********************************************************************************************
		//---------------------------------------------------------------------------------------------------------------------------
		//		@brief ���X�^���C�U�[�X�e�[�g�̃p�����[�^�ݒ�
		//		@param[in] rasterizerDesc ���X�^���C�U�[�X�e�[�g�̃p�����[�^
		//---------------------------------------------------------------------------------------------------------------------------
		static void	SetRasterizerDesc( D3D11_RASTERIZER_DESC rasterizerDesc );
		//---------------------------------------------------------------------------------------------------------------------------
		//		@brief ���X�^���C�U�[�X�e�[�g�p�����[�^�擾
		//		@return ���X�^���C�U�[�X�e�[�g�̃p�����[�^
		//---------------------------------------------------------------------------------------------------------------------------
		static D3D11_RASTERIZER_DESC	GetRasterizerDesc(){ return m_rasterizerDesc; }

		//*********************************************************************************************
		//		@name	�T���v���[�X�e�[�g�֘A
		//*********************************************************************************************
		//---------------------------------------------------------------------------------------------------------------------------
		//		@brief �T���v���[�X�e�[�g�ݒ�(�e�V�F�[�_�[���Ƃɂ���)
		//		@param[in] samplerType SamplerStateType�̒l
		//		@param[in] slotIndex �X���b�g�ԍ�
		//---------------------------------------------------------------------------------------------------------------------------
		static void	SetVSSamplerState( SamplerStateType samplerType, int slotIndex );
		static void	SetHSSamplerState( SamplerStateType samplerType, int slotIndex );
		static void	SetDSSamplerState( SamplerStateType samplerType, int slotIndex );
		static void	SetGSSamplerState( SamplerStateType samplerType, int slotIndex );
		static void	SetPSSamplerState( SamplerStateType samplerType, int slotIndex );
		static void	SetCSSamplerState( SamplerStateType samplerType, int slotIndex );
	
		//*********************************************************************************************
		//		@name	�u�����h�X�e�[�g�֘A
		//*********************************************************************************************
		//---------------------------------------------------------------------------------------------------------------------------
		//		@brief �u�����h�X�e�[�g�̐���
		//		@param[in] blendState �e�����_�[�^�[�Q�b�g�p�̃u�����h�̃p�����[�^�[
		//		@param[in] numBlendState �ݒ肷��u�����h(�����_�[�^�[�Q�b�g)�̐�
		//		@return �������ꂽ�u�����h�X�e�[�g
		//---------------------------------------------------------------------------------------------------------------------------
		static ID3D11BlendState*	CreateBlendState( D3D11_RENDER_TARGET_BLEND_DESC blendState[], UINT numBlendState );
		//---------------------------------------------------------------------------------------------------------------------------
		//		@brief �ėp�u�����h�X�e�[�g�̐ݒ�
		//		@param[in] blendState BlendStateType�̒l
		//---------------------------------------------------------------------------------------------------------------------------
		static void	SetBlendState( BlendStateType blendState );
		//---------------------------------------------------------------------------------------------------------------------------
		//		@brief �u�����h�X�e�[�g�̐ݒ�
		//		@param[in] blendState �u�����h�X�e�[�g
		//---------------------------------------------------------------------------------------------------------------------------
		static void	SetBlendStae( ID3D11BlendState* blendState );
		//---------------------------------------------------------------------------------------------------------------------------
		//		@brief �ėp�u�����h�X�e�[�g�̃p�����[�^���擾
		//		@param[in] blendState BlendStateType�̒l
		//		@return �u�����h�X�e�[�g�̃p�����[�^
		//---------------------------------------------------------------------------------------------------------------------------
		static D3D11_RENDER_TARGET_BLEND_DESC	GetBlend( BlendStateType blendState );
	private:
		//	�f�v�X�X�e���V���X�e�[�g
		static ID3D11DepthStencilState* m_pDepthStencilState;
		static D3D11_DEPTH_STENCIL_DESC	m_depthStencilDesc;
		static bool						m_bUpdateDepthStencilState;
		//	���X�^���C�U�[�X�e�[�g
		static ID3D11RasterizerState*	m_pRasterizerState;
		static D3D11_RASTERIZER_DESC	m_rasterizerDesc;
		static bool						m_bUpdateRasterizerState;	//	���X�^���C�U�[�X�e�[�g��K�v�Ȏ��ɍ�蒼������
		//	�ėp�T���v���[�X�e�[�g
		static ID3D11SamplerState*		m_pSamplerState[SamplerStateType::SAMPLER_STATE_MAX];
		//	�ėp�u�����h�X�e�[�g
		static ID3D11BlendState*		m_pBlendState[BlendStateType::BLEND_STATE_MAX];

		//*********************************************************************************************
		//		@name	�f�v�X�X�e���V���X�e�[�g�֘A
		//*********************************************************************************************
		//---------------------------------------------------------------------------------------------------------------------------
		//		@brief �f�v�X�X�e���V���X�e�[�g����
		//		@return ���������H
		//---------------------------------------------------------------------------------------------------------------------------
		static bool	CreateDepthStencilState();

		//*********************************************************************************************
		//		@name	���X�^���C�U�[�X�e�[�g�֘A
		//*********************************************************************************************
		//---------------------------------------------------------------------------------------------------------------------------
		//		@brief ���X�^���C�U�[�X�e�[�g����
		//		@return ���������H
		//---------------------------------------------------------------------------------------------------------------------------
		static bool	CreateRasterizerState();

		//*********************************************************************************************
		//		@name	�T���v���[�X�e�[�g�֘A
		//*********************************************************************************************
		//---------------------------------------------------------------------------------------------------------------------------
		//		@brief �ėp�T���v���[�X�e�[�g�̐���
		//		@return ���������H
		//---------------------------------------------------------------------------------------------------------------------------
		static bool	CreateGenericSamplerStates();

		//*********************************************************************************************
		//		@name	�u�����h�X�e�[�g�֘A
		//*********************************************************************************************
		//---------------------------------------------------------------------------------------------------------------------------
		//		@brief �ėp�u�����h�X�e�[�g�̐���
		//		@return ���������H
		//---------------------------------------------------------------------------------------------------------------------------
		static bool	CreateGenericBlendStates();
		//---------------------------------------------------------------------------------------------------------------------------
		//		@brief ���`�����p�u�����h�X�e�[�g�̃p�����[�^���擾
		//		@return ���`�����p�u�����h�X�e�[�g
		//---------------------------------------------------------------------------------------------------------------------------
		static D3D11_RENDER_TARGET_BLEND_DESC	GetAlphaBlend();
		//---------------------------------------------------------------------------------------------------------------------------
		//		@brief ���Z�����p�u�����h�X�e�[�g�̃p�����[�^���擾
		//		@return ���Z�����p�u�����h�X�e�[�g
		//---------------------------------------------------------------------------------------------------------------------------
		static D3D11_RENDER_TARGET_BLEND_DESC	GetAddBlend();
		//---------------------------------------------------------------------------------------------------------------------------
		//		@brief ���Z�����p�u�����h�X�e�[�g�̃p�����[�^���擾
		//		@return ���Z�����p�u�����h�X�e�[�g
		//---------------------------------------------------------------------------------------------------------------------------
		static D3D11_RENDER_TARGET_BLEND_DESC	GetSubBlend();
		//---------------------------------------------------------------------------------------------------------------------------
		//		@brief ��Z�����p�u�����h�X�e�[�g�̃p�����[�^���擾
		//		@return ��Z�����p�u�����h�X�e�[�g
		//---------------------------------------------------------------------------------------------------------------------------
		static D3D11_RENDER_TARGET_BLEND_DESC	GetMulBlend();
		//---------------------------------------------------------------------------------------------------------------------------
		//		@brief �����Ȃ��p�u�����h�X�e�[�g�̃p�����[�^���擾
		//		@return �����Ȃ��p�u�����h�X�e�[�g
		//---------------------------------------------------------------------------------------------------------------------------
		static D3D11_RENDER_TARGET_BLEND_DESC	GetNoneBlend();

	};// class mdxRenderState
}// namespace MDX

#endif // !MDX_RENDER_STATE_H