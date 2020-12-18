#include "Button.h"

namespace gui {

Button::Button(Rect b) : Label(b) { Init(); }

Button::Button(Rect b, std::wstring t, bool fit) : Label(b, t, fit) { Init(); }

Button::Button(const Button& e) : Label(e) { Init(); }

void Button::Init() {
	AddEventHandler(
		EventHandler::New()->
		SetAction(GUI_MOUSE_DOWN, [this](Window* w, int m) {
			if (m & buttons) state = ELEMENT_FOCUSED; 
		})->SetAction(GUI_MOUSE_UP, [this](Window* w, int m) { 
			if (m & buttons) state = ELEMENT_DEFAULT; 
		})
	);
}

void Button::Draw(Window* w) {
	Label::Draw(w);
}

}