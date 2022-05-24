#include "SdlHeader.h"

void AcousticORClassic(int TYPE, Mix_Chunk* chords[])
{
    if (TYPE)
    {
        // 1 бвагЭР (0 - 13)
        chords[0] = Mix_LoadWAV("./gamedata/sounds/guitar/acoustic/f1.wav");
        chords[1] = Mix_LoadWAV("./gamedata/sounds/guitar/acoustic/f2.wav");
        chords[2] = Mix_LoadWAV("./gamedata/sounds/guitar/acoustic/f3.wav");
        chords[3] = Mix_LoadWAV("./gamedata/sounds/guitar/acoustic/f4.wav");
        chords[4] = Mix_LoadWAV("./gamedata/sounds/guitar/acoustic/f5.wav");
        chords[5] = Mix_LoadWAV("./gamedata/sounds/guitar/acoustic/f6.wav");
        chords[6] = Mix_LoadWAV("./gamedata/sounds/guitar/acoustic/f7.wav");
        chords[7] = Mix_LoadWAV("./gamedata/sounds/guitar/acoustic/f8.wav");
        chords[8] = Mix_LoadWAV("./gamedata/sounds/guitar/acoustic/f9.wav");
        chords[9] = Mix_LoadWAV("./gamedata/sounds/guitar/acoustic/f10.wav");
        chords[10] = Mix_LoadWAV("./gamedata/sounds/guitar/acoustic/f11.wav");
        chords[11] = Mix_LoadWAV("./gamedata/sounds/guitar/acoustic/f12.wav");
        chords[12] = Mix_LoadWAV("./gamedata/sounds/guitar/acoustic/f13.wav");
        chords[13] = Mix_LoadWAV("./gamedata/sounds/guitar/acoustic/f14.wav");
        // 2 бђ№ѓэр (14 - 27) 14 - 18
        chords[14] = Mix_LoadWAV("./gamedata/sounds/guitar/acoustic/h1.wav");
        chords[15] = Mix_LoadWAV("./gamedata/sounds/guitar/acoustic/h2.wav");
        chords[16] = Mix_LoadWAV("./gamedata/sounds/guitar/acoustic/h3.wav");
        chords[17] = Mix_LoadWAV("./gamedata/sounds/guitar/acoustic/h4.wav");
        chords[18] = Mix_LoadWAV("./gamedata/sounds/guitar/acoustic/h5.wav");
        // 3 бвагЭР (28 - 41) 19 - 22
        chords[19] = Mix_LoadWAV("./gamedata/sounds/guitar/acoustic/g1.wav");
        chords[20] = Mix_LoadWAV("./gamedata/sounds/guitar/acoustic/g2.wav");
        chords[21] = Mix_LoadWAV("./gamedata/sounds/guitar/acoustic/g3.wav");
        chords[22] = Mix_LoadWAV("./gamedata/sounds/guitar/acoustic/g4.wav");
        // 4 бвагЭР (42 - 55) 23 - 27
        chords[23] = Mix_LoadWAV("./gamedata/sounds/guitar/acoustic/d1.wav");
        chords[24] = Mix_LoadWAV("./gamedata/sounds/guitar/acoustic/d2.wav");
        chords[25] = Mix_LoadWAV("./gamedata/sounds/guitar/acoustic/d3.wav");
        chords[26] = Mix_LoadWAV("./gamedata/sounds/guitar/acoustic/d4.wav");
        chords[27] = Mix_LoadWAV("./gamedata/sounds/guitar/acoustic/d5.wav");
        // 5 бвагЭР (56 - 69) 28 - 32
        chords[28] = Mix_LoadWAV("./gamedata/sounds/guitar/acoustic/a1.wav");
        chords[29] = Mix_LoadWAV("./gamedata/sounds/guitar/acoustic/a2.wav");
        chords[30] = Mix_LoadWAV("./gamedata/sounds/guitar/acoustic/a3.wav");
        chords[31] = Mix_LoadWAV("./gamedata/sounds/guitar/acoustic/a4.wav");
        chords[32] = Mix_LoadWAV("./gamedata/sounds/guitar/acoustic/a5.wav");
        // 6 бвагЭР(70 - 83) 33 - 37
        chords[33] = Mix_LoadWAV("./gamedata/sounds/guitar/acoustic/e1.wav");
        chords[34] = Mix_LoadWAV("./gamedata/sounds/guitar/acoustic/e2.wav");
        chords[35] = Mix_LoadWAV("./gamedata/sounds/guitar/acoustic/e3.wav");
        chords[36] = Mix_LoadWAV("./gamedata/sounds/guitar/acoustic/e4.wav");
        chords[37] = Mix_LoadWAV("./gamedata/sounds/guitar/acoustic/e5.wav");
        //
    }
    else
    {
        // 1 бвагЭР (0 - 13)
        chords[0] = Mix_LoadWAV("./gamedata/sounds/guitar/classic/f1.wav");
        chords[1] = Mix_LoadWAV("./gamedata/sounds/guitar/classic/f2.wav");
        chords[2] = Mix_LoadWAV("./gamedata/sounds/guitar/classic/f3.wav");
        chords[3] = Mix_LoadWAV("./gamedata/sounds/guitar/classic/f4.wav");
        chords[4] = Mix_LoadWAV("./gamedata/sounds/guitar/classic/f5.wav");
        chords[5] = Mix_LoadWAV("./gamedata/sounds/guitar/classic/f6.wav");
        chords[6] = Mix_LoadWAV("./gamedata/sounds/guitar/classic/f7.wav");
        chords[7] = Mix_LoadWAV("./gamedata/sounds/guitar/classic/f8.wav");
        chords[8] = Mix_LoadWAV("./gamedata/sounds/guitar/classic/f9.wav");
        chords[9] = Mix_LoadWAV("./gamedata/sounds/guitar/classic/f10.wav");
        chords[10] = Mix_LoadWAV("./gamedata/sounds/guitar/classic/f11.wav");
        chords[11] = Mix_LoadWAV("./gamedata/sounds/guitar/classic/f12.wav");
        chords[12] = Mix_LoadWAV("./gamedata/sounds/guitar/classic/f13.wav");
        chords[13] = Mix_LoadWAV("./gamedata/sounds/guitar/classic/f14.wav");
        // 2 бђ№ѓэр (14 - 27) 14 - 18
        chords[14] = Mix_LoadWAV("./gamedata/sounds/guitar/classic/h1.wav");
        chords[15] = Mix_LoadWAV("./gamedata/sounds/guitar/classic/h2.wav");
        chords[16] = Mix_LoadWAV("./gamedata/sounds/guitar/classic/h3.wav");
        chords[17] = Mix_LoadWAV("./gamedata/sounds/guitar/classic/h4.wav");
        chords[18] = Mix_LoadWAV("./gamedata/sounds/guitar/classic/h5.wav");
        // 3 бвагЭР (28 - 41) 19 - 22
        chords[19] = Mix_LoadWAV("./gamedata/sounds/guitar/classic/g1.wav");
        chords[20] = Mix_LoadWAV("./gamedata/sounds/guitar/classic/g2.wav");
        chords[21] = Mix_LoadWAV("./gamedata/sounds/guitar/classic/g3.wav");
        chords[22] = Mix_LoadWAV("./gamedata/sounds/guitar/classic/g4.wav");
        // 4 бвагЭР (42 - 55) 23 - 27
        chords[23] = Mix_LoadWAV("./gamedata/sounds/guitar/classic/d1.wav");
        chords[24] = Mix_LoadWAV("./gamedata/sounds/guitar/classic/d2.wav");
        chords[25] = Mix_LoadWAV("./gamedata/sounds/guitar/classic/d3.wav");
        chords[26] = Mix_LoadWAV("./gamedata/sounds/guitar/classic/d4.wav");
        chords[27] = Mix_LoadWAV("./gamedata/sounds/guitar/classic/d5.wav");
        // 5 бвагЭР (56 - 69) 28 - 32
        chords[28] = Mix_LoadWAV("./gamedata/sounds/guitar/classic/a1.wav");
        chords[29] = Mix_LoadWAV("./gamedata/sounds/guitar/classic/a2.wav");
        chords[30] = Mix_LoadWAV("./gamedata/sounds/guitar/classic/a3.wav");
        chords[31] = Mix_LoadWAV("./gamedata/sounds/guitar/classic/a4.wav");
        chords[32] = Mix_LoadWAV("./gamedata/sounds/guitar/classic/a5.wav");
        // 6 бвагЭР(70 - 83) 33 - 37
        chords[33] = Mix_LoadWAV("./gamedata/sounds/guitar/classic/e1.wav");
        chords[34] = Mix_LoadWAV("./gamedata/sounds/guitar/classic/e2.wav");
        chords[35] = Mix_LoadWAV("./gamedata/sounds/guitar/classic/e3.wav");
        chords[36] = Mix_LoadWAV("./gamedata/sounds/guitar/classic/e4.wav");
        chords[37] = Mix_LoadWAV("./gamedata/sounds/guitar/classic/e5.wav");
        //
    }
}