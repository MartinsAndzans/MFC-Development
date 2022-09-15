#pragma once

/*************************************************
*                                                *
* Copyright(c) 2022 Martins Andzans              *
*                                                *
* - In MFC Application                           *
* - - This Header MUST be Included after <afx.h> *
*       or other MFC Headers                     *
*                                                *
*************************************************/

#ifndef _WIN32
#error "This SDK Works Only on Windows"
#endif // !_WIN32

#define WIN32_LEAN_AND_MEAN

//===== HEADERS ======//
#ifndef __AFX_H__
#include <Windows.h>
#endif // !__AFX_H__
#include <string>
#ifndef _STDINT
#include <stdint.h>
#endif // !__STDINT
//====================//

#ifdef _MSC_VER
#define DEBUGBREAK() __debugbreak()
#else // !_MSC_VER
#define DEBUGBREAK() DebugBreak()
#endif // _MSC_VER

#ifdef _DEBUG || DEBUG
#define dynamic_assert(condition, message) if (!(condition)) { winassert::dynamic_assert_impl(message, __FILE__, __LINE__)); DEBUGBREAK(); }
#elif _NDEBUG || NDEBUG
#define dynamic_assert(condition, message)
#endif // _DEBUG || DEBUG

namespace winassert {
	
	inline void dynamic_assert_impl(const std::string &Message, const std::string &File, int32_t Line) {

		std::string DebugString = "Assertion Failed: " + Message + " - File: " + File + " Line:" + std::to_string(Line);
		OutputDebugStringA(DebugString.c_str());

	}

}
