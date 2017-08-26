#pragma once
#include <sf2d.h>
#include <sftd.h>
#include <sfil.h>
#include <vector>
#include <string>
#include <3ds.h>
class menu
{
private:
	int hilighted = 1;
public:
	menu(int x, int y, int number, sftd_font *fnt, int textsize);
	~menu();
	int x; int y;
	void enable(bool enabl);
	bool enabled = false;
	void update();
	sf2d_texture *menutex = sfil_load_PNG_file("sdmc:/NAMELESS/menu/def.png", SF2D_PLACE_RAM);
	int count;
	sftd_font *font;
	int TXT_SIZE;
	std::vector<std::string> charvector;
	int selected = 0;
};

