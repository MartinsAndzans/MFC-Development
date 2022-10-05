#ifndef __ALGORITHMS__
#define __ALGORITHMS__

/*************************************************
*                                                *
*         Copyright(c) Martins Andzans        '  *
*                                                *
* - In MFC Application                           *
* - - This Header MUST be Included after <afx.h> *
*       or other MFC Headers                     *
*                                                *
*************************************************/

#define WIN32_LEAN_AND_MEAN

//===== HEADERS ======//
#ifndef __AFX_H__
#include <tchar.h>
#include <Windows.h>
#endif // !__AFX_H__
#include <memory>
#include <fstream>
#include <string>
#include <sstream>
#ifndef _STDINT
#include <stdint.h>
#endif // !_STDINT
//====================//

struct Algorithms {

	// # This Function Reverse Number #
	static constexpr uint32_t ReverseNumber(_In_ uint32_t Number) noexcept {

		uint32_t ReversedNumber = 0;
		
		/* Reverse Number */
		while (Number != 0) {

			// # Add zero to "ReversedNumber" #
			ReversedNumber *= 10;
			// # Get last digit from "Number" and add to "ReversedNumber" #
			ReversedNumber += Number % 10;
			// # Remove last digit from "Number" #
			Number /= 10;

		}

		return ReversedNumber;

	}

	// # This Function Sum all "Number" digits #
	static constexpr uint32_t SumDigits(_In_ uint32_t Number) noexcept {

		uint32_t Sum = 0;

		/* Sum Digits */
		while (Number != 0) {

			// # Get last digit from "Number" and add to "Sum" #
			Sum += Number % 10;
			// # Remove last digit from "Number" #
			Number /= 10;

		}

		return Sum;

	}

	/// <summary>This Function Find Requested Character Position String</summary>
	/// <param name="lpString">String Where Find Character</param>
	/// <param name="Characater">What Character Find</param>
	/// <param name="StringLength">String Length</param>
	/// <returns><para>If Character has been Found Function Returns Character Index</para>
	/// <para>If Character has not been Found Function Returns MAXSIZE_T Value</para></returns>
	static size_t FindChar(_In_count_(StringLength) const char *lpString, _In_ char Characater, _In_ size_t StringLength) noexcept {

		for (size_t i = 0; i < StringLength; i++) {
			if (lpString[i] == Characater) {
				return i;
			}
		}

		return MAXSIZE_T;

	}

	/// <summary>This Function Find Requested Character Position String</summary>
	/// <param name="lpString">String Where Find Character</param>
	/// <param name="UCharacater">What Unicode Character Find</param>
	/// <param name="StringLength">String Length</param>
	/// <returns><para>If Character has been Found Function Returns Character Index</para>
	/// <para>If Character has not been Found Function Returns MAXSIZE_T Value</para></returns>
	static size_t FindChar(_In_count_(StringLength) const wchar_t *lpString, _In_ wchar_t UCharacater, _In_ size_t StringLength) noexcept {

		for (size_t i = 0; i < StringLength; i++) {
			if (lpString[i] == UCharacater) {
				return i;
			}
		}

		return MAXSIZE_T;

	}

	// # This Function Converts all UPPERCASE Letters to lowercase Letters #
	static void ToLower(_Inout_count_(StringLength) char *lpString, _In_ size_t StringLength) noexcept {

		// * Start of Upper Case Letters *
		static constexpr int16_t StartUpper = 65;
		// * End of Upper Case Letters *
		static constexpr int16_t EndUpper = 98;

		// * Difference Between Lower and Upper Case Letters *
		static constexpr int16_t Difference = 32;

		for (size_t i = 0; i < StringLength; i++) {
			if (lpString[i] >= StartUpper && lpString[i] <= EndUpper) {
				lpString[i] += Difference;
			}
		}

	}

	// # This Function Converts all lowercase Letters to UPPERCASE Letters #
	static void ToUpper(_Inout_count_(StringLength) char *lpString, _In_ size_t StringLength) noexcept {

		// * Start of Lower Case Letters *
		static constexpr int16_t StartLower = 97;
		// * End of Lower Case Letters *
		static constexpr int16_t EndLower = 122;

		// * Difference Between Lower and Upper Case Letters *
		static constexpr int16_t Difference = 32;

		for (size_t i = 0; i < StringLength; i++) {
			if (lpString[i] >= StartLower && lpString[i] <= EndLower) {
				lpString[i] -= Difference;
			}
		}

	}

