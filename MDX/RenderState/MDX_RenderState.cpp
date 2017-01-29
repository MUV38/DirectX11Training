#include "MDX_RenderState.h"
#include "../System/MDX_System.h"
#include "../Utillity/MDX_Utillity.h"

using namespace MDX;

ID3D11DepthStencilState*	RenderState::m_pDepthStencilState = nullptr;
D3D11_DEPTH_STENCIL_DESC	RenderState::m_depthStencilDesc;
bool						RenderState::m_bUpdateDepthStencilState = false;
ID3D11RasterizerState*		RenderState::m_pRasterizerState = nullptr;
D3D11_RASTERIZER_DESC		RenderState::m_rasterizerDesc;
bool						RenderState::m_bUpdateRasterizerState = false;
ID3D11SamplerState*			RenderState::m_pSamplerState[SamplerStateType::SAMPLER_STATE_MAX];
ID3D11BlendState*			RenderState::m_pBlendState[BlendStateType::BLEND_STATE_MAX];

//---------------------------------------------------------------------------------------------------------------------------
//		@brief ���
//---------------------------------------------------------------------------------------------------------------------------
void	RenderState::Release()
{
	// �f�v�X�X�e���V���X�e�[�g�̉��
	SAFE_RELEASE(m_pDepthStencilState);
	//	���X�^���C�U�[�X�e�[�g�̉��
	SAFE_RELEASE( m_pRasterizerState );
	//	�T���v���[�X�e�[�g�̉��
	for( int i=0 ; i<SamplerStateType::SAMPLER_STATE_MAX ; i++ ){
		SAFE_RELEASE( m_pSamplerState[i] );
	}
	//	�u�����h�X�e�[�g�̉��
	for( int i=0 ; i<BlendStateType::BLEND_STATE_MAX ; i++ ){
		SAFE_RELEASE( m_pBlendState[i] );
	}
}

//---------------------------------------------------------------------------------------------------------------------------
//		@brief ������
//		@note �V�X�e���̏��������ɂP�񂾂��Ăяo��
//---------------------------------------------------------------------------------------------------------------------------
bool	RenderState::Initialize()
{
	//	�f�v�X�X�e���V���X�e�[�g�̐���
	if (!CreateDepthStencilState()) {
		return false;
	}

	//	���X�^���C�U�[�X�e�[�g�̐���
	if( !CreateRasterizerState() ){
		return false;
	}

	//	�ėp�T���v���[�X�e�[�g�̐���
	if( !CreateGenericSamplerStates() ){
		return false;
	}

	//	�ėp�u�����h�X�e�[�g�̐���
	if( !CreateGenericBlendStates() ){
		return false;
	}
	
	return true;
}

//---------------------------------------------------------------------------------------------------------------------------
//		@brief �X�V
//		@note mdxSystem::Update�Ŗ��t���[���Ă�
//---------------------------------------------------------------------------------------------------------------------------
void	RenderState::Update()
{
	// �f�v�X�X�e���V���X�e�[�g�̍�蒼��
	if (m_bUpdateDepthStencilState) {
		SAFE_RELEASE(m_pDepthStencilState);
		HRESULT	hr = S_OK;
		hr = MDX_GET_DEVICE->CreateDepthStencilState(&m_depthStencilDesc, &m_pDepthStencilState);
		assert(SUCCEEDED(hr) && "�f�v�X�X�e���V���X�e�[�g�č쐬���s�I�I");
		MDX_GET_IMMEDIATE_CONTEXT->OMSetDepthStencilState(m_pDepthStencilState, 0);
		m_bUpdateDepthStencilState = false;
	}

	//	���X�^���C�U�[�X�e�[�g�̍�蒼��
	if( m_bUpdateRasterizerState ){
		SAFE_RELEASE( m_pRasterizerState );
		HRESULT	hr = S_OK;
		hr = MDX_GET_DEVICE->CreateRasterizerState( &m_rasterizerDesc, &m_pRasterizerState );
		assert(SUCCEEDED(hr) && "���X�^���C�U�[�X�e�[�g�č쐬���s�I�I" );
		MDX_GET_IMMEDIATE_CONTEXT->RSSetState( m_pRasterizerState );
		m_bUpdateRasterizerState = false;
	}


	MDX_GET_IMMEDIATE_CONTEXT->OMSetDepthStencilState(m_pDepthStencilState, 0);
	MDX_GET_IMMEDIATE_CONTEXT->RSSetState(m_pRasterizerState);
}

