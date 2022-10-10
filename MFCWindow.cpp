#pragma comment(linker,"\"/manifestdependency:type='win32' \
name='Microsoft.Windows.Common-Controls' version='6.0.0.0' \
processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

#include <stdexcept>
#include <afxwin.h>
#include <afxcmn.h>
#include <memory>
#include <vector>
#include <string>
#include <assert.h>

#include "gdiplus.h"
#include "Algorithms.h"
#include "SystemError.h"
#include "SystemClipboard.h"

#include "Resource.h"

template<typename PixelData>
class FrameBuffer {
public:
	
	explicit FrameBuffer()
		: m_Size() {
		m_FrameBuffer = std::make_unique<PixelData[]>(m_Size.width * m_Size.height);
	}
	
	explicit FrameBuffer(int32_t width, int32_t height)
		: m_Size(width, height) {
		m_FrameBuffer = std::make_unique<PixelData[]>(m_Size.width * m_Size.height);
		
	}

	void SetPixel(int32_t x, int32_t y, const PixelData &pixelData) noexcept {

		if (x >= 0 && y >= 0 && x < m_Size.width && y < m_Size.height) {
			m_FrameBuffer[y * m_Size.width + x] = pixelData;
		}

	}
	
	PixelData GetPixel(int32_t x, int32_t y) const {

		if (x >= 0 && y >= 0 && x < m_Size.width && y < m_Size.height) {
			return m_FrameBuffer[y * m_Size.width + x];
		} else {
			throw (std::out_of_range("Pixel Outside Frame Buffer"));
		}

	}
	
	~FrameBuffer() noexcept = default;
	
private:

	Size2I m_Size;
	std::unique_ptr<PixelData[]> m_FrameBuffer;

};

#pragma push_macro("MessageBox")
#undef MessageBox

namespace WinGui {
	
	struct MessageBox {

		// # Message Box Icons #
		enum class Icon : INT16 { Error, Warning, Information, Question };
		enum class Buttons : INT16 { Ok, OkCancel, RetryCancel, YesNo, YesNoCancel, AbortRetryIgnore };
		enum class DefaultButton : INT16 { Button1, Button2, Button3 };
		
		HWND hWndParent;
		std::wstring Caption;
		std::wstring Message;

		Icon MsgBoxIcon;
		Buttons MsgBoxButtons;
		DefaultButton MsgBoxDefaultButton;

		// # Message Box Results #
		enum class Result : INT16 { Ok, Cancel, Yes, No, Abort, Retry, Ignore, Error };

		//----------------------------------------
		// Constructors

		MessageBox() 
			: hWndParent(HWND_DESKTOP), Caption(L"Caption"), Message(L"Message To User"), 
			MsgBoxIcon(Icon::Information), MsgBoxButtons(Buttons::Ok),
			MsgBoxDefaultButton(DefaultButton::Button1)
		{ /*...*/ }

		//----------------------------------------

		Result Show() {

			MSGBOXPARAMSW MsgBox = { 0 };
			MsgBox.cbSize = sizeof(MSGBOXPARAMSW);



			MessageBoxIndirectW(&MsgBox);

		}

		// # Default Destructor #
		~MessageBox() noexcept = default;

	};

}

#pragma pop_macro("MessageBox")

struct Gate {

	Gate()
		: rcTop(), rcBottom(), Scored(false)
	{ /*...*/ }

	Gate(const Rect4I &rcTop, const Rect4I &rcBottom, bool Scored)
		: rcTop(rcTop), rcBottom(rcBottom), Scored(Scored)
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

	Player(const Rect4I &rcPlayer, State Fly)
		: rcPlayer(rcPlayer), Fly(Fly)
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
		RECT WindowRect = { X, Y, X + Width, Y + Height };
		AdjustWindowRect(&WindowRect, Style, NULL);

		m_szMinWindowSize = Size2I(WindowRect.right - WindowRect.left, WindowRect.bottom - WindowRect.top);
		
		// Create Window
		if (!this->Create(NULL, _T("Flappy Bird"), Style, WindowRect)) {
			throw std::runtime_error(SystemError(GetLastError()).GetErrorMessage());
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

		if (!m_Font_SegoeUI.CreatePointFont(200, _T("SegoeUI"), this->GetDC())) {	
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

			Rect4I rcScoreTriger(Gate.rcTop.Left(), Gate.rcTop.Bottom(), Gate.rcTop.Size.width, Gate.rcBottom.Top() - Gate.rcTop.Bottom());
			
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

				break;

			}

		}

		if (m_Player.rcPlayer.Top() > m_CanvasSize.height || m_Player.rcPlayer.Bottom() < 0) {

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
			GdiPlus::FillRectangle(MemoryDC, Vertex2I(), m_CanvasSize, SkyBlueBrush);
			
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

			Rect4I Client(ClientRect.left, ClientRect.top, ClientRect.Width(), ClientRect.Height());

			WindowDC.StretchBlt(Client.Left(), Client.Top(), Client.Size.width, Client.Size.height,
				&MemoryDC, 0, 0, m_CanvasSize.width, m_CanvasSize.height, SRCCOPY);

			GdiPlus::DrawText(WindowDC, { 4, 4 }, { 200, 40 }, L"Flappy Bird", m_Font_SegoeUI, ColorU::Enum::Yellow);

			// WinErr Error(0x0000212F);
			// std::string FormatErrorMessage = SystemError(0x0000212F).Format("Error Code: $(ErrorCode) - $(ErrorMessage)");
			// std::string JSON = SystemError(0xFFFFFFFF).ToString();

			using namespace std::string_literals;

			SystemClipboard::SetClipboardText(this->GetSafeHwnd(), "Text To Clipboard Num: "s + std::to_string(Frame));
			assert(SystemClipboard::SetClipboardText(NULL, "Text To Clipboard") == SystemClipboard::InvalidParameter);
			assert(SystemClipboard::SetClipboardText(this->GetSafeHwnd(), "") == SystemClipboard::InvalidParameter);
			assert(SystemClipboard::SetClipboardText(NULL, "") == SystemClipboard::InvalidParameter);

			// std::string JSON = GateColors[1].ToString();
			GdiPlus::DrawText(WindowDC, { 4, 38 }, { 600, 40 }, SystemError(0x0000212F).GetErrorMessage(), m_Font_SegoeUI, ColorU::Enum::Yellow);

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
		
		minmax->ptMinTrackSize = { m_szMinWindowSize.width, m_szMinWindowSize.height };

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

	Size2I m_szMinWindowSize;

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
