#include "SdlHeader.h"
#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:wmainCRTStartup")

int main(int argc, char* argv[])
{
    setlocale(LC_ALL, "Russian");
    initialization();
    SDL_Surface* Play_Surface = IMG_Load("./gamedata/textures/TTH.png");

    SDL_Window* window = NULL;
    SDL_Renderer* renderer = NULL;
    Mix_Chunk* chords[38] = {
        // 1 бвагЭР (0 - 13)
        Mix_LoadWAV("./gamedata/sounds/guitar/acoustic/f1.wav"),
        Mix_LoadWAV("./gamedata/sounds/guitar/acoustic/f2.wav"),
        Mix_LoadWAV("./gamedata/sounds/guitar/acoustic/f3.wav"),
        Mix_LoadWAV("./gamedata/sounds/guitar/acoustic/f4.wav"),
        Mix_LoadWAV("./gamedata/sounds/guitar/acoustic/f5.wav"),
        Mix_LoadWAV("./gamedata/sounds/guitar/acoustic/f6.wav"),
        Mix_LoadWAV("./gamedata/sounds/guitar/acoustic/f7.wav"),
        Mix_LoadWAV("./gamedata/sounds/guitar/acoustic/f8.wav"),
        Mix_LoadWAV("./gamedata/sounds/guitar/acoustic/f9.wav"),
        Mix_LoadWAV("./gamedata/sounds/guitar/acoustic/f10.wav"),
        Mix_LoadWAV("./gamedata/sounds/guitar/acoustic/f11.wav"),
        Mix_LoadWAV("./gamedata/sounds/guitar/acoustic/f12.wav"),
        Mix_LoadWAV("./gamedata/sounds/guitar/acoustic/f13.wav"),
        Mix_LoadWAV("./gamedata/sounds/guitar/acoustic/f14.wav"),
        //
        // 2 бђ№ѓэр (14 - 27) 14 - 18
        Mix_LoadWAV("./gamedata/sounds/guitar/acoustic/h1.wav"),
        Mix_LoadWAV("./gamedata/sounds/guitar/acoustic/h2.wav"),
        Mix_LoadWAV("./gamedata/sounds/guitar/acoustic/h3.wav"),
        Mix_LoadWAV("./gamedata/sounds/guitar/acoustic/h4.wav"),
        Mix_LoadWAV("./gamedata/sounds/guitar/acoustic/h5.wav"),
        // 
        // 3 бвагЭР (28 - 41) 19 - 22
        Mix_LoadWAV("./gamedata/sounds/guitar/acoustic/g1.wav"),
        Mix_LoadWAV("./gamedata/sounds/guitar/acoustic/g2.wav"),
        Mix_LoadWAV("./gamedata/sounds/guitar/acoustic/g3.wav"),
        Mix_LoadWAV("./gamedata/sounds/guitar/acoustic/g4.wav"),
        // 
        // 4 бвагЭР (42 - 55) 23 - 27
        Mix_LoadWAV("./gamedata/sounds/guitar/acoustic/d1.wav"),
        Mix_LoadWAV("./gamedata/sounds/guitar/acoustic/d2.wav"),
        Mix_LoadWAV("./gamedata/sounds/guitar/acoustic/d3.wav"),
        Mix_LoadWAV("./gamedata/sounds/guitar/acoustic/d4.wav"),
        Mix_LoadWAV("./gamedata/sounds/guitar/acoustic/d5.wav"),
        //
        // 5 бвагЭР (56 - 69) 28 - 32
        Mix_LoadWAV("./gamedata/sounds/guitar/acoustic/a1.wav"),
        Mix_LoadWAV("./gamedata/sounds/guitar/acoustic/a2.wav"),
        Mix_LoadWAV("./gamedata/sounds/guitar/acoustic/a3.wav"),
        Mix_LoadWAV("./gamedata/sounds/guitar/acoustic/a4.wav"),
        Mix_LoadWAV("./gamedata/sounds/guitar/acoustic/a5.wav"),

        // 
        // 6 бвагЭР(70 - 83) 33 - 37
        Mix_LoadWAV("./gamedata/sounds/guitar/acoustic/e1.wav"),
        Mix_LoadWAV("./gamedata/sounds/guitar/acoustic/e2.wav"),
        Mix_LoadWAV("./gamedata/sounds/guitar/acoustic/e3.wav"),
        Mix_LoadWAV("./gamedata/sounds/guitar/acoustic/e4.wav"),
        Mix_LoadWAV("./gamedata/sounds/guitar/acoustic/e5.wav"),
        // 
    };

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

    GetInfoFromFile(&WIDTH, &HEIGHT, &VOLUME, &FULL_SCREEN, &MENU_AMBIENT);

    Mix_Volume(-1, VOLUME);
    Mix_VolumeMusic(VOLUME);

    const char TITLE[] = "Guitar Simulator";
    
    window = SDL_CreateWindow(TITLE,15, 15, WIDTH, HEIGHT, SDL_WINDOW_SHOWN);
    
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_PRESENTVSYNC);

    SDL_Texture* Play_texture = SDL_CreateTextureFromSurface(renderer, Play_Surface);
    SDL_FreeSurface(Play_Surface);
    SDL_SetTextureBlendMode(Play_texture, SDL_BLENDMODE_BLEND);
//--------------------------------------------
    
    float pseudoWidth = WIDTH;
    float pseudoHeight = HEIGHT;

    SDL_SetRenderDrawColor(renderer, 165, 165, 165, 0);
    SDL_RenderClear(renderer);
    bool quit = false;

    MainMenu(chords, chords_real, chords_boj, renderer, Play_texture, &pseudoWidth, &pseudoHeight, window, &VOLUME, &FULL_SCREEN, &MENU_AMBIENT);
    SaveInfoInFile(int(pseudoWidth), VOLUME, FULL_SCREEN, MENU_AMBIENT);
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyTexture(Play_texture);
    IMG_Quit();
    Mix_Quit();
    SDL_Quit();
    return 0;
}
