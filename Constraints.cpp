#include "Constraints.h"

namespace gui {

Constraint::Constraint(
	Position::Type src, 
	View* v, 
	Position::Type tgt, 
	int ofst, 
	Adjust adj
) : source(src), targetView(v), target(tgt), offset(ofst), adjust(adj) {
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
	default:
		return 0;
	}
}

void Constraint::ApplyTo(View* v) {
	if (!v || !v->parent) return;
	int i = TrueTargetValue() + offset;
	Point o = v->parent->TrueOrigin();

	if (source == Position::Top) {
		int n = i - o.y;
		if (adjust == Adjust::Align) {
			
		} else if (adjust == Adjust::Resize) {

		}
		//v->bounds.y = n;
		//ChangeBounds(i - o.y, &v->bounds.y, &v->bounds.height);
	} else if (source == Position::Bottom) {
		//if (adjust == Adjust::Resize) v->bounds.height = ;
		//else v->bounds.y = i - v->bounds.height + 1 - o.y;

		//ChangeBounds(, &v->bounds.y, &v->bounds.height);
	} else if (source == Position::Left) {
		//v->bounds.x = i - o.x;

	} else if (source == Position::Right) {
		//v->bounds.x = i - v->bounds.width + 1 - o.x;
	}	
}
}