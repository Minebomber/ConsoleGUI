#include "Console.h"

class Test : public gui::Console {
private:
	gui::Element* mElem = nullptr;

public:
	bool Initialize() override {
		gui::Window* w = new gui::Window(GetScreenWidth(), GetScreenHeight());
		SetCurrentWindow(w);

		mElem = new gui::Element({ 5, 13, 25, 17 });
		mElem->SetBackgroundColor(gui::BG_BLUE);
		mElem->SetBorder(new gui::TitledBorder(L"Title", gui::BG_BLUE | gui::FG_BLACK, 1 ));
		w->AddElement(mElem);

		return true;
	}

	~Test() {
		delete mElem;
		delete GetCurrentWindow();
	}
};

int main() {
	Test t;//       31
	t.CreateConsole(30, 30, 16, 16);
	t.Run();
	return 0;
}