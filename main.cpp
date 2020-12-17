#include "Console.h"

class Program : public gui::Console {
private:
	bool mNum = true;
	bool mAlp = true;
	bool mSpc = true;
	bool mSec = false;

	gui::TextField* msTf = nullptr;
	gui::TextField* stTf = nullptr;
	gui::ProgressBar* pgBr = nullptr;

	gui::Timer* pgTm = nullptr;

	int pgMs = 100;
	int pgSt = 1;
public:
	void SetupStyles(gui::Window* w) {
		using namespace gui;

		w->SetStyle<Element>(new ElementStyle(
			Color::Black(), Color::DarkGreen(),
			Color::Black(), Color::Green(),
			Color::Black(), Color::DarkGreen(),
			true
		));
	}

	void Initialize() override {
		using namespace gui;

		auto w = new Window(GetScreenWidth(), GetScreenHeight());
		SetupStyles(w);
		SetCurrentWindow(w);


		auto lab = new Label({ 3, 3 }, L"Test 123", true);

		for (int i = 0; i < 3; i++) {

		}

		w->AddElement(new Label({ 1, 1 }, L"Autosize Test", true));
	}
};

int main() {
	using namespace gui;

	Program p;
	p.Create(50, 20, 8, 16);
	p.Run();
	return 0;
}