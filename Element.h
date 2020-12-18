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

enum ElementState {
	ELEMENT_DEFAULT,
	ELEMENT_FOCUSED,
	ELEMENT_DISABLED,
};

class Element {
protected:
	std::vector<EventHandler*> mEventHandlers;
public:
	Rect bounds = { 0, 0, 0, 0 };
	int state = ELEMENT_DEFAULT;
	ElementStyle style;

	Element(Rect b) : bounds(b) {}
	Element(const Element& e) : bounds(e.bounds), style(e.style) {}

	virtual ~Element() {}

	virtual void Autosize() {}

	void HandleEvent(int e, Window* w, int i) {
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