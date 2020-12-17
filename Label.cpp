#include "Label.h"

namespace gui {

Label::Label(Rect b) : Element(b) {}

Label::Label(Rect b, std::wstring t, bool fit) : Element(b), mText(t) {
	if (fit) Autosize();
}

Label::Label(const Label& e) : Element(e), mText(e.mText), mAlignH(e.mAlignH), mAlignV(e.mAlignV), mTextWrap(e.mTextWrap) {}

void Label::Autosize() {
	int w = 0, h = 0, l = mText.length();
	for (int i = 0, nl = 0; i < l; i++) {
		if (i == l - 1 || mText[i] == L'\n') {
			if (i - nl >= w) { w = i - nl + 1; nl = i; }
			h++;
		}
	}
	SetBounds({mBounds.GetX(), mBounds.GetY(), w + 2 * mBorders, h + 2 * mBorders});
}

void Label::Draw(Window* w) {
	Element::Draw(w);
	w->RenderText(GetInnerBounds(), mText, GetCurrentForeground().Foreground() | GetCurrentBackground().Background(), mAlignH, mAlignV, mTextWrap);
}

}