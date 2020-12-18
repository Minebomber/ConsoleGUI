#pragma once

#include "Colors.h"

namespace gui {

class Style {
public:
	Color defaultForeground;
	Color defaultBackground;
	Color focusedForeground;
	Color focusedBackground;
	Color disabledForeground;
	Color disabledBackground;

	bool borders;

	static Style* Default(bool bds = true) {
		return new Style(
			Color::Grey(), Color::Black(),
			Color::White(), Color::Black(),
			Color::DarkGrey(), Color::Black(),
			bds
		);
	}

	static Style* Green(bool bds = true) {
		return new Style(
			Color::DarkGreen(), Color::Black(),
			Color::Green(), Color::Black(),
			Color::DarkGreen(), Color::Black(),
			bds
		);
	}

	static Style* Red(bool bds = true) {
		return new Style(
			Color::DarkRed(), Color::Black(),
			Color::Red(), Color::Black(),
			Color::DarkRed(), Color::Black(),
			bds
		);
	}

	Style() : defaultForeground(0), defaultBackground(0), focusedForeground(0),
		focusedBackground(0), disabledForeground(0), disabledBackground(0), borders(false) {}

	Style(Color defF, Color defB, Color focF, Color focB, Color disF, Color disB, bool bds) :
		defaultForeground(defF), defaultBackground(defB), focusedForeground(focF),
		focusedBackground(focB), disabledForeground(disF), disabledBackground(disB), borders(bds) {}

	Style(const Style& s) :
		defaultForeground(s.defaultForeground), defaultBackground(s.defaultBackground),
		focusedForeground(s.focusedForeground), focusedBackground(s.focusedBackground),
		disabledForeground(s.disabledForeground), disabledBackground(s.disabledBackground),
		borders(s.borders) {}
};

}