//*********************************************************************************************
//		@name	�f�v�X�X�e���V���X�e�[�g�֘A
//*********************************************************************************************
//---------------------------------------------------------------------------------------------------------------------------
//		@brief �f�v�X�X�e���V���X�e�[�g����
//		@return ���������H
//---------------------------------------------------------------------------------------------------------------------------
bool	RenderState::CreateDepthStencilState()
{
	m_depthStencilDesc.DepthEnable		= TRUE;
	m_depthStencilDesc.DepthWriteMask	= D3D11_DEPTH_WRITE_MASK_ALL;
	m_depthStencilDesc.DepthFunc		= D3D11_COMPARISON_LESS_EQUAL;

	m_depthStencilDesc.StencilEnable	= false;
	m_depthStencilDesc.StencilReadMask	= 0xFF;
	m_depthStencilDesc.StencilWriteMask = 0xFF;

	m_depthStencilDesc.FrontFace.StencilFailOp		= D3D11_STENCIL_OP_KEEP;
	m_depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	m_depthStencilDesc.FrontFace.StencilPassOp		= D3D11_STENCIL_OP_KEEP;
	m_depthStencilDesc.FrontFace.StencilFunc		= D3D11_COMPARISON_ALWAYS;

	m_depthStencilDesc.BackFace.StencilFailOp		= D3D11_STENCIL_OP_KEEP;
	m_depthStencilDesc.BackFace.StencilDepthFailOp	= D3D11_STENCIL_OP_DECR;
	m_depthStencilDesc.BackFace.StencilPassOp		= D3D11_STENCIL_OP_KEEP;
	m_depthStencilDesc.BackFace.StencilFunc			= D3D11_COMPARISON_ALWAYS;

	HRESULT	hr = S_OK;
	hr = MDX_GET_DEVICE->CreateDepthStencilState(&m_depthStencilDesc, &m_pDepthStencilState);
	assert(SUCCEEDED(hr) && "�f�v�X�X�e���V���X�e�[�g�쐬���s�I�I");
	MDX_GET_IMMEDIATE_CONTEXT->OMSetDepthStencilState(m_pDepthStencilState, 0);

	return true;
}
//---------------------------------------------------------------------------------------------------------------------------
//		@brief �f�v�X�X�e���V���X�e�[�g�̃p�����[�^�ݒ�
//		@param[in] depthStencilDesc �f�v�X�X�e���V���X�e�[�g�̃p�����[�^
//---------------------------------------------------------------------------------------------------------------------------
void	RenderState::SetDepthStencilDesc(D3D11_DEPTH_STENCIL_DESC depthStencilDesc)
{
	m_depthStencilDesc			= depthStencilDesc;
	m_bUpdateDepthStencilState	= true;
}

//*********************************************************************************************
//		@name	���X�^���C�U�[�X�e�[�g�֘A
//*********************************************************************************************
//---------------------------------------------------------------------------------------------------------------------------
//		@brief ���X�^���C�U�[�X�e�[�g����
//---------------------------------------------------------------------------------------------------------------------------
bool	RenderState::CreateRasterizerState()
{
	m_rasterizerDesc.FillMode = D3D11_FILL_SOLID;
	m_rasterizerDesc.CullMode = D3D11_CULL_BACK;
	m_rasterizerDesc.FrontCounterClockwise = FALSE;
	m_rasterizerDesc.DepthBias = 0;
	m_rasterizerDesc.DepthBiasClamp = 0;
	m_rasterizerDesc.SlopeScaledDepthBias = 0;
	m_rasterizerDesc.DepthClipEnable = TRUE;
	m_rasterizerDesc.ScissorEnable = FALSE;
	m_rasterizerDesc.MultisampleEnable = FALSE;
	m_rasterizerDesc.AntialiasedLineEnable = FALSE;

	HRESULT	hr = S_OK;
	hr = MDX_GET_DEVICE->CreateRasterizerState( &m_rasterizerDesc, &m_pRasterizerState );
	assert( SUCCEEDED(hr) && "���X�^���C�U�[�X�e�[�g�쐬���s�I�I" );
	MDX_GET_IMMEDIATE_CONTEXT->RSSetState( m_pRasterizerState );

	return true;
}

