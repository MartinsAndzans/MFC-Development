#ifndef __GRAPHICS_TYPES_H__
#define __GRAPHICS_TYPES_H__

/************************************
*                                   *
* Copyright(c) 2022 Martins Andzans *
* Licensed Under MIT License        *
*                                   *
************************************/

//===== HEADERS ======//
#include <string>
#include <utility>
#ifndef _STDINT
#include <stdint.h>
#endif // !_STDINT
//====================//

using float_t = float;
using double_t = double;

// # Geometry Vertex2T <Type> #
template<typename Type>
struct Vertex2T {

	Type x;
	Type y;

	//----------------------------------------
	// Constructors

	// # Default Constructor Initialize Object with Zeros #
	Vertex2T(void) noexcept
		: x(0), y(0)
	{ /*...*/ }

	Vertex2T(Type x, Type y) noexcept
		: x(x), y(y)
	{ /*...*/ }

	//----------------------------------------

	// # Copy Constructor #
	Vertex2T(const Vertex2T &other) noexcept = default;
	// # Copy Assigment Operator #
	Vertex2T& operator=(const Vertex2T &other) noexcept = default;

	// # Set X and Y To VALUE #
	void xy(Type value) noexcept {
		x = y = value;
	}

	/// <param name="xOffset">
	/// <para>In Screen Coordinate System</para>
	/// <para>- "-xOffset" Offset To Left</para>
	/// <para>- "xOffset" Offset To Right</para>
	/// </param>
	/// <param name="yOffset">
	/// <para>In Screen Coordinate System</para>
	/// <para>- "-yOffset" Offset To Up</para>
	/// <para>- "yOffset" Offset To Down</para>
	/// </param>
	void Offset(Type xOffset, Type yOffset) noexcept {
		x += xOffset;
		y += yOffset;
	}

	// # Offset This Object Location To Other Object Location #
	void Offset(const Vertex2T &offset) noexcept {
		x += offset.x;
		y += offset.y;
	}

	// # Returns TRUE If This Object Location is Equal with Other Object Location #
	bool Equal(const Vertex2T &other) const noexcept {
		return x == other.x && y == other.y;
	}

	// # Returns TRUE If X and Y is Zero #
	bool IsNull(void) const noexcept {
		return x == 0 && y == 0;
	}

	bool operator==(const Vertex2T &other) const noexcept {
		return Equal(other);
	}

	bool operator!=(const Vertex2T &other) const noexcept {
		return !Equal(other);
	}

	//----------------------------------------
	// Serializing

	// Returns "Vertex2T" Content in JSON Format
	// "Vertex2T": { "x": {}, "y": {} }
	std::string ToString(void) const {
		return R"("Vertex2T": { "x": )" + std::to_string(x) +
			   R"(, "y": )" + std::to_string(y) + R"( })";
	}

	//----------------------------------------

	// # Default Destructor #
	~Vertex2T() noexcept = default;

};

// # Geometry Vertex2S <int16_t> #
using Vertex2S = Vertex2T<int16_t>;
// # Geometry Vertex2US <uint16_t> #
using Vertex2US = Vertex2T<uint16_t>;
// # Geometry Vertex2I <int32_t> #
using Vertex2I = Vertex2T<int32_t>;
// # Geometry Vertex2UI <uint32_t> #
using Vertex2UI = Vertex2T<uint32_t>;
// # Geometry Vertex2L <int64_t> #
using Vertex2L = Vertex2T<int64_t>;
// # Geometry Vertex2UL <uint64_t> #
using Vertex2UL = Vertex2T<uint64_t>;
// # Geometry Vertex2F <float_t> #
using Vertex2F = Vertex2T<float_t>;
// # Geometry Vertex2D <double_t> #
using Vertex2D = Vertex2T<double_t>;

// # Geometry Size2T <Type> #
template<typename Type>
struct Size2T {

	Type width;
	Type height;

	//----------------------------------------
	// Constructors

	// # Default Constructor Initialize Object with Zeros #
	Size2T() noexcept
		: width(0), height(0)
	{ /*...*/ }

	Size2T(Type width, Type height) noexcept
		: width(width), height(height)
	{ /*...*/ }

	//----------------------------------------

	// # Copy Constructor #
	Size2T(const Size2T &other) noexcept = default;
	// # Copy Assigment Operator #
	Size2T& operator=(const Size2T &other) noexcept = default;
	
	// # Set Width and Height To VALUE #
	void wh(Type value) {
		width = height = value;
	}

	// # Scales This Object Size By Given Factor >= 1 #
	void Scale(int16_t wFactor, int16_t hFactor) noexcept {
		(wFactor > 1) ? width *= wFactor : width;
		(hFactor > 1) ? height *= hFactor : height;
	}

	// # Scales This Object Size By Given Factor >= 1 #
	void Scale(float_t wFactor, float_t hFactor) noexcept {
		(wFactor > 1.0F) ? width = static_cast<Type>(width * wFactor) : width;
		(hFactor > 1.0F) ? height = static_cast<Type>(height * hFactor) : height;
	}

	// # Returns TRUE If This Object Size is Equal with Other Object Size #
	bool Equal(const Size2T &other) const noexcept {
		return width == other.width && height == other.height;
	}

