#include "Console.h"

class ValueElement : public gui::Element {
private:
	gui::Label mLabel1, mLabel2, mLabel3;

public:
	ValueElement(RECT b) : 
		gui::Element(b),
		mLabel1({ b.left + 2, b.top + 2, b.right - 2, b.top + 2 }),
		mLabel2({ b.left + 2, b.top + 4, b.right - 2, b.top + 4 }),
		mLabel3({ b.left + 2, b.top + 6, b.right - 2, b.top + 6 })
	{
		mBorder = { L'#', gui::FG_GREY | gui::BG_DARK_GREY, 1 };
		mBackgroundColor = gui::BG_DARK_GREY;
		mLabel1.SetTextColor(gui::FG_WHITE);
		mLabel1.SetBackgroundColor(gui::BG_BLACK);
		mLabel2.SetTextColor(gui::FG_WHITE);
		mLabel2.SetBackgroundColor(gui::BG_BLACK);
		mLabel3.SetTextColor(gui::FG_WHITE);
		mLabel3.SetBackgroundColor(gui::BG_BLACK);
	}

	gui::Label& GetLabel1() { return mLabel1; }
	gui::Label& GetLabel2() { return mLabel2; }
	gui::Label& GetLabel3() { return mLabel3; }

	void Draw(gui::Console* c) override {
		Element::Draw(c);
		mLabel1.Draw(c);
		mLabel2.Draw(c);
		mLabel3.Draw(c);
	}
};

class Test : public gui::Console {
private:
	gui::Panel* pnl1 = nullptr, * pnl2 = nullptr;
	gui::Label* l1 = nullptr, * l2 = nullptr, * l3 = nullptr;
	gui::TextField* tf1 = nullptr, * tf2 = nullptr, * tf3 = nullptr;
	gui::Button* btn = nullptr;
	
	const int vWidth = 15;
	const int vHeight = 9;
	const int vSpacing = 2;

	const int nRows = 3;
	const int nCols = 3;

	const int x0 = 32;
	const int y0 = 4;

	int dy = 0;
	int dx = 0;

	std::future<void> disableFuture;

	void Add(int _) {
		if (dx >= nCols) return;
		int x = x0 + vSpacing + dx * (vSpacing + vWidth);
		int y = y0 + vSpacing + dy * (vSpacing + vHeight);
		ValueElement* e = new ValueElement({x, y, x + vWidth - 1, y + vHeight - 1});
		e->GetLabel1().SetText(tf1->GetText());
		e->GetLabel2().SetText(tf2->GetText());
		e->GetLabel3().SetText(tf3->GetText());
		tf1->SetText(L"");
		tf2->SetText(L"");
		tf3->SetText(L"");
		AddElement(e);
		dy++;
		if (dy >= nRows) {
			dy = 0;
			dx++;
		}

		if (dx >= nCols) {
			disableFuture = gui::ExecuteAsync([this]() {
				while (btn->GetPressed()) {}
				btn->SetTextColor(gui::FG_RED | gui::BG_GREY);
				btn->SetPressedTextColor(gui::FG_RED | gui::BG_GREY);
				btn->SetPressedBackgroundColor(gui::BG_GREY);
			});	
		}
	}

public:
	bool Initialize() override {
		pnl1 = new gui::Panel({ 2, 2, 28, 22 });
		pnl1->SetBackgroundColor(gui::BG_BLACK);
		pnl1->SetBorder({ L' ', gui::BG_DARK_CYAN, 1 });
		pnl1->GetTitleLabel().SetText(L"Add");
		pnl1->GetTitleLabel().SetBackgroundColor(gui::BG_DARK_CYAN);
		pnl1->GetTitleLabel().SetTextColor(gui::FG_WHITE | gui::BG_DARK_CYAN);
		pnl1->SetTitleHeight(3);
		AddElement(pnl1);

		pnl2 = new gui::Panel(*pnl1);
		pnl2->SetBounds({ 31, 2, 85, 38 });
		pnl2->GetTitleLabel().SetText(L"Results");
		AddElement(pnl2);

		l1 = new gui::Label({ 5, 7, 11, 7 });
		l1->SetText(L"Value 1");
		l1->SetBackgroundColor(gui::BG_BLACK);
		l1->SetTextColor(gui::FG_WHITE);
		AddElement(l1);

		l2 = new gui::Label(*l1);
		l2->SetText(L"Value 2");
		l2->SetBounds({ 5, 11, 11, 11 });
		AddElement(l2);

		l3 = new gui::Label(*l1);
		l3->SetText(L"Value 3");
		l3->SetBounds({ 5, 15, 11, 15 });
		AddElement(l3);

		tf1 = new gui::TextField({13, 6, 25, 8}, gui::Charset::Alphanum());
		tf1->SetBackgroundColor(gui::BG_DARK_GREY);
		tf1->SetTextColor(gui::FG_WHITE | gui::BG_DARK_GREY);
		tf1->SetBorder({ L'#', gui::FG_GREY | gui::BG_DARK_GREY, 1 });
		tf1->SetEnabledBackgroundColor(gui::BG_BLACK);
		tf1->SetEnabledTextColor(gui::FG_WHITE | gui::BG_BLACK);
		tf1->SetEnabledBorder({ L'#', gui::FG_WHITE, 1 });
		AddElement(tf1);

		tf2 = new gui::TextField(*tf1);
		tf2->SetBounds({ 13, 10, 25, 12 });
		AddElement(tf2);

		tf3 = new gui::TextField(*tf1);
		tf3->SetBounds({ 13, 14, 25, 16 });
		AddElement(tf3);

		btn = new gui::Button({ 6, 18, 24, 20 });
		btn->SetText(L"Submit");
		btn->SetTextColor(gui::BG_GREY | gui::FG_BLACK);
		btn->SetBackgroundColor(gui::BG_GREY);
		btn->SetPressedTextColor(gui::BG_DARK_GREY | gui::FG_WHITE);
		btn->SetPressedBackgroundColor(gui::BG_DARK_GREY);
		btn->SetAlignHorizontal(gui::TEXT_ALIGN_MID);
		btn->SetAlignVertical(gui::TEXT_ALIGN_MID);
		btn->SetButtons(gui::MOUSE_LEFT_BUTTON);

		AddElement(btn);
		btn->SetPressAction(std::bind(&Test::Add, this, std::placeholders::_1));

		return true;
	}

	~Test() {
		delete pnl1; delete pnl2;
		delete l1; delete l2; delete l3;
		delete tf1; delete tf2; delete tf3;
		delete btn;
	}
};

int main() {
	Test t;//       31
	t.CreateConsole(88, 41, 10, 18);
	t.Run();
	return 0;
}