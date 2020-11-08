#include "GUIElements.h"

const int& EventHandler::GetId() const { return id; }
void EventHandler::SetId(int i) { id = i; }

bool EventHandler::OnPressExists() const { return (bool)onPress; }
void EventHandler::InvokeOnPress(int i) { onPress(i); }
void EventHandler::SetOnPress(std::function<void(int)> f) { onPress = f; }

bool EventHandler::OnReleaseExists() const { return (bool)onRelease; }
void EventHandler::InvokeOnRelease(int i) { onRelease(i); }
void EventHandler::SetOnRelease(std::function<void(int)> f) { onRelease = f; }

const RECT& MouseHandler::GetBounds() { return bounds; }
void MouseHandler::SetBounds(RECT b) { bounds = b; }

const int& MouseHandler::GetButtons() { return buttons; }
void MouseHandler::SetButtons(int b) { buttons = b; }

GUIBorder::GUIBorder() {}
GUIBorder::GUIBorder(WCHAR ch, WORD cl) : chr(ch), color(cl) {}
GUIBorder::GUIBorder(WCHAR ch, WORD cl, int w) : chr(ch), color(cl), width(w) {}
GUIBorder::GUIBorder(int w) : width(w) {}

const WCHAR& GUIBorder::GetChar() const { return chr; }
void GUIBorder::SetChar(WCHAR c) { chr = c; }

const WORD& GUIBorder::GetColor() const { return color; }
void GUIBorder::SetColor(WORD c) { color = c; }

const int& GUIBorder::GetWidth() const { return width; }
void GUIBorder::SetWidth(int w) { width = w; }

GUIBorder::operator bool() const { return width != 0; }

void GUIBorder::Draw(ConsoleGUI* g, RECT bd) {
	for (int b = 0; b < width; b++) g->Rect({ bd.left + b, bd.top + b, bd.right - b, bd.bottom - b }, chr, color, false);
}

GUIElement::GUIElement(RECT b) : bounds(b) {}
GUIElement::GUIElement(const GUIElement& e) : bounds(e.bounds), background(e.background), backgroundColor(e.backgroundColor) {}
GUIElement::~GUIElement() {}

const int& GUIElement::GetId() const { return id; }
void GUIElement::SetId(int i) { id = i; }

const RECT& GUIElement::GetBounds() const { return bounds; }
void GUIElement::SetBounds(RECT b) { bounds = b; }

const WCHAR& GUIElement::GetBackground() const { return background; }
void GUIElement::SetBackground(WCHAR b) { background = b; }

const WORD& GUIElement::GetBackgroundColor() const { return backgroundColor; }
void GUIElement::SetBackgroundColor(WORD c) { backgroundColor = c; }

GUIBorder& GUIElement::GetBorder() { return border; }
void GUIElement::SetBorder(GUIBorder b) { border = b; }

void GUIElement::Draw(ConsoleGUI* g) {
	g->Rect(bounds, background, backgroundColor, true);
	border.Draw(g, bounds);
}

GUILabel::GUILabel(RECT b) : GUIElement(b) {}
GUILabel::GUILabel(const GUILabel& e) : GUIElement(e), text(e.text), textColor(e.textColor), hAlignment(e.hAlignment), vAlignment(e.vAlignment) {}

void GUILabel::UpdateTextLines() {
	textLines = 1;
	for (size_t ci = 0, li = 0; ci < text.length(); ci++) {
		if (text[ci] == L'\n') textLines++;
		if (ci - li > bounds.right - bounds.left - 2 * border.GetWidth()) { textLines++; li = ci; }
	}
}

const std::wstring& GUILabel::GetText() const { return text; }
void GUILabel::SetText(std::wstring t) { text = t; UpdateTextLines(); }

const WORD& GUILabel::GetTextColor() const { return textColor; }
void GUILabel::SetTextColor(WORD c) { textColor = c; }

const int& GUILabel::GetHorizontalAlignment() const { return hAlignment; }
void GUILabel::SetHorizontalAlignment(int h) { hAlignment = h; }

const int& GUILabel::GetVerticalAlignment() const { return vAlignment; }
void GUILabel::SetVerticalAlignment(int v) { vAlignment = v; }

