#ifndef __GDI_PLUS_H__
#define __GDI_PLUS_H__

/*************************************************
*                                                *
* Copyright(c) 2022 Martins Andzans              *
*                                                *
* - In MFC Application                           *
* - - This Header MUST be Included after <afx.h> *
*       or other MFC Headers                     *
*                                                *
*************************************************/

#ifndef _WIN32
#error "This SDK Works Only on Windows"
#endif // !_WIN32

//===== LIBRARYS =====//
#pragma comment(lib, "msimg32.lib") // For GradientFill() Function
//====================//

#define WIN32_LEAN_AND_MEAN

//===== HEADERS ======//
#ifndef __AFX_H__
#include <tchar.h>
#include <Windows.h>
#endif // !__AFX_H__
#include <array>
#include <vector>
#include <math.h>
#ifndef _STDINT
#include <stdint.h>
#endif // !__STDINT
#include "ColorU.h"
//====================//

enum HATCHSTYLE : INT {
	HATCH_HORIZONTAL = HS_HORIZONTAL, // Horizontal hatch
	HATCH_VERTICAL = HS_VERTICAL, // Vertical hatch
	HATCH_CROSS = HS_CROSS, // Horizontal and vertical crosshatch
	HATCH_DIAGCROSS = HS_DIAGCROSS, // 45 - degree crosshatch
	HATCH_BDIAGONAL = HS_BDIAGONAL, // 45 - degree upward left - to - right hatch
	HATCH_FDIAGONAL = HS_FDIAGONAL // 45 - degree downward left - to - right hatch
};

// # unique_ptr RAII #
class GdiBrush {
public:

	// ===== PATTERN BRUSH =====
	explicit GdiBrush(HBITMAP hBitmap)
		: m_hBrush(CreatePatternBrush(hBitmap))
	{ /*...*/ }

	// ===== SOLID BRUSH =====
	explicit GdiBrush(const ColorU &Color)
		: m_hBrush(CreateSolidBrush(Color))
	{ /*...*/ }
	
	// ===== SOLID BRUSH =====
	explicit GdiBrush(HATCHSTYLE Hatch, const ColorU &Color)
		: m_hBrush(CreateHatchBrush(Hatch, Color))
	{ /*...*/ }

	GdiBrush(const GdiBrush &other) = delete;
	GdiBrush& operator=(const GdiBrush &other) = delete;

	HBRUSH get(void) const {
		return m_hBrush;
	}
	
	operator HBRUSH() const {
		return m_hBrush;
	}

	~GdiBrush() noexcept {
		DeleteObject(m_hBrush);
	}
	

private:
	
	HBRUSH m_hBrush;

};

class GdiBitmap {
public:

	GdiBitmap() {

		constexpr INT32 Width = 100, Height = 100;

		m_hBitmap = NULL;

		// CreateBitmap()

	}

	~GdiBitmap() {
		DeleteObject(m_hBitmap);
	}

private:

	HBITMAP m_hBitmap;

};

// # Geometry Vertex #
struct Vertex2I {
	
	Vertex2I() noexcept
		: x(0), y(0)
	{ /*...*/ }
	
	Vertex2I(int32_t _x, int32_t _y) noexcept
		: x(_x), y(_y)
	{ /*...*/ }

	// # Copy Constructor #
	Vertex2I(const Vertex2I &other) noexcept = default;
	// # Copy Assigment Operator #
	Vertex2I& operator=(const Vertex2I &other) noexcept = default;

	bool operator==(const Vertex2I &other) const noexcept {
		return x == other.x && y == other.y;
	}

	bool operator!=(const Vertex2I &other) const noexcept {
		return x != other.x || y != other.y;
	}

	int32_t x;
	int32_t y;

};


// # Geometry Size #
struct Size2I {

	Size2I() noexcept
		: width(0), height(0)
	{ /*...*/ }

	Size2I(int32_t _width, int32_t _height) noexcept
		: width(abs(_width)), height(abs(_height))
	{ /*...*/ }

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

	int32_t width;
	int32_t height;

};

// # Triangle Geometry #
struct Triangle3I {

	Triangle3I() noexcept = default;

