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
					w->mousePosition.y - TrueOrigin().y - 2 - style.borders
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
	mOldHeight = bounds.height;
	bounds.height += options.size() + 1;
	mShowOptions = true;
}

void SelectionList::HideOptions() {
	bounds.height = mOldHeight;
	mShowOptions = false;
}

void SelectionList::Autosize() {
	View::Autosize();
	bounds.width += (*std::max_element(
		options.begin(),
		options.end(),
		[](const auto& a, const auto& b) { return a.size() < b.size(); }
	)).size();
	bounds.height += 1;
}

void SelectionList::Draw(Window* w) {
	View::Draw(w);
	Rect ib = InnerBounds();
	w->RenderText(
		InnerBounds(),
		options.at(selection),
		CurrentForeground().value | CurrentBackground().value << 4,
		TEXT_ALIGN_MIN,
		TEXT_ALIGN_MIN,
		TEXT_WRAP_CHAR
	);

	if (mShowOptions) {
		Rect ib = InnerBounds();
		ib.height = 1;
		ib.y++;
		for (int i = 0; i < options.size(); i++) {
			ib.y += ib.height;
			w->RenderText(
				ib,
				options.at(i),
				CurrentForeground().value | CurrentBackground().value << 4,
				TEXT_ALIGN_MIN,
				TEXT_ALIGN_MIN,
				TEXT_WRAP_CHAR
			);
		}
	}
}

}