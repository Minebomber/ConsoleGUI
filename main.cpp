#include "ConsoleGUI.h"

class Test : public ConsoleGUI {
public:
	bool Initialize() override {

		GUILabel* label = new GUILabel({ 10, 10, 40, 20 });
		label->SetText(L"Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua.");
		label->SetTextColor(FG_BLACK | BG_WHITE);

		label->SetTextWrap(WRAP_WORD);

		label->GetBorder().SetWidth(1);

		MouseHandler* clickHandler = new MouseHandler();
		clickHandler->SetButtons(MOUSE_LEFT_BUTTON | MOUSE_RIGHT_BUTTON);
		clickHandler->SetBounds({ 0, 0, GetScreenWidth(), GetScreenHeight() });

		clickHandler->SetOnPress([label](int m) {
			if ((1 << m) == MOUSE_LEFT_BUTTON) {
				// LMB
				int a = label->GetHorizontalAlignment();
				a++;
				a %= 3;
				label->SetHorizontalAlignment(a);
			} else {
				// RMB
				int a = label->GetVerticalAlignment();
				a++;
				a %= 3;
				label->SetVerticalAlignment(a);
			}
		});

		AddElement(label);
		AddMouseHandler(clickHandler);

		return true;
	}
};

int main() {
	Test t;
	t.CreateConsole(50, 50, 12, 16);
	t.Run();
	return 0;
}