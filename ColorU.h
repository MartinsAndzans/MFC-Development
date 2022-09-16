#ifndef __COLOR_U_H__
#define __COLOR_U_H__

/*************************************************
*                                                *
* Copyright(c) 2022 Martins Andzans              *
* Licensed Under MIT License                     *
*                                                *
* - In MFC Application                           *
* - - This Header MUST be Included after <afx.h> *
*       or other MFC Headers                     *
*                                                *
*************************************************/

#ifndef _WIN32
#error "This SDK Works Only on Windows"
#endif // !_WIN32

//===== HEADERS ======//
#ifndef __AFX_H__
#include <tchar.h>
#include <Windows.h>
#endif // !__AFX_H__
//-------------------------
// For Data Serializing
#include <ostream>
#include <iomanip>
//-------------------------
// For "ToString" Method
#include <string>
#include <sstream>
//-------------------------
//====================//

#undef RGB
// # XX0000FF - Red / 0xXX00FF00 - Green / 0xXXFF0000 - Blue #
#define RGB(r, g, b) (COLORREF)(((BYTE)(r))|(((WORD)((BYTE)(g)))<<8)|(((DWORD)((BYTE)(b)))<<16))

//----------------------------------------------------------------
// Class <ColorU>
// ---------------------------------------------------------------
// - Pixel Format
// -- # XX0000FF - Red / 0xXX00FF00 - Green / 0xXXFF0000 - Blue #
//----------------------------------------------------------------
class ColorU {
public:
	
	enum Enum : COLORREF {
		
		Aquamarine = RGB(0, 255, 150),

		White = RGB(255, 255, 255),
		Black = RGB(0, 0, 0),

		LightRed = RGB(255, 145, 145),
		Red = RGB(255, 0, 0),
		DarkRed = RGB(145, 0, 0),

		LightGreen = RGB(145, 255, 145),
		Green = RGB(0, 255, 0),
		DarkGreen = RGB(0, 145, 0),

		LightBlue = RGB(145, 145, 255),
		Blue = RGB(0, 0, 255),
		DarkBlue = RGB(0, 0, 145),

		Yellow = RGB(255, 255, 0),
		Orange = RGB(255, 150, 0)

	};

	enum class Value : BYTE {
		R, G, B
	};

	//----------------------------------------
	// Constructors

	// # Default Constructor "m_rgbColor" = "Black Color" #
	ColorU() noexcept {
		m_rgbColor = Enum::Black;
	}

	// # Known Color #
	ColorU(Enum KnownColor) noexcept {
		m_rgbColor = KnownColor;
	}

	// # Color From Byte Values #
	explicit ColorU(BYTE Red, BYTE Green, BYTE Blue) noexcept {
		m_rgbColor = RGB(Red, Green, Blue);
	}

	// # Use "RGB" Macro #
	ColorU(COLORREF rgbColor) noexcept {
		m_rgbColor = rgbColor;
	}

	// # Color From Normalized Float Values #
	explicit ColorU(FLOAT RedF, FLOAT GreenF, FLOAT BlueF) noexcept {

		BYTE Red, Green, Blue;

		RedF <= 0.0F ? Red = 0 : RedF >= 1.0F ? Red = 255 : Red = (BYTE)(RedF * 255);
		GreenF <= 0.0F ? Green = 0 : GreenF >= 1.0F ? Green = 255 : Green = (BYTE)(GreenF * 255);
		BlueF <= 0.0F ? Blue = 0 : BlueF >= 1.0F ? Blue = 255 : Blue = (BYTE)(BlueF * 255);

		m_rgbColor = RGB(Red, Green, Blue);

	}

	//----------------------------------------

	// # Copy Constructor #
	ColorU(const ColorU &other) noexcept = default;
	// # Copy Assigment Operator #
	ColorU& operator=(const ColorU &other) noexcept = default;

	//----------------------------------------
	// Setters
	
	// # Known Color #
	void SetColor(Enum KnownColor) noexcept {
		m_rgbColor = KnownColor;
	}

	// # Color From Byte Values #
	void SetColor(BYTE Red, BYTE Green, BYTE Blue) noexcept {
		m_rgbColor = RGB(Red, Green, Blue);
	}

	// # Use "RGB" Macro #
	void SetColor(COLORREF rgbColor) noexcept {
		m_rgbColor = rgbColor;
	}

