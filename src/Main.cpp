#include "SdlHeader.h"
#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:wmainCRTStartup")

int main(int argc, char* argv[])
{
    setlocale(LC_ALL, "Russian");
    initialization();
    SDL_Window* window = NULL;
    SDL_Renderer* renderer = NULL;
    Mix_Chunk* chords_real[42] = {
        // C D E F G A B
        Mix_LoadWAV("./gamedata/sounds/chords/C.wav"),               // 0
        Mix_LoadWAV("./gamedata/sounds/chords/Cm.wav"),
        Mix_LoadWAV("./gamedata/sounds/chords/C7.wav"),
            Mix_LoadWAV("./gamedata/sounds/chords/C_perebor.wav"),
            Mix_LoadWAV("./gamedata/sounds/chords/Cm_perebor.wav"),
            Mix_LoadWAV("./gamedata/sounds/chords/C7_perebor.wav"),
        Mix_LoadWAV("./gamedata/sounds/chords/D.wav"),              // 6
        Mix_LoadWAV("./gamedata/sounds/chords/Dm.wav"),
        Mix_LoadWAV("./gamedata/sounds/chords/D7.wav"),
            Mix_LoadWAV("./gamedata/sounds/chords/D_perebor.wav"),
            Mix_LoadWAV("./gamedata/sounds/chords/Dm_perebor.wav"),
            Mix_LoadWAV("./gamedata/sounds/chords/D7_perebor.wav"),
        Mix_LoadWAV("./gamedata/sounds/chords/E.wav"),               // 12
        Mix_LoadWAV("./gamedata/sounds/chords/Em.wav"),
        Mix_LoadWAV("./gamedata/sounds/chords/E7.wav"),
            Mix_LoadWAV("./gamedata/sounds/chords/E_perebor.wav"),
            Mix_LoadWAV("./gamedata/sounds/chords/Em_perebor.wav"),
            Mix_LoadWAV("./gamedata/sounds/chords/E7_perebor.wav"),
        Mix_LoadWAV("./gamedata/sounds/chords/F.wav"),              // 18
        Mix_LoadWAV("./gamedata/sounds/chords/Fm.wav"),
        Mix_LoadWAV("./gamedata/sounds/chords/F7.wav"),
            Mix_LoadWAV("./gamedata/sounds/chords/F_perebor.wav"),
            Mix_LoadWAV("./gamedata/sounds/chords/Fm_perebor.wav"),
            Mix_LoadWAV("./gamedata/sounds/chords/F7_perebor.wav"),
        Mix_LoadWAV("./gamedata/sounds/chords/G.wav"),              // 24
        Mix_LoadWAV("./gamedata/sounds/chords/Gm.wav"),
        Mix_LoadWAV("./gamedata/sounds/chords/G7.wav"),
            Mix_LoadWAV("./gamedata/sounds/chords/G_perebor.wav"),
            Mix_LoadWAV("./gamedata/sounds/chords/Gm_perebor.wav"),
            Mix_LoadWAV("./gamedata/sounds/chords/G7_perebor.wav"),
        Mix_LoadWAV("./gamedata/sounds/chords/A.wav"),             // 30
        Mix_LoadWAV("./gamedata/sounds/chords/Am.wav"),
        Mix_LoadWAV("./gamedata/sounds/chords/A7.wav"),
            Mix_LoadWAV("./gamedata/sounds/chords/A_perebor.wav"),
            Mix_LoadWAV("./gamedata/sounds/chords/Am_perebor.wav"),
            Mix_LoadWAV("./gamedata/sounds/chords/A7_perebor.wav"),
        Mix_LoadWAV("./gamedata/sounds/chords/B.wav"),             // 36
        Mix_LoadWAV("./gamedata/sounds/chords/Bm.wav"),
        Mix_LoadWAV("./gamedata/sounds/chords/B7.wav"),
            Mix_LoadWAV("./gamedata/sounds/chords/B_perebor.wav"),
            Mix_LoadWAV("./gamedata/sounds/chords/Bm_perebor.wav"),
            Mix_LoadWAV("./gamedata/sounds/chords/B7_perebor.wav"),
    };

    Mix_Chunk* chords_boj[16] = {
        Mix_LoadWAV("./gamedata/sounds/chords/boj/C_boj6.wav"),             // 0
            Mix_LoadWAV("./gamedata/sounds/chords/boj/C_boj6_gluh.wav"),
        Mix_LoadWAV("./gamedata/sounds/chords/boj/C_boj8.wav"),
            Mix_LoadWAV("./gamedata/sounds/chords/boj/C_boj8_gluh.wav"),
        Mix_LoadWAV("./gamedata/sounds/chords/boj/D_boj6.wav"),             // 4
            Mix_LoadWAV("./gamedata/sounds/chords/boj/D_boj6_gluh.wav"),
        Mix_LoadWAV("./gamedata/sounds/chords/boj/D_boj8.wav"),
            Mix_LoadWAV("./gamedata/sounds/chords/boj/D_boj8_gluh.wav"),
        Mix_LoadWAV("./gamedata/sounds/chords/boj/Em_boj6.wav"),            // 8
            Mix_LoadWAV("./gamedata/sounds/chords/boj/Em_boj6_gluh.wav"),
        Mix_LoadWAV("./gamedata/sounds/chords/boj/Em_boj8.wav"),
            Mix_LoadWAV("./gamedata/sounds/chords/boj/Em_boj8_gluh.wav"),
        Mix_LoadWAV("./gamedata/sounds/chords/boj/G_boj6.wav"),             // 12
            Mix_LoadWAV("./gamedata/sounds/chords/boj/G_boj6_gluh.wav"),
        Mix_LoadWAV("./gamedata/sounds/chords/boj/G_boj8.wav"),
            Mix_LoadWAV("./gamedata/sounds/chords/boj/G_boj8_gluh.wav"),
    };

    int WIDTH = 25;
    int HEIGHT = 45;
    int VOLUME;
    int FULL_SCREEN;
    int MENU_AMBIENT;
    int THEME;
    int TYPE;

    GetInfoFromFile(&WIDTH, &HEIGHT, &VOLUME, &FULL_SCREEN, &MENU_AMBIENT, &THEME, &TYPE);

    Mix_Volume(-1, VOLUME);
    Mix_VolumeMusic(VOLUME);

    const char TITLE[] = "Guitar Simulator";
    
    window = SDL_CreateWindow(TITLE,15, 15, WIDTH, HEIGHT, SDL_WINDOW_SHOWN);
    
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_PRESENTVSYNC);
//--------------------------------------------
    
    float pseudoWidth = WIDTH;
    float pseudoHeight = HEIGHT;

    SDL_SetRenderDrawColor(renderer, 165, 165, 165, 0);
    SDL_RenderClear(renderer);
    bool quit = false;

    MainMenu(chords_real, chords_boj, renderer, &pseudoWidth, &pseudoHeight, window, &VOLUME, &FULL_SCREEN, &MENU_AMBIENT, &THEME, &TYPE);
    SaveInfoInFile(int(pseudoWidth), VOLUME, FULL_SCREEN, MENU_AMBIENT, THEME, TYPE);
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    IMG_Quit();
    Mix_Quit();
    SDL_Quit();
    return 0;
}
