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
//		@brief 解放
//---------------------------------------------------------------------------------------------------------------------------
void	RenderState::Release()
{
	// デプスステンシルステートの解放
	SAFE_RELEASE(m_pDepthStencilState);
	//	ラスタライザーステートの解放
	SAFE_RELEASE( m_pRasterizerState );
	//	サンプラーステートの解放
	for( int i=0 ; i<SamplerStateType::SAMPLER_STATE_MAX ; i++ ){
		SAFE_RELEASE( m_pSamplerState[i] );
	}
	//	ブレンドステートの解放
	for( int i=0 ; i<BlendStateType::BLEND_STATE_MAX ; i++ ){
		SAFE_RELEASE( m_pBlendState[i] );
	}
}

//---------------------------------------------------------------------------------------------------------------------------
//		@brief 初期化
//		@note システムの初期化時に１回だけ呼び出す
//---------------------------------------------------------------------------------------------------------------------------
bool	RenderState::Initialize()
{
	//	デプスステンシルステートの生成
	if (!CreateDepthStencilState()) {
		return false;
	}

	//	ラスタライザーステートの生成
	if( !CreateRasterizerState() ){
		return false;
	}

	//	汎用サンプラーステートの生成
	if( !CreateGenericSamplerStates() ){
		return false;
	}

	//	汎用ブレンドステートの生成
	if( !CreateGenericBlendStates() ){
		return false;
	}
	
	return true;
}

//---------------------------------------------------------------------------------------------------------------------------
//		@brief 更新
//		@note mdxSystem::Updateで毎フレーム呼ぶ
//---------------------------------------------------------------------------------------------------------------------------
void	RenderState::Update()
{
	// デプスステンシルステートの作り直し
	if (m_bUpdateDepthStencilState) {
		SAFE_RELEASE(m_pDepthStencilState);
		HRESULT	hr = S_OK;
		hr = MDX_GET_DEVICE->CreateDepthStencilState(&m_depthStencilDesc, &m_pDepthStencilState);
		assert(SUCCEEDED(hr) && "デプスステンシルステート再作成失敗！！");
		MDX_GET_IMMEDIATE_CONTEXT->OMSetDepthStencilState(m_pDepthStencilState, 0);
		m_bUpdateDepthStencilState = false;
	}

	//	ラスタライザーステートの作り直し
	if( m_bUpdateRasterizerState ){
		SAFE_RELEASE( m_pRasterizerState );
		HRESULT	hr = S_OK;
		hr = MDX_GET_DEVICE->CreateRasterizerState( &m_rasterizerDesc, &m_pRasterizerState );
		assert(SUCCEEDED(hr) && "ラスタライザーステート再作成失敗！！" );
		MDX_GET_IMMEDIATE_CONTEXT->RSSetState( m_pRasterizerState );
		m_bUpdateRasterizerState = false;
	}


	MDX_GET_IMMEDIATE_CONTEXT->OMSetDepthStencilState(m_pDepthStencilState, 0);
	MDX_GET_IMMEDIATE_CONTEXT->RSSetState(m_pRasterizerState);
}

//*********************************************************************************************
//		@name	デプスステンシルステート関連
//*********************************************************************************************
//---------------------------------------------------------------------------------------------------------------------------
//		@brief デプスステンシルステート生成
//		@return 生成成功？
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
	assert(SUCCEEDED(hr) && "デプスステンシルステート作成失敗！！");
	MDX_GET_IMMEDIATE_CONTEXT->OMSetDepthStencilState(m_pDepthStencilState, 0);

	return true;
}
//---------------------------------------------------------------------------------------------------------------------------
//		@brief デプスステンシルステートのパラメータ設定
//		@param[in] depthStencilDesc デプスステンシルステートのパラメータ
//---------------------------------------------------------------------------------------------------------------------------
void	RenderState::SetDepthStencilDesc(D3D11_DEPTH_STENCIL_DESC depthStencilDesc)
{
	m_depthStencilDesc			= depthStencilDesc;
	m_bUpdateDepthStencilState	= true;
}

//*********************************************************************************************
//		@name	ラスタライザーステート関連
//*********************************************************************************************
//---------------------------------------------------------------------------------------------------------------------------
//		@brief ラスタライザーステート生成
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
	assert( SUCCEEDED(hr) && "ラスタライザーステート作成失敗！！" );
	MDX_GET_IMMEDIATE_CONTEXT->RSSetState( m_pRasterizerState );

	return true;
}

