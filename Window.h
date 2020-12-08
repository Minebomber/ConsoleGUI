#pragma once

#include <vector>
#include "Elements.h"

namespace gui {

class WindowScheme {
protected:
	WORD mText;
	WORD mBackground;
	WORD mBorder;
	
	WORD mPressedText;
	WORD mPressedBackground;
	WORD mPressedBorder;

	WORD mDisabledText;
	WORD mDisabledBackground;
	WORD mDisabledBorder;

	bool mBorders;
public:
	WindowScheme(
		WORD t, WORD bg, WORD bd, 
		WORD pt, WORD pbg, WORD pbd, 
		WORD dt, WORD dbg, WORD dbd, 
		bool bds
	) : mText(t), mBackground(bg), mBorder(bd), mPressedText(pt), mPressedBackground(pbg),
		mPressedBorder(pbd), mDisabledText(dt), mDisabledBackground(dbg), mDisabledBorder(dbd),
		mBorders(bds) {}

	const WORD& GetTextColor() const { return mText; }
	void SetTextColor(WORD c) { mText = c; }
	const WORD& GetBackgroundColor() const { return mBackground; }
	void SetBackgroundColor(WORD c) { mBackground = c; }
	const WORD& GetBorderColor() const { return mBorder; }
	void SetBorderColor(WORD c) { mBorder = c; }
	const int GetBorderWidth() const { return mBorders; }

	const WORD& GetPressedTextColor() const { return mPressedText; }
	void SetPressedTextColor(WORD c) { mPressedText = c; }
	const WORD& GetPressedBackgroundColor() const { return mPressedBackground; }
	void SetPressedBackgroundColor(WORD c) { mPressedBackground = c; }
	const WORD& GetPressedBorderColor() const { return mPressedBorder; }
	void SetPressedBorderColor(WORD c) { mPressedBorder = c; }
	const int GetPressedBorderWidth() const { return mBorders; }

	const WORD& GetDisabledTextColor() const { return mDisabledText; }
	void SetDisabledTextColor(WORD c) { mDisabledText = c; }
	const WORD& GetDisabledBackgroundColor() const { return mDisabledBackground; }
	void SetDisabledBackgroundColor(WORD c) { mDisabledBackground = c; }
	const WORD& GetDisabledBorderColor() const { return mDisabledBorder; }
	void SetDisabledBorderColor(WORD c) { mDisabledBorder = c; }
	const int GetDisabledBorderWidth() const { return mBorders; }

	static WindowScheme* Default();
	static WindowScheme* Green();
	static WindowScheme* Red();
};

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

	std::function<void(void)> mOnHideCallback;
	std::function<void(void)> mOnShowCallback;

	WindowScheme* mColorScheme = nullptr;
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

	WindowScheme* GetColorScheme() { return mColorScheme; }
	void SetColorScheme(WindowScheme* cs) { if (mColorScheme) delete mColorScheme; mColorScheme = cs; }

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

	void OnHide() { if (mOnHideCallback) mOnHideCallback(); }
	void OnShow() { if (mOnShowCallback) mOnShowCallback(); }

	void SetOnHideCallback(std::function<void(void)> f) { mOnHideCallback = f; }
	void SetOnShowCallback(std::function<void(void)> f) { mOnShowCallback = f; }
};

}


