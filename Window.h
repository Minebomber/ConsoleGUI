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

class View;

class Window {
	friend class Console;
protected:
	//std::vector<View*> mElements;

	bool mKeyboard[256] = { 0 };
	bool mMouseButtons[3] = { 0 };

	CHAR_INFO* mBuffer = nullptr;
	std::unordered_map<std::type_index, Style*> mStyleMap;

	Point mDrawOffset = { 0, 0 };

	//View* SubElementAtPoint(, const Point& p);
	//void ApplyToSubElements(View* e, std::function<void(View*)> f);
public:
	View* view;

	//Rect bounds = { 0, 0 };

	// Which element gets keyboard, mousemove/drag events
	View* focusedElement = nullptr;
	Point mousePosition;

	WCHAR baseChar = L' ';
	WORD baseColor = 0;
	
	std::function<void(void)> onHideCallback;
	std::function<void(void)> onShowCallback;

	Window(int w, int h);
	Window(int w, int h, Style* defStyle);

	virtual ~Window();

	void SetChar(int x, int y, WCHAR chr, WORD clr);
	void FillScreen(WCHAR chr, WORD clr);
	void DrawRect(Rect r, WCHAR chr, WORD clr, bool fill = false);
	void WriteString(int x, int y, const std::wstring& str, WORD clr);
	void WriteString(int x, int y, const std::wstring& str, WORD clr, int st, int w);
	void RenderText(Rect r, const std::wstring& txt, WORD clr, int alignH, int alignV, int wrap);

	void PushOffset(const Point& p) { mDrawOffset += p; }
	void PopOffset(const Point& p) { mDrawOffset -= p; }

	void ApplyStyle(View* v, bool applyToSub = false);

	template <typename T> Style* GetStyle() {
		if (auto s = mStyleMap[std::type_index(typeid(T))]) return s;
		else return mStyleMap[std::type_index(typeid(View))];
	}

	template <typename T> Style* GetStyle(const T& e) {
		if (auto s = mStyleMap[std::type_index(typeid(e))]) return s;
		else return mStyleMap[std::type_index(typeid(View))];
	}

	template <typename T> void SetStyle(Style* s) {
		mStyleMap[std::type_index(typeid(T))] = s;
	}

	template <typename T> void SetStyle(T e, Style* s) {
		mStyleMap[std::type_index(typeid(e))] = s;
	}

	//void AddElement(View* e, bool applyStyle = true, bool postAutosize = true);
	//void AddElements(std::initializer_list<View*> es, bool applyStyle = true, bool postAutosize = true);
	//void RemoveElement(View* e);
	
	/*void ApplyToElements(std::function<void(View*)> f, bool applyToSub = true) { 
		for (View* e : mElements) {
			f(e);
			if (applyToSub) ApplyToSubElements(e, f);
		}
	}*/

	void ApplyToAllViews(std::function<void(View*)> f, View* root = nullptr);

	View* ViewAtPoint(Point p, View* root = nullptr);

	void Display();

	void OnHide() { if (onHideCallback) onHideCallback(); }
	void OnShow() { if (onShowCallback) onShowCallback(); }
};

}


