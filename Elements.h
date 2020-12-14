#pragma once
#include "Console.h"
#include "Window.h"
#include "Events.h"
#include "Rect.h"

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
	MOUSE_BUTTON_LEFT = 0x1,
	MOUSE_BUTTON_RIGHT = 0x2,
	MOUSE_BUTTON_CENTER = 0x4,
};

enum TextAlignment {
	TEXT_ALIGN_MIN,
	TEXT_ALIGN_MID,
	TEXT_ALIGN_MAX,
};

enum TextWrap {
	TEXT_WRAP_CHAR,
	TEXT_WRAP_WORD,
};

class Charset {
public:
	static std::wstring Numeric() { return L"0123456789"; }
	static std::wstring Alphabet() { return L" ABCDEFGHIJKLMNOPQRSTUVWXYZ"; }
	static std::wstring Alphanum() { return L" ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789"; }
	static std::wstring All() { 
		return 
			L" ABCDEFGHIJKLMNOPQRSTUVWXYZ"
			"0123456789"
			"\xBA\xBB\xBC\xBD\xBE\xBF\xC0\xDB\xDC\xDD\xDE";
	}
};

class Window;

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

	virtual void Draw(Window* w, Rect b);
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
	
	virtual void Draw(Window* w, Rect b) override;
};

class Element {
	friend class Window;
	friend class Console;
protected:
	Rect mBounds = { 0, 0, 0, 0 };
	WORD mBackgroundColor = BG_WHITE;
	Border* mBorder = nullptr;

	std::vector<EventHandler*> mEventHandlers;
public:
	Element(Rect b) : mBounds(b), mBorder(new Border(FG_WHITE, 0)) {}
	Element(const Element& e) : mBounds(e.mBounds), mBackgroundColor(e.mBackgroundColor), mBorder(e.mBorder->Clone()) {}
	virtual ~Element() { if (mBorder) delete mBorder; }

	const Rect& GetBounds() const { return mBounds; }
	virtual void SetBounds(Rect b) { mBounds = b; }

	const WORD& GetBackgroundColor() const { return mBackgroundColor; }
	void SetBackgroundColor(WORD c) { mBackgroundColor = c; }

	Border* GetBorder() { return mBorder; }
	virtual void SetBorder(Border* b) { if (mBorder) delete mBorder; mBorder = b; }

	void AddEventHandler(EventHandler* e) { mEventHandlers.push_back(e); }
	void RemoveEventHandler(EventHandler* e) { std::remove(mEventHandlers.begin(), mEventHandlers.end(), e); }

	virtual void Draw(Window* w);
};

class Label : public Element {
	friend class Window;
protected:
	std::wstring mText = L"";
	WORD mTextColor = FG_WHITE;
	int mAlignH = TEXT_ALIGN_MID;
	int mAlignV = TEXT_ALIGN_MID;
	int mTextWrap = TEXT_WRAP_WORD;
	
	void RenderText(Window* w, Rect r, const std::wstring& s, WORD cl);
public:
	Label(Rect b) : Element(b) {}
	Label(const Label& e) : Element(e), mText(e.mText), mTextColor(e.mTextColor), mAlignH(e.mAlignH), mAlignV(e.mAlignV), mTextWrap(e.mTextWrap) {}

	const std::wstring& GetText() const { return mText; }
	void SetText(std::wstring t) { mText = t; }

	const WORD& GetTextColor() const { return mTextColor; }
	void SetTextColor(WORD c) { mTextColor = c; }

	const int& GetAlignHorizontal() const { return mAlignH; }
	void SetAlignHorizontal(int h) { mAlignH = h; }

	const int& GetAlignVertical() const { return mAlignV; }
	void SetAlignVertical(int v) { mAlignV = v; }

	const int& GetTextWrap() const { return mTextWrap; }
	void SetTextWrap(int w) { mTextWrap = w; }

	virtual void Draw(Window* w) override;
};

