#pragma once

#include <vector>
#include "Elements.h"
#include "Rect.h"

namespace gui {

class WindowScheme {
protected:
	WORD mDefaultForeground;
	WORD mDefaultBackground;

	WORD mFocusedForeground;
	WORD mFocusedBackground;
	
	WORD mDisabledForeground;
	WORD mDisabledBackground;

	bool mBorderEnabled;
public:
	WindowScheme(
		WORD fg, WORD bg,
		WORD ffg, WORD fbg,
		WORD dfg, WORD dbg,
		bool bd
	) : mDefaultForeground(fg), mDefaultBackground(bg),
		mFocusedForeground(ffg), mFocusedBackground(fbg),
		mDisabledForeground(dfg), mDisabledBackground(dbg),
		mBorderEnabled(bd) {}

	const bool& GetBorderEnabled() const { return mBorderEnabled; }
	void SetBorderEnabled(bool e) { mBorderEnabled = e; }

	const WORD& GetDefaultForeground() const { return mDefaultForeground; }
	void SetDefaultForeground(WORD c) { mDefaultForeground = c; }
	const WORD& GetDefaultBackground() const { return mDefaultBackground; }
	void SetDefaultBackground(WORD c) { mDefaultBackground = c; }

	const WORD& GetFocusedForeground() const { return mFocusedForeground; }
	void SetFocusedForeground(WORD c) { mFocusedForeground = c; }
	const WORD& GetFocusedBackground() const { return mFocusedBackground; }
	void SetFocusedBackground(WORD c) { mFocusedBackground = c; }

	const WORD& GetDisabledForeground() const { return mDisabledForeground; }
	void SetDisabledForeground(WORD c) { mDisabledForeground = c; }
	const WORD& GetDisabledBackground() const { return mDisabledBackground; }
	void SetDisabledBackground(WORD c) { mDisabledBackground = c; }

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

	Element* mFocusedElement = nullptr;

	bool mKeyboard[256] = { 0 };
	bool mMouseButtons[3] = { 0 };

	Point mMousePosition{ 0 };

	WCHAR mBaseChar = L' ';
	WORD mBaseColor = 0;

	CHAR_INFO* mBuffer = nullptr;

	std::function<void(void)> mOnHideCallback;
	std::function<void(void)> mOnShowCallback;

	WindowScheme* mScheme = nullptr;
public:
	Window(int w, int h) : mWidth(w), mHeight(h), mBuffer(new CHAR_INFO[w * h]) {}
	virtual ~Window();

	void SetChar(int x, int y, WCHAR chr, WORD clr);
	void FillScreen(WCHAR chr, WORD clr);
	void DrawRect(Rect r, WCHAR chr, WORD clr, bool fill = false);
	void WriteString(int x, int y, const std::wstring& str, WORD clr);
	void WriteString(int x, int y, const std::wstring& str, WORD clr, int st, int w);

	const Point& GetMousePosition() const;

	const WCHAR& GetBaseChar() const { return mBaseChar; }
	void SetBaseChar(WCHAR c) { mBaseChar = c; }
	const WORD& GetBaseColor() const { return mBaseColor; }
	void SetBaseColor(WORD c) { mBaseColor = c; }

	WindowScheme* GetScheme() { return mScheme; }
	void SetScheme(WindowScheme* cs) { if (mScheme) delete mScheme; mScheme = cs; }

	Element* GetFocusedElement() const { return mFocusedElement; }
	void SetFocusedElement(Element* e) { mFocusedElement = e; }

	void AddElement(Element* e);
	void RemoveElement(Element* e);

	void ApplyToElements(std::function<void(Element*)> f) { for (Element* e : mElements) f(e); }

	Element* GetElementAtPoint(const Point& p);

	void Display();

	void OnHide() { if (mOnHideCallback) mOnHideCallback(); }
	void OnShow() { if (mOnShowCallback) mOnShowCallback(); }

	void SetOnHideCallback(std::function<void(void)> f) { mOnHideCallback = f; }
	void SetOnShowCallback(std::function<void(void)> f) { mOnShowCallback = f; }
};

}


