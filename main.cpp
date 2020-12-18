#include "Console.h"

class Program : public gui::Console {
public:
	gui::Window* SetupWindow() {
		auto w = new gui::Window(GetScreenWidth(), GetScreenHeight());
		w->SetStyle<gui::Element>(gui::Style::Green(true));
		SetCurrentWindow(w);
		return w;
	}

	void Initialize() override {
		using namespace gui;
		auto w = SetupWindow();

		auto e1 = new Label({}, L"Test label 1", true);
		w->AddElement(e1);

		auto e2 = new Label({}, L"Test label 2", true);
		w->AddElement(e2);

		auto e3 = new Label({}, L"Test label 3", true);
		w->AddElement(e3);

		auto e4 = new Label({}, L"Test label 4", true);
		w->AddElement(e4);

		auto e5 = new Label({}, L"Test label 5", true);
		w->AddElement(e5);

		e2->AddConstraint({ &Rect::AlignLeftToLeft, e1 })->
			AddConstraint({ &Rect::AlignTopToBottom, e1, 1 });
		e3->AddConstraint({ &Rect::AlignLeftToLeft, e2 })->
			AddConstraint({ &Rect::AlignTopToBottom, e2, 1 });
		e4->AddConstraint({ &Rect::AlignLeftToLeft, e3 })->
			AddConstraint({ &Rect::AlignTopToBottom, e3, 1 });
		e5->AddConstraint({ &Rect::AlignLeftToLeft, e4 })->
			AddConstraint({ &Rect::AlignTopToBottom, e4, 1 });

	}
};

int main() {
	Program p;
	p.Create(50, 20, 8, 16);
	p.Run();
	return 0;
}