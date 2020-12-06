#include "GUIElements.h"

namespace gui {

EventHandler::EventHandler() : mPressAction(), mReleaseAction() {}

const int& EventHandler::GetId() const { return mId; }
void EventHandler::SetId(int i) { mId = i; }

bool EventHandler::PressActionExists() const { return (bool)mPressAction; }
void EventHandler::InvokePressAction(Console* c, int i) { mPressAction(c, i); }
void EventHandler::SetPressAction(std::function<void(Console*, int)> f) { mPressAction = f; }

bool EventHandler::ReleaseActionExists() const { return (bool)mReleaseAction; }
void EventHandler::InvokeReleaseAction(Console* c, int i) { mReleaseAction(c, i); }
void EventHandler::SetReleaseAction(std::function<void(Console*, int)> f) { mReleaseAction = f; }

MouseHandler::MouseHandler() {}
MouseHandler::MouseHandler(RECT b) : mBounds(b) {}
MouseHandler::MouseHandler(RECT b, int bt) : mBounds(b), mButtons(bt) {}

const RECT& MouseHandler::GetBounds() { return mBounds; }
void MouseHandler::SetBounds(RECT b) { mBounds = b; }

const int& MouseHandler::GetButtons() { return mButtons; }
void MouseHandler::SetButtons(int b) { mButtons = b; }

KeyboardHandler::KeyboardHandler() : EventHandler() {}
KeyboardHandler::KeyboardHandler(std::wstring k) : EventHandler(), mKeys(k) {}

const std::wstring& KeyboardHandler::GetKeys() const { return mKeys; }
void KeyboardHandler::SetKeys(std::wstring k) { mKeys = k; }

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

void Border::Draw(Console* c, RECT bd) {
	for (int b = 0; b < mWidth; b++) c->Rect({ bd.left + b, bd.top + b, bd.right - b, bd.bottom - b }, mChar, mColor, false);
}

Element::Element(RECT b) : mBounds(b), mMouseHandler(nullptr), mKeyboardHandler(nullptr) {}
Element::Element(const Element& e) : mBounds(e.mBounds), mBackground(e.mBackground), mBackgroundColor(e.mBackgroundColor), mMouseHandler(e.mMouseHandler), mKeyboardHandler(e.mKeyboardHandler) {}
Element::~Element() { if (mMouseHandler) delete mMouseHandler; if (mKeyboardHandler) delete mKeyboardHandler; }

const int& Element::GetId() const { return mId; }
void Element::SetId(int i) { mId = i; }

const RECT& Element::GetBounds() const { return mBounds; }
void Element::SetBounds(RECT b) { mBounds = b; if (mMouseHandler) mMouseHandler->SetBounds(b); }

const WCHAR& Element::GetBackground() const { return mBackground; }
void Element::SetBackground(WCHAR b) { mBackground = b; }

const WORD& Element::GetBackgroundColor() const { return mBackgroundColor; }
void Element::SetBackgroundColor(WORD c) { mBackgroundColor = c; }

const Border& Element::GetBorder() const { return mBorder; }
void Element::SetBorder(Border b) { mBorder = b; }

void Element::SetupHandlers() {}

void Element::Draw(Console* c) {
	c->Rect(mBounds, mBackground, mBackgroundColor, true);
	mBorder.Draw(c, mBounds);
}

Label::Label(RECT b) : Element(b) {}
Label::Label(const Label& e) : Element(e), mText(e.mText), mTextColor(e.mTextColor), mAlignH(e.mAlignH), mAlignV(e.mAlignV) {}

void Label::UpdateTextLines() {
	mTextLines = 1;
	for (int ci = 0, li = 0; ci < (int)mText.length(); ci++) {
		if (mText[ci] == L'\n') { mTextLines++; li = ci; }
		if (ci - li > mBounds.right - mBounds.left - 2 * mBorder.GetWidth()) { mTextLines++; li = ci; }
	}
}

void Label::UpdateTextOffsetY() {
	mTextOffsetY = 0;
	if (mAlignV == TEXT_ALIGN_MID) mTextOffsetY = (mBounds.bottom - mBounds.top - 2 * mBorder.GetWidth() - mTextLines + 1) / 2;
	else if (mAlignV == TEXT_ALIGN_MAX) mTextOffsetY = mBounds.bottom - mBounds.top - 2 * mBorder.GetWidth() - mTextLines;
}

const std::wstring& Label::GetText() const { return mText; }
void Label::SetText(std::wstring t) { mText = t; UpdateTextLines(); UpdateTextOffsetY(); }

const WORD& Label::GetTextColor() const { return mTextColor; }
void Label::SetTextColor(WORD c) { mTextColor = c; }

const int& Label::GetAlignHorizontal() const { return mAlignH; }
void Label::SetAlignHorizontal(int h) { mAlignH = h; }

const int& Label::GetAlignVertical() const { return mAlignV; }
void Label::SetAlignVertical(int v) { mAlignV = v; UpdateTextOffsetY(); }

const int& Label::GetTextWrap() const { return mTextWrap; }
void Label::SetTextWrap(int w) { mTextWrap = w; }

void Label::SetBorder(Border b) { mBorder = b; UpdateTextLines(); UpdateTextOffsetY(); }

void Label::RenderText(Console* c, int minX, int maxX, int minY, int maxY, std::wstring s, WORD cl) {
	int spanX = maxX - minX;
	int y = minY + mTextOffsetY;

	size_t nlPos = s.find(L'\n');
	size_t cIdx = 0;
	while (cIdx < s.length() && y <= maxY) {
		// Line ends with char limit or newline
		// Add newlines as optional wrap bc find can be expensive
		// wrap should be bitwise so can or togeth
		// refactor bc linelen would be end - cidx, so can use if remaining or not??

		int elIdx = (nlPos == std::string::npos) ? s.length() : nlPos;
		int lineLen = elIdx - cIdx;
		// Stay within bounds
		if (lineLen > maxX - minX) {
			if (mTextWrap == WRAP_WORD) {
				// Last char is idx of space
				elIdx = s.rfind(L' ', cIdx + spanX);
				lineLen = elIdx - cIdx;
			} else if (mTextWrap == WRAP_CHAR || lineLen > spanX) { 
				// def to char wrap if no spac
				elIdx -= (lineLen - spanX);
				lineLen = spanX + 1;
			}
		}
		// Adjust line spacing x
		int xOffset = 0;
		if (mAlignH == TEXT_ALIGN_MID) xOffset = (spanX - lineLen + 1) / 2;
		else if (mAlignH == TEXT_ALIGN_MAX)  xOffset = spanX - lineLen + 1;

		c->Write(minX + xOffset, y, s.substr(cIdx, lineLen), cl);

		y++;
		cIdx = elIdx + 1;
		nlPos = s.find(L'\n', cIdx);
	}
}

void Label::Draw(Console* c) {
	Element::Draw(c);
	RenderText(
		c, 
		mBounds.left + mBorder.GetWidth(), 
		mBounds.right - mBorder.GetWidth(), 
		mBounds.top + mBorder.GetWidth(), 
		mBounds.bottom - mBorder.GetWidth(), 
		mText, 
		mTextColor
	);
}

Button::Button(RECT b) : Label(b), mPressAction(), mReleaseAction() { SetupHandlers(); }

const WORD& Button::GetPressedTextColor() const { return mPressedTextColor; }
void Button::SetPressedTextColor(WORD c) { mPressedTextColor = c; }

const WCHAR& Button::GetPressedBackground() const { return mPressedBackground; }
void Button::SetPressedBackground(WCHAR b) { mPressedBackground = b; }

const WORD& Button::GetPressedBackgroundColor() const { return mPressedBackgroundColor; }
void Button::SetPressedBackgroundColor(WORD c) { mPressedBackgroundColor = c; }

const Border& Button::GetPressedBorder() const { return mPressedBorder; }
void Button::SetPressedBorder(Border b) { mPressedBorder = b; }

void Button::SetPressAction(std::function<void(int)> f) { mPressAction = f; }
void Button::SetReleaseAction(std::function<void(int)> f) { mReleaseAction = f; }

const int& Button::GetButtons() { return mMouseHandler->GetButtons(); }
void Button::SetButtons(int b) { mMouseHandler->SetButtons(b); }

void Button::SetupHandlers() {
	mMouseHandler = new MouseHandler(mBounds, MOUSE_LEFT_BUTTON);
	mMouseHandler->SetPressAction([this](Console* c, int m) { mPressed = true; if (mPressAction) mPressAction(m); });
	mMouseHandler->SetReleaseAction([this](Console* c, int m) { mPressed = false; if (mReleaseAction) mReleaseAction(m); });
}

void Button::Draw(Console* c) {
	WCHAR bg = mPressed ? mPressedBackground : mBackground;
	WORD bgC = mPressed ? mPressedBackgroundColor : mBackgroundColor;
	WORD tC = mPressed ? mPressedTextColor : mTextColor;

	c->Rect(mBounds, bg, bgC, true);
	if (mPressed) mPressedBorder.Draw(c, mBounds);
	else mBorder.Draw(c, mBounds);

	RenderText(c,
		mBounds.left + (mPressed ? mPressedBorder.GetWidth() : mBorder.GetWidth()),
		mBounds.right - (mPressed ? mPressedBorder.GetWidth() : mBorder.GetWidth()),
		mBounds.top + (mPressed ? mPressedBorder.GetWidth() : mBorder.GetWidth()),
		mBounds.bottom - (mPressed ? mPressedBorder.GetWidth() : mBorder.GetWidth()),
		mText,
		tC
	);
}

std::wstring Charset::Get(CharsetT t) {
	switch (t) {
	case CharsetT::NUMERIC:
		return L"0123456789";
	case CharsetT::ALPHABET:
		return L" ABCDEFGHIJKLMNOPQRSTUVWXYZ";
	case CharsetT::ALPHANUM:
		return L" ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
	default:
		return L"";
	}
}

TextField::TextField(RECT b) : Label(b) { SetupHandlers(); }
TextField::TextField(RECT b, CharsetT c) : Label(b), mCharset(c) { SetupHandlers(); }

void TextField::Backspace() {
	while (mDeleting && mText.length() > 0) {
		mText = mText.substr(0, mText.length() - 1);
		mNumDeleted++;
		std::this_thread::sleep_for(std::chrono::milliseconds(300 - std::min(200, mNumDeleted * 50)));
	}
}

void TextField::SetupHandlers() {

	mMouseHandler = new MouseHandler(mBounds, MOUSE_LEFT_BUTTON);

	mMouseHandler->SetPressAction([this](Console* c, int m) {
		c->SetActiveKeyboardHandler(mKeyboardHandler);
		mEnabled = true;
	});

	mKeyboardHandler = new KeyboardHandler(L"\x08\x0D\x10 " + Charset::Get(mCharset));

	mKeyboardHandler->SetPressAction([this](Console* c, int k) {
		if (k == VK_SHIFT) mCapitalize = true;
		else if (k == VK_BACK) {
			mDeleting = true;
			// Have only 1 thread at a time, create new only when exit
			if (!mDeleteFuture.valid() || mDeleteFuture.wait_for(std::chrono::milliseconds(0)) == std::future_status::ready) {
				mDeleteFuture = ExecuteAsync(std::bind(&TextField::Backspace, this));
			}
		} else if (k == VK_RETURN) {
			mText += L'\n';
		} else {
			// Get text for vk
			UINT sc = MapVirtualKey(k, MAPVK_VK_TO_VSC);
			static byte b[256];
			GetKeyboardState(b);
			if (mCapitalize) b[0x10] = 0x80;
			WCHAR c;
			int r = ToUnicode((UINT)k, sc, b, &c, 1, 0);
			mText += c;
		}
	});

	mKeyboardHandler->SetReleaseAction([this](Console* c, int k) {
		if (k == VK_SHIFT) mCapitalize = false;
		else if (k == VK_BACK) {
			mDeleting = false;
			mNumDeleted = 0;
		}
	});
}

const WORD& TextField::GetEnabledTextColor() const { return mEnabledTextColor; }
void TextField::SetEnabledTextColor(WORD c) { mEnabledTextColor = c; }

const WCHAR& TextField::GetEnabledBackground() const { return mEnabledBackground; }
void TextField::SetEnabledBackground(WCHAR b) { mEnabledBackground = b; }

const WORD& TextField::GetEnabledBackgroundColor() const { return mEnabledBackgroundColor; }
void TextField::SetEnabledBackgroundColor(WORD c) { mEnabledBackgroundColor = c; }

const Border& TextField::GetEnabledBorder() const { return mEnabledBorder; }
void TextField::SetEnabledBorder(Border b) { mEnabledBorder = b; }

const bool& TextField::GetEnabled() const { return mEnabled; }
void TextField::SetEnabled(bool b) { mEnabled = b; }

void TextField::Draw(Console* c) {

	WCHAR bg = mEnabled ? mEnabledBackground : mBackground;
	WORD bgC = mEnabled ? mEnabledBackgroundColor : mBackgroundColor;
	WORD tC = mEnabled ? mEnabledTextColor : mTextColor;

	c->Rect(mBounds, bg, bgC, true);
	if (mEnabled) mEnabledBorder.Draw(c, mBounds);
	else mBorder.Draw(c, mBounds);

	RenderText(c,
		mBounds.left + (mEnabled ? mEnabledBorder.GetWidth() : mBorder.GetWidth()),
		mBounds.right - (mEnabled ? mEnabledBorder.GetWidth() : mBorder.GetWidth()),
		mBounds.top + (mEnabled ? mEnabledBorder.GetWidth() : mBorder.GetWidth()),
		mBounds.bottom - (mEnabled ? mEnabledBorder.GetWidth() : mBorder.GetWidth()),
		mText + (mEnabled ? L"_" : L""),
		tC
	);
}

Panel::Panel(RECT b) : Element(b), mTitleLabel({ b.left, b.top, b.right, b.top + mTitleHeight - 1 }) {
	mTitleLabel.SetAlignHorizontal(TEXT_ALIGN_MID);
	mTitleLabel.SetAlignVertical(TEXT_ALIGN_MID);
}

Label& Panel::GetTitleLabel() { return mTitleLabel; }

const int& Panel::GetTitleHeight() const { return mTitleHeight; }
void Panel::SetTitleHeight(int h) { mTitleHeight = h; SetBounds(mBounds); }

void Panel::SetBounds(RECT b) { Element::SetBounds(b); mTitleLabel.SetBounds({ b.left, b.top, b.right, b.top + mTitleHeight - 1 }); }

void Panel::Draw(Console* c) {
	Element::Draw(c);
	mTitleLabel.Draw(c);
}

ContentPanel::ContentPanel(RECT b) : Panel(b) {}
ContentPanel::ContentPanel(RECT b, Element* c) : Panel(b), mContent(c) {}

Element* ContentPanel::GetContent() { return mContent; }
void ContentPanel::SetContent(Element* c) { mContent = c; SetBounds(mBounds); }

void ContentPanel::SetBounds(RECT b) { Panel::SetBounds(b); if (mContent) mContent->SetBounds({ b.left, b.top + mTitleHeight, b.right, b.bottom }); }

void ContentPanel::Draw(Console* c) {
	Panel::Draw(c);
	if (mContent) mContent->Draw(c);
}

}