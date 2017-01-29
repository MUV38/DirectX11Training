#include "MDX_Utillity.h"

#include <algorithm>

// テンプレートの実体化
template float MDX::Clamp<float>(float min, float max, float value);

namespace MDX{
	// 拡張子の取得
	std::string GetExtension(const char* filename){
		std::string extension = "";
		
		std::string str = filename;
		unsigned int index = str.find(".");
		if( index != std::string::npos ){
			// "."は除く
			if( index+1 < str.size() ){
				extension = str.substr(index+1);
			}
		}

		return extension;
	}

	// 値をクランプ
	template<class T>
	T Clamp(T min, T max, T value)
	{
		T result = value;
		
		result = std::min<T>(max, value);
		result = std::max<T>(min, value);

		return result;
	}

}
