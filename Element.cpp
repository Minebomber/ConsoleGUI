#include "Element.h"

namespace gui {

Color Element::GetCurrentForeground() const {
	switch (mState) {
	case ELEMENT_FOCUSED:
		return mFocusedForeground;
	case ELEMENT_DISABLED:
		return mDisabledForeground;
	default:
		return mDefaultForeground;
	}
}

Color Element::GetCurrentBackground() const {
	switch (mState) {
	case ELEMENT_FOCUSED:
		return mFocusedBackground;
	case ELEMENT_DISABLED:
		return mDisabledBackground;
	default:
		return mDefaultBackground;
	}
}

Rect Element::GetInnerBounds() const {
	return {
		mBounds.GetX() + mBorders,
		mBounds.GetY() + mBorders,
		mBounds.GetWidth() - (2 * mBorders),
		mBounds.GetHeight() - (2 * mBorders)
	};
}

void Element::Draw(Window* w) {
	w->DrawRect(mBounds, L' ', GetCurrentBackground().Background(), true);

	if (mBorders) {
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