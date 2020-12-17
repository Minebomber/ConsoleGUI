#include "Element.h"

namespace gui {

Color Element::GetCurrentForeground() const {
	switch (mState) {
	case ELEMENT_FOCUSED:
		return mStyle.GetFocusedForeground();
	case ELEMENT_DISABLED:
		return mStyle.GetDisabledForeground();
	default:
		return mStyle.GetDefaultForeground();
	}
}

Color Element::GetCurrentBackground() const {
	switch (mState) {
	case ELEMENT_FOCUSED:
		return mStyle.GetFocusedBackground();
	case ELEMENT_DISABLED:
		return mStyle.GetDisabledBackground();
	default:
		return mStyle.GetDefaultBackground();
	}
}

Rect Element::GetInnerBounds() const {
	return {
		mBounds.GetX() + mStyle.GetBorders(),
		mBounds.GetY() + mStyle.GetBorders(),
		mBounds.GetWidth() - (2 * mStyle.GetBorders()),
		mBounds.GetHeight() - (2 * mStyle.GetBorders())
	};
}

void Element::Draw(Window* w) {
	w->DrawRect(mBounds, L' ', GetCurrentBackground().Background(), true);

	if (mStyle.GetBorders()) {
		int x0 = mBounds.Left(); int x1 = mBounds.Right();
		int y0 = mBounds.Top(); int y1 = mBounds.Bottom();
		WORD cl = GetCurrentForeground().Foreground() | GetCurrentBackground().Background();

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