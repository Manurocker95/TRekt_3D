#include "button.h"


button::button(int posx, int posy, sftd_font *txtfont, unsigned int txtcolor, unsigned int txtsize, std::string  txt)
{
	x = posx;
	y = posy;
	font = txtfont;
	color = txtcolor;
	size = txtsize;
	text = txt;
	type = 0;			// Types: 0 = Text, 1 = image, 2 = image + text
}

button::button(int posx, int posy, sf2d_texture *image)
{
	x = posx;
	y = posy;
	btnImage = image;
	type = 1;			// Types: 0 = Text, 1 = image, 2 = image + text
}

button::button(int posx, int posy, sf2d_texture * image, sftd_font *txtfont, unsigned int txtcolor, unsigned int txtsize, std::string  txt)
{
	x = posx;
	y = posy;
	font = txtfont;
	color = txtcolor;
	size = txtsize;
	text = txt;
	type = 2;			// Types: 0 = Text, 1 = image, 2 = image + text
	btnImage = image;
}

button::~button()
{
	delete btnImage;
	delete font;
	x = 0;
	y = 0;
	color = 0;
	size = 0;
}

void button::Draw(u16 offset)
{
	switch (type)
	{
	case 0:
		sftd_draw_text(font, x - offset, y, color, size, text.c_str());
		break;
	case 1:
		sf2d_draw_texture(btnImage, x - offset, y);
		break;
	case 2:
		sf2d_draw_texture(btnImage, x - offset, y);
		sftd_draw_text(font, x - offset, y, color, size, text.c_str());
		break;
	}
}

void button::Update()
{

}

bool button::isPressed()
{
	if (hidKeysHeld() & KEY_TOUCH) {
		hidTouchRead(&touch);
		touch_x = touch.px;
		touch_y = touch.py;
		if (x < touch_x &&
			x + sizeof(text.c_str())*size > touch_x &&
			y < touch_y &&
			size + y > touch_y) {
			return true;
		}
		else return false;
	}

	else return false;
}