	// # This Function Encrypt Text To ASCII Value Code #
	static std::string EncryptText(_In_ std::string_view String) {

		std::string EncryptedString;

		for (const char &Character : String) {
			int16_t ascii_value = static_cast<int16_t>(Character);
			EncryptedString += std::to_string(ascii_value) + ':';
		}

		EncryptedString.pop_back();

		return EncryptedString;

	}

	// # This Function Decrypt Text From ASCII Value Code #
	static std::string DecryptText(_In_ std::string_view EncryptedString) {

		std::string DecryptedString;
		std::istringstream sstream(EncryptedString.data());

		while (!sstream.eof()) {

			std::string temp;
			std::getline(sstream, temp, ':');

			int16_t ascii_value;

			try {
				ascii_value = std::stoi(temp);
			} catch (const std::exception &e) {
				UNREFERENCED_PARAMETER(e);
				return "";
			}
			
			DecryptedString += static_cast<char>(ascii_value);
			
		}
		
		return DecryptedString;

	}

	static std::wstring StringToWideString(const std::string &String) {

		std::wstring WString;
		WString.reserve(String.length() + 1);

		for (const char &Character : String) {
			WString.push_back(static_cast<wchar_t>(Character));
		}

		return WString;

	}

	/// <param name="NewClipboardOwner">New Clipboard Owner Window Handle - Must Not Be NULL</param>
	/// <param name="String">What Text Save To Clipboard</param>
	/// <returns>
	///	<para>If Succeded Returns 0</para>
	///	<para>If Failed Returns WINAPI Error Code</para>
	/// </returns>
	static _Success_(return == 0) DWORD WINAPI SetClipboardText(_In_opt_ HWND NewClipboardOwner, _In_ const std::string &String) noexcept {

		if (!OpenClipboard(NewClipboardOwner)) {
			return GetLastError();
		}

		HGLOBAL hCopyData = NULL;

		if (String.empty()) {

			static constexpr char EmptyString[] = "<Empty String>";
			
			hCopyData = GlobalAlloc(GMEM_MOVEABLE | GMEM_ZEROINIT | GMEM_VALID_FLAGS,
				(ARRAYSIZE(EmptyString) + 1) * sizeof(char));
			
			if (hCopyData == NULL) {
				DWORD ErrorCode = GetLastError();
				CloseClipboard();
				return ErrorCode;
			}

			#pragma warning(disable:6387)
			LPVOID lpCopyData = GlobalLock(hCopyData);
			memcpy(lpCopyData, EmptyString, ARRAYSIZE(EmptyString) * sizeof(char));
			GlobalUnlock(hCopyData);
			#pragma warning(default:6387)

		} else {

			hCopyData = GlobalAlloc(GMEM_MOVEABLE | GMEM_ZEROINIT | GMEM_VALID_FLAGS,
				(String.length() + 1) * sizeof(char));

			if (hCopyData == NULL) {
				DWORD ErrorCode = GetLastError();
				CloseClipboard();
				return ErrorCode;
			}

			#pragma warning(disable:6387)
			LPVOID lpCopyData = GlobalLock(hCopyData);
			memcpy(lpCopyData, String.c_str(), String.length() * sizeof(char));
			GlobalUnlock(hCopyData);
			#pragma warning(default:6387)

		}

		EmptyClipboard();
		SetClipboardData(CF_TEXT, hCopyData);
		CloseClipboard();

		return 0;

	}

