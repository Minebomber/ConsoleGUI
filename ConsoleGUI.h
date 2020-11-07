#pragma once

#define NOMINMAX

#include <Windows.h>
#include <chrono>
#include <vector>
#include <string>
#include <algorithm>
#include <functional>
#include "Colors.h"

class MouseHandler;
class GUIElement;
#include "GUIElements.h"

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