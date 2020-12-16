#pragma once

#include <vector>

#include "Element.h"
#include "Styles.h"

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

	Element* mFocusedElement = nullptr;

	bool mKeyboard[256] = { 0 };
	bool mMouseButtons[3] = { 0 };

	Point mMousePosition;

	WCHAR mBaseChar = L' ';
	WORD mBaseColor = 0;

	CHAR_INFO* mBuffer = nullptr;

	std::function<void(void)> mOnHideCallback;
	std::function<void(void)> mOnShowCallback;

	StyleMap mStyleMap;
public:
	Window(int w, int h) : mWidth(w), mHeight(h), mBuffer(new CHAR_INFO[w * h]) {}
	Window(int w, int h, ElementStyle* defStyle);

	virtual ~Window();

	template <typename T>
	ElementStyle* GetStyle();

	template <typename T>
	void SetStyle(ElementStyle* s) { mStyleMap.SetStyle<T>(s); }

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

	Element* GetFocusedElement() const { return mFocusedElement; }
	void SetFocusedElement(Element* e) { mFocusedElement = e; }

	void AddElement(Element* e, bool applyStyle = true);
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


