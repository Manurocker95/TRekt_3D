/* This file is part of T-Rekt 3D!

Copyright (C) 2017 Manuel Rodríguez Matesanz
>    This program is free software: you can redistribute it and/or modify
>    it under the terms of the GNU General Public License as published by
>    the Free Software Foundation, either version 3 of the License, or
>    (at your option) any later version.
>
>    This program is distributed in the hope that it will be useful,
>    but WITHOUT ANY WARRANTY; without even the implied warranty of
>    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
>    GNU General Public License for more details.
>
>    You should have received a copy of the GNU General Public License
>    along with this program.  If not, see <http://www.gnu.org/licenses/>.
>    See LICENSE for information.
*/

#ifndef _METEO_H_
#define _METEO_H_

#define FRAMECOUNTERMETEO 5

#include "sf2d.h"
#include "sfil.h"
#include "filepaths.h"
#include "Settings.h"
#include "Dino.h"

class Meteorite
{
public:
	enum MState { FALL, STAY };
	Meteorite();
	Meteorite(int x, int y, sf2d_texture* sprite, bool multipleFrames, u16 numFrames, u16 sizePerFrame);
	~Meteorite();
	void Draw(float offset);
	void Update();
	int getX();
	void setX(int value);
	int getY();
	void setY(int value);
	void Fall();
	void moveToCoord(int x, int y);
	sf2d_texture* getSprite();
	void setSprite(sf2d_texture* sprite);
	void setDeadSprite(sf2d_texture* sprite, u16 numFrames, u16 sizePerFrame);
	void end();
	MState getState();
	void setState(MState value);
	void setInvincible(bool value);
	bool getInvincible();
	void setNumFrames(u16 value);
	u16 getNumFrames();
	void setFrameSize(u16 value);
	u16 getFrameSize();
	void reset();
	bool CheckCollision(Dinosaur* d);
private:
	int m_x, m_originalX;
	int m_y, m_originalY;
	u16 m_numFrames;
	u16 m_sizePerFrame;
	u16 m_currentFrame;
	u16 m_secondaryCounter;
	u16 m_offsetFrameX;
	bool m_male;
	bool m_moving;
	bool m_multipleFrames;
	bool m_invincible;						// Si el modo invencible está activado o no	
	sf2d_texture * m_sprite;
	MState m_state;
};

#endif