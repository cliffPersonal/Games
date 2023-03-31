#pragma once

#include "SDL2/SDL.h"
#include <unordered_map>
#include <string>
#include <vector>

class Game{
public:
    Game();
    bool Initialize();
    void Shutdown();
    void RunLoop();
    void AddActor(class Actor *actor);
    void RemoveActor(class Actor *actor);
    void AddSprite(class SpriteComponent *sc);
    void RemoveSprite(class SpriteComponent *sc);
    void AddVehicle(class Vehicle *veh);
    void RemoveVehicle(class Vehicle *veh);
    void AddLog(class Log *log);
    void RemoveLog(class Log *log);
    class CollisionComponent* GetGoal();
    class Frog* GetFrog();
    std::vector<class Vehicle*>& GetVehicles();
    std::vector<class Log*>& GetLogs();
    SDL_Texture* GetTexture(const char *filename);
private:
    SDL_Window *window;
    SDL_Renderer *renderer;
    bool isGameOver;
    const int WINDOW_WIDTH = 896;
    const int WINDOW_HEIGHT = 1024;
    float previousTime;
    std::vector<class Actor*> actors;
    std::unordered_map<std::string, SDL_Texture*> hashmap;
    std::vector<class SpriteComponent*> sprites;
    std::vector<class Vehicle*> cars;
    std::vector<class Log*> logs;
    class Frog* frog;
    class CollisionComponent* goal;
    
    void LoadData();
    void UnloadData();
    void ProcessInput();
    void UpdateGame();
    void GenerateOutput();
};
