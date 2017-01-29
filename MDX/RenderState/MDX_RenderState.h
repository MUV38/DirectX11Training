#ifndef MDX_RENDER_STATE_H
#define MDX_RENDER_STATE_H

#include <d3d11.h>
#include "../DXSystem/MDX_DXSystem.h"

namespace MDX
{

	//*********************************************************************************************
	//		@name	サンプラーステート用定数
	//*********************************************************************************************
	enum SamplerStateType
	{
		//	異方性フィルタリング
		FILTER_ANISOTROPIC_ADDRESS_WRAP,
		FILTER_ANISOTROPIC_ADDRESS_CLAMP,
		//	線形フィルタリング
		FILTER_LINEAR_ADDRESS_WRAP,
		FILTER_LINEAR_ADDRESS_CLAMP,
		//	最近点サンプリング
		FILTER_POINT_ADDRESS_WRAP,
		FILTER_POINT_ADDRESS_CLAMP,

		SAMPLER_STATE_MAX
	};

	//*********************************************************************************************
	//		@name	ブレンドステート用定数
	//*********************************************************************************************
	enum BlendStateType
	{
		ALPHA,	//	線形合成
		ADD,		//	加算合成
		SUB,			// 減算合成
		MUL,		//	乗算合成
		NONE,		//	ブレンドなし
		BLEND_STATE_MAX
	};

	//*********************************************************************************************
	//		@name	描画状態設定クラス
	//*********************************************************************************************
	class RenderState
	{
	public:
		RenderState(){}
		~RenderState(){}

		//---------------------------------------------------------------------------------------------------------------------------
		//		@brief 解放
		//---------------------------------------------------------------------------------------------------------------------------
		static void	Release();

		//---------------------------------------------------------------------------------------------------------------------------
		//		@brief 初期化
		//		@note システムの初期化時に１回だけ呼び出す
		//---------------------------------------------------------------------------------------------------------------------------
		static bool	Initialize();

		//---------------------------------------------------------------------------------------------------------------------------
		//		@brief 更新
		//		@note mdxSystem::Updateで毎フレーム呼ぶ
		//---------------------------------------------------------------------------------------------------------------------------
		static void	Update();

		//*********************************************************************************************
		//		@name	デプスステンシルステート関連
		//*********************************************************************************************
		//---------------------------------------------------------------------------------------------------------------------------
		//		@brief デプスステンシルステートのパラメータ設定
		//		@param[in] depthStencilDesc デプスステンシルステートのパラメータ
		//---------------------------------------------------------------------------------------------------------------------------
		static void	SetDepthStencilDesc(D3D11_DEPTH_STENCIL_DESC depthStencilDesc);
		//---------------------------------------------------------------------------------------------------------------------------
		//		@brief ラスタライザーステートパラメータ取得
		//		@return ラスタライザーステートのパラメータ
		//---------------------------------------------------------------------------------------------------------------------------
		static D3D11_DEPTH_STENCIL_DESC	GetDepthStencilDesc() { return m_depthStencilDesc; }

		//*********************************************************************************************
		//		@name	ラスタライザーステート関連
		//*********************************************************************************************
		//---------------------------------------------------------------------------------------------------------------------------
		//		@brief ラスタライザーステートのパラメータ設定
		//		@param[in] rasterizerDesc ラスタライザーステートのパラメータ
		//---------------------------------------------------------------------------------------------------------------------------
		static void	SetRasterizerDesc( D3D11_RASTERIZER_DESC rasterizerDesc );
		//---------------------------------------------------------------------------------------------------------------------------
		//		@brief ラスタライザーステートパラメータ取得
		//		@return ラスタライザーステートのパラメータ
		//---------------------------------------------------------------------------------------------------------------------------
		static D3D11_RASTERIZER_DESC	GetRasterizerDesc(){ return m_rasterizerDesc; }

		//*********************************************************************************************
		//		@name	サンプラーステート関連
		//*********************************************************************************************
		//---------------------------------------------------------------------------------------------------------------------------
		//		@brief サンプラーステート設定(各シェーダーごとにある)
		//		@param[in] samplerType SamplerStateTypeの値
		//		@param[in] slotIndex スロット番号
		//---------------------------------------------------------------------------------------------------------------------------
		static void	SetVSSamplerState( SamplerStateType samplerType, int slotIndex );
		static void	SetHSSamplerState( SamplerStateType samplerType, int slotIndex );
		static void	SetDSSamplerState( SamplerStateType samplerType, int slotIndex );
		static void	SetGSSamplerState( SamplerStateType samplerType, int slotIndex );
		static void	SetPSSamplerState( SamplerStateType samplerType, int slotIndex );
		static void	SetCSSamplerState( SamplerStateType samplerType, int slotIndex );
	
