#ifndef _MDX_DXTKMESH_H_
#define	_MDX_DXTKMESH_H_

#include <memory>

#include <Model.h>
#include <Effects.h>

#include "MDX_BaseMesh.h"

/**
* @brief namespace MDX
*/
namespace MDX
{
	/**
	* @brief DirectXTKメッシュクラス
	*/
	class DXTKMesh : public BaseMesh
	{
	public:
		DXTKMesh();
		~DXTKMesh();

		/**
		* @brief ファイル読み込み
		* @param [in] device	デバイス
		* @param [in] filename	ファイル名
		* @return 読み込み成功か？
		*/
		bool Load(const char* filename) override;

		/**
		* @brief 解放
		*/
		void Release() override;

		/**
		* @brief 更新
		*/
		void Update() override;

		/**
		* @brief 描画
		*/
		void Draw() override;

	private:

	private:
		std::shared_ptr<DirectX::Model>			m_model;
		std::unique_ptr<DirectX::EffectFactory>	m_effectFactory;
	};
}

#endif // !_MDX_DXTKMESH_H_
