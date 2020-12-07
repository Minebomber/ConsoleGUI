#pragma once
#include "Console.h"
#include "Window.h"

namespace gui {

enum Colors {
	FG_BLACK = 0x0000,
	FG_DARK_BLUE = 0x0001,
	FG_DARK_GREEN = 0x0002,
	FG_DARK_CYAN = 0x0003,
	FG_DARK_RED = 0x0004,
	FG_DARK_MAGENTA = 0x0005,
	FG_DARK_YELLOW = 0x0006,
	FG_GREY = 0x0007,
	FG_DARK_GREY = 0x0008,
	FG_BLUE = 0x0009,
	FG_GREEN = 0x000A,
	FG_CYAN = 0x000B,
	FG_RED = 0x000C,
	FG_MAGENTA = 0x000D,
	FG_YELLOW = 0x000E,
	FG_WHITE = 0x000F,
	BG_BLACK = 0x0000,
	BG_DARK_BLUE = 0x0010,
	BG_DARK_GREEN = 0x0020,
	BG_DARK_CYAN = 0x0030,
	BG_DARK_RED = 0x0040,
	BG_DARK_MAGENTA = 0x0050,
	BG_DARK_YELLOW = 0x0060,
	BG_GREY = 0x0070,
	BG_DARK_GREY = 0x0080,
	BG_BLUE = 0x0090,
	BG_GREEN = 0x00A0,
	BG_CYAN = 0x00B0,
	BG_RED = 0x00C0,
	BG_MAGENTA = 0x00D0,
	BG_YELLOW = 0x00E0,
	BG_WHITE = 0x00F0,
};

enum MouseButtons {
	MOUSE_LEFT_BUTTON = 0b001,
	MOUSE_RIGHT_BUTTON = 0b010,
	MOUSE_CENTER_BUTTON = 0b100,
};

enum TextAlignment {
	TEXT_ALIGN_MIN,
	TEXT_ALIGN_MID,
	TEXT_ALIGN_MAX,
};

enum TextWrap {
	WRAP_CHAR,
	WRAP_WORD,
};

class Charset {
public:
	static std::wstring Numeric() { return L"0123456789"; }
	static std::wstring Alphabet() { return L" ABCDEFGHIJKLMNOPQRSTUVWXYZ"; }
	static std::wstring Alphanum() { return L" ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789"; }
};

class Window;

class EventHandler {
	friend class Window;
	friend class Console;
protected:
	int mId = -1;
	std::function<void(Window*, int)> mPressAction;
	std::function<void(Window*, int)> mReleaseAction;
public:
	EventHandler() : mPressAction(), mReleaseAction() {}
	EventHandler(const EventHandler& h) : mPressAction(h.mPressAction), mReleaseAction(h.mReleaseAction) {}

	const int& GetId() const { return mId; }
	void SetId(int i) { mId = i; }

	bool PressActionExists() const { return (bool)mPressAction; }
	void InvokePressAction(Window *c, int i) { mPressAction(c, i); }
	void SetPressAction(std::function<void(Window*, int)> f) { mPressAction = f; }

	bool ReleaseActionExists() const { return (bool)mReleaseAction; }
	void InvokeReleaseAction(Window* c, int i) { mReleaseAction(c, i); }
	void SetReleaseAction(std::function<void(Window*, int)> f) { mReleaseAction = f; }
};

class MouseHandler : public EventHandler {
	friend class Window;
	friend class Console;
protected:
	RECT mBounds = { 0, 0, 0, 0 };
	int mButtons = 0;
public:
	MouseHandler() : EventHandler() {}
	MouseHandler(RECT b) : EventHandler(), mBounds(b) {}
	MouseHandler(RECT b, int bt) : EventHandler(), mBounds(b), mButtons(bt) {}
	MouseHandler(const MouseHandler& h) : EventHandler(h), mBounds(h.mBounds), mButtons(h.mButtons) {}

