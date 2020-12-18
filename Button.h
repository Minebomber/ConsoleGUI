#pragma once

#include "Label.h"

namespace gui {

enum MouseButtons {
	MOUSE_BUTTON_LEFT = 0x1,
	MOUSE_BUTTON_RIGHT = 0x2,
	MOUSE_BUTTON_CENTER = 0x4,
};

class Button : public Label {
private:
	void Init();
public:
	int buttons = MOUSE_BUTTON_LEFT;

	Button(Rect b);
	Button(Rect b, std::wstring t, bool fit = false);
	Button(const Button& e);

	virtual void Draw(Window* w) override;
};

}