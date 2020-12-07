#include "Console.h"

class Test : public gui::Console {
private:
	gui::Button* btn1, * btn2;
	gui::Window* w1, * w2;
	gui::Label* l1, * l2;
public:
	bool Initialize() override {
		w1 = new gui::Window(GetScreenWidth(), GetScreenHeight());
		SetCurrentWindow(w1);
		w2 = new gui::Window(GetScreenWidth(), GetScreenHeight());

		btn1 = new gui::Button({ 5, 13, 25, 17 });
		btn1->SetBackgroundColor(gui::BG_BLUE);
		btn1->GetBorder()->SetColor(gui::FG_BLUE);
		btn1->GetBorder()->SetWidth(1);
		btn1->SetTextColor(gui::FG_WHITE | gui::BG_BLUE);
		btn1->SetText(L"ToWindow2");
		btn1->SetPressedTextColor(gui::FG_BLACK | gui::BG_DARK_BLUE);
		btn1->SetPressedBackgroundColor(gui::BG_DARK_BLUE);
		btn1->GetPressedBorder()->SetColor(gui::FG_DARK_BLUE);
		btn1->GetPressedBorder()->SetWidth(1);
		btn1->SetAlignHorizontal(gui::TEXT_ALIGN_MID);
		btn1->SetAlignVertical(gui::TEXT_ALIGN_MID);
		w1->AddElement(btn1);

		btn2 = new gui::Button({ 5, 13, 25, 17 });
		btn2->SetBackgroundColor(gui::BG_RED);
		btn2->GetBorder()->SetColor(gui::FG_RED);
		btn2->GetBorder()->SetWidth(1);
		btn2->SetTextColor(gui::FG_WHITE | gui::BG_RED);
		btn2->SetText(L"ToWindow1");
		btn2->SetPressedTextColor(gui::FG_BLACK | gui::BG_DARK_RED);
		btn2->SetPressedBackgroundColor(gui::BG_DARK_RED);
		btn2->GetPressedBorder()->SetColor(gui::FG_DARK_RED);
		btn2->GetPressedBorder()->SetWidth(1);
		btn2->SetAlignHorizontal(gui::TEXT_ALIGN_MID);
		btn2->SetAlignVertical(gui::TEXT_ALIGN_MID);
		w2->AddElement(btn2);

		btn1->SetReleaseAction([this](int _) {
			SetCurrentWindow(w2);
		});

		btn2->SetReleaseAction([this](int _) {
			SetCurrentWindow(w1);
		});

		l1 = new gui::Label({ 0, 0, 10, 2 });
		l1->SetBorder(new gui::TitledBorder(L"# 1", gui::FG_WHITE, 1));
		l1->SetBackgroundColor(gui::BG_BLACK);
		l1->SetTextColor(gui::FG_WHITE);
		l1->SetAlignHorizontal(gui::TEXT_ALIGN_MID);
		l1->SetText(L"Window1");
		w1->AddElement(l1);

		l2 = new gui::Label(*l1);
		l2->SetBounds({ 12, 0, 22, 2 });
		((gui::TitledBorder*)l2->GetBorder())->SetTitle(L"# 2");
		l2->SetText(L"Window2");
		w2->AddElement(l2);

		return true;
	}

	~Test() {
		delete btn1; delete btn2;
		delete l1; delete l2;
		delete w1; delete w2;
		SetCurrentWindow(nullptr);
	}
};

int main() {
	Test t;//       31
	t.CreateConsole(30, 30, 8, 16);
	t.Run();
	return 0;
}