//---------------------------------------------------------------------------------------------------------------------------
//		@brief ラスタライザーステートのパラメータ設定
//		@param[in] rasterizerDesc ラスタライザーステートのパラメータ
//---------------------------------------------------------------------------------------------------------------------------
void	RenderState::SetRasterizerDesc( D3D11_RASTERIZER_DESC rasterizerDesc )
{
	m_rasterizerDesc = rasterizerDesc;

	//	更新要求
	m_bUpdateRasterizerState = true;
}


//*********************************************************************************************
//		@name	サンプラーステート関連
//*********************************************************************************************
//---------------------------------------------------------------------------------------------------------------------------
//		@brief サンプラーステート設定(各シェーダーごとにある)
//		@param[in] samplerType SamplerStateTypeの値
//		@param[in] slotIndex 設定するスロット番号
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
//		@brief 汎用サンプラーステートの生成
//		@return 生成成功？
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
	//		アドレッシングモードがラップ
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
	//		アドレッシングモードがクランプ
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
	assert(false && "汎用サンプラステート生成失敗");
	return false;
}

//*********************************************************************************************
//		@name	ブレンドステート関連
//*********************************************************************************************
//---------------------------------------------------------------------------------------------------------------------------
//		@brief 汎用ブレンドステートの設定
//		@param[in] blendState BlendStateTypeの値
//---------------------------------------------------------------------------------------------------------------------------
void	RenderState::SetBlendState( BlendStateType blendState )
{
	MDX_GET_IMMEDIATE_CONTEXT->OMSetBlendState( m_pBlendState[blendState], 0, 0xFFFFFFFF );
}
//---------------------------------------------------------------------------------------------------------------------------
//		@brief ブレンドステートの設定
//		@param[in] blendState ブレンドステート
//---------------------------------------------------------------------------------------------------------------------------
void	RenderState::SetBlendStae( ID3D11BlendState* blendState )
{
	MDX_GET_IMMEDIATE_CONTEXT->OMSetBlendState( blendState, 0, 0xFFFFFFFF );
}

//---------------------------------------------------------------------------------------------------------------------------
//		@brief 汎用ブレンドステートの生成
//		@return 生成成功？
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
	assert(false && "汎用ブレンドステート作成失敗");
	return false;
}

//---------------------------------------------------------------------------------------------------------------------------
//		@brief ブレンドステートの生成
//---------------------------------------------------------------------------------------------------------------------------
ID3D11BlendState*	RenderState::CreateBlendState( D3D11_RENDER_TARGET_BLEND_DESC blendState[], UINT numBlendState )
{
	//	レンダーターゲットは8つまで
	if( numBlendState > DXSystem::MRT_MAX ) return NULL;

	ID3D11BlendState*	pBlendState = NULL;
	D3D11_BLEND_DESC	blendDesc;
	blendDesc.AlphaToCoverageEnable = TRUE;
	blendDesc.IndependentBlendEnable = TRUE;

	//	ブレンドをコピー
	for( UINT i=0 ; i<numBlendState ; i++ ){
		memcpy( &blendDesc.RenderTarget[i], &blendState[i], sizeof( D3D11_RENDER_TARGET_BLEND_DESC ) );
	}

	//	ブレンドステート作成
	HRESULT	hr = MDX_GET_DEVICE->CreateBlendState( &blendDesc, &pBlendState );
	if( FAILED( hr ) ){
		assert(false && "ブレンドステート作成失敗！！");
		return NULL;
	}

	return pBlendState;
}

//---------------------------------------------------------------------------------------------------------------------------
//		@brief 汎用ブレンドステートのパラメータを取得
//		@return ブレンドステートのパラメータ
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
//		@brief 線形合成用ブレンドステートのパラメータを取得
//		@return 線形合成用ブレンドステート
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
//		@brief 加算合成用ブレンドステートのパラメータを取得
//		@return 加算合成用ブレンドステート
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
//		@brief 減算合成用ブレンドステートのパラメータを取得
//		@return 減算合成用ブレンドステート
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
//		@brief 乗算合成用ブレンドステートのパラメータを取得
//		@return 乗算合成用ブレンドステート
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
//		@brief 合成なし用ブレンドステートのパラメータを取得
//		@return 合成なし用ブレンドステート
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
