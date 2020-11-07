#include "ConsoleGUI.h"

class Test : public ConsoleGUI {
public:
	bool Initialize() override {

		GUIButton* e = new GUIButton({ 0, 0, 10, 10 });
		
		e->background = L' ';
		e->backgroundColor = BG_DARK_RED;

		e->pressedBackgroundColor = BG_BLACK;
		e->pressedTextColor = FG_RED;

		e->text = L"Test";
		e->textColor = FG_BLACK | BG_DARK_RED;

		e->border.color = BG_RED;
		e->border.width = 1;

		e->hAlignment = TEXT_ALIGN_MID;
		e->vAlignment = TEXT_ALIGN_MID;

		e->handler.buttons = MOUSE_RIGHT_BUTTON;

		AddElement(e);

		return true;
	}
};

int main() {
	Test t;
	t.CreateConsole(50, 50, 16, 16);
	t.Run();
	return 0;
}