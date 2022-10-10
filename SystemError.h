#ifndef __SYSTEM_ERROR_H__
#define __SYSTEM_ERROR_H__

/*****************************************
*                                        *
* Copyright(c) [2022] Martins Andzans    *
* Licensed Under: [MIT License]          *
*                                        *
* - In MFC Application This Header       *
* - - Must Be Included After MFC Headers *
*                                        *
*****************************************/

#ifndef _WIN32
#error "This Header Works Only For Windows Applications"
#endif // !_WIN32

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif // !WIN32_LEAN_AND_MEAN

//===== HEADERS ======//
#ifndef __AFX_H__
#include <Windows.h>
#endif // !__AFX_H__
#include <string>
#include <utility>
#include <regex>
#ifndef _STDINT
#include <stdint.h>
#endif // !_STDINT
//====================//

//----------------------------------------------------------------
// Class "SystemError"
// - This Class Represents Error Message For <WIN32-API> Error Code
//----------------------------------------------------------------
class SystemError {
public:

	//----------------------------------------
	// Constructors

	explicit SystemError(uint64_t ErrorCode) : m_ErrorCode(ErrorCode) {

		switch (ErrorCode) {
		case ERROR_SUCCESS:

			m_ErrorMessage = "The operation completed successfully.";
			break;
			
		default:

			LPSTR lpErrorMessage = nullptr;

			//----------------------------------------
			// Get System Error Message

			static constexpr DWORD Flags =
				FORMAT_MESSAGE_FROM_SYSTEM |
				FORMAT_MESSAGE_ALLOCATE_BUFFER |
				FORMAT_MESSAGE_IGNORE_INSERTS |
				FORMAT_MESSAGE_MAX_WIDTH_MASK;

			#pragma warning(disable:6387)
			DWORD Length = FormatMessageA(
				Flags, nullptr, (DWORD)(ErrorCode),
				MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
				(LPSTR)&lpErrorMessage, 0, nullptr);
			#pragma warning(default:6387)

			//----------------------------------------

			if (Length != 0) {

				try {
					m_ErrorMessage = std::string(lpErrorMessage, Length - 1);
				} catch (const std::exception &e) {
					LocalFree(lpErrorMessage);
					throw;
				}

			} else {

				m_ErrorMessage = "Unknown Error.";

			}

			break;

		}
		
	}

	//----------------------------------------

	// # Copy Constructor # 
	SystemError(const SystemError &other) = default;
	// # Copy Assigment Operator #
	SystemError& operator=(const SystemError &other) = default;

	//----------------------------------------
	// Getters

	// # Returns Error Code #
	uint64_t GetErrorCode(void) const {
		return m_ErrorCode;
	}

	// # Returns Error Message #
	std::string GetErrorMessage(void) const {
		return m_ErrorMessage;
	}

	//----------------------------------------

	// Reserved Words:
	// - $(ErrorCode) - Macro For Error Code
	// - $(ErrorMessage) - Macro For Error Message
	// Example:
	// - "Error Code: $(ErrorCode) - $(ErrorMessage)"
	// Returns:
	// - Formated String
	std::string Format(const std::string &Template) const {

		std::string Temp = Template;

		std::pair<std::regex, std::string> Tokens[] = {
			{ std::regex(R"(\$\(ErrorCode\))"), std::to_string(m_ErrorCode) },
			{ std::regex(R"(\$\(ErrorMessage\))"), m_ErrorMessage }
		};

		for (size_t i = 0; i < _countof(Tokens); i++) {
			Temp = std::regex_replace(Temp, Tokens[i].first, Tokens[i].second);
		}

		return Temp;

	}

	// Returns TRUE:
	// - If This Object Error Code Is Equal With Other Object Error Code
	bool Equal(const SystemError &other) const noexcept {
		return m_ErrorCode == other.m_ErrorCode;
	}

	bool operator==(const SystemError &other) const noexcept {
		return Equal(other);
	}
	
	bool operator!=(const SystemError &other) const noexcept {
		return !Equal(other);
	}


	//----------------------------------------
	// Serializing

	// Returns "SystemError" Content in JSON Format
	// { "ErrorCode": {}, "ErrorMessage": "{}" }
	std::string ToString(void) const {
		return R"({ "ErrorCode": )" + std::to_string(m_ErrorCode)
			+ R"(, "ErrorMessage": ")" + m_ErrorMessage + R"(" })";
	}

	//----------------------------------------

	// # Default Destructor #
	~SystemError() noexcept = default;

private:

	uint64_t m_ErrorCode;
	std::string m_ErrorMessage;

};

#endif // !__SYSTEM_ERROR_H__
