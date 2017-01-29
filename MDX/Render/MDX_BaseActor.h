/**
* @file MDX_BaseActor.h
* @brief アクター基底クラス
* @author Toru Miyawaki
*/

#ifndef MDX_BASE_ACTOR_H
#define MDX_BASE_ACTOR_H

#include <d3d11.h>

#include <memory>
#include <functional>

#include "MDX_IActor.h"
#include "../Math/MDX_Math.h"
#include "../Shader/MDX_Shader.h"
#include "MDX_RenderManager.h"

typedef void(*OnBeginDrawPtr)(void*);

/**
* @brief namespace MDX
*/
namespace MDX{
	/**
	* @brief アクター基底クラス
	*/
	class BaseActor : public IActor
	{
	public:
		BaseActor();
		virtual ~BaseActor();

		/**
		* @brief 常駐データ初期化
		* アプリケーション初期化時に一回だけ呼び出し
		*/
		static bool InitResidentData();

		/**
		* @brief 常駐データ解放
		* アプリケーション解放時に一回だけ呼び出し
		*/
		static void ReleaseResidentData();

		/**
		* @brief 解放
		* 継承先の同メソッド内で呼び出すこと
		*/
		virtual void Release();

		/**
		* @brief 更新
		* 継承先の同メソッド内で呼び出すこと
		*/
		virtual void Update();

		/**
		* @brief 描画
		* 継承先の同メソッド内で呼び出すこと
		*/
		virtual void Draw();

		/**
		* @brief 変換行列コンスタントバッファ更新
		*/
		void BaseActor::UpdateCBMatrix();
		
		/**
		* @brief PBR用コンスタントバッファ更新
		*/
		void BaseActor::UpdateCBPbr();

		/**
		* @biref ゲッター
		*/
		Vector3f GetPos() const { return m_pos; }
		Vector3f GetAngle() const { return m_angle; }
		Vector3f GetScale() const { return m_scale; }
		RenderManager::CBPbr* GetPbrParam() { return &m_pbr; }
		std::shared_ptr<Shader> GetShader() { return m_shader; }

		/**
		* @brief セッター
		*/
		void SetPos(const Vector3f& pos){ m_pos = pos; }
		void SetAngle(const Vector3f& angle){ m_angle = angle; }
		void SetScale(const Vector3f& scale){ m_scale = scale; }
		void SetPbrParam(const RenderManager::CBPbr& pbr) { m_pbr = pbr; }
		void SetShader(std::shared_ptr<Shader> shader){ m_shader = shader; }

		void SetOnBeginDraw(OnBeginDrawPtr callback, void* arg){ m_OnBeginDraw = callback; m_OnBeginDrawArg = arg; }

	//-------------------------------------------------------------------------
	// PBRパラメーター
	//-------------------------------------------------------------------------
	public:
		/**
		* @biref ゲッター
		*/
		float GetRughness() const { return m_pbr.roughness; }
		float GetMetallic() const { return m_pbr.metallic; }

		/**
		* @biref セッター
		*/
		void SetRoughness(float roughness);
		void SetMetallic(float metallic);

	//-------------------------------------------------------------------------
	// コールバック
	//-------------------------------------------------------------------------
	protected:
		OnBeginDrawPtr	m_OnBeginDraw;
		void*			m_OnBeginDrawArg;

	protected:
		Vector3f m_pos = Vector3f(0, 0, 0);
		Vector3f m_angle = Vector3f(0, 0, 0);
		Vector3f m_scale = Vector3f(1, 1, 1);


		// TODO: 描画系変数がここにあるのはおかしい
		static ID3D11SamplerState* m_sampler;
		RenderManager::CBPbr m_pbr;
		std::shared_ptr<Shader>	m_shader;
	};

}

#endif // !MDX_BASE_ACTOR_H
