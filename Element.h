#pragma once

#define NOMINMAX
#include <Windows.h>
#include <future>
#include <thread>
#include <vector>

#include "Style.h"
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