	const RECT& GetBounds() { return mBounds; }
	void SetBounds(RECT b) { mBounds = b; }

	const int& GetButtons() { return mButtons; }
	void SetButtons(int b) { mButtons = b; }
};

class KeyboardHandler : public EventHandler {
	friend class Window;
	friend class Console;
protected:
	std::wstring mKeys = L"";
public:
	KeyboardHandler() : EventHandler() {}
	KeyboardHandler(std::wstring k) : EventHandler(), mKeys(k) {}
	KeyboardHandler(const KeyboardHandler& h) : EventHandler(h), mKeys(h.mKeys) {}

	const std::wstring& GetKeys() const { return mKeys; }
	void SetKeys(std::wstring k) { mKeys = k; }
};

class Border {
	friend class Window;
protected:
	WORD mColor = FG_WHITE;
	int mWidth = 1;
public:
	Border() {}
	Border(WORD cl, int w) : mColor(cl), mWidth(w) {}
	Border(int w) : mWidth(w) {}

	virtual Border* Clone() { return new Border(mColor, mWidth); }

	const WORD& GetColor() const { return mColor; }
	void SetColor(WORD c) { mColor = c; }

	const int& GetWidth() const { return mWidth; }
	void SetWidth(int w) { mWidth = w; }

	operator bool() const { return mWidth != 0; }

	virtual void Draw(Window* c, RECT b);
};

class TitledBorder : public Border {
	friend class Window;
protected:
	std::wstring mTitle = L"";
public:
	TitledBorder(std::wstring t) : Border(), mTitle(t) {}
	TitledBorder(std::wstring t, WORD cl) : Border(cl, 1), mTitle(t) {}
	TitledBorder(std::wstring t, WORD cl, int w) : Border(cl, w), mTitle(t) {}

	virtual Border* Clone() override { return new TitledBorder(mTitle, mColor, mWidth); }

	const std::wstring& GetTitle() const { return mTitle; }
	void SetTitle(std::wstring s) { mTitle = s; }
	
	virtual void Draw(Window* c, RECT b) override;
};

class Element {
	friend class Window;
protected:
	int mId = -1;
	RECT mBounds = { 0, 0, 0, 0 };
	WORD mBackgroundColor = BG_WHITE;
	Border* mBorder = nullptr;
	MouseHandler* mMouseHandler = nullptr;
	KeyboardHandler* mKeyboardHandler = nullptr;

	virtual void SetupHandlers() {}
public:
	Element(RECT b) : mBounds(b), mBorder(new Border(FG_WHITE, 0)), mMouseHandler(nullptr), mKeyboardHandler(nullptr) {}
	Element(const Element& e) : mBounds(e.mBounds), mBackgroundColor(e.mBackgroundColor), mBorder(e.mBorder->Clone()), mMouseHandler(e.mMouseHandler), mKeyboardHandler(e.mKeyboardHandler) {}
	virtual ~Element() { if (mBorder) delete mBorder;  if (mMouseHandler) delete mMouseHandler; if (mKeyboardHandler) delete mKeyboardHandler; }

	const int& GetId() const { return mId; }
	void SetId(int i) { mId = i; }

	const RECT& GetBounds() const { return mBounds; }
	virtual void SetBounds(RECT b) { mBounds = b; if (mMouseHandler) mMouseHandler->SetBounds(b); }

	const WORD& GetBackgroundColor() const { return mBackgroundColor; }
	void SetBackgroundColor(WORD c) { mBackgroundColor = c; }

	Border& GetBorder() { return *mBorder; }
	virtual void SetBorder(Border* b) { if (mBorder) delete mBorder; mBorder = b; }

	virtual void Draw(Window* c);
};

class Label : public Element {
	friend class Window;
protected:
	std::wstring mText = L"";
	WORD mTextColor = FG_WHITE;
	int mAlignH = TEXT_ALIGN_MIN;
	int mAlignV = TEXT_ALIGN_MIN;
	int mTextWrap = WRAP_CHAR;

