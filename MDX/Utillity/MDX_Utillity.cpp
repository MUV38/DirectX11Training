#include "MDX_Utillity.h"

#include <algorithm>

// �e���v���[�g�̎��̉�
template float MDX::Clamp<float>(float min, float max, float value);

namespace MDX{
	// �g���q�̎擾
	std::string GetExtension(const char* filename){
		std::string extension = "";
		
		std::string str = filename;
		unsigned int index = str.find(".");
		if( index != std::string::npos ){
			// "."�͏���
			if( index+1 < str.size() ){
				extension = str.substr(index+1);
			}
		}

		return extension;
	}

	// �l���N�����v
	template<class T>
	T Clamp(T min, T max, T value)
	{
		T result = value;
		
		result = std::min<T>(max, value);
		result = std::max<T>(min, value);

		return result;
	}

}
