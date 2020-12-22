#include "Console.h"

class Program : public gui::Console {
public:
	gui::Window* SetupWindow() {
		using namespace gui;
		auto w = new Window(GetScreenWidth(), GetScreenHeight());
		w->SetStyle<Element>(new Style(Color::White(), Color::DarkBlue(), Color::White(), Color::Blue(), Color::White(), Color::DarkBlue(), false));

		SetCurrentWindow(w);
		return w;
	}

	void Initialize() override {
		using namespace gui;
		auto w = SetupWindow();

		auto e = new Checkbox({ 0, 0 }, L"Checkbox Test", true);
		w->AddElement(e);
		e->padding = { 2, 1, 2, 1 };
		e->style.borders = true;
		e->Autosize();
	}
};

int main() {
	Program p;
	p.Create(50, 20, 8, 16);
	p.Run();
	return 0;
}