const int& GUILabel::GetTextWrap() const { return textWrap; }
void GUILabel::SetTextWrap(int w) { textWrap = w; }

void GUILabel::RenderText(ConsoleGUI* g, int minX, int maxX, int minY, int maxY, WORD c) {
	int yOffset = 0; // default to min
	switch (vAlignment) {
	case TEXT_ALIGN_MID:
		yOffset = (maxY - minY - textLines + 1) / 2;
		break;
	case TEXT_ALIGN_MAX:
		yOffset = maxY - minY - textLines;
	default:
		break;
	}

	int y = minY + yOffset;

	size_t nlPos = text.find(L'\n');
	size_t cIdx = 0;
	while (cIdx < text.length() && y <= maxY) {
		int elIdx = (nlPos == std::string::npos) ? text.length() : nlPos;
		int lineLen = elIdx - cIdx;

		// Stay within bounds
		if (lineLen > maxX - minX) {
			if (textWrap == WRAP_WORD) {
				elIdx = text.rfind(L' ', cIdx + (maxX - minX));
				lineLen = elIdx - cIdx;
			} else if (textWrap == WRAP_CHAR || lineLen > maxX - minX) { // def to char if no spac
				elIdx -= (lineLen - (maxX - minX));
				lineLen = maxX - minX + 1;
			}
		}
		int xOffset = 0;
		switch (hAlignment) {
		case TEXT_ALIGN_MID:
			xOffset = (maxX - minX - lineLen + 1) / 2;
			break;
		case TEXT_ALIGN_MAX:
			xOffset = maxX - minX - lineLen + 1;
		default:
			break;
		}

		g->Write(minX + xOffset, y, text.substr(cIdx, lineLen), c);

		y++;
		cIdx = elIdx + 1;
		nlPos = text.find(L'\n', cIdx);
	}
}

void GUILabel::Draw(ConsoleGUI* g) {
	GUIElement::Draw(g);
	RenderText(g, bounds.left + border.GetWidth(), bounds.right - border.GetWidth(), bounds.top + border.GetWidth(), bounds.bottom - border.GetWidth(), textColor);
}

GUIButton::GUIButton(RECT b) : GUILabel(b), handler(), OnPress(), OnRelease() { SetupHandler(); }

void GUIButton::SetBounds(RECT b) { GUIElement::SetBounds(b); handler.SetBounds(b); }

const WORD& GUIButton::GetPressedTextColor() const { return pressedTextColor; }
void GUIButton::SetPressedTextColor(WORD c) { pressedTextColor = c; }

const WCHAR& GUIButton::GetPressedBackground() const { return pressedBackground; }
void GUIButton::SetPressedBackground(WCHAR b) { pressedBackground = b; }

const WORD& GUIButton::GetPressedBackgroundColor() const { return pressedBackgroundColor; }
void GUIButton::SetPressedBackgroundColor(WORD c) { pressedBackgroundColor = c; }

GUIBorder& GUIButton::GetPressedBorder() { return pressedBorder; }
void GUIButton::SetPressedBorder(GUIBorder b) { pressedBorder = b; }

MouseHandler& GUIButton::GetHandler() { return handler; }

void GUIButton::SetupHandler() {
	handler.SetBounds(bounds);
	handler.SetOnPress([this](int m) { pressed = true; if (OnPress) OnPress(m); });
	handler.SetOnRelease([this](int m) { pressed = false; if (OnRelease) OnRelease(m); });
}

void GUIButton::Draw(ConsoleGUI* g) {
	WCHAR bg = pressed ? pressedBackground : background;
	WORD bgC = pressed ? pressedBackgroundColor : backgroundColor;
	WORD tC = pressed ? pressedTextColor : textColor;

	g->Rect(bounds, bg, bgC, true);
	if (pressed) pressedBorder.Draw(g, bounds);
	else border.Draw(g, bounds);

	RenderText(g,
			   bounds.left + (pressed ? pressedBorder.GetWidth() : border.GetWidth()),
			   bounds.right - (pressed ? pressedBorder.GetWidth() : border.GetWidth()),
			   bounds.top + (pressed ? pressedBorder.GetWidth() : border.GetWidth()),
			   bounds.bottom - (pressed ? pressedBorder.GetWidth() : border.GetWidth()),
			   tC);
}