	Triangle3I(const Vertex2I &_Vertex1, const Vertex2I &_Vertex2, const Vertex2I &_Vertex3) noexcept
		: Vertex1(_Vertex1), Vertex2(_Vertex2), Vertex3(_Vertex3)
	{ /*...*/ }

	// # Copy Constructor #
	Triangle3I(const Triangle3I &other) noexcept = default;
	// # Copy Assigment Operator #
	Triangle3I& operator=(const Triangle3I &other) noexcept = default;

	bool operator==(const Triangle3I &other) const noexcept {
		return Vertex1 == other.Vertex1 && Vertex2 == other.Vertex2 && Vertex3 == other.Vertex3;
	}

	bool operator!=(const Triangle3I &other) const noexcept {
		return Vertex1 != other.Vertex1 || Vertex2 != other.Vertex2 || Vertex3 != other.Vertex3;
	}

	Vertex2I Vertex1;
	Vertex2I Vertex2;
	Vertex2I Vertex3;

};

// # Image Load Properties #
enum class LoadMode : UINT16 {
	LOAD_FROM_FILE,
	LOAD_FROM_FILE_MONOCHROME,
	LOAD_FROM_RESOURCE,
	LOAD_FROM_RESOURCE_MONOCHROME
};

struct GdiPlus {

	static void _stdcall DrawLine(HDC hdc, const Vertex2I &lineBegin, const Vertex2I &lineEnd,
		const ColorU &strokeColor = ColorU::Enum::DarkBlue, UINT32 strokeWidth = 1U) {


		if (lineBegin == lineEnd) {
			
			ColorU PrevColor = SetDCBrushColor(hdc, strokeColor);

			UINT32 Radius = ceil(strokeWidth / 2.0F);
			FillEllipse(hdc, lineBegin, Radius, Radius, (HBRUSH)(GetStockObject(DC_BRUSH)));

			SetDCBrushColor(hdc, PrevColor);
			
		} else {
			
			HPEN StrokePen = CreatePen(PS_SOLID, strokeWidth, strokeColor);
			HGDIOBJ PrevPen = SelectObject(hdc, StrokePen);
			
			MoveToEx(hdc, lineBegin.x, lineBegin.y, nullptr);
			LineTo(hdc, lineEnd.x, lineEnd.y);
			
			SelectObject(hdc, PrevPen);
			DeleteObject(StrokePen);
		
		}


	}

	static void _stdcall DrawRectangle(HDC hdc, const Vertex2I &Position, const Size2I &Size,
		const ColorU &strokeColor = ColorU::Enum::DarkBlue, UINT32 strokeWidth = 1U) noexcept {

		HPEN StrokePen = CreatePen(PS_SOLID, strokeWidth, strokeColor);
		HGDIOBJ PrevPen = SelectObject(hdc, StrokePen);
		
		POINT PointBuffer[] = {
			{ Position.x, Position.y },
			{ Position.x + Size.width, Position.y },
			{ Position.x + Size.width, Position.y + Size.height },
			{ Position.x, Position.y + Size.height }
		};

		BYTE PointTypes[] = {
			PT_MOVETO,
			PT_LINETO,
			PT_LINETO,
			PT_LINETO | PT_CLOSEFIGURE
		};

		PolyDraw(hdc, PointBuffer, PointTypes, ARRAYSIZE(PointBuffer));

		SelectObject(hdc, PrevPen);
		DeleteObject(StrokePen);

	}

	static void _stdcall DrawEllipse(HDC hdc, const Vertex2I &CenterPoint, UINT32 RadiusX, UINT32 RadiusY,
		const ColorU &strokeColor = ColorU::Enum::DarkBlue, UINT32 strokeWidth = 1U) noexcept {

		HPEN StrokePen = CreatePen(PS_SOLID, strokeWidth, strokeColor);
		HGDIOBJ PrevPen = SelectObject(hdc, StrokePen);

		ArcTo(hdc, CenterPoint.x - RadiusX, CenterPoint.y - RadiusY, CenterPoint.x + RadiusX, CenterPoint.y + RadiusY,
			CenterPoint.x + RadiusX, CenterPoint.y, CenterPoint.x + RadiusX, CenterPoint.y);

		SelectObject(hdc, PrevPen);
		DeleteObject(StrokePen);
		
	}

