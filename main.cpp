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

		auto v1 = new View({ {1, 1}, 5, 3 });
		auto v2 = new View({ {10, 5}, 4, 4 });
		w->view->AddSubview(v1);
		w->view->AddSubview(v2);
		w->ApplyStyle(v1);
		w->ApplyStyle(v2); v2->style.defaultBackground = Color::DarkRed();

		v1->AddConstraint({ Position::Top, v2, Position::Bottom, 0, true });
	}
};

int main() {
	Program p;
	p.Create(50, 20, 8, 16);
	p.Run();
	return 0;
}