#include "ProgressBar.h"

namespace gui {

ProgressBar::ProgressBar(Rect b) : Element(b) {}
ProgressBar::ProgressBar(Rect b, float p) : Element(b), progress(p) {}
ProgressBar::ProgressBar(const ProgressBar& e) : Element(e), progress(e.progress) {}

void ProgressBar::Draw(Window* w) {
	Element::Draw(w);
	Rect displayArea = InnerBounds();
	w->DrawRect(displayArea, L'\x2591', style.defaultBackground.value, true);
	displayArea.width = (int)((float)displayArea.width * progress);
	w->DrawRect(displayArea, L'\x2588', style.defaultForeground.value, true);
}

}