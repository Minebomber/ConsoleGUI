#include "Button.h"

namespace gui {

Button::Button(Rect b) : Label(b) { Init(); }
Button::Button(const Button& e) : Label(e) { Init(); }

void Button::Init() {
	EventHandler* h = new EventHandler();
	h->SetMouseDownAction([this](Window* w, int m) { if (m & mButtons) mState = ELEMENT_FOCUSED; });
	h->SetMouseUpAction([this](Window* w, int m) { if (m & mButtons) mState = ELEMENT_DEFAULT; });
	AddEventHandler(h);
}

void Button::Draw(Window* w) {
	Label::Draw(w);
}

}