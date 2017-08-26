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

#pragma once
#ifndef _GAME_SCREEN_H_
#define _GAME_SCREEN_H_

#include "sfil.h"
#include "sftd.h"
#include "sf2d.h"

#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */
#include <iostream>
#include <stdio.h>
#include <vector>  //for std::vector
#include <string>  //for std::string

#include "font_ttf.h"			//Don't worry if it seems to have error here
#include "font2_ttf.h"
#include "Dino.h"
#include "Meteorite.h"
#include "button.h"
#include "sound.h"
#include "scene.h"
#include "SceneManager.h"

#include "Settings.h"

using std::vector;

class GameScreen : public Scene
{

public:

	enum SCREEN {TITLE, GAME, END};

	GameScreen(u32 score);				// Constructor
	~GameScreen();						// Destructor
	void Start() override;				// initialize
	void Draw() override;				// Draw
	void CheckInputs() override;		// CheckInput
	void Update() override;				// Update
	void CheckScore();
	void ResetGame();
	void DieByAge();
	void goToTitle();
	void EndGame();
	void procreate();
private:

	SCREEN m_screen;
	bool m_gameStarted;
	bool m_pause;						// Boolean for pausing the game
	bool m_meteosFalling;
	bool m_playedBefore;
	float m_offset;						// Offset for 3D
	u8 m_bgFrame, m_bgCounter;	
	u8 m_bgOffsetX;						// Offset for the background
	u32 m_score, m_maxScore;			// Current score and Max Score
	u32 m_timeToStartGame = 200;		// Max time for starting game
	u32 m_timerToGame;					// Count down timer to start game (Ready? Go!)
	u32 m_timeToFall = 150;		// Max time for starting game
	u32 m_timerToFall;					// Count down timer to start game (Ready? Go!)
	u32 held;
	touchPosition touch;
	sf2d_texture *m_bgTop,*m_bgBot;		// Background images
	sf2d_texture * m_ground, *m_logo;	// Ground - TRekt Logo
	sf2d_texture * m_playBtn;			// Play Button 
	sf2d_texture * m_pauseBtn;			// Pause Button
	sf2d_texture * m_resetBtn;			// Reset Button
	sf2d_texture * m_dinoSM, *m_dinoSF;	// Dino textures
	sf2d_texture * m_meteoSp;			// Meteo Textures
	std::vector <Dinosaur*> m_players;	// Vector of Dinosaurs
	std::vector <Meteorite*> m_meteos;  // Vector of meteorites
	sound * m_bgm;						// Sounds
	sftd_font* font, *font2;			// Main Font, Font 2
	u32 m_meteoPositions [10];
	bool m_numOfDeath;
	bool m_maleAlive, m_femaleAlive;
	int dinosAlive;
};

#endif