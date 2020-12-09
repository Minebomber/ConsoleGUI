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
#include "Window.h"
#include "Bounds.h"

namespace gui {

class MouseHandler;
class KeyboardHandler;
class Element;
class Window;

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

	bool mInitialized = false;
	bool mRunning = false;

	bool mFocused = true;

	Window* mCurrentWindow = nullptr;
public:
	Console();
	virtual ~Console();
	void CreateConsole(int sW, int sH, int cW, int cH);

	Window* GetCurrentWindow() { return mCurrentWindow; }
	void SetCurrentWindow(Window* w);

	void Run();
	void Stop();

	virtual bool Initialize() = 0;

	const int& GetScreenWidth() const { return mScreenWidth; }
	const int& GetScreenHeight() const { return mScreenHeight; }
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