	int mTextLines = 1;
	virtual void UpdateTextLines();

	int mTextOffsetY = 0;
	virtual void UpdateTextOffsetY();

	void RenderText(Window* c, int minX, int maxX, int minY, int maxY, std::wstring s, WORD cl);
public:
	Label(RECT b) : Element(b) {}
	Label(const Label& e) : Element(e), mText(e.mText), mTextColor(e.mTextColor), mAlignH(e.mAlignH), mAlignV(e.mAlignV), mTextWrap(e.mTextWrap), mTextLines(e.mTextLines), mTextOffsetY(e.mTextOffsetY) {}

	const std::wstring& GetText() const { return mText; }
	void SetText(std::wstring t) { mText = t; UpdateTextLines(); UpdateTextOffsetY(); }

	const WORD& GetTextColor() const { return mTextColor; }
	void SetTextColor(WORD c) { mTextColor = c; }

	const int& GetAlignHorizontal() const { return mAlignH; }
	void SetAlignHorizontal(int h) { mAlignH = h; }

	const int& GetAlignVertical() const { return mAlignV; }
	void SetAlignVertical(int v) { mAlignV = v; UpdateTextOffsetY(); }

	const int& GetTextWrap() const { return mTextWrap; }
	void SetTextWrap(int w) { mTextWrap = w; }

	void SetBorder(Border* b) override { Element::SetBorder(b); UpdateTextLines(); UpdateTextOffsetY(); }

	virtual void SetBounds(RECT b) override { Element::SetBounds(b); UpdateTextLines(); UpdateTextOffsetY();}

	virtual void Draw(Window* c) override;
};

class Button : public Label {
	friend class Window;
protected:
	WORD mPressedTextColor = FG_WHITE;
	WORD mPressedBackgroundColor = BG_WHITE;
	Border* mPressedBorder = { 0 };
	bool mPressed = false;

	std::function<void(int)> mPressAction;
	std::function<void(int)> mReleaseAction;

	virtual void SetupHandlers() override;
public:
	Button(RECT b) : Label(b), mPressedBorder(new Border(FG_WHITE, 0)), mPressAction(), mReleaseAction() { SetupHandlers(); }
	Button(const Button& e) : Label(e), mPressedBorder(e.mPressedBorder->Clone()), mPressedTextColor(e.mPressedTextColor), mPressedBackgroundColor(e.mPressedBackgroundColor), mPressed(e.mPressed), mPressAction(e.mPressAction), mReleaseAction(e.mReleaseAction) { SetupHandlers();  }
	virtual ~Button() { if (mPressedBorder) delete mPressedBorder; }

	const WORD& GetPressedTextColor() const { return mPressedTextColor; }
	void SetPressedTextColor(WORD c) { mPressedTextColor = c; }

	const WORD& GetPressedBackgroundColor() const { return mPressedBackgroundColor; }
	void SetPressedBackgroundColor(WORD c) { mPressedBackgroundColor = c; }

	Border& GetPressedBorder() { return *mPressedBorder; }
	void SetPressedBorder(Border* b) { if (mPressedBorder) delete mPressedBorder; mPressedBorder = b; }

	const bool& GetPressed() const { return mPressed; }

	void SetPressAction(std::function<void(int)> f) { mPressAction = f; }
	void SetReleaseAction(std::function<void(int)> f) { mReleaseAction = f; }
	
	const int& GetButtons() { return mMouseHandler->GetButtons(); }
	void SetButtons(int b) { mMouseHandler->SetButtons(b); }

	virtual void Draw(Window* c) override;
};

class TextField : public Label {
	friend class Window;
protected:
	WORD mEnabledTextColor = FG_WHITE;
	WORD mEnabledBackgroundColor = BG_WHITE;
	Border* mEnabledBorder = nullptr;
	bool mEnabled = false;

