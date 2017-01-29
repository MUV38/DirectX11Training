#ifndef	MDX_SPRITE_H
#define	MDX_SPRITE_H

#include <d3d11.h>
#include "../Math/MDX_Math.h"
#include "../Texture/MDX_Texture.h"
#include "../Shader/MDX_Shader.h"

namespace MDX
{
	/**
	* @brief 2D頂点情報構造体
	*/
	struct mdx2DVertex
	{
		float	x, y, z;
	};
	/**
	* @brief 描画時に書き換える2D頂点情報構造体
	*/
	struct mdx2DVertexRewrite
	{
		float tu, tv;
		DWORD color;
	};
	/**
	* @brief 2D描画の入力レイアウト
	*/
	//static D3D11_INPUT_ELEMENT_DESC	g_inputLayout2D[] = {
	//	{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	//	{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 1, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	//	{ "COLOR", 0, DXGI_FORMAT_B8G8R8A8_UNORM, 1, 8, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	//};
	/**
	* @brief 2D頂点変換行列用コンスタントバッファ
	*/
	struct mdxCB2DTransformMatrix
	{
		Matrix4X4	matWorld;
		Matrix4X4	matProjection;
	};

	//*********************************************************************************************
	//		@name ポリゴン描画クラス
	//*********************************************************************************************
	class Sprite
	{
	public:
		Sprite();
		~Sprite();

		/**
		* @brief 初期化
		* @return 初期化成功か?
		*/
		static bool Initialize();

		/**
		* @brief 解放
		*/
		static void Release();

		/**
		* @brief 2Ｄポリゴン描画
		* @param[in] x X座標
		* @param[in] y Y座標
		* @param[in] width 横幅
		* @param[in] height 縦幅
		* @param[in] srcX 切り取り開始位置のＸ座標
		* @param[in] srcY 切り取り開始位置のＹ座標
		* @param[in] srcWidth 横幅の切り取りサイズ
		* @param[in] srcHeight 縦幅の切り取りサイズ
		* @param[in] texture テクスチャ(レジスタはt0固定)
		* @param[in] angle 回転量(ラジアン)
		* @param[in] scale スケール
		* @param[in] color 色
		*/
		static void Render2D( int x, int y, int width, int height, int srcX, int srcY, int srcWidth, int srcHeight, Texture* texture=nullptr, float angle = 0, float scale = 1, DWORD color = 0xFFFFFFFF );

		/**
		* @brief 2Ｄポリゴン描画
		* @param[in] x X座標
		* @param[in] y Y座標
		* @param[in] width 横幅
		* @param[in] height 縦幅
		* @param[in] shaderResource テクスチャ(レジスタはt0固定)
		* @param[in] texWidth テクスチャ横幅
		* @param[in] texHeight テクスチャ縦幅
		* @param[in] angle 回転量(ラジアン)
		* @param[in] scale スケール
		* @param[in] color 色
		*/
		static void Render2D( int x, int y, int width, int height,
							  MDX::Shader* shader = nullptr,
							  ID3D11ShaderResourceView* shaderResource=nullptr, UINT texWidth = 1, UINT texHeight=1,
							  float angle = 0, float scale = 1, DWORD color = 0xFFFFFFFF );

	private:
		//	2D用バッファ
		static ID3D11Buffer*	m_pVB2D;		//	変更されない
		static ID3D11Buffer*	m_pVB2DRewrite;	//	描画時に書き換える
		static ID3D11Buffer*	m_pIB2D;
		static ID3D11Buffer*	m_pCB2DTransform;
		static Shader			m_shader;


	}; // class Sprite
} // namespace MDX

#endif // MDX_SPRITE_H
