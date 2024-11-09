#include "lib/iyore.h"
#include "lib/menu.h"

// extern declarations of variables
SDL_Window *window;
SDL_Renderer *renderer;
TTF_Font *titleFont;
TTF_Font *topicsFont;
TTF_Font *textfont;
TTF_Font *smallfont;
TTF_Font *hidingPasswordFont;
SDL_Surface *eyeImgSF;
SDL_Surface *eyeSlashImgSF;
SDL_Surface *IRiconImgSF;

int main(void)
{
    // SDL, SDL_image and SDL_ttf configuration
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        putStr("SDL could not initialize! SDL_Error: ");
        putStr(SDL_GetError());
        putChar('\n');
        return (-1);
    }

    if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG))
    {
        putStr("SDL could not initialize! SDL_Error: ");
        putStr(SDL_GetError());
        putChar('\n');
        SDL_Quit();
        return (-1);
    }

    if (TTF_Init() == -1)
    {
        putStr("TTF could not initialize! TTF_Error: ");
        putStr(TTF_GetError());
        putChar('\n');
        SDL_Quit();
        IMG_Quit();
        return (-1);
    }

    // Create a window and renderer
    window = SDL_CreateWindow("IYORE", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
    if (!window)
    {
        putStr("Window could not be created! SDL_Error: ");
        putStr(SDL_GetError());
        putChar('\n');
        SDL_DestroyWindow(window);
        SDL_Quit();
        TTF_Quit();
        return (-1);
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer)
    {
        putStr("Renderer could not be created! SDL_Error: ");
        putStr(SDL_GetError());
        putChar('\n');
        SDL_DestroyWindow(window);
        SDL_DestroyRenderer(renderer);
        SDL_Quit();
        TTF_Quit();
        IMG_Quit();
        return (-1);
    }

    // Load font
    titleFont = TTF_OpenFont("./font/Roboto-BoldItalic.ttf", 52);
    topicsFont = TTF_OpenFont("./font/Roboto-BoldItalic.ttf", 28);
    textfont = TTF_OpenFont("./font/Roboto-BoldItalic.ttf", 20);
    smallfont = TTF_OpenFont("./font/Roboto-BoldItalic.ttf", 16);
    // hidingPasswordFont = TTF_OpenFont("./font/Roboto-Regular.ttf", 24);
    hidingPasswordFont = TTF_OpenFont("./font/Roboto-ThinItalic.ttf", 24);
    if (titleFont == NULL || topicsFont == NULL || textfont == NULL || smallfont == NULL)
    {
        putStr("Failed to load font! TTF_Error: ");
        putStr(TTF_GetError());
        putChar('\n');
        SDL_DestroyWindow(window);
        SDL_DestroyRenderer(renderer);
        SDL_Quit();
        TTF_Quit();
        IMG_Quit();
        return (-1);
    }

    eyeImgSF = IMG_Load("./imgs/eye-solid.svg");
    eyeSlashImgSF = IMG_Load("./imgs/eye-slash-solid.svg");
    IRiconImgSF = IMG_Load("./imgs/IR-icon.png");
    if (eyeImgSF == NULL || eyeSlashImgSF == NULL || IRiconImgSF == NULL)
    {
        SDL_DestroyWindow(window);
        SDL_DestroyRenderer(renderer);
        putStr("Failed to load surface! IMG_Error: ");
        putStr(IMG_GetError());
        putChar('\n');
        SDL_DestroyWindow(window);
        SDL_DestroyRenderer(renderer);
        SDL_Quit();
        TTF_Quit();
        IMG_Quit();
        return (-1);
    }

    menu();

    TTF_CloseFont(textfont);
    TTF_CloseFont(topicsFont);

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    SDL_FreeSurface(eyeImgSF);
    SDL_FreeSurface(eyeSlashImgSF);

    SDL_Quit();
    TTF_Quit();
    IMG_Quit();

    return (0);
}
