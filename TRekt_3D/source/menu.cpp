#include "menu.h"

menu::menu(int posx, int posy, int number, sftd_font *fnt , int textsize)
{
	x = posx;
	y = posy;
	count = number;
	font = fnt;
	TXT_SIZE = textsize;
}


menu::~menu()
{
	x = 0;
	y = 0;
	enabled = false;
	count = 0;
	TXT_SIZE = 0;
	charvector.clear();
	selected = 0;
	hilighted = 1;
	std::vector<std::string>().swap(charvector);
}

void menu::enable(bool enabl)
{
	enabled = enabl;
}

void menu::update()
{
	selected = 0;
	if (enabled == true) {
		int distance = 215 / (TXT_SIZE * count);
		sf2d_draw_texture(menutex, x, y);
		for (int i = 1; i < count + 1; i++) {
			sftd_draw_text(font, x + TXT_SIZE, y + (TXT_SIZE * i * distance), RGBA8(255, 255, 255, 255), TXT_SIZE, charvector[i-1].c_str());
		}
		sftd_draw_text(font, x + 1, y + (TXT_SIZE * hilighted * distance), RGBA8(255, 255, 255, 255), TXT_SIZE, ">");
		if (hidKeysDown() & KEY_UP)   { hilighted -= 1; }
		if (hidKeysDown() & KEY_DOWN) { hilighted += 1; }
		if (hilighted < 1) { hilighted = count; }
		if (hilighted > count) { hilighted = 1; }
		if (hidKeysDown() & KEY_A) { selected = hilighted; enabled = false; }
	}
}
