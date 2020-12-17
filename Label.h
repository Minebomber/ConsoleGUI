#pragma once
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

class Label : public Element {
	friend class Window;
protected:
	std::wstring mText = L"";

	int mAlignH = TEXT_ALIGN_MID;
	int mAlignV = TEXT_ALIGN_MID;
	int mTextWrap = TEXT_WRAP_WORD;

	void RenderText(Window* w, Rect r, const std::wstring& s, WORD cl);
public:
	Label(Rect b);
	Label(Rect b, std::wstring t, bool fit = false);
	Label(const Label& e);

	void Autosize();

	const std::wstring& GetText() const { return mText; }
	void SetText(std::wstring t) { mText = t; }

	const int& GetAlignHorizontal() const { return mAlignH; }
	void SetAlignHorizontal(int h) { mAlignH = h; }

	const int& GetAlignVertical() const { return mAlignV; }
	void SetAlignVertical(int v) { mAlignV = v; }

	const int& GetTextWrap() const { return mTextWrap; }
	void SetTextWrap(int w) { mTextWrap = w; }

	virtual void SetBorders(bool b) override { Element::SetBorders(b); Autosize(); }

	virtual void Draw(Window* w);
};

}