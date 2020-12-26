#include "Button.h"

namespace gui {

Button::Button(Rect b) : Label(b) { Init(); }

Button::Button(Rect b, std::wstring t, bool fit) : Label(b, t, fit) { Init(); }

Button::Button(const Button& e) : Label(e) { Init(); }

void Button::Init() {
	AddEventHandler(
		EventHandler::New()->
		SetAction(EventType::MouseDown, [this](Window* w, int m) {
			state = State::Focused; 
		})->SetAction(EventType::MouseUp, [this](Window* w, int m) {
			state = State::Default;
		})
	);
}

void Button::Draw(Window* w) {
	Label::Draw(w);
}

}