#ifndef __GDI_PLUS_H__
#define __GDI_PLUS_H__

/*****************************************
*                                        *
* Copyright(c) [2022] Martins Andzans    *
* Licensed Under [MIT License]           *
*                                        *
* - In MFC Application This Header       *
* - - Must Be Included After MFC Headers *
*                                        *
*****************************************/

#ifndef _WIN32
#error "This Header Works Only For Windows Applications"
#endif // !_WIN32

//===== LIBRARYS =====//
#pragma comment(lib, "msimg32.lib") // For GradientFill() Function
//====================//

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif // !WIN32_LEAN_AND_MEAN

//===== HEADERS ======//
#ifndef __AFX_H__
#include <Windows.h>
#endif // !__AFX_H__
#include <array>
#ifndef _STDINT
#include <stdint.h>
#endif // !__STDINT
#include "Windows/Types/GraphicsTypes.h"
#include "Windows/Types/ColorU.h"
//====================//

// # Image Load Properties #
enum class LoadMode : UINT16 {
	LOAD_FROM_FILE,
	LOAD_FROM_FILE_MONOCHROME,
	LOAD_FROM_RESOURCE,
	LOAD_FROM_RESOURCE_MONOCHROME
};

struct GdiPlus {

	static void _stdcall DrawLine(_In_opt_ HDC hdc, _In_ const Vertex2I &lineBegin, _In_ const Vertex2I &lineEnd,
		_In_ const Windows::ColorU &strokeColor = Windows::ColorU::Enum::DarkBlue, _In_ UINT32 strokeWidth = 1U) {

		if (hdc != NULL) {

			HPEN StrokePen = CreatePen(PS_SOLID, strokeWidth, strokeColor);
			HGDIOBJ PrevPen = SelectObject(hdc, StrokePen);

			MoveToEx(hdc, lineBegin.x, lineBegin.y, nullptr);
			LineTo(hdc, lineEnd.x, lineEnd.y);

			SelectObject(hdc, PrevPen);
			DeleteObject(StrokePen);

		}

	}

	static void _stdcall DrawRectangle(_In_opt_ HDC hdc, _In_ const Vertex2I &Location, _In_ const Size2I &Size,
		_In_ const Windows::ColorU &strokeColor = Windows::ColorU::Enum::DarkBlue, _In_ UINT32 strokeWidth = 1U) noexcept {

		if (hdc != NULL) {

			HPEN StrokePen = CreatePen(PS_SOLID, strokeWidth, strokeColor);
			HGDIOBJ PrevPen = SelectObject(hdc, StrokePen);

			static constexpr BYTE PointTypes[] = {
				PT_MOVETO,
				PT_LINETO,
				PT_LINETO,
				PT_LINETO | PT_CLOSEFIGURE
			};

			POINT PointBuffer[] = {
				{ Location.x, Location.y },
				{ Location.x + Size.width, Location.y },
				{ Location.x + Size.width, Location.y + Size.height },
				{ Location.x, Location.y + Size.height }
			};

			PolyDraw(hdc, PointBuffer, PointTypes, ARRAYSIZE(PointBuffer));

			SelectObject(hdc, PrevPen);
			DeleteObject(StrokePen);

		}

	}

	static void _stdcall DrawEllipse(_In_opt_ HDC hdc, _In_ const Vertex2I &CenterPoint, _In_ UINT32 RadiusX, _In_ UINT32 RadiusY,
		_In_ const Windows::ColorU &strokeColor = Windows::ColorU::Enum::DarkBlue, _In_ UINT32 strokeWidth = 1U) noexcept {

		if (hdc != NULL) {

			HPEN StrokePen = CreatePen(PS_SOLID, strokeWidth, strokeColor);
			HGDIOBJ PrevPen = SelectObject(hdc, StrokePen);

			ArcTo(hdc, CenterPoint.x - RadiusX, CenterPoint.y - RadiusY, CenterPoint.x + RadiusX, CenterPoint.y + RadiusY,
				CenterPoint.x + RadiusX, CenterPoint.y, CenterPoint.x + RadiusX, CenterPoint.y);

			SelectObject(hdc, PrevPen);
			DeleteObject(StrokePen);

		}

	}

