#include "MDX_Font.h"
#include "../Utillity/MDX_Utillity.h"
#include "../System/MDX_System.h"

#include <stdio.h>
#include <stdlib.h>
#include <locale.h>

namespace MDX{
	Font::Font() : 
		m_spriteBatch(nullptr),
		m_spriteFont(nullptr)
	{
	}

	Font::~Font(){
		Release();
	}

	// ‰Šú‰»
	bool Font::Init(){
		m_spriteBatch = new DirectX::SpriteBatch(MDX_GET_IMMEDIATE_CONTEXT);
		m_spriteFont = new DirectX::SpriteFont(MDX_GET_DEVICE, L"DATA\\Font\\sprite_font.spf");

		return true;
	}

	// ‰ð•ú
	void Font::Release() {
		SAFE_DELETE(m_spriteBatch);
		SAFE_DELETE(m_spriteFont);
	}

	// •`‰æ
	void Font::Draw(const char* text, float x, float y){
		wchar_t wText[512];
		setlocale( LC_CTYPE, "jpn" );
		size_t size = 0;
		mbstowcs_s(&size, wText, text, 512);

		m_spriteBatch->Begin();
		m_spriteFont->DrawString(m_spriteBatch, wText, DirectX::XMFLOAT2(x, y));
		m_spriteBatch->End();
	}
}