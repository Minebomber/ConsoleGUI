#include "Console.h"

class Test : public gui::Console {
public:
	bool Initialize() override {
		gui::Window* w = new gui::Window(GetScreenWidth(), GetScreenHeight());
		SetCurrentWindow(w);

		w->SetScheme(gui::WindowScheme::Default());

		gui::Checkbox* e = new gui::Checkbox({ 3, 3, 15, 3 });
		e->SetText(L"Checkbox");
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