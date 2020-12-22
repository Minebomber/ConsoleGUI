#pragma once

#include <vector>
#include <unordered_map>
#include <typeinfo>
#include <typeindex>

#include "Element.h"

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

class Window {
	friend class Console;
protected:
	std::vector<Element*> mElements;

	bool mKeyboard[256] = { 0 };
	bool mMouseButtons[3] = { 0 };

	CHAR_INFO* mBuffer = nullptr;
	std::unordered_map<std::type_index, Style*> mStyleMap;

	Element* SubElementAtPoint(Element* e, const Point& p);
	void ApplyToSubElements(Element* e, std::function<void(Element*)> f);
public:
	Rect bounds = { 0, 0 };

	// Which element gets keyboard, mousemove/drag events
	Element* focusedElement = nullptr;
	Point mousePosition;

	WCHAR baseChar = L' ';
	WORD baseColor = 0;
	
	std::function<void(void)> onHideCallback;
	std::function<void(void)> onShowCallback;

	Window(int w, int h) : bounds(0, 0, w, h), mBuffer(new CHAR_INFO[w * h]) {}
	Window(int w, int h, Style* defStyle);

	virtual ~Window();

	void SetChar(int x, int y, WCHAR chr, WORD clr);
	void FillScreen(WCHAR chr, WORD clr);
	void DrawRect(Rect r, WCHAR chr, WORD clr, bool fill = false);
	void WriteString(int x, int y, const std::wstring& str, WORD clr);
	void WriteString(int x, int y, const std::wstring& str, WORD clr, int st, int w);
	void RenderText(Rect r, const std::wstring& txt, WORD clr, int alignH, int alignV, int wrap);

	void ApplyStyle(Element* e);

	template <typename T> Style* GetStyle() {
		if (auto s = mStyleMap[std::type_index(typeid(T))]) return s;
		else return mStyleMap[std::type_index(typeid(Element))];
	}

	template <typename T> Style* GetStyle(const T& e) {
		if (auto s = mStyleMap[std::type_index(typeid(e))]) return s;
		else return mStyleMap[std::type_index(typeid(Element))];
	}

	template <typename T> void SetStyle(Style* s) {
		mStyleMap[std::type_index(typeid(T))] = s;
	}

	template <typename T> void SetStyle(T e, Style* s) {
		mStyleMap[std::type_index(typeid(e))] = s;
	}

	void AddElement(Element* e, bool applyStyle = true, bool postAutosize = true);
	void AddElements(std::initializer_list<Element*> es, bool applyStyle = true, bool postAutosize = true);
	void RemoveElement(Element* e);

	void ApplyToElements(std::function<void(Element*)> f, bool applyToSub = true) { 
		for (Element* e : mElements) {
			f(e);
			if (applyToSub) ApplyToSubElements(e, f);
		}
	}

	Element* GetElementAtPoint(const Point& p);

	void Display();

	void OnHide() { if (onHideCallback) onHideCallback(); }
	void OnShow() { if (onShowCallback) onShowCallback(); }
};

}


