#pragma comment(linker,"\"/manifestdependency:type='win32' \
name='Microsoft.Windows.Common-Controls' version='6.0.0.0' \
processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

#include <stdexcept>
#include <afxwin.h>
#include <afxcmn.h>
#include <memory>
#include <vector>
#include <string>

#include "gdiplus.h"
#include "Algorithms.h"

#include "Resource.h"

template<typename PixelData>
class FrameBuffer {
public:
	
	explicit FrameBuffer() {

		m_Width = 2, m_Height = 2;
		m_FrameBuffer = std::make_unique<PixelData[]>(m_Width * m_Height);

	}
	
	explicit FrameBuffer(int32_t width, int32_t height) {

		m_Width = width, m_Height = height;
		m_FrameBuffer = std::make_unique<PixelData[]>(m_Width * m_Height);
		
	}

	~FrameBuffer() = default;

	void SetPixel(int32_t x, int32_t y, const PixelData &pixelData) noexcept {

		if (x >= 0 && y >= 0 && x < m_Width && y < m_Height) {
			m_FrameBuffer[y * m_Width + x] = pixelData;
		}

	}
	
	PixelData GetPixel(int32_t x, int32_t y) const {

		if (x >= 0 && y >= 0 && x < m_Width && y < m_Height) {
			return m_FrameBuffer[y * m_Width + x];
		} else {
			throw (std::out_of_range("Pixel Outside Frame Buffer"));
		}

	}
	
private:

	int32_t m_Width, m_Height;
	std::unique_ptr<PixelData[]> m_FrameBuffer;

};

class IPlayer {

	enum class PlayFrom { Start, Point };

public:

	virtual void Play(PlayFrom PlayParam, uint32_t TimeStamp) = 0;
	virtual void Pause() = 0;
	virtual void Resume() = 0;
	virtual void Stop() = 0;

};

#pragma push_macro("MessageBox")
#undef MessageBox

namespace WinGui {
	
	struct MessageBox {
		
		// # Message Box Icons #
		enum class Icon : INT16 { Error, Warning, Information, Question };

		enum class Buttons : INT16 { Ok, OkCancel, RetryCancel, YesNo, YesNoCancel, AbortRetryIgnore };
		enum class DefaultButton : INT16 { Button1, Button2, Button3 };

		// # Message Box Results #
		enum class Result : INT16 { Ok, Cancel, Yes, No, Abort, Retry, Ignore, Error };

		//----------------------------------------
		// Constructors

		MessageBox() 
			: m_hWndParent(HWND_DESKTOP), m_Caption(L"Message Box"), m_Message(L"This is Message Box"),
			m_Icon(Icon::Information), m_Buttons(Buttons::Ok), m_DefaultButton(DefaultButton::Button1)
		{ /*...*/ }

		MessageBox(const std::wstring &Message)
			: m_hWndParent(HWND_DESKTOP), m_Caption(L"Message Box"), m_Message(Message),
			m_Icon(Icon::Information), m_Buttons(Buttons::Ok), m_DefaultButton(DefaultButton::Button1)
		{ /*...*/ }

		MessageBox(HWND hWndParent, const std::wstring &Message)
			: m_hWndParent(hWndParent), m_Caption(L"Message Box"), m_Message(Message),
			m_Icon(Icon::Information), m_Buttons(Buttons::Ok), m_DefaultButton(DefaultButton::Button1)
		{ /*...*/ }

		MessageBox(HWND hWndParent, const std::wstring &Message, Buttons MsgBoxButons)
			: m_hWndParent(hWndParent), m_Caption(L"Message Box"), m_Message(Message),
			m_Icon(Icon::Information), m_Buttons(MsgBoxButons), m_DefaultButton(DefaultButton::Button1)
		{ /*...*/ }

		//----------------------------------------

		void SetMessage(const std::wstring &NewMessage) {
			m_Message = NewMessage;
		}

