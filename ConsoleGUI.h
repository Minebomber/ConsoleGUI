#pragma once

#define NOMINMAX

#include <Windows.h>
#include <chrono>
#include <vector>
#include <string>
#include <algorithm>
#include <functional>

enum Color {
	FG_BLACK = 0x0000,
	FG_DARK_BLUE = 0x0001,
	FG_DARK_GREEN = 0x0002,
	FG_DARK_CYAN = 0x0003,
	FG_DARK_RED = 0x0004,
	FG_DARK_MAGENTA = 0x0005,
	FG_DARK_YELLOW = 0x0006,
	FG_GREY = 0x0007, // Thanks MS :-/
	FG_DARK_GREY = 0x0008,
	FG_BLUE = 0x0009,
	FG_GREEN = 0x000A,
	FG_CYAN = 0x000B,
	FG_RED = 0x000C,
	FG_MAGENTA = 0x000D,
	FG_YELLOW = 0x000E,
	FG_WHITE = 0x000F,
	BG_BLACK = 0x0000,
	BG_DARK_BLUE = 0x0010,
	BG_DARK_GREEN = 0x0020,
	BG_DARK_CYAN = 0x0030,
	BG_DARK_RED = 0x0040,
	BG_DARK_MAGENTA = 0x0050,
	BG_DARK_YELLOW = 0x0060,
	BG_GREY = 0x0070,
	BG_DARK_GREY = 0x0080,
	BG_BLUE = 0x0090,
	BG_GREEN = 0x00A0,
	BG_CYAN = 0x00B0,
	BG_RED = 0x00C0,
	BG_MAGENTA = 0x00D0,
	BG_YELLOW = 0x00E0,
	BG_WHITE = 0x00F0,
};

struct KeyState {
	bool pressed;
	bool released;
	bool held;
};

class GUIElement;

class EventHandler {
public:
	int id = -1;
	std::function<void(int)> OnPress;
	std::function<void(int)> OnRelease;
};

enum MouseButtons {
	MOUSE_LEFT_BUTTON = 0b001,
	MOUSE_RIGHT_BUTTON = 0b010,
	MOUSE_CENTER_BUTTON = 0b100,
};

class MouseHandler : public EventHandler {
public:
	RECT bounds = { 0, 0, 0, 0 };
	int buttons = 0;
};

class ConsoleGUI {
private:
	int screenWidth = 0;
	int screenHeight = 0;
	int charWidth = 0;
	int charHeight = 0;

	HANDLE console = nullptr;
	HANDLE consoleIn = nullptr;

	CONSOLE_FONT_INFOEX originalConsoleFontInfo;
	CONSOLE_SCREEN_BUFFER_INFO originalConsoleScreenBufferInfo;
	CONSOLE_CURSOR_INFO originalConsoleCursorInfo;

	SMALL_RECT windowRect{ 0, 0, 0, 0 };

	CHAR_INFO* screenBuffer = nullptr;

	bool initialized = false;
	bool running = false;

	struct KeyState {
		bool pressed;
		bool released;
		bool held;
	} keyboard[256] = { 0 }, mouseButtons[5] = { 0 };

	COORD mousePosition;

	WCHAR baseChar = L' ';
	WORD baseColor = BG_BLACK;

	std::vector<GUIElement*> elements;

	std::vector<MouseHandler*> mouseHandlers;

public:
	ConsoleGUI();
	virtual ~ConsoleGUI();
	void CreateConsole(int sW, int sH, int cW, int cH);

	void Set(int x, int y, WCHAR chr, WORD clr);
	void Fill(WCHAR chr, WORD clr);
	void Rect(RECT r, WCHAR chr, WORD clr, bool fill = false);
	void Write(int x, int y, std::wstring str, WORD clr);

	void Run();

	virtual bool Initialize() = 0;

	void SetBaseChar(WCHAR c);
	WCHAR GetBaseChar();
	void SetBaseColor(WORD c);
	WORD GetBaseColor();

	void AddElement(GUIElement* e);
	GUIElement* GetElement(int i);
	GUIElement* GetElement(GUIElement* e);
	void RemoveElement(int i);
	void RemoveElement(GUIElement* e);

	void AddMouseHandler(MouseHandler* h);
	MouseHandler* GetMouseHandler(int i);
	MouseHandler* GetMouseHandler(MouseHandler* h);
	void RemoveMouseHandler(int i);
	void RemoveMouseHandler(MouseHandler* h);
	
};

class GUIBorder {
public:
	WCHAR chr = L' ';
	WORD color = BG_WHITE;
	int width = 1;

	GUIBorder();
	GUIBorder(WCHAR ch, WORD cl);
	GUIBorder(WCHAR ch, WORD cl, int w);
	GUIBorder(int w);

	operator bool() const;

	virtual void Draw(ConsoleGUI* g, RECT bd);
};

class GUIElement {
public:
	int id = -1;
	RECT bounds = { 0, 0, 0, 0 };
	WCHAR background = L' ';
	WORD backgroundColor = BG_WHITE;
	GUIBorder border = { 0 };

	GUIElement(RECT b);
	GUIElement(const GUIElement& e);
	virtual ~GUIElement();
	virtual void Draw(ConsoleGUI* g);
};

enum TextAlignment {
	TEXT_ALIGN_MIN,
	TEXT_ALIGN_MID,
	TEXT_ALIGN_MAX,
};

enum TextWrap {
	WRAP_CHAR,
	WRAP_NEWLINE,
	WRAP_WORD,
};

class GUILabel : public GUIElement {
protected:
	void RenderText(ConsoleGUI* g, int minX, int maxX, int minY, int maxY, WORD c);
public:
	std::wstring text = L"";
	WORD textColor = FG_WHITE;
	int hAlignment = TEXT_ALIGN_MIN;
	int vAlignment = TEXT_ALIGN_MIN;

	GUILabel(RECT b);
	GUILabel(const GUILabel& e);

	virtual void Draw(ConsoleGUI* g) override;
};

class GUIButton : public GUILabel {
private:
	void SetupHandler();
public:
	WORD pressedTextColor = FG_WHITE;
	WCHAR pressedBackground = L' ';
	WORD pressedBackgroundColor = BG_WHITE;
	GUIBorder pressedBorder = { 0 };
	bool pressed = false;

	MouseHandler handler;
	
	std::function<void(int)> OnPress;
	std::function<void(int)> OnRelease;

	GUIButton(RECT b);

	virtual void Draw(ConsoleGUI* g) override;
};

