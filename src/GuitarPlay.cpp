#include "SdlHeader.h"



Uint16 audioFormat;
int audioFrequency, audioChannelCount, audioAllocatedMixChannelsCount;

static inline Uint16 formatSampleSize(Uint16 format) { return (format & 0xFF) / 8; }


static int computeChunkLengthMillisec(int chunkSize)
{
    const Uint32 points = chunkSize / formatSampleSize(audioFormat);
    const Uint32 frames = (points / audioChannelCount);
    return ((frames * 1000) / audioFrequency);
}

template<typename AudioFormatType>
struct PlaybackSpeedEffectHandler
{
    const AudioFormatType* const chunkData;
    const float& speedFactor;
    float position; 
    const int duration;
    const int chunkSize;
    const bool loop;
    const bool attemptSelfHalting;
    bool altered;

    PlaybackSpeedEffectHandler(const Mix_Chunk& chunk, const float& speed, bool loop, bool trySelfHalt)
        : chunkData(reinterpret_cast<AudioFormatType*>(chunk.abuf)), speedFactor(speed),
        position(0), duration(computeChunkLengthMillisec(chunk.alen)),
        chunkSize(chunk.alen / formatSampleSize(audioFormat)),
        loop(loop), attemptSelfHalting(trySelfHalt), altered(false)
    {}

    
    void modifyStreamPlaybackSpeed(int mixChannel, void* stream, int length)
    {
        AudioFormatType* buffer = static_cast<AudioFormatType*>(stream);
        const int bufferSize = length / sizeof(AudioFormatType);
        const float speedFactor = this->speedFactor;

        if (position < duration || loop)
        {
            const float delta = 1000.0 / audioFrequency,
                vdelta = delta * speedFactor;

            if (!altered && speedFactor != 1.0f)
                altered = true;

            if (altered)
            {
                for (int i = 0; i < bufferSize; i += audioChannelCount)
                {
                    const int j = i / audioChannelCount;
                    const float x = position + j * vdelta;
                    const int k = floor(x / delta);
                    const float prop = (x / delta) - k;

                    for (int c = 0; c < audioChannelCount; c++)
                    {
                        if (k * audioChannelCount + audioChannelCount - 1 < chunkSize || loop)
                        {
                            AudioFormatType v0 = chunkData[(k * audioChannelCount + c) % chunkSize],
                                v1 = chunkData[((k + 1) * audioChannelCount + c) % chunkSize];
                            buffer[i + c] = v0 + prop * (v1 - v0);  // linear interpolation (single multiplication)
                        }
                        else
                        {
                            buffer[i + c] = 0;
                        }
                    }
                }
            }

            position += (bufferSize / audioChannelCount) * vdelta;

            if (loop) while (position > duration)
                position -= duration;
        }
        else
        {
            for (int i = 0; i < bufferSize; i++)
                buffer[i] = 0;

            if (attemptSelfHalting)
                Mix_HaltChannel(mixChannel);
        }
    }

    
    static void mixEffectFuncCallback(int channel, void* stream, int length, void* userData)
    {
        static_cast<PlaybackSpeedEffectHandler*>(userData)->modifyStreamPlaybackSpeed(channel, stream, length);
    }

    
    static void mixEffectDoneCallback(int, void* userData)
    {
        delete static_cast<PlaybackSpeedEffectHandler*>(userData);
    }

    
    static void registerEffect(int channel, const Mix_Chunk& chunk, const float& speed, bool loop, bool trySelfHalt)
    {
        Mix_RegisterEffect(channel, mixEffectFuncCallback, mixEffectDoneCallback, new PlaybackSpeedEffectHandler(chunk, speed, loop, trySelfHalt));
    }
};


void setupPlaybackSpeedEffect(const Mix_Chunk* const chunk, const float& speed, int channel, bool loop = false, bool trySelfHalt = false)
{
    switch (audioFormat)
    {
    case AUDIO_U8:  PlaybackSpeedEffectHandler<Uint8 >::registerEffect(channel, *chunk, speed, loop, trySelfHalt); break;
    case AUDIO_S8:  PlaybackSpeedEffectHandler<Sint8 >::registerEffect(channel, *chunk, speed, loop, trySelfHalt); break;
    case AUDIO_U16: PlaybackSpeedEffectHandler<Uint16>::registerEffect(channel, *chunk, speed, loop, trySelfHalt); break;
    default:
    case AUDIO_S16: PlaybackSpeedEffectHandler<Sint16>::registerEffect(channel, *chunk, speed, loop, trySelfHalt); break;
    case AUDIO_S32: PlaybackSpeedEffectHandler<Sint32>::registerEffect(channel, *chunk, speed, loop, trySelfHalt); break;
    case AUDIO_F32: PlaybackSpeedEffectHandler<float >::registerEffect(channel, *chunk, speed, loop, trySelfHalt); break;
    }
}

void ShowPlayingAccord(SDL_Renderer* renderer, SDL_Texture* texture, SDL_Rect* atlas, int x, int y, float Width, float Height)
{
    SDL_Rect showAccord;
    if (Width <= 1 && Width > 0.8)
    {
        showAccord = { int(Width != 1 ? x * Width + 5 * Width : x) , int(Height != 1 ? y * Height + 5 * Height : y) , int(105 * Width), int(105 * Height) };
    }
    else
    {
        showAccord = { int(x * Width + 10 * Width) , int( y * Height + 10 * Height) , int(105 * Width), int(105 * Height) };
    }
    SDL_RenderCopy(renderer, texture, &atlas[8], &showAccord);
    SDL_RenderPresent(renderer);
    SDL_Delay(50);
}

void ShowPlayingString(SDL_Renderer* renderer, SDL_Texture* texture, SDL_Rect* atlas, int x, int y, float Width, float Height)
{
    SDL_Rect showString = { x * Width , y * Height ,59 * Width, 59 * Height };
    SDL_RenderCopy(renderer, texture, &atlas[10], &showString);
    SDL_RenderPresent(renderer);
    SDL_Delay(25);
}
//------------------------------------------------------------------------------------------------------------------------------------------------

// Where is my mind
void PlaySong1(SDL_Renderer* renderer, SDL_Texture* texture, SDL_Rect* atlas, float Width, float Height, int* CurrentNote, int* PressedNote, int* PlaySong, Mix_Music* GoodJob, int *Wrong)
{
    short tmpNote = *CurrentNote;
    SDL_Rect showString;
    switch (*CurrentNote)
    {
    case 1: case 3: case 5: case 7: case 14: case 15: case 17: case 19: case 21: case 23: case 25: case 27: case 34: case 35: case 37: case 39:
        showString = { int(810 * Width) , int(768 * Height) ,int(69 * Width), int(69 * Height) };
        SDL_RenderCopy(renderer, texture, &atlas[10], &showString);
        if(*PressedNote == SDL_SCANCODE_T)
            (*CurrentNote)++;
        break;
    case 2: case 4: case 6: case 8: case 10: case 12: case 22: case 24: case 26: case 28: case 30: case 32:
        showString = { int(690 * Width) , int(708 * Height) ,int(69 * Width), int(69 * Height) };
        SDL_RenderCopy(renderer, texture, &atlas[10], &showString);
        if(*PressedNote == SDL_SCANCODE_4)
            (*CurrentNote)++;
        break;
    case 9: case 11: case 13: case 16: case 18: case 20: case 29: case 31: case 33: case 36: case 38: case 40:
        showString = { int(690 * Width) , int(768 * Height) ,int(69 * Width), int(69 * Height) };
        SDL_RenderCopy(renderer, texture, &atlas[10], &showString);
        if(*PressedNote == SDL_SCANCODE_R)
            (*CurrentNote)++;
        break;
    case 41:
        *PlaySong = 0;
        Mix_PlayMusic(GoodJob, 1);
        break;
    }
    if (tmpNote == *CurrentNote && *PressedNote && *PressedNote != SDL_SCANCODE_X)
    {
        (*Wrong)--;
    }
    *PressedNote = 0;
}

