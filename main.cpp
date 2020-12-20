#include "Console.h"

class Draggable : public gui::Element {
private:
	gui::Point mOriginOffset{ 0, 0 };
	void Init() {
		AddEventHandler(new gui::EventHandler({
			{gui::EventType::MouseDown, [this](gui::Window* w, int i) {
				mOriginOffset = w->mousePosition - bounds.Min();
				w->focusedElement = this;
			}},
			{gui::EventType::MouseUp, [this](gui::Window* w, int i) {
				w->focusedElement = nullptr;
			}},
			{gui::EventType::MouseDrag, [this](gui::Window* w, int i) {
				gui::Point p = w->mousePosition - mOriginOffset;
				this->bounds.x = std::max(0, std::min(p.x, w->GetWidth() - bounds.width));;
				this->bounds.y = std::max(0, std::min(p.y, w->GetHeight() - bounds.height));;
			}}
		}));
	}
public:
	Draggable(gui::Rect b) : gui::Element(b) { Init(); }
	Draggable(const Draggable& d) : Draggable(d.bounds) { Init(); }
};

class Program : public gui::Console {
public:
	gui::Window* SetupWindow() {
		auto w = new gui::Window(GetScreenWidth(), GetScreenHeight());
		w->SetStyle<gui::Element>(gui::Style::Red(true));
		SetCurrentWindow(w);
		return w;
	}

	void Initialize() override {
		using namespace gui;
		auto w = SetupWindow();

		Draggable* draggable = new Draggable({ 0, 0, 3, 3 });
		w->AddElement(draggable);

		auto e2 = new Label({}, L"Test", true);
		w->AddElement(e2);

		auto e3 = new Label({}, L"test", true);
		w->AddElement(e3);

		auto e4 = new Label({}, L"i want die", true);
		w->AddElement(e4);

		auto e5 = new Label({}, L"Test label 5", true);
		w->AddElement(e5);

		e2->AddConstraint(new HorizontalCenterConstraint(draggable))->
			AddConstraint(new TopToBottomConstraint(draggable, 1));

		e3->AddConstraint(new HorizontalCenterConstraint(e2))->
			AddConstraint(new TopToBottomConstraint(e2, 1));

		e4->AddConstraint(new HorizontalCenterConstraint(e3))->
			AddConstraint(new TopToBottomConstraint(e3, 1));

		e5->AddConstraint(new HorizontalCenterConstraint(e4))->
			AddConstraint(new TopToBottomConstraint(e4, 1));
	}
};

int main() {
	Program p;
	p.Create(50, 20, 8, 16);
	p.Run();
	return 0;
}