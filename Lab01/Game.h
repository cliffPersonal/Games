#pragma once

#include "SDL2/SDL.h"

class Game{
public:
    Game();
    bool Initialize();
    void Shutdown();
    void RunLoop();
private:
    SDL_Window *window;
    SDL_Renderer *renderer;
    bool isGameOver;
    const int WINDOW_WIDTH = 1024;
    const int WINDOW_HEIGHT = 768;
    const int THICKNESS = 20;
    const int HEIGHT = 150;
    const int xVelo = 300;
    const int yVelo = 300;
    float previousTime;
    enum direction {Up, None, Down};
    direction dir;
    SDL_Point paddlePosition;
    SDL_Point ballPosition;
    SDL_Point velocity;
    
    void ProcessInput();
    void UpdateGame();
    void GenerateOutput();
};
