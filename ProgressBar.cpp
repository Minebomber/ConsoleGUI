#include "ProgressBar.h"

namespace gui {

ProgressBar::ProgressBar(Rect b) : Element(b) {}
ProgressBar::ProgressBar(Rect b, float p) : Element(b), mProgress(p) {}
ProgressBar::ProgressBar(const ProgressBar& e) : Element(e), mProgress(e.mProgress) {}

void ProgressBar::Draw(Window* w) {
	Element::Draw(w);
	Rect displayArea = GetInnerBounds();
	w->DrawRect(displayArea, L'\x2591', GetDefaultForeground().Foreground(), true);
	displayArea.SetWidth((int)((float)displayArea.GetWidth() * mProgress));
	w->DrawRect(displayArea, L'\x2588', GetDefaultBackground().Foreground(), true);
}

}