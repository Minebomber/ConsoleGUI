#include "Console.h"



class Test : public gui::Console {
private:
	bool mDragging = false;

	gui::Label* mLabel = nullptr;
	gui::MouseHandler* mHandler = nullptr;

	COORD mPrev{ -1, -1 };

	int mDOriginX = -1;
	int mDOriginY = -1;

	std::future<void> dragFuture;

	void StartDrag(gui::Console* _, int __) {
		mPrev = GetMousePosition();

		mDOriginX = mPrev.X - mLabel->GetBounds().left;
		mDOriginY = mPrev.Y - mLabel->GetBounds().top;

		mDragging = true;

		dragFuture = gui::ExecuteAsync(std::bind(&Test::Drag, this));
	}

	void Drag() {
		while (mDragging) {
			COORD curr = GetMousePosition();
			if (mPrev.X == -1 && mPrev.Y == -1) mPrev = curr;
			if (curr.X != mPrev.X || curr.Y != mPrev.Y) {
				int x = curr.X - mDOriginX;
				int y = curr.Y - mDOriginY;
				int w = mLabel->GetBounds().right - mLabel->GetBounds().left;
				int h = mLabel->GetBounds().bottom - mLabel->GetBounds().top;
				int dX = 0;
				int dY = 0;

				if (x < 0) dX += -x;
				if (x + w > GetScreenWidth() - 1) dX -= x + w - GetScreenWidth() + 1;
				if (y < 0) dY += -y;
				if (y + h > GetScreenHeight() - 1) dY -= y + h - GetScreenHeight() + 1;
				
				mLabel->SetBounds(
					{
						x + dX,
						y + dY,
						x + dX + w,
						y + dY + h
					}
				);
				mPrev = curr;
			}
		}
	}

	void StopDrag(gui::Console* _, int __) {
		mDragging = false;

	}

public:
	bool Initialize() override {
		mLabel = new gui::Label({10, 13, 20, 17});
		mLabel->SetBackgroundColor(gui::BG_DARK_BLUE);
		mLabel->SetTextColor(gui::FG_WHITE | gui::BG_DARK_BLUE);
		mLabel->SetText(L"Draggable");
		mLabel->SetAlignHorizontal(gui::TEXT_ALIGN_MID);
		mLabel->SetAlignVertical(gui::TEXT_ALIGN_MID);
		AddElement(mLabel);

		mHandler = new gui::MouseHandler({0, 0, GetScreenWidth(), GetScreenHeight()}, gui::MOUSE_LEFT_BUTTON);
		mHandler->SetPressAction(std::bind(&Test::StartDrag, this, std::placeholders::_1, std::placeholders::_2));
		mHandler->SetReleaseAction(std::bind(&Test::StopDrag, this, std::placeholders::_1, std::placeholders::_2));
		AddMouseHandler(mHandler);

		return true;
	}

	~Test() {
		delete mLabel;
	}
};

int main() {
	Test t;//       31
	t.CreateConsole(30, 30, 16, 16);
	t.Run();
	return 0;
}