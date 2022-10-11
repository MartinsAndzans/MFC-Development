#ifndef __ALGORITHMS__
#define __ALGORITHMS__

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
#include <sstream>
#include <memory>
#include <fstream>
#ifndef _STDINT
#include <stdint.h>
#endif // !_STDINT
//====================//

#ifndef UNREFERENCED_PARAMETER
#define UNREFERENCED_PARAMETER(P) (P)
#endif // !UNREFERENCED_PARAMETER

struct Algorithms {

	// # This Function Reverse Number #
	template<typename Type>
	static constexpr Type ReverseNumber(_In_ const Type &Number) noexcept {

		bool Negative = (Number < 0) ? true : false;

		Type OriginalNumber = (Negative) ? -Number : Number;
		Type ReversedNumber = 0;

		/* Reverse Number */
		while (OriginalNumber != 0) {

			// # Add zero to "ReversedNumber" #
			ReversedNumber *= 10;
			// # Get last digit from "OriginalNumber" and add to "ReversedNumber" #
			ReversedNumber += OriginalNumber % 10;
			// # Remove last digit from "OriginalNumber" #
			OriginalNumber /= 10;

		}

		return (Negative) ? -ReversedNumber : ReversedNumber;

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
	static std::string EncryptText(_In_ const std::string &String) {

		std::string EncryptedString;

		for (const char &Character : String) {
			int16_t ascii_value = static_cast<int16_t>(Character);
			EncryptedString += std::to_string(ascii_value) + ':';
		}

		EncryptedString.pop_back();

		return EncryptedString;

	}

	// # This Function Decrypt Text From ASCII Value Code #
	static std::string DecryptText(_In_ const std::string &EncryptedString) {

		std::string DecryptedString;
		std::istringstream sstream(EncryptedString);

		while (!sstream.eof()) {

			std::string temp;
			std::getline(sstream, temp, ':');

			int16_t ascii_value;

			try {
				ascii_value = std::stoi(temp);
			} catch (...) {
				return "";
			}
			
			DecryptedString += static_cast<char>(ascii_value);
			
		}
		
		return DecryptedString;

	}

	static std::wstring StringToWideString(const std::string &String) {

		std::wstring WideString;
		WideString.reserve(String.length() + 1);

		for (const char &Character : String) {
			WideString.push_back(static_cast<wchar_t>(Character));
		}

		return WideString;

	}


	// # This Function Get WINAPI Error Message From System Resources #
	static std::string GetWINAPIErrorMessage(_In_ uint32_t ErrorCode) {

		//----------------------------------------
		// Error Message Bufffer

		static constexpr size_t MAX_CHAR_STRING = 410U;
		CHAR ErrorMessageBuffer[MAX_CHAR_STRING] = { 0 };
		
		//----------------------------------------

		//----------------------------------------
		// Get System Error Message

		static constexpr uint32_t Flags =
			FORMAT_MESSAGE_FROM_SYSTEM |
			FORMAT_MESSAGE_IGNORE_INSERTS |
			FORMAT_MESSAGE_MAX_WIDTH_MASK;

		DWORD ErrorMessageLength = FormatMessageA(
			Flags, nullptr, ErrorCode, LANG_USER_DEFAULT,
			ErrorMessageBuffer, ARRAYSIZE(ErrorMessageBuffer) - 1, nullptr);
		
		//----------------------------------------
		
		if (ErrorMessageLength == 0) {
			return "Error Code: " + std::to_string(ErrorCode) + " - Unknown Error";
		} else {
			return "Error Code: " + std::to_string(ErrorCode) + " - " + std::string(ErrorMessageBuffer, ErrorMessageLength - 1);
		}

	}

	/// <summary>This Function Throws "bad_alloc" and "runtime_error" Exceptions</summary>
	/// <param name="Bitmap">Handle To Bitmap</param>
	/// <param name="FilePath">File Path With ".bmp" Extension</param>
	/// <param name="BitmapSize">Bitmap Size in Pixels</param>
	static void SaveBitmapToFile(_In_ HBITMAP Bitmap, _In_ const std::string &FilePath, _In_ SIZE BitmapSize) {
		
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
