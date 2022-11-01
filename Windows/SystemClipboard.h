#ifndef __SYSTEM_CLIPBOARD_H__
#define __SYSTEM_CLIPBOARD_H__

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
#pragma comment(lib, "User32.lib") // # For System Clipboard Management Functions #
//=====================//

//===== HEADERS ======//
#ifndef __AFX_H__
#include <Windows.h>
#endif // !__AFX_H__
#include <string>
#ifndef _STDINT
#include <stdint.h>
#endif // !_STDINT
//====================//

using lpstr_t = char *;
using lpcstr_t = const char *;
using lpwstr_t = wchar_t *;
using lpcwstr_t = const wchar_t *;

namespace Windows {

	struct SystemClipboard {

		enum class ErrorCode : uint32_t {
			Success,
			InvalidParameter,
			CannotOpenClipboard,
			RequestedClipboardFormatNotAvailable,
			OutOfMemory
		};

		static std::string ErrorMessage(ErrorCode u32ErrorCode) {
			
			switch (u32ErrorCode) {
			case ErrorCode::Success:
				return "The operation completed successfully.";
			case ErrorCode::InvalidParameter:
				return "The parameter is incorrect.";
			case ErrorCode::CannotOpenClipboard:
				return "Cannot open clipboard.";
			case ErrorCode::RequestedClipboardFormatNotAvailable:
				return "Requested clipboard format not available.";
			case ErrorCode::OutOfMemory:
				return "Not enough memory resources are available to complete this operation.";
			}

		}

		/// <summary>This Function Copy String Into Clipboard</summary>
		/// <param name="hWnd">New Clipboard Owner - Must Be Valid Window Handle</param>
		/// <param name="String">String To Be Copied Into Clipboard - Must NOT Be Empty String</param>
		/// <returns>
		/// <para>IF "hWnd" Is NOT Valid Window Handle OR "String" Is Empty String - [InvalidParameter]</para>
		/// <para>IF Cannot Open Clipboard - [CannotOpenClipboard]</para>
		/// <para>IF Not Enough Memory To Copy String Into Clipboard - [OutOfMemory]</para>
		/// <para>IF String Has Been Copied Into Clipboard - [Success]</para>
		/// </returns>
		static ErrorCode _stdcall SetClipboardText(_In_opt_ HWND hWnd, _In_opt_ const std::string &String) {

			if (!IsWindow(hWnd) || String.empty()) {
				return ErrorCode::InvalidParameter;
			}

			if (!OpenClipboard(hWnd)) {
				return ErrorCode::CannotOpenClipboard;
			}

			HGLOBAL hCopyData = GlobalAlloc(GMEM_MOVEABLE | GMEM_ZEROINIT | GMEM_VALID_FLAGS,
				(String.length() + 1) * sizeof(char));

			if (hCopyData == NULL) {
				CloseClipboard();
				return ErrorCode::OutOfMemory;
			}

			#pragma warning(disable:6387)
			lpstr_t lpCopyData = (lpstr_t)(GlobalLock(hCopyData));
			memcpy(lpCopyData, String.c_str(), String.length() * sizeof(char));
			GlobalUnlock(hCopyData);
			#pragma warning(default:6387)

			EmptyClipboard();
			SetClipboardData(CF_TEXT, hCopyData);
			CloseClipboard();

			return ErrorCode::Success;

		}

		/// <summary>This Function Copy String Into Clipboard</summary>
		/// <param name="hWnd">New Clipboard Owner - Must Be Valid Window Handle</param>
		/// <param name="UnicodeString">String To Be Copied Into Clipboard - Must NOT Be Empty String</param>
		/// <returns>
		/// <para>IF "hWnd" Is NOT Valid Window Handle OR "UnicodeString" Is Empty String - [InvalidParameter]</para>
		/// <para>IF Cannot Open Clipboard - [CannotOpenClipboard]</para>
		/// <para>IF Not Enough Memory To Copy String Into Clipboard - [OutOfMemory]</para>
		/// <para>IF String Has Been Copied Into Clipboard - [Success]</para>
		/// </returns>
		static ErrorCode _stdcall SetClipboardText(_In_opt_ HWND hWnd, _In_opt_ const std::wstring &UnicodeString) {

			if (!IsWindow(hWnd) || UnicodeString.empty()) {
				return ErrorCode::InvalidParameter;
			}

			if (!OpenClipboard(hWnd)) {
				return ErrorCode::CannotOpenClipboard;
			}

			HGLOBAL hCopyData = GlobalAlloc(GMEM_MOVEABLE | GMEM_ZEROINIT | GMEM_VALID_FLAGS,
				(UnicodeString.length() + 1) * sizeof(wchar_t));

			if (hCopyData == NULL) {
				CloseClipboard();
				return ErrorCode::OutOfMemory;
			}

			#pragma warning(disable:6387)
			lpwstr_t lpCopyData = (lpwstr_t)(GlobalLock(hCopyData));
			memcpy(lpCopyData, UnicodeString.c_str(), UnicodeString.length() * sizeof(wchar_t));
			GlobalUnlock(hCopyData);
			#pragma warning(default:6387)

			EmptyClipboard();
			SetClipboardData(CF_UNICODETEXT, hCopyData);
			CloseClipboard();

			return ErrorCode::Success;

		}

