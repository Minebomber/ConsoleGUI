#include "ProgressBar.h"

namespace gui {

ProgressBar::ProgressBar(Rect b) : View(b) {}
ProgressBar::ProgressBar(Rect b, float p) : View(b), progress(p) {}
ProgressBar::ProgressBar(const ProgressBar& e) : View(e), progress(e.progress) {}

void ProgressBar::Autosize() {
	int r = bounds.right;
	View::Autosize();
	bounds.right = r;
	bounds.bottom++;
}

void ProgressBar::Draw(Window* w) {
	View::Draw(w);
	Rect displayArea = InnerBounds();
	w->DrawRect(displayArea, L'\x2591', style.defaultBackground.value, true);
	displayArea.right = displayArea.left + std::max(
		0, 
		std::min((int)((float)displayArea.Width() * progress), displayArea.Width())
	) - 1;
	w->DrawRect(displayArea, L'\x2588', style.defaultForeground.value, true);
}

}