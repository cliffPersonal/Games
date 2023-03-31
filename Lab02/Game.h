#pragma once

#include "Actor.h"
#include "SDL2/SDL.h"
#include <unordered_map>
#include <string>

class Game{
public:
    Game();
    bool Initialize();
    void Shutdown();
    void RunLoop();
    void AddActor(Actor *actor);
    void RemoveActor(Actor *actor);
    void AddSprite(class SpriteComponent *sc);
    void RemoveSprite(class SpriteComponent *sc);
    void AddAsteroid(class Asteroid *ast);
    void RemoveAsteroid(class Asteroid *ast);
    std::vector<class Asteroid*> GetAsteroids(){return asteroids;};
    SDL_Texture* GetTexture(const char *filename);
private:
    SDL_Window *window;
    SDL_Renderer *renderer;
    bool isGameOver;
    const int WINDOW_WIDTH = 1024;
    const int WINDOW_HEIGHT = 768;
    float previousTime;
    std::vector<Actor*> actors;
    std::unordered_map<std::string, SDL_Texture*> hashmap;
    std::vector<class SpriteComponent*> sprites;
    std::vector<class Asteroid*> asteroids;
    
    void LoadData();
    void UnloadData();
    void ProcessInput();
    void UpdateGame();
    void GenerateOutput();
};
