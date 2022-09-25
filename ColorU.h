#ifndef __COLOR_U_H__
#define __COLOR_U_H__

/*************************************************
*                                                *
* Copyright(c) 2022 Martins Andzans              *
* Licensed Under MIT License                     *
*                                                *
*************************************************/

//===== HEADERS ======//
#include <string>
#include <sstream>
#include <iomanip>
#ifndef _STDINT
#include <stdint.h>
#endif // !_STDINT
//====================//

//======== REDEFINE MACROS ========//
#pragma push_macro("RGB")
#pragma push_macro("GetRValue")
#pragma push_macro("GetGValue")
#pragma push_macro("GetBValue")
//=================================//

#ifndef _MINWINDEF_
typedef float FLOAT;
typedef uint8_t BYTE;
typedef uint16_t WORD;
typedef uint32_t DWORD;
#endif // !_MINWINDEF_
#ifndef _WINDEF_
typedef DWORD COLORREF;
#endif // !_WINDEF_

#undef RGB
// # XX0000FF - Red / 0xXX00FF00 - Green / 0xXXFF0000 - Blue #
#define RGB(r, g, b) (COLORREF)(((BYTE)(r))|(((WORD)((BYTE)(g)))<<8)|(((DWORD)((BYTE)(b)))<<16))
#undef GetRValue
#define GetRValue(rgb) (BYTE)((COLORREF)(rgb))
#undef GetGValue
#define GetGValue(rgb) (BYTE)(((WORD)((COLORREF)(rgb)))>>8)
#undef GetBValue
#define GetBValue(rgb) (BYTE)(((COLORREF)(rgb))>>16)

//----------------------------------------------------------------
// Class "ColorU"
// - This Class Provides Functionality For RGB Color Manipulations
// ---------------------------------------------------------------
// Pixel Format
// - # XX0000FF - Red / 0xXX00FF00 - Green / 0xXXFF0000 - Blue #
//----------------------------------------------------------------
class ColorU {
public:
	
	enum Enum : COLORREF {
		
		White = RGB(191, 191, 191),
		Gray = RGB(89, 89, 89),
		
		BrightWhite = RGB(255, 255, 255),
		Black = RGB(0, 0, 0),

		Red = RGB(255, 0, 0),
		Green = RGB(0, 255, 0),
		Blue = RGB(0, 0, 255),
		
		LightRed = RGB(255, 128, 128),
		LightGreen = RGB(128, 255, 128),
		LightBlue = RGB(128, 128, 255),
		
		DarkRed = RGB(128, 0, 0),
		DarkGreen = RGB(0, 128, 0),
		DarkBlue = RGB(0, 0, 128),

		Aqua = RGB(0, 191, 255),
		Purple = RGB(191, 0, 255),
		Yellow = RGB(255, 191, 0),

		LightAqua = RGB(0, 255, 255),
		LightPurple = RGB(255, 0, 255),
		LightYellow = RGB(255, 255, 0)

	};


	enum Component : BYTE {
		R, G, B
	};

	//----------------------------------------
	// Constructors

	// # Default Constructor "m_rgbColor" = "Black Color" #
	ColorU(void) noexcept {
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

	/// <summary>This Method Set One Component To Color</summary>
	/// <param name="Component">R - Red Component / G - Green Component / B - Blue Component</param>
	/// <param name="Value">New Value For Requested Component</param>
	void SetXValue(Component Component, BYTE Value) noexcept {
		
		switch (Component) {
		case Component::R:
			m_rgbColor = RGB(Value, GetGValue(m_rgbColor), GetBValue(m_rgbColor));
			break;
		case Component::G:
			m_rgbColor = RGB(GetRValue(m_rgbColor), Value, GetBValue(m_rgbColor));
			break;
		case Component::B:
			m_rgbColor = RGB(GetRValue(m_rgbColor), GetGValue(m_rgbColor), Value);
			break;
		}

	}
	
	#pragma warning(disable:4715)

	/// <param name="Component">R - Red Component / G - Green Component / B - Blue Component</param>
	/// <returns>Returns Requested Component Value</returns>
	BYTE GetXValue(Component Component) const noexcept {

		switch (Component) {
		case Component::R:
			return GetRValue(m_rgbColor);
		case Component::G:
			return GetGValue(m_rgbColor);
		case Component::B:
			return GetBValue(m_rgbColor);
		}

	}

	#pragma warning(default:4715)

	bool Equal(const ColorU &other) const noexcept {
		return m_rgbColor == other.m_rgbColor;
	}

	bool operator==(const ColorU &other) const noexcept {
		return Equal(other);
	}

	bool operator!=(const ColorU &other) const noexcept {
		return !Equal(other);
	}

	//----------------------------------------
	// Serializing

	// Returns "ColorU" Content in JSON Format
	// "ColorU": { "m_rgbColor": "0x00BBGGRR" }
	std::string ToString(void) const {
		
		std::stringstream sstream;
		sstream << R"("ColorU": { "m_rgbColor": "0x)"
			<< std::setw(sizeof(m_rgbColor) * 2)
			<< std::setfill('0') << std::setbase(16)
			<< std::uppercase << m_rgbColor << R"(" })";

		std::string JsonFormat;
		std::getline(sstream, JsonFormat);

		return JsonFormat;

	}

	//----------------------------------------

	COLORREF Data(void) const noexcept {
		return m_rgbColor;
	}

	operator COLORREF() const noexcept {
		return m_rgbColor;
	}

	// # Default Destructor #
	~ColorU() noexcept = default;

private:

	COLORREF m_rgbColor;

};

#endif // !__COLOR_U_H__
