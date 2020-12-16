#pragma once

namespace gui {

enum Colors {
	FG_BLACK = 0x0000,
	FG_DARK_BLUE = 0x0001,
	FG_DARK_GREEN = 0x0002,
	FG_DARK_CYAN = 0x0003,
	FG_DARK_RED = 0x0004,
	FG_DARK_MAGENTA = 0x0005,
	FG_DARK_YELLOW = 0x0006,
	FG_GREY = 0x0007,
	FG_DARK_GREY = 0x0008,
	FG_BLUE = 0x0009,
	FG_GREEN = 0x000A,
	FG_CYAN = 0x000B,
	FG_RED = 0x000C,
	FG_MAGENTA = 0x000D,
	FG_YELLOW = 0x000E,
	FG_WHITE = 0x000F,
	BG_BLACK = 0x0000,
	BG_DARK_BLUE = 0x0010,
	BG_DARK_GREEN = 0x0020,
	BG_DARK_CYAN = 0x0030,
	BG_DARK_RED = 0x0040,
	BG_DARK_MAGENTA = 0x0050,
	BG_DARK_YELLOW = 0x0060,
	BG_GREY = 0x0070,
	BG_DARK_GREY = 0x0080,
	BG_BLUE = 0x0090,
	BG_GREEN = 0x00A0,
	BG_CYAN = 0x00B0,
	BG_RED = 0x00C0,
	BG_MAGENTA = 0x00D0,
	BG_YELLOW = 0x00E0,
	BG_WHITE = 0x00F0,
};

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