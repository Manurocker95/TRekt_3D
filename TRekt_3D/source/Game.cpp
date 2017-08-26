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

#include "Game.h"

GameScreen::GameScreen(u32 score) : Scene ()
{
	m_maxScore = score;
	// we initialize data
	Start();
}

GameScreen::~GameScreen()
{
	delete m_bgTop;
	delete m_bgBot;
	m_players.clear();
	m_meteos.clear();
	delete m_bgm;
	delete font;
	delete font2;
	delete m_ground;
	delete m_playBtn;
	delete m_pauseBtn;
	delete m_resetBtn;
}

void GameScreen::Start()
{
	m_screen = TITLE;
	m_pause = false;
	m_playedBefore = false;
	m_gameStarted = false;
	m_timerToGame = 0;
	m_score = 0;

	// We will use 2 channels for sounds: 1 = BGM, 2= Sound effects so they can be played at same time. You can set as channels as you want.
	// We clear the channels
	ndspChnWaveBufClear(1);
	ndspChnWaveBufClear(2);

	// We load our font called font.ttf in data folder. Set the data folder in MakeFile
	font = sftd_load_font_mem(font_ttf, font_ttf_size);
	font2 = sftd_load_font_mem(font2_ttf, font2_ttf_size);

	// We initialize our game variables
	m_offset = 0;
	m_bgOffsetX = 0;
	m_bgCounter = 0;
	m_bgFrame = 0;

	m_dinoSM = sfil_load_PNG_file(IMG_DINO_SPRITE, SF2D_PLACE_RAM);
	m_dinoSF = sfil_load_PNG_file(IMG_DINO_SPRITE_F, SF2D_PLACE_RAM);
	m_meteoSp = sfil_load_PNG_file(IMG_METEO_SPRITE, SF2D_PLACE_RAM);

	u16 m_space = 0;

	m_players.reserve(MAXNUMBEROFDINOS);

	for (int i = 0; i < STARTINGDINOSAURS; i++)
	{
		u16 gender = rand() % 100;
		
		if (gender % 2 == 0)
		{
			m_players.push_back(new Dinosaur(100 + m_space, 200, m_dinoSM, true, 4, 20, 26, true));
		}
		else
		{
			m_players.push_back(new Dinosaur(100 + m_space, 200, m_dinoSF, true, 4, 20, 26, false));
		}

		m_space += PIXELSBETWEENDINOS;
	}

	u16 m_meteospace = 40;
	m_meteos.reserve(NUMOFMETEOS);
	for (int i = 0; i < NUMOFMETEOS; i++)
	{
		m_meteos.push_back(new Meteorite(m_meteospace*i, -50, m_meteoSp, true, 4, 40));
	}

	// We load our images and place them into RAM so they can be painted
	m_bgTop = sfil_load_PNG_file(IMG_BG_GAME_TOP, SF2D_PLACE_RAM);
	m_bgBot = sfil_load_PNG_file(IMG_BG_GAME_BOT, SF2D_PLACE_RAM);
	m_ground = sfil_load_PNG_file(IMG_GROUND, SF2D_PLACE_RAM);
	m_logo = sfil_load_PNG_file(IMG_LOGO, SF2D_PLACE_RAM);
	m_playBtn = sfil_load_PNG_file(IMG_BTN_PLAY, SF2D_PLACE_RAM);
	m_pauseBtn = sfil_load_PNG_file(IMG_BTN_PAUSE, SF2D_PLACE_RAM);
	m_resetBtn = sfil_load_PNG_file(IMG_BTN_RESET, SF2D_PLACE_RAM);

	// We load our sounds // PATH, CHANNEL, LOOP? -> // BGM plays with loop, but sfx just one time
	m_bgm = new sound(SND_BGM_GAME, 1, true);		

	// We play our bgm
	m_bgm->play();
}

void GameScreen::goToTitle()
{
	ResetGame();
	m_screen = TITLE;
}

