#ifndef _MDX_BASE_MESH_H
#define	_MDX_BASE_MESH_H

#include "../Render/MDX_BaseActor.h"

/**
* @brief namespace MDX
*/
namespace MDX
{
	/**
	* @brief メッシュ基底クラス
	*/
	class BaseMesh : public BaseActor
	{
	public:
		BaseMesh(){}
		virtual ~BaseMesh(){}

		/**
		* @brief ファイル読み込み
		* @param [in] filename ファイル名
		* @return 読み込み成功か？
		*/
		virtual bool Load(const char* filename) = 0;

		/**
		* @brief 解放
		*/
		virtual void Release() = 0;

		/**
		* @brief 更新
		*/
		virtual void Update() = 0;

		/**
		* @brief 描画
		*/
		virtual void Draw() = 0;

	private:

	};
}

#endif // !_MDX_BASE_MESH_H
