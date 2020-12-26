#pragma once
#include "View.h"

namespace gui {

class SelectionList : public View {
private:
	bool mShowOptions = false;
	int mOldBottom = 0;

	void Init();
public:
	std::vector<std::wstring> options;
	int selection;

	SelectionList(Rect b);
	SelectionList(Rect b, std::initializer_list<std::wstring> opts, int select = 0);
	
	void ShowOptions();
	void HideOptions();

	virtual void Autosize() override;

	virtual void Draw(Window* w) override;
};

}
