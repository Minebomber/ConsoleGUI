#include "Constraints.h"

namespace gui {

void LeftToLeftConstraint::ApplyTo(Element* e) { 
	e->bounds.AlignLeftToLeft(target->bounds, offset); 
}

void LeftToRightConstraint::ApplyTo(Element* e) { 
	e->bounds.AlignLeftToRight(target->bounds, offset); 
}

void TopToTopConstraint::ApplyTo(Element* e) { 
	e->bounds.AlignTopToTop(target->bounds, offset); 
}

void TopToBottomConstraint::ApplyTo(Element* e) { 
	e->bounds.AlignTopToBottom(target->bounds, offset); 
}

void RightToRightConstraint::ApplyTo(Element* e) {
	e->bounds.AlignRightToRight(target->bounds, offset); 
}

void RightToLeftConstraint::ApplyTo(Element* e) { 
	e->bounds.AlignRightToLeft(target->bounds, offset); 
}

void BottomToBottomConstraint::ApplyTo(Element* e) { 
	e->bounds.AlignBottomToBottom(target->bounds, offset); 
}

void BottomToTopConstraint::ApplyTo(Element* e) { 
	e->bounds.AlignBottomToTop(target->bounds, offset); 
}

void HorizontalCenterConstraint::ApplyTo(Element* e) {
	e->bounds.CenterHorizontalWith(target->bounds, offset); 
}

void VerticalCenterConstraint::ApplyTo(Element* e) { 
	e->bounds.CenterHorizontalWith(target->bounds, offset);
}

void EqualWidthConstraint::ApplyTo(Element* e) {
	e->bounds.width = target->bounds.width;
}

void EqualHeightConstraint::ApplyTo(Element* e) {
	e->bounds.width = target->bounds.width;
}

}