#pragma once

#define NOMINMAX
#include <Windows.h>
#include <future>
#include <thread>
#include <vector>

#include "ElementStyle.h"
#include "Rect.h"
#include "Events.h"
#include "Window.h"

namespace gui {

class Window;

class Element {
protected:
	std::vector<EventHandler*> mEventHandlers;
public:
	enum class State {
		Default,
		Focused,
		Disabled
	};
	State state = State::Default;

	class Style {
	public:
		Color defaultForeground;
		Color defaultBackground;
		Color focusedForeground;
		Color focusedBackground;
		Color disabledForeground;
		Color disabledBackground;

		bool borders;

		static Style* Default(bool bds = true) {
			return new Style(
				Color::Grey(), Color::Black(),
				Color::White(), Color::Black(),
				Color::DarkGrey(), Color::Black(),
				bds
			);
		}

		static Style* Green(bool bds = true) {
			return new Style(
				Color::DarkGreen(), Color::Black(),
				Color::Green(), Color::Black(),
				Color::DarkGreen(), Color::Black(),
				bds
			);
		}

		static Style* Red(bool bds = true) {
			return new Style(
				Color::DarkRed(), Color::Black(),
				Color::Red(), Color::Black(),
				Color::DarkRed(), Color::Black(),
				bds
			);
		}

		Style() : defaultForeground(0), defaultBackground(0), focusedForeground(0),
			focusedBackground(0), disabledForeground(0), disabledBackground(0), borders(false) {}

		Style(Color defF, Color defB, Color focF, Color focB, Color disF, Color disB, bool bds) :
			defaultForeground(defF), defaultBackground(defB), focusedForeground(focF),
			focusedBackground(focB), disabledForeground(disF), disabledBackground(disB), borders(bds) {}

		Style(const Style& s) :
			defaultForeground(s.defaultForeground), defaultBackground(s.defaultBackground),
			focusedForeground(s.focusedForeground), focusedBackground(s.focusedBackground),
			disabledForeground(s.disabledForeground), disabledBackground(s.disabledBackground),
			borders(s.borders) {}
	};
	Style style;

	Rect bounds = { 0, 0, 0, 0 };

	Element(Rect b) : bounds(b) {}
	Element(const Element& e) : bounds(e.bounds), style(e.style) {}

	virtual ~Element() {}

	virtual void Autosize() {}

	void HandleEvent(EventType e, Window* w, int i) {
		for (EventHandler* h : mEventHandlers)
			if (h->ActionExists(e)) h->InvokeAction(e, w, i);
	}

	void AddEventHandler(EventHandler* e) { mEventHandlers.push_back(e); }
	void RemoveEventHandler(EventHandler* e) { mEventHandlers.erase(std::remove(mEventHandlers.begin(), mEventHandlers.end(), e), mEventHandlers.end()); }

	Color CurrentForeground() const;
	Color CurrentBackground() const;
	Rect InnerBounds() const;

	operator Rect() const { return bounds; }

	virtual void Draw(Window* w);
};

}