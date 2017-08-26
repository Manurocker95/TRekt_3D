#include "Meteorite.h"

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

// Default Constructor
Meteorite::Meteorite()
{

}

// Constructor with all stuff needed
Meteorite::Meteorite(int x, int y, sf2d_texture * sprite, bool multipleFrames, u16 numFrames, u16 sizePerFrame)
{
	m_x = x;
	m_originalX = x;
	m_originalY = y;
	m_y = y;
	m_sprite = sprite;
	m_multipleFrames = multipleFrames;
	m_numFrames = numFrames;
	m_sizePerFrame = sizePerFrame;
	m_currentFrame = 0;
	m_secondaryCounter = 0;
	m_invincible = false;
	m_state = STAY;
	m_offsetFrameX = 0;
	m_moving = false;
}

Meteorite::~Meteorite()
{
	delete m_sprite;
}

void Meteorite::Draw(float offset)
{
	if (!m_moving)
	{
		return;
	}

	if (m_multipleFrames)
	{
		sf2d_draw_texture_part(m_sprite, m_x - offset, m_y, m_currentFrame*m_sizePerFrame, 0, m_sizePerFrame, 47);
	}
	else
	{
		sf2d_draw_texture(m_sprite, m_x, m_y);
	}
}

void Meteorite::Update()
{
	if (!m_moving)
	{
		return;
	}
	else
	{
		m_secondaryCounter++;

		if (m_secondaryCounter > FRAMECOUNTERMETEO)
		{
			m_currentFrame++;
			m_secondaryCounter = 0;
		}

		if (m_currentFrame >= m_numFrames)
		{
			m_currentFrame = 0;
		}

		if (m_y > HEIGHT + 47)
		{
			m_y = -50;
			m_moving = false;
			m_state = STAY;
		}
		else
		{
			m_y += METEORITEMOVEMENT;
		}
	}
}

void Meteorite::moveToCoord(int x, int y)
{
	m_x = x;
	m_y = y;
}

///summary: Si n
void Meteorite::Fall()
{
	if (!m_moving)
	{
		m_moving = true;
		m_state = FALL;
	}
}

sf2d_texture* Meteorite::getSprite()
{
	return m_sprite;
}

void Meteorite::setSprite(sf2d_texture* sprite)
{
	m_sprite = sprite;
}

void Meteorite::setDeadSprite(sf2d_texture* sprite, u16 numFrames, u16 sizePerFrame)
{
	m_sprite = sprite;
	m_currentFrame = 0;
	m_numFrames = numFrames;
	m_sizePerFrame = sizePerFrame;
}

void Meteorite::setX(int value)
{
	m_x = value;
}

int Meteorite::getX()
{
	return m_x;
}

void Meteorite::setY(int value)
{
	m_y = value;
}

int Meteorite::getY()
{
	return m_y;
}

void Meteorite::end()
{
	sf2d_free_texture(m_sprite);
	delete(this);
}

Meteorite::MState Meteorite::getState()
{
	return m_state;
}

void Meteorite::setState(Meteorite::MState value)
{
	m_state = value;
}

void Meteorite::setInvincible(bool value)
{
	m_invincible = value;
}

bool Meteorite::getInvincible()
{
	return m_invincible;
}

void Meteorite::setNumFrames(u16 value)
{
	m_numFrames = value;
	m_currentFrame = 0;
}

u16 Meteorite::getNumFrames()
{
	return m_numFrames;
}

void Meteorite::setFrameSize(u16 value)
{
	m_sizePerFrame = value;
}

u16 Meteorite::getFrameSize()
{
	return m_sizePerFrame;
}

void Meteorite::reset()
{
	m_x = m_originalX;
	m_y = m_originalY;
	m_currentFrame = 0;
	m_secondaryCounter = 0;
	m_invincible = false;
	m_state = STAY;
	m_offsetFrameX = 0;
	m_moving = false;
}

bool Meteorite::CheckCollision(Dinosaur* d)
{
	return (m_x < d->getX() + d->getFrameSize() && m_x + m_sizePerFrame > d->getX() && m_y < d->getY() + d->getSizeYPerFrame() && m_y + 47 > d->getY());
}