#pragma once

#define NOMINMAX
#include <Windows.h>
#include <future>
#include <thread>
#include <vector>

#include "Colors.h"
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

	Color mDefaultForeground = Color::White();
	Color mDefaultBackground = Color::Black();

	Color mFocusedForeground = Color::White();
	Color mFocusedBackground = Color::Black();

	Color mDisabledForeground  = Color::White();
	Color mDisabledBackground  = Color::Black();

	bool mBorders = false;

	std::vector<EventHandler*> mEventHandlers;
public:

	Element(Rect b) : mBounds(b) {}
	Element(const Element& e) : 
		mBounds(e.mBounds), mBorders(e.mBorders),
		mDefaultForeground(e.mDefaultForeground), mDefaultBackground(e.mDefaultBackground), 
		mFocusedForeground(e.mFocusedForeground), mFocusedBackground(e.mFocusedBackground), 
		mDisabledForeground(e.mDisabledForeground), mDisabledBackground(e.mDisabledBackground) 
	{}

	virtual ~Element() {}

	virtual void Autosize() {}

	void HandleEvent(int e, Window* w, int i) {
		for (EventHandler* h : mEventHandlers)
			if (h->ActionExists(e)) h->InvokeAction(e, w, i);
	}

	const Rect& GetBounds() const { return mBounds; }
	virtual void SetBounds(Rect b) { mBounds = b; }

	const Color& GetDefaultForeground() const { return mDefaultForeground; }
	void SetDefaultForeground(Color c) { mDefaultForeground = c; }
	const Color& GetDefaultBackground() const { return mDefaultBackground; }
	void SetDefaultBackground(Color c) { mDefaultBackground = c; }

	const Color& GetFocusedForeground() const { return mFocusedForeground; }
	void SetFocusedForeground(Color c) { mFocusedForeground = c; }
	const Color& GetFocusedBackground() const { return mFocusedBackground; }
	void SetFocusedBackground(Color c) { mFocusedBackground = c; }

	const Color& GetDisabledForeground() const { return mDisabledBackground; }
	void SetDisabledForeground(Color c) { mDisabledBackground = c; }
	const Color& GetDisabledBackground() const { return mDisabledBackground; }
	void SetDisabledBackground(Color c) { mDisabledBackground = c; }

	void AddEventHandler(EventHandler* e) { mEventHandlers.push_back(e); }
	void RemoveEventHandler(EventHandler* e) { mEventHandlers.erase(std::remove(mEventHandlers.begin(), mEventHandlers.end(), e), mEventHandlers.end()); }

	const bool& GetBorders() const { return mBorders; }
	virtual void SetBorders(bool b) { mBorders = b; }

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