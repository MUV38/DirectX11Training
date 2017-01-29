/**
* @file MDX_UTILLITY_H
* @brief �֗��ȃN���X�A�֐��������܂Ƃ߂��t�@�C��
* @author Toru Miyawaki
*/

#ifndef MDX_UTILLITY_H
#define MDX_UTILLITY_H

#include <string>

/**
* @brief delete�}�N��
*/
#define SAFE_DELETE(x){\
	if( x ){\
		delete x;\
		x = nullptr;\
	}\
}
/**
* @brief delete[]�}�N��
*/
#define SAFE_DELETE_ARRAY(x){\
	if( x ){\
		delete [] x;\
		x = nullptr;\
	}\
}
/**
* @brief Release�}�N��
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
	* @brief �g���q�̎擾
	* @param [in] filename �t�@�C����
	* @return �g���q��
	*/
	std::string GetExtension(const char* filename);

	/**
	* @brief �l���N�����v
	* @param [in] min �ŏ��l
	* @param [in] max �ő�l
	* @param [in] value �l
	* @return �N�����v�l
	*/
	template <class T> T Clamp(T min, T max, T value);
}

#endif //! MDX_UTILLITY_H