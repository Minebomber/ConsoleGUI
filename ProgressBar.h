#pragma once

#include "Element.h"

namespace gui {

class ProgressBar : public Element {
protected:
	float mProgress = 0;
public:
	ProgressBar(Rect b);
	ProgressBar(Rect b, float p);
	ProgressBar(const ProgressBar& e);

	const float& GetProgress() const { return mProgress; }
	void SetProgress(float p) { if (p < 0) p = 0; if (p > 1) p = 1; mProgress = p; }
	void IncrementProgress(float p) { SetProgress(mProgress + p); }

	virtual void Draw(Window* w);
};

}