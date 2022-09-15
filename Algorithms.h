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
#include <memory>
#ifndef __AFX_H__
#include <tchar.h>
#include <Windows.h>
#endif // !__AFX_H__
#include <string>
#include <sstream>
#include <fstream>
#ifndef _STDINT
#include <stdint.h>
#endif // !_STDINT
//====================//

struct Algorithms {

	// # This Function Reverse Number #
	static constexpr uint64_t ReverseNumber(_In_ uint64_t Number) noexcept {

		uint64_t ReversedNumber = 0;
		
		/* Reverse Number */
		while (Number != 0) {
			ReversedNumber *= 10; // # Add zero to "ReversedNumber" #
			ReversedNumber += Number % 10; // # Get last digit from "Number" and add to "ReversedNumber" #
			Number /= 10; // # Remove last digit from "Number" #
		}

		return ReversedNumber;

	}

	// # This Function Sum all "Number" digits #
	static constexpr uint64_t SumDigits(_In_ uint64_t Number) noexcept {

		uint64_t Sum = 0;

		/* Sum Digits */
		while (Number != 0) {
			Sum += Number % 10; // # Get last digit from "Number" and add to "Sum" #
			Number /= 10; // # Remove last digit from "Number" #
		}

		return Sum;

	}

	/// <summary>This Function Find Requested Character Position String</summary>
	/// <param name="lpString">String Where Find Character</param>
	/// <param name="Characater">What Character Find</param>
	/// <param name="StringLength">String Length</param>
	/// <returns>
	/// <para>If Character has been Found Function Returns Character Index</para>
	/// <para>If Character has not been Found Function Returns MAXSIZE_T Value</para>
	/// </returns>
	static size_t FindChar(_In_count_(StringLength) LPCSTR lpString, _In_ char Characater, _In_ size_t StringLength) noexcept {

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
	/// <returns>
	/// <para>If Character has been Found Function Returns Character Index</para>
	/// <para>If Character has not been Found Function Returns MAXSIZE_T Value</para>
	/// </returns>
	static size_t FindChar(_In_count_(StringLength) LPCWSTR lpString, _In_ wchar_t UCharacater, _In_ size_t StringLength) noexcept {

		for (size_t i = 0; i < StringLength; i++) {
			if (lpString[i] == UCharacater) {
				return i;
			}
		}

		return MAXSIZE_T;

	}

	// # This Function Converts all UPPERCASE Letters to lowercase Letters #
	static void ToLower(_Inout_count_(StringLength) LPSTR lpString, _In_ size_t StringLength) noexcept {

		// * Start of Upper Case Letters *
		constexpr int16_t StartUpper = 65;
		// * End of Upper Case Letters *
		constexpr int16_t EndUpper = 98;

		// * Difference Between Lower and Upper Case Letters  *
		constexpr int16_t Difference = 32;

		for (size_t i = 0; i < StringLength; i++) {
			if (lpString[i] >= StartUpper && lpString[i] <= EndUpper) {
				lpString[i] += Difference;
			}
		}

	}

	// # This Function Converts all lowercase Letters to UPPERCASE Letters #
	static void ToUpper(_Inout_count_(StringLength) LPSTR lpString, _In_ size_t StringLength) noexcept {

		// * Start of Lower Case Letters *
		constexpr int16_t StartLower = 97;
		// * End of Lower Case Letters *
		constexpr int16_t EndLower = 122;

		// * Difference Between Lower and Upper Case Letters  *
		constexpr int16_t Difference = 32;

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
			} catch (const std::exception&) {
				return "Encrypted Text Cannot be Decrypted, because is Corrupted!";
			}
			
			DecryptedString += static_cast<char>(ascii_value);
			
		}
		
