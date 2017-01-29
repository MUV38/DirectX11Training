/**
* @file MDX_RenderManager.h
* @brief レンダーマネージャーの定義
* @author Toru Miyawaki
*/

#ifndef MDX_RENDER_MANAGER_H
#define MDX_RENDER_MANAGER_H

#include <d3d11.h>

#include <vector>
#include <memory>

#include "../Shader/MDX_Shader.h"
#include "../Math/MDX_Math.h"
#include "../Texture/MDX_Texture.h"
#include "MDX_RenderTarget.h"
#include "MDX_ConstantBuffer.h"

/**
* @brief namespace MDX
*/
namespace MDX{
	class IActor;

	/**
	* @brief レンダーマネージャー
	*/
	class RenderManager{
	public:
		/**
		* @brief 変換行列コンスタントバッファ
		*/
		struct CBMatrix{
			Matrix4X4 matWorld;
			Matrix4X4 matView;
			Matrix4X4 matProjection;
		};
		/**
		* @brief シェーダー環境関連変数
		*/
		struct CBEnvironment{
			Vector3f viewPos;
			float tmp_viewPos;
			Vector3f vecLight;
			float tmp_vecLight;
		};

		/**
		* @brief 物理ベースレンダリング用コンスタントバッファ
		*/
		struct CBPbr{
			Vector4f	baseColor; // 便宜上のパラメータ
			float		roughness;
			float		metallic;
			Vector2f	tmp;
		};

	public:
		RenderManager();
		~RenderManager();

		/**
		* @brief 初期化
		* @return 初期化成功か？
		*/
		bool Init();

		/**
		* @brief 解放
		*/
		void Release();

		/**
		* @brief 更新
		*/
		void Update();

		/**
		* @brief 描画
		*/
		void Draw();

		/**
		* @brief アクターの追加
		* @param [in] アクタークラス
		*/
		void AddActor(IActor* actor);

		/**
		* @brief 変換行列コンスタントバッファ更新
		* @param [in] world ワールド変換行列
		* IActorクラスを継承したものはそれぞれの場所で呼び出す
		*/
		void UpdateCBMatrix(const Matrix4X4& world);

		/**
		* @brief 環境用コンスタントバッファ更新
		* @param [in] environment コンスタントバッファ更新に使う情報
		* どこかで呼び出す
		*/
		void UpdateCBEnvironmment(CBEnvironment* environment);

		/**
		* @brief PBR用コンスタントバッファ更新
		* @param [in] pbr PBRに使う情報
		* IActorクラスを継承したものはそれぞれの場所で呼び出す
		*/
		void UpdateCBPbr(CBPbr* pbr);

		/**
		* @brief ゲッター
		*/
		Matrix4X4 GetMatWorld() const { return m_matrix.matWorld; }
		Matrix4X4 GetMatView() const { return m_matrix.matView; }
		Matrix4X4 GetMatProjection() const { return m_matrix.matProjection; }
		std::shared_ptr<Shader> GetDefaultShader() const { return m_shader; }

		/**
		* @brief セッター
		*/
		void SetMatWorld(const Matrix4X4& world){ m_matrix.matWorld = world; }
		void SetMatView(const Matrix4X4& view){ m_matrix.matView = view; }
		void SetMatProjection(const Matrix4X4& prj){ m_matrix.matProjection = prj; }

	private:
		//レンダーターゲットの種類
		enum RenderTargetType{
			RT_HDR,
			RT_DOWN_SCALED_HDR,
			RT_BRIGHT_PASS_FILTER,
			RT_BLOOM,
			RT_BLOOM_FINAL,
			RT_LUMINANCE,
			RT_TYPE_MAX
		};

		static const int NUM_TONEMAP_TEXTURE = 4;
		static const int NUM_BLOOM_TEXTURE = 4;

		// ダウンスケール4x4用コンスタントバッファ
		struct CBDownScale4x4 {
			Vector2f	sampleOffset[4 * 4];
		};

		// 輝度計算初回用コンスタントバッファ
		struct CBSampleLumInitial {
			Vector2f	sampleOffset[3 * 3];
			Vector2f	tmp;
		};

	private:
		/**
		* @brief アクターの描画
		*/
		void DrawActor(IActor* actor);

		/**
		* @brief ダウンスケール4x4のサンプルオフセット取得
		*/
		void GetDownScale4x4SampleOffset(UINT width, UINT height, Vector2f* sampleOffset);

	private:
		// シーンをダウンスケール
		void Scene_DownScale();

		// 輝度計算
		void CalculateLuminance();

		// 順応輝度計算
		void CalculateAdaptionLuminance();

	private:
		std::shared_ptr<Shader> m_shader;
		std::shared_ptr<Shader> m_shaderDownScale4x4;
		std::shared_ptr<Shader> m_shaderSampleLumInitial;
		std::shared_ptr<Shader> m_shaderSampleLumInterative;
		std::shared_ptr<Shader> m_shaderSampleLumFinal;
		std::shared_ptr<Shader> m_shaderPostEffect;

		std::shared_ptr<Texture>	m_defaultIrradianceTex;
		std::shared_ptr<Texture>	m_defaultRadianceTex;

		ID3D11Buffer* m_cbMatrix = nullptr;
		ID3D11Buffer* m_cbEnvironment = nullptr;
		ID3D11Buffer* m_cbPbr = nullptr;

		std::shared_ptr<ConstantBuffer> m_cbDownScale4x4 = nullptr;
		std::shared_ptr<ConstantBuffer> m_cbSampleLumInitial = nullptr;

		std::vector<IActor*> m_actor;

		CBMatrix m_matrix;

		RenderTarget m_renderTarget[RT_TYPE_MAX];
		RenderTarget m_toneMapTexture[NUM_TONEMAP_TEXTURE];
		RenderTarget m_bloomTexture[NUM_BLOOM_TEXTURE];
		RenderTarget m_adaptionLumTexture[2];

		RenderTarget* m_adaptedLumLast = nullptr;
		RenderTarget* m_adaptedLumCur = nullptr;
	};


}

#endif // !MDX_RENDER_MANAGER_H