	static void _stdcall DrawBiezerCurve(HDC, Vertex2I, Vertex2I, Vertex2I, Vertex2I, const ColorU &strokeColor, UINT32 strokeWidth) {

		OutputDebugString(_T("Function Is Not Implemented Yet!"));
		// TODO: Implement Function

	}

	static void _stdcall DrawTriangle(HDC hdc, const Triangle3I &Triangle,
		const ColorU &strokeColor = ColorU::Enum::DarkBlue, UINT32 strokeWidth = 1U) noexcept {

		HPEN StrokePen = CreatePen(PS_SOLID, strokeWidth, strokeColor);
		HGDIOBJ PrevPen = SelectObject(hdc, StrokePen);

		POINT PointBuffer[] = {
			{ Triangle.Vertex1.x, Triangle.Vertex1.y },
			{ Triangle.Vertex2.x, Triangle.Vertex2.y },
			{ Triangle.Vertex3.x, Triangle.Vertex3.y }
		};

		BYTE PointTypes[] = {
			PT_MOVETO,
			PT_LINETO,
			PT_LINETO | PT_CLOSEFIGURE
		};

		PolyDraw(hdc, PointBuffer, PointTypes, ARRAYSIZE(PointBuffer));

		SelectObject(hdc, PrevPen);
		DeleteObject(StrokePen);

	}

	template<size_t ArraySize>
	static void _stdcall DrawGeometry(HDC hdc, const std::array<Vertex2I, ArraySize> &VertexBuffer,
		const ColorU &strokeColor = ColorU::Enum::DarkBlue, UINT32 strokeWidth = 1U) noexcept {
		
		static_assert(ArraySize >= 1, "At Least ONE Vertice is Required");

		HPEN StrokePen = CreatePen(PS_SOLID, strokeWidth, strokeColor);
		HGDIOBJ PrevPen = SelectObject(hdc, StrokePen);

		switch (VertexBuffer.size()) {
		case 1U: // Dot

			SetPixelV(hdc, VertexBuffer[0].x, VertexBuffer[0].y, strokeColor);
			break;

		case 2U: // Line

			MoveToEx(hdc, VertexBuffer[0].x, VertexBuffer[0].y, nullptr);
			LineTo(hdc, VertexBuffer[1].x, VertexBuffer[1].y);
			break;

		default: // Other Geometry

			POINT PointBuffer[VertexBuffer.size()] = { 0 };
			BYTE PointTypes[VertexBuffer.size()] = { 0 };

			for (size_t i = 0; i < ARRAYSIZE(PointBuffer); i++) {

				PointBuffer[i].x = VertexBuffer[i].x;
				PointBuffer[i].y = VertexBuffer[i].y;

				if (i == 0) {
					PointTypes[i] = PT_MOVETO;
				} else if (i == ARRAYSIZE(PointBuffer) - 1) {
					PointTypes[i] = PT_LINETO | PT_CLOSEFIGURE;
				} else {
					PointTypes[i] = PT_LINETO;
				}

			}

			PolyDraw(hdc, PointBuffer, PointTypes, ARRAYSIZE(PointBuffer));
			
			break;

		}

		SelectObject(hdc, PrevPen);
		DeleteObject(StrokePen);

	}

	static void _stdcall FillRectangle(HDC hdc, const Vertex2I &Position, const Size2I &Size, HBRUSH hBrush) noexcept {

		HPEN InvisiblePen = (HPEN)(GetStockObject(NULL_PEN));

		HGDIOBJ PrevPen = SelectObject(hdc, InvisiblePen);
		HGDIOBJ PrevBrush = SelectObject(hdc, hBrush);

		POINT PointBuffer[] = {
			{ Position.x, Position.y },
			{ Position.x + Size.width, Position.y },
			{ Position.x + Size.width, Position.y + Size.height },
			{ Position.x, Position.y + Size.height }
		};

		Polygon(hdc, PointBuffer, ARRAYSIZE(PointBuffer));
		
		SelectObject(hdc, PrevPen);
		SelectObject(hdc, PrevBrush);
		
	}