//---------------------------------------------------------------------------------------------------------------------------
//		@brief ���X�^���C�U�[�X�e�[�g�̃p�����[�^�ݒ�
//		@param[in] rasterizerDesc ���X�^���C�U�[�X�e�[�g�̃p�����[�^
//---------------------------------------------------------------------------------------------------------------------------
void	RenderState::SetRasterizerDesc( D3D11_RASTERIZER_DESC rasterizerDesc )
{
	m_rasterizerDesc = rasterizerDesc;

	//	�X�V�v��
	m_bUpdateRasterizerState = true;
}


//*********************************************************************************************
//		@name	�T���v���[�X�e�[�g�֘A
//*********************************************************************************************
//---------------------------------------------------------------------------------------------------------------------------
//		@brief �T���v���[�X�e�[�g�ݒ�(�e�V�F�[�_�[���Ƃɂ���)
//		@param[in] samplerType SamplerStateType�̒l
//		@param[in] slotIndex �ݒ肷��X���b�g�ԍ�
//---------------------------------------------------------------------------------------------------------------------------
void	RenderState::SetVSSamplerState( SamplerStateType samplerType, int slotIndex )
{
	MDX_GET_IMMEDIATE_CONTEXT->VSSetSamplers( slotIndex, 1, &m_pSamplerState[samplerType] );
}

void	RenderState::SetHSSamplerState( SamplerStateType samplerType, int slotIndex )
{
	MDX_GET_IMMEDIATE_CONTEXT->HSSetSamplers( slotIndex, 1, &m_pSamplerState[samplerType] );
}

void	RenderState::SetDSSamplerState( SamplerStateType samplerType, int slotIndex )
{
	MDX_GET_IMMEDIATE_CONTEXT->DSSetSamplers( slotIndex, 1, &m_pSamplerState[samplerType] );
}

void	RenderState::SetGSSamplerState( SamplerStateType samplerType, int slotIndex )
{
	MDX_GET_IMMEDIATE_CONTEXT->GSSetSamplers( slotIndex, 1, &m_pSamplerState[samplerType] );
}

void	RenderState::SetPSSamplerState( SamplerStateType samplerType, int slotIndex )
{
	MDX_GET_IMMEDIATE_CONTEXT->PSSetSamplers( slotIndex, 1, &m_pSamplerState[samplerType] );
}

void	RenderState::SetCSSamplerState( SamplerStateType samplerType, int slotIndex )
{
	MDX_GET_IMMEDIATE_CONTEXT->CSSetSamplers( slotIndex, 1, &m_pSamplerState[samplerType] );
}

