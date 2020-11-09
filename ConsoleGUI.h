#pragma once

#define NOMINMAX

#include <Windows.h>
#include <chrono>
#include <vector>
#include <string>
#include <algorithm>
#include <functional>
#include "Colors.h"
#include "GUIElements.h"

namespace gui {

class MouseHandler;
class Element;

class Console {
private:
	int mScreenWidth = 0;
	int mScreenHeight = 0;
	int mCharWidth = 0;
	int mCharHeight = 0;

	HANDLE mConsole = nullptr;
	HANDLE mConsoleIn = nullptr;

	CONSOLE_FONT_INFOEX mOriginalConsoleFontInfo;
	CONSOLE_SCREEN_BUFFER_INFO mOriginalConsoleScreenBufferInfo;
	CONSOLE_CURSOR_INFO mOriginalConsoleCursorInfo;

	SMALL_RECT mWindowRect{ 0, 0, 0, 0 };

	CHAR_INFO* mScreenBuffer = nullptr;

	bool mInitialized = false;
	bool mRunning = false;

	struct KeyState {
		bool pressed;
		bool released;
		bool held;
	} keyboard[256] = { 0 }, mouseButtons[5] = { 0 };

	COORD mMousePosition;

	WCHAR mBaseChar = L' ';
	WORD mBaseColor = BG_BLACK;

	std::vector<Element*> mElements;

	std::vector<MouseHandler*> mMouseHandlers;
public:
	Console();
	virtual ~Console();
	void CreateConsole(int sW, int sH, int cW, int cH);

	void Set(int x, int y, WCHAR chr, WORD clr);
	void Fill(WCHAR chr, WORD clr);
	void Rect(RECT r, WCHAR chr, WORD clr, bool fill = false);
	void Write(int x, int y, std::wstring str, WORD clr);

	void Run();

	virtual bool Initialize() = 0;

	const int& GetScreenWidth() const;
	const int& GetScreenHeight() const;

	const COORD& GetMousePosition() const;

	const WCHAR& GetBaseChar() const;
	void SetBaseChar(WCHAR c);
	const WORD& GetBaseColor() const;
	void SetBaseColor(WORD c);

	void AddElement(Element* e);
	Element* GetElement(int i);
	Element* GetElement(Element* e);
	void RemoveElement(int i);
	void RemoveElement(Element* e);

	void AddMouseHandler(MouseHandler* h);
	MouseHandler* GetMouseHandler(int i);
	MouseHandler* GetMouseHandler(MouseHandler* h);
	void RemoveMouseHandler(int i);
	void RemoveMouseHandler(MouseHandler* h);
};
}