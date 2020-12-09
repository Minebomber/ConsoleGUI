#include "Console.h"

class Test : public gui::Console {
private:
	gui::TextField* tf;
	gui::Element* el;
	gui::Button* btnHMin, * btnHMid, * btnHMax;
	gui::Button* btnVMin, * btnVMid, * btnVMax;

public:
	bool Initialize() override {
		gui::Window* w = new gui::Window(GetScreenWidth(), GetScreenHeight());
		SetCurrentWindow(w);

		w->SetScheme(gui::WindowScheme::Green());

		el = new gui::Element({ 0, 5, 29, 17 });
		el->SetBorder(new gui::TitledBorder(L"Text Alignment"));
		w->AddElement(el);

		tf = new gui::TextField({ 7, 10, 20, 11 }, gui::Charset::Alphanum());
		tf->SetTextWrap(gui::TEXT_WRAP_WORD);
		w->AddElement(tf);

		btnVMin = new gui::Button({ 1, 11, 5, 3 });
		btnVMin->SetText(L"MIN");
		btnVMin->SetPressAction([this](int _) {tf->SetAlignVertical(gui::TEXT_ALIGN_MIN); });
		w->AddElement(btnVMin);

		btnVMid = new gui::Button({ 1, 14, 5, 3 });
		btnVMid->SetText(L"MID");
		btnVMid->SetPressAction([this](int _) {tf->SetAlignVertical(gui::TEXT_ALIGN_MID); });
		w->AddElement(btnVMid);

		btnVMax = new gui::Button({ 1, 17, 5, 3 });
		btnVMax->SetText(L"MAX");
		btnVMax->SetPressAction([this](int _) {tf->SetAlignVertical(gui::TEXT_ALIGN_MAX); });
		w->AddElement(btnVMax);

		btnHMin = new gui::Button({ 9, 7, 5, 3 });
		btnHMin->SetText(L"MIN");
		btnHMin->SetPressAction([this](int _) {tf->SetAlignHorizontal(gui::TEXT_ALIGN_MIN); });
		w->AddElement(btnHMin);

		btnHMid = new gui::Button({ 15, 7, 5, 3 });
		btnHMid->SetText(L"MID");
		btnHMid->SetPressAction([this](int _) {tf->SetAlignHorizontal(gui::TEXT_ALIGN_MID); });
		w->AddElement(btnHMid);

		btnHMax = new gui::Button({ 21, 7, 5, 3 });
		btnHMax->SetText(L"MAX");
		btnHMax->SetPressAction([this](int _) {tf->SetAlignHorizontal(gui::TEXT_ALIGN_MAX); });
		w->AddElement(btnHMax);

		return true;
	}

	~Test() {
		delete btnHMin; delete btnHMid; delete btnHMax;
		delete btnVMin; delete btnVMid; delete btnVMax;
		delete el;
		delete tf;
		SetCurrentWindow(nullptr);
	}
};

int main() {
	Test t;
	t.CreateConsole(30, 30, 8, 16);
	t.Run();
	return 0;
}