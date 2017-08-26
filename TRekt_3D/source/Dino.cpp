#include "Dino.h"

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

//Constructor con valores por defecto
Dinosaur::Dinosaur()
{

}

Dinosaur::Dinosaur(int x, u16 y, bool multipleFrames, u16 numFrames, u16 sizePerFrame, bool male)
{
	m_x = x;
	m_originalX = x;
	m_y = y;
	m_multipleFrames = multipleFrames;
	m_numFrames = numFrames;
	m_sizePerFrame = sizePerFrame;
	m_currentFrame = 0;
	m_secondaryCounter = 0;
	m_sizeYPerFrame = 26;
	m_invincible = false;
	m_state = ALIVE;
	m_offsetFrameX = 0;
	m_startingYOffset = 0;
	m_moving = false;
	m_male = male;
	m_sprite = sfil_load_PNG_file(IMG_DINO_SPRITE, SF2D_PLACE_RAM);
}

Dinosaur::Dinosaur(int x, u16 y, sf2d_texture * sprite, bool multipleFrames, u16 numFrames, u16 sizePerFrame, u16 sizeYPerFrame, bool male)
{
	m_x = x;
	m_originalX = x;
	m_y = y;
	m_originalY = y;
	m_sprite = sprite;
	m_multipleFrames = multipleFrames;
	m_numFrames = numFrames;
	m_sizePerFrame = sizePerFrame;
	m_currentFrame = 0;
	m_secondaryCounter = 0;
	m_invincible = false;
	m_state = ALIVE;
	m_offsetFrameX = 0;
	m_startingYOffset = 0;
	m_stage = 0;
	m_moving = false;
	m_male = male;
	m_age = 0;
	m_sizeYPerFrame = sizeYPerFrame;
}

Dinosaur::~Dinosaur()
{
	delete m_sprite;
}

void Dinosaur::Draw(float offset)
{
	if (m_state == DEAD)
	{
		return;
	}

	if (m_multipleFrames)
	{
		sf2d_draw_texture_part(m_sprite, m_x - offset, m_y, m_currentFrame*m_sizePerFrame, m_startingYOffset, m_sizePerFrame, m_sizeYPerFrame);
	}
	else
	{
		sf2d_draw_texture(m_sprite, m_x, m_y);
	}
}

bool Dinosaur::Update()
{
	if (m_state == DEAD)
	{
		return true;
	}

	m_age++;

	if ((m_male && m_age == AGETOBEADULTMALE) || (!m_male && m_age == AGETOBEADULTFEMALE)) 
	{
		m_stage = 2;
		m_sizePerFrame = 30;
		m_sizeYPerFrame = 36;
		m_y = 190;
		if (m_offsetFrameX == 0)
		{
			m_startingYOffset = 52;
		}
		else
		{
			m_startingYOffset = 88;
		}
		
	}
	
	if ((m_male && m_age == AGETOBEOLDMALE) || (!m_male && m_age == AGETOBEOLDFEMALE))
	{
		m_stage = 4;
		m_sizePerFrame = 35;
		m_sizeYPerFrame = 30;
		m_y = 196;
		if (m_offsetFrameX == 0)
		{
			m_startingYOffset = 124;
		}
		else
		{
			m_startingYOffset = 154; 
		}
	}
	if ((m_male && m_age >= AGETODIEMALE) || (!m_male && m_age >= AGETODIEFEMALE))
	{
		m_state = DEAD;
		return true;
	}

	if (m_moving)
	{
		m_secondaryCounter++;

		if (m_secondaryCounter > FRAMECOUNTERDINOSAUR)
		{
			m_currentFrame++;
			m_secondaryCounter = 0;
		}

		if (m_currentFrame >= m_numFrames)
		{
			m_currentFrame = 0;
		}

		m_moving = false;
	}
	else
	{
		if (m_currentFrame > 0)
		{
			m_secondaryCounter = 0;
			m_currentFrame = 0;
		}
	}

	return false;
}

void Dinosaur::moveToCoord(int x, u16 y)
{
	m_x = x;
	m_y = y;
}

///summary: Si n
void Dinosaur::move(int value, u16 dir)
{
	if (m_state == DEAD)
	{
		return;
	}

	m_moving = true;

	m_offsetFrameX = dir;

	if (dir == 0)
	{
		switch (m_stage)
		{
		case 0:
			m_startingYOffset = 0;
			break;
		case 2:
			m_startingYOffset = 52;
			break;
		case 4:
			m_startingYOffset = 124;
			break;
		default:
			break;
		}
		
	}
	else
	{
		switch (m_stage)
		{
		case 0:
			m_startingYOffset = 26;
			break;
		case 2:
			m_startingYOffset = 88;
			break;
		case 4:
			m_startingYOffset = 154;
			break;
		default:
			break;
		}
	}

	m_x += value*DINOSAURMOVEMENT;

	if (m_x > TOP_WIDTH)
	{
		m_x = 0;
	}

	if (m_x < 0-m_sizePerFrame)
	{
		m_x = TOP_WIDTH;
	}
}

sf2d_texture* Dinosaur::getSprite()
{
	return m_sprite;
}

void Dinosaur::setSprite(sf2d_texture* sprite)
{
	m_sprite = sprite;
}

void Dinosaur::setTotallyNewSprite(sf2d_texture* sprite, u16 numFrames, u16 sizePerFrame, u16 sizeYPerFrame)
{
	m_sprite = sprite;
	m_currentFrame = 0;
	m_numFrames = numFrames;
	m_sizePerFrame = sizePerFrame;
	m_sizeYPerFrame = sizeYPerFrame;
	m_stage = 0;
}

void Dinosaur::setX(int value)
{
	m_x = value;
}

int Dinosaur::getX()
{
	return m_x;
}

void Dinosaur::setY(u16 value)
{
	m_y = value;
}

u16 Dinosaur::getY()
{
	return m_y;
}

void Dinosaur::end()
{
	sf2d_free_texture(m_sprite);
	delete(this);
}

Dinosaur::MState Dinosaur::getState()
{
	return m_state;
}

void Dinosaur::setState(Dinosaur::MState value)
{
	m_state = value;
}

void Dinosaur::setInvincible(bool value)
{
	m_invincible = value;
}

bool Dinosaur::getInvincible()
{
	return m_invincible;
}

void Dinosaur::setNumFrames(u16 value)
{
	m_numFrames = value;
	m_currentFrame = 0;
}

u16 Dinosaur::getNumFrames()
{
	return m_numFrames;
}

void Dinosaur::setFrameSize(u16 value)
{
	m_sizePerFrame = value;
}

u16 Dinosaur::getFrameSize()
{
	return m_sizePerFrame;
}

void Dinosaur::reset()
{
	m_x = m_originalX;
	m_y = m_originalY;
	m_currentFrame = 0;
	m_secondaryCounter = 0;
	m_invincible = false;
	m_state = ALIVE;
	m_offsetFrameX = 0;
	m_moving = false;
	m_sizePerFrame = 20;
	m_sizeYPerFrame = 26;
	m_startingYOffset = 0;
	m_stage = 0;
}

u16 Dinosaur::getSizeYPerFrame()
{
	return m_sizeYPerFrame;
}

void Dinosaur::Die()
{
	m_state = DEAD;
}