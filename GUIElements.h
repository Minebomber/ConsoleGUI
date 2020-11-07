#pragma once

#include "Colors.h"

class ConsoleGUI;
#include "ConsoleGUI.h"

class EventHandler {
public:
	int id = -1;
	std::function<void(int)> OnPress;
	std::function<void(int)> OnRelease;
};

enum MouseButtons {
	MOUSE_LEFT_BUTTON = 0b001,
	MOUSE_RIGHT_BUTTON = 0b010,
	MOUSE_CENTER_BUTTON = 0b100,
};

class MouseHandler : public EventHandler {
public:
	RECT bounds = { 0, 0, 0, 0 };
	int buttons = 0;
};

class GUIBorder {
public:
	WCHAR chr = L' ';
	WORD color = BG_WHITE;
	int width = 1;

	GUIBorder();
	GUIBorder(WCHAR ch, WORD cl);
	GUIBorder(WCHAR ch, WORD cl, int w);
	GUIBorder(int w);

	operator bool() const;

	virtual void Draw(ConsoleGUI* g, RECT bd);
};

class GUIElement {
public:
	int id = -1;
	RECT bounds = { 0, 0, 0, 0 };
	WCHAR background = L' ';
	WORD backgroundColor = BG_WHITE;
	GUIBorder border = { 0 };

	GUIElement(RECT b);
	GUIElement(const GUIElement& e);
	virtual ~GUIElement();
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
	void RenderText(ConsoleGUI* g, int minX, int maxX, int minY, int maxY, WORD c);
public:
	std::wstring text = L"";
	WORD textColor = FG_WHITE;
	int hAlignment = TEXT_ALIGN_MIN;
	int vAlignment = TEXT_ALIGN_MIN;

	GUILabel(RECT b);
	GUILabel(const GUILabel& e);

	virtual void Draw(ConsoleGUI* g) override;
};

class GUIButton : public GUILabel {
private:
	void SetupHandler();
public:
	WORD pressedTextColor = FG_WHITE;
	WCHAR pressedBackground = L' ';
	WORD pressedBackgroundColor = BG_WHITE;
	GUIBorder pressedBorder = { 0 };
	bool pressed = false;

	MouseHandler handler;

	std::function<void(int)> OnPress;
	std::function<void(int)> OnRelease;

	GUIButton(RECT b);

	virtual void Draw(ConsoleGUI* g) override;
};