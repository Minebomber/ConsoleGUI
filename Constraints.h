#pragma once

#include "Rect.h"
#include "View.h"
#include "Window.h"

namespace gui {

class View;

struct Position {
private:
	struct Attribute {
		enum {
			AxisX  = 0b1000,
			AxisY  = 0b0100,
			LocMin = 0b0010,
			LocMax = 0b0001,
		};
	};
public:
	enum Type {
		Top = Attribute::AxisY | Attribute::LocMin,
		Bottom = Attribute::AxisY | Attribute::LocMax,
		Left = Attribute::AxisX | Attribute::LocMin,
		Right = Attribute::AxisX | Attribute::LocMax,
	};
};

class Constraint {
private:
	int TrueValueForTarget();
public:
	enum class Adjust {
		Align,
		Resize
	};

	Position::Type source;
	View* targetView;
	Position::Type target;
	Adjust adjust = Adjust::Align;
	int offset;

	Constraint(Position::Type src, View* v, Position::Type tgt, int ofst = 0);

	void ApplyTo(View* v);

	bool operator==(const Constraint& c) { return source == c.source && targetView == c.targetView && target == c.target; }
};

}