//---------------------------------------------------------------------------------------------------------------------------
//		@brief �ėp�T���v���[�X�e�[�g�̐���
//		@return ���������H
//---------------------------------------------------------------------------------------------------------------------------
bool	RenderState::CreateGenericSamplerStates()
{
	HRESULT	hr;

	D3D11_SAMPLER_DESC	sampDesc;
	ZeroMemory( &sampDesc, sizeof( sampDesc ) );
	sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	sampDesc.MinLOD = 0;
	sampDesc.MaxLOD = D3D11_FLOAT32_MAX;

	//---------------------------------------------------------------------------------------------------------------------------
	//		�A�h���b�V���O���[�h�����b�v
	//---------------------------------------------------------------------------------------------------------------------------
	sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;

	//	FILTER_ANISOTROPIC_ADDRESS_WRAP
	sampDesc.Filter = D3D11_FILTER_ANISOTROPIC;
	hr = MDX_GET_DEVICE->CreateSamplerState( &sampDesc, &m_pSamplerState[SamplerStateType::FILTER_ANISOTROPIC_ADDRESS_WRAP] );
	if( FAILED( hr ) ){ goto EXIT; }
	//	FILTER_LINEAR_ADDRESS_WRAP
	sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	hr = MDX_GET_DEVICE->CreateSamplerState( &sampDesc, &m_pSamplerState[SamplerStateType::FILTER_LINEAR_ADDRESS_WRAP] );
	if( FAILED( hr ) ){ goto EXIT; }
	//	FILTER_POINT_ADDRESS_WRAP
	sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
	hr = MDX_GET_DEVICE->CreateSamplerState( &sampDesc, &m_pSamplerState[SamplerStateType::FILTER_POINT_ADDRESS_WRAP] );
	if( FAILED( hr ) ){ goto EXIT; }

	//---------------------------------------------------------------------------------------------------------------------------
	//		�A�h���b�V���O���[�h���N�����v
	//---------------------------------------------------------------------------------------------------------------------------
	sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;

	//	FILTER_ANISOTROPIC_ADDRESS_CLAMP
	sampDesc.Filter = D3D11_FILTER_ANISOTROPIC;
	hr = MDX_GET_DEVICE->CreateSamplerState( &sampDesc, &m_pSamplerState[SamplerStateType::FILTER_ANISOTROPIC_ADDRESS_CLAMP] );
	if( FAILED( hr ) ){ goto EXIT; }
	//	FILTER_LINEAR_ADDRESS_CLAMP
	sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	hr =MDX_GET_DEVICE->CreateSamplerState( &sampDesc, &m_pSamplerState[SamplerStateType::FILTER_LINEAR_ADDRESS_CLAMP] );
	if( FAILED( hr ) ){ goto EXIT; }
	//	FILTER_POINT_ADDRESS_CLAMP
	sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
	hr = MDX_GET_DEVICE->CreateSamplerState( &sampDesc, &m_pSamplerState[SamplerStateType::FILTER_POINT_ADDRESS_CLAMP] );
	if( FAILED( hr ) ){ goto EXIT; }

	return true;
EXIT:
	assert(false && "�ėp�T���v���X�e�[�g�������s");
	return false;
}

//*********************************************************************************************
//		@name	�u�����h�X�e�[�g�֘A
//*********************************************************************************************
//---------------------------------------------------------------------------------------------------------------------------
//		@brief �ėp�u�����h�X�e�[�g�̐ݒ�
//		@param[in] blendState BlendStateType�̒l
//---------------------------------------------------------------------------------------------------------------------------
void	RenderState::SetBlendState( BlendStateType blendState )
{
	MDX_GET_IMMEDIATE_CONTEXT->OMSetBlendState( m_pBlendState[blendState], 0, 0xFFFFFFFF );
}
//---------------------------------------------------------------------------------------------------------------------------
//		@brief �u�����h�X�e�[�g�̐ݒ�
//		@param[in] blendState �u�����h�X�e�[�g
//---------------------------------------------------------------------------------------------------------------------------
void	RenderState::SetBlendStae( ID3D11BlendState* blendState )
{
	MDX_GET_IMMEDIATE_CONTEXT->OMSetBlendState( blendState, 0, 0xFFFFFFFF );
}

//---------------------------------------------------------------------------------------------------------------------------
//		@brief �ėp�u�����h�X�e�[�g�̐���
//		@return ���������H
//---------------------------------------------------------------------------------------------------------------------------
bool	RenderState::CreateGenericBlendStates()
{
	D3D11_RENDER_TARGET_BLEND_DESC	blendDesc[8];
	//	ALPHA
	for( int i=0 ; i<DXSystem::MRT_MAX ; i++ ){
		blendDesc[i] = GetAlphaBlend();
	}
	m_pBlendState[BlendStateType::ALPHA] = CreateBlendState( blendDesc, 8 );
	if( !m_pBlendState[BlendStateType::ALPHA] ){ goto EXIT; }
	//	ADD
	for( int i=0 ; i<DXSystem::MRT_MAX ; i++ ){
		blendDesc[i] = GetAddBlend();
	}
	m_pBlendState[BlendStateType::ADD] = CreateBlendState( blendDesc, 8 );
	if( !m_pBlendState[BlendStateType::ADD] ){ goto EXIT; }
	//	SUB
	for( int i=0 ; i<DXSystem::MRT_MAX ; i++ ){
		blendDesc[i] = GetSubBlend();
	}
	m_pBlendState[BlendStateType::SUB] = CreateBlendState( blendDesc, 8 );
	if( !m_pBlendState[BlendStateType::SUB] ){ goto EXIT; }
	//	MUL
	for( int i=0 ; i<DXSystem::MRT_MAX ; i++ ){
		blendDesc[i] = GetMulBlend();
	}
	m_pBlendState[BlendStateType::MUL] = CreateBlendState( blendDesc, 8 );
	if( !m_pBlendState[BlendStateType::MUL] ){ goto EXIT; }
	//	NONE
	for( int i=0 ; i<DXSystem::MRT_MAX ; i++ ){
		blendDesc[i] = GetNoneBlend();
	}
	m_pBlendState[BlendStateType::NONE] = CreateBlendState( blendDesc, 8 );
	if( !m_pBlendState[BlendStateType::NONE] ){ goto EXIT; }

	return true;

EXIT:
	assert(false && "�ėp�u�����h�X�e�[�g�쐬���s");
	return false;
}

