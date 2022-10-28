#ifndef __COLOR_U_H__
#define __COLOR_U_H__

/**************************************
*                                     *
* Copyright(c) [2022] Martins Andzans *
* Licensed Under [MIT License]        *
*                                     *
**************************************/

//===== HEADERS ======//
#include <string>
#include <type_traits>
#ifndef _STDINT
#include <stdint.h>
#endif // !_STDINT
//====================//

#ifndef _MINWINDEF_
using BYTE = BYTE;
using DWORD = DWORD;
using WORD = WORD;
using FLOAT = float;
#endif // !_MINWINDEF_
#ifndef _WINDEF_
using COLORREF = DWORD;
#endif // !_WINNT

#pragma warning(disable:4005)
// # XX0000FF - Red / 0xXX00FF00 - Green / 0xXXFF0000 - Blue #
#define RGB(r, g, b) (COLORREF)(((BYTE)(r))|(((WORD)((BYTE)(g)))<<8)|(((DWORD)((BYTE)(b)))<<16))
#define GetRValue(rgb) (BYTE)((DWORD)(rgb))
#define GetGValue(rgb) (BYTE)(((WORD)((DWORD)(rgb)))>>8)
#define GetBValue(rgb) (BYTE)(((DWORD)(rgb))>>16)
#pragma warning(default:4005)

namespace Windows {

	//----------------------------------------------------------------
	// Class "ColorU"
	// - This Class Provides Functionality For GDI Compatible RGB Color Manipulations
	// ---------------------------------------------------------------
	// Pixel Format
	// - # XX0000FF - Red / 0xXX00FF00 - Green / 0xXXFF0000 - Blue #
	//----------------------------------------------------------------
	class ColorU {
	public:
	
		enum class Enum : COLORREF {
	
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
	
		enum class Component : BYTE {
			R, G, B
		};
	
		//----------------------------------------
		// Constructors
	
		// # Default Constructor #
		// # Initialize Color To Black #
		ColorU(void) noexcept {
			m_rgbColor = static_cast<std::underlying_type_t<Enum>>(Enum::Black);
		}
	
		// # Known Color #
		ColorU(Enum KnownColor) noexcept {
			m_rgbColor = static_cast<std::underlying_type_t<Enum>>(KnownColor);
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
		ColorU(const ColorU& other) noexcept = default;
		// # Copy Assigment Operator #
		ColorU& operator=(const ColorU& other) noexcept = default;
	
		//----------------------------------------
		// Setters
	
		// # Known Color #
		void SetColor(Enum KnownColor) noexcept {
			m_rgbColor = static_cast<std::underlying_type_t<Enum>>(KnownColor);
		}
	
		// # Color From Byte Values #
		void SetColor(BYTE Red, BYTE Green, BYTE Blue) noexcept {
			m_rgbColor = RGB(Red, Green, Blue);
		}
	
		// # Use "RGB" Macro #
		void SetColor(DWORD rgbColor) noexcept {
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
	
		/// <summary>This Method Set Requested Color Component To Value</summary>
		/// <param name="Component">R - Red Component / G - Green Component / B - Blue Component</param>
		/// <param name="Value">New Value For Requested Color Component</param>
		void SetXValue(Component crComponent, BYTE Value) noexcept {
	
			switch (crComponent) {
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
		/// <returns>Requested Color Component Value</returns>
		BYTE GetXValue(Component crComponent) const noexcept {
	
			switch (crComponent) {
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
		// { "Red": {}, "Green": {}, "Blue": {} }
		std::string ToString(void) const {
	
			BYTE Red = GetRValue(m_rgbColor);
			BYTE Green = GetGValue(m_rgbColor);
			BYTE Blue = GetBValue(m_rgbColor);
	
			return R"({ "Red": )" + std::to_string(Red) + R"(, "Green": )" + std::to_string(Green) +
				R"(, "Blue": )" + std::to_string(Blue) + R"( })";
	
		}
	
		//----------------------------------------
	
		COLORREF data(void) const noexcept {
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

}

#endif // !__COLOR_U_H__
