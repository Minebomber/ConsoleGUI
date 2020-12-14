#include "Console.h"

class Test : public gui::Console {
public:
	bool Initialize() override {
		auto w = new gui::Window(GetScreenWidth(), GetScreenHeight());
		SetCurrentWindow(w);

		w->SetScheme(gui::WindowScheme::Red());

		auto e1 = new gui::Element({ 1, 1, 28, 3 });
		w->AddElement(e1);

		auto e2 = new gui::Label({ 1, 5, 28, 3 });
		e2->SetText(L"Test Label");
		w->AddElement(e2);

		auto e3 = new gui::Button({ 1, 9, 28, 3 });
		e3->SetText(L"Test Button");
		w->AddElement(e3);

		auto e4 = new gui::TextField({ 1, 13, 28, 7 });
		w->AddElement(e4);

		auto e5 = new gui::Checkbox({ 1, 21, 28, 3 });
		e5->SetText(L"Text Checkbox");
		w->AddElement(e5);

		return true;
	}
};

int main() {
	Test t;
	t.CreateConsole(30, 30, 8, 16);
	t.Run();
	return 0;
}