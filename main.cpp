#include "Console.h"

class Program : public gui::Console {
public:
	gui::Window* SetupWindow() {
		using namespace gui;
		auto w = new Window(GetScreenWidth(), GetScreenHeight());
		w->SetStyle<Element>(new Style(
			Color::White(), Color::DarkBlue(), 
			Color::White(), Color::Blue(), 
			Color::White(), Color::DarkBlue(), 
			true)
		);

		w->SetStyle<Label>(new Style(
			Color::White(), Color::DarkBlue(),
			Color::White(), Color::Blue(),
			Color::DarkGrey(), Color::Grey(),
			false
		));

		w->SetStyle<TextField>(new Style(
			Color::DarkBlue(), Color::Grey(),
			Color::DarkBlue(), Color::White(),
			Color::DarkGrey(), Color::Grey(),
			false
		));

		SetCurrentWindow(w);
		return w;
	}

	void Initialize() override {
		using namespace gui;
		auto w = SetupWindow();

		auto cont = new Element({ 1, 1, 20, 10 });
		cont->padding = { 1, 1, 1, 1 };

		auto l = new Label({}, L"Test", true);
		cont->AddSubElement(l);

		auto t = new TextField({0, 0, 5, 1});
		t->AddConstraint(new VerticalCenterConstraint(l));
		t->AddConstraint(new LeftToRightConstraint(l, 2));
		cont->AddSubElement(t);

		w->AddElement(cont, true, false);
		
		cont->style.borders = true;
	}
};

int main() {
	Program p;
	p.Create(50, 20, 8, 16);
	p.Run();
	return 0;
}