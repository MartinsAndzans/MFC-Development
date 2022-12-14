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

//===== LIBRARYS ======//
#pragma comment(lib, "Kernel32.lib") // # For "FormatMessageA" Function #
//=====================//

//===== HEADERS ======//
#ifndef __AFX_H__
#include <Windows.h>
#endif // !__AFX_H__
#include <string>
#include <regex>
#ifndef _STDINT
#include <stdint.h>
#endif // !_STDINT
//====================//

namespace Windows {

	//----------------------------------------------------------------
	// Class "SystemError"
	// - This Class Represents Error Message For System Error Code
	//----------------------------------------------------------------
	class SystemError {
	private:
		
		using lpstr_t = char *;

		static void GetWINAPIErrroMessage(uint32_t ErrorCode, std::string &ErrorMessage) {
			
			lpstr_t lpErrorMessage = nullptr;

			static constexpr uint32_t Flags =
				FORMAT_MESSAGE_FROM_SYSTEM |
				FORMAT_MESSAGE_ALLOCATE_BUFFER |
				FORMAT_MESSAGE_IGNORE_INSERTS |
				FORMAT_MESSAGE_MAX_WIDTH_MASK;

			#pragma warning(disable:6387)
			uint32_t ErrorMessageLength = FormatMessageA(
				Flags, nullptr, ErrorCode, LANG_USER_DEFAULT,
				(lpstr_t)(&lpErrorMessage), 0, nullptr);
			#pragma warning(default:6387)

			if (ErrorMessageLength != 0) {

				try {
					ErrorMessage = std::string(lpErrorMessage, ErrorMessageLength - 1);
					LocalFree(lpErrorMessage);
				} catch (...) {
					LocalFree(lpErrorMessage);
					throw;
				}

			} else {

				ErrorMessage = "Unknown Error.";

			}

		}

	public:

		//----------------------------------------
		// Constructors

		explicit SystemError(uint32_t ErrorCode = ERROR_SUCCESS) : m_ErrorCode(ErrorCode) {

			switch (ErrorCode) {
			case ERROR_SUCCESS:
				m_ErrorMessage = "The operation completed successfully.";
				break;
			default:
				GetWINAPIErrroMessage(ErrorCode, m_ErrorMessage);
				break;
			}

		}

		//----------------------------------------

		// # Copy Constructor # 
		SystemError(const SystemError &other) = default;
		// # Copy Assigment Operator #
		SystemError& operator=(const SystemError &other) = default;
		
		// TODO: Move Semantics
		
		//----------------------------------------
		// Getters

		// # Returns Error Code #
		const uint32_t& GetErrorCode(void) const noexcept {
			return m_ErrorCode;
		}

		// # Returns Error Message #
		const std::string& GetErrorMessage(void) const noexcept {
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

			std::string Formated = Template;

			Formated = std::regex_replace(Formated,
				std::regex(R"(\$\(ErrorCode\))"), std::to_string(m_ErrorCode));
			Formated = std::regex_replace(Formated,
				std::regex(R"(\$\(ErrorMessage\))"), m_ErrorMessage);

			return Formated;

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
			return R"({ "ErrorCode": )" + std::to_string(m_ErrorCode) +
				R"(, "ErrorMessage": ")" + m_ErrorMessage + R"(" })";
		}

		//----------------------------------------

		// # Default Destructor #
		~SystemError() noexcept = default;

	private:

		uint32_t m_ErrorCode;
		std::string m_ErrorMessage;

	};

}

#endif // !__SYSTEM_ERROR_H__
