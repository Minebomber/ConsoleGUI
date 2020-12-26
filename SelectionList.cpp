#include "SelectionList.h"

namespace gui {

SelectionList::SelectionList(Rect b) : View(b), options(), selection(0) { Init(); }

SelectionList::SelectionList(Rect b, std::initializer_list<std::wstring> opts, int select) :
	View(b), options(opts), selection(select) { Init(); }

void SelectionList::Init() {
	AddEventHandler(new EventHandler({
		{EventType::MouseDown, [this](Window* w, int i) {
			if (mShowOptions) {
				// Get clicked
				selection = std::max(
					0,
					std::min(
					(int)options.size() - 1,
					w->mousePosition.y - TrueOrigin().y - 2 - style.borders - padding.top
					)
				);
				HideOptions();
			} else {
				ShowOptions();
			}
		}}
	}));
}

void SelectionList::ShowOptions() {
	mOldBottom = bounds.bottom;
	bounds.bottom += options.size() + 1;
	mShowOptions = true;
}

void SelectionList::HideOptions() {
	bounds.bottom = mOldBottom;
	mShowOptions = false;
}

void SelectionList::Autosize() {
	View::Autosize();
	bounds.right += (*std::max_element(
		options.begin(),
		options.end(),
		[](const auto& a, const auto& b) { return a.size() < b.size(); }
	)).size();
	bounds.bottom += 1;
}

void SelectionList::Draw(Window* w) {
	View::Draw(w);
	Rect ib = InnerBounds();
	w->RenderText(
		InnerBounds(),
		options.at(selection),
		CurrentForeground().value | CurrentBackground().value << 4,
		Alignment::Min,
		Alignment::Min,
		TextWrap::Char
	);

	if (mShowOptions) {
		Rect ib = InnerBounds();
		
		ib.top += 1;
		ib.bottom = ib.top + 1;
		for (int i = 0; i < options.size(); i++) {
			int h = ib.Height();
			ib.top += h;
			ib.bottom += h;
			w->RenderText(
				ib,
				options.at(i),
				CurrentForeground().value | CurrentBackground().value << 4,
				Alignment::Min,
				Alignment::Min,
				TextWrap::Char
			);
		}
	}
}

}