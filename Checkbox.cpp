#include "Checkbox.h"

namespace gui {

Checkbox::Checkbox(Rect b) : Label(b) { alignH = TEXT_ALIGN_MIN; alignV = TEXT_ALIGN_MIN; Init(); }
Checkbox::Checkbox(const Checkbox& e) : Label(e), checked(e.checked) { alignH = TEXT_ALIGN_MIN; alignV = TEXT_ALIGN_MIN; Init(); }

void Checkbox::Init() {
	AddEventHandler(
		EventHandler::New()->
		SetAction(EVENT_MOUSE_DOWN, [this](Window* w, int m) { state = ELEMENT_FOCUSED; })->
		SetAction(EVENT_MOUSE_UP, [this](Window* w, int m) { state = ELEMENT_DEFAULT; checked = !checked; })
	);
}

void Checkbox::Autosize() {
	Label::Autosize();
	bounds.width += 4;
}

void Checkbox::Draw(Window* w) {
	Element::Draw(w);
	Rect textBounds = InnerBounds();
	WORD cl = CurrentForeground().value | CurrentBackground().value << 4;
	w->WriteString(
		textBounds.x, 
		textBounds.y, 
		checked ? L"[\x25CF]" : L"[ ]", 
		cl
	);
	textBounds.x += 4;
	textBounds.width -= 4;
	w->RenderText(textBounds, text, cl, alignH, alignV, textWrap);
}

}