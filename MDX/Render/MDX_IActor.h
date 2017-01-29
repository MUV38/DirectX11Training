/**
* @file MDX_IActor.h
* @brief アクターの定義
* @author Toru Miyawaki
*/

#ifndef MDX_IACTOR_H
#define MDX_IACTOR_H

/**
* @brief namespace MDX
*/
namespace MDX{
	/**
	* @brief アクタークラス
	*/
	class IActor{
	public:
		virtual ~IActor(){}

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

		/**
		* @brief 変換行列コンスタントバッファ更新
		*/
		virtual void UpdateCBMatrix() = 0;
		
		/**
		* @brief PBR用コンスタントバッファ更新
		*/
		virtual void UpdateCBPbr() = 0;

	};

}

#endif // !MDX_IACTOR_H