	static void _stdcall FillEllipse(HDC hdc, const Vertex2I &CenterPoint, UINT32 RadiusX, UINT32 RadiusY, HBRUSH hBrush) noexcept {

		HPEN InvisiblePen = (HPEN)(GetStockObject(NULL_PEN));

		HGDIOBJ PrevPen = SelectObject(hdc, InvisiblePen);
		HGDIOBJ PrevBrush = SelectObject(hdc, hBrush);

		Ellipse(hdc, CenterPoint.x - RadiusX - 1, CenterPoint.y - RadiusY - 1, CenterPoint.x + RadiusX + 1, CenterPoint.y + RadiusY + 1);

		SelectObject(hdc, PrevPen);
		SelectObject(hdc, PrevBrush);

	}

	static void _stdcall FillTriangle(HDC hdc, const Triangle3I &Triangle, HBRUSH hBrush) noexcept {

		HPEN InvisiblePen = (HPEN)(GetStockObject(NULL_PEN));

		HGDIOBJ PrevPen = SelectObject(hdc, InvisiblePen);
		HGDIOBJ PrevBrush = SelectObject(hdc, hBrush);

		POINT PointBuffer[] = {
			{ Triangle.Vertex1.x, Triangle.Vertex1.y },
			{ Triangle.Vertex2.x, Triangle.Vertex2.y },
			{ Triangle.Vertex3.x, Triangle.Vertex3.y }
		};

		Polygon(hdc, PointBuffer, ARRAYSIZE(PointBuffer));

		SelectObject(hdc, PrevPen);
		SelectObject(hdc, PrevBrush);

	}

	template<size_t ArraySize>
	static void _stdcall FillGeometry(HDC hdc, const std::array<Vertex2I, ArraySize> &VertexBuffer, HBRUSH hBrush) noexcept {

		static_assert(ArraySize >= 2, "At Least TWO Vertices is Required");

		HPEN InvisiblePen = (HPEN)(GetStockObject(NULL_PEN));

		HGDIOBJ PrevPen = SelectObject(hdc, InvisiblePen);
		HGDIOBJ PrevBrush = SelectObject(hdc, hBrush);

		INT32 PrevMode = SetPolyFillMode(hdc, WINDING);
		
		POINT PointBuffer[VertexBuffer.size()] = {0};
		
		for (size_t i = 0; i < VertexBuffer.size(); i++) {
			PointBuffer[i].x = VertexBuffer[i].x;
			PointBuffer[i].y = VertexBuffer[i].y;
		}

		Polygon(hdc, PointBuffer, ARRAYSIZE(PointBuffer));
		
		SetPolyFillMode(hdc, PrevMode);

		SelectObject(hdc, PrevPen);
		SelectObject(hdc, PrevBrush);

	}

	static Size2I _stdcall DrawTextT(_In_ HDC hdc, _In_ const Vertex2I &Position, _In_ const Size2I &Size,
		_In_z_ LPCTSTR Text, _In_ HFONT TextFont, _In_ const ColorU &TextColor = ColorU::Enum::DarkBlue) {

		ColorU PrevColor = SetTextColor(hdc, TextColor);
		HGDIOBJ PrevFont = SelectObject(hdc, TextFont);

		SIZE szTextSize = { 0 };
		INT TextLength = (INT)(_tcslen(Text));

		//do {
			GetTextExtentPoint32(hdc, Text, TextLength, &szTextSize);
		//} while (szTextSize.cx > Size.width);
		
		DrawState(hdc, NULL, NULL, (LPARAM)(Text), (WPARAM)(TextLength), Position.x, Position.y, Size.width, Size.height, DST_TEXT | DT_END_ELLIPSIS);

		//TextOut(hdc, Position.x, Position.y, Text, TextLength);

		SetTextColor(hdc, PrevColor);
		SelectObject(hdc, PrevFont);

		return { szTextSize.cx, szTextSize.cy };

	}

