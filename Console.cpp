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

void Console::Run() {
	if (!mInitialized) return;

	if (!Initialize()) return;

	auto timePoint1 = std::chrono::system_clock::now();
	auto timePoint2 = std::chrono::system_clock::now();

	SHORT oldKeyStates[256] = { 0 };
	SHORT newKeyStates[256] = { 0 };
	bool oldMouseStates[3] = { 0 };
	bool newMouseStates[3] = { 0 };

	float elapsedTime = 1;

	mRunning = true;
	while (mRunning) {

		// Time
		timePoint2 = std::chrono::system_clock::now();
		std::chrono::duration<float> timeDelta = timePoint2 - timePoint1;
		timePoint1 = timePoint2;
		elapsedTime = timeDelta.count();
		if (mCurrentWindow) {
			for (int k = 0; k < 256; k++) {
				newKeyStates[k] = GetAsyncKeyState(k);
				mCurrentWindow->mKeyboard[k].pressed = false;
				mCurrentWindow->mKeyboard[k].released = false;
				if (newKeyStates[k] != oldKeyStates[k]) {
					if (newKeyStates[k] & 0x8000) {
						mCurrentWindow->mKeyboard[k].pressed = !mCurrentWindow->mKeyboard[k].held;
						mCurrentWindow->mKeyboard[k].held = true;
					} else {
						mCurrentWindow->mKeyboard[k].released = true;
						mCurrentWindow->mKeyboard[k].held = false;
					}
					if (mCurrentWindow->mActiveKeyboardHandler) {
						if (mCurrentWindow->mActiveKeyboardHandler->mKeys.find(k) != std::wstring::npos) {
							if (mCurrentWindow->mKeyboard[k].pressed && mCurrentWindow->mActiveKeyboardHandler->mPressAction) mCurrentWindow->mActiveKeyboardHandler->mPressAction(mCurrentWindow, k);
							if (mCurrentWindow->mKeyboard[k].released && mCurrentWindow->mActiveKeyboardHandler->mReleaseAction) mCurrentWindow->mActiveKeyboardHandler->mReleaseAction(mCurrentWindow, k);
						}
					}

				}
				oldKeyStates[k] = newKeyStates[k];
			}

			INPUT_RECORD inputBuffer[32];
			DWORD events = 0;
			GetNumberOfConsoleInputEvents(mConsoleIn, &events);
			if (events > 0) ReadConsoleInput(mConsoleIn, inputBuffer, 32, &events);

			for (DWORD i = 0; i < events; i++) {
				switch (inputBuffer[i].EventType) {
				case FOCUS_EVENT:
					//isFocused = inputBuffer[i].Event.FocusEvent.bSetFocus;
					break;
				case MOUSE_EVENT:
					switch (inputBuffer[i].Event.MouseEvent.dwEventFlags) {
					case MOUSE_MOVED:
						mCurrentWindow->mMousePosition = inputBuffer[i].Event.MouseEvent.dwMousePosition;
						break;
					case 0:
					case DOUBLE_CLICK:
						for (int m = 0; m < 3; m++) {
							newMouseStates[m] = (inputBuffer[i].Event.MouseEvent.dwButtonState & (1 << m)) > 0;
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

			// Mouse
			for (int m = 0; m < 3; m++) {
				mCurrentWindow->mMouseButtons[m].pressed = false;
				mCurrentWindow->mMouseButtons[m].released = false;
				if (newMouseStates[m] != oldMouseStates[m]) {
					if (newMouseStates[m]) {
						mCurrentWindow->mMouseButtons[m].pressed = true;
						mCurrentWindow->mMouseButtons[m].held = true;
					} else {
						mCurrentWindow->mMouseButtons[m].released = true;
						mCurrentWindow->mMouseButtons[m].held = false;
					}

					if (m == 0 && mCurrentWindow->mMouseButtons[m].pressed) {
						mCurrentWindow->mActiveKeyboardHandler = nullptr;
						mCurrentWindow->ApplyToElements([](Element* e) { if (auto t = dynamic_cast<TextField*>(e)) t->SetEnabled(false); });
					}
					// Check click handlers only if state change
					for (MouseHandler* h : mCurrentWindow->mMouseHandlers) {
						if (h->mButtons & (1 << m) &&
							mCurrentWindow->mMousePosition.X >= h->mBounds.left &&
							mCurrentWindow->mMousePosition.X <= h->mBounds.right &&
							mCurrentWindow->mMousePosition.Y >= h->mBounds.top &&
							mCurrentWindow->mMousePosition.Y <= h->mBounds.bottom) {
							if (mCurrentWindow->mMouseButtons[m].pressed && h->mPressAction) h->mPressAction(mCurrentWindow, 1 << m);
							if (mCurrentWindow->mMouseButtons[m].released && h->mReleaseAction) h->mReleaseAction(mCurrentWindow, 1 << m);
						}
					}
				}
				oldMouseStates[m] = newMouseStates[m];
			}

			mCurrentWindow->Display();

			// Display
			WCHAR title[256];
			swprintf(title, 256, L"FPS: %3.0f", 1 / elapsedTime);
			SetConsoleTitle(title);
			WriteConsoleOutput(mConsole, mCurrentWindow->mBuffer, { (SHORT)mScreenWidth, (SHORT)mScreenHeight }, { 0, 0 }, &mWindowRect);

			// Check exit
			if (mCurrentWindow->mKeyboard[VK_ESCAPE].held && mCurrentWindow->mKeyboard['Q'].held) Stop();
		}
	}
}

void Console::Stop() { mRunning = false; }

}