		Result Show() {

			MSGBOXPARAMSW MsgBox = { 0 };
			MsgBox.cbSize = sizeof(MSGBOXPARAMSW);



			MessageBoxIndirectW(&MsgBox);

		}
		
		static Result WINAPI Show(HWND hWndOwner, const std::wstring &Title, const std::wstring &Message,
			Icon Icon, Buttons Btns, DefaultButton DefBtn) {

			MSGBOXPARAMSW MsgBox = { 0 };
			MsgBox.cbSize = sizeof(MSGBOXPARAMSW);
			
			MsgBox.dwLanguageId = MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT);
			
			MsgBox.hwndOwner = hWndOwner;
			MsgBox.lpszCaption = Title.c_str();
			MsgBox.lpszText = Message.c_str();

			DWORD MsgBoxStyle = 0UL;

			switch (Icon) {
			case Icon::Error:
				MsgBoxStyle |= MB_ICONERROR; break;
			case Icon::Warning:
				MsgBoxStyle |= MB_ICONWARNING; break;
			case Icon::Information:
				MsgBoxStyle |= MB_ICONINFORMATION; break;
			case Icon::Question:
				MsgBoxStyle |= MB_ICONQUESTION; break;
			}

			switch (Btns) {
			case Buttons::Ok:
				MsgBoxStyle |= MB_OK; break;
			case Buttons::OkCancel:
				MsgBoxStyle |= MB_OKCANCEL; break;
			case Buttons::RetryCancel:
				MsgBoxStyle |= MB_RETRYCANCEL; break;
			case Buttons::YesNo:
				MsgBoxStyle |= MB_YESNO; break;
			case Buttons::YesNoCancel:
				MsgBoxStyle |= MB_YESNOCANCEL; break;
			case Buttons::AbortRetryIgnore:
				MsgBoxStyle |= MB_ABORTRETRYIGNORE; break;
			}

			INT result = MessageBoxIndirectW(&MsgBox);

			switch (result) {
			case IDOK:
				return Result::Ok;
			case IDCANCEL:
				return Result::Cancel;
			case IDYES:
				return Result::Yes;
			case IDNO:
				return Result::No;
			case IDABORT:
				return Result::Abort;
			case IDRETRY:
				return Result::Retry;
			case IDIGNORE:
				return Result::Ignore;
			default:
				return Result::Error;
			}


		}

		private:

			HWND m_hWndParent;

			std::wstring m_Caption;
			std::wstring m_Message;
			
			Icon m_Icon;
			Buttons m_Buttons;
			DefaultButton m_DefaultButton;

	};

}

#pragma pop_macro("MessageBox")

struct Gate {

	Gate()
		: rcTop(), rcBottom(), Scored(false)
	{ /*...*/ }

	Gate(const Rect4I &_rcTop, const Rect4I &_rcBottom, bool _Scored)
		: rcTop(_rcTop), rcBottom(_rcBottom), Scored(_Scored)
	{ /*...*/ }

	Rect4I rcTop;
	Rect4I rcBottom;
	bool Scored;

};

struct Player {

	enum State {
		UP,
		DOWN
	};
	
	Player()
		: rcPlayer(), Fly(DOWN)
	{ /*...*/ }

	Player(const Rect4I &_rcPlayer, State _Up)
		: rcPlayer(_rcPlayer), Fly(_Up)
	{ /*...*/ }

	Rect4I rcPlayer;
	State Fly;

};

class FlappyBird {
public:

	FlappyBird(const CSize &CanvasSize) {
		m_CanvasSize = CanvasSize;
		Reset();
	}
	
	void Update(UINT32 Frame) {



	}

