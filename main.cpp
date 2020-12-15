#include "Console.h"

class Test : public gui::Console {
public:
	bool Initialize() override {
		auto w = new gui::Window(GetScreenWidth(), GetScreenHeight());
		SetCurrentWindow(w);

		w->SetScheme(gui::WindowScheme::Red());

		auto e1 = new gui::Element({ 1, 1, 28, 10 });
		e1->AddEventHandler(new gui::EventHandler(
			nullptr,
			nullptr,
			[e1](auto w, auto m) {
				e1->SetDefaultForegroundColor(rand() % 16);
			}
		));
		w->AddElement(e1);

		return true;
	}
};

int main() {
	Test t;
	t.CreateConsole(30, 30, 8, 16);
	t.Run();
	return 0;
}