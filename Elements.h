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

enum ElementState {
	ELEMENT_DEFAULT,
	ELEMENT_FOCUSED,
	ELEMENT_DISABLED,
};

class Element {
	friend class Window;
	friend class Console;
protected:
	Rect mBounds = { 0, 0, 0, 0 };

	int mState = ELEMENT_DEFAULT;

	WORD mDefaultForegroundColor = FG_WHITE;
	WORD mDefaultBackgroundColor = BG_BLACK;

	WORD mFocusedForegroundColor = FG_WHITE;
	WORD mFocusedBackgroundColor = FG_WHITE;

	WORD mDisabledForegroundColor = FG_WHITE;
	WORD mDisabledBackgroundColor = FG_WHITE;

	bool mDisplayBorders = true;

	std::vector<EventHandler*> mEventHandlers;
public:
	Element(Rect b) : mBounds(b) {}
	Element(const Element& e) : mBounds(e.mBounds) {}
	virtual ~Element() {}

	const Rect& GetBounds() const { return mBounds; }
	virtual void SetBounds(Rect b) { mBounds = b; }

	const WORD& GetDefaultForegroundColor() const { return mDefaultForegroundColor; }
	void SetDefaultForegroundColor(WORD c) { mDefaultForegroundColor = c; }
	const WORD& GetDefaultBackgroundColor() const { return mDefaultBackgroundColor; }
	void SetDefaultBackgroundColor(WORD c) { mDefaultBackgroundColor = c; }

	const WORD& GetFocusedForegroundColor() const { return mFocusedForegroundColor; }
	void SetFocusedForegroundColor(WORD c) { mFocusedForegroundColor = c; }
	const WORD& GetFocusedBackgroundColor() const { return mFocusedBackgroundColor; }
	void SetFocusedBackgroundColor(WORD c) { mFocusedBackgroundColor = c; }

	const WORD& GetDisabledForegroundColor() const { return mDisabledBackgroundColor; }
	void SetDisabledForegroundColor(WORD c) { mDisabledBackgroundColor = c; }
	const WORD& GetDisabledBackgroundColor() const { return mDisabledBackgroundColor; }
	void SetDisabledBackgroundColor(WORD c) { mDisabledBackgroundColor = c; }

	void AddEventHandler(EventHandler* e) { mEventHandlers.push_back(e); }
	void RemoveEventHandler(EventHandler* e) { mEventHandlers.erase(std::remove(mEventHandlers.begin(), mEventHandlers.end(), e), mEventHandlers.end()); }

	WORD GetCurrentForegroundColor() const;
	WORD GetCurrentBackgroundColor() const;
	Rect GetInnerBounds() const;

	virtual void Draw(Window* w);
};

class Label : public Element {
	friend class Window;
protected:
	std::wstring mText = L"";

	int mAlignH = TEXT_ALIGN_MID;
	int mAlignV = TEXT_ALIGN_MID;
	int mTextWrap = TEXT_WRAP_WORD;
	
	void RenderText(Window* w, Rect r, const std::wstring& s, WORD cl);
public:
	Label(Rect b) : Element(b) {}
	Label(const Label& e) : Element(e), mText(e.mText), mAlignH(e.mAlignH), mAlignV(e.mAlignV), mTextWrap(e.mTextWrap) {}

	const std::wstring& GetText() const { return mText; }
	void SetText(std::wstring t) { mText = t; }

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
	int mButtons = MOUSE_BUTTON_LEFT;

	void Init();
public:
	Button(Rect b) : Label(b) { Init(); }
	Button(const Button& e) : Label(e) { Init();  }

	const int& GetButtons() const { return mButtons; }
	void SetButtons(int b) { mButtons = b; }

	virtual void Draw(Window* w) override;
};

class TextField : public Label {
	friend class Window;
protected:
	bool mCapitalize = false;

	std::wstring mCharset = Charset::All();

	void Init();
public:
	TextField(Rect b) : Label(b) { mAlignH = TEXT_ALIGN_MIN; mAlignV = TEXT_ALIGN_MIN; Init(); }
	TextField(Rect b, std::wstring c) : Label(b), mCharset(c) { mAlignH = TEXT_ALIGN_MIN; mAlignV = TEXT_ALIGN_MIN; Init(); }
	TextField(const TextField& e) : Label(e), mCharset(e.mCharset) { Init(); }

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