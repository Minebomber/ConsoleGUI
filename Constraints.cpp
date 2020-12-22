#include "Constraints.h"

namespace gui {

Constraint::Constraint(Element* e, int o) : Constraint(&e->bounds, o) {}
Constraint::Constraint(Window* w, int o) : Constraint(&w->bounds, o) {}

void LeftToLeftConstraint::ApplyTo(Element* e) { 
	e->bounds.AlignLeftToLeft(*target, offset); 
}

void LeftToRightConstraint::ApplyTo(Element* e) { 
	e->bounds.AlignLeftToRight(*target, offset); 
}

void TopToTopConstraint::ApplyTo(Element* e) { 
	e->bounds.AlignTopToTop(*target, offset); 
}

void TopToBottomConstraint::ApplyTo(Element* e) { 
	e->bounds.AlignTopToBottom(*target, offset); 
}

void RightToRightConstraint::ApplyTo(Element* e) {
	e->bounds.AlignRightToRight(*target, offset); 
}

void RightToLeftConstraint::ApplyTo(Element* e) { 
	e->bounds.AlignRightToLeft(*target, offset); 
}

void BottomToBottomConstraint::ApplyTo(Element* e) { 
	e->bounds.AlignBottomToBottom(*target, offset); 
}

void BottomToTopConstraint::ApplyTo(Element* e) { 
	e->bounds.AlignBottomToTop(*target, offset); 
}

void HorizontalCenterConstraint::ApplyTo(Element* e) {
	e->bounds.CenterHorizontalWith(*target, offset); 
}

void VerticalCenterConstraint::ApplyTo(Element* e) { 
	e->bounds.CenterVerticalWith(*target, offset);
}

void EqualWidthConstraint::ApplyTo(Element* e) {
	e->bounds.width = target->width;
}

void EqualHeightConstraint::ApplyTo(Element* e) {
	e->bounds.width = target->height;
}

}