	/// <param name="NewClipboardOwner">New Clipboard Owner Window Handle - Must Not Be NULL</param>
	/// <param name="UString">What UText Save To Clipboard</param>
	/// <returns>
	///	<para>If Succeded Returns 0</para>
	///	<para>If Failed Returns WINAPI Error Code</para>
	/// </returns>
	static _Success_(return == 0) DWORD WINAPI SetClipboardText(_In_opt_ HWND NewClipboardOwner, _In_ const std::wstring &UString) noexcept {

		if (!OpenClipboard(NewClipboardOwner)) {
			return GetLastError();
		}

		HGLOBAL hCopyData = NULL;

		if (UString.empty()) {

			static constexpr wchar_t EmptyString[] = L"<Empty Unicode String>";

			hCopyData = GlobalAlloc(GMEM_MOVEABLE | GMEM_ZEROINIT | GMEM_VALID_FLAGS,
				(ARRAYSIZE(EmptyString) + 1) * sizeof(wchar_t));


			if (hCopyData == NULL) {
				DWORD ErrorCode = GetLastError();
				CloseClipboard();
				return ErrorCode;
			}

			#pragma warning(disable:6387)
			LPVOID lpCopyData = GlobalLock(hCopyData);
			memcpy(lpCopyData, EmptyString, ARRAYSIZE(EmptyString) * sizeof(wchar_t));
			GlobalUnlock(hCopyData);
			#pragma warning(default:6387)

		} else {

			hCopyData = GlobalAlloc(GMEM_MOVEABLE | GMEM_ZEROINIT | GMEM_VALID_FLAGS,
				(UString.length() + 1) * sizeof(wchar_t));

			if (hCopyData == NULL) {
				DWORD ErrorCode = GetLastError();
				CloseClipboard();
				return ErrorCode;
			}

			#pragma warning(disable:6387)
			LPVOID lpCopyData = GlobalLock(hCopyData);
			memcpy(lpCopyData, UString.c_str(), UString.length() * sizeof(wchar_t));
			GlobalUnlock(hCopyData);
			#pragma warning(default:6387)

		}

		EmptyClipboard();
		SetClipboardData(CF_UNICODETEXT, hCopyData);
		CloseClipboard();

		return 0;

	}

	/// <param name="NewClipboardOwner">New Clipboard Owner Window Handle - Must Not Be NULL</param>
	/// <param name="StringBuffer">Where To Save Text From Clipboard</param>
	/// <returns>
	///	<para>If Succeded Returns 0</para>
	///	<para>If Failed Returns WINAPI Error Code</para>
	/// </returns>
	static _Success_(return == 0) DWORD WINAPI GetClipboardText(_In_opt_ HWND NewClipboardOwner, _Out_ std::string &StringBuffer) {

		// Open Clipboard For Reading
		if (!OpenClipboard(NewClipboardOwner)) {
			return GetLastError();
		}

		DWORD ErrorCode = 0;

		if (!IsClipboardFormatAvailable(CF_TEXT)) {
			ErrorCode = GetLastError();
			StringBuffer = "";
			CloseClipboard();
			return ErrorCode;
		}

		// Try Get Clipboard Text
		HGLOBAL hClipboardData = GetClipboardData(CF_TEXT);

		// Clipboard Doesn't Contain Text
		if (hClipboardData == NULL) {
			ErrorCode = GetLastError();
			StringBuffer = "";
			CloseClipboard();
			return ErrorCode;
		}

		LPCVOID lpClipboardData = GlobalLock(hClipboardData);

		if (lpClipboardData == nullptr) {
			ErrorCode = GetLastError();
			StringBuffer = "";
			CloseClipboard();
			return ErrorCode;
		}

		StringBuffer = static_cast<const char*>(lpClipboardData);
		GlobalUnlock(hClipboardData);

		CloseClipboard();

		return 0;

	}

	/// <param name="NewClipboardOwner">New Clipboard Owner Window Handle - Must Not Be NULL</param>
	/// <param name="UStringBuffer">Where To Save UText From Clipboard</param>
	/// <returns>
	///	<para>If Succeded Returns 0</para>
	///	<para>If Failed Returns WINAPI Error Code</para>
	/// </returns>
	static _Success_(return == 0) DWORD WINAPI GetClipboardText(_In_opt_ HWND NewClipboardOwner, _Out_ std::wstring &UStringBuffer) {

		// Open Clipboard For Reading
		if (!OpenClipboard(NewClipboardOwner)) {
			return GetLastError();
		}
		
		DWORD ErrorCode = 0;

		if (!IsClipboardFormatAvailable(CF_UNICODETEXT)) {
			ErrorCode = GetLastError();
			UStringBuffer = L"";
			CloseClipboard();
			return ErrorCode;
		}

		// Try Get Clipboard UText
		HGLOBAL hClipboardData = GetClipboardData(CF_UNICODETEXT);

		// Clipboard Doesn't Contain UText
		if (hClipboardData == NULL) {
			ErrorCode = GetLastError();
			UStringBuffer = L"";
			CloseClipboard();
			return ErrorCode;
		}

		LPCVOID lpClipboardData = GlobalLock(hClipboardData);

		if (lpClipboardData == nullptr) {
			ErrorCode = GetLastError();
			UStringBuffer = L"";
			CloseClipboard();
			return ErrorCode;
		}

		UStringBuffer = static_cast<const wchar_t*>(lpClipboardData);
		GlobalUnlock(hClipboardData);
		
		CloseClipboard();

		return 0;

	}


