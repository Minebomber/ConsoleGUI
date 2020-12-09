#include "Console.h"

class Test : public gui::Console {
public:
	bool Initialize() override {
		gui::Window* w = new gui::Window(GetScreenWidth(), GetScreenHeight());
		SetCurrentWindow(w);

		w->SetScheme(gui::WindowScheme::Default());

		gui::Element* e = new gui::Label({0, 0, 10, 10});
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