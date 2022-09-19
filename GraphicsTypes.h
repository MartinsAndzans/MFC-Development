#ifndef __GRAPHICS_TYPES_H__
#define __GRAPHICS_TYPES_H__

/*************************************************
*                                                *
* Copyright(c) 2022 Martins Andzans              *
* Licensed Under MIT License                     *
*                                                *
*************************************************/

//===== HEADERS ======//
#include "IJType.h"
#include <utility>
//====================//

//======== REDEFINE MACROS ========//
#pragma push_macro("LODWORD")
#pragma push_macro("HIDWORD")
#undef LODWORD
#undef HIDWORD
//=================================//

#define MAKEDOUBLEDWORD(l, h) (int64_t)(((int32_t)(l))|((int64_t)((int32_t)(h))<<32))
#define LODWORD(data) (int32_t)((int64_t)(data))
#define HIDWORD(data) (int32_t)(((int64_t)(data))>>32)

// # Geometry Vertex #
struct Vertex2I : implements IJType {

	int32_t x, y;

	//----------------------------------------
	// Constructors

	// # Default Constructor Set "Vertex2I" To (0, 0) #
	Vertex2I() noexcept
		: x(0), y(0)
	{ /*...*/ }

	// # Set "Vertex2I" To (initX, initY) #
	Vertex2I(int32_t initX, int32_t initY) noexcept
		: x(initX), y(initY)
	{ /*...*/ }

	// # Use "MAKEDOUBLEDWORD" Macro - LODWORD = X | HIDWORD = Y #
	Vertex2I(int64_t initPosition) noexcept
		: x(LODWORD(initPosition)), y(HIDWORD(initPosition))
	{ /*...*/ }

	//----------------------------------------

	// # Copy Constructor #
	Vertex2I(const Vertex2I &other) noexcept = default;
	// # Copy Assigment Operator #
	Vertex2I& operator=(const Vertex2I &other) noexcept = default;

	//----------------------------------------
	// Setters

	// # Set "Vertex2I" To (newX, newY) #
	void SetPoint(int32_t newX, int32_t newY) noexcept {
		x = newX;
		y = newY;
	}

	// # Use "MAKEDOUBLEDWORD" Macro - LODWORD = X | HIDWORD = Y #
	void SetPoint(int64_t newPosition) noexcept {
		x = LODWORD(newPosition);
		y = HIDWORD(newPosition);
	}

	//----------------------------------------

	// # Offsets "x" by "xOffset" and "y" by "yOffset" #
	void Offset(int32_t xOffset, int32_t yOffset) noexcept {
		x += xOffset;
		y += yOffset;
	}

	// # Use "MAKEDOUBLEDWORD" Macro - LODWORD = XOFFSET | HIDWORD = YOFFSET #
	// # Offsets "x" by "xOffset" and "y" by "yOffset" #
	void Offset(int64_t Offset) noexcept {
		x += LODWORD(Offset);
		y += HIDWORD(Offset);
	}

	bool operator==(const Vertex2I &other) const noexcept {
		return x == other.x && y == other.y;
	}

	bool operator!=(const Vertex2I &other) const noexcept {
		return x != other.x || y != other.y;
	}

	//----------------------------------------
	// Serializing

	// Returns "Vertex2I" Content in JSON Format
	// "Vertex2I": { "x": x, "y": y }
	std::string ToString() const override {
		return "\"Vertex2I\": { \"x\": " + std::to_string(x)
			+ ", \"y\": " + std::to_string(y) + " }";
	}

	//----------------------------------------

	// # Default Destructor #
	~Vertex2I() noexcept override = default;

};

// # Geometry Size #
struct Size2I : implements IJType {

	int32_t width, height;

	//----------------------------------------
	// Constructors

	// # Default Constructor Set "Size2I" To (0, 0) #
	Size2I() noexcept
		: width(0), height(0)
	{ /*...*/ }

	// # Set "Size2I" To (initWidth, initHeight) #
	Size2I(int32_t initWidth, int32_t initHeight) noexcept
		: width(initWidth), height(initHeight)
	{ /*...*/ }

	// # Use "MAKEDOUBLEDWORD" Macro - LODWORD = WIDTH | HIDWORD = HEIGHT #
	Size2I(int64_t initSize) noexcept
		: width(LODWORD(initSize)), height(HIDWORD(initSize))
	{ /*...*/ }

	//----------------------------------------

	// # Copy Constructor #
	Size2I(const Size2I &other) noexcept = default;
	// # Copy Assigment Operator #
	Size2I& operator=(const Size2I &other) noexcept = default;

	//----------------------------------------
	// Setters

	// # Set "Size2I" To (newWidth, newHeight) #
	void SetSize(int32_t newWidth, int32_t newHeight) noexcept {
		width = newWidth;
		height = newHeight;
	}

	// # Use "MAKEDOUBLEDWORD" Macro - LODWORD = WIDTH | HIDWORD = HEIGHT #
	void SetSize(int64_t newSize) noexcept {
		width = LODWORD(newSize);
		height = HIDWORD(newSize);
	}