	// # This Function Get WINAPI Error Message From System Resources #
	static std::string GetWINAPIErrorMessage(_In_ DWORD dwLastError) {

		//========== ERROR MESSAGE BUFFER ==========//
		static constexpr size_t MAX_CHAR_STRING = 256U;
		CHAR LastErrorMessage[MAX_CHAR_STRING] = { ' ', '-', ' ' };
		//==========================================//

		//========== GET LAST ERROR MESSAGE ==========//
		DWORD Length = FormatMessageA(FORMAT_MESSAGE_FROM_SYSTEM,
			nullptr, dwLastError, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
			LastErrorMessage + 3, ARRAYSIZE(LastErrorMessage) - 4, nullptr);
		//============================================//
		
		if (Length == 0) {
			return "Error Code: " + std::to_string(dwLastError) + " - Unknown Error";
		} else {
			return "Error Code: " + std::to_string(dwLastError) + LastErrorMessage;
		}

	}

	/// <summary>This Function Throws "bad_alloc" and "runtime_error" Exceptions</summary>
	/// <param name="Bitmap">Handle To Bitmap</param>
	/// <param name="FilePath">File Path With ".bmp" Extension</param>
	/// <param name="BitmapSize">Bitmap Size in Pixels</param>
	static void SaveBitmapToFile(_In_ HBITMAP Bitmap, _In_z_ LPCTSTR FilePath, _In_ SIZE BitmapSize) {
		
		std::ofstream image;

		static constexpr WORD BM = 0x4D42; // * ASCII 'B' = 0x42 / 'M' = 0x4D *
		const DWORD BitmapSizeCXxCY = BitmapSize.cx * BitmapSize.cy; // * Bitmap Size [cx x cy] *

		BITMAPFILEHEADER bmfheader = { 0 };
		bmfheader.bfType = BM; // # Bitmap Signature #
		bmfheader.bfSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + sizeof(COLORREF) * BitmapSizeCXxCY; // # File Size #
		bmfheader.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER); // # Offset To Color Bits #
		bmfheader.bfReserved1 = 0x0000;
		bmfheader.bfReserved2 = 0x0000;

		BITMAPINFOHEADER bmiheader = { 0 };
		bmiheader.biSize = sizeof(BITMAPINFOHEADER);
		bmiheader.biWidth = BitmapSize.cx; // # Bitmap width in pixels #
		bmiheader.biHeight = BitmapSize.cy; // # Bitmap height in pixels #
		bmiheader.biPlanes = 0x01; // # Bitmap has a one plane #
		bmiheader.biBitCount = 0x20; // # Bitmap has 4 Bytes Per Pixel #
		bmiheader.biCompression = BI_RGB;
		bmiheader.biSizeImage = BitmapSizeCXxCY;
		bmiheader.biXPelsPerMeter = BitmapSize.cx;
		bmiheader.biYPelsPerMeter = BitmapSize.cy;
		bmiheader.biClrUsed = 0x00000000;
		bmiheader.biClrImportant = 0x00000000;

		std::unique_ptr<COLORREF[]> lpBitmapBytes = std::make_unique<COLORREF[]>(BitmapSizeCXxCY);

		BITMAPINFO bminfo = { 0 };
		bminfo.bmiHeader = bmiheader;
		
		HDC ScreenDC = GetDC(HWND_DESKTOP);
		GetDIBits(ScreenDC, Bitmap, 0, BitmapSize.cy, lpBitmapBytes.get(), &bminfo, DIB_RGB_COLORS);
		ReleaseDC(HWND_DESKTOP, ScreenDC);

		image.open(FilePath, std::ios::binary);
		if (!image.is_open()) {
			throw(std::runtime_error("Cannot Open/Create File"));
		}

		image.write(reinterpret_cast<char*>(&bmfheader), sizeof(BITMAPFILEHEADER)); // # BITMAP FILE HEADER #
		image.write(reinterpret_cast<char*>(&bmiheader), sizeof(BITMAPINFOHEADER)); // # BITMAP INFO HEADER #
		image.write(reinterpret_cast<char*>(lpBitmapBytes.get()), sizeof(COLORREF) * BitmapSizeCXxCY); // # BITMAP BYTE ARRAY #
		
		image.close();

	}

};

#endif // !__ALGORITHMS__
