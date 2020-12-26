#pragma once

#include "Label.h"

namespace gui {

class Button : public Label {
private:
	void Init();
public:
	Button(Rect b);
	Button(Rect b, std::wstring t, bool fit = false);
	Button(const Button& e);

	virtual void Draw(Window* w) override;
};

}