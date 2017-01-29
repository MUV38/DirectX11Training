#include "MDX_BaseActor.h"
#include "../System/MDX_System.h"
#include "../Utillity/MDX_Utillity.h"

namespace MDX{
	// 静的メンバ変数
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
	* @brief 常駐データ初期化
	* アプリケーション初期化時に一回だけ呼び出し
	*/
	bool BaseActor::InitResidentData(){

		D3D11_SAMPLER_DESC samplerDesc;
		samplerDesc.Filter = D3D11_FILTER_ANISOTROPIC;         // サンプリング時に使用するフィルタ。ここでは異方性フィルターを使用する。
		samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;     // 0 ～ 1 の範囲外にある u テクスチャー座標の描画方法
		samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;     // 0 ～ 1 の範囲外にある v テクスチャー座標
		samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;     // 0 ～ 1 の範囲外にある w テクスチャー座標
		samplerDesc.MipLODBias = 0;                            // 計算されたミップマップ レベルからのバイアス
		samplerDesc.MaxAnisotropy = 16;                        // サンプリングに異方性補間を使用している場合の限界値。有効な値は 1 ～ 16 。
		samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;  // 比較オプション。
		// 境界色
		for(int i=0 ; i<4 ; i++){
			samplerDesc.BorderColor[0] = 0.0f;
		}
		samplerDesc.MinLOD = 0;                                // アクセス可能なミップマップの下限値
		samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;                // アクセス可能なミップマップの上限値
		MDX_GET_DEVICE->CreateSamplerState(&samplerDesc, &m_sampler);

		return true;
	}

	/**
	* @brief 常駐データ解放
	* アプリケーション解放時に一回だけ呼び出し
	*/
	void BaseActor::ReleaseResidentData(){
		SAFE_RELEASE(m_sampler);
	}

	// 解放
	void BaseActor::Release(){
	}

	// 更新
	void BaseActor::Update(){
	}

	// 描画
	void BaseActor::Draw(){
		// 描画開始コールバック
		if (m_OnBeginDraw) {
			m_OnBeginDraw(m_OnBeginDrawArg);
		}

		// TODO: この処理は別のクラスに移す
		if (m_shader) {
			m_shader->SetShader();
		}else {
			MDX_GET_RENDER_MANAGER->GetDefaultShader()->SetShader();
		}
	}

	// 変換行列コンスタントバッファ更新
	void BaseActor::UpdateCBMatrix()
	{
		// ワールド変換行列
		DirectX::XMMATRIX mat = DirectX::XMMatrixIdentity();
		mat *= DirectX::XMMatrixScaling(m_scale.x, m_scale.y, m_scale.z);
		mat *= DirectX::XMMatrixRotationX(m_angle.x);
		mat *= DirectX::XMMatrixRotationY(m_angle.y);
		mat *= DirectX::XMMatrixRotationZ(m_angle.z);
		mat *= DirectX::XMMatrixTranslation(m_pos.x, m_pos.y, m_pos.z);
		mat = DirectX::XMMatrixTranspose(mat);
		DirectX::XMFLOAT4X4 matWorld;
		DirectX::XMStoreFloat4x4(&matWorld, mat);

		// レンダーマネージャーへ
		MDX_GET_RENDER_MANAGER->UpdateCBMatrix(matWorld);
	}

	// PBR用コンスタントバッファ更新
	void BaseActor::UpdateCBPbr(){
		// レンダーマネージャーへ
		MDX_GET_RENDER_MANAGER->UpdateCBPbr(&m_pbr);
	}

	//-------------------------------------------------------------------------
	// PBRパラメーター
	//-------------------------------------------------------------------------
	void BaseActor::SetRoughness(float roughness) {
		m_pbr.roughness = MDX::Clamp<float>(0, 1, roughness);
	}
	void BaseActor::SetMetallic(float metallic) {
		m_pbr.metallic = MDX::Clamp<float>(0, 1, metallic);
	}
}