void GameScreen::Draw()
{
	switch (m_screen)
	{
	case TITLE:
		// 3D effect. It works this way: GFX_RIGHT SHOULD BE X POSITION - OFFSET FOR S.3D OBJECTS and GFX_LEFT X POSITION, so it makes the effect for each eye
		sf2d_start_frame(GFX_TOP, GFX_LEFT);
		sf2d_draw_texture_part(m_bgTop, 0, 0, m_bgOffsetX*TOP_WIDTH, m_bgFrame*HEIGHT, TOP_WIDTH, HEIGHT);
		sf2d_draw_texture(m_ground, 0, HEIGHT - 14);

		sf2d_draw_texture(m_logo, 70, 100);
		sf2d_end_frame();

		// If we have activated 3D in Settings
		if (STEREOSCOPIC_3D_ACTIVATED)
		{
			// We check the offset by the slider
			m_offset = CONFIG_3D_SLIDERSTATE * MULTIPLIER_3D;
			sf2d_start_frame(GFX_TOP, GFX_RIGHT);
			sf2d_draw_texture_part(m_bgTop, 0, 0, m_bgOffsetX*TOP_WIDTH, m_bgFrame*HEIGHT, TOP_WIDTH, HEIGHT);
			sf2d_draw_texture(m_logo, 70 - m_offset, 100);
			sf2d_end_frame();
		}

		// Bottom screen (We just show an image)
		sf2d_start_frame(GFX_BOTTOM, GFX_LEFT);
		sf2d_draw_texture(m_bgBot, 0, 0);
		sf2d_draw_texture(m_playBtn, 115, 100);
		sftd_draw_text(font, 20, 5, C_BLACK, 20, "Best Score: ");
		sftd_draw_text(font, 105, 5, C_BLACK, 20, std::to_string(m_maxScore).c_str());
		sftd_draw_text(font, 20, HEIGHT - 30, C_BLACK, 15, "Manurocker95 (C) 2017");
		sftd_draw_text(font, 230, HEIGHT - 30, C_BLACK, 15, "VERSION: ");
		sftd_draw_text(font, 280, HEIGHT - 30, C_BLACK, 15, VERSION);

		if (DEBUGMODE)
			sftd_draw_text(font, 230, 8, C_BLACK, 15, "DEBUG MODE");

		sf2d_end_frame();
		break;

	case GAME:
		// 3D effect. It works this way: GFX_RIGHT SHOULD BE X POSITION - OFFSET FOR S.3D OBJECTS and GFX_LEFT X POSITION, so it makes the effect for each eye
		sf2d_start_frame(GFX_TOP, GFX_LEFT);
		sf2d_draw_texture_part(m_bgTop, 0, 0, m_bgOffsetX*TOP_WIDTH, m_bgFrame*HEIGHT, TOP_WIDTH, HEIGHT);
		
		// Vector of Dinos
		for (const auto& dino : m_players)
			dino->Draw(0);

		// Vector of Meteorites
		for (const auto& meteo : m_meteos)
			meteo->Draw(0);

		sf2d_draw_texture(m_ground, 0, HEIGHT - 14);

		if (m_gameStarted)
		{
			sftd_draw_text(font2, 30, 10, C_BLACK, 15, "Score:");
			sftd_draw_text(font2, 105, 10, C_BLACK, 15, std::to_string(m_score).c_str());
		}

		if (!m_pause)
		{
			if (!m_gameStarted)
			{
				if (m_timerToGame < m_timeToStartGame / 4)
				{
					sftd_draw_text(font2, 50, HEIGHT / 2 - 50, C_BLACK, 20, "Avoid the meteorites!");		// 3D Text - Cool... huh?
				}
				else if (m_timerToGame < m_timeToStartGame * 3 / 4)
				{
					sftd_draw_text(font2, 130, HEIGHT / 2 - 50, C_BLACK, 30, "Ready?");
				}
				else
				{
					sftd_draw_text(font2, 150, HEIGHT / 2 - 50, C_BLACK, 30, "Go!");
				}
			}
		}
		else
		{
			sf2d_draw_rectangle(0, 0, 420, 240, RGBA8(0x00, 0x00, 0x00, 125));
			sftd_draw_text(font2, 130, HEIGHT / 2 - 50, C_BLUE, 30, "PAUSE");
			sftd_draw_text(font, 100, HEIGHT / 2 - 10, C_BLUE, 15, "Tap or press Start to Resume the game");
		}

		sf2d_end_frame();

		// If we have activated 3D in Settings
		if (STEREOSCOPIC_3D_ACTIVATED)
		{
			// We check the offset by the slider
			m_offset = CONFIG_3D_SLIDERSTATE * MULTIPLIER_3D;
			
			// 3D Object
			sf2d_start_frame(GFX_TOP, GFX_RIGHT);
			sf2d_draw_texture_part(m_bgTop, 0, 0, m_bgOffsetX*TOP_WIDTH, m_bgFrame*HEIGHT, TOP_WIDTH, HEIGHT);
			// Vector of Dinosaurs
			for (const auto& dino : m_players)
				dino->Draw(m_offset);

			// Vector of Meteorites
			for (const auto& meteo : m_meteos)
				meteo->Draw(m_offset);

			sf2d_draw_texture(m_ground, 0 - m_offset, HEIGHT - 14);

			if (m_gameStarted)
			{
				sftd_draw_text(font2, 30 - m_offset, 10, C_BLACK, 15, "Score:");
				sftd_draw_text(font2, 105 - m_offset, 10, C_BLACK, 15, std::to_string(m_score).c_str());
			}

			if (!m_pause)
			{
				if (!m_gameStarted)
				{
					if (m_timerToGame < m_timeToStartGame / 4)
					{
						sftd_draw_text(font2, 50-m_offset, HEIGHT / 2 - 50, C_BLACK, 20, "Avoid the meteorites!");		// 3D Text - Cool... huh?
					}
					else if (m_timerToGame < m_timeToStartGame * 3 / 4)
					{
						sftd_draw_text(font2, 130 - m_offset, HEIGHT / 2 - 50, C_BLACK, 30, "Ready?");
					}
					else
					{
						sftd_draw_text(font2, 150 - m_offset, HEIGHT / 2 - 50, C_BLACK, 30, "Go!");
					}
				}
			}
			else
			{
				sf2d_draw_rectangle(0-m_offset, 0, 420, 240, RGBA8(0x00, 0x00, 0x00, 125));
				sftd_draw_text(font2, 130 - m_offset, HEIGHT / 2 - 50, C_BLUE, 30, "PAUSE");
				sftd_draw_text(font, 100-m_offset, HEIGHT / 2 - 10, C_BLUE, 15, "Tap or press Start to Resume the game");
			}

			sf2d_end_frame();
		}

		// Bottom screen 
		sf2d_start_frame(GFX_BOTTOM, GFX_LEFT);
		sf2d_draw_texture(m_bgBot, 0, 0);
		sf2d_draw_texture(m_pauseBtn, 70, 70);
		sf2d_draw_texture(m_resetBtn, 170, 70);
		sftd_draw_text(font, 20, 5, C_BLACK, 20, "Best Score: ");
		sftd_draw_text(font, 105, 5, C_BLACK, 20, std::to_string(m_maxScore).c_str());
		sftd_draw_text(font, 20, HEIGHT - 30, C_BLACK, 15, "Manurocker95 (C) 2017");
		sftd_draw_text(font, 230, HEIGHT - 30, C_BLACK, 15, "VERSION:");
		sftd_draw_text(font, 280, HEIGHT - 30, C_BLACK, 15, VERSION);

		if (DEBUGMODE)
			sftd_draw_text(font, 230, 8, C_BLACK, 15, "DEBUG MODE");

		if (m_pause)
		{
			sf2d_draw_rectangle(0, 0, 340, 240, RGBA8(0x00, 0x00, 0x00, 125));
			sftd_draw_text(font2, 90, HEIGHT / 2 - 50, C_BLUE, 30, "PAUSE");
			sftd_draw_text(font, 60, HEIGHT / 2 - 10, C_BLUE, 15, "Tap or press Start to Resume the game");
		}

		sf2d_end_frame();
		break;
	case END:
		// 3D effect. It works this way: GFX_RIGHT SHOULD BE X POSITION - OFFSET FOR S.3D OBJECTS and GFX_LEFT X POSITION, so it makes the effect for each eye
		sf2d_start_frame(GFX_TOP, GFX_LEFT);
		sf2d_draw_texture_part(m_bgTop, 0, 0, m_bgOffsetX*TOP_WIDTH, m_bgFrame*HEIGHT, TOP_WIDTH, HEIGHT);
		sf2d_draw_texture(m_ground, 0, HEIGHT - 14);
		sf2d_draw_rectangle(0, 0, 420, 240, RGBA8(0x00, 0x00, 0x00, 125));
		sftd_draw_text(font2, 100, HEIGHT / 2 - 50, C_WHITE, 20, "GAME OVER");
		sftd_draw_text(font, 100, HEIGHT / 2 - 10, C_WHITE, 15, "Tap or press any button to go to title");
		sf2d_end_frame();

		// If we have activated 3D in Settings
		if (STEREOSCOPIC_3D_ACTIVATED)
		{
			// We check the offset by the slider
			m_offset = CONFIG_3D_SLIDERSTATE * MULTIPLIER_3D;

			// 3D Object
			sf2d_start_frame(GFX_TOP, GFX_RIGHT);
			sf2d_draw_texture_part(m_bgTop, 0, 0, m_bgOffsetX*TOP_WIDTH, m_bgFrame*HEIGHT, TOP_WIDTH, HEIGHT);
			sf2d_draw_texture(m_ground, 0, HEIGHT - 14);
			sf2d_draw_rectangle(0, 0, 420, 240, RGBA8(0x00, 0x00, 0x00, 125));
			sftd_draw_text(font2, 100 - m_offset, HEIGHT / 2 - 50, C_WHITE, 20, "GAME OVER");
			sftd_draw_text(font, 100 - m_offset, HEIGHT / 2 - 10, C_WHITE, 15, "Tap or press any button to go to title");
			sf2d_end_frame();
		}


		// Bottom screen 
		sf2d_start_frame(GFX_BOTTOM, GFX_LEFT);
		sf2d_draw_texture(m_bgBot, 0, 0);
		sftd_draw_text(font, 20, 5, C_BLACK, 20, "Best Score: ");
		sftd_draw_text(font, 105, 5, C_BLACK, 20, std::to_string(m_maxScore).c_str());
		sftd_draw_text(font, 20, HEIGHT - 30, C_BLACK, 15, "Manurocker95 (C) 2017");
		sftd_draw_text(font, 230, HEIGHT - 30, C_BLACK, 15, "VERSION:");
		sftd_draw_text(font, 280, HEIGHT - 30, C_BLACK, 15, VERSION);

		if (DEBUGMODE)
			sftd_draw_text(font, 230, 8, C_BLACK, 15, "DEBUG MODE");

		sf2d_draw_rectangle(0, 0, 340, 240, RGBA8(0x00, 0x00, 0x00, 125));
		sftd_draw_text(font, 60, HEIGHT / 2 - 10, C_WHITE, 15, "Tap or press any button to go to title");

		sf2d_end_frame();
		break;
	}
}

