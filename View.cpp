#include "View.h"

namespace gui {

View::~View() {
	for (EventHandler* h : mEventHandlers) delete h;
	for (View* se : mSubviews) delete se;
}

Rect View::InnerBounds() const {
	return {
		bounds.left + style.borders + padding.left,
		bounds.top + style.borders + padding.top,
		bounds.right - style.borders - padding.right,
		bounds.bottom - style.borders - padding.bottom
	};
}

Point View::TrueOrigin() const {
	Point origin = InnerBounds().Min();
	View* p = parent;
	while (p) {
		origin += p->InnerBounds().Min();
		p = p->parent;
	}
	return origin;
}

Rect View::TrueBounds() const {
	Point o = parent ? parent->TrueOrigin() : Point(0, 0);
	return { 
		o.x + bounds.left, 
		o.y + bounds.top, 
		o.x + bounds.right, 
		o.y + bounds.bottom 
	};
}

Rect View::TrueInnerBounds() const {
	Point o = TrueOrigin();
	Rect ib = InnerBounds();
	return { ib.left + o.x, ib.top + o.y, ib.right + o.x, ib.bottom + o.y };
}

void View::Autosize() {
	bounds = {
		{ bounds.left, bounds.top },
		2 * style.borders + padding.TotalX(),
		2 * style.borders + padding.TotalY()
	};
}

Color View::CurrentForeground() const {
	switch (state) {
	case State::Focused:
		return style.focusedForeground;
	case State::Disabled:
		return style.disabledForeground;
	default:
		return style.defaultForeground;
	}
}

Color View::CurrentBackground() const {
	switch (state) {
	case State::Focused:
		return style.focusedBackground;
	case State::Disabled:
		return style.disabledBackground;
	default:
		return style.defaultBackground;
	}
}

void View::AddConstraint(Constraint c) {
	mConstraints.push_back(c);
}

void View::RemoveConstraint(Constraint c) {
	mConstraints.erase(
		std::remove(mConstraints.begin(), mConstraints.end(), c),
		mConstraints.end()
	);
}

void View::ApplyConstraints() { 
	for (Constraint& c : mConstraints) c.ApplyTo(this); 
}

void View::Draw(Window* w) {
	ApplyConstraints();

	w->DrawRect(bounds, L' ', CurrentBackground().value << 4, true);

	if (style.borders) {
		int x0 = bounds.left; int x1 = bounds.right;
		int y0 = bounds.top; int y1 = bounds.bottom;
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

	w->PushOffset(InnerBounds().Min());
	for (View* e : mSubviews) e->Draw(w);
	w->PopOffset(InnerBounds().Min());
}

}