	// # Returns TRUE If Width and Height is Zero #
	bool IsNull(void) const noexcept {
		return width == 0 && height == 0;
	}

	bool operator==(const Size2T &other) const noexcept {
		return Equal(other);
	}

	bool operator!=(const Size2T &other) const noexcept {
		return !Equal(other);
	}

	//----------------------------------------
	// Serializing

	// Returns "Size2T" Content in JSON Format
	// "Size2T": { "width": {}, "height": {} }
	std::string ToString(void) const {
		return R"("Size2T": { "width": )" + std::to_string(width)
			+ R"(, "height": )" + std::to_string(height) + R"( })";
	}

	//----------------------------------------

	// # Default Destructor #
	~Size2T() noexcept = default;

};

// # Geometry Size2S <int16_t> #
using Size2S = Size2T<int16_t>;
// # Geometry Size2US <uint16_t> #
using Size2US = Size2T<uint16_t>;
// # Geometry Size2I <int32_t> #
using Size2I = Size2T<int32_t>;
// # Geometry Size2UI <uint32_t> #
using Size2UI = Size2T<uint32_t>;
// # Geometry Size2L <int64_t> #
using Size2L = Size2T<int64_t>;
// # Geometry Size2UL <uint64_t> #
using Size2UL = Size2T<uint64_t>;
// # Geometry Size2F <float_t> #
using Size2F = Size2T<float_t>;
// # Geometry Size2D <double_t> #
using Size2D = Size2T<double_t>;

// # Geometry Rect4T <Type> #
template<typename Type>
struct Rect4T {

	Vertex2T<Type> Location;
	Size2T<Type> Size;

	//----------------------------------------
	// Constructors

	// # Default Constructor Initialize Object with Zeros #
	Rect4T() noexcept
		: Location(), Size()
	{ /*...*/ }

	Rect4T(Type x, Type y, Type width, Type height) noexcept
		: Location(x, y), Size(width, height)
	{ /*...*/ }

	Rect4T(const Vertex2T<Type> &Location, const Size2T<Type> &Size) noexcept
		: Location(Location), Size(Size)
	{ /*...*/ }

	//----------------------------------------

	// # Copy Constructor #
	Rect4T(const Rect4T &other) noexcept = default;
	// # Copy Assigment Operator #
	Rect4T& operator=(const Rect4T &other) noexcept = default;

	// # Returns Rectangle "Center-Point" Coordinates #
	Vertex2T<Type> CenterPoint(void) const noexcept {
		return Vertex2T<Type>(Size.width / 2 + Location.x, Size.height / 2 + Location.y);
	}

	// # Returns TRUE If This Rectangle Intersect with Other Rectangle #
	bool IntersectWith(const Rect4T &other) const noexcept {

		// # No Intersection Return FALSE #
		if (other.Location.x + other.Size.width <= Location.x ||
			other.Location.x >= Location.x + Size.width ||
			other.Location.y + other.Size.height <= Location.y ||
			other.Location.y >= Location.y + Size.height) {
			return false;
		}

		// # Intersection Return TRUE #
		return true;

	}

	// # Returns TRUE If Point is Inside Rectangle #
	bool PointInRect(const Vertex2T<Type> &point) const noexcept {

		// # Point Outside Rectangle #
		if (point.x <= Location.x ||
			point.x >= Location.x + Size.width ||
			point.y <= Location.y ||
			point.y >= Location.y + Size.height) {
			return false;
		}

		// # Point Inside Rectangle #
		return true;

	}

	// # Returns TRUE If This Rectangle is Equal with Other Rectangle #
	bool Equal(const Rect4T &other) const noexcept {
		return Location == other.Location && Size == other.Size;
	}

	// # Returns TRUE If Rectangle Location and Size Is Zero #
	bool IsNull(void) const noexcept {
		return Location.IsNull() && Size.IsNull();
	}

	bool operator==(const Rect4T &other) const noexcept {
		return Equal(other);
	}
	
	bool operator!=(const Rect4T &other) const noexcept {
		return !Equal(other);
	}

	//----------------------------------------
	// Serializing

	// Returns "Rect4T" Content in JSON Format
	// "Rect4T": { "Vertex2T": { "x": {}, "y": {} }, "Size2T": { "width": {}, "height": {} } })";
	std::string ToString(void) const {
		return R"("Rect4I": { )" + Location.ToString()
			+ R"(, )" + Size.ToString() + R"( })";
	}

	//----------------------------------------

	// # Default Destructor #
	~Rect4T() noexcept = default;

};

// # Geometry Rect4S <int16_t> #
using Rect4S = Rect4T<int16_t>;
// # Geometry Rect4US <uint16_t> #
using Rect4US = Rect4T<uint16_t>;
// # Geometry Rect4I <int32_t> #
using Rect4I = Rect4T<int32_t>;
// # Geometry Rect4UI <uint32_t> #
using Rect4UI = Rect4T<uint32_t>;
// # Geometry Rect4L <int64_t> #
using Rect4L = Rect4T<int64_t>;
// # Geometry Rect4UL <uint64_t> #
using Rect4UL = Rect4T<uint64_t>;
// # Geometry Rect4F <float_t> #
using Rect4F = Rect4T<float_t>;
// # Geometry Rect4D <double_t> #
using Rect4D = Rect4T<double_t>;

#endif // !__GRAPHICS_TYPES_H__