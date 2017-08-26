#include "textbox.h"



textbox::textbox(unsigned int _color, int _delayTime, sftd_font * _dasFont, int _size, std::string _character)
{
	delayTime = _delayTime;
	constDelaytime = delayTime;
	creationTime = 0;
	currentIndex = 0;
	color = _color;
	dasFont = _dasFont;
	size = _size;
	character = _character;
}


textbox::~textbox()
{
	stringvector.clear();
	std::vector<std::string>().swap(stringvector);
}

int textbox::draw() {
	if (shown == true) {
		hidScanInput();
		a += 10;
		delayTime = constDelaytime;
		if (hidKeysHeld() & KEY_A) {
			delayTime /= 5;

		}
		while (a - creationTime > delayTime) {
			creationTime += delayTime;
			currentIndex += 1;
		}
		sf2d_draw_texture(textex, 0, 110);
		int charasize = 110 / (character.length());
		int charax = 62 - ((character.length()*charasize) / 2);
		int charay = 150 - (character.length() / 2);
		sftd_draw_text(dasFont, charax, charay, RGBA8(255, 255, 255, 255), charasize, character.c_str());
		sftd_draw_text(dasFont, 0+10, 110+70, color, size, stringvector[txt].substr(0,currentIndex).c_str());
		if (currentIndex > int(stringvector[txt].length())) { currentIndex = int(stringvector[txt].length()); }
		if (currentIndex == int(stringvector[txt].length()) && (hidKeysDown() & KEY_A)) {
			if (txt < textboxcount-1) {
				currentIndex = 0;
				txt++;
				shown = true;
				return 0;
			}
			else {
				currentIndex = 0;
				txt = 0;
				shown = false;
				return 1;
			}
		}
		else { shown = true;  return 0; }
	}
	else return 1;
}

void textbox::addtext(std::string text)
{
	stringvector.push_back(text);
	textboxcount++;
}
