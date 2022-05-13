#include <iostream>

#include <SDL.h>
#include <clocale>
#include <cmath>
using namespace std;

void draw_circle(SDL_Renderer* renderer, int x, int y, int radius)
{
    for (int w = 0; w < radius * 2; w++)
    {
        for (int h = 0; h < radius * 2; h++)
        {
            int dx = radius - w;
            int dy = radius - h;
            if ((dx * dx + dy * dy) <= (radius * radius))
            {
                SDL_RenderDrawPoint(renderer, x + dx, y + dy);
            }
        }
    }
}

int main(int argc, char* argv[])
{
    setlocale(LC_ALL, "Russian");
    SDL_Window* window = NULL;

    const int HEIGHT = 1024;
    const int WIDTH = 768;
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        cout << "Ошибка инициализации" <<
            SDL_GetError();
    }
    window = SDL_CreateWindow(u8"Скатенок Илья", 25, 25, HEIGHT, WIDTH, SDL_WINDOW_SHOWN);

    SDL_Renderer* renderer = NULL;
    renderer = SDL_CreateRenderer(window, -1, 0);
    // Фон
    SDL_SetRenderDrawColor(renderer, 90, 90, 90, 0);
    SDL_RenderClear(renderer);
    //
    bool quit = false;
    while (!quit)
    {
        SDL_Event event;

        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
            {
                quit = true;
            }
        }
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 0);
        // Сам снеговик
        draw_circle(renderer, HEIGHT / 2, WIDTH / 1.45, HEIGHT / 10 + WIDTH / 10);
        draw_circle(renderer, HEIGHT / 2, WIDTH / 2.5, HEIGHT / 15 + WIDTH / 15);
        draw_circle(renderer, HEIGHT / 2, WIDTH / 5, HEIGHT / 20 + WIDTH / 20);
        //
        // Глаза
        SDL_SetRenderDrawColor(renderer, 5, 5, 5, 0);
        draw_circle(renderer, HEIGHT / 2.17, WIDTH / 6.6, HEIGHT / 105 + WIDTH / 105);
        draw_circle(renderer, HEIGHT / 1.85, WIDTH / 6.6, HEIGHT / 105 + WIDTH / 105);
        //
        // Улыбка
        SDL_RenderDrawLine(renderer, HEIGHT / 2.25, WIDTH / 4.5, HEIGHT / 2, WIDTH / 4.25);
        SDL_RenderDrawLine(renderer, HEIGHT / 2, WIDTH / 4.25, HEIGHT / 1.8, WIDTH / 4.35);
        //
        // Руки
        SDL_SetRenderDrawColor(renderer, 65, 12, 12, 0);
        SDL_RenderDrawLine(renderer, HEIGHT / 2.475, WIDTH / 3.1, HEIGHT / 4, WIDTH / 2);
        SDL_RenderDrawLine(renderer, HEIGHT / 1.675, WIDTH / 3.1, HEIGHT / 1.45, WIDTH / 2);
        //
        // Нос
        SDL_SetRenderDrawColor(renderer, 255, 127, 0, 0);
        SDL_Rect nose;
        nose.x = HEIGHT / 2.05;
        nose.y = WIDTH / 5.35;
        nose.h = HEIGHT / 1.95 - HEIGHT / 2.05;
        nose.w = WIDTH / 4.9 - WIDTH / 5.35;
        SDL_RenderDrawRect(renderer, &nose);
        SDL_RenderFillRect(renderer, &nose);
        //
        SDL_RenderPresent(renderer);

    }

    SDL_DestroyWindow(window);

    SDL_Quit();
    return 0;
}

