#include "Console.h"

class Program : public gui::Console {
public:
	gui::Window* SetupWindow() {
		auto w = new gui::Window(GetScreenWidth(), GetScreenHeight());
		w->SetStyle<gui::Element>(gui::ElementStyle::Green(true));
		SetCurrentWindow(w);
		return w;
	}

	void Initialize() override {
		using namespace gui;

		auto w = SetupWindow();

		auto lab = new Label({}, L"Test", true);
		w->AddElement(lab);
		for (int i = 0; i < 10; i++) {
			auto l = new Label({}, L"Test", true);
			l->AlignBottomToBottom(lab)->AlignLeftToRight(lab);
			w->AddElement(l);
			l->SetDefaultForeground(rand() % 256);
			lab = l;
		}
	}
};

int main() {
	Program p;
	p.Create(50, 20, 8, 16);
	p.Run();
	return 0;
}