	/// <summary>This Function Draw Image From File with ".bmp" Extension or Application Resource</summary>
	/// <param name="FileName">More Information is Located in "Mode" Parameter Anotation</param>
	/// <param name="Mode"><para>LOAD_FROM_FILE - Load Image From File with ".bmp" Extension</para>
	/// <para>LOAD_FROM_RESOURCE - Load Image From Resource - Use "MAKEINTRESOURCE" Macro OR Prefix Resource ID with "#"</para>
	/// <para>MONOCHROME - Load Monochrome Image (Black - White)</para></param>
	/// <returns><para>If Succeded Returns TRUE</para><para>If Failed Returns FALSE</para></returns>
	static _Success_(return == 0) DWORD _stdcall DrawImageT(_In_ HDC hdc, _In_z_ LPCTSTR FileName, _In_ LoadMode Mode,
		_In_ const Vertex2I &Position, _In_ const Size2I &Size) noexcept {

 		HBITMAP ImageBitmap = NULL;
		HINSTANCE hInstance = GetModuleHandle(NULL);
		
		switch (Mode) {
		//--------------------------------------------------------------------------------------------------
		// Load Image From File
		case LoadMode::LOAD_FROM_FILE:
			ImageBitmap = (HBITMAP)(LoadImage(NULL, FileName, IMAGE_BITMAP, Size.width, Size.height, LR_LOADFROMFILE | LR_VGACOLOR));
			break;
		case LoadMode::LOAD_FROM_FILE_MONOCHROME:
			ImageBitmap = (HBITMAP)(LoadImage(NULL, FileName, IMAGE_BITMAP, Size.width, Size.height, LR_LOADFROMFILE | LR_MONOCHROME));
			break;
		//--------------------------------------------------------------------------------------------------
		// Load Image From Resource
		case LoadMode::LOAD_FROM_RESOURCE:
			ImageBitmap = (HBITMAP)(LoadImage(hInstance, FileName, IMAGE_BITMAP, Size.width, Size.height, LR_VGACOLOR));
			break;
		case LoadMode::LOAD_FROM_RESOURCE_MONOCHROME:
			ImageBitmap = (HBITMAP)(LoadImage(hInstance, FileName, IMAGE_BITMAP, Size.width, Size.height, LR_MONOCHROME));
			break;
		//--------------------------------------------------------------------------------------------------
		}

		if (ImageBitmap == NULL) {
			return GetLastError();
		}

		HDC BitmapDC = CreateCompatibleDC(hdc);

		//--------------------------------------------------------------------------------------------------
		// Draw Image
		HGDIOBJ PrevBitmap = SelectObject(BitmapDC, ImageBitmap);
		BitBlt(hdc, Position.x, Position.y, Size.width, Size.height, BitmapDC, 0, 0, SRCCOPY);
		SelectObject(BitmapDC, PrevBitmap);
		//--------------------------------------------------------------------------------------------------

		//--------------------------------------------------------------------------------------------------
		// Cleanup Resources
		DeleteDC(BitmapDC);
		DeleteObject(ImageBitmap);
		//--------------------------------------------------------------------------------------------------

		return 0;

	}

	template<size_t ArraySize>
	static void _stdcall FillGradientH(HDC hdc, const Vertex2I &Position, const Size2I &Size,
		const std::array<ColorU, ArraySize> &ColorCollection = { ColorU::Enum::DarkBlue, ColorU::Enum::LightBlue, ColorU::Enum::DarkBlue }) noexcept {

		static_assert(ArraySize >= 2, "At Least TWO Colors is Required");

		//--------------------------------------------------------------------------------------------------
		// Create Memory Device Context
		HDC MemoryDC = CreateCompatibleDC(hdc);
		HBITMAP Bitmap = CreateCompatibleBitmap(hdc, Size.width, Size.height);
		//--------------------------------------------------------------------------------------------------

		HGDIOBJ PrevBitmap = SelectObject(MemoryDC, Bitmap);

		const INT32 GradientWidth = Size.width / (ArraySize - 1); // * Gradient Width Between TWO Colors *

		INT32 GradientBeginX = 0; // * Gradient Starting Point *
		INT32 GradientEndX = GradientWidth; // * Gradient Ending Point *

		for (size_t i = 0; i < ArraySize - 1; i++) {
			
			// COLOR16 0x0000 - # (COLOR16)(0xFF) << 8 : 0xFF00 #

			TRIVERTEX Gradient[] = {
				#pragma warning(disable:4838)
				
				{
					GradientBeginX, 0,                                   // X, Y
					ColorCollection[i].GetXValue(ColorU::Value::R) << 8, // Red
					ColorCollection[i].GetXValue(ColorU::Value::G) << 8, // Green
					ColorCollection[i].GetXValue(ColorU::Value::B) << 8, // Blue
					0x0000                                               // Alpha
				},

				{
					GradientEndX, Size.height,                               // X, Y
					ColorCollection[i + 1].GetXValue(ColorU::Value::R) << 8, // Red
					ColorCollection[i + 1].GetXValue(ColorU::Value::G) << 8, // Green
					ColorCollection[i + 1].GetXValue(ColorU::Value::B) << 8, // Blue
					0x0000                                                   // Alpha
				}

				#pragma warning(default:4838)
			};
			
			GRADIENT_RECT GradientRect[] = { { 0, 1 } };

			GradientFill(MemoryDC, Gradient, ARRAYSIZE(Gradient), GradientRect, ARRAYSIZE(GradientRect), GRADIENT_FILL_RECT_H);

			GradientBeginX += GradientWidth;
			GradientEndX += GradientWidth;

		}

		BitBlt(hdc, Position.x, Position.y, Size.width, Size.height, MemoryDC, 0, 0, SRCCOPY);

		SelectObject(MemoryDC, PrevBitmap);
		
		//--------------------------------------------------------------------------------------------------
		// Cleanup Resources
		DeleteDC(MemoryDC);
		DeleteObject(Bitmap);
		//--------------------------------------------------------------------------------------------------

	}

