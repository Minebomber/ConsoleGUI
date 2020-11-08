#include "GUIElements.h"

namespace gui {

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

Border::Border() {}
Border::Border(WCHAR ch, WORD cl) : chr(ch), color(cl) {}
Border::Border(WCHAR ch, WORD cl, int w) : chr(ch), color(cl), width(w) {}
Border::Border(int w) : width(w) {}

const WCHAR& Border::GetChar() const { return chr; }
void Border::SetChar(WCHAR c) { chr = c; }

const WORD& Border::GetColor() const { return color; }
void Border::SetColor(WORD c) { color = c; }

const int& Border::GetWidth() const { return width; }
void Border::SetWidth(int w) { width = w; }

Border::operator bool() const { return width != 0; }

void Border::Draw(Console* g, RECT bd) {
	for (int b = 0; b < width; b++) g->Rect({ bd.left + b, bd.top + b, bd.right - b, bd.bottom - b }, chr, color, false);
}

Element::Element(RECT b) : bounds(b) {}
Element::Element(const Element& e) : bounds(e.bounds), background(e.background), backgroundColor(e.backgroundColor) {}
Element::~Element() {}

const int& Element::GetId() const { return id; }
void Element::SetId(int i) { id = i; }

const RECT& Element::GetBounds() const { return bounds; }
void Element::SetBounds(RECT b) { bounds = b; }

const WCHAR& Element::GetBackground() const { return background; }
void Element::SetBackground(WCHAR b) { background = b; }

const WORD& Element::GetBackgroundColor() const { return backgroundColor; }
void Element::SetBackgroundColor(WORD c) { backgroundColor = c; }

Border& Element::GetBorder() { return border; }
void Element::SetBorder(Border b) { border = b; }

void Element::Draw(Console* g) {
	g->Rect(bounds, background, backgroundColor, true);
	border.Draw(g, bounds);
}

Label::Label(RECT b) : Element(b) {}
Label::Label(const Label& e) : Element(e), text(e.text), textColor(e.textColor), hAlignment(e.hAlignment), vAlignment(e.vAlignment) {}

void Label::UpdateTextLines() {
	textLines = 1;
	for (size_t ci = 0, li = 0; ci < text.length(); ci++) {
		if (text[ci] == L'\n') textLines++;
		if (ci - li > bounds.right - bounds.left - 2 * border.GetWidth()) { textLines++; li = ci; }
	}
}

void Label::UpdateTextOffsetY() {
	textOffsetY = 0;
	if (vAlignment == TEXT_ALIGN_MID) textOffsetY = (bounds.bottom - bounds.top - 2 * border.GetWidth() - textLines + 1) / 2;
	else if (vAlignment == TEXT_ALIGN_MAX) textOffsetY = bounds.bottom - bounds.top - 2 * border.GetWidth() - textLines;
}

const std::wstring& Label::GetText() const { return text; }
void Label::SetText(std::wstring t) { text = t; UpdateTextLines(); }

const WORD& Label::GetTextColor() const { return textColor; }
void Label::SetTextColor(WORD c) { textColor = c; }

const int& Label::GetHorizontalAlignment() const { return hAlignment; }
void Label::SetHorizontalAlignment(int h) { hAlignment = h; }

const int& Label::GetVerticalAlignment() const { return vAlignment; }
void Label::SetVerticalAlignment(int v) { vAlignment = v; UpdateTextOffsetY(); }

const int& Label::GetTextWrap() const { return textWrap; }
void Label::SetTextWrap(int w) { textWrap = w; }

void Label::RenderText(Console* g, int minX, int maxX, int minY, int maxY, WORD c) {
	int y = minY + textOffsetY;

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
		if (hAlignment == TEXT_ALIGN_MID) xOffset = (maxX - minX - lineLen + 1) / 2;
		else if (hAlignment == TEXT_ALIGN_MAX)  xOffset = maxX - minX - lineLen + 1;

		g->Write(minX + xOffset, y, text.substr(cIdx, lineLen), c);

		y++;
		cIdx = elIdx + 1;
		nlPos = text.find(L'\n', cIdx);
	}
}

void Label::Draw(Console* g) {
	Element::Draw(g);
	RenderText(g, bounds.left + border.GetWidth(), bounds.right - border.GetWidth(), bounds.top + border.GetWidth(), bounds.bottom - border.GetWidth(), textColor);
}

Button::Button(RECT b) : Label(b), handler(), OnPress(), OnRelease() { SetupHandler(); }

void Button::SetBounds(RECT b) { Element::SetBounds(b); handler.SetBounds(b); }

const WORD& Button::GetPressedTextColor() const { return pressedTextColor; }
void Button::SetPressedTextColor(WORD c) { pressedTextColor = c; }

const WCHAR& Button::GetPressedBackground() const { return pressedBackground; }
void Button::SetPressedBackground(WCHAR b) { pressedBackground = b; }

const WORD& Button::GetPressedBackgroundColor() const { return pressedBackgroundColor; }
void Button::SetPressedBackgroundColor(WORD c) { pressedBackgroundColor = c; }

Border& Button::GetPressedBorder() { return pressedBorder; }
void Button::SetPressedBorder(Border b) { pressedBorder = b; }

MouseHandler& Button::GetHandler() { return handler; }

void Button::SetupHandler() {
	handler.SetBounds(bounds);
	handler.SetOnPress([this](int m) { pressed = true; if (OnPress) OnPress(m); });
	handler.SetOnRelease([this](int m) { pressed = false; if (OnRelease) OnRelease(m); });
}

void Button::Draw(Console* g) {
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

Panel::Panel(RECT b) : Element(b), titleLabel({ b.left, b.top, b.right, b.top + titleHeight - 1 }) {
	titleLabel.SetHorizontalAlignment(TEXT_ALIGN_MID);
	titleLabel.SetVerticalAlignment(TEXT_ALIGN_MID);
}

Label& Panel::GetTitleLabel() { return titleLabel; }

const int& Panel::GetTitleHeight() const { return titleHeight; }
void Panel::SetTitleHeight(int h) { titleHeight = h; }

void Panel::SetBounds(RECT b) { Element::SetBounds(b); titleLabel.SetBounds({ b.left, b.top, b.right, b.top + titleHeight - 1 }); }

void Panel::Draw(Console* g) {
	Element::Draw(g);
	titleLabel.Draw(g);
}

ContentPanel::ContentPanel(RECT b) : Panel(b) {}
ContentPanel::ContentPanel(RECT b, Element* c) : Panel(b), content(c) {}

Element* ContentPanel::GetContent() { return content; }
void ContentPanel::SetContent(Element* c) { content = c; }

void ContentPanel::SetBounds(RECT b) { Panel::SetBounds(b); if (content) content->SetBounds({ b.left, b.top + titleHeight, b.right, b.bottom }); }

void ContentPanel::Draw(Console* g) {
	Panel::Draw(g);
	if (content) content->Draw(g);
}

}