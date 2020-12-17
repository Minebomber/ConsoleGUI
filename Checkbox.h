#pragma once
#include "Label.h"

namespace gui {

class Checkbox : public Label {
protected:
	bool mChecked = false;

	void Init();
public:
	Checkbox(Rect b);
	Checkbox(const Checkbox& e);

	const bool& GetChecked() const { return mChecked; }
	void SetChecked(bool c) { mChecked = c; }

	virtual void Autosize();

	virtual void Draw(Window* w);
};


}