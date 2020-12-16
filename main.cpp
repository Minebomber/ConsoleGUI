#include "Console.h"

class Program : public gui::Console {
private:

public:
	void Initialize() override {
		using namespace gui;
		auto w = new Window(
			GetScreenWidth(), 
			GetScreenHeight(), 
			ElementStyle::Green(false)
		);
		SetCurrentWindow(w);

		auto tfStyle = new ElementStyle(
			Color::Black(), Color::DarkGreen(),
			Color::Black(), Color::Green(),
			Color::Black(), Color::DarkGreen(),
			false
		);

		w->SetStyle<TextField>(tfStyle);

		auto lab = new Label({ 1, 1, 13, 1 });
		lab->SetText(L"Test Label");
		w->AddElement(lab);

		auto tf = new TextField({ 15, 1, 5, 1 });
		tf->SetMode(TEXT_MODE_NUMBERS);

		w->AddElement(tf);
	}
};

int main() {
	using namespace gui;

	Program p;
	p.Create(50, 20, 8, 16);
	p.Run();
	return 0;
}