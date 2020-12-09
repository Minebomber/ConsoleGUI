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
	// Set console to default
	SetCurrentConsoleFontEx(mConsole, false, &mOriginalConsoleFontInfo);
	SetConsoleScreenBufferSize(mConsole, mOriginalConsoleScreenBufferInfo.dwSize);
	SetConsoleWindowInfo(mConsole, true, &mOriginalConsoleScreenBufferInfo.srWindow);
	mOriginalConsoleCursorInfo.bVisible = true;
	SetConsoleCursorInfo(mConsole, &mOriginalConsoleCursorInfo);
}

void Console::CreateConsole(int sW, int sH, int cW, int cH) {
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
	else delete mCurrentWindow;
	mCurrentWindow = w; 
}

void Console::Run() {
	if (!mInitialized) return;

	if (!Initialize()) return;

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
					mFocused = inputBuffer[i].Event.FocusEvent.bSetFocus;
					break;
				case KEY_EVENT:
					if (!mFocused) break;
					{
						// Keyboard
						int k = inputBuffer[i].Event.KeyEvent.wVirtualKeyCode;
						mCurrentWindow->mKeyboard[k] = inputBuffer[i].Event.KeyEvent.bKeyDown;
						// Notify keyboard handler
						if (mCurrentWindow->mActiveKeyboardHandler) {
							if (mCurrentWindow->mActiveKeyboardHandler->mKeys.find(k) != std::wstring::npos) {
								if (mCurrentWindow->mKeyboard[k] && mCurrentWindow->mActiveKeyboardHandler->mPressAction)
									mCurrentWindow->mActiveKeyboardHandler->mPressAction(mCurrentWindow, k);
								if (!mCurrentWindow->mKeyboard[k] && mCurrentWindow->mActiveKeyboardHandler->mReleaseAction)
									mCurrentWindow->mActiveKeyboardHandler->mReleaseAction(mCurrentWindow, k);
							}
						}
					}
					break;
				case MOUSE_EVENT:
					if (!mFocused) break;
					switch (inputBuffer[i].Event.MouseEvent.dwEventFlags) {
					case MOUSE_MOVED:
						mCurrentWindow->mMousePosition = {
							inputBuffer[i].Event.MouseEvent.dwMousePosition.X,
							inputBuffer[i].Event.MouseEvent.dwMousePosition.Y
						};
						break;
					case 0:
					case DOUBLE_CLICK:
						// Mouse
						for (int m = 0; m < 3; m++) {
							mCurrentWindow->mMouseButtons[m] = inputBuffer[i].Event.MouseEvent.dwButtonState & (1 << m);
							// Notify mouse handlers
							for (MouseHandler* h : mCurrentWindow->mMouseHandlers) {
								if (h->mButtons & (1 << m) && h->mBounds.Contains(mCurrentWindow->GetMousePosition())) {
									if (mCurrentWindow->mMouseButtons[m] && h->mPressAction) h->mPressAction(mCurrentWindow, 1 << m);
									if (!mCurrentWindow->mMouseButtons[m] && h->mReleaseAction) h->mReleaseAction(mCurrentWindow, 1 << m);
								}
							}
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

void Console::Stop() { mRunning = false; }

}