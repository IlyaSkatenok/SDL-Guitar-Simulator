#include "SdlHeader.h"

void VolumeLevelChanger(int PositionX, int PositionTextureX, int TextureW, int* positionXVolume, int* CurrentVolume)
{
	// сколько занимает единица звук
	float leveler = float(TextureW) / 128;

	PositionX =  PositionX - PositionTextureX;
	*positionXVolume = PositionTextureX + TextureW;
	*positionXVolume -=  128 * leveler - PositionX ;
	*CurrentVolume = float(PositionX) / leveler;
	Mix_Volume(-1, *CurrentVolume);
	Mix_VolumeMusic(*CurrentVolume);
}

void MenuSettings(SDL_Renderer* renderer, SDL_Texture* texture, SDL_Rect* atlas, SDL_Rect* atlasScreen, int *Isclosed, SDL_Window* window, float* Width, float* Height, SDL_Rect* MenuBlocksScreenPos, 
	int *MusicStopped, Mix_Music* MenuAmbient[], int* IsFull, int* CurrentVolume)
{
	SDL_Surface* Guitar_Surface = IMG_Load("./gamedata/textures/Guitar_BackGround3.png");
	SDL_Texture* Guitar_Texture = SDL_CreateTextureFromSurface(renderer, Guitar_Surface);
	SDL_FreeSurface(Guitar_Surface);
	SDL_SetTextureBlendMode(Guitar_Texture, SDL_BLENDMODE_BLEND);
	bool quit = false;
	int MouseClicked = 0;
	int MenuChoosen = 6;

	SDL_Rect Settings[7];
	
	InitSettingAtlasPositions(Settings);

	SDL_Rect SettingsScreenPos[9];
	
	InitSettingTexturesPositions(SettingsScreenPos, atlasScreen, *Width, *Height);

	while (!quit)
	{
		SDL_Event event;

		if (!Mix_PlayingMusic() && *MusicStopped)
		{
			Mix_PlayMusic(MenuAmbient[rand() % 7], 0);
		}

		for (int i = 4; i < 7; i++)
		{
			SDL_RenderCopy(renderer, texture, &atlas[i], &atlasScreen[i]);
		}
		SDL_RenderCopy(renderer, texture, &atlas[10], &atlasScreen[7]);
		SDL_RenderCopy(renderer, texture, &Settings[0], &SettingsScreenPos[0]);
		// Элипс громкости
		SettingsScreenPos[6].x = 1250 * (*Width / 1920) + *CurrentVolume * (4.544 * *Width / 1920);
		if (*Width / 1920 < 0.8)
		{
			SettingsScreenPos[6].x -= 15;
		}
		SDL_RenderCopy(renderer, texture, &Settings[6], &SettingsScreenPos[6]);
		if (*MusicStopped)
		{
			SDL_RenderCopy(renderer, texture, &Settings[5], &SettingsScreenPos[7]);
		}
		if (*IsFull)
		{
			SDL_RenderCopy(renderer, texture, &Settings[5], &SettingsScreenPos[8]);
		}

		while (SDL_PollEvent(&event))
		{
			if (event.type == SDL_QUIT)
			{
				quit = true;
				*Isclosed = 1;
			}
			if (event.button.button == SDL_BUTTON_LEFT && event.type == SDL_MOUSEBUTTONDOWN)
			{
				MouseClicked = 1;
			}
			if (event.type == SDL_MOUSEBUTTONUP)
			{
				if (MouseClicked)
				{
					for (int i = 0; i < 9; i++)
					{
						if ((event.button.x >= SettingsScreenPos[i].x) && (event.button.x <= SettingsScreenPos[i].x + SettingsScreenPos[i].w)
							&& (event.button.y >= SettingsScreenPos[i].y) && (event.button.y <= SettingsScreenPos[i].y + SettingsScreenPos[i].h))
						{
							switch (i)
							{
							case 0:
								quit = true;
								break;
							case 1:
								*Width = 1920;
								*Height = 1080;
								SDL_SetWindowSize(window, 1920, 1080);
								InitMenuTexturesPositions(MenuBlocksScreenPos, *Width, *Height);
								InitSettingTexturesPositions(SettingsScreenPos, atlasScreen, *Width, *Height);
								break;
							case 2:
								*Width = 1600;
								*Height = 900;
								SDL_SetWindowSize(window, 1600, 900);
								InitMenuTexturesPositions(MenuBlocksScreenPos, *Width, *Height);
								InitSettingTexturesPositions(SettingsScreenPos, atlasScreen, *Width, *Height);
								break;
							case 3:
								*Width = 1366;
								*Height = 768;
								SDL_SetWindowSize(window, 1366, 768);
								InitMenuTexturesPositions(MenuBlocksScreenPos, *Width, *Height);
								InitSettingTexturesPositions(SettingsScreenPos, atlasScreen, *Width, *Height);
								break;
							case 4:
								*Width = 1024;
								*Height = 768;
								SDL_SetWindowSize(window, 1024, 768);
								InitMenuTexturesPositions(MenuBlocksScreenPos, *Width, *Height);
								InitSettingTexturesPositions(SettingsScreenPos, atlasScreen, *Width, *Height);
								break;
							case 5:
								VolumeLevelChanger(event.button.x, SettingsScreenPos[5].x, SettingsScreenPos[5].w, &SettingsScreenPos[6].x, CurrentVolume);
								break;
							case 7:
								*MusicStopped = !*MusicStopped;
								if (!*MusicStopped)
								{
									Mix_HaltMusic();
								}
								break;
							case 8:
								*IsFull = !*IsFull;
								if (*IsFull)
								{
									SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN);
								}
								else
								{
									SDL_SetWindowFullscreen(window, 0);
								}
								break;
							}
						}

					}
				}
				MouseClicked = 0;
			}
			if (event.type == SDL_MOUSEMOTION)
			{
				for (int i = 1; i < 5; i++)
				{
					if ((event.button.x >= SettingsScreenPos[i].x) && (event.button.x <= SettingsScreenPos[i].x + SettingsScreenPos[i].w)
						&& (event.button.y >= SettingsScreenPos[i].y) && (event.button.y <= SettingsScreenPos[i].y + SettingsScreenPos[i].h))
					{
						MenuChoosen = i;
						break;
					}
					if (i == 3)
					{
						MenuChoosen = 6;
					}
				}
			}
		}
		SettingsScreenPos[MenuChoosen].x -= 9;
		SettingsScreenPos[MenuChoosen].y -= 9;
		SettingsScreenPos[MenuChoosen].w += 15;
		SettingsScreenPos[MenuChoosen].h += 15;
		switch (MenuChoosen)
		{
		case 1:
			SDL_RenderCopy(renderer, texture, &atlas[9], &SettingsScreenPos[1]);
			break;
		case 2:
			SDL_RenderCopy(renderer, texture, &atlas[9], &SettingsScreenPos[2]);
			break;
		case 3:
			SDL_RenderCopy(renderer, texture, &atlas[9], &SettingsScreenPos[3]);
			break;
		case 4:
			SDL_RenderCopy(renderer, texture, &atlas[9], &SettingsScreenPos[4]);
			break;
		}
		SettingsScreenPos[MenuChoosen].x += 9;
		SettingsScreenPos[MenuChoosen].y += 9;
		SettingsScreenPos[MenuChoosen].w -= 15;
		SettingsScreenPos[MenuChoosen].h -= 15;
		SDL_RenderCopy(renderer, Guitar_Texture, NULL, NULL);
		SDL_RenderPresent(renderer);
		SDL_RenderClear(renderer);
	}
	SDL_DestroyTexture(Guitar_Texture);
}


