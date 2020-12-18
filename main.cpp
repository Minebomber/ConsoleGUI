#include "Console.h"

class Program : public gui::Console {
public:
	gui::Window* SetupWindow() {
		auto w = new gui::Window(GetScreenWidth(), GetScreenHeight());
		w->SetStyle<gui::Element>(gui::ElementStyle::Green(true));
		SetCurrentWindow(w);
		return w;
	}

	void Initialize() override {
		using namespace gui;
		auto w = SetupWindow();

		auto e = new Element({ 1, 1, 2, 2 });
		w->AddElement(e);
		e->style.defaultForeground = Color::Blue();

		auto b = new Button({ 5, 3 }, L"Test Button", true);
		b->AddEventHandler(new EventHandler({
			{ EventType::MouseDown, [b, e](auto _w, auto _i) {
				_w->focusedElement = b;
				e->style.defaultForeground = Color::Red();
			} },
			{ EventType::MouseUp, [e](auto _w, auto _i) {
				_w->focusedElement = nullptr;
				e->style.defaultForeground = Color::Blue();
			} },
			{ EventType::MouseDrag, [e](auto&&... _) {
				e->style.defaultForeground = rand() % 16;
			}}
		}));
		w->AddElement(b);
	}
};

int main() {
	Program p;
	p.Create(50, 20, 8, 16);
	p.Run();
	return 0;
}