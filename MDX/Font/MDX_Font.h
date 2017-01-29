/**
* @file MDX_Font.h
* @brief �t�H���g�`��N���X�̒�`
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
	* @brief �t�H���g�`��N���X
	*/
	class Font{
	public:
		Font();
		~Font();

		/**
		* @brief ������
		*/
		bool Init();

		/**
		* @brief ���
		*/
		void Release();

		/**
		* @brief �`��
		* @param [in] text �`��e�L�X�g
		* @param [in] x X���W
		* @param [in] y Y���W
		*/
		void Draw(const char* text, float x, float y);
	private:
		DirectX::SpriteBatch* m_spriteBatch;
		DirectX::SpriteFont* m_spriteFont;
	};
}

#endif // !MDX_FONT_H
