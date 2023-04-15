//
// gametest by Charles Yiu
// - repository: https://github.com/charlesyiu/gametest
// - website: https://yiu.ch/arles
// - email: charlesyiu.w@gmail.com
//

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_ttf.h>

#define CHECK_ERROR(test, message) \
    do { \
        if((test)) { \
            fprintf(stderr, "%s\n", (message)); \
            exit(1); \
        } \
    } while(0)

static const int width = 375;
static const int height = 100;

const struct SDL_Color White = {255, 255, 255, 0 };
enum Centering {
    Left,
    Center,
    Right
};

TTF_Font* Font(char* file, int size) {
    TTF_Font* font = TTF_OpenFont(file, size);
    CHECK_ERROR(font == NULL, "error: font not found\n");
    return font;
}
void Text(SDL_Renderer *renderer, char* text, int x, int y, enum Centering centering, SDL_Color color, TTF_Font *font) {
    CHECK_ERROR(font == NULL, "error: font not found\n");
    SDL_Surface* surface = TTF_RenderText_Shaded(font, text, color, (const struct SDL_Color) {
        .r = 0,
        .g = 0,
        .b = 0,
        .a = 0
    });
    CHECK_ERROR(surface == NULL, "error: surface error\n");
    SDL_Texture* message = SDL_CreateTextureFromSurface(renderer, surface);
    CHECK_ERROR(message == NULL, "error: texture error\n");
    struct SDL_Rect rect = {
            .w = surface->w,
            .h = surface->h,
            .y = y - (surface->h / 2)
    };
    if (centering == Left)
        rect.x = x;
    else if (centering == Center)
        rect.x = x - (rect.w / 2);
    else
        rect.x = x - rect.w;

    SDL_RenderCopy(renderer, message, NULL, &rect);
    SDL_FreeSurface(surface);
    SDL_DestroyTexture(message);
}
char* Resource(char* cwd, char* path) {
    int length = (int)strlen(cwd) + (int)strlen(path) + 15;
    char* resource = malloc(length);
    snprintf(resource, length, "%s/../Resources/%s", cwd, path);
    return resource;
}

int main(int argc, char **argv) {
    char* cwd = getcwd(NULL, 0);

    CHECK_ERROR(SDL_Init(SDL_INIT_VIDEO) != 0, SDL_GetError());

    SDL_Window *window = SDL_CreateWindow("Counter", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
    CHECK_ERROR(window == NULL, SDL_GetError());

    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    CHECK_ERROR(renderer == NULL, SDL_GetError());

    CHECK_ERROR(TTF_Init()==-1, "error: ttf_init");

    char* font_resource = Resource(cwd, "arial.ttf");
    TTF_Font* font = Font(font_resource, 50);
    int counts[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    int index = 0;

    bool running = true;
    SDL_Event event;
    while(running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
            } else if (event.type == SDL_KEYDOWN) {
                const char *key = SDL_GetKeyName(event.key.keysym.sym);
                if (strcmp(key, "Right") == 0) {
                    counts[index]++;
                    if (counts[index] > 999)
                        counts[index] = 0;
                } else if (strcmp(key, "Left") == 0) {
                    counts[index]--;
                    if (counts[index] < 0)
                        counts[index] = 999;
                } else if (strcmp(key, "Up") == 0) {
                    index++;
                    if (index > 9)
                        index = 0;
                } else if (strcmp(key, "Down") == 0) {
                    index--;
                    if (index < 0)
                        index = 9;
                }
            }
        }

        SDL_RenderClear(renderer);

        int length = snprintf(NULL, 0, "%d", counts[index]);
        char* text_content = malloc( length + 1 );
        snprintf(text_content, length + 13, "count[%d] = %d;", index, counts[index]);
        Text(renderer, text_content, 20, height / 2, Left, White, font);

        SDL_RenderPresent(renderer);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    free(font_resource);
    free(cwd);

    return 0;
}