		return DecryptedString;

	}

	/// <returns><para>If Succeded Returns 0</para><para>If Failed Returns WINAPI Error Code</para></returns>
	static _Success_(return == 0) DWORD WINAPI SetClipboardText(_In_ HWND NewClipboardOwner, _In_count_(TextLength) LPCTSTR Text, _In_ size_t TextLength) noexcept {

		if (!OpenClipboard(NewClipboardOwner)) {
			return GetLastError();
		}

		if (TextLength == 0) {

			constexpr _TCHAR EmptyString[] = _T("<Empty String>");
			
			HGLOBAL hCopyData = GlobalAlloc(GMEM_MOVEABLE | GMEM_ZEROINIT | GMEM_VALID_FLAGS, (ARRAYSIZE(EmptyString) + 1) * sizeof(_TCHAR));
			if (hCopyData == NULL) {
				CloseClipboard();
				return GetLastError();
			}

			LPVOID lpCopyData = nullptr;
			while (lpCopyData == nullptr)
				lpCopyData = GlobalLock(hCopyData);
			memcpy(lpCopyData, EmptyString, ARRAYSIZE(EmptyString) * sizeof(_TCHAR));
			GlobalUnlock(hCopyData);

			EmptyClipboard();
			#if defined(UNICODE) || defined(_UNICODE)
			SetClipboardData(CF_UNICODETEXT, hCopyData);
			#else // !UNICODE || _UNICODE
			SetClipboardData(CF_TEXT, hCopyData);
			#endif // UNICODE || _UNICODE
			CloseClipboard();

			return 0;

		} else {

			HGLOBAL hCopyData = GlobalAlloc(GMEM_MOVEABLE | GMEM_ZEROINIT | GMEM_VALID_FLAGS, (TextLength + 1) * sizeof(_TCHAR));
			if (hCopyData == NULL) {
				CloseClipboard();
				return GetLastError();
			}

			LPVOID lpCopyData = nullptr;
			while (lpCopyData == nullptr)
				lpCopyData = GlobalLock(hCopyData);
			memcpy(lpCopyData, Text, TextLength * sizeof(_TCHAR));
			GlobalUnlock(hCopyData);

			EmptyClipboard();
			#if defined(UNICODE) || defined(_UNICODE)
			SetClipboardData(CF_UNICODETEXT, hCopyData);
			#else // !UNICODE || _UNICODE
			SetClipboardData(CF_TEXT, hCopyData);
			#endif // UNICODE || _UNICODE
			CloseClipboard();

			return 0;

		}

	}

	#if defined(UNICODE) || defined(_UNICODE)
	/// <returns><para>If Succeded Returns 0</para><para>If Failed Returns WINAPI Error Code</para></returns>
	static _Success_(return == 0) DWORD WINAPI GetClipboardText(_In_ HWND NewClipboardOwner, _Inout_ std::wstring &StringBuffer) noexcept {
	#else // !UNICODE || _UNICODE
	/// <returns><para>If Succeded Returns 0</para><para>If Failed Returns WINAPI Error Code</para></returns>
	static _Success_(return == 0) DWORD WINAPI GetClipboardText(_In_ HWND NewClipboardOwner, _Inout_ std::string &StringBuffer) noexcept {
	#endif // UNICODE || _UNICODE

		if (!OpenClipboard(NewClipboardOwner)) {
			return GetLastError();
		}

		#if defined(UNICODE) || defined(_UNICODE)
		HGLOBAL hClipboardData = GetClipboardData(CF_UNICODETEXT);
		#else // !UNICODE || _UNICODE
		HGLOBAL hClipboardData = GetClipboardData(CF_TEXT);
		#endif // UNICODE || _UNICODE
		if (hClipboardData == NULL) {
			CloseClipboard();
			return GetLastError();
		}

		LPVOID lpClipboardData = nullptr;
		while (lpClipboardData == nullptr)
			lpClipboardData = GlobalLock(hClipboardData);
		StringBuffer += reinterpret_cast<LPCTSTR>(lpClipboardData);
		GlobalUnlock(hClipboardData);

		CloseClipboard();

		return 0;

	}

	// # This Function Get WINAPI Error Message From System Resources #
	static std::string GetWINAPIErrorMessage(_In_ DWORD dwLastError) noexcept {

		//========== ERROR MESSAGE BUFFER ==========//
		constexpr size_t MAX_CHAR_STRING = 256U;
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

	/// <summary><para>This Function Throws "bad_alloc" and "runtime_error" Exceptions</para></summary>
	/// <param name="Bitmap">Handle To Bitmap</param>
	/// <param name="FilePath">File path with ".bmp" extension</param>
	/// <param name="BitmapSize">Bitmap size in pixels</param>
	/// <returns><para>If Succeded Returns TRUE</para><para>If Failed Returns FALSE</para></returns>
	static void SaveBitmapToFile(_In_ HBITMAP Bitmap, _In_z_ LPCTSTR FilePath, _In_ SIZE BitmapSize)
		#ifdef _MSVC_LANG
			#if _MSVC_LANG <= 201402L
				throw(std::bad_alloc, std::runtime_error)
			#endif
		#else
			#if __cplusplus <= 201402L
				throw(std::bad_alloc, std::runtime_error)
			#endif
		#endif
		{
		
		std::ofstream image;

		constexpr WORD BM = 0x4D42; // * ASCII 'B' = 0x42 / 'M' = 0x4D *
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
