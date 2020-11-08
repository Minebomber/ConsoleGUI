#include "ConsoleGUI.h"

namespace gui {

Console::Console() {
	console = GetStdHandle(STD_OUTPUT_HANDLE);
	consoleIn = GetStdHandle(STD_INPUT_HANDLE);

	// Get console defaults
	originalConsoleFontInfo.cbSize = sizeof(originalConsoleFontInfo);
	GetCurrentConsoleFontEx(console, false, &originalConsoleFontInfo);
	GetConsoleScreenBufferInfo(console, &originalConsoleScreenBufferInfo);
	GetConsoleCursorInfo(console, &originalConsoleCursorInfo);
	originalConsoleCursorInfo.bVisible = false;
	SetConsoleCursorInfo(console, &originalConsoleCursorInfo);
}

Console::~Console() {
	// Set console to default
	SetCurrentConsoleFontEx(console, false, &originalConsoleFontInfo);
	SetConsoleScreenBufferSize(console, originalConsoleScreenBufferInfo.dwSize);
	SetConsoleWindowInfo(console, true, &originalConsoleScreenBufferInfo.srWindow);
	originalConsoleCursorInfo.bVisible = true;
	SetConsoleCursorInfo(console, &originalConsoleCursorInfo);
}

void Console::CreateConsole(int sW, int sH, int cW, int cH) {
	screenWidth = sW; screenHeight = sH; charWidth = cW; charHeight = cH;

	if (console == INVALID_HANDLE_VALUE) return;

	windowRect = { 0, 0, 1, 1 };
	SetConsoleWindowInfo(console, TRUE, &windowRect);

	COORD coord = { (SHORT)screenWidth, (SHORT)screenHeight };
	if (!SetConsoleScreenBufferSize(console, coord)) return;

	if (!SetConsoleActiveScreenBuffer(console)) return;

	CONSOLE_FONT_INFOEX cfi;
	cfi.cbSize = sizeof(cfi);
	cfi.nFont = 0;
	cfi.dwFontSize.X = cW;
	cfi.dwFontSize.Y = cH;
	cfi.FontFamily = FF_DONTCARE;
	cfi.FontWeight = FW_NORMAL;
	wcscpy_s(cfi.FaceName, L"Consolas");

	if (!SetCurrentConsoleFontEx(console, false, &cfi)) return;

	CONSOLE_SCREEN_BUFFER_INFO csbi;
	if (!GetConsoleScreenBufferInfo(console, &csbi)) return;

	if (sW > csbi.dwMaximumWindowSize.X) return;
	if (sH > csbi.dwMaximumWindowSize.Y) return;

	windowRect = { 0, 0, (SHORT)sW - 1, (SHORT)sH - 1 };
	if (!SetConsoleWindowInfo(console, TRUE, &windowRect)) return;

	if (!SetConsoleMode(consoleIn, ENABLE_EXTENDED_FLAGS | ENABLE_WINDOW_INPUT | ENABLE_MOUSE_INPUT)) return;

	screenBuffer = new CHAR_INFO[sW * sH];
	Fill(baseChar, baseColor);

	initialized = true;
}

void Console::Set(int x, int y, WCHAR chr, WORD clr) {
	screenBuffer[y * screenWidth + x].Char.UnicodeChar = chr;
	screenBuffer[y * screenWidth + x].Attributes = clr;
}

void Console::Fill(WCHAR chr, WORD clr) {
	for (int i = 0; i < screenWidth * screenHeight; i++) {
		screenBuffer[i].Char.UnicodeChar = chr;
		screenBuffer[i].Attributes = clr;
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
		screenBuffer[y * screenWidth + x + i].Char.UnicodeChar = str[i];
		screenBuffer[y * screenWidth + x + i].Attributes = clr;
	}
}

void Console::Run() {
	if (!initialized) return;

	if (!Initialize()) return;

	auto timePoint1 = std::chrono::system_clock::now();
	auto timePoint2 = std::chrono::system_clock::now();

	SHORT oldKeyStates[256] = { 0 };
	SHORT newKeyStates[256] = { 0 };
	bool oldMouseStates[3] = { 0 };
	bool newMouseStates[3] = { 0 };

	float elapsedTime = 1;

	running = true;
	while (running) {
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
		GetNumberOfConsoleInputEvents(consoleIn, &events);
		if (events > 0) ReadConsoleInput(consoleIn, inputBuffer, 32, &events);

		for (DWORD i = 0; i < events; i++) {
			switch (inputBuffer[i].EventType) {
			case FOCUS_EVENT:
				//isFocused = inputBuffer[i].Event.FocusEvent.bSetFocus;
				break;
			case MOUSE_EVENT:
				switch (inputBuffer[i].Event.MouseEvent.dwEventFlags) {
				case MOUSE_MOVED:
					mousePosition = inputBuffer[i].Event.MouseEvent.dwMousePosition;
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
				for (MouseHandler* h : mouseHandlers) {
					if (h->mButtons & (1 << m) &&
						mousePosition.X >= h->mBounds.left &&
						mousePosition.X <= h->mBounds.right &&
						mousePosition.Y >= h->mBounds.top &&
						mousePosition.Y <= h->mBounds.bottom) {
						if (mouseButtons[m].pressed && h->mPressAction) h->mPressAction(m);
						if (mouseButtons[m].released && h->mReleaseAction) h->mReleaseAction(m);
					}
				}
			}
			oldMouseStates[m] = newMouseStates[m];
		}

		Fill(baseChar, baseColor);
		for (Element* e : elements) {
			e->Draw(this);
		}

		// Display
		WCHAR title[256];
		swprintf(title, 256, L"FPS: %3.0f", 1 / elapsedTime);
		SetConsoleTitle(title);
		WriteConsoleOutput(console, screenBuffer, { (SHORT)screenWidth, (SHORT)screenHeight }, { 0, 0 }, &windowRect);
	}
}

const int& Console::GetScreenWidth() const { return screenWidth; }
const int& Console::GetScreenHeight() const { return screenHeight; }

const WCHAR& Console::GetBaseChar() const { return baseChar; }
void Console::SetBaseChar(WCHAR c) { baseChar = c; }

const WORD& Console::GetBaseColor() const { return baseColor; }
void Console::SetBaseColor(WORD c) { baseColor = c; }

void Console::AddElement(Element* e) {
	for (size_t i = 0; i < elements.size(); i++) {
		if (elements.at(i) == nullptr) {
			e->mId = i;
			elements.at(i) = e;
			return;
		}
	}
	elements.push_back(e);
	e->mId = elements.size() - 1;

	if (auto b = dynamic_cast<Button*>(e)) AddMouseHandler(&b->mHandler);
}

Element* Console::GetElement(int i) {
	return elements.at(i);
}

Element* Console::GetElement(Element* e) {
	return GetElement(e->mId);
}

void Console::RemoveElement(int i) {
	elements.at(i)->mId = -1;
	elements.at(i) = nullptr;
}

void Console::RemoveElement(Element* e) {
	RemoveElement(e->mId);

	if (auto b = dynamic_cast<Button*>(e)) RemoveMouseHandler(&b->mHandler);
}

void Console::AddMouseHandler(MouseHandler* h) {
	for (size_t i = 0; i < mouseHandlers.size(); i++) {
		if (mouseHandlers.at(i) == nullptr) {
			h->mId = i;
			mouseHandlers.at(i) = h;
			return;
		}
	}
	mouseHandlers.push_back(h);
	h->mId = elements.size() - 1;
}

MouseHandler* Console::GetMouseHandler(int i) {
	return mouseHandlers.at(i);
}

MouseHandler* Console::GetMouseHandler(MouseHandler* h) {
	return GetMouseHandler(h->mId);
}

void Console::RemoveMouseHandler(int i) {
	mouseHandlers.at(i)->mId = -1;
	mouseHandlers.at(i) = nullptr;
}

void Console::RemoveMouseHandler(MouseHandler* h) {
	RemoveMouseHandler(h->mId);
}

}