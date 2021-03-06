#include "Checkbox.h"

namespace gui {

Checkbox::Checkbox(Rect b) : Label(b) { 
	alignH = Alignment::Min; alignV = Alignment::Min; Init();
}

Checkbox::Checkbox(Rect b, std::wstring t, bool fit) : Label(b, t, fit) { 
	alignH = Alignment::Min; alignV = Alignment::Min; Init();
}

Checkbox::Checkbox(const Checkbox& e) : Label(e), checked(e.checked) { 
	alignH = Alignment::Min; alignV = Alignment::Min; Init();
}

void Checkbox::Init() {
	AddEventHandler(
		EventHandler::New()->
		SetAction(EventType::MouseDown, [this](Window* w, int m) { state = State::Focused; })->
		SetAction(EventType::MouseUp, [this](Window* w, int m) { state = State::Default; checked = !checked; })
	);
}

void Checkbox::Autosize() {
	Label::Autosize();
	bounds.right += 4;
}

void Checkbox::Draw(Window* w) {
	View::Draw(w);
	Rect textBounds = InnerBounds();
	WORD cl = CurrentForeground().value | CurrentBackground().value << 4;
	w->WriteString(
		textBounds.left, 
		textBounds.top, 
		checked ? L"[\x25CF]" : L"[ ]", 
		cl
	);
	textBounds.left += 4;
	w->RenderText(textBounds, text, cl, alignH, alignV, wrap);
}

}