	// # Color From Normalized Float Values #
	void SetColor(FLOAT RedF, FLOAT GreenF, FLOAT BlueF) noexcept {

		BYTE Red, Green, Blue;

		RedF <= 0.0F ? Red = 0 : RedF >= 1.0F ? Red = 255 : Red = (BYTE)(RedF * 255);
		GreenF <= 0.0F ? Green = 0 : GreenF >= 1.0F ? Green = 255 : Green = (BYTE)(GreenF * 255);
		BlueF <= 0.0F ? Blue = 0 : BlueF >= 1.0F ? Blue = 255 : Blue = (BYTE)(BlueF * 255);

		m_rgbColor = RGB(Red, Green, Blue);

	}

	//----------------------------------------

	// # Set X Value #
	void SetXValue(Value Type, BYTE Value) noexcept {

		switch (Type) {
		case Value::R:
			m_rgbColor = RGB(Value, GetGValue(m_rgbColor), GetBValue(m_rgbColor));
			break;
		case Value::G:
			m_rgbColor = RGB(GetRValue(m_rgbColor), Value, GetBValue(m_rgbColor));
			break;
		case Value::B:
			m_rgbColor = RGB(GetRValue(m_rgbColor), GetGValue(m_rgbColor), Value);
			break;
		}

	}
	
	#pragma warning(disable:4715)

	// # Get X Value #
	BYTE GetXValue(Value Type) const noexcept {

		switch (Type) {
		case Value::R:
			return GetRValue(m_rgbColor);
		case Value::G:
			return GetGValue(m_rgbColor);
		case Value::B:
			return GetBValue(m_rgbColor);
		}

	}

	#pragma warning(default:4715)

	bool operator==(const ColorU &other) const noexcept {
		return m_rgbColor == other.m_rgbColor;
	}

	bool operator!=(const ColorU &other) const noexcept {
		return m_rgbColor != other.m_rgbColor;
	}


	// Returns "ColorU" Content in JSON Format
	std::string ToStringA() const {

		std::string Serialized;
		std::stringstream StringStream;
		StringStream << *this;
		std::getline(StringStream, Serialized);
		return Serialized;

	}

	// Returns "ColorU" Content in JSON Format
	std::wstring ToStringW() const {

		std::wstring Serialized;
		std::wstringstream StringStream;
		StringStream << *this;
		std::getline(StringStream, Serialized);
		return Serialized;

	}

	#if defined(__AFX_H__)
	CString ToStringT() {
		#if defined(UNICODE) | defined(_UNICODE)
		return ToStringW().c_str();
		#else // !defined(UNICODE) | !defined(_UNICODE)
		return ToStringA().c_str();
		#endif // defined(UNICODE) | defined(_UNICODE)
	}
	#endif // defined(__AFX_H__)

	COLORREF data(void) const noexcept {
		return m_rgbColor;
	}

	operator COLORREF() const noexcept {
		return m_rgbColor;
	}

	friend std::ostream& operator<<(std::ostream &outStream, const ColorU &crColor);
	friend std::wostream& operator<<(std::wostream &outStream, const ColorU &crColor);

private:

	COLORREF m_rgbColor;

};

std::ostream& operator<<(std::ostream &outStream, const ColorU &crColor) {
	
	// Save Original Stream State
	std::ios SavedState(nullptr);
	SavedState.copyfmt(outStream);
	
	// Output "ColorU" Content in JSON Format 
	// R"({ "m_rgbColor": "0x{crColor.data()}" })"
	outStream << "{ \"m_rgbColor\": \"0x"
		<< std::uppercase << std::setfill('0')
		<< std::setw(sizeof(crColor.data()) * 2)
		<< std::hex << crColor.data() << "\" }";
	
	// Restore Original Stream State
	outStream.copyfmt(SavedState);
	
	// Return Stream Reference
	return outStream;

}

std::wostream& operator<<(std::wostream &outWStream, const ColorU &crColor) {
	
	// Save Original Stream State
	std::wios SavedState(nullptr);
	SavedState.copyfmt(outWStream);
	
	// Output "ColorU" Content in JSON Format
	// LR"({ "m_rgbColor": "0x{crColor.data()}" })"
	outWStream << L"{ \"m_rgbColor\": \"0x"
		<< std::uppercase << std::setfill(L'0')
		<< std::setw(sizeof(crColor.data()) * 2)
		<< std::hex << crColor.data() << L"\" }";

	// Restore Original Stream State
	outWStream.copyfmt(SavedState);
	
	// Return Stream Reference
	return outWStream;

}

#endif // !__COLOR_U_H__
