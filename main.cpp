#include "Console.h"

class Test : public gui::Console {
public:
	bool Initialize() override {
		auto w = new gui::Window(GetScreenWidth(), GetScreenHeight());
		SetCurrentWindow(w);

		w->SetScheme(gui::WindowScheme::Default());

		auto e = new gui::Button({ 3, 3, 10, 3 });
		e->SetText(L"Button");
		e->AddEventHandler(new gui::EventHandler(
			[e](gui::Window* w, int m) {
				e->SetTextColor(rand() % 16);
			}
		));
		w->AddElement(e);

		return true;
	}
};

int main() {
	Test t;
	t.CreateConsole(30, 30, 8, 16);
	t.Run();
	return 0;
}