		/// <summary>This Function Copy Text Data From Clipboard Into String Buffer</summary>
		/// <param name="hWnd">New Clipboard Owner - Must Be Valid Window Handle</param>
		/// <param name="StringBuffer">Where To Copy Text Data From Clipboard</param>
		/// <returns>
		/// <para>IF "hWnd" Is NOT Valid Window Handle - [InvalidParameter]</para>
		/// <para>IF Cannot Open Clipboard - [CannotOpenClipboard]</para>
		/// <para>IF Text Is Not Available In Clipboard - [RequestedFormatNotAvailable]</para>
		/// <para>IF Text Data Has Been Copied From Clipboard Into String Buffer - [Success]</para>
		/// </returns>
		static ErrorCode _stdcall GetClipboardText(_In_opt_ HWND hWnd, _Inout_ std::string &StringBuffer) {

			if (!IsWindow(hWnd)) {
				return ErrorCode::InvalidParameter;
			}

			if (!OpenClipboard(hWnd)) {
				return ErrorCode::CannotOpenClipboard;
			}

			HGLOBAL hClipboardData = GetClipboardData(CF_TEXT);

			if (hClipboardData == NULL) {
				CloseClipboard();
				return ErrorCode::RequestedClipboardFormatNotAvailable;
			}

			#pragma warning(disable:6387)
			lpstr_t lpClipboardData = (lpstr_t)(GlobalLock(hClipboardData));
			StringBuffer += lpClipboardData;
			GlobalUnlock(hClipboardData);
			#pragma warning(default:6387)

			CloseClipboard();

			return ErrorCode::Success;

		}

		/// <summary>This Function Copy Text Data From Clipboard Into String Buffer</summary>
		/// <param name="hWnd">New Clipboard Owner - Must Be Valid Window Handle</param>
		/// <param name="UnicodeStringBuffer">Where To Copy Text Data From Clipboard</param>
		/// <returns>
		/// <para>IF "hWnd" Is NOT Valid Window Handle - [InvalidParameter]</para>
		/// <para>IF Cannot Open Clipboard - [CannotOpenClipboard]</para>
		/// <para>IF Text Is Not Available In Clipboard - [RequestedFormatNotAvailable]</para>
		/// <para>IF Text Data Has Been Copied From Clipboard Into String Buffer - [Success]</para>
		/// </returns>
		static ErrorCode _stdcall GetClipboardText(_In_opt_ HWND hWnd, _Inout_ std::wstring &UnicodeStringBuffer){

			if (!IsWindow(hWnd)) {
				return ErrorCode::InvalidParameter;
			}

			if (!OpenClipboard(hWnd)) {
				return ErrorCode::CannotOpenClipboard;
			}

			HGLOBAL hClipboardData = GetClipboardData(CF_UNICODETEXT);

			if (hClipboardData == NULL) {
				CloseClipboard();
				return ErrorCode::RequestedClipboardFormatNotAvailable;
			}

			#pragma warning(disable:6387)
			lpwstr_t lpClipboardData = (lpwstr_t)(GlobalLock(hClipboardData));
			UnicodeStringBuffer += lpClipboardData;
			GlobalUnlock(hClipboardData);
			#pragma warning(default:6387)

			CloseClipboard();

			return ErrorCode::Success;

		}

	};

}

#endif // !__SYSTEM_CLIPBOARD_H__
