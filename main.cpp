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
		e->SetDefaultForeground(Color::Red());

		auto b = new Button({ 5, 3 }, L"Test Button", true);
		b->SetButtons(MOUSE_BUTTON_LEFT);
		b->AddEventHandler(
			EventHandler::New()->
			SetMouseDownAction([b, e](auto _w, auto _i) {
				_w->SetFocusedElement(b);
				e->SetDefaultForeground(Color::Red());
			})->
			SetMouseUpAction([e](auto _w, auto _i) {
				_w->SetFocusedElement(nullptr);
				e->SetDefaultForeground(Color::Blue());
			})->
			SetKeyDownAction([e](auto&&... _) {
				e->SetDefaultForeground(rand() % 16);
			})
		);
		w->AddElement(b);
	}
};

int main() {
	Program p;
	p.Create(50, 20, 8, 16);
	p.Run();
	return 0;
}