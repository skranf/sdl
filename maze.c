#include <stdio.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>


#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480
#define MAZE_WIDTH 20
#define MAZE_HEIGHT 15
#define CELL_SIZE (SCREEN_WIDTH / MAZE_WIDTH)
#define BALL_SIZE 20


int maze[MAZE_HEIGHT][MAZE_WIDTH] =
{
    {0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
    {0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 1},
    {1, 1, 0, 1, 0, 1, 0, 1, 1, 1, 1, 0, 1, 0, 1, 0, 1, 0, 1, 1},
    {1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 1, 0, 0, 1},
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 0, 1, 0, 1, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 1},
    {1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 0, 1, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 1},
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 0, 1, 0, 1, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 1, 1},
    {1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 1, 0, 1},
    {1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0},
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 2},
};


typedef struct
{
    int x;
    int y;
} Ball;


bool initializeSDL(SDL_Window **window, SDL_Renderer **renderer, char *game)
{
    SDL_Init(SDL_INIT_VIDEO);
    *window = SDL_CreateWindow(game, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    *renderer = SDL_CreateRenderer(*window, -1, SDL_RENDERER_ACCELERATED);
    return true;
}


void handleEvents()
{
    SDL_Event e;
    SDL_PollEvent(&e);
}


void updateBallPosition(Ball *ball)
{
    int mouseX, mouseY;
    SDL_GetMouseState(&mouseX, &mouseY);
    if (mouseX >= 0 && mouseX <= SCREEN_WIDTH - BALL_SIZE && mouseY >= 0 && mouseY <= SCREEN_HEIGHT - BALL_SIZE)
    {
        ball->x = mouseX;
        ball->y = mouseY;
    }
}


bool checkCollision(Ball *ball)
{
    int cellX = ball->x / CELL_SIZE;
    int cellY = ball->y / CELL_SIZE;
    return (maze[cellY][cellX] == 1);
}

bool checkEnd(Ball *ball)
{
    int cellX = ball->x / CELL_SIZE;
    int cellY = ball->y / CELL_SIZE;
    return (maze[cellY][cellX] == 2);
}

void drawMaze(SDL_Renderer *renderer)
{
    for (int i = 0; i < MAZE_HEIGHT; i++)
    {
        for (int j = 0; j < MAZE_WIDTH; j++)
        {
            if (maze[i][j] == 1)
            {
                SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
                SDL_Rect wallRect = {j * CELL_SIZE, i * CELL_SIZE, CELL_SIZE, CELL_SIZE};
                SDL_RenderFillRect(renderer, &wallRect);
            }
            else
            {
                SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
                SDL_Rect pathRect = {j * CELL_SIZE, i * CELL_SIZE, CELL_SIZE, CELL_SIZE};
                SDL_RenderFillRect(renderer, &pathRect);
            }
        }
    }
}


void drawBall(Ball *ball, SDL_Renderer *renderer)
{
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    SDL_Rect ballRect = {ball->x, ball->y, BALL_SIZE, BALL_SIZE};
    SDL_RenderFillRect(renderer, &ballRect);
}


void showMessage(SDL_Renderer *renderer, char *message)
{
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer);
    SDL_Color textColor = {0, 0, 0};
    TTF_Font *font = TTF_OpenFont("./font.ttf", 28);
    SDL_Surface *textSurface = TTF_RenderText_Solid(font, message, textColor);
    SDL_Texture *textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
    int textX = (SCREEN_WIDTH - textSurface->w) / 2;
    int textY = (SCREEN_HEIGHT - textSurface->h) / 2;
    SDL_RenderCopy(renderer, textTexture, NULL, &(SDL_Rect){textX, textY, textSurface->w, textSurface->h});
    SDL_RenderPresent(renderer);
    SDL_Delay(1000);
    SDL_FreeSurface(textSurface);
    SDL_DestroyTexture(textTexture);
    TTF_CloseFont(font);
}

int main()
{
    SDL_Window *window = NULL;
    SDL_Renderer *renderer = NULL;
    Ball ball = {0, 0};

    initializeSDL(&window, &renderer, "Sors Du Labyrinthe ");
    TTF_Init();

    bool isRunning = true;
    while (isRunning)
    {
        handleEvents();
        updateBallPosition(&ball);
        drawMaze(renderer);
        drawBall(&ball, renderer);
        if (checkCollision(&ball))
        {
            showMessage(renderer, "PERDU ...");
            isRunning = false;
        }
        if (checkEnd(&ball))
        {
            showMessage(renderer, "GAGNE !");
            isRunning = false;
        }
        SDL_RenderPresent(renderer);
        SDL_Delay(10);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    TTF_Quit();

    return 0;
}
