#include <3ds.h>
#include <string>
#include <sf2d.h>
#include <sftd.h>
#include <sfil.h>
#include <vector>
class textbox
{
private:
	int textboxcount = 0;
	std::vector<std::string> stringvector;
	int txt = 0;

public:
	sftd_font * dasFont;
	bool shown = false;
	int xpos;
	int ypos;
	int size;
	unsigned int color;
	int a = 0;
	std::string character;
	int delayTime;
	int constDelaytime;
	int creationTime;
	int currentIndex;
	sf2d_texture *textex = sfil_load_PNG_file("sdmc:/NAMELESS/textboxes/default.png", SF2D_PLACE_RAM);
	textbox(unsigned int _color, int _delayTime, sftd_font * _dasFont, int _size, std::string _character);
	~textbox();
	int draw();
	void addtext(std::string text);
};

