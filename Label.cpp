#include "Label.h"

namespace gui {

Label::Label(Rect b) : View(b) {}

Label::Label(Rect b, std::wstring t, bool fit) : View(b), text(t) {
	if (fit) Autosize();
}

Label::Label(const Label& e) : 
	View(e), text(e.text), alignH(e.alignH), alignV(e.alignV), wrap(e.wrap) {}

void Label::Autosize() {
	int w = 0, h = 0, l = text.length();
	for (int i = 0, nl = 0; i < l; i++) {
		if (i == l - 1 || text[i] == L'\n') {
			if (i - nl >= w) { w = i - nl + 1; nl = i; }
			h++;
		}
	}

	bounds = { 
		bounds.x, 
		bounds.y, 
		w + 2 * style.borders + padding.TotalX(), 
		h + 2 * style.borders + padding.TotalY()
	};
}

void Label::Draw(Window* w) {
	View::Draw(w);
	w->RenderText(
		InnerBounds(), 
		text, 
		CurrentForeground().value | CurrentBackground().value << 4, 
		alignH, 
		alignV, 
		wrap
	);
}

}