	//----------------------------------------

	void AddSize(const Size2I &other) noexcept {
		width += other.width;
		height += other.height;
	}

	void SubstractSize(const Size2I &other) noexcept {
		width -= other.width;
		height -= other.height;
	}

	bool operator==(const Size2I &other) const noexcept {
		return width == other.width && height == other.height;
	}

	bool operator!=(const Size2I &other) const noexcept {
		return width != other.width || height != other.height;
	}

	void operator+=(const Size2I &other) noexcept {
		AddSize(other);
	}

	void operator-=(const Size2I &other) noexcept {
		SubstractSize(other);
	}

	Size2I operator+(const Size2I& other) noexcept {
		Size2I Temp(*this);
		Temp.AddSize(other);
		return Temp;
	}


	Size2I operator-(const Size2I& other) noexcept {
		Size2I Temp(*this);
		Temp.SubstractSize(other);
		return Temp;
	}

	//----------------------------------------
	// Serializing

	// Returns "Size2I" Content in JSON Format
	// "Size21": { "width": width, "height": height }
	std::string ToString() const override {
		return "\"Size2I\": { \"width\": " + std::to_string(width)
			+ ", \"height\": " + std::to_string(height) + " }";
	}

	//----------------------------------------

	// # Default Destructor #
	~Size2I() noexcept override = default;

};

struct Rect4I : implements IJType {

	int32_t left;
	int32_t top;
	int32_t right;
	int32_t bottom;

	//----------------------------------------
	// Constructors

	Rect4I() noexcept {
		left = 0;
		top = 0;
		right = 0;
		bottom = 0;
	}

	Rect4I(int32_t initLeft, int32_t initTop,
		int32_t initRight, int32_t initBottom) noexcept {
		left = initLeft;
		top = initTop;
		right = initRight;
		bottom = initBottom;
	}

	Rect4I(const Vertex2I &vLeftTop,
		const Vertex2I &vRightBottom) noexcept {
		left = vLeftTop.x;
		top = vLeftTop.y;
		right = vRightBottom.x;
		bottom = vRightBottom.y;
	}

	//----------------------------------------

	// # Copy Constructor #
	Rect4I(const Rect4I &other) noexcept = default;
	// # Copy Assigment Operator #
	Rect4I& operator=(const Rect4I &other) noexcept = default;

	// # Returns Rectangle "Width" #
	int32_t Width() const noexcept {
		return (left >= right) ? left - right : right - left;
	}

	// # Returns Rectangle "Height" #
	int32_t Height() const noexcept {
		return (top >= bottom) ? top - bottom : bottom - top;
	}

	// # Returns Rectangle "LeftTop" Coordinates #
	Vertex2I LeftTop() const noexcept {

		Vertex2I vLeftTop;

		vLeftTop.x = (left >= right) ? right : left;
		vLeftTop.y = (top >= bottom) ? bottom : top;

		return vLeftTop;

	}

	// # Returns Rectangle "RightBottom" Coordinates #
	Vertex2I RightBottom() const noexcept {

		Vertex2I vRightBottom;

		vRightBottom.x = (left >= right) ? left : right;
		vRightBottom.y = (top >= bottom) ? top : bottom;
		
		return vRightBottom;

	}

	// # This Method Normamalizes Rectangle #
	void NormalizeRect() noexcept {
		
		if (left > right) {
			std::swap(left, right);
		}

		if (top > bottom){
			std::swap(top, bottom);
		}

	}

	// # Return TRUE If Rectangles Is Equal #
	bool EqualRect(const Rect4I &other) const noexcept {
		return LeftTop() == other.LeftTop() && RightBottom() == other.RightBottom();
	}

	// # Return TRUE If Rectangle Is (0, 0, 0, 0) #
	bool IsRectNull() const noexcept {
		return left == 0 && top == 0 && right == 0 && bottom == 0;
	}

	// # Return TRUE If Rectangle Has No Area #
	bool IsRectEmpty() const noexcept {
		return Width() == 0 || Height() == 0;
	}

	bool operator==(const Rect4I &other) const noexcept {
		return EqualRect(other);
	}
	
	bool operator!=(const Rect4I &other) const noexcept {
		return !EqualRect(other);
	}

	//----------------------------------------
	// Serializing

	// Returns "Size2I" Content in JSON Format
	// "Rect4I": { "left": left, "top": top, "right": right, "bottom": bottom }
	std::string ToString() const override {

		Vertex2I vLeftTop = LeftTop();
		Vertex2I vRightBottom = RightBottom();

		return "\"Rect4I\": { \"left\": " + std::to_string(vLeftTop.x)
			+ ", \"top\": " + std::to_string(vLeftTop.y)
			+ ", \"right\": " + std::to_string(vRightBottom.x)
			+ ", \"bottom\": " + std::to_string(vRightBottom.y) + " }";

	}

	//----------------------------------------

	// # Default Destructor #
	~Rect4I() noexcept override = default;

};

#endif // !__GRAPHICS_TYPES_H__
