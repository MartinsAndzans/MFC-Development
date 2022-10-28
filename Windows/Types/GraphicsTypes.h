#ifndef __GRAPHICS_TYPES_H__
#define __GRAPHICS_TYPES_H__

/**************************************
*                                     *
* Copyright(c) [2022] Martins Andzans *
* Licensed Under: [MIT License]       *
*                                     *
**************************************/

//===== HEADERS ======//
#include <string>
#include <type_traits>
#ifndef _STDINT
#include <stdint.h>
#endif // !_STDINT
//====================//

#ifndef _INC_MATH
	#if defined _M_IX86 && _M_IX86_FP < 2 && !defined _M_FP_FAST
		using float_t = long double;
		using double_t = long double;
	#else
		using float_t = float;
		using double_t = double;
	#endif
#endif // !_INC_MATH

// # Geometry Vertex2T <Type> #
template<typename Type>
struct Vertex2T {

	static_assert(std::is_arithmetic<Type>::value,
		"only arithmetic types allowed");

	Type x;
	Type y;

	//----------------------------------------
	// Constructors

	// # Default Constructor #
	// # Initiliaze Object With Zeros #
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

	// # Offset This Location To Other Location #
	void Offset(const Vertex2T &offset) noexcept {
		x += offset.x;
		y += offset.y;
	}

	// Returns TRUE:
	// - If This Location Is Equal With Other Location
	bool Equal(const Vertex2T &other) const noexcept {
		return x == other.x && y == other.y;
	}

	// Returns TRUE:
	// - If X And Y Is Zero
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
	// { "x": {}, "y": {} }
	std::string ToString(void) const {
		return R"({ "x": )" + std::to_string(x) +
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

	static_assert(std::is_arithmetic<Type>::value,
		"only arithmetic types allowed");

	Type width;
	Type height;

	//----------------------------------------
	// Constructors

	// # Default Constructor #
	// # Initiliaze Object With Zeros #
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
	void wh(Type value) noexcept {
		width = height = value;
	}

	// # Scales This Object Size By Given Factor >= 1 #
	void Scale(Type wFactor, Type hFactor) noexcept {
		width = (wFactor > 1) ? width * wFactor : width;
		height = (hFactor > 1) ? height * hFactor : height;
	}

	// Returns TRUE:
	// - If This Size Is Equal With Other Size
	bool Equal(const Size2T &other) const noexcept {
		return width == other.width && height == other.height;
	}

	// Returns TRUE:
	// - If Width And Height Is Zero
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
	// { "width": {}, "height": {} }
	std::string ToString(void) const {
		return R"({ "width": )" + std::to_string(width) +
			R"(, "height": )" + std::to_string(height) + R"( })";
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

	static_assert(std::is_arithmetic<Type>::value,
		"only arithmetic types allowed");

	Vertex2T<Type> Location;
	Size2T<Type> Size;

	//----------------------------------------
	// Constructors

	// # Default Constructor #
	// # Initiliaze Object With Zeros #
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

	// # Return Rectangle Left Coordinate #
	Type Left(void) const noexcept {
		return Location.x;
	}
	
	// # Return Rectangle Top Coordinate #
	Type Top(void) const noexcept {
		return Location.y;
	}

	// # Return Rectangle Right Coordinate #
	Type Right(void) const noexcept {
		return Location.x + Size.width;
	}
	
	// # Return Rectangle Bottom Coordinate #
	Type Bottom(void) const noexcept {
		return Location.y + Size.height;
	}

	// # Returns Rectangle "Center-Point" Coordinates #
	Vertex2T<Type> CenterPoint(void) const noexcept {
		return Vertex2T<Type>(Size.width / 2 + Location.x, Size.height / 2 + Location.y);
	}
	
	// # Returns TRUE If This Rectangle Intersect With Other Rectangle #
	bool IntersectWith(const Rect4T &other) const noexcept {

		// No Intersaction States
		//           <3>
		//       +---------+
		//  <1>  |    T    |  <2>
		//       +---------+
		//           <4>

		// # No Intersection Return FALSE #
		if (other.Location.x + other.Size.width <= Location.x || // <1>
			other.Location.x >= Location.x + Size.width || // <2>
			other.Location.y + other.Size.height <= Location.y || // <3>
			other.Location.y >= Location.y + Size.height) { // <4>
			return false;
		}

		// # Intersection Return TRUE #
		return true;

	}

	// # Returns TRUE If Point is Inside Rectangle #
	bool PointInRect(const Vertex2T<Type> &point) const noexcept {

		// Point Outside Rectangle States
		//           <3>
		//       +---------+
		//  <1>  |    T    |  <2>
		//       +---------+
		//           <4>

		// # Point Outside Rectangle #
		if (point.x <= Location.x || // <1>
			point.x >= Location.x + Size.width || // <2>
			point.y <= Location.y || // <3>
			point.y >= Location.y + Size.height) { // <4 >
			return false;
		}

		// # Point Inside Rectangle #
		return true;

	}

	// Returns TRUE:
	// - If This Rectangle Is Equal With Other Rectangle
	bool Equal(const Rect4T &other) const noexcept {
		return Location == other.Location && Size == other.Size;
	}
	
	// Returns TRUE:
	// - If Location And Size Is Zero
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
	// { "Location": { "x": {}, "y": {} }, "Size": { "width": {}, "height": {} } }
	std::string ToString(void) const {
		return R"({ "Location": )" + Location.ToString() +
			R"(, "Size": )" + Size.ToString() + R"( })";
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
