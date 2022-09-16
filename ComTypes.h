#ifndef __COMTYPES_H__
#define __COMTYPES_H__

/*************************************************
*                                                *
* Copyright(c) 2022 Martins Andzans              *
* Licensed Under MIT License                     *
*                                                *
*************************************************/

//===== HEADERS ======//
//-------------------------
// For Data Serializing
#include <ostream>
#include <iomanip>
//-------------------------
//For "ToString" Method
#include <string>
#include <sstream>
//-------------------------
#ifndef _STDINT
#include <stdint.h>
#endif // !_STDINT
//====================//

#ifdef LODWORD
#undef LODWORD
#endif // LODWORD
#ifdef HIDWORD
#undef HIDWORD
#endif // HIDWORD

#define MAKEDOUBLEDWORD(l, h) (int64_t)(((int32_t)(l))|((int64_t)((int32_t)(h))<<32))
#define LODWORD(data) (int32_t)((int64_t)(data))
#define HIDWORD(data) (int32_t)(((int64_t)(data))>>32)

// # Geometry Vertex #
struct Vertex2I {
	
	int32_t x;
	int32_t y;

	//----------------------------------------
	// Constructors

	Vertex2I() noexcept
		: x(0), y(0)
	{ /*...*/ }

	Vertex2I(int32_t initX, int32_t initY) noexcept
		: x(initX), y(initY)
	{ /*...*/ }

	// # Use "MAKEDOUBLEDWORD" Macro - LODWORD() = x | HIDWORD() = y #
	Vertex2I(int64_t initPosition) noexcept
		: x(LODWORD(initPosition)), y(HIDWORD(initPosition))
	{ /*...*/ }

	//----------------------------------------

	//----------------------------------------
	// Setters

	void SetPoint(int32_t newX, int32_t newY) noexcept {
		x = newX, y = newY;
	}

	// # Use "MAKEDOUBLEDWORD" Macro - LODWORD() = x | HIDWORD() = y #
	void SetPoint(int64_t newPosition) noexcept {
		x = LODWORD(newPosition), y = HIDWORD(newPosition);
	}

	//----------------------------------------
	
	// # Copy Constructor #
	Vertex2I(const Vertex2I &other) noexcept = default;
	// # Copy Assigment Operator #
	Vertex2I& operator=(const Vertex2I &other) noexcept = default;

	//----------------------------------------

	bool operator==(const Vertex2I &other) const noexcept {
		return x == other.x && y == other.y;
	}

	bool operator!=(const Vertex2I &other) const noexcept {
		return x != other.x || y != other.y;
	}

	//----------------------------------------
	// Serializing

	// Returns "Vertex2I" Content in JSON Format
	std::string ToStringA() const {
		return "\"Position\": { \"x\": " + std::to_string(x) + ", \"y\": " + std::to_string(y) + " }";
	}

	// Returns "Vertex2I" Content in JSON Format
	std::wstring ToStringW() const {
		return L"\"Position\": { \"x\": " + std::to_wstring(x) + L", \"y\": " + std::to_wstring(y) + L" }";
	}

	#if defined(_WINDOWS_) || defined(__AFX_H__)
	LPCTSTR ToStringT() const {
		#if defined(UNICODE) || defined(_UNICODE)
		return ToStringW().c_str();
		#else // !defined(UNICODE) | !defined(_UNICODE)
		return ToStringA().c_str();
		#endif // defined(UNICODE) | defined(_UNICODE)
	}
	#endif // defined(_WINDOWS_) || defined(__AFX_H__)

	//----------------------------------------

	#if defined(_WINDOWS_) || defined(__AFX_H__)
	operator POINT() const noexcept {
		return { .x = x, .y = y };
	}
	#endif // defined(__WINDOWS_H__) || defined(__AFX_H__)


};

std::ostream& operator<<(std::ostream &outStream, const Vertex2I &Position) {
	outStream << "\"Position\": { \"x\": " << Position.x << ", \"y\": " << Position.y << " }";
	return outStream;
}

std::wostream& operator<<(std::wostream &outWStream, const Vertex2I &Position) {
	outWStream << L"\"Position\": { \"x\": " << Position.x << L", \"y\": " << Position.y << L" }";
	return outWStream;
}

// # Geometry Size #
struct Size2I {

	int32_t width;
	int32_t height;

	//----------------------------------------
	// Constructors
	Size2I() noexcept
		: width(0), height(0)
	{ /*...*/ }

	Size2I(int32_t initWidth, int32_t initHeight) noexcept
		: width(abs(initWidth)), height(abs(initHeight))
	{ /*...*/ }

	// # Use "MAKEDOUBLEDWORD" Macro - LODWORD() = width | HIDWORD() = height #
	Size2I(int64_t initSize) noexcept
		: width(abs(LODWORD(initSize))), height(abs(HIDWORD(initSize)))
	{ /*...*/ }

	//----------------------------------------

	//----------------------------------------
	// Setters

	void SetSize(int32_t newWidth, int32_t newHeight) noexcept {
		width = newWidth, height = newHeight;
	}

	// # Use "MAKEDOUBLEDWORD" Macro - LODWORD() = width | HIDWORD() = height #
	void SetSize(int64_t newSize) noexcept {
		width = LODWORD(newSize), height = HIDWORD(newSize);
	}

	//----------------------------------------

	// # Copy Constructor #
	Size2I(const Size2I &other) noexcept = default;
	// # Copy Assigment Operator #
	Size2I& operator=(const Size2I &other) noexcept = default;

	bool operator==(const Size2I &other) const noexcept {
		return width == other.width && height == other.height;
	}

	bool operator!=(const Size2I &other) const noexcept {
		return width != other.width || height != other.height;
	}

	//----------------------------------------
	// Serializing

	// Returns "Size2I" Content in JSON Format
	std::string ToStringA() const {
		return "\"Size\": { \"width\": " + std::to_string(width) + ", \"height\": " + std::to_string(height) + " }";
	}

	// Returns "Size2I" Content in JSON Format
	std::wstring ToStringW() const {
		return L"\"Size\": { \"width\": " + std::to_wstring(width) + L", \"height\": " + std::to_wstring(height) + L" }";
	}

	#if defined(_WINDOWS_) || defined(__AFX_H__)
	LPCTSTR ToStringT() const {
		#if defined(UNICODE) || defined(_UNICODE)
		return ToStringW().c_str();
		#else // !defined(UNICODE) | !defined(_UNICODE)
		return ToStringA().c_str();
		#endif // defined(UNICODE) | defined(_UNICODE)
	}
	#endif // defined(_WINDOWS_) || ddefined(__AFX_H__)

	//----------------------------------------

	#if defined(_WINDOWS_) || defined(__AFX_H__)
	operator SIZE() const noexcept {
		return { .cx = width, .cy = height };
	}
	#endif // defined(__WINDOWS_H__) || defined(__AFX_H__)

};

std::ostream& operator<<(std::ostream &outStream, const Size2I &Size) {
	outStream << "\"Size\": { \"width\": " << Size.width << ", \"height\": " << Size.height << " }";
	return outStream;
}

std::wostream& operator<<(std::wostream &outStream, const Size2I &Size) {
	outStream << L"\"Size\": { \"width\": " << Size.width << L", \"height\": " << Size.height << L" }";
	return outStream;
}

struct Rect4I {

	int32_t left;
	int32_t top;
	int32_t right;
	int32_t bottom;

	Rect4I()
		: left(0), top(0), right(0), bottom(0)
	{ /*...*/ }



};

#endif // !__COMTYPES_H__
