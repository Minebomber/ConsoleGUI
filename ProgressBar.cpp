#include "ProgressBar.h"

namespace gui {

ProgressBar::ProgressBar(Rect b) : Element(b) {}
ProgressBar::ProgressBar(Rect b, float p) : Element(b), progress(p) {}
ProgressBar::ProgressBar(const ProgressBar& e) : Element(e), progress(e.progress) {}

void ProgressBar::Autosize() {
	int w = bounds.width;
	Element::Autosize();
	bounds.width = w;
	bounds.height++;
}

void ProgressBar::Draw(Window* w) {
	Element::Draw(w);
	Rect displayArea = InnerBounds();
	w->DrawRect(displayArea, L'\x2591', style.defaultBackground.value, true);
	displayArea.width = std::max(
		0, 
		std::min((int)((float)displayArea.width * progress), displayArea.width)
	);
	w->DrawRect(displayArea, L'\x2588', style.defaultForeground.value, true);
}

}