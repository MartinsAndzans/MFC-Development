#ifndef __SYSTEM_CLIPBOARD_H__
#define __SYSTEM_CLIPBOARD_H__

/**************************************
*                                     *
* Copyright(c) [2022] Martins Andzans *
* Licensed Under: [MIT License]       *
*                                     *
**************************************/

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
#ifndef _STDINT
#include <stdint.h>
#endif // !_STDINT
//====================//

struct SystemClipboard {

	/// <summary>
	/// This Function Copy Text To System Clipboard
	/// </summary>
	/// <param name="hWnd">New Clipboard Owner - Must Be Valid Window Handle</param>
	/// <param name="ANSIText">Text To System Clipboard - Must NOT Be Empty String</param>
	/// <returns>
	/// <para>IF "hWnd" Is NOT Valid Window Handle OR "ANSIText" Is Empty String - [ERROR_INVALID_PARAMETER]</para>
	/// <para>IF Clipboard Has Been Opened By Another Process - [ERROR_CLIPBOARD_NOT_OPEN]</para>
	/// <para>IF Not Enough Memory To Copy Text To System Clipboard - [ERROR_OUTOFMEMORY]</para>
	/// <para>IF Text Has Been Copied To Clipboard - [ERROR_SUCCESS]</para>
	/// </returns>
	static uint32_t _stdcall SetClipboardText(_In_opt_ HWND hWnd, _In_opt_ const std::string &ANSIText) {
		
		if (!IsWindow(hWnd) || ANSIText.empty()) {
			return ERROR_INVALID_PARAMETER;
		}

		if (!OpenClipboard(hWnd)) {
   			return ERROR_CLIPBOARD_NOT_OPEN;
		}

		HGLOBAL hCopyData = GlobalAlloc(GMEM_MOVEABLE | GMEM_ZEROINIT | GMEM_VALID_FLAGS,
			(ANSIText.length() + 1) * sizeof(char));

		if (hCopyData == NULL) {
			CloseClipboard();
			return ERROR_OUTOFMEMORY;
		}

		#pragma warning(disable:6387)
		LPSTR lpCopyData = (LPSTR)(GlobalLock(hCopyData));
		memcpy(lpCopyData, ANSIText.c_str(), ANSIText.length() * sizeof(char));
		GlobalUnlock(hCopyData);
		#pragma warning(default:6387)

		EmptyClipboard();
		SetClipboardData(CF_TEXT, hCopyData);
		CloseClipboard();

		return ERROR_SUCCESS;

	}

	/// <summary>
	/// This Function Copy Text To System Clipboard
	/// </summary>
	/// <param name="hWnd">New Clipboard Owner - Must Be Valid Window Handle</param>
	/// <param name="UnicodeText">Text To System Clipboard - Must NOT Be Empty String</param>
	/// <returns>
	/// <para>IF "hWnd" Is NOT Valid Window Handle OR "UnicodeText" Is Empty String - [InvalidParameter]</para>
	/// <para>IF Clipboard Has Been Opened By Another Process - [OpenedByAnotherProcess]</para>
	/// <para>IF Not Enough Memory To Copy Text To System Clipboard - [OutOfMemory]</para>
	/// <para>IF Text Has Been Copied To Clipboard - [Success]</para>
	/// </returns>
	static uint32_t _stdcall SetClipboardText(_In_opt_ HWND hWnd, _In_opt_ const std::wstring &UnicodeText) {

		if (!IsWindow(hWnd) || UnicodeText.empty()) {
			return ERROR_INVALID_PARAMETER;
		}

		if (!OpenClipboard(hWnd)) {
			return ERROR_CLIPBOARD_NOT_OPEN;
		}

		HGLOBAL hCopyData = GlobalAlloc(GMEM_MOVEABLE | GMEM_ZEROINIT | GMEM_VALID_FLAGS,
			(UnicodeText.length() + 1) * sizeof(wchar_t));

		if (hCopyData == NULL) {
			CloseClipboard();
			return ERROR_OUTOFMEMORY;
		}

		#pragma warning(disable:6387)
		LPWSTR lpCopyData = (LPWSTR)(GlobalLock(hCopyData));
		memcpy(lpCopyData, UnicodeText.c_str(), UnicodeText.length() * sizeof(wchar_t));
		GlobalUnlock(hCopyData);
		#pragma warning(default:6387)

		EmptyClipboard();
		SetClipboardData(CF_UNICODETEXT, hCopyData);
		CloseClipboard();

		return ERROR_SUCCESS;

	}

	static uint32_t _stdcall GetClipboardText(_In_opt_ HWND hWnd, _Inout_ std::string &ANSITextBuffer) {

		if (!IsWindow(hWnd)) {
			return ERROR_INVALID_PARAMETER;
		}

		if (!OpenClipboard(hWnd)) {
			return ERROR_CLIPBOARD_NOT_OPEN;
		}

		HGLOBAL hClipboardData = GetClipboardData(CF_TEXT);

		if (hClipboardData != NULL) {

			LPSTR lpClipboardData = (LPSTR)(GlobalLock(hClipboardData));

			if (lpClipboardData != nullptr) {

				ANSITextBuffer += lpClipboardData;
				GlobalUnlock(hClipboardData);

			}

		}

		CloseClipboard();

		return ERROR_SUCCESS;

	}

	static uint32_t _stdcall GetClipboardText(_In_opt_ HWND hWnd, _Inout_ std::wstring &UnicodeTextBuffer) {

		if (!IsWindow(hWnd)) {
			return ERROR_INVALID_PARAMETER;
		}

		if (!OpenClipboard(hWnd)) {
			return ERROR_CLIPBOARD_NOT_OPEN;
		}

		HGLOBAL hClipboardData = GetClipboardData(CF_UNICODETEXT);

		if (hClipboardData != NULL) {

			LPWSTR lpClipboardData = (LPWSTR)(GlobalLock(hClipboardData));

			if (lpClipboardData != nullptr) {

				UnicodeTextBuffer += lpClipboardData;
				GlobalUnlock(hClipboardData);

			}

		}

		CloseClipboard();

		return ERROR_SUCCESS;

	}

};

#endif // !__SYSTEM_CLIPBOARD_H__