	static void _stdcall DrawTriangle(_In_opt_ HDC hdc, _In_ const std::array<Vertex2I, 3> &Triangle,
		_In_ const Windows::ColorU &strokeColor = Windows::ColorU::Enum::DarkBlue, _In_ UINT32 strokeWidth = 1U) noexcept {

		if (hdc != NULL) {

			HPEN StrokePen = CreatePen(PS_SOLID, strokeWidth, strokeColor);
			HGDIOBJ PrevPen = SelectObject(hdc, StrokePen);

			static constexpr BYTE PointTypes[] = {
				PT_MOVETO,
				PT_LINETO,
				PT_LINETO | PT_CLOSEFIGURE
			};

			POINT PointBuffer[] = {
				{ Triangle[0].x, Triangle[0].y },
				{ Triangle[1].x, Triangle[1].y },
				{ Triangle[2].x, Triangle[2].y }
			};

			PolyDraw(hdc, PointBuffer, PointTypes, ARRAYSIZE(PointBuffer));

			SelectObject(hdc, PrevPen);
			DeleteObject(StrokePen);

		}

	}

	template<size_t ArraySize>
	static void _stdcall DrawGeometry(_In_opt_ HDC hdc, _In_ const std::array<Vertex2I, ArraySize> &VertexBuffer,
		_In_ const Windows::ColorU &strokeColor = Windows::ColorU::Enum::DarkBlue, _In_ UINT32 strokeWidth = 1U) noexcept {
		
		static_assert(ArraySize >= 1, "At Least ONE Vertice is Required");

		if (hdc != NULL) {

			HPEN StrokePen = CreatePen(PS_SOLID, strokeWidth, strokeColor);
			HGDIOBJ PrevPen = SelectObject(hdc, StrokePen);

			switch (ArraySize) {
			case 1U: // # Pixel #
				
				MoveToEx(hdc, VertexBuffer[0].x, VertexBuffer[0].y, nullptr);
				LineTo(hdc, VertexBuffer[0].x, VertexBuffer[0].y);
				break;
			
			case 2U: // # Line #
				
				MoveToEx(hdc, VertexBuffer[0].x, VertexBuffer[0].y, nullptr);
				LineTo(hdc, VertexBuffer[1].x, VertexBuffer[1].y);
				break;
			
			default: // # Other Geometry #

				BYTE PointTypes[ArraySize] = { 0 };
				POINT PointBuffer[ArraySize] = { 0 };

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

	}

	static void _stdcall FillRectangle(_In_opt_ HDC hdc, _In_ const Vertex2I &Location,
		_In_ const Size2I &Size, _In_ HBRUSH hBrush) noexcept {

		if (hdc != NULL) {

			HPEN InvisiblePen = (HPEN)(GetStockObject(NULL_PEN));

			HGDIOBJ PrevPen = SelectObject(hdc, InvisiblePen);
			HGDIOBJ PrevBrush = SelectObject(hdc, hBrush);

			POINT PointBuffer[] = {
				{ Location.x, Location.y },
				{ Location.x + Size.width, Location.y },
				{ Location.x + Size.width, Location.y + Size.height },
				{ Location.x, Location.y + Size.height }
			};

			Polygon(hdc, PointBuffer, ARRAYSIZE(PointBuffer));

			SelectObject(hdc, PrevPen);
			SelectObject(hdc, PrevBrush);

		}
		
	}

	static void _stdcall FillEllipse(_In_opt_ HDC hdc, _In_ const Vertex2I &CenterPoint,
		_In_ UINT32 RadiusX, _In_ UINT32 RadiusY, _In_ HBRUSH hBrush) noexcept {

		if (hdc != NULL) {

			HPEN InvisiblePen = (HPEN)(GetStockObject(NULL_PEN));

			HGDIOBJ PrevPen = SelectObject(hdc, InvisiblePen);
			HGDIOBJ PrevBrush = SelectObject(hdc, hBrush);

			Ellipse(hdc, CenterPoint.x - RadiusX - 1, CenterPoint.y - RadiusY - 1,
				CenterPoint.x + RadiusX + 1, CenterPoint.y + RadiusY + 1);

			SelectObject(hdc, PrevPen);
			SelectObject(hdc, PrevBrush);

		}

	}

	static void _stdcall FillTriangle(_In_opt_ HDC hdc,
		_In_ const std::array<Vertex2I, 3> &Triangle, _In_ HBRUSH hBrush) noexcept {

		if (hdc != NULL) {

			HPEN InvisiblePen = (HPEN)(GetStockObject(NULL_PEN));

			HGDIOBJ PrevPen = SelectObject(hdc, InvisiblePen);
			HGDIOBJ PrevBrush = SelectObject(hdc, hBrush);

			POINT PointBuffer[] = {
				{ Triangle[0].x, Triangle[0].y },
				{ Triangle[1].x, Triangle[1].y },
				{ Triangle[2].x, Triangle[2].y }
			};

			Polygon(hdc, PointBuffer, ARRAYSIZE(PointBuffer));

			SelectObject(hdc, PrevPen);
			SelectObject(hdc, PrevBrush);

		}

	}

	template<size_t ArraySize>
	static void _stdcall FillGeometry(_In_opt_ HDC hdc,
		_In_ const std::array<Vertex2I, ArraySize> &VertexBuffer, _In_ HBRUSH hBrush) noexcept {

		if (hdc != NULL) {

			static_assert(ArraySize >= 3, "At Least THREE Vertices is Required");

			HPEN InvisiblePen = (HPEN)(GetStockObject(NULL_PEN));

			HGDIOBJ PrevPen = SelectObject(hdc, InvisiblePen);
			HGDIOBJ PrevBrush = SelectObject(hdc, hBrush);

			INT32 PrevMode = SetPolyFillMode(hdc, WINDING);

			POINT PointBuffer[ArraySize] = { 0 };

			for (size_t i = 0; i < ArraySize; i++) {
				PointBuffer[i].x = VertexBuffer[i].x;
				PointBuffer[i].y = VertexBuffer[i].y;
			}

			Polygon(hdc, PointBuffer, ARRAYSIZE(PointBuffer));

			SetPolyFillMode(hdc, PrevMode);

			SelectObject(hdc, PrevPen);
			SelectObject(hdc, PrevBrush);

		}

	}

	#pragma push_macro("DrawText")
	#undef DrawText

	static void _stdcall DrawText(_In_ HDC hdc, _In_ const Vertex2I &Location, _In_ const Size2I &Size,
		_In_ const std::string &String, _In_ HFONT TextFont, _In_ const Windows::ColorU &TextColor = Windows::ColorU::Enum::DarkBlue) {

		COLORREF PrevColor = SetTextColor(hdc, TextColor);
		HGDIOBJ PrevFont = SelectObject(hdc, TextFont);

		RECT rcClipingRect = {
			Location.x,
			Location.y,
			Location.x + Size.width,
			Location.y + Size.height
		};

		static constexpr UINT Flags = DT_LEFT | DT_SINGLELINE | DT_END_ELLIPSIS;
		DrawTextA(hdc, String.c_str(), (INT)(String.length()), &rcClipingRect, Flags);

		SetTextColor(hdc, PrevColor);
		SelectObject(hdc, PrevFont);

	}

	static void _stdcall DrawText(_In_ HDC hdc, _In_ const Vertex2I &Location, _In_ const Size2I &Size,
		_In_ const std::wstring &String, _In_ HFONT TextFont, _In_ const Windows::ColorU &TextColor = Windows::ColorU::Enum::DarkBlue) {

		COLORREF PrevColor = SetTextColor(hdc, TextColor);
		HGDIOBJ PrevFont = SelectObject(hdc, TextFont);

		RECT rcClipingRect = {
			Location.x,
			Location.y,
			Location.x + Size.width,
			Location.y + Size.height
		};
		
		static constexpr UINT Flags = DT_LEFT | DT_SINGLELINE | DT_END_ELLIPSIS;
		DrawTextW(hdc, String.c_str(), (INT)(String.length()), &rcClipingRect, Flags);

		SetTextColor(hdc, PrevColor);
		SelectObject(hdc, PrevFont);

	}

	/// <summary>This Function Draw Image From File with ".bmp" Extension or Application Resource</summary>
	/// <param name="FileName">More Information is Located in "Mode" Parameter Anotation</param>
	/// <param name="Mode"><para>LOAD_FROM_FILE - Load Image From File with ".bmp" Extension</para>
	/// <para>LOAD_FROM_RESOURCE - Load Image From Resource - Use "MAKEINTRESOURCE" Macro OR Prefix Resource ID with "#"</para>
	/// <para>MONOCHROME - Load Monochrome Image (Black - White)</para></param>
	/// <returns>
	/// <para>If Succeded Returns TRUE</para>
	/// <para>If Failed Returns FALSE</para>
	/// </returns>
	static _Success_(return == 0) DWORD WINAPI DrawImageT(_In_ HDC hdc, _In_z_ LPCTSTR FileName, _In_ LoadMode Mode,
		_In_ const Vertex2I &Location, _In_ const Size2I &Size) noexcept {

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
		BitBlt(hdc, Location.x, Location.y, Size.width, Size.height, BitmapDC, 0, 0, SRCCOPY);
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
	static void WINAPI FillGradientH(HDC hdc, const Vertex2I &Location, const Size2I &Size,
		const std::array<Windows::ColorU, ArraySize> &ColorCollection) noexcept {

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
					GradientBeginX, 0,                                       // X, Y
					ColorCollection[i].GetXValue(Windows::ColorU::Component::R) << 8, // Red
					ColorCollection[i].GetXValue(Windows::ColorU::Component::G) << 8, // Green
					ColorCollection[i].GetXValue(Windows::ColorU::Component::B) << 8, // Blue
					0x0000                                                   // Alpha
				},

				{
					GradientEndX, Size.height,                                   // X, Y
					ColorCollection[i + 1].GetXValue(Windows::ColorU::Component::R) << 8, // Red
					ColorCollection[i + 1].GetXValue(Windows::ColorU::Component::G) << 8, // Green
					ColorCollection[i + 1].GetXValue(Windows::ColorU::Component::B) << 8, // Blue
					0x0000                                                       // Alpha
				}

				#pragma warning(default:4838)
			};
			
			GRADIENT_RECT GradientRect[] = { { 0, 1 } };

			GradientFill(MemoryDC, Gradient, ARRAYSIZE(Gradient), GradientRect, ARRAYSIZE(GradientRect), GRADIENT_FILL_RECT_H);

			GradientBeginX += GradientWidth;
			GradientEndX += GradientWidth;

		}

		BitBlt(hdc, Location.x, Location.y, Size.width, Size.height, MemoryDC, 0, 0, SRCCOPY);

		SelectObject(MemoryDC, PrevBitmap);
		
		//--------------------------------------------------------------------------------------------------
		// Cleanup Resources
		DeleteDC(MemoryDC);
		DeleteObject(Bitmap);
		//--------------------------------------------------------------------------------------------------

	}

