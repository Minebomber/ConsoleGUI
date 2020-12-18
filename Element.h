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

	struct Constraint {
		void (Rect::* target)(const Rect&, int);
		Element* element;
		int offset;

		bool operator==(const Constraint& c) {
			return element == c.element && target == c.target && offset == c.offset;
		}
	};
	std::vector<Constraint> constraints;

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

	Element* AddConstraint(Constraint c) {
		constraints.push_back(c); 
		return this;
	}

	Element* RemoveConstraint(Constraint c) { 
		constraints.erase(
			std::remove(constraints.begin(), constraints.end(), c), 
			constraints.end()
		);
		return this;
	}

	Color CurrentForeground() const;
	Color CurrentBackground() const;
	Rect InnerBounds() const;

	void ApplyConstraints() {
		for (Constraint c : constraints) (bounds.*c.target)(c.element->bounds, c.offset);
	}

	operator Rect() const { return bounds; }

	virtual void Draw(Window* w);
};

}