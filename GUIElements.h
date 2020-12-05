#pragma once

#include "Colors.h"
#include "ConsoleGUI.h"

namespace gui {

class Console;

class EventHandler {
	friend class Console;
protected:
	int mId = -1;
	std::function<void(Console*, int)> mPressAction;
	std::function<void(Console*, int)> mReleaseAction;
public:
	EventHandler();

	const int& GetId() const;
	void SetId(int i);

	bool PressActionExists() const;
	void InvokePressAction(Console *c, int i);
	void SetPressAction(std::function<void(Console*, int)> f);

	bool ReleaseActionExists() const;
	void InvokeReleaseAction(Console* c, int i);
	void SetReleaseAction(std::function<void(Console*, int)> f);
};

enum MouseButtons {
	MOUSE_LEFT_BUTTON = 0b001,
	MOUSE_RIGHT_BUTTON = 0b010,
	MOUSE_CENTER_BUTTON = 0b100,
};

class MouseHandler : public EventHandler {
	friend class Console;
protected:
	RECT mBounds = { 0, 0, 0, 0 };
	int mButtons = 0;
public:
	MouseHandler();
	MouseHandler(RECT b);
	MouseHandler(RECT b, int bt);

	const RECT& GetBounds();
	void SetBounds(RECT b);

	const int& GetButtons();
	void SetButtons(int b);
};

class KeyboardHandler : public EventHandler {
	friend class Console;
protected:
	std::wstring mKeys = L"";
public:
	KeyboardHandler();
	KeyboardHandler(std::wstring k);

	const std::wstring& GetKeys() const;
	void SetKeys(std::wstring k);
};

class Border {
	friend class Console;
protected:
	WCHAR mChar = L' ';
	WORD mColor = BG_WHITE;
	int mWidth = 1;
public:
	Border();
	Border(WCHAR ch, WORD cl);
	Border(WCHAR ch, WORD cl, int w);
	Border(int w);

	const WCHAR& GetChar() const;
	void SetChar(WCHAR c);

	const WORD& GetColor() const;
	void SetColor(WORD c);

	const int& GetWidth() const;
	void SetWidth(int w);

	operator bool() const;

	virtual void Draw(Console* g, RECT bd);
};

class Element {
	friend class Console;
protected:
	int mId = -1;
	RECT mBounds = { 0, 0, 0, 0 };
	WCHAR mBackground = L' ';
	WORD mBackgroundColor = BG_WHITE;
	Border mBorder = { 0 };
	MouseHandler* mMouseHandler = nullptr;
	KeyboardHandler* mKeyboardHandler = nullptr;

	virtual void SetupHandlers(); 
public:
	Element(RECT b);
	Element(const Element& e);
	virtual ~Element();

	const int& GetId() const;
	void SetId(int i);

	const RECT& GetBounds() const;
	virtual void SetBounds(RECT b);

	const WCHAR& GetBackground() const;
	void SetBackground(WCHAR b);

	const WORD& GetBackgroundColor() const;
	void SetBackgroundColor(WORD c);

	const Border& GetBorder() const;
	virtual void SetBorder(Border b);

	virtual void Draw(Console* c);
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

class Label : public Element {
	friend class Console;
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

	void RenderText(Console* c, int minX, int maxX, int minY, int maxY, std::wstring s, WORD cl);
public:
	Label(RECT b);
	Label(const Label& e);

	const std::wstring& GetText() const;
	void SetText(std::wstring t);

	const WORD& GetTextColor() const;
	void SetTextColor(WORD c);

	const int& GetAlignHorizontal() const;
	void SetAlignHorizontal(int h);

	const int& GetAlignVertical() const;
	void SetAlignVertical(int v);

	const int& GetTextWrap() const;
	void SetTextWrap(int w);

	void SetBorder(Border b) override;

	virtual void Draw(Console* c) override;
};

class Button : public Label {
	friend class Console;
protected:
	WORD mPressedTextColor = FG_WHITE;
	WCHAR mPressedBackground = L' ';
	WORD mPressedBackgroundColor = BG_WHITE;
	Border mPressedBorder = { 0 };
	bool mPressed = false;

	std::function<void(int)> mPressAction;
	std::function<void(int)> mReleaseAction;

	virtual void SetupHandlers() override;
public:
	Button(RECT b);

	const WORD& GetPressedTextColor() const;
	void SetPressedTextColor(WORD c);

	const WCHAR& GetPressedBackground() const;
	void SetPressedBackground(WCHAR b);

	const WORD& GetPressedBackgroundColor() const;
	void SetPressedBackgroundColor(WORD c);

	const Border& GetPressedBorder() const;
	void SetPressedBorder(Border b);

	void SetPressAction(std::function<void(int)> f);
	void SetReleaseAction(std::function<void(int)> f);
	
	const int& GetButtons();
	void SetButtons(int b);

	virtual void Draw(Console* c) override;
};

enum class CharsetT {
	NUMERIC = 1,
	ALPHABET = 2,
	ALPHANUM = 3,
};

class Charset {
public:
	static std::wstring Get(CharsetT t);
};

class TextField : public Label {
	friend class Console;
protected:
	virtual void SetupHandlers() override;

	bool mCapitalize = false;
	bool mDeleting = false;
	int mNumDeleted = 0;

	std::future<void> mDeleteFuture;

	void Backspace();

	CharsetT mCharset = CharsetT::ALPHANUM;
public:
	TextField(RECT b);
	TextField(RECT b, CharsetT c);

	virtual void Draw(Console* c) override;
};

class Panel : public Element {
	friend class Console;
protected:
	int mTitleHeight = 3;
	Label mTitleLabel;
public:
	Panel(RECT b);

	Label& GetTitleLabel();

	const int& GetTitleHeight() const;
	void SetTitleHeight(int h);

	virtual void SetBounds(RECT b) override;

	virtual void Draw(Console* c) override;
};

class ContentPanel : public Panel {
	friend class Console;
protected:
	Element* mContent = nullptr;
public:
	ContentPanel(RECT b);
	ContentPanel(RECT b, Element* c);

	Element* GetContent();
	void SetContent(Element* c);

	virtual void SetBounds(RECT b) override;

	virtual void Draw(Console* c) override;
};
}