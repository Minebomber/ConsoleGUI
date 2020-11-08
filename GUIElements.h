#pragma once

#include "Colors.h"
#include "ConsoleGUI.h"

namespace gui {

class Console;

class EventHandler {
protected:
	int id = -1;
	std::function<void(int)> onPress;
	std::function<void(int)> onRelease;
public:
	const int& GetId() const;
	void SetId(int i);

	bool OnPressExists() const;
	void InvokeOnPress(int i);
	void SetOnPress(std::function<void(int)> f);

	bool OnReleaseExists() const;
	void InvokeOnRelease(int i);
	void SetOnRelease(std::function<void(int)> f);
};

enum MouseButtons {
	MOUSE_LEFT_BUTTON = 0b001,
	MOUSE_RIGHT_BUTTON = 0b010,
	MOUSE_CENTER_BUTTON = 0b100,
};

class MouseHandler : public EventHandler {
protected:
	RECT bounds = { 0, 0, 0, 0 };
	int buttons = 0;
public:
	const RECT& GetBounds();
	void SetBounds(RECT b);

	const int& GetButtons();
	void SetButtons(int b);
};

class Border {
protected:
	WCHAR chr = L' ';
	WORD color = BG_WHITE;
	int width = 1;
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
protected:
	int id = -1;
	RECT bounds = { 0, 0, 0, 0 };
	WCHAR background = L' ';
	WORD backgroundColor = BG_WHITE;
	Border border = { 0 };
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

	Border& GetBorder();
	void SetBorder(Border b);

	virtual void Draw(Console* g);
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
protected:
	std::wstring text = L"";
	WORD textColor = FG_WHITE;
	int hAlignment = TEXT_ALIGN_MIN;
	int vAlignment = TEXT_ALIGN_MIN;
	int textWrap = WRAP_CHAR;

	int textLines = 1;
	virtual void UpdateTextLines();

	int textOffsetY = 0;
	virtual void UpdateTextOffsetY();

	void RenderText(Console* g, int minX, int maxX, int minY, int maxY, WORD c);
public:
	Label(RECT b);
	Label(const Label& e);

	const std::wstring& GetText() const;
	void SetText(std::wstring t);

	const WORD& GetTextColor() const;
	void SetTextColor(WORD c);

	const int& GetHorizontalAlignment() const;
	void SetHorizontalAlignment(int h);

	const int& GetVerticalAlignment() const;
	void SetVerticalAlignment(int v);

	const int& GetTextWrap() const;
	void SetTextWrap(int w);

	virtual void Draw(Console* g) override;
};

class Button : public Label {
protected:
	WORD pressedTextColor = FG_WHITE;
	WCHAR pressedBackground = L' ';
	WORD pressedBackgroundColor = BG_WHITE;
	Border pressedBorder = { 0 };
	bool pressed = false;

	MouseHandler handler;

	std::function<void(int)> OnPress;
	std::function<void(int)> OnRelease;

	void SetupHandler();
public:
	Button(RECT b);

	void SetBounds(RECT b) override;

	const WORD& GetPressedTextColor() const;
	void SetPressedTextColor(WORD c);

	const WCHAR& GetPressedBackground() const;
	void SetPressedBackground(WCHAR b);

	const WORD& GetPressedBackgroundColor() const;
	void SetPressedBackgroundColor(WORD c);

	Border& GetPressedBorder();
	void SetPressedBorder(Border b);

	MouseHandler& GetHandler();

	virtual void Draw(Console* g) override;
};

class Panel : public Element {
protected:
	int titleHeight = 3;
	Label titleLabel;
public:
	Panel(RECT b);

	Label& GetTitleLabel();

	const int& GetTitleHeight() const;
	void SetTitleHeight(int h);

	virtual void SetBounds(RECT b) override;

	virtual void Draw(Console* g) override;
};

class ContentPanel : public Panel {
protected:
	Element* content = nullptr;
public:
	ContentPanel(RECT b);
	ContentPanel(RECT b, Element* c);

	Element* GetContent();
	void SetContent(Element* c);

	virtual void SetBounds(RECT b) override;

	virtual void Draw(Console* g) override;
};
}