	template<size_t ArraySize>
	static void _stdcall FillGradientV(HDC hdc, const Vertex2I &Position, const Size2I &Size,
		const std::array<ColorU, ArraySize> &ColorCollection = { ColorU::Enum::DarkBlue, ColorU::Enum::LightBlue, ColorU::Enum::DarkBlue }) {

		static_assert(ArraySize >= 2, "At Least TWO Colors is Required");

		//--------------------------------------------------------------------------------------------------
		// Create Memory Device Context
		HDC MemoryDC = CreateCompatibleDC(hdc);
		HBITMAP Bitmap = CreateCompatibleBitmap(hdc, Size.width, Size.height);
		//--------------------------------------------------------------------------------------------------

		HGDIOBJ PrevBitmap = SelectObject(MemoryDC, Bitmap);

		const INT32 GradientHeight = Size.height / (ArraySize - 1); // * Gradient Height Between TWO Colors *

		INT32 GradientBeginY = 0; // * Gradient Starting Point *
		INT32 GradientEndY = GradientHeight; // * Gradient Ending Point *

		for (size_t i = 0; i < ArraySize - 1; i++) {

			// COLOR16 0x0000 - # (COLOR16)(0xFF) << 8 : 0xFF00 #

			TRIVERTEX Gradient[] = {
				#pragma warning(disable:4838)

				{
					0, GradientBeginY,                                   // X, Y
					ColorCollection[i].GetXValue(ColorU::Value::R) << 8, // Red
					ColorCollection[i].GetXValue(ColorU::Value::G) << 8, // Green
					ColorCollection[i].GetXValue(ColorU::Value::B) << 8, // Blue
					0x0000                                               // Alpha
				},

				{
					Size.width, GradientEndY,                                // X, Y
					ColorCollection[i + 1].GetXValue(ColorU::Value::R) << 8, // Red
					ColorCollection[i + 1].GetXValue(ColorU::Value::G) << 8, // Green
					ColorCollection[i + 1].GetXValue(ColorU::Value::B) << 8, // Blue
					0x0000                                                   // Alpha
				}

				#pragma warning(default:4838)
			};

			GRADIENT_RECT GradientRect[] = { { 0, 1 } };

			GradientFill(MemoryDC, Gradient, ARRAYSIZE(Gradient), GradientRect, ARRAYSIZE(GradientRect), GRADIENT_FILL_RECT_V);

			GradientBeginY += GradientHeight;
			GradientEndY += GradientHeight;

		}

		BitBlt(hdc, Position.x, Position.y, Size.width, Size.height, MemoryDC, 0, 0, SRCCOPY);

		SelectObject(MemoryDC, PrevBitmap);

		//--------------------------------------------------------------------------------------------------
		// Cleanup Resources
		DeleteDC(MemoryDC);
		DeleteObject(Bitmap);
		//--------------------------------------------------------------------------------------------------

	}

};

#endif // !__GDI_PLUS_H__
