#pragma once

#include "Colors.h"

class ConsoleGUI;
#include "ConsoleGUI.h"

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

class GUIBorder {
protected:
	WCHAR chr = L' ';
	WORD color = BG_WHITE;
	int width = 1;
public:
	GUIBorder();
	GUIBorder(WCHAR ch, WORD cl);
	GUIBorder(WCHAR ch, WORD cl, int w);
	GUIBorder(int w);

	const WCHAR& GetChar() const;
	void SetChar(WCHAR c);

	const WORD& GetColor() const;
	void SetColor(WORD c);

	const int& GetWidth() const;
	void SetWidth(int w);

	operator bool() const;

	virtual void Draw(ConsoleGUI* g, RECT bd);
};

class GUIElement {
protected:
	int id = -1;
	RECT bounds = { 0, 0, 0, 0 };
	WCHAR background = L' ';
	WORD backgroundColor = BG_WHITE;
	GUIBorder border = { 0 };
public:
	GUIElement(RECT b);
	GUIElement(const GUIElement& e);
	virtual ~GUIElement();
	
	const int& GetId() const;
	void SetId(int i);
	
	const RECT& GetBounds() const;
	virtual void SetBounds(RECT b);
	
	const WCHAR& GetBackground() const;
	void SetBackground(WCHAR b);

	const WORD& GetBackgroundColor() const;
	void SetBackgroundColor(WORD c);

	GUIBorder& GetBorder();
	void SetBorder(GUIBorder b);

	virtual void Draw(ConsoleGUI* g);
};

enum TextAlignment {
	TEXT_ALIGN_MIN,
	TEXT_ALIGN_MID,
	TEXT_ALIGN_MAX,
};

enum TextWrap {
	WRAP_CHAR,
	WRAP_NEWLINE,
	WRAP_WORD,
};

class GUILabel : public GUIElement {
protected:
	std::wstring text = L"";
	WORD textColor = FG_WHITE;
	int hAlignment = TEXT_ALIGN_MIN;
	int vAlignment = TEXT_ALIGN_MIN;

	void RenderText(ConsoleGUI* g, int minX, int maxX, int minY, int maxY, WORD c);
public:
	GUILabel(RECT b);
	GUILabel(const GUILabel& e);

	const std::wstring& GetText() const;
	void SetText(std::wstring t);

	const WORD& GetTextColor() const;
	void SetTextColor(WORD c);

	const int& GetHorizontalAlignment() const;
	void SetHorizontalAlignment(int h);

	const int& GetVerticalAlignment() const;
	void SetVerticalAlignment(int v);

	virtual void Draw(ConsoleGUI* g) override;
};

class GUIButton : public GUILabel {
protected:
	WORD pressedTextColor = FG_WHITE;
	WCHAR pressedBackground = L' ';
	WORD pressedBackgroundColor = BG_WHITE;
	GUIBorder pressedBorder = { 0 };
	bool pressed = false;

	MouseHandler handler;

	std::function<void(int)> OnPress;
	std::function<void(int)> OnRelease;

	void SetupHandler();
public:
	GUIButton(RECT b);

	void SetBounds(RECT b) override;

	const WORD& GetPressedTextColor() const;
	void SetPressedTextColor(WORD c);

	const WCHAR& GetPressedBackground() const;
	void SetPressedBackground(WCHAR b);

	const WORD& GetPressedBackgroundColor() const;
	void SetPressedBackgroundColor(WORD c);

	GUIBorder& GetPressedBorder();
	void SetPressedBorder(GUIBorder b);

	MouseHandler& GetHandler();

	virtual void Draw(ConsoleGUI* g) override;
};