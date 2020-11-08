#include "GUIElements.h"

namespace gui {

const int& EventHandler::GetId() const { return mId; }
void EventHandler::SetId(int i) { mId = i; }

bool EventHandler::PressActionExists() const { return (bool)mPressAction; }
void EventHandler::InvokePressAction(int i) { mPressAction(i); }
void EventHandler::SetPressAction(std::function<void(int)> f) { mPressAction = f; }

bool EventHandler::ReleaseActionExists() const { return (bool)mReleaseAction; }
void EventHandler::InvokeReleaseAction(int i) { mReleaseAction(i); }
void EventHandler::SetReleaseAction(std::function<void(int)> f) { mReleaseAction = f; }

const RECT& MouseHandler::GetBounds() { return mBounds; }
void MouseHandler::SetBounds(RECT b) { mBounds = b; }

const int& MouseHandler::GetButtons() { return mButtons; }
void MouseHandler::SetButtons(int b) { mButtons = b; }

Border::Border() {}
Border::Border(WCHAR ch, WORD cl) : mChar(ch), mColor(cl) {}
Border::Border(WCHAR ch, WORD cl, int w) : mChar(ch), mColor(cl), mWidth(w) {}
Border::Border(int w) : mWidth(w) {}

const WCHAR& Border::GetChar() const { return mChar; }
void Border::SetChar(WCHAR c) { mChar = c; }

const WORD& Border::GetColor() const { return mColor; }
void Border::SetColor(WORD c) { mColor = c; }

const int& Border::GetWidth() const { return mWidth; }
void Border::SetWidth(int w) { mWidth = w; }

Border::operator bool() const { return mWidth != 0; }

void Border::Draw(Console* g, RECT bd) {
	for (int b = 0; b < mWidth; b++) g->Rect({ bd.left + b, bd.top + b, bd.right - b, bd.bottom - b }, mChar, mColor, false);
}

Element::Element(RECT b) : mBounds(b) {}
Element::Element(const Element& e) : mBounds(e.mBounds), mBackground(e.mBackground), mBackgroundColor(e.mBackgroundColor) {}
Element::~Element() {}

const int& Element::GetId() const { return mId; }
void Element::SetId(int i) { mId = i; }

const RECT& Element::GetBounds() const { return mBounds; }
void Element::SetBounds(RECT b) { mBounds = b; }

const WCHAR& Element::GetBackground() const { return mBackground; }
void Element::SetBackground(WCHAR b) { mBackground = b; }

const WORD& Element::GetBackgroundColor() const { return mBackgroundColor; }
void Element::SetBackgroundColor(WORD c) { mBackgroundColor = c; }

Border& Element::GetBorder() { return mBorder; }
void Element::SetBorder(Border b) { mBorder = b; }

void Element::Draw(Console* g) {
	g->Rect(mBounds, mBackground, mBackgroundColor, true);
	mBorder.Draw(g, mBounds);
}

Label::Label(RECT b) : Element(b) {}
Label::Label(const Label& e) : Element(e), mText(e.mText), mTextColor(e.mTextColor), mAlignH(e.mAlignH), mAlignV(e.mAlignV) {}

void Label::UpdateTextLines() {
	mTextLines = 1;
	for (size_t ci = 0, li = 0; ci < mText.length(); ci++) {
		if (mText[ci] == L'\n') mTextLines++;
		if (ci - li > mBounds.right - mBounds.left - 2 * mBorder.GetWidth()) { mTextLines++; li = ci; }
	}
}

void Label::UpdateTextOffsetY() {
	mTextOffsetY = 0;
	if (mAlignV == TEXT_ALIGN_MID) mTextOffsetY = (mBounds.bottom - mBounds.top - 2 * mBorder.GetWidth() - mTextLines + 1) / 2;
	else if (mAlignV == TEXT_ALIGN_MAX) mTextOffsetY = mBounds.bottom - mBounds.top - 2 * mBorder.GetWidth() - mTextLines;
}

const std::wstring& Label::GetText() const { return mText; }
void Label::SetText(std::wstring t) { mText = t; UpdateTextLines(); }

const WORD& Label::GetTextColor() const { return mTextColor; }
void Label::SetTextColor(WORD c) { mTextColor = c; }

const int& Label::GetAlignHorizontal() const { return mAlignH; }
void Label::SetAlignHorizontal(int h) { mAlignH = h; }

const int& Label::GetAlignVertical() const { return mAlignV; }
void Label::SetAlignVertical(int v) { mAlignV = v; UpdateTextOffsetY(); }

const int& Label::GetTextWrap() const { return mTextWrap; }
void Label::SetTextWrap(int w) { mTextWrap = w; }

void Label::RenderText(Console* g, int minX, int maxX, int minY, int maxY, WORD c) {
	int y = minY + mTextOffsetY;

	size_t nlPos = mText.find(L'\n');
	size_t cIdx = 0;
	while (cIdx < mText.length() && y <= maxY) {
		// Line ends with char limit or newline
		// Add newlines as optional wrap bc find can be expensive
		// wrap should be bitwise so can or togeth
		// refactor bc linelen would be end - cidx, so can use if remaining or not??

		int elIdx = (nlPos == std::string::npos) ? mText.length() : nlPos;
		int lineLen = elIdx - cIdx;
		// Stay within bounds
		if (lineLen > maxX - minX) {
			if (mTextWrap == WRAP_WORD) {
				// Last char is idx of space
				elIdx = mText.rfind(L' ', cIdx + (maxX - minX));
				lineLen = elIdx - cIdx;
			} else if (mTextWrap == WRAP_CHAR || lineLen > maxX - minX) { 
				// def to char wrap if no spac
				elIdx -= (lineLen - (maxX - minX));
				lineLen = maxX - minX + 1;
			}
		}
		// Adjust line spacing x
		int xOffset = 0;
		if (mAlignH == TEXT_ALIGN_MID) xOffset = (maxX - minX - lineLen + 1) / 2;
		else if (mAlignH == TEXT_ALIGN_MAX)  xOffset = maxX - minX - lineLen + 1;

		g->Write(minX + xOffset, y, mText.substr(cIdx, lineLen), c);

		y++;
		cIdx = elIdx + 1;
		nlPos = mText.find(L'\n', cIdx);
	}
}

void Label::Draw(Console* g) {
	Element::Draw(g);
	RenderText(g, mBounds.left + mBorder.GetWidth(), mBounds.right - mBorder.GetWidth(), mBounds.top + mBorder.GetWidth(), mBounds.bottom - mBorder.GetWidth(), mTextColor);
}

Button::Button(RECT b) : Label(b), mHandler(), mPressAction(), mReleaseAction() { SetupHandler(); }

void Button::SetBounds(RECT b) { Element::SetBounds(b); mHandler.SetBounds(b); }

const WORD& Button::GetPressedTextColor() const { return mPressedTextColor; }
void Button::SetPressedTextColor(WORD c) { mPressedTextColor = c; }

const WCHAR& Button::GetPressedBackground() const { return mPressedBackground; }
void Button::SetPressedBackground(WCHAR b) { mPressedBackground = b; }

const WORD& Button::GetPressedBackgroundColor() const { return mPressedBackgroundColor; }
void Button::SetPressedBackgroundColor(WORD c) { mPressedBackgroundColor = c; }

Border& Button::GetPressedBorder() { return mPressedBorder; }
void Button::SetPressedBorder(Border b) { mPressedBorder = b; }

void Button::SetPressAction(std::function<void(int)> f) { mPressAction = f; }
void Button::SetReleaseAction(std::function<void(int)> f) { mReleaseAction = f; }

const int& Button::GetButtons() { int b = mHandler.GetButtons(); return b; }
void Button::SetButtons(int b) { mHandler.SetButtons(b); }

void Button::SetupHandler() {
	mHandler.SetBounds(mBounds);
	mHandler.SetPressAction([this](int m) { mPressed = true; if (mPressAction) mPressAction(m); });
	mHandler.SetReleaseAction([this](int m) { mPressed = false; if (mReleaseAction) mReleaseAction(m); });
}

void Button::Draw(Console* g) {
	WCHAR bg = mPressed ? mPressedBackground : mBackground;
	WORD bgC = mPressed ? mPressedBackgroundColor : mBackgroundColor;
	WORD tC = mPressed ? mPressedTextColor : mTextColor;

	g->Rect(mBounds, bg, bgC, true);
	if (mPressed) mPressedBorder.Draw(g, mBounds);
	else mBorder.Draw(g, mBounds);

	RenderText(g,
			   mBounds.left + (mPressed ? mPressedBorder.GetWidth() : mBorder.GetWidth()),
			   mBounds.right - (mPressed ? mPressedBorder.GetWidth() : mBorder.GetWidth()),
			   mBounds.top + (mPressed ? mPressedBorder.GetWidth() : mBorder.GetWidth()),
			   mBounds.bottom - (mPressed ? mPressedBorder.GetWidth() : mBorder.GetWidth()),
			   tC);
}

Panel::Panel(RECT b) : Element(b), mTitleLabel({ b.left, b.top, b.right, b.top + mTitleHeight - 1 }) {
	mTitleLabel.SetAlignHorizontal(TEXT_ALIGN_MID);
	mTitleLabel.SetAlignVertical(TEXT_ALIGN_MID);
}

Label& Panel::GetTitleLabel() { return mTitleLabel; }

const int& Panel::GetTitleHeight() const { return mTitleHeight; }
void Panel::SetTitleHeight(int h) { mTitleHeight = h; }

void Panel::SetBounds(RECT b) { Element::SetBounds(b); mTitleLabel.SetBounds({ b.left, b.top, b.right, b.top + mTitleHeight - 1 }); }

void Panel::Draw(Console* g) {
	Element::Draw(g);
	mTitleLabel.Draw(g);
}

ContentPanel::ContentPanel(RECT b) : Panel(b) {}
ContentPanel::ContentPanel(RECT b, Element* c) : Panel(b), mpContent(c) {}

Element* ContentPanel::GetContent() { return mpContent; }
void ContentPanel::SetContent(Element* c) { mpContent = c; }

void ContentPanel::SetBounds(RECT b) { Panel::SetBounds(b); if (mpContent) mpContent->SetBounds({ b.left, b.top + mTitleHeight, b.right, b.bottom }); }

void ContentPanel::Draw(Console* g) {
	Panel::Draw(g);
	if (mpContent) mpContent->Draw(g);
}

}