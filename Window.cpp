#include "Window.h"

namespace gui {

Window::Window(int w, int h) : view(new View({ 0, 0, w, h })), mBuffer(new CHAR_INFO[w * h]) {}

Window::Window(int w, int h, Style* defStyle) : view(new View({ 0, 0, w, h })),
mBuffer(new CHAR_INFO[w * h]), mStyleMap({ {std::type_index(typeid(View)), defStyle} }) {}


void Window::SetChar(int x, int y, WCHAR chr, WORD clr) {
	x += mDrawOffset.x;
	y += mDrawOffset.y;
	int i = y * view->bounds.width + x;
	if (i < view->bounds.width * view->bounds.height) {
		mBuffer[i].Char.UnicodeChar = chr;
		mBuffer[i].Attributes = clr;
	}
}

void Window::FillScreen(WCHAR chr, WORD clr) {
	for (int i = 0; i < view->bounds.width * view->bounds.height; i++) {
		mBuffer[i].Char.UnicodeChar = chr;
		mBuffer[i].Attributes = clr;
	}
}

void Window::DrawRect(Rect r, WCHAR chr, WORD clr, bool fill) {
	for (int i = 0; i < r.height; i++) {
		for (int j = 0; j < r.width; j++) {
			if (fill) SetChar(j + r.x, i + r.y, chr, clr);
			else if (i == 0 || i == r.height - 1 || j == 0 || j == r.width - 1) 
				SetChar(j + r.x, i + r.y, chr, clr);
		}
	}
}

void Window::WriteString(int x, int y, const std::wstring& str, WORD clr) {
	x += mDrawOffset.x;
	y += mDrawOffset.y;
	for (int i = 0; i < str.size(); i++) {
		int idx = y * view->bounds.width + x + i;
		if (idx < view->bounds.width * view->bounds.height) {
			mBuffer[idx].Char.UnicodeChar = std::max(str[i], L' ');
			mBuffer[idx].Attributes = clr;
		}
	}
}

void Window::WriteString(int x, int y, const std::wstring& str, WORD clr, int st, int w) {
	x += mDrawOffset.x;
	y += mDrawOffset.y;
	for (int i = 0; i < w; i++) {
		int idx = y * view->bounds.width + x + i;
		if (idx < view->bounds.width * view->bounds.height) {
			mBuffer[idx].Char.UnicodeChar = std::max(str[st + i], L' ');
			mBuffer[idx].Attributes = clr;
		}
	}
}

void Window::RenderText(Rect r, const std::wstring& txt, WORD clr, Alignment alignH, Alignment alignV, TextWrap wrap) {
	int textWidth = r.width;
	int textHeight = r.height;

	if (textHeight < 0)
		return;

	struct LineInfo {
		int dX, sI, lW;
	};
	LineInfo* lines = new LineInfo[textHeight];
	int currentLine = 0;

	int lineBeginIdx = 0;
	int currentIdx = 0;
	while (currentIdx < txt.length() && currentLine < textHeight) {
		int lineWidth = currentIdx - lineBeginIdx + 1;
		// Adjust line spacing x
		int xOffset = 0;
		if (alignH == Alignment::Mid) xOffset = (textWidth - lineWidth) / 2;
		else if (alignH == Alignment::Max)  xOffset = textWidth - lineWidth;

		if (lineWidth >= textWidth) {
			// line too long
			if (wrap == TextWrap::Word) {
				// find space
				auto iStart = txt.rbegin() + (txt.length() - currentIdx - 1);
				auto iEnd = txt.rbegin() + (txt.length() - lineBeginIdx);
				auto iSpace = std::find(iStart, iEnd, L' ');

				if (iSpace != iEnd) {
					// if end of string, print line as is
					// else print until space, newline there
					if (currentIdx == txt.length() - 1) {
						lines[currentLine] = { xOffset, lineBeginIdx, lineWidth }; currentLine++;
					} else {
						int spaceIdx = (int)std::distance(txt.begin(), iSpace.base()) - 1;
						lineWidth = spaceIdx - lineBeginIdx;
						if (alignH == Alignment::Mid) xOffset = (textWidth - lineWidth) / 2;
						else if (alignH == Alignment::Max)  xOffset = textWidth - lineWidth;

						lines[currentLine] = { xOffset, lineBeginIdx, lineWidth }; currentLine++;
						lineBeginIdx = spaceIdx + 1;
					}
				} else {
					// char wrap
					// display line from linebegin->current
					// move to next line
					lines[currentLine] = { xOffset, lineBeginIdx, lineWidth }; currentLine++;
					lineBeginIdx = currentIdx + 1;
				}
			} else {
				// char wrap
					// display line from linebegin->current
				// move to next line
				lines[currentLine] = { xOffset, lineBeginIdx, lineWidth }; currentLine++;
				lineBeginIdx = currentIdx + 1;
			}
		} else if (txt[currentIdx] == L'\n') {
			// break on newline
			// dont want to include \n, print linewidth - 1
			lineWidth--;
			if (alignH == Alignment::Mid) xOffset = (textWidth - lineWidth) / 2;
			else if (alignH == Alignment::Max)  xOffset = textWidth - lineWidth;

			lines[currentLine] = { xOffset, lineBeginIdx, lineWidth }; currentLine++;
			// move to next line
			lineBeginIdx = currentIdx + 1;
		} else if (currentIdx == txt.length() - 1) {
			// display to end
			// same line, no wrapping
			lines[currentLine] = { xOffset, lineBeginIdx, lineWidth }; currentLine++;
		}
		currentIdx++;
	}

	int yOffset = 0;
	if (alignV == Alignment::Mid) yOffset = (textHeight - currentLine) / 2;
	else if (alignV == Alignment::Max) yOffset = textHeight - currentLine;
	for (int i = 0; i < currentLine; i++)
		WriteString(r.x + lines[i].dX, r.y + yOffset + i, txt, clr, lines[i].sI, lines[i].lW);

	delete[] lines;
}

void Window::ApplyStyle(View* v, bool applyToSub) {
	if (Style* s = GetStyle(*v)) { v->style = *s; }
	ApplyToAllViews([this](View* sv) {
		if (Style* s = GetStyle(*sv)) { sv->style = *s; }
	}, v);
}

void Window::ApplyToAllViews(std::function<void(View*)> f, View* root) {
	if (!root) root = view;
	for (View* v : root->mSubviews) {
		f(v);
		ApplyToAllViews(f, v);
	}
}

View* Window::ViewAtPoint(Point p, View* root) {
	if (!root) root = view;
	View* found = root;
	p -= root->InnerBounds().Min();
	for (View* v : root->mSubviews) {
		if (v->bounds.Contains(p)) found = ViewAtPoint(p, v);
	}
	return found;
}

void Window::Display() {
	FillScreen(baseChar, baseColor);
	view->Draw(this);

}

Window::~Window() {
	delete view;
	for (auto p : mStyleMap) delete p.second;
	delete[] mBuffer;
}

}