// Лирика
void PlaySong2(SDL_Renderer* renderer, SDL_Texture* texture, SDL_Rect* atlas, float Width, float Height, int* CurrentNote, int* PressedNote, int* PlaySong, Mix_Music* GoodJob, int* Wrong)
{
    short tmpNote = *CurrentNote;
    SDL_Rect showString;
    switch (*CurrentNote)
    {
        //(3)
    case 1: case 10: case 11: case 18: case 20:
        showString = { int(560 * Width) , int(768 * Height) ,int(69 * Width), int(69 * Height) };
        SDL_RenderCopy(renderer, texture, &atlas[10], &showString);
        if(*PressedNote == SDL_SCANCODE_E)
            (*CurrentNote)++;
        break;
        // 1
    case 2: case 8: case 9: case 21: case 27: case 28: case 29: case 30: case 31: case 32: case 33:
        showString = { int(310 * Width) , int(708 * Height) ,int(69 * Width), int(69 * Height) };
        SDL_RenderCopy(renderer, texture, &atlas[10], &showString);
        if(*PressedNote == SDL_SCANCODE_1)
            (*CurrentNote)++;
        break;
        // 3
    case 3: case 4: case 5: case 6: case 7: case 22: case 23: case 24: case 25: case 26: case 38:
        showString = { int(560 * Width) , int(708 * Height) ,int(69 * Width), int(69 * Height) };
        SDL_RenderCopy(renderer, texture, &atlas[10], &showString);
        if(*PressedNote == SDL_SCANCODE_3)
            (*CurrentNote)++;
        break;
        // 6
    case 34: case 36:
        showString = { int(940 * Width) , int(708 * Height) ,int(69 * Width), int(69 * Height) };
        SDL_RenderCopy(renderer, texture, &atlas[10], &showString);
        if(*PressedNote == SDL_SCANCODE_6)
            (*CurrentNote)++;
        break;
        // 8
    case 35:
        showString = { int(1190 * Width) , int(708 * Height) ,int(69 * Width), int(69 * Height) };
        SDL_RenderCopy(renderer, texture, &atlas[10], &showString);
        if(*PressedNote == SDL_SCANCODE_8)
            (*CurrentNote)++;
        break;
        // 5
    case 37:
        showString = { int(810 * Width) , int(708 * Height) ,int(69 * Width), int(69 * Height) };
        SDL_RenderCopy(renderer, texture, &atlas[10], &showString);
        if(*PressedNote == SDL_SCANCODE_5)
            (*CurrentNote)++;
        break;
        // (4)
    case 12: case 13: case 14: case 15: case 17:
        showString = { int(690 * Width) , int(768 * Height) ,int(69 * Width), int(69 * Height) };
        SDL_RenderCopy(renderer, texture, &atlas[10], &showString);
        if(*PressedNote == SDL_SCANCODE_R)
            (*CurrentNote)++;
        break;
        // (6)
    case 16:
        showString = { int(940 * Width) , int(768 * Height) ,int(69 * Width), int(69 * Height) };
        SDL_RenderCopy(renderer, texture, &atlas[10], &showString);
        if(*PressedNote == SDL_SCANCODE_Y)
            (*CurrentNote)++;
        break;
        // [0]
    case 19:
        showString = { int(190 * Width) , int(828 * Height) ,int(69 * Width), int(69 * Height) };
        SDL_RenderCopy(renderer, texture, &atlas[10], &showString);
        if(*PressedNote == SDL_SCANCODE_A)
            (*CurrentNote)++;
        break;
    case 39:
        *PlaySong = 0;
        Mix_PlayMusic(GoodJob, 1);
        break;
    }
    if (tmpNote == *CurrentNote && *PressedNote && *PressedNote != SDL_SCANCODE_X)
    {
        (*Wrong)--;
    }
    *PressedNote = 0;
}

// Всё идет по плану
void PlaySong3(SDL_Renderer* renderer, SDL_Texture* texture, SDL_Rect* atlas, float Width, float Height, int* CurrentNote, int* PressedNote, int* PlaySong, Mix_Music* GoodJob, int* Wrong)
{
    short tmpNote = *CurrentNote;
    SDL_Rect showString;
    switch (*CurrentNote)
    {
        //(3)
    case 1: case 10: case 13: case 22: case 23: case 24: case 25: case 35: case 39: case 47: case 51: 
        showString = { int(560 * Width) , int(768 * Height) ,int(69 * Width), int(69 * Height) };
        SDL_RenderCopy(renderer, texture, &atlas[10], &showString);
        if(*PressedNote == SDL_SCANCODE_E)
            (*CurrentNote)++;
        break;
        // (1)
    case 2: case 3: case 14: case 15: case 26: case 27: case 28: case 40: case 41: case 42: case 52: case 53: case 54:
        showString = { int(310 * Width) , int(768 * Height) ,int(69 * Width), int(69 * Height) };
        SDL_RenderCopy(renderer, texture, &atlas[10], &showString);
        if(*PressedNote == SDL_SCANCODE_Q)
            (*CurrentNote)++;
        break;
        // 3
    case 7: case 19: case 32: case 46: case 59:
        showString = { int(560 * Width) , int(708 * Height) ,int(69 * Width), int(69 * Height) };
        SDL_RenderCopy(renderer, texture, &atlas[10], &showString);
        if(*PressedNote == SDL_SCANCODE_3)
            (*CurrentNote)++;
        break;
        // 1
    case 8: case 20: case 33:
        showString = { int(310 * Width) , int(708 * Height) ,int(69 * Width), int(69 * Height) };
        SDL_RenderCopy(renderer, texture, &atlas[10], &showString);
        if(*PressedNote == SDL_SCANCODE_1)
            (*CurrentNote)++;
        break;
        // 8
    case 60: case 62: case 68:
        showString = { int(1190 * Width) , int(708 * Height) ,int(69 * Width), int(69 * Height) };
        SDL_RenderCopy(renderer, texture, &atlas[10], &showString);
        if(*PressedNote == SDL_SCANCODE_8)
            (*CurrentNote)++;
        break;
        // 5
    case 4: case 5: case 6: case 16: case 17: case 18: case 29: case 30: case 31: case 43: case 44: case 45: case 55: case 56: case 57: case 58: case 64: case 66: case 70: case 72:
        showString = { int(810 * Width) , int(708 * Height) ,int(69 * Width), int(69 * Height) };
        SDL_RenderCopy(renderer, texture, &atlas[10], &showString);
        if(*PressedNote == SDL_SCANCODE_5)
            (*CurrentNote)++;
        break;
        // 7
    case 61: case 63: case 67: case 69:
        showString = { int(1070 * Width) , int(708 * Height) ,int(69 * Width), int(69 * Height) };
        SDL_RenderCopy(renderer, texture, &atlas[10], &showString);
        if(*PressedNote == SDL_SCANCODE_7)
            (*CurrentNote)++;
        break;
        // 0
    case 9: case 11: case 21: case 34: case 36: case 37: case 38: case 48: case 49: case 50: case 65: case 71:
        showString = { int(190 * Width) , int(708 * Height) ,int(69 * Width), int(69 * Height) };
        SDL_RenderCopy(renderer, texture, &atlas[10], &showString);
        if(*PressedNote == SDL_SCANCODE_GRAVE)
            (*CurrentNote)++;
        break;
    // (5)
    case 12:
        showString = { int(810 * Width) , int(768 * Height) ,int(69 * Width), int(69 * Height) };
        SDL_RenderCopy(renderer, texture, &atlas[10], &showString);
        if(*PressedNote == SDL_SCANCODE_T)
            (*CurrentNote)++;
        break;
    case 73:
        *PlaySong = 0;
        Mix_PlayMusic(GoodJob, 1);
        break;
    }
    if (tmpNote == *CurrentNote && *PressedNote && *PressedNote != SDL_SCANCODE_X)
    {
        (*Wrong)--;
    }
    *PressedNote = 0;
}