	void Redraw(HDC PaintDC, const CRect &ClientRect) {

		HDC MemoryDC = CreateCompatibleDC(PaintDC);
		HBITMAP Bitmap = CreateCompatibleBitmap(PaintDC, m_CanvasSize.cx, m_CanvasSize.cy);

		HGDIOBJ PrevBitmap = SelectObject(MemoryDC, Bitmap);

		if (!m_GameOver) {

			

		} else {



		}

		StretchBlt(PaintDC, 0, 0, ClientRect.Width(), ClientRect.Height(), MemoryDC, 0, 0, m_CanvasSize.cx, m_CanvasSize.cy, SRCCOPY);

		SelectObject(MemoryDC, PrevBitmap);
		
		DeleteDC(MemoryDC);
		DeleteObject(Bitmap);

	}

private:

	void Reset(void) {



	}


private:

	Player m_Player;
	std::vector<Gate> m_Gates;

	bool m_GameOver;
	CSize m_CanvasSize;

};

class MainWindow : public CFrameWnd {
public:

	MainWindow(HICON hWindowIcon) : CFrameWnd() {

		// Window Style
		constexpr DWORD Style = WS_OVERLAPPEDWINDOW;
		// Window Client Area Size
		constexpr INT32 Width = 1280, Height = 800;
		
		// Window Position In Center of Screen
		INT32 X = GetSystemMetrics(SM_CXSCREEN) / 2 - Width / 2;
		INT32 Y = GetSystemMetrics(SM_CYSCREEN) / 2 - Height / 2;
		
		// Window Rectangle
		CRect WindowRect = { X, Y, X + Width, Y + Height };
		AdjustWindowRect(&WindowRect, Style, NULL);

		m_szMinWindowSize = { WindowRect.Width(), WindowRect.Height()};
		
		// Create Window
		if (!this->Create(NULL, _T("Flappy Bird"), Style, WindowRect)) {
			throw std::runtime_error(Algorithms::GetWINAPIErrorMessage(GetLastError()));
		}

		this->SetIcon(hWindowIcon, FALSE); // Small Icon
		this->SetIcon(hWindowIcon, TRUE); // Big Icon

		// Set Canvas Size
		m_CanvasSize.width = 400;
		m_CanvasSize.height = 200;

		static constexpr INT16 PlayerWidth = 40, PlayerHeight = 20;

		m_Player.rcPlayer.Location.x = 10;
		m_Player.rcPlayer.Location.y = m_CanvasSize.height / 2 - PlayerHeight / 2;
		m_Player.rcPlayer.Size.width = PlayerWidth;
		m_Player.rcPlayer.Size.height = PlayerHeight;

		Frame = 0;
		m_Score = 0;

		// Set Update Timer
		constexpr UINT FPS = 1000 / 60; // * 60 FPS *
		this->SetTimer(0xFF, FPS, NULL);
		
	}

protected:

	afx_msg INT OnCreate(LPCREATESTRUCT lpCreateStruct) {

		if (!m_Font_SegoeUI.CreateFontW(40, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, ANSI_CHARSET,
			OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY, VARIABLE_PITCH, _T("SegoeUI"))) {	
			return -1;
		}

		if (!m_StaticScore.Create(_T("Score Count: X"), WS_CHILD | WS_VISIBLE | SS_OWNERDRAW, { 0, 0, 0, 0 }, this)) {
			return -1;
		}

		return 0;

	}

	afx_msg void OnDrawItem(INT nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct) {

		CRect ClientRect;
		this->GetClientRect(&ClientRect);

		if (m_StaticScore.GetSafeHwnd() == lpDrawItemStruct->hwndItem) {

			// Get Static Box Text
			CString StaticText;
			m_StaticScore.GetWindowTextW(StaticText);

			if (StaticText.Compare(_T("")) == 0) {

				m_StaticScore.SetWindowPos(NULL, 0, 0, 0, 0, SWP_HIDEWINDOW);

			} else {

				CDC ItemDC;
				ItemDC.Attach(lpDrawItemStruct->hDC);

				INT PrevBKMode = ItemDC.SetBkMode(TRANSPARENT);
				HGDIOBJ PrevFont = ItemDC.SelectObject(m_Font_SegoeUI);
				COLORREF PrevColor = ItemDC.SetTextColor(ColorU::Enum::Yellow);

				CSize TextSize;
				GetTextExtentPoint32(ItemDC, StaticText, StaticText.GetLength(), &TextSize);

				constexpr INT16 Padding = 6;
				m_StaticScore.SetWindowPos(NULL, ClientRect.right / 2 - TextSize.cx / 2, ClientRect.top + Padding,
					TextSize.cx, TextSize.cy, SWP_SHOWWINDOW);

				ItemDC.TextOutW(0, 0, StaticText);
				
				ItemDC.SetBkMode(PrevBKMode);
				ItemDC.SelectObject(PrevFont);
				ItemDC.SetTextColor(PrevColor);

				ItemDC.Detach();

			}

		}

	}

