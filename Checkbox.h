#pragma once
#include "Label.h"

namespace gui {

class Checkbox : public Label {
private:
	void Init();
public:
	bool checked = false;

	Checkbox(Rect b);
	Checkbox(Rect b, std::wstring t, bool fit = false);
	Checkbox(const Checkbox& e);

	virtual void Autosize() override;
	virtual void Draw(Window* w) override;
};

}