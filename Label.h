#pragma once
#include "Element.h"

namespace gui {

class Label : public Element {
public:
	std::wstring text = L"";

	int alignH = TEXT_ALIGN_MID;
	int alignV = TEXT_ALIGN_MID;
	int textWrap = TEXT_WRAP_WORD;
	
	Label(Rect b);
	Label(Rect b, std::wstring t, bool fit = false);
	Label(const Label& e);

	virtual void Autosize() override;
	virtual void Draw(Window* w) override;
};

}