	afx_msg void OnTimer(UINT_PTR nIDEvent) {

		constexpr INT16 Space = 200, GateWidth = 20, HoleHeight = 60;

		// Spawn Gates
		if (m_Gates.size() <= 6) {
			
			INT32 HoleLocation = rand() % (m_CanvasSize.height - HoleHeight) - 10;
			
			if (m_Gates.empty()) {

				m_Gates.push_back({
					Rect4I(Vertex2I(m_CanvasSize.width + 20, 0), Size2I(GateWidth, HoleLocation)),
					Rect4I(Vertex2I(m_CanvasSize.width + 20, HoleLocation + HoleHeight),
						Size2I(GateWidth, m_CanvasSize.height - HoleLocation - HoleHeight)),
					false
				});

			} else {

				const Gate &last = m_Gates.back();

				m_Gates.push_back({
					Rect4I(Vertex2I(last.rcTop.Location.x + last.rcTop.Size.width + Space, 0), Size2I(GateWidth, HoleLocation)),
					Rect4I(Vertex2I(last.rcBottom.Location.x + last.rcTop.Size.width + Space, HoleLocation + HoleHeight),
						Size2I(GateWidth, m_CanvasSize.height - HoleLocation - HoleHeight)),
					false
				});

			}

		}

		// Collision Detection
		for (Gate &Gate : m_Gates) {

			Rect4I rcScoreTriger(Gate.rcTop.Location.x, Gate.rcTop.Location.y + Gate.rcTop.Size.height,
				Gate.rcTop.Size.width, m_CanvasSize.height - Gate.rcTop.Size.width - Gate.rcBottom.Size.width);
			
  			if (m_Player.rcPlayer.IntersectWith(Gate.rcTop) || m_Player.rcPlayer.IntersectWith(Gate.rcBottom)) {

				this->KillTimer(0xFF);

 				m_Gates.clear();

				static constexpr INT16 PlayerWidth = 40, PlayerHeight = 20;

				m_Player.rcPlayer.Location.x = 10;
				m_Player.rcPlayer.Location.y = m_CanvasSize.height / 2 - PlayerHeight / 2;
				m_Player.rcPlayer.Size.width = PlayerWidth;
				m_Player.rcPlayer.Size.height = PlayerHeight;
				m_Player.Fly = Player::State::DOWN;

				std::string GameOver = "GAME OVER!\nYour Score: " + std::to_string(m_Score);
				MessageBoxA(this->GetSafeHwnd(), GameOver.c_str(), "-- GAME OVER --", MB_ICONINFORMATION | MB_OK);

				m_Score = 0;

				constexpr UINT FPS = 1000 / 60; // * 60 FPS *
				this->SetTimer(0xFF, FPS, NULL);

				break;

			} else if (m_Player.rcPlayer.IntersectWith(rcScoreTriger) && Gate.Scored == false) {

				m_Score += 1;
				Gate.Scored = true;
				MessageBeep(MB_ICONINFORMATION);

				break;

			}

		}

		if (m_Player.rcPlayer.Location.y > m_CanvasSize.height || m_Player.rcPlayer.Location.y + m_Player.rcPlayer.Size.height < 0) {

			this->KillTimer(0xFF);
			m_Gates.clear();

			static constexpr INT16 PlayerWidth = 40, PlayerHeight = 20;

			m_Player.rcPlayer.Location.x = 10;
			m_Player.rcPlayer.Location.y = m_CanvasSize.height / 2 - PlayerHeight / 2;
			m_Player.rcPlayer.Size.width = PlayerWidth;
			m_Player.rcPlayer.Size.height = PlayerHeight;
			m_Player.Fly = Player::State::DOWN;
			
			CString GameOver;
			GameOver.Format(_T("GAME OVER!\nYour Score: %I64d\n"), m_Score);
			this->MessageBox(GameOver, _T("-- GAME OVER --"), MB_ICONINFORMATION | MB_OK);
			
			m_Score = 0;
			
			constexpr UINT FPS = 1000 / 60; // * 60 FPS *
			this->SetTimer(0xFF, FPS, NULL);

		}

		// Move Gates
		if (Frame % 3 == 0) {

			INT16 GateSpeed = (m_Score > 20) ? 10 : 8;

			for (size_t i = 0; i < m_Gates.size(); i++) {

				if (m_Gates[i].rcTop.Location.x + m_Gates[i].rcTop.Size.width < -20) {

					m_Gates.erase(m_Gates.begin() + i);
					i -= 1;
					continue;

				}

				m_Gates[i].rcTop.Location.Offset(-GateSpeed, 0);
				m_Gates[i].rcBottom.Location.Offset(-GateSpeed, 0);

			}

		}

		// Player Movement
		if (m_Player.Fly == Player::State::DOWN) {
			m_Player.rcPlayer.Location.Offset(0, 2);
		} else {
			m_Player.rcPlayer.Location.Offset(0, -2);
		}

		CString StaticText;
		StaticText.Format(_T("Score - %I64d"), m_Score);

		m_StaticScore.SetWindowTextW(StaticText);
		m_Gates.shrink_to_fit();

		Frame += 1;

		this->InvalidateRect(NULL, FALSE);

	}

