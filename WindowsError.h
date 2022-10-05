#ifndef __WINDOWS_ERROR_H__
#define __WINDOWS_ERROR_H__

/**************************************
*                                     *
* Copyright(c) [2022] Martins Andzans *
* Licensed Under: [MIT License]       *
*                                     *
**************************************/

#ifndef _WIN32
#error "This Header Works Only For Windows Applications"
#endif // !_WIN32

#define WIN32_LEAN_AND_MEAN

//===== HEADERS ======//
#ifndef __AFX_H__
#include <Windows.h>
#endif // !__AFX_H__
#include <regex>
#include <string>
#ifndef _STDINT
#include <stdint.h>
#endif // !_STDINT
//====================//

//----------------------------------------------------------------
// Class "WindowsError"
// - This Class Represents Error Message For <WIN32-API> Error Code
//----------------------------------------------------------------
class WindowsError {
public:

	//----------------------------------------
	// Constructors

	WindowsError(DWORD ErrorCode) {

		m_ErrorCode = ErrorCode;

		// # Pointer To Windows Error Message Buffer #
		LPSTR lpErrorMessage = nullptr;

		//----------------------------------------
		// Get Windows Error Message

		static constexpr DWORD Flags =
			FORMAT_MESSAGE_FROM_SYSTEM |
			FORMAT_MESSAGE_IGNORE_INSERTS |
			FORMAT_MESSAGE_ALLOCATE_BUFFER |
			FORMAT_MESSAGE_MAX_WIDTH_MASK;
		
		#pragma warning(disable:6387)
		DWORD ErrorMessageLength = FormatMessageA(
			Flags, nullptr, m_ErrorCode,
			MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
			(LPSTR)&lpErrorMessage, 0, nullptr);
		#pragma warning(default:6387)
		
		//----------------------------------------

		if (ErrorMessageLength != 0) {
			m_ErrorMessage = std::string(lpErrorMessage, ErrorMessageLength - 1);
			LocalFree(lpErrorMessage);
		} else {
			m_ErrorMessage = "Unknown Error";
		}
		
	}

	//----------------------------------------

	// # Copy Constructor #
	WindowsError(const WindowsError &other) = default;
	// # Copy Assigment Operator #
	WindowsError& operator=(const WindowsError &other) = default;

	//----------------------------------------
	// Getters

	// # Returns Error Code #
	DWORD GetErrorCode(void) const noexcept {
		return m_ErrorCode;
	}

	// # Returns Error Message #
	LPCSTR GetErrorMessage(void) const noexcept {
		return m_ErrorMessage.c_str();
	}

	//----------------------------------------

	// Reserved Words:
	// - <ErrorCode/> - /Keyword/ For Error Code
	// - <ErrorMessage/> - /Keyword/ For Error Message
	// Example:
	// - "Error Code: <ErrorCode/> - <ErrorMessage/>"
	// Returns:
	// - Formated String
	std::string Format(const std::string &FormatString) const {

		std::string Temp = FormatString;

		std::regex Tokens[] = {
			std::regex(R"(<ErrorCode/>)"), // <0>
			std::regex(R"(<ErrorMessage/>)") // <1>
		};

		std::string Replacements[] = {
			std::to_string(m_ErrorCode), // <0>
			std::string(m_ErrorMessage) // <1>
		};

		for (size_t i = 0; i < ARRAYSIZE(Tokens); i++) {
			Temp = std::regex_replace(Temp, Tokens[i], Replacements[i]);
		}

		return Temp;

	}

	// Returns TRUE:
	// - If This Object Error Code Is Equal With Other Object Error Code
	bool Equal(const WindowsError &other) const noexcept {
		return m_ErrorCode == other.m_ErrorCode;
	}

	bool operator==(const WindowsError &other) const noexcept {
		return Equal(other);
	}
	
	bool operator!=(const WindowsError &other) const noexcept {
		return !Equal(other);
	}


	//----------------------------------------
	// Serializing

	// Returns "WindowsError" Content in JSON Format
	// { "ErrorCode": {}, "ErrorMessage": "{}" }
	std::string ToString(void) const {
		return R"({ "ErrorCode": )" + std::to_string(m_ErrorCode)
			+ R"(, "ErrorMessage": ")" + m_ErrorMessage + R"(" })";
	}

	//----------------------------------------

	// # Default Destructor #
	~WindowsError() noexcept = default;

private:

	DWORD m_ErrorCode;
	std::string m_ErrorMessage;

};

#endif // !__WINDOWS_ERROR_H__
