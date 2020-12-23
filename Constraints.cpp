#include "Constraints.h"

namespace gui {

Constraint::Constraint(View* e, int o) : Constraint(&e->bounds, o) {}
Constraint::Constraint(Window* w, int o) : Constraint(&w->view->bounds, o) {}

void LeftToLeftConstraint::ApplyTo(View* e) { 
	e->bounds.AlignLeftToLeft(*target, offset); 
}

void LeftToRightConstraint::ApplyTo(View* e) { 
	e->bounds.AlignLeftToRight(*target, offset); 
}

void TopToTopConstraint::ApplyTo(View* e) { 
	e->bounds.AlignTopToTop(*target, offset); 
}

void TopToBottomConstraint::ApplyTo(View* e) { 
	e->bounds.AlignTopToBottom(*target, offset); 
}

void RightToRightConstraint::ApplyTo(View* e) {
	e->bounds.AlignRightToRight(*target, offset); 
}

void RightToLeftConstraint::ApplyTo(View* e) { 
	e->bounds.AlignRightToLeft(*target, offset); 
}

void BottomToBottomConstraint::ApplyTo(View* e) { 
	e->bounds.AlignBottomToBottom(*target, offset); 
}

void BottomToTopConstraint::ApplyTo(View* e) { 
	e->bounds.AlignBottomToTop(*target, offset); 
}

void HorizontalCenterConstraint::ApplyTo(View* e) {
	e->bounds.CenterHorizontalWith(*target, offset); 
}

void VerticalCenterConstraint::ApplyTo(View* e) { 
	e->bounds.CenterVerticalWith(*target, offset);
}

void EqualWidthConstraint::ApplyTo(View* e) {
	e->bounds.width = target->width;
}

void EqualHeightConstraint::ApplyTo(View* e) {
	e->bounds.width = target->height;
}

}