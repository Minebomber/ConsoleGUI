#pragma once

#include "Label.h"

namespace gui {

enum MouseButtons {
	MOUSE_BUTTON_LEFT = 0x1,
	MOUSE_BUTTON_RIGHT = 0x2,
	MOUSE_BUTTON_CENTER = 0x4,
};

class Button : public Label {
	friend class Window;
protected:
	int mButtons = MOUSE_BUTTON_LEFT;

	void Init();
public:
	Button(Rect b);
	Button(const Button& e);

	const int& GetButtons() const { return mButtons; }
	void SetButtons(int b) { mButtons = b; }

	virtual void Draw(Window* w);
};

}