// Здравствуй небо в облаках
void PlaySong4(SDL_Renderer* renderer, SDL_Texture* texture, SDL_Rect* atlas, float Width, float Height, int* CurrentNote, int* PressedNote, int* PlaySong, Mix_Music* GoodJob, int* Wrong)
{
    short tmpNote = *CurrentNote;
    SDL_Rect showString;
    switch (*CurrentNote)
    {
        // (7)
    case 1: case 2: case 3: case 8: case 9: case 10: case 15: case 16: case 17: case 22: case 23: case 24: case 40: case 42: case 46: case 50: case 54: case 58: case 62: case 66:
        showString = { int(1070 * Width) , int(768 * Height) ,int(69 * Width), int(69 * Height) };
        SDL_RenderCopy(renderer, texture, &atlas[10], &showString);
        if(*PressedNote == SDL_SCANCODE_U)
            (*CurrentNote)++;
        break;
        // (5)
    case 4: case 6: case 11: case 13: case 18: case 20: case 25: case 27: case 30: case 34: case 35: case 41:
        showString = { int(810 * Width) , int(768 * Height) ,int(69 * Width), int(69 * Height) };
        SDL_RenderCopy(renderer, texture, &atlas[10], &showString);
        if(*PressedNote == SDL_SCANCODE_T)
            (*CurrentNote)++;
        break;
        // (4)
    case 5: case 12: case 19: case 26: case 39:
        showString = { int(690 * Width) , int(768 * Height) ,int(69 * Width), int(69 * Height) };
        SDL_RenderCopy(renderer, texture, &atlas[10], &showString);
        if(*PressedNote == SDL_SCANCODE_R)
            (*CurrentNote)++;
        break;
        // (2)
    case 7: case 21: case 31: case 33: case 36:
        showString = { int(440 * Width) , int(768 * Height) ,int(69 * Width), int(69 * Height) };
        SDL_RenderCopy(renderer, texture, &atlas[10], &showString);
        if(*PressedNote == SDL_SCANCODE_W)
            (*CurrentNote)++;
        break;
        // (9)
    case 14: case 28: case 29: case 43: case 45: case 47: case 49: case 51: case 53: case 55: case 57: case 59: case 61: case 63: case 65:
        showString = { int(1320 * Width) , int(768 * Height) ,int(69 * Width), int(69 * Height) };
        SDL_RenderCopy(renderer, texture, &atlas[10], &showString);
        if(*PressedNote == SDL_SCANCODE_O)
            (*CurrentNote)++;
        break;
        // (10)
    case 44: case 48: case 52: case 56: case 60: case 64:
        showString = { int(1440 * Width) , int(768 * Height) ,int(69 * Width), int(69 * Height) };
        SDL_RenderCopy(renderer, texture, &atlas[10], &showString);
        if(*PressedNote == SDL_SCANCODE_P)
            (*CurrentNote)++;
        break;
        // (0)
    case 38: 
        showString = { int(190 * Width) , int(768 * Height) ,int(69 * Width), int(69 * Height) };
        SDL_RenderCopy(renderer, texture, &atlas[10], &showString);
        if(*PressedNote == SDL_SCANCODE_TAB)
            (*CurrentNote)++;
        break;
        // [3]
    case 32: case 37:
        showString = { int(560 * Width) , int(828 * Height) ,int(69 * Width), int(69 * Height) };
        SDL_RenderCopy(renderer, texture, &atlas[10], &showString);
        if(*PressedNote == SDL_SCANCODE_F)
            (*CurrentNote)++;
        break;
    case 67:
        *PlaySong = 0;
        Mix_PlayMusic(GoodJob, 1);
        break;
    }
    if (tmpNote == *CurrentNote && *PressedNote && *PressedNote != SDL_SCANCODE_X)
    {
        (*Wrong)--;
    }
    *PressedNote = 0;
}

// Кукла колдуна
void PlaySong5(SDL_Renderer* renderer, SDL_Texture* texture, SDL_Rect* atlas, float Width, float Height, int* CurrentNote, int* PressedNote, int* PlaySong, Mix_Music* GoodJob, int* Wrong)
{
    short tmpNote = *CurrentNote;
    SDL_Rect showString;
    switch (*CurrentNote)
    {
    case 12: case 17: case 31: case 36:
        showString = { int(940 * Width) , int(708 * Height) ,int(69 * Width), int(69 * Height) };
        SDL_RenderCopy(renderer, texture, &atlas[10], &showString);
        if(*PressedNote == SDL_SCANCODE_6)
            (*CurrentNote)++;
        break;
    case 13: case 14: case 18: case 32: case 33: case 37:
        showString = { int(810 * Width) , int(708 * Height) ,int(69 * Width), int(69 * Height) };
        SDL_RenderCopy(renderer, texture, &atlas[10], &showString);
        if(*PressedNote == SDL_SCANCODE_5)
            (*CurrentNote)++;
        break;
    case 4: case 5: case 8: case 9: case 23: case 24: case 27: case 28:
        showString = { int(560 * Width) , int(768 * Height) ,int(69 * Width), int(69 * Height) };
        SDL_RenderCopy(renderer, texture, &atlas[10], &showString);
        if(*PressedNote == SDL_SCANCODE_E)
            (*CurrentNote)++;
        break;
    case 2: case 3: case 7: case 10: case 19: case 21: case 22: case 26: case 29: case 38:
        showString = { int(810 * Width) , int(768 * Height) ,int(69 * Width), int(69 * Height) };
        SDL_RenderCopy(renderer, texture, &atlas[10], &showString);
        if(*PressedNote == SDL_SCANCODE_T)
            (*CurrentNote)++;
        break;
    case 1: case 6: case 11: case 15: case 16: case 20: case 25: case 30: case 34: case 35:
        showString = { int(940 * Width) , int(768 * Height) ,int(69 * Width), int(69 * Height) };
        SDL_RenderCopy(renderer, texture, &atlas[10], &showString);
        if(*PressedNote == SDL_SCANCODE_Y)
            (*CurrentNote)++;
        break;
    case 39:
        *PlaySong = 0;
        Mix_PlayMusic(GoodJob, 1);
        break;
    }
    if (tmpNote == *CurrentNote && *PressedNote && *PressedNote != SDL_SCANCODE_X)
    {
        (*Wrong)--;
    }
    *PressedNote = 0;
}

