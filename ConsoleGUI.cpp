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
					if (h->buttons & (1 << m) &&
						mousePosition.X >= h->bounds.left &&
						mousePosition.X <= h->bounds.right &&
						mousePosition.Y >= h->bounds.top &&
						mousePosition.Y <= h->bounds.bottom) {
						if (mouseButtons[m].pressed && h->OnPress) h->OnPress(m);
						if (mouseButtons[m].released && h->OnRelease) h->OnRelease(m);
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

void ConsoleGUI::SetBaseChar(WCHAR c) { baseChar = c; }
WCHAR ConsoleGUI::GetBaseChar() { return baseChar; }
void ConsoleGUI::SetBaseColor(WORD c) { baseColor = c; }
WORD ConsoleGUI::GetBaseColor() { return baseColor; }

void ConsoleGUI::AddElement(GUIElement* e) {
	for (size_t i = 0; i < elements.size(); i++) {
		if (elements.at(i) == nullptr) {
			e->id = i;
			elements.at(i) = e;
			return;
		}
	}
	elements.push_back(e);
	e->id = elements.size() - 1;

	if (auto b = dynamic_cast<GUIButton*>(e)) AddMouseHandler(&b->handler);
}

GUIElement* ConsoleGUI::GetElement(int i) {
	return elements.at(i);
}

GUIElement* ConsoleGUI::GetElement(GUIElement* e) {
	return GetElement(e->id);
}

void ConsoleGUI::RemoveElement(int i) {
	elements.at(i)->id = -1;
	elements.at(i) = nullptr;
}

void ConsoleGUI::RemoveElement(GUIElement* e) {
	RemoveElement(e->id);

	if (auto b = dynamic_cast<GUIButton*>(e)) RemoveMouseHandler(&b->handler);
}

void ConsoleGUI::AddMouseHandler(MouseHandler* h) {
	for (size_t i = 0; i < mouseHandlers.size(); i++) {
		if (mouseHandlers.at(i) == nullptr) {
			h->id = i;
			mouseHandlers.at(i) = h;
			return;
		}
	}
	mouseHandlers.push_back(h);
	h->id = elements.size() - 1;
}

MouseHandler* ConsoleGUI::GetMouseHandler(int i) {
	return mouseHandlers.at(i);
}

MouseHandler* ConsoleGUI::GetMouseHandler(MouseHandler* h) {
	return GetMouseHandler(h->id);
}

void ConsoleGUI::RemoveMouseHandler(int i) {
	mouseHandlers.at(i)->id = -1;
	mouseHandlers.at(i) = nullptr;
}

void ConsoleGUI::RemoveMouseHandler(MouseHandler* h) {
	RemoveMouseHandler(h->id);
}

GUIBorder::GUIBorder() {}
GUIBorder::GUIBorder(WCHAR ch, WORD cl) : chr(ch), color(cl) {}
GUIBorder::GUIBorder(WCHAR ch, WORD cl, int w) : chr(ch), color(cl), width(w) {}
GUIBorder::GUIBorder(int w) : width(w) {}

GUIBorder::operator bool() const { return width != 0; }

void GUIBorder::Draw(ConsoleGUI* g, RECT bd) {
	for (int b = 0; b < width; b++) g->Rect({ bd.left + b, bd.top + b, bd.right - b, bd.bottom - b }, chr, color, false);
}

GUIElement::GUIElement(RECT b) : bounds(b) {}
GUIElement::GUIElement(const GUIElement& e) : bounds(e.bounds), background(e.background), backgroundColor(e.backgroundColor) {}
GUIElement::~GUIElement() {}

void GUIElement::Draw(ConsoleGUI* g) {
	g->Rect(bounds, background, backgroundColor, true);
	border.Draw(g, bounds);
}

GUILabel::GUILabel(RECT b) : GUIElement(b) {}
GUILabel::GUILabel(const GUILabel& e) : GUIElement(e), text(e.text), textColor(e.textColor), hAlignment(e.hAlignment), vAlignment(e.vAlignment) {}

void GUILabel::RenderText(ConsoleGUI* g, int minX, int maxX, int minY, int maxY, WORD c) {
	int yOffset = 0; // default to min
	switch (vAlignment) {
	case TEXT_ALIGN_MID:
		yOffset = (maxY - minY - std::count(text.begin(), text.end(), L'\n') + 1) / 2;
		break;
	case TEXT_ALIGN_MAX:
		yOffset = maxY - minY - std::count(text.begin(), text.end(), L'\n');
	default:
		break;
	}

	int y = minY + yOffset;

	int nlPos = text.find(L'\n');
	int cIdx = 0;

	while (cIdx < text.length() && y <= maxY) {
		int elIdx = (nlPos == std::string::npos) ? text.length() : nlPos;
		int lineLen = elIdx - cIdx;

		int xOffset = 0;
		switch (hAlignment) {
		case TEXT_ALIGN_MID:
			xOffset = (maxX - minX - lineLen + 1) / 2;
			break;
		case TEXT_ALIGN_MAX:
			xOffset = maxX - minX - lineLen + 1;
		default:
			break;
		}

		g->Write(minX + xOffset, y, text.substr(cIdx, lineLen), c);

		y++;
		cIdx = elIdx + 1;
		nlPos = text.find(L'\n', cIdx);
	}
}

void GUILabel::Draw(ConsoleGUI* g) {
	GUIElement::Draw(g);
	RenderText(g, bounds.left + border.width, bounds.right - border.width, bounds.top + border.width, bounds.bottom - border.width, textColor);
}

GUIButton::GUIButton(RECT b) : GUILabel(b), handler(), OnPress(), OnRelease() { SetupHandler(); }

void GUIButton::SetupHandler() {
	handler.bounds = bounds;
	handler.OnPress = [this](int m) { pressed = true; if (OnPress) OnPress(m); };
	handler.OnRelease = [this](int m) { pressed = false; if (OnRelease) OnRelease(m); };
}

void GUIButton::Draw(ConsoleGUI* g) {
	WCHAR bg = pressed ? pressedBackground : background;
	WORD bgC = pressed ? pressedBackgroundColor : backgroundColor;
	WORD tC = pressed ? pressedTextColor : textColor;

	g->Rect(bounds, bg, bgC, true);
	if (pressed) pressedBorder.Draw(g, bounds);
	else border.Draw(g, bounds);
	
	RenderText(g,
			   bounds.left + (pressed ? pressedBorder.width : border.width),
			   bounds.right - (pressed ? pressedBorder.width : border.width),
			   bounds.top + (pressed ? pressedBorder.width : border.width),
			   bounds.bottom - (pressed ? pressedBorder.width : border.width),
			   tC);

}