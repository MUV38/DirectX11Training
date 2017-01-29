/**
* @file MDX_Singleton.h
* @brief シングルトンにする基底クラス
* @author Toru Miywaki
*/

#ifndef MDX_SINGLETON_H
#define MDX_SINGLETON_H

/**
* @brief namespace MDX
*/
namespace MDX{
	/**
	* @brief シングルトン用基底クラス
	*/
	template <class T>
	class Singleton{
	public:
		Singleton(){}
		virtual ~Singleton(){}

		static T& GetInstance(){
			static T instance;
			return instance;
		}
	private:
		Singleton(const Singleton& obj) = delete;
		Singleton& operator=(const Singleton& obj) = delete;
	};
}

#endif // !MDX_SINGLETON_H