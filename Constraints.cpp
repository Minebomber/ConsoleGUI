#include "Constraints.h"

namespace gui {

Constraint::Constraint(
	Position::Type src, 
	View* v, 
	Position::Type tgt, 
	int ofst, 
	bool prsvSz
) : source(src), targetView(v), target(tgt), offset(ofst), preserveSize(prsvSz) {
	if (src >> 2 != tgt >> 2) throw "Invalid constraint, different axis for source and target";
}

int Constraint::TrueTargetValue() {
	switch (target) {
	case Position::Top:	
		return targetView->TrueBounds().top;
	case Position::Bottom:
		return targetView->TrueBounds().bottom;
	case Position::Left:
		return targetView->TrueBounds().left;
	case Position::Right:
		return targetView->TrueBounds().right;
	case Position::CenterX:
		return targetView->TrueBounds().Center().x;
	case Position::CenterY:
		return targetView->TrueBounds().Center().y;
	default:
		return 0;
	}
}

void Constraint::SetValue(int i, int* tgt, int* opp) {
	int old = *tgt;
	*tgt = i;
	if (preserveSize) *opp += *tgt - old;
}

void Constraint::ApplyTo(View* v) {
	if (!v || !v->parent) return;
	int i = TrueTargetValue() + offset;
	Point o = v->parent->TrueOrigin();

	if (source == Position::Top) {
		SetValue(i - o.y, &v->bounds.top, &v->bounds.bottom);
	} else if (source == Position::Bottom) {
		SetValue(i - o.y, &v->bounds.bottom, &v->bounds.top);
	} else if (source == Position::Left) {
		SetValue(i - o.x, &v->bounds.left, &v->bounds.right);
	} else if (source == Position::Right) {
		SetValue(i - o.x, &v->bounds.right, &v->bounds.left);
	} else if (source == Position::CenterX) {
		SetValue(i - o.x - v->bounds.Width() / 2, &v->bounds.left, &v->bounds.right);
	} else if (source == Position::CenterY) {
		SetValue(i - o.y - v->bounds.Height() / 2, &v->bounds.top, &v->bounds.bottom);
	}
}
}