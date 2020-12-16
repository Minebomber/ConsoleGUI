#pragma once

#include <vector>
#include <unordered_map>
#include <typeinfo>
#include <typeindex>

#include "Element.h"
#include "ElementStyle.h"

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

	std::unordered_map<std::type_index, ElementStyle*> mStyleMap;
public:
	Window(int w, int h) : mWidth(w), mHeight(h), mBuffer(new CHAR_INFO[w * h]) {}
	Window(int w, int h, ElementStyle* defStyle);

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

	Element* GetFocusedElement() const { return mFocusedElement; }
	void SetFocusedElement(Element* e) { mFocusedElement = e; }

	template <typename T> ElementStyle* GetStyle() {
		if (auto s = mStyleMap[std::type_index(typeid(T))]) return s;
		else return mStyleMap[std::type_index(typeid(Element))];
	}

	template <typename T> ElementStyle* GetStyle(const T& e) {
		if (auto s = mStyleMap[std::type_index(typeid(e))]) return s;
		else return mStyleMap[std::type_index(typeid(Element))];
	}

	template <typename T> void SetStyle(ElementStyle* s) {
		mStyleMap[std::type_index(typeid(T))] = s;
	}

	template <typename T> void SetStyle(T e, ElementStyle* s) {
		mStyleMap[std::type_index(typeid(e))] = s;
	}

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


