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
		e->Style().SetDefaultForeground(Color::Red());

		auto b = new Button({ 5, 3 }, L"Test Button", true);
		b->AddEventHandler(
			EventHandler::New()->
			SetAction(EVENT_MOUSE_DOWN, [b, e](auto _w, auto _i) {
				_w->SetFocusedElement(b);
				e->Style().SetDefaultForeground(Color::Red());
			})->
			SetAction(EVENT_MOUSE_UP, [e](auto _w, auto _i) {
				_w->SetFocusedElement(nullptr);
				e->Style().SetDefaultForeground(Color::Blue());
			})->
			SetAction(EVENT_KEY_DOWN, [e](auto&&... _) {
				e->Style().SetDefaultForeground(rand() % 16);
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