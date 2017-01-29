/**
* @file MDX_Mode.h
* @brief モード管理をするクラス
* @author Toru Miyawaki
*/

#ifndef MDX_MODE_H
#define MDX_MODE_H

/**
* @brief namespace MDX
*/
namespace MDX{
	/**
	* @brief モードの定義
	*/
#define MODE_DEF(func, name){MODE_##name,}
	enum Mode{
#include "Table/MDX_ModeTable.h"
		MODE_MAX
	};
#undef MODE_DEF
	/**
	* @brief サブモードの定義
	*/
	enum ModeSub{
#define MODE_SUB_DEF(func, name){\
	MODE_SUB_##name,\
}
#include "Table/MDX_ModeSubTable.h"
#undef MODE_SUB_DEF
		MODE_SUB_MAX
	};

}

#endif //! MDX_MODE_H