class Button : public Label {
	friend class Window;
protected:
	WORD mPressedTextColor = FG_WHITE;
	WORD mPressedBackgroundColor = BG_WHITE;
	Border* mPressedBorder = nullptr;
	bool mPressed = false;

	int mButtons = MOUSE_BUTTON_LEFT;

	void Init();
public:
	Button(Rect b) : Label(b), mPressedBorder(new Border(FG_WHITE, 0)) { Init(); }
	Button(const Button& e) : Label(e), mPressedBorder(e.mPressedBorder->Clone()), mPressedTextColor(e.mPressedTextColor), mPressedBackgroundColor(e.mPressedBackgroundColor), mPressed(e.mPressed) { Init();  }
	virtual ~Button() { if (mPressedBorder) delete mPressedBorder; }

	const WORD& GetPressedTextColor() const { return mPressedTextColor; }
	void SetPressedTextColor(WORD c) { mPressedTextColor = c; }

	const WORD& GetPressedBackgroundColor() const { return mPressedBackgroundColor; }
	void SetPressedBackgroundColor(WORD c) { mPressedBackgroundColor = c; }

	Border* GetPressedBorder() { return mPressedBorder; }
	void SetPressedBorder(Border* b) { if (mPressedBorder) delete mPressedBorder; mPressedBorder = b; }

	const bool& GetPressed() const { return mPressed; }

	const int& GetButtons() const { return mButtons; }
	void SetButtons(int b) { mButtons = b; }

	virtual void Draw(Window* w) override;
};

class TextField : public Label {
	friend class Window;
protected:
	WORD mDisabledTextColor = FG_WHITE;
	WORD mDisabledBackgroundColor = BG_WHITE;
	Border* mDisabledBorder = nullptr;
	bool mDisabled = true;
	
	bool mCapitalize = false;

	std::wstring mCharset = Charset::All();

	void Init();
public:
	TextField(Rect b) : Label(b), mDisabledBorder(new Border(FG_WHITE, 0)) { mAlignH = TEXT_ALIGN_MIN; mAlignV = TEXT_ALIGN_MIN; Init(); }
	TextField(Rect b, std::wstring c) : Label(b), mCharset(c), mDisabledBorder(new Border(FG_WHITE, 0)) { mAlignH = TEXT_ALIGN_MIN; mAlignV = TEXT_ALIGN_MIN; Init(); }
	TextField(const TextField& e) : Label(e), mDisabledBorder(e.mDisabledBorder->Clone()), mDisabledTextColor(e.mDisabledTextColor), mDisabledBackgroundColor(e.mDisabledBackgroundColor), mDisabled(e.mDisabled), mCharset(e.mCharset) { Init(); }
	virtual ~TextField() { if (mDisabledBorder) delete mDisabledBorder; }

	const WORD& GetDisabledTextColor() const { return mDisabledTextColor; }
	void SetDisabledTextColor(WORD c) { mDisabledTextColor = c; }

	const WORD& GetDisabledBackgroundColor() const { return mDisabledBackgroundColor; }
	void SetDisabledBackgroundColor(WORD c) { mDisabledBackgroundColor = c; }

	Border* GetDisabledBorder() { return mDisabledBorder; }
	void SetDisabledBorder(Border* b) { if (mDisabledBorder) delete mDisabledBorder; mDisabledBorder = b; }

	const bool& GetDisabled() const { return mDisabled; }
	void SetDisabled(bool b) { mDisabled = b; }

	virtual void Draw(Window* w) override;
};

class Checkbox : public Label {
protected:
	bool mChecked = false;

	void Init();
public:
	Checkbox(Rect b) : Label(b) { mAlignH = TEXT_ALIGN_MIN; mAlignV = TEXT_ALIGN_MIN; Init(); }
	Checkbox(const Checkbox& e) : Label(e), mChecked(e.mChecked) { mAlignH = TEXT_ALIGN_MIN; mAlignV = TEXT_ALIGN_MIN; Init(); }

	const bool& GetChecked() const { return mChecked; }
	void SetChecked(bool c) { mChecked = c; }

	virtual void Draw(Window* w) override;
};

}