		//*********************************************************************************************
		//		@name	ブレンドステート関連
		//*********************************************************************************************
		//---------------------------------------------------------------------------------------------------------------------------
		//		@brief ブレンドステートの生成
		//		@param[in] blendState 各レンダーターゲット用のブレンドのパラメーター
		//		@param[in] numBlendState 設定するブレンド(レンダーターゲット)の数
		//		@return 生成されたブレンドステート
		//---------------------------------------------------------------------------------------------------------------------------
		static ID3D11BlendState*	CreateBlendState( D3D11_RENDER_TARGET_BLEND_DESC blendState[], UINT numBlendState );
		//---------------------------------------------------------------------------------------------------------------------------
		//		@brief 汎用ブレンドステートの設定
		//		@param[in] blendState BlendStateTypeの値
		//---------------------------------------------------------------------------------------------------------------------------
		static void	SetBlendState( BlendStateType blendState );
		//---------------------------------------------------------------------------------------------------------------------------
		//		@brief ブレンドステートの設定
		//		@param[in] blendState ブレンドステート
		//---------------------------------------------------------------------------------------------------------------------------
		static void	SetBlendStae( ID3D11BlendState* blendState );
		//---------------------------------------------------------------------------------------------------------------------------
		//		@brief 汎用ブレンドステートのパラメータを取得
		//		@param[in] blendState BlendStateTypeの値
		//		@return ブレンドステートのパラメータ
		//---------------------------------------------------------------------------------------------------------------------------
		static D3D11_RENDER_TARGET_BLEND_DESC	GetBlend( BlendStateType blendState );
	private:
		//	デプスステンシルステート
		static ID3D11DepthStencilState* m_pDepthStencilState;
		static D3D11_DEPTH_STENCIL_DESC	m_depthStencilDesc;
		static bool						m_bUpdateDepthStencilState;
		//	ラスタライザーステート
		static ID3D11RasterizerState*	m_pRasterizerState;
		static D3D11_RASTERIZER_DESC	m_rasterizerDesc;
		static bool						m_bUpdateRasterizerState;	//	ラスタライザーステートを必要な時に作り直すため
		//	汎用サンプラーステート
		static ID3D11SamplerState*		m_pSamplerState[SamplerStateType::SAMPLER_STATE_MAX];
		//	汎用ブレンドステート
		static ID3D11BlendState*		m_pBlendState[BlendStateType::BLEND_STATE_MAX];

		//*********************************************************************************************
		//		@name	デプスステンシルステート関連
		//*********************************************************************************************
		//---------------------------------------------------------------------------------------------------------------------------
		//		@brief デプスステンシルステート生成
		//		@return 生成成功？
		//---------------------------------------------------------------------------------------------------------------------------
		static bool	CreateDepthStencilState();

		//*********************************************************************************************
		//		@name	ラスタライザーステート関連
		//*********************************************************************************************
		//---------------------------------------------------------------------------------------------------------------------------
		//		@brief ラスタライザーステート生成
		//		@return 生成成功？
		//---------------------------------------------------------------------------------------------------------------------------
		static bool	CreateRasterizerState();

		//*********************************************************************************************
		//		@name	サンプラーステート関連
		//*********************************************************************************************
		//---------------------------------------------------------------------------------------------------------------------------
		//		@brief 汎用サンプラーステートの生成
		//		@return 生成成功？
		//---------------------------------------------------------------------------------------------------------------------------
		static bool	CreateGenericSamplerStates();

		//*********************************************************************************************
		//		@name	ブレンドステート関連
		//*********************************************************************************************
		//---------------------------------------------------------------------------------------------------------------------------
		//		@brief 汎用ブレンドステートの生成
		//		@return 生成成功？
		//---------------------------------------------------------------------------------------------------------------------------
		static bool	CreateGenericBlendStates();
		//---------------------------------------------------------------------------------------------------------------------------
		//		@brief 線形合成用ブレンドステートのパラメータを取得
		//		@return 線形合成用ブレンドステート
		//---------------------------------------------------------------------------------------------------------------------------
		static D3D11_RENDER_TARGET_BLEND_DESC	GetAlphaBlend();
		//---------------------------------------------------------------------------------------------------------------------------
		//		@brief 加算合成用ブレンドステートのパラメータを取得
		//		@return 加算合成用ブレンドステート
		//---------------------------------------------------------------------------------------------------------------------------
		static D3D11_RENDER_TARGET_BLEND_DESC	GetAddBlend();
		//---------------------------------------------------------------------------------------------------------------------------
		//		@brief 減算合成用ブレンドステートのパラメータを取得
		//		@return 減算合成用ブレンドステート
		//---------------------------------------------------------------------------------------------------------------------------
		static D3D11_RENDER_TARGET_BLEND_DESC	GetSubBlend();
		//---------------------------------------------------------------------------------------------------------------------------
		//		@brief 乗算合成用ブレンドステートのパラメータを取得
		//		@return 乗算合成用ブレンドステート
		//---------------------------------------------------------------------------------------------------------------------------
		static D3D11_RENDER_TARGET_BLEND_DESC	GetMulBlend();
		//---------------------------------------------------------------------------------------------------------------------------
		//		@brief 合成なし用ブレンドステートのパラメータを取得
		//		@return 合成なし用ブレンドステート
		//---------------------------------------------------------------------------------------------------------------------------
		static D3D11_RENDER_TARGET_BLEND_DESC	GetNoneBlend();

	};// class mdxRenderState
}// namespace MDX

#endif // !MDX_RENDER_STATE_H
