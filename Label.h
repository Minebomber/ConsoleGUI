#pragma once
#include "Element.h"

namespace gui {

class Label : public Element {
	friend class Window;
protected:
	std::wstring mText = L"";

	int mAlignH = TEXT_ALIGN_MID;
	int mAlignV = TEXT_ALIGN_MID;
	int mTextWrap = TEXT_WRAP_WORD;
public:
	Label(Rect b);
	Label(Rect b, std::wstring t, bool fit = false);
	Label(const Label& e);

	virtual void Autosize() override;

	const std::wstring& GetText() const { return mText; }
	void SetText(std::wstring t, bool fit = false) { mText = t; if (fit) Autosize(); }

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