// Спокойная ночь
void PlaySong6(SDL_Renderer* renderer, SDL_Texture* texture, SDL_Rect* atlas, float Width, float Height, int* CurrentNote, int* PressedNote, int* PlaySong, Mix_Music* GoodJob, int* Wrong)
{
    short tmpNote = *CurrentNote;
    SDL_Rect showString;
    switch (*CurrentNote)
    {
    case 1: case 2: case 3: case 27: case 28: case 29:
        showString = { int(190 * Width) , int(768 * Height) ,int(69 * Width), int(69 * Height) };
        SDL_RenderCopy(renderer, texture, &atlas[10], &showString);
        if(*PressedNote == SDL_SCANCODE_TAB)
            (*CurrentNote)++;
        break;
    case 4: case 5: case 6: case 7: case 8: case 10: case 12: case 13: case 14: case 24: case 26: case 30: case 31: case 32: case 33: case 34: case 36: case 38: case 39: case 40: case 50: case 52:
        showString = { int(560 * Width) , int(768 * Height) ,int(69 * Width), int(69 * Height) };
        SDL_RenderCopy(renderer, texture, &atlas[10], &showString);
        if( *PressedNote == SDL_SCANCODE_E)
            (*CurrentNote)++;
        break;
    case 15: case 16: case 17: case 25: case 41: case 42: case 43: case 51:
        showString = { int(440 * Width) , int(768 * Height) ,int(69 * Width), int(69 * Height) };
        SDL_RenderCopy(renderer, texture, &atlas[10], &showString);
        if(*PressedNote == SDL_SCANCODE_W)
            (*CurrentNote)++;
        break; 
    case 21: case 23: case 47: case 49:
        showString = { int(940 * Width) , int(768 * Height) ,int(69 * Width), int(69 * Height) };
        SDL_RenderCopy(renderer, texture, &atlas[10], &showString);
        if(*PressedNote == SDL_SCANCODE_Y)
            (*CurrentNote)++;
        break;
    case 9: case 11: case 35: case 37:
        showString = { int(810 * Width) , int(708 * Height) ,int(69 * Width), int(69 * Height) };
        SDL_RenderCopy(renderer, texture, &atlas[10], &showString);
        if (*PressedNote == SDL_SCANCODE_5) 
            (*CurrentNote)++;
        break;
    case 18: case 19: case 20: case 22: case 44: case 45: case 46: case 48:
        showString = { int(190 * Width) , int(708 * Height) ,int(69 * Width), int(69 * Height) };
        SDL_RenderCopy(renderer, texture, &atlas[10], &showString);
        if (*PressedNote == SDL_SCANCODE_GRAVE)
                (*CurrentNote)++;
        break;
    case 53:
        *PlaySong = 0;
        Mix_PlayMusic(GoodJob, 1);
        break;
    }
    if (tmpNote == *CurrentNote && *PressedNote && *PressedNote != SDL_SCANCODE_X)
    {
        (*Wrong)--;
    }
    *PressedNote = 0;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------

// РАБОТАЕМ
void PlayItAgain(Mix_Chunk* chords_real[], Mix_Chunk* chords_boj[], SDL_Renderer* renderer, SDL_Texture* texture, int *Isclosed, float Width, float Height
    , Mix_Music* GuitarHero[], Mix_Music* GuitarReaction[], int TYPE)
{
    Mix_Chunk* chords[38];
    AcousticORClassic(TYPE, chords);
    SDL_Surface* help_Surface = IMG_Load("./gamedata/textures/help_player.png");
    SDL_Texture* help_Texture = SDL_CreateTextureFromSurface(renderer, help_Surface);
    SDL_FreeSurface(help_Surface);
    SDL_SetTextureAlphaMod(help_Texture, 150);

    SDL_Surface* play_it_Surface = IMG_Load("./gamedata/textures/play_it.png");
    SDL_Texture* play_it_Texture = SDL_CreateTextureFromSurface(renderer, play_it_Surface);
    SDL_FreeSurface(play_it_Surface);


    SDL_Rect PlayBlocks[17];
    int PlaySong = 0;
    int CurrentNote = 0;
    int PressedNote = 0;
    int WrongNote = 3;
    InitPlayAtlasPositions(PlayBlocks);

    SDL_Rect PlayBlocksScreenPos[12];

    InitPlayTexturesPositions(PlayBlocksScreenPos, Width, Height);

    Width /= 1920;
    Height /= 1080;
    int ChangerTone = PlayBlocksScreenPos[6].w / 6.8;
    if (Width < 0.8)
    {
        PlayBlocksScreenPos[8].x += 5;
    }
    SDL_Rect Help_Pos = { int(PlayBlocksScreenPos[1].x + 180 * Width), int(PlayBlocksScreenPos[1].y + 20 * Height),int(1715 * Width),int(360 * Height) };
    SDL_Rect Play_it_Pos = { int(PlayBlocksScreenPos[1].x + PlayBlocksScreenPos[1].w / 3.3),int(PlayBlocksScreenPos[1].y - 45 * Height),int(786 * Width),int(50 * Height) };

    // Смена тональности
    Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, 4096);
    Mix_QuerySpec(&audioFrequency, &audioFormat, &audioChannelCount);  // query specs
    audioAllocatedMixChannelsCount = Mix_AllocateChannels(MIX_CHANNELS);
    float CurrentSpeed = 1;
    int channel;
    //
    
    int line = 0;
    int IsAccord = 0;

    int AccordType = 0;
    int TypeTone = 0;

    int MouseClicked = 0;
    int HelpOn = 0;


    bool quit = false;
    while (!quit)
    {
        SDL_Event event;
        for (int i = 0; i < 8; i++)
        {
            SDL_RenderCopy(renderer, texture, &PlayBlocks[i], &PlayBlocksScreenPos[i]);
        }
        if (HelpOn && !IsAccord)
        {
            SDL_RenderCopy(renderer, help_Texture, NULL, &Help_Pos);
        }
        if (PlaySong && CurrentNote == 1)
        {
            SDL_RenderCopy(renderer, play_it_Texture, NULL, &Play_it_Pos);
        }

        if (!IsAccord)
        {
            PlayBlocksScreenPos[3].x -= 4;
            PlayBlocksScreenPos[3].y -= 4;
            PlayBlocksScreenPos[3].w += 5;
            PlayBlocksScreenPos[3].h += 5;
            SDL_RenderCopy(renderer, texture, &PlayBlocks[9], &PlayBlocksScreenPos[3]);
            PlayBlocksScreenPos[3].x += 4;
            PlayBlocksScreenPos[3].y += 4;
            PlayBlocksScreenPos[3].w -= 5;
            PlayBlocksScreenPos[3].h -= 5;
        }
        else
        {
            PlayBlocksScreenPos[2].x -= 4;
            PlayBlocksScreenPos[2].y -= 4;
            PlayBlocksScreenPos[2].w += 5;
            PlayBlocksScreenPos[2].h += 5;
            SDL_RenderCopy(renderer, texture, &PlayBlocks[9], &PlayBlocksScreenPos[2]);
            PlayBlocksScreenPos[2].x += 4;
            PlayBlocksScreenPos[2].y += 4;
            PlayBlocksScreenPos[2].w -= 5;
            PlayBlocksScreenPos[2].h -= 5;
            if (TypeTone)
            {
                PlayBlocksScreenPos[4].x -= 4;
                PlayBlocksScreenPos[4].y -= 4;
                PlayBlocksScreenPos[4].w += 5;
                PlayBlocksScreenPos[4].h += 5;
                SDL_RenderCopy(renderer, texture, &PlayBlocks[9], &PlayBlocksScreenPos[4]);
                PlayBlocksScreenPos[4].x += 4;
                PlayBlocksScreenPos[4].y += 4;
                PlayBlocksScreenPos[4].w -= 5;
                PlayBlocksScreenPos[4].h -= 5;
            }
        }
        SDL_RenderCopy(renderer, texture, &PlayBlocks[11], &PlayBlocksScreenPos[8]);


        if (AccordType)
        {
            SDL_RenderCopy(renderer, texture, &PlayBlocks[12], &PlayBlocksScreenPos[9]);
            switch (AccordType)
            {
            case 1:
                SDL_RenderCopy(renderer, texture, &PlayBlocks[14], &PlayBlocksScreenPos[10]);
                break;
            case 2:
                SDL_RenderCopy(renderer, texture, &PlayBlocks[16], &PlayBlocksScreenPos[11]);
                SDL_RenderCopy(renderer, texture, &PlayBlocks[14], &PlayBlocksScreenPos[10]);
                break;
            case 3:
                SDL_RenderCopy(renderer, texture, &PlayBlocks[15], &PlayBlocksScreenPos[10]);
                break;
            case 4:
                SDL_RenderCopy(renderer, texture, &PlayBlocks[16], &PlayBlocksScreenPos[11]);
                SDL_RenderCopy(renderer, texture, &PlayBlocks[15], &PlayBlocksScreenPos[10]);
                break;
            }
        }
        else
        {
            SDL_RenderCopy(renderer, texture, &PlayBlocks[13], &PlayBlocksScreenPos[10]);
        }

        while (SDL_PollEvent(&event))
        {
            if (event.button.button == SDL_BUTTON_LEFT && event.type == SDL_MOUSEBUTTONDOWN)
            {
                MouseClicked = 1;
            }
            if (event.type == SDL_MOUSEBUTTONUP)
            {
                if (MouseClicked)
                {
                        if ((event.button.x >= PlayBlocksScreenPos[5].x) && (event.button.x <= PlayBlocksScreenPos[5].x + PlayBlocksScreenPos[5].w)
                            && (event.button.y >= PlayBlocksScreenPos[5].y) && (event.button.y <= PlayBlocksScreenPos[5].y + PlayBlocksScreenPos[5].h))
                        {
                            quit = true;
                        }
                }
                MouseClicked = 0;
            }

            if (event.type == SDL_QUIT)
            {
                quit = true;
                *Isclosed = 1;
            }
            if (!WrongNote)
            {
                PlaySong = 0;
                WrongNote = 3;
                Mix_PlayMusic(GuitarReaction[rand() % 3], 1);
            }
            if (event.type == SDL_KEYDOWN)
            {
                if (!IsAccord)
                {
                    if (PlaySong)
                    {
                        PressedNote = event.key.keysym.scancode;
                    }
                    switch (event.key.keysym.scancode)
                    {
                        // Переход к игре аккордов
                    case SDL_SCANCODE_SPACE:
                    {
                        PlaySong = 0;
                        IsAccord = !IsAccord;
                        line = 0;
                        Mix_HaltMusic();
                        break;
                    }
                    //

                    // Смена струн
                    case SDL_SCANCODE_Z:
                    {
                        line = !line;
                        break;
                    }
                    //
                // Струна 1
                    case SDL_SCANCODE_GRAVE: case SDL_SCANCODE_SEMICOLON:
                        if (!line)
                        {
                            Mix_PlayChannel(0, chords[0], 0);
                            if (event.key.keysym.scancode == SDL_SCANCODE_GRAVE)
                            {
                                ShowPlayingString(renderer, texture, PlayBlocks, 195, 708, Width, Height);
                                PressedNote = SDL_SCANCODE_GRAVE;
                            }
                            else
                            {
                                ShowPlayingString(renderer, texture, PlayBlocks, 1325, 828, Width, Height);
                            }
                        }
                        else
                        {
                            Mix_PlayChannel(0, chords[28], 0);
                            if (event.key.keysym.scancode == SDL_SCANCODE_GRAVE)
                            {
                                ShowPlayingString(renderer, texture, PlayBlocks, 195, 888, Width, Height);
                            }
                            else
                            {
                                ShowPlayingString(renderer, texture, PlayBlocks, 1325, 1000, Width, Height);
                            }
                        }
                        break;
                    case SDL_SCANCODE_1: case SDL_SCANCODE_APOSTROPHE:
                        if (!line)
                        {
                            Mix_PlayChannel(0, chords[1], 0);
                            if (event.key.keysym.scancode == SDL_SCANCODE_1)
                            {
                                ShowPlayingString(renderer, texture, PlayBlocks, 315, 708, Width, Height);
                                PressedNote = SDL_SCANCODE_1;
                            }
                            else
                            {
                                ShowPlayingString(renderer, texture, PlayBlocks, 1445, 828, Width, Height);
                            }
                        }
                        else
                        {
                            Mix_PlayChannel(0, chords[23], 0);
                            if (event.key.keysym.scancode == SDL_SCANCODE_1)
                            {
                                ShowPlayingString(renderer, texture, PlayBlocks, 315, 888, Width, Height);
                            }
                            else
                            {
                                ShowPlayingString(renderer, texture, PlayBlocks, 1445, 1000, Width, Height);
                            }
                        }
                        break;
                    case SDL_SCANCODE_2:
                        if (!line)
                        {
                            Mix_PlayChannel(0, chords[2], 0);
                            ShowPlayingString(renderer, texture, PlayBlocks, 445, 708, Width, Height);
                        }
                        else
                        {
                            Mix_PlayChannel(0, chords[25], 0);
                            ShowPlayingString(renderer, texture, PlayBlocks, 445, 888, Width, Height);
                        }
                        break;
                    case SDL_SCANCODE_3:
                        if (!line)
                        {
                            Mix_PlayChannel(0, chords[3], 0);
                            ShowPlayingString(renderer, texture, PlayBlocks, 565, 708, Width, Height);
                        }
                        else
                        {
                            Mix_PlayChannel(0, chords[26], 0);
                            ShowPlayingString(renderer, texture, PlayBlocks, 565, 888, Width, Height);
                        }
                        break;
                    case SDL_SCANCODE_4:
                        if (!line)
                        {
                            Mix_PlayChannel(0, chords[4], 0);
                            ShowPlayingString(renderer, texture, PlayBlocks, 695, 708, Width, Height);
                        }
                        else
                        {
                            Mix_PlayChannel(0, chords[27], 0);
                            ShowPlayingString(renderer, texture, PlayBlocks, 695, 888, Width, Height);
                        }
                        break;
                    case SDL_SCANCODE_5:
                        if (!line)
                        {
                            Mix_PlayChannel(0, chords[5], 0);
                            ShowPlayingString(renderer, texture, PlayBlocks, 815, 708, Width, Height);
                        }
                        else
                        {
                            Mix_PlayChannel(0, chords[19], 0);
                            ShowPlayingString(renderer, texture, PlayBlocks, 815, 888, Width, Height);
                        }
                        break;
                    case SDL_SCANCODE_6:
                        if (!line)
                        {
                            Mix_PlayChannel(0, chords[6], 0);
                            ShowPlayingString(renderer, texture, PlayBlocks, 945, 708, Width, Height);
                        }
                        else
                        {
                            Mix_PlayChannel(0, chords[20], 0);
                            ShowPlayingString(renderer, texture, PlayBlocks, 945, 888, Width, Height);
                        }
                        break;
                    case SDL_SCANCODE_7:
                        if (!line)
                        {
                            Mix_PlayChannel(0, chords[7], 0);
                            ShowPlayingString(renderer, texture, PlayBlocks, 1075, 708, Width, Height);
                        }
                        else
                        {
                            Mix_PlayChannel(0, chords[21], 0);
                            ShowPlayingString(renderer, texture, PlayBlocks, 1075, 888, Width, Height);
                        }
                        break;
                    case SDL_SCANCODE_8:
                        if (!line)
                        {
                            Mix_PlayChannel(0, chords[8], 0);
                            ShowPlayingString(renderer, texture, PlayBlocks, 1195, 708, Width, Height);
                        }
                        else
                        {
                            Mix_PlayChannel(0, chords[22], 0);
                            ShowPlayingString(renderer, texture, PlayBlocks, 1195, 888, Width, Height);
                        }
                        break;
                    case SDL_SCANCODE_9:
                        if (!line)
                        {
                            Mix_PlayChannel(0, chords[9], 0);
                            ShowPlayingString(renderer, texture, PlayBlocks, 1325, 708, Width, Height);
                        }
                        else
                        {
                            Mix_PlayChannel(0, chords[14], 0);
                            ShowPlayingString(renderer, texture, PlayBlocks, 1325, 888, Width, Height);
                        }
                        break;
                    case SDL_SCANCODE_0:
                        if (!line)
                        {
                            Mix_PlayChannel(0, chords[10], 0);
                            ShowPlayingString(renderer, texture, PlayBlocks, 1445, 708, Width, Height);
                        }
                        else
                        {
                            Mix_PlayChannel(0, chords[15], 0);
                            ShowPlayingString(renderer, texture, PlayBlocks, 1445, 888, Width, Height);
                        }
                        break;
                    case SDL_SCANCODE_MINUS:
                        if (!line)
                        {
                            Mix_PlayChannel(0, chords[11], 0);
                            ShowPlayingString(renderer, texture, PlayBlocks, 1565, 708, Width, Height);
                        }
                        else
                        {
                            Mix_PlayChannel(0, chords[16], 0);
                            ShowPlayingString(renderer, texture, PlayBlocks, 1565, 888, Width, Height);
                        }
                        break;
                    case SDL_SCANCODE_EQUALS:
                        if (!line)
                        {
                            Mix_PlayChannel(0, chords[12], 0);
                            ShowPlayingString(renderer, texture, PlayBlocks, 1690, 708, Width, Height);
                        }
                        else
                        {
                            Mix_PlayChannel(0, chords[17], 0);
                            ShowPlayingString(renderer, texture, PlayBlocks, 1690, 888, Width, Height);
                        }
                        break;
                    case SDL_SCANCODE_BACKSPACE:
                        if (!line)
                        {
                            Mix_PlayChannel(0, chords[13], 0);
                            ShowPlayingString(renderer, texture, PlayBlocks, 1825, 708, Width, Height);
                        }
                        else
                        {
                            Mix_PlayChannel(0, chords[18], 0);
                            ShowPlayingString(renderer, texture, PlayBlocks, 1825, 888, Width, Height);
                        }
                        break;
                        // Струна 2
                    case SDL_SCANCODE_TAB: case SDL_SCANCODE_G:
                        if (!line)
                        {
                            Mix_PlayChannel(0, chords[14], 0);
                            if (event.key.keysym.scancode == SDL_SCANCODE_TAB)
                            {
                                ShowPlayingString(renderer, texture, PlayBlocks, 195, 768, Width, Height);
                            }
                            else
                            {
                                ShowPlayingString(renderer, texture, PlayBlocks, 695, 828, Width, Height);
                            }
                        }
                        else
                        {
                            Mix_PlayChannel(0, chords[28], 0);
                            if (event.key.keysym.scancode == SDL_SCANCODE_TAB)
                            {
                                ShowPlayingString(renderer, texture, PlayBlocks, 195, 948, Width, Height);
                            }
                            else
                            {
                                ShowPlayingString(renderer, texture, PlayBlocks, 695, 1000, Width, Height);
                            }
                        }
                        break;
                    case SDL_SCANCODE_Q: case SDL_SCANCODE_H:
                        if (!line)
                        {
                            Mix_PlayChannel(0, chords[15], 0);
                            if (event.key.keysym.scancode == SDL_SCANCODE_Q)
                            {
                                ShowPlayingString(renderer, texture, PlayBlocks, 315, 768, Width, Height);
                            }
                            else
                            {
                                ShowPlayingString(renderer, texture, PlayBlocks, 815, 828, Width, Height);
                            }
                        }
                        else
                        {
                            Mix_PlayChannel(0, chords[29], 0);
                            if (event.key.keysym.scancode == SDL_SCANCODE_Q)
                            {
                                ShowPlayingString(renderer, texture, PlayBlocks, 315, 948, Width, Height);
                            }
                            else
                            {
                                ShowPlayingString(renderer, texture, PlayBlocks, 815, 1000, Width, Height);
                            }
                        }
                        break;
                    case SDL_SCANCODE_W: case SDL_SCANCODE_J:
                        if (!line)
                        {
                            Mix_PlayChannel(0, chords[16], 0);
                            if (event.key.keysym.scancode == SDL_SCANCODE_W)
                            {
                                ShowPlayingString(renderer, texture, PlayBlocks, 445, 768, Width, Height);
                            }
                            else
                            {
                                ShowPlayingString(renderer, texture, PlayBlocks, 945, 828, Width, Height);
                            }
                        }
                        else
                        {
                            Mix_PlayChannel(0, chords[30], 0);
                            if (event.key.keysym.scancode == SDL_SCANCODE_W)
                            {
                                ShowPlayingString(renderer, texture, PlayBlocks, 445, 948, Width, Height);
                            }
                            else
                            {
                                ShowPlayingString(renderer, texture, PlayBlocks, 945, 1000, Width, Height);
                            }
                        }
                        break;
                    case SDL_SCANCODE_E: case SDL_SCANCODE_K:
                        if (!line)
                        {
                            Mix_PlayChannel(0, chords[17], 0);
                            if (event.key.keysym.scancode == SDL_SCANCODE_E)
                            {
                                ShowPlayingString(renderer, texture, PlayBlocks, 565, 768, Width, Height);
                            }
                            else
                            {
                                ShowPlayingString(renderer, texture, PlayBlocks, 1075, 828, Width, Height);
                            }
                        }
                        else
                        {
                            Mix_PlayChannel(0, chords[31], 0);
                            if (event.key.keysym.scancode == SDL_SCANCODE_E)
                            {
                                ShowPlayingString(renderer, texture, PlayBlocks, 565, 948, Width, Height);
                            }
                            else
                            {
                                ShowPlayingString(renderer, texture, PlayBlocks, 1075, 1000, Width, Height);
                            }
                        }
                        break;
                    case SDL_SCANCODE_R: case SDL_SCANCODE_L:
                        if (!line)
                        {
                            Mix_PlayChannel(0, chords[18], 0);
                            if (event.key.keysym.scancode == SDL_SCANCODE_R)
                            {
                                ShowPlayingString(renderer, texture, PlayBlocks, 695, 768, Width, Height);
                            }
                            else
                            {
                                ShowPlayingString(renderer, texture, PlayBlocks, 1195, 828, Width, Height);
                            }
                        }
                        else
                        {
                            Mix_PlayChannel(0, chords[32], 0);
                            if (event.key.keysym.scancode == SDL_SCANCODE_R)
                            {
                                ShowPlayingString(renderer, texture, PlayBlocks, 695, 948, Width, Height);
                            }
                            else
                            {
                                ShowPlayingString(renderer, texture, PlayBlocks, 1195, 1000, Width, Height);
                            }
                        }
                        break;
                    case SDL_SCANCODE_T:
                        if (!line)
                        {
                            Mix_PlayChannel(0, chords[0], 0);
                            ShowPlayingString(renderer, texture, PlayBlocks, 815, 768, Width, Height);
                        }
                        else
                        {
                            Mix_PlayChannel(0, chords[23], 0);
                            ShowPlayingString(renderer, texture, PlayBlocks, 815, 948, Width, Height);
                        }
                        break;
                    case SDL_SCANCODE_Y:
                        if (!line)
                        {
                            Mix_PlayChannel(0, chords[1], 0);
                            ShowPlayingString(renderer, texture, PlayBlocks, 945, 768, Width, Height);
                        }
                        else
                        {
                            Mix_PlayChannel(0, chords[24], 0);
                            ShowPlayingString(renderer, texture, PlayBlocks, 945, 948, Width, Height);
                        }
                        break;
                    case SDL_SCANCODE_U:
                        if (!line)
                        {
                            Mix_PlayChannel(0, chords[2], 0);
                            ShowPlayingString(renderer, texture, PlayBlocks, 1075, 768, Width, Height);
                        }
                        else
                        {
                            Mix_PlayChannel(0, chords[25], 0);
                            ShowPlayingString(renderer, texture, PlayBlocks, 1075, 948, Width, Height);
                        }
                        break;
                    case SDL_SCANCODE_I:
                        if (!line)
                        {
                            Mix_PlayChannel(0, chords[3], 0);
                            ShowPlayingString(renderer, texture, PlayBlocks, 1195, 768, Width, Height);
                        }
                        else
                        {
                            Mix_PlayChannel(0, chords[26], 0);
                            ShowPlayingString(renderer, texture, PlayBlocks, 1195, 948, Width, Height);
                        }
                        break;
                    case SDL_SCANCODE_O:
                        if (!line)
                        {
                            Mix_PlayChannel(0, chords[4], 0);
                            ShowPlayingString(renderer, texture, PlayBlocks, 1325, 768, Width, Height);
                        }
                        else
                        {
                            Mix_PlayChannel(0, chords[27], 0);
                            ShowPlayingString(renderer, texture, PlayBlocks, 1325, 948, Width, Height);
                        }
                        break;
                    case SDL_SCANCODE_P:
                        if (!line)
                        {
                            Mix_PlayChannel(0, chords[5], 0);
                            ShowPlayingString(renderer, texture, PlayBlocks, 1445, 768, Width, Height);
                        }
                        else
                        {
                            Mix_PlayChannel(0, chords[19], 0);
                            ShowPlayingString(renderer, texture, PlayBlocks, 1445, 948, Width, Height);
                        }
                        break;
                    case SDL_SCANCODE_LEFTBRACKET:
                        if (!line)
                        {
                            Mix_PlayChannel(0, chords[6], 0);
                            ShowPlayingString(renderer, texture, PlayBlocks, 1565, 768, Width, Height);
                        }
                        else
                        {
                            Mix_PlayChannel(0, chords[20], 0);
                            ShowPlayingString(renderer, texture, PlayBlocks, 1565, 948, Width, Height);
                        }
                        break;
                    case SDL_SCANCODE_RIGHTBRACKET:
                        if (!line)
                        {
                            Mix_PlayChannel(0, chords[7], 0);
                            ShowPlayingString(renderer, texture, PlayBlocks, 1690, 768, Width, Height);
                        }
                        else
                        {
                            Mix_PlayChannel(0, chords[21], 0);
                            ShowPlayingString(renderer, texture, PlayBlocks, 1690, 948, Width, Height);
                        }
                        break;
                    case SDL_SCANCODE_BACKSLASH:
                        if (!line)
                        {
                            Mix_PlayChannel(0, chords[8], 0);
                            ShowPlayingString(renderer, texture, PlayBlocks, 1825, 768, Width, Height);
                        }
                        else
                        {
                            Mix_PlayChannel(0, chords[22], 0);
                            ShowPlayingString(renderer, texture, PlayBlocks, 1825, 948, Width, Height);
                        }
                        break;
                    // 3 струна
                    case SDL_SCANCODE_A:
                        if (!line)
                        {
                            Mix_PlayChannel(0, chords[19], 0);
                            ShowPlayingString(renderer, texture, PlayBlocks, 195, 828, Width, Height);
                        }
                        else
                        {
                            Mix_PlayChannel(0, chords[33], 0);
                            ShowPlayingString(renderer, texture, PlayBlocks, 195, 1000, Width, Height);
                        }
                        break;
                    case SDL_SCANCODE_S:
                        if (!line)
                        {
                            Mix_PlayChannel(0, chords[20], 0);
                            ShowPlayingString(renderer, texture, PlayBlocks, 315, 828, Width, Height);
                        }
                        else
                        {
                            Mix_PlayChannel(0, chords[34], 0);
                            ShowPlayingString(renderer, texture, PlayBlocks, 315, 1000, Width, Height);
                        }
                        break;
                    case SDL_SCANCODE_D:
                        if (!line)
                        {
                            Mix_PlayChannel(0, chords[21], 0);
                            ShowPlayingString(renderer, texture, PlayBlocks, 445, 828, Width, Height);
                        }
                        else
                        {
                            Mix_PlayChannel(0, chords[35], 0);
                            ShowPlayingString(renderer, texture, PlayBlocks, 445, 1000, Width, Height);
                        }
                        break;
                    case SDL_SCANCODE_F:
                        if (!line)
                        {
                            Mix_PlayChannel(0, chords[22], 0);
                            ShowPlayingString(renderer, texture, PlayBlocks, 565, 828, Width, Height);
                        }
                        else
                        {
                            Mix_PlayChannel(0, chords[36], 0);
                            ShowPlayingString(renderer, texture, PlayBlocks, 565, 1000, Width, Height);
                        }
                        break;
                    case SDL_SCANCODE_X:
                        HelpOn = !HelpOn;
                        break;
//---------------------------------------------
                    case SDL_SCANCODE_ESCAPE:
                        Mix_HaltMusic();
                        break;
                    case SDL_SCANCODE_V:
                        WrongNote = 3;
                        PressedNote = 0;
                        PlaySong = 1;
                        CurrentNote = 1;
                        break;
                    case SDL_SCANCODE_B:
                        WrongNote = 3;
                        PressedNote = 0;
                        PlaySong = 2;
                        CurrentNote = 1;
                        break;
                    case SDL_SCANCODE_N:
                        WrongNote = 3;
                        PressedNote = 0;
                        PlaySong = 3;
                        CurrentNote = 1;
                        break;
                    case SDL_SCANCODE_M:
                        WrongNote = 3;
                        PressedNote = 0;
                        PlaySong = 4;
                        CurrentNote = 1;
                        break;
                    case 54:
                        WrongNote = 3;
                        PressedNote = 0;
                        PlaySong = 5;
                        CurrentNote = 1;
                        break;
                    case 55:
                        WrongNote = 3;
                        PressedNote = 0;
                        PlaySong = 6;
                        CurrentNote = 1;
                        break;
                    }
                }
                else
                {
                    switch (event.key.keysym.scancode)
                    {
                        // Переход к игре на струнах
                    case SDL_SCANCODE_SPACE:
                    {
                        IsAccord = !IsAccord;
                        TypeTone = 0;
                        AccordType = 0;
                        PlayBlocksScreenPos[8].x = 5 + PlayBlocksScreenPos[6].w / 2;
                        CurrentSpeed = 1;
                        break;
                    }
                    //------------------------------------------------------------------------------------------------------
                    // -Аккорды-
                    case SDL_SCANCODE_0:
                        if (!AccordType)
                        {
                            TypeTone = !TypeTone;
                            TypeTone *= 3;
                        }
                        else
                        {
                            TypeTone = 0;
                        }
                        break;
                    case SDL_SCANCODE_MINUS:
                        if (CurrentSpeed > 0.87)
                        {
                            PlayBlocksScreenPos[8].x -= ChangerTone;
                            CurrentSpeed /= 1.06;
                        }
                        break;
                        // -Минор-
                    case SDL_SCANCODE_EQUALS:
                        if (CurrentSpeed < 1.18)
                        {
                            PlayBlocksScreenPos[8].x += ChangerTone;
                            CurrentSpeed *= 1.06;
                        }
                        break;
                        // Без боя
                    case SDL_SCANCODE_1:
                        AccordType = 0;
                        break;
                        // Бой шестерка
                    case SDL_SCANCODE_2:
                        AccordType = 1;
                        TypeTone = 0;
                        break;
                        // Бой восьмёрка
                    case SDL_SCANCODE_3:
                        AccordType = 2;
                        TypeTone = 0;
                        break;
                    case SDL_SCANCODE_4:
                        AccordType = 3;
                        TypeTone = 0;
                        break;
                    case SDL_SCANCODE_5:
                        AccordType = 4;
                        TypeTone = 0;
                        break;
                        // -C-
                    case SDL_SCANCODE_Q:
                        ShowPlayingAccord(renderer, texture, PlayBlocks, 17, 12, Width, Height); // 1024x768 - 29;20 | 1366x768 - 25;20 | 1600x900 - 20;15 | 1920x1080 - 17;12
                        if (!AccordType)
                        {
                            channel = Mix_PlayChannel(0, chords_real[0 + TypeTone], 0);
                            setupPlaybackSpeedEffect(chords_real[0 + TypeTone], CurrentSpeed, channel, false);
                        }
                        else
                        {
                            channel = Mix_PlayChannel(0, chords_boj[-1 + AccordType], 0);
                            setupPlaybackSpeedEffect(chords_boj[-1 + AccordType], CurrentSpeed, channel, false);
                        }
                        break;
                        // -D-
                    case SDL_SCANCODE_W:
                        ShowPlayingAccord(renderer, texture, PlayBlocks, 123, 12, Width, Height);
                        if (!AccordType)
                        {
                            channel = Mix_PlayChannel(0, chords_real[6 + TypeTone], 0);
                            setupPlaybackSpeedEffect(chords_real[6 + TypeTone], CurrentSpeed, channel, false);
                        }
                        else
                        {
                            channel = Mix_PlayChannel(0, chords_boj[3 + AccordType], 0);
                            setupPlaybackSpeedEffect(chords_boj[3 + AccordType], CurrentSpeed, channel, false);
                        }
                        break;
                        // -E-
                    case SDL_SCANCODE_E:
                        ShowPlayingAccord(renderer, texture, PlayBlocks, 228, 12, Width, Height);
                        if (!AccordType)
                        {
                            channel = Mix_PlayChannel(0, chords_real[12 + TypeTone], 0);
                            setupPlaybackSpeedEffect(chords_real[12 + TypeTone], CurrentSpeed, channel, false);
                        }
                        else
                        {
                            channel = Mix_PlayChannel(0, chords_boj[3 + AccordType], 0);
                            setupPlaybackSpeedEffect(chords_boj[3 + AccordType], CurrentSpeed * 1.13, channel, false);
                        }
                        break;
                        // -F-
                    case SDL_SCANCODE_R:
                        ShowPlayingAccord(renderer, texture, PlayBlocks, 333, 12, Width, Height);
                        if (!AccordType)
                        {
                            channel = Mix_PlayChannel(0, chords_real[18 + TypeTone], 0);
                            setupPlaybackSpeedEffect(chords_real[18 + TypeTone], CurrentSpeed, channel, false);
                        }
                        else
                        {
                            channel = Mix_PlayChannel(0, chords_boj[3 + AccordType], 0);
                            setupPlaybackSpeedEffect(chords_boj[3 + AccordType], CurrentSpeed * 1.208, channel, false);
                        }
                        break;
                        // -G-
                    case SDL_SCANCODE_T:
                        ShowPlayingAccord(renderer, texture, PlayBlocks, 438, 12, Width, Height);
                        if (!AccordType)
                        {
                            channel = Mix_PlayChannel(0, chords_real[24 + TypeTone], 0);
                            setupPlaybackSpeedEffect(chords_real[24 + TypeTone], CurrentSpeed, channel, false);
                        }
                        else
                        {
                            channel = Mix_PlayChannel(0, chords_boj[11 + AccordType], 0);
                            setupPlaybackSpeedEffect(chords_boj[11 + AccordType], CurrentSpeed * 1.03, channel, false);
                        }
                        break;
                        // -A-
                    case SDL_SCANCODE_Y:
                        ShowPlayingAccord(renderer, texture, PlayBlocks, 543, 12, Width, Height);
                        if (!AccordType)
                        {
                            channel = Mix_PlayChannel(0, chords_real[30 + TypeTone], 0);
                            setupPlaybackSpeedEffect(chords_real[30 + TypeTone], CurrentSpeed, channel, false);
                        }
                        else
                        {
                            channel = Mix_PlayChannel(0, chords_boj[11 + AccordType], 0);
                            setupPlaybackSpeedEffect(chords_boj[11 + AccordType], CurrentSpeed * 1.165, channel, false);
                        }
                        break;
                        // -B-
                    case SDL_SCANCODE_U:
                        ShowPlayingAccord(renderer, texture, PlayBlocks, 647, 12, Width, Height);
                        if (!AccordType)
                        {
                            channel = Mix_PlayChannel(0, chords_real[36 + TypeTone], 0);
                            setupPlaybackSpeedEffect(chords_real[36 + TypeTone], CurrentSpeed, channel, false);
                        }
                        else
                        {
                            channel = Mix_PlayChannel(0, chords_boj[11 + AccordType], 0);
                            setupPlaybackSpeedEffect(chords_boj[11 + AccordType], CurrentSpeed * 1.3, channel, false);
                        }
                        break;
                        // -Cm-
                    case SDL_SCANCODE_A:
                        ShowPlayingAccord(renderer, texture, PlayBlocks, 17, 129, Width, Height);
                        if (!AccordType)
                        {
                            channel = Mix_PlayChannel(0, chords_real[1 + TypeTone], 0);
                            setupPlaybackSpeedEffect(chords_real[1 + TypeTone], CurrentSpeed, channel, false);
                        }
                        else
                        {
                            channel = Mix_PlayChannel(0, chords_boj[3 + AccordType], 0);
                            setupPlaybackSpeedEffect(chords_boj[3 + AccordType], CurrentSpeed * 0.89, channel, false);
                        }
                        break;
                        // -Dm-
                    case SDL_SCANCODE_S:
                        ShowPlayingAccord(renderer, texture, PlayBlocks, 123, 129, Width, Height);
                        if (!AccordType)
                        {
                            channel = Mix_PlayChannel(0, chords_real[7 + TypeTone], 0);
                            setupPlaybackSpeedEffect(chords_real[7 + TypeTone], CurrentSpeed, channel, false);
                        }
                        else
                        {
                            channel = Mix_PlayChannel(0, chords_boj[7 + AccordType], 0);
                            setupPlaybackSpeedEffect(chords_boj[7 + AccordType], CurrentSpeed * 0.84, channel, false);
                        }
                        break;
                        // -Em-
                    case SDL_SCANCODE_D:
                        ShowPlayingAccord(renderer, texture, PlayBlocks, 228, 129, Width, Height);
                        if (!AccordType)
                        {
                            channel = Mix_PlayChannel(0, chords_real[13 + TypeTone], 0);
                            setupPlaybackSpeedEffect(chords_real[13 + TypeTone], CurrentSpeed, channel, false);
                        }
                        else
                        {
                            channel = Mix_PlayChannel(0, chords_boj[7 + AccordType], 0);
                            setupPlaybackSpeedEffect(chords_boj[7 + AccordType], CurrentSpeed, channel, false);
                        }
                        break;
                        // -Fm-
                    case SDL_SCANCODE_F:
                        ShowPlayingAccord(renderer, texture, PlayBlocks, 333, 129, Width, Height);
                        if (!AccordType)
                        {
                            channel = Mix_PlayChannel(0, chords_real[19 + TypeTone], 0);
                            setupPlaybackSpeedEffect(chords_real[19 + TypeTone], CurrentSpeed, channel, false);
                        }
                        else
                        {
                            channel = Mix_PlayChannel(0, chords_boj[7 + AccordType], 0);
                            setupPlaybackSpeedEffect(chords_boj[7 + AccordType], CurrentSpeed * 1.07, channel, false);
                        }
                        break;
                        // -Gm-
                    case SDL_SCANCODE_G:
                        ShowPlayingAccord(renderer, texture, PlayBlocks, 438, 129, Width, Height);
                        if (!AccordType)
                        {
                            channel = Mix_PlayChannel(0, chords_real[25 + TypeTone], 0);
                            setupPlaybackSpeedEffect(chords_real[25 + TypeTone], CurrentSpeed, channel, false);
                        }
                        else
                        {
                            channel = Mix_PlayChannel(0, chords_boj[7 + AccordType], 0);
                            setupPlaybackSpeedEffect(chords_boj[7 + AccordType], CurrentSpeed * 1.21, channel, false);
                        }
                        break;
                        // -Am-
                    case SDL_SCANCODE_H:
                        ShowPlayingAccord(renderer, texture, PlayBlocks, 543, 129, Width, Height);
                        if (!AccordType)
                        {
                            channel = Mix_PlayChannel(0, chords_real[31 + TypeTone], 0);
                            setupPlaybackSpeedEffect(chords_real[31 + TypeTone], CurrentSpeed, channel, false);
                        }
                        else
                        {
                            channel = Mix_PlayChannel(0, chords_boj[7 + AccordType], 0);
                            setupPlaybackSpeedEffect(chords_boj[7 + AccordType], CurrentSpeed * 1.145, channel, false);
                        }
                        break;
                        // -Bm-
                    case SDL_SCANCODE_J:
                        ShowPlayingAccord(renderer, texture, PlayBlocks, 647, 129, Width, Height);
                        if (!AccordType)
                        {
                            channel = Mix_PlayChannel(0, chords_real[37 + TypeTone], 0);
                            setupPlaybackSpeedEffect(chords_real[37 + TypeTone], CurrentSpeed, channel, false);
                        }
                        else
                        {
                            channel = Mix_PlayChannel(0, chords_boj[-1 + AccordType], 0);
                            setupPlaybackSpeedEffect(chords_boj[-1 + AccordType], CurrentSpeed * 1.11, channel, false);
                        }
                        break;
                    // -C7-
                    case SDL_SCANCODE_Z:
                        if (!AccordType)
                        {
                            ShowPlayingAccord(renderer, texture, PlayBlocks, 17, 254, Width, Height);
                            channel = Mix_PlayChannel(0, chords_real[2 + TypeTone], 0);
                            setupPlaybackSpeedEffect(chords_real[2 + TypeTone], CurrentSpeed, channel, false);
                        }
                        break;
                        // -D7-
                    case SDL_SCANCODE_X:
                        if (!AccordType)
                        {
                            ShowPlayingAccord(renderer, texture, PlayBlocks, 123, 254, Width, Height);
                            channel = Mix_PlayChannel(0, chords_real[8 + TypeTone], 0);
                            setupPlaybackSpeedEffect(chords_real[8 + TypeTone], CurrentSpeed, channel, false);
                        }
                        break;
                        // -E7-
                    case SDL_SCANCODE_C:
                        if (!AccordType)
                        {
                            ShowPlayingAccord(renderer, texture, PlayBlocks, 228, 254, Width, Height);
                            channel = Mix_PlayChannel(0, chords_real[14 + TypeTone], 0);
                            setupPlaybackSpeedEffect(chords_real[14 + TypeTone], CurrentSpeed, channel, false);
                        }
                        break;
                        // -F7-
                    case SDL_SCANCODE_V:
                        if (!AccordType)
                        {
                            ShowPlayingAccord(renderer, texture, PlayBlocks, 333, 254, Width, Height);
                            channel = Mix_PlayChannel(0, chords_real[20 + TypeTone], 0);
                            setupPlaybackSpeedEffect(chords_real[20 + TypeTone], CurrentSpeed, channel, false);
                        }
                        break;
                        // -G7-
                    case SDL_SCANCODE_B:
                        if (!AccordType)
                        {
                            ShowPlayingAccord(renderer, texture, PlayBlocks, 438, 254, Width, Height);
                            channel = Mix_PlayChannel(0, chords_real[26 + TypeTone], 0);
                            setupPlaybackSpeedEffect(chords_real[26 + TypeTone], CurrentSpeed, channel, false);
                        }
                        break;
                        // -A7-
                    case SDL_SCANCODE_N:
                        if (!AccordType)
                        {
                            ShowPlayingAccord(renderer, texture, PlayBlocks, 543, 254, Width, Height);
                            channel = Mix_PlayChannel(0, chords_real[32 + TypeTone], 0);
                            setupPlaybackSpeedEffect(chords_real[32 + TypeTone], CurrentSpeed, channel, false);
                        }
                        break;
                        // -B7-
                    case SDL_SCANCODE_M:
                        if (!AccordType)
                        {
                            ShowPlayingAccord(renderer, texture, PlayBlocks, 647, 254, Width, Height);
                            channel = Mix_PlayChannel(0, chords_real[38 + TypeTone], 0);
                            setupPlaybackSpeedEffect(chords_real[38 + TypeTone], CurrentSpeed, channel, false);
                        }
                        break;
                    }
                }
            }
        }
        switch (PlaySong) 
        {
        case 1:
            PlaySong1(renderer, texture, PlayBlocks, Width, Height, &CurrentNote, &PressedNote, &PlaySong, GuitarHero[0], &WrongNote);
            break;
        case 2:
            PlaySong2(renderer, texture, PlayBlocks, Width, Height, &CurrentNote, &PressedNote, &PlaySong, GuitarHero[1], &WrongNote);
            break;
        case 3:
            PlaySong3(renderer, texture, PlayBlocks, Width, Height, &CurrentNote, &PressedNote, &PlaySong, GuitarHero[2], &WrongNote);
            break;
        case 4:
            PlaySong4(renderer, texture, PlayBlocks, Width, Height, &CurrentNote, &PressedNote, &PlaySong, GuitarHero[3], &WrongNote);
            break;
        case 5:
            PlaySong5(renderer, texture, PlayBlocks, Width, Height, &CurrentNote, &PressedNote, &PlaySong, GuitarHero[4], &WrongNote);
            break;
        case 6:
            PlaySong6(renderer, texture, PlayBlocks, Width, Height, &CurrentNote, &PressedNote, &PlaySong, GuitarHero[5], &WrongNote);
            break;
        }
        SDL_RenderPresent(renderer);
        SDL_RenderClear(renderer);
    }
    for (int i = 0; i < 38; i++)
    {
        Mix_FreeChunk(chords[i]);
    }
    SDL_DestroyTexture(help_Texture);
    SDL_DestroyTexture(play_it_Texture);
}