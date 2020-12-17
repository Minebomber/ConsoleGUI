#include "Checkbox.h"

namespace gui {

Checkbox::Checkbox(Rect b) : Label(b) { mAlignH = TEXT_ALIGN_MIN; mAlignV = TEXT_ALIGN_MIN; Init(); }
Checkbox::Checkbox(const Checkbox& e) : Label(e), mChecked(e.mChecked) { mAlignH = TEXT_ALIGN_MIN; mAlignV = TEXT_ALIGN_MIN; Init(); }

void Checkbox::Init() {
	EventHandler* h = new EventHandler();
	h->SetMouseDownAction([this](Window* w, int m) { mState = ELEMENT_FOCUSED; });
	h->SetMouseUpAction([this](Window* w, int m) { mState = ELEMENT_DEFAULT; mChecked = !mChecked; });
	AddEventHandler(h);
}

void Checkbox::Autosize() {
	Label::Autosize();
	Rect r = mBounds;
	r.SetWidth(r.GetWidth() + 4);
	SetBounds(r);
}

void Checkbox::Draw(Window* w) {
	Element::Draw(w);
	Rect textBounds = GetInnerBounds();
	WORD cl = GetCurrentForeground().Foreground() | GetCurrentBackground().Background();
	w->WriteString(
		textBounds.GetX(), 
		textBounds.GetY(), 
		mChecked ? L"[\x25CF]" : L"[ ]", 
		cl
	);
	textBounds.SetX(textBounds.GetX() + 4);
	textBounds.SetWidth(textBounds.GetWidth() - 4);
	w->RenderText(textBounds, mText, cl, mAlignH, mAlignV, mTextWrap);
}

}