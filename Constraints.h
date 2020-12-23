#pragma once

#include "Rect.h"
#include "Element.h"
#include "Window.h"

namespace gui {

class View;
class Window;

class Constraint {
public:
	Rect* target;
	int offset;

	Constraint(Rect* t, int o = 0) : target(t), offset(o) {}
	Constraint(View* e, int o = 0);
	Constraint(Window* w, int o = 0);
	
	virtual void ApplyTo(View* e) = 0;
};

class LeftToLeftConstraint : public Constraint {
public:
	using Constraint::Constraint;
	void ApplyTo(View* e) override;
};

class LeftToRightConstraint : public Constraint {
public:
	using Constraint::Constraint;
	void ApplyTo(View* e) override;
};

class TopToTopConstraint : public Constraint {
public:
	using Constraint::Constraint;
	void ApplyTo(View* e) override;
};

class TopToBottomConstraint : public Constraint {
public:
	using Constraint::Constraint;
	void ApplyTo(View* e) override;
};

class RightToRightConstraint : public Constraint {
public:
	using Constraint::Constraint;
	void ApplyTo(View* e) override;
};

class RightToLeftConstraint : public Constraint {
public:
	using Constraint::Constraint;
	void ApplyTo(View* e) override;
};

class BottomToBottomConstraint : public Constraint {
public:
	using Constraint::Constraint;
	void ApplyTo(View* e) override;
};

class BottomToTopConstraint : public Constraint {
public:
	using Constraint::Constraint;
	void ApplyTo(View* e) override;
};

class HorizontalCenterConstraint : public Constraint {
public:
	using Constraint::Constraint;
	void ApplyTo(View* e) override;
};

class VerticalCenterConstraint : public Constraint {
public:
	using Constraint::Constraint;
	void ApplyTo(View* e) override;
};

class EqualWidthConstraint : public Constraint {
public:
	using Constraint::Constraint;
	void ApplyTo(View* e) override;
};

class EqualHeightConstraint : public Constraint {
public:
	using Constraint::Constraint;
	void ApplyTo(View* e) override;
};

}