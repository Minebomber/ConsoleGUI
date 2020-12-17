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

		w->SetStyle<Element>(ElementStyle::Green(false));
	}

	void Initialize() override {
		using namespace gui;

		auto w = new Window(GetScreenWidth(), GetScreenHeight());
		SetupStyles(w);
		SetCurrentWindow(w);


		auto lab = new Label({}, L"Test", true);
		w->AddElement(lab);
		for (int i = 0; i < 5; i++) {
			auto l = new Label({}, L"Test", true);
			l->CenterWith(lab, { 1, 1 });
			w->AddElement(l);
			l->SetDefaultForeground(rand() % 16);
			lab = l;
		}

	}
};

int main() {
	using namespace gui;

	Program p;
	p.Create(50, 20, 8, 16);
	p.Run();
	return 0;
}