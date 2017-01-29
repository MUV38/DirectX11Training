/**
* @file MDX_UtilDebug.h
* @brief �f�o�b�O�֘A�Ɏg������̂��܂Ƃ߂��t�@�C��
* @author Toru Miyawaki
*/

#ifndef MDX_UTILDEBUG_H
#define MDX_UTILDEBUG_H

#include <iostream>

/**
* @brief �f�o�b�O�p�R���\�[���ɕ�����`�悷��
*/
#if defined(_DEBUG) || defined(DEBUG)
#define OUTPUT_CONSOLE(message){\
	std::cout << message;\
}
#else
#define OUTPUT_CONSOLE(message)
#endif
/**
* @brief �f�o�b�O�p�R���\�[���ɕ�����`�悷��(���s�t��)
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
	* @brief �f�o�b�O�p�R���\�[���E�B���h�E
	*/
	class DebugConsoleWindow{
	public:
		/**
		* @brief �R���\�[���E�B���h�E���J��
		*/
		void Open();
		/**
		* @brief �R���\�[���E�B���h�E�����
		*/
		void Close();
	private:
		int m_handle = 0; ///< �E�B���h�E�n���h��
	};
}

#endif //! MDX_UTILDEBUG_H