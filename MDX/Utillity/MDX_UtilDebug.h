/**
* @file MDX_UtilDebug.h
* @brief デバッグ関連に使えるものをまとめたファイル
* @author Toru Miyawaki
*/

#ifndef MDX_UTILDEBUG_H
#define MDX_UTILDEBUG_H

#include <iostream>

/**
* @brief デバッグ用コンソールに文字を描画する
*/
#if defined(_DEBUG) || defined(DEBUG)
#define OUTPUT_CONSOLE(message){\
	std::cout << message;\
}
#else
#define OUTPUT_CONSOLE(message)
#endif
/**
* @brief デバッグ用コンソールに文字を描画する(改行付き)
*/
#if defined(_DEBUG) || defined(DEBUG)
#define OUTPUT_CONSOLE_LINE(message){\
	std::cout << message << std::endl;\
}
#else
#define OUTPUT_CONSOLE_LINE(message)
#endif


/**
* @brief namespace MDX
*/
namespace MDX{
	/**
	* @brief デバッグ用コンソールウィンドウ
	*/
	class DebugConsoleWindow{
	public:
		/**
		* @brief コンソールウィンドウを開く
		*/
		void Open();
		/**
		* @brief コンソールウィンドウを閉じる
		*/
		void Close();
	private:
		int m_handle = 0; ///< ウィンドウハンドル
	};
}

#endif //! MDX_UTILDEBUG_H