void GameScreen::Update()
{
	if (!m_pause)
	{
		m_bgCounter++;

		if (m_bgCounter > BGFRAMECOUNTER)
		{
			m_bgFrame++;
			m_bgCounter = 0;
		}

		if (m_bgFrame >= 4)
		{
			m_bgFrame = 0;
			if (m_bgOffsetX == 0)
			{
				m_bgOffsetX = 1;
			}
			else
			{
				m_bgOffsetX = 0;
			}
		}

		if (m_screen == GAME)
		{
			if (!m_gameStarted)
			{
				m_timerToGame++;

				if (m_timerToGame >= m_timeToStartGame)
				{
					m_gameStarted = true;
					return;
				}
			}
			else
			{
				for (const auto & dino : m_players)
				{
					if (dino->getState() != Dinosaur::DEAD)
					{
						if (dino->Update())
						{
							dino->Die();
							m_numOfDeath++;
						}
						else
						{
							for (const auto & meteo : m_meteos)
							{
								if (meteo->CheckCollision(dino))
								{
									dino->Die();
									m_numOfDeath++;
									break;
								}
							}
						}
					}
				}

				if (m_numOfDeath >= STARTINGDINOSAURS)
				{
					EndGame();
					return;
				}
			}
		}
	}
}

void GameScreen::CheckInputs()
{
	hidScanInput();
	held = hidKeysHeld();
	
	switch (m_screen)
	{
	case TITLE:
		if (hidKeysDown() & KEY_TOUCH)
		{
			hidTouchRead(&touch);
			if ((touch.px > 115 && touch.px < 215) && (touch.py > 100 && touch.py < 140))
			{
				//Inicializamos el juego
				if (!m_playedBefore)
				{
					m_playedBefore = true;
					m_screen = GAME;
					return;
				}
				else
				{
					ResetGame();
					return;
				}
				
			}
		}

		if ((hidKeysDown() & KEY_A))
		{
			//Inicializamos el juego
			if (!m_playedBefore)
			{
				m_playedBefore = true;
				m_screen = GAME;
				return;
			}
			else
			{
				ResetGame();
				return;
			}
		}
		break;
	case GAME:

		if (m_pause)
		{
			if (hidKeysDown() & KEY_TOUCH)
			{
				m_pause = false;
			}
		}
		else
		{
			if (hidKeysDown() & KEY_TOUCH)
			{
				hidTouchRead(&touch);
				if ((touch.px > 70 && touch.px < 144) && (touch.py > 70 && touch.py < 144))
				{
					// PAUSE
					m_pause = true;
				}
				else if ((touch.px > 170 && touch.px < 244) && (touch.py > 70 && touch.py < 144))
				{
					// RESET
					ResetGame();
					return;
				}
			}

			if (m_gameStarted)
			{
				if (DEBUGMODE)
				{
					if (hidKeysDown() & KEY_DOWN)
					{
						for (const auto & meteo : m_meteos)
							meteo->Fall();
					}

					if ((hidKeysDown() & KEY_R))
						m_score += 10;

					if ((hidKeysDown() & KEY_L))
					{
						EndGame();
					}
				}

				if ((held & KEY_RIGHT))
				{
					for (const auto& dino : m_players)
						dino->move(1, 1);
				}

				if ((held & KEY_LEFT))
				{
					for (const auto& dino : m_players)
						dino->move(-1, 0);
				}
			}
		}

		if (hidKeysDown() & KEY_START)
		{
			m_pause = !m_pause;
		}

		break;

	case END:

		if (hidKeysDown())
		{
			goToTitle();
		}

		break;
	}
	
	// We Exit pressing Select
	if ((hidKeysDown() & KEY_SELECT))
	{
		//CheckScore();
		SceneManager::instance()->exitGame();
	}
}

void GameScreen::EndGame()
{
	m_screen = END;
	m_numOfDeath = 0;
	CheckScore();
}

void GameScreen::ResetGame()
{
	// We reset everything (Score and stuff)
	m_score = 0;
	m_timerToGame = 0;
	
	m_offset = 0;
	m_bgOffsetX = 0;
	m_bgCounter = 0;
	m_bgFrame = 0;

	m_gameStarted = false;
	m_pause = false;
	m_numOfDeath = 0;
	m_screen = GAME;

	// Vector of Dinosaurs
	for (const auto& dino : m_players)
		dino->reset();

	// Vector of Meteorites
	for (const auto& meteo : m_meteos)
		meteo->reset();
}

void GameScreen::CheckScore()
{
	if (m_score > m_maxScore)
	{
		m_maxScore = m_score;

		// We overwrite our .sav file with new data
		std::ofstream outfile(DATA_FILE);
		outfile << m_score;
		outfile.close();
	}
}

void GameScreen::DieByAge()
{
	m_numOfDeath++;
}