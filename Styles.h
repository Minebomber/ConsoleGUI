#pragma once

#define NOMINMAX
#include <Windows.h>
#include <unordered_map>
#include <typeinfo>
#include <typeindex>

#include "Colors.h"

namespace gui {

class ElementStyle {
private:
	Color mDefaultForeground;
	Color mDefaultBackground;
	Color mFocusedForeground;
	Color mFocusedBackground;
	Color mDisabledForeground;
	Color mDisabledBackground;

	bool mBorders;
public:
	static ElementStyle* Default(bool borders = true) {
		return new ElementStyle(
			Color::Grey(), Color::Black(),
			Color::White(), Color::Black(),
			Color::DarkGrey(), Color::Black(),
			borders
		);
	}

	static ElementStyle* Green(bool borders = true) {
		return new ElementStyle(
			Color::DarkGreen(), Color::Black(),
			Color::Green(), Color::Black(),
			Color::DarkGreen(), Color::Black(),
			borders
		);
	}

	static ElementStyle* Red(bool borders = true) {
		return new ElementStyle(
			Color::DarkRed(), Color::Black(),
			Color::Red(), Color::Black(),
			Color::DarkRed(), Color::Black(),
			borders
		);
	}

	ElementStyle(Color defF, Color defB, Color focF, Color focB, Color disF, Color disB, bool bds) :
		mDefaultForeground(defF), mDefaultBackground(defB), mFocusedForeground(focF),
		mFocusedBackground(focB), mDisabledForeground(disF), mDisabledBackground(disB), mBorders(bds) {}

	const Color& GetDefaultForeground() const { return mDefaultForeground; }
	void SetDefaultForeground(Color c) { mDefaultForeground = c; }

	const Color& GetDefaultBackground() const { return mDefaultBackground; }
	void SetDefaultBackground(Color c) { mDefaultBackground = c; }

	const Color& GetFocusedForeground() const { return mFocusedForeground; }
	void SetFocusedForeground(Color c) { mFocusedForeground = c; }

	const Color& GetFocusedBackground() const { return mFocusedBackground; }
	void SetFocusedBackground(Color c) { mFocusedBackground = c; }

	const Color& GetDisabledForeground() const { return mDisabledForeground; }
	void SetDisabledForeground(Color c) { mDisabledForeground = c; }

	const Color& GetDisabledBackground() const { return mDisabledBackground; }
	void SetDisabledBackground(Color c) { mDisabledBackground = c; }

	const bool& GetBorders() const { return mBorders; }
	void SetBorders(bool b) { mBorders = b; }
};

}