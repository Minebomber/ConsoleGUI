#pragma once

#include <vector>
#include <unordered_map>
#include <typeinfo>
#include <typeindex>

#include "Element.h"
#include "ElementStyle.h"

namespace gui {

enum TextAlignment {
	TEXT_ALIGN_MIN,
	TEXT_ALIGN_MID,
	TEXT_ALIGN_MAX,
};

enum TextWrap {
	TEXT_WRAP_CHAR,
	TEXT_WRAP_WORD,
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

	bool mKeyboard[256] = { 0 };
	bool mMouseButtons[3] = { 0 };

	CHAR_INFO* mBuffer = nullptr;
	std::unordered_map<std::type_index, Element::Style*> mStyleMap;
public:
	Element* focusedElement = nullptr;
	Point mousePosition;

	WCHAR baseChar = L' ';
	WORD baseColor = 0;
	
	std::function<void(void)> onHideCallback;
	std::function<void(void)> onShowCallback;

	Window(int w, int h) : mWidth(w), mHeight(h), mBuffer(new CHAR_INFO[w * h]) {}
	Window(int w, int h, Element::Style* defStyle);

	virtual ~Window();

	void SetChar(int x, int y, WCHAR chr, WORD clr);
	void FillScreen(WCHAR chr, WORD clr);
	void DrawRect(Rect r, WCHAR chr, WORD clr, bool fill = false);
	void WriteString(int x, int y, const std::wstring& str, WORD clr);
	void WriteString(int x, int y, const std::wstring& str, WORD clr, int st, int w);
	void RenderText(Rect r, const std::wstring& txt, WORD clr, int alignH, int alignV, int wrap);

	void ApplyStyle(Element* e);

	template <typename T> Element::Style* GetStyle() {
		if (auto s = mStyleMap[std::type_index(typeid(T))]) return s;
		else return mStyleMap[std::type_index(typeid(Element))];
	}

	template <typename T> Element::Style* GetStyle(const T& e) {
		if (auto s = mStyleMap[std::type_index(typeid(e))]) return s;
		else return mStyleMap[std::type_index(typeid(Element))];
	}

	template <typename T> void SetStyle(Element::Style* s) {
		mStyleMap[std::type_index(typeid(T))] = s;
	}

	template <typename T> void SetStyle(T e, Element::Style* s) {
		mStyleMap[std::type_index(typeid(e))] = s;
	}

	void AddElement(Element* e, bool applyStyle = true, bool postAutosize = true);
	void RemoveElement(Element* e);

	void ApplyToElements(std::function<void(Element*)> f) { for (Element* e : mElements) f(e); }

	Element* GetElementAtPoint(const Point& p);

	void Display();

	void OnHide() { if (onHideCallback) onHideCallback(); }
	void OnShow() { if (onShowCallback) onShowCallback(); }
};

}


