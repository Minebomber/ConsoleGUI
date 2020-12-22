#pragma once

#include "Element.h"

namespace gui {

class ProgressBar : public Element {
public:
	float progress = 0;

	ProgressBar(Rect b);
	ProgressBar(Rect b, float p);
	ProgressBar(const ProgressBar& e);

	virtual void Autosize() override;
	virtual void Draw(Window* w) override;
};

}