	// # No Default Background #
	afx_msg BOOL OnEraseBkgnd(CDC *WindowDC) { return TRUE; }
	
	afx_msg void OnPaint() {

		CPaintDC WindowDC(this);

		if (!this->IsIconic()) {

			CRect ClientRect;
			this->GetClientRect(&ClientRect);

			WindowDC.SetBkMode(TRANSPARENT);

			CBitmap Bitmap;
			CDC MemoryDC;

			MemoryDC.CreateCompatibleDC(&WindowDC);
			Bitmap.CreateCompatibleBitmap(&WindowDC, m_CanvasSize.width, m_CanvasSize.height);

			HGDIOBJ PrevBitmap = MemoryDC.SelectObject(Bitmap);

			// Clear Screen
			CBrush SkyBlueBrush(ColorU::Enum::LightBlue);
			GdiPlus::FillRectangle(MemoryDC, { ClientRect.left, ClientRect.top }, { ClientRect.Width(), ClientRect.Height() }, SkyBlueBrush);
			
			std::array<ColorU, 3> PlayerColors = {
				ColorU::Enum::Yellow,
				ColorU::Enum::LightYellow,
				ColorU::Enum::Yellow
			};

			// Draw Player
			GdiPlus::FillGradientH(MemoryDC, m_Player.rcPlayer.Location, m_Player.rcPlayer.Size, PlayerColors);

			std::array<ColorU, 3> GateColors = {
				ColorU::Enum::DarkGreen,
				ColorU::Enum::Green,
				ColorU::Enum::DarkGreen
			};

			// Draw Gates
			for (const Gate &Gate : m_Gates) {

				GdiPlus::FillGradientH(MemoryDC, Gate.rcTop.Location, Gate.rcTop.Size, GateColors);
				GdiPlus::FillGradientH(MemoryDC, Gate.rcBottom.Location, Gate.rcBottom.Size, GateColors);

			}

			WindowDC.StretchBlt(ClientRect.left, ClientRect.top, ClientRect.Width(), ClientRect.Height(),
				&MemoryDC, 0, 0, m_CanvasSize.width, m_CanvasSize.height, SRCCOPY);

			GdiPlus::DrawTextA(WindowDC, { 4, 4 }, { 50, 20 }, "Flappy Bird", m_Font_SegoeUI, ColorU::Enum::Yellow);

			// std::string JSON = GateColors[1].ToString();
			// GdiPlus::DrawTextT(WindowDC, { 4, 38 }, { 50, 20 }, Algorithms::StringToWString(JSON).c_str(), m_Font_SegoeUI, ColorU::Enum::Yellow);

			MemoryDC.SelectObject(PrevBitmap);
		
		}

	}

	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) {

		if (nChar == VK_ESCAPE) {
			this->PostMessageW(WM_CLOSE);
		} else if (nChar == VK_SPACE) {
			m_Player.Fly = Player::State::UP;
		}

	}

	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags) {

		if (nChar == VK_SPACE) {
			m_Player.Fly = Player::State::DOWN;
		}

	}

	afx_msg void OnGetMinMaxInfo(LPMINMAXINFO minmax) {
		
		minmax->ptMinTrackSize = { m_szMinWindowSize.cx, m_szMinWindowSize.cy };

	}

	afx_msg void OnClose() {

		this->KillTimer(0xFF);

		if (this->MessageBox(_T("You Want to Quit?"), _T("INFORMATION"), MB_ICONINFORMATION | MB_YESNO | MB_DEFBUTTON2) == IDYES) {
			this->DestroyWindow();
		} else {
			constexpr UINT FPS = 1000 / 60; // * 60 FPS *
			this->SetTimer(0xFF, FPS, NULL);
		}
	}


