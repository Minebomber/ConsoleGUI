#include "Element.h"

namespace gui {

Color Element::CurrentForeground() const {
	switch (state) {
	case State::Focused:
		return style.focusedForeground;
	case State::Disabled:
		return style.disabledForeground;
	default:
		return style.defaultForeground;
	}
}

Color Element::CurrentBackground() const {
	switch (state) {
	case State::Focused:
		return style.focusedBackground;
	case State::Disabled:
		return style.disabledBackground;
	default:
		return style.defaultBackground;
	}
}

Rect Element::InnerBounds() const {
	return {
		bounds.x + style.borders,
		bounds.y + style.borders,
		bounds.width - (2 * style.borders),
		bounds.height - (2 * style.borders)
	};
}

void Element::Draw(Window* w) {
	ApplyConstraints();

	w->DrawRect(bounds, L' ', CurrentBackground().value << 4, true);

	if (style.borders) {
		int x0 = bounds.Left(); int x1 = bounds.Right();
		int y0 = bounds.Top(); int y1 = bounds.Bottom();
		WORD cl = CurrentForeground().value | CurrentBackground().value << 4;

		w->SetChar(x0, y0, L'\x250F', cl);
		w->SetChar(x1, y0, L'\x2513', cl);
		w->SetChar(x0, y1, L'\x2517', cl);
		w->SetChar(x1, y1, L'\x251B', cl);

		for (int x = x0 + 1; x < x1; x++) {
			w->SetChar(x, y0, L'\x2501', cl);
			w->SetChar(x, y1, L'\x2501', cl);
		}

		for (int y = y0 + 1; y < y1; y++) {
			w->SetChar(x0, y, L'\x2503', cl);
			w->SetChar(x1, y, L'\x2503', cl);
		}
	}
}

}