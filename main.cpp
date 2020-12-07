#include "Console.h"

class Test : public gui::Console {
private:
	gui::Panel* pnl;
	gui::Label* l1, * l2, * l3;
	gui::TextField* tf1, * tf2, * tf3;
	gui::Button* btn;
	
	void Check(int _) {
		if (tf1->GetText() == L"a") {
			l1->SetTextColor(gui::FG_DARK_GREEN);
		} else {
			l1->SetTextColor(gui::FG_DARK_RED);
		}

		if (tf2->GetText() == L"b") {
			l2->SetTextColor(gui::FG_DARK_GREEN);
		} else {
			l2->SetTextColor(gui::FG_DARK_RED);
		}
		 
		if (tf3->GetText() == L"c") {
			l3->SetTextColor(gui::FG_DARK_GREEN);
		} else {
			l3->SetTextColor(gui::FG_DARK_RED);
		}

		gui::RunAfterDelay(250, [this]() {
			l1->SetTextColor(gui::FG_WHITE);
			l2->SetTextColor(gui::FG_WHITE);
			l3->SetTextColor(gui::FG_WHITE);
			tf1->SetText(L"");
			tf2->SetText(L"");
			tf3->SetText(L"");
		});
	}

public:
	bool Initialize() override {
		pnl = new gui::Panel({ 2, 2, 28, 24 });
		pnl->SetBackgroundColor(gui::BG_BLACK);
		pnl->SetBorder({ L' ', gui::BG_DARK_CYAN, 1 });
		pnl->GetTitleLabel().SetText(L"Test Program");
		pnl->GetTitleLabel().SetBackgroundColor(gui::BG_DARK_CYAN);
		pnl->GetTitleLabel().SetTextColor(gui::FG_WHITE | gui::BG_DARK_CYAN);
		pnl->SetTitleHeight(5);
		AddElement(pnl);

		l1 = new gui::Label({ 5, 9, 11, 9 });
		l1->SetText(L"Value 1");
		l1->SetBackgroundColor(gui::BG_BLACK);
		l1->SetTextColor(gui::FG_WHITE);
		AddElement(l1);

		l2 = new gui::Label(*l1);
		l2->SetText(L"Value 2");
		l2->SetBounds({ 5, 13, 11, 13 });
		AddElement(l2);

		l3 = new gui::Label(*l1);
		l3->SetText(L"Value 3");
		l3->SetBounds({ 5, 17, 11, 17 });
		AddElement(l3);

		tf1 = new gui::TextField({13, 8, 25, 10}, gui::Charset::Alphanum());
		tf1->SetBackgroundColor(gui::BG_DARK_GREY);
		tf1->SetTextColor(gui::FG_WHITE | gui::BG_DARK_GREY);
		tf1->SetBorder({ L'#', gui::FG_GREY | gui::BG_DARK_GREY, 1 });
		tf1->SetEnabledBackgroundColor(gui::BG_BLACK);
		tf1->SetEnabledTextColor(gui::FG_WHITE | gui::BG_BLACK);
		tf1->SetEnabledBorder({ L'#', gui::FG_WHITE, 1 });
		AddElement(tf1);

		tf2 = new gui::TextField(*tf1);
		tf2->SetBounds({ 13, 12, 25, 14 });
		AddElement(tf2);

		tf3 = new gui::TextField(*tf1);
		tf3->SetBounds({ 13, 16, 25, 18 });
		AddElement(tf3);

		btn = new gui::Button({ 6, 20, 24, 22 });
		btn->SetText(L"Check");
		btn->SetTextColor(gui::BG_GREY | gui::FG_BLACK);
		btn->SetBackgroundColor(gui::BG_GREY);
		btn->SetPressedTextColor(gui::BG_DARK_GREY | gui::FG_WHITE);
		btn->SetPressedBackgroundColor(gui::BG_DARK_GREY);
		btn->SetAlignHorizontal(gui::TEXT_ALIGN_MID);
		btn->SetAlignVertical(gui::TEXT_ALIGN_MID);
		btn->SetButtons(gui::MOUSE_LEFT_BUTTON);

		AddElement(btn);
		btn->SetPressAction(std::bind(&Test::Check, this, std::placeholders::_1));

		return true;
	}

	~Test() {
		delete pnl;
		delete l1; delete l2; delete l3;
		delete tf1; delete tf2; delete tf3;
		delete btn;
	}
};

int main() {
	Test t;//       31
	t.CreateConsole(31, 27, 10, 18);
	t.Run();
	return 0;
}