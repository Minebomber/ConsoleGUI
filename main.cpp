#include "Console.h"

class Test : public gui::Console {
private:

public:
	bool Initialize() override {
		gui::Window* w = new gui::Window(GetScreenWidth(), GetScreenHeight());
		SetCurrentWindow(w);

		w->SetScheme(gui::WindowScheme::Default());


		

		return true;
	}

	~Test() {
		SetCurrentWindow(nullptr);
	}
};

int main() {
	Test t;
	t.CreateConsole(30, 30, 8, 16);
	t.Run();
	return 0;
}