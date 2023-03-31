//
//  Game.cpp
//  Game-mac
//

#include "Game.h"

Game::Game(){
    isGameOver = false;
    paddlePosition = {THICKNESS, (WINDOW_HEIGHT / 2) - (HEIGHT / 2)};
    ballPosition = {(WINDOW_WIDTH / 2) - (THICKNESS / 2), (WINDOW_HEIGHT / 2) - (THICKNESS / 2) };
    velocity = {xVelo, yVelo};
    dir = None;
}

bool Game::Initialize(){
    if (SDL_Init(SDL_INIT_VIDEO|SDL_INIT_AUDIO) != 0) {
        SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
        return false;
    }
    window = SDL_CreateWindow(
        "Lab1",                            // Title
        SDL_WINDOWPOS_UNDEFINED,           // initial x position
        SDL_WINDOWPOS_UNDEFINED,           // initial y position
        WINDOW_WIDTH,                      // width
        WINDOW_HEIGHT,                     // height
        0
    );
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED |  SDL_RENDERER_PRESENTVSYNC);
    previousTime = (float)SDL_GetTicks();
    return true;
}

void Game::ProcessInput(){
    //See if x is clicked
    SDL_Event event;
    while(SDL_PollEvent(&event)){
        if (event.type == SDL_QUIT) {
            isGameOver = true;
        }
    }
    
    //Get Keyboard Inputs
    const Uint8 *state = SDL_GetKeyboardState(NULL);
    if (state[SDL_SCANCODE_ESCAPE]) {
        isGameOver = true;
    }
    if (state[SDL_SCANCODE_UP]) {
        dir = Up;
    }
    if (state[SDL_SCANCODE_DOWN]) {
        dir = Down;
    }
}

void Game::UpdateGame(){
    //Frame limiting, delta time, max delta time
    float currentTime = (float)SDL_GetTicks();
    while(currentTime - previousTime < 16){
        currentTime = (float)SDL_GetTicks();
    }
    float delta = (currentTime - previousTime) / 1000;
    if(delta > 0.033){
        delta = (float)0.033;
    }
    previousTime = currentTime;
    
    //Paddle Movement
    int movementNumber = 300;
    if(dir == Up){
        paddlePosition.y -= static_cast<int>(delta * movementNumber);
    }
    if(dir == Down){
        paddlePosition.y += static_cast<int>(delta * movementNumber);
    }
    
    //Paddle Restriction
    if(paddlePosition.y < THICKNESS){
        paddlePosition.y = THICKNESS;
    }
    if(paddlePosition.y > WINDOW_HEIGHT - HEIGHT - THICKNESS){
        paddlePosition.y = WINDOW_HEIGHT - HEIGHT - THICKNESS;
    }
    
    //Ball Movement
    ballPosition.x += static_cast<int>(delta * velocity.x);
    ballPosition.y += static_cast<int>(delta * velocity.y);
    
    //Ball Bounce off Wall
    if(ballPosition.y < THICKNESS){
        velocity.y = -velocity.y;
        ballPosition.y = THICKNESS;
    }
    if(ballPosition.y > WINDOW_HEIGHT - THICKNESS - THICKNESS){
        velocity.y = -velocity.y;
        ballPosition.y = WINDOW_HEIGHT - THICKNESS - THICKNESS;
    }
    if(ballPosition.x > WINDOW_WIDTH - THICKNESS - THICKNESS){
        velocity.x = -velocity.x;
        ballPosition.x = WINDOW_WIDTH - THICKNESS - THICKNESS;
    }
    
    //Ball Bounce off Paddle
    if(ballPosition.x > paddlePosition.x && ballPosition.x < paddlePosition.x + THICKNESS && ballPosition.y + THICKNESS > paddlePosition.y && ballPosition.y < paddlePosition.y + HEIGHT){
        velocity.x = -velocity.x;
        ballPosition.x = paddlePosition.x + THICKNESS;
    }
    
    //Losing Condition -> Quit
    if(ballPosition.x + THICKNESS < 0){
        isGameOver = true;
    }
    dir = None;
}

void Game::GenerateOutput(){
    //Background Color
    SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
    SDL_RenderClear(renderer);
    
    //Walls
    SDL_Rect topWall;
    SDL_Rect rightWall;
    SDL_Rect bottomWall;
    topWall.x = 0;
    topWall.y = 0;
    topWall.w = WINDOW_WIDTH;
    topWall.h = THICKNESS;
    rightWall.x = WINDOW_WIDTH - THICKNESS;
    rightWall.y = 0;
    rightWall.w = THICKNESS;
    rightWall.h = WINDOW_HEIGHT;
    bottomWall.x = 0;
    bottomWall.y = WINDOW_HEIGHT - THICKNESS;
    bottomWall.w = WINDOW_WIDTH;
    bottomWall.h = THICKNESS;
    
    //Paddle
    SDL_Rect paddle;
    paddle.x = paddlePosition.x;
    paddle.y = paddlePosition.y;
    paddle.w = THICKNESS;
    paddle.h = HEIGHT;
    
    //Ball
    SDL_Rect ball;
    ball.x = ballPosition.x;
    ball.y = ballPosition.y;
    ball.w = THICKNESS;
    ball.h = THICKNESS;
    
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderFillRect(renderer, &topWall);
    SDL_RenderFillRect(renderer, &rightWall);
    SDL_RenderFillRect(renderer, &bottomWall);
    SDL_RenderFillRect(renderer, &paddle);
    SDL_RenderFillRect(renderer, &ball);
    SDL_RenderPresent(renderer);
}

void Game::RunLoop(){
    while(!isGameOver){
        ProcessInput();
        UpdateGame();
        GenerateOutput();
    }
}

void Game::Shutdown(){
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}
