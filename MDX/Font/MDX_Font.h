/**
* @file MDX_Font.h
* @brief フォント描画クラスの定義
* @author Toru Miyawaki
*/

#ifndef MDX_FONT_H
#define MDX_FONT_H

#include <SpriteFont.h>
#include <SpriteBatch.h>

/**
* @brief namespace MDX
*/
namespace MDX{
	/**
	* @brief フォント描画クラス
	*/
	class Font{
	public:
		Font();
		~Font();

		/**
		* @brief 初期化
		*/
		bool Init();

		/**
		* @brief 解放
		*/
		void Release();

		/**
		* @brief 描画
		* @param [in] text 描画テキスト
		* @param [in] x X座標
		* @param [in] y Y座標
		*/
		void Draw(const char* text, float x, float y);
	private:
		DirectX::SpriteBatch* m_spriteBatch;
		DirectX::SpriteFont* m_spriteFont;
	};
}

#endif // !MDX_FONT_H
