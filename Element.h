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
	friend class Window;
	friend class Console;
protected:
	Rect mBounds = { 0, 0, 0, 0 };

	int mState = ELEMENT_DEFAULT;

	ElementStyle mStyle;

	std::vector<EventHandler*> mEventHandlers;
public:

	Element(Rect b) : mBounds(b) {}
	Element(const Element& e) : mBounds(e.mBounds), mStyle(e.mStyle) 
	{}

	virtual ~Element() {}

	virtual void Autosize() {}

	void HandleEvent(int e, Window* w, int i) {
		for (EventHandler* h : mEventHandlers)
			if (h->ActionExists(e)) h->InvokeAction(e, w, i);
	}

	const Rect& GetBounds() const { return mBounds; }
	virtual void SetBounds(Rect b) { mBounds = b; }

	Rect& Bounds() { return mBounds; }

	ElementStyle& Style() { return mStyle; }

	void AddEventHandler(EventHandler* e) { mEventHandlers.push_back(e); }
	void RemoveEventHandler(EventHandler* e) { mEventHandlers.erase(std::remove(mEventHandlers.begin(), mEventHandlers.end(), e), mEventHandlers.end()); }

	Color GetCurrentForeground() const;
	Color GetCurrentBackground() const;
	Rect GetInnerBounds() const;

	// Alignment passthroughs
	Element* AlignLeftToLeft(Element* e, int offset = 0) {
		Rect r = GetBounds(); SetBounds(r.AlignLeftToLeft(e->GetBounds(), offset));
		return this;
	}

	Element* AlignLeftToRight(Element* e, int offset = 0) {
		Rect r = GetBounds(); SetBounds(r.AlignLeftToRight(e->GetBounds(), offset));
		return this;
	}

	Element* AlignTopToTop(Element* e, int offset = 0) {
		Rect r = GetBounds(); SetBounds(r.AlignTopToTop(e->GetBounds(), offset));
		return this;
	}

	Element* AlignTopToBottom(Element* e, int offset = 0) {
		Rect r = GetBounds(); SetBounds(r.AlignTopToBottom(e->GetBounds(), offset));
		return this;
	}
	
	Element* AlignRightToRight(Element* e, int offset = 0) {
		Rect r = GetBounds(); SetBounds(r.AlignRightToRight(e->GetBounds(), offset));
		return this;
	}

	Element* AlignRightToLeft(Element* e, int offset = 0) {
		Rect r = GetBounds(); SetBounds(r.AlignRightToLeft(e->GetBounds(), offset));
		return this;
	}

	Element* AlignBottomToBottom(Element* e, int offset = 0) {
		Rect r = GetBounds(); SetBounds(r.AlignBottomToBottom(e->GetBounds(), offset));
		return this;
	}

	Element* AlignBottomToTop(Element* e, int offset = 0) {
		Rect r = GetBounds(); SetBounds(r.AlignBottomToTop(e->GetBounds(), offset));
		return this;
	}
	
	Element* CenterHorizontalWith(Element* e, int offset = 0) {
		Rect r = GetBounds(); SetBounds(r.CenterHorizontalWith(e->GetBounds(), offset));
		return this;
	}

	Element* CenterVerticalWith(Element* e, int offset = 0) {
		Rect r = GetBounds(); SetBounds(r.CenterVerticalWith(e->GetBounds(), offset));
		return this;
	}

	Element* CenterWith(Element* e, Point offset = { 0, 0 }) {
		Rect r = GetBounds(); SetBounds(r.CenterWith(e->GetBounds(), offset));
		return this;
	}

	virtual void Draw(Window* w);
};

}