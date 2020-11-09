#include "ConsoleGUI.h"

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
	cfi.nFont = 0;
	cfi.dwFontSize.X = cW;
	cfi.dwFontSize.Y = cH;
	cfi.FontFamily = FF_DONTCARE;
	cfi.FontWeight = FW_NORMAL;
	wcscpy_s(cfi.FaceName, L"Consolas");

	if (!SetCurrentConsoleFontEx(mConsole, false, &cfi)) return;

	CONSOLE_SCREEN_BUFFER_INFO csbi;
	if (!GetConsoleScreenBufferInfo(mConsole, &csbi)) return;

	if (sW > csbi.dwMaximumWindowSize.X) return;
	if (sH > csbi.dwMaximumWindowSize.Y) return;

	mWindowRect = { 0, 0, (SHORT)sW - 1, (SHORT)sH - 1 };
	if (!SetConsoleWindowInfo(mConsole, TRUE, &mWindowRect)) return;

	if (!SetConsoleMode(mConsoleIn, ENABLE_EXTENDED_FLAGS | ENABLE_WINDOW_INPUT | ENABLE_MOUSE_INPUT)) return;

	mScreenBuffer = new CHAR_INFO[sW * sH];
	Fill(mBaseChar, mBaseColor);

	mInitialized = true;
}

void Console::Set(int x, int y, WCHAR chr, WORD clr) {
	mScreenBuffer[y * mScreenWidth + x].Char.UnicodeChar = chr;
	mScreenBuffer[y * mScreenWidth + x].Attributes = clr;
}

void Console::Fill(WCHAR chr, WORD clr) {
	for (int i = 0; i < mScreenWidth * mScreenHeight; i++) {
		mScreenBuffer[i].Char.UnicodeChar = chr;
		mScreenBuffer[i].Attributes = clr;
	}
}

void Console::Rect(RECT r, WCHAR chr, WORD clr, bool fill) {
	for (int i = r.top; i <= r.bottom; i++) {
		for (int j = r.left; j <= r.right; j++) {
			if (fill) Set(j, i, chr, clr);
			else if (i == r.top || i == r.bottom || j == r.left || j == r.right) Set(j, i, chr, clr);
		}
	}
}

void Console::Write(int x, int y, std::wstring str, WORD clr) {
	for (size_t i = 0; i < str.size(); i++) {
		mScreenBuffer[y * mScreenWidth + x + i].Char.UnicodeChar = str[i];
		mScreenBuffer[y * mScreenWidth + x + i].Attributes = clr;
	}
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

		for (int k = 0; k < 256; k++) {
			newKeyStates[k] = GetAsyncKeyState(k);
			keyboard[k].pressed = false;
			keyboard[k].released = false;
			if (newKeyStates[k] != oldKeyStates[k]) {
				if (newKeyStates[k] & 0x8000) {
					keyboard[k].pressed = !keyboard[k].held;
					keyboard[k].held = true;
				} else {
					keyboard[k].released = true;
					keyboard[k].held = false;
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
					mMousePosition = inputBuffer[i].Event.MouseEvent.dwMousePosition;
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
			mouseButtons[m].pressed = false;
			mouseButtons[m].released = false;
			if (newMouseStates[m] != oldMouseStates[m]) {
				if (newMouseStates[m]) {
					mouseButtons[m].pressed = true;
					mouseButtons[m].held = true;
				} else {
					mouseButtons[m].released = true;
					mouseButtons[m].held = false;
				}
				// Check click handlers only if state change
				for (MouseHandler* h : mMouseHandlers) {
					if (h->mButtons & (1 << m) &&
						mMousePosition.X >= h->mBounds.left &&
						mMousePosition.X <= h->mBounds.right &&
						mMousePosition.Y >= h->mBounds.top &&
						mMousePosition.Y <= h->mBounds.bottom) {
						if (mouseButtons[m].pressed && h->mPressAction) h->mPressAction(1 << m);
						if (mouseButtons[m].released && h->mReleaseAction) h->mReleaseAction(1 << m);
					}
				}
			}
			oldMouseStates[m] = newMouseStates[m];
		}

		Fill(mBaseChar, mBaseColor);
		for (Element* e : mElements) {
			e->Draw(this);
		}

		// Display
		WCHAR title[256];
		swprintf(title, 256, L"FPS: %3.0f", 1 / elapsedTime);
		SetConsoleTitle(title);
		WriteConsoleOutput(mConsole, mScreenBuffer, { (SHORT)mScreenWidth, (SHORT)mScreenHeight }, { 0, 0 }, &mWindowRect);
	}
}

const int& Console::GetScreenWidth() const { return mScreenWidth; }
const int& Console::GetScreenHeight() const { return mScreenHeight; }

const COORD& Console::GetMousePosition() const { return mMousePosition; }

const WCHAR& Console::GetBaseChar() const { return mBaseChar; }
void Console::SetBaseChar(WCHAR c) { mBaseChar = c; }

const WORD& Console::GetBaseColor() const { return mBaseColor; }
void Console::SetBaseColor(WORD c) { mBaseColor = c; }

void Console::AddElement(Element* e) {
	for (size_t i = 0; i < mElements.size(); i++) {
		if (mElements.at(i) == nullptr) {
			e->mId = i;
			mElements.at(i) = e;
			return;
		}
	}
	mElements.push_back(e);
	e->mId = mElements.size() - 1;

	if (auto b = dynamic_cast<Button*>(e)) AddMouseHandler(&b->mHandler);
}

Element* Console::GetElement(int i) {
	return mElements.at(i);
}

Element* Console::GetElement(Element* e) {
	return GetElement(e->mId);
}

void Console::RemoveElement(int i) {
	mElements.at(i)->mId = -1;
	mElements.at(i) = nullptr;
}

void Console::RemoveElement(Element* e) {
	RemoveElement(e->mId);

	if (auto b = dynamic_cast<Button*>(e)) RemoveMouseHandler(&b->mHandler);
}

void Console::AddMouseHandler(MouseHandler* h) {
	for (size_t i = 0; i < mMouseHandlers.size(); i++) {
		if (mMouseHandlers.at(i) == nullptr) {
			h->mId = i;
			mMouseHandlers.at(i) = h;
			return;
		}
	}
	mMouseHandlers.push_back(h);
	h->mId = mElements.size() - 1;
}

MouseHandler* Console::GetMouseHandler(int i) {
	return mMouseHandlers.at(i);
}

MouseHandler* Console::GetMouseHandler(MouseHandler* h) {
	return GetMouseHandler(h->mId);
}

void Console::RemoveMouseHandler(int i) {
	mMouseHandlers.at(i)->mId = -1;
	mMouseHandlers.at(i) = nullptr;
}

void Console::RemoveMouseHandler(MouseHandler* h) {
	RemoveMouseHandler(h->mId);
}

}