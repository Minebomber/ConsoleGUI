#include "ConsoleGUI.h"

ConsoleGUI::ConsoleGUI() {
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

ConsoleGUI::~ConsoleGUI() {
	// Set console to default
	SetCurrentConsoleFontEx(console, false, &originalConsoleFontInfo);
	SetConsoleScreenBufferSize(console, originalConsoleScreenBufferInfo.dwSize);
	SetConsoleWindowInfo(console, true, &originalConsoleScreenBufferInfo.srWindow);
	originalConsoleCursorInfo.bVisible = true;
	SetConsoleCursorInfo(console, &originalConsoleCursorInfo);
}

void ConsoleGUI::CreateConsole(int sW, int sH, int cW, int cH) {
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

void ConsoleGUI::Set(int x, int y, WCHAR chr, WORD clr) {
	screenBuffer[y * screenWidth + x].Char.UnicodeChar = chr;
	screenBuffer[y * screenWidth + x].Attributes = clr;
}

void ConsoleGUI::Fill(WCHAR chr, WORD clr) {
	for (int i = 0; i < screenWidth * screenHeight; i++) {
		screenBuffer[i].Char.UnicodeChar = chr;
		screenBuffer[i].Attributes = clr;
	}
}

void ConsoleGUI::Rect(RECT r, WCHAR chr, WORD clr, bool fill) {
	for (int i = r.top; i <= r.bottom; i++) {
		for (int j = r.left; j <= r.right; j++) {
			if (fill) Set(j, i, chr, clr);
			else if (i == r.top || i == r.bottom || j == r.left || j == r.right) Set(j, i, chr, clr);
		}
	}
}

void ConsoleGUI::Write(int x, int y, std::wstring str, WORD clr) {
	for (size_t i = 0; i < str.size(); i++) {
		screenBuffer[y * screenWidth + x + i].Char.UnicodeChar = str[i];
		screenBuffer[y * screenWidth + x + i].Attributes = clr;
	}
}

void ConsoleGUI::Run() {
	if (!initialized) return;

	if (!Initialize()) return;

	auto timePoint1 = std::chrono::system_clock::now();
	auto timePoint2 = std::chrono::system_clock::now();

	SHORT oldKeyStates[256] = { 0 };
	SHORT newKeyStates[256] = { 0 };
	bool oldMouseStates[3] = { 0 };
	bool newMouseStates[3] = { 0 };

	running = true;
	while (running) {
		// Time
		timePoint2 = std::chrono::system_clock::now();
		std::chrono::duration<float> timeDelta = timePoint2 - timePoint1;
		timePoint1 = timePoint2;
		float elapsedTime = timeDelta.count();

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
					if (h->GetButtons() & (1 << m) &&
						mousePosition.X >= h->GetBounds().left &&
						mousePosition.X <= h->GetBounds().right &&
						mousePosition.Y >= h->GetBounds().top &&
						mousePosition.Y <= h->GetBounds().bottom) {
						if (mouseButtons[m].pressed && h->OnPressExists()) h->InvokeOnPress(m);
						if (mouseButtons[m].released && h->OnReleaseExists()) h->InvokeOnRelease(m);
					}
				}
			}
			oldMouseStates[m] = newMouseStates[m];
		}

		Fill(baseChar, baseColor);
		for (GUIElement* e : elements) {
			e->Draw(this);
		}

		// Display
		WCHAR title[256];
		swprintf(title, 256, L"FPS: %3.0f", 1 / elapsedTime);
		SetConsoleTitle(title);
		WriteConsoleOutput(console, screenBuffer, { (SHORT)screenWidth, (SHORT)screenHeight }, { 0, 0 }, &windowRect);
	}
}

const int& ConsoleGUI::GetScreenWidth() const { return screenWidth; }
const int& ConsoleGUI::GetScreenHeight() const { return screenHeight; }

const WCHAR& ConsoleGUI::GetBaseChar() const { return baseChar; }
void ConsoleGUI::SetBaseChar(WCHAR c) { baseChar = c; }

const WORD& ConsoleGUI::GetBaseColor() const { return baseColor; }
void ConsoleGUI::SetBaseColor(WORD c) { baseColor = c; }

void ConsoleGUI::AddElement(GUIElement* e) {
	for (size_t i = 0; i < elements.size(); i++) {
		if (elements.at(i) == nullptr) {
			e->SetId(i);
			elements.at(i) = e;
			return;
		}
	}
	elements.push_back(e);
	e->SetId(elements.size() - 1);

	if (auto b = dynamic_cast<GUIButton*>(e)) AddMouseHandler(&b->GetHandler());
}

GUIElement* ConsoleGUI::GetElement(int i) {
	return elements.at(i);
}

GUIElement* ConsoleGUI::GetElement(GUIElement* e) {
	return GetElement(e->GetId());
}

void ConsoleGUI::RemoveElement(int i) {
	elements.at(i)->SetId(-1);
	elements.at(i) = nullptr;
}

void ConsoleGUI::RemoveElement(GUIElement* e) {
	RemoveElement(e->GetId());

	if (auto b = dynamic_cast<GUIButton*>(e)) RemoveMouseHandler(&b->GetHandler());
}

void ConsoleGUI::AddMouseHandler(MouseHandler* h) {
	for (size_t i = 0; i < mouseHandlers.size(); i++) {
		if (mouseHandlers.at(i) == nullptr) {
			h->SetId(i);
			mouseHandlers.at(i) = h;
			return;
		}
	}
	mouseHandlers.push_back(h);
	h->SetId(elements.size() - 1);
}

MouseHandler* ConsoleGUI::GetMouseHandler(int i) {
	return mouseHandlers.at(i);
}

MouseHandler* ConsoleGUI::GetMouseHandler(MouseHandler* h) {
	return GetMouseHandler(h->GetId());
}

void ConsoleGUI::RemoveMouseHandler(int i) {
	mouseHandlers.at(i)->SetId(1);
	mouseHandlers.at(i) = nullptr;
}

void ConsoleGUI::RemoveMouseHandler(MouseHandler* h) {
	RemoveMouseHandler(h->GetId());
}

