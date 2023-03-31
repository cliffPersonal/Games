#pragma once

#include "Math.h"
#include "SDL2/SDL.h"
#include "SDL2/SDL_mixer.h"
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
    void AddBlock(class Block *b);
    void RemoveBlock(class Block *b);
    void AddGoomba(class Goomba *g);
    void RemoveGoomba(class Goomba *g);
    void SetCameraX(float value);
    int GetSoundValue(){return soundValue;};
    class CollisionComponent* GetGoal();
    class Player* GetPlayer(){return player;};
    std::vector<class Block*>& GetBlocks(){return blocks;};
    std::vector<class Goomba*>& GetGoombas(){return goombas;};
    Vector2 GetCameraPosition(){return cameraPosition;};
    SDL_Texture* GetTexture(const char *filename);
    Mix_Chunk* GetSound(const std::string& filename);
private:
    SDL_Window *window;
    SDL_Renderer *renderer;
    bool isGameOver;
    int soundValue;
    const int WINDOW_WIDTH = 600;
    const int WINDOW_HEIGHT = 448;
    float previousTime;
    std::vector<class Actor*> actors;
    std::unordered_map<std::string, SDL_Texture*> hashmap;
    std::unordered_map<std::string, Mix_Chunk*> soundHashmap;
    std::vector<class SpriteComponent*> sprites;
    std::vector<class Block*> blocks;
    std::vector<class Goomba*> goombas;
    class CollisionComponent* goal;
    class Player* player;
    Vector2 cameraPosition;
    
    void LoadData();
    void UnloadData();
    void ProcessInput();
    void UpdateGame();
    void GenerateOutput();
};
