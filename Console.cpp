#include "Console.h"

namespace gui {

Console::Console() {
	mConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	mConsoleIn = GetStdHandle(STD_INPUT_HANDLE);

	// Get console defaults
	mOriginalConsoleFontInfo.cbSize = sizeof(mOriginalConsoleFontInfo);
	GetCurrentConsoleFontEx(mConsole, false, &mOriginalConsoleFontInfo);
	GetConsoleScreenBufferInfo(mConsole, &mOriginalConsoleScreenBufferInfo);
	GetConsoleCursorInfo(mConsole, &mOriginalConsoleCursorInfo);
	mOriginalConsoleCursorInfo.bVisible = false;
	SetConsoleCursorInfo(mConsole, &mOriginalConsoleCursorInfo);
}

Console::~Console() {
	SetCurrentWindow(nullptr);
	// Set console to default
	SetCurrentConsoleFontEx(mConsole, false, &mOriginalConsoleFontInfo);
	SetConsoleScreenBufferSize(mConsole, mOriginalConsoleScreenBufferInfo.dwSize);
	SetConsoleWindowInfo(mConsole, true, &mOriginalConsoleScreenBufferInfo.srWindow);
	mOriginalConsoleCursorInfo.bVisible = true;
	SetConsoleCursorInfo(mConsole, &mOriginalConsoleCursorInfo);
}

void Console::Create(int sW, int sH, int cW, int cH) {
	mScreenWidth = sW; mScreenHeight = sH; mCharWidth = cW; mCharHeight = cH;

	if (mConsole == INVALID_HANDLE_VALUE) return;

	mWindowRect = { 0, 0, 1, 1 };
	SetConsoleWindowInfo(mConsole, TRUE, &mWindowRect);

	COORD coord = { (SHORT)mScreenWidth, (SHORT)mScreenHeight };
	if (!SetConsoleScreenBufferSize(mConsole, coord)) return;

	if (!SetConsoleActiveScreenBuffer(mConsole)) return;

	CONSOLE_FONT_INFOEX cfi;
	cfi.cbSize = sizeof(cfi);
	GetCurrentConsoleFontEx(mConsole, false, &cfi);
	cfi.dwFontSize.X = cW;
	cfi.dwFontSize.Y = cH;
	cfi.FontFamily = FF_MODERN;
	if (!SetCurrentConsoleFontEx(mConsole, false, &cfi)) return;

	CONSOLE_SCREEN_BUFFER_INFO csbi;
	if (!GetConsoleScreenBufferInfo(mConsole, &csbi)) return;

	if (sW > csbi.dwMaximumWindowSize.X) return;
	if (sH > csbi.dwMaximumWindowSize.Y) return;

	mWindowRect = { 0, 0, (SHORT)sW - 1, (SHORT)sH - 1 };
	if (!SetConsoleWindowInfo(mConsole, TRUE, &mWindowRect)) return;

	if (!SetConsoleMode(mConsoleIn, ENABLE_EXTENDED_FLAGS | ENABLE_WINDOW_INPUT | ENABLE_MOUSE_INPUT)) return;

	mInitialized = true;
}

void Console::SetCurrentWindow(Window* w) { 
	if (mCurrentWindow) mCurrentWindow->OnHide(); 
	if (w) w->OnShow();
	else if (mCurrentWindow) delete mCurrentWindow;
	mCurrentWindow = w; 
}

void Console::Run() {
	if (!mInitialized) return;

	Initialize();

	auto timePoint1 = std::chrono::system_clock::now();
	auto timePoint2 = std::chrono::system_clock::now();

	float elapsedTime = 1;

	mRunning = true;
	while (mRunning) {

		// Time
		timePoint2 = std::chrono::system_clock::now();
		std::chrono::duration<float> timeDelta = timePoint2 - timePoint1;
		timePoint1 = timePoint2;
		elapsedTime = timeDelta.count();
		if (mCurrentWindow) {

			// Events
			INPUT_RECORD inputBuffer[32];
			DWORD events = 0;
			GetNumberOfConsoleInputEvents(mConsoleIn, &events);
			if (events > 0) ReadConsoleInput(mConsoleIn, inputBuffer, 32, &events);

			for (DWORD i = 0; i < events; i++) {
				switch (inputBuffer[i].EventType) {
				case FOCUS_EVENT:
					//mFocused = inputBuffer[i].Event.FocusEvent.bSetFocus;
					break;
				case KEY_EVENT:
					if (!mFocused) break;
					{
						// Keyboard
						int k = inputBuffer[i].Event.KeyEvent.wVirtualKeyCode;
						mCurrentWindow->mKeyboard[k] = inputBuffer[i].Event.KeyEvent.bKeyDown;

						// Send keyboard events to focused element
						if (mCurrentWindow->focusedElement) {
							if (mCurrentWindow->mKeyboard[k]) 
								mCurrentWindow->focusedElement->HandleEvent(EVENT_KEY_DOWN, mCurrentWindow, k);
							else 
								mCurrentWindow->focusedElement->HandleEvent(EVENT_KEY_UP, mCurrentWindow, k);
						}
					}
					break;
				case MOUSE_EVENT:
					if (!mFocused) break;
					switch (inputBuffer[i].Event.MouseEvent.dwEventFlags) {
					case MOUSE_MOVED:
						{
							Point p = { inputBuffer[i].Event.MouseEvent.dwMousePosition.X, inputBuffer[i].Event.MouseEvent.dwMousePosition.Y };
							if (p != mCurrentWindow->mousePosition) {
								mCurrentWindow->mousePosition = p;
								if (int btnState = inputBuffer[i].Event.MouseEvent.dwButtonState)
									if (auto e = mCurrentWindow->GetElementAtPoint(p))
										for (int m = 0; m < 3; m++)
											if (((1 << m) & btnState))
												e->HandleEvent(EVENT_MOUSE_DRAG, mCurrentWindow, 1 << m);
							}
						}
						break;
					case 0:
					case DOUBLE_CLICK:
						// Mouse
						for (int m = 0; m < 3; m++) {
							bool pressed = inputBuffer[i].Event.MouseEvent.dwButtonState & (1 << m);

							if (!mCurrentWindow->mMouseButtons[m] && pressed) {
								// Clear focused at start, handlers will set if needed
								mCurrentWindow->focusedElement = nullptr;
								mCurrentWindow->ApplyToElements([](Element* e) { e->state = ELEMENT_DEFAULT; });
							}

							if (pressed != mCurrentWindow->mMouseButtons[m]) {
								mCurrentWindow->mMouseButtons[m] = pressed;
								if (Element* e = mCurrentWindow->GetElementAtPoint(mCurrentWindow->mousePosition)) {
									if (pressed) e->HandleEvent(EVENT_MOUSE_DOWN, mCurrentWindow, 1 << m);
									else e->HandleEvent(EVENT_MOUSE_UP, mCurrentWindow, 1 << m);
								}
							}
						}
						break;
					case MOUSE_WHEELED:
						if (mCurrentWindow->focusedElement) {
							int d = inputBuffer[i].Event.MouseEvent.dwButtonState;
							if (d < 0)
								mCurrentWindow->focusedElement->HandleEvent(EVENT_MOUSE_WHEELDOWN, mCurrentWindow, d);
							else if (d > 0)
								mCurrentWindow->focusedElement->HandleEvent(EVENT_MOUSE_WHEELUP, mCurrentWindow, d);
						}
						break;
					default:
						break;
					}
					break;
				default:
					break;
				}
			}

			if (mFocused) {
				mCurrentWindow->Display();

				// Display
				WCHAR title[256];
				swprintf(title, 256, L"FPS: %3.0f", 1 / elapsedTime);
				SetConsoleTitle(title);
				WriteConsoleOutput(mConsole, mCurrentWindow->mBuffer, { (SHORT)mScreenWidth, (SHORT)mScreenHeight }, { 0, 0 }, &mWindowRect);

				// Check exit
				if (mCurrentWindow->mKeyboard[VK_ESCAPE] && mCurrentWindow->mKeyboard['Q']) Stop();

			}
		}
	}
}

void Console::Stop() { 
	if (mCurrentWindow) {
		mCurrentWindow->focusedElement = nullptr;
		mCurrentWindow->ApplyToElements([](Element* e) { e->state = ELEMENT_DEFAULT; });
	}
	mRunning = false; 
}

}