void MenuHelper(SDL_Renderer* renderer, float* Width, float* Height, int* Isclosed, int MusicStopped, Mix_Music* MenuAmbient[])
{
	SDL_Surface* Help_Surface = IMG_Load("./gamedata/textures/Helper.png");
	SDL_Texture* Help_Texture = SDL_CreateTextureFromSurface(renderer, Help_Surface);
	SDL_FreeSurface(Help_Surface);

	SDL_Rect Exit_Pos = {1763 * (*Width / 1920), 1 * (*Height / 1080), 156 * (*Width / 1920), 45 * (*Height / 1080) };

	int MouseClicked = 0;
	bool quit = false;

	while (!quit)
	{
		SDL_Event event;
		if (!Mix_PlayingMusic() && MusicStopped)
		{
			Mix_PlayMusic(MenuAmbient[rand() % 7], 0);
		}
		SDL_RenderCopy(renderer, Help_Texture, NULL, NULL);
		while (SDL_PollEvent(&event))
		{
			if (event.type == SDL_QUIT)
			{
				quit = true;
				*Isclosed = 1;
			}
			if (event.button.button == SDL_BUTTON_LEFT && event.type == SDL_MOUSEBUTTONDOWN)
			{
				MouseClicked = 1;
			}
			if (event.type == SDL_MOUSEBUTTONUP)
			{
				if (MouseClicked)
				{
					if ((event.button.x >= Exit_Pos.x) && (event.button.x <= Exit_Pos.x + Exit_Pos.w)
						&& (event.button.y >= Exit_Pos.y) && (event.button.y <= Exit_Pos.y + Exit_Pos.h))
					{
						quit = true;
					}
				}
				MouseClicked = 0;
			}
		}
		SDL_RenderPresent(renderer);
		SDL_RenderClear(renderer);
	}

	SDL_DestroyTexture(Help_Texture);
}