//---------------------------------------------------------------------------------------------------------------------------
//		@brief �u�����h�X�e�[�g�̐���
//---------------------------------------------------------------------------------------------------------------------------
ID3D11BlendState*	RenderState::CreateBlendState( D3D11_RENDER_TARGET_BLEND_DESC blendState[], UINT numBlendState )
{
	//	�����_�[�^�[�Q�b�g��8�܂�
	if( numBlendState > DXSystem::MRT_MAX ) return NULL;

	ID3D11BlendState*	pBlendState = NULL;
	D3D11_BLEND_DESC	blendDesc;
	blendDesc.AlphaToCoverageEnable = TRUE;
	blendDesc.IndependentBlendEnable = TRUE;

	//	�u�����h���R�s�[
	for( UINT i=0 ; i<numBlendState ; i++ ){
		memcpy( &blendDesc.RenderTarget[i], &blendState[i], sizeof( D3D11_RENDER_TARGET_BLEND_DESC ) );
	}

	//	�u�����h�X�e�[�g�쐬
	HRESULT	hr = MDX_GET_DEVICE->CreateBlendState( &blendDesc, &pBlendState );
	if( FAILED( hr ) ){
		assert(false && "�u�����h�X�e�[�g�쐬���s�I�I");
		return NULL;
	}

	return pBlendState;
}

//---------------------------------------------------------------------------------------------------------------------------
//		@brief �ėp�u�����h�X�e�[�g�̃p�����[�^���擾
//		@return �u�����h�X�e�[�g�̃p�����[�^
//---------------------------------------------------------------------------------------------------------------------------
D3D11_RENDER_TARGET_BLEND_DESC	RenderState::GetBlend( BlendStateType blendState )
{
	D3D11_RENDER_TARGET_BLEND_DESC	blendDesc;

	switch( blendState )
	{
	case BlendStateType::ALPHA:	blendDesc = GetAlphaBlend();	break;
	case BlendStateType::ADD:	blendDesc = GetAddBlend();		break;
	case BlendStateType::SUB:		blendDesc = GetSubBlend();		break;
	case BlendStateType::MUL:		blendDesc = GetMulBlend();		break;
	case BlendStateType::NONE:	blendDesc = GetNoneBlend();	break;
	default:									blendDesc = GetNoneBlend();	break;
	}

	return blendDesc;
}

//---------------------------------------------------------------------------------------------------------------------------
//		@brief ���`�����p�u�����h�X�e�[�g�̃p�����[�^���擾
//		@return ���`�����p�u�����h�X�e�[�g
//---------------------------------------------------------------------------------------------------------------------------
D3D11_RENDER_TARGET_BLEND_DESC	RenderState::GetAlphaBlend()
{
	D3D11_RENDER_TARGET_BLEND_DESC	blendDesc;
	blendDesc.BlendEnable = TRUE;
	blendDesc.SrcBlend = D3D11_BLEND_SRC_ALPHA;
	blendDesc.DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	blendDesc.BlendOp = D3D11_BLEND_OP_ADD;
	blendDesc.SrcBlendAlpha = D3D11_BLEND_ONE;
	blendDesc.DestBlendAlpha = D3D11_BLEND_ZERO;
	blendDesc.BlendOpAlpha = D3D11_BLEND_OP_ADD;
	blendDesc.RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	return blendDesc;
}

