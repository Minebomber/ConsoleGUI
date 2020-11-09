#include "ConsoleGUI.h"

class CustomView : public gui::Element {
public:
	gui::MouseHandler mHandler;
	COORD c = { 0, 0 };
	CustomView(RECT b) : gui::Element(b) { mHandler.SetButtons(gui::MOUSE_LEFT_BUTTON); }

	void SetBorder(gui::Border b) override {
		Element::SetBorder(b);
		mHandler.SetBounds(
			{
			mBounds.left + b.GetWidth(), 
			mBounds.top + b.GetWidth(), 
			mBounds.right - b.GetWidth(), 
			mBounds.bottom - b.GetWidth()
			}
		);
	}

	void Draw(gui::Console* g) override {
		gui::Element::Draw(g);
		g->Set(
			mBounds.left + mBorder.GetWidth() + c.X - 2, 
			mBounds.top + mBorder.GetWidth() + c.Y - 7, 
			L' ', 
			BG_DARK_RED
		);
	}
};

class Test : public gui::Console {
private:
	gui::ContentPanel* panel;
	CustomView* cV;

	gui::Label* l;

	int lClicks = 0;
	int rClicks = 0;
public:
	bool Initialize() override {

		panel = new gui::ContentPanel({ 1, 1, 94, 52 });
		panel->SetTitleHeight(5);
		panel->GetTitleLabel().SetText(L"Custom\nView\nTest");
		panel->GetTitleLabel().SetTextColor(FG_BLACK | BG_WHITE);
		panel->GetTitleLabel().SetBorder({ L' ', BG_GREY, 0 });
		panel->GetTitleLabel().SetBackgroundColor(BG_WHITE);
		panel->SetBackgroundColor(BG_DARK_GREY);
		
		cV = new CustomView({});
		panel->SetContent(cV);
		cV->SetBackgroundColor(BG_BLACK);
		cV->SetBorder({ L' ', BG_DARK_GREY, 1 });
		cV->mHandler.SetPressAction(
			[this](int m) {
				cV->c = GetMousePosition();
			}
		);

		AddElement(panel);
		AddMouseHandler(&cV->mHandler);
		
		//AddElement(l);

		return true;
	}

	~Test() {
		delete panel;
		
	}
};

int main() {
	Test t;
	t.CreateConsole(96, 54, 12, 12);
	t.Run();
	return 0;
}