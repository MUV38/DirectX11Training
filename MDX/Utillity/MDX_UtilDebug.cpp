#include "MDX_UtilDebug.h"

#include <Windows.h>
#include <cstdio>
#include <iostream>
#include <io.h>
#include <Fcntl.h>

namespace MDX{
	void DebugConsoleWindow::Open() {
		//AllocConsole();
		//m_handle = _open_osfhandle( (long)GetStdHandle( STD_OUTPUT_HANDLE ), _O_TEXT );
		//*stdout = *_fdopen( m_handle, "w" );
		//setvbuf( stdout, NULL, _IONBF, 0 );
	}

	void DebugConsoleWindow::Close() {
		//_close(m_handle);
	}
}