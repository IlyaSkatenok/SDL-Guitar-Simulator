#include <iostream>
#include <SDL.h>
#include <SDL_mixer.h>
#include <SDL_image.h>
#include <clocale>
#include <string>
#include <fstream>

using namespace std;

int initialization();


void MainMenu(Mix_Chunk* chords_real[], Mix_Chunk* chords_boj[], SDL_Renderer* renderer,float* Width, float* Height, SDL_Window* window
	, int* VOLUME, int* FULL_SCREEN, int* MENU_AMBIENT, int* THEME, int* TYPE);

void PlayItAgain(Mix_Chunk* chords_real[], Mix_Chunk* chords_boj[],SDL_Renderer* renderer, SDL_Texture* texture, int *Isclosed, float Width, float Height, 
	Mix_Music* GuitarHero[], Mix_Music* GuitarReaction[], int TYPE);
//

void InitPlayAtlasPositions(SDL_Rect* PlayBlocks);

void InitMenuAtlasPositions(SDL_Rect* MenuBlocks);

void InitMenuTexturesPositions(SDL_Rect* MenuBlocksScreenPos, float Width, float Height);

void InitSettingAtlasPositions(SDL_Rect* Settings);

void InitSettingTexturesPositions(SDL_Rect* SettingsScreenPos, SDL_Rect* atlasScreen, float Width, float Height);

void InitPlayTexturesPositions(SDL_Rect* PlayBlocksScreenPos, float Width, float Height);

void GetInfoFromFile(int* WIDTH, int* HEIGHT, int* VOLUME, int* FULL_SCREEN, int* MENU_AMBIENT, int* THEME, int* TYPE);

void SaveInfoInFile(int WIDTH, int VOLUME, int FULL_SCREEN, int MENU_AMBIENT, int THEME, int TYPE);

void AcousticORClassic(int TYPE, Mix_Chunk* chords[]);