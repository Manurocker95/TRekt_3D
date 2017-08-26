#pragma once
#include <sf2d.h>
#include <sftd.h>
#include <sfil.h>
#include <3ds.h>
#include <string>

class button
{
public:
	button(int posx, int posy, sf2d_texture * btnImage);
	button(int posx, int posy, sftd_font *txtfont, unsigned int txtcolor, unsigned int txtsize, std::string txt);
	button(int posx, int posy, sf2d_texture * btnImage, sftd_font *txtfont, unsigned int txtcolor, unsigned int txtsize, std::string txt);
	~button();
	void Draw(u16 offset);
	void Update();
	bool isPressed();

private:
	int x; 
	int y; 
	int type;
	unsigned int color; 
	unsigned int size; 
	std::string text;
	sftd_font *font;
	sf2d_texture *btnImage;
	touchPosition touch;
	u16 touch_x = 320 / 2;
	u16 touch_y = 240 / 2;
};

