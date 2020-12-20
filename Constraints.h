#pragma once

#include "Rect.h"
#include "Element.h"

namespace gui {

class Element;

class Constraint {
public:
	Element* target;
	int offset;

	Constraint(Element* t) : target(t), offset(0) {}
	Constraint(Element* t, int o) : target(t), offset(o) {}

	virtual void ApplyTo(Element* e) = 0;
};

class LeftToLeftConstraint : public Constraint {
public:
	using Constraint::Constraint;
	void ApplyTo(Element* e) override;
};

class LeftToRightConstraint : public Constraint {
public:
	using Constraint::Constraint;
	void ApplyTo(Element* e) override;
};

class TopToTopConstraint : public Constraint {
public:
	using Constraint::Constraint;
	void ApplyTo(Element* e) override;
};

class TopToBottomConstraint : public Constraint {
public:
	using Constraint::Constraint;
	void ApplyTo(Element* e) override;
};

class RightToRightConstraint : public Constraint {
public:
	using Constraint::Constraint;
	void ApplyTo(Element* e) override;
};

class RightToLeftConstraint : public Constraint {
public:
	using Constraint::Constraint;
	void ApplyTo(Element* e) override;
};

class BottomToBottomConstraint : public Constraint {
public:
	using Constraint::Constraint;
	void ApplyTo(Element* e) override;
};

class BottomToTopConstraint : public Constraint {
public:
	using Constraint::Constraint;
	void ApplyTo(Element* e) override;
};

class HorizontalCenterConstraint : public Constraint {
public:
	using Constraint::Constraint;
	void ApplyTo(Element* e) override;
};

class VerticalCenterConstraint : public Constraint {
public:
	using Constraint::Constraint;
	void ApplyTo(Element* e) override;
};

class EqualWidthConstraint : public Constraint {
public:
	using Constraint::Constraint;
	void ApplyTo(Element* e) override;
};

class EqualHeightConstraint : public Constraint {
public:
	using Constraint::Constraint;
	void ApplyTo(Element* e) override;
};

}