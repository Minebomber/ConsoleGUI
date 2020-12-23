#include "Console.h"

class Program : public gui::Console {
public:
	gui::Window* SetupWindow() {
		using namespace gui;
		auto w = new Window(GetScreenWidth(), GetScreenHeight());
		w->SetStyle<View>(new Style(
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

		w->SetStyle<Button>(new Style(
			Color::White(), Color::DarkBlue(),
			Color::DarkGrey(), Color::DarkBlue(),
			Color::DarkGrey(), Color::Grey(),
			true
		));

		SetCurrentWindow(w);
		return w;
	}

	void Initialize() override {
		using namespace gui;
		auto w = SetupWindow();

		auto cont = new View({ 1, 1, 30, 15 });
		cont->padding = { 0, 1, 1, 1 };

		auto l = new Label({}, L"Multiline\nTest\nLabel", true);
		cont->AddSubview(l);

		auto t = new TextField({0, 0, 15, 3});
		t->AddConstraint(new VerticalCenterConstraint(l));
		t->AddConstraint(new LeftToRightConstraint(l, 2));
		cont->AddSubview(t);
		 
		auto sl = new SelectionList({}, {L"Option 1", L"Option 2", L"Option 3", L"Option 4"});
		sl->AddConstraint(new HorizontalCenterConstraint(cont));
		sl->AddConstraint(new TopToBottomConstraint(t, 2));
		
		cont->AddSubview(sl);

		w->view->AddSubview(cont);

		w->ApplyStyle(cont, true);
		sl->Autosize();
	}
};

int main() {
	Program p;
	p.Create(50, 20, 8, 16);
	p.Run();
	return 0;
}