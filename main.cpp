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
		
		w->SetColorScheme(gui::WindowScheme::Red());

		lab = new gui::Label({ 3, 3, 12, 5 });
		lab->SetText(L"Test");

		w->AddElement(lab);

		tf = new gui::TextField({ 2, 10, 27, 20 }, gui::Charset::Alphanum());
		tf->SetTextWrap(gui::WRAP_WORD);
		tf->SetAlignVertical(gui::TEXT_ALIGN_MID);
		tf->SetAlignHorizontal(gui::TEXT_ALIGN_MIN);
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
	Test t;
	t.CreateConsole(30, 30, 8, 16);
	t.Run();
	return 0;
}