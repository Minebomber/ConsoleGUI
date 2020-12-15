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
						if (mCurrentWindow->mFocusedElement) {
							for (EventHandler* h : mCurrentWindow->mFocusedElement->mEventHandlers) {
								if (mCurrentWindow->mKeyboard[k] && h->KeyDownActionExists()) h->InvokeKeyDownAction(mCurrentWindow, k);
								if (!mCurrentWindow->mKeyboard[k] && h->KeyUpActionExists()) h->InvokeKeyUpAction(mCurrentWindow, k);
							}
						}

					}
					break;
				case MOUSE_EVENT:
					if (!mFocused) break;
					switch (inputBuffer[i].Event.MouseEvent.dwEventFlags) {
					case MOUSE_MOVED:
						{
							Point p = { inputBuffer[i].Event.MouseEvent.dwMousePosition.X, inputBuffer[i].Event.MouseEvent.dwMousePosition.Y };
							if (p != mCurrentWindow->mMousePosition) {
								mCurrentWindow->mMousePosition = p;
								if (int btnState = inputBuffer[i].Event.MouseEvent.dwButtonState)
									if (auto e = mCurrentWindow->GetElementAtPoint(p))
										for (int m = 0; m < 3; m++)
											if (((1 << m) & btnState))
												for (EventHandler* h : e->mEventHandlers)
													if (h->MouseDragActionExists())
														h->InvokeMouseDragAction(mCurrentWindow, 1 << m);
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
								mCurrentWindow->mFocusedElement = nullptr;
								mCurrentWindow->ApplyToElements([](Element* e) { e->mState = ELEMENT_DEFAULT; });
							}

							if (pressed != mCurrentWindow->mMouseButtons[m]) {
								mCurrentWindow->mMouseButtons[m] = pressed;
								if (Element* e = mCurrentWindow->GetElementAtPoint(mCurrentWindow->mMousePosition)) {
									for (EventHandler* h : e->mEventHandlers) {
										if (pressed && h->MouseDownActionExists()) h->InvokeMouseDownAction(mCurrentWindow, 1 << m);
										if (!pressed && h->MouseUpActionExists()) h->InvokeMouseUpAction(mCurrentWindow, 1 << m);
									}
								}
							}
						}
						break;
					case MOUSE_WHEELED:
						if (mCurrentWindow->mFocusedElement) {
							for (EventHandler* h : mCurrentWindow->mFocusedElement->mEventHandlers) {
								int d = inputBuffer[i].Event.MouseEvent.dwButtonState;
								if (d < 0 && h->MouseWheelDownActionExists()) h->InvokeMouseWheelDownAction(mCurrentWindow, d);
								if (d > 0 && h->MouseWheelUpActionExists()) h->InvokeMouseWheelUpAction(mCurrentWindow, d);
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

void Console::Stop() { 
	if (mCurrentWindow) {
		mCurrentWindow->mFocusedElement = nullptr;
		mCurrentWindow->ApplyToElements([](Element* e) { e->mState = ELEMENT_DEFAULT; });
	}
	mRunning = false; 
}

}