//---------------------------------------------------------------------------------------------------------------------------
//		@brief ���Z�����p�u�����h�X�e�[�g�̃p�����[�^���擾
//		@return ���Z�����p�u�����h�X�e�[�g
//---------------------------------------------------------------------------------------------------------------------------
D3D11_RENDER_TARGET_BLEND_DESC	RenderState::GetAddBlend()
{
	D3D11_RENDER_TARGET_BLEND_DESC	blendDesc;
	blendDesc.BlendEnable = TRUE;
	blendDesc.SrcBlend = D3D11_BLEND_SRC_ALPHA;
	blendDesc.DestBlend = D3D11_BLEND_ONE;
	blendDesc.BlendOp = D3D11_BLEND_OP_ADD;
	blendDesc.SrcBlendAlpha = D3D11_BLEND_ONE;
	blendDesc.DestBlendAlpha = D3D11_BLEND_ZERO;
	blendDesc.BlendOpAlpha = D3D11_BLEND_OP_ADD;
	blendDesc.RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	return blendDesc;
}

//---------------------------------------------------------------------------------------------------------------------------
//		@brief ���Z�����p�u�����h�X�e�[�g�̃p�����[�^���擾
//		@return ���Z�����p�u�����h�X�e�[�g
//---------------------------------------------------------------------------------------------------------------------------
D3D11_RENDER_TARGET_BLEND_DESC	RenderState::GetSubBlend()
{
	D3D11_RENDER_TARGET_BLEND_DESC	blendDesc;
	blendDesc.BlendEnable = TRUE;
	blendDesc.SrcBlend = D3D11_BLEND_SRC_ALPHA;
	blendDesc.DestBlend = D3D11_BLEND_ONE;
	blendDesc.BlendOp = D3D11_BLEND_OP_REV_SUBTRACT;
	blendDesc.SrcBlendAlpha = D3D11_BLEND_ONE;
	blendDesc.DestBlendAlpha = D3D11_BLEND_ZERO;
	blendDesc.BlendOpAlpha = D3D11_BLEND_OP_ADD;
	blendDesc.RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	return blendDesc;
}

//---------------------------------------------------------------------------------------------------------------------------
//		@brief ��Z�����p�u�����h�X�e�[�g�̃p�����[�^���擾
//		@return ��Z�����p�u�����h�X�e�[�g
//---------------------------------------------------------------------------------------------------------------------------
D3D11_RENDER_TARGET_BLEND_DESC	RenderState::GetMulBlend()
{
	D3D11_RENDER_TARGET_BLEND_DESC	blendDesc;
	blendDesc.BlendEnable = TRUE;
	blendDesc.SrcBlend = D3D11_BLEND_ZERO;
	blendDesc.DestBlend = D3D11_BLEND_SRC_COLOR;
	blendDesc.BlendOp = D3D11_BLEND_OP_ADD;
	blendDesc.SrcBlendAlpha = D3D11_BLEND_ONE;
	blendDesc.DestBlendAlpha = D3D11_BLEND_ZERO;
	blendDesc.BlendOpAlpha = D3D11_BLEND_OP_ADD;
	blendDesc.RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	return blendDesc;
}

//---------------------------------------------------------------------------------------------------------------------------
//		@brief �����Ȃ��p�u�����h�X�e�[�g�̃p�����[�^���擾
//		@return �����Ȃ��p�u�����h�X�e�[�g
//---------------------------------------------------------------------------------------------------------------------------
D3D11_RENDER_TARGET_BLEND_DESC	RenderState::GetNoneBlend()
{
	D3D11_RENDER_TARGET_BLEND_DESC	blendDesc;
	blendDesc.BlendEnable = FALSE;
	blendDesc.SrcBlend = D3D11_BLEND_ONE;
	blendDesc.DestBlend = D3D11_BLEND_ZERO;
	blendDesc.BlendOp = D3D11_BLEND_OP_ADD;
	blendDesc.SrcBlendAlpha = D3D11_BLEND_ONE;
	blendDesc.DestBlendAlpha = D3D11_BLEND_ZERO;
	blendDesc.BlendOpAlpha = D3D11_BLEND_OP_ADD;
	blendDesc.RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	return blendDesc;
}
