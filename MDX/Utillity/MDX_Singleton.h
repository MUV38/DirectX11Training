/**
* @file MDX_Singleton.h
* @brief �V���O���g���ɂ�����N���X
* @author Toru Miywaki
*/

#ifndef MDX_SINGLETON_H
#define MDX_SINGLETON_H

/**
* @brief namespace MDX
*/
namespace MDX{
	/**
	* @brief �V���O���g���p���N���X
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