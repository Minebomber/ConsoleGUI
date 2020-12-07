#pragma once

#include <vector>
#include "Elements.h"

namespace gui {

class Element;
class MouseHandler;
class KeyboardHandler;

class Window {
	friend class Console;
protected:
	int mWidth = 0;
	int mHeight = 0;

	std::vector<Element*> mElements;

	std::vector<MouseHandler*> mMouseHandlers;

	KeyboardHandler* mActiveKeyboardHandler = nullptr;

	struct KeyState {
		bool pressed;
		bool released;
		bool held;
	} mKeyboard[256] = { 0 }, mMouseButtons[3] = { 0 };

	COORD mMousePosition{ 0 };

	WCHAR mBaseChar = L' ';
	WORD mBaseColor = 0;

	CHAR_INFO* mBuffer = nullptr;
public:
	Window(int w, int h) : mWidth(w), mHeight(h), mBuffer(new CHAR_INFO[w * h]) {}

	void Set(int x, int y, WCHAR chr, WORD clr);
	void Fill(WCHAR chr, WORD clr);
	void Rect(RECT r, WCHAR chr, WORD clr, bool fill = false);
	void Write(int x, int y, std::wstring str, WORD clr);

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

	void Display();
};

}


