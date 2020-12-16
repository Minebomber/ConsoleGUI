#pragma once

namespace gui {

class Color {
private:
	unsigned short mValue;
public:
	static Color Black() { return Color(0x0); }
	static Color DarkBlue() { return Color(0x1); }
	static Color DarkGreen() { return Color(0x2); }
	static Color DarkCyan() { return Color(0x3); }
	static Color DarkRed() { return Color(0x4); }
	static Color DarkMagenta() { return Color(0x5); }
	static Color DarkYellow() { return Color(0x6); }
	static Color Grey() { return Color(0x7); }
	static Color DarkGrey() { return Color(0x8); }
	static Color Blue() { return Color(0x9); }
	static Color Green() { return Color(0xA); }
	static Color Cyan() { return Color(0xB); }
	static Color Red() { return Color(0xC); }
	static Color Magenta() { return Color(0xD); }
	static Color Yellow() { return Color(0xE); }
	static Color White() { return Color(0xF); }

	Color() : mValue(0) {}
	Color(unsigned short v) : mValue(v) {}

	unsigned short Foreground() const { return mValue; }
	unsigned short Background() const { return mValue << 4; }
};

}