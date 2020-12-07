#include "Console.h"

class Test : public gui::Console {
private:
	gui::TextField* tf;
	gui::Label* lab;
	gui::Button* btn;

public:
	bool Initialize() override {
		gui::Window* w = new gui::Window(GetScreenWidth(), GetScreenHeight());
		SetCurrentWindow(w);
		
		w->SetColorScheme(gui::WindowScheme::Green());

		lab = new gui::Label({ 3, 3, 13, 5 });
		lab->SetText(L"Test");

		w->AddElement(lab);

		tf = new gui::TextField({ 10, 10, 25, 12 }, gui::Charset::Alphanum());
		w->AddElement(tf);

		btn = new gui::Button({ 3, 7, 13, 9 });
		btn->SetText(L"Button");
		w->AddElement(btn);

		return true;
	}

	~Test() {
		delete btn;
		delete lab;
		delete tf;
		SetCurrentWindow(nullptr);
	}
};

int main() {
	Test t;//       31
	t.CreateConsole(30, 30, 8, 16);
	t.Run();
	return 0;
}