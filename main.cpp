#include "Console.h"



class Test : public gui::Console {
private:
	gui::Element* mElem = nullptr;

	
public:
	bool Initialize() override {
		mElem = new gui::Element({ 5, 13, 25, 17 });
		mElem->SetBackgroundColor(gui::BG_BLUE);
		mElem->SetBorder(new gui::TitledBorder(L"Title", gui::BG_BLUE | gui::FG_BLACK, 1 ));
		AddElement(mElem);
		return true;
	}

	~Test() {
		delete mElem;
	}
};

int main() {
	Test t;//       31
	t.CreateConsole(30, 30, 16, 16);
	t.Run();
	return 0;
}