#pragma once

#define NOMINMAX

#include <Windows.h>
#include <chrono>
#include <vector>
#include <string>
#include <algorithm>
#include <functional>
#include <future>
#include <thread>
#include <atomic>
#include "Elements.h"

namespace gui {

class MouseHandler;
class KeyboardHandler;
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
	} mKeyboard[256] = { 0 }, mMouseButtons[3] = { 0 };

	COORD mMousePosition;

	WCHAR mBaseChar = L' ';
	WORD mBaseColor = 0;

	std::vector<Element*> mElements;

	std::vector<MouseHandler*> mMouseHandlers;

	KeyboardHandler* mActiveKeyboardHandler = nullptr;
public:
	Console();
	virtual ~Console();
	void CreateConsole(int sW, int sH, int cW, int cH);

	void Set(int x, int y, WCHAR chr, WORD clr);
	void Fill(WCHAR chr, WORD clr);
	void Rect(RECT r, WCHAR chr, WORD clr, bool fill = false);
	void Write(int x, int y, std::wstring str, WORD clr);

	void Run();
	void Stop();

	virtual bool Initialize() = 0;

	const int& GetScreenWidth() const { return mScreenWidth; }
	const int& GetScreenHeight() const { return mScreenHeight; }

	const COORD& GetMousePosition() const { return mMousePosition; }

	const WCHAR& GetBaseChar() const { return mBaseChar; }
	void SetBaseChar(WCHAR c) { mBaseChar = c; }
	const WORD& GetBaseColor() const { return mBaseColor; }
	void SetBaseColor(WORD c) { mBaseColor = c; }

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

	KeyboardHandler* GetActiveKeyboardHandler() const { return mActiveKeyboardHandler; }
	void SetActiveKeyboardHandler(KeyboardHandler* h) { mActiveKeyboardHandler = h; }

	void ApplyToElements(std::function<void(Element*)> f) { for (Element* e : mElements) f(e); }
};

inline void RunAfterDelay(int ms, std::function<void()> f) {
	std::thread t([ms, f] { std::this_thread::sleep_for(std::chrono::milliseconds(ms)); f(); });
	t.detach();
}

inline std::future<void> ExecuteAsync(std::function<void()> f) {
	std::future<void> r = std::async(std::launch::async, f);
	return r;
}

}