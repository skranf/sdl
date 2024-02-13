#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#define SCREEN_WIDTH 900
#define SCREEN_HEIGHT 600
#define BACKGROUND_WIDTH 1600
#define BACKGROUND_HEIGHT 600
#define PLAYER_WIDTH 64
#define PLAYER_HEIGHT 64
#define PLAYER_SPEED 5
#define NUM_FRAMES 6
#define BACKGROUND_SCROLL_SPEED 5

SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;
SDL_Texture *backgroundTexture = NULL;
SDL_Texture *playerTexture = NULL;

int currentFrame = 0;
int playerX = 100;
int playerY = 480;

void initSDL()
{
    SDL_Init(SDL_INIT_VIDEO);
    IMG_Init(IMG_INIT_PNG);
    window = SDL_CreateWindow("Runner Game", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
}

void loadMedia()
{
    SDL_Surface *backgroundSurface = IMG_Load("background.png");
    backgroundTexture = SDL_CreateTextureFromSurface(renderer, backgroundSurface);
    SDL_FreeSurface(backgroundSurface);

    SDL_Surface *playerSurface = IMG_Load("player.png");
    playerTexture = SDL_CreateTextureFromSurface(renderer, playerSurface);
    SDL_FreeSurface(playerSurface);
}

void closeSDL()
{
    SDL_DestroyTexture(backgroundTexture);
    SDL_DestroyTexture(playerTexture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void render()
{
    SDL_RenderClear(renderer);

    // Scrolling background
    static int backgroundOffset = 0;
    backgroundOffset -= BACKGROUND_SCROLL_SPEED;
    if (backgroundOffset > BACKGROUND_WIDTH)
    {
        backgroundOffset = 0;
    }

    // Render background
    SDL_Rect backgroundRect1 = {backgroundOffset, 0, BACKGROUND_WIDTH, BACKGROUND_HEIGHT};
    SDL_Rect backgroundRect2 = {backgroundOffset - BACKGROUND_WIDTH, 0, BACKGROUND_WIDTH, BACKGROUND_HEIGHT};
    SDL_RenderCopy(renderer, backgroundTexture, NULL, &backgroundRect1);
    SDL_RenderCopy(renderer, backgroundTexture, NULL, &backgroundRect2);

    // Render player
    SDL_Rect playerRect = {playerX, playerY, PLAYER_WIDTH, PLAYER_HEIGHT};
    SDL_Rect playerFrame = {currentFrame * PLAYER_WIDTH, 0, PLAYER_WIDTH, PLAYER_HEIGHT};
    SDL_RenderCopy(renderer, playerTexture, &playerFrame, &playerRect);

    SDL_RenderPresent(renderer);
}

void handleInput()
{
    SDL_Event e;
    while (SDL_PollEvent(&e) != 0)
    {
        if (e.type == SDL_QUIT)
        {
            closeSDL();
            exit(0);
        }
    }
}

int main()
{
    initSDL();
    loadMedia();

    int quit = 0;
    while (!quit)
    {
        handleInput();
        currentFrame = (currentFrame + 1) % NUM_FRAMES;
        render();
        SDL_Delay(100);
    }

    closeSDL();

    return 0;
}
