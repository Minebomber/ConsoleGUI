#include "Console.h"

class Program : public gui::Console {
public:
	gui::Window* SetupWindow() {
		using namespace gui;
		auto w = new Window(GetScreenWidth(), GetScreenHeight());
		w->SetStyle<Element>(Style::Default(false));

		SetCurrentWindow(w);
		return w;
	}

	void Initialize() override {
		using namespace gui;
		auto w = SetupWindow();

		auto e = new SelectionList({ 0, 0, 10, 3 }, {L"First", L"Second", L"Third", L"Fourth", L"Fifth"}, 1);
		w->AddElement(e);
		e->style.borders = true;
	}
};

int main() {
	Program p;
	p.Create(50, 20, 8, 16);
	p.Run();
	return 0;
}