private:

	//--------------------
	// Fonts
	CFont m_Font_SegoeUI;
	//--------------------

	CSize m_szMinWindowSize;

	Size2I m_CanvasSize;
	UINT64 m_Score;
	Player m_Player;
	std::vector<Gate> m_Gates;
	CStatic m_StaticScore;

	UINT32 Frame;


protected:

	DECLARE_MESSAGE_MAP()

};

BEGIN_MESSAGE_MAP(MainWindow, CFrameWnd)
  ON_WM_CREATE()
  ON_WM_DRAWITEM()
  ON_WM_TIMER()
  ON_WM_ERASEBKGND()
  ON_WM_PAINT()
  ON_WM_KEYDOWN()
  ON_WM_KEYUP()
  ON_WM_GETMINMAXINFO()
  ON_WM_CLOSE()
END_MESSAGE_MAP()

class MFCApp : public CWinApp {
public:
	
	BOOL InitInstance() {

		if (!this->IsWindows7()) {
			MessageBoxW(HWND_DESKTOP, L"This Application only runs on Windows 7 or higher!", L"Warning", MB_ICONWARNING | MB_OK);
			return FALSE;
		}

		SYSTEMTIME st = { 0 };
		GetSystemTime(&st);

		srand(st.wMilliseconds);
		
		m_MainWindowIcon = this->LoadIcon(IDI_MAIN_WINDOW_ICON);
		if (m_MainWindowIcon == NULL) {
			return FALSE;
		}

		MainWindow *Frame = nullptr;
		
		try {
			Frame = new MainWindow(m_MainWindowIcon);
		} catch (std::runtime_error &e) {
			MessageBoxA(HWND_DESKTOP, e.what(), "Initilization Error", MB_ICONERROR | MB_OK);
			return FALSE;
		}

		this->m_pMainWnd = Frame;

		Frame->ShowWindow(m_nCmdShow);
		
		return TRUE;

	}

	INT ExitInstance() {

		if (m_MainWindowIcon != NULL) {
			DeleteObject(m_MainWindowIcon);
		}

		return 0;

	}

private:

	HICON m_MainWindowIcon = NULL;

};

MFCApp Application;