	template<size_t ArraySize>
	static void WINAPI FillGradientV(HDC hdc, const Vertex2I &Location, const Size2I &Size,
		const std::array<Windows::ColorU, ArraySize> &ColorCollection) {

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
					0, GradientBeginY,                                       // X, Y
					ColorCollection[i].GetXValue(Windows::ColorU::Component::R) << 8, // Red
					ColorCollection[i].GetXValue(Windows::ColorU::Component::G) << 8, // Green
					ColorCollection[i].GetXValue(Windows::ColorU::Component::B) << 8, // Blue
					0x0000                                                   // Alpha
				},

				{
					Size.width, GradientEndY,                                    // X, Y
					ColorCollection[i + 1].GetXValue(Windows::ColorU::Component::R) << 8, // Red
					ColorCollection[i + 1].GetXValue(Windows::ColorU::Component::G) << 8, // Green
					ColorCollection[i + 1].GetXValue(Windows::ColorU::Component::B) << 8, // Blue
					0x0000                                                       // Alpha
				}

				#pragma warning(default:4838)
			};

			GRADIENT_RECT GradientRect[] = { { 0, 1 } };

			GradientFill(MemoryDC, Gradient, ARRAYSIZE(Gradient), GradientRect, ARRAYSIZE(GradientRect), GRADIENT_FILL_RECT_V);

			GradientBeginY += GradientHeight;
			GradientEndY += GradientHeight;

		}

		BitBlt(hdc, Location.x, Location.y, Size.width, Size.height, MemoryDC, 0, 0, SRCCOPY);

		SelectObject(MemoryDC, PrevBitmap);

		//--------------------------------------------------------------------------------------------------
		// Cleanup Resources
		DeleteDC(MemoryDC);
		DeleteObject(Bitmap);
		//--------------------------------------------------------------------------------------------------

	}

};

#endif // !__GDI_PLUS_H__
