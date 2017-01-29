/**
* @file MDX_UTILLITY_H
* @brief 便利なクラス、関数たちをまとめたファイル
* @author Toru Miyawaki
*/

#ifndef MDX_UTILLITY_H
#define MDX_UTILLITY_H

#include <string>

/**
* @brief deleteマクロ
*/
#define SAFE_DELETE(x){\
	if( x ){\
		delete x;\
		x = nullptr;\
	}\
}
/**
* @brief delete[]マクロ
*/
#define SAFE_DELETE_ARRAY(x){\
	if( x ){\
		delete [] x;\
		x = nullptr;\
	}\
}
/**
* @brief Releaseマクロ
*/
#define SAFE_RELEASE(x){\
	if( x ){\
		x->Release();\
		x = nullptr;\
	}\
}

/**
* @brief namespace MDX
*/
namespace MDX{
	/**
	* @brief 拡張子の取得
	* @param [in] filename ファイル名
	* @return 拡張子名
	*/
	std::string GetExtension(const char* filename);

	/**
	* @brief 値をクランプ
	* @param [in] min 最小値
	* @param [in] max 最大値
	* @param [in] value 値
	* @return クランプ値
	*/
	template <class T> T Clamp(T min, T max, T value);
}

#endif //! MDX_UTILLITY_H