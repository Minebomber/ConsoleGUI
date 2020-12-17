#include "Console.h"

class Program : public gui::Console {
public:
	gui::Window* SetupWindow() {
		auto w = new gui::Window(GetScreenWidth(), GetScreenHeight());
		w->SetStyle<gui::Element>(gui::ElementStyle::Green(false));
		SetCurrentWindow(w);
		return w;
	}

	void Initialize() override {
		using namespace gui;

		auto w = SetupWindow();
	}
};

int main() {
	Program p;
	p.Create(50, 20, 8, 16);
	p.Run();
	return 0;
}