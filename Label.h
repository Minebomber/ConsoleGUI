#pragma once
#include "Element.h"

namespace gui {

class Label : public View {
public:
	std::wstring text = L"";

	Alignment alignH = Alignment::Mid;
	Alignment alignV = Alignment::Mid;
	TextWrap wrap = TextWrap::Word;
	
	Label(Rect b);
	Label(Rect b, std::wstring t, bool fit = false);
	Label(const Label& e);

	virtual void Autosize() override;
	virtual void Draw(Window* w) override;
};

}