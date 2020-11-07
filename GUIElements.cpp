#include "GUIElements.h"

GUIBorder::GUIBorder() {}
GUIBorder::GUIBorder(WCHAR ch, WORD cl) : chr(ch), color(cl) {}
GUIBorder::GUIBorder(WCHAR ch, WORD cl, int w) : chr(ch), color(cl), width(w) {}
GUIBorder::GUIBorder(int w) : width(w) {}

GUIBorder::operator bool() const { return width != 0; }

void GUIBorder::Draw(ConsoleGUI* g, RECT bd) {
	for (int b = 0; b < width; b++) g->Rect({ bd.left + b, bd.top + b, bd.right - b, bd.bottom - b }, chr, color, false);
}

GUIElement::GUIElement(RECT b) : bounds(b) {}
GUIElement::GUIElement(const GUIElement& e) : bounds(e.bounds), background(e.background), backgroundColor(e.backgroundColor) {}
GUIElement::~GUIElement() {}

void GUIElement::Draw(ConsoleGUI* g) {
	g->Rect(bounds, background, backgroundColor, true);
	border.Draw(g, bounds);
}

GUILabel::GUILabel(RECT b) : GUIElement(b) {}
GUILabel::GUILabel(const GUILabel& e) : GUIElement(e), text(e.text), textColor(e.textColor), hAlignment(e.hAlignment), vAlignment(e.vAlignment) {}

void GUILabel::RenderText(ConsoleGUI* g, int minX, int maxX, int minY, int maxY, WORD c) {
	int yOffset = 0; // default to min
	switch (vAlignment) {
	case TEXT_ALIGN_MID:
		yOffset = (maxY - minY - std::count(text.begin(), text.end(), L'\n') + 1) / 2;
		break;
	case TEXT_ALIGN_MAX:
		yOffset = maxY - minY - std::count(text.begin(), text.end(), L'\n');
	default:
		break;
	}

	int y = minY + yOffset;

	int nlPos = text.find(L'\n');
	int cIdx = 0;

	while (cIdx < text.length() && y <= maxY) {
		int elIdx = (nlPos == std::string::npos) ? text.length() : nlPos;
		int lineLen = elIdx - cIdx;

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
	RenderText(g, bounds.left + border.width, bounds.right - border.width, bounds.top + border.width, bounds.bottom - border.width, textColor);
}

GUIButton::GUIButton(RECT b) : GUILabel(b), handler(), OnPress(), OnRelease() { SetupHandler(); }

void GUIButton::SetupHandler() {
	handler.bounds = bounds;
	handler.OnPress = [this](int m) { pressed = true; if (OnPress) OnPress(m); };
	handler.OnRelease = [this](int m) { pressed = false; if (OnRelease) OnRelease(m); };
}

void GUIButton::Draw(ConsoleGUI* g) {
	WCHAR bg = pressed ? pressedBackground : background;
	WORD bgC = pressed ? pressedBackgroundColor : backgroundColor;
	WORD tC = pressed ? pressedTextColor : textColor;

	g->Rect(bounds, bg, bgC, true);
	if (pressed) pressedBorder.Draw(g, bounds);
	else border.Draw(g, bounds);

	RenderText(g,
			   bounds.left + (pressed ? pressedBorder.width : border.width),
			   bounds.right - (pressed ? pressedBorder.width : border.width),
			   bounds.top + (pressed ? pressedBorder.width : border.width),
			   bounds.bottom - (pressed ? pressedBorder.width : border.width),
			   tC);

}