void MainMenu(Mix_Chunk* chords[], Mix_Chunk* chords_real[], Mix_Chunk* chords_boj[], SDL_Renderer* renderer, SDL_Texture* textureGame,float *Width, float *Height, SDL_Window* window
	, int* VOLUME, int* FULL_SCREEN, int* MENU_AMBIENT)
{

	SDL_Surface* Cursor_Surface = IMG_Load("./gamedata/textures/Cursor.png");
	SDL_Cursor* cursor = SDL_CreateColorCursor(Cursor_Surface, 0, 0);
	SDL_SetCursor(cursor);

	Mix_Music* MenuAmbient[7] =
	{
		Mix_LoadMUS("./gamedata/sounds/ambient/guitar_1.ogg"),
		Mix_LoadMUS("./gamedata/sounds/ambient/guitar_2.ogg"),
		Mix_LoadMUS("./gamedata/sounds/ambient/guitar_3.ogg"),
		Mix_LoadMUS("./gamedata/sounds/ambient/guitar_4.ogg"),
		Mix_LoadMUS("./gamedata/sounds/ambient/guitar_5.ogg"),
		Mix_LoadMUS("./gamedata/sounds/ambient/guitar_6.ogg"),
		Mix_LoadMUS("./gamedata/sounds/ambient/guitar_7.ogg")
	};

	Mix_Music* GuitarHero[6] =
	{
		Mix_LoadMUS("./gamedata/sounds/guitar_hero/track_1.ogg"),
		Mix_LoadMUS("./gamedata/sounds/guitar_hero/track_2.ogg"),
		Mix_LoadMUS("./gamedata/sounds/guitar_hero/track_3.ogg"),
		Mix_LoadMUS("./gamedata/sounds/guitar_hero/track_4.ogg"),
		Mix_LoadMUS("./gamedata/sounds/guitar_hero/track_5.ogg"),
		Mix_LoadMUS("./gamedata/sounds/guitar_hero/track_6.ogg")
	};

	Mix_Music* GuitarHeroReaction[3] =
	{
		Mix_LoadMUS("./gamedata/sounds/guitar_hero/reaction/reaction_music_1.ogg"),
		Mix_LoadMUS("./gamedata/sounds/guitar_hero/reaction/reaction_music_2.ogg"),
		Mix_LoadMUS("./gamedata/sounds/guitar_hero/reaction/reaction_music_3.ogg")
	};

	srand(time(NULL));

	// Подключается из файла настроек
	if (*MENU_AMBIENT)
	{
		Mix_PlayMusic(MenuAmbient[rand() % 7], 0);
	}

	if (*FULL_SCREEN)
	{
		SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN);
	}
	//

	SDL_Surface* Menu_Surface = IMG_Load("./gamedata/textures/Menu.png");
	SDL_Texture* Menu_Texture = SDL_CreateTextureFromSurface(renderer, Menu_Surface);
	SDL_FreeSurface(Menu_Surface);
	SDL_SetTextureBlendMode(Menu_Texture, SDL_BLENDMODE_BLEND);

	SDL_Rect MenuBlocks[11];

	InitMenuAtlasPositions(MenuBlocks);

	SDL_Rect MenuBlocksScreenPos[8];

	InitMenuTexturesPositions(MenuBlocksScreenPos,*Width, *Height);


	SDL_Surface* Guitar_Surface = IMG_Load("./gamedata/textures/Guitar_BackGround2.png");
	SDL_Texture* Guitar_Texture = SDL_CreateTextureFromSurface(renderer, Guitar_Surface);
	SDL_FreeSurface(Guitar_Surface);
	SDL_SetTextureBlendMode(Guitar_Texture, SDL_BLENDMODE_BLEND);

	SDL_Surface* LOGO_Surface = IMG_Load("./gamedata/textures/LOGO2.png");
	SDL_Texture* LOGO_Texture = SDL_CreateTextureFromSurface(renderer, LOGO_Surface);
	SDL_FreeSurface(LOGO_Surface);
	SDL_SetTextureBlendMode(LOGO_Texture, SDL_BLENDMODE_BLEND);
	SDL_Rect LOGO_RECT = { MenuBlocksScreenPos[0].x - 220 * (*Width / 1920),MenuBlocksScreenPos[3].y - 200 * (*Height / 1080),983 * (*Width / 1920), 200 * (*Height / 1080)};


	int Menu = 0;
	int MouseClicked = 0;
	int WindowClosed = 0;

	int MenuChoosen = 5;

	bool quit = false;
	while (!quit)
	{
		SDL_RenderCopy(renderer, Guitar_Texture, NULL, NULL);
		SDL_RenderCopy(renderer, LOGO_Texture, NULL, &LOGO_RECT);
		if (!Mix_PlayingMusic() && *MENU_AMBIENT)
		{
			Mix_PlayMusic(MenuAmbient[rand() % 7], 0);
		}
		
		SDL_Event event;
		for (int i = 0; i < 4; i++)
		{
			SDL_RenderCopy(renderer, Menu_Texture, &MenuBlocks[i], &MenuBlocksScreenPos[i]);
		}
		while (SDL_PollEvent(&event))
		{
			if (event.type == SDL_QUIT)
			{
				quit = true;
			}
            if (event.button.button == SDL_BUTTON_LEFT && event.type == SDL_MOUSEBUTTONDOWN)
            {
				MouseClicked = 1;
            }
			if (event.type == SDL_MOUSEBUTTONUP)
			{
				if (MouseClicked) 
				{
					for (int i = 0; i < 4; i++)
					{
						if ((event.button.x >= MenuBlocksScreenPos[i].x) && (event.button.x <= MenuBlocksScreenPos[i].x + MenuBlocksScreenPos[i].w)
							&& (event.button.y >= MenuBlocksScreenPos[i].y) && (event.button.y <= MenuBlocksScreenPos[i].y + MenuBlocksScreenPos[i].h))
						{
							switch (i)
							{
							case 0:
								quit = true;
								SDL_DestroyTexture(Menu_Texture);
								break;
							case 1:
								SDL_RenderClear(renderer);
								MenuSettings(renderer, Menu_Texture, MenuBlocks, MenuBlocksScreenPos,  &WindowClosed, window, Width, Height, MenuBlocksScreenPos, 
									MENU_AMBIENT, MenuAmbient, FULL_SCREEN, VOLUME);
								if (WindowClosed)
								{
									quit = true;
								}
								LOGO_RECT = { int(MenuBlocksScreenPos[0].x - 220 * (*Width / 1920)),int(MenuBlocksScreenPos[3].y - 200 * (*Height / 1080)), int(983 * (*Width / 1920)), int(200 * (*Height / 1080)) };
								i = 5;
								break;
							case 2:
								SDL_RenderClear(renderer);
								Mix_HaltMusic();
								PlayItAgain(chords, chords_real, chords_boj, renderer, textureGame,  &WindowClosed, *Width, *Height, GuitarHero, GuitarHeroReaction);
								if (WindowClosed)
								{
									quit = true;
								}
								Mix_HaltMusic();
								break;
							case 3:
								SDL_RenderClear(renderer);
								MenuHelper(renderer, Width,Height, &WindowClosed, *MENU_AMBIENT, MenuAmbient);
								if (WindowClosed)
								{
									quit = true;
								}
								break;
							}
						}
					}
				}
				MouseClicked = 0;
			}
			if (event.type == SDL_MOUSEMOTION)
			{
				for (int i = 0; i < 4; i++)
				{
					if ((event.button.x >= MenuBlocksScreenPos[i].x) && (event.button.x <= MenuBlocksScreenPos[i].x + MenuBlocksScreenPos[i].w)
						&& (event.button.y >= MenuBlocksScreenPos[i].y) && (event.button.y <= MenuBlocksScreenPos[i].y + MenuBlocksScreenPos[i].h))
					{
						MenuChoosen = i;
						break;
					}
					if (i == 3)
					{
						MenuChoosen = 5;
					}
				}
			}

		}
		MenuBlocksScreenPos[MenuChoosen].x -= 5;
		MenuBlocksScreenPos[MenuChoosen].y -= 5;
		MenuBlocksScreenPos[MenuChoosen].w += 10;
		MenuBlocksScreenPos[MenuChoosen].h += 10;
		switch (MenuChoosen)
		{
		case 0:
			SDL_RenderCopy(renderer, Menu_Texture, &MenuBlocks[9], &MenuBlocksScreenPos[0]);
			break;
		case 1:
			SDL_RenderCopy(renderer, Menu_Texture, &MenuBlocks[9], &MenuBlocksScreenPos[1]);
			break;
		case 2:
			SDL_RenderCopy(renderer, Menu_Texture, &MenuBlocks[9], &MenuBlocksScreenPos[2]);
			break;
		case 3:
			SDL_RenderCopy(renderer, Menu_Texture, &MenuBlocks[9], &MenuBlocksScreenPos[3]);
			break;
		}
		MenuBlocksScreenPos[MenuChoosen].x += 5;
		MenuBlocksScreenPos[MenuChoosen].y += 5;
		MenuBlocksScreenPos[MenuChoosen].w -= 10;
		MenuBlocksScreenPos[MenuChoosen].h -= 10;

		SDL_RenderPresent(renderer);
		SDL_RenderClear(renderer);
	}

	SDL_DestroyTexture(Menu_Texture);
	SDL_DestroyTexture(LOGO_Texture);
	SDL_DestroyTexture(Guitar_Texture);
}