#include "Constraints.h"

namespace gui {

Constraint::Constraint(Position::Type src, View* v, Position::Type tgt, int ofst) : source(src), targetView(v), target(tgt), offset(ofst) {
	if (src >> 2 != tgt >> 2) throw "Invalid constraint, different axis for source and target";
}

int Constraint::TrueValueForTarget() {
	switch (target) {
	case Position::Top:	
		return targetView->TrueBounds().Top();
	case Position::Bottom:
		return targetView->TrueBounds().Bottom();
	case Position::Left:
		return targetView->TrueBounds().Left();
	case Position::Right:
		return targetView->TrueBounds().Right();
	default:
		return 0;
	}
}

void Constraint::ApplyTo(View* v) {
	if (!v || !v->parent) return;
	int i = TrueValueForTarget() + offset;
	Point o = v->parent->TrueOrigin();
	switch (source) {
	case Position::Top:
		v->bounds.y = i						- o.y;
		break;
	case Position::Bottom:
		v->bounds.y = i - v->bounds.height+1- o.y;
		break;
	case Position::Left:
		v->bounds.x = i						- o.x;
		break;
	case Position::Right:
		v->bounds.x = i - v->bounds.width+1 - o.x;
		break;
	default: break;
	}
	
}
}