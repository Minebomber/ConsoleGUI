#pragma once
#include "Element.h"

namespace gui {

class SelectionList : public Element {
private:
	bool mShowOptions = false;
	int mOldHeight = 0;

	void Init();
public:
	std::vector<std::wstring> options;
	int selection;

	SelectionList(Rect b);
	SelectionList(Rect b, std::initializer_list<std::wstring> opts, int select = 0);
	
	virtual void Autosize() override {}

	virtual void Draw(Window* w) override;
};

}
