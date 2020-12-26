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

		auto c1 = new View({ {0, 0}, 25, 10 });
		w->view->AddSubview(c1);
		w->ApplyStyle(c1);
		//c1->style.borders = false;
		c1->padding = { 1, 1, 1, 1 };

		auto v1 = new View({ {1, 1}, 5, 5 });
		c1->AddSubview(v1);
		//w->view->AddSubview(v1);
		w->ApplyStyle(v1);
		v1->style.defaultBackground = Color::Cyan();
		//auto v2 = new View({ {10, 5}, 4, 4 });
		
		auto v2 = new View({ {40, 10}, 5, 3 });
		w->view->AddSubview(v2);
		w->ApplyStyle(v2);
		v2->style.defaultBackground = Color::Magenta();

		v2->AddConstraint({ Position::Left, v1, Position::Right, 0, true });

		//w->view->AddSubview(v2);
		//w->ApplyStyle(v1);
		//w->ApplyStyle(v2); v2->style.defaultBackground = Color::DarkRed();

		//v1->AddConstraint({ Position::CenterX, v2, Position::Left, 0, true });
	}
};

int main() {
	Program p;
	p.Create(50, 20, 8, 16);
	p.Run();
	return 0;
}