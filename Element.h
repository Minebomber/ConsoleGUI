#pragma once

#define NOMINMAX
#include <Windows.h>
#include <future>
#include <thread>
#include <vector>

#include "Style.h"
#include "Rect.h"
#include "Events.h"
#include "Constraints.h"
#include "Window.h"

namespace gui {

class Window;
class Constraint;

class View {
	friend class Window;
protected:
	std::vector<EventHandler*> mEventHandlers;
	std::vector<Constraint> mConstraints;
	std::vector<View*> mSubviews;
public:
	View* parent = nullptr;

	enum class State {
		Default,
		Focused,
		Disabled
	};
	State state = State::Default;

	Style style;

	Rect bounds = { 0, 0, 0, 0 };

	struct Padding {
		int left, top, right, bottom;
		int TotalX() const { return left + right; }
		int TotalY() const { return top + bottom; }
	};

	Padding padding = { 0 }; // use for autosize

	View(Rect b) : bounds(b) {}
	View(const View& e) : bounds(e.bounds), style(e.style) {}

	virtual ~View();

	virtual void Autosize();

	void HandleEvent(EventType e, Window* w, int i) {
		for (EventHandler* h : mEventHandlers)
			if (h->ActionExists(e)) h->InvokeAction(e, w, i);
	}

	void AddEventHandler(EventHandler* e) { mEventHandlers.push_back(e); }
	void RemoveEventHandler(EventHandler* e) { mEventHandlers.erase(std::remove(mEventHandlers.begin(), mEventHandlers.end(), e), mEventHandlers.end()); }

	void AddConstraint(Constraint c);

	void RemoveConstraint(Constraint c);

	void AddSubview(View* v) {
		v->parent = this;
		mSubviews.push_back(v);
	}

	void RemoveSubview(View* v) {
		v->parent = nullptr;
		mSubviews.erase(
			std::remove(mSubviews.begin(), mSubviews.end(), v),
			mSubviews.end()
		);
	}

	Point TrueOrigin() const;

	Rect TrueBounds() const;

	Rect TrueInnerBounds() const;

	Color CurrentForeground() const;
	Color CurrentBackground() const;
	Rect InnerBounds() const;

	void ApplyConstraints();

	operator Rect() const { return bounds; }

	virtual void Draw(Window* w);
};

}