/**
* @file MDX_DefaultVertexLayout.h
* @brief デフォルトの頂点レイアウトの定義
* @author Toru Miyawaki
*/

#ifndef MDX_DEFAULT_VERTEX_LAYOUT_H
#define MDX_DEFAULT_VERTEX_LAYOUT_H

#include <d3d11.h>
#include "../Math/MDX_Math.h"

/**
* @brief namespace MDX
*/
namespace MDX{
	/**
	* @brief 3D頂点レイアウト
	*/
	static const D3D11_INPUT_ELEMENT_DESC g_defaultVertexLayout3D[] = {
		{ "SV_Position", 0, DXGI_FORMAT_R32G32B32_FLOAT,    0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL",      0, DXGI_FORMAT_R32G32B32_FLOAT,    0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR",       0, DXGI_FORMAT_B8G8R8A8_UNORM,     0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TANGENT",     0, DXGI_FORMAT_R32G32B32_FLOAT,    0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "BINORMAL",    0, DXGI_FORMAT_R32G32B32_FLOAT,    0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD",    0, DXGI_FORMAT_R32G32_FLOAT,       0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "BLENDINDICES",0, DXGI_FORMAT_R8G8B8A8_UINT,      0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "BLENDWEIGHT", 0, DXGI_FORMAT_R8G8B8A8_UNORM,     0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	/**
	* @brief 3D頂点情報
	*/
	struct DefaultVertexInfo{
		Vector3f pos;
		Vector3f normal;
		UINT     color;
		Vector3f tangent;
		Vector3f binormal;
		Vector2f uv;
		UINT     blendIndices;
		UINT     blendWeight;
	};
	/**
	* @brief 2D描画の入力レイアウト
	*/
	static D3D11_INPUT_ELEMENT_DESC	g_defaultVertexLayout2D[] = {
		{ "SV_Position", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 1, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_B8G8R8A8_UNORM, 1, 8, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	/**
	* @brief 2D頂点情報
	*/
	struct DefaultVertex2DInfo{
		Vector3f pos;
		Vector2f uv;
		UINT     color;
	};

}

#endif // !MDX_DEFAULT_VERTEX_LAYOUT_H