	bool mCapitalize = false;
	bool mDeleting = false;
	int mNumDeleted = 0;

	std::future<void> mDeleteFuture;

	std::wstring mCharset = Charset::Numeric();

	virtual void SetupHandlers() override;
	void Backspace();
public:
	TextField(RECT b) : Label(b), mEnabledBorder(new Border(FG_WHITE, 0)) { SetupHandlers(); }
	TextField(RECT b, std::wstring c) : Label(b), mCharset(c) { SetupHandlers(); }
	TextField(const TextField& e) : Label(e), mEnabledBorder(e.mEnabledBorder->Clone()), mEnabledTextColor(e.mEnabledTextColor), mEnabledBackgroundColor(e.mEnabledBackgroundColor), mEnabled(e.mEnabled), mCharset(e.mCharset) { SetupHandlers(); }
	virtual ~TextField() { if (mEnabledBorder) delete mEnabledBorder; }

	const WORD& GetEnabledTextColor() const { return mEnabledTextColor; }
	void SetEnabledTextColor(WORD c) { mEnabledTextColor = c; }

	const WORD& GetEnabledBackgroundColor() const { return mEnabledBackgroundColor; }
	void SetEnabledBackgroundColor(WORD c) { mEnabledBackgroundColor = c; }

	Border& GetEnabledBorder() { return* mEnabledBorder; }
	void SetEnabledBorder(Border* b) { if (mEnabledBorder) delete mEnabledBorder; mEnabledBorder = b; }

	const bool& GetEnabled() const { return mEnabled; }
	void SetEnabled(bool b) { mEnabled = b; }

	virtual void Draw(Window* c) override;
};

class Panel : public Element {
	friend class Window;
protected:
	int mTitleHeight = 3;
	Label mTitleLabel;
public:
	Panel(RECT b) : Element(b), mTitleLabel({ b.left, b.top, b.right, b.top + mTitleHeight - 1 }) {
		mTitleLabel.SetAlignHorizontal(TEXT_ALIGN_MID);
		mTitleLabel.SetAlignVertical(TEXT_ALIGN_MID);
	}
	Panel(RECT b, int h) : Element(b), mTitleHeight(h), mTitleLabel({ b.left, b.top, b.right, b.top + mTitleHeight - 1 }) {
		mTitleLabel.SetAlignHorizontal(TEXT_ALIGN_MID);
		mTitleLabel.SetAlignVertical(TEXT_ALIGN_MID);
	}
	Panel(const Panel& e) : Element(e), mTitleHeight(e.mTitleHeight), mTitleLabel(e.mTitleLabel) {}

	Label& GetTitleLabel() { return mTitleLabel; }

	const int& GetTitleHeight() const { return mTitleHeight; }
	void SetTitleHeight(int h) { mTitleHeight = h; SetBounds(mBounds); }

	virtual void SetBounds(RECT b) override { Element::SetBounds(b); mTitleLabel.SetBounds({ b.left, b.top, b.right, b.top + mTitleHeight - 1 }); }

	virtual void Draw(Window* c) override;
};

class ContentPanel : public Panel {
	friend class Window;
protected:
	Element* mContent = nullptr;
public:
	ContentPanel(RECT b) : Panel(b) {}
	ContentPanel(RECT b, Element* c) : Panel(b), mContent(c) {}
	ContentPanel(const ContentPanel& e) : Panel(e), mContent(e.mContent) {}
	virtual ~ContentPanel() { if (mContent) delete mContent; }

	Element& GetContent() { return *mContent; }
	void SetContent(Element* c) { mContent = c; SetBounds(mBounds); }

	virtual void SetBounds(RECT b) override { Panel::SetBounds(b); if (mContent) mContent->SetBounds({ b.left, b.top + mTitleHeight, b.right, b.bottom }); }

	virtual void Draw(Window* c) override;
};
}