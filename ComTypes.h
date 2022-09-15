#ifndef __COMTYPES_H__
#define __COMTYPES_H__

/*************************************************
*                                                *
* Copyright(c) 2022 Martins Andzans              *
*                                                *
* - In MFC Application                           *
* - - This Header MUST be Included after <afx.h> *
*       or other MFC Headers                     *
*                                                *
*************************************************/

//===== HEADERS ======//
#ifndef __AFX_H__
#include <tchar.h>
#include <Windows.h>
#endif // !__AFX_H__
#include <stdint.h>
//====================//

#ifndef _WIN32
#error "This SDK Works Only on Windows"
#endif // !_WIN32

class Vertex2I {
public:
	
	Vertex2I() noexcept
		: m_x(0), m_y(0)
	{ /*...*/ }

	Vertex2I(int32_t x, int32_t y) noexcept
		: m_x(x), m_y(y)
	{ /*...*/ }



private:

	int32_t m_x;
	int32_t m_y;

};



#endif // !__COMTYPES_H__
